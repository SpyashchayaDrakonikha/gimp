#include "config.h"

#ifdef __GNUC__
#warning GTK_DISABLE_DEPRECATED
#endif
#undef GTK_DISABLE_DEPRECATED

#include <gtk/gtk.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "gimpressionist.h"
#include "ppmtool.h"
#include "size.h"
#include "infile.h"

#include "preview.h"

#include "libgimp/stdplugins-intl.h"


#define RESPONSE_APPLY 1

#define MAPFILE "data.out"

static GtkWidget *smwindow;
static GtkWidget *smvectorprev;
static GtkWidget *smpreviewprev;
static GtkWidget *prev_button;
static GtkWidget *next_button;
static GtkWidget *add_button;
static GtkWidget *kill_button;

static GtkObject *smvectprevbrightadjust = NULL;

static GtkObject *sizadjust = NULL;
static GtkObject *smstradjust = NULL;
static GtkObject *smstrexpadjust = NULL;
static GtkWidget *sizevoronoi = NULL;

#define OMWIDTH 150
#define OMHEIGHT 150

static smvector_t smvector[MAXSIZEVECT];
static int numsmvect = 0;

static double getsiz_from_gui(double x, double y)
{
    return getsiz_proto(x,y, numsmvect, smvector,
                        GTK_ADJUSTMENT(smstrexpadjust)->value,
                        GTK_TOGGLE_BUTTON(sizevoronoi)->active);
}

static void updatesmpreviewprev(void)
{
  gint x, y;
  static ppm_t nsbuffer;
  guchar black[3] = {0,0,0};
  guchar gray[3] = {120,120,120};

  if (! PPM_IS_INITED (&nsbuffer))
    {
      ppm_new(&nsbuffer,OMWIDTH,OMHEIGHT);
    }
  fill(&nsbuffer, black);

  for (y = 6; y < OMHEIGHT-4; y += 10)
    {
      for (x = 6; x < OMWIDTH-4; x += 10)
         {
           gdouble siz = 5 * getsiz_from_gui(x/(double)OMWIDTH,
                                             y/(double)OMHEIGHT);
           ppm_drawline (&nsbuffer, x-siz, y-siz, x+siz, y-siz, gray);
           ppm_drawline (&nsbuffer, x+siz, y-siz, x+siz, y+siz, gray);
           ppm_drawline (&nsbuffer, x+siz, y+siz, x-siz, y+siz, gray);
           ppm_drawline (&nsbuffer, x-siz, y+siz, x-siz, y-siz, gray);
         }
    }

  for (y = 0; y < OMHEIGHT; y++)
    gtk_preview_draw_row (GTK_PREVIEW(smpreviewprev),
                          nsbuffer.col + y * nsbuffer.width * 3,
                          0, y, OMWIDTH);

  gtk_widget_queue_draw (smpreviewprev);
}

static gint selectedsmvector = 0;
static ppm_t update_vector_preview_backup = {0,0,NULL};
static ppm_t update_vector_preview_sbuffer = {0,0,NULL};

static void updatesmvectorprev(void)
{
  static int ok = 0;
  gint i, x, y;
  gdouble val;
  static gdouble last_val = 0.0;
  guchar gray[3] = {120,120,120};
  guchar red[3] = {255,0,0};
  guchar white[3] = {255,255,255};

  if (smvectprevbrightadjust)
    val = 1.0 - GTK_ADJUSTMENT(smvectprevbrightadjust)->value / 100.0;
  else
    val = 0.5;

  if(!ok || (val != last_val))
    {
#if 0
      if (!PPM_IS_INITED (&infile))
         updatepreview (NULL, (void *)2); /* Force grabarea() */
      ppm_copy(&infile, &update_vector_preview_backup);
#else
      infile_copy_to_ppm (&update_vector_preview_backup);
#endif
      ppm_apply_brightness(&update_vector_preview_backup, val, 1,1,1);
      if (update_vector_preview_backup.width != OMWIDTH || 
          update_vector_preview_backup.height != OMHEIGHT)
         resize_fast(&update_vector_preview_backup, OMWIDTH, OMHEIGHT);
      ok = 1;
  }
  ppm_copy(&update_vector_preview_backup, &update_vector_preview_sbuffer);

  for (i = 0; i < numsmvect; i++)
    {
      x = smvector[i].x * OMWIDTH;
      y = smvector[i].y * OMHEIGHT;
      if (i == selectedsmvector)
      {
         ppm_drawline (&update_vector_preview_sbuffer, x-5, y, x+5, y, red);
         ppm_drawline (&update_vector_preview_sbuffer, x, y-5, x, y+5, red);
      }
      else
      {
         ppm_drawline (&update_vector_preview_sbuffer, x-5, y, x+5, y, gray);
         ppm_drawline (&update_vector_preview_sbuffer, x, y-5, x, y+5, gray);
      }
      ppm_put_rgb (&update_vector_preview_sbuffer, x, y, white);
  }

  for (y = 0; y < OMHEIGHT; y++)
    gtk_preview_draw_row (GTK_PREVIEW(smvectorprev),
                          update_vector_preview_sbuffer.col +
                          y * update_vector_preview_sbuffer.width * 3,
                          0, y, OMWIDTH);

  gtk_widget_queue_draw (smvectorprev);

  gtk_widget_set_sensitive (prev_button, (numsmvect > 1));
  gtk_widget_set_sensitive (next_button, (numsmvect > 1));
  gtk_widget_set_sensitive (add_button, (numsmvect < MAXORIENTVECT));
  gtk_widget_set_sensitive (kill_button, (numsmvect > 1));
}

void size_map_free_resources ()
{
  ppm_kill (&update_vector_preview_backup);
  ppm_kill (&update_vector_preview_sbuffer);
}

static gboolean smadjignore = FALSE;

static void updatesmsliders(void)
{
  smadjignore = TRUE;
  gtk_adjustment_set_value(GTK_ADJUSTMENT(sizadjust),
                              smvector[selectedsmvector].siz);
  gtk_adjustment_set_value(GTK_ADJUSTMENT(smstradjust),
                              smvector[selectedsmvector].str);
  smadjignore = FALSE;
}

static void smprevclick(GtkWidget *w, gpointer data)
{
  selectedsmvector--;
  if(selectedsmvector < 0) selectedsmvector = numsmvect-1;
  updatesmsliders();
  updatesmvectorprev();
}

static void smnextclick(GtkWidget *w, gpointer data)
{
  selectedsmvector++;
  if(selectedsmvector == numsmvect) selectedsmvector = 0;
  updatesmsliders();
  updatesmvectorprev();
}

static void smaddclick(GtkWidget *w, gpointer data)
{
  smvector[numsmvect].x = 0.5;
  smvector[numsmvect].y = 0.5;
  smvector[numsmvect].siz = 50.0;
  smvector[numsmvect].str = 1.0;
  selectedsmvector = numsmvect;
  numsmvect++;
  updatesmsliders();
  updatesmvectorprev();
  updatesmpreviewprev();
}

static void smdeleteclick(GtkWidget *w, gpointer data)
{
  int i;

  for (i = selectedsmvector; i < numsmvect-1; i++)
    {
      smvector[i] = smvector[i+1];
    }
  numsmvect--;
  if(selectedsmvector >= numsmvect) selectedsmvector = 0;
  updatesmsliders();
  updatesmvectorprev();
  updatesmpreviewprev();
}

static void smmapclick(GtkWidget *w, GdkEventButton *event)
{
  if(event->button == 1) {
    smvector[selectedsmvector].x = event->x / (double)OMWIDTH;
    smvector[selectedsmvector].y = event->y / (double)OMHEIGHT;

  } else if(event->button == 2) {
    if(numsmvect + 1 == MAXSIZEVECT) return;
    smvector[numsmvect].x = event->x / (double)OMWIDTH;
    smvector[numsmvect].y = event->y / (double)OMHEIGHT;
    smvector[numsmvect].siz = 0.0;
    smvector[numsmvect].str = 1.0;
    selectedsmvector = numsmvect;
    numsmvect++;
    updatesmsliders();
  }
#if 0
  else if(event->button == 3) {
    double d;
    d = atan2(OMWIDTH * smvector[selectedsmvector].x - event->x,
               OMHEIGHT * smvector[selectedsmvector].y - event->y);
    smvector[selectedsmvector].dir = radtodeg(d);
    updatesmsliders();
    */
  }
#endif
  updatesmvectorprev();
  updatesmpreviewprev();
}

static void angsmadjmove(GtkWidget *w, gpointer data)
{
  if (!smadjignore)
    {
      smvector[selectedsmvector].siz = GTK_ADJUSTMENT(sizadjust)->value;
      updatesmvectorprev();
      updatesmpreviewprev();
    }
}

static void strsmadjmove(GtkWidget *w, gpointer data)
{
  if (!smadjignore)
    {
      smvector[selectedsmvector].str = GTK_ADJUSTMENT(smstradjust)->value;
      updatesmvectorprev();
      updatesmpreviewprev();
    }
}

static void smstrexpsmadjmove(GtkWidget *w, gpointer data)
{
  if (!smadjignore)
    {
      updatesmvectorprev();
      updatesmpreviewprev();
    }
}

static void
smresponse (GtkWidget *widget,
            gint       response_id,
            gpointer   data)
{
  switch (response_id)
    {
    case RESPONSE_APPLY:
    case GTK_RESPONSE_OK:
      {
        gint i;

        for (i = 0; i < numsmvect; i++)
          pcvals.sizevector[i] = smvector[i];

        pcvals.numsizevector = numsmvect;
        pcvals.sizestrexp  = GTK_ADJUSTMENT (smstrexpadjust)->value;
        pcvals.sizevoronoi = GTK_TOGGLE_BUTTON (sizevoronoi)->active;
      }
      break;
    }

  if (response_id != RESPONSE_APPLY)
    gtk_widget_hide (widget);
}

static void initsmvectors(void)
{
  if (pcvals.numsizevector)
    {
      gint i;

      numsmvect = pcvals.numsizevector;
      for (i = 0; i < numsmvect; i++)
         {
           smvector[i] = pcvals.sizevector[i];
         }
    }
  else
    {
      /* Shouldn't happen */
      numsmvect = 1;
      smvector[0].x = 0.5;
      smvector[0].y = 0.5;
      smvector[0].siz = 0.0;
      smvector[0].str = 1.0;
    }
  if(selectedsmvector >= numsmvect)
    selectedsmvector = numsmvect-1;
}

#if 0
static void update_sizemap_dialog(void)
{
  if (smwindow)
    {
      initsmvectors();

      gtk_adjustment_set_value(GTK_ADJUSTMENT(smstrexpadjust),
                                  pcvals.sizestrexp);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sizevoronoi),
                                       pcvals.sizevoronoi);

      updatesmvectorprev();
      updatesmpreviewprev();
    }
}
#endif

void create_sizemap_dialog(void)
{
  GtkWidget *tmpw, *tmpw2;
  GtkWidget *table1;
  GtkWidget *table2;
  GtkWidget *hbox;

  initsmvectors();

  if (smwindow)
    {
      updatesmvectorprev ();
      updatesmpreviewprev ();
      gtk_window_present (GTK_WINDOW (smwindow));
      return;
    }

  smwindow =
    gimp_dialog_new (_("Size Map Editor"), "gimpressionist",
                     NULL, 0,
                       gimp_standard_help_func, HELP_ID,

                       GTK_STOCK_APPLY,  RESPONSE_APPLY,
                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                       GTK_STOCK_OK,     GTK_RESPONSE_OK,

                     NULL);

  g_signal_connect (smwindow, "response",
                    G_CALLBACK (smresponse),
                    NULL);
  g_signal_connect (smwindow, "destroy",
                      G_CALLBACK (gtk_widget_destroyed),
                    &smwindow);

  table1 = gtk_table_new(2, 5, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 6);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (smwindow)->vbox), table1);
  gtk_widget_show(table1);

  tmpw2 = tmpw = gtk_frame_new( _("Smvectors"));
  gtk_container_set_border_width (GTK_CONTAINER (tmpw), 2);
  gtk_table_attach(GTK_TABLE(table1), tmpw, 0,1,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
  gtk_widget_show(tmpw);

  tmpw = hbox = gtk_hbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(tmpw2), tmpw);
  gtk_widget_show(tmpw);

  tmpw = gtk_event_box_new();
  gimp_help_set_help_data (tmpw, _("The smvector-field. Left-click to move selected smvector, Right-click to point it towards mouse, Middle-click to add a new smvector."), NULL);
  gtk_box_pack_start(GTK_BOX(hbox), tmpw, FALSE, FALSE, 0);
  tmpw2 = tmpw;

  tmpw = smvectorprev = gtk_preview_new(GTK_PREVIEW_COLOR);
  gtk_preview_size(GTK_PREVIEW(tmpw), OMWIDTH, OMHEIGHT);
  gtk_container_add(GTK_CONTAINER(tmpw2), tmpw);
  gtk_widget_show(tmpw);
  gtk_widget_add_events (tmpw2, GDK_BUTTON_PRESS_MASK);
  g_signal_connect(tmpw2, "button_press_event",
                   G_CALLBACK (smmapclick), NULL);
  gtk_widget_show(tmpw2);

  smvectprevbrightadjust = gtk_adjustment_new(50.0, 0.0, 100.0, 1.0, 1.0, 1.0);
  tmpw = gtk_vscale_new(GTK_ADJUSTMENT(smvectprevbrightadjust));
  gtk_scale_set_draw_value (GTK_SCALE (tmpw), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), tmpw,FALSE,FALSE,0);
  gtk_widget_show(tmpw);
  g_signal_connect (smvectprevbrightadjust, "value_changed",
                    G_CALLBACK (updatesmvectorprev), NULL);
  gimp_help_set_help_data (tmpw, _("Adjust the preview's brightness"), NULL);

  tmpw2 = tmpw = gtk_frame_new( _("Preview"));
  gtk_container_set_border_width (GTK_CONTAINER (tmpw), 2);
  gtk_table_attach(GTK_TABLE(table1), tmpw, 1,2,0,1,GTK_EXPAND,GTK_EXPAND,0,0);
  gtk_widget_show(tmpw);

  tmpw = smpreviewprev = gtk_preview_new(GTK_PREVIEW_COLOR);
  gtk_preview_size(GTK_PREVIEW(tmpw), OMWIDTH, OMHEIGHT);
  gtk_container_add(GTK_CONTAINER(tmpw2), tmpw);
  gtk_widget_show(tmpw);

  hbox = gtk_hbox_new(TRUE,0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 2);
  gtk_table_attach_defaults(GTK_TABLE(table1), hbox, 0, 1, 1, 2);
  gtk_widget_show(hbox);

  prev_button = tmpw = gtk_button_new_with_mnemonic("_<<");
  gtk_box_pack_start(GTK_BOX(hbox),tmpw,FALSE,TRUE,0);
  gtk_widget_show(tmpw);
  g_signal_connect (tmpw, "clicked",
                    G_CALLBACK (smprevclick), NULL);
  gimp_help_set_help_data (tmpw, _("Select previous smvector"), NULL);

  next_button = tmpw = gtk_button_new_with_mnemonic("_>>");
  gtk_box_pack_start(GTK_BOX(hbox),tmpw,FALSE,TRUE,0);
  gtk_widget_show(tmpw);
  g_signal_connect (tmpw, "clicked",
                      G_CALLBACK(smnextclick), NULL);
  gimp_help_set_help_data (tmpw, _("Select next smvector"), NULL);

  add_button = tmpw = gtk_button_new_with_mnemonic( _("A_dd"));
  gtk_box_pack_start(GTK_BOX(hbox),tmpw,FALSE,TRUE,0);
  gtk_widget_show(tmpw);
  g_signal_connect (tmpw, "clicked",
                      G_CALLBACK(smaddclick), NULL);
  gimp_help_set_help_data (tmpw, _("Add new smvector"), NULL);

  kill_button = tmpw = gtk_button_new_with_mnemonic( _("_Kill"));
  gtk_box_pack_start(GTK_BOX(hbox),tmpw,FALSE,TRUE,0);
  gtk_widget_show(tmpw);
  g_signal_connect (tmpw, "clicked",
                      G_CALLBACK(smdeleteclick), NULL);
  gimp_help_set_help_data (tmpw, _("Delete selected smvector"), NULL);

  table2 = gtk_table_new(3, 4, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE(table2), 4);
  gtk_table_attach_defaults(GTK_TABLE(table1), table2, 0, 2, 2, 3);
  gtk_widget_show(table2);

  sizadjust =
    gimp_scale_entry_new (GTK_TABLE(table2), 0, 0,
                             _("_Size:"),
                             150, 6, 50.0,
                             0.0, 100.0, 1.0, 10.0, 1,
                             TRUE, 0, 0,
                             _("Change the angle of the selected smvector"),
                             NULL);
  g_signal_connect (sizadjust, "value_changed", G_CALLBACK(angsmadjmove),
                      NULL);

  smstradjust =
    gimp_scale_entry_new (GTK_TABLE(table2), 0, 1,
                             _("S_trength:"),
                             150, 6, 1.0,
                             0.1, 5.0, 0.1, 0.5, 1,
                             TRUE, 0, 0,
                             _("Change the strength of the selected smvector"),
                             NULL);
  g_signal_connect (smstradjust, "value_changed", G_CALLBACK(strsmadjmove),
                      NULL);

  smstrexpadjust =
    gimp_scale_entry_new (GTK_TABLE(table2), 0, 2,
                             _("St_rength exp.:"),
                             150, 6, 1.0,
                             0.1, 10.9, 0.1, 0.5, 1,
                             TRUE, 0, 0,
                             _("Change the exponent of the strength"),
                             NULL);
  g_signal_connect (smstrexpadjust, "value_changed",
                      G_CALLBACK(smstrexpsmadjmove), NULL);

  sizevoronoi = tmpw = gtk_check_button_new_with_mnemonic( _("_Voronoi"));
  gtk_table_attach_defaults(GTK_TABLE(table2), tmpw, 3, 4, 0, 1);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmpw), FALSE);
  gtk_widget_show (tmpw);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmpw), pcvals.sizevoronoi);
  g_signal_connect(tmpw, "clicked",
                     G_CALLBACK(smstrexpsmadjmove), NULL);
  gimp_help_set_help_data (tmpw, _("Voronoi-mode makes only the smvector closest to the given point have any influence"), NULL);

  gtk_widget_show(smwindow);

  updatesmvectorprev();
  updatesmpreviewprev();
}
