/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Object properties deserialization routines
 * Copyright (C) 2001-2002  Sven Neumann <sven@gimp.org>
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

#ifndef __GIMP_CONFIG_DESERIALIZE_H__
#define __GIMP_CONFIG_DESERIALIZE_H__


gboolean    gimp_config_deserialize_properties (GimpConfig *config,
                                                GScanner   *scanner,
                                                gint        nest_level);
GTokenType  gimp_config_deserialize_property   (GimpConfig *config,
                                                GScanner   *scanner,
                                                gint        nest_level);


#endif /* __GIMP_CONFIG_DESERIALIZE_H__ */
