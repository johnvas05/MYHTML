%{
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
%}

%union {
    char* string_val;
}

/* Ορισμοί Token όπως συμφωνούν με το Flex */
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

%token <string_val> ID_ATTR STYLE_ATTR
%token <string_val> HREF_ATTR SRC_ATTR ALT_ATTR
%token <string_val> TYPE_ATTR VALUE_ATTR FOR_ATTR
%token <string_val> WIDTH_ATTR HEIGHT_ATTR CHECKBOX_COUNT_ATTR
%token CHARSET_ATTR NAME_CONTENT_ATTR
%token <string_val> TEXT

%type <string_val> text

%%

document: MYHTML_START head_tag body_tag MYHTML_END
        { /* (Εδώ μπορούν να προστεθούν επιπλέον έλεγχοι, π.χ. association label-input) */ }
        ;

head_tag: HEAD_START head_content HEAD_END
        ;

head_content: title_tag meta_tag
            | title_tag
            ;

meta_tag: META_START CHARSET_ATTR CLOSE_TAG
        ;

title_tag: TITLE_START text TITLE_END
        { 
            validate_title_length($2);
            free($2);
        }
        ;

body_tag: BODY_START body_content BODY_END
        ;

body_content: /* empty */
            | body_content body_element
            ;

body_element: p_element
            | a_element
            | img_element
            | input_element
            | form_element
            | LABEL_START ID_ATTR FOR_ATTR CLOSE_TAG TEXT LABEL_END {
                validate_id($2);
                add_label_for($3);
                free($2);
                free($3);
                free($5);
              }
            | comment
            | TEXT { free($1); }
            ;

/* Υλοποίηση στοιχείου παραγράφου (<p>) */
p_element: P_START ID_ATTR CLOSE_TAG text P_END {
              validate_id($2);
              free($4);
          }
          | P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END {
              validate_id($3);
              free($5);
          }
          ;

/* Υλοποίηση υπερσυνδέσμου (<a>) */
a_element: A_START ID_ATTR HREF_ATTR CLOSE_TAG text A_END {
              validate_id($2);
              validate_href($3);
              free($5);
          }
          ;

/* Υλοποίηση εικόνας (<img>) */
img_element: IMG_START ID_ATTR SRC_ATTR ALT_ATTR CLOSE_TAG {
              validate_id($2);
              validate_src($3);
          }
          ;

/* Υλοποίηση input στοιχείου (<input>) */
input_element: INPUT_START ID_ATTR TYPE_ATTR CLOSE_TAG {
                 validate_id($2);
                 add_input_id($2);
                 int isSubmit = validate_input_type($3);
                 lastInputWasSubmit = isSubmit;
             }
             ;

/* Υλοποίηση φόρμας (<form>) */
form_element: FORM_START ID_ATTR CHECKBOX_COUNT_ATTR CLOSE_TAG form_content FORM_END {
                  validate_id($2);
                  validate_checkbox_count($3);
                  
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

                  free($2);
                  free($3);
             }
             ;

/* Περιεχόμενο φόρμας.
   **Διορθωμένη έκδοση:** Αφαιρέσαμε την παραγωγή που οδηγούσε σε διπλή εμφάνιση του input_element,
   ώστε όλο το περιεχόμενο της φόρμας να διαβάζεται μέσω του body_element.
*/
form_content: /* empty */
            | form_content body_element
            ;

/* Συνάρτηση εμφάνισης του αρχείου (για επιβεβαίωση της εισόδου) */
text: TEXT { 
         $$ = strdup($1);
         free($1);
      }
     | text TEXT {
         char* new_text = malloc(strlen($1) + strlen($2) + 1);
         strcpy(new_text, $1);
         strcat(new_text, $2);
         free($1); free($2);
         $$ = new_text;
       }
     ;

comment: COMMENT_START comment_text COMMENT_END
       ;

comment_text: /* empty */
           | comment_text TEXT { free($2); }
           ;

%%

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
