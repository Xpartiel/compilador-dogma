/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 34 "parser.y"

    #include "ast.h"
    #include "type.h"
    #include "semantic.h"

#line 55 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    LIT_STRING = 259,              /* LIT_STRING  */
    ID = 260,                      /* ID  */
    ASSIGN = 261,                  /* ASSIGN  */
    PARENTHESES_OPEN = 262,        /* PARENTHESES_OPEN  */
    PARENTHESES_CLOSE = 263,       /* PARENTHESES_CLOSE  */
    BEGIN_LOOP = 264,              /* BEGIN_LOOP  */
    END_LOOP = 265,                /* END_LOOP  */
    BREAK = 266,                   /* BREAK  */
    CONTINUE = 267,                /* CONTINUE  */
    RETURN_VALUE = 268,            /* RETURN_VALUE  */
    RETURN_TYPE = 269,             /* RETURN_TYPE  */
    IF = 270,                      /* IF  */
    ELSE_IF = 271,                 /* ELSE_IF  */
    ELSE = 272,                    /* ELSE  */
    TYPE_DOUBLE = 273,             /* TYPE_DOUBLE  */
    TYPE_FLOAT = 274,              /* TYPE_FLOAT  */
    TYPE_INTEGER = 275,            /* TYPE_INTEGER  */
    TYPE_STRING = 276,             /* TYPE_STRING  */
    TYPE_BOOLEAN = 277,            /* TYPE_BOOLEAN  */
    TYPE_LIST = 278,               /* TYPE_LIST  */
    TYPE_ARRAY = 279,              /* TYPE_ARRAY  */
    ERROR = 280,                   /* ERROR  */
    LIST_SEPARATOR = 281,          /* LIST_SEPARATOR  */
    BEGIN_SEQUENCE = 282,          /* BEGIN_SEQUENCE  */
    SEQUENCE_SEPARATOR = 283,      /* SEQUENCE_SEPARATOR  */
    END_SEQUENCE = 284,            /* END_SEQUENCE  */
    OP_EQUALS = 285,               /* OP_EQUALS  */
    OP_LESSER = 286,               /* OP_LESSER  */
    OP_GREATER = 287,              /* OP_GREATER  */
    OP_GREATER_EQUAL = 288,        /* OP_GREATER_EQUAL  */
    OP_LESSER_EQUAL = 289,         /* OP_LESSER_EQUAL  */
    OP_PLUS = 290,                 /* OP_PLUS  */
    OP_MINUS = 291,                /* OP_MINUS  */
    OP_TIMES = 292,                /* OP_TIMES  */
    OP_DIVIDE = 293,               /* OP_DIVIDE  */
    BOOL_AND = 294,                /* BOOL_AND  */
    BOOL_OR = 295,                 /* BOOL_OR  */
    BOOL_NOT = 296,                /* BOOL_NOT  */
    UMINUS = 297                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 41 "parser.y"

    double num; /* NUMBER token */
    char* id;   /* ID token */
    char* str;  /* LIT_STRING token */
    int boolean;
    ASTNode* node;  /* para no-terminales que producen AST */
    Type* type;     /* identificador de tipos */

#line 123 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
