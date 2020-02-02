/* A Bison parser, made by GNU Bison 3.5.  */

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

#ifndef YY_YY_SRC_PARSE_H_INCLUDED
# define YY_YY_SRC_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 5 "src/parse.y"

	#include <complex.h>
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
	result parse_string(const char* str, queue* out, int* tknCnt);
	result parse(void);

#line 66 "src/parse.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_EOF = 0,
    COMMENT = 258,
    PLUS = 259,
    MINUS = 260,
    STAR = 261,
    SLASH = 262,
    EXP = 263,
    UNDERSCORE = 264,
    SQRT = 265,
    ROOT = 266,
    LN = 267,
    LOG = 268,
    E = 269,
    ABS = 270,
    FLOOR = 271,
    CEIL = 272,
    ASIN = 273,
    ACOS = 274,
    ATAN = 275,
    SINH = 276,
    COSH = 277,
    TANH = 278,
    SECH = 279,
    CSCH = 280,
    COTH = 281,
    SIN = 282,
    COS = 283,
    TAN = 284,
    SEC = 285,
    CSC = 286,
    COT = 287,
    PI = 288,
    MI = 289,
    Y = 290,
    FZ = 291,
    EQUALS = 292,
    LESSTHAN = 293,
    GREATERTHAN = 294,
    LTHANEQTO = 295,
    GTHANEQTO = 296,
    LETR = 297,
    LBRAC = 298,
    RBRAC = 299,
    EOL = 300,
    NUM = 301,
    BINARY = 302,
    UNARY = 303
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* "number"  */
  cplx NUM;
#line 130 "src/parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t scanner, result *res, queue* out, int* tknCnt);
/* "%code provides" blocks.  */
#line 24 "src/parse.y"

	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res, queue *out, int* tknCnt)
	YY_DECL;

	/* void yyerror(yyscan_t scanner, result *res, const char *msg, ...); */
	void yyerror(yyscan_t scanner, result *res, queue *out, int* tknCnt, const char *msg, ...);

#line 152 "src/parse.h"

#endif /* !YY_YY_SRC_PARSE_H_INCLUDED  */
