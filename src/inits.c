#include <stdio.h>
#include <unistd.h>		/* for sleep() */

#include "ovdisis.h"
#include "various.h"
#include "defcmap.h"
#include "fonts/fonts.h"


/* Initialization functions */

void init_cmap(VDI_Workstation *wk)
{
  int i, ti;

  /* I'm using the allocated cmap from FBgetcmap()
     from within the FB struct. Maybe I should
     have a separate pointer outside it. It doesn't
     matter now though, since they both would point
     at the same cmap. */
  switch (wk->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    /* wk->fb->cmap is set by FBgetcmap */
    FBgetcmap(wk->fb);
    ADEBUG("v_opnwk: FB cmap length %d allocated: %p\n",
	   wk->fb->cmap->len, wk->fb->cmap);

    for (i = 0; i < wk->fb->cmap->len; i++) {
      wk->fb->cmap->red[i] =
	default_cmap[wk->inq.attr.planes]->red[i];
      wk->fb->cmap->green[i] =
	default_cmap[wk->inq.attr.planes]->green[i];
      wk->fb->cmap->blue[i] =
	default_cmap[wk->inq.attr.planes]->blue[i];

      ti = gem2tos_color(wk->inq.attr.planes, i);

      wk->vdi_cmap.red[i] =
	((int)default_cmap[wk->inq.attr.planes]->red[ti] * 1000) / 65535;
      wk->vdi_cmap.green[i] =
	((int)default_cmap[wk->inq.attr.planes]->green[ti] * 1000) / 65535;
      wk->vdi_cmap.blue[i] =
	((int)default_cmap[wk->inq.attr.planes]->blue[ti] * 1000) / 65535;
    }
    /* Put the changed cmap on to the FrameBuffer */
    wk->fb->cmap->start = 0;
    wk->fb->cmap->len = (1 << wk->inq.attr.planes);
    wk->fb->cmap->end = (1 << wk->inq.attr.planes) - 1;

    sleep(1);			/* This is apparently needed in 8 bit mode (oFBis bug?) */

    FBputcmap(wk->fb, wk->fb->cmap);
    break;

  case 16:			/* TrueColor mode, use 256 virtual pens */
    /* cmap not used in TrueColor mode */
    for (i = 0; i < default_cmap[8]->len; i++) {
      /* Convert colour index as in 8 bpl mode */
      ti = gem2tos_color(8, i);

      wk->vdi_cmap.red[i] = ((int)default_cmap[8]->red[ti] * 1000) / 65535;
      wk->vdi_cmap.green[i] = ((int)default_cmap[8]->green[ti] * 1000) / 65535;
      wk->vdi_cmap.blue[i] = ((int)default_cmap[8]->blue[ti] * 1000) / 65535;
    }
    break;

  default:
    /* Unknown mode, do nothing */
    /* Perhaps it would be better to exit here? */
    break;
  }
}


/* Check all these in real TOS!!! */

void init_marker(VDI_Workstation *wk)
{
  wk->marker_a.color = 1;
  wk->marker_a.width = 1;
  wk->marker_a.height = 1;
  wk->marker_a.type = 1;
}

void init_line(VDI_Workstation *wk)
{
  wk->line_a.color = 1;
  wk->line_a.width = 1;
  wk->line_a.ends_beg = 0;
  wk->line_a.ends_end = 0;
  wk->line_a.type = 0;
  wk->line_a.udsty = 0xffff;
}

void init_fill(VDI_Workstation *wk)
{
  wk->fill_a.color = 1;
  wk->fill_a.interior = 1;
  wk->fill_a.style = 0;
  wk->fill_a.perimeter = 1;
  wk->fill_a.udpat_nbpl = 1;
  /* skip user defined pattern for now */
}


static void insert_font(FontInfo **fonts, FontInfo *new)
{
  FontInfo *fp=*fonts;

  if(!fp) {
    *fonts = new;
  } else {
    /* Find last font */
    while(fp->next) fp = fp->next;
    
    fp->next = new;
  }

  new->next = NULL;
}

void init_text(VDI_Workstation *wk)
{
  FBFONT fnt;

  wk->fonts = NULL;

  insert_font(&wk->fonts, &system_6x6);
  insert_font(&wk->fonts, &system_8x8);
  insert_font(&wk->fonts, &system_8x16);

  /* default font is 8x8, a bit of a hack here... */
  wk->text_a.font = &system_8x8;

  wk->text_a.fontid = 1;
  wk->text_a.color = 1;
  wk->text_a.textheight = wk->text_a.font->top + wk->text_a.font->bottom;
  wk->text_a.textwidth = wk->text_a.font->wchar;
  wk->text_a.cellheight = wk->text_a.font->formheight;
  wk->text_a.cellwidth = wk->text_a.font->wcell;
  wk->text_a.effects = 0x0000;
  wk->text_a.h_alignment = 0;
  wk->text_a.v_alignment = 0;
  wk->text_a.rotation = 0;

  /* Tell oFBis about the default font */
  fnt.data = wk->text_a.font->data;
  fnt.width = wk->text_a.font->wcell;
  fnt.height = wk->text_a.font->formheight;
  FBsetfont(wk->fb, &fnt);
}



/* Copy functions, might seem pointless at the moment */

void copy_cmap(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  int i;

  for(i = 0; i < wk1->dev.attr.colors; i++) {
    wk2->vdi_cmap.red[i]   = wk1->vdi_cmap.red[i];
    wk2->vdi_cmap.green[i] = wk1->vdi_cmap.green[i];
    wk2->vdi_cmap.blue[i]  = wk1->vdi_cmap.blue[i];
  }
}

void copy_marker(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  wk2->marker_a = wk1->marker_a;
}

void copy_line(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  wk2->line_a = wk1->line_a;
}

void copy_fill(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  wk2->fill_a = wk1->fill_a;
}

/* This should probably really reserve new and copy the data */
void copy_text(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  wk2->text_a = wk1->text_a;
  wk2->fonts = wk1->fonts;
}

