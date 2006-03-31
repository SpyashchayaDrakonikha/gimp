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

#include <stdlib.h>
#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpbase/gimpprotocol.h"

#include "pdb-types.h"
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimp.h"
#include "plug-in/plug-in-def.h"
#include "plug-in/plug-in-params.h"
#include "plug-in/plug-in-proc-def.h"
#include "plug-in/plug-in.h"
#include "plug-in/plug-ins-query.h"
#include "plug-in/plug-ins.h"

static GimpProcedure plugins_query_proc;
static GimpProcedure plugin_domain_register_proc;
static GimpProcedure plugin_help_register_proc;
static GimpProcedure plugin_menu_register_proc;
static GimpProcedure plugin_menu_branch_register_proc;
static GimpProcedure plugin_icon_register_proc;

void
register_plug_in_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * plugins_query
   */
  procedure = gimp_procedure_init (&plugins_query_proc, 1, 12);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("search-string",
                                                       "search string",
                                                       "If not an empty string then use this as a search pattern",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("menu-path",
                                                         "menu path",
                                                         "The menu path of the plugin",
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("plugin-accelerator",
                                                         "plugin accelerator",
                                                         "String representing keyboard accelerator (could be empty string)",
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("plugin-location",
                                                         "plugin location",
                                                         "Location of the plugin program",
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("plugin-image-type",
                                                         "plugin image type",
                                                         "Type of image that this plugin will work on",
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32ARRAY,
                                   g_param_spec_pointer ("plugin-install-time",
                                                         "plugin install time",
                                                         "Time that the plugin was installed",
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-plugins",
                                                     "num plugins",
                                                     "The number of plugins",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("plugin-real-name",
                                                         "plugin real name",
                                                         "The internal name of the plugin",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * plugin_domain_register
   */
  procedure = gimp_procedure_init (&plugin_domain_register_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("domain-name",
                                                       "domain name",
                                                       "The name of the textdomain (must be unique)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("domain-path",
                                                       "domain path",
                                                       "The absolute path to the compiled message catalog (may be NULL)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  procedural_db_register (gimp, procedure);

  /*
   * plugin_help_register
   */
  procedure = gimp_procedure_init (&plugin_help_register_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("domain-name",
                                                       "domain name",
                                                       "The XML namespace of the plug-in's help pages",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("domain-uri",
                                                       "domain uri",
                                                       "The root URI of the plug-in's help pages",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * plugin_menu_register
   */
  procedure = gimp_procedure_init (&plugin_menu_register_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure for which to install the menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("menu-path",
                                                       "menu path",
                                                       "The procedure's additional menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * plugin_menu_branch_register
   */
  procedure = gimp_procedure_init (&plugin_menu_branch_register_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("menu-path",
                                                       "menu path",
                                                       "The sub-menu's menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("menu-name",
                                                       "menu name",
                                                       "The name of the sub-menu",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * plugin_icon_register
   */
  procedure = gimp_procedure_init (&plugin_icon_register_proc, 4, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure for which to install the icon",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_enum ("icon-type",
                                                  "icon type",
                                                  "The type of the icon: { GIMP_ICON_TYPE_STOCK_ID (0), GIMP_ICON_TYPE_INLINE_PIXBUF (1), GIMP_ICON_TYPE_IMAGE_FILE (2) }",
                                                  GIMP_TYPE_ICON_TYPE,
                                                  GIMP_ICON_TYPE_STOCK_ID,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_int ("icon-data-length",
                                                 "icon data length",
                                                 "The length of 'icon-data' (1 <= icon_data_length)",
                                                 1, G_MAXINT32, 1,
                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT8ARRAY,
                               g_param_spec_pointer ("icon-data",
                                                     "icon data",
                                                     "The procedure's icon. The format depends on the 'icon_type' parameter",
                                                     GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
plugins_query_invoker (GimpProcedure *procedure,
                       Gimp          *gimp,
                       GimpContext   *context,
                       GimpProgress  *progress,
                       Argument      *args)
{
  Argument *return_vals;
  gchar *search_string;
  gint32 num_plugins = 0;
  gchar **menu_path = NULL;
  gchar **plugin_accelerator = NULL;
  gchar **plugin_location = NULL;
  gchar **plugin_image_type = NULL;
  gint32 *plugin_install_time = NULL;
  gchar **plugin_real_name = NULL;

  search_string = (gchar *) g_value_get_string (&args[0].value);

  num_plugins = plug_ins_query (gimp, search_string,
                                &menu_path,
                                &plugin_accelerator,
                                &plugin_location,
                                &plugin_image_type,
                                &plugin_real_name,
                                &plugin_install_time);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);

  g_value_set_int (&return_vals[1].value, num_plugins);
  g_value_set_pointer (&return_vals[2].value, menu_path);
  g_value_set_int (&return_vals[3].value, num_plugins);
  g_value_set_pointer (&return_vals[4].value, plugin_accelerator);
  g_value_set_int (&return_vals[5].value, num_plugins);
  g_value_set_pointer (&return_vals[6].value, plugin_location);
  g_value_set_int (&return_vals[7].value, num_plugins);
  g_value_set_pointer (&return_vals[8].value, plugin_image_type);
  g_value_set_int (&return_vals[9].value, num_plugins);
  g_value_set_pointer (&return_vals[10].value, plugin_install_time);
  g_value_set_int (&return_vals[11].value, num_plugins);
  g_value_set_pointer (&return_vals[12].value, plugin_real_name);

  return return_vals;
}

static GimpProcedure plugins_query_proc =
{
  TRUE, TRUE,
  "gimp-plugins-query",
  "gimp-plugins-query",
  "Queries the plugin database for its contents.",
  "This procedure queries the contents of the plugin database.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugins_query_invoker } }
};

static Argument *
plugin_domain_register_invoker (GimpProcedure *procedure,
                                Gimp          *gimp,
                                GimpContext   *context,
                                GimpProgress  *progress,
                                Argument      *args)
{
  gboolean success = TRUE;
  gchar *domain_name;
  gchar *domain_path;

  domain_name = (gchar *) g_value_get_string (&args[0].value);
  domain_path = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          plug_in_def_set_locale_domain_name (gimp->current_plug_in->plug_in_def,
                                              domain_name);
          plug_in_def_set_locale_domain_path (gimp->current_plug_in->plug_in_def,
                                              domain_path);
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure plugin_domain_register_proc =
{
  TRUE, TRUE,
  "gimp-plugin-domain-register",
  "gimp-plugin-domain-register",
  "Registers a textdomain for localisation.",
  "This procedure adds a textdomain to the list of domains Gimp searches for strings when translating its menu entries. There is no need to call this function for plug-ins that have their strings included in the gimp-std-plugins domain as that is used by default. If the compiled message catalog is not in the standard location, you may specify an absolute path to another location. This procedure can only be called in the query function of a plug-in and it has to be called before any procedure is installed.",
  "Sven Neumann <sven@gimp.org>",
  "Sven Neumann",
  "2000",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugin_domain_register_invoker } }
};

static Argument *
plugin_help_register_invoker (GimpProcedure *procedure,
                              Gimp          *gimp,
                              GimpContext   *context,
                              GimpProgress  *progress,
                              Argument      *args)
{
  gboolean success = TRUE;
  gchar *domain_name;
  gchar *domain_uri;

  domain_name = (gchar *) g_value_get_string (&args[0].value);
  domain_uri = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          plug_in_def_set_help_domain_name (gimp->current_plug_in->plug_in_def,
                                            domain_name);
          plug_in_def_set_help_domain_uri (gimp->current_plug_in->plug_in_def,
                                           domain_uri);
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure plugin_help_register_proc =
{
  TRUE, TRUE,
  "gimp-plugin-help-register",
  "gimp-plugin-help-register",
  "Register a help path for a plug-in.",
  "This procedure changes the help rootdir for the plug-in which calls it. All subsequent calls of gimp_help from this plug-in will be interpreted relative to this rootdir.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2000",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugin_help_register_invoker } }
};

static Argument *
plugin_menu_register_invoker (GimpProcedure *procedure,
                              Gimp          *gimp,
                              GimpContext   *context,
                              GimpProgress  *progress,
                              Argument      *args)
{
  gboolean success = TRUE;
  gchar *procedure_name;
  gchar *menu_path;

  procedure_name = (gchar *) g_value_get_string (&args[0].value);
  menu_path = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      if (gimp->current_plug_in)
        {
          gchar *canonical = gimp_canonicalize_identifier (procedure_name);

          success = plug_in_menu_register (gimp->current_plug_in,
                                           canonical, menu_path);
          g_free (canonical);
        }
      else
        {
          success = FALSE;
        }
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure plugin_menu_register_proc =
{
  TRUE, TRUE,
  "gimp-plugin-menu-register",
  "gimp-plugin-menu-register",
  "Register an additional menu path for a plug-in procedure.",
  "This procedure installs an additional menu entry for the given procedure.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2004",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugin_menu_register_invoker } }
};

static Argument *
plugin_menu_branch_register_invoker (GimpProcedure *procedure,
                                     Gimp          *gimp,
                                     GimpContext   *context,
                                     GimpProgress  *progress,
                                     Argument      *args)
{
  gboolean success = TRUE;
  gchar *menu_path;
  gchar *menu_name;

  menu_path = (gchar *) g_value_get_string (&args[0].value);
  menu_name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      if (gimp->current_plug_in)
        {
          plug_ins_menu_branch_add (gimp, gimp->current_plug_in->prog,
                                    menu_path, menu_name);

          if (! gimp->no_interface)
            {
              gimp_menus_create_branch (gimp, gimp->current_plug_in->prog,
                                        menu_path, menu_name);
            }
          else
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure plugin_menu_branch_register_proc =
{
  TRUE, TRUE,
  "gimp-plugin-menu-branch-register",
  "gimp-plugin-menu-branch-register",
  "Register a sub-menu.",
  "This procedure installs an sub-menu which does not belong to any procedure.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2005",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugin_menu_branch_register_invoker } }
};

static Argument *
plugin_icon_register_invoker (GimpProcedure *procedure,
                              Gimp          *gimp,
                              GimpContext   *context,
                              GimpProgress  *progress,
                              Argument      *args)
{
  gboolean success = TRUE;
  gchar *procedure_name;
  gint32 icon_type;
  gint32 icon_data_length;
  guint8 *icon_data;

  procedure_name = (gchar *) g_value_get_string (&args[0].value);
  icon_type = g_value_get_enum (&args[1].value);
  icon_data_length = g_value_get_int (&args[2].value);
  icon_data = g_value_get_pointer (&args[3].value);

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          PlugInProcDef *proc_def;
          gchar         *canonical;

          canonical = gimp_canonicalize_identifier (procedure_name);

          proc_def = plug_in_proc_def_find (gimp->current_plug_in->plug_in_def->proc_defs,
                                            canonical);

          g_free (canonical);

          if (proc_def)
            plug_in_proc_def_set_icon (proc_def, icon_type,
                                       icon_data, icon_data_length);
          else
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure plugin_icon_register_proc =
{
  TRUE, TRUE,
  "gimp-plugin-icon-register",
  "gimp-plugin-icon-register",
  "Register an icon for a plug-in procedure.",
  "This procedure installs an icon for the given procedure.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2004",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { plugin_icon_register_invoker } }
};
