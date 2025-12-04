/* ===================================
           SECCIÓN 1: C prologue      
    copiado al inicio del .c generado
   =================================== */
%{
#include "ast.h"
#include "type.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ASTNode *root = NULL;

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
    #include "type.h"
    #include "semantic.h"
}


%union {
    double num; /* NUMBER token */
    char* id;   /* ID token */
    char* str;  /* LIT_STRING token */
    int boolean;
    ASTNode* node;  /* para no-terminales que producen AST */
    Type* type;     /* identificador de tipos */
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
%token LIST_SEPARATOR

%token BEGIN_SEQUENCE SEQUENCE_SEPARATOR END_SEQUENCE
%token OP_EQUALS OP_LESSER OP_GREATER OP_GREATER_EQUAL OP_LESSER_EQUAL
%token OP_PLUS OP_MINUS OP_TIMES OP_DIVIDE
%token BOOL_AND BOOL_OR BOOL_NOT

/* Precedencias */
%left  BOOL_AND
%left  BOOL_OR
%right BOOL_NOT
%left  OP_EQUALS
%left  OP_LESSER OP_GREATER OP_LESSER_EQUAL OP_GREATER_EQUAL
%right UMINUS
%left  OP_PLUS OP_MINUS
%left  OP_TIMES OP_DIVIDE

/* Tipo de no-terminales (ASTNode) */
%type <node> program statement_list statement expresion block if_statement elif_chain else_part
%type <node> loop_expr var_declaration init_data_structure list_item return_expr continue_expr break_expr
%type <node> function_call fun_declaration optional_arg_list arg_list param_list param_decl  
%type <node> optional_param_list var_init assign_expr
%type <type> type

/* Símbolo inicial */
%start program

%%

/* ============================
   SECCIÓN 3: REGLAS (GRAMÁTICA) 
   Aquí creamos nodos AST en las acciones.
   ============================ */

program:
    /* vacío */     {
        root = new_sequence(); }
    | statement_list    { root = $1; }
    ;

statement_list:
    statement   {
        $$ = new_sequence();
        sequence_add( $$ , $1 ); }
    | statement_list statement {
        sequence_add( $1 , $2 );
        $$ = $1; }
    ;


/* { codigo } */
block:
    BEGIN_SEQUENCE statement_list END_SEQUENCE  {
        $$ = $2; }
    ;

continue_expr:
    CONTINUE SEQUENCE_SEPARATOR { $$ = new_continue(); }
    ;

break_expr:
    BREAK SEQUENCE_SEPARATOR    { $$ = new_break(); }
    ;

return_expr:
    RETURN_VALUE expresion SEQUENCE_SEPARATOR { $$ = new_return($2); }
    ;

statement:
    fun_declaration   { $$ = $1; }
    | expresion SEQUENCE_SEPARATOR  { $$ = $1; }
    | return_expr   { $$ = $1; }
    | continue_expr { $$ = $1; }
    | break_expr    { $$ = $1; }
    | assign_expr SEQUENCE_SEPARATOR    { $$ = $1; }
    | var_declaration   { $$ = $1; }
    | if_statement  { $$ = $1; }
    | loop_expr     { $$ = $1; }
    | block { $$ = $1; }
    ;

expresion:
    NUMBER  {
        $$ = new_num( $1 );  }
    | LIT_STRING    {
        $$ = new_string( $1 ); }
    | ID    {
        $$ = new_identifier( $1 ); }
    | BOOL_NOT expresion    {
        $$ = new_unop( "!" , $2 ); }
    | OP_MINUS expresion %prec UMINUS   {
        $$ = new_unop("-", $2); }
    | PARENTHESES_OPEN expresion PARENTHESES_CLOSE {
        $$ = $2; }
    | expresion OP_PLUS expresion   {
        $$ = new_binop( "+" , $1 , $3 ); }
    | expresion OP_MINUS expresion  {
        $$ = new_binop( "-" , $1 , $3 ); }
    | expresion OP_TIMES expresion   {
        $$ = new_binop( "*" , $1 , $3 );  }
    | expresion OP_DIVIDE expresion   {
        if( eval_ast($3) == 0.0 ){
            yyerror("division por 0"); /* // TODO REMOVE*/
            $$ = new_num(0.0);
        }else{
            $$ = new_binop( "/" , $1 , $3 );  } }
    | expresion OP_EQUALS expresion {
        $$ = new_binop( "=" , $1 , $3 ); }
    | expresion OP_LESSER expresion {
        $$ = new_binop( "<" , $1 , $3 ); }
    | expresion OP_GREATER expresion    {
        $$ = new_binop( ">" , $1 , $3 ); }
    | expresion OP_LESSER_EQUAL expresion   {
        $$ = new_binop( "<=" , $1 , $3 ); }
    | expresion OP_GREATER_EQUAL expresion  {
        $$ = new_binop( ">=" , $1 , $3 ); }
    | expresion BOOL_AND expresion  {
        $$ = new_binop( "&&" , $1 , $3 ); }
    | expresion BOOL_OR expresion   {
        $$ = new_binop( "||" , $1 , $3 ); }
    | function_call {
        $$ = $1; }
    | TYPE_ARRAY type PARENTHESES_OPEN expresion PARENTHESES_CLOSE {
        $$ = new_array($2, $4); }
    | TYPE_LIST type PARENTHESES_OPEN expresion PARENTHESES_CLOSE {
        $$ = new_list($2, $4);  }
    ;

if_statement:
    expresion IF block SEQUENCE_SEPARATOR {
        $$ = new_if($1, $3, NULL, NULL); }
    | expresion IF block elif_chain SEQUENCE_SEPARATOR {
        $$ = new_if($1, $3, $4, NULL); }
    | expresion IF block else_part SEQUENCE_SEPARATOR   {
        $$ = new_if($1, $3, NULL, $4); }
    | expresion IF block elif_chain else_part SEQUENCE_SEPARATOR   {
        $$ = new_if($1, $3, $4, $5); }
    ;

elif_chain:
    ELSE_IF expresion IF block  {
        $$ = new_elseif($2, $4, NULL); }
    | ELSE_IF expresion IF block elif_chain {
        $$ = new_elseif($2, $4, $5); }
    ;

else_part:
    ELSE block  {
        $$ = $2; }
    ;

loop_expr:
    BEGIN_LOOP statement_list END_LOOP SEQUENCE_SEPARATOR   {
        $$ = new_loop($2); }
    ;

type:
    TYPE_INTEGER    {
        $$ = new_type( T_integer , NULL ); }
    | TYPE_FLOAT    {
        $$ = new_type( T_float , NULL ); }
    | TYPE_DOUBLE   {
        $$ = new_type( T_double , NULL ); }
    | TYPE_BOOLEAN  { 
        $$ = new_type( T_boolean , NULL); }
    | TYPE_STRING   { 
        $$ = new_type( T_string , NULL ); }
    | TYPE_ARRAY type   { 
        $$ = new_type( T_array , $2 );
        print_type($2); }
    | TYPE_LIST type    {
        $$ = new_type( T_list , $2 );
        print_type($2); }
    ;

// tipo variable ( numero );

var_init:
      /* vacío */ { $$ = NULL; }
    | ASSIGN expresion  {
        /* <- expr asignación normal */
        $$ = $2; }
    | ASSIGN init_data_structure {
        /* array/lista con inicialización */
        $$ = $2; }
    ;

assign_expr:
    ID ASSIGN expresion { $$ = new_assign( $1 , $3 ); }
    ;

/*
Declaracion de variable con tipo. Inicializacion opcional
*/
var_declaration:
    type ID var_init SEQUENCE_SEPARATOR {
        /* $1 = type, $2 = ID, $3 = var_init (puede ser NULL) */
        $$ = new_declaration($2, $1, $3);
    }
    ;



init_data_structure:
    BEGIN_SEQUENCE list_item END_SEQUENCE {
        $$ = $2;
    };

list_item:
    expresion {
        $$ = new_init_data_structure();
        init_data_structure_add( $$ , $1 ); }
    | list_item LIST_SEPARATOR expresion{
        init_data_structure_add( $1 , $3 );
        $$ = $1;
    }

param_decl:
    type ID{ $$ = new_declaration($2, $1, NULL);}
    ;

param_list:
    param_decl  { 
        $$ = new_sequence();
        sequence_add($$, $1); }
    | param_list LIST_SEPARATOR param_decl {
        sequence_add($1, $3);
        $$ = $1; }
    ;

optional_param_list:
    /* vacío */         { $$ = new_sequence(); }
    | param_list           { $$ = $1; }
    ;

fun_declaration:
    type RETURN_TYPE ID PARENTHESES_OPEN optional_param_list PARENTHESES_CLOSE block SEQUENCE_SEPARATOR{
        /* nombreFuncion ( args ) -> type { ... }; */
        int param_count = sequence_length($5);
        ASTNode **params_array = sequence_to_array($5);
        $$ = new_function_declaration($3, params_array, param_count, $1, $7);
    }
    ;

arg_list:
    expresion   {
        $$ = new_sequence();
        sequence_add($$, $1);   }
    | arg_list LIST_SEPARATOR expresion {
        sequence_add($1, $3);
        $$ = $1; }
    ;

optional_arg_list:
    /* vacío */ { $$ = new_sequence(); }
    | arg_list    { $$ = $1; }
    ;

function_call:
    ID PARENTHESES_OPEN optional_arg_list PARENTHESES_CLOSE  {
        /* nombreFuncion ( args ) */
        int arg_count = sequence_length($3); 
        ASTNode **args_array = sequence_to_array($3);
        $$ = new_function_call($1, args_array, arg_count);  }
    ;


%%

/* ============================
   SECCIÓN 4: C epilogue (main opcional)
   ============================ */

int main(int argc, char** argv){
    printf("Parser AST - inicia. Introduce sentencias terminadas en ';'\n");
    yyparse();

    if( !root ){
        printf("No se generó AST.\n");
        return 1;
    }

    // 2. ANALISIS SEMANTICO
    sem_init();
    sem_check_program(root);

    if (sem_errors > 0) {
        printf("Errores semánticos: el código intermedio NO se generará.\n");
        return 1;
    }

    return 0;
}
