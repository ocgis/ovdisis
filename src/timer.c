/*
** timer.c
**
** Copyright 1998 - 2000 Christer Gustavsson <cg@nocrew.org>
** Copyright 2003        Vincent Barrilliot (vincent_barrilliot@yahoo.com)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/


#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

#include "ovdisis.h"


/* Define to use signal instead of sigaction */
/* FIXME: make it an option in the configure script */
#define USE_SIGNAL 0


/* This is needed for the timer handler */
static VDI_Workstation *global_vwk;

/* This is used to generate the 20ms VDI timer tics */
static struct itimerval timer_value = {{0, 50000}, {0, 50000}};
static struct itimerval old_timer_value;

#ifndef USE_SIGNAL
/* Used for timer handler */
static struct sigaction old_sa;
#endif


/*
** Description
** For each timer tick (20 ms) this routine is called and will call a
** callback routine that the user has setup.
*/
static void timer_handler (int signal_number) {
  if (global_vwk->timv != NULL)
    global_vwk->timv ();

#ifdef USE_SIGNAL
  signal(SIGALRM, &timer_handler);
#endif
}


/*
** Description
** Initialize the event handler and startup a thread to handle mouse,
** keyboard and timer events.
*/
/* Install a timer handler */
int start_timer_manager(VDI_Workstation * vwk) {
  global_vwk = vwk;
#ifndef USE_SIGNAL
  struct sigaction sa;
  sa.sa_handler = &timer_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags   = SA_NODEFER;
  sa.sa_flags  &= ~SA_RESTART;
  sigaction(SIGALRM, &sa, &old_sa);
#else
  signal(SIGALRM, &timer_handler);
#endif  

  setitimer(ITIMER_REAL, &timer_value, &old_timer_value);
  return 1;
}


/*
** Description
** Kill timer
*/
void terminate_timer_manager (void) {
  /* We stop the reception of events before freeing the buffers */
  setitimer(ITIMER_REAL, &old_timer_value, NULL);

#ifdef USE_SIGNAL
  signal(SIGALRM, SIG_DFL);
#else
  sigaction(SIGALRM, &old_sa, &old_sa);
#endif

  DEBUG2("event.c: timer handler stopped\n");
}
