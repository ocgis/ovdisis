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

#include "ggi_visual.h"
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
    return (void *)vis;
  }
  else
  {
    ggiClose(vis);
    ggiExit();

    return NULL;
  }
}

void
ggi_visual_close (void * vis) {
  ggiClose(VISUAL_T(vis));
  ggiExit();
}

void
ggi_visual_clear (VDI_Workstation * vwk)
{
  fprintf(stderr, "Implement ggi_visual_clear\n");
}


void
ggi_visual_set_write_mode (void * fb,
			   int    write_mode) {
  fprintf(stderr, "Implement ggi_visual_set_write_mode\n");
}

