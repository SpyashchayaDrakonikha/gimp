/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpchannellistview.h
 * Copyright (C) 2001 Michael Natterer
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

#ifndef __GIMP_CHANNEL_LIST_VIEW_H__
#define __GIMP_CHANNEL_LIST_VIEW_H__


#include "gimpdrawablelistview.h"


#define GIMP_TYPE_CHANNEL_LIST_VIEW            (gimp_channel_list_view_get_type ())
#define GIMP_CHANNEL_LIST_VIEW(obj)            (GTK_CHECK_CAST ((obj), GIMP_TYPE_CHANNEL_LIST_VIEW, GimpChannelListView))
#define GIMP_CHANNEL_LIST_VIEW_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), GIMP_TYPE_CHANNEL_LIST_VIEW, GimpChannelListViewClass))
#define GIMP_IS_CHANNEL_LIST_VIEW(obj)         (GTK_CHECK_TYPE ((obj), GIMP_TYPE_CHANNEL_LIST_VIEW))
#define GIMP_IS_CHANNEL_LIST_VIEW_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_CHANNEL_LIST_VIEW))


typedef struct _GimpChannelListViewClass  GimpChannelListViewClass;

struct _GimpChannelListView
{
  GimpDrawableListView  parent_instance;

  GtkWidget            *component_frame;
  GtkWidget            *component_list;

  GtkWidget            *toselection_button;
};

struct _GimpChannelListViewClass
{
  GimpDrawableListViewClass  parent_class;
};


GtkType   gimp_channel_list_view_get_type (void);


#endif  /*  __GIMP_CHANNEL_LIST_VIEW_H__  */
