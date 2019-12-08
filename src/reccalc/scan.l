/* Prologue (directives).   -*- C++ -*- */

	/* Disable Flex features we don't need, to avoid warnings. */
%option nodefault noinput nounput noyywrap

%option reentrant

%{
	#include <assert.h>
	#include <limits.h> /* INT_MIN */
	#include <float.h>
	#include <stdlib.h> /* strtol */

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
%}

	/* Rules. */

	/* Scan a float. */
[0-9]+\.[0-9]+	{
	errno = 0;
	float n = strtof (yytext, NULL);
	if (! (FLT_MIN <= n && n <= FLT_MAX && errno != ERANGE)) {
		yyerror (yyscanner, res, "float is out of range");
	}
	yylval->NUM = (float) n;
	return NUM;
}

	/* Scan an integer.  */
[0-9]+   {
	errno = 0;
	long n = strtol (yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror (yyscanner, res, "integer is out of range");
	}
	yylval->NUM = (int) n;
	return NUM;
}

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

\(		nesting += 1; BEGIN SC_STRING;

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	return EOL;

(y)|(f\([a-z]\))	{
	errno = 0;
	if(strlen(yytext) - 1) {
		yylval->pronum = yytext[2];
	}
	else {
		yylval->pronum = 'x';	
	}
	printf("%c\n", yylval->pronum);
	return LINESTART;
}
[a-z]		return LETR;
=		return EQUALS;
\<		return LESSTHAN;
\>		return GREATERTHAN;
\<=		return LTHANEQTO;
\>=		return GTHANEQTO;

.		;/*yyerror (yyscanner, res, "syntax error, invalid character: %c", yytext[0]);*/

<SC_STRING>
{
	\(+	nesting += yyleng; STR_APPEND ();
	\)		{
		if (!--nesting) {
			BEGIN INITIAL;
			if (str) {
				str[size] = 0;
			}
			yylval->STR = str;
			return STR;
		}
		else {
			STR_APPEND ();
		}
	}
	[^()]+  STR_APPEND ();
}

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
