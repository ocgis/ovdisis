/*
 * otestis.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
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
  int virtual_wk1;
  int work_in[11] = { 1,1,1,1,1,1,1,1,1,1,2 };
  int work_out[57];


  /* Open physical workstation */
  printf("--- Physical workstation ----------\n");
  printf("Opening physical workstation...");
  v_opnwk(work_in, &physical_wk, work_out);
  if(!physical_wk) {
    printf("Failed\n");
    return 0;
  }
  printf(" OK, got handle %d\n",physical_wk);


  /* Close physical workstation */
  printf("Now going to close that physical workstation : Press a RETURN to continue");
  getc(stdin);
  
  printf("Closing physical workstation...");
  v_clswk(physical_wk);
  /* If it fails for some reason, the program will exit because of the exception */
  printf("OK\n");


  /* Open virtual workstation */
  printf("\n--- Virtual workstation ----------\n");
  printf("We are going to open the physical workstation again and create a virtual workstation\n");
  printf("Press RETURN to continue");
  getc(stdin);
  v_opnwk(work_in, &physical_wk, work_out);
  printf("Opening virtual workstation...");
  virtual_wk1 = physical_wk; /* That's the way we tell the VDI from which physical we want a virtual wk */
  v_opnvwk(work_in, &virtual_wk1, work_out);
  if(!virtual_wk1) {
    printf("Failed\n");
    printf("Closing the physical workstatoin and exiting.\n");
    v_clswk(physical_wk);
    return 0;
  }
  printf("OK, got handle %d\n",virtual_wk1);

  /* Close virtual workstation */
  printf("Now going to close that physical workstation : Press RETURN to continue\n");
  getc(stdin);
  
  printf("Closing virtual workstation...");
  v_clsvwk(virtual_wk1);
  /* If it fails for some reason, the program will exit because of the exception */
  printf("OK\n");
  printf("Now closing the physical workstation and exiting\n");
  v_clswk(physical_wk);
  
  return 0;
}
