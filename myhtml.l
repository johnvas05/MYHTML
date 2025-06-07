%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myhtml.tab.h"

#define YY_NO_INPUT
#define YY_NO_UNPUT

extern int yylex();
void yyerror(const char *s);
extern int debug;
%}

%option noyywrap
%option yylineno

%x COMMENT

WS      [ \t\r\n]+
DIGIT   [0-9]
LETTER  [a-zA-Z]
ALPHANUMERIC ({LETTER}|{DIGIT})({LETTER}|{DIGIT})*
URL_CHAR  [a-zA-Z0-9/:._~%#-]
SPECIAL_CHAR [!@#$%^&*()\-_+=\[\]{}:;,.<>/?|\\']

%%

{WS}                  ; /* Skip whitespace */

"<!--"               { BEGIN(COMMENT); return COMMENT_START; }
<COMMENT>"-->"      { BEGIN(INITIAL); return COMMENT_END; }
<COMMENT>[^-]+|"-"  { yylval.string_val = strdup(yytext); return TEXT; }

"<MYHTML>"            { return MYHTML_START; }
"</MYHTML>"           { return MYHTML_END; }
"<head>"              { return HEAD_START; }
"</head>"             { return HEAD_END; }
"<body>"              { return BODY_START; }
"</body>"             { return BODY_END; }
"<title>"             { return TITLE_START; }
"</title>"            { return TITLE_END; }
"<meta"               { return META_START; }
"<p"                  { return P_START; }
"</p>"                { return P_END; }
"<div"                { return DIV_START; }
"</div>"              { return DIV_END; }
"<a"                  { return A_START; }
"</a>"                { return A_END; }
"<img"                { return IMG_START; }
"<form"               { return FORM_START; }
"</form>"             { return FORM_END; }
"<label"              { return LABEL_START; }
"</label>"            { return LABEL_END; }
"<input"              { return INPUT_START; }
">"                   { return CLOSE_TAG; }
"="                   { return '='; }

" id=\""[^\"]+"\""    { yylval.string_val = strdup(yytext); return ID_ATTR; }
" style=\""[^\"]+"\""  { yylval.string_val = strdup(yytext); return STYLE_ATTR; }
" href=\""[^\"]+"\""   { yylval.string_val = strdup(yytext); return HREF_ATTR; }
" src=\""[^\"]+"\""    { yylval.string_val = strdup(yytext); return SRC_ATTR; }
" alt=\""[^\"]+"\""    { yylval.string_val = strdup(yytext); return ALT_ATTR; }
" type=\""[^\"]+"\""   { yylval.string_val = strdup(yytext); return TYPE_ATTR; }
" value=\""[^\"]+"\""  { yylval.string_val = strdup(yytext); return VALUE_ATTR; }
" for=\""[^\"]+"\""    { yylval.string_val = strdup(yytext); return FOR_ATTR; }
" charset=\"UTF-8\""    { return CHARSET_ATTR; }
" width=\""{DIGIT}+"\""  { yylval.string_val = strdup(yytext); return WIDTH_ATTR; }
" height=\""{DIGIT}+"\""  { yylval.string_val = strdup(yytext); return HEIGHT_ATTR; }
"name=\""[^\"]+"\""[ ]+"content=\""[^\"]+"\""  { return NAME_CONTENT_ATTR; }
" checkbox_count=\""[^\"]+"\""  { yylval.string_val = strdup(yytext); return CHECKBOX_COUNT_ATTR; }

[^<>" \t\n\r=][^<>]*  { yylval.string_val = strdup(yytext); return TEXT; }

"<"[^> \t\n\r]*       { /* Invalid tag */
                        yyerror("Invalid tag");
                        return TEXT;
                      }

["]                   { /* Unmatched quote */
                        yyerror("Unmatched quote");
                        return TEXT;
                      }

%% 