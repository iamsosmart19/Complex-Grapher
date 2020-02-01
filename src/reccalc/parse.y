// Prologue (directives).
%expect 0

// Emitted in the header file, before the definition of YYSTYPE.
%code requires {
	#include <complex.h>
	#include "stack.h"
	#include "queue.h"

	typedef double complex cplx;
	typedef void* yyscan_t;
	typedef struct {
		// Whether to print the intermediate results.
		int verbose;
		// Value of the last computation.
		cplx value;
		// Number of errors.
		int nerrs;
	} result;
	result parse_string(const char* str, stack* s, queue* q);
	result parse(void);
}

// Emitted in the header file, after the definition of YYSTYPE.
%code provides {
	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res)
	YY_DECL;

	/* void yyerror(yyscan_t scanner, result *res, const char *msg, ...); */
	void yyerror(yyscan_t scanner, result *res, stack *op, queue *out, const char *msg, ...);
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
	result parse_string(const char* str, stack* s, queue* q);
	result parse(void);
	int precValues[5] = {2, 2, 3, 3, 4};
}

%define api.pure full
%define api.value.type union
%define parse.error verbose
%define parse.trace

 // Scanner and error count are exchanged between main, yyparse and yylex.
%param {yyscan_t scanner}{result *res}

//custom yyparse paramaters
%parse-param {stack* op}{queue* out}

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

	LBRAC			"("
	RBRAC			")"

	EOL		"end-of-line"
	TOK_EOF	0 "end-of-file"
;

%token <cplx> NUM "number" 
%token <cplx> LETR "variable" 

// Precedence (from lowest to highest) and associativity.
%precedence BINARY
%left "+" "-"
%left "*" "/"
%right "^"
%right "log" "_" "root"
%precedence UNARY

%%
// Rules.
input:
	exp %prec BINARY | 
	line |
	input line
;

line:
	eqtn eol  {
		while(top(*op) != DBL_MIN + DBL_MIN * I) {
			enqueue(*out, pop(*op));
		}
	} | 
	"//" eqtn eol {
		;
	} |
	error eol { /*printf("err\n");*/ yyerrok; }
;

eqtn:
	linestart mid exp
; 

linestart:
	"y"	|
	"f(z)"
;

mid:
	"=" |
	"<" |
	">" |
	"<=" |
	">="
;

eol:
	TOK_EOF | EOL 
;

exp:
	NUM %prec UNARY	{ /*printf("%lf%+lfi\n", creal($1), cimag($1));*/ enqueue(*out, $1); } | 
	"e"				{ enqueue(*out, M_E); } |
	"pi"			{ enqueue(*out, M_PI); } |
	"i"	%prec UNARY	{ enqueue(*out, I); } |
	LETR %prec UNARY { enqueue(*out, DBL_MAX + DBL_MAX * I); } |
	"(" exp ")"		{ } |

	exp "+" exp		{ } | 
	exp "-" exp		{ }	| 
	exp "*" exp		{ }	|
	exp "/" exp		{ } | 
	exp "^" exp		{ } | 

	"+" exp %prec UNARY		{ }	| 
	"-" exp %prec UNARY		{ } | 

	"sqrt" "(" exp ")" %prec UNARY	{ push(*op, 1 + DBL_MAX * I); } |
	"root" "(" exp ")" "(" exp ")"	{ push(*op, 2 + DBL_MAX * I); } |

	"log" "_" "(" exp ")" "(" exp ")"		{ push(*op, 3 + DBL_MAX * I); } |
	"ln" "(" exp ")" %prec UNARY	{ push(*op, 4 + DBL_MAX * I); } |

	"abs" "(" exp ")" %prec UNARY	{ push(*op, 5 + DBL_MAX * I); } |
	"floor" "(" exp ")" %prec UNARY { push(*op, 6 + DBL_MAX * I); } |
	"ceil" "(" exp ")" %prec UNARY	{ push(*op, 7 + DBL_MAX * I); } |

	"asin" "(" exp ")" %prec UNARY	{ push(*op, 8 + DBL_MAX * I); } |
	"acos" "(" exp ")" %prec UNARY	{ push(*op, 9 + DBL_MAX * I); } |
	"atan" "(" exp ")" %prec UNARY	{ push(*op, 10 + DBL_MAX * I); } |
	"sinh" "(" exp ")" %prec UNARY	{ push(*op, 11 + DBL_MAX * I); } |
	"cosh" "(" exp ")" %prec UNARY	{ push(*op, 12 + DBL_MAX * I); } |
	"tanh" "(" exp ")" %prec UNARY	{ push(*op, 13 + DBL_MAX * I); } |
	"sech" "(" exp ")" %prec UNARY	{ push(*op, 14 + DBL_MAX * I); } |
	"csch" "(" exp ")" %prec UNARY	{ push(*op, 15 + DBL_MAX * I); } |
	"coth" "(" exp ")" %prec UNARY	{ push(*op, 16 + DBL_MAX * I); } |
	"sin" "(" exp ")" %prec UNARY	{ push(*op, 17 + DBL_MAX * I); } |
	"cos" "(" exp ")" %prec UNARY	{ push(*op, 18 + DBL_MAX * I); } |
	"tan" "(" exp ")" %prec UNARY	{ push(*op, 19 + DBL_MAX * I); } |
	"sec" "(" exp ")" %prec UNARY	{ push(*op, 20 + DBL_MAX * I); } |
	"csc" "(" exp ")" %prec UNARY	{ push(*op, 21 + DBL_MAX * I); } |
	"cot" "(" exp ")" %prec UNARY	{ push(*op, 22 + DBL_MAX * I); } 
;

%%
// Epilogue (C code).

#include "scan.h"

result parse(void) {
	yyscan_t scanner;
	yylex_init(&scanner);
	result res = {1, 0, 0};
	stack blank = stackInit();
	queue blonk = queueInit();
	stack* op = &blank;
	queue* out = &blonk;
	yyparse(scanner, &res, op, out);
	yylex_destroy(scanner);
	return res;
}

//For operators set real() to INT_MAX and imaginary part to op value
result parse_string(const char* str, stack* op, queue* out) {
	/* printf("%s\n", str); */
	yyscan_t scanner;
	yylex_init(&scanner);
	YY_BUFFER_STATE buf = yy_scan_string(str ? str : "", scanner);
	result res = {0, 0, 0};
	yyparse(scanner, &res, op, out);
	yy_delete_buffer(buf, scanner);
	yylex_destroy(scanner);
	return res;
}

void yyerror(yyscan_t scanner, result *res, stack *op, queue *out, const char *msg, ...) {
	(void) scanner;
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	fputc('\n', stderr);
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
