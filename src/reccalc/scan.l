/* Prologue (directives).   -*- C++ -*- */

	/* Disable Flex features we don't need, to avoid warnings. */
%option nodefault noinput nounput noyywrap

%option reentrant

%{
	#include <assert.h>
	#include <limits.h> /* INT_MIN */
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

\+		return PLUS;
-		return MINUS;
\*		return STAR;
\/		return SLASH;
_		return UNDERSCORE;

sqrt 	return SQRT;

ln		return LN;
log		return LOG;
e		return E;

sin		return SIN;
cos		return COS;
tan		return TAN;
pi		return PI;

\(		nesting += 1; BEGIN SC_STRING;

	/* Scan an integer.  */
[0-9]+   {
	errno = 0;
	long n = strtol (yytext, NULL, 10);
	if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
		yyerror (yyscanner, res, "integer is out of range");
	}
	yylval->NUM = (float) n;
	return NUM;
}

 /* Ignore white spaces. */
[ \t]+	continue;

[\n\r]	return EOL;

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
