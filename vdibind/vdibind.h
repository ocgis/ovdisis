/*
 * vdibind.h
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

#ifndef _VDIBIND_H_
#define _VDIBIND_H_

#include "vdidefs.h"

extern void v_opnwk(int *, int *, int *);
extern void v_clswk(int);
extern void v_opnvwk(int *, int *, int *);
extern void v_clsvwk(int);
extern void v_clrwk(int);
extern void v_updwk(int);
extern void vs_clip(int, int, int *);
extern int vswr_mode(int, int);
extern void vs_color(int, int, int *);
extern void v_gtext(int, int, int, char *);
extern int vst_color(int, int);
extern void vst_height(int, int, int *, int *, int *, int *);
extern int vst_point(int, int, int *, int *, int *, int *);
extern int vst_effects(int, int);
extern void vst_alignment(int, int, int, int *, int *);
extern int vst_rotation(int, int);
extern void v_pline(int, int, int *);
extern int vsl_color(int, int);
extern int vsl_width(int, int);
extern int vsl_type(int, int);
extern void vsl_udsty(int, int);
extern void vsl_ends(int, int, int);
extern void v_pmarker(int, int, int *);
extern int vsm_color(int, int);
extern int vsm_height(int, int);
extern int vsm_type(int, int);
extern void v_fillarea(int, int, int *);
extern void vr_recfl(int, int *);
extern void v_contourfill(int, int, int, int);
extern int vsf_color(int, int);
extern int vsf_interior(int, int);
extern int vsf_style(int, int);
extern void vsf_udpat(int, int *, int);
extern int vsf_perimeter(int, int);
extern void v_bar(int, int *);
extern void v_arc(int, int, int, int, int, int);
extern void v_pieslice(int, int, int, int, int, int);
extern void v_circle(int, int, int, int);
extern void v_ellipse(int, int, int, int, int);
extern void v_ellarc(int, int, int, int, int, int, int);
extern void v_ellpie(int, int, int, int, int, int, int);
extern void v_rbox(int, int *);
extern void v_rfbox(int, int *);
extern void v_justified(int, int, int, char *, int, int, int);
extern void vro_cpyfm(int, int, int *, MFDB *, MFDB *);
extern void vrt_cpyfm(int, int, int *, MFDB *, MFDB *, int *);
extern void vr_trnfm(int, MFDB *, MFDB *);
extern void v_get_pixel(int, int, int, int *, int *);
extern void vsc_form(int, int *);
extern void vex_timv(int, void *, void *, int *);
extern void v_show_c(int, int);
extern void v_hide_c(int);
extern void vq_mouse(int, int *, int *, int *);
extern void vex_butv(int, void *, void *);
extern void vex_motv(int, void *, void *);
extern void vex_curv(int, void *, void *);
extern void vq_extnd(int, int, int *);
extern void vq_key_s(int, int *);
extern void vq_color(int, int, int, int *);
extern void vql_attributes(int, int *);
extern void vqm_attributes(int, int *);
extern void vqf_attributes(int, int *);
extern void vqt_attributes(int, int *);
extern void vqt_extent(int, char *, int *);
extern void vqt_width(int, char, int *, int *, int *);
extern void vqt_name(int, int, char *);
extern void vqt_fontinfo(int, int *, int *, int *, int *);
extern void vqin_mode(int, int, int *);
extern void vq_chcells(int, int *, int *);
extern void v_exit_cur(int);
extern void v_enter_cur(int);
extern void v_curup(int);
extern void v_curdown(int);
extern void v_curright(int);
extern void v_curleft(int);
extern void v_curhome(int);
extern void v_eeos(int);
extern void v_eeol(int);
extern void vs_curaddress(int, int, int);
extern void v_curtext(int, char *);
extern void v_rvon(int);
extern void v_rvoff(int);
extern void vq_curaddress(int, int *, int *);
extern void v_hardcopy(int);

#endif /* _VDIBIND_H_ */


