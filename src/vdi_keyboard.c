/*
 * vdi_keyboard.c
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

#include "ovdisis.h"
#include "vdi_keyboard.h"

/*
** Description
** Implementation of vex_keyv. Just fill in the handler address in the vdi
** workstation and let the handler thread do the calls.
**
** 1999-03-07 CG
*/
void
vdi_vex_keyv (VDI_Workstation *vwk)
{
  /* Return the old vector */
  vdipb->contrl[9] = MSW(vwk->keyv);
  vdipb->contrl[10] = LSW(vwk->keyv);
  
  /* Setup the new vector */
  vwk->keyv = (void *)
    (((unsigned short)vdipb->contrl[7] << 16) +
    (unsigned short)vdipb->contrl[8]);

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

