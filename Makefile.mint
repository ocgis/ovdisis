SUBDIRS = src

all: subdirs otestis

CSRCS = otestis.c
OBJS = $(CSRCS:.c=.o)

LDLIBS = src/libovdisis.a

include mincl.mint
