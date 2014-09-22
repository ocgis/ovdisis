/*
** sdl_visual_blit.c
**
** Copyright 2001 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#define DEBUGLEVEL 3

#include "sdl_visual.h"
#include "sdl_visual_blit.h"
#include "ovdisis.h"
#include "various.h"


void
sdl_visual_bitblt(VDI_Workstation * vwk,
                  int               mode,
                  RECT *            srccor,
                  RECT *            dstcor,
                  MFDB *            src,
                  MFDB *            dst)
{
  RECT tmpcor;

  DEBUG3("sdl_visual_bitblt entered\n");

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
        DEBUG1("Blit 1\n");
        /*
    	ggiCopyBox( VISUAL_T(vwk->visual->private),
		    srccor->x1 + dstcor->x1 - tmpcor.x1,
		    srccor->y1 + dstcor->y1 - tmpcor.y1,
		    dstcor->x2 - dstcor->x1 + 1,
		    dstcor->y2 - dstcor->y1 + 1,
		    dstcor->x1,
		    dstcor->y1);
        */
	return;
	
      case D_ONLY:
	/* I am not sure that I understand the purpose of this */
        DEBUG1("Blit 2\n");
        /*
    	ggiCopyBox( VISUAL_T(vwk->visual->private),
		    dstcor->x1,
		    dstcor->y1,
		    dstcor->x2 - dstcor->x1 + 1,
		    dstcor->y2 - dstcor->y1 + 1,
		    dstcor->x1,
		    dstcor->y1);
        */
	return;

      default:
      {
	/* We assume that oVDIsis has already checked that the mode
	 * is between 0 and 15
	 * gargl, I bet this code is sloooooooow */
	int           x,y;
	int           w,h;
        SDL_Surface * screen;

        screen = VISUAL_T(vwk->visual->private);

        if(SDL_MUSTLOCK(screen))
        {
          if(SDL_LockSurface(screen) < 0)
          {
            return;
          }
        }

	w = min( srccor->x2 - srccor->x1  ,  dstcor->x2 - dstcor->x1 );
	h = min( srccor->y2 - srccor->y1  ,  dstcor->y2 - dstcor->y1 );

	for(y = 0; y < h; y++)
        {
	  for(x = 0;  x < w; x++)
          {
            Uint16 * srcp;
            Uint16 * dstp;

            /* FIXME: support other bit depths than 16 */
            srcp = (Uint16 *)screen->pixels +
              (srccor->y1 + y + dstcor->y1 - tmpcor.y1) * screen->pitch / 2 +
              srccor->x1 + x + dstcor->x1 - tmpcor.x1;
            dstp = (Uint16 *)screen->pixels +
              (dstcor->y1 + y) * screen->pitch/2 + dstcor->x1 + x;

            *dstp = *srcp;

	    /* Process */
	    /* FIXME: replacement_mode[mode]( color_src, &color_dst ); */
	  }
	}

        if(SDL_MUSTLOCK(screen))
        {
          SDL_UnlockSurface(screen);
        }

        SDL_UpdateRect(screen, dstcor->x1, dstcor->y1, w, h);
	return;
	}
      }
    }
    else {
      /* Blit from screen to memory */
      int y;
      int w,h;
      /*ggi_visual_t *vis = &VISUAL_T(vwk->visual->private);
      ggi_pixel pixel_src;
      ggi_color color_src;*/
      u_int8_t *dst_ptr;

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

      switch( mode ) {
       case S_ONLY:
        DEBUG1("Blit 4\n");
        /*
        for( y = 0 ; y < h ; y++ ) {
         dst_ptr = ((u_int8_t *)dst->fd_addr)
                 + (dst->fd_w * dst->fd_nplanes + 7)/8 * (y + dstcor->y1)
                 + dstcor->x1 * dst->fd_nplanes/8;
         ggiGetBox(VISUAL_T(vwk->visual->private), srccor->x1, srccor->y1 + y, w, 1, dst_ptr);
        }
        */
	break;
	
       case D_ONLY:
	break;

       default:
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported blit mode (display to memory)\n");
        break;
       }
    }
  }
  else {
    if( dst->fd_addr == NULL ) {
      /* Blit memory to screen */
      int y;
      int w,h;
      /*ggi_visual_t *vis = &VISUAL_T(vwk->visual->private);
      ggi_pixel pixel_src;
      ggi_color color_src;*/
      u_int8_t *src_ptr;

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

      switch( mode ) {
       case S_ONLY:
        DEBUG1("Blit 5\n");
        /*
        for( y = 0 ; y < h ; y++ ) {
         src_ptr = ((u_int8_t *)src->fd_addr)
                 + (src->fd_w * src->fd_nplanes + 7)/8 * (y + srccor->y1)
                 + srccor->x1 * src->fd_nplanes/8;
         ggiPutBox(VISUAL_T(vwk->visual->private), dstcor->x1, dstcor->y1 + y, w, 1, src_ptr);
        }
        */
	break;
	
       case D_ONLY:
	break;

       default:
         fprintf(stderr,
	    "ggi_visual_blit: Unsupported blit mode (memory to display)\n");
        break;
       }

    }
    else {
    /* Blit memory to memory */
     int y;
     int w,h;
     u_int8_t *src_ptr, *dst_ptr;

     w = min( srccor->x2 - srccor->x1 + 1 ,  dstcor->x2 - dstcor->x1 + 1);
     h = min( srccor->y2 - srccor->y1 + 1 ,  dstcor->y2 - dstcor->y1 + 1);

     if( dst->fd_nplanes!=8 && dst->fd_nplanes!=16 && dst->fd_nplanes!=24 && dst->fd_nplanes!=32 )
       {
        fprintf(stderr, "ggi_visual_blit: Unsupported bit depth (memory to memory)\n");
        return;
       }

      switch( mode ) {
       case S_ONLY:
        for( y = 0 ; y < h ; y++ ) {
         src_ptr = ((u_int8_t *)src->fd_addr)
                 + (src->fd_w * src->fd_nplanes + 7)/8 * (y + srccor->y1)
                 + srccor->x1 * src->fd_nplanes/8;
         dst_ptr = ((u_int8_t *)dst->fd_addr)
                 + (dst->fd_w * dst->fd_nplanes + 7)/8 * (y + dstcor->y1)
                 + dstcor->x1 * dst->fd_nplanes/8;
         memcpy(dst_ptr, src_ptr, w*dst->fd_nplanes/8);  /* Copy it */
        }
	break;
	
       case D_ONLY:
	break;

       default:
        fprintf(stderr, "ggi_visual_blit: Unsupported blit mode (memory to memory)\n");
        break;
       }

    }
  }
  DEBUG3("sdl_visual_bitblt exited\n");
}


void
sdl_visual_bitbltt (VDI_Workstation * vwk,
		    int               mode,
		    int               fgcol,
		    int               bgcol,
		    RECT *            srccor,
		    RECT *            dstcor,
		    MFDB *            src,
		    MFDB *            dst)
{
  DEBUG3("sdl_visual_bitbltt entered\n");
  DEBUG3("sdl_visual_bitbltt exited\n");
}
