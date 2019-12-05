// Prologue (directives).
%expect 0

// Emitted in the header file, before the definition of YYSTYPE.
%code requires {
	typedef void* yyscan_t;
	typedef struct {
		// Whether to print the intermediate results.
		int verbose;
		// Value of the last computation.
		float value;
		// Number of errors.
		int nerrs;
	} result;
}

// Emitted in the header file, after the definition of YYSTYPE.
%code provides {
	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex (YYSTYPE* yylval, yyscan_t yyscanner, result *res)
	YY_DECL;

	void yyerror (yyscan_t scanner, result *res, const char *msg, ...);
}

// Emitted on top of the implementation file.
%code top {
	#include <stdarg.h> // va_list.
	#include <stdio.h>  // printf.
	#include <stdlib.h> // getenv.
	#include <math.h>
}

%code {
  result parse_string (const char* cp);
  result parse (void);
}

%define api.pure full
%define api.value.type union
%define parse.error verbose
%define parse.trace
%verbose

 // Scanner and error count are exchanged between main, yyparse and yylex.
%param {yyscan_t scanner}{result *res}

%token
	PLUS	"+"
	MINUS	"-"
	STAR	"*"
	SLASH	"/"
	UNDERSCORE	"_"

	SQRT	"sqrt"

	LN		"ln"
	LOG		"log"
	E		"e"

	SIN		"sin"
	COS		"cos"
	TAN		"tan"
	PI		"pi"

	EOL		"end-of-line"
	TOK_EOF	0 "end-of-file"
;

%token <float> NUM "number"
%type <float> exp
%printer { fprintf (yyo, "%f", $$); } <float>

%token <char*> STR "string"
%printer { fprintf (yyo, "\"%s\"", $$); } <char*>
%destructor { free ($$); } <char*>

// Precedence (from lowest to highest) and associativity.
%left "+" "-"
%left "*" "/"
%right "log" "_"
%precedence UNARY

%%
// Rules.
input:
	line | 
	input line
;

line:
	exp eol {
		res->value = $exp;
		if (res->verbose) {
			printf ("%f\n", $exp);
		}
    } | 
	error eol { yyerrok; }
;

eol:
	TOK_EOF | EOL
;

exp:
	NUM				{ $$ = $1; } | 
	"e"				{ $$ = M_E; } |
	"pi"			{ $$ = M_PI; } |
	exp "+" exp		{ $$ = $1 + $3; } | 
	exp "-" exp		{ $$ = $1 - $3; }	| 
	exp "*" exp		{ $$ = $1 * $3; }	|
	exp "/" exp		{
		if ($3 == 0) {
			yyerror (scanner, res, "invalid division by zero");
			YYERROR;
		}
		else {
			$$ = $1 / $3;
		}
	} | 

	"+" exp %prec UNARY		{ $$ = + $2; }	| 
	"-" exp %prec UNARY		{ $$ = - $2; } | 

	"sqrt" exp %prec UNARY	{ $$ = sqrt($2); } |

	"log" "_" exp STR		{ $$ = log(atof($4)) / log($3); } |
	"ln" exp %prec UNARY	{ $$ = log($2); } |

	"sin" exp %prec UNARY	{ $$ = sin($2); } |
	"cos" exp %prec UNARY	{ $$ = cos($2); } |
	"tan" exp %prec UNARY	{ $$ = tan($2); } |

	STR {
		result r = parse_string ($1);
		free ($1);
		if (r.nerrs) {
			res->nerrs += r.nerrs;
			YYERROR;
		}
		else {
			$$ = r.value;
		}
	}
;

%%
// Epilogue (C code).

#include "scan.h"

result parse (void) {
	yyscan_t scanner;
	yylex_init (&scanner);
	result res = {1, 0, 0};
	yyparse (scanner, &res);
	yylex_destroy (scanner);
	return res;
}

result parse_string (const char *str) {
	yyscan_t scanner;
	yylex_init (&scanner);
	YY_BUFFER_STATE buf = yy_scan_string (str ? str : "", scanner);
	result res = {0, 0, 0};
	yyparse (scanner, &res);
	yy_delete_buffer (buf, scanner);
	yylex_destroy (scanner);
	return res;
}

void yyerror (yyscan_t scanner, result *res, const char *msg, ...) {
	(void) scanner;
	va_list args;
	va_start (args, msg);
	vfprintf (stderr, msg, args);
	va_end (args);
	fputc ('\n', stderr);
	res->nerrs += 1;
}

int main(void) {
	// Possibly enable parser runtime debugging.
	yydebug = !!getenv ("YYDEBUG");
	result res = parse ();
	// Exit on failure if there were errors.
	return !!res.nerrs;
}
