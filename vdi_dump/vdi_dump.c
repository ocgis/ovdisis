#include <stdio.h>
#include <stdlib.h>

#ifdef __TOS__
#include <aes.h>
#include <vdi.h>
#else
#include "../vdibind/vdibind.h"

VDIPARBLK e_vdiparblk;
VDIPB o_vdipb;          /* Must have this name when using vdibind!!! */
#endif /* __TOS__ */

char *dev_int[] = {
  "xres","yres","noscale","wpixel","hpixel",
  "cheights","linetypes","linewidths","markertypes","markersizes",
  "faces","patterns","hatches","colors","ngdps",
  "cangdps[0]","cangdps[1]","cangdps[2]","cangdps[3]","cangdps[4]",
  "cangdps[5]","cangdps[6]","cangdps[7]","cangdps[8]","cangdps[9]",
  "gdpattr[0]","gdpattr[1]","gdpattr[2]","gdpattr[3]","gdpattr[4]",
  "gdpattr[5]","gdpattr[6]","gdpattr[7]","gdpattr[8]","gdpattr[9]",
  "cancolor","cantextrot","canfillarea","cancellarray","palette",
  "locators","valuators","choicedevs","stringdevs","wstype"
};
char *dev_pts[] = {
  "minwchar","minhchar","maxwchar","maxhchar",
  "minwline","zero5","maxwline","zero7",
  "minwmark","minhmark","maxwmark","maxhmark"
};

char *inq_int[] = {
  "screentype","bgcolors","textfx","canscale","planes",
  "lut","rops","cancontourfill","textrot","writemodes",
  "inputmodes","textalign","inking","rubberbanding","maxvertices",
  "maxintin","mousebuttons","widestyles","widemodes"
};

int do_stuff(FILE *);

/* Hmm, nice main() */
int main(int argc, char *argv[])
{
  FILE *fp;

  if(argc < 2)
    {
      fprintf(stderr,"Usage: %s filename\n",argv[0]);
      return 1;
    }

  if((fp=fopen(argv[1],"w"))!=NULL)
    {
      do_stuff(fp);
      fclose(fp);
    }
  return 0;
}

int do_stuff(FILE *fp)
{
  int i,vp1,vh1;
  int work_in[11] = { 1,1,1,1,1,1,1,1,1,1,2 };
  int work_out[57];

#ifndef __TOS__
  /* initialize o_vdipb */
  o_vdipb.contrl=e_vdiparblk.contrl;
  o_vdipb.intin=e_vdiparblk.intin;
  o_vdipb.ptsin=e_vdiparblk.ptsin; 
  o_vdipb.intout=e_vdiparblk.intout; 
  o_vdipb.ptsout=e_vdiparblk.ptsout;
#endif /* __TOS__ */


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

  for(i=0 ; i<45 ; i++)
    fprintf(fp,"%s = %d\n",dev_int[i],work_out[i]);
  for(i=0 ; i<12 ; i++)
    fprintf(fp,"%s = %d\n",dev_pts[i],work_out[i+45]);
  fprintf(fp,"\n");

  vq_extnd(vh1, 1, work_out);
  for(i=0 ; i<19 ; i++)
    fprintf(fp,"%s = %d\n",inq_int[i],work_out[i]);
  for(; i<57 ; i++)
    fprintf(fp,"reserved[%d] = %d\n",i-19,work_out[i]);

  v_clsvwk(vh1);

#ifndef __TOS__
  v_clswk(vp1);
#endif /* __TOS__ */


  return 1;
}


