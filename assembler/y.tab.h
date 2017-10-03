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
     CLASS = 258,
     FIELD = 259,
     METHOD = 260,
     LOCALS = 261,
     LPAR = 262,
     RPAR = 263,
     LPAR2 = 264,
     RPAR2 = 265,
     LPAR3 = 266,
     RPAR3 = 267,
     LPAR4 = 268,
     RPAR4 = 269,
     COMMA = 270,
     COLON = 271,
     SEMICOLON = 272,
     EMPTY = 273,
     TYPE_INT = 274,
     TYPE_BOOL = 275,
     TYPE_CHAR = 276,
     TYPE_THREAD = 277,
     TYPE_LOCK = 278,
     LDINT = 279,
     LDCHAR = 280,
     LDBOOL = 281,
     ADD = 282,
     SUB = 283,
     MUL = 284,
     DIV = 285,
     LDARG = 286,
     LDLOC = 287,
     STLOC = 288,
     FORK = 289,
     WAIT = 290,
     SHARED = 291,
     SYNC = 292,
     NEWLOCK = 293,
     POP = 294,
     NOP = 295,
     BRFALSE = 296,
     BR = 297,
     MOD = 298,
     CEQ = 299,
     CGT = 300,
     CLT = 301,
     AND = 302,
     OR = 303,
     NOT = 304,
     NEWARR = 305,
     LDLEN = 306,
     LDELEM = 307,
     STELEM = 308,
     NEWOBJ = 309,
     CALL = 310,
     LDFLD = 311,
     STFLD = 312,
     PRINTINT = 313,
     PRINTCHAR = 314,
     PRINTBOOL = 315,
     DUP = 316,
     RET = 317,
     INT = 318,
     BOOL = 319,
     CHAR = 320,
     ID = 321
   };
#endif
/* Tokens.  */
#define CLASS 258
#define FIELD 259
#define METHOD 260
#define LOCALS 261
#define LPAR 262
#define RPAR 263
#define LPAR2 264
#define RPAR2 265
#define LPAR3 266
#define RPAR3 267
#define LPAR4 268
#define RPAR4 269
#define COMMA 270
#define COLON 271
#define SEMICOLON 272
#define EMPTY 273
#define TYPE_INT 274
#define TYPE_BOOL 275
#define TYPE_CHAR 276
#define TYPE_THREAD 277
#define TYPE_LOCK 278
#define LDINT 279
#define LDCHAR 280
#define LDBOOL 281
#define ADD 282
#define SUB 283
#define MUL 284
#define DIV 285
#define LDARG 286
#define LDLOC 287
#define STLOC 288
#define FORK 289
#define WAIT 290
#define SHARED 291
#define SYNC 292
#define NEWLOCK 293
#define POP 294
#define NOP 295
#define BRFALSE 296
#define BR 297
#define MOD 298
#define CEQ 299
#define CGT 300
#define CLT 301
#define AND 302
#define OR 303
#define NOT 304
#define NEWARR 305
#define LDLEN 306
#define LDELEM 307
#define STELEM 308
#define NEWOBJ 309
#define CALL 310
#define LDFLD 311
#define STFLD 312
#define PRINTINT 313
#define PRINTCHAR 314
#define PRINTBOOL 315
#define DUP 316
#define RET 317
#define INT 318
#define BOOL 319
#define CHAR 320
#define ID 321




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 34 "Assembler.y"
{
	int integer;
	int bool;
	char byte;
	char* string;
	struct type* type;
	struct stack* stack;
}
/* Line 1529 of yacc.c.  */
#line 190 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

