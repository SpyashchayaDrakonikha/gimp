/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * plug-ins.c
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

#include "string.h"

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpbase/gimpprotocol.h"
#include "libgimpbase/gimpwire.h"
#include "libgimpconfig/gimpconfig.h"

#include "plug-in-types.h"

#include "config/gimpcoreconfig.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"

#include "pdb/gimp-pdb.h"
#include "pdb/gimppluginprocedure.h"

#include "plug-in.h"
#include "plug-ins.h"
#include "plug-in-data.h"
#include "plug-in-def.h"
#include "plug-in-params.h"
#include "plug-in-progress.h"
#include "plug-in-rc.h"
#include "plug-in-run.h"

#include "gimp-intl.h"


#define STD_PLUGINS_DOMAIN  GETTEXT_PACKAGE "-std-plug-ins"


typedef struct _PlugInLocaleDomain PlugInLocaleDomain;
typedef struct _PlugInHelpDomain   PlugInHelpDomain;

struct _PlugInLocaleDomain
{
  gchar *prog_name;
  gchar *domain_name;
  gchar *domain_path;
};

struct _PlugInHelpDomain
{
  gchar *prog_name;
  gchar *domain_name;
  gchar *domain_uri;
};


static void                  plug_ins_init_file         (const GimpDatafileData *file_data,
                                                         gpointer                data);
static void                  plug_ins_add_to_db         (Gimp             *gimp,
                                                         GimpContext      *context);
static GimpPlugInProcedure * plug_ins_proc_def_insert   (Gimp             *gimp,
                                                         GimpPlugInProcedure *proc);
static gint                  plug_ins_file_proc_compare (gconstpointer     a,
                                                         gconstpointer     b,
                                                         gpointer          data);


/*  public functions  */

void
plug_ins_init (Gimp               *gimp,
               GimpContext        *context,
               GimpInitStatusFunc  status_callback)
{
  gchar   *filename;
  gchar   *basename;
  gchar   *path;
  GSList  *list;
  GList   *extensions = NULL;
  gint     n_plugins;
  gint     n_extensions;
  gint     nth;
  GError  *error = NULL;

  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (status_callback != NULL);

  plug_in_init (gimp);

  /* search for binaries in the plug-in directory path */
  path = gimp_config_path_expand (gimp->config->plug_in_path, TRUE, NULL);

  gimp_datafiles_read_directories (path,
                                   G_FILE_TEST_IS_EXECUTABLE,
				   plug_ins_init_file,
                                   &gimp->plug_in_defs);

  g_free (path);

  /* read the pluginrc file for cached data */
  if (gimp->config->plug_in_rc_path)
    {
      filename = gimp_config_path_expand (gimp->config->plug_in_rc_path,
                                          TRUE, NULL);

      if (! g_path_is_absolute (filename))
        {
          gchar *str = g_build_filename (gimp_directory (), filename, NULL);

          g_free (filename);
          filename = str;
        }
    }
  else
    {
      filename = gimp_personal_rc_file ("pluginrc");
    }

  status_callback (_("Resource configuration"),
                   gimp_filename_to_utf8 (filename), 0.0);

  if (gimp->be_verbose)
    g_print (_("Parsing '%s'\n"), gimp_filename_to_utf8 (filename));

  if (! plug_in_rc_parse (gimp, filename, &error))
    {
      if (error->code != GIMP_CONFIG_ERROR_OPEN_ENOENT)
        g_message (error->message);

      g_clear_error (&error);
    }

  /*  query any plug-ins that have changed since we last wrote out
   *  the pluginrc file
   */
  status_callback (_("Querying new Plug-ins"), "", 0.0);

  for (list = gimp->plug_in_defs, n_plugins = 0; list; list = list->next)
    {
      PlugInDef *plug_in_def = list->data;

      if (plug_in_def->needs_query)
        n_plugins++;
    }

  if (n_plugins)
    {
      gimp->write_pluginrc = TRUE;

      for (list = gimp->plug_in_defs, nth = 0; list; list = list->next)
        {
          PlugInDef *plug_in_def = list->data;

          if (! plug_in_def->needs_query)
            continue;

          basename = g_filename_display_basename (plug_in_def->prog);
          status_callback (NULL, basename,
			   (gdouble) nth++ / (gdouble) n_plugins);
          g_free (basename);

	  if (gimp->be_verbose)
	    g_print (_("Querying plug-in: '%s'\n"),
		     gimp_filename_to_utf8 (plug_in_def->prog));

	  plug_in_call_query (gimp, context, plug_in_def);
	}
    }

  /* initialize the plug-ins */
  status_callback (_("Initializing Plug-ins"), "", 0.0);

  for (list = gimp->plug_in_defs, n_plugins = 0; list; list = list->next)
    {
      PlugInDef *plug_in_def = list->data;

      if (plug_in_def->has_init)
        n_plugins++;
    }

  if (n_plugins)
    {
      for (list = gimp->plug_in_defs, nth = 0; list; list = list->next)
        {
          PlugInDef *plug_in_def = list->data;

          if (! plug_in_def->has_init)
            continue;

          basename = g_filename_display_basename (plug_in_def->prog);
          status_callback (NULL, basename,
			   (gdouble) nth++ / (gdouble) n_plugins);
          g_free (basename);

          if (gimp->be_verbose)
            g_print (_("Initializing plug-in: '%s'\n"),
                     gimp_filename_to_utf8 (plug_in_def->prog));

          plug_in_call_init (gimp, context, plug_in_def);
        }
    }

  status_callback (NULL, "", 1.0);

  /* insert the proc defs */
  for (list = gimp->plug_in_defs; list; list = list->next)
    {
      PlugInDef *plug_in_def = list->data;
      GSList    *list2;

      for (list2 = plug_in_def->proc_defs; list2; list2 = list2->next)
	{
	  GimpPlugInProcedure *proc = list2->data;
          GimpPlugInProcedure *overridden_proc;

 	  proc->mtime = plug_in_def->mtime;

	  overridden_proc = plug_ins_proc_def_insert (gimp, proc);

          if (overridden_proc)
            {
              GSList *list3;

              g_printerr ("removing duplicate PDB procedure \"%s\" "
                          "registered by '%s'\n",
                          GIMP_PROCEDURE (overridden_proc)->name,
                          gimp_filename_to_utf8 (overridden_proc->prog));

              /* search the plugin list to see if any plugins had references to
               * the overridden_proc.
               */
              for (list3 = gimp->plug_in_defs; list3; list3 = list3->next)
                {
                  PlugInDef *plug_in_def2 = list3->data;

                  plug_in_def2->proc_defs =
                    g_slist_remove (plug_in_def2->proc_defs, overridden_proc);
                }

              /* also remove it from the lists of load and save procs */
              gimp->load_procs = g_slist_remove (gimp->load_procs,
                                                 overridden_proc);
              gimp->save_procs = g_slist_remove (gimp->save_procs,
                                                 overridden_proc);

              g_object_unref (overridden_proc);
            }
	}
    }

  /* write the pluginrc file if necessary */
  if (gimp->write_pluginrc)
    {
      if (gimp->be_verbose)
	g_print (_("Writing '%s'\n"), gimp_filename_to_utf8 (filename));

      if (! plug_in_rc_write (gimp->plug_in_defs, filename, &error))
        {
          g_message ("%s", error->message);
          g_clear_error (&error);
        }

      gimp->write_pluginrc = FALSE;
    }

  g_free (filename);

  /* add the plug-in procs to the procedure database */
  plug_ins_add_to_db (gimp, context);

  /* create help_path and locale_domain lists */
  for (list = gimp->plug_in_defs; list; list = list->next)
    {
      PlugInDef *plug_in_def = list->data;

      if (plug_in_def->locale_domain_name)
        plug_ins_locale_domain_add (gimp,
                                    plug_in_def->prog,
                                    plug_in_def->locale_domain_name,
                                    plug_in_def->locale_domain_path);

      if (plug_in_def->help_domain_name)
        plug_ins_help_domain_add (gimp,
                                  plug_in_def->prog,
                                  plug_in_def->help_domain_name,
                                  plug_in_def->help_domain_uri);
    }

  if (! gimp->no_interface)
    {
      gimp->load_procs = g_slist_sort_with_data (gimp->load_procs,
                                                 plug_ins_file_proc_compare,
                                                 gimp);
      gimp->save_procs = g_slist_sort_with_data (gimp->save_procs,
                                                 plug_ins_file_proc_compare,
                                                 gimp);

      gimp_menus_init (gimp, gimp->plug_in_defs, STD_PLUGINS_DOMAIN);
    }

  /* build list of automatically started extensions */
  for (list = gimp->plug_in_proc_defs, nth = 0; list; list = list->next, nth++)
    {
      GimpPlugInProcedure *proc = list->data;

      if (proc->prog                                         &&
	  GIMP_PROCEDURE (proc)->proc_type == GIMP_EXTENSION &&
	  GIMP_PROCEDURE (proc)->num_args  == 0)
	{
          extensions = g_list_prepend (extensions, proc);
        }
    }

  extensions   = g_list_reverse (extensions);
  n_extensions = g_list_length (extensions);

  /* run the available extensions */
  if (extensions)
    {
      GList *list;

      status_callback (_("Starting Extensions"), "", 0.0);

      for (list = extensions, nth = 0; list; list = g_list_next (list), nth++)
        {
          GimpPlugInProcedure *proc = list->data;
          GValueArray         *args;

	  if (gimp->be_verbose)
	    g_print (_("Starting extension: '%s'\n"),
                     GIMP_PROCEDURE (proc)->name);

	  status_callback (NULL, GIMP_PROCEDURE (proc)->name,
                           (gdouble) nth / (gdouble) n_extensions);

          args = g_value_array_new (0);

	  plug_in_run (gimp, context, NULL, GIMP_PROCEDURE (proc),
                       args, FALSE, TRUE, -1);

          g_value_array_free (args);
	}

      g_list_free (extensions);
    }

  status_callback ("", "", 1.0);

  /* free up stuff */
  for (list = gimp->plug_in_defs; list; list = list->next)
    plug_in_def_free (list->data, FALSE);

  g_slist_free (gimp->plug_in_defs);
  gimp->plug_in_defs = NULL;
}

void
plug_ins_exit (Gimp *gimp)
{
  GSList *list;

  plug_in_exit (gimp);

  for (list = gimp->plug_in_menu_branches; list; list = list->next)
    {
      PlugInMenuBranch *branch = list->data;

      g_free (branch->prog_name);
      g_free (branch->menu_path);
      g_free (branch->menu_label);
      g_free (branch);
    }

  g_slist_free (gimp->plug_in_menu_branches);
  gimp->plug_in_menu_branches = NULL;

  for (list = gimp->plug_in_locale_domains; list; list = list->next)
    {
      PlugInLocaleDomain *domain = list->data;

      g_free (domain->prog_name);
      g_free (domain->domain_name);
      g_free (domain->domain_path);
      g_free (domain);
    }

  g_slist_free (gimp->plug_in_locale_domains);
  gimp->plug_in_locale_domains = NULL;

  for (list = gimp->plug_in_help_domains; list; list = list->next)
    {
      PlugInHelpDomain *domain = list->data;

      g_free (domain->prog_name);
      g_free (domain->domain_name);
      g_free (domain->domain_uri);
      g_free (domain);
    }

  g_slist_free (gimp->plug_in_help_domains);
  gimp->plug_in_help_domains = NULL;

  plug_in_data_free (gimp);
}

void
plug_ins_add_internal (Gimp                *gimp,
                       GimpPlugInProcedure *proc)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (GIMP_IS_PLUG_IN_PROCEDURE (proc));

  gimp->plug_in_proc_defs = g_slist_prepend (gimp->plug_in_proc_defs,
                                             proc);
}

GimpPlugInProcedure *
plug_ins_file_register_magic (Gimp        *gimp,
                              const gchar *name,
                              const gchar *extensions,
                              const gchar *prefixes,
                              const gchar *magics)
{
  GimpPlugInProcedure *proc;
  GSList              *list;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  if (gimp->current_plug_in && gimp->current_plug_in->plug_in_def)
    list = gimp->current_plug_in->plug_in_def->proc_defs;
  else
    list = gimp->plug_in_proc_defs;

  proc = gimp_plug_in_procedure_find (list, name);

  if (proc)
    gimp_plug_in_procedure_set_file_proc (proc, extensions, prefixes, magics);

  return proc;
}

GimpPlugInProcedure *
plug_ins_file_register_mime (Gimp        *gimp,
                             const gchar *name,
                             const gchar *mime_type)
{
  GimpPlugInProcedure *proc;
  GSList              *list;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (mime_type != NULL, NULL);

  if (gimp->current_plug_in && gimp->current_plug_in->plug_in_def)
    list = gimp->current_plug_in->plug_in_def->proc_defs;
  else
    list = gimp->plug_in_proc_defs;

  proc = gimp_plug_in_procedure_find (list, name);

  if (proc)
    gimp_plug_in_procedure_set_mime_type (proc, mime_type);

  return proc;
}

GimpPlugInProcedure *
plug_ins_file_register_thumb_loader (Gimp        *gimp,
                                     const gchar *load_proc,
                                     const gchar *thumb_proc)
{
  GimpPlugInProcedure *proc;
  GSList              *list;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (load_proc, NULL);
  g_return_val_if_fail (thumb_proc, NULL);

  if (gimp->current_plug_in && gimp->current_plug_in->plug_in_def)
    list = gimp->current_plug_in->plug_in_def->proc_defs;
  else
    list = gimp->plug_in_proc_defs;

  proc = gimp_plug_in_procedure_find (list, load_proc);

  if (proc)
    gimp_plug_in_procedure_set_thumb_loader (proc, thumb_proc);

  return proc;
}

void
plug_ins_def_add_from_rc (Gimp      *gimp,
                          PlugInDef *plug_in_def)
{
  GSList *list;
  gchar  *basename1;

  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (plug_in_def != NULL);
  g_return_if_fail (plug_in_def->prog != NULL);

  if (! g_path_is_absolute (plug_in_def->prog))
    {
      g_warning ("plug_ins_def_add_from_rc: filename not absolute (skipping)");
      plug_in_def_free (plug_in_def, TRUE);
      return;
    }

  basename1 = g_path_get_basename (plug_in_def->prog);

  /*  If this is a file load or save plugin, make sure we have
   *  something for one of the extensions, prefixes, or magic number.
   *  Other bits of code rely on detecting file plugins by the presence
   *  of one of these things, but Nick Lamb's alien/unknown format
   *  loader needs to be able to register no extensions, prefixes or
   *  magics. -- austin 13/Feb/99
   */
  for (list = plug_in_def->proc_defs; list; list = list->next)
    {
      GimpPlugInProcedure *proc = list->data;

      if (! proc->extensions &&
          ! proc->prefixes   &&
          ! proc->magics     &&
	  proc->menu_paths   &&
	  (! strncmp (proc->menu_paths->data, "<Load>", 6) ||
	   ! strncmp (proc->menu_paths->data, "<Save>", 6)))
	{
	  proc->extensions = g_strdup ("");
	}
    }

  /*  Check if the entry mentioned in pluginrc matches an executable
   *  found in the plug_in_path.
   */
  for (list = gimp->plug_in_defs; list; list = list->next)
    {
      PlugInDef *ondisk_plug_in_def = list->data;
      gchar     *basename2;

      basename2 = g_path_get_basename (ondisk_plug_in_def->prog);

      if (! strcmp (basename1, basename2))
	{
	  if (! g_ascii_strcasecmp (plug_in_def->prog,
                                    ondisk_plug_in_def->prog) &&
	      (plug_in_def->mtime == ondisk_plug_in_def->mtime))
	    {
	      /* Use pluginrc entry, deleting ondisk entry */
	      list->data = plug_in_def;
	      plug_in_def_free (ondisk_plug_in_def, TRUE);
	    }
	  else
	    {
              /* Use ondisk entry, deleting pluginrc entry */
	      plug_in_def_free (plug_in_def, TRUE);
	    }

	  g_free (basename2);
	  g_free (basename1);

	  return;
	}

      g_free (basename2);
    }

  g_free (basename1);

  gimp->write_pluginrc = TRUE;
  g_printerr ("executable not found: '%s'\n",
              gimp_filename_to_utf8 (plug_in_def->prog));
  plug_in_def_free (plug_in_def, FALSE);
}

void
plug_ins_temp_proc_def_add (Gimp                *gimp,
                            GimpPlugInProcedure *proc)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (GIMP_IS_PLUG_IN_PROCEDURE (proc));

  if (! gimp->no_interface)
    {
      if (proc->menu_label || proc->menu_paths)
        gimp_menus_create_item (gimp, proc, NULL);
    }

  /*  Register the procedural database entry  */
  gimp_pdb_register (gimp, GIMP_PROCEDURE (proc));

  /*  Add the definition to the global list  */
  gimp->plug_in_proc_defs = g_slist_prepend (gimp->plug_in_proc_defs, proc);
}

void
plug_ins_temp_proc_def_remove (Gimp                *gimp,
                               GimpPlugInProcedure *proc)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (GIMP_IS_PLUG_IN_PROCEDURE (proc));

  if (! gimp->no_interface)
    {
      if (proc->menu_label || proc->menu_paths)
        gimp_menus_delete_item (gimp, proc);
    }

  /*  Unregister the procedural database entry  */
  gimp_pdb_unregister (gimp, GIMP_PROCEDURE (proc)->name);

  /*  Remove the definition from the global list  */
  gimp->plug_in_proc_defs = g_slist_remove (gimp->plug_in_proc_defs, proc);

  /*  Destroy the definition  */
  g_object_unref (proc);
}

void
plug_ins_menu_branch_add (Gimp        *gimp,
                          const gchar *prog_name,
                          const gchar *menu_path,
                          const gchar *menu_label)
{
  PlugInMenuBranch *branch;

  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (prog_name != NULL);
  g_return_if_fail (menu_path != NULL);
  g_return_if_fail (menu_label != NULL);

  branch = g_new (PlugInMenuBranch, 1);

  branch->prog_name  = g_strdup (prog_name);
  branch->menu_path  = g_strdup (menu_path);
  branch->menu_label = g_strdup (menu_label);

  gimp->plug_in_menu_branches = g_slist_append (gimp->plug_in_menu_branches,
                                                branch);

#ifdef VERBOSE
  g_print ("added menu branch \"%s\" at path \"%s\"\n",
           branch->menu_label, branch->menu_path);
#endif
}

void
plug_ins_locale_domain_add (Gimp        *gimp,
                            const gchar *prog_name,
                            const gchar *domain_name,
                            const gchar *domain_path)
{
  PlugInLocaleDomain *domain;

  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (prog_name != NULL);
  g_return_if_fail (domain_name != NULL);

  domain = g_new (PlugInLocaleDomain, 1);

  domain->prog_name   = g_strdup (prog_name);
  domain->domain_name = g_strdup (domain_name);
  domain->domain_path = g_strdup (domain_path);

  gimp->plug_in_locale_domains = g_slist_prepend (gimp->plug_in_locale_domains,
                                                  domain);

#ifdef VERBOSE
  g_print ("added locale domain \"%s\" for path \"%s\"\n",
           domain->domain_name ? domain->domain_name : "(null)",
           domain->domain_path ?
           gimp_filename_to_utf8 (domain->domain_path) : "(null)");
#endif
}

const gchar *
plug_ins_locale_domain (Gimp         *gimp,
                        const gchar  *prog_name,
                        const gchar **domain_path)
{
  GSList *list;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  if (domain_path)
    *domain_path = gimp_locale_directory ();

  /*  A NULL prog_name is GIMP itself, return the default domain  */
  if (! prog_name)
    return NULL;

  for (list = gimp->plug_in_locale_domains; list; list = list->next)
    {
      PlugInLocaleDomain *domain = list->data;

      if (domain && domain->prog_name &&
          ! strcmp (domain->prog_name, prog_name))
        {
          if (domain_path && domain->domain_path)
            *domain_path = domain->domain_path;

          return domain->domain_name;
        }
    }

  return STD_PLUGINS_DOMAIN;
}

void
plug_ins_help_domain_add (Gimp        *gimp,
                          const gchar *prog_name,
                          const gchar *domain_name,
                          const gchar *domain_uri)
{
  PlugInHelpDomain *domain;

  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (prog_name != NULL);
  g_return_if_fail (domain_name != NULL);

  domain = g_new (PlugInHelpDomain, 1);

  domain->prog_name   = g_strdup (prog_name);
  domain->domain_name = g_strdup (domain_name);
  domain->domain_uri  = g_strdup (domain_uri);

  gimp->plug_in_help_domains = g_slist_prepend (gimp->plug_in_help_domains,
                                                domain);

#ifdef VERBOSE
  g_print ("added help domain \"%s\" for base uri \"%s\"\n",
           domain->domain_name ? domain->domain_name : "(null)",
           domain->domain_uri  ? domain->domain_uri  : "(null)");
#endif
}

const gchar *
plug_ins_help_domain (Gimp         *gimp,
                      const gchar  *prog_name,
                      const gchar **domain_uri)
{
  GSList *list;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  if (domain_uri)
    *domain_uri = NULL;

  /*  A NULL prog_name is GIMP itself, return the default domain  */
  if (! prog_name)
    return NULL;

  for (list = gimp->plug_in_help_domains; list; list = list->next)
    {
      PlugInHelpDomain *domain = list->data;

      if (domain && domain->prog_name &&
          ! strcmp (domain->prog_name, prog_name))
        {
          if (domain_uri && domain->domain_uri)
            *domain_uri = domain->domain_uri;

          return domain->domain_name;
        }
    }

  return NULL;
}

gint
plug_ins_help_domains (Gimp    *gimp,
                       gchar ***help_domains,
                       gchar ***help_uris)
{
  GSList *list;
  gint    n_domains;
  gint    i;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), 0);
  g_return_val_if_fail (help_domains != NULL, 0);
  g_return_val_if_fail (help_uris != NULL, 0);

  n_domains = g_slist_length (gimp->plug_in_help_domains);

  *help_domains = g_new0 (gchar *, n_domains);
  *help_uris    = g_new0 (gchar *, n_domains);

  for (list = gimp->plug_in_help_domains, i = 0; list; list = list->next, i++)
    {
      PlugInHelpDomain *domain = list->data;

      (*help_domains)[i] = g_strdup (domain->domain_name);
      (*help_uris)[i]    = g_strdup (domain->domain_uri);
    }

  return n_domains;
}


/*  private functions  */

static void
plug_ins_init_file (const GimpDatafileData *file_data,
                    gpointer                data)
{
  PlugInDef  *plug_in_def;
  GSList    **plug_in_defs = data;
  GSList     *list;

  for (list = *plug_in_defs; list; list = list->next)
    {
      gchar *plug_in_name;

      plug_in_def  = list->data;
      plug_in_name = g_path_get_basename (plug_in_def->prog);

      if (g_ascii_strcasecmp (file_data->basename, plug_in_name) == 0)
	{
	  g_printerr ("skipping duplicate plug-in: '%s'\n",
                      gimp_filename_to_utf8 (file_data->filename));

          g_free (plug_in_name);

	  return;
	}

      g_free (plug_in_name);
    }

  plug_in_def = plug_in_def_new (file_data->filename);

  plug_in_def_set_mtime (plug_in_def, file_data->mtime);
  plug_in_def_set_needs_query (plug_in_def, TRUE);

  *plug_in_defs = g_slist_prepend (*plug_in_defs, plug_in_def);
}

static void
plug_ins_add_to_db (Gimp        *gimp,
                    GimpContext *context)
{
  GimpPlugInProcedure *proc;
  GSList              *list;

  for (list = gimp->plug_in_proc_defs; list; list = list->next)
    {
      proc = list->data;

      if (GIMP_PROCEDURE (proc)->proc_type != GIMP_INTERNAL)
	{
          gimp_pdb_register (gimp, GIMP_PROCEDURE (proc));
	}
      else
        {
          g_printerr ("%s: NOT adding %s (prog = %s) to PDB\n",
                      G_STRFUNC, GIMP_PROCEDURE (proc)->name, proc->prog);
        }
    }

  for (list = gimp->plug_in_proc_defs; list; list = list->next)
    {
      proc = list->data;

      if (proc->file_proc)
        {
          GValueArray *return_vals;

          if (proc->image_types)
            {
              return_vals =
                gimp_pdb_run_proc (gimp, context, NULL,
                                   "gimp-register-save-handler",
                                   G_TYPE_STRING, GIMP_PROCEDURE (proc)->name,
                                   G_TYPE_STRING, proc->extensions,
                                   G_TYPE_STRING, proc->prefixes,
                                   G_TYPE_NONE);
            }
          else
            {
              return_vals =
                gimp_pdb_run_proc (gimp, context, NULL,
                                   "gimp-register-magic-load-handler",
                                   G_TYPE_STRING, GIMP_PROCEDURE (proc)->name,
                                   G_TYPE_STRING, proc->extensions,
                                   G_TYPE_STRING, proc->prefixes,
                                   G_TYPE_STRING, proc->magics,
                                   G_TYPE_NONE);
            }

          g_value_array_free (return_vals);
	}
    }
}

static GimpPlugInProcedure *
plug_ins_proc_def_insert (Gimp                *gimp,
                          GimpPlugInProcedure *proc)
{
  GSList *list;

  for (list = gimp->plug_in_proc_defs; list; list = list->next)
    {
      GimpPlugInProcedure *tmp_proc = list->data;

      if (strcmp (GIMP_PROCEDURE (proc)->name,
                  GIMP_PROCEDURE (tmp_proc)->name) == 0)
	{
	  list->data = proc;

	  return tmp_proc;
	}
    }

  gimp->plug_in_proc_defs = g_slist_prepend (gimp->plug_in_proc_defs, proc);

  return NULL;
}

static gint
plug_ins_file_proc_compare (gconstpointer a,
                            gconstpointer b,
                            gpointer      data)
{
  Gimp                      *gimp   = data;
  const GimpPlugInProcedure *proc_a = a;
  const GimpPlugInProcedure *proc_b = b;
  gchar                     *label_a;
  gchar                     *label_b;
  gint                       retval = 0;

  if (strncmp (proc_a->prog, "gimp-xcf", 8) == 0)
    return -1;

  if (strncmp (proc_b->prog, "gimp-xcf", 8) == 0)
    return 1;

  label_a = gimp_plug_in_procedure_get_label (proc_a,
                                              plug_ins_locale_domain (gimp,
                                                                      proc_a->prog,
                                                                      NULL));
  label_b = gimp_plug_in_procedure_get_label (proc_b,
                                              plug_ins_locale_domain (gimp,
                                                                      proc_b->prog,
                                                                      NULL));

  if (label_a && label_b)
    retval = g_utf8_collate (label_a, label_b);

  g_free (label_a);
  g_free (label_b);

  return retval;
}
