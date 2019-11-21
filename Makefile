#C
CC = gcc
CFLAGS = -O3

.PHONY: clean

LIBS=-lm

BDIR=bin

#Dependencies
IDIR=src
_DEPS = asciimath/parser.h stack.h queue.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
_OBJ = asciimath/parser.o stack.o queue.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		@mkdir -p $(ODIR)/asciimath
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: $(OBJ)
		@mkdir -p $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(ODIR)/*.o
