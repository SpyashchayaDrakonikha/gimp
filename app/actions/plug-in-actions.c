/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "core/gimp.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"

#include "pdb/procedural_db.h"

#include "plug-in/plug-ins.h"
#include "plug-in/plug-in-proc.h"

#include "widgets/gimpactiongroup.h"
#include "widgets/gimphelp-ids.h"

#include "actions.h"
#include "plug-in-actions.h"
#include "plug-in-commands.h"

#include "gimp-intl.h"


/*  local function prototypes  */

static void   plug_in_actions_build_path (GimpActionGroup *group,
                                          const gchar     *path_original,
                                          const gchar     *path_translated);


/*  private variables  */

static GimpActionEntry plug_in_actions[] =
{
  { "plug-in-menu",                NULL, N_("Filte_rs")       },
  { "plug-in-blur-menu",           NULL, N_("_Blur")          },
  { "plug-in-colors-menu",         NULL, N_("_Colors")        },
  { "plug-in-colors-map-menu",     NULL, N_("Ma_p")           },
  { "plug-in-noise-menu",          NULL, N_("_Noise")         },
  { "plug-in-edge-detect-menu",    NULL, N_("Edge-De_tect")   },
  { "plug-in-enhance-menu",        NULL, N_("En_hance")       },
  { "plug-in-generic-menu",        NULL, N_("_Generic")       },
  { "plug-in-glass-effects-menu",  NULL, N_("Gla_ss Effects") },
  { "plug-in-light-effects-menu",  NULL, N_("_Light Effects") },
  { "plug-in-distorts-menu",       NULL, N_("_Distorts")      },
  { "plug-in-artistic-menu",       NULL, N_("_Artistic")      },
  { "plug-in-map-menu",            NULL, N_("_Map")           },
  { "plug-in-render-menu",         NULL, N_("_Render")        },
  { "plug-in-render-clouds-menu",  NULL, N_("_Clouds")        },
  { "plug-in-render-nature-menu",  NULL, N_("_Nature")        },
  { "plug-in-render-pattern-menu", NULL, N_("_Pattern")       },
  { "plug-in-web-menu",            NULL, N_("_Web")           },
  { "plug-in-animation-menu",      NULL, N_("An_imation")     },
  { "plug-in-combine-menu",        NULL, N_("C_ombine")       },
  { "plug-in-toys-menu",           NULL, N_("To_ys")          }
};

static GimpEnumActionEntry plug_in_repeat_actions[] =
{
  { "plug-in-repeat", GTK_STOCK_EXECUTE,
    N_("Repeat Last"), "<control>F", NULL,
    FALSE,
    GIMP_HELP_FILTER_REPEAT },

  { "plug-in-reshow", GIMP_STOCK_RESHOW_FILTER,
    N_("Re-Show Last"), "<control><shift>F", NULL,
    TRUE,
    GIMP_HELP_FILTER_RESHOW }
};


/*  public functions  */

void
plug_in_actions_setup (GimpActionGroup *group)
{
  GSList *list;

  gimp_action_group_add_actions (group,
                                 plug_in_actions,
                                 G_N_ELEMENTS (plug_in_actions));

  gimp_action_group_add_enum_actions (group,
                                      plug_in_repeat_actions,
                                      G_N_ELEMENTS (plug_in_repeat_actions),
                                      G_CALLBACK (plug_in_repeat_cmd_callback));

  for (list = group->gimp->plug_in_proc_defs;
       list;
       list = g_slist_next (list))
    {
      PlugInProcDef *proc_def = list->data;

      if (proc_def->prog         &&
          proc_def->menu_path    &&
          ! proc_def->extensions &&
          ! proc_def->prefixes   &&
          ! proc_def->magics)
        {
          plug_in_actions_add_proc (group, proc_def);
        }
    }
}

void
plug_in_actions_update (GimpActionGroup *group,
                        gpointer         data)
{
  GimpImage     *gimage;
  GimpImageType  type   = -1;
  GSList        *list;

  gimage = action_data_get_image (data);

  if (gimage)
    {
      GimpDrawable *drawable = gimp_image_active_drawable (gimage);

      type = gimp_drawable_type (drawable);
    }

  for (list = group->gimp->plug_in_proc_defs;
       list;
       list = g_slist_next (list))
    {
      PlugInProcDef *proc_def = list->data;

      if (proc_def->menu_path       &&
          proc_def->image_types_val &&
          ! proc_def->extensions    &&
          ! proc_def->prefixes      &&
          ! proc_def->magics)
        {
          gboolean sensitive;

          switch (type)
            {
            case GIMP_RGB_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_RGB_IMAGE;
              break;
            case GIMP_RGBA_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_RGBA_IMAGE;
              break;
            case GIMP_GRAY_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_GRAY_IMAGE;
              break;
            case GIMP_GRAYA_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_GRAYA_IMAGE;
              break;
            case GIMP_INDEXED_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_INDEXED_IMAGE;
              break;
            case GIMP_INDEXEDA_IMAGE:
              sensitive = proc_def->image_types_val & PLUG_IN_INDEXEDA_IMAGE;
              break;
            default:
              sensitive = FALSE;
              break;
            }

          gimp_action_group_set_action_sensitive (group,
                                                  proc_def->db_info.name,
                                                  sensitive);

          if (group->gimp->last_plug_in &&
              group->gimp->last_plug_in == &proc_def->db_info)
            {
              const gchar *progname;
              const gchar *path;
              gchar       *stripped;
              gchar       *basename;
              gchar       *ellipses;
              gchar       *repeat;
              gchar       *reshow;

              progname = plug_in_proc_def_get_progname (proc_def);

              path = dgettext (plug_ins_locale_domain (group->gimp,
                                                       progname, NULL),
                               proc_def->menu_path);

              stripped = gimp_strip_uline (path);
              basename = g_path_get_basename (stripped);

              g_free (stripped);

              ellipses = strstr (basename, "...");

              if (ellipses && ellipses == (basename + strlen (basename) - 3))
                *ellipses = '\0';

              repeat = g_strdup_printf (_("Re_peat \"%s\""), basename);
              reshow = g_strdup_printf (_("R_e-show \"%s\""), basename);

              g_free (basename);

              gimp_action_group_set_action_label (group, "plug-in-repeat",
                                                  repeat);
              gimp_action_group_set_action_label (group, "plug-in-reshow",
                                                  reshow);

              g_free (repeat);
              g_free (reshow);

              gimp_action_group_set_action_sensitive (group, "plug-in-repeat",
                                                      TRUE);
              gimp_action_group_set_action_sensitive (group, "plug-in-reshow",
                                                      TRUE);
	    }
	}
    }

  if (! group->gimp->last_plug_in)
    {
      gimp_action_group_set_action_label (group, "plug-in-repeat",
                                          _("Repeat Last"));
      gimp_action_group_set_action_label (group, "plug-in-reshow",
                                          _("Re-Show Last"));

      gimp_action_group_set_action_sensitive (group, "plug-in-repeat", FALSE);
      gimp_action_group_set_action_sensitive (group, "plug-in-reshow", FALSE);
    }
}

void
plug_in_actions_add_proc (GimpActionGroup *group,
                          PlugInProcDef   *proc_def)
{
  const gchar *progname;
  const gchar *locale_domain;
  const gchar *help_domain;
  gchar       *path_original;
  gchar       *path_translated;
  gchar       *help_id;
  gchar       *p1, *p2;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (proc_def != NULL);

  progname = plug_in_proc_def_get_progname (proc_def);

  locale_domain = plug_ins_locale_domain (group->gimp, progname, NULL);
  help_domain   = plug_ins_help_domain (group->gimp, progname, NULL);

  help_id = plug_in_proc_def_get_help_id (proc_def, help_domain);

  path_original   = g_strdup (proc_def->menu_path);
  path_translated = g_strdup (dgettext (locale_domain, path_original));

  p1 = strrchr (path_original, '/');
  p2 = strrchr (path_translated, '/');

  if (p1 && p2)
    {
      GimpPlugInActionEntry  entry;
      gchar                 *label;

      label = p2 + 1;

      entry.name        = proc_def->db_info.name;
      entry.stock_id    = NULL;
      entry.label       = label;
      entry.accelerator = proc_def->accelerator;
      entry.tooltip     = NULL;
      entry.proc_def    = proc_def;
      entry.help_id     = help_id;

#if 0
      g_print ("adding plug-in action '%s' (%s)\n",
               proc_def->db_info.name, label);
#endif

      gimp_action_group_add_plug_in_actions (group, &entry, 1,
                                             G_CALLBACK (plug_in_run_cmd_callback));

      *p1 = '\0';
      *p2 = '\0';

      plug_in_actions_build_path (group, path_original, path_translated);
    }

  g_free (path_original);
  g_free (path_translated);

  g_free (help_id);
}

void
plug_in_actions_remove_proc (GimpActionGroup *group,
                             PlugInProcDef   *proc_def)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (proc_def != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group),
                                        proc_def->db_info.name);

  if (action)
    {
#if 0
      g_print ("removing plug-in action '%s'\n",
               proc_def->db_info.name);
#endif

      gtk_action_group_remove_action (GTK_ACTION_GROUP (group), action);
    }
}


/*  private functions  */

static void
plug_in_actions_build_path (GimpActionGroup *group,
                            const gchar     *path_original,
                            const gchar     *path_translated)
{
  GHashTable *path_table;
  gchar      *p1, *p2;

  path_table = g_object_get_data (G_OBJECT (group), "plug-in-path-table");

  if (! path_table)
    {
      path_table = g_hash_table_new_full (g_str_hash, g_str_equal,
                                          g_free, NULL);

      g_object_set_data_full (G_OBJECT (group), "plug-in-path-table",
                              path_table,
                              (GDestroyNotify) g_hash_table_destroy);
    }

  p1 = strrchr (path_original, '/');
  p2 = strrchr (path_translated, '/');

  if (p1 && p2 && ! g_hash_table_lookup (path_table, path_original))
    {
      gchar     *copy_original   = g_strdup (path_original);
      gchar     *copy_translated = g_strdup (path_translated);
      gchar     *label;
      GtkAction *action;

      label = p2 + 1;

#if 0
      g_print ("adding plug-in submenu '%s' (%s)\n",
               path_original, label);
#endif

      action = gtk_action_new (path_original, label, NULL, NULL);
      gtk_action_group_add_action (GTK_ACTION_GROUP (group), action);
      g_object_unref (action);

      g_hash_table_insert (path_table, g_strdup (path_original), action);

      p1 = strrchr (copy_original, '/');
      p2 = strrchr (copy_translated, '/');

      *p1 = '\0';
      *p2 = '\0';

      plug_in_actions_build_path (group, copy_original, copy_translated);

      g_free (copy_original);
      g_free (copy_translated);
    }
}
