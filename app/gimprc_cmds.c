/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1999 Spencer Kimball and Peter Mattis
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

#include "procedural_db.h"

#include "gimprc.h"

static ProcRecord gimprc_query_proc;
static ProcRecord gimprc_set_proc;

void
register_gimprc_procs (void)
{
  procedural_db_register (&gimprc_query_proc);
  procedural_db_register (&gimprc_set_proc);
}

static Argument *
gimprc_query_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *token;
  gchar *value = NULL;

  token = (gchar *) args[0].value.pdb_pointer;
  if (token == NULL)
    success = FALSE;

  if (success) /* tests the custom tokens only */
    success = (value = gimprc_find_token (token)) != NULL;

  if (!success) /* try the standard ones, and do necessary conversions */
    success = (value = gimprc_value_to_str(token)) != NULL;

  return_args = procedural_db_return_args (&gimprc_query_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (value);

  return return_args;
}

static ProcArg gimprc_query_inargs[] =
{
  {
    PDB_STRING,
    "token",
    "The token to query for"
  }
};

static ProcArg gimprc_query_outargs[] =
{
  {
    PDB_STRING,
    "value",
    "The value associated with the queried token"
  }
};

static ProcRecord gimprc_query_proc =
{
  "gimp_gimprc_query",
  "Queries the gimprc file parser for information on a specified token.",
  "This procedure is used to locate additional information contained in the gimprc file considered extraneous to the operation of the GIMP. Plug-ins that need configuration information can expect it will be stored in the user gimprc file and can use this procedure to retrieve it. This query procedure will return the value associated with the specified token. This corresponds _only_ to entries with the format: (<token> <value>). The value must be a string. Entries not corresponding to this format will cause warnings to be issued on gimprc parsing a nd will not be queryable.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  PDB_INTERNAL,
  1,
  gimprc_query_inargs,
  1,
  gimprc_query_outargs,
  { { gimprc_query_invoker } }
};

static Argument *
gimprc_set_invoker (Argument *args)
{
  gboolean success = TRUE;
  gchar *token;
  gchar *value;

  token = (gchar *) args[0].value.pdb_pointer;
  if (token == NULL)
    success = FALSE;

  value = (gchar *) args[1].value.pdb_pointer;
  if (value == NULL)
    success = FALSE;

  if (success)
    save_gimprc_strings(token, value);

  return procedural_db_return_args (&gimprc_set_proc, success);
}

static ProcArg gimprc_set_inargs[] =
{
  {
    PDB_STRING,
    "token",
    "The token to modify"
  },
  {
    PDB_STRING,
    "value",
    "The value to set the token to"
  }
};

static ProcRecord gimprc_set_proc =
{
  "gimp_gimprc_set",
  "Sets a gimprc token to a value and saves it in the gimprc.",
  "This procedure is used to add or change additional information in the gimprc file that is considered extraneous to the operation of the GIMP. Plug-ins that need configuration information can use this function to store it, and gimp_gimprc_query to retrieve it. This will accept _only_ parameters in the format of (<token> <value>), where <token> and <value> must be strings. Entries not corresponding to this format will be eaten and no action will be performed. If the gimprc can not be written for whatever reason, gimp will complain loudly and the old gimprc will be saved in gimprc.old.",
  "Seth Burgess",
  "Seth Burgess",
  "1999",
  PDB_INTERNAL,
  2,
  gimprc_set_inargs,
  0,
  NULL,
  { { gimprc_set_invoker } }
};
