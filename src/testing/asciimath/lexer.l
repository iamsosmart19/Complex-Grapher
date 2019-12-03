%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "parser.tab.h"
	extern int yylex();
%}
%option noyywrap

/* Regex */
%%
	/* line Start */
(y)|(f\([a-z]\))			{ return LINESTART; }
=							{ return EQUALS; }
\<							{ return LESSTHAN; }
\>							{ return GREATERTHAN; }
\<=							{ return LTHANEQTO; }
\>=							{ return GTHANEQTO; }

	/* v */
	/* Numbers */
[0-9]+\.[0-9]+				{ yylval.fval = atof(yytext); return FLOAT; }
[0-9]+						{ yylval.ival = atoi(yytext); return INT; }

	/* greek letters */
alpha						{ return ALPHA; }
beta						{ return BETA; }
gamma						{ return GAMMA; }
Gamma						{ return GAMMACAP; }
delta						{ return DELTA; }
Delta						{ return DELTACAP; }
epsilon						{ return EPSILON; }
varepsilon					{ return VAREPSILON; }
zeta						{ return ZETA; }
eta							{ return ETA; }
theta						{ return THETA; }
Theta						{ return THETACAP; }
vartheta					{ return VARTHETA; }
iota						{ return IOTA; }
kappa						{ return KAPPA; }
lambda						{ return LAMBDA; }
Lambda						{ return LAMBDACAP; }
mu							{ return MU; }
nu							{ return NU; }
xi							{ return XI; }
Xi							{ return XICAP; }
pi							{ return PI; }
Pi							{ return PICAP; }
rho							{ return SIGMA; }
sigma						{ return SIGMACAP; }
Sigma						{ return TAU; }
tau							{ return UPSILON; }
upsilon						{ return PHI; }
phi							{ return PHICAP; }
Phi							{ return VARPHI; }
varphi						{ return CHI; }
chi							{ return PSI; }
psi							{ return PSICAP; }
Psi							{ return OMEGA; }
omega

	/* Other Constants */
e							{ return E; }
oo							{ return INFINITY; }

	/* u */
sqrt						{ return SQRT; }

log							{ return LOG; }
ln							{ return LN; }

abs							{ return ABS; }
floor						{ return FLOOR; }
ceil						{ return CEIL; }

arcsin						{ return ASIN; }
arccos						{ return ACOS; }
arctan						{ return ATAN; }
sinh						{ return SINH; }
cosh						{ return COSH; }
tanh						{ return TANH; }
sech						{ return SECH; }
csch						{ return CSCH; }
coth						{ return COTH; }
sin							{ return SIN; }
cos							{ return COS; }
tan							{ return TAN; }
sec							{ return SEC; }
csc							{ return CSC; }
cot							{ return COT; }

text						{ return TEXT; }
bb							{ return BB; }
hat							{ return HAT; }

	/* b */
frac						{ return FRAC; }
root						{ return ROOT; }

\+							{ return ADD; }
-							{ return SUB;}
xx							{ return MUL; }
\/							{ return DIV; }

\^							{ return EXP; }

	/* l */
\(							{ return LBRAC; }
[\[]						{ return LSBRAC; }
\{							{ return LCBRAC; }
\(:							{ return LANGLE; }
\<\<						{ return LABRAC; }

	/* r */
\)							{ return RBRAC; }
\]							{ return RSBRAC; }
\}							{ return RCBRAC; }
:\)							{ return RANGLE; }
\>\>						{ return RABRAC; }

[\n\r]						{ return ENDL; }
.							{ ; }
%%

#ifdef DEBUG_LEX
YYSTYPE yylval;

int main(void)
{
    int token;
    while ((token = yylex()) != 0)
        printf("Token: %d (%s)\n", token, yytext);
    return 0;
}
#endif
