SUBDIRS = visuals

# Make a shared library using libtool

DEFS = @DEFS@ -DVISUALSDIR=\"@VISUALSDIR@\"

target_os = @target_os@
ovdisisdir = @OSISINCLUDE@
ovdisis_HEADERS = vdibind.h

BINDINGS_SRC = vdibind.c
TRAP_SRC = vdi_call_mint.c

DRIVER_SRC = \
	vdi_call.c vdi_characters.c vdi_control.c vdi_various.c vdi_fill.c \
	vdi_gdps.c vdi_line.c vdi_marker.c vdi_mouse.c \
	vdi_raster.c vdi_text.c vdi_device.c \
	various.c inits.c event.c \
	system_6x6.c system_8x8.c system_8x16.c \
	defcmap.h event.h fonts.h inits.h ovdisis.h various.h \
	vdi_characters.h vdi_control.h vdi_fill.h vdi_gdps.h \
	vdi_line.h vdi_marker.h vdi_mouse.h vdi_raster.h vdi_text.h \
	vdi_device.h vdi_various.h

EXTRA_DIST = $(BINDINGS_SRC) $(TRAP_SRC) $(DRIVER_SRC)

if BUILD_BINDINGS_ONLY
lib_LIBRARIES = libovdisis.a

libovdisis_a_SOURCES = \
	$(BINDINGS_SRC) $(TRAP_SRC)
else
lib_LTLIBRARIES = libovdisis.la

libovdisis_la_SOURCES = \
	$(BINDINGS_SRC) $(DRIVER_SRC)

endif

libovdisis_la_LDFLAGS = @LT_VERSION_INFO@
