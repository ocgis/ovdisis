/*
 * vdi_mouse.h
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

#ifndef _VDI_MOUSE_H_
#define _VDI_MOUSE_H_

extern void vdi_vsc_form(VDI_Workstation *);
extern void vdi_v_show_c(VDI_Workstation *);
extern void vdi_v_hide_c(VDI_Workstation *);
extern void vdi_vq_mouse(VDI_Workstation *);
extern void vdi_vex_butv(VDI_Workstation *);
extern void vdi_vex_motv(VDI_Workstation *);
extern void vdi_vex_curv(VDI_Workstation *);

#endif /* _VDI_MOUSE_H_ */
