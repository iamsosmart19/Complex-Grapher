#C
CC = gcc
CFLAGS = -I

.PHONY: clean

LIBS=-lm

BDIR=bin

#Dependencies
IDIR=src
_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: $(OBJ)
		@mkdir -p $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(ODIR)/*.o
