SRCS = vdi_dump.c
OBJS = vdi_dump.o
EXEC = vdi_dump
SUBDIROBJS = ../vdibind/vdibind.o ../src/ovdisis.a
LDLIBS = -lfb

PREREQ :=
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif

CFLAGS = $(COPTIONS) -I../vdibind -I..

all:	$(PREREQ) $(EXEC)

$(EXEC): force $(OBJS)
	$(CC) -o $@ $(OBJS) $(SUBDIROBJS) $(LDLIBS)


dep depend .depend: 
	$(CC) $(CFLAGS) -MM $(SRCS) > .depend

clean:
	rm -f core *~ $(OBJS) .depend

realclean: clean
	rm -f $(EXEC)

force:

ifeq (.depend,$(wildcard .depend))
include .depend
endif







