/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpintstore.c
 * Copyright (C) 2004  Sven Neumann <sven@gimp.org>
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

#ifndef __GIMP_INT_STORE_H__
#define __GIMP_INT_STORE_H__

#include <gtk/gtkliststore.h>

G_BEGIN_DECLS

typedef enum
{
  GIMP_INT_STORE_VALUE,
  GIMP_INT_STORE_LABEL,
  GIMP_INT_STORE_STOCK_ID,
  GIMP_INT_STORE_PIXBUF,
  GIMP_INT_STORE_USER_DATA,
  GIMP_INT_STORE_NUM_COLUMNS
} GimpIntStoreColumns;


#define GIMP_TYPE_INT_STORE            (gimp_int_store_get_type ())
#define GIMP_INT_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_INT_STORE, GimpIntStore))
#define GIMP_INT_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_INT_STORE, GimpIntStoreClass))
#define GIMP_IS_INT_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_INT_STORE))
#define GIMP_IS_INT_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_INT_STORE))
#define GIMP_INT_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_INT_STORE, GimpIntStoreClass))


typedef struct _GimpIntStoreClass  GimpIntStoreClass;

struct _GimpIntStoreClass
{
  GtkListStoreClass  parent_instance;
};

struct _GimpIntStore
{
  GtkListStore       parent_instance;
};


GType          gimp_int_store_get_type        (void) G_GNUC_CONST;

GtkListStore * gimp_int_store_new             (void);

gboolean       gimp_int_store_lookup_by_value (GtkTreeModel  *model,
                                               gint           value,
                                               GtkTreeIter   *iter);


G_END_DECLS

#endif  /* __GIMP_INT_STORE_H__ */
