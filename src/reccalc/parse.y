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
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res, stack *op, queue *out)
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
%param {stack* op}{queue* out}

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

	LETR			"z"

	LBRAC			"("
	RBRAC			")"

	EOL		"end-of-line"
	TOK_EOF	0 "end-of-file"
;

%token <cplx> NUM "number"

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
		while(s_top(*op) != -DBL_MAX - DBL_MAX * I) {
			enqueue(out, s_pop(op));
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
	NUM %prec UNARY	{ } | 
	"e"				{ } |
	"pi"			{ } |
	"i"	%prec UNARY	{ } |
	"z" %prec UNARY { } |
	"(" exp ")"		{ } |

	exp "+" exp		{
		if( s_top(*op) == -DBL_MAX - DBL_MAX * I ) {
			goto skipAddLoop;
		}
		while( (cimag(s_top(*op)) == DBL_MAX) ||
			 ( cimag(s_top(*op)) == -DBL_MAX) && precValues[(int)creal(s_top(*op))] > precValues[0] ) {
			printf("ADD: %lf\n", creal(s_top(*op)));
			enqueue(out, s_pop(op));
		}
			skipAddLoop:
		s_push(op, 0 - DBL_MAX * I);
	} | 
	exp "-" exp		{ 
		if( s_top(*op) == -DBL_MAX - DBL_MAX * I ) {
			goto skipSubLoop;
		}
		while( (cimag(s_top(*op)) == DBL_MAX) ||
			 ( cimag(s_top(*op)) == -DBL_MAX) && precValues[(int)creal(s_top(*op))] > precValues[1] ) {
			printf("SUB: %lf\n", creal(s_top(*op)));
			enqueue(out, s_pop(op));
		}
			skipSubLoop:
		s_push(op, 1 - DBL_MAX * I);
	}	| 
	exp "*" exp		{
		if( s_top(*op) == -DBL_MAX - DBL_MAX * I ) {
			goto skipAstLoop;
		}
		while( (cimag(s_top(*op)) == DBL_MAX) ||
			 ( cimag(s_top(*op)) == -DBL_MAX) && precValues[(int)creal(s_top(*op))] > precValues[2] ) {
			printf("AST: %lf\n", creal(s_top(*op)));
			enqueue(out, s_pop(op));
		}
			skipAstLoop:
		s_push(op, 2 - DBL_MAX * I);
	}	|
	exp "/" exp		{
		if( s_top(*op) == -DBL_MAX - DBL_MAX * I ) {
			goto skipDivLoop;
		}
		while( (cimag(s_top(*op)) == DBL_MAX) ||
			 ( cimag(s_top(*op)) == -DBL_MAX) && precValues[(int)creal(s_top(*op))] > precValues[3] ) {
			printf("DIV: %lf\n", creal(s_top(*op)));
			enqueue(out, s_pop(op));
		}
			skipDivLoop:
		s_push(op, 3 - DBL_MAX * I);
	} | 
	exp "^" exp		{
		if( s_top(*op) == -DBL_MAX - DBL_MAX * I ) {
			goto skipExpLoop;
		}
		while( (cimag(s_top(*op)) == DBL_MAX) ||
			 ( cimag(s_top(*op)) == -DBL_MAX) && precValues[(int)creal(s_top(*op))] > precValues[4] ) {
			printf("EXP: %lf\n", creal(s_top(*op)));
			enqueue(out, s_pop(op));
		}
			skipExpLoop:
		s_push(op, 4 - DBL_MAX * I);
	} | 

	"+" exp %prec UNARY		{ }	| 
	"-" exp %prec UNARY		{ } | 

	"sqrt" "(" exp ")" %prec UNARY	{ } |
	"root" "(" exp ")" "(" exp ")"	{ } |

	"ln" "(" exp ")" %prec UNARY	{ } |
	"log" "_" "(" exp ")" "(" exp ")"		{ } |

	"abs" "(" exp ")" %prec UNARY	{ } |
	"floor" "(" exp ")" %prec UNARY { } |
	"ceil" "(" exp ")" %prec UNARY	{ } |

	"asin" "(" exp ")" %prec UNARY	{ } |
	"acos" "(" exp ")" %prec UNARY	{ } |
	"atan" "(" exp ")" %prec UNARY	{ } |
	"sinh" "(" exp ")" %prec UNARY	{ } |
	"cosh" "(" exp ")" %prec UNARY	{ } |
	"tanh" "(" exp ")" %prec UNARY	{ } |
	"sech" "(" exp ")" %prec UNARY	{ } |
	"csch" "(" exp ")" %prec UNARY	{ } |
	"coth" "(" exp ")" %prec UNARY	{ } |
	"sin" "(" exp ")" %prec UNARY	{ } |
	"cos" "(" exp ")" %prec UNARY	{ } |
	"tan" "(" exp ")" %prec UNARY	{ } |
	"sec" "(" exp ")" %prec UNARY	{ } |
	"csc" "(" exp ")" %prec UNARY	{ } |
	"cot" "(" exp ")" %prec UNARY	{ } 
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
