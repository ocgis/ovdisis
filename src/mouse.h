/*
** mouse.h
**
** Copyright 1998 - 2000 Christer Gustavsson <cg@nocrew.org>
** Copyright 2003        Vincent Barrilliot <vincent_barrilliot@yahoo.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/


#ifndef _MOUSE_H_
#define _MOUSE_H_

extern MFORM mouse_cursor;

int start_mouse_manager( VDI_Workstation * vwk );
void terminate_mouse_manager();

void set_mouse_position( int x,  int y );
void get_mouse_position( short *x, short *y );

void do_draw_mouse(); 
void set_mouse_drawer(void (*curv)() );
void just_draw_mouse_cursor(int x, int y);

void set_mouse_form(VDI_Workstation *vwk, MFORM * cursor);
void set_mouse_buttons( unsigned int new_status );
void get_mouse_buttons( unsigned int *status );
void mouse_moved( unsigned int x, unsigned int y ); 

/*
** Description
** Increase mouse cursor visibility one level
**
** 1999-01-03 CG
*/
void increase_mouse_visibility ();

/*
** Description
** Decrease mouse cursor visibility one level
**
** 1999-01-03 CG
*/
void decrease_mouse_visibility ();

#endif
