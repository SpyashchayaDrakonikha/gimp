/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimphistogram module Copyright (C) 1999 Jay Cox <jaycox@earthlink.net>
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

#ifndef __GIMP_HISTOGRAM_P_H__
#define __GIMP_HISTOGRAM_P_H__

#include "config.h"

#ifdef ENABLE_MP
#include <pthread.h>
#endif

struct _GimpHistogram
{
  int bins;
  double **values;
  int nchannels;
#ifdef ENABLE_MP
  pthread_mutex_t mutex;
  int nthreads;
  double ***tmp_values;
  char *tmp_slots;
#endif /* ENABLE_MP */
};


#endif /* __GIMP_HISTOGRAM_P_H__ */
