/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimppatheditor.h
 * Copyright (C) 1999-2003 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_PATH_EDITOR_H__
#define __GIMP_PATH_EDITOR_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


#define GIMP_TYPE_PATH_EDITOR            (gimp_path_editor_get_type ())
#define GIMP_PATH_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_PATH_EDITOR, GimpPathEditor))
#define GIMP_PATH_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PATH_EDITOR, GimpPathEditorClass))
#define GIMP_IS_PATH_EDITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE (obj, GIMP_TYPE_PATH_EDITOR))
#define GIMP_IS_PATH_EDITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PATH_EDITOR))
#define GIMP_PATH_EDITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_PATH_EDITOR, GimpPathEditorClass))


typedef struct _GimpPathEditorClass  GimpPathEditorClass;

struct _GimpPathEditor
{
  GtkVBox           parent_instance;

  GtkWidget        *upper_hbox;

  GtkWidget        *new_button;
  GtkWidget        *up_button;
  GtkWidget        *down_button;
  GtkWidget        *delete_button;

  GtkWidget        *file_entry;

  GtkListStore     *dir_list;

  GtkTreeSelection *sel;
  GtkTreePath      *sel_path;

  gint              num_items;
};

struct _GimpPathEditorClass
{
  GtkVBoxClass  parent_class;

  void (* path_changed) (GimpPathEditor *editor);
};


/* For information look into the C source or the html documentation */

GType       gimp_path_editor_get_type (void) G_GNUC_CONST;

GtkWidget * gimp_path_editor_new      (const gchar    *filesel_title,
				       const gchar    *path);

gchar     * gimp_path_editor_get_path (GimpPathEditor *editor);
void        gimp_path_editor_set_path (GimpPathEditor *editor,
                                       const gchar    *path);


G_END_DECLS

#endif /* __GIMP_PATH_EDITOR_H__ */
