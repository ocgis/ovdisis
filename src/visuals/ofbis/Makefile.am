OFBIS_VISUAL_SOURCES = \
	ofbis_visual.h \
	ofbis_visual_cmap.c ofbis_visual_cmap.h \
	ofbis_visual_control.c ofbis_visual_control.h \
	ofbis_visual_event.c ofbis_visual_event.h \
	ofbis_visual_init.c ofbis_visual_init.h \
	ofbis_visual_mouse.c ofbis_visual_mouse.h \
	ofbis_visual_mutex.c ofbis_visual_mutex.h \
	ofbis_visual_various.c ofbis_visual_various.h

INCLUDES = -I../.. @OFBIS_CFLAGS@
LIBS = 

visualsdir = @VISUALSDIR@

visuals_LTLIBRARIES = libovdisis_ofbis.la

libovdisis_ofbis_la_SOURCES = $(OFBIS_VISUAL_SOURCES)
libovdisis_ofbis_la_LDFLAGS = -version-info 0:1:0
libovdisis_ofbis_la_LIBADD = @OFBIS_LIBS@
