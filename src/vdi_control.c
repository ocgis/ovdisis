/*
 * vdi_control.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
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

#define DEBUGLEVEL 2

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include "event.h"
#include "inits.h"
#include "ovdisis.h"
#include "various.h"
#include "vdi_control.h"
#include "workstation_memory.h"

void vdi_v_clrwk(VDI_Workstation *);	/* Used by v_opnwk */

/*
** Description
** Find a visual to use and initialize it
**
** 1999-08-29 CG
*/
static VDI_Visual *init_visual (void) {
  void *       handle;
  char         module_name[256];
  VDI_Visual * (*visual_init)(void);
  char *       error;
  char *       visualtype;

  strcpy(module_name, VISUALSDIR);
  strcat(module_name, "/libovdisis_");

  /* Read visual type from environment variable, 
   * if available, otherwise use GGI, and let it figure out what the driver should be
   */
  visualtype = getenv("OVDISIS_VISUAL");
  if(visualtype)
    strcat(module_name, visualtype);
  else
    strcat(module_name, "ggi");

  strcat(module_name, ".so");
  handle = dlopen(module_name, RTLD_NOW);

  if(handle == NULL)
  {
    fprintf(stderr, "Couldn't open visual %s\n", module_name);
    exit(1);
  }

  visual_init = dlsym(handle, "init");

  error = dlerror();
  if(error != NULL)
  {
    fprintf(stderr, "%s\n", error);
    dlclose(handle);
    exit(1);
  }

  return visual_init();
}


/*
** Description
** Implementation of the vdi function v_opnwk
** NOTE: the parameter has no significance
**
** 1998-10-13 CG
** 1998-12-07 CG
** 1999-08-29 CG
** 2003-09-26 VB Changed the memory management
*/
void vdi_v_opnwk(VDI_Workstation *vwk) {
  VDIWK  *wk;

  wk = get_physical_workstation_memory();

  if( wk == NULL ) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open workstation */
    DEBUG1("v_opnwk: Can't get memory for physical workstation, sorry!\n");
    return;
  }
 
  /* We are now ready to open the display */
  wk->physical->visual = init_visual();

  /* This holds the visual's private data */
  wk->physical->visual->private = VISUAL_OPEN(wk->physical);
  
  /* Let's see if the visual could be opened with no problem */
  if (wk->physical->visual->private == NULL) {
    free_physical_workstation_memory(wk);
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open workstation */
    DEBUG1("v_opnwk: Error opening visual!\n");
    return;
  }

  vdipb->contrl[VDI_HANDLE] = wk->physical->handle;
  wk->physical->pid         = getpid();	/* pid of current process */

  DEBUG2("v_opnwk: Physical workstation, handle %d opened: %p\n", wk->physical->handle, wk->physical);


  /* Setting default values */

  /* v_opnvwk and vq_extnd values */
  init_workstation(wk->physical);

  wk->physical->write_mode = MD_REPLACE;
  wk->physical->clip.x1 = 0;
  wk->physical->clip.y1 = 0;
  wk->physical->clip.x2 = wk->physical->dev.attr.xres;
  wk->physical->clip.y2 = wk->physical->dev.attr.yres;

  DEBUG2("v_opnwk: Resolution         : %d,%d\n", wk->physical->dev.attr.xres,wk->physical->dev.attr.yres);
  DEBUG2("v_opnwk: Number of bitplanes: %d\n", wk->physical->inq.attr.planes);

  vdi_v_clrwk(wk->physical);	/* Clear screen */

  /* Set default things */
  init_cmap(wk->physical);
  init_marker(wk->physical);
  init_line(wk->physical);
  init_fill(wk->physical);
  init_device(wk->physical);
  init_text(wk->physical);

  ADEBUG("v_opnwk: Default palette set\n");

  /* Reset event vectors */
  wk->physical->butv = NULL;
  wk->physical->curv = NULL;
  wk->physical->motv = NULL;
  wk->physical->timv = NULL;

  /* Start event handler */
  start_event_handler (wk->physical);

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}


/*
** Description
** Implementation of v_clswk
*/
void vdi_v_clswk (VDI_Workstation *vwk) {
  int wk_id;
  VDIWK *wk;

  wk_id = vdipb->contrl[VDI_HANDLE];

  switch(get_workstation_type(wk_id)) {
  case WS_PHYSICAL:
    break;
  case WS_NOTOPEN:
    DEBUG2("v_clswk: Physical workstation, handle %d not open!\n", wk_id);
    break;
  default:
    DEBUG2("v_clswk: Handle %d is not a physical workstation!\n", wk_id);
  }

  /* Destroy event handler */
  stop_event_handler();
  wk = handle_to_VDIWK(wk_id);

  VISUAL_FREE_CMAP(wk->physical);
  VISUAL_CLOSE(wk->physical);

  DEBUG1("v_clswk: Display closed\n");

  /* Free virtual workstations left open */
  /* Will closing a physical workstation also close all its virtual ? */
  /* It will here! */
  free_physical_workstation_memory(wk);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/* Implementation of v_opnvwk */
void vdi_v_opnvwk(VDI_Workstation *vwk) {
  int i;
  VDIWK *wk;

  DEBUG3 ("ovdisis: vdi_v_opnvwk: entering\n");
  
  wk = get_virtual_workstation_memory(vdipb->contrl[VDI_HANDLE]); /* parameter: physical workstation's handle */
  
  if( wk == NULL ) {
    vdipb->contrl[VDI_HANDLE] = 0;	/* Could not open virtual workstation */
    DEBUG2("v_opnvwk: Handle %d is not a physical workstation!\n", vdipb->contrl[VDI_HANDLE]);
    return;
  }
  
  DEBUG1("v_opnvwk: New virtual workstation, handle %d opened linked to %d\n", wk->vwk->handle,wk->physical->handle);
  vdipb->contrl[VDI_HANDLE] = wk->vwk->handle;
  wk->vwk->pid = getpid();	/* pid of current process */
  wk->vwk->visual = wk->physical->visual;
  
  /* Copy some things */
  copy_workstation(wk->physical, wk->vwk);
  copy_cmap(wk->physical, wk->vwk);
  copy_marker(wk->physical, wk->vwk);
  copy_line(wk->physical, wk->vwk);
  copy_fill(wk->physical, wk->vwk);
  copy_device(wk->physical, wk->vwk);
  copy_text(wk->physical, wk->vwk);
  
  /* make the changes the user wants */
  /* intin[0] - workstation (1-10 == screen) */
  wk->vwk->line_a.type = (int)vdipb->intin[1];
  wk->vwk->line_a.color = (int)vdipb->intin[2];
  wk->vwk->marker_a.type = (int)vdipb->intin[3];
  wk->vwk->marker_a.color = (int)vdipb->intin[4];
  wk->vwk->text_a.fontid = (int)vdipb->intin[5];
  wk->vwk->text_a.color = (int)vdipb->intin[6];
  wk->vwk->fill_a.interior = (int)vdipb->intin[7];
  wk->vwk->fill_a.style = (int)vdipb->intin[8];
  wk->vwk->fill_a.color = (int)vdipb->intin[9];
  /* intin[10] - Coordinate system.  Only used in GDOS */

  wk->vwk->write_mode = wk->physical->write_mode;
  wk->vwk->clip.x1 = wk->physical->clip.x1;
  wk->vwk->clip.y1 = wk->physical->clip.y1;
  wk->vwk->clip.x2 = wk->physical->clip.x2;
  wk->vwk->clip.y2 = wk->physical->clip.y2;

  /* v_opnvwk values to return */
  for (i = 0; i < 45; i++)
    vdipb->intout[i] = wk->vwk->dev.tab[i] = wk->physical->dev.tab[i];
  for (i = 0; i < 12; i++)
    vdipb->ptsout[i] = wk->vwk->dev.tab[i + 45] = wk->physical->dev.tab[i + 45];

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}

void vdi_v_clsvwk(VDI_Workstation *vwk)
{
  free_virtual_workstation_memory( handle_to_VDIWK(vdipb->contrl[VDI_HANDLE]) );

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_clrwk(VDI_Workstation *vwk)
{
  VISUAL_CLEAR(vwk);
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_v_updwk(VDI_Workstation *vwk)
{
  DEBUG1("v_updwk: Call not implemented!\n");
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vs_clip(VDI_Workstation *vwk)
{
  if (vdipb->intin[0]) {	/* Clipping on */
    vwk->clip.x1 = vdipb->ptsin[0];
    vwk->clip.y1 = vdipb->ptsin[1];
    vwk->clip.x2 = vdipb->ptsin[2];
    vwk->clip.y2 = vdipb->ptsin[3];
    fix_rect(&vwk->clip);
    vwk->clip.x1 = max(0, vdipb->ptsin[0]);
    vwk->clip.y1 = max(0, vdipb->ptsin[1]);
    vwk->clip.x2 = min(vwk->dev.attr.xres, vdipb->ptsin[2]);
    vwk->clip.y2 = min(vwk->dev.attr.yres, vdipb->ptsin[3]);
  } else {			/* Clipping off */
    vwk->clip.x1 = 0;
    vwk->clip.y1 = 0;
    vwk->clip.x2 = vwk->dev.attr.xres;
    vwk->clip.y2 = vwk->dev.attr.yres;
  }
  DEBUG2("vs_clip: Clipping for handle %d set to: %d, %d, %d, %d.\n",
    vwk->handle, vwk->clip.x1, vwk->clip.y1, vwk->clip.x2, vwk->clip.y2);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}


/*
** Exported
*/
void
vdi_vswr_mode (VDI_Workstation * vwk)
{
  vwk->write_mode = vdipb->intin[0];
  vdipb->intout[0] = vwk->write_mode;

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}


void vdi_vq_extnd(VDI_Workstation *vwk)
{
  int i;

  if (vdipb->intin[0]) {	/* extended answers */
    for (i = 0; i < 45; i++)
      vdipb->intout[i] = vwk->inq.tab[i];
    for (i = 0; i < 12; i++)
      vdipb->ptsout[i] = vwk->inq.tab[i + 45];
  } else {			/* same answers as from v_opnvwk */
    for (i = 0; i < 45; i++)
      vdipb->intout[i] = vwk->dev.tab[i];
    for (i = 0; i < 12; i++)
      vdipb->ptsout[i] = vwk->dev.tab[i + 45];
  }

  vdipb->contrl[N_PTSOUT] = 6;
  vdipb->contrl[N_INTOUT] = 45;
}
