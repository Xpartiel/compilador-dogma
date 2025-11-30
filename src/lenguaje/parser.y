%{
/*
 * SECCIoN 1: DEFINICIONES
 * Codigo C que se copia al inicio del archivo C generado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    char *nombre;
    double valor;
}Variable;

Variable tabla[256];
int vars_act = 0;

// Funciones para guardar y obtener asignaciones
double obtener( char *id ){
    for( int i=0; i<vars_act; i++ ){
        if( strcmp( tabla[i].nombre, id ) == 0 ){
            return tabla[i].valor;
        }
    }
    return 0.0;
}

void guardar( char* id , double val ){
    for( int i = 0; i < vars_act; i++){
        if( strcmp( tabla[i].nombre , id ) == 0 ){
            tabla[i].valor = val;
            return;
        }
    }

    // No existia
    tabla[vars_act].valor = val;
    tabla[vars_act].nombre = strdup(id);
    vars_act++;
}


/* Prototipo de la funcion de análisis lexico (creada por Flex) */
int yylex(); 


/* Funcion para reportar errores sintácticos */
void yyerror(const char *s) {
    fprintf(stderr, "Error Sintactico: %s\n", s);
}
%}


/*
 * Declaracion de "tokens" (terminales).
 * Estos son los valores que Flex nos retornara.
 * Yacc generara automaticamente un enum para ellos en y.tab.h.
 */
/* Tipos para valores semánticos */
%union {
    double num;
    char *id;
    int boolean;
}

/* Tokens con sus tipos */
%token <num> NUMBER
%token <id> ID
%token ASSIGN       // <-
%token BEGIN_LOOP   // <<
%token END_LOOP     // >>
%token BREAK        // !!
%token CONTINUE     // <!
%token RETURN_VALUE // ::
%token RETURN_TYPE  // ->
%token ELSE_IF      // :|
%token TYPE_DOUBLE  // #..
%token TYPE_FLOAT   // #.
%token TYPE_INTEGER // #
%token TYPE_STRING  // @
%token TYPE_BOOLEAN // ?'
%token TYPE_LIST    // [>]
%token TYPE_ARRAY   // []
%token IF           // ?
%token SEQUENCE_SEPARATOR // ;
%token ELSE         // :
%token BEGIN_SEQUENCE   // {
%token END_SEQUENCE // }
%token OP_EQUALS    // =
%token OP_LESSER    // <
%token OP_GREATER   // >
%token OP_GREATER_EQUAL // >=
%token OP_LESSER_EQUAL  // <=
%token BOOL_AND     // &&
%token BOOL_OR      // ||
%token BOOL_NOT     // !


/* Precedencia y asociatividad para operadores reales (usando literales) */
%right BOOL_NOT
%left BOOL_AND
%left BOOL_OR
%left OP_GREATER
%left OP_LESSER
%left OP_GREATER_EQUAL
%left OP_LESSER_EQUAL
%left OP_EQUALS
%left '+' '-'
%left '*' '/'
%right UMINUS


/* El tipo del no terminal 'expresion' es num (double) */
%type <num> expresion

/*
 * Declaracion del simbolo inicial de la gramatica.
 * El analisis comenzara intentando encontrar un 'programa'.
 */
%start programa