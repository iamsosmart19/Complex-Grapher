%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "lexer.tab.h"
	extern int yylex();
%}
%option noyywrap

/* Regex */
%%
[0-9]+\.[0-9]+	{ yylval.fval = atof(yytext); return FLOAT; }
[0-9]+			{ yylval.ival = atoi(yytext); return INT; }
\+				{ return ADD; }
-				{ return SUB;}
\*				{ return MUL; }
\/				{ return DIV; }

log				{ ; }
ln				{ ; }

\^				{ return EXP; }

arcsin			{ return ASIN; }
arccos			{ return ACOS; }
arctan			{ return ATAN; }
sinh			{ return SINH; }
cosh			{ return COSH; }
tanh			{ return TANH; }
sech			{ return SECH; }
csch			{ return CSCH; }
coth			{ return COTH; }
sin				{ return SIN; }
cos				{ return COS; }
tan				{ return TAN; }
sec				{ return SEC; }
csc				{ return CSC; }
cot				{ return COT; }

phi				{ ; }
pi				{ ; }
e				{ ; }

[\n\r]			{ return ENDL; }
.				{ ; }
%%

#ifdef DEBUG_LEX
YYSTYPE yylval;

int main(void)
{
    int token;
    while ((token = yylex()) != 0)
        printf("Token: %d (%s)\n", token, yytext);
    return 0;
}
#endif
