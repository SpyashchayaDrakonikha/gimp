/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimppreview-popup.h
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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "core/gimpviewable.h"

#include "gimppreview.h"
#include "gimppreviewrenderer.h"
#include "gimppreview-popup.h"


#define PREVIEW_POPUP_DELAY 150


typedef struct _GimpPreviewPopup GimpPreviewPopup;

struct _GimpPreviewPopup
{
  GtkWidget    *widget;
  GimpViewable *viewable;

  gint          popup_width;
  gint          popup_height;
  gboolean      dot_for_dot;
  gint          button;
  gint          button_x;
  gint          button_y;

  guint         timeout_id;
  GtkWidget    *popup;
};


/*  local function prototypes  */

static void       gimp_preview_popup_hide           (GimpPreviewPopup *popup);
static gboolean   gimp_preview_popup_button_release (GtkWidget        *widget,
                                                     GdkEventButton   *bevent,
                                                     GimpPreviewPopup *popup);
static void       gimp_preview_popup_unmap          (GtkWidget        *widget,
                                                     GimpPreviewPopup *popup);
static gboolean   gimp_preview_popup_timeout        (GimpPreviewPopup *popup);


/*  public functions  */

gboolean
gimp_preview_popup_show (GtkWidget      *widget,
                         GdkEventButton *bevent,
                         GimpViewable   *viewable,
                         gint            preview_width,
                         gint            preview_height,
                         gboolean        dot_for_dot)
{
  GimpPreviewPopup *popup;
  gint              popup_width;
  gint              popup_height;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), FALSE);
  g_return_val_if_fail (bevent != NULL, FALSE);
  g_return_val_if_fail (GIMP_IS_VIEWABLE (viewable), FALSE);

  if (! gimp_viewable_get_popup_size (viewable,
                                      preview_width,
                                      preview_height,
                                      dot_for_dot,
                                      &popup_width,
                                      &popup_height))
    return FALSE;

  popup = g_new0 (GimpPreviewPopup, 1);

  popup->widget       = widget;
  popup->viewable     = viewable;
  popup->popup_width  = popup_width;
  popup->popup_height = popup_height;
  popup->dot_for_dot  = dot_for_dot;
  popup->button       = bevent->button;
  popup->button_x     = bevent->x;
  popup->button_y     = bevent->y;

  g_signal_connect (widget, "button_release_event",
                    G_CALLBACK (gimp_preview_popup_button_release),
                    popup);
  g_signal_connect (widget, "unmap",
                    G_CALLBACK (gimp_preview_popup_unmap),
                    popup);

  popup->timeout_id = g_timeout_add (PREVIEW_POPUP_DELAY,
				     (GSourceFunc) gimp_preview_popup_timeout,
				     popup);

  g_object_set_data_full (G_OBJECT (widget), "gimp-preview-popup", popup,
                          (GDestroyNotify) gimp_preview_popup_hide);

  gtk_grab_add (widget);

  return TRUE;
}


/*  private functions  */

static void
gimp_preview_popup_hide (GimpPreviewPopup *popup)
{
  if (popup->timeout_id)
    g_source_remove (popup->timeout_id);

  if (popup->popup)
    gtk_widget_destroy (popup->popup);

  g_signal_handlers_disconnect_by_func (popup->widget,
                                        gimp_preview_popup_button_release,
                                        popup);
  g_signal_handlers_disconnect_by_func (popup->widget,
                                        gimp_preview_popup_unmap,
                                        popup);

  gtk_grab_remove (popup->widget);

  g_free (popup);
}

static gboolean
gimp_preview_popup_button_release (GtkWidget        *widget,
				   GdkEventButton   *bevent,
                                   GimpPreviewPopup *popup)
{
  if (bevent->button == popup->button)
    g_object_set_data (G_OBJECT (popup->widget), "gimp-preview-popup", NULL);

  return FALSE;
}

static void
gimp_preview_popup_unmap (GtkWidget        *widget,
                          GimpPreviewPopup *popup)
{
  g_object_set_data (G_OBJECT (popup->widget), "gimp-preview-popup", NULL);
}

static gboolean
gimp_preview_popup_timeout (GimpPreviewPopup *popup)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *preview;
  GdkScreen *screen;
  gint       orig_x;
  gint       orig_y;
  gint       scr_width;
  gint       scr_height;
  gint       x;
  gint       y;

  popup->timeout_id = 0;

  window = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER (window), frame);
  gtk_widget_show (frame);

  preview = gimp_preview_new_full (popup->viewable,
                                   popup->popup_width,
                                   popup->popup_height,
                                   0, TRUE, FALSE, FALSE);
  gimp_preview_renderer_set_dot_for_dot (GIMP_PREVIEW (preview)->renderer,
                                         popup->dot_for_dot);
  gtk_container_add (GTK_CONTAINER (frame), preview);
  gtk_widget_show (preview);

  gdk_window_get_origin (popup->widget->window, &orig_x, &orig_y);

  screen = gtk_widget_get_screen (popup->widget);
  scr_width  = gdk_screen_get_width (screen);
  scr_height = gdk_screen_get_height (screen);

  x = orig_x + popup->button_x - (popup->popup_width  >> 1);
  y = orig_y + popup->button_y - (popup->popup_height >> 1);

  x = CLAMP (x, 0, scr_width  - popup->popup_width);
  y = CLAMP (y, 0, scr_height - popup->popup_height);

  gtk_window_move (GTK_WINDOW (window), x, y);
  gtk_widget_show (window);

  popup->popup = window;

  return FALSE;
}
