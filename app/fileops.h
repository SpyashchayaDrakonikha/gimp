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

#ifndef __FILEOPS_H__
#define __FILEOPS_H__

#ifdef G_OS_WIN32
#include <process.h>		/* For _getpid() */
#endif

#include "plug_in.h"


void file_ops_pre_init               (void);
void file_ops_post_init              (void);

void file_open_callback              (GtkWidget   *widget,
				      gpointer     data);

void file_save_callback              (GtkWidget   *widget,
				      gpointer     data);
void file_save_as_callback           (GtkWidget   *widget,
				      gpointer     data);

void file_revert_callback            (GtkWidget   *widget,
				      gpointer     data);

void file_open_by_extension_callback (GtkWidget   *widget,
				      gpointer     data);
void file_save_by_extension_callback (GtkWidget   *widget,
				      gpointer     data);

gint file_open                       (gchar       *filename,
				      gchar       *raw_filename);

PlugInProcDef * file_proc_find       (GSList      *procs,
				      gchar       *filename);

extern GSList *load_procs;
extern GSList *save_procs;

#endif /* __FILEOPS_H__ */
