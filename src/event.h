/*
** event.h
**
** Copyright 1998 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#ifndef _EVENT_H_
#define _EVENT_H_

#include "ovdisis.h"

/*
** Description
** Initialize the event handler and startup a thread to handle mouse,
** keyboard and timer events.
**
** 1998-10-13 CG
** 1998-12-07 CG
*/
void
init_event_handler (VDI_Workstation * vwk);

/*
** Description
** Kill event handler loop
**
** 1998-10-13 CG
** 1998-12-07 CG
*/
void
exit_event_handler (void);

#endif /* _EVENT_H_ */

/*
** Revision history
**
** 1998-10-13 CG
**            Created original
**
** 1998-10-14 CG
**            Added prototype for exit_event_handler
*/

