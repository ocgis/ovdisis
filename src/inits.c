/*
 * inits.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include <stdio.h>
#include <unistd.h>		/* for sleep() */

#include "ovdisis.h"
#include "various.h"
#include "defcmap.h"

#include "fonts.h"


/* Initialization functions */

void init_workstation(VDI_Workstation *wk)
{
  int i;
  int palsize = ((1 << wk->fb->vinf.red.length) *
		 (1 << wk->fb->vinf.green.length) *
		 (1 << wk->fb->vinf.blue.length));

  /* Setup data for v_opnwk */

  wk->dev.attr.xres = wk->fb->vinf.xres_virtual - 1;	/* Max addressable width */
  wk->dev.attr.yres = wk->fb->vinf.yres_virtual - 1;	/* Max addressable height */
  wk->dev.attr.noscale = 0;				/* Device Coord Units flag */
  wk->dev.attr.wpixel = 372;				/* Width of pixel in microns */
  wk->dev.attr.hpixel = 372;				/* Height of pixel in microns */
  wk->dev.attr.cheights = 3;				/* Number of different text sizes */
  wk->dev.attr.linetypes = 0;				/* Number of line types */
  wk->dev.attr.linewidths = 0;				/* Number of line widths */
  wk->dev.attr.markertypes = 6;				/* Number of marker types */
  wk->dev.attr.markersizes = 0;				/* Number of marker sizes */
  wk->dev.attr.faces = 1;				/* Number of fonts on device */
  wk->dev.attr.patterns = 24;				/* Number of patterns */
  wk->dev.attr.hatches = 12;				/* Number of hatch styles */
  wk->dev.attr.colors = 1 << wk->fb->vinf.bits_per_pixel; /* Number of colours, virt pens (TC) */
  wk->dev.attr.ngdps = 10;				/* Number of GDPs */
  for ( i = 0 ; i < 10 ; i++ ) {
    wk->dev.attr.cangdps[i] = i+1;			/* List of GDPs supported */
  }
  wk->dev.attr.gdpattr[0] = 3;				/* GDP attributes */
  wk->dev.attr.gdpattr[1] = 0;
  wk->dev.attr.gdpattr[2] = 3;
  wk->dev.attr.gdpattr[3] = 3;
  wk->dev.attr.gdpattr[4] = 3;
  wk->dev.attr.gdpattr[5] = 0;
  wk->dev.attr.gdpattr[6] = 3;
  wk->dev.attr.gdpattr[7] = 0;
  wk->dev.attr.gdpattr[8] = 3;
  wk->dev.attr.gdpattr[9] = 2;
  wk->dev.attr.cancolor = ( wk->fb->vinf.bits_per_pixel > 1 ) ? 1 : 0; /* Colour capability */
  wk->dev.attr.cantextrot = 0;				/* Text rotation */
  wk->dev.attr.canfillarea = 0;				/* Flag fill-out range */
  wk->dev.attr.cancellarray = 0;			/* Flag function cell-array */
  wk->dev.attr.palette = (palsize>32767)? 0 : palsize;	/* Number of available colours */
  wk->dev.attr.locators = 2;				/* Graphic cursor control */
  wk->dev.attr.valuators = 1;				/* Number-changeable inputs */
  wk->dev.attr.choicedevs = 1;				/* Key choice */
  wk->dev.attr.stringdevs = 1;				/* Alphanumeric input */
  wk->dev.attr.wstype = 2;				/* Type of work device */

  wk->dev.attr.minwchar = 6;				/* Minimum character width */
  wk->dev.attr.minhchar = 6;				/* Minimum character height */
  wk->dev.attr.maxwchar = 8;				/* Maximum character width */
  wk->dev.attr.maxhchar = 16;				/* Maximum character height */
  wk->dev.attr.minwline = 1;				/* Minimum line width */
  wk->dev.attr.zero5 = 0;				/* reserved */
  wk->dev.attr.maxwline = 1;				/* Maximum line width */
  wk->dev.attr.zero7 = 0;				/* reserved */
  wk->dev.attr.minwmark = 1;				/* Minimum marker width */
  wk->dev.attr.minhmark = 1;				/* Minimum marker height */
  wk->dev.attr.maxwmark = -1;				/* Maximum marker width */
  wk->dev.attr.maxhmark = -1;				/* Maximum marker height */


  /* Setup data for vq_extnd() */

  wk->inq.attr.screentype = 4;				/* screen type */
  wk->inq.attr.bgcolors = (palsize>32767)? 0 : palsize;	/* number of bg colours */
  wk->inq.attr.textfx = -1;				/* text effects */
  wk->inq.attr.canscale = 0;				/* enlarging raster */
  wk->inq.attr.planes = wk->fb->vinf.bits_per_pixel;	/* num of planes */
  wk->inq.attr.lut = 1;					/* look up table */
  wk->inq.attr.rops = 100;				/* 16*16 rops per second - no idea! */
  wk->inq.attr.cancontourfill = 0;			/* contour fill */
  wk->inq.attr.textrot = 0;				/* text rotation */
  wk->inq.attr.writemodes = 4;				/* num of drawing modes */
  wk->inq.attr.inputmodes = 2;				/* highest possible input mode */
  wk->inq.attr.textalign = 0;				/* text alignment */
  wk->inq.attr.inking = 0;				/* colour change with pens */
  wk->inq.attr.rubberbanding = 0;			/* colour change with band shift */
  wk->inq.attr.maxvertices = -1;			/* max points in pline,pmarker,farea */
  wk->inq.attr.maxintin = -1;				/* max len of intin */
  wk->inq.attr.mousebuttons = 2;			/* number of mouse buttons */
  wk->inq.attr.widestyles = 0;				/* line types for wide lines */
  wk->inq.attr.widemodes = 0;				/* drawing modes for wide lines */
  for ( i = 0 ; i < 25+12 ; i++ ) {
    wk->inq.attr.reserved[i] = 0;			/* clear reserved elements */
  }
}

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



/* Copy functions */

void copy_workstation(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  int i;

  /* v_opnvwk values */
  for (i = 0; i < 45+12; i++)
    wk2->dev.tab[i] = wk1->dev.tab[i];

  /* vq_extnd values */
  for (i = 0; i < 45+12; i++)
    wk2->inq.tab[i] = wk1->inq.tab[i];
}

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

