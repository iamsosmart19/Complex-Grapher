#C
CC = gcc
CFLAGS = -O3 -Wincompatible-pointer-types

BISON=bison
FLEX=flex
XSLTPROC=xsltproc

all: cplxgraph

.PHONY: clean

LIBS=-lm

BDIR=bin

#Dependencies
IDIR=src
_DEPS = stack.h queue.h scan.h parse.h eval.h main.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
_OBJ = stack.o queue.o scan.o parse.o eval.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

parse: $(IDIR)/parse.y
	$(BISON) $(BISONFLAGS) --defines -o $(IDIR)/$*.c $<

scan: $(IDIR)/scan.l
	$(FLEX) $(FLEXFLAGS) -o $(IDIR)/$*.c --header-file=$(IDIR)/$*.h $<

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: parse scan $(OBJ)
		@mkdir -p $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS)

run: cplxgraph
	./bin/cplxgraph

clean:
		rm -rf $(ODIR)/*.o
