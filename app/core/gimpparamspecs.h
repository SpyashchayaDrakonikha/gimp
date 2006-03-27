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

#ifndef __GIMP_PARAM_SPECS_H__
#define __GIMP_PARAM_SPECS_H__


/*
 * Keep in sync with libgimpconfig/gimpconfig-params.h
 */
#define GIMP_PARAM_NO_VALIDATE (1 << (6 + G_PARAM_USER_SHIFT))


/*
 * GIMP_TYPE_PARAM_STRING
 */

#define GIMP_TYPE_PARAM_STRING           (gimp_param_string_get_type ())
#define GIMP_IS_PARAM_SPEC_STRING(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_STRING))

GType        gimp_param_string_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_string      (const gchar  *name,
                                          const gchar  *nick,
                                          const gchar  *blurb,
                                          const gchar  *default_value,
                                          gboolean      no_validate,
                                          gboolean      null_ok,
                                          GParamFlags   flags);


/*
 * GIMP_TYPE_PARAM_IMAGE_ID
 */

#define GIMP_TYPE_PARAM_IMAGE_ID           (gimp_param_image_id_get_type ())
#define GIMP_IS_PARAM_SPEC_IMAGE_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_IMAGE_ID))

GType        gimp_param_image_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_image_id      (const gchar  *name,
                                            const gchar  *nick,
                                            const gchar  *blurb,
                                            Gimp         *gimp,
                                            GParamFlags   flags);

GimpImage  * gimp_value_get_image          (const GValue *value,
                                            Gimp         *gimp);
void         gimp_value_set_image          (GValue       *value,
                                            GimpImage    *image);



/*
 * GIMP_TYPE_PARAM_ITEM_ID
 */

#define GIMP_TYPE_PARAM_ITEM_ID           (gimp_param_item_id_get_type ())
#define GIMP_IS_PARAM_SPEC_ITEM_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_ITEM_ID))

GType        gimp_param_item_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_item_id      (const gchar  *name,
                                           const gchar  *nick,
                                           const gchar  *blurb,
                                           Gimp         *gimp,
                                           GType         item_type,
                                           GParamFlags   flags);

GimpItem   * gimp_value_get_item          (const GValue *value,
                                           Gimp         *gimp,
                                           GType         item_type);
void         gimp_value_set_item          (GValue       *value,
                                           GimpItem     *item);


/*
 * GIMP_TYPE_PARAM_DISPLAY_ID
 */

#define GIMP_TYPE_PARAM_DISPLAY_ID           (gimp_param_display_id_get_type ())
#define GIMP_IS_PARAM_SPEC_DISPLAY_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_DISPLAY_ID))

GType        gimp_param_display_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_display_id      (const gchar  *name,
                                              const gchar  *nick,
                                              const gchar  *blurb,
                                              Gimp         *gimp,
                                              GParamFlags   flags);

GimpObject * gimp_value_get_display          (const GValue *value,
                                              Gimp         *gimp);
void         gimp_value_set_display          (GValue       *value,
                                              GimpObject   *display);


/*
 * GIMP_TYPE_PARASITE
 */

#define GIMP_TYPE_PARASITE               (gimp_parasite_get_type ())
#define GIMP_VALUE_HOLDS_PARASITE(value) (G_TYPE_CHECK_VALUE_TYPE ((value), GIMP_TYPE_PARASITE))

GType   gimp_parasite_get_type           (void) G_GNUC_CONST;

/*
 * GIMP_TYPE_PARAM_PARASITE
 */

#define GIMP_TYPE_PARAM_PARASITE           (gimp_param_parasite_get_type ())
#define GIMP_IS_PARAM_SPEC_PARASITE(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_PARASITE))

GType        gimp_param_parasite_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_parasite      (const gchar  *name,
                                            const gchar  *nick,
                                            const gchar  *blurb,
                                            GParamFlags   flags);


#endif  /*  __GIMP_PARAM_SPECS_H__  */
