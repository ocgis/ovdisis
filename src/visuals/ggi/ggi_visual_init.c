/*
** ggi_visual_init.c
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
#include "ggi_visual_cmap.h"
#include "ggi_visual_control.h"
#include "ggi_visual_event.h"
#include "ggi_visual_mouse.h"
#include "ggi_visual_mutex.h"
#include "ggi_visual_various.h"

VDI_Visual *
init (void)
{
  static VDI_Visual visual = {
    ggi_visual_open,
    ggi_visual_close,
    ggi_visual_clear,
    ggi_visual_inquire,
    ggi_visual_set_write_mode,
    ggi_visual_free_cmap,
    ggi_visual_set_cmap,
    ggi_visual_get_cmap,
    ggi_visual_put_cmap,
    ggi_visual_get_pixel,
    ggi_visual_put_pixel,
    ggi_visual_hline,
    ggi_visual_line,
    ggi_visual_bitblt,
    ggi_visual_bitbltt,
    ggi_visual_put_char,
    ggi_visual_set_font,
    ggi_visual_save_mouse_bg,
    ggi_visual_restore_mouse_bg,
    ggi_visual_get_event,
    ggi_visual_mutex
  };

  return &visual;
}
