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

#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "core/gimp.h"
#include "core/gimpchannel.h"
#include "core/gimpcontext.h"
#include "core/gimpimage.h"
#include "core/gimplayer.h"

#include "widgets/gimpactiongroup.h"
#include "widgets/gimphelp-ids.h"

#include "actions.h"
#include "drawable-actions.h"
#include "drawable-commands.h"

#include "gimp-intl.h"


static GimpActionEntry drawable_actions[] =
{
  { "drawable-desaturate", GIMP_STOCK_CONVERT_GRAYSCALE,
    N_("_Desaturate"), NULL, NULL,
    G_CALLBACK (drawable_desaturate_cmd_callback),
    GIMP_HELP_LAYER_DESATURATE },

  { "drawable-invert", GIMP_STOCK_INVERT,
    N_("In_vert"), NULL, NULL,
    G_CALLBACK (drawable_invert_cmd_callback),
    GIMP_HELP_LAYER_INVERT },

  { "drawable-equalize", NULL,
    N_("_Equalize"), NULL, NULL,
    G_CALLBACK (drawable_equalize_cmd_callback),
    GIMP_HELP_LAYER_EQUALIZE },

  { "drawable-offset", NULL,
    N_("_Offset..."), "<control><shift>O", NULL,
    G_CALLBACK (drawable_offset_cmd_callback),
    GIMP_HELP_LAYER_OFFSET }
};

static GimpToggleActionEntry drawable_toggle_actions[] =
{
  { "drawable-visible", GIMP_STOCK_VISIBLE,
    N_("_Visible"), NULL, NULL,
    G_CALLBACK (drawable_visible_cmd_callback),
    FALSE,
    NULL },

  { "drawable-linked", GIMP_STOCK_LINKED,
    N_("_Linked"), NULL, NULL,
    G_CALLBACK (drawable_linked_cmd_callback),
    FALSE,
    NULL }
};

static GimpEnumActionEntry drawable_flip_actions[] =
{
  { "drawable-flip-horizontal", GIMP_STOCK_FLIP_HORIZONTAL,
    N_("Flip _Horizontally"), NULL, NULL,
    GIMP_ORIENTATION_HORIZONTAL,
    GIMP_HELP_LAYER_FLIP_HORIZONTAL },

  { "drawable-flip-vertical", GIMP_STOCK_FLIP_VERTICAL,
    N_("Flip _Vertically"), NULL, NULL,
    GIMP_ORIENTATION_VERTICAL,
    GIMP_HELP_LAYER_FLIP_VERTICAL }
};

static GimpEnumActionEntry drawable_rotate_actions[] =
{
  { "drawable-rotate-90", GIMP_STOCK_ROTATE_90,
    N_("Rotate 90 degrees _CW"), NULL, NULL,
    GIMP_ROTATE_90,
    GIMP_HELP_LAYER_ROTATE_90 },

  { "drawable-rotate-180", GIMP_STOCK_ROTATE_180,
    N_("Rotate _180 degrees"), NULL, NULL,
    GIMP_ROTATE_180,
    GIMP_HELP_LAYER_ROTATE_180 },

  { "drawable-rotate-270", GIMP_STOCK_ROTATE_270,
    N_("Rotate 90 degrees CC_W"), NULL, NULL,
    GIMP_ROTATE_270,
    GIMP_HELP_LAYER_ROTATE_270 }
};


void
drawable_actions_setup (GimpActionGroup *group)
{
  gimp_action_group_add_actions (group,
                                 drawable_actions,
                                 G_N_ELEMENTS (drawable_actions));

  gimp_action_group_add_toggle_actions (group,
                                        drawable_toggle_actions,
                                        G_N_ELEMENTS (drawable_toggle_actions));

  gimp_action_group_add_enum_actions (group,
                                      drawable_flip_actions,
                                      G_N_ELEMENTS (drawable_flip_actions),
                                      G_CALLBACK (drawable_flip_cmd_callback));

  gimp_action_group_add_enum_actions (group,
                                      drawable_rotate_actions,
                                      G_N_ELEMENTS (drawable_rotate_actions),
                                      G_CALLBACK (drawable_rotate_cmd_callback));
}

void
drawable_actions_update (GimpActionGroup *group,
                         gpointer         data)
{
  GimpImage    *gimage;
  GimpDrawable *drawable   = NULL;
  gboolean      is_rgb     = FALSE;
  gboolean      is_gray    = FALSE;
  gboolean      is_indexed = FALSE;
  gboolean      visible    = FALSE;
  gboolean      linked     = FALSE;

  gimage = action_data_get_image (data);

  if (gimage)
    {
      drawable = gimp_image_active_drawable (gimage);

      if (drawable)
        {
          GimpItem      *item          = GIMP_ITEM (drawable);
          GimpImageType  drawable_type = gimp_drawable_type (drawable);

          is_rgb     = GIMP_IMAGE_TYPE_IS_RGB     (drawable_type);
          is_gray    = GIMP_IMAGE_TYPE_IS_GRAY    (drawable_type);
          is_indexed = GIMP_IMAGE_TYPE_IS_INDEXED (drawable_type);

          visible = gimp_item_get_visible (item);
          linked  = gimp_item_get_linked  (item);
        }
    }

#define SET_SENSITIVE(action,condition) \
        gimp_action_group_set_action_sensitive (group, action, (condition) != 0)
#define SET_ACTIVE(action,condition) \
        gimp_action_group_set_action_active (group, action, (condition) != 0)

  SET_SENSITIVE ("drawable-desaturate", drawable &&   is_rgb);
  SET_SENSITIVE ("drawable-invert",     drawable && ! is_indexed);
  SET_SENSITIVE ("drawable-equalize",   drawable && ! is_indexed);
  SET_SENSITIVE ("drawable-offset",     drawable);

  SET_SENSITIVE ("drawable-visible", drawable);
  SET_SENSITIVE ("drawable-linked",  drawable);

  SET_ACTIVE ("drawable-visible", visible);
  SET_ACTIVE ("drawable-linked",  linked);

  SET_SENSITIVE ("drawable-flip-horizontal", drawable);
  SET_SENSITIVE ("drawable-flip-vertical",   drawable);

  SET_SENSITIVE ("drawable-rotate-90",  drawable);
  SET_SENSITIVE ("drawable-rotate-180", drawable);
  SET_SENSITIVE ("drawable-rotate-270", drawable);

#undef SET_SENSITIVE
#undef SET_ACTIVE
}
