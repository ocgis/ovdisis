GGI_VISUAL_SOURCES = \
	ggi_visual.h \
	ggi_visual_blit.c ggi_visual_blit.h \
	ggi_visual_cmap.c ggi_visual_cmap.h \
	ggi_visual_control.c ggi_visual_control.h \
	ggi_visual_event.c ggi_visual_event.h \
	ggi_visual_init.c ggi_visual_init.h \
	ggi_visual_mouse.c ggi_visual_mouse.h \
	ggi_visual_mutex.c ggi_visual_mutex.h \
	ggi_visual_text.c ggi_visual_text.h \
	ggi_visual_various.c ggi_visual_various.h

INCLUDES = -I../..
LIBS = 

visualsdir = @VISUALSDIR@

visuals_LTLIBRARIES = libovdisis_ggi.la

libovdisis_ggi_la_SOURCES = $(GGI_VISUAL_SOURCES)
libovdisis_ggi_la_LDFLAGS = -version-info 0:1:0
libovdisis_ggi_la_LIBADD = -lggi

