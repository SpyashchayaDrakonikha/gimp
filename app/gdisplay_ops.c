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

#include <gtk/gtk.h>

#include "apptypes.h"

#include "appenv.h"
#include "colormaps.h"
#include "cursorutil.h"
#include "fileops.h"
#include "gdisplay_ops.h"
#include "gimage.h"
#include "gimpui.h"
#include "gximage.h"
#include "menus.h"
#include "scale.h"
#include "gimprc.h"

#include "libgimp/gimpintl.h"

static void gdisplay_close_warning_callback  (GtkWidget *widget,
					      gboolean   close,
					      gpointer   data);
static void gdisplay_close_warning_dialog    (gchar     *image_name,
					      GDisplay  *gdisp);

/*
 *  This file is for operations on the gdisplay object
 */

gulong
gdisplay_white_pixel (GDisplay *gdisp)
{
  return g_white_pixel;
}

gulong
gdisplay_gray_pixel (GDisplay *gdisp)
{
  return g_gray_pixel;
}

gulong
gdisplay_black_pixel (GDisplay *gdisp)
{
  return g_black_pixel;
}

gulong
gdisplay_color_pixel (GDisplay *gdisp)
{
  return g_color_pixel;
}


void
gdisplay_xserver_resolution (gdouble *xres,
			     gdouble *yres)
{
  gint width, height;
  gint widthMM, heightMM;

  width  = gdk_screen_width ();
  height = gdk_screen_height ();

  widthMM  = gdk_screen_width_mm ();
  heightMM = gdk_screen_height_mm ();

  /*
   * From xdpyinfo.c:
   *
   * there are 2.54 centimeters to an inch; so there are 25.4 millimeters.
   *
   *     dpi = N pixels / (M millimeters / (25.4 millimeters / 1 inch))
   *         = N pixels / (M inch / 25.4)
   *         = N * 25.4 pixels / M inch
   */

  *xres = (width  * 25.4) / ((gdouble) widthMM);
  *yres = (height * 25.4) / ((gdouble) heightMM);
}


void
gdisplay_new_view (GDisplay *gdisp)
{
  GDisplay *new_gdisp;

  /* make sure the image has been fully loaded... */
  if (gdisp->gimage)
    {
      new_gdisp = gdisplay_new (gdisp->gimage, gdisp->scale);
    }
}


void
gdisplay_close_window (GDisplay *gdisp,
		       gboolean  kill_it)
{
  /*  FIXME: gimp_busy HACK not really appropriate here because we only
   *  want to prevent the busy image and display to be closed.  --Mitch
   */
  if (gimp_busy)
    return;

  /*  If the image has been modified, give the user a chance to save
   *  it before nuking it--this only applies if its the last view
   *  to an image canvas.  (a gimage with disp_count = 1)
   */
  if (! kill_it &&
      (gdisp->gimage->disp_count == 1) &&
      gdisp->gimage->dirty &&
      confirm_on_close)
    {
      gdisplay_close_warning_dialog
	(g_basename (gimage_filename (gdisp->gimage)), gdisp);
    }
  else
    {
      gtk_widget_destroy (gdisp->shell);
    }
}


void
gdisplay_shrink_wrap (GDisplay *gdisp)
{
  /* 
   * I'm pretty sure this assumes that the current size is < display size
   * Is this a valid assumption? 
   */
  GtkAllocation allocation;
  gint disp_width, disp_height;
  gint width, height;
  gint shell_x, shell_y;
  gint shell_width, shell_height;
  gint max_auto_width, max_auto_height;
  gint border_x, border_y;
  gint s_width, s_height;

  gboolean resize = FALSE;

  s_width  = gdk_screen_width ();
  s_height = gdk_screen_height ();

  width  = SCALEX (gdisp, gdisp->gimage->width);
  height = SCALEY (gdisp, gdisp->gimage->height);

  disp_width  = gdisp->disp_width;
  disp_height = gdisp->disp_height;

  shell_width  = gdisp->shell->allocation.width;
  shell_height = gdisp->shell->allocation.height;

  border_x = shell_width  - disp_width;
  border_y = shell_height - disp_height;

  max_auto_width  = (s_width  - border_x) * 0.75;
  max_auto_height = (s_height - border_y) * 0.75;

  allocation.x = 0;
  allocation.y = 0;

  /* If one of the display dimensions has changed and one of the
   * dimensions fits inside the screen
   */
  if (((width + border_x) < s_width || (height + border_y) < s_height) &&
      (width != disp_width || height != disp_height))
    {
      width  = ((width  + border_x) < s_width)  ? width  : max_auto_width;
      height = ((height + border_y) < s_height) ? height : max_auto_height;

      resize = TRUE;
    }
  /*  If the projected dimension is greater than current, but less than
   *  3/4 of the screen size, expand automagically
   */
  else if ((width > disp_width || height > disp_height) &&
	   (disp_width < max_auto_width || disp_height < max_auto_height))
    {
      width  = MIN (max_auto_width, width);
      height = MIN (max_auto_height, height);

      resize = TRUE;
    }

  if (resize)
    {
      if (width < gdisp->statusarea->requisition.width) 
        { 
          width = gdisp->statusarea->requisition.width; 
        }

#undef RESIZE_DEBUG
#ifdef RESIZE_DEBUG
      g_print ("1w:%d/%d d:%d/%d s:%d/%d b:%d/%d\n",
	       width, height,
	       disp_width, disp_height,
	       shell_width, shell_height,
	       border_x, border_y);
#endif /* RESIZE_DEBUG */

      gdisp->disp_width  = width;
      gdisp->disp_height = height;

      allocation.width  = width  + border_x;
      allocation.height = height + border_y;

      /*  don't call gdisplay_canvas_events() on any of the following
       *  changes because our caller has to do a full display update anyway
       */
      gtk_signal_handler_block_by_data (GTK_OBJECT (gdisp->canvas), gdisp);

      gtk_widget_size_allocate (gdisp->shell, &allocation);

      gdk_window_resize (gdisp->shell->window,
			 allocation.width,
			 allocation.height);

      /*  let Gtk/X/WM position the window  */
      while (gtk_events_pending ())
	gtk_main_iteration ();

      gdk_window_get_origin (gdisp->shell->window, &shell_x, &shell_y);

      /*  if the window is offscreen, center it...  */
      if (shell_x > s_width || shell_y > s_height ||
	  (shell_x + width +  border_x) < 0 || (shell_y + height + border_y) < 0)
	{
	  shell_x = (s_width  - width  - border_x) >> 1;
	  shell_y = (s_height - height - border_y) >> 1;

	  gdk_window_move (gdisp->shell->window, shell_x, shell_y);
	}

      gtk_signal_handler_unblock_by_data (GTK_OBJECT (gdisp->canvas), gdisp);
    }

  /*  If the width or height of the display has changed, recalculate
   *  the display offsets...
   */
  if (disp_width  != gdisp->disp_width ||
      disp_height != gdisp->disp_height)
    {
      gdisp->offset_x += (disp_width  - gdisp->disp_width) / 2;
      gdisp->offset_y += (disp_height - gdisp->disp_height) / 2;
    }
}

/********************************************************
 *   Routines to query before closing a dirty image     *
 ********************************************************/

static void
gdisplay_close_warning_callback (GtkWidget *widget,
				 gboolean   close,
				 gpointer   data)
{
  GDisplay *gdisp;

  gdisp = (GDisplay *) data;

  gdisp->warning_dialog = NULL;

  if (close)
    gtk_widget_destroy (gdisp->shell);
}

static void
gdisplay_close_warning_dialog (gchar    *image_name,
			       GDisplay *gdisp)
{
  GtkWidget *mbox;
  gchar     *warning_buf;

  if (gdisp->warning_dialog != NULL)
    {
      gdk_window_raise (gdisp->warning_dialog->window);
      return;
    }

  warning_buf =
    g_strdup_printf (_("Changes were made to %s.\nClose anyway?"), image_name);

  gdisp->warning_dialog = mbox =
    gimp_query_boolean_box (image_name,
			    gimp_standard_help_func,
			    "dialogs/really_close.html",
			    FALSE,
			    warning_buf,
			    _("Close"), _("Cancel"),
			    NULL, NULL,
			    gdisplay_close_warning_callback,
			    gdisp);

  g_free (warning_buf);

  gtk_widget_show (mbox);
}
