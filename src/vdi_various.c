#include <stdio.h>

#include "ovdisis.h"
#include "various.h"

void vdi_vs_color(VDI_Workstation *vwk)
{
  int i,ni;

  i = min(vdipb->intin[0], 255);

  ni = gem2tos_color(vwk->inq.attr.planes, i);
  
  vwk->vdi_cmap.red[i]   = vdipb->intin[1];
  vwk->vdi_cmap.green[i] = vdipb->intin[2];
  vwk->vdi_cmap.blue[i]  = vdipb->intin[3];

  /* Don't put cmap if in TrueColor mode */
  if(vwk->inq.attr.planes < 16)
    {
      vwk->fb->cmap->red[ni]   = (unsigned short)((vwk->vdi_cmap.red[i]   * 65535) / 1000);
      vwk->fb->cmap->green[ni] = (unsigned short)((vwk->vdi_cmap.green[i] * 65535) / 1000);
      vwk->fb->cmap->blue[ni]  = (unsigned short)((vwk->vdi_cmap.blue[i]  * 65535) / 1000);

      vwk->fb->cmap->start = ni;
      vwk->fb->cmap->end   = ni;
      vwk->fb->cmap->len   = 1;
      ADEBUG("vs_color: Putting cmap %p in framebuffer %p\n",
	     vwk->fb->cmap, vwk->fb);
      FBputcmap(vwk->fb, vwk->fb->cmap);
    }

  ADEBUG("vs_color: VDI colour %d is set to: %d, %d, %d\n",
	 i,vwk->vdi_cmap.red[i],vwk->vdi_cmap.green[i],vwk->vdi_cmap.blue[i]);

  if(vwk->inq.attr.planes < 16)
    ADEBUG("vs_color: Which means cmap colour %d is set to: %u, %u, %u\n",
	   ni, vwk->fb->cmap->red[ni], vwk->fb->cmap->green[ni], vwk->fb->cmap->blue[ni]);
}

void vdi_vq_color(VDI_Workstation *vwk)
{
  int i,ni;

  i = vdipb->intin[0];

  if(i > vwk->dev.attr.colors)
    vdipb->intout[0] = -1;    /* Color out of range */
  else
    {
      ni = gem2tos_color(vwk->inq.attr.planes, i);
      vdipb->intout[0] = i;

      /* Return the actual colour */
      if(vdipb->intin[1] && (vwk->inq.attr.planes < 16))
	{
	  vdipb->intout[1] = (short)((vwk->fb->cmap->red[ni]   * 1000) / 65535);
	  vdipb->intout[2] = (short)((vwk->fb->cmap->green[ni] * 1000) / 65535);
	  vdipb->intout[3] = (short)((vwk->fb->cmap->blue[ni]  * 1000) / 65535);

	  ADEBUG("vq_color: Cmap colour %d is %u, %u, %u\n",
		 ni,vwk->fb->cmap->red[ni],vwk->fb->cmap->green[ni],vwk->fb->cmap->blue[ni]);
	  ADEBUG("vq_color: Which means VDI colour %d is %u, %u, %u\n",
		 i,vdipb->intout[1],vdipb->intout[2],vdipb->intout[3]);
	}
      else /* Return the set value, or value of virtual pen if TC mode */
	{
	  vdipb->intout[1] = (short)vwk->vdi_cmap.red[i];
	  vdipb->intout[2] = (short)vwk->vdi_cmap.green[i];
	  vdipb->intout[3] = (short)vwk->vdi_cmap.blue[i];

	  ADEBUG("vq_color: VDI colour %d was set to %u, %u, %u\n",
		 i,vdipb->intout[1],vdipb->intout[2],vdipb->intout[3]);
	}
    }

  vdipb->contrl[N_INTOUT] = 4;
}

void vdi_v_get_pixel(VDI_Workstation *vwk)
{
  unsigned long ti;
  unsigned short tc;
  unsigned char *pti;

  ti = FBgetpixel(vwk->fb, vdipb->ptsin[0], vdipb->ptsin[1]);

  if(vwk->inq.attr.planes < 16)
    {
      vdipb->intout[0] = (unsigned short)ti;
      vdipb->intout[1] = (unsigned short)tos2gem_color(vwk->inq.attr.planes, ti);
    }
  else /* TrueColor mode */
    {
      /* FBgetpixel returns 24 bit Truecolor, 0xrr00ggbb */

      pti = (unsigned char *)&ti;
      tc = (((pti[0] << 8) & 0xf800) |
	    ((pti[2] << 3) & 0x07e0) |
	    ((pti[3] >> 3) & 0x001f));

      vdipb->intout[0] = 0;
      vdipb->intout[1] = tc;
    }

  vdipb->contrl[N_INTOUT] = 2;
}

void vdi_vex_timv(VDI_Workstation *vwk)
{
  EDEBUG("vex_timv: Call not implemented!\n");
}

void vdi_vq_key_s(VDI_Workstation *vwk)
{
  EDEBUG("vq_key_s: Call not implemented!\n");
}

void vdi_vqin_mode(VDI_Workstation *vwk)
{
  EDEBUG("vqin_mode: Call not implemented!\n");
}


