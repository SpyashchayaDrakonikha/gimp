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

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <glib-object.h>

#include "libgimpmath/gimpmath.h"

#include "core-types.h"

#include "gimpimage.h"
#include "gimplayer.h"
#include "gimplayermask.h"
#include "gimpmarshal.h"

#include "gimp-intl.h"


enum
{
  APPLY_CHANGED,
  EDIT_CHANGED,
  SHOW_CHANGED,
  LAST_SIGNAL
};


static void       gimp_layer_mask_class_init  (GimpLayerMaskClass *klass);
static void       gimp_layer_mask_init        (GimpLayerMask      *layer_mask);

static gboolean   gimp_layer_mask_is_attached (GimpItem           *item);
static GimpItem * gimp_layer_mask_duplicate   (GimpItem           *item,
                                               GType               new_type,
                                               gboolean            add_alpha);


static guint  layer_mask_signals[LAST_SIGNAL] = { 0 };

static GimpChannelClass *parent_class = NULL;


GType
gimp_layer_mask_get_type (void)
{
  static GType layer_mask_type = 0;

  if (! layer_mask_type)
    {
      static const GTypeInfo layer_mask_info =
      {
        sizeof (GimpLayerMaskClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_layer_mask_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpLayerMask),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_layer_mask_init,
      };

      layer_mask_type = g_type_register_static (GIMP_TYPE_CHANNEL,
						"GimpLayerMask",
						&layer_mask_info, 0);
    }

  return layer_mask_type;
}

static void
gimp_layer_mask_class_init (GimpLayerMaskClass *klass)
{
  GimpItemClass *item_class;

  item_class = GIMP_ITEM_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  layer_mask_signals[APPLY_CHANGED] =
    g_signal_new ("apply_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpLayerMaskClass, apply_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  layer_mask_signals[EDIT_CHANGED] =
    g_signal_new ("edit_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpLayerMaskClass, edit_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  layer_mask_signals[SHOW_CHANGED] =
    g_signal_new ("show_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpLayerMaskClass, show_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  item_class->is_attached = gimp_layer_mask_is_attached;
  item_class->duplicate   = gimp_layer_mask_duplicate;
}

static void
gimp_layer_mask_init (GimpLayerMask *layer_mask)
{
  layer_mask->layer      = NULL;
  layer_mask->apply_mask = TRUE;
  layer_mask->edit_mask  = TRUE;
  layer_mask->show_mask  = FALSE;
}

static gboolean
gimp_layer_mask_is_attached (GimpItem *item)
{
  GimpLayerMask *mask  = GIMP_LAYER_MASK (item);
  GimpLayer     *layer = gimp_layer_mask_get_layer (mask);

  return (GIMP_IS_IMAGE (item->gimage)        &&
          GIMP_IS_LAYER (layer)               &&
          gimp_layer_get_mask (layer) == mask &&
          gimp_item_is_attached (GIMP_ITEM (layer)));
}

static GimpItem *
gimp_layer_mask_duplicate (GimpItem *item,
                           GType     new_type,
                           gboolean  add_alpha)
{
  GimpLayerMask *layer_mask;
  GimpItem      *new_item;
  GimpLayerMask *new_layer_mask;

  g_return_val_if_fail (g_type_is_a (new_type, GIMP_TYPE_DRAWABLE), NULL);

  new_item = GIMP_ITEM_CLASS (parent_class)->duplicate (item, new_type,
                                                        add_alpha);

  if (! GIMP_IS_LAYER_MASK (new_item))
    return new_item;

  layer_mask     = GIMP_LAYER_MASK (item);
  new_layer_mask = GIMP_LAYER_MASK (new_item);

  new_layer_mask->apply_mask = layer_mask->apply_mask;
  new_layer_mask->edit_mask  = layer_mask->edit_mask;
  new_layer_mask->show_mask  = layer_mask->show_mask;

  return new_item;
}

GimpLayerMask *
gimp_layer_mask_new (GimpImage     *gimage,
		     gint           width,
		     gint           height,
		     const gchar   *name,
		     const GimpRGB *color)
{
  GimpLayerMask *layer_mask;

  layer_mask = g_object_new (GIMP_TYPE_LAYER_MASK, NULL);

  gimp_drawable_configure (GIMP_DRAWABLE (layer_mask),
			   gimage,
                           0, 0, width, height,
                           GIMP_GRAY_IMAGE, name);

  /*  set the layer_mask color and opacity  */
  GIMP_CHANNEL (layer_mask)->color       = *color;

  GIMP_CHANNEL (layer_mask)->show_masked = TRUE;

  /*  selection mask variables  */
  GIMP_CHANNEL (layer_mask)->x2          = width;
  GIMP_CHANNEL (layer_mask)->y2          = height;

  return layer_mask;
}

void
gimp_layer_mask_set_layer (GimpLayerMask *layer_mask,
			   GimpLayer     *layer)
{
  g_return_if_fail (GIMP_IS_LAYER_MASK (layer_mask));
  g_return_if_fail (layer == NULL || GIMP_IS_LAYER (layer));

  layer_mask->layer = layer;

  if (layer)
    {
      GIMP_ITEM (layer_mask)->offset_x = GIMP_ITEM (layer)->offset_x;
      GIMP_ITEM (layer_mask)->offset_y = GIMP_ITEM (layer)->offset_y;
    }
}

GimpLayer *
gimp_layer_mask_get_layer (const GimpLayerMask *layer_mask)
{
  g_return_val_if_fail (GIMP_IS_LAYER_MASK (layer_mask), NULL);

  return layer_mask->layer;
}

void
gimp_layer_mask_set_apply (GimpLayerMask *layer_mask,
                           gboolean       apply)
{
  g_return_if_fail (GIMP_IS_LAYER_MASK (layer_mask));

  if (layer_mask->apply_mask != apply)
    {
      layer_mask->apply_mask = apply ? TRUE : FALSE;

      if (layer_mask->layer)
        {
          GimpDrawable *drawable;

          drawable = GIMP_DRAWABLE (layer_mask->layer);

          gimp_drawable_update (drawable,
				0, 0,
				gimp_item_width  (GIMP_ITEM (drawable)),
				gimp_item_height (GIMP_ITEM (drawable)));
        }

      g_signal_emit (layer_mask, layer_mask_signals[APPLY_CHANGED], 0);
    }
}

gboolean
gimp_layer_mask_get_apply (const GimpLayerMask *layer_mask)
{
  g_return_val_if_fail (GIMP_IS_LAYER_MASK (layer_mask), FALSE);

  return layer_mask->apply_mask;
}

void
gimp_layer_mask_set_edit (GimpLayerMask *layer_mask,
                          gboolean       edit)
{
  g_return_if_fail (GIMP_IS_LAYER_MASK (layer_mask));

  if (layer_mask->edit_mask != edit)
    {
      layer_mask->edit_mask = edit ? TRUE : FALSE;

      g_signal_emit (layer_mask, layer_mask_signals[EDIT_CHANGED], 0);
    }
}

gboolean
gimp_layer_mask_get_edit (const GimpLayerMask *layer_mask)
{
  g_return_val_if_fail (GIMP_IS_LAYER_MASK (layer_mask), FALSE);

  return layer_mask->edit_mask;
}

void
gimp_layer_mask_set_show (GimpLayerMask *layer_mask,
                          gboolean       show)
{
  g_return_if_fail (GIMP_IS_LAYER_MASK (layer_mask));

  if (layer_mask->show_mask != show)
    {
      layer_mask->show_mask = show ? TRUE : FALSE;

      if (layer_mask->layer)
        {
          GimpDrawable *drawable;

          drawable = GIMP_DRAWABLE (layer_mask->layer);

          gimp_drawable_update (drawable,
				0, 0,
				gimp_item_width  (GIMP_ITEM (drawable)),
				gimp_item_height (GIMP_ITEM (drawable)));
        }

      g_signal_emit (layer_mask, layer_mask_signals[SHOW_CHANGED], 0);
    }
}

gboolean
gimp_layer_mask_get_show (const GimpLayerMask *layer_mask)
{
  g_return_val_if_fail (GIMP_IS_LAYER_MASK (layer_mask), FALSE);

  return layer_mask->show_mask;
}
