/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * GimpTextLayer
 * Copyright (C) 2002-2003  Sven Neumann <sven@gimp.org>
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

#ifndef __GIMP_TEXT_LAYER_H__
#define __GIMP_TEXT_LAYER_H__


#include "core/gimplayer.h"


#define GIMP_TYPE_TEXT_LAYER            (gimp_text_layer_get_type ())
#define GIMP_TEXT_LAYER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_TEXT_LAYER, GimpTextLayer))
#define GIMP_TEXT_LAYER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_TEXT_LAYER, GimpTextLayerClass))
#define GIMP_IS_TEXT_LAYER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_TEXT_LAYER))
#define GIMP_IS_TEXT_LAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_TEXT_LAYER))
#define GIMP_TEXT_LAYER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_TEXT_LAYER, GimpTextLayerClass))


typedef struct _GimpTextLayerClass GimpTextLayerClass;

struct _GimpTextLayer
{
  GimpLayer     layer;

  GimpText     *text;
  const gchar  *text_parasite;  /*  parasite name that this text was set from,
                                 *  and that should be removed when the text
                                 *  is changed.
                                 */
  guint         idle_render_id;
  gboolean      auto_rename;
  gboolean      modified;
};

struct _GimpTextLayerClass
{
  GimpLayerClass  parent_class;
};


GType       gimp_text_layer_get_type    (void) G_GNUC_CONST;

GimpLayer * gimp_text_layer_new         (GimpImage     *image,
                                         GimpText      *text);
GimpText  * gimp_text_layer_get_text    (GimpTextLayer *layer);
void        gimp_text_layer_set_text    (GimpTextLayer *layer,
                                         GimpText      *text);
void        gimp_text_layer_discard     (GimpTextLayer *layer);
void        gimp_text_layer_flush       (GimpTextLayer *layer);

gboolean    gimp_drawable_is_text_layer (GimpDrawable  *drawable);


#endif /* __GIMP_TEXT_LAYER_H__ */
