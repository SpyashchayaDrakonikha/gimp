/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2000 Peter Mattis and Spencer Kimball
 *
 * gimppalettes_pdb.c
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

#include "gimp.h"

/**
 * gimp_palettes_refresh:
 *
 * Refreshes current palettes.
 *
 * This procedure incorporates all palettes currently in the users
 * palette path.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_palettes_refresh (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_palettes_refresh",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_palettes_get_list:
 * @num_palettes: The number of palettes in the list.
 *
 * Retrieves a list of all of the available palettes
 *
 * This procedure returns a complete listing of available palettes.
 * Each name returned can be used as input to the command
 * 'gimp_palette_set_palette'.
 *
 * Returns: The list of palette names.
 */
gchar **
gimp_palettes_get_list (gint *num_palettes)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar **palette_list = NULL;
  gint i;

  return_vals = gimp_run_procedure ("gimp_palettes_get_list",
				    &nreturn_vals,
				    GIMP_PDB_END);

  *num_palettes = 0;

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      *num_palettes = return_vals[1].data.d_int32;
      palette_list = g_new (gchar *, *num_palettes);
      for (i = 0; i < *num_palettes; i++)
	palette_list[i] = g_strdup (return_vals[2].data.d_stringarray[i]);
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return palette_list;
}

/**
 * gimp_palettes_get_palette:
 * @num_colors: The palette num_colors.
 *
 * Retrieve information about the currently active palette.
 *
 * This procedure retrieves information about the currently active
 * palette. This includes the name, and the number of colors.
 *
 * Returns: The palette name.
 */
gchar *
gimp_palettes_get_palette (gint *num_colors)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp_palettes_get_palette",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      name = g_strdup (return_vals[1].data.d_string);
      *num_colors = return_vals[2].data.d_int32;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_palettes_set_palette:
 * @name: The palette name.
 *
 * Set the specified palette as the active palette.
 *
 * This procedure allows the active palette to be set by specifying its
 * name. The name is simply a string which corresponds to one of the
 * names of the installed palettes. If no matching palette is found,
 * this procedure will return an error. Otherwise, the specified
 * palette becomes active and will be used in all subsequent palette
 * operations.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_palettes_set_palette (gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_palettes_set_palette",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_palettes_get_palette_entry:
 * @name: the palette name (\"\" means currently active palette).
 * @entry_num: The entry to retrieve.
 * @num_colors: The palette num_colors.
 * @color: The color requested.
 *
 * Gets the specified palette entry from the currently active palette.
 *
 * This procedure retrieves the color of the zero-based entry specifed
 * for the current palette. It returns an error if the entry does not
 * exist.
 *
 * Returns: The palette name.
 */
gchar *
gimp_palettes_get_palette_entry (gchar   *name,
				 gint     entry_num,
				 gint    *num_colors,
				 GimpRGB *color)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *ret_name = NULL;

  return_vals = gimp_run_procedure ("gimp_palettes_get_palette_entry",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, entry_num,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      ret_name = g_strdup (return_vals[1].data.d_string);
      *num_colors = return_vals[2].data.d_int32;
      *color = return_vals[3].data.d_color;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return ret_name;
}
