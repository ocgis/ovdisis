/*
 * inits.h
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

#ifndef _INITS_H_
#define _INITS_H_

#include "inits.h"
#include "ovdisis.h"

void init_workstation(VDI_Workstation *);
void init_cmap(VDI_Workstation *);
void init_marker(VDI_Workstation *);
void init_line(VDI_Workstation *);
void init_fill(VDI_Workstation *);
void init_device(VDI_Workstation *);
void init_text(VDI_Workstation *);

void copy_workstation(VDI_Workstation *, VDI_Workstation *);
void copy_cmap(VDI_Workstation *, VDI_Workstation *);
void copy_marker(VDI_Workstation *, VDI_Workstation *);
void copy_line(VDI_Workstation *, VDI_Workstation *);
void copy_fill(VDI_Workstation *, VDI_Workstation *);
void copy_device(VDI_Workstation *, VDI_Workstation *);
void copy_text(VDI_Workstation *, VDI_Workstation *);

#endif /* _INITS_H_ */
