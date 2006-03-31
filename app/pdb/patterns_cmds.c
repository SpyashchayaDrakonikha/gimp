/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"

#include <string.h>

#include <glib-object.h>

#include "pdb-types.h"
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "base/temp-buf.h"
#include "core/gimp.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimplist.h"
#include "core/gimppattern.h"

static GimpProcedure patterns_refresh_proc;
static GimpProcedure patterns_get_list_proc;
static GimpProcedure patterns_get_pattern_proc;
static GimpProcedure patterns_get_pattern_data_proc;

void
register_patterns_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * patterns_refresh
   */
  procedure = gimp_procedure_init (&patterns_refresh_proc, 0, 0);
  procedural_db_register (gimp, procedure);

  /*
   * patterns_get_list
   */
  procedure = gimp_procedure_init (&patterns_get_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("filter",
                                                       "filter",
                                                       "An optional regular expression used to filter the list",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-patterns",
                                                     "num patterns",
                                                     "The number of patterns in the pattern list",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("pattern-list",
                                                         "pattern list",
                                                         "The list of pattern names",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * patterns_get_pattern
   */
  procedure = gimp_procedure_init (&patterns_get_pattern_proc, 0, 3);
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("name",
                                                           "name",
                                                           "The pattern name",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("width",
                                                     "width",
                                                     "The pattern width",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("height",
                                                     "height",
                                                     "The pattern height",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * patterns_get_pattern_data
   */
  procedure = gimp_procedure_init (&patterns_get_pattern_data_proc, 1, 6);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The pattern name (\"\" means currently active pattern)",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("actual-name",
                                                           "actual name",
                                                           "The pattern name",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("width",
                                                     "width",
                                                     "The pattern width",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("height",
                                                     "height",
                                                     "The pattern height",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("mask-bpp",
                                                     "mask bpp",
                                                     "Pattern bytes per pixel",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("length",
                                                     "length",
                                                     "Length of pattern mask data",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT8ARRAY,
                                   g_param_spec_pointer ("mask-data",
                                                         "mask data",
                                                         "The pattern mask data",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
patterns_refresh_invoker (GimpProcedure *procedure,
                          Gimp          *gimp,
                          GimpContext   *context,
                          GimpProgress  *progress,
                          Argument      *args)
{
  gimp_data_factory_data_refresh (gimp->pattern_factory);
  return gimp_procedure_get_return_values (procedure, TRUE);
}

static GimpProcedure patterns_refresh_proc =
{
  TRUE, TRUE,
  "gimp-patterns-refresh",
  "gimp-patterns-refresh",
  "Refresh current patterns. This function always succeeds.",
  "This procedure retrieves all patterns currently in the user's pattern path and updates all pattern dialogs accordingly.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { patterns_refresh_invoker } }
};

static Argument *
patterns_get_list_invoker (GimpProcedure *procedure,
                           Gimp          *gimp,
                           GimpContext   *context,
                           GimpProgress  *progress,
                           Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *filter;
  gint32 num_patterns = 0;
  gchar **pattern_list = NULL;

  filter = (gchar *) g_value_get_string (&args[0].value);

  if (success)
    {
      pattern_list = gimp_container_get_filtered_name_array (gimp->pattern_factory->container,
                                                             filter, &num_patterns);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_patterns);
      g_value_set_pointer (&return_vals[2].value, pattern_list);
    }

  return return_vals;
}

static GimpProcedure patterns_get_list_proc =
{
  TRUE, TRUE,
  "gimp-patterns-get-list",
  "gimp-patterns-get-list",
  "Retrieve a complete listing of the available patterns.",
  "This procedure returns a complete listing of available GIMP patterns. Each name returned can be used as input to the 'gimp-context-set-pattern'.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { patterns_get_list_invoker } }
};

static Argument *
patterns_get_pattern_invoker (GimpProcedure *procedure,
                              Gimp          *gimp,
                              GimpContext   *context,
                              GimpProgress  *progress,
                              Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *name = NULL;
  gint32 width = 0;
  gint32 height = 0;

  GimpPattern *pattern = gimp_context_get_pattern (context);

  if (pattern)
    {
      name   = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
      width  = pattern->mask->width;
      height = pattern->mask->height;
    }
  else
    success = FALSE;

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals[1].value, name);
      g_value_set_int (&return_vals[2].value, width);
      g_value_set_int (&return_vals[3].value, height);
    }

  return return_vals;
}

static GimpProcedure patterns_get_pattern_proc =
{
  TRUE, TRUE,
  "gimp-patterns-get-pattern",
  "gimp-patterns-get-pattern",
  "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
  "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
  "",
  "",
  "",
  "gimp-context-get-pattern",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { patterns_get_pattern_invoker } }
};

static Argument *
patterns_get_pattern_data_invoker (GimpProcedure *procedure,
                                   Gimp          *gimp,
                                   GimpContext   *context,
                                   GimpProgress  *progress,
                                   Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *name;
  gchar *actual_name = NULL;
  gint32 width = 0;
  gint32 height = 0;
  gint32 mask_bpp = 0;
  gint32 length = 0;
  guint8 *mask_data = NULL;

  name = (gchar *) g_value_get_string (&args[0].value);

  if (success)
    {
      GimpPattern *pattern;

      if (name && strlen (name))
        {
          pattern = (GimpPattern *)
            gimp_container_get_child_by_name (gimp->pattern_factory->container,
                                              name);
        }
      else
        {
          pattern = gimp_context_get_pattern (context);
        }

      if (pattern)
        {
          actual_name = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
          width       = pattern->mask->width;
          height      = pattern->mask->height;
          mask_bpp    = pattern->mask->bytes;
          length      = pattern->mask->height * pattern->mask->width *
                        pattern->mask->bytes;
          mask_data   = g_memdup (temp_buf_data (pattern->mask), length);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals[1].value, actual_name);
      g_value_set_int (&return_vals[2].value, width);
      g_value_set_int (&return_vals[3].value, height);
      g_value_set_int (&return_vals[4].value, mask_bpp);
      g_value_set_int (&return_vals[5].value, length);
      g_value_set_pointer (&return_vals[6].value, mask_data);
    }

  return return_vals;
}

static GimpProcedure patterns_get_pattern_data_proc =
{
  TRUE, TRUE,
  "gimp-patterns-get-pattern-data",
  "gimp-patterns-get-pattern-data",
  "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
  "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
  "",
  "",
  "",
  "gimp-pattern-get-pixels",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { patterns_get_pattern_data_invoker } }
};
