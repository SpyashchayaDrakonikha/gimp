/* The GIMP -- an image manipulation program
 * Copyright (C) 1999 Manish Singh <yosh@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_COLOR_DISPLAY_H__
#define __GIMP_COLOR_DISPLAY_H__

G_BEGIN_DECLS

/* For information look at the html documentation */


#define GIMP_TYPE_COLOR_DISPLAY            (gimp_color_display_get_type ())
#define GIMP_COLOR_DISPLAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_COLOR_DISPLAY, GimpColorDisplay))
#define GIMP_COLOR_DISPLAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_COLOR_DISPLAY, GimpColorDisplayClass))
#define GIMP_IS_COLOR_DISPLAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_COLOR_DISPLAY))
#define GIMP_IS_COLOR_DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_COLOR_DISPLAY))
#define GIMP_COLOR_DISPLAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_COLOR_DISPLAY, GimpColorDisplayClass))


typedef struct _GimpColorDisplayClass GimpColorDisplayClass;

struct _GimpColorDisplay
{
  GObject  parent_instance;
};

struct _GimpColorDisplayClass
{
  GObjectClass  parent_class;

  const gchar *name;
  const gchar *help_page;

  /*  virtual functions  */
  GimpColorDisplay * (* clone)            (GimpColorDisplay *display);
  void               (* convert)          (GimpColorDisplay *display,
                                           guchar           *buf,
                                           gint              width,
                                           gint              height,
                                           gint              bpp,
                                           gint              bpl);

  void               (* load_state)       (GimpColorDisplay *display,
                                           GimpParasite     *state);
  GimpParasite     * (* save_state)       (GimpColorDisplay *display);
  void               (* configure)        (GimpColorDisplay *display,
                                           GFunc             ok_func,
                                           gpointer          ok_data,
                                           GFunc             cancel_func,
                                           gpointer          cancel_data);
  void               (* configure_cancel) (GimpColorDisplay *display);
};


GType              gimp_color_display_get_type     (void) G_GNUC_CONST;
GimpColorDisplay * gimp_color_display_new          (GType              display_type);

GimpColorDisplay * gimp_color_display_clone        (GimpColorDisplay  *display);

void           gimp_color_display_convert          (GimpColorDisplay  *display,
                                                    guchar            *buf,
                                                    gint               width,
                                                    gint               height,
                                                    gint               bpp,
                                                    gint               bpl);
void           gimp_color_display_load_state       (GimpColorDisplay *display,
                                                    GimpParasite     *state);
GimpParasite * gimp_color_display_save_state       (GimpColorDisplay *display);
void           gimp_color_display_configure        (GimpColorDisplay *display,
                                                    GFunc             ok_func,
                                                    gpointer          ok_data,
                                                    GFunc             cancel_func,
                                                    gpointer          cancel_data);
void           gimp_color_display_configure_cancel (GimpColorDisplay *display);


G_END_DECLS

#endif /* __GIMP_COLOR_DISPLAY_H__ */
