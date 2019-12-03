#!/bin/bash
bison -v -d *.y
flex *.l
gcc -v lex.yy.c parser.tab.c -lfl -o lex
#./lex
