/*
 * vdi_control.h
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

#ifndef _VDI_CONTROL_H_
#define _VDI_CONTROL_H_

extern void vdi_v_opnwk(VDI_Workstation *);
extern void vdi_v_clswk(VDI_Workstation *);

extern void vdi_v_opnvwk(VDI_Workstation *);
extern void vdi_v_clsvwk(VDI_Workstation *);
extern void vdi_v_clrwk(VDI_Workstation *);
extern void vdi_v_updwk(VDI_Workstation *);
extern void vdi_vs_clip(VDI_Workstation *);
extern void vdi_vswr_mode(VDI_Workstation *);
extern void vdi_vq_extnd(VDI_Workstation *);

#endif /* _VDI_CONTROL_H_ */
