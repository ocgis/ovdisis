/*
 * otestis.c
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
#include <stdlib.h>

#ifdef __TOS__
#include <aes.h>
#include <vdi.h>
#define M_DOT 1
#define M_PLUS 2
#define M_ASTERISK 3
#define M_SQUARE 4
#define M_CROSS 5
#define M_DIAMOND 6

#define CLIP_OFF 0
#define CLIP_ON 1
#else
#include "ovdi_more.h" /* for ovdi_getchar() */
#include "vdibind.h"
#endif /* __TOS__ */

int do_stuff(void);

/* Hmm, nice main() */
int main()
{
  do_stuff();
  return 0;
}

int do_stuff()
{
  int i,vp1,vh1,vh2,pxy[16];
  int work_in[11] = { 1,1,1,1,1,1,1,1,1,1,2 };
  int work_out[57];

#ifdef __TOS__
  vp1 = graf_handle(&i,&i,&i,&i);
#else
  /* Open physical workstation */
  v_opnwk(work_in, &vp1, work_out);
  if(!vp1)
    return 0;
#endif /* __TOS__ */


  vh1 = vp1;
  v_opnvwk(work_in, &vh1, work_out);
  if(!vh1)
    return 0;
  /* Should close the open workstation */

  v_clrwk(vh1);

  vsm_color(vh1, 1);
  vsm_height(vh1, 90);
  vsm_type(vh1, M_DOT);
  pxy[0] = 100;
  pxy[1] = 100;
  v_pmarker(vh1, 1, pxy);
  
  vh2 = vp1;
  v_opnvwk(work_in, &vh2, work_out);
  if(!vh2)
    return 0;
  /* Should close the open workstations */

  vsm_height(vh2, 50);
  vsm_color(vh2, 2);
  vsm_type(vh2, M_PLUS);
  pxy[0] = 150;
  pxy[1] = 150;
  v_pmarker(vh2, 1, pxy);


  vsl_color(vh2, 2);
  pxy[0] = 350;
  pxy[1] = 50;
  pxy[2] = 600;
  pxy[3] = 130;
  pxy[4] = 400;
  pxy[5] = 400;
  pxy[6] = 50;
  pxy[7] = 100;
  v_pline(vh2, 4, pxy);


  pxy[0] = 550;
  pxy[1] = 50;
  pxy[2] = 680;
  pxy[3] = 200;
  v_rbox(vh2, pxy);

  pxy[0]+=5; pxy[1]+=5; 
  pxy[2]-=5; pxy[3]-=5; 
  vsf_color(vh2, 3);
  v_rfbox(vh2, pxy);

  v_arc(vh2, 600,200,50,0,600);
  v_arc(vh2, 600,200,50,1000,3000);
  v_ellarc(vh2, 600,350,100,50,0,600);
  v_ellarc(vh2, 600,350,100,50,1000,3000);

  pxy[0] = 105;
  pxy[1] = 105;
  pxy[2] = 480;
  pxy[3] = 310;
  vs_clip(vh2, CLIP_ON, pxy);
  
  vsf_color(vh2, 1);
  vsf_interior(vh2, FIS_HOLLOW);
  vsf_perimeter(vh2, 1);
  pxy[0] = 105;
  pxy[1] = 105;
  pxy[2] = 480;
  pxy[3] = 310;
  v_bar(vh2, pxy);

  vsm_type(vh1, M_CROSS);
  pxy[0] = 300;
  pxy[1] = 200;
  v_pmarker(vh1, 1, pxy);
  
  vsm_type(vh2, M_DIAMOND);
  pxy[0] = 120;
  pxy[1] = 120;
  v_pmarker(vh2, 1, pxy);

  vsm_color(vh2, 5);
  vsm_type(vh2, M_SQUARE);
  pxy[0] = 400;
  pxy[1] = 300;
  v_pmarker(vh2, 1, pxy);

  vsm_color(vh1, 2);
  vsm_type(vh1, M_ASTERISK);
  pxy[0] = 500;
  pxy[1] = 200;
  v_pmarker(vh1, 1, pxy);


  pxy[0] = 460;
  pxy[1] = 170;
  pxy[2] = 530;
  pxy[3] = 220;
  vs_clip(vh1, CLIP_ON, pxy);
  
  vsf_color(vh1, 1);
  vsf_interior(vh1, FIS_HOLLOW);
  vsf_perimeter(vh1, 1);
  pxy[0] = 460;
  pxy[1] = 170;
  pxy[2] = 530;
  pxy[3] = 220;
  v_bar(vh1, pxy);


  vsm_color(vh1, 7);
  vsm_type(vh1, M_ASTERISK);
  pxy[0] = 500;
  pxy[1] = 200;
  v_pmarker(vh1, 1, pxy);

  vsl_color(vh2, 6);
  pxy[0] = 350;
  pxy[1] = 50;
  pxy[2] = 600;
  pxy[3] = 130;
  pxy[4] = 400;
  pxy[5] = 400;
  pxy[6] = 50;
  pxy[7] = 100;
  v_pline(vh2, 4, pxy);

  vsf_color(vh2, 11);
  pxy[0] = 300;
  pxy[1] = 90;
  pxy[2] = 500;
  pxy[3] = 150;
  v_bar(vh2, pxy);

  vsf_color(vh2, 10);
  vsf_interior(vh2, FIS_SOLID);
  pxy[0]+=4; pxy[1]+=4;
  pxy[2]-=4; pxy[3]-=4;
  vr_recfl(vh2, pxy);

  vsf_color(vh2, 12);
  v_circle(vh2, 130, 270, 50);

  vsf_color(vh2, 13);
  vsf_interior(vh2, FIS_HOLLOW);
  v_circle(vh2, 130, 270, 54);

  vs_clip(vh2, CLIP_OFF, pxy);

  vsl_color(vh2, 8);
  pxy[0] = 0;
  pxy[1] = 0;
  pxy[2] = work_out[0];
  pxy[3] = work_out[1];
  v_pline(vh2, 2, pxy);

  for(i=0 ; i<128 ; i++)
    {
      pxy[4] = 50 + (pxy[0] = 0);
      pxy[6] = 50 + (pxy[2] = 30);
      pxy[5] = pxy[7] = pxy[1] = pxy[3] = i*4+5;
      vsl_color(vh2, i);
      v_pline(vh2, 2, pxy);
      pxy[1]++; pxy[3]++;
      v_pline(vh2, 2, pxy);

      vsl_color(vh2, i+128);
      v_pline(vh2, 2, &pxy[4]);
      pxy[5]++; pxy[7]++;
      v_pline(vh2, 2, &pxy[4]);

    }

  /* default font, 8x8 */
  for(i=0 ; i<8 ; i++)
    {
      vst_color(vh2, i);
      v_gtext(vh2, 70, i*8, "NoCrew");
    }
  vst_effects(vh2, UNDERLINED);
  v_gtext(vh2, 70, i*8, "NoCrew");

  vst_effects(vh2, 0);
  /* 6x6 font */
  vst_point(vh2, 8, &i, &i, &i, &i);
  for(i=0 ; i<8 ; i++)
    {
      vst_color(vh2, i);
      v_gtext(vh2, 130, i*6, "NoCrew");
    }
  vst_effects(vh2, UNDERLINED);
  v_gtext(vh2, 130, i*6, "NoCrew");

  vst_effects(vh2, 0);
  /* 8x16 font */
  vst_point(vh2, 10, &i, &i, &i, &i);
  for(i=0 ; i<8 ; i++)
    {
      vst_color(vh2, i);
      v_gtext(vh2, 180, i*16, "NoCrew");
    }
  vst_effects(vh2, UNDERLINED);
  v_gtext(vh2, 180, i*16, "NoCrew");
  

  /* Test v_get_pixel, it works, trust me! :) */
  /*
  for(i=0 ; i<16 ; i++)
    {
      int pvalue,pcol;

      v_get_pixel(vh2, 15, i*4+10, &pvalue, &pcol);
      printf("otestis: v_get_pixel %d,%d Value: %d Color %d\n",15,i*4+10,pvalue,pcol);
    }
   */

  vs_clip(vh2, CLIP_OFF, pxy);

  vsf_color(vh2, 1);
  vsf_interior(vh2, FIS_HOLLOW);
  vsf_perimeter(vh2, 1);
  pxy[0]=280; pxy[1]=380; pxy[2]=380; pxy[3]=480;
  v_bar(vh2, pxy);
  pxy[0]=400; pxy[1]=380; pxy[2]=580; pxy[3]=480;
  v_bar(vh2, pxy);

#ifdef __TOS__
  getchar();
#else
  while(ovdi_getchar(vp1) != 'd')
    ;
#endif /* __TOS__ */

  {
    MFDB src,dst;
    int rc[8], i;

    pxy[0]=280; pxy[1]=380; pxy[2]=380; pxy[3]=480;
    vs_clip(vh2, CLIP_ON, pxy);

    /* all other settings in MFDB are calculated when using the screen */
    src.fd_addr = 0; /* screen */
    dst.fd_addr = 0; /* also screen */

    rc[0] = 0; rc[1] = 0; rc[2] = 200; rc[3] = 200; /* src */
    rc[4] = 300; rc[5] = 400; rc[6] = 495; rc[7] = 480; /* dst */

    vro_cpyfm(vh2, S_XOR_D, rc, &src, &dst);

    for(i=0 ; i<16 ; i++) {
#ifdef __TOS__
      getchar();
#else
      while(ovdi_getchar(vp1) != 's')
	;
#endif /* __TOS__ */

      rc[4]-=3; rc[5]-=2;
      vro_cpyfm(vh2, NOT_S, rc, &src, &dst);
    }


    vs_clip(vh2, CLIP_OFF, pxy);

    rc[0]=0; rc[1]=0; rc[2]=100; rc[3]=100;
    rc[4]=30; rc[5]=30; rc[6]=130; rc[7]=130;
    pxy[0]=2; pxy[1]=3; /* fg and bg colour */
    vrt_cpyfm(vh2, MD_REPLACE, rc, &src, &dst, pxy);


    pxy[0]=400; pxy[1]=380; pxy[2]=580; pxy[3]=480;
    vs_clip(vh2, CLIP_ON, pxy);

    rc[0] = 0; rc[1] = 0; rc[2] = 200; rc[3] = 200; /* src */
    rc[4] = 390; rc[5] = 370; rc[6] = 495; rc[7] = 480; /* dst */

    vro_cpyfm(vh2, S_ONLY, rc, &src, &dst);

    vs_clip(vh2, CLIP_OFF, pxy);
  }

#ifdef __TOS__
  getchar();
#else
  while(ovdi_getchar(vp1) != 's')
    ;
#endif /* __TOS__ */

  pxy[0] = 1000;
  pxy[1] = 0;
  pxy[2] = 0;
  vs_color(vh2, 1, pxy);
  pxy[0] = 500;
  pxy[1] = 500;
  pxy[2] = 500;
  vs_color(vh2, 7, pxy);

  vq_color(vh2, 1, 0, pxy);
  vq_color(vh2, 7, 1, pxy);

  vsl_color(vh2, 1);
  pxy[0] = 130;
  pxy[1] = 80;
  pxy[2] = 160;
  pxy[3] = 350;
  v_pline(vh2, 2, pxy);

#ifdef __TOS__
  getchar();
#else
  while(ovdi_getchar(vp1) != 'a')
    ;
#endif /* __TOS__ */
  
  v_clsvwk(vh1);
  v_clsvwk(vh2);


#ifndef __TOS__
  v_clswk(vp1);
#endif /* __TOS__ */


  return 0;
}


