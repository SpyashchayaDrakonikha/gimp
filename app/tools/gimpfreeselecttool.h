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

#ifndef __FREE_SELECT_H__
#define __FREE_SELECT_H__

#include "scan_convert.h"


/*  free select action functions  */

void   free_select_button_press   (Tool *, GdkEventButton *, gpointer);
void   free_select_button_release (Tool *, GdkEventButton *, gpointer);
void   free_select_motion         (Tool *, GdkEventMotion *, gpointer);


/*  free select functions  */

void   free_select                (GimpImage        *gimage,
				   gint              num_pts,
				   ScanConvertPoint *pts,
				   SelectOps         op,
				   gboolean          antialias,
				   gboolean          feather,
				   gdouble           feather_radius);
void   free_select_draw           (Tool             *tool);

Tool * tools_new_free_select      (void);
void   tools_free_free_select     (Tool             *tool);


#endif  /*  __FREE_SELECT_H__  */
