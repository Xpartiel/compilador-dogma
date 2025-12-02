/* ===================================
           SECCIÓN 1: C prologue      
    copiado al inicio del .c generado
   =================================== */
%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



/* Forward del lexer */
int yylex(void);


/* Reporte de errores sintácticos */
void yyerror(const char *s) {
    fprintf(stderr, "Error Sintactico: %s\n", s);
}

%}



/* =====================
        SECCIÓN 2:
    Declaraciones Bison
       TOKENS Y TIPOS   
   ===================== */
%code requires {
    #include "ast.h"
}


%union {
    double num;      /* NUMBER token */
    char* id;        /* ID token */
    char* str;       /* LIT_STRING token */
    int boolean;     /* (opcional) si llegas a tener literales booleanos */
    ASTNode* node;   /* para no-terminales que producen AST */
}

/* Tokens con valor */
%token <num> NUMBER
%token <str> LIT_STRING
%token <id> ID


/* Tokens sin valor inmediato */
%token ASSIGN PARENTHESES_OPEN PARENTHESES_CLOSE
%token BEGIN_LOOP END_LOOP BREAK CONTINUE RETURN_VALUE RETURN_TYPE
%token IF ELSE_IF ELSE
%token TYPE_DOUBLE TYPE_FLOAT TYPE_INTEGER TYPE_STRING TYPE_BOOLEAN TYPE_LIST TYPE_ARRAY
%token ERROR

%token BEGIN_SEQUENCE SEQUENCE_SEPARATOR END_SEQUENCE
%token OP_EQUALS OP_LESSER OP_GREATER OP_GREATER_EQUAL OP_LESSER_EQUAL
%token OP_PLUS OP_MINUS OP_TIMES OP_DIVIDE
%token BOOL_AND BOOL_OR BOOL_NOT

/* Precedencias */
%right BOOL_NOT
%left  BOOL_AND
%left  BOOL_OR
%left  OP_EQUALS
%left  OP_LESSER OP_GREATER OP_LESSER_EQUAL OP_GREATER_EQUAL
%right UMINUS
%left  OP_PLUS OP_MINUS
%left  OP_TIMES OP_DIVIDE

/* Tipo de no-terminales (ASTNode) */
%type <node> program statement_list statement expresion

/* Símbolo inicial */
%start program

%%

/* ============================
   SECCIÓN 3: REGLAS (GRAMÁTICA) 
   Aquí creamos nodos AST en las acciones.
   ============================ */

program:
    /* vacío */     { $$ = new_sequence(); }
    | statement_list    { $$ = $1; }
    ;

statement_list:
    statement  { $$ = new_sequence(); sequence_add( $$ , $2 ); }
    | statement_list statement { sequence_add( $1 , $2 ); $$ = $1; }
    ;

block:
    BEGIN_SEQUENCE statement_list END_SEQUENCE  { $$ = $2; }

statement:
    expresion SEQUENCE_SEPARATOR    { $$ = $1; }
    | ID ASSIGN expresion SEQUENCE_SEPARATOR {
            ASTNode *node = new_assign($1, $3);
            double val = eval_ast($3);
            printf("[Asignacion]: %s = %g\n", $1, val);
            guardar($1, val);
            $$ = node;
        }
    | if_statement  { $$ = $1; }
    ;

expresion:
    NUMBER  {
        $$ = new_num( $1 );
        printf("[Numero]: %g\n", $1); }
    | LIT_STRING    {
        $$ = new_string($1);
        printf("String reconocida: %s\n" , $1 ); }
    | ID    {
        $$ = new_identifier( $1 );
        printf( "Variable %s" , $1 ); }
    | BOOL_NOT expresion    {
        $$ = new_unop( "!" , $2 );
        printf( "Negacion NOT: %g" , eval_ast($$) );}
    | OP_MINUS expresion %prec UMINUS   {
            $$ = new_unop("-", $2);
            printf("Unario negativo aplicado: %g\n", eval_ast($$));
        }
    | PARENTHESES_OPEN expresion PARENTHESES_CLOSE { $$ = $2; }
    | expresion OP_PLUS expresion   {
        $$ = new_binop( "+" , $1 , $3 );
        printf( "Suma resulta en: %g\n" , eval_ast($$) ); }
    | expresion OP_MINUS expresion   {
        $$ = new_binop( "-" , $1 , $3 );
        printf( "Resta resulta en: %g\n" , eval_ast($$) ); }
    | expresion OP_TIMES expresion   {
        $$ = new_binop( "*" , $1 , $3 );
        printf( "Producto resulta en: %g\n" , eval_ast($$) ); }
    | expresion OP_DIVIDE expresion   {
        if( eval_ast($3) == 0.0 ){
            yyerror("division por 0");
            $$ = new_num(0.0);
        }else{
            $$ = new_binop( "/" , $1 , $3 );
            printf( "Division resulta en: %g\n" , eval_ast($$) ); }}
    | expresion OP_EQUALS expresion {
        $$ = new_binop( "=" , $1 , $3 );
        printf( "Comparacion de Igualdad: %g\n" , eval_ast($$) );}
    | expresion OP_LESSER expresion {
        $$ = new_binop( "<" , $1 , $3 );
        printf( "Comparacion menor que: %g\n" , eval_ast($$) );}
    | expresion OP_GREATER expresion    {
        $$ = new_binop( ">" , $1 , $3 );
        printf( "Comparacion mayor que: %g\n" , eval_ast($$) );}
    | expresion OP_LESSER_EQUAL expresion   {
        $$ = new_binop( "<=" , $1 , $3 );
        printf( "Comparacion menor o igual que: %g\n" , eval_ast($$) );}
    | expresion OP_GREATER_EQUAL expresion  {
        $$ = new_binop( ">=" , $1 , $3 );
        printf( "Comparacion menor o igual que: %g\n" , eval_ast($$) );}
    | expresion BOOL_AND expresion  {
        $$ = new_binop( "&&" , $1 , $3 );
        printf( "Operacion AND: %g\n" ,eval_ast($$) ); }
    | expresion BOOL_OR expresion   {
        $$ = new_binop( "||" , $1 , $3 );
        printf( "Operacion OR: %g\n" ,eval_ast($$) ); }
    ;

if_statement:
    expresion IF block SEQUENCE_SEPARATOR {
        $$ = new_if($1, $3, NULL, NULL);
        printf("[CONDICIONAL SIMPLE]\n");   }
    | expresion IF block elif_chain SEQUENCE_SEPARATOR {
        $$ = new_if($1, $3, $4, NULL);
        printf("[CONDICIONAL CON ELSE_IF, NO ELSE]\n");   }
    | expresion IF block else_part SEQUENCE_SEPARATOR   {
        $$ = new_if($1, $3, NULL, $4);
        printf("[CONDICIONAL CON ELSE, NO ELSE_IF]\n");}
    | expresion IF block elif_chain else_part SEQUENCE_SEPARATOR   {
        $$ = new_if($1, $3, $4, $5);
        printf("[CONDICIONAL CON ELSE_IF Y ELSE]\n"); }
    ;

elif_chain:
    ELSE_IF expresion IF block  {
        $$ = new_elseif($2, $4, NULL);
        printf("[ELSE_IF LAST - BRANCH]\n");}
    | ELSE_IF expresion IF block elif_chain {
        $$ = new_elseif($2, $4, $5);
        printf("[ELSE_IF BRANCH]\n");}
;

else_part:
    ELSE block  { $$ = $2; }
;

%%

/* ============================
   SECCIÓN 4: C epilogue (main opcional)
   ============================ */

int main(int argc, char** argv){
    printf("Parser AST - inicia. Introduce sentencias terminadas en ';'\n");
    yyparse();
    return 0;
}
