BINDINGS_SRC = vdibind.c
TRAP_SRC = vdi_call_mint.c

CSRCS = $(BINDINGS_SRC) $(TRAP_SRC)

OBJS = $(CSRCS:.c=.o)

all: libovdisis.a vdibind.h

libovdisis.a: $(OBJS)
	$(AR) cru $@ $(OBJS)
	-$(RANLIB) $@

vdibind.h: vdibind.h.in
	cat $< | sed s/@S_INT16_T@/signed\ short/g | \
                 sed s/@U_INT16_T@/unsigned\ short/g > vdibind.h

include ../mincl.mint
