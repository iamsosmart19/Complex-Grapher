#C
CC = gcc
CFLAGS = -O3 -Wextra -Wall -fno-stack-protector

BISON=bison
FLEX=flex
XSLTPROC=xsltproc

all: cplxgraph

.PHONY: clean

LIBS=-lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lOpenCL

BDIR=bin

DDIR=dbg

#Dependencies
IDIR=src
_DEPS = stack.h queue.h scan.h parse.h eval.h main.h parsetest.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
# _OBJ = stack.o queue.o scan.o parse.o eval.o parsetest.o 
_OBJ = stack.o queue.o scan.o parse.o eval.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

parse: $(IDIR)/parse.y
	$(BISON) $(BISONFLAGS) --defines -o $(IDIR)/parse.c $< -v --report-file=$(DDIR)/parse.out

scan: $(IDIR)/scan.l
	$(FLEX) $(FLEXFLAGS) -o $(IDIR)/scan.c --header-file=$(IDIR)/scan.h $<

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: parse scan $(OBJ)
		@mkdir -p $(BDIR)
		@cp $(IDIR)/*.vert $(BDIR)
		@cp $(IDIR)/*.frag $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $(OBJ) $(IDIR)/glad/glad.c $(CFLAGS) $(LIBS)

run: cplxgraph
	cd bin; ./cplxgraph

clean:
		rm -rf $(ODIR)/*.o
