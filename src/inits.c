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

#include "defcmap.h"
#include "fonts.h"
#include "inits.h"
#include "ovdisis.h"
#include "various.h"


/* Initialization functions */

void init_workstation(VDI_Workstation *wk)
{
  int         i;
  Visual_Attr visual_attr;

  VISUAL_INQUIRE(wk, &visual_attr);

  /* Setup data for v_opnwk */

  wk->dev.attr.xres = visual_attr.x_res - 1;  /* Max visible width */
  wk->dev.attr.yres = visual_attr.y_res - 1;  /* Max visible height */
  wk->dev.attr.noscale = 0;                   /* Device Coord Units flag */
  wk->dev.attr.wpixel = 372;                  /* Width of pixel in microns */
  wk->dev.attr.hpixel = 372;                  /* Height of pixel in microns */
  wk->dev.attr.cheights = 3;                  /* Number of different text sizes */
  wk->dev.attr.linetypes = 7;                 /* Number of line types */
  wk->dev.attr.linewidths = 0;                /* Number of line widths */
  wk->dev.attr.markertypes = 6;               /* Number of marker types */
  wk->dev.attr.markersizes = 8;               /* Number of marker sizes */
  wk->dev.attr.faces = 1;                     /* Number of fonts on device */
  wk->dev.attr.patterns = 24;                 /* Number of patterns */
  wk->dev.attr.hatches = 12;                  /* Number of hatch styles */
  if(visual_attr.number_of_colours > 256) {
    /* 256 virtual pens for truecolour */
    wk->dev.attr.colors = 256;
  } else {
    /* Number of colours is the same as number of virtual pens*/
    wk->dev.attr.colors = visual_attr.number_of_colours;
  }
  wk->dev.attr.ngdps = 10;                    /* Number of GDPs */
  for ( i = 0 ; i < 10 ; i++ ) {
    wk->dev.attr.cangdps[i] = i+1;            /* List of GDPs supported */
  }
  wk->dev.attr.gdpattr[0] = 3;                /* GDP attributes */
  wk->dev.attr.gdpattr[1] = 0;
  wk->dev.attr.gdpattr[2] = 3;
  wk->dev.attr.gdpattr[3] = 3;
  wk->dev.attr.gdpattr[4] = 3;
  wk->dev.attr.gdpattr[5] = 0;
  wk->dev.attr.gdpattr[6] = 3;
  wk->dev.attr.gdpattr[7] = 0;
  wk->dev.attr.gdpattr[8] = 3;
  wk->dev.attr.gdpattr[9] = 2;
  /* Colour capability */
  wk->dev.attr.cancolor = (visual_attr.number_of_colours > 1 ) ? 1 : 0;
  wk->dev.attr.cantextrot = 1;				/* Text rotation */
  wk->dev.attr.canfillarea = 1;				/* Flag fill-out range */
  wk->dev.attr.cancellarray = 0;			/* Flag function cell-array */
  /* Number of available colours */
  wk->dev.attr.palette =
    (visual_attr.palette_size > 32767) ? 0 : visual_attr.palette_size;
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
  wk->dev.attr.minwmark = 15;				/* Minimum marker width */
  wk->dev.attr.minhmark = 11;				/* Minimum marker height */
  wk->dev.attr.maxwmark = 120;				/* Maximum marker width */
  wk->dev.attr.maxhmark = 120;				/* Maximum marker height */

  /* Setup data for vq_extnd() */

  wk->inq.attr.screentype = 4;				/* screen type */
  /* number of bg colours */
  wk->inq.attr.bgcolors =
    (visual_attr.palette_size > 32767) ? 0 : visual_attr.palette_size;
  wk->inq.attr.textfx = -1;				/* text effects */
  wk->inq.attr.canscale = 0;				/* enlarging raster */
  /* number of planes */
  wk->inq.attr.planes = visual_attr.bits_per_pixel;
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
  int planes = (wk->inq.attr.planes < 8) ? wk->inq.attr.planes : 8;

  /* I'm using the allocated cmap from FBgetcmap()
     from within the FB struct. Maybe I should
     have a separate pointer outside it. It doesn't
     matter now though, since they both would point
     at the same cmap. */
  for (i = 0; i < (1 << planes); i++) {
    ti = gem2tos_color(planes, i);
    
    wk->vdi_cmap.red[i]   =
      ((int)default_cmap[planes]->red[ti]   * 1000) / 65535;
    wk->vdi_cmap.green[i] =
      ((int)default_cmap[planes]->green[ti] * 1000) / 65535;
    wk->vdi_cmap.blue[i]  =
      ((int)default_cmap[planes]->blue[ti]  * 1000) / 65535;
  }
  
  VISUAL_PUT_CMAP(wk);
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

void init_device(VDI_Workstation *wk)
{
  int i;

  for(i = 1 ; i <= 4 ; i++)
    wk->device_mode[i] = REQUEST_MODE;
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

  /* Tell visual about the default font */
  VISUAL_SET_FONT (wk,
                   wk->text_a.font->data,
                   wk->text_a.font->wcell,
                   wk->text_a.font->formheight);
}



/* Copy functions
 *
 * Some of these functions check if the pid of the workstations
 * are different. If they are, things are initiated as well as 
 * copied.
 */
 
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

  /* Copy the cmap from the physical workstation. */
  for(i = 0; i < wk1->dev.attr.colors; i++) {
    wk2->vdi_cmap.red[i]   = wk1->vdi_cmap.red[i];
    wk2->vdi_cmap.green[i] = wk1->vdi_cmap.green[i];
    wk2->vdi_cmap.blue[i]  = wk1->vdi_cmap.blue[i];
  }

  /* FIXME: Check if we need to do this */
#if 0
  /* If we're within the same process, no need to do more. */
  if(wk1->pid == wk2->pid)
    return;

  /* In case we are in different processes, initialize the 
   * framebuffer cmap with the new values.
   */
  switch (wk1->inq.attr.planes) {
  case 1:
  case 2:
  case 4:
  case 8:
    /* wk2->fb->cmap is set by FBgetcmap */
    FBgetcmap(wk2->fb);
    ADEBUG("v_opnwk: FB cmap length %d allocated: %p\n",
	   wk2->fb->cmap->len, wk2->fb->cmap);

    for (i = 0; i < wk2->fb->cmap->len; i++) {
      ti = tos2gem_color(wk2->inq.attr.planes, i);

      wk2->fb->cmap->red[i]   = (wk2->vdi_cmap.red[ti]   * 65536) / 1000;
      wk2->fb->cmap->green[i] = (wk2->vdi_cmap.green[ti] * 65536) / 1000;
      wk2->fb->cmap->blue[i]  = (wk2->vdi_cmap.blue[ti]  * 65536) / 1000;
    }
    /* Put the changed cmap on to the FrameBuffer */
    wk2->fb->cmap->start = 0;
    wk2->fb->cmap->len = (1 << wk2->inq.attr.planes);
    wk2->fb->cmap->end = (1 << wk2->inq.attr.planes) - 1;

    /* Actually, we don't need to set the palette since we're
     * using the same Virtual Console for the framebuffer
     */
    /* Or maybe we do after all.. urgh.. */
#if 0
    sleep(1);			/* This is apparently needed in 8 bit mode (oFBis bug?) */

    FBputcmap(wk2->fb, wk2->fb->cmap);
#endif
    break;

  case 16: /* TrueColor mode, nothing to be done */
    break;
  default:
    /* Unknown mode, do nothing */
    /* Perhaps it would be better to exit here? */
    break;
  }
#endif
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

void copy_device(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  int i;

  for(i = 1 ; i <= 4 ; i++)
    wk2->device_mode[i] = wk1->device_mode[i];
}

/* This should probably really reserve new and copy the data */
void copy_text(VDI_Workstation *wk1, VDI_Workstation *wk2)
{
  wk2->text_a = wk1->text_a;
  wk2->fonts = wk1->fonts;

  /* If we're within the same process, don't do more. */
  if(wk1->pid == wk2->pid)
    return;
    
  /* FIXME! */
  /* I'm lazy for now, and just initializing without copying! */
  init_text(wk2);  
}

