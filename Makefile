CC = gcc
COPTIONS = -O2 -Wall -pipe
LDLIBS = -lfb -lm
AR = ar
DO_MAKE = $(MAKE) 'CC=$(CC)' 'COPTIONS=$(COPTIONS) -DDEBUGLEVEL=2' 'AR=$(AR)'
EXEC = otestis

SUBDIRS = src vdibind vdi_dump
SUBDIROBJS = src/ovdisis.a vdibind/vdibind.o 

OBJS = otestis.o
SRCS = otestis.c

PREREQ :=
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif

CFLAGS = $(COPTIONS) -I. -Isrc -Ivdibind


all: $(PREREQ) $(EXEC)

$(EXEC): subdirs $(OBJS)
	$(CC) -o $@ $(OBJS) $(SUBDIROBJS) $(LDLIBS)

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
	rm -f $(EXEC)

tar:
	cd ..; tar cvzf $(notdir $(shell pwd))-`date "+%Y%m%d"`.tar.gz \
		`find $(notdir $(shell pwd)) -type f -a ! -name '*.[oa]' \
		-a ! -name .depend -a ! -name '*~' -a ! -name '*.orig' \
		-a ! -perm +a=x | grep -v '/CVS/'`


force:

ifeq (.depend,$(wildcard .depend))
include .depend
endif
