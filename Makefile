CC = gcc
CFLAGS = -O2 -Wall -pipe
LDLIBS = -lfb
AR = ar
DO_MAKE = $(MAKE) 'CC=$(CC)' 'CFLAGS=$(CFLAGS) -DDEBUGLEVEL=2' 'AR=$(AR)'
EXEC = otestis

SUBDIRS = src vdibind vdi_dump
SUBDIROBJS = src/ovdisis.a vdibind/vdibind.o 

OBJS = otestis.o
SRCS = otestis.c

PREREQ :=
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif

# I've had some problems with FBgetchar() together with
# vs_color. It seems to work with linux 2.0.25, but not
# with 2.0.21. If you have 2.0.21, uncomment the next
# line, or the program might hang.
#CFLAGS += -DNOKEYCHECK


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
