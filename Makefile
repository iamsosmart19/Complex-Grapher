#C
CC = gcc
CFLAGS = -O3 -Wincompatible-pointer-types

BISON = bison
FLEX = flex
XSLTPROC = xsltproc

.PHONY: clean

LIBS=-lm

BDIR=bin

#Dependencies
IDIR=src
_DEPS = stack.h queue.h main.h scan.h parse.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
_OBJ = stack.o queue.o scan.o parse.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: $(OBJ)
		@mkdir -p $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

parse.c parse.h: $(IDIR)/parse.y
	$(BISON) $(BISONFLAGS) --defines -o $(IDIR)/$*.c $<

scan.c scan.h: $(IDIR)/scan.l
	$(FLEX) $(FLEXFLAGS) -o $(IDIR)/$*.c --header-file=$(IDIR)/$*.h $<

run: cplxgraph
	./bin/cplxgraph

clean:
		rm -rf $(ODIR)/*.o
