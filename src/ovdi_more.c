/*
 * ovdi_more.c
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
#include <unistd.h>

#include "ovdisis.h"

extern VDIWK wk[MAX_HANDLES];    /* declared in vdi_control.c */
extern int wk_open[MAX_HANDLES]; /* ----------- " ----------- */ 


int ovdi_getchar(int handle)
{
  if(wk_open[handle-1])
    return FBgetchar(wk[handle-1].physical->fb);

  return 0;
}
