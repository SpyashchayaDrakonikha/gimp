/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2000 Peter Mattis and Spencer Kimball
 *
 * gimptools_pdb.h
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* NOTE: This file is autogenerated by pdbgen.pl */

#ifndef __GIMP_TOOLS_PDB_H__
#define __GIMP_TOOLS_PDB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* For information look into the C source or the html documentation */


void   gimp_airbrush           (gint32                    drawable_ID,
				gdouble                   pressure,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_airbrush_default   (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_blend              (gint32                    drawable_ID,
				GimpBlendMode             blend_mode,
				GimpLayerModeEffects      paint_mode,
				GimpGradientType          gradient_type,
				gdouble                   opacity,
				gdouble                   offset,
				GimpRepeatMode            repeat,
				gboolean                  supersample,
				gint                      max_depth,
				gdouble                   threshold,
				gdouble                   x1,
				gdouble                   y1,
				gdouble                   x2,
				gdouble                   y2);
void   gimp_bucket_fill        (gint32                    drawable_ID,
				GimpBucketFillMode        fill_mode,
				GimpLayerModeEffects      paint_mode,
				gdouble                   opacity,
				gdouble                   threshold,
				gboolean                  sample_merged,
				gdouble                   x,
				gdouble                   y);
void   gimp_by_color_select    (gint32                    drawable_ID,
				guchar                    red,
				guchar                    green,
				guchar                    blue,
				gint                      threshold,
				GimpChannelOps            operation,
				gboolean                  antialias,
				gboolean                  feather,
				gdouble                   feather_radius,
				gboolean                  sample_merged);
void   gimp_clone              (gint32                    drawable_ID,
				gint32                    src_drawable_ID,
				GimpCloneType             clone_type,
				gdouble                   src_x,
				gdouble                   src_y,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_clone_default      (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_color_picker       (gint32                    image_ID,
				gint32                    drawable_ID,
				gdouble                   x,
				gdouble                   y,
				gboolean                  sample_merged,
				gboolean                  sample_average,
				gdouble                   average_radius,
				gboolean                  save_color,
				guchar                   *red,
				guchar                   *green,
				guchar                   *blue);
void   gimp_convolve           (gint32                    drawable_ID,
				gdouble                   pressure,
				GimpConvolveType          convolve_type,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_convolve_default   (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_crop               (gint32                    image_ID,
				gint                      new_width,
				gint                      new_height,
				gint                      offx,
				gint                      offy);
void   gimp_dodgeburn          (gint32                    drawable_ID,
				gdouble                   exposure,
				GimpDodgeBurnType         dodgeburn_type,
				GimpDodgeBurnMode         dodgeburn_mode,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_dodgeburn_default  (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_ellipse_select     (gint32                    image_ID,
				gdouble                   x,
				gdouble                   y,
				gdouble                   width,
				gdouble                   height,
				GimpChannelOps            operation,
				gboolean                  antialias,
				gboolean                  feather,
				gdouble                   feather_radius);
void   gimp_eraser             (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes,
				GimpBrushApplicationMode  hardness,
				GimpPaintApplicationMode  method);
void   gimp_eraser_default     (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
gint32 gimp_flip               (gint32                    drawable_ID,
				GimpOrientationType       flip_type);
void   gimp_free_select        (gint32                    image_ID,
				gint                      num_segs,
				gdouble                  *segs,
				GimpChannelOps            operation,
				gboolean                  antialias,
				gboolean                  feather,
				gdouble                   feather_radius);
void   gimp_fuzzy_select       (gint32                    drawable_ID,
				gdouble                   x,
				gdouble                   y,
				gint                      threshold,
				GimpChannelOps            operation,
				gboolean                  antialias,
				gboolean                  feather,
				gdouble                   feather_radius,
				gboolean                  sample_merged);
void   gimp_paintbrush         (gint32                    drawable_ID,
				gdouble                   fade_out,
				gint                      num_strokes,
				gdouble                  *strokes,
				GimpPaintApplicationMode  method,
				gdouble                   gradient_length);
void   gimp_paintbrush_default (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_pencil             (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
gint32 gimp_perspective        (gint32                    drawable_ID,
				gboolean                  interpolation,
				gdouble                   x0,
				gdouble                   y0,
				gdouble                   x1,
				gdouble                   y1,
				gdouble                   x2,
				gdouble                   y2,
				gdouble                   x3,
				gdouble                   y3);
void   gimp_rect_select        (gint32                    image_ID,
				gdouble                   x,
				gdouble                   y,
				gdouble                   width,
				gdouble                   height,
				GimpChannelOps            operation,
				gboolean                  feather,
				gdouble                   feather_radius);
gint32 gimp_rotate             (gint32                    drawable_ID,
				gboolean                  interpolation,
				gdouble                   angle);
gint32 gimp_scale              (gint32                    drawable_ID,
				gboolean                  interpolation,
				gdouble                   x0,
				gdouble                   y0,
				gdouble                   x1,
				gdouble                   y1);
gint32 gimp_shear              (gint32                    drawable_ID,
				gboolean                  interpolation,
				GimpOrientationType       shear_type,
				gdouble                   magnitude);
void   gimp_smudge             (gint32                    drawable_ID,
				gdouble                   pressure,
				gint                      num_strokes,
				gdouble                  *strokes);
void   gimp_smudge_default     (gint32                    drawable_ID,
				gint                      num_strokes,
				gdouble                  *strokes);
gint32 gimp_transform_2d       (gint32                    drawable_ID,
				gboolean                  interpolation,
				gdouble                   source_x,
				gdouble                   source_y,
				gdouble                   scale_x,
				gdouble                   scale_y,
				gdouble                   angle,
				gdouble                   dest_x,
				gdouble                   dest_y);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GIMP_TOOLS_PDB_H__ */
