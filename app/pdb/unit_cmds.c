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


#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimpunit.h"

static GimpProcedure unit_get_number_of_units_proc;
static GimpProcedure unit_get_number_of_built_in_units_proc;
static GimpProcedure unit_new_proc;
static GimpProcedure unit_get_deletion_flag_proc;
static GimpProcedure unit_set_deletion_flag_proc;
static GimpProcedure unit_get_identifier_proc;
static GimpProcedure unit_get_factor_proc;
static GimpProcedure unit_get_digits_proc;
static GimpProcedure unit_get_symbol_proc;
static GimpProcedure unit_get_abbreviation_proc;
static GimpProcedure unit_get_singular_proc;
static GimpProcedure unit_get_plural_proc;

void
register_unit_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * unit_get_number_of_units
   */
  procedure = gimp_procedure_init (&unit_get_number_of_units_proc, 0, 1);
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-units",
                                                     "num units",
                                                     "The number of units",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_number_of_built_in_units
   */
  procedure = gimp_procedure_init (&unit_get_number_of_built_in_units_proc, 0, 1);
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-units",
                                                     "num units",
                                                     "The number of built-in units",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_new
   */
  procedure = gimp_procedure_init (&unit_new_proc, 7, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("identifier",
                                                       "identifier",
                                                       "The new unit's identifier",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("factor",
                                                    "factor",
                                                    "The new unit's factor",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_int ("digits",
                                                 "digits",
                                                 "The new unit's digits",
                                                 G_MININT32, G_MAXINT32, 0,
                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("symbol",
                                                       "symbol",
                                                       "The new unit's symbol",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("abbreviation",
                                                       "abbreviation",
                                                       "The new unit's abbreviation",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("singular",
                                                       "singular",
                                                       "The new unit's singular form",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("plural",
                                                       "plural",
                                                       "The new unit's plural form",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   gimp_param_spec_unit ("unit-id",
                                                         "unit id",
                                                         "The new unit's ID",
                                                         TRUE,
                                                         FALSE,
                                                         GIMP_UNIT_PIXEL,
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_deletion_flag
   */
  procedure = gimp_procedure_init (&unit_get_deletion_flag_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_boolean ("deletion-flag",
                                                         "deletion flag",
                                                         "The unit's deletion flag",
                                                         FALSE,
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_set_deletion_flag
   */
  procedure = gimp_procedure_init (&unit_set_deletion_flag_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("deletion-flag",
                                                     "deletion flag",
                                                     "The new deletion flag of the unit",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_identifier
   */
  procedure = gimp_procedure_init (&unit_get_identifier_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("identifier",
                                                           "identifier",
                                                           "The unit's textual identifier",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_factor
   */
  procedure = gimp_procedure_init (&unit_get_factor_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_FLOAT,
                                   g_param_spec_double ("factor",
                                                        "factor",
                                                        "The unit's factor",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                        GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_digits
   */
  procedure = gimp_procedure_init (&unit_get_digits_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("digits",
                                                     "digits",
                                                     "The unit's number of digits",
                                                     G_MININT32, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_symbol
   */
  procedure = gimp_procedure_init (&unit_get_symbol_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("symbol",
                                                           "symbol",
                                                           "The unit's symbol",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_abbreviation
   */
  procedure = gimp_procedure_init (&unit_get_abbreviation_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("abbreviation",
                                                           "abbreviation",
                                                           "The unit's abbreviation",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_singular
   */
  procedure = gimp_procedure_init (&unit_get_singular_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("singular",
                                                           "singular",
                                                           "The unit's singular form",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * unit_get_plural
   */
  procedure = gimp_procedure_init (&unit_get_plural_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_unit ("unit-id",
                                                     "unit id",
                                                     "The unit's integer ID",
                                                     TRUE,
                                                     FALSE,
                                                     GIMP_UNIT_PIXEL,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRING,
                                   gimp_param_spec_string ("plural",
                                                           "plural",
                                                           "The unit's plural form",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
unit_get_number_of_units_invoker (GimpProcedure *procedure,
                                  Gimp          *gimp,
                                  GimpContext   *context,
                                  GimpProgress  *progress,
                                  Argument      *args)
{
  Argument *return_vals;
  gint32 num_units = 0;

  num_units = _gimp_unit_get_number_of_units (gimp);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);
  g_value_set_int (&return_vals[1].value, num_units);

  return return_vals;
}

static GimpProcedure unit_get_number_of_units_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-number-of-units",
  "gimp-unit-get-number-of-units",
  "Returns the number of units.",
  "This procedure returns the number of defined units.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_number_of_units_invoker } }
};

static Argument *
unit_get_number_of_built_in_units_invoker (GimpProcedure *procedure,
                                           Gimp          *gimp,
                                           GimpContext   *context,
                                           GimpProgress  *progress,
                                           Argument      *args)
{
  Argument *return_vals;
  gint32 num_units = 0;

  num_units = _gimp_unit_get_number_of_built_in_units (gimp);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);
  g_value_set_int (&return_vals[1].value, num_units);

  return return_vals;
}

static GimpProcedure unit_get_number_of_built_in_units_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-number-of-built-in-units",
  "gimp-unit-get-number-of-built-in-units",
  "Returns the number of built-in units.",
  "This procedure returns the number of defined units built-in to the GIMP.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_number_of_built_in_units_invoker } }
};

static Argument *
unit_new_invoker (GimpProcedure *procedure,
                  Gimp          *gimp,
                  GimpContext   *context,
                  GimpProgress  *progress,
                  Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *identifier;
  gdouble factor;
  gint32 digits;
  gchar *symbol;
  gchar *abbreviation;
  gchar *singular;
  gchar *plural;
  GimpUnit unit_id = 0;

  identifier = (gchar *) g_value_get_string (&args[0].value);
  factor = g_value_get_double (&args[1].value);
  digits = g_value_get_int (&args[2].value);
  symbol = (gchar *) g_value_get_string (&args[3].value);
  abbreviation = (gchar *) g_value_get_string (&args[4].value);
  singular = (gchar *) g_value_get_string (&args[5].value);
  plural = (gchar *) g_value_get_string (&args[6].value);

  if (success)
    {
      unit_id = _gimp_unit_new (gimp, identifier, factor, digits,
                                symbol, abbreviation, singular, plural);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_int (&return_vals[1].value, unit_id);

  return return_vals;
}

static GimpProcedure unit_new_proc =
{
  TRUE, TRUE,
  "gimp-unit-new",
  "gimp-unit-new",
  "Creates a new unit and returns it's integer ID.",
  "This procedure creates a new unit and returns it's integer ID. Note that the new unit will have it's deletion flag set to TRUE, so you will have to set it to FALSE with gimp_unit_set_deletion_flag to make it persistent.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_new_invoker } }
};

static Argument *
unit_get_deletion_flag_invoker (GimpProcedure *procedure,
                                Gimp          *gimp,
                                GimpContext   *context,
                                GimpProgress  *progress,
                                Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gboolean deletion_flag = FALSE;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      deletion_flag = _gimp_unit_get_deletion_flag (gimp, unit_id);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_boolean (&return_vals[1].value, deletion_flag);

  return return_vals;
}

static GimpProcedure unit_get_deletion_flag_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-deletion-flag",
  "gimp-unit-get-deletion-flag",
  "Returns the deletion flag of the unit.",
  "This procedure returns the deletion flag of the unit. If this value is TRUE the unit's definition will not be saved in the user's unitrc file on gimp exit.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_deletion_flag_invoker } }
};

static Argument *
unit_set_deletion_flag_invoker (GimpProcedure *procedure,
                                Gimp          *gimp,
                                GimpContext   *context,
                                GimpProgress  *progress,
                                Argument      *args)
{
  gboolean success = TRUE;
  GimpUnit unit_id;
  gboolean deletion_flag;

  unit_id = g_value_get_int (&args[0].value);
  deletion_flag = g_value_get_boolean (&args[1].value);

  if (success)
    {
      _gimp_unit_set_deletion_flag (gimp, unit_id, deletion_flag);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure unit_set_deletion_flag_proc =
{
  TRUE, TRUE,
  "gimp-unit-set-deletion-flag",
  "gimp-unit-set-deletion-flag",
  "Sets the deletion flag of a unit.",
  "This procedure sets the unit's deletion flag. If the deletion flag of a unit is TRUE on gimp exit, this unit's definition will not be saved in the user's unitrc.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_set_deletion_flag_invoker } }
};

static Argument *
unit_get_identifier_invoker (GimpProcedure *procedure,
                             Gimp          *gimp,
                             GimpContext   *context,
                             GimpProgress  *progress,
                             Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gchar *identifier = NULL;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      identifier = g_strdup (_gimp_unit_get_identifier (gimp, unit_id));
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_string (&return_vals[1].value, identifier);

  return return_vals;
}

static GimpProcedure unit_get_identifier_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-identifier",
  "gimp-unit-get-identifier",
  "Returns the textual identifier of the unit.",
  "This procedure returns the textual identifier of the unit. For built-in units it will be the english singular form of the unit's name. For user-defined units this should equal to the singular form.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_identifier_invoker } }
};

static Argument *
unit_get_factor_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gdouble factor = 0.0;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      factor = _gimp_unit_get_factor (gimp, unit_id);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_double (&return_vals[1].value, factor);

  return return_vals;
}

static GimpProcedure unit_get_factor_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-factor",
  "gimp-unit-get-factor",
  "Returns the factor of the unit.",
  "This procedure returns the unit's factor which indicates how many units make up an inch. Note that asking for the factor of \"pixels\" will produce an error.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_factor_invoker } }
};

static Argument *
unit_get_digits_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gint32 digits = 0;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      digits = _gimp_unit_get_digits (gimp, unit_id);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_int (&return_vals[1].value, digits);

  return return_vals;
}

static GimpProcedure unit_get_digits_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-digits",
  "gimp-unit-get-digits",
  "Returns the number of digits of the unit.",
  "This procedure returns the number of digits you should provide in input or output functions to get approximately the same accuracy as with two digits and inches. Note that asking for the digits of \"pixels\" will produce an error.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_digits_invoker } }
};

static Argument *
unit_get_symbol_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gchar *symbol = NULL;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      symbol = g_strdup (_gimp_unit_get_symbol (gimp, unit_id));
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_string (&return_vals[1].value, symbol);

  return return_vals;
}

static GimpProcedure unit_get_symbol_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-symbol",
  "gimp-unit-get-symbol",
  "Returns the symbol of the unit.",
  "This procedure returns the symbol of the unit (\"''\" for inches).",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_symbol_invoker } }
};

static Argument *
unit_get_abbreviation_invoker (GimpProcedure *procedure,
                               Gimp          *gimp,
                               GimpContext   *context,
                               GimpProgress  *progress,
                               Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gchar *abbreviation = NULL;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      abbreviation = g_strdup (_gimp_unit_get_abbreviation (gimp, unit_id));
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_string (&return_vals[1].value, abbreviation);

  return return_vals;
}

static GimpProcedure unit_get_abbreviation_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-abbreviation",
  "gimp-unit-get-abbreviation",
  "Returns the abbreviation of the unit.",
  "This procedure returns the abbreviation of the unit (\"in\" for inches).",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_abbreviation_invoker } }
};

static Argument *
unit_get_singular_invoker (GimpProcedure *procedure,
                           Gimp          *gimp,
                           GimpContext   *context,
                           GimpProgress  *progress,
                           Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gchar *singular = NULL;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      singular = g_strdup (_gimp_unit_get_singular (gimp, unit_id));
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_string (&return_vals[1].value, singular);

  return return_vals;
}

static GimpProcedure unit_get_singular_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-singular",
  "gimp-unit-get-singular",
  "Returns the singular form of the unit.",
  "This procedure returns the singular form of the unit.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_singular_invoker } }
};

static Argument *
unit_get_plural_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         Argument      *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpUnit unit_id;
  gchar *plural = NULL;

  unit_id = g_value_get_int (&args[0].value);

  if (success)
    {
      plural = g_strdup (_gimp_unit_get_plural (gimp, unit_id));
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_string (&return_vals[1].value, plural);

  return return_vals;
}

static GimpProcedure unit_get_plural_proc =
{
  TRUE, TRUE,
  "gimp-unit-get-plural",
  "gimp-unit-get-plural",
  "Returns the plural form of the unit.",
  "This procedure returns the plural form of the unit.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { unit_get_plural_invoker } }
};
