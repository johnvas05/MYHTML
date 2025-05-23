%{
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
%}

%union {
    char* string_val;
}

%token MYHTML_START MYHTML_END
%token HEAD_START HEAD_END
%token BODY_START BODY_END
%token TITLE_START TITLE_END
%token META_START
%token P_START P_END
%token DIV_START DIV_END
%token A_START A_END
%token IMG_START
%token FORM_START FORM_END
%token LABEL_START LABEL_END
%token INPUT_START
%token CLOSE_TAG
%token COMMENT_START COMMENT_END

%token ID_ATTR STYLE_ATTR
%token HREF_ATTR SRC_ATTR ALT_ATTR
%token TYPE_ATTR VALUE_ATTR FOR_ATTR
%token WIDTH_ATTR HEIGHT_ATTR
%token CHARSET_ATTR NAME_CONTENT_ATTR

%token <string_val> TEXT

%type <string_val> text
%type <string_val> comment_text

%left TEXT

%%

document: MYHTML_START head_tag body_tag MYHTML_END
        { if (debug) printf("Successfully parsed MYHTML document\n"); }
        ;

head_tag: HEAD_START head_content HEAD_END
        { if (debug) printf("Parsed head section\n"); }
        | /* empty */
        { if (debug) printf("Empty head section\n"); }
        ;

body_tag: BODY_START body_content BODY_END
        { if (debug) printf("Parsed body section\n"); }
        ;

head_content: title_tag meta_tags
            { if (debug) printf("Parsed head content with meta tags\n"); }
            | title_tag
            { if (debug) printf("Parsed head content with title only\n"); }
            ;

meta_tags: meta_tag
        { if (debug) printf("Parsed single meta tag\n"); }
        | meta_tags meta_tag
        { if (debug) printf("Parsed additional meta tag\n"); }
        ;

title_tag: TITLE_START text TITLE_END
        { if (debug) printf("Parsed title: %s\n", $2); free($2); }
        ;

meta_tag: META_START meta_attributes CLOSE_TAG
       { if (debug) printf("Parsed meta tag\n"); }
       ;

meta_attributes: CHARSET_ATTR
               | NAME_CONTENT_ATTR
               ;

body_content: /* empty */
           | body_content body_element
           ;

body_element: p_element
           | a_element
           | img_element
           | form_element
           | div_element
           | comment
           | TEXT { free($1); }
           ;

comment: COMMENT_START comment_text COMMENT_END
       ;

comment_text: /* empty */ { $$ = strdup(""); }
           | comment_text TEXT { 
               char* new_text = malloc(strlen($1) + strlen($2) + 1);
               strcpy(new_text, $1);
               strcat(new_text, $2);
               free($1);
               free($2);
               $$ = new_text;
           }
           ;

p_element: P_START ID_ATTR CLOSE_TAG text P_END {
            free($4);
         }
         | P_START ID_ATTR STYLE_ATTR CLOSE_TAG text P_END {
            free($5);
         }
         | P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END {
            free($5);
         }
         ;

a_element: A_START ID_ATTR HREF_ATTR CLOSE_TAG a_content A_END
         ;

a_content: /* empty */
         | text
         | img_element
         | text img_element
         | img_element text
         ;

img_element: IMG_START img_attributes CLOSE_TAG
           ;

img_attributes: SRC_ATTR ALT_ATTR ID_ATTR
              | SRC_ATTR ID_ATTR ALT_ATTR
              | ALT_ATTR SRC_ATTR ID_ATTR
              | ALT_ATTR ID_ATTR SRC_ATTR
              | ID_ATTR ALT_ATTR SRC_ATTR
              | ID_ATTR SRC_ATTR ALT_ATTR
              ;

form_element: FORM_START ID_ATTR CLOSE_TAG form_content FORM_END
           ;

form_content: form_item
           | form_content form_item
           ;

form_item: input_element
        | label_element
        ;

input_element: INPUT_START input_attributes CLOSE_TAG
            ;

input_attributes: ID_ATTR TYPE_ATTR
                | TYPE_ATTR ID_ATTR
                | ID_ATTR TYPE_ATTR VALUE_ATTR
                | TYPE_ATTR ID_ATTR VALUE_ATTR
                ;

label_element: LABEL_START label_attributes CLOSE_TAG text LABEL_END {
                free($4);
             }
             | LABEL_START label_attributes CLOSE_TAG LABEL_END
             ;

label_attributes: ID_ATTR FOR_ATTR
                | FOR_ATTR ID_ATTR
                ;

div_element: DIV_START div_attributes CLOSE_TAG div_content DIV_END
           ;

div_attributes: ID_ATTR
              | ID_ATTR STYLE_ATTR
              | STYLE_ATTR ID_ATTR
              ;

div_content: /* empty */
           | div_content body_element
           ;

text: TEXT {
        $$ = strdup($1);
        free($1);
    }
    | text TEXT {
        char* new_text = malloc(strlen($1) + strlen($2) + 1);
        strcpy(new_text, $1);
        strcat(new_text, $2);
        free($1);
        free($2);
        $$ = new_text;
    }
    ;

%%

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