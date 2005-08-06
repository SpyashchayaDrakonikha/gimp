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

#include "libgimpconfig/gimpconfig.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "base/siox.h"

#include "core/gimptoolinfo.h"

#include "gimpforegroundselectoptions.h"
#include "gimptooloptions-gui.h"

#include "gimp-intl.h"


enum
{
  PROP_0,
  PROP_EXPANDED,
  PROP_BACKGROUND,
  PROP_STROKE_WIDTH,
  PROP_SMOOTHNESS,
  PROP_GRANULARITY_L,
  PROP_GRANULARITY_A,
  PROP_GRANULARITY_B
};


static void   gimp_foreground_select_options_class_init   (GimpForegroundSelectOptionsClass *klass);

static void   gimp_foreground_select_options_set_property (GObject         *object,
                                                           guint            property_id,
                                                           const GValue    *value,
                                                           GParamSpec      *pspec);
static void   gimp_foreground_select_options_get_property (GObject         *object,
                                                           guint            property_id,
                                                           GValue          *value,
                                                           GParamSpec      *pspec);


GType
gimp_foreground_select_options_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo info =
      {
        sizeof (GimpForegroundSelectOptionsClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gimp_foreground_select_options_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data     */
        sizeof (GimpForegroundSelectOptions),
        0,              /* n_preallocs    */
        (GInstanceInitFunc) NULL
      };

      type = g_type_register_static (GIMP_TYPE_SELECTION_OPTIONS,
                                     "GimpForegroundSelectOptions",
                                     &info, 0);
    }

  return type;
}

static void
gimp_foreground_select_options_class_init (GimpForegroundSelectOptionsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gimp_foreground_select_options_set_property;
  object_class->get_property = gimp_foreground_select_options_get_property;

  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_EXPANDED,
                                    "expanded", NULL,
                                    FALSE,
                                    0);
  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_BACKGROUND,
                                    "background", NULL,
                                    FALSE,
                                    0);
  GIMP_CONFIG_INSTALL_PROP_INT (object_class, PROP_STROKE_WIDTH,
                                "stroke-width",
                                _("Size of the brush used for refinements"),
                                1, 80, 24,
                                0);
  GIMP_CONFIG_INSTALL_PROP_INT (object_class, PROP_SMOOTHNESS,
                                "smoothness",
                                _("Smaller values give a more accurate "
                                  "selection border but may introduce holes "
                                  "in the selection"),
                                0, 8, SIOX_DEFAULT_SMOOTHNESS,
                                0);
  GIMP_CONFIG_INSTALL_PROP_DOUBLE (object_class, PROP_GRANULARITY_L,
                                   "granularity-l",
                                   _("Resolution for brightness component"),
                                   0.0, 10.0, SIOX_DEFAULT_GRANULARITY_L,
                                   0);
  GIMP_CONFIG_INSTALL_PROP_DOUBLE (object_class, PROP_GRANULARITY_A,
                                   "granularity-a",
                                   _("Resolution for red/green component"),
                                   0.0, 10.0, SIOX_DEFAULT_GRANULARITY_A,
                                   0);
  GIMP_CONFIG_INSTALL_PROP_DOUBLE (object_class, PROP_GRANULARITY_B,
                                   "granularity-b",
                                   _("Resolution for yellow/blue component"),
                                   0.0, 10.0, SIOX_DEFAULT_GRANULARITY_B,
                                   0);
}

static void
gimp_foreground_select_options_set_property (GObject      *object,
                                             guint         property_id,
                                             const GValue *value,
                                             GParamSpec   *pspec)
{
  GimpForegroundSelectOptions *options = GIMP_FOREGROUND_SELECT_OPTIONS (object);

  switch (property_id)
    {
    case PROP_EXPANDED:
      options->expanded = g_value_get_boolean (value);
      break;
    case PROP_BACKGROUND:
      options->background = g_value_get_boolean (value);
      break;
    case PROP_STROKE_WIDTH:
      options->stroke_width = g_value_get_int (value);
      break;
    case PROP_SMOOTHNESS:
      options->smoothness = g_value_get_int (value);
      break;
    case PROP_GRANULARITY_L:
      options->limits[0] = g_value_get_double (value);
      break;
    case PROP_GRANULARITY_A:
      options->limits[1] = g_value_get_double (value);
      break;
    case PROP_GRANULARITY_B:
      options->limits[2] = g_value_get_double (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_foreground_select_options_get_property (GObject    *object,
                                             guint       property_id,
                                             GValue     *value,
                                             GParamSpec *pspec)
{
  GimpForegroundSelectOptions *options = GIMP_FOREGROUND_SELECT_OPTIONS (object);

  switch (property_id)
    {
    case PROP_EXPANDED:
      g_value_set_boolean (value, options->expanded);
      break;
    case PROP_BACKGROUND:
      g_value_set_boolean (value, options->background);
      break;
    case PROP_STROKE_WIDTH:
      g_value_set_int (value, options->stroke_width);
      break;
    case PROP_SMOOTHNESS:
      g_value_set_int (value, options->smoothness);
      break;
    case PROP_GRANULARITY_L:
      g_value_set_double (value, options->limits[0]);
      break;
    case PROP_GRANULARITY_A:
      g_value_set_double (value, options->limits[1]);
      break;
    case PROP_GRANULARITY_B:
      g_value_set_double (value, options->limits[2]);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

GtkWidget *
gimp_foreground_select_options_gui (GimpToolOptions *tool_options)
{
  GObject   *config = G_OBJECT (tool_options);
  GtkWidget *vbox   = gimp_selection_options_gui (tool_options);
  GtkWidget *hbox;
  GtkWidget *frame;
  GtkWidget *scale;
  GtkWidget *label;
  GtkWidget *inner_frame;
  GtkWidget *table;
  GtkObject *adj;
  gint       row = 0;

  /*  foreground / background  */
  frame = gimp_prop_boolean_radio_frame_new (config, "background",
                                             _("Interactive refinement"),
                                             _("Mark background"),
                                             _("Mark foreground"));

  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  /*  stroke width  */
  inner_frame = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (GTK_BIN (frame)->child),
                      inner_frame, FALSE, FALSE, 2);
  gtk_widget_show (inner_frame);

  hbox = gtk_hbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (inner_frame), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  label = gtk_label_new (_("Small brush"));
  gimp_label_set_attributes (GTK_LABEL (label),
                             PANGO_ATTR_STYLE, PANGO_STYLE_ITALIC,
                             PANGO_ATTR_SCALE,  PANGO_SCALE_SMALL,
                             -1);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  label = gtk_label_new (_("Large brush"));
  gimp_label_set_attributes (GTK_LABEL (label),
                             PANGO_ATTR_STYLE, PANGO_STYLE_ITALIC,
                             PANGO_ATTR_SCALE,  PANGO_SCALE_SMALL,
                             -1);
  gtk_box_pack_end (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  scale = gimp_prop_hscale_new (config, "stroke-width", 1.0, 5.0, 0);
  gtk_scale_set_draw_value (GTK_SCALE (scale), FALSE);
  gtk_box_pack_start (GTK_BOX (inner_frame), scale, FALSE, FALSE, 0);
  gtk_widget_show (scale);

  /*  smoothness  */
  table = gtk_table_new (1, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 2);
  gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  scale = gimp_prop_hscale_new (config, "smoothness", 0.1, 1.0, 0);
  gtk_range_set_update_policy (GTK_RANGE (scale), GTK_UPDATE_DELAYED);
  gtk_scale_set_value_pos (GTK_SCALE (scale), GTK_POS_RIGHT);
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 0,
                             _("Smoothing:"), 0.0, 0.5, scale, 2, FALSE);

  /*  granularity  */
  frame = gimp_prop_expander_new (config, "expanded", _("Granularity"));
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  inner_frame = gimp_frame_new ("<expander>");
  gtk_container_add (GTK_CONTAINER (frame), inner_frame);
  gtk_widget_show (inner_frame);

  table = gtk_table_new (3, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 2);
  gtk_table_set_row_spacings (GTK_TABLE (table), 2);
  gtk_container_add (GTK_CONTAINER (inner_frame), table);
  gtk_widget_show (table);

  adj = gimp_prop_opacity_entry_new (config, "granularity-l",
                                     GTK_TABLE (table), 0, row++, "L");
  gtk_range_set_update_policy (GTK_RANGE (GIMP_SCALE_ENTRY_SCALE (adj)),
                               GTK_UPDATE_DELAYED);

  adj = gimp_prop_opacity_entry_new (config, "granularity-a",
                                     GTK_TABLE (table), 0, row++, "a");
  gtk_range_set_update_policy (GTK_RANGE (GIMP_SCALE_ENTRY_SCALE (adj)),
                               GTK_UPDATE_DELAYED);

  adj = gimp_prop_opacity_entry_new (config, "granularity-b",
                                     GTK_TABLE (table), 0, row++, "b");
  gtk_range_set_update_policy (GTK_RANGE (GIMP_SCALE_ENTRY_SCALE (adj)),
                               GTK_UPDATE_DELAYED);

  return vbox;
}
