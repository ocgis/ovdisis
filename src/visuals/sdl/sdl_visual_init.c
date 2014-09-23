/*
** ggi_visual_init.c
**
** Copyright 1999 - 2000 Christer Gustavsson <cg@nocrew.org>
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
#include "sdl_visual_blit.h"
#include "sdl_visual_cmap.h"
#include "sdl_visual_control.h"
#include "sdl_visual_event.h"
#include "sdl_visual_mouse.h"
#include "sdl_visual_mutex.h"
#include "sdl_visual_text.h"
#include "sdl_visual_various.h"

VDI_Visual * init(void);

VDI_Visual *
init(void)
{
  static VDI_Visual visual =
  {
    sdl_visual_open,
    sdl_visual_close,
    sdl_visual_clear,
    sdl_visual_inquire,
    sdl_visual_set_write_mode,
    sdl_visual_free_cmap,
    sdl_visual_set_cmap,
    sdl_visual_get_cmap,
    sdl_visual_put_cmap,
    sdl_visual_get_pixel,
    sdl_visual_put_pixel,
    sdl_visual_put_pixels,
    sdl_visual_hline,
    sdl_visual_line,
    sdl_visual_bitblt,
    sdl_visual_bitbltt,
    sdl_visual_put_char,
    sdl_visual_set_font,
    sdl_visual_save_mouse_bg,
    sdl_visual_restore_mouse_bg,
    sdl_visual_get_event,
    sdl_visual_mutex
  };

  return &visual;
}
