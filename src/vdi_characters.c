/*
 * vdi_characters.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
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

static void vdi_vq_chcells(VDI_Workstation *);
static void vdi_v_exit_cur(VDI_Workstation *);
static void vdi_v_enter_cur(VDI_Workstation *);
static void vdi_v_curup(VDI_Workstation *);
static void vdi_v_curdown(VDI_Workstation *);
static void vdi_v_curright(VDI_Workstation *);
static void vdi_v_curleft(VDI_Workstation *);
static void vdi_v_curhome(VDI_Workstation *);
static void vdi_v_eeos(VDI_Workstation *);
static void vdi_v_eeol(VDI_Workstation *);
static void vdi_v_curaddress(VDI_Workstation *);
static void vdi_v_curtext(VDI_Workstation *);
static void vdi_v_rvon(VDI_Workstation *);
static void vdi_v_rvoff(VDI_Workstation *);
static void vdi_vq_curaddress(VDI_Workstation *);
static void vdi_v_hardcopy(VDI_Workstation *);


VdiFunction *vdi_charfuncs[] = {
  UNUSED, vdi_vq_chcells, vdi_v_exit_cur, vdi_v_enter_cur, vdi_v_curup,
  vdi_v_curdown, vdi_v_curright, vdi_v_curleft, vdi_v_curhome, vdi_v_eeos,
  vdi_v_eeol, vdi_v_curaddress, vdi_v_curtext, vdi_v_rvon, vdi_v_rvoff,
  vdi_vq_curaddress, UNUSED, vdi_v_hardcopy
};


void vdi_characters(VDI_Workstation *vwk)
{
  if(vdi_charfuncs[vdipb->contrl[SUBROUTINE]] && 
     (vdipb->contrl[SUBROUTINE] <= 17) && (vdipb->contrl[SUBROUTINE] >= 0))
    vdi_charfuncs[vdipb->contrl[SUBROUTINE]](vwk);
  else
    EDEBUG("vdi_characters: Subroutine %d doesn't exist!\n",vdipb->contrl[SUBROUTINE]);
}

void vdi_vq_chcells(VDI_Workstation *vwk)
{
  EDEBUG("vq_chcells: Call not implemented!\n");
}

void vdi_v_exit_cur(VDI_Workstation *vwk)
{
  EDEBUG("v_exit_cur: Call not implemented!\n");
}

void vdi_v_enter_cur(VDI_Workstation *vwk)
{
  EDEBUG("v_enter_cur: Call not implemented!\n");
}

void vdi_v_curup(VDI_Workstation *vwk)
{
  EDEBUG("v_curup: Call not implemented!\n");
}

void vdi_v_curdown(VDI_Workstation *vwk)
{
  EDEBUG("v_curdown: Call not implemented!\n");
}

void vdi_v_curright(VDI_Workstation *vwk)
{
  EDEBUG("v_curright: Call not implemented!\n");
}

void vdi_v_curleft(VDI_Workstation *vwk)
{
  EDEBUG("v_curleft: Call not implemented!\n");
}

void vdi_v_curhome(VDI_Workstation *vwk)
{
  EDEBUG("v_curhome: Call not implemented!\n");
}

void vdi_v_eeos(VDI_Workstation *vwk)
{
  EDEBUG("v_eeos: Call not implemented!\n");
}

void vdi_v_eeol(VDI_Workstation *vwk)
{
  EDEBUG("v_eeol: Call not implemented!\n");
}

void vdi_v_curaddress(VDI_Workstation *vwk)
{
  EDEBUG("v_curaddress: Call not implemented!\n");
}

void vdi_v_curtext(VDI_Workstation *vwk)
{
  EDEBUG("v_curtext: Call not implemented!\n");
}

void vdi_v_rvon(VDI_Workstation *vwk)
{
  EDEBUG("v_rvon: Call not implemented!\n");
}

void vdi_v_rvoff(VDI_Workstation *vwk)
{
  EDEBUG("v_rvoff: Call not implemented!\n");
}

void vdi_vq_curaddress(VDI_Workstation *vwk)
{
  EDEBUG("vq_curaddress: Call not implemented!\n");
}

void vdi_v_hardcopy(VDI_Workstation *vwk)
{
  EDEBUG("v_hardcopy: Call not implemented!\n");
}


