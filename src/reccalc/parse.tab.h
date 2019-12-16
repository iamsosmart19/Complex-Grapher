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

#ifndef YY_YY_PARSE_TAB_H_INCLUDED
# define YY_YY_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 6 "parse.y"

  typedef void* yyscan_t;
  typedef struct
  {
    // Whether to print the intermediate results.
    int verbose;
    // Value of the last computation.
    int value;
    // Number of errors.
    int nerrs;
  } result;

#line 61 "parse.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_EOF = 0,
    TOK_PLUS = 258,
    TOK_MINUS = 259,
    TOK_STAR = 260,
    TOK_SLASH = 261,
    TOK_EOL = 262,
    TOK_NUM = 263,
    TOK_STR = 264,
    TOK_UNARY = 265
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* "string"  */
  char* TOK_STR;
  /* "number"  */
  int TOK_NUM;
  /* exp  */
  int TOK_exp;
#line 91 "parse.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t scanner, result *res);
/* "%code provides" blocks.  */
#line 21 "parse.y"

  // Tell Flex the expected prototype of yylex.
  // The scanner argument must be named yyscanner.
#define YY_DECL                                                         \
  enum yytokentype yylex (YYSTYPE* yylval, yyscan_t yyscanner, result *res)
  YY_DECL;

  void yyerror (yyscan_t scanner, result *res, const char *msg, ...);

#line 113 "parse.tab.h"

#endif /* !YY_YY_PARSE_TAB_H_INCLUDED  */