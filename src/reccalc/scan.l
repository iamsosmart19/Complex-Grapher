/* Prologue (directives).   -*- C++ -*- */

	/* Disable Flex features we don't need, to avoid warnings. */
%option nodefault noinput nounput noyywrap

%option reentrant

%{
	#include <assert.h>
	#include <limits.h> /* INT_MIN */
	#include <float.h>
	#include <stdlib.h> /* strtol */

	#include <complex.h>
	#include <math.h>
	#include "parse.h"
%}

%x SC_STRING

%%
%{
	int nesting = 0;
	char *str = NULL;
	int size = 0;
	int capacity = 0;

	#define STR_APPEND()											\
	do {															\
		if (capacity < size + yyleng + 1) {							\
			do														\
				capacity = capacity ? 2 * capacity : 128;			\
			while (capacity < size + yyleng + 1);					\
			str = realloc (str, capacity);							\
		}															\
		memcpy (str + size, yytext, yyleng);						\
		size += yyleng;												\
		assert (size < capacity);									\
	} while (0)
	/* yyin = fopen("input.txt", "r"); */
%}

	/* Rules. */

	/* Scan a float. */
[0-9]+\.[0-9]+	{
	errno = 0;
	double n = strtod(yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	enqueue(out, n); 
	return NUM;
}

	/* Scan an integer.  */
[0-9]+   {
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	enqueue(out, n); 
	return NUM;
}

[0-9]+i		{
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	enqueue(out, n * I); 
	return NUM;
}

[0-9]+\.[0-9]+i	{
	errno = 0;
	double n = strtod(yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	enqueue(out, n * I); 
	return NUM;
}

\/\/	return COMMENT;

\+		{ return  PLUS; }
-		{ return  MINUS; }
\*		{ return  STAR; }
\/		{ return  SLASH; }
\^		{ return  EXP; }
_		{ return  UNDERSCORE; }

sqrt 	{ s_push(op, 0 + DBL_MAX * I); return  SQRT; }
root	{ s_push(op, 1 + DBL_MAX * I); return  ROOT; }

ln		{ s_push(op, 2 + DBL_MAX * I); return  LN; }
log		{ s_push(op, 3 + DBL_MAX * I); return  LOG; }
e		{ enqueue(out, M_E); return  E; }

abs		{ s_push(op, 4 + DBL_MAX * I); return  ABS; }
floor	{ s_push(op, 5 + DBL_MAX * I); return  FLOOR; }
ceil	{ s_push(op, 6 + DBL_MAX * I); return  CEIL; }

arcsin	{ s_push(op, 7 + DBL_MAX * I); return  ASIN; }
arccos	{ s_push(op, 8 + DBL_MAX * I); return  ACOS; }
arctan	{ s_push(op, 9 + DBL_MAX * I); return  ATAN; }
sinh	{ s_push(op, 10 + DBL_MAX * I); return  SINH; }
cosh	{ s_push(op, 11 + DBL_MAX * I); return  COSH; }
tanh	{ s_push(op, 12 + DBL_MAX * I); return  TANH; }
sech	{ s_push(op, 13 + DBL_MAX * I); return  SECH; }
csch	{ s_push(op, 14 + DBL_MAX * I); return  CSCH; }
coth	{ s_push(op, 15 + DBL_MAX * I); return  COTH; }
sin		{ s_push(op, 16 + DBL_MAX * I); return  SIN; }
cos		{ s_push(op, 17 + DBL_MAX * I); return  COS; }
tan		{ s_push(op, 18 + DBL_MAX * I); return  TAN; }
sec		{ s_push(op, 19 + DBL_MAX * I); return  SEC; }
csc		{ s_push(op, 20 + DBL_MAX * I); return  CSC; }
cot		{ s_push(op, 21 + DBL_MAX * I); return  COT; }
pi		{ enqueue(out, M_PI); return  PI; }

i		{ enqueue(out, I); return  MI; }

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	{ return  EOL; }

y		{ return  Y; }
f\(z\)	{ return  FZ; }

z		{ enqueue(out, DBL_MAX + DBL_MAX * I); return LETR; }
=		{ return  EQUALS; }
\<		{ return  LESSTHAN; }
\>		{ return  GREATERTHAN; }
\<=		{ return  LTHANEQTO; }
\>=		{ return  GTHANEQTO; }

	/* \(		nesting += 1; BEGIN SC_STRING; */
\(		{ s_push(op, 0); return LBRAC; }
\)		{ s_push(op, 0); return RBRAC; }

.		;yyerror(yyscanner, res, NULL, NULL, "syntax error, invalid character: %c", yytext[0]);

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
