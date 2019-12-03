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

line:
	ENDL | expr ENDL { printf("\n\r"); }
;

expr:
	intexpr expr | intexpr
;

intexpr:
	simpexpr EXP simpexpr |
	simpexpr
;

simpexpr:
	variable |
	left simpexpr right |
	unary simpexpr |
	simpexpr bin simpexpr |
	binfunc simpexpr simpexpr
;

left:
	LBRAC |
	LSBRAC |
	LCBRAC |
	LANGLE |
	LABRAC
;

right:
	RBRAC |
	RSBRAC |
	RCBRAC |
	RANGLE |
	RABRAC
;

variable:
	num |
	greek | 
	const
;

num:
	INT		{printf("%d ", $1); } |
	FLOAT	{printf("%1.1f ", $1); }
;

greek:
	ALPHA			{ printf("alpha "); } |
	BETA			{ printf("beta "); } |
	GAMMA			{ printf("gamma "); } |
	GAMMACAP		{ printf("gammacap "); } |
	DELTA			{ printf("delta "); } |
	DELTACAP		{ printf("deltacap "); } |
	EPSILON			{ printf("epsilon "); } |
	VAREPSILON		{ printf("varepsilon "); } |
	ZETA			{ printf("zeta "); } |
	ETA				{ printf("eta "); } |
	THETA			{ printf("theta "); } |
	THETACAP		{ printf("thetacap "); } |
	VARTHETA		{ printf("vartheta "); } |
	IOTA			{ printf("iota "); } |
	KAPPA			{ printf("kappa "); } |
	LAMBDA			{ printf("lambda "); } |
	LAMBDACAP		{ printf("lambdacap "); } |
	MU				{ printf("mu "); } |
	NU				{ printf("nu "); } |
	XI				{ printf("xi "); } |
	XICAP			{ printf("xicap "); } |
	PI				{ printf("pi "); } |
	PICAP			{ printf("picap "); } |
	SIGMA			{ printf("sigma "); } |
	SIGMACAP		{ printf("sigmacap "); } |
	TAU				{ printf("tau "); } |
	UPSILON			{ printf("upsilon "); } |
	PHI				{ printf("phi "); } |
	PHICAP			{ printf("phicap "); } |
	VARPHI			{ printf("varphi "); } |
	CHI				{ printf("chi "); } |
	PSI				{ printf("psi "); } |
	PSICAP			{ printf("psicap "); } |
	OMEGA			{ printf("omega "); }
;

const:
	E				{ printf("e "); } |
	INFINITY		{ printf("oo "); }
;

bin:
	ADD {printf("+ ");} |
	SUB {printf("- ");} |
	MUL {printf("* ");} |
	DIV {printf("/ ");} |
	EXP {printf("^ ");} 
;

binfunc:
	FRAC |
	ROOT 
;

unary:
	SUB %prec NEG {printf("-");} |

	SQRT { printf("sqrt "); } |

	LOG { printf("log "); } |
	LN { printf("ln "); } |

	ABS { printf("abs "); } |
	FLOOR { printf("floor "); } |
	CEIL { printf("ceil "); } |

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

