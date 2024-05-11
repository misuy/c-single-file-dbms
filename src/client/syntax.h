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

#ifndef YY_YY_SRC_CLIENT_SYNTAX_H_INCLUDED
# define YY_YY_SRC_CLIENT_SYNTAX_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LBRACE = 258,                  /* LBRACE  */
    RBRACE = 259,                  /* RBRACE  */
    DOT = 260,                     /* DOT  */
    COMMA = 261,                   /* COMMA  */
    ASTERISK = 262,                /* ASTERISK  */
    SEMICOLON = 263,               /* SEMICOLON  */
    FROM = 264,                    /* FROM  */
    IN = 265,                      /* IN  */
    WHERE = 266,                   /* WHERE  */
    SELECT = 267,                  /* SELECT  */
    INSERT = 268,                  /* INSERT  */
    DELETE = 269,                  /* DELETE  */
    UPDATE = 270,                  /* UPDATE  */
    JOIN = 271,                    /* JOIN  */
    ON = 272,                      /* ON  */
    CREATE = 273,                  /* CREATE  */
    VARIABLE = 274,                /* VARIABLE  */
    BOOL_CONSTANT = 275,           /* BOOL_CONSTANT  */
    INTEGER_CONSTANT = 276,        /* INTEGER_CONSTANT  */
    FLOAT_CONSTANT = 277,          /* FLOAT_CONSTANT  */
    STRING_CONSTANT = 278,         /* STRING_CONSTANT  */
    BOOL_TYPE = 279,               /* BOOL_TYPE  */
    UINT_8_TYPE = 280,             /* UINT_8_TYPE  */
    UINT_16_TYPE = 281,            /* UINT_16_TYPE  */
    UINT_32_TYPE = 282,            /* UINT_32_TYPE  */
    INT_32_TYPE = 283,             /* INT_32_TYPE  */
    FLOAT_TYPE = 284,              /* FLOAT_TYPE  */
    STRING_TYPE = 285,             /* STRING_TYPE  */
    ASSIGN = 286,                  /* ASSIGN  */
    AND = 287,                     /* AND  */
    OR = 288,                      /* OR  */
    EQUAL = 289,                   /* EQUAL  */
    NOT_EQUAL = 290,               /* NOT_EQUAL  */
    LESS = 291,                    /* LESS  */
    NOT_LESS = 292,                /* NOT_LESS  */
    GREATER = 293,                 /* GREATER  */
    NOT_GREATER = 294,             /* NOT_GREATER  */
    SUBSTR = 295,                  /* SUBSTR  */
    NOT = 296                      /* NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct Node *  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif




#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

struct FiniteAutomata;
struct Text;
struct Node;

int yyparse (struct FiniteAutomata *automata, struct Text *text, struct Node **result_node);
int yypush_parse (yypstate *ps,
                  int pushed_char, YYSTYPE const *pushed_val, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node);
int yypull_parse (yypstate *ps, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node);
yypstate *yypstate_new (void);
void yypstate_delete (yypstate *ps);


#endif /* !YY_YY_SRC_CLIENT_SYNTAX_H_INCLUDED  */
