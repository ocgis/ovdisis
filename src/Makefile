DO_MAKE = $(MAKE) 'CC=$(CC)' 'COPTIONS=$(COPTIONS)' 'AR=$(AR)'

SRCS = vdi_call.c vdi_characters.c vdi_control.c vdi_various.c vdi_fill.c vdi_gdps.c vdi_line.c vdi_marker.c vdi_mouse.c vdi_raster.c vdi_text.c various.c ovdi_more.c inits.c
OBJS = vdi_call.o vdi_characters.o vdi_control.o vdi_various.o vdi_fill.o vdi_gdps.o vdi_line.o vdi_marker.o vdi_mouse.o vdi_raster.o vdi_text.o various.o ovdi_more.o inits.o

SUBDIRS = fonts
SUBDIROBJS = fonts/fonts.o

PREREQ :=
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif

CFLAGS = $(COPTIONS) -I.. -Ifonts

all:	$(PREREQ) ovdisis.a

ovdisis.a: $(OBJS) subdirs
	rm -f $@
	$(AR) rcs $@ $(OBJS) $(SUBDIROBJS)

subdirs: force
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir; \
	done

dep depend .depend: 
	$(CC) $(CFLAGS) -MM $(SRCS) > .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir dep; \
	done

clean:
	rm -f core *~ $(OBJS) .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir clean; \
	done

realclean:
	rm -f core *~ $(OBJS) .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir realclean; \
	done
	rm -f ovdisis.a

force:


ifeq (.depend,$(wildcard .depend))
include .depend
endif







