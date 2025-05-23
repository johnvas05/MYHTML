/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "myhtml.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IDS 100
#define MAX_LABELS 100

extern int yylex(void);
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);

/* Global variables for validation */
char* usedIds[MAX_IDS];
int usedIdsCount = 0;
char* labelInputMap[MAX_LABELS];
int labelInputMapCount = 0;
int hasError = 0;

/* Debug flag */
int debug = 1;

/* Function to display file contents */
void display_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    printf("\n=== Contents of file %s ===\n", filename);
    char buffer[1024];
    int line_num = 1;
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%3d | %s", line_num++, buffer);
    }
    printf("=== End of file ===\n\n");

    fclose(fp);
}

#line 114 "myhtml.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "myhtml.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_MYHTML_START = 3,               /* MYHTML_START  */
  YYSYMBOL_MYHTML_END = 4,                 /* MYHTML_END  */
  YYSYMBOL_HEAD_START = 5,                 /* HEAD_START  */
  YYSYMBOL_HEAD_END = 6,                   /* HEAD_END  */
  YYSYMBOL_BODY_START = 7,                 /* BODY_START  */
  YYSYMBOL_BODY_END = 8,                   /* BODY_END  */
  YYSYMBOL_TITLE_START = 9,                /* TITLE_START  */
  YYSYMBOL_TITLE_END = 10,                 /* TITLE_END  */
  YYSYMBOL_META_START = 11,                /* META_START  */
  YYSYMBOL_P_START = 12,                   /* P_START  */
  YYSYMBOL_P_END = 13,                     /* P_END  */
  YYSYMBOL_DIV_START = 14,                 /* DIV_START  */
  YYSYMBOL_DIV_END = 15,                   /* DIV_END  */
  YYSYMBOL_A_START = 16,                   /* A_START  */
  YYSYMBOL_A_END = 17,                     /* A_END  */
  YYSYMBOL_IMG_START = 18,                 /* IMG_START  */
  YYSYMBOL_FORM_START = 19,                /* FORM_START  */
  YYSYMBOL_FORM_END = 20,                  /* FORM_END  */
  YYSYMBOL_LABEL_START = 21,               /* LABEL_START  */
  YYSYMBOL_LABEL_END = 22,                 /* LABEL_END  */
  YYSYMBOL_INPUT_START = 23,               /* INPUT_START  */
  YYSYMBOL_CLOSE_TAG = 24,                 /* CLOSE_TAG  */
  YYSYMBOL_COMMENT_START = 25,             /* COMMENT_START  */
  YYSYMBOL_COMMENT_END = 26,               /* COMMENT_END  */
  YYSYMBOL_ID_ATTR = 27,                   /* ID_ATTR  */
  YYSYMBOL_STYLE_ATTR = 28,                /* STYLE_ATTR  */
  YYSYMBOL_HREF_ATTR = 29,                 /* HREF_ATTR  */
  YYSYMBOL_SRC_ATTR = 30,                  /* SRC_ATTR  */
  YYSYMBOL_ALT_ATTR = 31,                  /* ALT_ATTR  */
  YYSYMBOL_TYPE_ATTR = 32,                 /* TYPE_ATTR  */
  YYSYMBOL_VALUE_ATTR = 33,                /* VALUE_ATTR  */
  YYSYMBOL_FOR_ATTR = 34,                  /* FOR_ATTR  */
  YYSYMBOL_WIDTH_ATTR = 35,                /* WIDTH_ATTR  */
  YYSYMBOL_HEIGHT_ATTR = 36,               /* HEIGHT_ATTR  */
  YYSYMBOL_CHARSET_ATTR = 37,              /* CHARSET_ATTR  */
  YYSYMBOL_NAME_CONTENT_ATTR = 38,         /* NAME_CONTENT_ATTR  */
  YYSYMBOL_TEXT = 39,                      /* TEXT  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_document = 41,                  /* document  */
  YYSYMBOL_head_tag = 42,                  /* head_tag  */
  YYSYMBOL_body_tag = 43,                  /* body_tag  */
  YYSYMBOL_head_content = 44,              /* head_content  */
  YYSYMBOL_meta_tags = 45,                 /* meta_tags  */
  YYSYMBOL_title_tag = 46,                 /* title_tag  */
  YYSYMBOL_meta_tag = 47,                  /* meta_tag  */
  YYSYMBOL_meta_attributes = 48,           /* meta_attributes  */
  YYSYMBOL_body_content = 49,              /* body_content  */
  YYSYMBOL_body_element = 50,              /* body_element  */
  YYSYMBOL_comment = 51,                   /* comment  */
  YYSYMBOL_comment_text = 52,              /* comment_text  */
  YYSYMBOL_p_element = 53,                 /* p_element  */
  YYSYMBOL_a_element = 54,                 /* a_element  */
  YYSYMBOL_a_content = 55,                 /* a_content  */
  YYSYMBOL_img_element = 56,               /* img_element  */
  YYSYMBOL_img_attributes = 57,            /* img_attributes  */
  YYSYMBOL_form_element = 58,              /* form_element  */
  YYSYMBOL_form_content = 59,              /* form_content  */
  YYSYMBOL_form_item = 60,                 /* form_item  */
  YYSYMBOL_input_element = 61,             /* input_element  */
  YYSYMBOL_input_attributes = 62,          /* input_attributes  */
  YYSYMBOL_label_element = 63,             /* label_element  */
  YYSYMBOL_label_attributes = 64,          /* label_attributes  */
  YYSYMBOL_div_element = 65,               /* div_element  */
  YYSYMBOL_div_attributes = 66,            /* div_attributes  */
  YYSYMBOL_div_content = 67,               /* div_content  */
  YYSYMBOL_text = 68                       /* text  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   113

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  119

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    78,    78,    82,    85,    88,    92,    94,    98,   100,
     104,   108,   112,   113,   116,   117,   120,   121,   122,   123,
     124,   125,   126,   129,   132,   133,   143,   146,   149,   154,
     157,   158,   159,   160,   161,   164,   167,   168,   169,   170,
     171,   172,   175,   178,   179,   182,   183,   186,   189,   190,
     191,   192,   195,   198,   201,   202,   205,   208,   209,   210,
     213,   214,   217,   221
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "MYHTML_START",
  "MYHTML_END", "HEAD_START", "HEAD_END", "BODY_START", "BODY_END",
  "TITLE_START", "TITLE_END", "META_START", "P_START", "P_END",
  "DIV_START", "DIV_END", "A_START", "A_END", "IMG_START", "FORM_START",
  "FORM_END", "LABEL_START", "LABEL_END", "INPUT_START", "CLOSE_TAG",
  "COMMENT_START", "COMMENT_END", "ID_ATTR", "STYLE_ATTR", "HREF_ATTR",
  "SRC_ATTR", "ALT_ATTR", "TYPE_ATTR", "VALUE_ATTR", "FOR_ATTR",
  "WIDTH_ATTR", "HEIGHT_ATTR", "CHARSET_ATTR", "NAME_CONTENT_ATTR", "TEXT",
  "$accept", "document", "head_tag", "body_tag", "head_content",
  "meta_tags", "title_tag", "meta_tag", "meta_attributes", "body_content",
  "body_element", "comment", "comment_text", "p_element", "a_element",
  "a_content", "img_element", "img_attributes", "form_element",
  "form_content", "form_item", "input_element", "input_attributes",
  "label_element", "label_attributes", "div_element", "div_attributes",
  "div_content", "text", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-58)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       1,    11,    48,    44,    63,   -58,    35,    65,    64,   -58,
      72,   -58,    -9,   -58,    27,    64,   -58,    -5,   -58,   -58,
     -58,   -58,   -58,    53,   -58,   -58,    39,    41,    51,    20,
      52,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,    30,    54,    55,    57,    56,    58,    42,    29,    32,
      61,    62,    13,    35,    66,    67,   -58,   -58,   -58,    68,
      69,    59,    70,    71,    73,    75,   -58,    40,   -58,   -58,
      -8,    35,    35,    10,   -16,   -58,   -58,   -58,   -58,   -58,
     -58,   -19,    14,    22,   -58,   -58,   -58,   -58,    -7,    -3,
     -58,   -58,    76,    35,    -6,    60,    77,    81,    50,    79,
      83,   -58,   -58,   -58,   -58,   -58,    49,   -58,   -58,   -58,
       5,    78,    80,   -58,   -58,    16,   -58,   -58,   -58
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     0,     1,     0,     0,     7,    14,
       0,    62,     0,     3,     0,     6,     8,     0,     2,    10,
      63,    12,    13,     0,     9,     5,     0,     0,     0,     0,
       0,    24,    22,    15,    21,    16,    17,    18,    19,    20,
      11,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,     0,
       0,     0,     0,     0,     0,     0,    35,     0,    23,    25,
       0,     0,     0,     0,    30,    41,    40,    37,    36,    39,
      38,     0,     0,     0,    43,    45,    46,    26,     0,     0,
      56,    61,     0,    32,    31,     0,     0,     0,     0,     0,
       0,    42,    44,    27,    28,    29,    34,    33,    54,    55,
       0,    48,    49,    47,    53,     0,    50,    51,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -58,   -58,   -58,   -58,   -58,    82,   -58,   -58,
      23,   -58,   -58,   -58,   -58,   -58,   -57,   -58,   -58,   -58,
      12,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -53
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     4,    10,     7,    15,     8,    16,    23,    17,
      33,    34,    52,    35,    36,    92,    37,    50,    38,    83,
      84,    85,   100,    86,    97,    39,    45,    73,    12
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      70,    19,    29,    25,     1,    87,   103,    26,    95,    27,
     104,    28,    29,    29,    30,    96,     3,    93,    88,    89,
      31,    94,    26,    11,    27,    90,    28,   114,    29,    30,
      20,    20,    20,    20,    32,    31,    20,   107,   118,    68,
     106,    98,   101,    81,    11,    82,    99,    47,     5,    32,
      48,    49,    69,     6,    53,    20,    62,   115,    54,    64,
      63,    81,    65,    82,    21,    22,    41,    42,    43,    44,
       9,    13,    60,    61,    11,    14,    18,    40,    46,    51,
      58,    55,   111,    56,    57,    66,    67,    59,    20,    76,
      71,    72,    74,   105,   108,   102,    91,    24,    78,     0,
      75,    77,    80,    79,   109,   110,   112,   113,     0,     0,
       0,   116,     0,   117
};

static const yytype_int8 yycheck[] =
{
      53,    10,    18,     8,     3,    13,    13,    12,    27,    14,
      13,    16,    18,    18,    19,    34,     5,    74,    71,    72,
      25,    74,    12,    39,    14,    15,    16,    22,    18,    19,
      39,    39,    39,    39,    39,    25,    39,    94,    22,    26,
      93,    27,    20,    21,    39,    23,    32,    27,     0,    39,
      30,    31,    39,     9,    24,    39,    27,   110,    28,    27,
      31,    21,    30,    23,    37,    38,    27,    28,    27,    28,
       7,     6,    30,    31,    39,    11,     4,    24,    27,    27,
      24,    27,    32,    28,    27,    24,    24,    29,    39,    30,
      24,    24,    24,    17,    34,    83,    73,    15,    27,    -1,
      31,    31,    27,    30,    27,    24,    27,    24,    -1,    -1,
      -1,    33,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    41,     5,    42,     0,     9,    44,    46,     7,
      43,    39,    68,     6,    11,    45,    47,    49,     4,    10,
      39,    37,    38,    48,    47,     8,    12,    14,    16,    18,
      19,    25,    39,    50,    51,    53,    54,    56,    58,    65,
      24,    27,    28,    27,    28,    66,    27,    27,    30,    31,
      57,    27,    52,    24,    28,    27,    28,    27,    24,    29,
      30,    31,    27,    31,    27,    30,    24,    24,    26,    39,
      68,    24,    24,    67,    24,    31,    30,    31,    27,    30,
      27,    21,    23,    59,    60,    61,    63,    13,    68,    68,
      15,    50,    55,    56,    68,    27,    34,    64,    27,    32,
      62,    20,    60,    13,    13,    17,    68,    56,    34,    27,
      24,    32,    27,    24,    22,    68,    33,    33,    22
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    44,    44,    45,    45,
      46,    47,    48,    48,    49,    49,    50,    50,    50,    50,
      50,    50,    50,    51,    52,    52,    53,    53,    53,    54,
      55,    55,    55,    55,    55,    56,    57,    57,    57,    57,
      57,    57,    58,    59,    59,    60,    60,    61,    62,    62,
      62,    62,    63,    63,    64,    64,    65,    66,    66,    66,
      67,    67,    68,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     3,     0,     3,     2,     1,     1,     2,
       3,     3,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     2,     5,     6,     6,     6,
       0,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     5,     1,     2,     1,     1,     3,     2,     2,
       3,     3,     5,     4,     2,     2,     5,     1,     2,     2,
       0,     2,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* document: MYHTML_START head_tag body_tag MYHTML_END  */
#line 79 "myhtml.y"
        { if (debug) printf("Successfully parsed MYHTML document\n"); }
#line 1249 "myhtml.tab.c"
    break;

  case 3: /* head_tag: HEAD_START head_content HEAD_END  */
#line 83 "myhtml.y"
        { if (debug) printf("Parsed head section\n"); }
#line 1255 "myhtml.tab.c"
    break;

  case 4: /* head_tag: %empty  */
#line 85 "myhtml.y"
        { if (debug) printf("Empty head section\n"); }
#line 1261 "myhtml.tab.c"
    break;

  case 5: /* body_tag: BODY_START body_content BODY_END  */
#line 89 "myhtml.y"
        { if (debug) printf("Parsed body section\n"); }
#line 1267 "myhtml.tab.c"
    break;

  case 6: /* head_content: title_tag meta_tags  */
#line 93 "myhtml.y"
            { if (debug) printf("Parsed head content with meta tags\n"); }
#line 1273 "myhtml.tab.c"
    break;

  case 7: /* head_content: title_tag  */
#line 95 "myhtml.y"
            { if (debug) printf("Parsed head content with title only\n"); }
#line 1279 "myhtml.tab.c"
    break;

  case 8: /* meta_tags: meta_tag  */
#line 99 "myhtml.y"
        { if (debug) printf("Parsed single meta tag\n"); }
#line 1285 "myhtml.tab.c"
    break;

  case 9: /* meta_tags: meta_tags meta_tag  */
#line 101 "myhtml.y"
        { if (debug) printf("Parsed additional meta tag\n"); }
#line 1291 "myhtml.tab.c"
    break;

  case 10: /* title_tag: TITLE_START text TITLE_END  */
#line 105 "myhtml.y"
        { if (debug) printf("Parsed title: %s\n", (yyvsp[-1].string_val)); free((yyvsp[-1].string_val)); }
#line 1297 "myhtml.tab.c"
    break;

  case 11: /* meta_tag: META_START meta_attributes CLOSE_TAG  */
#line 109 "myhtml.y"
       { if (debug) printf("Parsed meta tag\n"); }
#line 1303 "myhtml.tab.c"
    break;

  case 22: /* body_element: TEXT  */
#line 126 "myhtml.y"
                  { free((yyvsp[0].string_val)); }
#line 1309 "myhtml.tab.c"
    break;

  case 24: /* comment_text: %empty  */
#line 132 "myhtml.y"
                          { (yyval.string_val) = strdup(""); }
#line 1315 "myhtml.tab.c"
    break;

  case 25: /* comment_text: comment_text TEXT  */
#line 133 "myhtml.y"
                               { 
               char* new_text = malloc(strlen((yyvsp[-1].string_val)) + strlen((yyvsp[0].string_val)) + 1);
               strcpy(new_text, (yyvsp[-1].string_val));
               strcat(new_text, (yyvsp[0].string_val));
               free((yyvsp[-1].string_val));
               free((yyvsp[0].string_val));
               (yyval.string_val) = new_text;
           }
#line 1328 "myhtml.tab.c"
    break;

  case 26: /* p_element: P_START ID_ATTR CLOSE_TAG text P_END  */
#line 143 "myhtml.y"
                                                {
            free((yyvsp[-1].string_val));
         }
#line 1336 "myhtml.tab.c"
    break;

  case 27: /* p_element: P_START ID_ATTR STYLE_ATTR CLOSE_TAG text P_END  */
#line 146 "myhtml.y"
                                                           {
            free((yyvsp[-1].string_val));
         }
#line 1344 "myhtml.tab.c"
    break;

  case 28: /* p_element: P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END  */
#line 149 "myhtml.y"
                                                           {
            free((yyvsp[-1].string_val));
         }
#line 1352 "myhtml.tab.c"
    break;

  case 52: /* label_element: LABEL_START label_attributes CLOSE_TAG text LABEL_END  */
#line 195 "myhtml.y"
                                                                     {
                free((yyvsp[-1].string_val));
             }
#line 1360 "myhtml.tab.c"
    break;

  case 62: /* text: TEXT  */
#line 217 "myhtml.y"
           {
        (yyval.string_val) = strdup((yyvsp[0].string_val));
        free((yyvsp[0].string_val));
    }
#line 1369 "myhtml.tab.c"
    break;

  case 63: /* text: text TEXT  */
#line 221 "myhtml.y"
                {
        char* new_text = malloc(strlen((yyvsp[-1].string_val)) + strlen((yyvsp[0].string_val)) + 1);
        strcpy(new_text, (yyvsp[-1].string_val));
        strcat(new_text, (yyvsp[0].string_val));
        free((yyvsp[-1].string_val));
        free((yyvsp[0].string_val));
        (yyval.string_val) = new_text;
    }
#line 1382 "myhtml.tab.c"
    break;


#line 1386 "myhtml.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 231 "myhtml.y"


void yyerror(const char *s) {
    hasError = 1;
    fprintf(stderr, "Syntax error at line %d: %s\nNear text: '%s'\n", yylineno, s, yytext);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }
    
    // Display the input file contents first
    display_file(argv[1]);
    
    extern FILE *yyin;
    yyin = input;
    
    int result = yyparse();
    
    if (result == 0 && !hasError) {
        printf("Analysis result: File %s is syntactically correct.\n", argv[1]);
    }
    
    /* Clean up allocated memory */
    for(int i = 0; i < usedIdsCount; i++) {
        free(usedIds[i]);
    }
    for(int i = 0; i < labelInputMapCount; i++) {
        free(labelInputMap[i]);
    }
    
    fclose(input);
    return (result || hasError);
} 
