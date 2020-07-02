// Prologue (directives).
%expect 0

// Emitted in the header file, before the definition of YYSTYPE.
%code requires {
	#include <complex.h>
	#include "queue.h"

	//Definition of complex numbers
	typedef double complex cplx;
	typedef void* yyscan_t;
	typedef struct {
		// Whether to print the intermediate results.
		int verbose;
		// Value of the last computation.
		cplx value;
		// Number of errors.
		int nerrs;
		//Address of error
		char* err;
	} result;
	//Parses str and places the result in out; result records errors
	result parse_string(const char* str, queue* out);
	//Parses from stdin(standard input)
	result parse(void);
}

// Emitted in the header file, after the definition of YYSTYPE.
%code provides {
	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res, queue *out)
	YY_DECL;

	/* void yyerror(yyscan_t scanner, result *res, const char *msg, ...); */
	void yyerror(yyscan_t scanner, result *res, queue *out, const char *msg, ...);
}

// Emitted on top of the implementation file.
%code top {
	#include <stdarg.h> // va_list.
	#include <stdio.h>  // printf.
	#include <stdlib.h> // getenv.

	#include <math.h>
	#include <tgmath.h>
	#include <float.h>
	#include <complex.h>
}

%code {
	//Redeclaration of functions

	result parse_string(const char* str, queue* out);
	result parse(void);
}

%define api.pure full
%define api.value.type union
//Enables debugging options
%define parse.error verbose
%define parse.trace

 // Scanner and error count are exchanged between main, yyparse and yylex.
%param {yyscan_t scanner}{result *res}

//custom yyparse paramaters
%param {queue* out}

%token
	COMMENT "//"
	PLUS	"+"
	MINUS	"-"
	STAR	"*"
	SLASH	"/"
	EXP		"^"
	UNDERSCORE	"_"

	SQRT	"sqrt"
	ROOT	"root"

	LN		"ln"
	LOG		"log"
	E		"e"

	ABS		"abs"
	FLOOR	"floor"
	CEIL	"ceil"
	REAL	"real"
	IMAG	"imag"

	ASIN	"asin"
	ACOS	"acos"
	ATAN	"atan"
	SINH	"sinh"
	COSH	"cosh"
	TANH	"tanh"
	SECH	"sech"
	CSCH	"csch"
	COTH	"coth"
	SIN		"sin"
	COS		"cos"
	TAN		"tan"
	SEC		"sec"
	CSC		"csc"
	COT		"cot"
	PI		"pi"

	MI		"i"

	Y		"y"
	FZ		"f(z)"
	EQUALS			"="
	LESSTHAN		"<"
	GREATERTHAN		">"
	LTHANEQTO		"<="
	GTHANEQTO		">="

	LETR			"z"

	LBRAC			"("
	RBRAC			")"

	CONJ			"|"

	COMMA			","

	EOL		"end-of-line"
	TOK_EOF	0 "end-of-file"
;

//Assigns variable cplx to token NUM
%token <cplx> NUM "number"

// Precedence (from lowest to highest) and associativity.
%precedence BINARY
%left "+" "-"
%left "*" "/"
%right "^"
%right "log" "_" "root"
%precedence UNARY

%%
// Rules
//Composition of input
input:
	line |
	input line
;

//Defines a line
line:
	exp eol  { } | 
	error eol { /*printf("err\n");*/ yyerrok; }
;

/* linestart: */
/* 	"y"	| */
/* 	"f(z)" */
/* ; */

/* mid: */
/* 	"=" | */
/* 	"<" | */
/* 	">" | */
/* 	"<=" | */
/* 	">=" */
/* ; */

//Defines what EOL is 
eol:
	TOK_EOF | EOL 
;

//Valid expressions
//enqueue adds the function value to the queue out
exp:
	NUM %prec UNARY	{ } | 
	"e"				{ } |
	"pi"			{ } |
	"i"	%prec UNARY	{ } |
	"z" %prec UNARY { } |
	brexp			{ } |

	exp "+" exp		{
		enqueue(out, 0 - DBL_MAX * I);
	} | 
	exp "-" exp		{ 
		enqueue(out, 1 - DBL_MAX * I);
	}	| 
	exp "*" exp		{
		enqueue(out, 2 - DBL_MAX * I);
	}	|
	exp "/" exp		{
		enqueue(out, 3 - DBL_MAX * I);
	} | 
	exp "^" exp		{
		enqueue(out, 4 - DBL_MAX * I);
	} | 

	"+" exp %prec UNARY		{ }	| 
	"-" exp %prec UNARY		{ enqueue(out, 5 - DBL_MAX * I); } | 

	"sqrt" "(" exp ")" %prec UNARY	{ enqueue(out, 0 + DBL_MAX * I); } |
	"root" "(" exp "," exp ")"		{ enqueue(out, 1 + DBL_MAX * I); } |

	"ln" "(" exp ")" %prec UNARY	{ enqueue(out, 2 + DBL_MAX * I); } |
	"log" "_" "(" exp ")" "(" exp ")"		{ enqueue(out, 3 + DBL_MAX * I); } |

	"abs" "(" exp ")" %prec UNARY	{ enqueue(out, 4 + DBL_MAX * I); } |
	"floor" "(" exp ")" %prec UNARY { enqueue(out, 5 + DBL_MAX * I); } |
	"ceil" "(" exp ")" %prec UNARY	{ enqueue(out, 6 + DBL_MAX * I); } |
	"real" "(" exp ")" %prec UNARY	{ enqueue(out, 22 + DBL_MAX * I); } |
	"imag" "(" exp ")" %prec UNARY	{ enqueue(out, 23 + DBL_MAX * I); } |

	"asin" "(" exp ")" %prec UNARY	{ enqueue(out, 7 + DBL_MAX * I); } |
	"acos" "(" exp ")" %prec UNARY	{ enqueue(out, 8 + DBL_MAX * I); } |
	"atan" "(" exp ")" %prec UNARY	{ enqueue(out, 9 + DBL_MAX * I); } |
	"sinh" "(" exp ")" %prec UNARY	{ enqueue(out, 10 + DBL_MAX * I); } |
	"cosh" "(" exp ")" %prec UNARY	{ enqueue(out, 11 + DBL_MAX * I); } |
	"tanh" "(" exp ")" %prec UNARY	{ enqueue(out, 12 + DBL_MAX * I); } |
	"sech" "(" exp ")" %prec UNARY	{ enqueue(out, 13 + DBL_MAX * I); } |
	"csch" "(" exp ")" %prec UNARY	{ enqueue(out, 14 + DBL_MAX * I); } |
	"coth" "(" exp ")" %prec UNARY	{ enqueue(out, 15 + DBL_MAX * I); } |
	"sin" "(" exp ")" %prec UNARY	{ enqueue(out, 16 + DBL_MAX * I); } |
	"cos" "(" exp ")" %prec UNARY	{ enqueue(out, 17 + DBL_MAX * I); } |
	"tan" "(" exp ")" %prec UNARY	{ enqueue(out, 18 + DBL_MAX * I); } |
	"sec" "(" exp ")" %prec UNARY	{ enqueue(out, 19 + DBL_MAX * I); } |
	"csc" "(" exp ")" %prec UNARY	{ enqueue(out, 20 + DBL_MAX * I); } |
	"cot" "(" exp ")" %prec UNARY	{ enqueue(out, 21 + DBL_MAX * I); } |

	"|" exp "|" %prec UNARY			{ enqueue(out, 24 + DBL_MAX * I); }
;

//Bracket expression
brexp:
	"(" exp ")"	{ } |
	//Formalisation of declarations such as (z+1)(z-1)
	"(" exp ")" brexp { enqueue(out, 2 - DBL_MAX * I); }
;

%%
// Epilogue (C code).

#include "scan.h"

//Parses from standard input
result parse(void) {
	//Declaration and initialisation of scanner
	yyscan_t scanner;
	yylex_init(&scanner);

	result res = {1, 0, 0, NULL};
	//Parses from standard input
	yyparse(scanner, &res, NULL);

	//Once parsing finishes destroy scanner and return result
	yylex_destroy(scanner);
	return res;
}

//For operators set real() to INT_MAX and imaginary part to op value

//Parses from provided string and outputs to a queue out
result parse_string(const char* str, queue* out) {
	//Declaration and initialisation of scanner
	yyscan_t scanner;
	yylex_init(&scanner);

	//Sets buffer to str provided that str is not empty
	YY_BUFFER_STATE buf = yy_scan_string(str ? str : "", scanner);

	result res = {0, 0, 0, NULL};
	//Parses str
	yyparse(scanner, &res, out);
	//Once parsing finishes destroy scanner and return result
	yy_delete_buffer(buf, scanner);
	yylex_destroy(scanner);
	return res;
}

//Places errors that occur while parsing in res->err
void yyerror(yyscan_t scanner, result *res, queue *out, const char *msg, ...) {
	(void) scanner;

	//Get additional arguments (...)
	va_list args;
	va_start(args, msg);
	/* vfprintf(stderr, msg, args); */

	//Allocate space for err to hold message
	res->err = malloc(sizeof(char) * 256);
	//Copy error from msg to err
	sprintf(res->err, msg, args);

	va_end(args);
	res->nerrs += 1;
}

/*
int main(void) {
	// Possibly enable parser runtime debugging.
	yydebug = !!getenv("YYDEBUG");
	result res = parse();
	// Exit on failure if there were errors.
	return !!res.nerrs;
}
*/

/*
int main(void) {
	// Possibly enable parser runtime debugging.
	// Exit on failure if there were errors.
	FILE* sample = fopen("input.txt", "r");
	char function[1024];
	fgets(function, 1024, sample);
	fclose(sample);

	yydebug = !!getenv("YYDEBUG");
	result res = parse_string(function);

	return !!res.nerrs;
}
*/
