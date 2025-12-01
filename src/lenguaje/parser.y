%{
/*  ============================
    SECCIÓN 1: C prologue
    Código C que se copia al inicio del .c generado.
    ============================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Tabla simple de variables (la que compartiste) */
typedef struct{
    char *nombre;
    double valor;
} Variable;

Variable tabla[256];
int vars_act = 0;

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
    tabla[vars_act].valor = val;
    tabla[vars_act].nombre = strdup(id);
    vars_act++;
}

/* Forward del lexer */
int yylex(void);

/* Reporte de errores sintácticos */
void yyerror(const char *s) {
    fprintf(stderr, "Error Sintactico: %s\n", s);
}

/* ========================
    DEFINICION DE TIPOS AST
   ======================== */

typedef enum {
    AST_NUMBER,
    AST_STRING,
    AST_BOOLEAN,
    AST_IDENTIFIER,
    AST_BINOP,
    AST_UNOP,
    AST_ASSIGN,
    AST_SEQUENCE,
    AST_IF,
    AST_LOOP,
    AST_RETURN
} ASTKind;

typedef struct ASTNode ASTNode;

typedef struct {
    char op[4];
    ASTNode *left;
    ASTNode *right;
} AST_BinOp;

typedef struct {
    char op[4];
    ASTNode *expr;
} AST_UnOp;

typedef struct {
    char *name;
    ASTNode *value;
} AST_Assign;

typedef struct {
    ASTNode** list;
    int count;
    int cap;
} AST_Sequence;

struct ASTNode {
    ASTKind kind;
    union {
        double num;
        char *str;
        int boolean;
        char *id;
        AST_BinOp binop;
        AST_UnOp unop;
        AST_Assign assign;
        AST_Sequence sequence;
    };
};

/* CONSTRUCTORES */
// TODO malloc revision
ASTNode* new_num(double n){
    ASTNode* node = malloc(sizeof(ASTNode));
    node->kind = AST_NUMBER;
    node->num = n;
    return node;
}

ASTNode *new_string(char *s){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_STRING;
    n->str = s;
    return n;
}

ASTNode* new_boolean(int v){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_BOOL;
    n->boolean = v;
    return n;
}

ASTNode* new_identifier(char* s){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_IDENTIFIER;
    n->id = s;
    return n;
}

ASTNode* new_binop(char* op, ASTNode* l, ASTNode* r){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_BINOP;
    strncpy(n->binop.op, op, 3);
    n->binop.op[3] = 0;
    n->binop.left = l;
    n->binop.right = r;
    return n;
}

ASTNode *new_unop(char *op, ASTNode *expr){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_UNOP;
    strncpy(n->unop.op, op, 3);
    n->unop.op[3] = 0;
    n->unop.expr = expr;
    return n;
}

ASTNode *new_assign(char *id, ASTNode *expr){
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_ASSIGN;
    n->assign.name = id;
    n->assign.value = expr;
    return n;
}


ASTNode *new_sequence(){
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_SEQUENCE;
    n->sequence.count = 0;
    n->sequence.cap = 4;
    n->sequence.list = malloc(sizeof(ASTNode*) * 4);
    return n;
}


void sequence_add(ASTNode *seq, ASTNode *elem){
    if (seq->sequence.count == seq->sequence.cap){
        seq->sequence.cap *= 2;
        seq->sequence.list = realloc(seq->sequence.list,
                                     seq->sequence.cap * sizeof(ASTNode*));
    }
    seq->sequence.list[ seq->sequence.count++ ] = elem;
}

/* Liberar AST (recursivo) */
void free_ast(ASTNode* n){
    if(!n) return;
    switch(n->kind){
        case AST_NUMBER: break;
        case AST_STRING: free(n->str); break;
        case AST_BOOLEAN: break;
        case AST_IDENTIFIER: free(n->id); break;
        case AST_BINOP:
            free_ast(n->binop.left);
            free_ast(n->binop.right);
            break;
        case AST_UNOP:
            free_ast(n->unop.expr);
            break;
        case AST_ASSIGN:
            free(n->assign.id);
            free_ast(n->assign.value);
            break;
        case AST_IF:
            free_ast(n->if_stmt.cond);
            free_ast(n->if_stmt.then_branch);
            free_ast(n->if_stmt.else_branch);
            break;
        case AST_LOOP:
            free_ast(n->loop_stmt.cond);
            free_ast(n->loop_stmt.body);
            break;
        case AST_RETURN:
            free_ast(n->ret.value);
            break;
        case AST_SEQUENCE:
            /* Si implementas secuencias con listas, liberarlas aquí */
            break;
    }
    free(n);
}


/* Evaluador simple: devuelve double (0.0 = false, !=0 = true).
   Ejecuta ASSIGN (usa guardar()) y devuelve el valor asignado. */
double eval_ast(ASTNode* n){
    if(!n) return 0.0;
    switch(n->kind){
        case AST_NUMBER: return n->num;
        case AST_STRING: /* strings no se usan en eval numérico */ return 0.0;
        case AST_BOOLEAN: return n->boolean ? 1.0 : 0.0;
        case AST_IDENTIFIER: return obtener(n->id);
        case AST_ASSIGN: {
            double v = eval_ast(n->assign.value);
            guardar(n->assign.id, v);
            return v;
        }
        case AST_UNOP: {
            double v = eval_ast(n->unop.expr);
            if(strcmp(n->unop.op, "-") == 0) return -v;
            if(strcmp(n->unop.op, "!") == 0) return (v == 0.0) ? 1.0 : 0.0;
            return 0.0;
        }
        case AST_BINOP: {
            double L = eval_ast(n->binop.left);
            double R = eval_ast(n->binop.right);
            const char* op = n->binop.op;
            if(strcmp(op, "+") == 0) return L + R;
            if(strcmp(op, "-") == 0) return L - R;
            if(strcmp(op, "*") == 0) return L * R;
            if(strcmp(op, "/") == 0) { if(R == 0.0) { yyerror("division por cero"); return 0.0; } return L / R; }
            //if(strcmp(op, "==") == 0) return (L == R) ? 1.0 : 0.0;
            if(strcmp(op, "=") == 0) return (L == R) ? 1.0 : 0.0; /* igualdad sintáctica si la usas */
            if(strcmp(op, "<") == 0) return (L < R) ? 1.0 : 0.0;
            if(strcmp(op, ">") == 0) return (L > R) ? 1.0 : 0.0;
            if(strcmp(op, "<=") == 0) return (L <= R) ? 1.0 : 0.0;
            if(strcmp(op, ">=") == 0) return (L >= R) ? 1.0 : 0.0;
            if(strcmp(op, "&&") == 0) return ( (L!=0.0) && (R!=0.0) ) ? 1.0 : 0.0;
            if(strcmp(op, "||") == 0) return ( (L!=0.0) || (R!=0.0) ) ? 1.0 : 0.0;
            return 0.0;
        }
        default:
            return 0.0;
    }
}
%}

/* ================================
    SECCIÓN 2: Declaraciones Bison
   ================================ */

/* union con primitivas y nodo */
%union {
    ASTNode* node;   /* para no-terminales que producen AST */
    double num;      /* NUMBER token */
    char* id;        /* ID token */
    char* str;       /* LIT_STRING token */
    int boolean;     /* (opcional) si llegas a tener literales booleanos */
}

/* Tokens (sin modificar nombres) */
%token <num> NUMBER
%token <str> LIT_STRING
%token <id> ID
%token ASSIGN       

%token BEGIN_LOOP  END_LOOP BREAK CONTINUE RETURN_VALUE RETURN_TYPE
%token IF ELSE_IF ELSE
%token TYPE_DOUBLE TYPE_FLOAT TYPE_INTEGER TYPE_STRING TYPE_BOOLEAN TYPE_LIST TYPE_ARRAY

%token BEGIN_SEQUENCE SEQUENCE_SEPARATOR END_SEQUENCE
%token OP_EQUALS OP_LESSER OP_GREATER OP_GREATER_EQUAL OP_LESSER_EQUAL  // <=
%token BOOL_AND BOOL_OR BOOL_NOT

/* Precedencias (de menor a mayor) */
%right BOOL_NOT
%left  BOOL_AND
%left  BOOL_OR
%left  OP_EQUALS
%left  OP_LESSER OP_GREATER OP_LESSER_EQUAL OP_GREATER_EQUAL
%left  '+' '-'
%left  '*' '/'
%right UMINUS

/* Los no-terminals principales devuelven ASTNode* */
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
    statement   { $$ = new_sequence(); sequence_add( $$ , $1 ); }
    | statement_list statement { sequence_add( $1 , $2 ); $$ = $1; }
    ;

statement:
    expresion SEQUENCE_SEPARATOR    { $$ = $1; }
    | ID ASSIGN expresion SEQUENCE_SEPARATOR { $$ = new_assign( $1 , $3 ); }
    ;

expresion:
    NUMBER  { $$ = new_num( $1 ); }
    | LIT_STRING    { $$ = new_string( $1 ); }
    | ID    { $$ = new_identifier( $1 ); }
    | expresion '+' expresion   { $$ = new_binop( "+" , $1 , $3 ); }
    | expresion '-' expresion   { $$ = new_binop( "-" , $1 , $3 ); }
    | expresion '*' expresion   { $$ = new_binop( "*" , $1 , $3 ); }
    | expresion '/' expresion   { $$ = new_binop( "/" , $1 , $3 ); }
    | expresion OP_EQUALS expresion { $$ = new_binop( "=" , $1 , $3 ); }
    | expresion OP_LESSER expresion { $$ = new_binop( "<" , $1 , $3 ); }
    | expresion OP_GREATER expresion    { $$ = new_binop( ">" , $1 , $3 ); }
    | expresion OP_LESSER_EQUAL expresion   { $$ = new_binop( "<=" , $1 , $3 ); }
    | expresion OP_GREATER_EQUAL expresion  { $$ = new_binop( ">=" , $1 , $3 ); }
    | expresion BOOL_AND expresion  { $$ = new_binop( "&&" , $1 , $3 ); }
    | expresion BOOL_OR expresion   { $$ = new_binop( "||" , $1 , $3 ); }
    | BOOL_NOT expresion    { $$ = new_unop( "!" , $2 ); }
    | '-' expresion %prec UMINUS    { $$ = new_unop( "neg" , $2); }
    | '(' expresion ')' { $$ = $2; }
    ;
%%

/* ============================
   SECCIÓN 4: C epilogue (main opcional)
   ============================ */

int main(int argc, char** argv){
    printf("Parser AST - inicia. Introduce sentencias terminadas en ';'\\n");
    yyparse();
    return 0;
}
