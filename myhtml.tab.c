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
#define MAX_LABELS 100
#define MAX_TITLE_LENGTH 60
#define MAX_ATTRIBUTES 10
#define MAX_STYLES 10

extern int yylex(void);
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);

/* Struct for holding attributes */
typedef struct {
    char* attributes[MAX_ATTRIBUTES];
    int attr_count;
} AttributeList;

/* Global variables for validation */
char* usedIds[MAX_IDS];
int usedIdsCount = 0;

typedef struct {
    char* label_id;
    char* input_id;
    int line_no;
} LabelInputPair;

LabelInputPair labelInputMap[MAX_LABELS];
int labelInputMapCount = 0;

int hasError = 0;
int hasSubmitInput = 0;
int submitInputLine = 0;

/* Form validation */
int expectedCheckboxCount = -1;  // -1 means no count specified
int actualCheckboxCount = 0;
int formStartLine = 0;
int hasCheckboxInput = 0;  // Flag to track if form has any checkbox

/* Debug flag */
int debug = 1;

/* Style validation helpers */
typedef struct {
    char* property;
    char* value;
} StylePair;

/* Extract style pairs from style attribute */
void validate_style_attribute(const char* attr_str) {
    // Skip the initial ' style="' part
    const char* start = strchr(attr_str, '"');
    if (!start) return;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return;
    
    // Copy the style content for tokenization
    int len = end - start;
    char* style_content = malloc(len + 1);
    strncpy(style_content, start, len);
    style_content[len] = '\0';
    
    // Store style pairs for uniqueness check
    StylePair styles[MAX_STYLES];
    int style_count = 0;
    
    // Check for missing semicolons between properties
    int property_count = 0;
    char* s = style_content;
    while (*s) {
        if (*s == ':') property_count++;
        s++;
    }
    
    s = style_content;
    int semicolon_count = 0;
    while (*s) {
        if (*s == ';') semicolon_count++;
        s++;
    }
    
    // If there are multiple properties, we need semicolons between them
    if (property_count > 1 && semicolon_count < property_count - 1) {
        printf("Error at line %d: Missing semicolon between properties\n", yylineno);
        hasError = 1;
        free(style_content);
        return;
    }
    
    // Split by semicolon
    char* token = strtok(style_content, ";");
    while (token != NULL) {
        // Skip leading whitespace
        while (*token == ' ') token++;
        
        // Find the colon separator
        char* colon = strchr(token, ':');
        if (!colon) {
            printf("Error at line %d: Invalid style format, missing colon in '%s'\n", 
                   yylineno, token);
            hasError = 1;
            token = strtok(NULL, ";");
            continue;
        }
        
        // Split into property and value
        *colon = '\0';
        char* property = token;
        char* value = colon + 1;
        
        // Skip leading whitespace in value
        while (*value == ' ') value++;
        
        // Remove trailing whitespace from property
        char* end = property + strlen(property) - 1;
        while (end > property && *end == ' ') {
            *end = '\0';
            end--;
        }
        
        // Remove trailing whitespace from value
        end = value + strlen(value) - 1;
        while (end > value && *end == ' ') {
            *end = '\0';
            end--;
        }
        
        // Check for duplicate properties
        for (int i = 0; i < style_count; i++) {
            if (strcmp(styles[i].property, property) == 0) {
                printf("Error at line %d: Duplicate style property '%s'\n", 
                       yylineno, property);
                hasError = 1;
                goto next_token;
            }
        }
        
        // Validate property name
        if (strcmp(property, "background_color") != 0 &&
            strcmp(property, "color") != 0 &&
            strcmp(property, "font_family") != 0 &&
            strcmp(property, "font_size") != 0) {
            printf("Error at line %d: Invalid style property '%s'\n", 
                   yylineno, property);
            hasError = 1;
            goto next_token;
        }
        
        // Validate value based on property
        if (strcmp(property, "font_size") == 0) {
            // Should be number followed by px or %
            char* unit = NULL;
            if (strlen(value) > 2) {
                unit = value + strlen(value) - 2;
            }
            
            // Check if value ends with px or %
            if (!unit || (strcmp(unit, "px") != 0 && strcmp(unit, "%") != 0)) {
                printf("Error at line %d: Font size must end with 'px' or '%%'\n", 
                       yylineno);
                hasError = 1;
                goto next_token;
            }
            
            // Temporarily null-terminate before unit to check number
            *unit = '\0';
            char* endptr;
            strtol(value, &endptr, 10);
            if (*endptr != '\0') {
                printf("Error at line %d: Invalid numeric value in font size\n", 
                       yylineno);
                hasError = 1;
                goto next_token;
            }
            // Restore the unit
            *unit = unit[0];
        }
        
        // Store valid style pair
        if (style_count < MAX_STYLES) {
            styles[style_count].property = strdup(property);
            styles[style_count].value = strdup(value);
            style_count++;
        }
        
    next_token:
        token = strtok(NULL, ";");
    }
    
    // Free style pairs and content
    for (int i = 0; i < style_count; i++) {
        free(styles[i].property);
        free(styles[i].value);
    }
    free(style_content);
}

/* Extract ID value from attribute string */
char* extract_id(const char* attr_str) {
    // Skip the initial ' id="' part
    const char* start = strchr(attr_str, '"');
    if (!start) return NULL;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    
    // Calculate length and allocate memory
    int len = end - start;
    char* id = malloc(len + 1);
    strncpy(id, start, len);
    id[len] = '\0';
    
    return id;
}

/* Extract attribute value from attribute string */
char* extract_attr_value(const char* attr_str) {
    // Skip to the first quote
    const char* start = strchr(attr_str, '"');
    if (!start) return NULL;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    
    // Calculate length and allocate memory
    int len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    
    return value;
}

/* Check if ID is already used */
int is_id_used(const char* id) {
    for (int i = 0; i < usedIdsCount; i++) {
        if (strcmp(usedIds[i], id) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Check if a string starts with a prefix */
int starts_with(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

/* Validate URL format */
void validate_url(const char* attr_str, const char* attr_name) {
    char* url = extract_attr_value(attr_str);
    if (!url) return;
    
    // Check if it's an absolute URL
    if (starts_with(url, "http://") || 
        starts_with(url, "https://") || 
        starts_with(url, "ftp://") ||
        starts_with(url, "file://")) {
        // Valid absolute URL, no additional validation needed
    }
    // Must be a relative URL
    else {
        // Check if it contains only valid URL characters
        for (int i = 0; url[i]; i++) {
            char c = url[i];
            if (!((c >= 'a' && c <= 'z') || 
                  (c >= 'A' && c <= 'Z') || 
                  (c >= '0' && c <= '9') ||
                  c == '/' || c == '.' || c == '-' || 
                  c == '_' || c == '~' || c == '%')) {
                hasError = 1;
                fprintf(stderr, "Error at line %d: Invalid character in %s URL: '%c'\n", 
                        yylineno, attr_name, c);
                break;
            }
        }
    }
    
    free(url);
}

/* Validate href value */
void validate_href(const char* attr_str) {
    char* href = extract_attr_value(attr_str);
    if (!href) return;
    
    // If href starts with #, it's an element ID reference
    if (href[0] == '#') {
        // Skip the # character
        char* referenced_id = href + 1;
        if (!is_id_used(referenced_id)) {
            hasError = 1;
            fprintf(stderr, "Error at line %d: href references non-existent ID: '%s'\n", 
                    yylineno, referenced_id);
        }
    }
    // Otherwise validate as URL
    else {
        validate_url(attr_str, "href");
    }
    
    free(href);
}

/* Validate src value */
void validate_src(const char* attr_str) {
    validate_url(attr_str, "src");
}

/* Validate ID uniqueness */
void validate_id(const char* attr_str) {
    char* id = extract_id(attr_str);
    if (!id) return;
    
    if (is_id_used(id)) {
        hasError = 1;
        fprintf(stderr, "Error at line %d: Duplicate ID found: '%s'\n", yylineno, id);
    } else {
        if (usedIdsCount < MAX_IDS) {
            usedIds[usedIdsCount++] = strdup(id);
        } else {
            fprintf(stderr, "Warning: Maximum number of IDs reached\n");
        }
    }
    free(id);
}

/* Validation function for title length */
void validate_title_length(const char* title) {
    if (strlen(title) > MAX_TITLE_LENGTH) {
        hasError = 1;
        fprintf(stderr, "Error at line %d: Title length exceeds maximum of %d characters (current length: %lu)\n", 
                yylineno, MAX_TITLE_LENGTH, strlen(title));
    }
}

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

/* Validate input type */
int validate_input_type(const char* attr_str) {
    // Skip the initial ' type="' part
    const char* start = strchr(attr_str, '"');
    if (!start) return 0;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return 0;
    
    // Calculate length and allocate memory
    int len = end - start;
    char* type = malloc(len + 1);
    strncpy(type, start, len);
    type[len] = '\0';
    
    // Check if type is valid
    int valid = (strcmp(type, "text") == 0 ||
                strcmp(type, "checkbox") == 0 ||
                strcmp(type, "radio") == 0 ||
                strcmp(type, "submit") == 0);
                
    // Check submit type constraints
    if (strcmp(type, "submit") == 0) {
        if (hasSubmitInput) {
            printf("Error at line %d: Multiple submit inputs found. First submit was at line %d\n", 
                   yylineno, submitInputLine);
            valid = 0;
        } else {
            hasSubmitInput = 1;
            submitInputLine = yylineno;
        }
    }
    
    free(type);
    return valid;
}

/* Extract value from for attribute */
char* extract_for_value(const char* attr_str) {
    // Skip the initial ' for="' part
    const char* start = strchr(attr_str, '"');
    if (!start) return NULL;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    
    // Calculate length and allocate memory
    int len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    
    return value;
}

/* Store label-input association for later validation */
void store_label_for(const char* label_id, const char* for_attr, int line) {
    char* for_value = extract_for_value(for_attr);
    if (!for_value) return;
    
    if (labelInputMapCount < MAX_LABELS) {
        labelInputMap[labelInputMapCount].label_id = strdup(label_id);
        labelInputMap[labelInputMapCount].input_id = for_value;
        labelInputMap[labelInputMapCount].line_no = line;
        labelInputMapCount++;
    }
}

/* Check if an ID belongs to an input element */
int is_input_id(const char* id) {
    // Remove the leading space and quotes from id
    const char* start = strchr(id, '"');
    if (!start) return 0;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return 0;
    
    // Extract the ID value
    int len = end - start;
    char* clean_id = malloc(len + 1);
    strncpy(clean_id, start, len);
    clean_id[len] = '\0';
    
    // Check if this ID belongs to an input element
    for (int i = 0; i < usedIdsCount; i++) {
        if (strcmp(usedIds[i], clean_id) == 0) {
            free(clean_id);
            return 1;
        }
    }
    
    free(clean_id);
    return 0;
}

/* Validate all label-input associations */
void validate_label_input_associations() {
    // Track which input IDs are referenced
    char* referenced_inputs[MAX_LABELS];
    int referenced_count = 0;
    
    // Check each label-input pair
    for (int i = 0; i < labelInputMapCount; i++) {
        // Check if input exists
        int input_exists = 0;
        for (int j = 0; j < usedIdsCount; j++) {
            if (strcmp(usedIds[j], labelInputMap[i].input_id) == 0) {
                input_exists = 1;
                break;
            }
        }
        
        if (!input_exists) {
            printf("Error at line %d: Label references non-existent input id '%s'\n",
                   labelInputMap[i].line_no, labelInputMap[i].input_id);
            hasError = 1;
            continue;
        }
        
        // Check for multiple labels pointing to same input
        for (int j = 0; j < referenced_count; j++) {
            if (strcmp(referenced_inputs[j], labelInputMap[i].input_id) == 0) {
                printf("Error at line %d: Multiple labels pointing to input with id '%s'\n",
                       labelInputMap[i].line_no, labelInputMap[i].input_id);
                hasError = 1;
                break;
            }
        }
        
        // Add to referenced inputs
        if (referenced_count < MAX_LABELS) {
            referenced_inputs[referenced_count++] = strdup(labelInputMap[i].input_id);
        }
    }
    
    // Free referenced inputs
    for (int i = 0; i < referenced_count; i++) {
        free(referenced_inputs[i]);
    }
}

/* Extract numeric value from checkbox_count attribute */
int extract_checkbox_count(const char* attr_str) {
    // Skip the initial ' checkbox_count="' part
    const char* start = strchr(attr_str, '"');
    if (!start) return -1;
    start++; // Move past the quote
    
    // Find the closing quote
    const char* end = strchr(start, '"');
    if (!end) return -1;
    
    // Check if value is a positive integer
    char* endptr;
    long value = strtol(start, &endptr, 10);
    
    // Validate the value
    if (endptr != end || value <= 0) {
        printf("Error at line %d: checkbox_count must be a positive integer\n", yylineno);
        hasError = 1;
        return -1;
    }
    
    return (int)value;
}

#line 613 "myhtml.tab.c"

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
  YYSYMBOL_body_tag = 44,                  /* body_tag  */
  YYSYMBOL_head_content = 45,              /* head_content  */
  YYSYMBOL_meta_tags = 46,                 /* meta_tags  */
  YYSYMBOL_title_tag = 47,                 /* title_tag  */
  YYSYMBOL_meta_tag = 48,                  /* meta_tag  */
  YYSYMBOL_meta_attributes = 49,           /* meta_attributes  */
  YYSYMBOL_body_content = 50,              /* body_content  */
  YYSYMBOL_body_element = 51,              /* body_element  */
  YYSYMBOL_comment = 52,                   /* comment  */
  YYSYMBOL_comment_text = 53,              /* comment_text  */
  YYSYMBOL_p_element = 54,                 /* p_element  */
  YYSYMBOL_a_element = 55,                 /* a_element  */
  YYSYMBOL_a_content = 56,                 /* a_content  */
  YYSYMBOL_img_element = 57,               /* img_element  */
  YYSYMBOL_img_attributes = 58,            /* img_attributes  */
  YYSYMBOL_form_element = 59,              /* form_element  */
  YYSYMBOL_60_1 = 60,                      /* $@1  */
  YYSYMBOL_form_attributes = 61,           /* form_attributes  */
  YYSYMBOL_form_content = 62,              /* form_content  */
  YYSYMBOL_form_item = 63,                 /* form_item  */
  YYSYMBOL_input_element = 64,             /* input_element  */
  YYSYMBOL_input_attributes = 65,          /* input_attributes  */
  YYSYMBOL_label_element = 66,             /* label_element  */
  YYSYMBOL_label_attributes = 67,          /* label_attributes  */
  YYSYMBOL_div_element = 68,               /* div_element  */
  YYSYMBOL_div_attributes = 69,            /* div_attributes  */
  YYSYMBOL_div_content = 70,               /* div_content  */
  YYSYMBOL_text = 71                       /* text  */
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
#define YYLAST   117

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  126

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
       0,   583,   583,   590,   593,   596,   600,   602,   606,   608,
     612,   620,   624,   625,   628,   629,   632,   633,   634,   635,
     636,   637,   638,   641,   644,   645,   655,   660,   665,   671,
     677,   678,   679,   680,   681,   684,   687,   691,   695,   699,
     703,   707,   713,   713,   739,   742,   745,   749,   753,   756,
     757,   760,   761,   762,   763,   766,   796,   802,   808,   815,
     822,   827,   831,   835,   840,   843,   846,   850,   856,   859,
     862,   866,   872,   873,   876,   880
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
  "body_tag", "head_content", "meta_tags", "title_tag", "meta_tag",
  "meta_attributes", "body_content", "body_element", "comment",
  "comment_text", "p_element", "a_element", "a_content", "img_element",
  "img_attributes", "form_element", "$@1", "form_attributes",
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

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      14,    50,    19,    39,    53,   -56,    36,    71,    67,   -56,
      75,   -56,    -8,   -56,    28,    67,   -56,    -7,   -56,   -56,
     -56,   -56,   -56,    56,   -56,   -56,    43,    45,    54,    38,
      22,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -18,    55,    57,    59,    60,    58,    44,    20,    26,
      64,    46,    62,    66,    17,    36,    68,    69,   -56,   -56,
     -56,    70,    65,    61,    72,    73,    74,    78,   -56,   -56,
     -56,   -56,   -56,   -56,   -10,    36,    36,    10,    -4,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,    -9,     0,   -56,
     -56,    80,    36,    -2,    21,   -56,   -56,   -56,    76,   -56,
     -56,   -19,    31,   -56,   -56,   -56,   -56,   -56,    77,    79,
      83,    63,    81,   -56,    85,   -56,   -56,     5,    82,    84,
     -56,   -56,    12,   -56,   -56,   -56
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     0,     1,     0,     0,     7,    14,
       0,    74,     0,     3,     0,     6,     8,     0,     2,    10,
      75,    12,    13,     0,     9,     5,     0,     0,     0,     0,
      48,    24,    22,    15,    21,    16,    17,    18,    19,    20,
      11,     0,     0,    69,     0,     0,     0,     0,     0,     0,
       0,    44,    45,     0,     0,     0,     0,     0,    70,    71,
      72,     0,     0,     0,     0,     0,     0,     0,    35,    46,
      47,    42,    23,    25,     0,     0,     0,     0,    30,    41,
      40,    37,    36,    39,    38,    49,    28,     0,     0,    68,
      73,     0,    32,    31,     0,    26,    27,    29,    34,    33,
      43,     0,    63,    54,    53,    50,    51,    52,     0,     0,
       0,    60,    61,    62,     0,    66,    67,     0,    56,    57,
      55,    65,     0,    58,    59,    64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -56,   -56,   -56,   -56,   -56,    86,   -56,   -56,
      25,     4,   -56,   -56,   -56,   -56,   -39,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     4,    10,     7,    15,     8,    16,    23,    17,
      33,    34,    54,    35,    36,    91,    37,    50,    38,    85,
      53,    94,   105,   106,   114,   107,   110,    39,    45,    77,
      12
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      74,    25,    19,    86,    95,    26,    55,    27,   108,    28,
      56,    29,    30,    96,    29,   109,    29,     1,    31,     5,
      87,    88,    26,    93,    27,    89,    28,   121,    29,    30,
      20,    20,    20,    32,   125,    31,    11,    98,    20,    92,
      20,   100,   101,    72,   102,    11,    31,    64,     6,    51,
      32,    65,    20,    66,    99,     3,    67,    73,   111,    52,
       9,   103,   122,   112,   113,    47,    21,    22,    48,    49,
      41,    42,    43,    44,    62,    63,    11,    13,    14,    18,
      40,    46,    57,    69,    60,    58,    59,    61,    68,    70,
      71,    80,    75,    76,    78,   118,    79,    97,   104,     0,
      82,    24,    90,    81,    83,    84,   116,   117,   119,   120,
       0,   115,     0,     0,     0,   123,    20,   124
};

static const yytype_int8 yycheck[] =
{
      55,     8,    10,    13,    13,    12,    24,    14,    27,    16,
      28,    18,    19,    13,    18,    34,    18,     3,    25,     0,
      75,    76,    12,    78,    14,    15,    16,    22,    18,    19,
      40,    40,    40,    40,    22,    25,    40,    92,    40,    78,
      40,    20,    21,    26,    23,    40,    25,    27,     9,    27,
      40,    31,    40,    27,    93,     5,    30,    40,    27,    37,
       7,    40,   117,    32,    33,    27,    38,    39,    30,    31,
      27,    28,    27,    28,    30,    31,    40,     6,    11,     4,
      24,    27,    27,    37,    24,    28,    27,    29,    24,    27,
      24,    30,    24,    24,    24,    32,    31,    17,    94,    -1,
      27,    15,    77,    31,    30,    27,    27,    24,    27,    24,
      -1,    34,    -1,    -1,    -1,    33,    40,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    42,     5,    43,     0,     9,    45,    47,     7,
      44,    40,    71,     6,    11,    46,    48,    50,     4,    10,
      40,    38,    39,    49,    48,     8,    12,    14,    16,    18,
      19,    25,    40,    51,    52,    54,    55,    57,    59,    68,
      24,    27,    28,    27,    28,    69,    27,    27,    30,    31,
      58,    27,    37,    61,    53,    24,    28,    27,    28,    27,
      24,    29,    30,    31,    27,    31,    27,    30,    24,    37,
      27,    24,    26,    40,    71,    24,    24,    70,    24,    31,
      30,    31,    27,    30,    27,    60,    13,    71,    71,    15,
      51,    56,    57,    71,    62,    13,    13,    17,    71,    57,
      20,    21,    23,    40,    52,    63,    64,    66,    27,    34,
      67,    27,    32,    33,    65,    34,    27,    24,    32,    27,
      24,    22,    71,    33,    33,    22
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    45,    45,    46,    46,
      47,    48,    49,    49,    50,    50,    51,    51,    51,    51,
      51,    51,    51,    52,    53,    53,    54,    54,    54,    55,
      56,    56,    56,    56,    56,    57,    58,    58,    58,    58,
      58,    58,    60,    59,    61,    61,    61,    61,    61,    62,
      62,    63,    63,    63,    63,    64,    65,    65,    65,    65,
      65,    65,    65,    65,    66,    66,    67,    67,    68,    69,
      69,    69,    70,    70,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     3,     0,     3,     2,     1,     1,     2,
       3,     3,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     2,     6,     6,     5,     6,
       0,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     0,     6,     1,     1,     2,     2,     0,     0,
       2,     1,     1,     1,     1,     3,     2,     2,     3,     3,
       1,     1,     1,     0,     5,     4,     2,     2,     5,     1,
       2,     2,     0,     2,     1,     2
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
#line 584 "myhtml.y"
        { 
            validate_label_input_associations();
            if (debug) printf("Successfully parsed MYHTML document\n"); 
        }
#line 1763 "myhtml.tab.c"
    break;

  case 3: /* head_tag: HEAD_START head_content HEAD_END  */
#line 591 "myhtml.y"
        { if (debug) printf("Parsed head section\n"); }
#line 1769 "myhtml.tab.c"
    break;

  case 4: /* head_tag: %empty  */
#line 593 "myhtml.y"
        { if (debug) printf("Empty head section\n"); }
#line 1775 "myhtml.tab.c"
    break;

  case 5: /* body_tag: BODY_START body_content BODY_END  */
#line 597 "myhtml.y"
        { if (debug) printf("Parsed body section\n"); }
#line 1781 "myhtml.tab.c"
    break;

  case 6: /* head_content: title_tag meta_tags  */
#line 601 "myhtml.y"
            { if (debug) printf("Parsed head content with meta tags\n"); }
#line 1787 "myhtml.tab.c"
    break;

  case 7: /* head_content: title_tag  */
#line 603 "myhtml.y"
            { if (debug) printf("Parsed head content with title only\n"); }
#line 1793 "myhtml.tab.c"
    break;

  case 8: /* meta_tags: meta_tag  */
#line 607 "myhtml.y"
        { if (debug) printf("Parsed single meta tag\n"); }
#line 1799 "myhtml.tab.c"
    break;

  case 9: /* meta_tags: meta_tags meta_tag  */
#line 609 "myhtml.y"
        { if (debug) printf("Parsed additional meta tag\n"); }
#line 1805 "myhtml.tab.c"
    break;

  case 10: /* title_tag: TITLE_START text TITLE_END  */
#line 613 "myhtml.y"
        { 
            validate_title_length((yyvsp[-1].string_val));
            if (debug) printf("Parsed title: %s\n", (yyvsp[-1].string_val)); 
            free((yyvsp[-1].string_val)); 
        }
#line 1815 "myhtml.tab.c"
    break;

  case 11: /* meta_tag: META_START meta_attributes CLOSE_TAG  */
#line 621 "myhtml.y"
       { if (debug) printf("Parsed meta tag\n"); }
#line 1821 "myhtml.tab.c"
    break;

  case 22: /* body_element: TEXT  */
#line 638 "myhtml.y"
                  { free((yyvsp[0].string_val)); }
#line 1827 "myhtml.tab.c"
    break;

  case 24: /* comment_text: %empty  */
#line 644 "myhtml.y"
                          { (yyval.string_val) = strdup(""); }
#line 1833 "myhtml.tab.c"
    break;

  case 25: /* comment_text: comment_text TEXT  */
#line 645 "myhtml.y"
                               { 
               char* new_text = malloc(strlen((yyvsp[-1].string_val)) + strlen((yyvsp[0].string_val)) + 1);
               strcpy(new_text, (yyvsp[-1].string_val));
               strcat(new_text, (yyvsp[0].string_val));
               free((yyvsp[-1].string_val));
               free((yyvsp[0].string_val));
               (yyval.string_val) = new_text;
           }
#line 1846 "myhtml.tab.c"
    break;

  case 26: /* p_element: P_START ID_ATTR STYLE_ATTR CLOSE_TAG text P_END  */
#line 655 "myhtml.y"
                                                           {
            validate_id((yyvsp[-4].string_val));
            validate_style_attribute((yyvsp[-3].string_val));
            free((yyvsp[-1].string_val));
         }
#line 1856 "myhtml.tab.c"
    break;

  case 27: /* p_element: P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END  */
#line 660 "myhtml.y"
                                                           {
            validate_id((yyvsp[-3].string_val));
            validate_style_attribute((yyvsp[-4].string_val));
            free((yyvsp[-1].string_val));
         }
#line 1866 "myhtml.tab.c"
    break;

  case 28: /* p_element: P_START ID_ATTR CLOSE_TAG text P_END  */
#line 665 "myhtml.y"
                                                {
            validate_id((yyvsp[-3].string_val));
            free((yyvsp[-1].string_val));
         }
#line 1875 "myhtml.tab.c"
    break;

  case 29: /* a_element: A_START ID_ATTR HREF_ATTR CLOSE_TAG a_content A_END  */
#line 671 "myhtml.y"
                                                               {
            validate_id((yyvsp[-4].string_val));
            validate_href((yyvsp[-3].string_val));
         }
#line 1884 "myhtml.tab.c"
    break;

  case 36: /* img_attributes: SRC_ATTR ALT_ATTR ID_ATTR  */
#line 687 "myhtml.y"
                                          {
                validate_src((yyvsp[-2].string_val));
                validate_id((yyvsp[0].string_val));
              }
#line 1893 "myhtml.tab.c"
    break;

  case 37: /* img_attributes: SRC_ATTR ID_ATTR ALT_ATTR  */
#line 691 "myhtml.y"
                                          {
                validate_src((yyvsp[-2].string_val));
                validate_id((yyvsp[-1].string_val));
              }
#line 1902 "myhtml.tab.c"
    break;

  case 38: /* img_attributes: ALT_ATTR SRC_ATTR ID_ATTR  */
#line 695 "myhtml.y"
                                          {
                validate_src((yyvsp[-1].string_val));
                validate_id((yyvsp[0].string_val));
              }
#line 1911 "myhtml.tab.c"
    break;

  case 39: /* img_attributes: ALT_ATTR ID_ATTR SRC_ATTR  */
#line 699 "myhtml.y"
                                          {
                validate_src((yyvsp[0].string_val));
                validate_id((yyvsp[-1].string_val));
              }
#line 1920 "myhtml.tab.c"
    break;

  case 40: /* img_attributes: ID_ATTR ALT_ATTR SRC_ATTR  */
#line 703 "myhtml.y"
                                          {
                validate_src((yyvsp[0].string_val));
                validate_id((yyvsp[-2].string_val));
              }
#line 1929 "myhtml.tab.c"
    break;

  case 41: /* img_attributes: ID_ATTR SRC_ATTR ALT_ATTR  */
#line 707 "myhtml.y"
                                          {
                validate_src((yyvsp[-1].string_val));
                validate_id((yyvsp[-2].string_val));
              }
#line 1938 "myhtml.tab.c"
    break;

  case 42: /* $@1: %empty  */
#line 713 "myhtml.y"
                                                   {
                formStartLine = yylineno;
                actualCheckboxCount = 0;  // Reset counter for new form
                hasCheckboxInput = 0;     // Reset checkbox presence flag
                hasSubmitInput = 0;       // Reset submit button state
                submitInputLine = 0;      // Reset submit line number
             }
#line 1950 "myhtml.tab.c"
    break;

  case 43: /* form_element: FORM_START form_attributes CLOSE_TAG $@1 form_content FORM_END  */
#line 720 "myhtml.y"
                                   {
                // Check if checkbox_count is specified but no checkboxes exist
                if (expectedCheckboxCount != -1 && !hasCheckboxInput) {
                    printf("Error at line %d: Form has checkbox_count attribute but contains no checkbox inputs\n",
                           formStartLine);
                    hasError = 1;
                }
                // Check if count matches (only if we have at least one checkbox)
                else if (expectedCheckboxCount != -1 && hasCheckboxInput && 
                         actualCheckboxCount != expectedCheckboxCount) {
                    printf("Error at line %d: Form has %d checkbox inputs, but checkbox_count specified %d\n",
                           formStartLine, actualCheckboxCount, expectedCheckboxCount);
                    hasError = 1;
                }
                expectedCheckboxCount = -1;  // Reset for next form
                hasCheckboxInput = 0;        // Reset for next form
             }
#line 1972 "myhtml.tab.c"
    break;

  case 44: /* form_attributes: ID_ATTR  */
#line 739 "myhtml.y"
                         {
                    validate_id((yyvsp[0].string_val));
                }
#line 1980 "myhtml.tab.c"
    break;

  case 45: /* form_attributes: CHECKBOX_COUNT_ATTR  */
#line 742 "myhtml.y"
                                      {
                    expectedCheckboxCount = extract_checkbox_count((yyvsp[0].string_val));
                }
#line 1988 "myhtml.tab.c"
    break;

  case 46: /* form_attributes: ID_ATTR CHECKBOX_COUNT_ATTR  */
#line 745 "myhtml.y"
                                              {
                    validate_id((yyvsp[-1].string_val));
                    expectedCheckboxCount = extract_checkbox_count((yyvsp[0].string_val));
                }
#line 1997 "myhtml.tab.c"
    break;

  case 47: /* form_attributes: CHECKBOX_COUNT_ATTR ID_ATTR  */
#line 749 "myhtml.y"
                                              {
                    expectedCheckboxCount = extract_checkbox_count((yyvsp[-1].string_val));
                    validate_id((yyvsp[0].string_val));
                }
#line 2006 "myhtml.tab.c"
    break;

  case 54: /* form_item: TEXT  */
#line 763 "myhtml.y"
               { free((yyvsp[0].string_val)); }
#line 2012 "myhtml.tab.c"
    break;

  case 55: /* input_element: INPUT_START input_attributes CLOSE_TAG  */
#line 766 "myhtml.y"
                                                      {
        // Validate input type here
        int i;
        int found_type = 0;
        for (i = 0; i < (yyvsp[-1].attr_list).attr_count; i++) {
            if (strstr((yyvsp[-1].attr_list).attributes[i], " type=\"") != NULL) {
                found_type = 1;
                if (!validate_input_type((yyvsp[-1].attr_list).attributes[i])) {
                    printf("Error at line %d: Invalid input type\n", yylineno);
                    hasError = 1;
                }
                // Check if this is a checkbox input
                if (strstr((yyvsp[-1].attr_list).attributes[i], " type=\"checkbox\"") != NULL) {
                    actualCheckboxCount++;
                    hasCheckboxInput = 1;  // Set flag when we find a checkbox
                }
                break;
            }
        }
        if (!found_type) {
            printf("Error at line %d: Input element must have a type attribute\n", yylineno);
            hasError = 1;
        }
        // Free allocated memory
        for (i = 0; i < (yyvsp[-1].attr_list).attr_count; i++) {
            free((yyvsp[-1].attr_list).attributes[i]);
        }
    }
#line 2045 "myhtml.tab.c"
    break;

  case 56: /* input_attributes: ID_ATTR TYPE_ATTR  */
#line 796 "myhtml.y"
                                    {
                    (yyval.attr_list).attr_count = 2;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[-1].string_val));
                    (yyval.attr_list).attributes[1] = strdup((yyvsp[0].string_val));
                    validate_id((yyvsp[-1].string_val));
                }
#line 2056 "myhtml.tab.c"
    break;

  case 57: /* input_attributes: TYPE_ATTR ID_ATTR  */
#line 802 "myhtml.y"
                                    {
                    (yyval.attr_list).attr_count = 2;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[-1].string_val));
                    (yyval.attr_list).attributes[1] = strdup((yyvsp[0].string_val));
                    validate_id((yyvsp[0].string_val));
                }
#line 2067 "myhtml.tab.c"
    break;

  case 58: /* input_attributes: ID_ATTR TYPE_ATTR VALUE_ATTR  */
#line 808 "myhtml.y"
                                               {
                    (yyval.attr_list).attr_count = 3;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[-2].string_val));
                    (yyval.attr_list).attributes[1] = strdup((yyvsp[-1].string_val));
                    (yyval.attr_list).attributes[2] = strdup((yyvsp[0].string_val));
                    validate_id((yyvsp[-2].string_val));
                }
#line 2079 "myhtml.tab.c"
    break;

  case 59: /* input_attributes: TYPE_ATTR ID_ATTR VALUE_ATTR  */
#line 815 "myhtml.y"
                                               {
                    (yyval.attr_list).attr_count = 3;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[-2].string_val));
                    (yyval.attr_list).attributes[1] = strdup((yyvsp[-1].string_val));
                    (yyval.attr_list).attributes[2] = strdup((yyvsp[0].string_val));
                    validate_id((yyvsp[-1].string_val));
                }
#line 2091 "myhtml.tab.c"
    break;

  case 60: /* input_attributes: ID_ATTR  */
#line 822 "myhtml.y"
                          {
                    (yyval.attr_list).attr_count = 1;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[0].string_val));
                    validate_id((yyvsp[0].string_val));
                }
#line 2101 "myhtml.tab.c"
    break;

  case 61: /* input_attributes: TYPE_ATTR  */
#line 827 "myhtml.y"
                            {
                    (yyval.attr_list).attr_count = 1;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[0].string_val));
                }
#line 2110 "myhtml.tab.c"
    break;

  case 62: /* input_attributes: VALUE_ATTR  */
#line 831 "myhtml.y"
                             {
                    (yyval.attr_list).attr_count = 1;
                    (yyval.attr_list).attributes[0] = strdup((yyvsp[0].string_val));
                }
#line 2119 "myhtml.tab.c"
    break;

  case 63: /* input_attributes: %empty  */
#line 835 "myhtml.y"
                              {
                    (yyval.attr_list).attr_count = 0;
                }
#line 2127 "myhtml.tab.c"
    break;

  case 64: /* label_element: LABEL_START label_attributes CLOSE_TAG text LABEL_END  */
#line 840 "myhtml.y"
                                                                     {
                free((yyvsp[-1].string_val));
             }
#line 2135 "myhtml.tab.c"
    break;

  case 66: /* label_attributes: ID_ATTR FOR_ATTR  */
#line 846 "myhtml.y"
                                   {
                    validate_id((yyvsp[-1].string_val));
                    store_label_for((yyvsp[-1].string_val), (yyvsp[0].string_val), yylineno);
                }
#line 2144 "myhtml.tab.c"
    break;

  case 67: /* label_attributes: FOR_ATTR ID_ATTR  */
#line 850 "myhtml.y"
                                   {
                    validate_id((yyvsp[0].string_val));
                    store_label_for((yyvsp[0].string_val), (yyvsp[-1].string_val), yylineno);
                }
#line 2153 "myhtml.tab.c"
    break;

  case 69: /* div_attributes: ID_ATTR  */
#line 859 "myhtml.y"
                        {
                validate_id((yyvsp[0].string_val));
              }
#line 2161 "myhtml.tab.c"
    break;

  case 70: /* div_attributes: ID_ATTR STYLE_ATTR  */
#line 862 "myhtml.y"
                                   {
                validate_id((yyvsp[-1].string_val));
                validate_style_attribute((yyvsp[0].string_val));
              }
#line 2170 "myhtml.tab.c"
    break;

  case 71: /* div_attributes: STYLE_ATTR ID_ATTR  */
#line 866 "myhtml.y"
                                   {
                validate_id((yyvsp[0].string_val));
                validate_style_attribute((yyvsp[-1].string_val));
              }
#line 2179 "myhtml.tab.c"
    break;

  case 74: /* text: TEXT  */
#line 876 "myhtml.y"
           {
        (yyval.string_val) = strdup((yyvsp[0].string_val));
        free((yyvsp[0].string_val));
    }
#line 2188 "myhtml.tab.c"
    break;

  case 75: /* text: text TEXT  */
#line 880 "myhtml.y"
                {
        char* new_text = malloc(strlen((yyvsp[-1].string_val)) + strlen((yyvsp[0].string_val)) + 1);
        strcpy(new_text, (yyvsp[-1].string_val));
        strcat(new_text, (yyvsp[0].string_val));
        free((yyvsp[-1].string_val));
        free((yyvsp[0].string_val));
        (yyval.string_val) = new_text;
    }
#line 2201 "myhtml.tab.c"
    break;


#line 2205 "myhtml.tab.c"

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

#line 890 "myhtml.y"


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
        free(labelInputMap[i].label_id);
        free(labelInputMap[i].input_id);
    }
    
    fclose(input);
    return (result || hasError);
} 
