# Make a shared library using libtool

ovdisisdir = @OSISINCLUDE@
ovdisis_HEADERS = vdibind.h

lib_LTLIBRARIES = libovdisis.la

libovdisis_la_SOURCES = \
	vdi_call.c vdi_characters.c vdi_control.c vdi_various.c vdi_fill.c \
	vdi_gdps.c vdi_keyboard.c vdi_line.c vdi_marker.c vdi_mouse.c \
	vdi_raster.c vdi_text.c various.c ovdi_more.c inits.c event.c \
	system_6x6.c system_8x8.c system_8x16.c vdibind.c \
	defcmap.h event.h fonts.h inits.h ovdi_more.h ovdisis.h various.h \
	vdi_characters.h vdi_control.h vdi_fill.h vdi_gdps.h vdi_keyboard.h \
	vdi_line.h \
	vdi_marker.h vdi_mouse.h vdi_raster.h vdi_text.h vdi_various.h

libovdisis_la_LDFLAGS = -version-info 0:1:0
