/*
 * vdi_call_mint.c
 *
 * Copyright 1999 Christer Gustavsson <cg@nocrew.org>
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

#include "vdibind.h"

#undef DEBUGLEVEL

VDIPB *vdipb;

void (*vdi_handler)(VDIPB * vdipb) = NULL;

void vdi_call(VDIPB *vdiparblk)
{
  if (vdi_handler != NULL) {
    vdi_handler (vdiparblk);
  } else {
    /* Call vdi */
    __asm__ volatile("movl %0,    d1\n"
		     "movq #0x73, d0\n"
		     "trap #2\n"
		     :                                    /* No outputs */
		     : "g" (vdiparblk)                    /* Inputs */
		     : "d0", "d1", "d2", "a0", "a1", "a2" /* Clobbered regs */
		     );
  }
}


