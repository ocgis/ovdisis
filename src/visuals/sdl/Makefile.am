SDL_VISUAL_SOURCES = \
	sdl_visual.h \
	sdl_visual_blit.c sdl_visual_blit.h \
	sdl_visual_cmap.c sdl_visual_cmap.h \
	sdl_visual_control.c sdl_visual_control.h \
	sdl_visual_event.c sdl_visual_event.h \
	sdl_visual_init.c sdl_visual_init.h \
	sdl_visual_mouse.c sdl_visual_mouse.h \
	sdl_visual_mutex.c sdl_visual_mutex.h \
	sdl_visual_text.c sdl_visual_text.h \
	sdl_visual_various.c sdl_visual_various.h

INCLUDES = -I../..
LIBS = 

visualsdir = @VISUALSDIR@

visuals_LTLIBRARIES = libovdisis_sdl.la

libovdisis_sdl_la_SOURCES = $(SDL_VISUAL_SOURCES)
libovdisis_sdl_la_LDFLAGS = -version-info 0:1:0
libovdisis_sdl_la_LIBADD = -lSDL

