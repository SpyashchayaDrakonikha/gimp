/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * GimpGrid
 * Copyright (C) 2003  Henrik Brix Andersen <brix@gimp.org>
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

#ifndef __GIMP_GRID_H__
#define __GIMP_GRID_H__


#include "gimpobject.h"


#define GIMP_TYPE_GRID            (gimp_grid_get_type ())
#define GIMP_GRID(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_GRID, GimpGrid))
#define GIMP_GRID_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_GRID, GimpGridClass))
#define GIMP_IS_GRID(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_GRID))
#define GIMP_IS_GRID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_GRID))
#define GIMP_GRID_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_GRID, GimpGridClass))


typedef struct _GimpGridClass  GimpGridClass;

struct _GimpGrid
{
  GimpObject    parent_instance;

  gdouble        xspacing;
  gdouble        yspacing;
  GimpUnit       spacing_unit;
  gdouble        xoffset;
  gdouble        yoffset;
  GimpUnit       offset_unit;
  GimpRGB        fgcolor;
  GimpRGB        bgcolor;
  GimpGridStyle  type;
};


struct _GimpGridClass
{
  GimpObjectClass  parent_class;
};


GType gimp_grid_get_type (void) G_GNUC_CONST;


#endif /* __GIMP_GRID_H__ */
