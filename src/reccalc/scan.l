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
	yylval->NUM = n;
	return NUM;
}

	/* Scan an integer.  */
[0-9]+   {
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	yylval->NUM = n;
	return NUM;
}

[0-9]+i		{
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	yylval->NUM = n * I;
	return NUM;
}

[0-9]+\.[0-9]+i	{
	errno = 0;
	double n = strtod(yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	yylval->NUM = n*I;
	return NUM;
}

[0-9]+z		{
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	yylval->LETR = n;
	return LETR;
}

[0-9]+\.[0-9]+z	{
	errno = 0;
	double n = strtod(yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	yylval->LETR = n;
	return LETR;
}

\/\/	return COMMENT;

\+		return PLUS;
-		return MINUS;
\*		return STAR;
\/		return SLASH;
\^		return EXP;
_		return UNDERSCORE;

sqrt 	return SQRT;
root	return ROOT;

ln		return LN;
log		return LOG;
e		return E;

abs		return ABS;
floor	return FLOOR;
ceil	return CEIL;

arcsin	return ASIN;
arccos	return ACOS;
arctan	return ATAN;
sinh	return SINH;
cosh	return COSH;
tanh	return TANH;
sech	return SECH;
csch	return CSCH;
coth	return COTH;
sin		return SIN;
cos		return COS;
tan		return TAN;
sec		return SEC;
csc		return CSC;
cot		return COT;
pi		return PI;

i		return MI;

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	return EOL;

y		return Y;
f\(z\)	return FZ;

z		return LETR;
=		return EQUALS;
\<		return LESSTHAN;
\>		return GREATERTHAN;
\<=		return LTHANEQTO;
\>=		return GTHANEQTO;

	/* \(		nesting += 1; BEGIN SC_STRING; */
\(		return LBRAC;
\)		return RBRAC;

.		;yyerror(yyscanner, res, NULL, NULL, "syntax error, invalid character: %c", yytext[0]);

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
