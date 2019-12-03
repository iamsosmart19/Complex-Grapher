%{
	#include <stdio.h>
	#include <stdlib.h>
	
	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;
	
	void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
}

%token <ival> INT
%token <fval> FLOAT

%token LINESTART
%token EQUALS
%token LESSTHAN
%token GREATERTHAN
%token LTHANEQTO
%token GTHANEQTO

%token ALPHA
%token BETA
%token GAMMA
%token GAMMACAP
%token DELTA
%token DELTACAP
%token EPSILON
%token VAREPSILON
%token ZETA
%token ETA
%token THETA
%token THETACAP
%token VARTHETA
%token IOTA
%token KAPPA
%token LAMBDA
%token LAMBDACAP
%token MU
%token NU
%token XI
%token XICAP
%token PI
%token PICAP
%token SIGMA
%token SIGMACAP
%token TAU
%token UPSILON
%token PHI
%token PHICAP
%token VARPHI
%token CHI
%token PSI
%token PSICAP
%token OMEGA

%token E
%token INFINITY

%token SQRT
%token LOG
%token LN

%token ABS
%token FLOOR
%token CEIL

%token ASIN
%token ACOS
%token ATAN
%token SINH
%token COSH
%token TANH
%token SECH
%token CSCH
%token COTH
%token SIN
%token COS
%token TAN
%token SEC
%token CSC
%token COT 

%token TEXT
%token BB
%token HAT

%token FRAC
%token ROOT

%left ADD SUB 
%left MUL DIV
%precedence NEG
%left BRACK
%right EXP

%token LBRAC
%token LSBRAC
%token LCBRAC
%token LANGLE
%token LABRAC

%token RBRAC
%token RSBRAC
%token RCBRAC
%token RANGLE
%token RABRAC

%token ENDL

%%
asciimath:
	%empty | asciimath line
;

num:
	INT		{printf("%d", $1); } |
	FLOAT	{printf("%1.1f", $1); }
;

bin:
	ADD {printf("+");} |
	SUB {printf("-");} |
	MUL {printf("*");} |
	DIV {printf("/");} |
	EXP {printf("^");} 
;

unary:
	BRACK {printf("[");} |
	SUB %prec NEG {printf("-");} |
	ASIN {printf("^");} |
	ACOS {printf("^");} |
	ATAN {printf("^");} |
	SINH {printf("^");} |
	COSH {printf("^");} |
	TANH {printf("^");} |
	SECH {printf("^");} |
	CSCH {printf("^");} |
	COTH {printf("^");} |
	SIN {printf("^");} |
	COS {printf("^");} |
	TAN {printf("^");} |
	SEC {printf("^");} |
	CSC {printf("^");} |
	COT  {printf("^");} 
;

%%
//#define DEBUG
int main(void) {
#ifndef DEBUG
	FILE* input = fopen("input.txt", "r");

	yyin = input;
#endif
	yyparse();
}

void yyerror(const char *s) {
	printf("EEK, parse error!  Message: %s\n", s);
	exit(-1);
}

