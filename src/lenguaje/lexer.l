%{
/*
 * SECCIÓN 1: DEFINICIONES
 * Incluimos el archivo de cabecera que Bison generará (parser.tab.h).
 * Este archivo contiene las definiciones de los tokens (NUMBER, ID, etc.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h" /* Archivo generado por Bison */

/* Para compatibilidad con la unión yylval */
extern YYSTYPE yylval;
%}


/* regex */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER_INT      {DIGIT}+
NUMBER_FLOAT    {DIGIT}+(\.{DIGIT}+)?
COMMENT     "/\\*"([^*]|\\*+[^*/])*"\\*/"

%%
{COMMENT}   { /* comentario uni/multi linea */ }

{NUMBER_INT}    { yylval.entero = atof(yytext); return NUMBER_INT; }
{NUMBER_FLOAT}  { yylval.flotante = atof(yytext); return NUMBER_FLOAT; }
{ID}        { yylval.id = strdup(yytext); return ID; }

"+"         { return OP_PLUS; }
"-"         { return OP_MINUS; }
"*"         { return OP_TIMES; }
"/"         { return OP_DIVIDE; }

"("         { return PARENTHESES_OPEN; }
")"         { return PARENTHESES_CLOSE; }

\"([^"\\]|\\.)*\" {
    
    int len = yyleng;
    char *contenido = (char*) malloc(len - 1);
    strncpy(contenido, yytext + 1, len - 2);
    contenido[len - 2] = '\0';
    yylval.str = contenido;
    return LIT_STRING;
}


[ \t]+      { /* Ignorar */ }


"<="        { return OP_LESSER_EQUAL; }
">="        { return OP_GREATER_EQUAL; }

"<-"        { return ASSIGN; }
"<<"        { return BEGIN_LOOP; }
">>"        { return END_LOOP; }
"!!"        { return BREAK; }
"<!"        { return CONTINUE; }
"::"        { return RETURN_VALUE; }
"->"        { return RETURN_TYPE; } /* return para funcion */
":|"        { return ELSE_IF; }
"&&"        { return BOOL_AND; }
"||"        { return BOOL_OR; }

"#."        { return TYPE_FLOAT; }  /* float */
"#"         { return TYPE_INTEGER; }    /* int */
"@"         { return TYPE_STRING; } /* string */
"?'"       { return TYPE_BOOLEAN; }    /* bool */
"[>]"       { return TYPE_LIST; } /* list */
"[]"        { return TYPE_ARRAY; }    /* array */

"?"         { return IF; }
";"         { return SEQUENCE_SEPARATOR; }
":"         { return ELSE; }
"{"         { return BEGIN_SEQUENCE; }
"}"         { return END_SEQUENCE; }
","         { return LIST_SEPARATOR; }

"!"         { return BOOL_NOT; }
">"         { return OP_GREATER;}
"="         { return OP_EQUALS; }
"<"         { return OP_LESSER; }

\n          { /* return '\n'; */ }
\r          { /* ignorar */ }

.           { fprintf(stderr, "Error léxico: caracter inesperado '%s'\n", yytext); return ERROR; }
%%

int yywrap() {
    return 1;
}