/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_NUM = 258,
     T_INT = 259,
     T_EXTENDS = 260,
     T_OPENCURLY = 261,
     T_CLOSECURLY = 262,
     T_OPENPAREN = 263,
     T_CLOSEPAREN = 264,
     T_RETURN = 265,
     T_ID = 266,
     T_BOOL = 267,
     T_IF = 268,
     T_ELSE = 269,
     T_WHILE = 270,
     T_PRINT = 271,
     T_DOT = 272,
     T_OR = 273,
     T_AND = 274,
     T_LESSEQUAL = 275,
     T_LESS = 276,
     T_PLUS = 277,
     T_MINUS = 278,
     T_TIMES = 279,
     T_DIV = 280,
     T_NOT = 281,
     T_ARROW = 282,
     T_EQUAL = 283,
     T_NONE = 284,
     T_NEW = 285,
     T_COMMA = 286,
     T_TRUE = 287,
     T_FALSE = 288,
     T_COLON = 289,
     T_E = 290
   };
#endif
/* Tokens.  */
#define T_NUM 258
#define T_INT 259
#define T_EXTENDS 260
#define T_OPENCURLY 261
#define T_CLOSECURLY 262
#define T_OPENPAREN 263
#define T_CLOSEPAREN 264
#define T_RETURN 265
#define T_ID 266
#define T_BOOL 267
#define T_IF 268
#define T_ELSE 269
#define T_WHILE 270
#define T_PRINT 271
#define T_DOT 272
#define T_OR 273
#define T_AND 274
#define T_LESSEQUAL 275
#define T_LESS 276
#define T_PLUS 277
#define T_MINUS 278
#define T_TIMES 279
#define T_DIV 280
#define T_NOT 281
#define T_ARROW 282
#define T_EQUAL 283
#define T_NONE 284
#define T_NEW 285
#define T_COMMA 286
#define T_TRUE 287
#define T_FALSE 288
#define T_COLON 289
#define T_E 290




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

