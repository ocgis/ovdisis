/*
** ggi_visual_blit.c
**
** Copyright 2000 Christer Gustavsson <cg@nocrew.org>
** Copyright 2000 Vincent Barrilliot <vincent_barrilliot@yahoo.com)
** Copyright 2000 Thomas Huth <thothy@gmx.net>
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
#include <stdlib.h>

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
  RECT        tmpcor;

  /* Fix the coordinates so that the width and the height of the
   * rectangle are positive */
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

  /* See if we can use GGI functions */
  if( src->fd_addr == NULL ) {
    /* Copy from screen */
    if( dst->fd_addr == NULL ) {
      /* to screen */
      switch( mode ) {
      case S_ONLY:
    	ggiCopyBox( VISUAL_T(vwk->visual->private),
		    srccor->x1 + dstcor->x1 - tmpcor.x1,
		    srccor->y1 + dstcor->y1 - tmpcor.y1,
		    dstcor->x2 - dstcor->x1 + 1,
		    dstcor->y2 - dstcor->y1 + 1,
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
	  for( x = 0 ;  x < w ; x++ ) {
	    /* Get the display-dependent pixel formats */
	    ggiGetPixel( *vis, 
			 srccor->x1 + x + dstcor->x1 - tmpcor.x1, 
			 srccor->y1 + y + dstcor->y1 - tmpcor.y1, 
			 &pixel_src );
	    ggiGetPixel( *vis, 
			 dstcor->x1 + x, 
			 dstcor->y1 + y, 
			 &pixel_dst );

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
    else {
      /* Blit from screen to memory */
      int x,y;
      int w,h;
      /*ggi_visual_t *vis = &VISUAL_T(vwk->visual->private);
      ggi_pixel pixel_src;
      ggi_color color_src;*/
      u_int8_t *src_ptr, *dst_ptr;

      w = min( srccor->x2 - srccor->x1 + 1 ,  dstcor->x2 - dstcor->x1 + 1);
      h = min( srccor->y2 - srccor->y1 + 1 ,  dstcor->y2 - dstcor->y1 + 1);

      /* Hmpf, not a very clean method: */
      if( vwk->inq.attr.planes!=8 && vwk->inq.attr.planes!=16
         && vwk->inq.attr.planes!=24 && vwk->inq.attr.planes!=32 )
        {
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported bit depth (display to memory)\n");
         return;
        }

      /* Get a temporary buffer: */
      src_ptr = malloc( w * h * vwk->inq.attr.planes/8 );
      if( src_ptr==NULL )
        {
         fprintf(stderr,
	    "ggi_visual_blit: Not enough temporary memory (display to memory)\n");
         return;

        }

      /* Read the graphics data in the temporary buffer: */
      ggiGetBox(VISUAL_T(vwk->visual->private), srccor->x1, srccor->y1, w, h, src_ptr);

      switch( mode ) {
       case S_ONLY:
        for( y = 0 ; y < h ; y++ ) {
         dst_ptr = ((u_int8_t *)dst->fd_addr)
                 + (y + dstcor->y1) * ((dst->fd_w+7)/8 * dst->fd_nplanes)
                 + dstcor->x1 * dst->fd_nplanes/8;
         memcpy(dst_ptr, src_ptr+(y * w * vwk->inq.attr.planes/8), w*dst->fd_nplanes/8);
        }
	break;
	
       case D_ONLY:
	break;

       default:
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported blit mode (display to memory)\n");
        break;
       }
      
      free(src_ptr);
    }
  }
  else {
    if( dst->fd_addr == NULL ) {
      /* Blit memory to screen */
      int x,y;
      int w,h;
      /*ggi_visual_t *vis = &VISUAL_T(vwk->visual->private);
      ggi_pixel pixel_src;
      ggi_color color_src;*/
      u_int8_t *src_ptr, *dst_ptr;

      w = min( srccor->x2 - srccor->x1 + 1 ,  dstcor->x2 - dstcor->x1 + 1);
      h = min( srccor->y2 - srccor->y1 + 1 ,  dstcor->y2 - dstcor->y1 + 1);

      /* Hmpf, not a very clean method: */
      if( vwk->inq.attr.planes!=8 && vwk->inq.attr.planes!=16
         && vwk->inq.attr.planes!=24 && vwk->inq.attr.planes!=32 )
        {
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported bit depth (memory to display)\n");
         return;
        }

      /* Get a temporary buffer: */
      dst_ptr = malloc( w * h * vwk->inq.attr.planes/8 );
      if( dst_ptr==NULL )
        {
         fprintf(stderr,
	    "ggi_visual_blit: Not enough temporary memory (memory to display)\n");
         return;
        }

      switch( mode ) {
       case S_ONLY:
        for( y = 0 ; y < h ; y++ ) {
         src_ptr = ((u_int8_t *)src->fd_addr)
                 + (y + srccor->y1) * ((src->fd_w+7)/8 * src->fd_nplanes)
                 + srccor->x1 * src->fd_nplanes/8;
         memcpy(dst_ptr+(y * w * vwk->inq.attr.planes/8), src_ptr, w*src->fd_nplanes/8);
        }
	break;
	
       case D_ONLY:
	break;

       default:
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported blit mode (memory to display)\n");
        break;
       }

      /* Now put it on the screen: */
      ggiPutBox(VISUAL_T(vwk->visual->private), dstcor->x1, dstcor->y1, w, h, dst_ptr);
      
      free(dst_ptr);
    }
    else {
    /* Blit memory to memory */
    fprintf(stderr,
	    "Implement ggi_visual_bitblt (memory to memory)\n");
    }
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
