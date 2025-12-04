#ifndef AST_H
#define AST_H

#include "type.h"

/* ===========================================
    INTERFAZ PARA ARBOL DE SINTAXIS ABSTRACTA
   =========================================== */


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

/* Declaracion Adelantada de Nodo */
typedef struct ASTNode ASTNode;

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