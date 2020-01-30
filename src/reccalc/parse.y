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

	void yyerror(yyscan_t scanner, result *res, const char *msg, ...);
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

	EOL		"end-of-line"
	TOK_EOF	0 "end-of-file"
;

%token <cplx> NUM "number"
%token <cplx> LETR "variable" 

%type <cplx> exp

%printer { fprintf(yyo, "%lf%+lfi", creal($$), cimag($$)); } <cplx>

%type <cplx> sexp

%type <cplx> eqtn

%token <char*> STR "string"
%printer { fprintf(yyo, "\"%s\"", $$); } <char*>
%destructor { free($$); } <char*>

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
	exp %prec BINARY	{ res->value = $exp; /*printf("\t%.7lf%+.7lfi\n", creal($exp), cimag($exp)); */} | 
	line |
	input line
;

line:
	eqtn eol {
		res->value = $eqtn;
		if (res->verbose) {
			printf("\t%.7lf%+.7lfi\n", creal($eqtn), cimag($eqtn));
		}
    } | 
	"//" eqtn eol {
		;
	} |
	error eol { /*printf("err\n");*/ yyerrok; }
;

eqtn:
	linestart mid exp { $$ = $exp; }
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
	NUM %prec UNARY	{ $$ = $1; /*printf("%lf%+lfi\n", creal($1), cimag($1));*/ enqueue(*out, $1); } | 
	"e"				{ $$ = M_E; enqueue(*out, M_E); } |
	"pi"			{ $$ = M_PI; enqueue(*out, M_PI); } |
	"i"	%prec UNARY	{ $$ = I; enqueue(*out, I); } |
	LETR %prec UNARY { $$ = $1; } |
	sexp			{ $$ = $1; } |

	exp "+" exp		{ $$ = $1 + $3; } | 
	exp "-" exp		{ $$ = $1 - $3; }	| 
	exp "*" exp		{ $$ = $1 * $3; }	|
	exp "/" exp		{
		if ($3 == 0) {
			yyerror(scanner, res, "invalid division by zero");
			YYERROR;
		}
		else {
			$$ = $1 / $3;
		}
	} | 
	exp "^" exp		{ $$ = pow($1, $3); } | 

	"+" exp %prec UNARY		{ $$ = + $2; }	| 
	"-" exp %prec UNARY		{ $$ = - $2; } | 

	"sqrt" sexp %prec UNARY	{ $$ = sqrt($2); } |
	"root" sexp sexp 	 	{ $$ = pow($3, 1./$2); } |

	"log" "_" sexp sexp		{ $$ = log($4) / log($3); } |
	"ln" sexp %prec UNARY	{ $$ = log($2); } |

	"floor" sexp %prec UNARY { $$ = floor((float)$2); } |
	"ceil" sexp %prec UNARY { $$ = ceil((float)$2); } |

	"asin" sexp %prec UNARY	{ $$ = asin($2); } |
	"acos" sexp %prec UNARY	{ $$ = acos($2); } |
	"atan" sexp %prec UNARY	{ $$ = atan($2); } |
	"sinh" sexp %prec UNARY	{ $$ = sinh($2); } |
	"cosh" sexp %prec UNARY	{ $$ = cosh($2); } |
	"tanh" sexp %prec UNARY	{ $$ = tanh($2); } |
	"sech" sexp %prec UNARY	{ $$ = 1./cosh($2); } |
	"csch" sexp %prec UNARY	{ $$ = 1./sinh($2); } |
	"coth" sexp %prec UNARY	{ $$ = 1./tanh($2); } |
	"sin" sexp %prec UNARY	{ $$ = sin($2); printf("top: %lf%+lfi\n", creal(front(*out)), cimag(front(*out))); } |
	"cos" sexp %prec UNARY	{ $$ = cos($2); } |
	"tan" sexp %prec UNARY	{ $$ = tan($2); } |
	"sec" sexp %prec UNARY	{ $$ = 1./cos($2); } |
	"csc" sexp %prec UNARY	{ $$ = 1./sin($2); } |
	"cot" sexp %prec UNARY	{ $$ = 1./tan($2); } 
;

sexp:
	STR {
		/* printf("str\n"); */
		stack op = stackInit();
		queue out = queueInit();
		result r = parse_string($1, &op, &out);
		free($1);
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

void yyerror(yyscan_t scanner, result *res, const char *msg, ...) {
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
