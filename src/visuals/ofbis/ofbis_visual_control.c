/*
** ofbis_visual_control.c
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

#include <ofbis.h>
#include <stdlib.h>

#include "ovdisis.h"

void *
ofbis_visual_open (void) {
  return (void *)FBopen(NULL, FB_OPEN_NEW_VC /* | FB_NO_KBD */ );
}

void
ofbis_visual_close (void * fb) {
  FBclose((FB *)fb);
}

void
ofbis_visual_clear (VDI_Workstation * vwk)
{
  unsigned long fill, *mem, len, *end;

  if (vwk->inq.attr.planes < 16)
    fill = 0L;
  else				/* TrueColor mode */
    fill = 0xffdfffdfL;		/* TOS way, I'd rather have 0xffffffffL */

  mem = (unsigned long *)((FB *)vwk->visual->private)->sbuf;

  len = ((unsigned long)(vwk->dev.attr.xres + 1) *
	 (unsigned long)(vwk->dev.attr.yres + 1) *
	 (unsigned long)vwk->inq.attr.planes) / 8;

  end = (unsigned long *)((unsigned long)((FB *)vwk->visual->private)->sbuf + len);

  ADEBUG("v_clrwk: Filling screen %p to %p (%ld) with %08lx\n", mem, end, len, fill);

  for (; mem < end; mem++)
    *mem = fill;
}


void
ofbis_visual_set_write_mode (void * fb,
			     int    write_mode) {
  switch (write_mode) {
  case MD_REPLACE:
    FBsetwritemode ((FB *)fb, FB_REPLACE);
    break;

  case MD_TRANS:
    FBsetwritemode ((FB *)fb, FB_TRANS);
    break;

  case MD_XOR:
    FBsetwritemode ((FB *)fb, FB_XOR);
    break;

  case MD_ERASE:
    FBsetwritemode ((FB *)fb, FB_ERASE);
    break;

  default:
    ;
  }
}

