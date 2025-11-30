%{
/*
 * SECCIÓN 1: DEFINICIONES
 * Incluimos el archivo de cabecera que Bison generará (parser.tab.h).
 * Este archivo contiene las definiciones de los tokens (NUMBER, ID, etc.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h" // Archivo generado por Bison

/* Para compatibilidad con la unión yylval */
extern YYSTYPE yylval;
%}

/* regex */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?
OPERATOR    [+\-*/]
COMMENT     "/\\*"([^*]|\\*+[^*/])*"\\*/"

%%
/* comentario */
{COMMENT}   { /* comentario uni/multi linea */ }

{NUMBER}    { yylval.num = atof(yytext); return NUMBER; }
//{DIGIT}+(\.{DIGIT}+)? { yylval.str = strdup(yytext); return LIT_NUMBER; }
//{NUMBER}    { yylval.num = atof(yytext); return NUMBER; }
{ID}        { yylval.id = strdup(yytext); return ID; }

//{DIGIT}+    {yylval.intVal = atoi(yytext); return LIT_INTEGER;}
//{DIGIT}+"."{DIGIT}+ { yylval.floatVal = atof(yytext); return LIT_FLOAT;} // ESTA MAL

/* Quita espacios */
[ \t]+      { /* Ignorar */ }

/* operadores relacionales largos */
"<="        { return OP_LESSER_EQUAL; }
">="        { return OP_GREATER_EQUAL; }

/* tokens largos */
"<-"        { return ASSIGN; }
"<<"        { return BEGIN_LOOP; }
">>"        { return END_LOOP; }
"!!"        { return BREAK; }
"<!"        { return CONTINUE; }
"::"        { return RETURN_VALUE; }
"->"        { return RETURN_TYPE; } // return para funcion
":|"        { return ELSE_IF; }
"&&"        { return BOOL_AND; }
"||"        { return BOOL_OR; }

/* tipo de dato */
"#.."       { return TYPE_DOUBLE; } // double
"#."        { return TYPE_FLOAT; } // float
"#"         { return TYPE_INTEGER; } // int
"@"         { return TYPE_STRING; } // string
"\?'"       { return TYPE_BOOLEAN; } // bool
"\[>\]"       { return TYPE_LIST; } // list
"\[\]"        { return TYPE_ARRAY; } // array


/* palabras reservadas */
"?"         { return IF; }
";"         { return SEQUENCE_SEPARATOR; }
":"         { return ELSE; }
"{"         { return BEGIN_SEQUENCE; }
"}"         { return END_SEQUENCE; }

/* operadores cortos */
"!"         { return BOOL_NOT; }
"="         { return OP_EQUALS; }
"<"         { return OP_LESSER; }
">"         { return OP_GREATER;}

\n          { return '\n'; }
\r          { /* ignorar */ }
//.           { return yytext[0]; } // Retorna cualquier otro carácter tal cual

{OPERATOR}  { /* dejamos esto por compatibilidad si se usa; pero preferimos literales */ return yytext[0]; }

.           { fprintf(stderr, "Error léxico: caracter inesperado '%s'\n", yytext); return ERROR; }
%%

int yywrap() {
    return 1;
}