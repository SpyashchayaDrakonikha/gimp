/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Pixelize plug-in (ported to GIMP v1.0)
 * Copyright (C) 1997 Eiichi Takamori <taka@ma1.seikyou.ne.jp>
 * original pixelize.c for GIMP 0.54 by Tracy Scott
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * version 1.04
 * This version requires GIMP v0.99.10 or above.
 *
 * This plug-in "pixelizes" the image.
 *
 *	Eiichi Takamori <taka@ma1.seikyou.ne.jp>
 *	http://ha1.seikyou.ne.jp/home/taka/gimp/
 *
 * Changes from version 1.03 to version 1.04:
 * - Added gtk_rc_parse
 * - Added entry with scale
 * - Fixed bug that large pixelwidth >=64 sometimes caused core dump
 *
 * Changes from gimp-0.99.9/plug-ins/pixelize.c to version 1.03:
 * - Fixed comments and help strings
 * - Fixed `RGB, GRAY' to `RGB*, GRAY*'
 * - Fixed procedure db name `pixelize' to `plug_in_pixelize'
 *
 * Differences from Tracy Scott's original `pixelize' plug-in:
 *
 * - Algorithm is modified to work around with the tile management.
 *   The way of pixelizing is switched by the value of pixelwidth.  If
 *   pixelwidth is greater than (or equal to) tile width, then this
 *   plug-in makes GimpPixelRgn with that width and proceeds. Otherwise,
 *   it makes the region named `PixelArea', whose size is smaller than
 *   tile width and is multiply of pixel width, and acts onto it.
 */

/* pixelize filter written for the GIMP
 *  -Tracy Scott
 *
 * This filter acts as a low pass filter on the color components of
 * the provided region
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "libgimp/stdplugins-intl.h"


#ifdef RCSID
static char rcsid[] = "$Id$";
#endif


/* Some useful macros */

#define TILE_CACHE_SIZE  16
#define SCALE_WIDTH     125
#define ENTRY_WIDTH      50

typedef struct
{
  gint pixelwidth;
} PixelizeValues;

typedef struct
{
  gint run;
} PixelizeInterface;

typedef struct
{
  gint x, y, w, h;
  gint width;
  guchar *data;
} PixelArea;

/* Declare local functions.
 */
static void	query	(void);
static void	run	(gchar	 *name,
			 gint	 nparams,
			 GimpParam	 *param,
			 gint	 *nreturn_vals,
			 GimpParam	 **return_vals);

static gint	pixelize_dialog      (void);
static void	pixelize_ok_callback (GtkWidget *widget,
				      gpointer   data);

static void	pixelize	(GimpDrawable *drawable);
static void	pixelize_large	(GimpDrawable *drawable, gint pixelwidth);
static void	pixelize_small	(GimpDrawable *drawable, gint pixelwidth,
				 gint tile_width);
static void	pixelize_sub    (gint pixelwidth, gint bpp);

/***** Local vars *****/

GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,	 /* init_proc  */
  NULL,	 /* quit_proc  */
  query, /* query_proc */
  run	 /* run_proc   */
};

static PixelizeValues pvals =
{
  10
};

static PixelizeInterface pint =
{
  FALSE	    /* run */
};

static PixelArea area;

/***** Functions *****/

MAIN ()

static void
query (void)
{
  static GimpParamDef args[]=
  {
    { GIMP_PDB_INT32, "run_mode", "Interactive, non-interactive" },
    { GIMP_PDB_IMAGE, "image", "Input image (unused)" },
    { GIMP_PDB_DRAWABLE, "drawable", "Input drawable" },
    { GIMP_PDB_INT32, "pixelwidth", "Pixel width	 (the decrease in resolution)" }
  };
  static gint nargs = sizeof (args) / sizeof (args[0]);

  gimp_install_procedure ("plug_in_pixelize",
			  "Pixelize the contents of the specified drawable",
			  "Pixelize the contents of the specified drawable with speficied pixelizing width.",
			  "Spencer Kimball & Peter Mattis, Tracy Scott, (ported to 1.0 by) Eiichi Takamori",
			  "Spencer Kimball & Peter Mattis, Tracy Scott",
			  "1995",
			  N_("<Image>/Filters/Blur/Pixelize..."),
			  "RGB*, GRAY*",
			  GIMP_PLUGIN,
			  nargs, 0,
			  args, NULL);
}

static void
run (gchar   *name,
     gint    nparams,
     GimpParam  *param,
     gint    *nreturn_vals,
     GimpParam  **return_vals)
{
  static GimpParam values[1];
  GimpDrawable *drawable;
  GimpRunModeType run_mode;
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;

  run_mode = param[0].data.d_int32;

  *nreturn_vals = 1;
  *return_vals = values;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  /*  Get the specified drawable  */
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      INIT_I18N_UI();
      /*  Possibly retrieve data  */
      gimp_get_data ("plug_in_pixelize", &pvals);

      /*  First acquire information with a dialog  */
      if (! pixelize_dialog ())
	{
	  gimp_drawable_detach (drawable);
	  return;
	}
      break;

    case GIMP_RUN_NONINTERACTIVE:
      INIT_I18N();
      /*  Make sure all the arguments are there!  */
      if (nparams != 4)
	status = GIMP_PDB_CALLING_ERROR;
      if (status == GIMP_PDB_SUCCESS)
	{
	  pvals.pixelwidth = (gdouble) param[3].data.d_int32;
	}
      if ((status == GIMP_PDB_SUCCESS) &&
	  pvals.pixelwidth <= 0)
	status = GIMP_PDB_CALLING_ERROR;
      break;

    case GIMP_RUN_WITH_LAST_VALS:
      INIT_I18N();
      /*  Possibly retrieve data  */
      gimp_get_data ("plug_in_pixelize", &pvals);
      break;

    default:
      break;
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      /*  Make sure that the drawable is gray or RGB color  */
      if (gimp_drawable_is_rgb (drawable->drawable_id) ||
	  gimp_drawable_is_gray (drawable->drawable_id))
	{
	  gimp_progress_init (_("Pixelizing..."));

	  /*  set the tile cache size  */
	  gimp_tile_cache_ntiles (TILE_CACHE_SIZE);

	  /*  run the pixelize effect  */
	  pixelize (drawable);

	  if (run_mode != GIMP_RUN_NONINTERACTIVE)
	    gimp_displays_flush ();

	  /*  Store data  */
	  if (run_mode == GIMP_RUN_INTERACTIVE)
	    gimp_set_data ("plug_in_pixelize", &pvals, sizeof (PixelizeValues));
	}
      else
	{
	  /* g_message ("pixelize: cannot operate on indexed color images"); */
	  status = GIMP_PDB_EXECUTION_ERROR;
	}
    }

  values[0].data.d_status = status;

  gimp_drawable_detach (drawable);
}

static gint
pixelize_dialog (void)
{
  GtkWidget *dlg;
  GtkWidget *frame;
  GtkWidget *table;
  GtkObject *adjustment;

  gimp_ui_init ("pixelize", FALSE);

  dlg = gimp_dialog_new (_("Pixelize"), "pixelize",
			 gimp_standard_help_func, "filters/pixelize.html",
			 GTK_WIN_POS_MOUSE,
			 FALSE, TRUE, FALSE,

			 GTK_STOCK_OK, pixelize_ok_callback,
			 NULL, NULL, NULL, TRUE, FALSE,
			 GTK_STOCK_CANCEL, gtk_widget_destroy,
			 NULL, 1, NULL, FALSE, TRUE,

			 NULL);

  gtk_signal_connect (GTK_OBJECT (dlg), "destroy",
		      GTK_SIGNAL_FUNC (gtk_main_quit),
		      NULL);

  /*  parameter settings  */
  frame = gtk_frame_new (_("Parameter Settings"));
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_IN);
  gtk_container_set_border_width (GTK_CONTAINER (frame), 6);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dlg)->vbox), frame, TRUE, TRUE, 0);

  table = gtk_table_new (1, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 4);
  gtk_container_set_border_width (GTK_CONTAINER (table), 4);
  gtk_container_add (GTK_CONTAINER (frame), table);

  adjustment =
    gimp_scale_entry_new (GTK_TABLE (table), 0, 0,
			  _("Pixel Width:"), SCALE_WIDTH, ENTRY_WIDTH,
			  pvals.pixelwidth, 1, 64, 1, 8, 0,
			  FALSE, 0, GIMP_MAX_IMAGE_SIZE,
			  NULL, NULL);
  gtk_signal_connect (GTK_OBJECT (adjustment), "value_changed",
		      GTK_SIGNAL_FUNC (gimp_int_adjustment_update),
		      &pvals.pixelwidth);

  gtk_widget_show (frame);
  gtk_widget_show (table);
  gtk_widget_show (dlg);

  gtk_main ();
  gdk_flush ();

  return pint.run;
}

/*  Pixelize interface functions  */

static void
pixelize_ok_callback (GtkWidget *widget,
		      gpointer	 data)
{
  pint.run = TRUE;

  gtk_widget_destroy (GTK_WIDGET (data));
}

/*
  Pixelize Effect
 */

static void
pixelize (GimpDrawable *drawable)
{
  gint tile_width;
  gint pixelwidth;

  tile_width = gimp_tile_width();
  pixelwidth = pvals.pixelwidth;

  if (pixelwidth < 0)
    pixelwidth = - pixelwidth;
  if (pixelwidth < 1)
    pixelwidth = 1;

  if (pixelwidth >= tile_width)
    pixelize_large (drawable, pixelwidth);
  else
    pixelize_small (drawable, pixelwidth, tile_width);
}

/*
  This function operates on the image when pixelwidth >= tile_width.
  It simply sets the size of GimpPixelRgn as pixelwidth and proceeds.
 */
static void
pixelize_large (GimpDrawable *drawable,
		gint       pixelwidth)
{
  GimpPixelRgn src_rgn, dest_rgn;
  guchar *src_row, *dest_row;
  guchar *src, *dest;
  gulong *average;
  gint row, col, b, bpp;
  gint x, y, x_step, y_step;
  gulong count;
  gint x1, y1, x2, y2;
  gint progress, max_progress;
  gpointer pr;

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

  bpp = gimp_drawable_bpp(drawable->drawable_id);
  average = g_new(gulong, bpp);

  /* Initialize progress */
  progress = 0;
  max_progress = 2 * (x2 - x1) * (y2 - y1);

  for (y = y1; y < y2; y += pixelwidth - (y % pixelwidth))
    {
      for (x = x1; x < x2; x += pixelwidth - (x % pixelwidth))
	{
	  x_step = pixelwidth - (x % pixelwidth);
	  y_step = pixelwidth - (y % pixelwidth);
	  x_step = MIN(x_step, x2-x);
	  y_step = MIN(y_step, y2-y);

	  gimp_pixel_rgn_init (&src_rgn, drawable,
			       x, y, x_step, y_step, FALSE, FALSE);
	  for (b = 0; b < bpp;	b++)
	    average[b] = 0;
	  count = 0;

	  for (pr = gimp_pixel_rgns_register (1, &src_rgn);
	       pr != NULL;
	       pr = gimp_pixel_rgns_process (pr))
	    {
	      src_row = src_rgn.data;
	      for (row = 0; row < src_rgn.h; row++)
		{
		  src = src_row;
		  for (col = 0; col < src_rgn.w; col++)
		    {
		      for(b = 0; b < bpp; b++)
			average[b] += src[b];
		      src += src_rgn.bpp;
		      count += 1;
		    }
		  src_row += src_rgn.rowstride;
		}
	      /* Update progress */
	      progress += src_rgn.w * src_rgn.h;
	      gimp_progress_update ((double) progress / (double) max_progress);
	    }

	  if (count > 0)
	    {
	      for (b = 0; b < bpp; b++)
		average[b] = (guchar) (average[b] / count);
	    }

	  gimp_pixel_rgn_init (&dest_rgn, drawable,
			       x, y, x_step, y_step, TRUE, TRUE);
	  for (pr = gimp_pixel_rgns_register (1, &dest_rgn);
	       pr != NULL;
	       pr = gimp_pixel_rgns_process (pr))
	    {
	      dest_row = dest_rgn.data;
	      for (row = 0; row < dest_rgn.h; row++)
		{
		  dest = dest_row;
		  for (col = 0; col < dest_rgn.w; col++)
		    {
		      for (b = 0; b < bpp; b++)
			dest[b] = average[b];
		      dest += dest_rgn.bpp;
		      count += 1;
		    }
		  dest_row += dest_rgn.rowstride;
		}
	      /* Update progress */
	      progress += dest_rgn.w * dest_rgn.h;
	      gimp_progress_update ((double) progress / (double) max_progress);
	    }
	}
    }

  g_free (average);

  /*  update the blurred region	 */
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));
}


/*
   This function operates on PixelArea, whose width and height are
   multiply of pixel width, and less than the tile size (to enhance
   its speed).

   If any coordinates of mask boundary is not multiply of pixel width
   (e.g.  x1 % pixelwidth != 0), operates on the region whose width
   or height is the remainder.
 */
static void
pixelize_small (GimpDrawable *drawable,
		gint       pixelwidth,
		gint       tile_width)
{
  GimpPixelRgn src_rgn, dest_rgn;
  gint bpp;
  gint x1, y1, x2, y2;
  gint progress, max_progress;

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
  gimp_pixel_rgn_init (&src_rgn, drawable, x1, y1, x2-x1, y2-y1, FALSE, FALSE);
  gimp_pixel_rgn_init (&dest_rgn, drawable, x1, y1, x2-x1, y2-y1, TRUE, TRUE);

  /* Initialize progress */
  progress = 0;
  max_progress = (x2 - x1) * (y2 - y1);

  bpp = drawable->bpp;

  area.width = (tile_width / pixelwidth) * pixelwidth;
  area.data= g_new(guchar, (glong) bpp * area.width * area.width );

  for(area.y = y1; area.y < y2;
       area.y += area.width - (area.y % area.width))
    {
      area.h = area.width - (area.y % area.width);
      area.h = MIN(area.h, y2 - area.y);
      for(area.x = x1; area.x < x2;
	   area.x += area.width - (area.x % area.width))
	{
	  area.w = area.width - (area.x % area.width);
	  area.w = MIN(area.w, x2 - area.x);

	  gimp_pixel_rgn_get_rect (&src_rgn, area.data,
				   area.x, area.y, area.w, area.h);

	  pixelize_sub(pixelwidth, bpp);

	  gimp_pixel_rgn_set_rect (&dest_rgn, area.data,
				   area.x, area.y, area.w, area.h);

	  /* Update progress */
	  progress += area.w * area.h;
	  gimp_progress_update ((double) progress / (double) max_progress);
	}
    }

  g_free(area.data);

  /*  update the pixelized region  */
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));
}

/*
  This function acts on one PixelArea.	Since there were so many
  nested FORs in pixelize_small(), I put a few of them here...
  */

static void
pixelize_sub (gint pixelwidth,
	      gint bpp)
{
  glong average[4];		/* bpp <= 4 */
  gint	x, y, w, h;
  guchar *buf_row, *buf;
  gint	row, col;
  gint	rowstride;
  gint	count;
  gint	i;

  rowstride = area.w * bpp;

  for (y = area.y; y < area.y + area.h; y += pixelwidth - (y % pixelwidth))
    {
      h = pixelwidth - (y % pixelwidth);
      h = MIN(h, area.y + area.h - y);
      for (x = area.x; x < area.x + area.w; x += pixelwidth - (x % pixelwidth))
	{
	  w = pixelwidth - (x % pixelwidth);
	  w = MIN (w, area.x + area.w - x);

	  for (i = 0; i < bpp; i++)
	    average[i] = 0;
	  count = 0;

	  /* Read */
	  buf_row = area.data + (y-area.y)*rowstride + (x-area.x)*bpp;

	  for (row = 0; row < h; row++)
	    {
	      buf = buf_row;
	      for (col = 0; col < w; col++)
		{
		  for (i = 0; i < bpp; i++)
		    average[i] += buf[i];
		  count++;
		  buf += bpp;
		}
	      buf_row += rowstride;
	    }

	  /* Average */
	  if (count > 0)
	    {
	      for (i = 0; i < bpp; i++)
		average[i] /= count;
	    }

	  /* Write */
	  buf_row = area.data + (y-area.y)*rowstride + (x-area.x)*bpp;

	  for (row = 0; row < h; row++)
	    {
	      buf = buf_row;
	      for (col = 0; col < w; col++)
		{
		  for (i = 0; i < bpp; i++)
		    buf[i] = average[i];
		  count++;
		  buf += bpp;
		}
	      buf_row += rowstride;
	    }
	}
    }
}
