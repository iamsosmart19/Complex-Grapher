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

%left ADD SUB 
%left MUL DIV
%precedence NEG
%right EXP

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

%token ENDL

%%
asciimath:
	%empty | asciimath line;

line:
	ENDL | expression ENDL;

expression:
	unary expression | expression binary expression | num;

num:
	INT { printf("int: %d", $1); } |
	FLOAT { printf("float: %f", $1); }; 

binary:
	 ADD { printf("+");} |
	 SUB { printf("-");} |
	 MUL { printf("*");} |
	 DIV { printf("/");} |

	 EXP { printf("^");} 
;

unary:
	 SUB %prec NEG |
	 ASIN { printf("^");} |
	 ACOS { printf("^");} |
	 ATAN { printf("^");} |
	 SINH { printf("^");} |
	 COSH { printf("^");} |
	 TANH { printf("^");} |
	 SECH { printf("^");} |
	 CSCH { printf("^");} |
	 COTH { printf("^");} |
	 SIN { printf("^");} |
	 COS { printf("^");} |
	 TAN { printf("^");} |
	 SEC { printf("^");} |
	 CSC { printf("^");} |
	 COT  { printf("^");} 
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

