/* Lighting Effects - A plug-in for GIMP
 *
 * Dialog creation and updaters, callbacks and event-handlers
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

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <glib/gprintf.h>

#include "lighting_ui.h"
#include "lighting_main.h"
#include "lighting_image.h"
#include "lighting_apply.h"
#include "lighting_preview.h"
#include "lighting_stock.h"

#include "libgimp/stdplugins-intl.h"


extern LightingValues mapvals;

static GtkWidget   *appwin            = NULL;
static GtkNotebook *options_note_book = NULL;


GdkGC     *gc          = NULL;
GtkWidget *previewarea = NULL;

static GtkWidget *pointlightwid[NUM_LIGHTS];
static GtkWidget *dirlightwid[NUM_LIGHTS];
static GtkWidget *colorbutton[NUM_LIGHTS];
static GtkWidget *light_type_combo[NUM_LIGHTS];
static gchar     *lighting_effects_path       = NULL;


static void create_main_notebook      (GtkWidget      *container);

#ifdef _LIGHTNING_UNUSED_CODE
static void xyzval_update             (GtkEntry       *entry);
#endif

static void toggle_update             (GtkWidget      *widget,
                                       gpointer        data);

static void lightmenu_callback        (GtkWidget      *widget,
                                       gpointer        data);

static gboolean  bumpmap_constrain    (gint32          image_id,
                                       gint32          drawable_id,
                                       gpointer        data);
static gboolean  envmap_constrain     (gint32          image_id,
                                       gint32          drawable_id,
                                       gpointer        data);
static void     envmap_combo_callback (GtkWidget      *widget,
                                       gpointer        data);
static void     save_lighting_preset  (GtkWidget      *widget,
                                       gpointer        data);
static void     file_chooser_response (GtkFileChooser *chooser,
                                       gint            response_id,
                                       gpointer        data);
static void     load_lighting_preset  (GtkWidget      *widget,
                                       gpointer        data);
static void     load_file_chooser_response (GtkFileChooser *chooser,
                                            gint       response_id,
                                            gpointer   data);


#ifdef _LIGHTNING_UNUSED_CODE
/**********************************************************/
/* Update entry fields that affect the preview parameters */
/**********************************************************/

static void
xyzval_update (GtkEntry *entry)
{
  gdouble *valueptr;
  gdouble  value;

  valueptr = (gdouble *) g_object_get_data (G_OBJECT (entry), "ValuePtr");
  value = atof (gtk_entry_get_text (entry));

  *valueptr = value;
}
#endif


/**********************/
/* Std. toggle update */
/**********************/

static void
toggle_update (GtkWidget *widget,
               gpointer   data)
{
  gimp_toggle_button_update (widget, data);

  draw_preview_image (TRUE);
}


/*****************************************/
/* Main window light type menu callback. */
/*****************************************/

static void
lightmenu_callback (GtkWidget *widget,
                    gpointer   data)
{
  gint k;

  gimp_int_combo_box_get_active (GIMP_INT_COMBO_BOX (widget), (gint *) data);

  for (k = 0; k < NUM_LIGHTS; k++)
    {
      if (mapvals.lightsource[k].type == POINT_LIGHT)
        {
          gtk_widget_hide (dirlightwid[k]);
          gtk_widget_show (pointlightwid[k]);
        }
      else if (mapvals.lightsource[k].type == DIRECTIONAL_LIGHT)
        {
          gtk_widget_hide (pointlightwid[k]);
          gtk_widget_show (dirlightwid[k]);
        }
      else
        {
          gtk_widget_hide (pointlightwid[k]);
          gtk_widget_hide (dirlightwid[k]);
        }
    }

  interactive_preview_callback(NULL);
}

static void
mapmenu2_callback (GtkWidget *widget,
                   gpointer   data)
{
  gimp_int_combo_box_get_active (GIMP_INT_COMBO_BOX (widget), (gint *) data);

  draw_preview_image (TRUE);
}

/******************************************/
/* Main window "Preview!" button callback */
/******************************************/

static void
preview_callback (GtkWidget *widget)
{
  draw_preview_image (TRUE);
}




/*********************************************/
/* Main window "-" (zoom in) button callback */
/*********************************************/
/*
static void
zoomout_callback (GtkWidget *widget)
{
  mapvals.preview_zoom_factor *= 0.5;
  draw_preview_image (TRUE);
}
*/
/*********************************************/
/* Main window "+" (zoom out) button callback */
/*********************************************/
/*
static void
zoomin_callback (GtkWidget *widget)
{
  mapvals.preview_zoom_factor *= 2.0;
  draw_preview_image (TRUE);
}
*/
/**********************************************/
/* Main window "Apply" button callback.       */
/* Render to GIMP image, close down and exit. */
/**********************************************/

static gint
bumpmap_constrain (gint32   image_id,
                   gint32   drawable_id,
                   gpointer data)
{
  return  ((gimp_drawable_width (drawable_id) ==
            gimp_drawable_width (mapvals.drawable_id)) &&
           (gimp_drawable_height (drawable_id) ==
            gimp_drawable_height (mapvals.drawable_id)));
}

static gint
envmap_constrain (gint32   image_id,
                  gint32   drawable_id,
                  gpointer data)
{
  return (!gimp_drawable_is_gray (drawable_id) &&
          !gimp_drawable_has_alpha (drawable_id));
}

static void
envmap_combo_callback (GtkWidget *widget,
                       gpointer   data)
{
  gimp_int_combo_box_get_active (GIMP_INT_COMBO_BOX (widget),
                                 &mapvals.envmap_id);

  env_width = gimp_drawable_width   (mapvals.envmap_id);
  env_height = gimp_drawable_height (mapvals.envmap_id);
}

/***********************/
/* Dialog constructors */
/***********************/

static GtkWidget *
create_options_page (void)
{
  GtkWidget *page;
  GtkWidget *frame;
  GtkWidget *vbox;
  GtkWidget *toggle;

  page = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (page), 12);

  /* General options */

  frame = gimp_frame_new (_("General Options"));
  gtk_box_pack_start (GTK_BOX (page), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  toggle = gtk_check_button_new_with_mnemonic (_("T_ransparent background"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.transparent_background);
  gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (toggle_update),
                    &mapvals.transparent_background);
  gtk_widget_show (toggle);

  gimp_help_set_help_data (toggle,
                           _("Make destination image transparent where bump "
                             "height is zero"),NULL);

  toggle = gtk_check_button_new_with_mnemonic (_("Cre_ate new image"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.create_new_image);
  gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &mapvals.create_new_image);
  gtk_widget_show (toggle);

  gimp_help_set_help_data (toggle,
                           _("Create a new image when applying filter"), NULL);

  toggle = gtk_check_button_new_with_mnemonic (_("High _Quality preview"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.previewquality);
  gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (toggle_update),
                    &mapvals.previewquality);
  gtk_widget_show (toggle);

  gimp_help_set_help_data (toggle,
                           _("Enable/disable high quality preview"), NULL);


  gtk_widget_show (page);

  return page;
}

/******************************/
/* Create light settings page */
/******************************/

static GtkWidget *
create_light_page (void)
{
  GtkWidget *page;
  GtkWidget *frame;
  GtkWidget *table;
  GtkWidget *table2;
  GtkWidget *ebox;
  GtkWidget *button;
  GtkObject *adj;
  GtkWidget *label;
  gint       k;
  gchar      label_text[20];

  page = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (page), 12);

  frame = gimp_frame_new (_("Light Settings"));
  gtk_box_pack_start (GTK_BOX (page), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  table = gtk_table_new (8, 8, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  /* row labels */
  label = gtk_label_new (_("Type"));
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
  gtk_widget_show (label);

  label = gtk_label_new (_("Color"));
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
  gtk_widget_show (label);

  gtk_table_set_col_spacing (GTK_TABLE (table), 2, 12);
  gtk_table_set_col_spacing (GTK_TABLE (table), 4, 12);

  for (k = 0; k < NUM_LIGHTS; k++)
    {
      g_sprintf (label_text, "Light %d", k + 1);
      label = gtk_label_new (label_text);
      gtk_table_attach_defaults (GTK_TABLE (table), label, 2*k + 1, 2*k + 2, 0, 1);
      gtk_widget_show (label);

      light_type_combo[k] = gimp_int_combo_box_new (_("None"),        NO_LIGHT,
                                                    _("Directional"), DIRECTIONAL_LIGHT,
                                                    _("Point"),       POINT_LIGHT,
                                                    /* _("Spot"),     SPOT_LIGHT, */
                                                    NULL);
      gimp_int_combo_box_set_active (GIMP_INT_COMBO_BOX (light_type_combo[k]),
                                     mapvals.lightsource[k].type);
      g_signal_connect (light_type_combo[k], "changed",
                        G_CALLBACK (lightmenu_callback),
                        &mapvals.lightsource[k].type);
      ebox = gtk_event_box_new ();
      gtk_container_add (GTK_CONTAINER (ebox), light_type_combo[k]);
      gtk_widget_show (ebox);
      gtk_table_attach_defaults (GTK_TABLE (table), ebox, 2*k + 1, 2*k + 2, 1, 2);
      gtk_widget_show (light_type_combo[k]);
      gimp_help_set_help_data (ebox, _("Type of light source to apply"), NULL);
      
      colorbutton[k] = gimp_color_button_new (_("Select lightsource color"),
                                           64, 16,
                                           &mapvals.lightsource[k].color,
                                           GIMP_COLOR_AREA_FLAT);
      g_signal_connect (colorbutton[k], "color_changed",
                        G_CALLBACK (gimp_color_button_get_color),
                        &mapvals.lightsource[k].color);
      g_signal_connect (colorbutton[k], "color_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gtk_widget_show (colorbutton[k]);
      gtk_table_attach_defaults (GTK_TABLE (table), colorbutton[k], 2*k + 1, 2*k + 2, 2, 3);
      gimp_help_set_help_data (colorbutton[k],
                               _("Set light source color"), NULL);

      /* Position vector settings */
      pointlightwid[k] = gtk_vbox_new (FALSE, 0);
      if (mapvals.lightsource[k].type == POINT_LIGHT)
        gtk_widget_show (pointlightwid[k]);
      table2 = gtk_table_new (3, 2, FALSE);
      gtk_table_set_col_spacings (GTK_TABLE (table2), 6);
      gtk_table_set_row_spacings (GTK_TABLE (table2), 6);
      gtk_container_add (GTK_CONTAINER (pointlightwid[k]), table2);
      gtk_table_attach_defaults (GTK_TABLE (table), pointlightwid[k], 2*k + 1, 2*k + 2, 4, 5);
      gtk_widget_show (table2);

      spin_pos_x[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].position.x,
                                         -100.0, 100.0,
                                         0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 0,
                                 _("_X:"), 0.0, 0.5,
                                 spin_pos_x[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].position.x);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_pos_x[k],
                               _("Light source X position in XYZ space"), NULL);
      
      spin_pos_y[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].position.y,
                                            -100.0, 100.0,
                                            0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 1,
                                 _("_Y:"), 0.0, 0.5,
                                 spin_pos_y[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].position.y);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_pos_y[k],
                           _("Light source Y position in XYZ space"), NULL);

      spin_pos_z[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].position.z,
                                            -100.0, 100.0,
                                            0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 2,
                                 _("_Z:"), 0.0, 0.5,
                                 spin_pos_z[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].position.z);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_pos_z[k],
                               _("Light source Z position in XYZ space"), NULL);

      /* Direction vector settings */
      dirlightwid[k] = gtk_vbox_new (FALSE, 0);
      if (mapvals.lightsource[k].type == DIRECTIONAL_LIGHT)
        gtk_widget_show (dirlightwid[k]);
      table2 = gtk_table_new (3, 2, FALSE);
      gtk_widget_show (table2);
      gtk_table_set_col_spacings (GTK_TABLE (table2), 6);
      gtk_table_set_row_spacings (GTK_TABLE (table2), 6);
      gtk_container_add (GTK_CONTAINER (dirlightwid[k]), table2);
      gtk_table_attach_defaults (GTK_TABLE (table), dirlightwid[k], 2*k + 1, 2*k + 2, 4, 5);
      
      spin_dir_x[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].direction.x,
                                            -100.0, 100.0, 0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 0,
                                 _("X:"), 0.0, 0.5,
                                 spin_dir_x[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].direction.x);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_dir_x[k],
                               _("Light source X direction in XYZ space"), NULL);
      
      spin_dir_y[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].direction.y,
                                            -100.0, 100.0, 0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 1,
                                 _("Y:"), 0.0, 0.5,
                                 spin_dir_y[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].direction.y);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_dir_y[k],
                               _("Light source Y direction in XYZ space"), NULL);
      
      spin_dir_z[k] = gimp_spin_button_new (&adj, mapvals.lightsource[k].direction.z,
                                            -100.0, 100.0, 0.1, 1.0, 1.0, 0.0, 2);
      gimp_table_attach_aligned (GTK_TABLE (table2), 0, 2,
                                 _("Z:"), 0.0, 0.5,
                                 spin_dir_z[k], 1, TRUE);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (gimp_double_adjustment_update),
                        &mapvals.lightsource[k].direction.z);
      g_signal_connect (adj, "value_changed",
                        G_CALLBACK (interactive_preview_callback),
                        NULL);
      gimp_help_set_help_data (spin_dir_z[k],
                               _("Light source Z direction in XYZ space"), NULL);

      label = gtk_label_new (_("Lighting preset:"));
      gtk_table_set_row_spacing (GTK_TABLE (table), 4, 24);
      gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 3, 5, 6);
      gtk_widget_show (label);

      button = gtk_button_new_with_mnemonic (_("_Save"));
      gtk_table_attach_defaults (GTK_TABLE (table), button, 3, 4, 5, 6);
      g_signal_connect (button, "clicked",
                        G_CALLBACK (save_lighting_preset),
                        NULL);
      gtk_widget_show (button);

      button = gtk_button_new_with_mnemonic (_("_Load"));
      gtk_table_attach_defaults (GTK_TABLE (table), button, 5, 6, 5, 6);
      g_signal_connect (button, "clicked",
                        G_CALLBACK (load_lighting_preset),
                        NULL);
      gtk_widget_show (button);
    }

  gtk_widget_show (page);

  return page;
}

/*********************************/
/* Create material settings page */
/*********************************/

static GtkWidget *
create_material_page (void)
{
  GtkSizeGroup *group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
  GtkWidget    *page;
  GtkWidget    *frame;
  GtkWidget    *table;
  GtkWidget    *label;
  GtkWidget    *hbox;
  GtkWidget    *spinbutton;
  GtkWidget    *image;
  GtkWidget    *button;
  GtkObject    *adj;

  page = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (page), 12);

  frame = gimp_frame_new (_("Material properties"));
  gtk_box_pack_start (GTK_BOX (page), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  hbox = gtk_hbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (frame), hbox);
  gtk_widget_show (hbox);

  table = gtk_table_new (5, 4, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_box_pack_start (GTK_BOX (hbox), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  /* Ambient intensity */

  image = gtk_image_new_from_stock (STOCK_INTENSITY_AMBIENT_LOW,
                                    GTK_ICON_SIZE_BUTTON);
  label = gimp_table_attach_aligned (GTK_TABLE (table), 0, 0,
                                     _("_Glowing:"), 0.0, 0.5,
                                     image, 1, FALSE);
  gtk_size_group_add_widget (group, label);
  spinbutton = gimp_spin_button_new (&adj, mapvals.material.ambient_int,
                                     0, G_MAXFLOAT, 0.1, 1.0, 1.0, 0.0, 2);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 0, 1,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (gimp_double_adjustment_update),
                    &mapvals.material.ambient_int);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);
  gtk_widget_show (spinbutton);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gimp_help_set_help_data (spinbutton,
                           _("Amount of original color to show where no "
                             "direct light falls"), NULL);
  image = gtk_image_new_from_stock (STOCK_INTENSITY_AMBIENT_HIGH,
                                    GTK_ICON_SIZE_BUTTON);
  gtk_table_attach (GTK_TABLE (table), image, 3, 4, 0, 1,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (image);

  /* Diffuse intensity */

  image = gtk_image_new_from_stock (STOCK_INTENSITY_DIFFUSE_LOW,
                                    GTK_ICON_SIZE_BUTTON);
  label = gimp_table_attach_aligned (GTK_TABLE (table), 0, 1,
                                     _("_Bright:"), 0.0, 0.5,
                                     image, 1, FALSE);
  gtk_size_group_add_widget (group, label);
  spinbutton = gimp_spin_button_new (&adj, mapvals.material.diffuse_int,
                                     0, G_MAXFLOAT, 0.1, 1.0, 1.0, 0.0, 2);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 1, 2,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (gimp_double_adjustment_update),
                    &mapvals.material.diffuse_int);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);
  gtk_widget_show (spinbutton);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gimp_help_set_help_data (spinbutton,
                           _("Intensity of original color when lit by a light "
                             "source"), NULL);
  image = gtk_image_new_from_stock (STOCK_INTENSITY_DIFFUSE_HIGH,
                                    GTK_ICON_SIZE_BUTTON);
  gtk_table_attach (GTK_TABLE (table), image, 3, 4, 1, 2,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (image);

  /* Specular reflection */

  image = gtk_image_new_from_stock (STOCK_REFLECTIVITY_SPECULAR_LOW,
                                    GTK_ICON_SIZE_BUTTON);
  label = gimp_table_attach_aligned (GTK_TABLE (table), 0, 2,
                                     _("_Shiny:"), 0.0, 0.5,
                                     image, 1, FALSE);
  gtk_size_group_add_widget (group, label);
  spinbutton = gimp_spin_button_new (&adj, mapvals.material.specular_ref,
                                     0, G_MAXFLOAT, 0.1, 1.0, 1.0, 0.0, 2);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 2, 3,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (gimp_double_adjustment_update),
                    &mapvals.material.specular_ref);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);
  gtk_widget_show (spinbutton);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gimp_help_set_help_data (spinbutton,
                           _("Controls how intense the highlights will be"),
                           NULL);
  image = gtk_image_new_from_stock (STOCK_REFLECTIVITY_SPECULAR_HIGH,
                                    GTK_ICON_SIZE_BUTTON);
  gtk_table_attach (GTK_TABLE (table), image, 3, 4, 2, 3,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (image);

  /* Highlight */
  image = gtk_image_new_from_stock (STOCK_REFLECTIVITY_HIGHLIGHT_LOW,
                                    GTK_ICON_SIZE_BUTTON);
  label = gimp_table_attach_aligned (GTK_TABLE (table), 0, 3,
                                     _("_Polished:"), 0.0, 0.5,
                                     image, 1, FALSE);
  gtk_size_group_add_widget (group, label);
  spinbutton = gimp_spin_button_new (&adj, mapvals.material.highlight,
                                     0, G_MAXFLOAT, 0.1, 1.0, 1.0, 0.0, 2);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 3, 4,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (gimp_double_adjustment_update),
                    &mapvals.material.highlight);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);
  gtk_widget_show (spinbutton);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gimp_help_set_help_data (spinbutton,
                           _("Higher values makes the highlights more focused"),
                           NULL);
  image = gtk_image_new_from_stock (STOCK_REFLECTIVITY_HIGHLIGHT_HIGH,
                                    GTK_ICON_SIZE_BUTTON);
  gtk_table_attach (GTK_TABLE (table), image, 3, 4, 3, 4,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (image);

  /* Metallic */
  button = gtk_check_button_new_with_mnemonic (_("_Metallic"));
  gtk_table_attach (GTK_TABLE (table), button, 0, 3, 4, 5,
                    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  g_signal_connect (button, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &mapvals.material.metallic);
  g_signal_connect (button, "toggled",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);
  gtk_widget_show (button);

  gtk_widget_show (page);

  return page;
}

/* Create Bump mapping page */

static GtkWidget *
create_bump_page (void)
{
  GtkWidget *page;
  GtkWidget *toggle;
  GtkWidget *frame;
  GtkWidget *table;
  GtkWidget *combo;
  GtkWidget *spinbutton;
  GtkObject *adj;

  page = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (page), 12);

  frame = gimp_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (page), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  toggle = gtk_check_button_new_with_mnemonic (_("E_nable bump mapping"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.bump_mapped);
  gtk_frame_set_label_widget (GTK_FRAME (frame), toggle);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &mapvals.bump_mapped);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);

  gimp_help_set_help_data (toggle,
                           _("Enable/disable bump-mapping (image depth)"),
                           NULL);

  table = gtk_table_new (6, 2, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  gtk_widget_set_sensitive (table, mapvals.bump_mapped);
  g_object_set_data (G_OBJECT (toggle), "set_sensitive", table);

  combo = gimp_drawable_combo_box_new (bumpmap_constrain, NULL);
  gimp_int_combo_box_connect (GIMP_INT_COMBO_BOX (combo), mapvals.bumpmap_id,
                              G_CALLBACK (gimp_int_combo_box_get_active),
                              &mapvals.bumpmap_id);

  gimp_table_attach_aligned (GTK_TABLE (table), 0, 0,
                             _("Bumpm_ap image:"), 0.0, 0.5,
                             combo, 1, FALSE);

  combo = gimp_int_combo_box_new (_("Linear"),      LINEAR_MAP,
                                  _("Logarithmic"), LOGARITHMIC_MAP,
                                  _("Sinusoidal"),  SINUSOIDAL_MAP,
                                  _("Spherical"),   SPHERICAL_MAP,
                                  NULL);
  gimp_int_combo_box_set_active (GIMP_INT_COMBO_BOX (combo),
                                 mapvals.bumpmaptype);

  g_signal_connect (combo, "changed",
                    G_CALLBACK (mapmenu2_callback),
                    &mapvals.bumpmaptype);

  gimp_table_attach_aligned (GTK_TABLE (table), 0, 1,
                             _("Cu_rve:"), 0.0, 0.5, combo, 1, FALSE);

  spinbutton = gimp_spin_button_new (&adj, mapvals.bumpmax,
                                     0, G_MAXFLOAT, 0.01, 0.1, 1.0, 0.0, 2);
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 2,
                             _("Ma_ximum height:"), 0.0, 0.5,
                             spinbutton, 1, TRUE);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (gimp_double_adjustment_update),
                    &mapvals.bumpmax);
  g_signal_connect (adj, "value_changed",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);

  gimp_help_set_help_data (spinbutton,
                           _("Maximum height for bumps"),
                           NULL);

  gtk_widget_show (page);

  return page;
}

static GtkWidget *
create_environment_page (void)
{
  GtkWidget *page;
  GtkWidget *toggle;
  GtkWidget *table;
  GtkWidget *frame;
  GtkWidget *combo;
  GtkWidget *ebox;

  page = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (page), 12);

  frame = gimp_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (page), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  toggle = gtk_check_button_new_with_mnemonic (_("E_nable environment mapping"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.env_mapped);
  gtk_frame_set_label_widget (GTK_FRAME (frame), toggle);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &mapvals.env_mapped);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);

  gimp_help_set_help_data (toggle,
                           _("Enable/disable environment-mapping (reflection)"),
                           NULL);

  table = gtk_table_new (3, 2, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  gtk_widget_set_sensitive (table, mapvals.env_mapped);
  g_object_set_data (G_OBJECT (toggle), "set_sensitive", table);

  combo = gimp_drawable_combo_box_new (envmap_constrain, NULL);
  gimp_int_combo_box_connect (GIMP_INT_COMBO_BOX (combo), mapvals.envmap_id,
                              G_CALLBACK (envmap_combo_callback),
                              NULL);

  ebox = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (ebox), combo);
  gtk_widget_show (combo);

  gimp_table_attach_aligned (GTK_TABLE (table), 0, 0,
                             _("En_vironment image:"), 0.0, 0.5,
                             ebox, 1, FALSE);
  gimp_help_set_help_data (ebox, _("Environment image to use"), NULL);

  gtk_widget_show (page);

  return page;
}

/*****************************/
/* Create notebook and pages */
/*****************************/

static void
create_main_notebook (GtkWidget *container)
{
  GtkWidget *page;

  options_note_book = GTK_NOTEBOOK (gtk_notebook_new ());
  gtk_container_add (GTK_CONTAINER (container),
                     GTK_WIDGET (options_note_book));

  page = create_options_page ();
  gtk_notebook_append_page (options_note_book, page,
                            gtk_label_new_with_mnemonic (_("Op_tions")));

  page = create_light_page ();
  gtk_notebook_append_page (options_note_book, page,
                            gtk_label_new_with_mnemonic (_("_Light")));

  page = create_material_page ();
  gtk_notebook_append_page (options_note_book, page,
                            gtk_label_new_with_mnemonic (_("_Material")));

  page = create_bump_page ();
  gtk_notebook_append_page (options_note_book, page,
                            gtk_label_new_with_mnemonic (_("_Bump Map")));

  page = create_environment_page ();
  gtk_notebook_append_page (options_note_book, page,
                            gtk_label_new_with_mnemonic (_("_Environment Map")));

  /*
  if (mapvals.bump_mapped == TRUE)
    {
      bump_page = create_bump_page ();
      bump_page_pos = g_list_length (options_note_book->children);
      gtk_notebook_append_page (options_note_book, bump_page,
                                gtk_label_new (_("Bumpmap")));
    }

  if (mapvals.env_mapped == TRUE)
    {
      env_page = create_environment_page ();
      env_page_pos = g_list_length (options_note_book->children);
      gtk_notebook_append_page (options_note_book, env_page,
                                gtk_label_new (_("Environment")));
    }
  */
  gtk_widget_show (GTK_WIDGET (options_note_book));
}

/********************************/
/* Create and show main dialog. */
/********************************/

gboolean
main_dialog (GimpDrawable *drawable)
{
  GtkWidget *main_hbox;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *toggle;
  gboolean   run = FALSE;

  /*
  GtkWidget *image;
  */

  gimp_ui_init ("Lighting", FALSE);
  lighting_effects_path = gimp_gimprc_query ("lighting-effects-path");

  lighting_stock_init ();

  appwin = gimp_dialog_new (_("Lighting Effects"), "Lighting",
                            NULL, 0,
                            gimp_standard_help_func, "plug-in-lighting",

                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,

                            NULL);

  main_hbox = gtk_hbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_hbox), 12);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (appwin)->vbox), main_hbox,
                      FALSE, FALSE, 0);
  gtk_widget_show (main_hbox);

  /* Create the Preview */
  /* ================== */

  vbox = gtk_vbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (main_hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show (vbox);

  /* Add preview widget and various buttons to the first part */
  /* ======================================================== */

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  gtk_widget_realize (appwin);

  previewarea = gtk_drawing_area_new ();
  gtk_widget_set_size_request (previewarea, PREVIEW_WIDTH, PREVIEW_HEIGHT);
  gtk_widget_set_events (previewarea, (GDK_EXPOSURE_MASK |
                                       GDK_BUTTON1_MOTION_MASK |
                                       GDK_BUTTON_PRESS_MASK |
                                       GDK_BUTTON_RELEASE_MASK));
  g_signal_connect (previewarea, "event",
                    G_CALLBACK (preview_events),
                    previewarea);
  gtk_container_add (GTK_CONTAINER (frame), previewarea);
  gtk_widget_show (previewarea);

  /* create preview options, frame and vbox */
  hbox = gtk_hbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  button = gtk_button_new_with_mnemonic (_("_Update"));
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  g_signal_connect (button, "clicked",
                    G_CALLBACK (preview_callback),
                    NULL);
  gtk_widget_show (button);

  gimp_help_set_help_data (button, _("Recompute preview image"), NULL);

  toggle = gtk_check_button_new_with_mnemonic (_("I_nteractive"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                mapvals.interactive_preview);
  gtk_box_pack_start (GTK_BOX (hbox), toggle, TRUE, TRUE, 0);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &mapvals.interactive_preview);
  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (interactive_preview_callback),
                    NULL);


  gtk_widget_show (toggle);

  gimp_help_set_help_data (toggle,
                           _("Enable/disable real time preview of changes"),
                           NULL);

  create_main_notebook (main_hbox);

  gtk_widget_show (appwin);

  {
    GdkCursor *cursor;

    cursor = gdk_cursor_new_for_display (gtk_widget_get_display (previewarea),
                                         GDK_HAND2);
    gdk_window_set_cursor (previewarea->window, cursor);
    gdk_cursor_unref (cursor);
  }

  image_setup (drawable, TRUE);

  if (gimp_dialog_run (GIMP_DIALOG (appwin)) == GTK_RESPONSE_OK)
    run = TRUE;

  if (preview_rgb_data != NULL)
    g_free (preview_rgb_data);

  gtk_widget_destroy (appwin);

  return run;
}


static void
save_lighting_preset (GtkWidget *widget,
                      gpointer   data)
{
  static GtkWidget *window = NULL;

  if (! window)
    {
      window =
        gtk_file_chooser_dialog_new (_("Save Lighting Preset"),
                                     GTK_WINDOW (appwin),
                                     GTK_FILE_CHOOSER_ACTION_SAVE,

                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                     GTK_STOCK_SAVE,   GTK_RESPONSE_OK,

                                     NULL);

      g_signal_connect (window, "destroy",
                        G_CALLBACK (gtk_widget_destroyed),
                        &window);
      g_signal_connect (window, "response",
                        G_CALLBACK (file_chooser_response),
                        NULL);
    }

  if (lighting_effects_path)
    {
      GList *list;
      gchar *dir;

      list = gimp_path_parse (lighting_effects_path, 16, FALSE, 0);
      dir = gimp_path_get_user_writable_dir (list);
      gimp_path_free (list);

      if (! dir)
        dir = g_strdup (gimp_directory ());

      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), dir);

      g_free (dir);
    }
  else
    {
      const gchar *tmp = g_get_tmp_dir ();

      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), tmp);
    }


  gtk_window_present (GTK_WINDOW (window));
}


static void
file_chooser_response (GtkFileChooser *chooser,
                       gint            response_id,
                       gpointer        data)
{
  FILE          *fp;
  gint           num_lights = 0;
  gint           k;
  LightSettings *source;
  gchar          buffer1[G_ASCII_DTOSTR_BUF_SIZE];
  gchar          buffer2[G_ASCII_DTOSTR_BUF_SIZE];
  gchar          buffer3[G_ASCII_DTOSTR_BUF_SIZE];
  gint           blen       = G_ASCII_DTOSTR_BUF_SIZE;

  if (response_id == GTK_RESPONSE_OK)
    {
      gchar   *filename;

      filename = gtk_file_chooser_get_filename (chooser);

      fp = fopen (filename, "w");
      if (!fp)
          g_message ("Cannot open file '%s' for saving", filename);
      else
        {
          for (k = 0; k < NUM_LIGHTS; k++)
            if (mapvals.lightsource[k].type != NO_LIGHT)
              ++num_lights;

          fprintf (fp, "Number of lights: %d\n", num_lights);

          for (k = 0; k < NUM_LIGHTS; k++)
            if (mapvals.lightsource[k].type != NO_LIGHT)
              {
                source = &mapvals.lightsource[k];

                switch (source->type)
                  {
                  case POINT_LIGHT:
                    fprintf (fp, "Type: Point\n");
                    break;
                  case DIRECTIONAL_LIGHT:
                    fprintf (fp, "Type: Directional\n");
                    break;
                  case SPOT_LIGHT:
                    fprintf (fp, "Type: Spot\n");
                    break;
                  default:
                    g_message ("Unknown light type: %d", mapvals.lightsource[k].type);
                    continue;
                  }

                fprintf (fp, "Position: %s %s %s\n",
                         g_ascii_dtostr (buffer1, blen, source->position.x),
                         g_ascii_dtostr (buffer2, blen, source->position.y),
                         g_ascii_dtostr (buffer3, blen, source->position.z));

                fprintf (fp, "Direction: %s %s %s\n",
                         g_ascii_dtostr (buffer1, blen, source->direction.x),
                         g_ascii_dtostr (buffer2, blen, source->direction.y),
                         g_ascii_dtostr (buffer3, blen, source->direction.z));

                fprintf (fp, "Color: %s %s %s\n",
                         g_ascii_dtostr (buffer1, blen, source->color.r),
                         g_ascii_dtostr (buffer2, blen, source->color.g),
                         g_ascii_dtostr (buffer3, blen, source->color.b));

                fprintf (fp, "Intensity: %s\n", 
                         g_ascii_dtostr (buffer1, blen, source->intensity));
              }
          
          fclose (fp);
        }
    }

  gtk_widget_destroy (GTK_WIDGET (chooser));
}

static void
load_lighting_preset (GtkWidget *widget,
                      gpointer   data)
{
  static GtkWidget *window = NULL;

  if (! window)
    {
      window =
        gtk_file_chooser_dialog_new (_("Load Lighting Preset"),
                                     GTK_WINDOW (appwin),
                                     GTK_FILE_CHOOSER_ACTION_OPEN,

                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                     GTK_STOCK_OPEN,   GTK_RESPONSE_OK,

                                     NULL);

      gtk_dialog_set_default_response (GTK_DIALOG (window), GTK_RESPONSE_OK);

      g_signal_connect (window, "destroy",
                        G_CALLBACK (gtk_widget_destroyed),
                        &window);
      g_signal_connect (window, "response",
                        G_CALLBACK (load_file_chooser_response),
                        NULL);
    }

  if (lighting_effects_path)
    {
      GList *list;
      gchar *dir;

      list = gimp_path_parse (lighting_effects_path, 16, FALSE, 0);
      dir = gimp_path_get_user_writable_dir (list);
      gimp_path_free (list);

      if (! dir)
        dir = g_strdup (gimp_directory ());

      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), dir);

      g_free (dir);
    }
  else
    {
      const gchar *tmp = g_get_tmp_dir ();

      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), tmp);
    }


  gtk_window_present (GTK_WINDOW (window));
}


static void
load_file_chooser_response (GtkFileChooser *chooser,
                            gint            response_id,
                            gpointer        data)
{
  FILE          *fp;
  gint           num_lights;
  gint           k;
  LightSettings *source;
  gchar          buffer1[G_ASCII_DTOSTR_BUF_SIZE];
  gchar          buffer2[G_ASCII_DTOSTR_BUF_SIZE];
  gchar          buffer3[G_ASCII_DTOSTR_BUF_SIZE];
  gchar          type_label[20];
  gchar         *endptr;


  if (response_id == GTK_RESPONSE_OK)
    {
      gchar   *filename;

      filename = gtk_file_chooser_get_filename (chooser);

      fp = fopen (filename, "r");
      if (!fp)
          g_message ("Cannot open file '%s' for reading", filename);
      else
        {
          fscanf (fp, "Number of lights: %d", &num_lights);

          for (k = 0; k < num_lights; k++)
            {
              source = &mapvals.lightsource[k];

              fscanf (fp, " Type: %s", type_label);

              if (!strcmp (type_label, "Point"))
                source->type = POINT_LIGHT;
              else if (!strcmp (type_label, "Directional"))
                source->type = DIRECTIONAL_LIGHT;
              else if (!strcmp (type_label, "Spot"))
                source->type = SPOT_LIGHT;
              else
                {
                  g_message ("Unknown light type: %s", type_label);
                  fclose (fp);
                  return;
                }

              fscanf (fp, " Position: %s %s %s", buffer1, buffer2, buffer3);
              source->position.x = g_ascii_strtod (buffer1, &endptr);
              source->position.y = g_ascii_strtod (buffer2, &endptr);
              source->position.z = g_ascii_strtod (buffer3, &endptr);

              fscanf (fp, " Direction: %s %s %s", buffer1, buffer2, buffer3);
              source->direction.x = g_ascii_strtod (buffer1, &endptr);
              source->direction.y = g_ascii_strtod (buffer2, &endptr);
              source->direction.z = g_ascii_strtod (buffer3, &endptr);

              fscanf (fp, " Color: %s %s %s", buffer1, buffer2, buffer3);
              source->color.r = g_ascii_strtod (buffer1, &endptr);
              source->color.g = g_ascii_strtod (buffer2, &endptr);
              source->color.b = g_ascii_strtod (buffer3, &endptr);
              source->color.a = 1.0;

              fscanf (fp, " Intensity: %s", buffer1);
              source->intensity = g_ascii_strtod (buffer1, &endptr);

              gimp_int_combo_box_set_active (GIMP_INT_COMBO_BOX (light_type_combo[k]),
                                             source->type);
              gimp_color_button_set_color (GIMP_COLOR_BUTTON (colorbutton[k]),
                                           &source->color);
              }

          fclose (fp);
        }
    }
  
  gtk_widget_destroy (GTK_WIDGET (chooser));
  interactive_preview_callback (GTK_WIDGET (chooser));
}

