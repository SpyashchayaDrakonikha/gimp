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

#include "libgimpmath/gimpmath.h"
#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "resolution-calibrate-dialog.h"

#include "gimp-intl.h"


#define SET_STYLE(widget, style) \
        if (style) gtk_widget_modify_style (widget, style)


static GtkWidget *calibrate_entry = NULL;
static gdouble    calibrate_xres  = 1.0;
static gdouble    calibrate_yres  = 1.0;
static gint       ruler_width     = 1;
static gint       ruler_height    = 1;


static void
resolution_calibrate_ok (GtkWidget *button,
			 gpointer   data)
{
  GtkWidget *resolution_entry;
  GtkWidget *chain_button;
  gdouble x, y;

  resolution_entry = g_object_get_data (G_OBJECT (data), "resolution-entry");

  x = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (calibrate_entry), 0);
  y = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (calibrate_entry), 1);

  calibrate_xres = (gdouble)ruler_width  * calibrate_xres / x;
  calibrate_yres = (gdouble)ruler_height * calibrate_yres / y;

  chain_button = g_object_get_data (G_OBJECT (resolution_entry),
                                    "chain_button");
  if (chain_button &&
      ABS (x -y) > GIMP_MIN_RESOLUTION)
    gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain_button), FALSE);

  gimp_size_entry_set_refval (GIMP_SIZE_ENTRY (resolution_entry),
                              0, calibrate_xres);
  gimp_size_entry_set_refval (GIMP_SIZE_ENTRY (resolution_entry),
                              1, calibrate_yres);

  gtk_widget_destroy (GTK_WIDGET (data));
}

/**
 * resolution_calibrate_dialog:
 * @resolution_entry: a #GimpSizeEntry to connect the dialog to
 * @pixbuf:           an optional #GdkPixbuf for the upper left corner
 * @dialog_style:     a #GtkStyle for the main dialog (used by the
 *                    user_installation_dialog)
 * @ruler_style:      a #GtkStyle for the rulers and the entry area
 *                    (used by the user_installation_dialog)
 * @expose_callback:  an "expose_event" handler used by the
 *                    user_installation_dialog
 *
 * Displays a dialog that allows the user to interactively determine
 * her monitor resolution. This dialog runs it's own GTK main loop and
 * is connected to a #GimpSizeEntry handling the resolution to be
 * set. The style and callback parameters are supposed to be only used
 * by the user_installation_dialog.
 **/
void
resolution_calibrate_dialog (GtkWidget  *resolution_entry,
                             GdkPixbuf  *pixbuf,
			     GtkRcStyle *dialog_style,
			     GtkRcStyle *ruler_style,
			     GCallback   expose_callback)
{
  GtkWidget *dialog;
  GtkWidget *table;
  GtkWidget *ebox;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *darea;
  GtkWidget *ruler;
  GtkWidget *label;
  GdkScreen *screen;
  GList     *list;

  g_return_if_fail (GIMP_IS_SIZE_ENTRY (resolution_entry));
  g_return_if_fail (pixbuf == NULL || GDK_IS_PIXBUF (pixbuf));

  /*  this dialog can only exist once  */
  if (calibrate_entry)
    return;

  dialog = gimp_dialog_new (_("Calibrate Monitor Resolution"),
			    "calibrate_resolution",
			    NULL, NULL,
			    GTK_WIN_POS_CENTER,
			    FALSE, FALSE, FALSE,

			    GTK_STOCK_CANCEL, gtk_widget_destroy,
			    NULL, 1, NULL, FALSE, TRUE,

			    GTK_STOCK_OK, resolution_calibrate_ok,
			    NULL, NULL, NULL, TRUE, FALSE,

			    NULL);

  g_signal_connect (dialog, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_object_set_data (G_OBJECT (dialog), "resolution-entry", resolution_entry);
  g_signal_connect_object (resolution_entry, "destroy",
                           G_CALLBACK (gtk_widget_destroy),
                           dialog, G_CONNECT_SWAPPED);
  g_signal_connect_object (resolution_entry, "unmap",
                           G_CALLBACK (gtk_widget_destroy),
                           dialog, G_CONNECT_SWAPPED);

  SET_STYLE (dialog, dialog_style);
  gtk_container_set_border_width (GTK_CONTAINER (GTK_DIALOG (dialog)->action_area), 8);

  screen = gtk_widget_get_screen (dialog);

  ruler_width  = gdk_screen_get_width (screen);
  ruler_height = gdk_screen_get_height (screen);

  ruler_width  = ruler_width  - 300 - (ruler_width  % 100);
  ruler_height = ruler_height - 300 - (ruler_height % 100);

  table = gtk_table_new (4, 4, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (table), 8);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), table);
  gtk_widget_show (table);

  if (pixbuf)
    {
      GtkWidget *image = gtk_image_new_from_pixbuf (pixbuf);
      gtk_table_attach (GTK_TABLE (table), image, 0, 1, 0, 1,
                        GTK_SHRINK, GTK_SHRINK, 4, 4);
      gtk_widget_show (image);
    }

  ruler = gtk_hruler_new ();
  SET_STYLE (ruler, ruler_style);
  gtk_widget_set_size_request (ruler, ruler_width, 32);
  gtk_ruler_set_range (GTK_RULER (ruler), 0, ruler_width, 0, ruler_width);
  gtk_table_attach (GTK_TABLE (table), ruler, 1, 3, 0, 1,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (ruler);

  ruler = gtk_vruler_new ();
  SET_STYLE (ruler, ruler_style);
  gtk_widget_set_size_request (ruler, 32, ruler_height);
  gtk_ruler_set_range (GTK_RULER (ruler), 0, ruler_height, 0, ruler_height);
  gtk_table_attach (GTK_TABLE (table), ruler, 0, 1, 1, 3,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (ruler);

  ebox = gtk_event_box_new ();
  SET_STYLE (ebox, ruler_style);
  gtk_table_attach (GTK_TABLE (table), ebox, 1, 2, 1, 2,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (ebox);

  table = gtk_table_new (3, 3, FALSE);
  gtk_container_add (GTK_CONTAINER (ebox), table);
  gtk_widget_show (table);

  darea = gtk_drawing_area_new ();
  SET_STYLE (darea, dialog_style);
  gtk_widget_set_size_request (darea, 16, 16);
  if (expose_callback)
    g_signal_connect (darea, "expose_event",
                      G_CALLBACK (expose_callback),
                      GINT_TO_POINTER (GTK_CORNER_TOP_LEFT));
  gtk_table_attach (GTK_TABLE (table), darea, 0, 1, 0, 1,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (darea);

  darea = gtk_drawing_area_new ();
  SET_STYLE (darea, dialog_style);
  gtk_widget_set_size_request (darea, 16, 16);
  if (expose_callback)
    g_signal_connect (darea, "expose_event",
                      G_CALLBACK (expose_callback),
                      GINT_TO_POINTER (GTK_CORNER_BOTTOM_LEFT));
  gtk_table_attach (GTK_TABLE (table), darea, 0, 1, 2, 3,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (darea);

  darea = gtk_drawing_area_new ();
  SET_STYLE (darea, dialog_style);
  gtk_widget_set_size_request (darea, 16, 16);
  if (expose_callback)
    g_signal_connect (darea, "expose_event",
                      G_CALLBACK (expose_callback),
                      GINT_TO_POINTER (GTK_CORNER_TOP_RIGHT));
  gtk_table_attach (GTK_TABLE (table), darea, 2, 3, 0, 1,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (darea);

  darea = gtk_drawing_area_new ();
  SET_STYLE (darea, dialog_style);
  gtk_widget_set_size_request (darea, 16, 16);
  if (expose_callback)
    g_signal_connect (darea, "expose_event",
                      G_CALLBACK (expose_callback),
                      GINT_TO_POINTER (GTK_CORNER_BOTTOM_RIGHT));
  gtk_table_attach (GTK_TABLE (table), darea, 2, 3, 2, 3,
		    GTK_SHRINK, GTK_SHRINK, 0, 0);
  gtk_widget_show (darea);

  vbox = gtk_vbox_new (FALSE, 16);
  gtk_table_attach_defaults (GTK_TABLE (table), vbox, 1, 2, 1, 2);
  gtk_widget_show (vbox);

  label =
    gtk_label_new (_("Measure the rulers and enter their lengths below."));
  SET_STYLE (label, ruler_style);
  gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  calibrate_xres =
    gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (resolution_entry), 0);
  calibrate_yres =
    gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (resolution_entry), 1);

  calibrate_entry =
    gimp_coordinates_new  (GIMP_UNIT_INCH, "%p",
			   FALSE, FALSE, 10,
			   GIMP_SIZE_ENTRY_UPDATE_SIZE,
			   FALSE,
			   FALSE,
			   _("_Horizontal:"),
			   ruler_width,
			   calibrate_xres,
			   1, GIMP_MAX_IMAGE_SIZE,
			   0, 0,
			   _("_Vertical:"),
			   ruler_height,
			   calibrate_yres,
			   1, GIMP_MAX_IMAGE_SIZE,
			   0, 0);
  gtk_widget_hide (GTK_WIDGET (GIMP_COORDINATES_CHAINBUTTON (calibrate_entry)));
  g_signal_connect (dialog, "destroy",
                    G_CALLBACK (gtk_widget_destroyed),
                    &calibrate_entry);

  if (ruler_style)
    {
      for (list = GTK_TABLE (calibrate_entry)->children;
	   list;
	   list = g_list_next (list))
	{
	  GtkTableChild *child = (GtkTableChild *) list->data;

	  if (child && GTK_IS_LABEL (child->widget))
	    SET_STYLE (GTK_WIDGET (child->widget), ruler_style);
	}
    }
  gtk_box_pack_end (GTK_BOX (hbox), calibrate_entry, FALSE, FALSE, 0);
  gtk_widget_show (calibrate_entry);

  gtk_widget_show (dialog);

  gtk_main ();
}
