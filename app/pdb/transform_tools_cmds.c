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

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "config/gimpcoreconfig.h"
#include "core/gimp-transform-utils.h"
#include "core/gimp.h"
#include "core/gimpdrawable-transform.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"

#include "libgimpmath/gimpmath.h"

static ProcRecord flip_proc;
static ProcRecord perspective_proc;
static ProcRecord rotate_proc;
static ProcRecord scale_proc;
static ProcRecord shear_proc;
static ProcRecord transform_2d_proc;

void
register_transform_tools_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &flip_proc);
  procedural_db_register (gimp, &perspective_proc);
  procedural_db_register (gimp, &rotate_proc);
  procedural_db_register (gimp, &scale_proc);
  procedural_db_register (gimp, &shear_proc);
  procedural_db_register (gimp, &transform_2d_proc);
}

static Argument *
flip_invoker (Gimp     *gimp,
              Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gint32 flip_type;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  flip_type = args[1].value.pdb_int;
  if (flip_type < GIMP_ORIENTATION_HORIZONTAL || flip_type > GIMP_ORIENTATION_VERTICAL)
    success = FALSE;

  if (success)
    {
      success = gimp_item_is_attached (GIMP_ITEM (drawable));
    
      if (success)
	success = gimp_drawable_transform_flip (drawable, flip_type);
    }

  return_args = procedural_db_return_args (&flip_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg flip_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "flip_type",
    "Type of flip: GIMP_ORIENTATION_HORIZONTAL (0) or GIMP_ORIENTATION_VERTICAL (1)"
  }
};

static ProcArg flip_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The flipped drawable"
  }
};

static ProcRecord flip_proc =
{
  "gimp_flip",
  "Flip the specified drawable about its center either vertically or horizontally.",
  "This tool flips the specified drawable if no selection exists. If a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then flipd by the specified amount. The return value is the ID of the flipped drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and flipped drawable. The flip type parameter indicates whether the flip will be applied horizontally or vertically.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  flip_inargs,
  1,
  flip_outargs,
  { { flip_invoker } }
};

static Argument *
perspective_invoker (Gimp     *gimp,
                     Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble trans_info[8];

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  interpolation = args[1].value.pdb_int ? TRUE : FALSE;

  trans_info[X0] = args[2].value.pdb_float;

  trans_info[Y0] = args[3].value.pdb_float;

  trans_info[X1] = args[4].value.pdb_float;

  trans_info[Y1] = args[5].value.pdb_float;

  trans_info[X2] = args[6].value.pdb_float;

  trans_info[Y2] = args[7].value.pdb_float;

  trans_info[X3] = args[8].value.pdb_float;

  trans_info[Y3] = args[9].value.pdb_float;

  if (success)
    {
      success = gimp_item_is_attached (GIMP_ITEM (drawable));
    
      if (success)
	{
	  gint                  x1, y1, x2, y2;
	  GimpMatrix3           matrix;
	  GimpInterpolationType interpolation_type;
    
	  gimp_drawable_mask_bounds (drawable, &x1, &y1, &x2, &y2);
    
	  /* Assemble the transformation matrix */
	  gimp_transform_matrix_perspective (x1, y1, x2, y2,
					     trans_info[X0], trans_info[Y0],
					     trans_info[X1], trans_info[Y1],
					     trans_info[X2], trans_info[Y2],
					     trans_info[X3], trans_info[Y3],
					     &matrix);
    
	  if (interpolation)
	    interpolation_type = gimp->config->interpolation_type;
	  else
	    interpolation_type = GIMP_INTERPOLATION_NONE;
    
	  /* Perspective the selection */
	  success = gimp_drawable_transform_affine (drawable,
						    &matrix, GIMP_TRANSFORM_FORWARD,
						    interpolation_type, TRUE,
						    FALSE);
	}
    }

  return_args = procedural_db_return_args (&perspective_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg perspective_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "interpolation",
    "Whether to use interpolation"
  },
  {
    GIMP_PDB_FLOAT,
    "x0",
    "The new x coordinate of upper-left corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "y0",
    "The new y coordinate of upper-left corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "x1",
    "The new x coordinate of upper-right corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "y1",
    "The new y coordinate of upper-right corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "x2",
    "The new x coordinate of lower-left corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "y2",
    "The new y coordinate of lower-left corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "x3",
    "The new x coordinate of lower-right corner of original bounding box"
  },
  {
    GIMP_PDB_FLOAT,
    "y3",
    "The new y coordinate of lower-right corner of original bounding box"
  }
};

static ProcArg perspective_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The newly mapped drawable"
  }
};

static ProcRecord perspective_proc =
{
  "gimp_perspective",
  "Perform a possibly non-affine transformation on the specified drawable.",
  "This tool performs a possibly non-affine transformation on the specified drawable by allowing the corners of the original bounding box to be arbitrarily remapped to any values. The specified drawable is remapped if no selection exists. However, if a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then remapped as specified. The interpolation parameter can be set to TRUE to indicate that either linear or cubic interpolation should be used to smooth the resulting remapped drawable. The return value is the ID of the remapped drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and remapped drawable. The 4 coordinates specify the new locations of each corner of the original bounding box. By specifying these values, any affine transformation (rotation, scaling, translation) can be affected. Additionally,"
  "these values can be specified such that the resulting transformed drawable will appear to have been projected via a perspective transform.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  10,
  perspective_inargs,
  1,
  perspective_outargs,
  { { perspective_invoker } }
};

static Argument *
rotate_invoker (Gimp     *gimp,
                Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble angle;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  interpolation = args[1].value.pdb_int ? TRUE : FALSE;

  angle = args[2].value.pdb_float;

  if (success)
    {
      success = gimp_item_is_attached (GIMP_ITEM (drawable));
    
      if (success)
	{
	  gint                  x1, y1, x2, y2;
	  GimpMatrix3           matrix;
	  GimpInterpolationType interpolation_type;
    
	  gimp_drawable_mask_bounds (drawable, &x1, &y1, &x2, &y2);
    
	  /* Assemble the transformation matrix */
	  gimp_transform_matrix_rotate (x1, y1, x2, y2, angle, &matrix);
    
	  if (interpolation)
	    interpolation_type = gimp->config->interpolation_type;
	  else
	    interpolation_type = GIMP_INTERPOLATION_NONE;
    
	  /* Rotate the selection */
	  success = gimp_drawable_transform_affine (drawable,
						    &matrix, GIMP_TRANSFORM_FORWARD,
						    interpolation_type, FALSE,
						    FALSE);
	}
    }

  return_args = procedural_db_return_args (&rotate_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg rotate_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "interpolation",
    "Whether to use interpolation"
  },
  {
    GIMP_PDB_FLOAT,
    "angle",
    "The angle of rotation (radians)"
  }
};

static ProcArg rotate_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The rotated drawable"
  }
};

static ProcRecord rotate_proc =
{
  "gimp_rotate",
  "Rotate the specified drawable about its center through the specified angle.",
  "This tool rotates the specified drawable if no selection exists. If a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then rotated by the specified amount. The interpolation parameter can be set to TRUE to indicate that either linear or cubic interpolation should be used to smooth the resulting rotated drawable. The return value is the ID of the rotated drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and rotated drawable.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  3,
  rotate_inargs,
  1,
  rotate_outargs,
  { { rotate_invoker } }
};

static Argument *
scale_invoker (Gimp     *gimp,
               Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble trans_info[4];

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  interpolation = args[1].value.pdb_int ? TRUE : FALSE;

  trans_info[X0] = args[2].value.pdb_float;

  trans_info[Y0] = args[3].value.pdb_float;

  trans_info[X1] = args[4].value.pdb_float;

  trans_info[Y1] = args[5].value.pdb_float;

  if (success)
    {
      success = (gimp_item_is_attached (GIMP_ITEM (drawable)) &&
		 trans_info[X0] < trans_info[X1] &&
		 trans_info[Y0] < trans_info[X1]);
    
      if (success)
	{
	  gint                  x1, y1, x2, y2;
	  GimpMatrix3           matrix;
	  GimpInterpolationType interpolation_type;
    
	  gimp_drawable_mask_bounds (drawable, &x1, &y1, &x2, &y2);
    
	  /* Assemble the transformation matrix */
	  gimp_transform_matrix_scale (x1, y1, x2, y2,
				       trans_info[X0], trans_info[Y0],
				       trans_info[X1], trans_info[Y1],
				       &matrix);
    
	  if (interpolation)
	    interpolation_type = gimp->config->interpolation_type;
	  else
	    interpolation_type = GIMP_INTERPOLATION_NONE;
    
	  /* Scale the selection */
	  success = gimp_drawable_transform_affine (drawable,
						    &matrix, GIMP_TRANSFORM_FORWARD,
						    interpolation_type, TRUE,
						    FALSE);
	}
    }

  return_args = procedural_db_return_args (&scale_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg scale_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "interpolation",
    "Whether to use interpolation"
  },
  {
    GIMP_PDB_FLOAT,
    "x0",
    "The new x coordinate of upper-left corner of newly scaled region"
  },
  {
    GIMP_PDB_FLOAT,
    "y0",
    "The new y coordinate of upper-left corner of newly scaled region"
  },
  {
    GIMP_PDB_FLOAT,
    "x1",
    "The new x coordinate of lower-right corner of newly scaled region"
  },
  {
    GIMP_PDB_FLOAT,
    "y1",
    "The new y coordinate of lower-right corner of newly scaled region"
  }
};

static ProcArg scale_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The scaled drawable"
  }
};

static ProcRecord scale_proc =
{
  "gimp_scale",
  "Scale the specified drawable.",
  "This tool scales the specified drawable if no selection exists. If a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then scaled by the specified amount. The interpolation parameter can be set to TRUE to indicate that either linear or cubic interpolation should be used to smooth the resulting scaled drawable. The return value is the ID of the scaled drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and scaled drawable.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  6,
  scale_inargs,
  1,
  scale_outargs,
  { { scale_invoker } }
};

static Argument *
shear_invoker (Gimp     *gimp,
               Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gboolean interpolation;
  gint32 shear_type;
  gdouble magnitude;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  interpolation = args[1].value.pdb_int ? TRUE : FALSE;

  shear_type = args[2].value.pdb_int;
  if (shear_type < GIMP_ORIENTATION_HORIZONTAL || shear_type > GIMP_ORIENTATION_VERTICAL)
    success = FALSE;

  magnitude = args[3].value.pdb_float;

  if (success)
    {
      success = gimp_item_is_attached (GIMP_ITEM (drawable));
    
      if (success)
	{
	  gint                  x1, y1, x2, y2;
	  GimpMatrix3           matrix;
	  GimpInterpolationType interpolation_type;
    
	  gimp_drawable_mask_bounds (drawable, &x1, &y1, &x2, &y2);
    
	  /* Assemble the transformation matrix */
	  gimp_transform_matrix_shear (x1, y1, x2, y2,
				       shear_type, magnitude,
				       &matrix);
    
	  if (interpolation)
	    interpolation_type = gimp->config->interpolation_type;
	  else
	    interpolation_type = GIMP_INTERPOLATION_NONE;
    
	  /* Shear the selection */
	  success = gimp_drawable_transform_affine (drawable,
						    &matrix, GIMP_TRANSFORM_FORWARD,
						    interpolation_type, FALSE,
						    FALSE);
	}
    }

  return_args = procedural_db_return_args (&shear_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg shear_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "interpolation",
    "Whether to use interpolation"
  },
  {
    GIMP_PDB_INT32,
    "shear_type",
    "Type of shear: GIMP_ORIENTATION_HORIZONTAL (0) or GIMP_ORIENTATION_VERTICAL (1)"
  },
  {
    GIMP_PDB_FLOAT,
    "magnitude",
    "The magnitude of the shear"
  }
};

static ProcArg shear_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The sheared drawable"
  }
};

static ProcRecord shear_proc =
{
  "gimp_shear",
  "Shear the specified drawable about its center by the specified magnitude.",
  "This tool shears the specified drawable if no selection exists. If a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then sheard by the specified amount. The interpolation parameter can be set to TRUE to indicate that either linear or cubic interpolation should be used to smooth the resulting sheared drawable. The return value is the ID of the sheard drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and sheard drawable. The shear type parameter indicates whether the shear will be applied horizontally or vertically. The magnitude can be either positive or negative and indicates the extent (in pixels) to shear by.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  4,
  shear_inargs,
  1,
  shear_outargs,
  { { shear_invoker } }
};

static Argument *
transform_2d_invoker (Gimp     *gimp,
                      Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble source_x;
  gdouble source_y;
  gdouble scale_x;
  gdouble scale_y;
  gdouble angle;
  gdouble dest_x;
  gdouble dest_y;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  interpolation = args[1].value.pdb_int ? TRUE : FALSE;

  source_x = args[2].value.pdb_float;

  source_y = args[3].value.pdb_float;

  scale_x = args[4].value.pdb_float;

  scale_y = args[5].value.pdb_float;

  angle = args[6].value.pdb_float;

  dest_x = args[7].value.pdb_float;

  dest_y = args[8].value.pdb_float;

  if (success)
    {
      success = gimp_item_is_attached (GIMP_ITEM (drawable));
    
      if (success)
	{
	  GimpMatrix3           matrix;
	  GimpInterpolationType interpolation_type;
    
	  /* Assemble the transformation matrix */
	  gimp_matrix3_identity  (&matrix);
	  gimp_matrix3_translate (&matrix, -source_x, -source_y);
	  gimp_matrix3_scale     (&matrix, scale_x, scale_y);
	  gimp_matrix3_rotate    (&matrix, angle);
	  gimp_matrix3_translate (&matrix, dest_x, dest_y);
    
	  if (interpolation)
	    interpolation_type = gimp->config->interpolation_type;
	  else
	    interpolation_type = GIMP_INTERPOLATION_NONE;
    
	  /* Transform the selection */
	  success = gimp_drawable_transform_affine (drawable,
						    &matrix, GIMP_TRANSFORM_FORWARD,
						    interpolation_type, TRUE,
						    FALSE);
	}
    }

  return_args = procedural_db_return_args (&transform_2d_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (drawable));

  return return_args;
}

static ProcArg transform_2d_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "interpolation",
    "Whether to use interpolation"
  },
  {
    GIMP_PDB_FLOAT,
    "source_x",
    "X coordinate of the transformation center"
  },
  {
    GIMP_PDB_FLOAT,
    "source_y",
    "Y coordinate of the transformation center"
  },
  {
    GIMP_PDB_FLOAT,
    "scale_x",
    "Amount to scale in x direction"
  },
  {
    GIMP_PDB_FLOAT,
    "scale_y",
    "Amount to scale in y direction"
  },
  {
    GIMP_PDB_FLOAT,
    "angle",
    "The angle of rotation (radians)"
  },
  {
    GIMP_PDB_FLOAT,
    "dest_x",
    "X coordinate of where the centre goes"
  },
  {
    GIMP_PDB_FLOAT,
    "dest_y",
    "Y coordinate of where the centre goes"
  }
};

static ProcArg transform_2d_outargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The transformed drawable"
  }
};

static ProcRecord transform_2d_proc =
{
  "gimp_transform_2d",
  "Transform the specified drawable in 2d.",
  "This tool transforms the specified drawable if no selection exists. If a selection exists, the portion of the drawable which lies under the selection is cut from the drawable and made into a floating selection which is then transformed. The interpolation parameter can be set to TRUE to indicate that either linear or cubic interpolation should be used to smooth the resulting drawable. The transformation is done by scaling the image by the x and y scale factors about the point (source_x, source_y), then rotating around the same point, then translating that point to the new position (dest_x, dest_y). The return value is the ID of the rotated drawable. If there was no selection, this will be equal to the drawable ID supplied as input. Otherwise, this will be the newly created and transformed drawable.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  9,
  transform_2d_inargs,
  1,
  transform_2d_outargs,
  { { transform_2d_invoker } }
};
