/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpdialogfactory.c
 * Copyright (C) 2001 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "core/gimpcontext.h"

#include "gimpcursor.h"
#include "gimpdialogfactory.h"
#include "gimpdock.h"
#include "gimpdockbook.h"
#include "gimpdockable.h"
#include "gimpmenufactory.h"
#include "gimpsessioninfo.h"


/* #define DEBUG_FACTORY */

#ifdef DEBUG_FACTORY
#define D(stmnt) stmnt
#else
#define D(stmnt)
#endif


typedef enum
{
  GIMP_DIALOG_VISIBILITY_UNKNOWN = 0,
  GIMP_DIALOG_VISIBILITY_INVISIBLE,
  GIMP_DIALOG_VISIBILITY_VISIBLE
} GimpDialogVisibilityState;

typedef enum
{
  GIMP_DIALOG_SHOW_ALL,
  GIMP_DIALOG_HIDE_ALL,
  GIMP_DIALOG_SHOW_TOOLBOX
} GimpDialogShowState;


static void   gimp_dialog_factory_class_init (GimpDialogFactoryClass *klass);
static void   gimp_dialog_factory_init       (GimpDialogFactory      *factory);

static void   gimp_dialog_factory_dispose             (GObject           *object);
static void   gimp_dialog_factory_finalize            (GObject           *object);

static void     gimp_dialog_factory_set_widget_data   (GtkWidget         *dialog,
                                                       GimpDialogFactory *factory,
                                                       GimpDialogFactoryEntry *entry);
static gboolean gimp_dialog_factory_set_user_pos      (GtkWidget         *dialog,
                                                       GdkEventConfigure *cevent,
                                                       gpointer           data);
static gboolean gimp_dialog_factory_dialog_configure  (GtkWidget         *dialog,
                                                       GdkEventConfigure *cevent,
                                                       GimpDialogFactory *factory);
static void   gimp_dialog_factories_save_foreach      (gconstpointer      key,
						       GimpDialogFactory *factory,
						       GimpConfigWriter  *writer);
static void   gimp_dialog_factories_restore_foreach   (gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);
static void   gimp_dialog_factories_clear_foreach     (gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);
static void   gimp_dialog_factories_hide_foreach      (gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);
static void   gimp_dialog_factories_show_foreach      (gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);
static void   gimp_dialog_factories_set_busy_foreach  (gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);
static void   gimp_dialog_factories_unset_busy_foreach(gconstpointer      key,
						       GimpDialogFactory *factory,
						       gpointer           data);


static GimpObjectClass *parent_class = NULL;


GType
gimp_dialog_factory_get_type (void)
{
  static GType factory_type = 0;

  if (! factory_type)
    {
      static const GTypeInfo factory_info =
      {
        sizeof (GimpDialogFactoryClass),
        NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gimp_dialog_factory_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GimpDialogFactory),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gimp_dialog_factory_init,
      };

      factory_type = g_type_register_static (GIMP_TYPE_OBJECT,
					     "GimpDialogFactory",
					     &factory_info, 0);
    }

  return factory_type;
}

static void
gimp_dialog_factory_class_init (GimpDialogFactoryClass *klass)
{
  GObjectClass *object_class;

  object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->dispose  = gimp_dialog_factory_dispose;
  object_class->finalize = gimp_dialog_factory_finalize;

  klass->factories = g_hash_table_new (g_str_hash, g_str_equal);
}

static void
gimp_dialog_factory_init (GimpDialogFactory *factory)
{
  factory->menu_factory       = NULL;
  factory->new_dock_func      = NULL;
  factory->registered_dialogs = NULL;
  factory->session_infos      = NULL;
  factory->open_dialogs       = NULL;
}

static void
gimp_dialog_factory_dispose (GObject *object)
{
  GimpDialogFactory *factory;
  GList             *list;

  factory = GIMP_DIALOG_FACTORY (object);

  /*  start iterating from the beginning each time we destroyed a
   *  toplevel because destroying a dock may cause lots of items
   *  to be removed from factory->open_dialogs
   */
  while (factory->open_dialogs)
    {
      for (list = factory->open_dialogs; list; list = g_list_next (list))
        {
          if (GTK_WIDGET_TOPLEVEL (list->data))
            {
              gtk_widget_destroy (GTK_WIDGET (list->data));
              break;
            }
        }

      /*  the list being non-empty without any toplevel is an error,
       *  so eek and chain up
       */
      if (! list)
        {
          g_warning ("%s: stale non-toplevel entries in factory->open_dialogs",
                     G_GNUC_FUNCTION);
          break;
        }
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_dialog_factory_finalize (GObject *object)
{
  GimpDialogFactory *factory;
  GList             *list;

  factory = GIMP_DIALOG_FACTORY (object);

  g_hash_table_remove (GIMP_DIALOG_FACTORY_GET_CLASS (object)->factories,
		       GIMP_OBJECT (factory)->name);

  for (list = factory->registered_dialogs; list; list = g_list_next (list))
    {
      GimpDialogFactoryEntry *entry;

      entry = (GimpDialogFactoryEntry *) list->data;

      g_free (entry->identifier);
      g_free (entry);
    }

  if (factory->registered_dialogs)
    {
      g_list_free (factory->registered_dialogs);
      factory->registered_dialogs = NULL;
    }
  if (factory->open_dialogs)
    {
      g_list_free (factory->open_dialogs);
      factory->open_dialogs = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

GimpDialogFactory *
gimp_dialog_factory_new (const gchar       *name,
			 GimpContext       *context,
			 GimpMenuFactory   *menu_factory,
			 GimpDialogNewFunc  new_dock_func)
{
  GimpDialogFactory *factory;
  gpointer           key;

  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);
  g_return_val_if_fail (! menu_factory || GIMP_IS_MENU_FACTORY (menu_factory),
			NULL);

  if (gimp_dialog_factory_from_name (name))
    {
      g_warning ("%s: dialog factory \"%s\" already exists",
		 G_GNUC_FUNCTION, name);
      return NULL;
    }

  factory = g_object_new (GIMP_TYPE_DIALOG_FACTORY, NULL);

  gimp_object_set_name (GIMP_OBJECT (factory), name);

  /*  hack to keep the toolbox on the pool position  */
  if (strcmp (name, "toolbox") == 0)
    key = "";
  else
    key = GIMP_OBJECT (factory)->name;

  g_hash_table_insert (GIMP_DIALOG_FACTORY_GET_CLASS (factory)->factories,
		       key, factory);

  factory->context       = context;
  factory->menu_factory  = menu_factory;
  factory->new_dock_func = new_dock_func;

  return factory;
}

GimpDialogFactory *
gimp_dialog_factory_from_name (const gchar *name)
{
  GimpDialogFactoryClass *factory_class;
  GimpDialogFactory      *factory;

  g_return_val_if_fail (name != NULL, NULL);

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);
  if (! factory_class)
    return NULL;

  /*  hack to keep the toolbox on the pool position  */
  if (strcmp (name, "toolbox") == 0)
    name = "";

  factory =
    (GimpDialogFactory *) g_hash_table_lookup (factory_class->factories, name);

  return factory;
}

void
gimp_dialog_factory_register_entry (GimpDialogFactory *factory,
				    const gchar       *identifier,
				    GimpDialogNewFunc  new_func,
                                    gint               preview_size,
				    gboolean           singleton,
				    gboolean           session_managed,
				    gboolean           remember_size,
				    gboolean           remember_if_open)
{
  GimpDialogFactoryEntry *entry;

  g_return_if_fail (GIMP_IS_DIALOG_FACTORY (factory));
  g_return_if_fail (identifier != NULL);

  entry = g_new0 (GimpDialogFactoryEntry, 1);

  entry->identifier       = g_strdup (identifier);
  entry->new_func         = new_func;
  entry->preview_size     = preview_size;
  entry->singleton        = singleton ? TRUE : FALSE;
  entry->session_managed  = session_managed ? TRUE : FALSE;
  entry->remember_size    = remember_size ? TRUE : FALSE;
  entry->remember_if_open = remember_if_open ? TRUE : FALSE;

  factory->registered_dialogs = g_list_prepend (factory->registered_dialogs,
						entry);
}

GimpDialogFactoryEntry *
gimp_dialog_factory_find_entry (GimpDialogFactory *factory,
				const gchar       *identifier)
{
  GList *list;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (identifier != NULL, NULL);

  for (list = factory->registered_dialogs; list; list = g_list_next (list))
    {
      GimpDialogFactoryEntry *entry;

      entry = (GimpDialogFactoryEntry *) list->data;

      if (! strcmp (identifier, entry->identifier))
        return entry;
    }

  return NULL;
}

GimpSessionInfo *
gimp_dialog_factory_find_session_info (GimpDialogFactory *factory,
				       const gchar       *identifier)
{
  GList *list;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (identifier != NULL, NULL);

  for (list = factory->session_infos; list; list = g_list_next (list))
    {
      GimpSessionInfo *info;

      info = (GimpSessionInfo *) list->data;

      if ((info->toplevel_entry &&
	   ! strcmp (identifier, info->toplevel_entry->identifier)) ||
	  (info->dockable_entry &&
	   ! strcmp (identifier, info->dockable_entry->identifier)))
	{
	  return info;
	}
    }

  return NULL;
}

static GtkWidget *
gimp_dialog_factory_dialog_new_internal (GimpDialogFactory *factory,
                                         GdkScreen         *screen,
					 GimpContext       *context,
					 const gchar       *identifier,
                                         gint               preview_size,
					 gboolean           raise_if_found)
{
  GimpDialogFactoryEntry *entry;
  GtkWidget              *dialog = NULL;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (identifier != NULL, NULL);

  entry = gimp_dialog_factory_find_entry (factory, identifier);

  if (! entry)
    {
      g_warning ("%s: no entry registered for \"%s\"",
		 G_GNUC_FUNCTION, identifier);
      return NULL;
    }

  if (! entry->new_func)
    {
      g_warning ("%s: entry for \"%s\" has no constructor",
		 G_GNUC_FUNCTION, identifier);
      return NULL;
    }

  /*  a singleton dialog is always only raised if it already exisits  */
  if (raise_if_found || entry->singleton)
    {
      GimpSessionInfo *info;

      info = gimp_dialog_factory_find_session_info (factory, identifier);

      if (info)
	dialog = info->widget;
    }

  /*  create the dialog if it was not found  */
  if (! dialog)
    {
      GtkWidget *dock = NULL;

      /*  If the dialog will be a dockable (factory->new_dock_func) and
       *  we are called from gimp_dialog_factory_dialog_raise() (! context),
       *  create a new dock _before_ creating the dialog.
       *  We do this because the new dockable needs to be created in it's
       *  dock's context.
       */
      if (factory->new_dock_func && ! context)
	{
          GtkWidget *dockbook;

	  dock     = gimp_dialog_factory_dock_new (factory, screen);
          dockbook = gimp_dockbook_new (factory->menu_factory);

	  gimp_dock_add_book (GIMP_DOCK (dock),
			      GIMP_DOCKBOOK (dockbook),
                              0);
	}

      /*  Create the new dialog in the appropriate context which is
       *  - the passed context if not NULL
       *  - the newly created dock's context if we just created it
       *  - the factory's context, which happens when raising a toplevel
       *    dialog was the original request.
       */
      if (preview_size < GIMP_PREVIEW_SIZE_TINY)
        preview_size = entry->preview_size;

      if (context)
	dialog = entry->new_func (factory,
                                  context,
                                  preview_size);
      else if (dock)
	dialog = entry->new_func (factory,
                                  GIMP_DOCK (dock)->context,
                                  preview_size);
      else
	dialog = entry->new_func (factory,
                                  factory->context,
                                  preview_size);

      if (dialog)
	{
          gimp_dialog_factory_set_widget_data (dialog, factory, entry);

	  /*  If we created a dock before, the newly created dialog is
	   *  supposed to be a GimpDockable.
	   */
	  if (dock)
	    {
	      if (GIMP_IS_DOCKABLE (dialog))
		{
		  gimp_dock_add (GIMP_DOCK (dock), GIMP_DOCKABLE (dialog),
				 0, 0);

		  gtk_widget_show (dock);
		}
	      else
		{
		  g_warning ("%s: GimpDialogFactory is a dockable factory "
			     "but constructor for \"%s\" did not return a "
			     "GimpDockable",
			     G_GNUC_FUNCTION, identifier);

		  gtk_widget_destroy (dialog);
                  gtk_widget_destroy (dock);

		  dialog = NULL;
                  dock   = NULL;
		}
	    }
	}
      else if (dock)
	{
	  g_warning ("%s: constructor for \"%s\" returned NULL",
		     G_GNUC_FUNCTION, identifier);

	  gtk_widget_destroy (dock);

	  dock = NULL;
	}

      if (dialog)
	gimp_dialog_factory_add_dialog (factory, dialog);
    }

  /*  finally, if we found an existing dialog or created a new one,
   *  raise it
   */
  if (dialog)
    {
      if (GTK_WIDGET_TOPLEVEL (dialog))
	{
          gtk_window_set_screen (GTK_WINDOW (dialog), screen);
          gtk_window_present (GTK_WINDOW (dialog));
	}
      else if (GIMP_IS_DOCKABLE (dialog))
	{
	  GimpDockable *dockable;
	  gint          page_num;

	  dockable = GIMP_DOCKABLE (dialog);

	  if (dockable->dockbook && dockable->dockbook->dock)
	    {
	      page_num =
		gtk_notebook_page_num (GTK_NOTEBOOK (dockable->dockbook),
				       dialog);

	      if (page_num != -1)
		{
		  GtkWidget *toplevel;

		  gtk_notebook_set_current_page (GTK_NOTEBOOK (dockable->dockbook),
                                                 page_num);

		  toplevel = gtk_widget_get_toplevel (dialog);

                  gtk_window_present (GTK_WINDOW (toplevel));
		}
	    }
	}
    }

  return dialog;
}

/**
 * gimp_dialog_factory_dialog_new:
 * @factory:      a #GimpDialogFactory
 * @screen:       the #GdkScreen the dialog should appear on
 * @identifier:   the identifier of the dialog as registered with
 *                gimp_dialog_factory_register_entry()
 * @preview_size:
 *
 * Creates a new toplevel dialog or a #GimpDockable, depending on whether
 * %factory is a toplevel of dockable factory.
 *
 * Implicitly raises singleton dialogs.
 *
 * Return value: the newly created dialog or an already existing singleton
 *               dialog.
 **/
GtkWidget *
gimp_dialog_factory_dialog_new (GimpDialogFactory *factory,
                                GdkScreen         *screen,
				const gchar       *identifier,
                                gint               preview_size)
{
  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);
  g_return_val_if_fail (identifier != NULL, NULL);

  return gimp_dialog_factory_dialog_new_internal (factory,
                                                  screen,
						  factory->context,
						  identifier,
                                                  preview_size,
						  FALSE);
}

/**
 * gimp_dialog_factory_dialog_raise:
 * @factory:      a #GimpDialogFactory
 * @screen:       the #GdkScreen the dialog should appear on
 * @identifiers:  a '|' separated list of identifiers of dialogs as
 *                registered with gimp_dialog_factory_register_entry()
 * @preview_size:
 *
 * Raises any of a list of already existing toplevel dialog or
 * #GimpDockable if it was already created by this %facory.
 *
 * Implicitly creates the first dialog in the list if none of the dialogs
 * were found.
 *
 * Return value: the raised or newly created dialog.
 **/
GtkWidget *
gimp_dialog_factory_dialog_raise (GimpDialogFactory *factory,
                                  GdkScreen         *screen,
				  const gchar       *identifiers,
                                  gint               preview_size)
{
  GtkWidget *dialog;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);
  g_return_val_if_fail (identifiers != NULL, NULL);

  /*  If the identifier is a list, try to find a matching dialog and
   *  raise it. If there's no match, use the first list item.
   */
  if (strchr (identifiers, '|'))
    {
      gchar **ids = g_strsplit (identifiers, "|", 0);
      gint    i;

      for (i = 0; ids[i]; i++)
        {
          GimpSessionInfo *info;

          info = gimp_dialog_factory_find_session_info (factory, ids[i]);
          if (info && info->widget)
            break;
        }

      dialog = gimp_dialog_factory_dialog_new_internal (factory,
                                                        screen,
                                                        NULL,
                                                        ids[i] ? ids[i] : ids[0],
                                                        preview_size,
                                                        TRUE);
      g_strfreev (ids);
    }
  else
    {
      dialog = gimp_dialog_factory_dialog_new_internal (factory,
                                                        screen,
                                                        NULL,
                                                        identifiers,
                                                        preview_size,
                                                        TRUE);
    }

  return dialog;
}

/**
 * gimp_dialog_factory_dockable_new:
 * @factory:      a #GimpDialogFactory
 * @dock:         a #GimpDock crated by this %factory.
 * @identifier:   the identifier of the dialog as registered with
 *                gimp_dialog_factory_register_entry()
 * @preview_size:
 *
 * Creates a new #GimpDockable in the context of the #GimpDock it will be
 * added to.
 *
 * Implicitly raises & returns an already existing singleton dockable,
 * so callers should check that dockable->dockbook is NULL before trying
 * to add it to it's #GimpDockbook.
 *
 * Return value: the newly created #GimpDockable or an already existing
 *               singleton dockable.
 **/
GtkWidget *
gimp_dialog_factory_dockable_new (GimpDialogFactory *factory,
				  GimpDock          *dock,
				  const gchar       *identifier,
                                  gint               preview_size)
{
  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (GIMP_IS_DOCK (dock), NULL);
  g_return_val_if_fail (identifier != NULL, NULL);

  return gimp_dialog_factory_dialog_new_internal (factory,
                                                  gtk_widget_get_screen (GTK_WIDGET (dock)),
						  dock->context,
						  identifier,
                                                  preview_size,
						  FALSE);
}

/**
 * gimp_dialog_factory_dock_new:
 * @factory: a #GimpDialogFacotry
 * @screen:  the #GdkScreen the dock should appear on
 *
 * Returns a new #GimpDock in this %factory's context. We use a function
 * pointer passed to this %factory's constructor instead of simply
 * gimp_dock_new() because we may want different instances of
 * #GimpDialogFactory create different subclasses of #GimpDock.
 *
 * Return value: the newly created #GimpDock.
 **/
GtkWidget *
gimp_dialog_factory_dock_new (GimpDialogFactory *factory,
                              GdkScreen         *screen)
{
  GtkWidget *dock;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);
  g_return_val_if_fail (factory->new_dock_func != NULL, NULL);

  dock = factory->new_dock_func (factory, factory->context, 0);

  if (dock)
    {
      gtk_window_set_screen (GTK_WINDOW (dock), screen);

      gimp_dialog_factory_set_widget_data (dock, factory, NULL);

      gimp_dialog_factory_add_dialog (factory, dock);
    }

  return dock;
}

void
gimp_dialog_factory_add_dialog (GimpDialogFactory *factory,
				GtkWidget         *dialog)
{
  GimpDialogFactory      *dialog_factory;
  GimpDialogFactoryEntry *entry;
  GimpSessionInfo        *info;
  GList                  *list;

  g_return_if_fail (GIMP_IS_DIALOG_FACTORY (factory));
  g_return_if_fail (GTK_IS_WIDGET (dialog));

  if (g_list_find (factory->open_dialogs, dialog))
    {
      g_warning ("%s: dialog already registered", G_GNUC_FUNCTION);
      return;
    }

  dialog_factory = gimp_dialog_factory_from_widget (dialog, &entry);

  if (! (dialog_factory && (entry || GIMP_IS_DOCK (dialog))))
    {
      g_warning ("%s: dialog was not created by a GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return;
    }

  if (dialog_factory != factory)
    {
      g_warning ("%s: dialog was created by a different GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return;
    }

  if (entry) /* dialog is a toplevel (but not a GimpDock) or a GimpDockable */
    {
      gboolean toplevel;

      toplevel = GTK_WIDGET_TOPLEVEL (dialog);

      D (g_print ("%s: adding %s \"%s\"\n",
                  G_GNUC_FUNCTION,
                  toplevel ? "toplevel" : "dockable",
                  entry->identifier));

      for (list = factory->session_infos; list; list = g_list_next (list))
	{
	  info = (GimpSessionInfo *) list->data;

	  if ((info->toplevel_entry == entry) ||
	      (info->dockable_entry == entry))
	    {
	      if (info->widget)
		{
		  if (entry->singleton)
		    {
		      g_warning ("%s: singleton dialog \"%s\" created twice",
				 G_GNUC_FUNCTION, entry->identifier);

                      D (g_print ("%s: corrupt session info: %p (widget %p)\n",
                                  G_GNUC_FUNCTION,
                                  info, info->widget));

		      return;
		    }

		  continue;
		}

              info->widget = dialog;

              D (g_print ("%s: updating session info %p (widget %p) for %s \"%s\"\n",
                          G_GNUC_FUNCTION,
                          info, info->widget,
                          toplevel ? "toplevel" : "dockable",
                          entry->identifier));

	      if (entry->session_managed)
                gimp_session_info_set_geometry (info);

	      break;
	    }
	}

      if (! list) /*  didn't find a session info  */
	{
	  info = g_new0 (GimpSessionInfo, 1);

	  info->widget = dialog;

          D (g_print  ("%s: creating session info %p (widget %p) for %s \"%s\"\n",
                       G_GNUC_FUNCTION,
                       info, info->widget,
                       toplevel ? "toplevel" : "dockable",
                       entry->identifier));

	  if (toplevel)
            {
              info->toplevel_entry = entry;

              /*  if we create a new session info, we never call
               *  gimp_dialog_factory_set_window_geometry(), but still
               *  the dialog needs GDK_HINT_USER_POS so it keeps its
               *  position when hidden/shown within this(!) session.
               */
              if (entry->session_managed)
                g_signal_connect (dialog, "configure_event",
                                  G_CALLBACK (gimp_dialog_factory_set_user_pos),
                                  NULL);
            }
	  else
            {
              info->dockable_entry = entry;
            }

	  factory->session_infos = g_list_append (factory->session_infos, info);
	}
    }
  else /*  dialog is a GimpDock  */
    {
      D (g_print ("%s: adding dock\n", G_GNUC_FUNCTION));

      for (list = factory->session_infos; list; list = g_list_next (list))
	{
	  info = (GimpSessionInfo *) list->data;

          /*  take the first empty slot  */
          if (! info->toplevel_entry &&
              ! info->dockable_entry &&
              ! info->widget)
	    {
	      info->widget = dialog;

              D (g_print ("%s: updating session info %p (widget %p) for dock\n",
                          G_GNUC_FUNCTION,
                          info, info->widget));

	      gimp_session_info_set_geometry (info);

	      break;
	    }
	}

      if (! list) /*  didn't find a session info  */
	{
	  info = g_new0 (GimpSessionInfo, 1);

	  info->widget = dialog;

          D (g_print ("%s: creating session info %p (widget %p) for dock\n",
                      G_GNUC_FUNCTION,
                      info, info->widget));

	  factory->session_infos = g_list_append (factory->session_infos, info);
	}
    }

  factory->open_dialogs = g_list_prepend (factory->open_dialogs, dialog);

  g_signal_connect_object (dialog, "destroy",
                           G_CALLBACK (gimp_dialog_factory_remove_dialog),
                           factory,
                           G_CONNECT_SWAPPED);

  if (entry                  &&
      entry->session_managed &&
      GTK_WIDGET_TOPLEVEL (dialog))
    {
      g_signal_connect_object (dialog, "configure_event",
                               G_CALLBACK (gimp_dialog_factory_dialog_configure),
                               factory,
                               0);
    }
}

void
gimp_dialog_factory_add_foreign (GimpDialogFactory *factory,
                                 const gchar       *identifier,
                                 GtkWidget         *dialog)
{
  GimpDialogFactory      *dialog_factory;
  GimpDialogFactoryEntry *entry;

  g_return_if_fail (GIMP_IS_DIALOG_FACTORY (factory));
  g_return_if_fail (identifier != NULL);
  g_return_if_fail (GTK_IS_WIDGET (dialog));
  g_return_if_fail (GTK_WIDGET_TOPLEVEL (dialog));

  dialog_factory = gimp_dialog_factory_from_widget (dialog, &entry);

  if (dialog_factory || entry)
    {
      g_warning ("%s: dialog was created by a GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return;
    }

  entry = gimp_dialog_factory_find_entry (factory, identifier);

  if (! entry)
    {
      g_warning ("%s: no entry registered for \"%s\"",
		 G_GNUC_FUNCTION, identifier);
      return;
    }

  if (entry->new_func)
    {
      g_warning ("%s: entry for \"%s\" has a constructor (is not foreign)",
		 G_GNUC_FUNCTION, identifier);
      return;
    }

  gimp_dialog_factory_set_widget_data (dialog, factory, entry);

  gimp_dialog_factory_add_dialog (factory, dialog);
}

void
gimp_dialog_factory_remove_dialog (GimpDialogFactory *factory,
				   GtkWidget         *dialog)
{
  GimpDialogFactory      *dialog_factory;
  GimpDialogFactoryEntry *entry;
  GimpSessionInfo        *session_info;
  GList                  *list;

  g_return_if_fail (GIMP_IS_DIALOG_FACTORY (factory));
  g_return_if_fail (GTK_IS_WIDGET (dialog));

  if (! g_list_find (factory->open_dialogs, dialog))
    {
      g_warning ("%s: dialog not registered", G_GNUC_FUNCTION);
      return;
    }

  factory->open_dialogs = g_list_remove (factory->open_dialogs, dialog);

  dialog_factory = gimp_dialog_factory_from_widget (dialog, &entry);

  if (! (dialog_factory && (entry || GIMP_IS_DOCK (dialog))))
    {
      g_warning ("%s: dialog was not created by a GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return;
    }

  if (dialog_factory != factory)
    {
      g_warning ("%s: dialog was created by a different GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return;
    }

  D (g_print ("%s: removing \"%s\"\n",
              G_GNUC_FUNCTION,
              entry ? entry->identifier : "dock"));

  for (list = factory->session_infos; list; list = g_list_next (list))
    {
      session_info = (GimpSessionInfo *) list->data;

      if (session_info->widget == dialog)
	{
          D (g_print ("%s: clearing session info %p (widget %p) for \"%s\"\n",
                      G_GNUC_FUNCTION,
                      session_info, session_info->widget,
                      entry ? entry->identifier : "dock"));

	  session_info->widget = NULL;

	  /*  don't save session info for empty docks  */
	  if (GIMP_IS_DOCK (dialog))
	    {
	      factory->session_infos = g_list_remove (factory->session_infos,
						      session_info);

	      g_free (session_info);
	    }

	  break;
	}
    }
}

void
gimp_dialog_factories_session_save (GimpConfigWriter *writer)
{
  GimpDialogFactoryClass *factory_class;

  g_return_if_fail (writer != NULL);

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  g_hash_table_foreach (factory_class->factories,
			(GHFunc) gimp_dialog_factories_save_foreach,
			writer);
}

void
gimp_dialog_factories_session_restore (void)
{
  GimpDialogFactoryClass *factory_class;

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  g_hash_table_foreach (factory_class->factories,
			(GHFunc) gimp_dialog_factories_restore_foreach,
			NULL);
}

void
gimp_dialog_factories_session_clear (void)
{
  GimpDialogFactoryClass *factory_class;

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  g_hash_table_foreach (factory_class->factories,
			(GHFunc) gimp_dialog_factories_clear_foreach,
			NULL);
}

void
gimp_dialog_factories_toggle (GimpDialogFactory *toolbox_factory)
{
  static GimpDialogShowState toggle_state = GIMP_DIALOG_SHOW_ALL;
  static gboolean            doing_update = FALSE;

  GimpDialogFactoryClass *factory_class;

  if (doing_update)
    return;

  doing_update = TRUE;

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  switch (toggle_state)
    {
    case GIMP_DIALOG_SHOW_ALL:
      toggle_state = GIMP_DIALOG_HIDE_ALL;

      g_hash_table_foreach (factory_class->factories,
			    (GHFunc) gimp_dialog_factories_hide_foreach,
			    NULL);
      break;

    case GIMP_DIALOG_HIDE_ALL:
      toggle_state = GIMP_DIALOG_SHOW_TOOLBOX;

      gimp_dialog_factories_show_foreach (GIMP_OBJECT (toolbox_factory)->name,
                                          toolbox_factory,
                                          NULL);
      break;

    case GIMP_DIALOG_SHOW_TOOLBOX:
      toggle_state = GIMP_DIALOG_SHOW_ALL;

      g_hash_table_foreach (factory_class->factories,
			    (GHFunc) gimp_dialog_factories_show_foreach,
			    NULL);
    default:
      break;
    }

  doing_update = FALSE;
}

void
gimp_dialog_factories_set_busy (void)
{
  GimpDialogFactoryClass *factory_class;

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  g_hash_table_foreach (factory_class->factories,
			(GHFunc) gimp_dialog_factories_set_busy_foreach,
			NULL);
}

void
gimp_dialog_factories_unset_busy (void)
{
  GimpDialogFactoryClass *factory_class;

  factory_class = g_type_class_peek (GIMP_TYPE_DIALOG_FACTORY);

  g_hash_table_foreach (factory_class->factories,
			(GHFunc) gimp_dialog_factories_unset_busy_foreach,
			NULL);
}


static GQuark gimp_dialog_factory_key       = 0;
static GQuark gimp_dialog_factory_entry_key = 0;

GimpDialogFactory *
gimp_dialog_factory_from_widget (GtkWidget               *dialog,
                                 GimpDialogFactoryEntry **entry)
{
  g_return_val_if_fail (GTK_IS_WIDGET (dialog), NULL);

  if (! gimp_dialog_factory_key)
    {
      gimp_dialog_factory_key =
        g_quark_from_static_string ("gimp-dialog-factory");

      gimp_dialog_factory_entry_key =
        g_quark_from_static_string ("gimp-dialog-factory-entry");
    }

  if (entry)
    *entry = g_object_get_qdata (G_OBJECT (dialog),
                                 gimp_dialog_factory_entry_key);

  return g_object_get_qdata (G_OBJECT (dialog), gimp_dialog_factory_key);
}


/*  private functions  */

void
gimp_dialog_factory_set_widget_data (GtkWidget               *dialog,
                                     GimpDialogFactory       *factory,
                                     GimpDialogFactoryEntry  *entry)
{
  g_return_if_fail (GTK_IS_WIDGET (dialog));
  g_return_if_fail (GIMP_IS_DIALOG_FACTORY (factory));

  if (! gimp_dialog_factory_key)
    {
      gimp_dialog_factory_key =
        g_quark_from_static_string ("gimp-dialog-factory");

      gimp_dialog_factory_entry_key =
        g_quark_from_static_string ("gimp-dialog-factory-entry");
    }

  g_object_set_qdata (G_OBJECT (dialog), gimp_dialog_factory_key, factory);

  if (entry)
    g_object_set_qdata (G_OBJECT (dialog), gimp_dialog_factory_entry_key,
                        entry);
}

static gboolean
gimp_dialog_factory_set_user_pos (GtkWidget         *dialog,
                                  GdkEventConfigure *cevent,
                                  gpointer           data)
{
  g_signal_handlers_disconnect_by_func (dialog,
                                        gimp_dialog_factory_set_user_pos,
                                        data);

  gtk_window_set_geometry_hints (GTK_WINDOW (dialog), NULL, NULL,
                                 GDK_HINT_USER_POS);

  return FALSE;
}

static gboolean
gimp_dialog_factory_dialog_configure (GtkWidget         *dialog,
                                      GdkEventConfigure *cevent,
                                      GimpDialogFactory *factory)
{
  GimpDialogFactory      *dialog_factory;
  GimpDialogFactoryEntry *entry;
  GList                  *list;

  if (! g_list_find (factory->open_dialogs, dialog))
    {
      g_warning ("%s: dialog not registered", G_GNUC_FUNCTION);
      return FALSE;
    }

  dialog_factory = gimp_dialog_factory_from_widget (dialog, &entry);

  if (! dialog_factory || ! entry)
    {
      g_warning ("%s: dialog was not created by a GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return FALSE;
    }

  if (dialog_factory != factory)
    {
      g_warning ("%s: dialog was created by a different GimpDialogFactory",
		 G_GNUC_FUNCTION);
      return FALSE;
    }

  for (list = factory->session_infos; list; list = g_list_next (list))
    {
      GimpSessionInfo *session_info = list->data;

      if (session_info->widget == dialog)
        {
          D (g_print ("%s: updating session info for \"%s\"\n",
                      G_GNUC_FUNCTION, entry->identifier));

          gimp_session_info_get_geometry (session_info);

          break;
	}
    }

  return FALSE;
}

static void
gimp_dialog_factories_save_foreach (gconstpointer      key,
				    GimpDialogFactory *factory,
				    GimpConfigWriter  *writer)
{
  GList *infos;

  for (infos = factory->session_infos; infos; infos = g_list_next (infos))
    {
      GimpSessionInfo *info = infos->data;

      /*  we keep session info entries for all toplevel dialogs created
       *  by the factory but don't save them if they don't want to be
       *  managed
       */
      if (info->dockable_entry ||
	  (info->toplevel_entry && ! info->toplevel_entry->session_managed))
	continue;

      gimp_session_info_save (info, GIMP_OBJECT (factory)->name, writer);
    }
}

static void
gimp_dialog_factories_restore_foreach (gconstpointer      key,
				       GimpDialogFactory *factory,
				       gpointer           data)
{
  GList *infos;

  for (infos = factory->session_infos; infos; infos = g_list_next (infos))
    {
      GimpSessionInfo *info = infos->data;

      if (info->open)
        gimp_session_info_restore (info, factory);
    }
}

static void
gimp_dialog_factories_clear_foreach (gconstpointer      key,
                                     GimpDialogFactory *factory,
                                     gpointer           data)
{
  GList *list;

  for (list = factory->session_infos; list; list = g_list_next (list))
    {
      GimpSessionInfo *info;

      info = (GimpSessionInfo *) list->data;

      if (info->widget)
        continue;

#ifdef __GNUC__
#warning FIXME: implement session info deletion
#endif
    }
}


#define GIMP_DIALOG_VISIBILITY_KEY "gimp-dialog-visibility"

static void
gimp_dialog_factories_hide_foreach (gconstpointer      key,
				    GimpDialogFactory *factory,
				    gpointer           data)
{
  GList *list;

  for (list = factory->open_dialogs; list; list = g_list_next (list))
    {
      if (GTK_IS_WIDGET (list->data) && GTK_WIDGET_TOPLEVEL (list->data))
	{
	  GimpDialogVisibilityState visibility = GIMP_DIALOG_VISIBILITY_UNKNOWN;

	  if (GTK_WIDGET_VISIBLE (list->data))
	    {
	      visibility = GIMP_DIALOG_VISIBILITY_VISIBLE;

	      gtk_widget_hide (GTK_WIDGET (list->data));
	    }
	  else
	    {
	      visibility = GIMP_DIALOG_VISIBILITY_INVISIBLE;
	    }

	  g_object_set_data (G_OBJECT (list->data),
                             GIMP_DIALOG_VISIBILITY_KEY,
                             GINT_TO_POINTER (visibility));
	}
    }
}

static void
gimp_dialog_factories_show_foreach (gconstpointer      key,
				    GimpDialogFactory *factory,
				    gpointer           data)
{
  GList *list;

  for (list = factory->open_dialogs; list; list = g_list_next (list))
    {
      if (GTK_IS_WIDGET (list->data) && GTK_WIDGET_TOPLEVEL (list->data))
	{
	  GimpDialogVisibilityState visibility;

	  visibility =
	    GPOINTER_TO_INT (g_object_get_data (G_OBJECT (list->data),
                                                GIMP_DIALOG_VISIBILITY_KEY));

	  if (! GTK_WIDGET_VISIBLE (list->data) &&
	      visibility == GIMP_DIALOG_VISIBILITY_VISIBLE)
	    {
	      gtk_widget_show (GTK_WIDGET (list->data));
	    }
	}
    }
}

static void
gimp_dialog_factories_set_busy_foreach (gconstpointer      key,
                                        GimpDialogFactory *factory,
                                        gpointer           data)
{
  GdkDisplay *display = NULL;
  GdkCursor  *cursor  = NULL;
  GList      *list;

  for (list = factory->open_dialogs; list; list = g_list_next (list))
    {
      if (GTK_IS_WIDGET (list->data)       &&
	  GTK_WIDGET_TOPLEVEL (list->data) &&
	  GTK_WIDGET_VISIBLE (list->data))
	{
          GtkWidget *widget = GTK_WIDGET (list->data);

          if (!display || display != gtk_widget_get_display (widget))
            {
              display = gtk_widget_get_display (widget);

              if (cursor)
                gdk_cursor_unref (cursor);

              cursor = gimp_cursor_new (display,
                                        GDK_WATCH,
                                        GIMP_TOOL_CURSOR_NONE,
                                        GIMP_CURSOR_MODIFIER_NONE);
            }

	  gdk_window_set_cursor (widget->window, cursor);
	}
    }

  if (cursor)
    gdk_cursor_unref (cursor);
}

static void
gimp_dialog_factories_unset_busy_foreach (gconstpointer      key,
                                          GimpDialogFactory *factory,
                                          gpointer           data)
{
  GList *list;

  for (list = factory->open_dialogs; list; list = g_list_next (list))
    {
      if (GTK_IS_WIDGET (list->data)       &&
	  GTK_WIDGET_TOPLEVEL (list->data) &&
	  GTK_WIDGET_VISIBLE (list->data))
	{
	  gdk_window_set_cursor (GTK_WIDGET (list->data)->window, NULL);
	}
    }
}
