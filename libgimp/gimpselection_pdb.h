/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2000 Peter Mattis and Spencer Kimball
 *
 * gimpselection_pdb.h
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

#ifndef __GIMP_SELECTION_PDB_H__
#define __GIMP_SELECTION_PDB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* For information look into the C source or the html documentation */


void     gimp_selection_bounds   (gint32    image_ID,
				  gboolean *non_empty,
				  gint     *x1,
				  gint     *y1,
				  gint     *x2,
				  gint     *y2);
gboolean gimp_selection_is_empty (gint32    image_ID);
gint32   _gimp_selection_float   (gint32    drawable_ID,
				  gint      offx,
				  gint      offy);
void     gimp_selection_none     (gint32    image_ID);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GIMP_SELECTION_PDB_H__ */
