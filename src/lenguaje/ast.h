#ifndef AST_H
#define AST_H

#include "type.h"

/* ===========================================
    INTERFAZ PARA ARBOL DE SINTAXIS ABSTRACTA
   =========================================== */


/* Declaracion Adelantada de Nodo */
typedef struct ASTNode ASTNode;

/* ----------------------------------
    Enumeracion de tipos de nodos AST
   ----------------------------------- */
typedef enum {
    AST_NUMBER,
    AST_STRING,
    AST_BOOLEAN,
    AST_IDENTIFIER,
    AST_BINOP,
    AST_UNOP,
    AST_ASSIGN,
    AST_DECLARATION,
    AST_SEQUENCE,
    AST_IF,
    AST_ELSEIF,
    AST_LOOP,
    AST_ARRAY,
    AST_LIST,
    AST_INIT_LIST,
    AST_RETURN,
    AST_BREAK,
    AST_CONTINUE,
    AST_FUNCTION_DECL,
    AST_FUNCTION_CALL
} ASTKind;


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



int sequence_length( ASTNode * seq );
ASTNode **sequence_to_array( ASTNode *seq );

/* ---------------
    CONSTRUCTORES
   --------------- */
ASTNode *new_num(double);
ASTNode *new_string(char *);
ASTNode *new_boolean(int);

ASTNode *new_identifier(char *);

ASTNode *new_binop( char * , ASTNode *, ASTNode *);
ASTNode *new_unop( char * , ASTNode *);

ASTNode *new_assign(char * , ASTNode *);
ASTNode *new_declaration(char * , Type * , ASTNode * );

ASTNode *new_sequence();
void    sequence_add(ASTNode * , ASTNode *);

ASTNode *new_if(ASTNode * , ASTNode * , ASTNode * , ASTNode * );
ASTNode *new_elseif(ASTNode * ,ASTNode * ,ASTNode * );

ASTNode *new_loop(ASTNode * );
ASTNode *new_return(ASTNode *value);
ASTNode *new_break();
ASTNode *new_continue();

ASTNode* new_function_declaration(char *name, ASTNode **params, int param_count , Type *ret_type, ASTNode *body);
ASTNode* new_function_call(char *name, ASTNode **args , int arg_count);

ASTNode *new_array( Type * , ASTNode * );
ASTNode *new_list( Type * , ASTNode * );
ASTNode *new_init_data_structure();
void init_data_structure_add( ASTNode * , ASTNode * );

double  eval_ast( ASTNode * );
void free_ast( ASTNode * );

double  obtener( char * );
void    guardar( char * , double );

#endif