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

#ifndef _WORKSTATION_MEMORY_H_
#define _WORKSTATION_MEMORY_H_


/* 
** Description
** This return a VDIWK where the virtual workstation is the same as the
** physical one and where the physical->handle is initialised with a handle
** number
*/
VDIWK *get_physical_workstation_memory();

/**
** Description
** Free memory allocated with get_physical_workstation_memory
*/
void free_physical_workstation_memory(VDIWK *wk);

/* 
** Description
** This return a VDIWK where the virtual workstation is linked to the
** physical one (which handle is physical_wk_handle). The handle
** for the virtual workstation is set.
*/
VDIWK *get_virtual_workstation_memory(int physical_wk_handle);

/*
** Description
** Free memory allocated with get_virtual_workstation_memory
*/
void free_virtual_workstation_memory(VDIWK *wk);

/*
** Description
** Returns the VDIWK corresponding to the passed handle
*/
VDIWK *handle_to_VDIWK(int handle);

/*
** Description
** Returns the type of memory allocated. Possible values are 
** WS_PHYSICAL, WS_VIRTUAL, WS_NOTOPEN
*/
int get_workstation_type(int handle);


VDI_Workstation *wsalloc (int id);
void wsfree (int id);
VDI_Workstation *wsattach (int id);
void wsdetach (VDI_Workstation * ws);

#endif
