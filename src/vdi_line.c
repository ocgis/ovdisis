#include <stdio.h>

#include "ovdisis.h"
#include "various.h"

#define LOWERODD(a) ((a)&0x1?(a):(a)-1)

void vdi_v_pline(VDI_Workstation *vwk)
{
  int n,ni;
  unsigned long col;
  RECT cor;

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->line_a.color);
  col = get_color(vwk, ni);

  for(n=0 ; n < (int)vdipb->contrl[N_PTSIN]-1 ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2];
      cor.y1 = (int)vdipb->ptsin[1+n*2];
      cor.x2 = (int)vdipb->ptsin[2+n*2];
      cor.y2 = (int)vdipb->ptsin[3+n*2];
      if(do_lineclip(&cor, &vwk->clip))
	{
	  if(cor.y1 == cor.y2)
	    {
	      FBhline(vwk->fb, cor.x1, cor.x2, cor.y1, col);
	    }
	  else
	    {
	      FBline(vwk->fb, cor.x1, cor.y1, cor.x2, cor.y2, col);
	    }
	}
    }
}

void vdi_vsl_color(VDI_Workstation *vwk)
{
  vwk->line_a.color=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->line_a.color;

  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vsl_width(VDI_Workstation *vwk)
{
  vwk->line_a.width=(int)LOWERODD(vdipb->ptsin[0]);
  vdipb->ptsout[0]=(short)vwk->line_a.width;

  vdipb->contrl[N_PTSOUT]=1;
}

void vdi_vsl_type(VDI_Workstation *vwk)
{
  vwk->line_a.type=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->line_a.type;

  vdipb->contrl[N_INTOUT]=1;
}

void vdi_vsl_udsty(VDI_Workstation *vwk)
{
  vwk->line_a.udsty=(unsigned short)vdipb->intin[0];
}

void vdi_vsl_ends(VDI_Workstation *vwk)
{
  vwk->line_a.ends_beg=(int)vdipb->intin[0];
  vwk->line_a.ends_end=(int)vdipb->intin[1];
}

void vdi_vql_attributes(VDI_Workstation *vwk)
{
  vdipb->ptsout[0]=(short)vwk->line_a.width;
  vdipb->ptsout[1]=0;

  vdipb->intout[0]=(short)vwk->line_a.type;
  vdipb->intout[1]=(short)vwk->line_a.color;
  vdipb->intout[2]=(short)vwk->write_mode;
  vdipb->intout[3]=(short)vwk->line_a.ends_beg;
  vdipb->intout[4]=(short)vwk->line_a.ends_end;

  vdipb->contrl[N_PTSOUT]=1;
  vdipb->contrl[N_INTOUT]=5;
}

