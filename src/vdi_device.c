/*
 * vdi_device.c
 *
 * Copyright 1999 Tomas Berndtsson <tomas@nocrew.org>
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
#include <pthread.h>

#include "ovdisis.h"
#include "vdi_device.h"

#undef DEBUGLEVEL

/* defined in event.c */
extern pthread_cond_t key_cond;
extern pthread_mutex_t key_mutex;
extern int *key_scancode_buffer, *key_ascii_buffer;
extern int key_first_index, key_amount, key_buffer_length;


void vdi_vsm_string(VDI_Workstation *vwk)
{
  /* NOTE: this is also vrq_string since it has the same binding as vsm_string
   * The behavior (vrq or vsm) depends on the current mode for the device
   * (request or sample). This is what NVDI does anyways...
   */

  int maxlen, amount, i, getboth;
  int got;
  WORD value;

  /* There is echo info in there too, but we don't support that yet, 
   * if ever.
   */

  maxlen = (int)vdipb->intin[0];
  if(maxlen < 0) {
    maxlen = -maxlen;
    getboth = 1;
  } else {
    getboth = 0;
  }

  pthread_mutex_lock(&key_mutex);

  if( vwk->device_mode[STRING] == REQUEST_MODE ) {
    got = 0;
    while( got != maxlen ) {
      if( !key_amount )
	pthread_cond_wait(&key_cond, &key_mutex);

      amount = min( NR_INTOUT, maxlen); /* how many chars can we store ? */
      amount = min( amount, key_amount);

      for(i = 0 ; i < amount ; i++) {
	value = (WORD)key_ascii_buffer[key_first_index] & 0xff;

	if( value == 0xd ) { /* Returns stops the input */
	  if( ++key_first_index >= key_buffer_length )
	    key_first_index = 0;
	  key_amount -= amount;
	  goto end;
	}
	
	if( getboth )
	  value |= (WORD)(key_scancode_buffer[key_first_index] << 8);
	
	vdipb->intout[got++] = value;

	if( ++key_first_index >= key_buffer_length )
	  key_first_index = 0;
      }
      key_amount -= amount;
    } 
  }
  else {
    /* mode == SAMPLE_MODE */
    amount = min(NR_INTOUT, min(maxlen, key_amount));
    
    for(i = 0 ; i < amount ; i++) {
      value = (WORD)key_ascii_buffer[key_first_index] & 0xff;	
      if( getboth )
	value |= (WORD)(key_scancode_buffer[key_first_index] << 8);
      
      vdipb->intout[i] = value;
      
      if( ++key_first_index >= key_buffer_length )
	key_first_index = 0;
    }
    key_amount -= amount;
    got = amount;
  } 
  
 end:
  pthread_mutex_unlock(&key_mutex);
  vdipb->contrl[N_INTOUT] = got;
}

/* 
 * Also in this file, later:
 * vsm_choice
 * vsm_locator
 * vsm_valuator
 */

void vdi_vsin_mode(VDI_Workstation *vwk)
{
  int device, mode;

  device = (int)vdipb->intin[0];
  mode = (int)vdipb->intin[1];

  /* NVDI does that */
  if( mode != SAMPLE_MODE )
    mode = REQUEST_MODE;
    
  if(device >= 1 && device <= 4)
    vwk->device_mode[device] = mode;

  vdipb->intout[0] = (WORD)mode;
  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}
