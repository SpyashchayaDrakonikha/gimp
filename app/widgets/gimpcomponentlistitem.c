/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcomponentlistitem.c
 * Copyright (C) 2001 Michael Natterer
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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "apptypes.h"

#include "gdisplay.h"
#include "gimpcontainer.h"
#include "gimpdnd.h"
#include "gimpimage.h"
#include "gimpviewable.h"

#include "gimpcomponentlistitem.h"
#include "gimpimagepreview.h"
#include "gimppreview.h"

#include "pixmaps/eye.xpm"


static void   gimp_component_list_item_class_init (GimpComponentListItemClass *klass);
static void   gimp_component_list_item_init       (GimpComponentListItem      *list_item);

static void   gimp_component_list_item_set_viewable       (GimpListItem *list_item,
							   GimpViewable *viewable);

static void   gimp_component_list_item_eye_toggled        (GtkWidget    *widget,
							   gpointer      data);

static void   gimp_component_list_item_visibility_changed (GimpImage    *gimage,
							   ChannelType   channel,
							   gpointer      data);

static void   gimp_component_list_item_state_changed      (GtkWidget    *widget,
							   GtkStateType  previous_state);
static void   gimp_component_list_item_active_changed     (GimpImage    *gimage,
							   ChannelType   channel,
							   gpointer      data);


static GimpListItemClass *parent_class = NULL;


GtkType
gimp_component_list_item_get_type (void)
{
  static GtkType list_item_type = 0;

  if (! list_item_type)
    {
      static const GtkTypeInfo list_item_info =
      {
	"GimpComponentListItem",
	sizeof (GimpComponentListItem),
	sizeof (GimpComponentListItemClass),
	(GtkClassInitFunc) gimp_component_list_item_class_init,
	(GtkObjectInitFunc) gimp_component_list_item_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      list_item_type = gtk_type_unique (GIMP_TYPE_LIST_ITEM, &list_item_info);
    }

  return list_item_type;
}

static void
gimp_component_list_item_class_init (GimpComponentListItemClass *klass)
{
  GtkWidgetClass    *widget_class;
  GtkItemClass      *item_class;
  GimpListItemClass *list_item_class;

  widget_class    = (GtkWidgetClass *) klass;
  item_class      = (GtkItemClass *) klass;
  list_item_class = (GimpListItemClass *) klass;

  parent_class = gtk_type_class (GIMP_TYPE_LIST_ITEM);

  widget_class->state_changed   = gimp_component_list_item_state_changed;

  list_item_class->set_viewable = gimp_component_list_item_set_viewable;
}

static void
gimp_component_list_item_init (GimpComponentListItem *list_item)
{
  GtkWidget *abox;
  GtkWidget *pixmap;

  list_item->channel = 0;

  abox = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
  gtk_box_pack_start (GTK_BOX (GIMP_LIST_ITEM (list_item)->hbox), abox,
                      FALSE, FALSE, 0);
  gtk_widget_show (abox);

  list_item->eye_button = gtk_toggle_button_new ();
  gtk_button_set_relief (GTK_BUTTON (list_item->eye_button), GTK_RELIEF_NONE);
  gtk_container_add (GTK_CONTAINER (abox), list_item->eye_button);
  gtk_widget_show (list_item->eye_button);

  gtk_signal_connect (GTK_OBJECT (list_item->eye_button), "realize",
                      GTK_SIGNAL_FUNC (gimp_list_item_button_realize),
                      list_item);

  gtk_signal_connect (GTK_OBJECT (list_item->eye_button), "state_changed",
                      GTK_SIGNAL_FUNC (gimp_list_item_button_state_changed),
                      list_item);

  pixmap = gimp_pixmap_new (eye_xpm);
  gtk_container_add (GTK_CONTAINER (list_item->eye_button), pixmap);
  gtk_widget_show (pixmap);
}

GtkWidget *
gimp_component_list_item_new (GimpImage   *gimage,
			      gint         preview_size,
			      ChannelType  channel)
{
  GimpListItem *list_item;

  g_return_val_if_fail (gimage != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_IMAGE (gimage), NULL);
  g_return_val_if_fail (preview_size > 0 && preview_size <= 256, NULL);
  g_return_val_if_fail (channel >= RED_CHANNEL && channel <= ALPHA_CHANNEL,
			NULL);

  list_item = gtk_type_new (GIMP_TYPE_COMPONENT_LIST_ITEM);

  list_item->preview_size = preview_size;

  GIMP_COMPONENT_LIST_ITEM (list_item)->channel = channel;

  gimp_list_item_set_viewable (list_item, GIMP_VIEWABLE (gimage));

  GTK_PREVIEW (list_item->preview)->bpp  = 1;
  GTK_PREVIEW (list_item->preview)->type = GTK_PREVIEW_GRAYSCALE;

  return GTK_WIDGET (list_item);
}

static void
gimp_component_list_item_set_viewable (GimpListItem *list_item,
				       GimpViewable *viewable)
{
  GimpComponentListItem *component_item;
  GimpImage             *gimage;
  gboolean               visible;
  gboolean               active;
  gint                   pixel;

  if (GIMP_LIST_ITEM_CLASS (parent_class)->set_viewable)
    GIMP_LIST_ITEM_CLASS (parent_class)->set_viewable (list_item, viewable);

  component_item = GIMP_COMPONENT_LIST_ITEM (list_item);
  gimage         = GIMP_IMAGE (GIMP_PREVIEW (list_item->preview)->viewable);
  visible        = gimp_image_get_component_visible (gimage,
						     component_item->channel);
  active         = gimp_image_get_component_active (gimage,
						    component_item->channel);

  switch (component_item->channel)
    {
    case RED_CHANNEL:     pixel = RED_PIX;     break;
    case GREEN_CHANNEL:   pixel = GREEN_PIX;   break;
    case BLUE_CHANNEL:    pixel = BLUE_PIX;    break;
    case GRAY_CHANNEL:    pixel = GRAY_PIX;    break;
    case INDEXED_CHANNEL: pixel = INDEXED_PIX; break;
    case ALPHA_CHANNEL:   pixel = ALPHA_PIX;   break;

    default:
      pixel = 0;
    }

  GIMP_IMAGE_PREVIEW (list_item->preview)->channel = pixel;

  if (! visible)
    {
      GtkRequisition requisition;

      gtk_widget_size_request (component_item->eye_button, &requisition);

      gtk_widget_set_usize (component_item->eye_button,
                            requisition.width,
                            requisition.height);
      gtk_widget_hide (GTK_BIN (component_item->eye_button)->child);
    }

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (component_item->eye_button),
                                visible);

  if (active)
    gtk_item_select (GTK_ITEM (list_item));

  gtk_signal_connect (GTK_OBJECT (component_item->eye_button), "toggled",
                      GTK_SIGNAL_FUNC (gimp_component_list_item_eye_toggled),
                      list_item);

  gtk_signal_connect_while_alive
    (GTK_OBJECT (viewable), "component_visibility_changed",
     GTK_SIGNAL_FUNC (gimp_component_list_item_visibility_changed),
     list_item,
     GTK_OBJECT (list_item));

  gtk_signal_connect_while_alive
    (GTK_OBJECT (viewable), "component_active_changed",
     GTK_SIGNAL_FUNC (gimp_component_list_item_active_changed),
     list_item,
     GTK_OBJECT (list_item));
}

static void
gimp_component_list_item_eye_toggled (GtkWidget *widget,
				      gpointer   data)
{
  GimpComponentListItem *component_item;
  GimpListItem          *list_item;
  GimpImage             *gimage;
  gboolean               visible;

  component_item = GIMP_COMPONENT_LIST_ITEM (data);
  list_item      = GIMP_LIST_ITEM (data);
  gimage         = GIMP_IMAGE (GIMP_PREVIEW (list_item->preview)->viewable);
  visible        = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget));

  if (visible != gimp_image_get_component_visible (gimage,
						   component_item->channel))
    {
      if (! visible)
        {
          gtk_widget_set_usize (GTK_WIDGET (widget),
                                GTK_WIDGET (widget)->allocation.width,
                                GTK_WIDGET (widget)->allocation.height);
          gtk_widget_hide (GTK_BIN (widget)->child);
        }
      else
        {
          gtk_widget_show (GTK_BIN (widget)->child);
          gtk_widget_set_usize (GTK_WIDGET (widget), -1, -1);
        }

      gtk_signal_handler_block_by_func (GTK_OBJECT (gimage),
                                        gimp_component_list_item_visibility_changed,
                                        list_item);

      gimp_image_set_component_visible (gimage, component_item->channel,
					visible);

      gtk_signal_handler_unblock_by_func (GTK_OBJECT (gimage),
                                          gimp_component_list_item_visibility_changed,
                                          list_item);

      gdisplays_update_area (gimage, 0, 0, gimage->width, gimage->height);
      gdisplays_flush ();
    }
}

static void
gimp_component_list_item_visibility_changed (GimpImage   *gimage,
					     ChannelType  channel,
					     gpointer     data)
{
  GimpComponentListItem *component_item;
  GimpListItem          *list_item;
  GtkToggleButton       *toggle;
  gboolean               visible;

  component_item = GIMP_COMPONENT_LIST_ITEM (data);

  if (channel != component_item->channel)
    return;

  list_item = GIMP_LIST_ITEM (data);
  toggle    = GTK_TOGGLE_BUTTON (component_item->eye_button);
  visible   = gimp_image_get_component_visible (gimage, component_item->channel);

  if (visible != toggle->active)
    {
      if (! visible)
        {
          gtk_widget_set_usize (GTK_WIDGET (toggle),
                                GTK_WIDGET (toggle)->allocation.width,
                                GTK_WIDGET (toggle)->allocation.height);
          gtk_widget_hide (GTK_BIN (toggle)->child);
        }
      else
        {
          gtk_widget_show (GTK_BIN (toggle)->child);
          gtk_widget_set_usize (GTK_WIDGET (toggle), -1, -1);
        }

      gtk_signal_handler_block_by_func (GTK_OBJECT (toggle),
                                        gimp_component_list_item_eye_toggled,
                                        list_item);

      gtk_toggle_button_set_active (toggle, visible);

      gtk_signal_handler_unblock_by_func (GTK_OBJECT (toggle),
                                          gimp_component_list_item_eye_toggled,
                                          list_item);
    }
}

static void
gimp_component_list_item_state_changed (GtkWidget    *widget,
					GtkStateType  previous_state)
{
  /*
  GimpComponentListItem *component_item;
  GimpListItem          *list_item;
  GimpImage             *gimage;
  gboolean               active;
  */

  if (GTK_WIDGET_CLASS (parent_class)->state_changed)
    GTK_WIDGET_CLASS (parent_class)->state_changed (widget, previous_state);

  /*
  component_item = GIMP_COMPONENT_LIST_ITEM (widget);
  list_item      = GIMP_LIST_ITEM (widget);
  gimage         = GIMP_IMAGE (GIMP_PREVIEW (list_item->preview)->viewable);

  g_print ("state: %d\n", widget->state);

  if (widget->state == GTK_STATE_SELECTED &&
      previous_state == GTK_STATE_NORMAL)
    {
      active = TRUE;
    }
  else if (widget->state == GTK_STATE_NORMAL &&
	   previous_state == GTK_STATE_SELECTED)
    {
      active = FALSE;
    }
  else
    return;

  g_print ("active: %d\n", active);

  if (active != gimp_image_get_component_active (gimage,
						 component_item->channel))
    {
      gtk_signal_handler_block_by_func (GTK_OBJECT (gimage),
                                        gimp_component_list_item_active_changed,
                                        widget);

      gimp_image_set_component_active (gimage, component_item->channel,
				       active);

      gtk_signal_handler_unblock_by_func (GTK_OBJECT (gimage),
                                          gimp_component_list_item_active_changed,
                                          widget);
    }
  */
}

static void
gimp_component_list_item_active_changed (GimpImage   *gimage,
					 ChannelType  channel,
					 gpointer     data)
{
  GimpComponentListItem *component_item;
  gboolean               active;

  component_item = GIMP_COMPONENT_LIST_ITEM (data);

  if (channel != component_item->channel)
    return;

  active = gimp_image_get_component_active (gimage, component_item->channel);

  if (active)
    gtk_item_select (GTK_ITEM (data));
  else
    gtk_item_deselect (GTK_ITEM (data));
}
