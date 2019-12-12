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

#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 5 "parse.y"

	typedef void* yyscan_t;
	typedef struct {
		// Whether to print the intermediate results.
		int verbose;
		// Value of the last computation.
		float value;
		// Number of errors.
		int nerrs;
	} result;

#line 60 "parse.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_EOF = 0,
    PLUS = 258,
    MINUS = 259,
    STAR = 260,
    SLASH = 261,
    EXP = 262,
    UNDERSCORE = 263,
    SQRT = 264,
    ROOT = 265,
    LN = 266,
    LOG = 267,
    E = 268,
    ABS = 269,
    FLOOR = 270,
    CEIL = 271,
    ASIN = 272,
    ACOS = 273,
    ATAN = 274,
    SINH = 275,
    COSH = 276,
    TANH = 277,
    SECH = 278,
    CSCH = 279,
    COTH = 280,
    SIN = 281,
    COS = 282,
    TAN = 283,
    SEC = 284,
    CSC = 285,
    COT = 286,
    PI = 287,
    Y = 288,
    FZ = 289,
    LETR = 290,
    EQUALS = 291,
    LESSTHAN = 292,
    GREATERTHAN = 293,
    LTHANEQTO = 294,
    GTHANEQTO = 295,
    EOL = 296,
    NUM = 297,
    STR = 298,
    UNARY = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* "string"  */
  char* STR;
  /* "number"  */
  float NUM;
  /* eqtn  */
  float eqtn;
  /* exp  */
  float exp;
  /* sexp  */
  float sexp;
#line 128 "parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t scanner, result *res);
/* "%code provides" blocks.  */
#line 18 "parse.y"

	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res)
	YY_DECL;

	void yyerror(yyscan_t scanner, result *res, const char *msg, ...);

#line 149 "parse.h"

#endif /* !YY_YY_PARSE_H_INCLUDED  */
