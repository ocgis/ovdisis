/*
** ofbis_visual_various.c
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#define DEBUGLEVEL 0

#include <ofbis.h>
#include <stdio.h>

#include "ofbis_visual.h"
#include "ovdisis.h"
#include "various.h"

int
ofbis_visual_get_pixel (void * fb,
			int    x,
			int    y) {
  return FBgetpixel(FB_T(fb), x, y);
}


void
ofbis_visual_put_pixel (void * fb,
			int    x,
			int    y,
			int    c) {
  FBputpixel(FB_T(fb), x, y, c);
}


void
ofbis_visual_hline (void * fb,
		    int    x1,
		    int    x2,
		    int    y,
		    int    c) {
  FBhline(FB_T(fb), x1, x2, y, c);
}


void
ofbis_visual_line (void * fb,
		   int    x1,
		   int    y1,
		   int    x2,
		   int    y2,
		   int    c) {
  FBline(FB_T(fb), x1, y1, x2, y2, c);
}


void
ofbis_visual_bitblt (VDI_Workstation * vwk,
		     int               mode,
		     RECT *            srccor,
		     RECT *            dstcor,
		     MFDB *            src,
		     MFDB *            dst) {
  FBBLTPBLK * fbb;
  RECT        tmpcor;

  /* order the coordinates as I like them. */
  fix_rect(srccor);
  fix_rect(dstcor);

  /* Completely ignore what the user says what the destination 
     width and height should be and recalculate them here.
     They must be recalculated after the fix. */

  dstcor->x2 = srccor->x2 - srccor->x1 + dstcor->x1;
  dstcor->y2 = srccor->y2 - srccor->y1 + dstcor->y1;
  tmpcor = *dstcor;

  /* check first if clipping takes away everything,
     if destination is the screen */
  if(!dst->fd_addr && !do_rectclip(dstcor, &vwk->clip)) {
    return;
  }

  /* setup fbb according to the screen format */
  fbb = FBgetbltpblk(FB_T(vwk->visual->private));

  /* dst size is the same as src size here, only that dst
     might have been clipped earlier */
  fbb->b_wd = dstcor->x2 - dstcor->x1 + 1;
  fbb->b_ht = dstcor->y2 - dstcor->y1 + 1;
  fbb->op_tab = (mode << 0 |
		 mode << 8 |
		 mode << 16 |
		 mode << 24);

  /* adjust coordinates if destination is clipped */
  fbb->s_xmin = srccor->x1 + dstcor->x1-tmpcor.x1;
  fbb->s_ymin = srccor->y1 + dstcor->y1-tmpcor.y1;
  if(src->fd_addr) {
    fbb->plane_ct = src->fd_nplanes; /* use source no of bitplanes! */
    fbb->s_form = src->fd_addr;

    if(src->fd_stand) {
      /* block is in VDI format */

      fbb->s_nxwd = 2;
      fbb->s_nxln = src->fd_wdwidth * 2;
      fbb->s_nxpl = src->fd_wdwidth * 2 * src->fd_h;
    } /* else block is in machine dependent format */
    else 
    {
      /* block is in machine dependent format */

      fbb->s_nxwd = 2;
      fbb->s_nxln = src->fd_wdwidth * 2;
      fbb->s_nxpl = src->fd_wdwidth * 2 * src->fd_h;
    }      

    /*
    ** "machine dependent" could be discussed. 
    ** On one hand, machine dependent _is_ machine dependent, but
    ** on the other hand, machine dependent is most likely associated
    ** with Atari interleaved format. For now we treat them exactly the
    ** same
    */

  } /* else source is the screen, i.e. already setup by FBgetbltpblk() */

  fbb->d_xmin = dstcor->x1;
  fbb->d_ymin = dstcor->y1;
  if(dst->fd_addr) {
    fbb->d_form = dst->fd_addr;

    if(dst->fd_stand) {
      /* destination should be in VDI format */

      fbb->d_nxwd = 2;
      fbb->d_nxln = dst->fd_wdwidth * 2;
      fbb->d_nxpl = dst->fd_wdwidth * 2 * dst->fd_h;
    } /* else destination should be in machine dependent format */
    else
    {
      /* destination should be in machine dependent format */

      fbb->d_nxwd = 2;
      fbb->d_nxln = dst->fd_wdwidth * 2;
      fbb->d_nxpl = dst->fd_wdwidth * 2 * dst->fd_h;
    }

    /*
    ** "machine dependent" could be discussed. 
    ** On one hand, machine dependent _is_ machine dependent, but
    ** on the other hand, machine dependent is most likely associated
    ** with Atari interleaved format. For now we treat them exactly the
    ** same
    */

    /* See discussion about machine dependent above */

  } /* else destination is the screen, i.e. already setup by FBgetbltpblk() */

  FBbitblt (FB_T(vwk->visual->private), fbb);

  FBfreebltpblk(fbb);

  IDEBUG("vro_cpyfm: %d,%d -> %d,%d  w: %d h: %d mode: %d\n",fbb->s_xmin, fbb->s_ymin,
	 fbb->d_xmin, fbb->d_ymin, fbb->b_wd, fbb->b_ht, mode);
}


void
ofbis_visual_bitbltt (VDI_Workstation * vwk,
		      int               mode,
		      int               fgcol,
		      int               bgcol,
		      RECT *            srccor,
		      RECT *            dstcor,
		      MFDB *            src,
		      MFDB *            dst) {
  int         col_index;
  FBBLTPBLK * fbb;
  RECT        tmpcor;
  /* Completely ignore what the user says what the destination 
     width and height should be and recalculate them here.
     They must be recalculated after the fix. */

  dstcor->x2 = srccor->x2 - srccor->x1 + dstcor->x1;
  dstcor->y2 = srccor->y2 - srccor->y1 + dstcor->y1;
  tmpcor = *dstcor;

  /* check first if clipping takes away everything,
     if destination is the screen */
  if(!dst->fd_addr && !do_rectclip(dstcor, &vwk->clip)) {
    return;
  }

  /* setup fbb according to the screen format */
  fbb = FBgetbltpblk ((vwk->visual->private));

  /* emulate the four modes with different combinations
     of the ordinary 15 bitblt modes. */
  switch(mode) {
  case MD_TRANS:
    fbb->op_tab = (NOTS_AND_D << 24 |
		   NOTS_AND_D << 16 |
		   S_OR_D     <<  8 |
		   S_OR_D);
    break;

  case MD_XOR:
    fbb->op_tab = (S_XOR_D    << 24 |
		   S_XOR_D    << 16 |
		   S_XOR_D    <<  8 |
		   S_XOR_D);
    break;

  case MD_ERASE:
    fbb->op_tab = (S_AND_D    << 24 |
		   NOTS_OR_D  << 16 |
		   S_AND_D    <<  8 |
		   NOTS_OR_D);
    break;

  default: /* illegal mode specified, use replace mode */
  case MD_REPLACE:
    fbb->op_tab = (ALL_WHITE  << 24 |
		   NOT_S      << 16 |
		   S_ONLY     <<  8 |
		   ALL_BLACK);
    break;
  }

  /* foreground and background colour to put block in */
  col_index = gem2tos_color(vwk->inq.attr.planes, fgcol);
  fbb->fg_col = get_color(vwk, col_index);
  col_index = gem2tos_color(vwk->inq.attr.planes, bgcol);
  fbb->bg_col = get_color(vwk, col_index);

  /* dst size is the same as src size here, only that dst
     might have been clipped earlier */
  fbb->b_wd = dstcor->x2 - dstcor->x1 + 1;
  fbb->b_ht = dstcor->y2 - dstcor->y1 + 1;

  /* adjust coordinates if destination is clipped */
  fbb->s_xmin = srccor->x1 + dstcor->x1-tmpcor.x1;
  fbb->s_ymin = srccor->y1 + dstcor->y1-tmpcor.y1;
  if(src->fd_addr) {
    fbb->s_form = src->fd_addr;

    if(src->fd_stand) {
      /* block is in VDI format */

      fbb->s_nxwd = 2;
      fbb->s_nxln = src->fd_wdwidth * 2;
    } /* else block is in machine dependent format */

    /* See discussion about machine dependent in vro_cpyfm above */

  } /* else source is the screen, i.e. already setup by FBgetbltpblk() */

  /* use only the first bitplane from source block */
  fbb->s_nxpl = 0;

  fbb->d_xmin = dstcor->x1;
  fbb->d_ymin = dstcor->y1;
  if(dst->fd_addr) {
    fbb->d_form = dst->fd_addr;
    fbb->plane_ct = dst->fd_nplanes; /* use destination no of bitplanes! */

    if(dst->fd_stand) {
      /* destination should be in VDI format */

      fbb->d_nxwd = 2;
      fbb->d_nxln = dst->fd_wdwidth * 2;
      fbb->d_nxpl = dst->fd_wdwidth * 2 * dst->fd_h;
    } /* else destination should be in machine dependent format */

    /* See discussion about machine dependent in vro_cpyfm above */

  } /* else destination is the screen, i.e. already setup by FBgetbltpblk() */

  FBbitblt(FB_T(vwk->visual->private), fbb);

  FBfreebltpblk(fbb);

  IDEBUG("vrt_cpyfm: %d,%d -> %d,%d  w: %d h: %d mode: %d\n",fbb->s_xmin, fbb->s_ymin,
	 fbb->d_xmin, fbb->d_ymin, fbb->b_wd, fbb->b_ht, mode);
}


void
ofbis_visual_put_char (void * fb,
                       int    x,
                       int    y,
                       int    col,
                       int    ch) {
  FBputchar(FB_T(fb), x, y, col, 0, ch);
}


void
ofbis_visual_set_font (void * fb,
		       void * data,
		       int    width,
		       int    height) {
  FBFONT fnt;

  fnt.data = data;
  fnt.width = width;
  fnt.height = height;

  FBsetfont (FB_T(fb), &fnt);
}


void
ofbis_visual_inquire (void *        fb,
                      Visual_Attr * attr) {
  attr->x_res = FB_T(fb)->vinf.xres;
  attr->y_res = FB_T(fb)->vinf.yres;
  attr->palette_size = ((1 << FB_T(fb)->vinf.red.length) *
                        (1 << FB_T(fb)->vinf.green.length) *
                        (1 << FB_T(fb)->vinf.blue.length));
  attr->number_of_colours = 1 << FB_T(fb)->vinf.bits_per_pixel;
  attr->bits_per_pixel = FB_T(fb)->vinf.bits_per_pixel;
}
