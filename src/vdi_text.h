/*
 * vdi_text.h
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

#ifndef _VDI_TEXT_H_
#define _VDI_TEXT_H_

extern void vdi_v_gtext(VDI_Workstation *);

extern void vdi_vst_color(VDI_Workstation *);
extern void vdi_vst_height(VDI_Workstation *);
extern void vdi_vst_point(VDI_Workstation *);
extern void vdi_vst_effects(VDI_Workstation *);
extern void vdi_vst_alignment(VDI_Workstation *);
extern void vdi_vst_rotation(VDI_Workstation *);

extern void vdi_vqt_attributes(VDI_Workstation *);
extern void vdi_vqt_extent(VDI_Workstation *);
extern void vdi_vqt_width(VDI_Workstation *);

#endif /* _VDI_TEXT_H_ */

