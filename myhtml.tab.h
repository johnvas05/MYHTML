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

#ifndef YY_YY_MYHTML_TAB_H_INCLUDED
# define YY_YY_MYHTML_TAB_H_INCLUDED
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
    MYHTML_START = 258,            /* MYHTML_START  */
    MYHTML_END = 259,              /* MYHTML_END  */
    HEAD_START = 260,              /* HEAD_START  */
    HEAD_END = 261,                /* HEAD_END  */
    BODY_START = 262,              /* BODY_START  */
    BODY_END = 263,                /* BODY_END  */
    TITLE_START = 264,             /* TITLE_START  */
    TITLE_END = 265,               /* TITLE_END  */
    META_START = 266,              /* META_START  */
    P_START = 267,                 /* P_START  */
    P_END = 268,                   /* P_END  */
    DIV_START = 269,               /* DIV_START  */
    DIV_END = 270,                 /* DIV_END  */
    A_START = 271,                 /* A_START  */
    A_END = 272,                   /* A_END  */
    IMG_START = 273,               /* IMG_START  */
    FORM_START = 274,              /* FORM_START  */
    FORM_END = 275,                /* FORM_END  */
    LABEL_START = 276,             /* LABEL_START  */
    LABEL_END = 277,               /* LABEL_END  */
    INPUT_START = 278,             /* INPUT_START  */
    CLOSE_TAG = 279,               /* CLOSE_TAG  */
    COMMENT_START = 280,           /* COMMENT_START  */
    COMMENT_END = 281,             /* COMMENT_END  */
    ID_ATTR = 282,                 /* ID_ATTR  */
    STYLE_ATTR = 283,              /* STYLE_ATTR  */
    HREF_ATTR = 284,               /* HREF_ATTR  */
    SRC_ATTR = 285,                /* SRC_ATTR  */
    ALT_ATTR = 286,                /* ALT_ATTR  */
    TYPE_ATTR = 287,               /* TYPE_ATTR  */
    VALUE_ATTR = 288,              /* VALUE_ATTR  */
    FOR_ATTR = 289,                /* FOR_ATTR  */
    WIDTH_ATTR = 290,              /* WIDTH_ATTR  */
    HEIGHT_ATTR = 291,             /* HEIGHT_ATTR  */
    CHECKBOX_COUNT_ATTR = 292,     /* CHECKBOX_COUNT_ATTR  */
    CHARSET_ATTR = 293,            /* CHARSET_ATTR  */
    NAME_CONTENT_ATTR = 294,       /* NAME_CONTENT_ATTR  */
    TEXT = 295                     /* TEXT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 543 "myhtml.y"

    char* string_val;
    struct {
        char* attributes[10];
        int attr_count;
    } attr_list;

#line 112 "myhtml.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MYHTML_TAB_H_INCLUDED  */
