#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================
    IMPLEMENTACION ASTNode
   ======================== */

/* Tabla simple de variables */
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
    char *name;
    Type *type;
    ASTNode *init;
} AST_Declaration;

typedef struct {
    ASTNode** list;
    int count;
    int cap;
} AST_Sequence;


typedef struct {
    ASTNode* condition; // obligatorio
    ASTNode* if_branch; // obligatorio
    ASTNode* elif_list; // null si no hay else-if
    ASTNode* else_branch;   // null si no hay rama elses
} AST_If;


typedef struct {
    ASTNode* condition; // obligatrio
    ASTNode* branch;    // then, oblgatorio
    ASTNode* next;  // branch chaining, null si es el ultimo else-if
} AST_ElseIf ;


typedef struct {
    ASTNode *body;
}AST_Loop;


typedef struct{
    Type *type;
    ASTNode *size_expr;
} AST_Array;

typedef struct{
    ASTNode ** elements;
    int count;
    int cap;
} AST_InitDataStructure;

typedef struct{
    Type *type;
    ASTNode *size_expr;
} AST_List;

typedef struct {
    ASTNode *value;
} AST_Return;

typedef struct {
     int dummy;
} AST_Break;

typedef struct {
    int dummy;
} AST_Continue;


typedef struct {
    char *name; // nombre de la función
    ASTNode **params;    // lista de parámetros (AST_SEQUENCE de declaraciones)
    int param_count;
    Type *return_type;  // tipo declarado: T_integer, T_boolean, etc.
    ASTNode *body;       // bloque { }
} AST_FunctionDecl;

typedef struct {
    char *name; // nombre de la función a ejecutar
    ASTNode **args;       // lista de expresiones (AST_SEQUENCE)
    int arg_count;
} AST_FunctionCall;


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
        AST_Declaration declaration;
        AST_Sequence sequence;
        AST_If conditional;
        AST_ElseIf chained_conditional;
        AST_Loop loop_type;
        AST_Array array;
        AST_List list;
        AST_InitDataStructure initializator;
        AST_Return result;
        AST_Continue continuing;
        AST_Break breaking;
        AST_FunctionDecl function_declaration;
        AST_FunctionCall function_call;
    };
};

int sequence_length(ASTNode *seq) {
    if (!seq || seq->kind != AST_SEQUENCE) return 0;
    return seq->sequence.count;
}

ASTNode **sequence_to_array(ASTNode *seq) {
    if (!seq || seq->kind != AST_SEQUENCE) return NULL;
    int len = seq->sequence.count;
    ASTNode **arr = malloc(sizeof(ASTNode*) * len);
    for (int i = 0; i < len; i++) {
        arr[i] = seq->sequence.list[i];
    }
    return arr;
}

/* ------------------------------
    IMPLEMENTACION CONSTRUCTORES
   ------------------------------ */

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
    n->kind = AST_BOOLEAN;
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
    n->assign.name = strdup(id);
    n->assign.value = expr;
    return n;
}

ASTNode* new_declaration(char* name,
                        Type* t,
                        ASTNode* init) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_DECLARATION;
    n->declaration.name = name;
    n->declaration.type = t;
    n->declaration.init = init;
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


ASTNode* new_if(ASTNode* cond,
                ASTNode* if_branch,
                ASTNode* elseif_list,
                ASTNode* else_branch){
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_IF;
    n->conditional.condition = cond;
    n->conditional.if_branch = if_branch;
    n->conditional.elif_list = elseif_list;
    n->conditional.else_branch = else_branch;
    return n;
}

ASTNode* new_elseif(ASTNode* cond,
                    ASTNode* branch,
                    ASTNode* next){
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_ELSEIF;
    n->chained_conditional.condition = cond;
    n->chained_conditional.branch = branch;
    n->chained_conditional.next = next;
    return n;
}

ASTNode* new_loop(ASTNode* body) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->kind = AST_LOOP;
    n->loop_type.body = body;
    return n;
}


ASTNode* new_array(Type *t, ASTNode *size_expr) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_ARRAY;
    n->array.type = t;
    n->array.size_expr = size_expr;
    return n;
}

ASTNode *new_init_data_structure(){
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_INIT_LIST;
    n->initializator.count = 0;
    n->initializator.cap = 4;
    n->initializator.elements = malloc(sizeof(ASTNode*) * 4);
    return n;
}

void init_data_structure_add(ASTNode *lst, ASTNode *elem){
    if(lst->initializator.count == lst->initializator.cap){
        lst->initializator.cap *= 2;
        lst->initializator.elements =
            realloc(lst->initializator.elements, lst->initializator.cap * sizeof(ASTNode*));
    }
    lst->initializator.elements[lst->initializator.count++] = elem;
}

ASTNode* new_list(Type *t, ASTNode *size_expr) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_LIST;
    n->list.type = t;
    n->list.size_expr = size_expr;
    return n;
}
ASTNode *new_return(ASTNode *value) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_RETURN;
    n->result.value = value;
    return n;
}


ASTNode *new_break() {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_BREAK;
    return n;
}

ASTNode *new_continue() {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_CONTINUE;
    return n;
}


ASTNode *new_function_declaration(char *name,
                            ASTNode **params,
                            int param_count,
                            Type *return_type,
                           ASTNode *body)
{
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_FUNCTION_DECL;

    n->function_declaration.name = strdup(name);
    n->function_declaration.return_type = return_type;
    n->function_declaration.params = params;
    n->function_declaration.param_count = param_count;
    n->function_declaration.body = body;

    return n;
}

ASTNode *new_function_call(char *name, ASTNode **args, int arg_count) 
{
    ASTNode *n = malloc(sizeof(ASTNode));
    n->kind = AST_FUNCTION_CALL;

    n->function_call.name = strdup(name);
    n->function_call.args = args;
    n->function_call.arg_count = arg_count;

    return n;
}


/* Liberar AST (recursivo) */
void free_ast(ASTNode* n){
    if(!n) return;
    switch(n->kind){
        case AST_NUMBER: break;
        case AST_STRING:
            free(n->str); break;
        case AST_BOOLEAN: break;
        case AST_IDENTIFIER:
            free(n->id); break;
        case AST_BINOP:
            free_ast(n->binop.left);
            free_ast(n->binop.right);
            break;
        case AST_UNOP:
            free_ast(n->unop.expr);
            break;
        case AST_ASSIGN:
            free(n->assign.name);
            free_ast(n->assign.value);
            break;
        case AST_SEQUENCE:
            for(int i=0; i<n->sequence.count;i++){
                free_ast(n->sequence.list[i]);
            }free(n->sequence.list);
            break;
        case AST_IF:
            free_ast(n->conditional.condition);
            free_ast(n->conditional.if_branch);
            free_ast(n->conditional.elif_list);
            free_ast(n->conditional.else_branch);
            break;
        case AST_ELSEIF:
            free_ast(n->chained_conditional.condition);
            free_ast(n->chained_conditional.branch);
            free_ast(n->chained_conditional.next);
            break;
        case AST_LOOP:
            free_ast(n->loop_type.body);
            break;
        case AST_ARRAY:
            if(n->array.type) free_type(n->array.type);
            free_ast(n->array.size_expr);
            break;
        case AST_LIST:
            if(n->list.type) free_type(n->list.type);
            free_ast(n->list.size_expr);
            break;
        case AST_FUNCTION_DECL:
            free(n->function_declaration.name);
            if (n->function_declaration.return_type)
                free_type(n->function_declaration.return_type);
            /*
            for (int i = 0; i < n->function_declaration.param_count; i++)
                free_ast(n->function_declaration.params[i]);
            */
            free(n->function_declaration.params);
            free_ast(n->function_declaration.body);
            break;

        case AST_FUNCTION_CALL:
            free(n->function_call.name);
            /*
            for (int i = 0; i < n->function_call.arg_count; i++)
                free_ast(n->function_call.args[i]);
            */
            free(n->function_call.args);
            break;
    }free(n);
}


/* Evaluador simple: devuelve double (0.0 = false, !=0 = true).
   Ejecuta ASSIGN (usa guardar()) y devuelve el valor asignado. */
double eval_ast(ASTNode* n){
    if(!n) return 0.0;
    switch(n->kind){
        case AST_NUMBER:
            printf("Evaluando numero\n");
            return n->num;

        case AST_STRING:
            printf("[eval] Intento de evaluar string: \"%s\"\n", n->str);
            return 0.0;
        
        case AST_BOOLEAN:
            return n->boolean ? 1.0 : 0.0;

        case AST_IDENTIFIER:
            return obtener(n->id);
        
        case AST_ASSIGN: {
            return eval_ast(n->assign.value);
        }
        case AST_UNOP: {
            double v = eval_ast(n->unop.expr);
            
            if(strcmp(  n->unop.op, "-") == 0){
                return -v;
            }else if(strcmp(n->unop.op, "!") == 0){
                return (v == 0.0) ? 1.0 : 0.0;
            }else{
                printf("WARNING: Operacion unaria \"%s\" no reconocida",n->unop.op);
                return 0.0;
            }
        }
        case AST_BINOP: {
            double L = eval_ast(n->binop.left);
            double R = eval_ast(n->binop.right);
            const char* op = n->binop.op;
            if(strcmp(op, "+") == 0){
                return L + R;
            }else if(strcmp(op, "-") == 0){
                return L - R;
            } else if(strcmp(op, "*") == 0){
                return L * R;
            } else if(strcmp(op, "/") == 0){
                if(R == 0.0){
                    printf("division por cero");
                    return 0.0;
                }return L / R;
            } else if(strcmp(op, "=") == 0){
                return (L == R) ? 1.0 : 0.0;
            }else if(strcmp(op, "<") == 0){
                return (L < R) ? 1.0 : 0.0;
            }else if(strcmp(op, ">") == 0){
                return (L > R) ? 1.0 : 0.0;
            }else if(strcmp(op, "<=") == 0){
                return (L <= R) ? 1.0 : 0.0;
            }else if(strcmp(op, ">=") == 0){
                return (L >= R) ? 1.0 : 0.0;
            }else if(strcmp(op, "&&") == 0){
                return ( (L!=0.0) && (R!=0.0) ) ? 1.0 : 0.0;
            }else if(strcmp(op, "||") == 0){
                return ( (L!=0.0) || (R!=0.0) ) ? 1.0 : 0.0;
            }else{
                printf("Operador Binario \"%s\" no reconocido",op);
            }
            return 0.0;
        }
        case AST_SEQUENCE:{
            double last = 0.0;
            for(int i=0;i<n->sequence.count;i++){
                last = eval_ast(n->sequence.list[i]);
            }
            return last;
        }
        case AST_IF:{
            double c = eval_ast(n->conditional.condition);
            if(c != 0.0){
                return eval_ast(n->conditional.if_branch);
            }
            ASTNode* eif = n->conditional.elif_list;
            while(eif != NULL) {
                if(eval_ast(eif->chained_conditional.condition)) {
                    return eval_ast(eif->chained_conditional.branch);
                }
                eif = eif->chained_conditional.next;
            }
            if(n->conditional.else_branch != NULL)
                return eval_ast(n->conditional.else_branch);
            return 0.0;
        }
        case AST_LOOP:{
            /* while(1){ // REAL */ 
            for(int i=0; i<100; i++){ // TESTING */
                eval_ast(n->loop_type.body);
            }
            return 0.0;
        }
        /*
        case AST_ARRAY:{
            double size = eval_ast(n->array.size_expr);
            printf("[NEW_ARRAY] tipo=");
            print_type(n->array.type);
            printf(" size=%.0f\n", size);
            // Retornamos un "handle" simple: la dirección truncada como double (no portátil pero útil para pruebas)
            double handle = (double) (intptr_t) n; // pseudo-handle
            return handle;
        }
        case AST_LIST:{
            double size = eval_ast(n->list.size_expr);
            printf("[NEW_LIST] tipo=");
            print_type(n->list.type);
            printf(" size=%.0f\n", size);
            double handle = (double) (intptr_t) n;
            return handle;
        }
        */
        default:
            return 0.0;
    }
}