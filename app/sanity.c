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

#include "config.h"

#include <gtk/gtk.h>
#include <fontconfig/fontconfig.h>
#include <pango/pangoft2.h>

#include "sanity.h"


const gchar *
sanity_check (gboolean no_interface)
{
  gchar *abort_message = NULL;

  if (! no_interface)
    {
      const gchar *mismatch;

#define GTK_REQUIRED_MAJOR 2
#define GTK_REQUIRED_MINOR 2
#define GTK_REQUIRED_MICRO 2

      mismatch = gtk_check_version (GTK_REQUIRED_MAJOR,
                                    GTK_REQUIRED_MINOR,
                                    GTK_REQUIRED_MICRO);

      if (mismatch)
        {
          abort_message =
            g_strdup_printf
            ("%s\n\n"
             "The GIMP requires Gtk+ version %d.%d.%d or later.\n"
             "Installed Gtk+ version is %d.%d.%d.\n\n"
             "Somehow you or your software packager managed\n"
             "to install The GIMP with an older Gtk+ version.\n\n"
             "Please upgrade to Gtk+ version %d.%d.%d or later.",
             mismatch,
             GTK_REQUIRED_MAJOR, GTK_REQUIRED_MINOR, GTK_REQUIRED_MICRO,
             gtk_major_version, gtk_minor_version, gtk_micro_version,
             GTK_REQUIRED_MAJOR, GTK_REQUIRED_MINOR, GTK_REQUIRED_MICRO);
        }

#undef GTK_REQUIRED_MAJOR
#undef GTK_REQUIRED_MINOR
#undef GTK_REQUIRED_MICRO
    }

  if (! abort_message)
    {
      gint fc_version       = FcGetVersion ();
      gint fc_major_version = fc_version / 100 / 100;
      gint fc_minor_version = fc_version / 100 % 100;
      gint fc_micro_version = fc_version % 100;

#define FC_REQUIRED_MAJOR 2
#define FC_REQUIRED_MINOR 2
#define FC_REQUIRED_MICRO 0

      if (fc_version < ((FC_REQUIRED_MAJOR * 10000) +
                        (FC_REQUIRED_MINOR *   100) +
                        (FC_REQUIRED_MICRO *     1)))
        {
          abort_message =
            g_strdup_printf
            ("Fontconfig version too old!\n\n"
             "The GIMP requires fontconfig version %d.%d.%d or later.\n"
             "Installed fontconfig version is %d.%d.%d.\n\n"
             "Somehow you or your software packager managed\n"
             "to install The GIMP with an older fontconfig version.\n\n"
             "Please upgrade to fontconfig version %d.%d.%d or later.",
             FC_REQUIRED_MAJOR, FC_REQUIRED_MINOR, FC_REQUIRED_MICRO,
             fc_major_version, fc_minor_version, fc_micro_version,
             FC_REQUIRED_MAJOR, FC_REQUIRED_MINOR, FC_REQUIRED_MICRO);
        }

#undef FC_REQUIRED_MAJOR
#undef FC_REQUIRED_MINOR
#undef FC_REQUIRED_MICRO
    }

#if 0
  /*  disabled until we definitely figured if and how freetype causes
   *  some of these windows dll hell bugs
   */
  if (! abort_message)
    {
      FT_Library ft_library;
      FT_Int     ft_major_version;
      FT_Int     ft_minor_version;
      FT_Int     ft_micro_version;
      FT_Int     ft_version;

#ifdef G_OS_WIN32
#define FT_REQUIRED_MAJOR 2
#define FT_REQUIRED_MINOR 1
#define FT_REQUIRED_MICRO 7
#else
#define FT_REQUIRED_MAJOR 2
#define FT_REQUIRED_MINOR 1
#define FT_REQUIRED_MICRO 7
#endif

      if (FT_Init_FreeType (&ft_library) != 0)
        g_error ("FT_Init_FreeType() failed");

      FT_Library_Version (ft_library,
                          &ft_major_version,
                          &ft_minor_version,
                          &ft_micro_version);

      if (FT_Done_FreeType (ft_library) != 0)
        g_error ("FT_Done_FreeType() failed");

      ft_version = (ft_major_version * 10000 +
                    ft_minor_version *   100 +
                    ft_micro_version *     1);

      if (ft_version < ((FT_REQUIRED_MAJOR * 10000) +
                        (FT_REQUIRED_MINOR *   100) +
                        (FT_REQUIRED_MICRO *     1)))
        {
          abort_message =
            g_strdup_printf
            ("FreeType version too old!\n\n"
             "The GIMP requires FreeType version %d.%d.%d or later.\n"
             "Installed FreeType version is %d.%d.%d.\n\n"
             "Somehow you or your software packager managed\n"
             "to install The GIMP with an older FreeType version.\n\n"
             "Please upgrade to FreeType version %d.%d.%d or later.",
             FT_REQUIRED_MAJOR, FT_REQUIRED_MINOR, FT_REQUIRED_MICRO,
             ft_major_version, ft_minor_version, ft_micro_version,
             FT_REQUIRED_MAJOR, FT_REQUIRED_MINOR, FT_REQUIRED_MICRO);
        }


#undef FT_REQUIRED_MAJOR
#undef FT_REQUIRED_MINOR
#undef FT_REQUIRED_MICRO
    }
#endif

  return abort_message;
}
