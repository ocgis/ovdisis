/*
 * workstation_memory.h
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 1999 Christer Gustavsson <cg@nocrew.org>
 * Copyright 2003 Vincent Barrilliot <vincent_barrilliot@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#undef  DEBUGLEVEL

#include <stdio.h>
#include <sys/shm.h>
#include "ovdisis.h"
#include "workstation_memory.h"

/* Since the VDI may (and sure will) be used by several processes at a 
 * time and that memory maybe be protected (always on Linux, may be on
 * FreeMint), an application that opens a physical workstation will
 * own it, preventing other processes to access that workstation.
 * To avoid this, we say that a workstation's memory is a segment
 * of shared memory. This way, all instances of oVDIsis can use
 * workstations created by different processes.
 * 
 * Shared memory segment representing oVDIsis's workstations are
 * all made using the WSID macro defined in workstation_memory.h,
 * so oVDIsis know where to find them.
 */

/* Used to indentify your shared memory areas */
#define WSID(id) (0x42434400 | id)

/* FIXME: access to these should be mutexed */
static VDIWK wk[MAX_HANDLES];
static int wk_open[MAX_HANDLES]; /* This keep the state of each slice of the wk array */
static int first=1;              /* Used to clear the above array first time */


/*
** Description
** Get memory for a new workstation
**
** 1998-10-09 CG
** 2003-09-26 VB
*/
VDIWK *get_physical_workstation_memory() {
  int               wk_id;
  VDI_Workstation  *new_ws = NULL;

  /* Initialise array if needed */
  if(first) {
    for(wk_id=0 ; wk_id<MAX_HANDLES ; wk_id++)
      wk_open[wk_id] = WS_NOTOPEN;
    first = 0;
  }

  /* Find first free handle */
  for( wk_id=0 ;  wk_id < MAX_HANDLES ; wk_id++ )
    if( (new_ws = wsalloc(wk_id)) != NULL  )
      break;

  if( new_ws == NULL )
    return NULL;

  wk_open[wk_id] = WS_PHYSICAL;
  wk[wk_id].physical = new_ws;
  wk[wk_id].physical->handle = wk_id+1;
  wk[wk_id].vwk = wk[wk_id].physical;	/* Link workstation to physical */

  DEBUG3("get_physical_workstation_memory: Allocated physical workstation, handle: %d\n",wk[wk_id].physical->handle);
  return &wk[wk_id];  
}


/*
** Description
** Frees memory previously allocated with get_physical_workstation_memory
**
** 2003-09-26 VB
*/
void free_physical_workstation_memory(VDIWK *wk) {
  int i;
  int wk_id;
  /* Free workstations left open */
  /* Should closing a physical workstation also close all its virtual ? */
  /* It will here! */
  for (i = 0; i < MAX_HANDLES; i++)
    if (wk_open[i] && (wk[i].physical == wk->physical)) {
      wk_id = wk[i].vwk->handle;
      wsfree (i);
      wk_open[i] = WS_NOTOPEN;
      DEBUG3("free_physical_workstation_memory: Freed workstation, handle %d\n", wk_id);
    }
}


/*
** Description
** Frees memory previously allocated with get_virtual_workstation_memory
**
** 2003-09-26 VB
*/
void free_virtual_workstation_memory(VDIWK *wk) {
  int wk_id = wk->vwk->handle-1;

  switch( wk_open[wk_id] ) {
  case WS_NOTOPEN:
    DEBUG1("free_virtual_workstation_memory: Handle %d is not open\n", wk->vwk->handle);
    break;
  case WS_VIRTUAL:
    break;
  default:
    DEBUG1("free_virtual_workstation_memory: Handle %d is not a virtual workstation\n", wk->vwk->handle);
    return;
  }

  /* If we're not in the same process as where the physical
   * workstation was opened, we need to close the framebuffer
   * we opened in v_opnvwk.
   */
  if(wk->vwk->pid != wk->physical->pid)
    VISUAL_FREE_CMAP(wk->vwk);

  wsfree (wk_id);
  wk_open[wk_id] = WS_NOTOPEN;
}

/*
** Description
** Returns the type of memory allocated. Possible values are 
** WS_PHYSICAL, WS_VIRTUAL, WS_NOTOPEN
**
** 2003-09-26 VB
*/
int get_workstation_type(int handle) {
  return wk_open[handle-1];
}


/*
** Description
** Returns the VDIWK corresponding to the passed handle
**
** 2003-09-26 VB
*/
VDIWK *handle_to_VDIWK(int handle) {
  return &wk[handle-1];
}


/*
** Description
** Get memory for a new virtual workstation
**
** 1998-10-09 CG
** 2003-09-26 VB
*/
VDIWK *get_virtual_workstation_memory(int physical_wk_handle) {
  int wk_id;
  VDI_Workstation * physical;
  VDI_Workstation * virtual = NULL;

  DEBUG3 ("get_physical_workstation_memory: entering\n");

  physical = wsattach(physical_wk_handle-1);

  if (physical == NULL)
    return NULL;

  /* find first free handle */
  for( wk_id=0 ; wk_id < MAX_HANDLES ; wk_id++)
    if( (virtual = wsalloc(wk_id)) != NULL  )
      break;
    
  /* none free found */
  if (wk_id == MAX_HANDLES) {
    wsdetach (physical);
    DEBUG2("get_virtual_workstation_memory: We're all out of handles, I'm afraid!\n");
    return NULL;
  }

  wk[wk_id].physical = physical;  /* link virtual to physical, sort of */
  wk[wk_id].vwk      = virtual;
  wk_open[wk_id]     = WS_VIRTUAL;
  wk[wk_id].vwk->handle = wk_id+1;

  return &wk[wk_id];
}


/*
** Description
** Try to allocate a new workstation descriptor with a specified id.
** If successfull the address will be returned. Otherwise NULL will be
** returned.
**
** 1998-10-09 CG
*/
VDI_Workstation *wsalloc (int id) {
  int               shmid;
  VDI_Workstation * ws;

  shmid = shmget (WSID (id),
		  sizeof (VDI_Workstation),
		  IPC_CREAT | IPC_EXCL | 0644);

  if (shmid < 0) 
    /*    perror ("ovdisis: wsalloc: shmget"); */
    return NULL;

  ws = (VDI_Workstation *)shmat (shmid, NULL, 0);

  if ((int)ws == -1) {
    perror ("ovdisis: wsalloc: shmat");
    return NULL;
  }

  return ws;
}


/*
** Description
** Deallocate a workstation descriptor.
**
** 1998-10-09 CG
*/
void wsfree (int id) {
  int shmid;

  shmid = shmget (WSID (id),
                  sizeof (VDI_Workstation),
                  0);

  if (shmid < 0)
    perror ("ovdisis: wsfree: shmget");

  if (shmctl(shmid, IPC_RMID, NULL) < 0)
    perror ("ovdisis: wsfree: shmctl");
}


/*
** Description
** Attach to a workstation descriptor. If the workstation doesn't exist,
** NULL is returned.
**
** 1998-10-09 CG
*/
VDI_Workstation *wsattach (int id) {
  int               shmid;
  VDI_Workstation * ws;

  shmid = shmget (WSID (id),
                  sizeof (VDI_Workstation),
                  0);

  if (shmid < 0) {
    perror ("ovdisis: wsattach: shmget");
    return NULL;
  }

  ws = (VDI_Workstation *)shmat (shmid, NULL, 0);

  if ((int)ws == -1) {
    perror ("ovdisis: wsattach: shmat");
    return NULL;
  }

  return ws;
}


/*
** Description
** Detach from a workstation descriptor.
**
** 1998-10-09 CG
*/
void wsdetach (VDI_Workstation * ws) {
  if (shmdt ((char *)ws) < 0)
    perror ("ovdisis: wsdetach: shmdt");
}

