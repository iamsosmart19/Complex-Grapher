/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
		//Address of error
		char* err;
	} result;
	result parse_string(const char* str, queue* out);
	result parse(void);

#line 69 "src/parse.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    TOK_EOF = 0,                   /* "end-of-file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    COMMENT = 258,                 /* "//"  */
    PLUS = 259,                    /* "+"  */
    MINUS = 260,                   /* "-"  */
    STAR = 261,                    /* "*"  */
    SLASH = 262,                   /* "/"  */
    EXP = 263,                     /* "^"  */
    UNDERSCORE = 264,              /* "_"  */
    SQRT = 265,                    /* "sqrt"  */
    ROOT = 266,                    /* "root"  */
    LN = 267,                      /* "ln"  */
    LOG = 268,                     /* "log"  */
    E = 269,                       /* "e"  */
    ABS = 270,                     /* "abs"  */
    FLOOR = 271,                   /* "floor"  */
    CEIL = 272,                    /* "ceil"  */
    REAL = 273,                    /* "real"  */
    IMAG = 274,                    /* "imag"  */
    ASIN = 275,                    /* "asin"  */
    ACOS = 276,                    /* "acos"  */
    ATAN = 277,                    /* "atan"  */
    SINH = 278,                    /* "sinh"  */
    COSH = 279,                    /* "cosh"  */
    TANH = 280,                    /* "tanh"  */
    SECH = 281,                    /* "sech"  */
    CSCH = 282,                    /* "csch"  */
    COTH = 283,                    /* "coth"  */
    SIN = 284,                     /* "sin"  */
    COS = 285,                     /* "cos"  */
    TAN = 286,                     /* "tan"  */
    SEC = 287,                     /* "sec"  */
    CSC = 288,                     /* "csc"  */
    COT = 289,                     /* "cot"  */
    PI = 290,                      /* "pi"  */
    MI = 291,                      /* "i"  */
    Y = 292,                       /* "y"  */
    FZ = 293,                      /* "f(z)"  */
    EQUALS = 294,                  /* "="  */
    LESSTHAN = 295,                /* "<"  */
    GREATERTHAN = 296,             /* ">"  */
    LTHANEQTO = 297,               /* "<="  */
    GTHANEQTO = 298,               /* ">="  */
    LETR = 299,                    /* "z"  */
    LBRAC = 300,                   /* "("  */
    RBRAC = 301,                   /* ")"  */
    CONJ = 302,                    /* "|"  */
    COMMA = 303,                   /* ","  */
    EOL = 304,                     /* "end-of-line"  */
    NUM = 305,                     /* "number"  */
    BINARY = 306,                  /* BINARY  */
    UNARY = 307                    /* UNARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
  cplx NUM;                                /* "number"  */

#line 140 "src/parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t scanner, result *res, queue* out);
/* "%code provides" blocks.  */
#line 26 "src/parse.y"

	// Tell Flex the expected prototype of yylex.
	// The scanner argument must be named yyscanner.
	#define YY_DECL enum yytokentype yylex(YYSTYPE* yylval, yyscan_t yyscanner, result *res, queue *out)
	YY_DECL;

	/* void yyerror(yyscan_t scanner, result *res, const char *msg, ...); */
	void yyerror(yyscan_t scanner, result *res, queue *out, const char *msg, ...);

#line 162 "src/parse.h"

#endif /* !YY_YY_SRC_PARSE_H_INCLUDED  */
