/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpcolor_pdb.c
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

#include "config.h"

#include "gimp.h"
#undef GIMP_DISABLE_DEPRECATED
#undef __GIMP_COLOR_PDB_H__
#include "gimpcolor_pdb.h"

/**
 * gimp_brightness_contrast:
 * @drawable_ID: The drawable.
 * @brightness: Brightness adjustment.
 * @contrast: Contrast adjustment.
 *
 * Modify brightness/contrast in the specified drawable.
 *
 * This procedures allows the brightness and contrast of the specified
 * drawable to be modified. Both 'brightness' and 'contrast' parameters
 * are defined between -127 and 127.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_brightness_contrast (gint32 drawable_ID,
                          gint   brightness,
                          gint   contrast)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-brightness-contrast",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, brightness,
                                    GIMP_PDB_INT32, contrast,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @low_input: Intensity of lowest input.
 * @high_input: Intensity of highest input.
 * @gamma: Gamma correction factor.
 * @low_output: Intensity of lowest output.
 * @high_output: Intensity of highest output.
 *
 * Modifies intensity levels in the specified drawable.
 *
 * This tool allows intensity levels in the specified drawable to be
 * remapped according to a set of parameters. The low/high input levels
 * specify an initial mapping from the source intensities. The gamma
 * value determines how intensities between the low and high input
 * intensities are interpolated. A gamma value of 1.0 results in a
 * linear interpolation. Higher gamma values result in more high-level
 * intensities. Lower gamma values result in more low-level
 * intensities. The low/high output levels constrain the final
 * intensity mapping--that is, no final intensity will be lower than
 * the low output level and no final intensity will be higher than the
 * high output level. This tool is only valid on RGB color and
 * grayscale images. It will not operate on indexed drawables.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_levels (gint32               drawable_ID,
             GimpHistogramChannel channel,
             gint                 low_input,
             gint                 high_input,
             gdouble              gamma,
             gint                 low_output,
             gint                 high_output)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, low_input,
                                    GIMP_PDB_INT32, high_input,
                                    GIMP_PDB_FLOAT, gamma,
                                    GIMP_PDB_INT32, low_output,
                                    GIMP_PDB_INT32, high_output,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels_auto:
 * @drawable_ID: The drawable.
 *
 * This procedure is deprecated! Use gimp_levels_stretch() instead.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_levels_auto (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels-auto",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels_stretch:
 * @drawable_ID: The drawable.
 *
 * Automatically modifies intensity levels in the specified drawable.
 *
 * This procedure allows intensity levels in the specified drawable to
 * be remapped according to a set of guessed parameters. It is
 * equivalent to clicking the \"Auto\" button in the Levels tool. This
 * procedure is only valid on RGB color and grayscale images. It will
 * not operate on indexed drawables.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_levels_stretch (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels-stretch",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_posterize:
 * @drawable_ID: The drawable.
 * @levels: Levels of posterization.
 *
 * Posterize the specified drawable.
 *
 * This procedures reduces the number of shades allows in each
 * intensity channel to the specified 'levels' parameter.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_posterize (gint32 drawable_ID,
                gint   levels)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-posterize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, levels,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_desaturate:
 * @drawable_ID: The drawable.
 *
 * Desaturate the contents of the specified drawable.
 *
 * This procedure desaturates the contents of the specified drawable.
 * This procedure only works on drawables of type RGB color.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_desaturate (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-desaturate",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_desaturate_full:
 * @drawable_ID: The drawable.
 * @desaturate_mode: The formula to use to desaturate.
 *
 * Desaturate the contents of the specified drawable, with the
 * specified formula.
 *
 * This procedure desaturates the contents of the specified drawable,
 * with the specified formula. This procedure only works on drawables
 * of type RGB color.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.4
 */
gboolean
gimp_desaturate_full (gint32             drawable_ID,
                      GimpDesaturateMode desaturate_mode)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-desaturate-full",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, desaturate_mode,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_equalize:
 * @drawable_ID: The drawable.
 * @mask_only: Equalization option.
 *
 * Equalize the contents of the specified drawable.
 *
 * This procedure equalizes the contents of the specified drawable.
 * Each intensity channel is equalizeed independently. The equalized
 * intensity is given as inten' = (255 - inten). Indexed color
 * drawables are not valid for this operation. The 'mask_only' option
 * specifies whether to adjust only the area of the image within the
 * selection bounds, or the entire image based on the histogram of the
 * selected area. If there is no selection, the entire image is
 * adjusted based on the histogram for the entire image.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_equalize (gint32   drawable_ID,
               gboolean mask_only)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-equalize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, mask_only,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_invert:
 * @drawable_ID: The drawable.
 *
 * Invert the contents of the specified drawable.
 *
 * This procedure inverts the contents of the specified drawable. Each
 * intensity channel is inverted independently. The inverted intensity
 * is given as inten' = (255 - inten). Indexed color drawables are not
 * valid for this operation.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_invert (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-invert",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_curves_spline:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @num_points: The number of values in the control point array.
 * @control_pts: The spline control points: { cp1.x, cp1.y, cp2.x, cp2.y, ... }.
 *
 * Modifies the intensity curve(s) for specified drawable.
 *
 * Modifies the intensity mapping for one channel in the specified
 * drawable. The drawable must be either grayscale or RGB, and the
 * channel can be either an intensity component, or the value. The
 * 'control_pts' parameter is an array of integers which define a set
 * of control points which describe a Catmull Rom spline which yields
 * the final intensity curve. Use the gimp_curves_explicit() function
 * to explicitly modify intensity levels.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_curves_spline (gint32                drawable_ID,
                    GimpHistogramChannel  channel,
                    gint                  num_points,
                    const guint8         *control_pts)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-curves-spline",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, num_points,
                                    GIMP_PDB_INT8ARRAY, control_pts,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_curves_explicit:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @num_bytes: The number of bytes in the new curve (always 256).
 * @curve: The explicit curve.
 *
 * Modifies the intensity curve(s) for specified drawable.
 *
 * Modifies the intensity mapping for one channel in the specified
 * drawable. The drawable must be either grayscale or RGB, and the
 * channel can be either an intensity component, or the value. The
 * 'curve' parameter is an array of bytes which explicitly defines how
 * each pixel value in the drawable will be modified. Use the
 * gimp_curves_spline() function to modify intensity levels with
 * Catmull Rom splines.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_curves_explicit (gint32                drawable_ID,
                      GimpHistogramChannel  channel,
                      gint                  num_bytes,
                      const guint8         *curve)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-curves-explicit",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, num_bytes,
                                    GIMP_PDB_INT8ARRAY, curve,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_color_balance:
 * @drawable_ID: The drawable.
 * @transfer_mode: Transfer mode.
 * @preserve_lum: Preserve luminosity values at each pixel.
 * @cyan_red: Cyan-Red color balance.
 * @magenta_green: Magenta-Green color balance.
 * @yellow_blue: Yellow-Blue color balance.
 *
 * Modify the color balance of the specified drawable.
 *
 * Modify the color balance of the specified drawable. There are three
 * axis which can be modified: cyan-red, magenta-green, and
 * yellow-blue. Negative values increase the amount of the former,
 * positive values increase the amount of the latter. Color balance can
 * be controlled with the 'transfer_mode' setting, which allows
 * shadows, midtones, and highlights in an image to be affected
 * differently. The 'preserve-lum' parameter, if TRUE, ensures that the
 * luminosity of each pixel remains fixed.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_color_balance (gint32           drawable_ID,
                    GimpTransferMode transfer_mode,
                    gboolean         preserve_lum,
                    gdouble          cyan_red,
                    gdouble          magenta_green,
                    gdouble          yellow_blue)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-color-balance",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, transfer_mode,
                                    GIMP_PDB_INT32, preserve_lum,
                                    GIMP_PDB_FLOAT, cyan_red,
                                    GIMP_PDB_FLOAT, magenta_green,
                                    GIMP_PDB_FLOAT, yellow_blue,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_colorize:
 * @drawable_ID: The drawable.
 * @hue: Hue in degrees.
 * @saturation: Saturation in percent.
 * @lightness: Lightness in percent.
 *
 * Render the drawable as a grayscale image seen through a colored
 * glass.
 *
 * Desatures the drawable, then tints it with the specified color. This
 * tool is only valid on RGB color images. It will not operate on
 * grayscale or indexed drawables.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_colorize (gint32  drawable_ID,
               gdouble hue,
               gdouble saturation,
               gdouble lightness)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-colorize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_FLOAT, hue,
                                    GIMP_PDB_FLOAT, saturation,
                                    GIMP_PDB_FLOAT, lightness,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_histogram:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @start_range: Start of the intensity measurement range.
 * @end_range: End of the intensity measurement range.
 * @mean: Mean intensity value.
 * @std_dev: Standard deviation of intensity values.
 * @median: Median intensity value.
 * @pixels: Alpha-weighted pixel count for entire image.
 * @count: Alpha-weighted pixel count for range.
 * @percentile: Percentile that range falls under.
 *
 * Returns information on the intensity histogram for the specified
 * drawable.
 *
 * This tool makes it possible to gather information about the
 * intensity histogram of a drawable. A channel to examine is first
 * specified. This can be either value, red, green, or blue, depending
 * on whether the drawable is of type color or grayscale. The drawable
 * may not be indexed. Second, a range of intensities are specified.
 * The gimp_histogram() function returns statistics based on the pixels
 * in the drawable that fall under this range of values. Mean, standard
 * deviation, median, number of pixels, and percentile are all
 * returned. Additionally, the total count of pixels in the image is
 * returned. Counts of pixels are weighted by any associated alpha
 * values and by the current selection mask. That is, pixels that lie
 * outside an active selection mask will not be counted. Similarly,
 * pixels with transparent alpha values will not be counted.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_histogram (gint32                drawable_ID,
                GimpHistogramChannel  channel,
                gint                  start_range,
                gint                  end_range,
                gdouble              *mean,
                gdouble              *std_dev,
                gdouble              *median,
                gdouble              *pixels,
                gdouble              *count,
                gdouble              *percentile)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-histogram",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, start_range,
                                    GIMP_PDB_INT32, end_range,
                                    GIMP_PDB_END);

  *mean = 0.0;
  *std_dev = 0.0;
  *median = 0.0;
  *pixels = 0.0;
  *count = 0.0;
  *percentile = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *mean = return_vals[1].data.d_float;
      *std_dev = return_vals[2].data.d_float;
      *median = return_vals[3].data.d_float;
      *pixels = return_vals[4].data.d_float;
      *count = return_vals[5].data.d_float;
      *percentile = return_vals[6].data.d_float;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_hue_saturation:
 * @drawable_ID: The drawable.
 * @hue_range: Range of affected hues.
 * @hue_offset: Hue offset in degrees.
 * @lightness: Lightness modification.
 * @saturation: Saturation modification.
 *
 * Modify hue, lightness, and saturation in the specified drawable.
 *
 * This procedures allows the hue, lightness, and saturation in the
 * specified drawable to be modified. The 'hue-range' parameter
 * provides the capability to limit range of affected hues.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_hue_saturation (gint32       drawable_ID,
                     GimpHueRange hue_range,
                     gdouble      hue_offset,
                     gdouble      lightness,
                     gdouble      saturation)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-hue-saturation",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, hue_range,
                                    GIMP_PDB_FLOAT, hue_offset,
                                    GIMP_PDB_FLOAT, lightness,
                                    GIMP_PDB_FLOAT, saturation,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_threshold:
 * @drawable_ID: The drawable.
 * @low_threshold: The low threshold value.
 * @high_threshold: The high threshold value.
 *
 * Threshold the specified drawable.
 *
 * This procedures generates a threshold map of the specified drawable.
 * All pixels between the values of 'low_threshold' and
 * 'high_threshold' are replaced with white, and all other pixels with
 * black.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_threshold (gint32 drawable_ID,
                gint   low_threshold,
                gint   high_threshold)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-threshold",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, low_threshold,
                                    GIMP_PDB_INT32, high_threshold,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
