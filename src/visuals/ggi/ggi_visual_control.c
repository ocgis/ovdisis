/*
** ggi_visual_control.c
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

#include <ggi/ggi.h>
#include <stdio.h>
#include <stdlib.h>

#include "ggi_visual.h"
#include "ggi_visual_control.h"
#include "ovdisis.h"


void *
ggi_visual_open (void) {
  ggi_visual_t vis;
  int          err;

  ggiInit();

  vis = ggiOpen(NULL);
  
  err = ggiSetGraphMode(vis,GGI_AUTO,GGI_AUTO,GGI_AUTO,GGI_AUTO,GT_AUTO);

  if(err == 0)
  {
    private_t * private;

    private = malloc(sizeof(private_t));
    private->vis = vis;

    return (void *)private;
  }
  else
  {
    ggiClose(vis);
    ggiExit();

    return NULL;
  }
}


void
ggi_visual_close (void * private) {
  ggiClose(VISUAL_T(private));
  ggiExit();
  free(private);
}


void
ggi_visual_clear (VWKREF vwk)
{
  ggiSetGCForeground(VISUAL_T(vwk->visual->private),
                     COLOR_MAPPED(vwk->visual->private)[0]);
  ggiDrawBox(VISUAL_T(vwk->visual->private),
             0,
             0,
             vwk->dev.attr.xres - 1,
             vwk->dev.attr.yres - 1);
}


void
ggi_visual_set_write_mode (void * vis,
			   int    write_mode) {
  WRITE_MODE(vis) = write_mode;
}
