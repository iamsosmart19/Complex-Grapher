#!/bin/bash
bison -v -d *.y
flex *.l
gcc lex.yy.c parser.tab.c -lfl -o lex
./lex
