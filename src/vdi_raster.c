#include <stdio.h>

#include "ovdisis.h"
#include "various.h"

#define USHORTS_TO_POINTER(m,l) ((void *)(((unsigned long)(m)<<16) | ((unsigned long)(l&0xffff))))

void vdi_vro_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  FBBLTPBLK *fbb;
  RECT srccor, dstcor, tmpcor;

  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  /* order the coordinates as I like them. */
  fix_rect(&srccor);
  fix_rect(&dstcor);

  /* Completely ignore what the user says that the destination 
     width and height should be and recalculate them here.
     They must be recalculated after the fix. */

  dstcor.x2 = srccor.x2-srccor.x1 + dstcor.x1;
  dstcor.y2 = srccor.y2-srccor.y1 + dstcor.y1;
  tmpcor = dstcor;

  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  /* check first if clipping takes away everything,
     if destination is the screen */
  if(!dst->fd_addr && !do_rectclip(&dstcor, &vwk->clip))
    return;

  /* setup fbb according to the screen format */
  fbb = FBgetbltpblk(vwk->fb);

  /* dst size is the same as src size here, only that dst
     might have been clipped earlier */
  fbb->b_wd = dstcor.x2 - dstcor.x1 + 1;
  fbb->b_ht = dstcor.y2 - dstcor.y1 + 1;
  fbb->op_tab = (vdipb->intin[0] << 0 |
		 vdipb->intin[0] << 8 |
		 vdipb->intin[0] << 16 |
		 vdipb->intin[0] << 24);

  /* adjust coordinates if destination is clipped */
  fbb->s_xmin = srccor.x1 + dstcor.x1-tmpcor.x1;
  fbb->s_ymin = srccor.y1 + dstcor.y1-tmpcor.y1;
  if(src->fd_addr) {
    fbb->plane_ct = src->fd_nplanes; /* use source no of bitplanes! */
    fbb->s_form = src->fd_addr;

    if(src->fd_stand) {
      /* block is in VDI format */

      fbb->s_nxwd = 2;
      fbb->s_nxln = src->fd_wdwidth * 2;
      fbb->s_nxpl = src->fd_wdwidth * 2 * src->fd_h;
    } /* else block is in machine dependent format */

    /* "machine dependent" could be discussed. 
       On one hand, machine dependent _is_ machine dependent, but
       on the other hand, machine dependent is most likely associated
       with Atari interleaved format */

  } /* else source is the screen, i.e. already setup by FBgetbltpblk() */

  fbb->d_xmin = dstcor.x1;
  fbb->d_ymin = dstcor.y1;
  if(dst->fd_addr) {
    fbb->d_form = dst->fd_addr;

    if(dst->fd_stand) {
      /* destination should be in VDI format */

      fbb->d_nxwd = 2;
      fbb->d_nxln = dst->fd_wdwidth * 2;
      fbb->d_nxpl = dst->fd_wdwidth * 2 * dst->fd_h;
    } /* else destination should be in machine dependent format */

    /* See discussion about machine dependent above */

  } /* else destination is the screen, i.e. already setup by FBgetbltpblk() */

  FBbitblt(vwk->fb, fbb);

  FBfreebltpblk(fbb);

  IDEBUG("vro_cpyfm: %d,%d -> %d,%d  w: %d h: %d mode: %d\n",fbb->s_xmin, fbb->s_ymin,
	 fbb->d_xmin, fbb->d_ymin, fbb->b_wd, fbb->b_ht, vdipb->intin[0]);
}

void vdi_vrt_cpyfm(VDI_Workstation *vwk)
{
  MFDB *src,*dst;
  FBBLTPBLK *fbb;
  RECT srccor, dstcor, tmpcor;
  int col_index;

  srccor.x1 = vdipb->ptsin[0];
  srccor.y1 = vdipb->ptsin[1];
  srccor.x2 = vdipb->ptsin[2];
  srccor.y2 = vdipb->ptsin[3];
  dstcor.x1 = vdipb->ptsin[4];
  dstcor.y1 = vdipb->ptsin[5];
  dstcor.x2 = vdipb->ptsin[6];
  dstcor.y2 = vdipb->ptsin[7];

  /* order the coordinates as I like them. */
  fix_rect(&srccor);
  fix_rect(&dstcor);

  /* Completely ignore what the user says that the destination 
     width and height should be and recalculate them here.
     They must be recalculated after the fix. */

  dstcor.x2 = srccor.x2-srccor.x1 + dstcor.x1;
  dstcor.y2 = srccor.y2-srccor.y1 + dstcor.y1;
  tmpcor = dstcor;

  src = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[7],vdipb->contrl[8]);
  dst = (MFDB *)USHORTS_TO_POINTER(vdipb->contrl[9],vdipb->contrl[10]);

  /* check first if clipping takes away everything,
     if destination is the screen */
  if(!dst->fd_addr && !do_rectclip(&dstcor, &vwk->clip))
    return;

  /* setup fbb according to the screen format */
  fbb = FBgetbltpblk(vwk->fb);

  /* emulate the four modes with different combinations
     of the ordinary 15 bitblt modes. */
  switch(vdipb->intin[0]) {
  case MD_TRANS:
    fbb->op_tab = (NOTS_AND_D << 24 |
		   NOTS_AND_D << 16 |
		   S_OR_D     <<  8 |
		   S_OR_D);
    break;

  case MD_XOR:
    fbb->op_tab = (S_XOR_D    << 24 |
		   S_XOR_D    << 16 |
		   S_XOR_D    <<  8 |
		   S_XOR_D);
    break;

  case MD_ERASE:
    fbb->op_tab = (S_AND_D    << 24 |
		   NOTS_OR_D  << 16 |
		   S_AND_D    <<  8 |
		   NOTS_OR_D);
    break;

  default: /* illegal mode specified, use replace mode */
  case MD_REPLACE:
    fbb->op_tab = (ALL_WHITE  << 24 |
		   NOT_S      << 16 |
		   S_ONLY     <<  8 |
		   ALL_BLACK);
    break;
  }

  /* foreground and background colour to put block in */
  col_index = gem2tos_color(vwk->inq.attr.planes, vdipb->intin[1]);
  fbb->fg_col = get_color(vwk, col_index);
  col_index = gem2tos_color(vwk->inq.attr.planes, vdipb->intin[2]);
  fbb->bg_col = get_color(vwk, col_index);

  /* dst size is the same as src size here, only that dst
     might have been clipped earlier */
  fbb->b_wd = dstcor.x2 - dstcor.x1 + 1;
  fbb->b_ht = dstcor.y2 - dstcor.y1 + 1;

  /* adjust coordinates if destination is clipped */
  fbb->s_xmin = srccor.x1 + dstcor.x1-tmpcor.x1;
  fbb->s_ymin = srccor.y1 + dstcor.y1-tmpcor.y1;
  if(src->fd_addr) {
    fbb->s_form = src->fd_addr;

    if(src->fd_stand) {
      /* block is in VDI format */

      fbb->s_nxwd = 2;
      fbb->s_nxln = src->fd_wdwidth * 2;
    } /* else block is in machine dependent format */

    /* See discussion about machine dependent in vro_cpyfm above */

  } /* else source is the screen, i.e. already setup by FBgetbltpblk() */

  /* use only the first bitplane from source block */
  fbb->s_nxpl = 0;

  fbb->d_xmin = dstcor.x1;
  fbb->d_ymin = dstcor.y1;
  if(dst->fd_addr) {
    fbb->d_form = dst->fd_addr;
    fbb->plane_ct = dst->fd_nplanes; /* use destination no of bitplanes! */

    if(dst->fd_stand) {
      /* destination should be in VDI format */

      fbb->d_nxwd = 2;
      fbb->d_nxln = dst->fd_wdwidth * 2;
      fbb->d_nxpl = dst->fd_wdwidth * 2 * dst->fd_h;
    } /* else destination should be in machine dependent format */

    /* See discussion about machine dependent in vro_cpyfm above */

  } /* else destination is the screen, i.e. already setup by FBgetbltpblk() */

  FBbitblt(vwk->fb, fbb);

  FBfreebltpblk(fbb);

  IDEBUG("vrt_cpyfm: %d,%d -> %d,%d  w: %d h: %d mode: %d\n",fbb->s_xmin, fbb->s_ymin,
	 fbb->d_xmin, fbb->d_ymin, fbb->b_wd, fbb->b_ht, vdipb->intin[0]);
}

void vdi_vr_trnfm(VDI_Workstation *vwk)
{
  EDEBUG("vr_trnfm: Call not implemented!\n");
}

