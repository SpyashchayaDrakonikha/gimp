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
#include <stdio.h>
#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "core-types.h"

#include "gimp.h"
#include "gimp-parasites.h"
#include "gimpchannel.h"
#include "gimpimage.h"
#include "gimpimage-undo.h"
#include "gimpimage-undo-push.h"
#include "gimpitem.h"
#include "gimplayer.h"
#include "gimplist.h"
#include "gimpmarshal.h"
#include "gimppaintinfo.h"
#include "gimpparasitelist.h"

#include "vectors/gimpvectors.h"

#include "gimp-intl.h"


enum
{
  REMOVED,
  VISIBILITY_CHANGED,
  LINKED_CHANGED,
  LAST_SIGNAL
};


/*  local function prototypes  */

static void       gimp_item_class_init     (GimpItemClass *klass);
static void       gimp_item_init           (GimpItem      *item);

static void       gimp_item_finalize       (GObject       *object);

static void       gimp_item_name_changed   (GimpObject    *object);
static gsize      gimp_item_get_memsize    (GimpObject    *object,
                                            gsize         *gui_size);

static GimpItem * gimp_item_real_duplicate (GimpItem      *item,
                                            GType          new_type,
                                            gboolean       add_alpha);
static GimpItem * gimp_item_real_convert   (GimpItem      *item,
                                            GimpImage     *dest_image,
                                            GType          new_type,
                                            gboolean       add_alpha);
static void       gimp_item_real_rename    (GimpItem      *item,
                                            const gchar   *new_name,
                                            const gchar   *undo_desc);
static void       gimp_item_real_translate (GimpItem      *item,
                                            gint           offset_x,
                                            gint           offset_y,
                                            gboolean       push_undo);
static void       gimp_item_real_scale     (GimpItem      *item,
                                            gint           new_width,
                                            gint           new_height,
                                            gint           new_offset_x,
                                            gint           new_offset_y,
                                            GimpInterpolationType  interpolation);
static void       gimp_item_real_resize    (GimpItem      *item,
                                            gint           new_width,
                                            gint           new_height,
                                            gint           offset_x,
                                            gint           offset_y);


/*  private variables  */

static guint gimp_item_signals[LAST_SIGNAL] = { 0 };

static GimpViewableClass *parent_class = NULL;


GType
gimp_item_get_type (void)
{
  static GType item_type = 0;

  if (! item_type)
    {
      static const GTypeInfo item_info =
      {
        sizeof (GimpItemClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_item_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpItem),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_item_init,
      };

      item_type = g_type_register_static (GIMP_TYPE_VIEWABLE,
                                          "GimpItem",
                                          &item_info, 0);
    }

  return item_type;
}

static void
gimp_item_class_init (GimpItemClass *klass)
{
  GObjectClass    *object_class;
  GimpObjectClass *gimp_object_class;

  object_class      = G_OBJECT_CLASS (klass);
  gimp_object_class = GIMP_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  gimp_item_signals[REMOVED] =
    g_signal_new ("removed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpItemClass, removed),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  gimp_item_signals[VISIBILITY_CHANGED] =
    g_signal_new ("visibility_changed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (GimpItemClass, visibility_changed),
                  NULL, NULL,
                  gimp_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  gimp_item_signals[LINKED_CHANGED] =
    g_signal_new ("linked_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpItemClass, linked_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  object_class->finalize          = gimp_item_finalize;

  gimp_object_class->name_changed = gimp_item_name_changed;
  gimp_object_class->get_memsize  = gimp_item_get_memsize;

  klass->removed                  = NULL;
  klass->visibility_changed       = NULL;
  klass->linked_changed           = NULL;

  klass->duplicate                = gimp_item_real_duplicate;
  klass->convert                  = gimp_item_real_convert;
  klass->rename                   = gimp_item_real_rename;
  klass->translate                = gimp_item_real_translate;
  klass->scale                    = gimp_item_real_scale;
  klass->resize                   = gimp_item_real_resize;
  klass->flip                     = NULL;
  klass->rotate                   = NULL;
  klass->transform                = NULL;
  klass->stroke                   = NULL;
}

static void
gimp_item_init (GimpItem *item)
{
  item->ID        = 0;
  item->tattoo    = 0;
  item->gimage    = NULL;
  item->parasites = gimp_parasite_list_new ();
  item->width     = 0;
  item->height    = 0;
  item->offset_x  = 0;
  item->offset_y  = 0;
  item->visible   = FALSE;
  item->linked    = FALSE;
}

static void
gimp_item_finalize (GObject *object)
{
  GimpItem *item = GIMP_ITEM (object);

  if (item->gimage && item->gimage->gimp)
    {
      g_hash_table_remove (item->gimage->gimp->item_table,
			   GINT_TO_POINTER (item->ID));
      item->gimage = NULL;
    }

  if (item->parasites)
    {
      g_object_unref (item->parasites);
      item->parasites = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_item_name_changed (GimpObject *object)
{
  GimpItem *item;
  GimpList *list = NULL;

  item = GIMP_ITEM (object);

  /*  if no other items to check name against  */
  if (item->gimage == NULL)
    return;

  if (GIMP_IS_LAYER (item))
    list = GIMP_LIST (item->gimage->layers);
  else if (GIMP_IS_CHANNEL (item))
    list = GIMP_LIST (item->gimage->channels);
  else if (GIMP_IS_VECTORS (item))
    list = GIMP_LIST (item->gimage->vectors);

  if (list)
    gimp_list_uniquefy_name (list, object, FALSE);
}

static gsize
gimp_item_get_memsize (GimpObject *object,
                       gsize      *gui_size)
{
  GimpItem *item;
  gsize     memsize = 0;

  item = GIMP_ITEM (object);

  memsize += gimp_object_get_memsize (GIMP_OBJECT (item->parasites), gui_size);

  return memsize + GIMP_OBJECT_CLASS (parent_class)->get_memsize (object,
                                                                  gui_size);
}

static GimpItem *
gimp_item_real_duplicate (GimpItem *item,
                          GType     new_type,
                          gboolean  add_alpha)
{
  GimpItem *new_item;
  gchar    *new_name;

  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);
  g_return_val_if_fail (GIMP_IS_IMAGE (item->gimage), NULL);
  g_return_val_if_fail (g_type_is_a (new_type, GIMP_TYPE_ITEM), NULL);

  /*  formulate the new name  */
  {
    const gchar *name;
    gchar       *ext;
    gint         number;
    gint         len;

    name = gimp_object_get_name (GIMP_OBJECT (item));

    g_return_val_if_fail (name != NULL, NULL);

    ext = strrchr (name, '#');
    len = strlen (_("copy"));

    if ((strlen (name) >= len &&
         strcmp (&name[strlen (name) - len], _("copy")) == 0) ||
        (ext && (number = atoi (ext + 1)) > 0 &&
         ((int)(log10 (number) + 1)) == strlen (ext + 1)))
      {
        /* don't have redundant "copy"s */
        new_name = g_strdup (name);
      }
    else
      {
        new_name = g_strdup_printf (_("%s copy"), name);
      }
  }

  new_item = g_object_new (new_type, NULL);

  gimp_item_configure (new_item, gimp_item_get_image (item),
                       item->offset_x, item->offset_y,
                       item->width, item->height,
                       new_name);

  g_free (new_name);

  g_object_unref (new_item->parasites);
  new_item->parasites = gimp_parasite_list_copy (item->parasites);

  new_item->visible = item->visible;
  new_item->linked  = item->linked;

  return new_item;
}

static GimpItem *
gimp_item_real_convert (GimpItem  *item,
                        GimpImage *dest_image,
                        GType      new_type,
                        gboolean   add_alpha)
{
  GimpItem *new_item;

  new_item = gimp_item_duplicate (item, new_type, add_alpha);

  if (dest_image != item->gimage)
    {
      gimp_item_set_image (new_item, dest_image);

      /*  force a unique name  */
      gimp_object_name_changed (GIMP_OBJECT (new_item));
    }

  return new_item;
}

static void
gimp_item_real_rename (GimpItem    *item,
                       const gchar *new_name,
                       const gchar *undo_desc)
{
  if (item->gimage)
    gimp_image_undo_push_item_rename (item->gimage, undo_desc, item);

  gimp_object_set_name (GIMP_OBJECT (item), new_name);
}

static void
gimp_item_real_translate (GimpItem *item,
                          gint      offset_x,
                          gint      offset_y,
                          gboolean  push_undo)
{
  item->offset_x += offset_x;
  item->offset_y += offset_y;
}

static void
gimp_item_real_scale (GimpItem              *item,
                      gint                   new_width,
                      gint                   new_height,
                      gint                   new_offset_x,
                      gint                   new_offset_y,
                      GimpInterpolationType  interpolation)
{
  item->width     = new_width;
  item->height    = new_height;
  item->offset_x  = new_offset_x;
  item->offset_y  = new_offset_y;
}

static void
gimp_item_real_resize (GimpItem *item,
                       gint      new_width,
                       gint      new_height,
                       gint      offset_x,
                       gint      offset_y)
{
  item->offset_x = item->offset_x - offset_x;
  item->offset_y = item->offset_y - offset_y;
  item->width    = new_width;
  item->height   = new_height;
}

void
gimp_item_removed (GimpItem *item)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  g_signal_emit (item, gimp_item_signals[REMOVED], 0);
}

void
gimp_item_configure (GimpItem    *item,
                     GimpImage   *gimage,
                     gint         offset_x,
                     gint         offset_y,
                     gint         width,
                     gint         height,
                     const gchar *name)
{
  g_return_if_fail (GIMP_IS_ITEM (item));
  g_return_if_fail (GIMP_IS_IMAGE (gimage));

  if (item->ID == 0)
    {
      item->ID = gimage->gimp->next_item_ID++;

      g_hash_table_insert (gimage->gimp->item_table,
                           GINT_TO_POINTER (item->ID),
                           item);

      gimp_item_set_image (item, gimage);
    }

  item->width    = width;
  item->height   = height;
  item->offset_x = offset_x;
  item->offset_y = offset_y;

  gimp_object_set_name (GIMP_OBJECT (item), name ? name : _("Unnamed"));
}

GimpItem *
gimp_item_duplicate (GimpItem *item,
                     GType     new_type,
                     gboolean  add_alpha)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);
  g_return_val_if_fail (GIMP_IS_IMAGE (item->gimage), NULL);
  g_return_val_if_fail (g_type_is_a (new_type, GIMP_TYPE_ITEM), NULL);

  return GIMP_ITEM_GET_CLASS (item)->duplicate (item, new_type, add_alpha);
}

GimpItem *
gimp_item_convert (GimpItem  *item,
                   GimpImage *dest_image,
                   GType      new_type,
                   gboolean   add_alpha)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);
  g_return_val_if_fail (GIMP_IS_IMAGE (item->gimage), NULL);
  g_return_val_if_fail (GIMP_IS_IMAGE (dest_image), NULL);
  g_return_val_if_fail (g_type_is_a (new_type, GIMP_TYPE_ITEM), NULL);

  return GIMP_ITEM_GET_CLASS (item)->convert (item, dest_image,
                                              new_type, add_alpha);
}

void
gimp_item_rename (GimpItem    *item,
                  const gchar *new_name)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  item_class = GIMP_ITEM_GET_CLASS (item);

  if (! new_name || ! *new_name)
    new_name = item_class->default_name;

  if (strcmp (new_name, gimp_object_get_name (GIMP_OBJECT (item))))
    item_class->rename (item, new_name, item_class->rename_desc);
}

gint
gimp_item_width (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), -1);

  return item->width;
}

gint
gimp_item_height (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), -1);

  return item->height;
}

void
gimp_item_offsets (const GimpItem *item,
                   gint           *off_x,
                   gint           *off_y)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  if (off_x) *off_x = item->offset_x;
  if (off_y) *off_y = item->offset_y;
}

void
gimp_item_translate (GimpItem *item,
                     gint      off_x,
                     gint      off_y,
                     gboolean  push_undo)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->translate (item, off_x, off_y, push_undo);
}

/**
 * gimp_item_check_scaling:
 * @item:       Item to check
 * @new_width:  proposed width of item, in pixels
 * @new_height: proposed height of item, in pixels
 *
 * Scales item dimensions, then snaps them to pixel centers
 *
 * Returns: #FALSE if any dimension reduces to zero as a result
 *          of this; otherwise, returns #TRUE.
 **/
gboolean
gimp_item_check_scaling (const GimpItem *item,
                         gint            new_width,
                         gint            new_height)
{
  GimpImage *gimage;
  gdouble    img_scale_w;
  gdouble    img_scale_h;
  gint       new_item_width;
  gint       new_item_height;

  g_return_val_if_fail (GIMP_IS_ITEM (item), FALSE);

  gimage = gimp_item_get_image (item);

  img_scale_w     = (gdouble) new_width  / (gdouble) gimage->width;
  img_scale_h     = (gdouble) new_height / (gdouble) gimage->height;
  new_item_width  = ROUND (img_scale_w * (gdouble) item->width);
  new_item_height = ROUND (img_scale_h * (gdouble) item->height);

  return (new_item_width > 0 && new_item_height > 0);
}

void
gimp_item_scale (GimpItem              *item,
                 gint                   new_width,
                 gint                   new_height,
                 gint                   new_offset_x,
                 gint                   new_offset_y,
                 GimpInterpolationType  interpolation)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  if (new_width < 1 || new_height < 1)
    return;

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->scale (item, new_width, new_height, new_offset_x, new_offset_y,
                     interpolation);
}

/**
 * gimp_item_scale_by_factors:
 * @item:     Item to be transformed by explicit width and height factors.
 * @w_factor: scale factor to apply to width and horizontal offset
 * @h_factor: scale factor to apply to height and vertical offset
 * @interpolation:
 *
 * Scales item dimensions and offsets by uniform width and
 * height factors.
 *
 * Use gimp_item_scale_by_factors() in circumstances when the
 * same width and height scaling factors are to be uniformly
 * applied to a set of items. In this context, the item's
 * dimensions and offsets from the sides of the containing
 * image all change by these predetermined factors. By fiat,
 * the fixed point of the transform is the upper left hand
 * corner of the image. Returns gboolean FALSE if a requested
 * scale factor is zero or if a scaling zero's out a item
 * dimension; returns #TRUE otherwise.
 *
 * Use gimp_item_scale() in circumstances where new item width
 * and height dimensions are predetermined instead.
 *
 * Side effects: Undo set created for item. Old item imagery
 *               scaled & painted to new item tiles.
 *
 * Returns: #TRUE, if the scaled item has positive dimensions
 *          #FALSE if the scaled item has at least one zero dimension
 **/
gboolean
gimp_item_scale_by_factors (GimpItem              *item,
                            gdouble                w_factor,
                            gdouble                h_factor,
                            GimpInterpolationType  interpolation)
{
  gint new_width, new_height;
  gint new_offset_x, new_offset_y;

  g_return_val_if_fail (GIMP_IS_ITEM (item), FALSE);

  if (w_factor == 0.0 || h_factor == 0.0)
    {
      g_message ("gimp_item_scale_by_factors: Error. Requested width or height scale equals zero.");
      return FALSE;
    }

  new_offset_x = ROUND (w_factor * (gdouble) item->offset_x);
  new_offset_y = ROUND (h_factor * (gdouble) item->offset_y);
  new_width    = ROUND (w_factor * (gdouble) item->width);
  new_height   = ROUND (h_factor * (gdouble) item->height);

  if (new_width != 0 && new_height != 0)
    {
      gimp_item_scale (item,
                       new_width, new_height,
                       new_offset_x, new_offset_y,
                       interpolation);
      return TRUE;
    }

  return FALSE;
}

/**
 * gimp_item_scale_by_origin:
 * @item:         The item to be transformed by width & height scale factors
 * @new_width:    The width that item will acquire
 * @new_height:   The height that the item will acquire
 * @interpolation:
 * @local_origin: sets fixed point of the scaling transform. See below.
 *
 * Sets item dimensions to new_width and
 * new_height. Derives vertical and horizontal scaling
 * transforms from new width and height. If local_origin is
 * TRUE, the fixed point of the scaling transform coincides
 * with the item's center point.  Otherwise, the fixed
 * point is taken to be [-item->offset_x, -item->offset_y].
 *
 * Since this function derives scale factors from new and
 * current item dimensions, these factors will vary from
 * item to item because of aliasing artifacts; factor
 * variations among items can be quite large where item
 * dimensions approach pixel dimensions. Use
 * gimp_item_scale_by_factors() where constant scales are to
 * be uniformly applied to a number of items.
 *
 * Side effects: undo set created for item.
 *               Old item imagery scaled
 *               & painted to new item tiles
 **/
void
gimp_item_scale_by_origin (GimpItem              *item,
                           gint                   new_width,
                           gint                   new_height,
                           GimpInterpolationType  interpolation,
                           gboolean               local_origin)
{
  gint new_offset_x, new_offset_y;

  g_return_if_fail (GIMP_IS_ITEM (item));

  if (new_width == 0 || new_height == 0)
    {
      g_message ("gimp_layer_scale: Error. Requested width or height equals zero.");
      return;
    }

  if (local_origin)
    {
      new_offset_x = item->offset_x + ((item->width  - new_width)  / 2.0);
      new_offset_y = item->offset_y + ((item->height - new_height) / 2.0);
    }
  else
    {
      new_offset_x = (gint) (((gdouble) new_width *
                              (gdouble) item->offset_x /
                              (gdouble) item->width));

      new_offset_y = (gint) (((gdouble) new_height *
                              (gdouble) item->offset_y /
                              (gdouble) item->height));
    }

  gimp_item_scale (item,
                   new_width, new_height,
                   new_offset_x, new_offset_y,
                   interpolation);
}

void
gimp_item_resize (GimpItem *item,
                  gint      new_width,
                  gint      new_height,
                  gint      offset_x,
                  gint      offset_y)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  if (new_width < 1 || new_height < 1)
    return;

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->resize (item, new_width, new_height, offset_x, offset_y);
}

void
gimp_item_flip (GimpItem            *item,
                GimpOrientationType  flip_type,
                gdouble              axis,
                gboolean             clip_result)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->flip (item, flip_type, axis, clip_result);
}

void
gimp_item_rotate (GimpItem         *item,
                  GimpRotationType  rotate_type,
                  gdouble           center_x,
                  gdouble           center_y,
                  gboolean          clip_result)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->rotate (item, rotate_type, center_x, center_y, clip_result);
}

void
gimp_item_transform (GimpItem               *item,
                     const GimpMatrix3      *matrix,
                     GimpTransformDirection  direction,
                     GimpInterpolationType   interpolation,
                     gboolean                clip_result,
                     GimpProgressFunc        progress_callback,
                     gpointer                progress_data)
{
  GimpItemClass *item_class;

  g_return_if_fail (GIMP_IS_ITEM (item));

  item_class = GIMP_ITEM_GET_CLASS (item);

  item_class->transform (item, matrix, direction, interpolation,
                         clip_result,
                         progress_callback, progress_data);
}

gboolean
gimp_item_stroke (GimpItem      *item,
                  GimpDrawable  *drawable,
                  GimpPaintInfo *paint_info)
{
  GimpItemClass *item_class;

  g_return_val_if_fail (GIMP_IS_ITEM (item), FALSE);
  g_return_val_if_fail (GIMP_IS_DRAWABLE (drawable), FALSE);
  g_return_val_if_fail (GIMP_IS_PAINT_INFO (paint_info), FALSE);

  item_class = GIMP_ITEM_GET_CLASS (item);

  if (item_class->stroke)
    return item_class->stroke (item, drawable, paint_info);

  return FALSE;
}

gint
gimp_item_get_ID (GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), -1);

  return item->ID;
}

GimpItem *
gimp_item_get_by_ID (Gimp *gimp,
                     gint  item_id)
{
  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  if (gimp->item_table == NULL)
    return NULL;

  return (GimpItem *) g_hash_table_lookup (gimp->item_table,
                                           GINT_TO_POINTER (item_id));
}

GimpTattoo
gimp_item_get_tattoo (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), 0);

  return item->tattoo;
}

void
gimp_item_set_tattoo (GimpItem   *item,
                      GimpTattoo  tattoo)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  item->tattoo = tattoo;
}

GimpImage *
gimp_item_get_image (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);

  return item->gimage;
}

void
gimp_item_set_image (GimpItem  *item,
                     GimpImage *gimage)
{
  g_return_if_fail (GIMP_IS_ITEM (item));
  g_return_if_fail (! gimage || GIMP_IS_IMAGE (gimage));

  if (gimage == NULL)
    {
      item->tattoo = 0;
    }
  else if (item->tattoo == 0 || item->gimage != gimage)
    {
      item->tattoo = gimp_image_get_new_tattoo (gimage);
    }

  item->gimage = gimage;
}

void
gimp_item_parasite_attach (GimpItem     *item,
                           GimpParasite *parasite)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  /*  only set the dirty bit manually if we can be saved and the new
   *  parasite differs from the current one and we arn't undoable
   */
  if (gimp_parasite_is_undoable (parasite))
    {
      /* do a group in case we have attach_parent set */
      gimp_image_undo_group_start (item->gimage, GIMP_UNDO_GROUP_PARASITE_ATTACH,
                                   _("Attach Parasite"));

      gimp_image_undo_push_item_parasite (item->gimage, NULL, item, parasite);
    }
  else if (gimp_parasite_is_persistent (parasite) &&
	   ! gimp_parasite_compare (parasite,
				    gimp_item_parasite_find
				    (item, gimp_parasite_name (parasite))))
    {
      gimp_image_undo_push_cantundo (item->gimage,
                                     _("Attach Parasite to Item"));
    }

  gimp_parasite_list_add (item->parasites, parasite);

  if (gimp_parasite_has_flag (parasite, GIMP_PARASITE_ATTACH_PARENT))
    {
      gimp_parasite_shift_parent (parasite);
      gimp_image_parasite_attach (item->gimage, parasite);
    }
  else if (gimp_parasite_has_flag (parasite, GIMP_PARASITE_ATTACH_GRANDPARENT))
    {
      gimp_parasite_shift_parent (parasite);
      gimp_parasite_shift_parent (parasite);
      gimp_parasite_attach (item->gimage->gimp, parasite);
    }

  if (gimp_parasite_is_undoable (parasite))
    {
      gimp_image_undo_group_end (item->gimage);
    }
}

void
gimp_item_parasite_detach (GimpItem    *item,
                           const gchar *name)
{
  GimpParasite *parasite;

  g_return_if_fail (GIMP_IS_ITEM (item));

  parasite = gimp_parasite_list_find (item->parasites, name);

  if (! parasite)
    return;

  if (gimp_parasite_is_undoable (parasite))
    {
      gimp_image_undo_push_item_parasite_remove (item->gimage,
                                                 _("Remove Parasite from Item"),
                                                 item,
                                                 gimp_parasite_name (parasite));
    }
  else if (gimp_parasite_is_persistent (parasite))
    {
      gimp_image_undo_push_cantundo (item->gimage,
                                     _("Remove Parasite from Item"));
    }

  gimp_parasite_list_remove (item->parasites, name);
}

GimpParasite *
gimp_item_parasite_find (const GimpItem *item,
                         const gchar    *name)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);

  return gimp_parasite_list_find (item->parasites, name);
}

static void
gimp_item_parasite_list_foreach_func (gchar          *name,
                                      GimpParasite   *parasite,
                                      gchar        ***cur)
{
  *(*cur)++ = (gchar *) g_strdup (name);
}

gchar **
gimp_item_parasite_list (const GimpItem *item,
                         gint           *count)
{
  gchar **list;
  gchar **cur;

  g_return_val_if_fail (GIMP_IS_ITEM (item), NULL);
  g_return_val_if_fail (count != NULL, NULL);

  *count = gimp_parasite_list_length (item->parasites);

  cur = list = g_new (gchar *, *count);

  gimp_parasite_list_foreach (item->parasites,
			      (GHFunc) gimp_item_parasite_list_foreach_func,
			      &cur);

  return list;
}

gboolean
gimp_item_get_visible (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), FALSE);

  return item->visible;
}

void
gimp_item_set_visible (GimpItem *item,
                       gboolean  visible,
                       gboolean  push_undo)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  if (item->visible != visible)
    {
      if (push_undo)
        {
          GimpImage *gimage = gimp_item_get_image (item);

          if (gimage)
            gimp_image_undo_push_item_visibility (gimage, NULL, item);
        }

      item->visible = visible ? TRUE : FALSE;

      g_signal_emit (item, gimp_item_signals[VISIBILITY_CHANGED], 0);
    }
}

void
gimp_item_set_linked (GimpItem *item,
                      gboolean  linked,
                      gboolean  push_undo)
{
  g_return_if_fail (GIMP_IS_ITEM (item));

  if (item->linked != linked)
    {
      if (push_undo)
        {
          GimpImage *gimage = gimp_item_get_image (item);

          if (gimage)
            gimp_image_undo_push_item_linked (gimage, NULL, item);
        }

      item->linked = linked ? TRUE : FALSE;

      g_signal_emit (item, gimp_item_signals[LINKED_CHANGED], 0);
    }
}

gboolean
gimp_item_get_linked (const GimpItem *item)
{
  g_return_val_if_fail (GIMP_IS_ITEM (item), FALSE);

  return item->linked;
}
