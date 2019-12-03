/* A Bison parser, made by GNU Bison 3.4.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    LINESTART = 260,
    EQUALS = 261,
    LESSTHAN = 262,
    GREATERTHAN = 263,
    LTHANEQTO = 264,
    GTHANEQTO = 265,
    ALPHA = 266,
    BETA = 267,
    GAMMA = 268,
    GAMMACAP = 269,
    DELTA = 270,
    DELTACAP = 271,
    EPSILON = 272,
    VAREPSILON = 273,
    ZETA = 274,
    ETA = 275,
    THETA = 276,
    THETACAP = 277,
    VARTHETA = 278,
    IOTA = 279,
    KAPPA = 280,
    LAMBDA = 281,
    LAMBDACAP = 282,
    MU = 283,
    NU = 284,
    XI = 285,
    XICAP = 286,
    PI = 287,
    PICAP = 288,
    SIGMA = 289,
    SIGMACAP = 290,
    TAU = 291,
    UPSILON = 292,
    PHI = 293,
    PHICAP = 294,
    VARPHI = 295,
    CHI = 296,
    PSI = 297,
    PSICAP = 298,
    OMEGA = 299,
    E = 300,
    INFINITY = 301,
    SQRT = 302,
    LOG = 303,
    LN = 304,
    ABS = 305,
    FLOOR = 306,
    CEIL = 307,
    ASIN = 308,
    ACOS = 309,
    ATAN = 310,
    SINH = 311,
    COSH = 312,
    TANH = 313,
    SECH = 314,
    CSCH = 315,
    COTH = 316,
    SIN = 317,
    COS = 318,
    TAN = 319,
    SEC = 320,
    CSC = 321,
    COT = 322,
    TEXT = 323,
    BB = 324,
    HAT = 325,
    FRAC = 326,
    ROOT = 327,
    ADD = 328,
    SUB = 329,
    MUL = 330,
    DIV = 331,
    NEG = 332,
    BRACK = 333,
    EXP = 334,
    LBRAC = 335,
    LSBRAC = 336,
    LCBRAC = 337,
    LANGLE = 338,
    LABRAC = 339,
    RBRAC = 340,
    RSBRAC = 341,
    RCBRAC = 342,
    RANGLE = 343,
    RABRAC = 344,
    ENDL = 345
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "parser.y"

	int ival;
	float fval;

#line 153 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
