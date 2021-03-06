/* GIMP - The GNU Image Manipulation Program
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

#ifndef __GIMP_UNDO_EDITOR_H__
#define __GIMP_UNDO_EDITOR_H__


#include "gimpimageeditor.h"


#define GIMP_TYPE_UNDO_EDITOR            (gimp_undo_editor_get_type ())
#define GIMP_UNDO_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_UNDO_EDITOR, GimpUndoEditor))
#define GIMP_UNDO_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_UNDO_EDITOR, GimpUndoEditorClass))
#define GIMP_IS_UNDO_EDITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_UNDO_EDITOR))
#define GIMP_IS_UNDO_EDITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_UNDO_EDITOR))
#define GIMP_UNDO_EDITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_UNDO_EDITOR, GimpUndoEditorClass))


typedef struct _GimpUndoEditorClass GimpUndoEditorClass;

struct _GimpUndoEditor
{
  GimpImageEditor  parent_instance;

  GimpContext     *context;
  GimpContainer   *container;
  GtkWidget       *view;
  GimpViewSize     view_size;

  GimpUndo        *base_item;

  GtkWidget       *undo_button;
  GtkWidget       *redo_button;
  GtkWidget       *clear_button;
};

struct _GimpUndoEditorClass
{
  GimpImageEditorClass  parent_class;
};


GType       gimp_undo_editor_get_type  (void) G_GNUC_CONST;

GtkWidget * gimp_undo_editor_new       (GimpCoreConfig  *config,
                                        GimpMenuFactory *menu_factory);


#endif /* __GIMP_UNDO_EDITOR_H__ */
