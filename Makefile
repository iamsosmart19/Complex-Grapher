#C
CC = gcc
CFLAGS = -O3 -Wextra -Wall `pkg-config --cflags gtk+-3.0 epoxy gio-2.0` -g -Wno-unused-variable -Wno-unused-parameter 

BISON=bison
FLEX=flex
XSLTPROC=xsltproc

all: cplxgraph

.PHONY: clean

LIBS=-lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lOpenCL -lSDL2 -lSDL2_ttf `pkg-config --libs gtk+-3.0 epoxy gio-2.0`

BDIR=bin

DDIR=dbg

#Dependencies
IDIR=src
_DEPS = strstack.h stack.h queue.h scan.h parse.h main.h parsetest.h stb_image_write.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Object files
ODIR=obj
# _OBJ = stack.o queue.o scan.o parse.o eval.o parsetest.o 
_OBJ = strstack.o stack.o queue.o scan.o parse.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

GLIB_RESOURCES = glib-resources.h

bundle: src/glib-resources.xml src/glfiles/fragShad.frag src/glfiles/vertShad.vert
	glib-compile-resources --sourcedir=./src/ --target=./src/glib-resources.h --generate-source ./src/glib-resources.xml

parse: $(IDIR)/parse.y
		$(BISON) $(BISONFLAGS) --defines -o $(IDIR)/parse.c $< -v --report-file=$(DDIR)/parse.out

scan: $(IDIR)/scan.l
		$(FLEX) $(FLEXFLAGS) -o $(IDIR)/scan.c --header-file=$(IDIR)/scan.h $<

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
		@mkdir -p $(ODIR)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

cplxgraph: bundle parse scan $(OBJ)
		@mkdir -p $(BDIR)
		$(CC) $(LIBS) -o $(BDIR)/$@ $(OBJ) $(IDIR)/glad/glad.c $(CFLAGS) $(LIBS)

run: cplxgraph
		cd bin; ./cplxgraph

clean:
		rm -rf $(ODIR)/*.o
		rm -rf $(BDIR)/cplxgraph
