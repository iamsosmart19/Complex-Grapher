#!/bin/bash
bison -d *.y
flex *.l
gcc lex.yy.c lexer.tab.c -lfl -o lex
