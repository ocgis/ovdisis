/*
 * otestis.c
 *
 * Copyright 2003 Vincent Barrilliot <vbarrilliot@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

/* This program tests the workstation management */

#include <stdio.h>
#include <stdlib.h>
#include "vdibind.h"


int main() {
  int physical_wk;
  int work_in[11] = { 1,1,1,1,1,1,1,1,1,1,2 };
  int work_out[57];

  v_opnwk(work_in, &physical_wk, work_out);
  
  /* Draw mouse cursor */
  printf("Drawing mouse cursor\n");
  v_dspcur(physical_wk,50,50);

  /* Show mouse cursor */
#if 1
  printf("Showing mouse cursor\n");
  v_show_c(physical_wk,1);
#endif  

  /* Hide mouse curosr */
  printf("Hide mouse cursor: Press RETURN to continue.\n");
  fgetc(stdin);
  v_hide_c(physical_wk);

  printf("Closing the physical workstation and exiting: Press RETURN to continue\n");
  getc(stdin);
  v_clswk(physical_wk);
  return 0;
}
