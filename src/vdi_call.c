/*
 * vdi_call.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 * Copyright 1999 - 2001 Christer Gustavsson <cg@nocrew.org>
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

#include <netinet/in.h>
#include <stdio.h>

#include "ovdisis.h"

#include "vdi_text.h"
#include "vdi_marker.h"
#include "vdi_fill.h"
#include "vdi_line.h"
#include "vdi_gdps.h"
#include "vdi_characters.h"
#include "vdi_control.h"
#include "vdi_device.h"
#include "vdi_mouse.h"
#include "vdi_raster.h"
#include "vdi_various.h"
#include "workstation_memory.h"


#ifdef VDI_STRACE
typedef struct
{
  char * name;
} VDICB;

static
VDICB
vdidebug[] =
{
  /*   0 */
  { NULL },
  { "v_opnwk" },
  { "v_clswk" },
  { "v_clrwk" },
  { "v_updwk" },
  /*   5 */
  { "characters" },
  { "v_pline" },
  { "v_pmarker" },
  { "v_gtext" },
  { "v_fillarea" },
  /*  10 */
  { NULL },
  { "gdps" },
  { "vst_height" },
  { "vst_rotation" },
  { "vs_color" },
  /*  15 */
  { "vsl_type" },
  { "vsl_width" },
  { "vsl_color" },
  { "vsm_type" },
  { "vsm_height" },
  /*  20 */
  { "vsm_color" },
  { "vst_font" },
  { "vst_color" },
  { "vsf_interior" },
  { "vsf_style" },
  /*  25 */
  { "vsf_color" },
  { "vq_color" },
  { NULL },
  { NULL },
  { NULL },
  /*  30 */
  { NULL },
  { "vsm_string" },
  { "vswr_mode" },
  { "vsin_mode" },
  { NULL },
  /*  35 */
  { "vql_attributes" },
  { "vqm_attributes" },
  { "vqf_attributes" },
  { "vqt_attributes" },
  { "vst_alignment" },
  /*  40 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  45 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  50 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  55 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  60 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  65 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  70 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  75 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  80 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  85 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  90 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /*  95 */
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  { NULL },
  /* 100 */
  { "v_opnvwk" },
  { "v_clsvwk" },
  { "vq_extnd" },
  { "v_contourfill" },
  { "vsf_perimeter" },
  /* 105 */
  { "v_get_pixel" },
  { "vst_effects" },
  { "vst_point" },
  { "vsl_ends" },
  { "vro_cpyfm" },
  /* 110 */
  { "vr_trnfm" },
  { "vsc_form" },
  { "vsf_udpat" },
  { "vsl_udsty" },
  { "vr_recfl" },
  /* 115 */
  { "vqin_mode" },
  { "vqt_extent" },
  { "vqt_width" },
  { "vex_timv" },
  { NULL },
  /* 120 */
  { NULL },
  { "vrt_cpyfm" },
  { "v_show_c" },
  { "v_hide_c" },
  { "vq_mouse" },
  /* 125 */
  { "vex_butv" },
  { "vex_motv" },
  { "vex_curv" },
  { "vq_key_s" },
  { "vs_clip" },
  /* 130 */
  { "vqt_name" },
  { "vqt_fontinfo" }
};
#endif /*VDI_STRACE */


VdiFunction *vdi_functions[] = {
  /*   0 */ UNUSED, vdi_v_opnwk, vdi_v_clswk, vdi_v_clrwk, vdi_v_updwk,
  /*   5 */ vdi_characters, vdi_v_pline, vdi_v_pmarker, vdi_v_gtext, vdi_v_fillarea,
  /*  10 */ UNUSED, vdi_gdps, vdi_vst_height, vdi_vst_rotation, vdi_vs_color,
  /*  15 */ vdi_vsl_type, vdi_vsl_width, vdi_vsl_color, vdi_vsm_type, vdi_vsm_height,
  /*  20 */ vdi_vsm_color, vdi_vst_font, vdi_vst_color, vdi_vsf_interior, vdi_vsf_style,
  /*  25 */ vdi_vsf_color, vdi_vq_color, UNUSED, UNUSED, UNUSED,
  /*  30 */ UNUSED, vdi_vsm_string, vdi_vswr_mode, vdi_vsin_mode, UNUSED,
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

void (*vdi_handler)(VDIPB * vdipb) = NULL;

void vdi_call(VDIPB *vdiparblk) {
  int routine_nb = vdiparblk->contrl[ROUTINE];
  int handle     = vdiparblk->contrl[VDI_HANDLE];

  if (vdi_handler != NULL) {
    vdi_handler (vdiparblk);
  } else {
    int no_opcodes = sizeof(vdi_functions) / sizeof(VdiFunction *);
    
    vdipb=vdiparblk; /* This is how functions inside oVDIsis can use the passed VDIPB from the application */

    if ((routine_nb == 1) || (routine_nb == 100)) { 
      /* v_opn{v,}wk() are special */
      vdi_functions[routine_nb]((VDI_Workstation *)NULL);
      DEBUG3("vdi_call: Call to %s finished.\n",vdidebug[routine_nb].name);
    } else {
      if(routine_nb >= 0 && routine_nb <= no_opcodes) {
        if(vdi_functions[routine_nb]) {
          if(get_workstation_type(handle) != WS_NOTOPEN) {
            /* Call our function */
#ifdef VDI_STRACE
            DEBUG3("vdi_call: Call to VDI nr %d %s, handle %d.\n", routine_nb, vdidebug[routine_nb].name, handle);
#else
            DEBUG3("vdi_call: Call to VDI nr %d, handle %d.\n", routine_nb, handle);
#endif
            vdi_functions[routine_nb](handle_to_VDIWK(handle)->vwk);
            DEBUG3("vdi_call: Call to VDI nr %d, handle %d finished.\n", routine_nb, handle);
            
          } else {
            DEBUG2("vdi_call: Handle %d not open!\n",handle);
          }
        } else {
          DEBUG2("vdi_call: Unsupported VDI call %d!\n",routine_nb);
        }
      } else {
        DEBUG2("vdi_call: Unsupported VDI call %d!\n",routine_nb);
      }
    }
  }
}


#define FIX(dst, src, len, fun) \
  for(i = 0; i < len; i++) \
  { \
    dst[i] = fun(src[i]); \
  }

static
void
convert_mfdb (MFDB * dst,
              MFDB * src)
{
  dst->fd_addr = (void *)ntohl((long)src->fd_addr);
  dst->fd_w = ntohs (src->fd_w);
  dst->fd_h = ntohs (src->fd_h);
  dst->fd_wdwidth = ntohs (src->fd_wdwidth);
  dst->fd_stand = ntohs (src->fd_stand);
  dst->fd_nplanes = ntohs (src->fd_nplanes);
  dst->fd_r1 = ntohs (src->fd_r1);
  dst->fd_r2 = ntohs (src->fd_r2);
  dst->fd_r3 = ntohs (src->fd_r3);
}


void
vdi_call_be32(VDIPB * vdiparblk)
{
  int              i;
  WORD *           tmp_short;
  static VDIPARBLK e_vdiparblk;
  static VDIPB     o_vdipb =
  {
    e_vdiparblk.contrl,
    e_vdiparblk.intin,
    e_vdiparblk.ptsin,
    e_vdiparblk.intout,
    e_vdiparblk.ptsout
  };
  MFDB             src_mfdb;
  MFDB             dst_mfdb;

  tmp_short = (short *)ntohl((long)vdiparblk->contrl);
  FIX(e_vdiparblk.contrl, tmp_short, NR_CONTRL, ntohs);

  tmp_short = (short *)ntohl((long)vdiparblk->intin);
  FIX(e_vdiparblk.intin, tmp_short, e_vdiparblk.contrl[N_INTIN], ntohs);

  tmp_short = (short *)ntohl((long)vdiparblk->ptsin);
  FIX(e_vdiparblk.ptsin, tmp_short, e_vdiparblk.contrl[N_PTSIN] * 2, ntohs);

  if((e_vdiparblk.contrl[0] == 109) ||
     (e_vdiparblk.contrl[0] == 110) ||
     (e_vdiparblk.contrl[0] == 121))
  {
    tmp_short = (short *)ntohl((long)vdiparblk->contrl);
    convert_mfdb(&src_mfdb, (MFDB *)ntohl(*(long *)&tmp_short[7]));
    convert_mfdb(&dst_mfdb, (MFDB *)ntohl(*(long *)&tmp_short[9]));
    *(MFDB **)&e_vdiparblk.contrl[7] = &src_mfdb;
    *(MFDB **)&e_vdiparblk.contrl[9] = &dst_mfdb;
  }
     
  vdi_call(&o_vdipb);

  tmp_short = (short *)ntohl((long)vdiparblk->contrl);
  FIX(tmp_short, e_vdiparblk.contrl, NR_CONTRL, htons);

  tmp_short = (short *)ntohl((long)vdiparblk->intout);
  FIX(tmp_short, e_vdiparblk.intout,  e_vdiparblk.contrl[N_INTOUT], htons);

  tmp_short = (short *)ntohl((long)vdiparblk->ptsout);
  FIX(tmp_short, e_vdiparblk.ptsout, e_vdiparblk.contrl[N_PTSOUT] * 2, htons);
}
