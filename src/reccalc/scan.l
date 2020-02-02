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
	(*tknCnt)++;
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
	(*tknCnt)++;
	return NUM;
}

[0-9]+i		{
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	enqueue(out, n * I); 
	(*tknCnt)++;
	return NUM;
}

[0-9]+\.[0-9]+i	{
	errno = 0;
	double n = strtod(yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	enqueue(out, n * I); 
	(*tknCnt)++;
	return NUM;
}

\/\/	return COMMENT;

\+		{ (*tknCnt)++; return  PLUS; }
-		{ (*tknCnt)++; return  MINUS; }
\*		{ (*tknCnt)++; return  STAR; }
\/		{ (*tknCnt)++; return  SLASH; }
\^		{ (*tknCnt)++; return  EXP; }
_		{ (*tknCnt)++; return  UNDERSCORE; }

sqrt 	{ (*tknCnt)++; return  SQRT; }
root	{ (*tknCnt)++; return  ROOT; }

ln		{ (*tknCnt)++; return  LN; }
log		{ (*tknCnt)++; return  LOG; }
e		{ (*tknCnt)++; enqueue(out, M_E); return  E; }

abs		{ (*tknCnt)++; return  ABS; }
floor	{ (*tknCnt)++; return  FLOOR; }
ceil	{ (*tknCnt)++; return  CEIL; }

arcsin	{ (*tknCnt)++; return  ASIN; }
arccos	{ (*tknCnt)++; return  ACOS; }
arctan	{ (*tknCnt)++; return  ATAN; }
sinh	{ (*tknCnt)++; return  SINH; }
cosh	{ (*tknCnt)++; return  COSH; }
tanh	{ (*tknCnt)++; return  TANH; }
sech	{ (*tknCnt)++; return  SECH; }
csch	{ (*tknCnt)++; return  CSCH; }
coth	{ (*tknCnt)++; return  COTH; }
sin		{ (*tknCnt)++; return  SIN; }
cos		{ (*tknCnt)++; return  COS; }
tan		{ (*tknCnt)++; return  TAN; }
sec		{ (*tknCnt)++; return  SEC; }
csc		{ (*tknCnt)++; return  CSC; }
cot		{ (*tknCnt)++; return  COT; }
pi		{ (*tknCnt)++; enqueue(out, M_PI); return  PI; }

i		{ (*tknCnt)++; enqueue(out, I); return  MI; }

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	{ return  EOL; }

y		{ return Y; }
f\(z\)	{ return FZ; }

z		{ (*tknCnt)++; enqueue(out, DBL_MAX + DBL_MAX * I); return LETR; }
=		{ return  EQUALS; }
\<		{ return  LESSTHAN; }
\>		{ return  GREATERTHAN; }
\<=		{ return  LTHANEQTO; }
\>=		{ return  GTHANEQTO; }

\(		{ return LBRAC;}
\)		{ return RBRAC; }

.		;yyerror(yyscanner, res, NULL, NULL, "syntax error, invalid character: %c", yytext[0]);

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
