%{
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
%}

%union {
    char* string_val;
    struct {
        char* attributes[10];
        int attr_count;
    } attr_list;
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

%token <string_val> ID_ATTR STYLE_ATTR
%token <string_val> HREF_ATTR SRC_ATTR ALT_ATTR
%token <string_val> TYPE_ATTR VALUE_ATTR FOR_ATTR
%token <string_val> WIDTH_ATTR HEIGHT_ATTR
%token <string_val> CHECKBOX_COUNT_ATTR
%token CHARSET_ATTR NAME_CONTENT_ATTR

%token <string_val> TEXT

%type <string_val> text
%type <string_val> comment_text
%type <attr_list> input_attributes

%left TEXT

%%

document: MYHTML_START head_tag body_tag MYHTML_END
        { 
            validate_label_input_associations();
            if (debug) printf("Successfully parsed MYHTML document\n"); 
        }
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
        { 
            validate_title_length($2);
            if (debug) printf("Parsed title: %s\n", $2); 
            free($2); 
        }
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

p_element: P_START ID_ATTR STYLE_ATTR CLOSE_TAG text P_END {
            validate_id($2);
            validate_style_attribute($3);
            free($5);
         }
         | P_START STYLE_ATTR ID_ATTR CLOSE_TAG text P_END {
            validate_id($3);
            validate_style_attribute($2);
            free($5);
         }
         | P_START ID_ATTR CLOSE_TAG text P_END {
            validate_id($2);
            free($4);
         }
         ;

a_element: A_START ID_ATTR HREF_ATTR CLOSE_TAG a_content A_END {
            validate_id($2);
            validate_href($3);
         }
         ;

a_content: /* empty */
         | text
         | img_element
         | text img_element
         | img_element text
         ;

img_element: IMG_START img_attributes CLOSE_TAG
           ;

img_attributes: SRC_ATTR ALT_ATTR ID_ATTR {
                validate_src($1);
                validate_id($3);
              }
              | SRC_ATTR ID_ATTR ALT_ATTR {
                validate_src($1);
                validate_id($2);
              }
              | ALT_ATTR SRC_ATTR ID_ATTR {
                validate_src($2);
                validate_id($3);
              }
              | ALT_ATTR ID_ATTR SRC_ATTR {
                validate_src($3);
                validate_id($2);
              }
              | ID_ATTR ALT_ATTR SRC_ATTR {
                validate_src($3);
                validate_id($1);
              }
              | ID_ATTR SRC_ATTR ALT_ATTR {
                validate_src($2);
                validate_id($1);
              }
              ;

form_element: FORM_START form_attributes CLOSE_TAG {
                formStartLine = yylineno;
                actualCheckboxCount = 0;  // Reset counter for new form
                hasCheckboxInput = 0;     // Reset checkbox presence flag
                hasSubmitInput = 0;       // Reset submit button state
                submitInputLine = 0;      // Reset submit line number
             } 
             form_content FORM_END {
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
           ;

form_attributes: ID_ATTR {
                    validate_id($1);
                }
                | CHECKBOX_COUNT_ATTR {
                    expectedCheckboxCount = extract_checkbox_count($1);
                }
                | ID_ATTR CHECKBOX_COUNT_ATTR {
                    validate_id($1);
                    expectedCheckboxCount = extract_checkbox_count($2);
                }
                | CHECKBOX_COUNT_ATTR ID_ATTR {
                    expectedCheckboxCount = extract_checkbox_count($1);
                    validate_id($2);
                }
                | /* empty */
                ;

form_content: /* empty */
           | form_content form_item
           ;

form_item: input_element
        | label_element
        | comment
        | TEXT { free($1); }
        ;

input_element: INPUT_START input_attributes CLOSE_TAG {
        // Validate input type here
        int i;
        int found_type = 0;
        for (i = 0; i < $2.attr_count; i++) {
            if (strstr($2.attributes[i], " type=\"") != NULL) {
                found_type = 1;
                if (!validate_input_type($2.attributes[i])) {
                    printf("Error at line %d: Invalid input type\n", yylineno);
                    hasError = 1;
                }
                // Check if this is a checkbox input
                if (strstr($2.attributes[i], " type=\"checkbox\"") != NULL) {
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
        for (i = 0; i < $2.attr_count; i++) {
            free($2.attributes[i]);
        }
    }
    ;

input_attributes: ID_ATTR TYPE_ATTR {
                    $$.attr_count = 2;
                    $$.attributes[0] = strdup($1);
                    $$.attributes[1] = strdup($2);
                    validate_id($1);
                }
                | TYPE_ATTR ID_ATTR {
                    $$.attr_count = 2;
                    $$.attributes[0] = strdup($1);
                    $$.attributes[1] = strdup($2);
                    validate_id($2);
                }
                | ID_ATTR TYPE_ATTR VALUE_ATTR {
                    $$.attr_count = 3;
                    $$.attributes[0] = strdup($1);
                    $$.attributes[1] = strdup($2);
                    $$.attributes[2] = strdup($3);
                    validate_id($1);
                }
                | TYPE_ATTR ID_ATTR VALUE_ATTR {
                    $$.attr_count = 3;
                    $$.attributes[0] = strdup($1);
                    $$.attributes[1] = strdup($2);
                    $$.attributes[2] = strdup($3);
                    validate_id($2);
                }
                | ID_ATTR {
                    $$.attr_count = 1;
                    $$.attributes[0] = strdup($1);
                    validate_id($1);
                }
                | TYPE_ATTR {
                    $$.attr_count = 1;
                    $$.attributes[0] = strdup($1);
                }
                | VALUE_ATTR {
                    $$.attr_count = 1;
                    $$.attributes[0] = strdup($1);
                }
                | /* empty */ {
                    $$.attr_count = 0;
                }
                ;

label_element: LABEL_START label_attributes CLOSE_TAG text LABEL_END {
                free($4);
             }
             | LABEL_START label_attributes CLOSE_TAG LABEL_END
             ;

label_attributes: ID_ATTR FOR_ATTR {
                    validate_id($1);
                    store_label_for($1, $2, yylineno);
                }
                | FOR_ATTR ID_ATTR {
                    validate_id($2);
                    store_label_for($2, $1, yylineno);
                }
                ;

div_element: DIV_START div_attributes CLOSE_TAG div_content DIV_END
           ;

div_attributes: ID_ATTR {
                validate_id($1);
              }
              | ID_ATTR STYLE_ATTR {
                validate_id($1);
                validate_style_attribute($2);
              }
              | STYLE_ATTR ID_ATTR {
                validate_id($2);
                validate_style_attribute($1);
              }
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
        free(labelInputMap[i].label_id);
        free(labelInputMap[i].input_id);
    }
    
    fclose(input);
    return (result || hasError);
} 