/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"

#include <glib-object.h>

#include "pdb-types.h"

#include "core/gimp.h"

#include "gimp-intl.h"

/* Forward declarations for registering PDB procs */

void register_brush_procs           (Gimp *gimp);
void register_brush_select_procs    (Gimp *gimp);
void register_brushes_procs         (Gimp *gimp);
void register_channel_procs         (Gimp *gimp);
void register_color_procs           (Gimp *gimp);
void register_context_procs         (Gimp *gimp);
void register_convert_procs         (Gimp *gimp);
void register_display_procs         (Gimp *gimp);
void register_drawable_procs        (Gimp *gimp);
void register_edit_procs            (Gimp *gimp);
void register_fileops_procs         (Gimp *gimp);
void register_floating_sel_procs    (Gimp *gimp);
void register_font_select_procs     (Gimp *gimp);
void register_fonts_procs           (Gimp *gimp);
void register_gimprc_procs          (Gimp *gimp);
void register_gradient_procs        (Gimp *gimp);
void register_gradient_select_procs (Gimp *gimp);
void register_gradients_procs       (Gimp *gimp);
void register_guides_procs          (Gimp *gimp);
void register_help_procs            (Gimp *gimp);
void register_image_procs           (Gimp *gimp);
void register_layer_procs           (Gimp *gimp);
void register_message_procs         (Gimp *gimp);
void register_misc_procs            (Gimp *gimp);
void register_paint_tools_procs     (Gimp *gimp);
void register_palette_procs         (Gimp *gimp);
void register_palette_select_procs  (Gimp *gimp);
void register_palettes_procs        (Gimp *gimp);
void register_parasite_procs        (Gimp *gimp);
void register_paths_procs           (Gimp *gimp);
void register_pattern_procs         (Gimp *gimp);
void register_pattern_select_procs  (Gimp *gimp);
void register_patterns_procs        (Gimp *gimp);
void register_plug_in_procs         (Gimp *gimp);
void register_procedural_db_procs   (Gimp *gimp);
void register_progress_procs        (Gimp *gimp);
void register_selection_procs       (Gimp *gimp);
void register_selection_tools_procs (Gimp *gimp);
void register_text_tool_procs       (Gimp *gimp);
void register_transform_tools_procs (Gimp *gimp);
void register_undo_procs            (Gimp *gimp);
void register_unit_procs            (Gimp *gimp);

/* 413 procedures registered total */

void
internal_procs_init (Gimp               *gimp,
                     GimpInitStatusFunc  status_callback)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (status_callback != NULL);

  (* status_callback) (_("Internal Procedures"), _("Brush"), 0.0);
  register_brush_procs (gimp);

  (* status_callback) (NULL, _("Brush UI"), 0.019);
  register_brush_select_procs (gimp);

  (* status_callback) (NULL, _("Brushes"), 0.027);
  register_brushes_procs (gimp);

  (* status_callback) (NULL, _("Channel"), 0.041);
  register_channel_procs (gimp);

  (* status_callback) (NULL, _("Color"), 0.063);
  register_color_procs (gimp);

  (* status_callback) (NULL, _("Context"), 0.097);
  register_context_procs (gimp);

  (* status_callback) (NULL, _("Convert"), 0.15);
  register_convert_procs (gimp);

  (* status_callback) (NULL, _("Display procedures"), 0.157);
  register_display_procs (gimp);

  (* status_callback) (NULL, _("Drawable procedures"), 0.167);
  register_drawable_procs (gimp);

  (* status_callback) (NULL, _("Edit procedures"), 0.247);
  register_edit_procs (gimp);

  (* status_callback) (NULL, _("File Operations"), 0.266);
  register_fileops_procs (gimp);

  (* status_callback) (NULL, _("Floating selections"), 0.288);
  register_floating_sel_procs (gimp);

  (* status_callback) (NULL, _("Font UI"), 0.303);
  register_font_select_procs (gimp);

  (* status_callback) (NULL, _("Fonts"), 0.31);
  register_fonts_procs (gimp);

  (* status_callback) (NULL, _("Gimprc procedures"), 0.315);
  register_gimprc_procs (gimp);

  (* status_callback) (NULL, _("Gradient"), 0.329);
  register_gradient_procs (gimp);

  (* status_callback) (NULL, _("Gradient UI"), 0.4);
  register_gradient_select_procs (gimp);

  (* status_callback) (NULL, _("Gradients"), 0.407);
  register_gradients_procs (gimp);

  (* status_callback) (NULL, _("Guide procedures"), 0.419);
  register_guides_procs (gimp);

  (* status_callback) (NULL, _("Help procedures"), 0.433);
  register_help_procs (gimp);

  (* status_callback) (NULL, _("Image"), 0.436);
  register_image_procs (gimp);

  (* status_callback) (NULL, _("Layer"), 0.586);
  register_layer_procs (gimp);

  (* status_callback) (NULL, _("Message procedures"), 0.649);
  register_message_procs (gimp);

  (* status_callback) (NULL, _("Miscellaneous"), 0.656);
  register_misc_procs (gimp);

  (* status_callback) (NULL, _("Paint Tool procedures"), 0.661);
  register_paint_tools_procs (gimp);

  (* status_callback) (NULL, _("Palette"), 0.697);
  register_palette_procs (gimp);

  (* status_callback) (NULL, _("Palette UI"), 0.724);
  register_palette_select_procs (gimp);

  (* status_callback) (NULL, _("Palettes"), 0.731);
  register_palettes_procs (gimp);

  (* status_callback) (NULL, _("Parasite procedures"), 0.741);
  register_parasite_procs (gimp);

  (* status_callback) (NULL, _("Paths"), 0.77);
  register_paths_procs (gimp);

  (* status_callback) (NULL, _("Pattern"), 0.806);
  register_pattern_procs (gimp);

  (* status_callback) (NULL, _("Pattern UI"), 0.811);
  register_pattern_select_procs (gimp);

  (* status_callback) (NULL, _("Patterns"), 0.818);
  register_patterns_procs (gimp);

  (* status_callback) (NULL, _("Plug-in"), 0.828);
  register_plug_in_procs (gimp);

  (* status_callback) (NULL, _("Procedural database"), 0.84);
  register_procedural_db_procs (gimp);

  (* status_callback) (NULL, _("Progress"), 0.862);
  register_progress_procs (gimp);

  (* status_callback) (NULL, _("Image mask"), 0.874);
  register_selection_procs (gimp);

  (* status_callback) (NULL, _("Selection Tool procedures"), 0.918);
  register_selection_tools_procs (gimp);

  (* status_callback) (NULL, _("Text procedures"), 0.93);
  register_text_tool_procs (gimp);

  (* status_callback) (NULL, _("Transform Tool procedures"), 0.939);
  register_transform_tools_procs (gimp);

  (* status_callback) (NULL, _("Undo"), 0.954);
  register_undo_procs (gimp);

  (* status_callback) (NULL, _("Units"), 0.971);
  register_unit_procs (gimp);

}
