OFBIS_VISUAL_SOURCES = \
	ofbis_visual.h \
	ofbis_visual_cmap.c ofbis_visual_cmap.h \
	ofbis_visual_control.c ofbis_visual_control.h \
	ofbis_visual_event.c ofbis_visual_event.h \
	ofbis_visual_init.c \
	ofbis_visual_mouse.c ofbis_visual_mouse.h \
	ofbis_visual_various.c ofbis_visual_various.h

EXTRA_DIST = $(OFBIS_VISUAL_SOURCES)

if BUILD_OFBIS_VISUAL

INCLUDES = -I../..

visualsdir = @VISUALSDIR@

visuals_LTLIBRARIES = libovdisis_ofbis.la

libovdisis_ofbis_la_SOURCES = $(OFBIS_VISUAL_SOURCES)

endif

libovdisis_ofbis_la_LDFLAGS = -version-info 0:1:0
