/*
 * vdi_fill.h
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

#ifndef _VDI_FILL_H_
#define _VDI_FILL_H_

extern void vdi_v_fillarea(VDI_Workstation *);
extern void vdi_vr_recfl(VDI_Workstation *);
extern void vdi_v_contourfill(VDI_Workstation *);

extern void vdi_vsf_color(VDI_Workstation *);
extern void vdi_vsf_interior(VDI_Workstation *);
extern void vdi_vsf_style(VDI_Workstation *);
extern void vdi_vsf_udpat(VDI_Workstation *);
extern void vdi_vsf_perimeter(VDI_Workstation *);

extern void vdi_vqf_attributes(VDI_Workstation *);

#endif /* _VDI_FILL_H_ */
