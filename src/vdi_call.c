/*
 * vdi_call.c
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

#include "vdi_text.h"
#include "vdi_marker.h"
#include "vdi_fill.h"
#include "vdi_line.h"
#include "vdi_gdps.h"
#include "vdi_characters.h"
#include "vdi_control.h"
#include "vdi_mouse.h"
#include "vdi_raster.h"
#include "vdi_various.h"

#define DEBUGLEVEL 3

VdiFunction *vdi_functions[] = {
  /*   0 */ UNUSED, vdi_v_opnwk, vdi_v_clswk, vdi_v_clrwk, vdi_v_updwk,
  /*   5 */ vdi_characters, vdi_v_pline, vdi_v_pmarker, vdi_v_gtext, vdi_v_fillarea,
  /*  10 */ UNUSED, vdi_gdps, vdi_vst_height, vdi_vst_rotation, vdi_vs_color,
  /*  15 */ vdi_vsl_type, vdi_vsl_width, vdi_vsl_color, vdi_vsm_type, vdi_vsm_height,
  /*  20 */ vdi_vsm_color, vdi_vst_font, vdi_vst_color, vdi_vsf_interior, vdi_vsf_style,
  /*  25 */ vdi_vsf_color, vdi_vq_color, UNUSED, UNUSED, UNUSED,
  /*  30 */ UNUSED, UNUSED, vdi_vswr_mode, UNUSED, UNUSED,
  /*  35 */ vdi_vql_attributes, vdi_vqm_attributes, vdi_vqf_attributes, vdi_vqt_attributes, vdi_vst_alignment,
  /*  40 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  45 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  50 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  55 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  60 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  65 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  70 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  75 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  80 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  85 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  90 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /*  95 */ UNUSED, UNUSED, UNUSED, UNUSED, UNUSED,
  /* 100 */ vdi_v_opnvwk, vdi_v_clsvwk, vdi_vq_extnd, vdi_v_contourfill, vdi_vsf_perimeter,
  /* 105 */ vdi_v_get_pixel, vdi_vst_effects, vdi_vst_point, vdi_vsl_ends, vdi_vro_cpyfm,
  /* 110 */ vdi_vr_trnfm, vdi_vsc_form, vdi_vsf_udpat, vdi_vsl_udsty, vdi_vr_recfl,
  /* 115 */ vdi_vqin_mode, vdi_vqt_extent, vdi_vqt_width, vdi_vex_timv, UNUSED,
  /* 120 */ UNUSED, vdi_vrt_cpyfm, vdi_v_show_c, vdi_v_hide_c, vdi_vq_mouse,
  /* 125 */ vdi_vex_butv, vdi_vex_motv, vdi_vex_curv, vdi_vq_key_s, vdi_vs_clip,
  /* 130 */ vdi_vqt_name, vdi_vqt_fontinfo
};

VDIPB *vdipb;
extern VDIWK wk[MAX_HANDLES];    /* declared in vdi_control.c */
extern int wk_open[MAX_HANDLES]; /* ----------- " ----------- */ 


void vdi_call(VDIPB *vdiparblk)
{
  int no_opcodes = sizeof(vdi_functions) / sizeof(VdiFunction *);

  vdipb=vdiparblk;

  if ((vdipb->contrl[ROUTINE] == 1) ||
      (vdipb->contrl[ROUTINE] == 100)) { 
    /* v_opn{v,}wk() is special */
    vdi_functions[vdipb->contrl[ROUTINE]]((VDI_Workstation *)NULL);
    IDEBUG("vdi_call: Call to v_opnwk finished.\n");
  } else {
    if(vdipb->contrl[ROUTINE] >= 0 && vdipb->contrl[ROUTINE] <= no_opcodes) {
      if(vdi_functions[vdipb->contrl[ROUTINE]]) {
	if(wk_open[vdipb->contrl[VDI_HANDLE]-1]) {
	  /* Call our function */
	  vdi_functions[vdipb->contrl[ROUTINE]](wk[vdipb->contrl[VDI_HANDLE]-1].vwk);
	  IDEBUG("vdi_call: Call to VDI nr %d, handle %d finished.\n",
		 vdipb->contrl[ROUTINE], vdipb->contrl[VDI_HANDLE]);

	} else {
	  EDEBUG("vdi_call: Handle %d not open!\n",vdipb->contrl[VDI_HANDLE]);
	}
      } else {
	EDEBUG("vdi_call: Unsupported VDI call %d!\n",vdipb->contrl[ROUTINE]);
      }
    } else {
      EDEBUG("vdi_call: Unsupported VDI call %d!\n",vdipb->contrl[ROUTINE]);
    }
  }
}


