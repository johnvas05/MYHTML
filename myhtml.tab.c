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
#include <ctype.h>

#define MAX_IDS 100
#define MAX_TITLE_LENGTH 60

extern int yylex(void);
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);

char* usedIds[MAX_IDS];
int usedIdsCount = 0;
int hasError = 0;

/* Global variables για τον έλεγχο του input type */
int submitFound = 0;        /* Υπάρχει ήδη κάποιο input τύπου submit μέσα στη φόρμα */
int lastInputWasSubmit = 0;   /* Το τελευταίο input που διαβάστηκε στο περιεχόμενο της φόρμας είναι submit; */

/* Global variables για τον έλεγχο των checkbox */
int expectedCheckboxCount = 0;  /* Η αναμενόμενη τιμή από το checkbox_count */
int actualCheckboxCount = 0;    /* Ο πραγματικός αριθμός των checkbox που βρέθηκαν */
int hasCheckbox = 0;           /* Flag για το αν υπάρχει τουλάχιστον ένα checkbox */

/* Global variables για τον έλεγχο label-input */
char* inputIds[MAX_IDS];      /* Αποθήκευση των IDs των input elements */
char* labelFors[MAX_IDS];     /* Αποθήκευση των for attributes των labels */
int labelLines[MAX_IDS];      /* Αποθήκευση των γραμμών των labels για μηνύματα σφάλματος */
int inputIdsCount = 0;
int labelForsCount = 0;

/* Γενική συνάρτηση εξαγωγής τιμής από attribute (αναζητά το πρώτο ζευγάρι εισαγωγικών) */
char* extract_value(const char* attr_str) {
    const char* start = strchr(attr_str, '"');
    if (!start) return NULL;
    start++;  // Περνάμε το πρώτο "
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    int len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    return value;
}

/* Έλεγχος για διπλότυπο ID */
int is_id_used(const char* id) {
    for (int i = 0; i < usedIdsCount; i++) {
        if (strcmp(usedIds[i], id) == 0)
            return 1;
    }
    return 0;
}

/* Επικύρωση και αποθήκευση μοναδικού ID */
void validate_id(const char* attr) {
    char* id = extract_value(attr);
    if (!id) return;
    if (is_id_used(id)) {
        fprintf(stderr, "Error at line %d: Duplicate ID '%s'.\n", yylineno, id);
        exit(1);
    } else {
        if (usedIdsCount < MAX_IDS)
            usedIds[usedIdsCount++] = strdup(id);
        else
            fprintf(stderr, "Warning: Maximum number of IDs reached.\n");
    }
    free(id);
}

/* Βασική επικύρωση URL. Δέχεται απόλυτα URLs (http, https, ftp, file)
   ή, για σχετικές τιμές, ελέγχει μόνο τους επιτρεπόμενους χαρακτήρες */
void validate_url(const char* attr, const char* name) {
    char* url = extract_value(attr);
    if (url) {
        if (!(strncmp(url, "http://", 7) == 0 ||
              strncmp(url, "https://", 8) == 0 ||
              strncmp(url, "ftp://", 6) == 0 ||
              strncmp(url, "file://", 7) == 0)) {
            for (int i = 0; url[i]; i++) {
                if (!isalnum(url[i]) &&
                    strchr("/.-_~%", url[i]) == NULL) {
                    fprintf(stderr, "Error at line %d: Invalid character in %s URL: '%c'\n", 
                            yylineno, name, url[i]);
                    exit(1);
                }
            }
        }
    }
    free(url);
}

/* Επικύρωση του href: αν ξεκινά με '#' πρέπει να δείχνει σε υπάρχον ID */
void validate_href(const char* attr) {
    char* href = extract_value(attr);
    if (href) {
        if (href[0] == '#') {
            char* ref_id = href + 1;
            if (!is_id_used(ref_id)) {
                fprintf(stderr, "Error at line %d: href references non-existent ID '%s'\n", 
                        yylineno, ref_id);
                exit(1);
            }
        } else {
            validate_url(attr, "href");
        }
    }
    free(href);
}

/* Επικύρωση του src */
void validate_src(const char* attr) {
    validate_url(attr, "src");
}

/* Επικύρωση μήκους τίτλου */
void validate_title_length(const char* title) {
    if (strlen(title) > MAX_TITLE_LENGTH) {
        fprintf(stderr, "Error at line %d: Title length exceeds %d characters (current length: %lu).\n",
                yylineno, MAX_TITLE_LENGTH, strlen(title));
        exit(1);
    }
}

/* Επικύρωση του checkbox_count attribute */
void validate_checkbox_count(const char* attr) {
    char* count_str = extract_value(attr);
    if (!count_str) {
        fprintf(stderr, "Error at line %d: Unable to extract checkbox_count value.\n", yylineno);
        exit(1);
    }

    /* Έλεγχος αν είναι θετικός ακέραιος */
    for (int i = 0; count_str[i]; i++) {
        if (!isdigit(count_str[i])) {
            fprintf(stderr, "Error at line %d: checkbox_count must be a positive integer.\n", yylineno);
            free(count_str);
            exit(1);
        }
    }

    expectedCheckboxCount = atoi(count_str);
    if (expectedCheckboxCount <= 0) {
        fprintf(stderr, "Error at line %d: checkbox_count must be a positive integer.\n", yylineno);
        free(count_str);
        exit(1);
    }

    free(count_str);
}

/* Επικύρωση του input type.
   Επιτρεπόμενες τιμές: "text", "checkbox", "radio", "submit". 
   Επιστρέφει 1 αν είναι submit, διαφορετικά 0.
   Επίσης, ελέγχει για μοναδικότητα του submit input. */
int validate_input_type(const char* attr) {
    char* typeValue = extract_value(attr);
    if (!typeValue) {
        fprintf(stderr, "Error at line %d: Unable to extract input type.\n", yylineno);
        exit(1);
    }
    int isSubmit = 0;
    if (strcmp(typeValue, "text") != 0 &&
        strcmp(typeValue, "checkbox") != 0 &&
        strcmp(typeValue, "radio") != 0 &&
        strcmp(typeValue, "submit") != 0) {
        fprintf(stderr, "Error at line %d: Invalid input type '%s'.\n", yylineno, typeValue);
        exit(1);
    }
    if (strcmp(typeValue, "submit") == 0) {
        if (submitFound) {
            fprintf(stderr, "Error at line %d: Multiple submit inputs are not allowed.\n", yylineno);
            exit(1);
        }
        submitFound = 1;
        isSubmit = 1;
    } else if (strcmp(typeValue, "checkbox") == 0) {
        actualCheckboxCount++;
        hasCheckbox = 1;
    }
    free(typeValue);
    return isSubmit;
}

/* Συνάρτηση εμφάνισης του αρχείου (για επιβεβαίωση της εισόδου) */
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

/* Προσθήκη ID input element */
void add_input_id(const char* attr) {
    char* id = extract_value(attr);
    if (!id) return;
    if (inputIdsCount < MAX_IDS) {
        inputIds[inputIdsCount++] = strdup(id);
    }
    free(id);
}

/* Προσθήκη for attribute του label */
void add_label_for(const char* attr) {
    char* for_value = extract_value(attr);
    if (!for_value) return;
    if (labelForsCount < MAX_IDS) {
        labelFors[labelForsCount] = strdup(for_value);
        labelLines[labelForsCount] = yylineno;
        labelForsCount++;
    }
    free(for_value);
}

/* Έλεγχος όλων των label-input associations */
void validate_all_labels() {
    for (int i = 0; i < labelForsCount; i++) {
        int found = 0;
        for (int j = 0; j < inputIdsCount; j++) {
            if (strcmp(labelFors[i], inputIds[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Error at line %d: Label references non-existent input ID '%s'\n",
                    labelLines[i], labelFors[i]);
            exit(1);
        }
    }
}

#line 316 "myhtml.tab.c"

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
  YYSYMBOL_CHECKBOX_COUNT_ATTR = 37,       /* CHECKBOX_COUNT_ATTR  */
  YYSYMBOL_CHARSET_ATTR = 38,              /* CHARSET_ATTR  */
  YYSYMBOL_NAME_CONTENT_ATTR = 39,         /* NAME_CONTENT_ATTR  */
  YYSYMBOL_TEXT = 40,                      /* TEXT  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_document = 42,                  /* document  */
  YYSYMBOL_head_tag = 43,                  /* head_tag  */
  YYSYMBOL_head_content = 44,              /* head_content  */
  YYSYMBOL_meta_tag = 45,                  /* meta_tag  */
  YYSYMBOL_title_tag = 46,                 /* title_tag  */
  YYSYMBOL_body_tag = 47,                  /* body_tag  */
  YYSYMBOL_body_content = 48,              /* body_content  */
  YYSYMBOL_body_element = 49,              /* body_element  */
  YYSYMBOL_p_element = 50,                 /* p_element  */
  YYSYMBOL_a_element = 51,                 /* a_element  */
  YYSYMBOL_img_element = 52,               /* img_element  */
  YYSYMBOL_input_element = 53,             /* input_element  */
  YYSYMBOL_form_element = 54,              /* form_element  */
  YYSYMBOL_form_content = 55,              /* form_content  */
  YYSYMBOL_text = 56,                      /* text  */
  YYSYMBOL_comment = 57,                   /* comment  */
  YYSYMBOL_comment_text = 58               /* comment_text  */
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
#define YYLAST   67

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  31
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  73

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   277,   277,   281,   284,   285,   288,   291,   298,   301,
     302,   305,   306,   307,   308,   309,   310,   317,   318,   322,
     326,   333,   341,   348,   357,   399,   400,   404,   408,   417,
     420,   421
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
  "WIDTH_ATTR", "HEIGHT_ATTR", "CHECKBOX_COUNT_ATTR", "CHARSET_ATTR",
  "NAME_CONTENT_ATTR", "TEXT", "$accept", "document", "head_tag",
  "head_content", "meta_tag", "title_tag", "body_tag", "body_content",
  "body_element", "p_element", "a_element", "img_element", "input_element",
  "form_element", "form_content", "text", "comment", "comment_text", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-23)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       6,     9,    26,    18,    30,   -23,    -7,    32,    28,   -23,
      37,   -23,    -9,   -23,     4,   -23,    -8,   -23,   -23,   -23,
      19,   -23,   -21,    17,    20,    21,    22,    23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,    27,    25,
      16,    24,    29,    12,    31,    -4,    -7,    33,    34,    36,
      35,    38,    40,   -23,   -23,   -11,    -7,    -7,    41,   -23,
      13,   -23,   -23,   -10,   -12,   -23,     0,    39,   -23,   -23,
     -23,   -23,   -23
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     1,     0,     0,     5,     9,
       0,    27,     0,     3,     0,     4,     0,     2,     7,    28,
       0,     8,     0,     0,     0,     0,     0,     0,    30,    18,
      10,    11,    12,    13,    14,    15,    17,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    31,     0,     0,     0,     0,    25,
       0,    23,    19,     0,     0,    22,     0,     0,    20,    21,
      24,    26,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,    -6,   -23,
     -23,   -23,   -23,   -23,   -23,   -22,   -23,   -23
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     4,     7,    15,     8,    10,    16,    30,    31,
      32,    33,    34,    35,    66,    12,    36,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      21,    18,    62,    68,    22,    69,    38,    39,    23,     1,
      24,    25,    22,    26,     3,    27,    23,    28,    24,    25,
      70,    26,    53,    27,    55,    28,     5,     6,    19,    19,
      19,    19,    29,    11,    63,    64,    54,     9,    13,    14,
      29,    17,    20,    37,    40,    48,    51,    41,    42,    43,
      44,    46,    47,    67,    49,     0,     0,    56,    57,    59,
      71,    72,    60,    52,    61,    65,    50,    58
};

static const yytype_int8 yycheck[] =
{
       8,    10,    13,    13,    12,    17,    27,    28,    16,     3,
      18,    19,    12,    21,     5,    23,    16,    25,    18,    19,
      20,    21,    26,    23,    46,    25,     0,     9,    40,    40,
      40,    40,    40,    40,    56,    57,    40,     7,     6,    11,
      40,     4,    38,    24,    27,    29,    34,    27,    27,    27,
      27,    24,    27,    40,    30,    -1,    -1,    24,    24,    24,
      66,    22,    24,    32,    24,    24,    37,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    42,     5,    43,     0,     9,    44,    46,     7,
      47,    40,    56,     6,    11,    45,    48,     4,    10,    40,
      38,     8,    12,    16,    18,    19,    21,    23,    25,    40,
      49,    50,    51,    52,    53,    54,    57,    24,    27,    28,
      27,    27,    27,    27,    27,    58,    24,    27,    29,    30,
      37,    34,    32,    26,    40,    56,    24,    24,    31,    24,
      24,    24,    13,    56,    56,    24,    55,    40,    13,    17,
      20,    49,    22
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    44,    44,    45,    46,    47,    48,
      48,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      50,    51,    52,    53,    54,    55,    55,    56,    56,    57,
      58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     3,     2,     1,     3,     3,     3,     0,
       2,     1,     1,     1,     1,     1,     6,     1,     1,     5,
       6,     6,     5,     4,     6,     0,     2,     1,     2,     3,
       0,     2
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
#line 278 "myhtml.y"
        { /* (Εδώ μπορούν να προστεθούν επιπλέον έλεγχοι, π.χ. association label-input) */ }
#line 1405 "myhtml.tab.c"
    break;

  case 7: /* title_tag: TITLE_START text TITLE_END  */
#line 292 "myhtml.y"
        { 
            validate_title_length((yyvsp[-1].string_val));
            free((yyvsp[-1].string_val));
        }
#line 1414 "myhtml.tab.c"
    break;

  case 16: /* body_element: LABEL_START ID_ATTR FOR_ATTR CLOSE_TAG TEXT LABEL_END  */
#line 310 "myhtml.y"
                                                                    {
                validate_id((yyvsp[-4].string_val));
                add_label_for((yyvsp[-3].string_val));
                free((yyvsp[-4].string_val));
                free((yyvsp[-3].string_val));
                free((yyvsp[-1].string_val));
              }
#line 1426 "myhtml.tab.c"
    break;

  case 18: /* body_element: TEXT  */
#line 318 "myhtml.y"
                   { free((yyvsp[0].string_val)); }
#line 1432 "myhtml.tab.c"
    break;

  case 19: /* p_element: P_START ID_ATTR CLOSE_TAG text P_END  */
#line 322 "myhtml.y"
                                                {
              validate_id((yyvsp[-3].string_val));
              free((yyvsp[-1].string_val));
          }
#line 1441 "myhtml.tab.c"
    break;

  case 20: /* p_element: P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END  */
#line 326 "myhtml.y"
                                                            {
              validate_id((yyvsp[-3].string_val));
              free((yyvsp[-1].string_val));
          }
#line 1450 "myhtml.tab.c"
    break;

  case 21: /* a_element: A_START ID_ATTR HREF_ATTR CLOSE_TAG text A_END  */
#line 333 "myhtml.y"
                                                          {
              validate_id((yyvsp[-4].string_val));
              validate_href((yyvsp[-3].string_val));
              free((yyvsp[-1].string_val));
          }
#line 1460 "myhtml.tab.c"
    break;

  case 22: /* img_element: IMG_START ID_ATTR SRC_ATTR ALT_ATTR CLOSE_TAG  */
#line 341 "myhtml.y"
                                                           {
              validate_id((yyvsp[-3].string_val));
              validate_src((yyvsp[-2].string_val));
          }
#line 1469 "myhtml.tab.c"
    break;

  case 23: /* input_element: INPUT_START ID_ATTR TYPE_ATTR CLOSE_TAG  */
#line 348 "myhtml.y"
                                                       {
                 validate_id((yyvsp[-2].string_val));
                 add_input_id((yyvsp[-2].string_val));
                 int isSubmit = validate_input_type((yyvsp[-1].string_val));
                 lastInputWasSubmit = isSubmit;
             }
#line 1480 "myhtml.tab.c"
    break;

  case 24: /* form_element: FORM_START ID_ATTR CHECKBOX_COUNT_ATTR CLOSE_TAG form_content FORM_END  */
#line 357 "myhtml.y"
                                                                                     {
                  validate_id((yyvsp[-4].string_val));
                  validate_checkbox_count((yyvsp[-3].string_val));
                  
                  /* Έλεγχος για το submit button */
                  if (submitFound && !lastInputWasSubmit) {
                      fprintf(stderr, "Error at line %d: Submit input must be the last input element in the form.\n", yylineno);
                      exit(1);
                  }

                  /* Έλεγχος για το checkbox_count */
                  if (!hasCheckbox) {
                      fprintf(stderr, "Error at line %d: Form has checkbox_count but no checkbox inputs.\n", yylineno);
                      exit(1);
                  }
                  if (actualCheckboxCount != expectedCheckboxCount) {
                      fprintf(stderr, "Error at line %d: Expected %d checkbox inputs but found %d.\n", 
                              yylineno, expectedCheckboxCount, actualCheckboxCount);
                      exit(1);
                  }

                  /* Έλεγχος label-input associations */
                  validate_all_labels();

                  /* Επαναφορά μεταβλητών για ενδεχόμενη επόμενη φόρμα */
                  submitFound = 0;
                  lastInputWasSubmit = 0;
                  actualCheckboxCount = 0;
                  expectedCheckboxCount = 0;
                  hasCheckbox = 0;
                  inputIdsCount = 0;
                  labelForsCount = 0;

                  free((yyvsp[-4].string_val));
                  free((yyvsp[-3].string_val));
             }
#line 1521 "myhtml.tab.c"
    break;

  case 27: /* text: TEXT  */
#line 404 "myhtml.y"
           { 
         (yyval.string_val) = strdup((yyvsp[0].string_val));
         free((yyvsp[0].string_val));
      }
#line 1530 "myhtml.tab.c"
    break;

  case 28: /* text: text TEXT  */
#line 408 "myhtml.y"
                 {
         char* new_text = malloc(strlen((yyvsp[-1].string_val)) + strlen((yyvsp[0].string_val)) + 1);
         strcpy(new_text, (yyvsp[-1].string_val));
         strcat(new_text, (yyvsp[0].string_val));
         free((yyvsp[-1].string_val)); free((yyvsp[0].string_val));
         (yyval.string_val) = new_text;
       }
#line 1542 "myhtml.tab.c"
    break;

  case 31: /* comment_text: comment_text TEXT  */
#line 421 "myhtml.y"
                               { free((yyvsp[0].string_val)); }
#line 1548 "myhtml.tab.c"
    break;


#line 1552 "myhtml.tab.c"

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

#line 424 "myhtml.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s near '%s'\n", yylineno, s, yytext);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    display_file(argv[1]);
    extern FILE *yyin;
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }
    yyin = input;
    int result = yyparse();
    if (result == 0 && !hasError) {
        printf("Analysis result: File %s is syntactically correct.\n", argv[1]);
    }
    
    /* Καθαρισμός μνήμης */
    for (int i = 0; i < usedIdsCount; i++) {
        free(usedIds[i]);
    }
    for (int i = 0; i < inputIdsCount; i++) {
        free(inputIds[i]);
    }
    for (int i = 0; i < labelForsCount; i++) {
        free(labelFors[i]);
    }
    
    fclose(input);
    return (result || hasError);
}
