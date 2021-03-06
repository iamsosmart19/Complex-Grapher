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

%%
	/* Rules. */

	/* Scan a float. */
[0-9]+\.[0-9]+	{
	errno = 0;
	//Convert string to double
	double n = strtod(yytext, NULL);
	//If the resulting number is smaller or larger than the smallest
	//and largest allowed floats, or if the errno is ERANGE
	//throw an error stating the float is out of range
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	//Add number to out
	enqueue(out, n); 
	return NUM;
}

	/* Scan an integer.  */
[0-9]+   {
	errno = 0;
	//Convert string to a long
	long n = strtol(yytext, NULL, 10);
	//If the resulting number is smaller or larger than the smallest
	//and largest allowed long, or if the errno is ERANGE
	//throw an error stating the float is out of range
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	//Add number to out
	enqueue(out, n); 
	return NUM;
}

[0-9]+i		{
	errno = 0;
	//Convert string to a long
	//Note: strtol will automatically filter out the i
	long n = strtol(yytext, NULL, 10);
	//If the resulting number is smaller or larger than the smallest
	//and largest allowed long, or if the errno is ERANGE
	//throw an error stating the float is out of range
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "integer is out of range");
	}
	//Multiply by I because number is meant to be imaginary
	enqueue(out, n * I); 
	return NUM;
}

[0-9]+\.[0-9]+i	{
	errno = 0;
	//Convert string to double
	//Note: strtod will automatically filter out the i
	double n = strtod(yytext, NULL);
	//If the resulting number is smaller or larger than the smallest
	//and largest allowed floats, or if the errno is ERANGE
	//throw an error stating the float is out of range
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror(yyscanner, res, NULL, NULL, "float is out of range");
	}
	//Multiply by I because number is meant to be imaginary
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

sqrt 	{ return SQRT; }
root	{ return  ROOT; }

ln		{ return  LN; }
log		{ return  LOG; }
e		{ enqueue(out, M_E); return E; }

abs		{ return  ABS; }
floor	{ return  FLOOR; }
ceil	{ return  CEIL; }
re		{ return  REAL; }
im		{ return  IMAG; }

asin	{ return  ASIN; }
acos	{ return  ACOS; }
atan	{ return  ATAN; }
sinh	{ return  SINH; }
cosh	{ return  COSH; }
tanh	{ return  TANH; }
sech	{ return  SECH; }
csch	{ return  CSCH; }
coth	{ return  COTH; }
sin		{ return  SIN; }
cos		{ return  COS; }
tan		{ return  TAN; }
sec		{ return  SEC; }
csc		{ return  CSC; }
cot		{ return  COT; }
pi		{ enqueue(out, M_PI); return  PI; }

i		{ enqueue(out, I); return  MI; }

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	{ return  EOL; }

y		{ return Y; }
f\(z\)	{ return FZ; }

z		{ enqueue(out, DBL_MAX + DBL_MAX * I); return LETR; }
=		{ return  EQUALS; }
\<		{ return  LESSTHAN; }
\>		{ return  GREATERTHAN; }
\<=		{ return  LTHANEQTO; }
\>=		{ return  GTHANEQTO; }

\(		{ return LBRAC; }
\)		{ return RBRAC; }

\|		{ return CONJ; }

,		{ return COMMA; }

.		;yyerror(yyscanner, res, NULL, NULL, "syntax error, invalid character: %c", yytext[0]);

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
