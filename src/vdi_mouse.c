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
#include "mouse.h"

/*
** Exported
*/
void vdi_vsc_form (VDI_Workstation * vwk) {
  set_mouse_form( vwk, (MFORM *)&vdipb->intin[0]);

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
void vdi_v_show_c (VDI_Workstation *vwk) {
  increase_mouse_visibility( );

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
  decrease_mouse_visibility ( );

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/*
** Description
** Implementetation of vq_mouse()
*/
void vdi_vq_mouse(VDI_Workstation *vwk)
{
  unsigned int b;
  get_mouse_position( &(vdipb->ptsout[0]), &(vdipb->ptsout[1]) );
  get_mouse_buttons( &b );

  vdipb->intout[0] = b;

  vdipb->contrl[N_PTSOUT] = 2;
  vdipb->contrl[N_INTOUT] = 1;
}


/*
** Description
** Implementation of vex_butv. Just fill in the handler address in the vdi
** workstation and let the handler thread do the calls.
**
** 1998-12-06 CG
*/
void vdi_vex_butv (VDI_Workstation *vwk)
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
  vdipb->contrl[9] = MSW(vwk->motv);
  vdipb->contrl[10] = LSW(vwk->motv);
  
  /* Setup the new vector */
  vwk->motv = (void *)
    (((unsigned short)vdipb->contrl[7] << 16) +
    (unsigned short)vdipb->contrl[8]);

  /* Return number of returned values */
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/*
** Description
** Implementation of vex_curv. Just fill in the handler address in the vdi
** workstation and let the handler thread do the calls. The mouse position
** is not in D0 and D1 of the 68k of course, so you should get the mouse
** cursor's position using vq_mouse.
** Setting the vector to NULL will restore the original handler.
**
** 2003-11-22 VB
*/

void vdi_vex_curv(VDI_Workstation *vwk)
{
  vdipb->contrl[9] = MSW(vwk->curv);
  vdipb->contrl[10] = LSW(vwk->curv);

  set_mouse_drawer((void(*)())((unsigned short)vdipb->contrl[7] << 16) + (unsigned short)vdipb->contrl[8]);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}
