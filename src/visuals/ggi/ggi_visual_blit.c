/*
** ggi_visual_blit.c
**
** Copyright 2000 Christer Gustavsson <cg@nocrew.org>
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

void
ggi_visual_bitblt (VDI_Workstation * vwk,
		   int               mode,
		   RECT *            srccor,
		   RECT *            dstcor,
		   MFDB *            src,
		   MFDB *            dst)
{
  if((src->fd_addr == NULL) && (dst->fd_addr == NULL))
  {
    fix_rect(srccor);
    fix_rect(dstcor);

    ggiCopyBox(VISUAL_T(vwk->visual->private),
               srccor->x1,
               srccor->y1,
               srccor->x2 - srccor->x1 + 1,
               srccor->y2 - srccor->y1 + 1,
               dstcor->x1,
               dstcor->y1);
  }
  else
  {
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
