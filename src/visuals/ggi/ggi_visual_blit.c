/*
** ggi_visual_blit.c
**
** Copyright 2000 Christer Gustavsson <cg@nocrew.org>
**           2000 Vincent Barrilliot <vincent_barrilliot@yahoo.com)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <ggi/ggi.h>
#include <stdio.h>

#include "ggi_visual.h"
#include "ggi_visual_blit.h"
#include "ggi_visual_various.h"
#include "ovdisis.h"
#include "various.h"

static void all_white(ggi_color src, ggi_color *dst) {
  if( src.r == src.g  &&  src.g == src.b  &&  src.b == 0xffff )
    *dst = src;
}

static void s_and_d(ggi_color src, ggi_color *dst) {
  dst->r &= src.r;
  dst->g &= src.g;
  dst->b &= src.b;
}

static void s_and_notd(ggi_color src, ggi_color *dst) {
  dst->r = src.r & ~dst->r;
  dst->g = src.g & ~dst->g;
  dst->b = src.b & ~dst->b;

}

static void s_only(ggi_color src, ggi_color *dst) {
 *dst = src;
}

static void nots_and_d(ggi_color src, ggi_color *dst) {
  dst->r = ~src.r & dst->r;
  dst->g = ~src.g & dst->g;
  dst->b = ~src.b & dst->b;
}

static void d_only(ggi_color src, ggi_color *dst) {
}

static void s_xor_d(ggi_color src, ggi_color *dst) {
  dst->r = src.r ^ dst->r;
  dst->g = src.g ^ dst->g;
  dst->b = src.b ^ dst->b;
}

static void s_or_d(ggi_color src, ggi_color *dst) {
  dst->r |= src.r;
  dst->g |= src.g;
  dst->b |= src.b;
}

static void not_sord(ggi_color src, ggi_color *dst) {
  dst->r = ~(src.r | dst->r);
  dst->g = ~(src.g | dst->g);
  dst->b = ~(src.b | dst->b);
}

static void not_sxord(ggi_color src, ggi_color *dst) {
  dst->r = ~(src.r ^ dst->r);
  dst->g = ~(src.g ^ dst->g);
  dst->b = ~(src.b ^ dst->b);
}

static void not_d(ggi_color src, ggi_color *dst) {
  dst->r = ~dst->r;
  dst->g = ~dst->g;
  dst->b = ~dst->b;
}

static void s_or_notd(ggi_color src, ggi_color *dst) {
  dst->r = src.r | ~dst->r;
  dst->g = src.g | ~dst->g;
  dst->b = src.b | ~dst->b;
}

static void not_s(ggi_color src, ggi_color *dst) {
  dst->r = ~src.r;
  dst->g = ~src.g;
  dst->b = ~src.b;
}

static void nots_or_d(ggi_color src, ggi_color *dst) {
  dst->r |= ~src.r;
  dst->g |= ~src.g;
  dst->b |= ~src.b;
}

static void not_sandd(ggi_color src, ggi_color *dst) {
  dst->r = ~(src.r & dst->r);
  dst->g = ~(src.g & dst->g);
  dst->b = ~(src.b & dst->b);
}


static void all_black(ggi_color src, ggi_color *dst) {
  if( !src.r && !src.g && !src.b )
    *dst = src;
}


typedef void ReplacementMode(ggi_color, ggi_color*);

static ReplacementMode *replacement_mode[] = {
  /* 0  */ all_white, s_and_d, s_and_notd, s_only, nots_and_d,
  /* 5  */ d_only, s_xor_d, s_or_d, not_sord, not_sxord,
  /* 10 */ not_d, s_or_notd, not_s, nots_or_d, not_sandd,
  /* 15 */ all_black /* The All Blacks are the famous rugby team of New Zealand, and
   * in a Rugby team there are 15 players. Is it just by chance that this "all_black"
   * mode is #15 ???? */
};

void
ggi_visual_bitblt (VDI_Workstation * vwk,
		   int               mode,
		   RECT *            srccor,
		   RECT *            dstcor,
		   MFDB *            src,
		   MFDB *            dst)
{
  if( srccor->x1 == srccor->x2  ||  srccor->y1 == srccor->y2 )
    /* There's no point trying to copy a null area */
    return;

  /* Fix the coordinates so that the width and the height of the
   * rectangle are positive */
  fix_rect(srccor);
  fix_rect(dstcor);


  /* See if we can use GGI functions */
  if( src->fd_addr == NULL ) {
    /* Copy from screen */
    if( dst->fd_addr == NULL ) {
      /* to screen */
      switch( mode ) {
      case S_ONLY:
    	ggiCopyBox( VISUAL_T(vwk->visual->private),
		    srccor->x1,
		    srccor->y1,
		    srccor->x2 - srccor->x1 + 1,
		    srccor->y2 - srccor->y1 + 1,
		    dstcor->x1,
		    dstcor->y1);
	return;
	
      case D_ONLY:
	/* I am not sure that I understand the purpose of this */
    	ggiCopyBox( VISUAL_T(vwk->visual->private),
		    dstcor->x1,
		    dstcor->y1,
		    dstcor->x2 - dstcor->x1 + 1,
		    dstcor->y2 - dstcor->y1 + 1,
		    dstcor->x1,
		    dstcor->y1);
	return;

      default: {
	/* We assume that oVDIsis has already checked that the mode
	 * is between 0 and 15
	 * gargl, I bet this code is sloooooooow */
	int x,y;
	int w,h;
	ggi_visual_t *vis = &VISUAL_T(vwk->visual->private); /* cache as much as possible */
	ggi_pixel pixel_src, pixel_dst;
	ggi_color color_src, color_dst;

	w = min( srccor->x2 - srccor->x1  ,  dstcor->x2 - dstcor->x1 );
	h = min( srccor->y2 - srccor->y1  ,  dstcor->y2 - dstcor->y1 );

	for( y = 0 ; y < h ; y++ ) {
	  for( x = srccor->x1  ;  x < w ; x++ ) {
	    /* Get the display-dependent pixel formats */
	    ggiGetPixel( *vis, srccor->x1 + x, srccor->y1 + y, &pixel_src );
	    ggiGetPixel( *vis, dstcor->x1 + x, dstcor->y1 + y, &pixel_dst );

	    ggiUnmapPixel( *vis, pixel_src, &color_src);
	    ggiUnmapPixel( *vis, pixel_dst, &color_dst);
	    	    
	    /* Process */
	    replacement_mode[mode]( color_src, &color_dst );

	    pixel_dst = ggiMapColor( *vis, &color_dst);
	    ggiPutPixel( *vis, dstcor->x1 + x, dstcor->y1 + y, pixel_dst);
	  }
	}
	return;
	}
      }
    }
  }
  else {
    fprintf(stderr,
	    "Implement ggi_visual_bitblt (memory to display or vice versa\n");
  }
}


void
ggi_visual_bitbltt (VDI_Workstation * vwk,
		    int               mode,
		    int               fgcol,
		    int               bgcol,
		    RECT *            srccor,
		    RECT *            dstcor,
		    MFDB *            src,
		    MFDB *            dst)
{
  if((src->fd_addr == NULL) && (dst->fd_addr == NULL))
  {
    fprintf(stderr,
            "Implement ggi_visual_bitbltt (display to display)\n");
  }
  else if(dst->fd_addr == NULL)
  {
    char * data;
    int    x;
    int    y;

    /* Order coordinates in a good way */
    fix_rect(srccor);
    fix_rect(dstcor);

    data = src->fd_addr;

    for(y = 0; y <= (srccor->y2 - srccor->y1); y++)
    {
      for(x = 0; x <= (srccor->x2 - srccor->x1); x++)
      {
        char byte = 0;

        if((x % 256) == 0)
        {
          byte = *data++;
        }

        if(byte & 0x80)
        {
          ggi_visual_put_pixel(vwk,
                               x + dstcor->x1,
                               y + dstcor->y1,
                               fgcol);
        }

        byte <<= 1;
      }
    }
  }
  else
  {
    fprintf(stderr,
            "Implement ggi_visual_bitbltt (display to memory)\n");
  }
}
