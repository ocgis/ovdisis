SRCS = system_6x6.c system_8x8.c system_8x16.c
OBJS = system_6x6.o system_8x8.o system_8x16.o

PREREQ :=
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif

all:	$(PREREQ) fonts.o

fonts.o: $(OBJS)
	$(CC) -nostdlib -Wl,-r -o $@ $^

fntread: fntread.c
	$(CC) -pipe -Wall -o $@ $^

dep depend .depend: 
	$(CC) $(CFLAGS) -MM $(SRCS) > .depend

clean:
	rm -f core *~ $(OBJS) .depend

realclean: clean
	rm -f fonts.o fntread

ifeq (.depend,$(wildcard .depend))
include .depend
endif







