SRCS = vdibind.c
OBJS = vdibind.o

all: $(OBJS)

dep depend .depend: 
	$(CC) $(CFLAGS) -MM $(SRCS) > .depend

clean:
	rm -f core *~ .depend

realclean: clean
	rm -f $(OBJS)

ifeq (.depend,$(wildcard .depend))
include .depend
endif







