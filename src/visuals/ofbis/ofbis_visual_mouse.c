/*
** ofbis_visual_mouse.c
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include <ofbis.h>

#include "ofbis_visual.h"

static int x_last = 0;
static int y_last = 0;
/* Used when saving / restoring mouse background */

static unsigned short mouse_background_buffer[16*16*2];


/*
** Description
** Save background before drawing mouse
*/
void
ofbis_visual_save_mouse_bg (void * fb,
                            int    x,
                            int    y) {
  FBBLTPBLK * mouse_background;
  
  /* Setup mouse background blit */
  mouse_background = FBgetbltpblk (FB_T(fb));

  mouse_background->s_xmin = x;
  mouse_background->s_ymin = y;
  mouse_background->b_wd = 16;
  mouse_background->b_ht = 16;
  mouse_background->d_form = mouse_background_buffer;
  mouse_background->d_nxln = 16; /* FIXME for current resolution */

  FBbitblt (FB_T(fb), mouse_background);

  x_last = x;
  y_last = y;
}


/*
** Description
** Restore mouse background
*/
void
ofbis_visual_restore_mouse_bg (void * fb) {
  FBBLTPBLK * mouse_background;
  
  /* Setup mouse background blit */
  mouse_background = FBgetbltpblk (FB_T(fb));
  
  mouse_background->d_xmin = x_last;
  mouse_background->d_ymin = y_last;
  mouse_background->b_wd = 16;
  mouse_background->b_ht = 16;
  mouse_background->s_form = mouse_background_buffer;
  mouse_background->s_nxln = 16; /* FIXME for current resolution */

  FBbitblt (FB_T(fb), mouse_background);
}
