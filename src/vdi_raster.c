/*
 * vdi_raster.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 2000 Thomas Huth <thothy@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#define DEBUGLEVEL 0

#include <stdio.h>

#include "ovdisis.h"
#include "various.h"
#include "vdi_raster.h"
#include "various.h"

#include <stdlib.h>
#include <sys/types.h>

#define USHORTS_TO_POINTER(m,l) ((void *)(((unsigned long)(m)<<16) | ((unsigned long)(l&0xffff))))



static void init_mfdb( VDI_Workstation *vwk, MFDB *mfdb) 
{
    mfdb->fd_w   = vwk->dev.attr.xres;
    mfdb->fd_h  = vwk->dev.attr.yres;
    mfdb->fd_wdwidth = (vwk->dev.attr.xres + 15) % 16;
    mfdb->fd_stand   = 0;
    mfdb->fd_nplanes  = vwk->inq.attr.planes;
}



void vdi_vro_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  RECT srccor, dstcor;
  RECT screen;

  IDEBUG("vdi_vro_cpyfm entered\n");

  /* Check out the areas to copy */
  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  /* Get pointers of the MFDBs */
  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

#if 0  /* Vincents stuff - does not work yet: */
  /* Some inits and checks */
  if( src->fd_addr == NULL ) {
    /* Copying from the screen */
    init_mfdb( vwk, src );        /* This is specified in the Compendium */
    do_rectclip( &src, &screen ); /* Don't copy the outside the screen   */
  } else {
    fix_rect( src );
  }

  if( dst->fd_addr == NULL ) {
    init_mfdb( vwk, dst );
    do_rectclip( &dst, &screen); /* do_rectcplip fix_rects the clip rect */
  } else {
    fix_rect( dst );
  }
#endif

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  VISUAL_BITBLT(vwk, vdipb->intin[0], &srccor, &dstcor, src, dst);

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
  IDEBUG("vdi_vro_cpyfm exited\n");
}

void vdi_vrt_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  RECT srccor, dstcor;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  /* order the coordinates as I like them. */
  fix_rect(&srccor);
  fix_rect(&dstcor);

  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  VISUAL_BITBLTT(vwk,
                 vdipb->intin[0],
                 vdipb->intin[1],
                 vdipb->intin[2],
                 &srccor,
                 &dstcor,
                 src,
                 dst);

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vr_trnfm(VDI_Workstation *vwk)
{
  /* Convert device independant bitmaps into device dependants and
   * vice versa */
  /* NOTE: it's slow, sorry ! */

  MFDB *src, nsrc, *dst;
  int  w, h;
  int  x, y;

  long linesize; /* size of a line, in bytes */
  long whole_plane_size;
  int plane;
  u_int32_t pixelcol;
  u_int8_t  *line;
  u_int8_t *dst_ptr;

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;

  /* Get the pointers to the MFDBs */
  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  if( src->fd_stand == dst->fd_stand )
   {
    fprintf( stderr,"vr_trnfm: Want to convert to same mode !\n" );
    return;
   }

  if( src->fd_nplanes > dst->fd_nplanes )
   {
    fprintf( stderr,"vr_trnfm: Warning: Source has more bit planes than destination!\n");
   }

  if( dst->fd_nplanes != vwk->inq.attr.planes )
   {
    fprintf( stderr,"vr_trnfm: Warning: Destination color depth (%i) is not the actual one (%i)!\n",
        dst->fd_nplanes, vwk->inq.attr.planes);
   }

  /* I really can not imagine that vr_trnfm should also work with the screen directly */
  /* because it does _not_ take coordinates as parameters! */
  if( src->fd_addr == NULL || dst->fd_addr == NULL )
   {
    fprintf( stderr,"vr_trnfm: Transformation to/from screen not supported!\n");
    return;
   }

  /* If the MFDBs are the same, we use a temporary third MFDB */
  if( src->fd_addr == dst->fd_addr )
    {
     size_t l = ((size_t)(src->fd_w+7)) * src->fd_h * src->fd_nplanes / 8;
     nsrc.fd_addr=malloc( l );
     if( nsrc.fd_addr==NULL )
      {
       fprintf( stderr,"vr_trnfm: Not enough memory!\n");
       return;
      }
     nsrc.fd_w=src->fd_w;  nsrc.fd_h=src->fd_h;
     nsrc.fd_wdwidth=src->fd_wdwidth;
     nsrc.fd_stand=src->fd_stand;
     nsrc.fd_nplanes=src->fd_nplanes;
     nsrc.fd_r1=src->fd_r1;  nsrc.fd_r2=src->fd_r2;  nsrc.fd_r3=src->fd_r3;
     memcpy(nsrc.fd_addr, src->fd_addr, l);
    }
   else
    {
     nsrc=*src;
    }

  w = min( nsrc.fd_w, dst->fd_w );
  h = min( nsrc.fd_h, dst->fd_h );

  /* Let´s convert it: */
  if( nsrc.fd_stand && !dst->fd_stand )
   {
    /* Convert from independent to dependent */

   dst_ptr=dst->fd_addr;
   line = (u_int8_t *)nsrc.fd_addr;
   linesize = (nsrc.fd_w+7)/8;           /* With 1 byte we can code 8 pixel */
   whole_plane_size = (nsrc.fd_w+7)/8 * nsrc.fd_h;

   for( y = 0 ;  y < h  ; y++, line += linesize )
    {
     dst_ptr = ((u_int8_t *)dst->fd_addr) + y * ((dst->fd_w+7)/8*dst->fd_nplanes);
     for( x = 0  ;  x < w  ;  x++ )
      {
       pixelcol = 0;
       for( plane = 0 ; plane < nsrc.fd_nplanes ; plane++ )       /* Get the color value of the source pixel */
        {
         if( *( line+(plane*whole_plane_size)+x/8 ) & (128>>(x%8)) )
           pixelcol |= (1<<plane);
        }
       
       switch(dst->fd_nplanes)          /* Set the destination pixel - not very clean, since it */
        {                               /*  accesses the dest. MFDB directly, but I had no better idea - T.Huth*/
         case 1:
           if( x%8 == 0 )
             *dst_ptr++ = *( line + x/8 );  /* Copy directly, also not so nice, but I hope it works - T.Huth */
           break;
         case 2:
           fprintf( stderr,"vr_trnfm: Sorry, this color depth (2 bit) is not yet supported!\n");
           return;
           /*break;*/
         case 4:
           fprintf( stderr,"vr_trnfm: Sorry, this color depth (4 bit) is not yet supported!\n");
           return;
           /*break;*/
         case 8:
           *dst_ptr++ = pixelcol;
           break;
         case 16:
           *(u_int16_t *)dst_ptr = (u_int16_t)pixelcol;
           dst_ptr+=2;
           break;
         case 24:
           *dst_ptr++ = (u_int8_t)pixelcol;
           *dst_ptr++ = (u_int8_t)(pixelcol>>8);
           *dst_ptr++ = (u_int8_t)(pixelcol>>16);
           break;
         case 32:
           *(u_int32_t *)dst_ptr = pixelcol;
           dst_ptr+=4;
           break;
         default:
           fprintf( stderr,"vr_trnfm: Unsupported color depth!\n");
           return;
        }
      }
    }

  }
 else
  {
    /* Convert from independent to dependent */
   fprintf( stderr,"vr_trnfm: Conversion to standard format not yet supported!\n");
  }

 if( src->fd_addr == dst->fd_addr )
   free(nsrc.fd_addr);

}

