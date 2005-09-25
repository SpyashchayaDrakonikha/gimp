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

#include "tools-types.h"

#include "core/gimpimage.h"
#include "core/gimpimage-crop.h"
#include "core/gimptoolinfo.h"

#include "widgets/gimphelp-ids.h"

#include "display/gimpdisplay.h"

#include "gimprectangleoptions.h"
#include "gimprectangletool.h"
#include "gimpcropoptions.h"
#include "gimpcroptool.h"
#include "gimptoolcontrol.h"

#include "gimp-intl.h"


static void gimp_crop_tool_class_init     (GimpCropToolClass *klass);
static void gimp_crop_tool_init           (GimpCropTool      *crop_tool);
static void gimp_crop_tool_rectangle_tool_iface_init (GimpRectangleToolInterface *rectangle_iface);

static GObject * gimp_crop_tool_constructor (GType                  type,
                                             guint                  n_params,
                                             GObjectConstructParam *params);
static void gimp_crop_tool_finalize         (GObject *object);
static void gimp_crop_tool_control          (GimpTool       *tool,
                                             GimpToolAction  action,
                                             GimpDisplay    *gdisp);

static void gimp_crop_tool_cursor_update    (GimpTool                   *tool,
                                             GimpCoords                 *coords,
                                             GdkModifierType             state,
                                             GimpDisplay                *gdisp);

static gboolean gimp_crop_tool_execute      (GimpRectangleTool       *rectangle,
                                             gint                        x,
                                             gint                        y,
                                             gint                        w,
                                             gint                        h);

static GimpDrawToolClass *parent_class = NULL;


/*  public functions  */

void
gimp_crop_tool_register (GimpToolRegisterCallback  callback,
                         gpointer                  data)
{
  (* callback) (GIMP_TYPE_CROP_TOOL,
                GIMP_TYPE_CROP_OPTIONS,
                gimp_crop_options_gui,
                0,
                "gimp-crop-tool",
                _("Crop & Resize"),
                _("Crop or Resize an image"),
                N_("_Crop & Resize"), NULL,
                NULL, GIMP_HELP_TOOL_CROP,
                GIMP_STOCK_TOOL_CROP,
                data);
}

GType
gimp_crop_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpCropToolClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gimp_crop_tool_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data     */
        sizeof (GimpCropTool),
        0,              /* n_preallocs    */
        (GInstanceInitFunc) gimp_crop_tool_init,
      };
      static const GInterfaceInfo rectangle_info =
      {
        (GInterfaceInitFunc) gimp_crop_tool_rectangle_tool_iface_init,           /* interface_init */
        NULL,           /* interface_finalize */
        NULL            /* interface_data */
      };

      tool_type = g_type_register_static (GIMP_TYPE_DRAW_TOOL,
                                          "GimpCropTool",
                                          &tool_info, 0);
      g_type_add_interface_static (tool_type,
                                   GIMP_TYPE_RECTANGLE_TOOL,
                                   &rectangle_info);
    }

  return tool_type;
}

static void
gimp_crop_tool_class_init (GimpCropToolClass *klass)
{
  GObjectClass      *object_class    = G_OBJECT_CLASS (klass);
  GimpToolClass     *tool_class      = GIMP_TOOL_CLASS (klass);
  GimpDrawToolClass *draw_tool_class = GIMP_DRAW_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->constructor  = gimp_crop_tool_constructor;
  object_class->dispose      = gimp_rectangle_tool_dispose;
  object_class->finalize     = gimp_crop_tool_finalize;
  object_class->set_property = gimp_rectangle_tool_set_property;
  object_class->get_property = gimp_rectangle_tool_get_property;
  gimp_rectangle_tool_install_properties (object_class);

  tool_class->initialize     = gimp_rectangle_tool_initialize;
  tool_class->control        = gimp_crop_tool_control;
  tool_class->button_press   = gimp_rectangle_tool_button_press;
  tool_class->button_release = gimp_rectangle_tool_button_release;
  tool_class->motion         = gimp_rectangle_tool_motion;
  tool_class->key_press      = gimp_rectangle_tool_key_press;
  tool_class->modifier_key   = gimp_rectangle_tool_modifier_key;
  tool_class->oper_update    = gimp_rectangle_tool_oper_update;
  tool_class->cursor_update  = gimp_crop_tool_cursor_update;

  draw_tool_class->draw      = gimp_rectangle_tool_draw;
}

static void
gimp_crop_tool_init (GimpCropTool *crop_tool)
{
  GimpTool           *tool      = GIMP_TOOL (crop_tool);

  gimp_tool_control_set_tool_cursor (tool->control, GIMP_TOOL_CURSOR_CROP);
}

static void
gimp_crop_tool_rectangle_tool_iface_init (GimpRectangleToolInterface *rectangle_iface)
{
  rectangle_iface->execute      = gimp_crop_tool_execute;
}

static GObject *
gimp_crop_tool_constructor (GType                  type,
                            guint                  n_params,
                            GObjectConstructParam *params)
{
  GObject *object;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  gimp_rectangle_tool_constructor (object);

  return object;
}

static void
gimp_crop_tool_finalize (GObject *object)
{
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_crop_tool_control (GimpTool       *tool,
                        GimpToolAction  action,
                        GimpDisplay    *gdisp)
{
  gimp_rectangle_tool_control (tool, action, gdisp);

  GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static void
gimp_crop_tool_cursor_update (GimpTool        *tool,
                              GimpCoords      *coords,
                              GdkModifierType  state,
                              GimpDisplay     *gdisp)
{
  gimp_rectangle_tool_cursor_update (tool, coords, state, gdisp);
  gimp_tool_control_set_tool_cursor (tool->control, GIMP_TOOL_CURSOR_CROP);

  GIMP_TOOL_CLASS (parent_class)->cursor_update (tool, coords, state, gdisp);
}

static gboolean
gimp_crop_tool_execute (GimpRectangleTool  *rectangle,
                        gint                x,
                        gint                y,
                        gint                w,
                        gint                h)
{
  GimpTool        *tool      = GIMP_TOOL (rectangle);
  GimpCropOptions *options;
  GimpImage       *gimage;
  gint             max_x, max_y;
  gboolean         rectangle_exists;

  options = GIMP_CROP_OPTIONS (tool->tool_info->tool_options);

  gimage = tool->gdisp->gimage;
  max_x = gimage->width;
  max_y = gimage->height;

  rectangle_exists = (x <= max_x && y <= max_y &&
                      x + w >= 0 && y + h >= 0 &&
                      w > 0 && h > 0);

  if (x < 0)
    {
      w += x;
      x = 0;
    }

  if (y < 0)
    {
      h += y;
      y = 0;
    }

  if (x + w > max_x)
    w = max_x - x;

  if (y + h > max_y)
    h = max_y - y;

  /* if rectangle exists, crop it */
  if (rectangle_exists)
    {
      gimp_image_crop (gimage, GIMP_CONTEXT (options),
                       x, y, w + x, h + y,
                       options->layer_only,
                       options->crop_mode == GIMP_CROP_MODE_CROP);

      gimp_image_flush (gimage);

      return TRUE;
    }

  return TRUE;
}

