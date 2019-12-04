# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = reccalc
BISON = bison
FLEX = flex
XSLTPROC = xsltproc

LIBS=-lm

all: $(BASE)

%.c %.h %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --defines --xml --graph=$*.gv -o $*.c $<

%.c %.h: %.l
	$(FLEX) $(FLEXFLAGS) -o$*.c --header-file=$*.h $<

scan.o: scan.c parse.h
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)
parse.o: parse.c scan.h 
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)
$(BASE): parse.o scan.o
	$(CC) $(LIBS) $(CFLAGS) -o $@ $^

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$<

html: parse.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =						\
  $(BASE) *.o						\
  parse.[ch] parse.output parse.xml parse.html parse.gv	\
  scan.[ch]
clean:
	rm -f $(CLEANFILES)
