/*
** ofbis_visual_event.c
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

#include <ofbis.h>
#include <ofbis_keyboard.h>
#include <stdio.h>

#include "ofbis_visual.h"
#include "ovdisis.h"
#include "ovdisis_keyboard.h"

/*
** Description
** Map buttons in vdi format
*/
static
unsigned int
map_buttons (unsigned int ofbis_buttons) {
  /*
  ** FIXME: I'm not sure if the mapping is correct for the middle and the right
  ** buttons.
  */
  return (((ofbis_buttons & 0x4) ? 0 : 1) |
          ((ofbis_buttons & 0x2) ? 0 : 2) |
          ((ofbis_buttons & 0x1) ? 0 : 4));
}


/*
** Map pressed key from oFBis to VDI representation
*/
static
inline
void
map_key (FBKEYEVENT *            fe,
         Visual_Key_Event_Type * ve)
{
  ve->state = fe->state;

  switch(fe->ascii)
  {
  case FB_UC_Linefeed:
    ve->keycode = fe->keycode & 0x7f;
    ve->ascii   = VDI_KVAL(VDI_K_Return);
    break;

  case FB_UC_Delete:
    ve->keycode = fe->keycode & 0x7f;
    ve->ascii   = VDI_KVAL(VDI_K_BackSpace);
    break;

  default:
    ve->keycode = fe->keycode & 0x7f;
    ve->ascii = fe->ascii;
  }
}


void
ofbis_visual_get_event (void *         fb,
                        Visual_Event * visual_event)
{
  static int new_button_event = FALSE;
  static int old_buttons = 0;
  static int old_state = 0;
  FBEVENT    fe;

  if(new_button_event)
  {
    /* A button has been pressed or released */
    visual_event->type = Visual_Mouse_Button_Event;
    visual_event->mouse_button.state = old_state;
    visual_event->mouse_button.buttons = map_buttons(old_buttons);
    new_button_event = FALSE;
  }
  else
  {
    FBgetevent (FB_T(fb), &fe);
    
    switch (fe.type) {
    case FBNoEvent :
      visual_event->type = Visual_No_Event;
      break;
      
    case FBKeyEvent :
      visual_event->type = (fe.key.keycode & 0x80) ?
        Visual_Key_Press_Event : Visual_Key_Release_Event;
      map_key(&fe.key, &visual_event->key);
      break;
      
    case FBMouseEvent :
      if(fe.mouse.x || fe.mouse.y)
      {
        static int old_x       = 0;
        static int old_y       = 0;
        
        /* The mouse has been moved */
        old_x += fe.mouse.x;
        if(old_x < 0)
        {
          old_x = 0;
        }
        else if(old_x >= FB_T(fb)->vinf.xres)
        {
          old_x = FB_T(fb)->vinf.xres - 1;
        }
        
        old_y += fe.mouse.y;
        if(old_y < 0)
        {
          old_y = 0;
        }
        else if(old_y >= FB_T(fb)->vinf.yres)
        {
          old_y = FB_T(fb)->vinf.yres - 1;
        }
        
        visual_event->type = Visual_Mouse_Move_Event;
        visual_event->mouse_move.x = old_x;
        visual_event->mouse_move.y = old_y;
        
        if(old_buttons != fe.mouse.state)
        {
          old_buttons = fe.mouse.buttons;
          old_state = fe.mouse.state;
          new_button_event = TRUE;
        }
      }
      else if(old_buttons != fe.mouse.buttons)
      {
        /* A button has been pressed or released */
        visual_event->type = Visual_Mouse_Button_Event;
        visual_event->mouse_button.state = fe.mouse.state;
        visual_event->mouse_button.buttons = map_buttons(fe.mouse.buttons);
        
        old_buttons = fe.mouse.buttons;
      }
      break;
      
    default :
      ;
    }
  }
}
