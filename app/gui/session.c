/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Session-managment stuff
 * Copyright (C) 1998 Sven Neumann <sven@gimp.org>
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

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

#include "libgimpbase/gimpbase.h"

#include "gui-types.h"

#include "config/gimpconfig-error.h"
#include "config/gimpconfig-utils.h"
#include "config/gimpconfigwriter.h"
#include "config/gimpguiconfig.h"
#include "config/gimpscanner.h"

#include "core/gimp.h"

#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpsessioninfo.h"

#include "color-history.h"
#include "session.h"

#include "gimp-intl.h"


enum
{
  SESSION_INFO = 1,
  COLOR_HISTORY,
  LAST_TIP_SHOWN
};


static gchar * session_filename (Gimp *gimp);


/*  public functions  */

void
session_init (Gimp *gimp)
{
  gchar      *filename;
  GScanner   *scanner;
  GTokenType  token;
  GError     *error = NULL;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  filename = session_filename (gimp);

  scanner = gimp_scanner_new_file (filename, &error);

  if (! scanner && error->code == GIMP_CONFIG_ERROR_OPEN_ENOENT)
    {
      g_clear_error (&error);
      g_free (filename);

      filename = g_build_filename (gimp_sysconf_directory (),
                                   "sessionrc", NULL);
      scanner = gimp_scanner_new_file (filename, NULL);
    }

  if (! scanner)
    {
      g_clear_error (&error);
      g_free (filename);
      return;
    }

  g_scanner_scope_add_symbol (scanner, 0, "session-info",
                              GINT_TO_POINTER (SESSION_INFO));
  g_scanner_scope_add_symbol (scanner, 0, "color-history",
                              GINT_TO_POINTER (COLOR_HISTORY));
  g_scanner_scope_add_symbol (scanner, 0,  "last-tip-shown",
                              GINT_TO_POINTER (LAST_TIP_SHOWN));

  token = G_TOKEN_LEFT_PAREN;

  while (g_scanner_peek_next_token (scanner) == token)
    {
      token = g_scanner_get_next_token (scanner);

      switch (token)
        {
        case G_TOKEN_LEFT_PAREN:
          token = G_TOKEN_SYMBOL;
          break;

        case G_TOKEN_SYMBOL:
          if (scanner->value.v_symbol == GINT_TO_POINTER (SESSION_INFO))
            {
              g_scanner_set_scope (scanner, SESSION_INFO);
              token = gimp_session_info_deserialize (scanner, SESSION_INFO);

              if (token == G_TOKEN_RIGHT_PAREN)
                g_scanner_set_scope (scanner, 0);
              else
                break;
            }
          else if (scanner->value.v_symbol == GINT_TO_POINTER (COLOR_HISTORY))
            {
              while (g_scanner_peek_next_token (scanner) == G_TOKEN_LEFT_PAREN)
                {
                  GimpRGB color;

                  if (! gimp_scanner_parse_color (scanner, &color))
                    goto error;

                  color_history_add_from_rc (&color);
                }
            }
          else if (scanner->value.v_symbol == GINT_TO_POINTER (LAST_TIP_SHOWN))
            {
	      GimpGuiConfig *config = GIMP_GUI_CONFIG (gimp->config);

              token = G_TOKEN_INT;

              if (! gimp_scanner_parse_int (scanner, &config->last_tip))
                break;
            }
          token = G_TOKEN_RIGHT_PAREN;
          break;

        case G_TOKEN_RIGHT_PAREN:
          token = G_TOKEN_LEFT_PAREN;
          break;

        default: /* do nothing */
          break;
        }
    }

  if (token != G_TOKEN_LEFT_PAREN)
    {
      g_scanner_get_next_token (scanner);
      g_scanner_unexp_token (scanner, token, NULL, NULL, NULL,
                             _("fatal parse error"), TRUE);
    }

 error:

  if (error)
    {
      g_message (error->message);
      g_clear_error (&error);

      gimp_config_file_backup_on_error (filename, "sessionrc", NULL);
    }

  gimp_scanner_destroy (scanner);
  g_free (filename);
}

void
session_restore (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));

  gimp_dialog_factories_session_restore ();
}

void
session_save (Gimp *gimp)
{
  GimpConfigWriter *writer;
  gchar            *filename;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  filename = session_filename (gimp);

  writer =
    gimp_config_writer_new_file (filename,
				 TRUE,
				 "GIMP sessionrc\n\n"
				 "This file takes session-specific info "
				 "(that is info, you want to keep between "
				 "two GIMP sessions). You are not supposed "
				 "to edit it manually, but of course you "
				 "can do. The sessionrc will be entirely "
                                 "rewritten every time you quit The GIMP. "
                                 "If this file isn't found, defaults are "
                                 "used.",
				 NULL);
  g_free (filename);

  if (!writer)
    return;

  gimp_dialog_factories_session_save (writer);
  gimp_config_writer_linefeed (writer);

  /* save last tip shown */
  gimp_config_writer_open (writer, "last-tip-shown");
  gimp_config_writer_printf (writer, "%d",
                             GIMP_GUI_CONFIG (gimp->config)->last_tip + 1);
  gimp_config_writer_close (writer);
  gimp_config_writer_linefeed (writer);

  color_history_write (writer);

  gimp_config_writer_finish (writer, "end of sessionrc", NULL);
}

void
session_clear (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));

  g_print ("TODO: implement session_clear()\n");
}


static gchar *
session_filename (Gimp *gimp)
{
  gchar *filename = gimp_personal_rc_file ("sessionrc");

  if (gimp->session_name)
    {
      gchar *tmp = g_strconcat (filename, ".", gimp->session_name, NULL);

      g_free (filename);
      filename = tmp;
    }

  return filename;
}
