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

#include <stdio.h>

#include "ovdisis.h"

void *
ggi_visual_open (void) {
  fprintf(stderr, "Implement ggi_visual_open\n");
  return NULL;
}

void
ggi_visual_close (void * fb) {
  fprintf(stderr, "Implement ggi_visual_close\n");
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

