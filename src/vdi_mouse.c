/*
 * vdi_mouse.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 2000 Christer Gustavsson <cg@nocrew.org>
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

#include "event.h"
#include "ovdisis.h"
#include "vdi_mouse.h"

/*
** Exported
*/
void
vdi_vsc_form (VDI_Workstation * vwk)
{
  set_mouse_form((MFORM *)&vdipb->intin[0]);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/*
** Description
** Implementetation of v_show_c()
**
** 1999-01-03 CG
** 1999-05-22 CG
*/
void
vdi_v_show_c (VDI_Workstation *vwk) {
  increase_mouse_visibility ();

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/*
** Description
** Implementetation of v_hide_c()
**
** 1999-01-03 CG
** 1999-05-22 CG
*/
void vdi_v_hide_c(VDI_Workstation *vwk)
{
  decrease_mouse_visibility ();

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vq_mouse(VDI_Workstation *vwk)
{
  EDEBUG("vq_mouse: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 1;
  vdipb->contrl[N_INTOUT] = 1;
}


/*
** Description
** Implementation of vex_butv. Just fill in the handler address in the vdi
** workstation and let the handler thread do the calls.
**
** 1998-12-06 CG
*/
void
vdi_vex_butv (VDI_Workstation *vwk)
{
  /* Return the old vector */
  vdipb->contrl[9] = MSW(vwk->butv);
  vdipb->contrl[10] = LSW(vwk->butv);
  
  /* Setup the new vector */
  vwk->butv = (void *)
    (((unsigned short)vdipb->contrl[7] << 16) +
    (unsigned short)vdipb->contrl[8]);

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

/*
** Description
** Implementation of vex_motv. Just fill in the handler address in the vdi
** workstation and let the handler thread do the calls.
**
** 1998-12-13 CG
*/
void vdi_vex_motv(VDI_Workstation *vwk)
{
  /* Return the old vector */
  vdipb->contrl[9] = MSW(vwk->butv);
  vdipb->contrl[10] = LSW(vwk->butv);
  
  /* Setup the new vector */
  vwk->motv = (void *)
    (((unsigned short)vdipb->contrl[7] << 16) +
    (unsigned short)vdipb->contrl[8]);

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vex_curv(VDI_Workstation *vwk)
{
  EDEBUG("vex_curv: Call not implemented!\n");

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


