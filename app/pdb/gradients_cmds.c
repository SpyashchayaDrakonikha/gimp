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

#include "core/gimp.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimpgradient.h"
#include "core/gimplist.h"

static GimpProcedure gradients_refresh_proc;
static GimpProcedure gradients_get_list_proc;
static GimpProcedure gradients_sample_uniform_proc;
static GimpProcedure gradients_sample_custom_proc;
static GimpProcedure gradients_get_gradient_data_proc;

void
register_gradients_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * gradients_refresh
   */
  procedure = gimp_procedure_init (&gradients_refresh_proc, 0, 0);
  procedural_db_register (gimp, procedure);

  /*
   * gradients_get_list
   */
  procedure = gimp_procedure_init (&gradients_get_list_proc, 1, 2);
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
                                   g_param_spec_int ("num-gradients",
                                                     "num gradients",
                                                     "The number of loaded gradients",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("gradient-list",
                                                         "gradient list",
                                                         "The list of gradient names",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * gradients_sample_uniform
   */
  procedure = gimp_procedure_init (&gradients_sample_uniform_proc, 2, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_int ("num-samples",
                                                 "num samples",
                                                 "The number of samples to take",
                                                 2, G_MAXINT32, 2,
                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("reverse",
                                                     "reverse",
                                                     "Use the reverse gradient (TRUE or FALSE)",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("array-length",
                                                     "array length",
                                                     "Length of the color_samples array (4 * num_samples)",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_FLOATARRAY,
                                   g_param_spec_pointer ("color-samples",
                                                         "color samples",
                                                         "Color samples: { R1, G1, B1, A1, ..., Rn, Gn, Bn, An }",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * gradients_sample_custom
   */
  procedure = gimp_procedure_init (&gradients_sample_custom_proc, 3, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_int ("num-samples",
                                                 "num samples",
                                                 "The number of samples to take",
                                                 0, G_MAXINT32, 0,
                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOATARRAY,
                               g_param_spec_pointer ("positions",
                                                     "positions",
                                                     "The list of positions to sample along the gradient",
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("reverse",
                                                     "reverse",
                                                     "Use the reverse gradient (TRUE or FALSE)",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("array-length",
                                                     "array length",
                                                     "Length of the color_samples array (4 * num_samples)",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_FLOATARRAY,
                                   g_param_spec_pointer ("color-samples",
                                                         "color samples",
                                                         "Color samples: { R1, G1, B1, A1, ..., Rn, Gn, Bn, An }",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * gradients_get_gradient_data
   */
  procedure = gimp_procedure_init (&gradients_get_gradient_data_proc, 3, 3);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The gradient name (\"\" means current active gradient)",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_int ("sample-size",
                                                 "sample size",
                                                 "Size of the sample to return when the gradient is changed: (1 <= sample_size <= 10000)",
                                                 1, 10000, 1,
                                                 GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("reverse",
                                                     "reverse",
                                                     "Use the reverse gradient (TRUE or FALSE)",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("actual-name",
                                                           "actual name",
                                                           "The gradient name",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("width",
                                                     "width",
                                                     "The gradient sample width (r,g,b,a)",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_FLOATARRAY,
                                   g_param_spec_pointer ("grad-data",
                                                         "grad data",
                                                         "The gradient sample data",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
gradients_refresh_invoker (GimpProcedure *procedure,
                           Gimp          *gimp,
                           GimpContext   *context,
                           GimpProgress  *progress,
                           Argument      *args)
{
  gimp_data_factory_data_refresh (gimp->gradient_factory);
  return gimp_procedure_get_return_values (procedure, TRUE);
}

static GimpProcedure gradients_refresh_proc =
{
  TRUE, TRUE,
  "gimp-gradients-refresh",
  "gimp-gradients-refresh",
  "Refresh current gradients. This function always succeeds.",
  "This procedure retrieves all gradients currently in the user's gradient path and updates the gradient dialogs accordingly.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { gradients_refresh_invoker } }
};

static Argument *
gradients_get_list_invoker (GimpProcedure *procedure,
                            Gimp          *gimp,
                            GimpContext   *context,
                            GimpProgress  *progress,
                            Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *filter;
  gint32 num_gradients = 0;
  gchar **gradient_list = NULL;

  filter = (gchar *) g_value_get_string (&args[0].value);

  if (success)
    {
      gradient_list = gimp_container_get_filtered_name_array (gimp->gradient_factory->container,
                                                              filter, &num_gradients);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_gradients);
      g_value_set_pointer (&return_vals[2].value, gradient_list);
    }

  return return_vals;
}

static GimpProcedure gradients_get_list_proc =
{
  TRUE, TRUE,
  "gimp-gradients-get-list",
  "gimp-gradients-get-list",
  "Retrieve the list of loaded gradients.",
  "This procedure returns a list of the gradients that are currently loaded. You can later use the 'gimp-context-set-gradient' function to set the active gradient.",
  "Federico Mena Quintero",
  "Federico Mena Quintero",
  "1997",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { gradients_get_list_invoker } }
};

static Argument *
gradients_sample_uniform_invoker (GimpProcedure *procedure,
                                  Gimp          *gimp,
                                  GimpContext   *context,
                                  GimpProgress  *progress,
                                  Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gint32 num_samples;
  gboolean reverse;
  gint32 array_length = 0;
  gdouble *color_samples = NULL;

  num_samples = g_value_get_int (&args[0].value);
  reverse = g_value_get_boolean (&args[1].value);

  if (success)
    {
      GimpGradient        *gradient;
      GimpGradientSegment *seg = NULL;
      gdouble              pos, delta;
      GimpRGB              color;
      gdouble             *pv;

      pos   = 0.0;
      delta = 1.0 / (num_samples - 1);

      array_length = num_samples * 4;

      pv = color_samples = g_new (gdouble, array_length);

      gradient = gimp_context_get_gradient (context);

      while (num_samples--)
        {
          seg = gimp_gradient_get_color_at (gradient, seg, pos, reverse, &color);

          *pv++ = color.r;
          *pv++ = color.g;
          *pv++ = color.b;
          *pv++ = color.a;

          pos += delta;
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, array_length);
      g_value_set_pointer (&return_vals[2].value, color_samples);
    }

  return return_vals;
}

static GimpProcedure gradients_sample_uniform_proc =
{
  TRUE, TRUE,
  "gimp-gradients-sample-uniform",
  "gimp-gradients-sample-uniform",
  "This procedure is deprecated! Use 'gimp-gradient-get-uniform-samples' instead.",
  "This procedure is deprecated! Use 'gimp-gradient-get-uniform-samples' instead.",
  "",
  "",
  "",
  "gimp-gradient-get-uniform-samples",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { gradients_sample_uniform_invoker } }
};

static Argument *
gradients_sample_custom_invoker (GimpProcedure *procedure,
                                 Gimp          *gimp,
                                 GimpContext   *context,
                                 GimpProgress  *progress,
                                 Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gint32 num_samples;
  gdouble *positions;
  gboolean reverse;
  gint32 array_length = 0;
  gdouble *color_samples = NULL;

  num_samples = g_value_get_int (&args[0].value);
  positions = g_value_get_pointer (&args[1].value);
  reverse = g_value_get_boolean (&args[2].value);

  if (success)
    {
      GimpGradient        *gradient;
      GimpGradientSegment *seg = NULL;
      GimpRGB              color;
      gdouble             *pv;

      array_length = num_samples * 4;

      pv = color_samples = g_new (gdouble, array_length);

      gradient = gimp_context_get_gradient (context);

      while (num_samples--)
        {
          seg = gimp_gradient_get_color_at (gradient, seg, *positions,
                                            reverse, &color);

          *pv++ = color.r;
          *pv++ = color.g;
          *pv++ = color.b;
          *pv++ = color.a;

          positions++;
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, array_length);
      g_value_set_pointer (&return_vals[2].value, color_samples);
    }

  return return_vals;
}

static GimpProcedure gradients_sample_custom_proc =
{
  TRUE, TRUE,
  "gimp-gradients-sample-custom",
  "gimp-gradients-sample-custom",
  "This procedure is deprecated! Use 'gimp-gradient-get-custom-samples' instead.",
  "This procedure is deprecated! Use 'gimp-gradient-get-custom-samples' instead.",
  "",
  "",
  "",
  "gimp-gradient-get-custom-samples",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { gradients_sample_custom_invoker } }
};

static Argument *
gradients_get_gradient_data_invoker (GimpProcedure *procedure,
                                     Gimp          *gimp,
                                     GimpContext   *context,
                                     GimpProgress  *progress,
                                     Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *name;
  gint32 sample_size;
  gboolean reverse;
  gchar *actual_name = NULL;
  gint32 width = 0;
  gdouble *grad_data = NULL;

  name = (gchar *) g_value_get_string (&args[0].value);
  sample_size = g_value_get_int (&args[1].value);
  reverse = g_value_get_boolean (&args[2].value);

  if (success)
    {
      GimpGradient *gradient;

      if (sample_size < 1 || sample_size > 10000)
        sample_size = GIMP_GRADIENT_DEFAULT_SAMPLE_SIZE;

      if (name && strlen (name))
        {
          gradient = (GimpGradient *)
            gimp_container_get_child_by_name (gimp->gradient_factory->container,
                                              name);
        }
      else
        {
          gradient = gimp_context_get_gradient (context);
        }

      if (gradient)
        {
          GimpGradientSegment *seg = NULL;
          gdouble             *pv;
          gdouble              pos, delta;
          GimpRGB              color;

          pos   = 0.0;
          delta = 1.0 / (sample_size - 1);

          actual_name = g_strdup (gimp_object_get_name (GIMP_OBJECT (gradient)));
          grad_data   = g_new (gdouble, sample_size * 4);
          width       = sample_size * 4;

          pv = grad_data;

          while (sample_size)
            {
              seg = gimp_gradient_get_color_at (gradient, seg, pos, reverse, &color);

              *pv++ = color.r;
              *pv++ = color.g;
              *pv++ = color.b;
              *pv++ = color.a;

              pos += delta;
            }
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals[1].value, actual_name);
      g_value_set_int (&return_vals[2].value, width);
      g_value_set_pointer (&return_vals[3].value, grad_data);
    }

  return return_vals;
}

static GimpProcedure gradients_get_gradient_data_proc =
{
  TRUE, TRUE,
  "gimp-gradients-get-gradient-data",
  "gimp-gradients-get-gradient-data",
  "This procedure is deprecated! Use 'gimp-gradient-get-uniform-samples' instead.",
  "This procedure is deprecated! Use 'gimp-gradient-get-uniform-samples' instead.",
  "",
  "",
  "",
  "gimp-gradient-get-uniform-samples",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { gradients_get_gradient_data_invoker } }
};
