#ifndef AST_H
#define AST_H

/* ===========================================
    INTERFAZ PARA ARBOL DE SINTAXIS ABSTRACTA
   =========================================== */

#include <stdbool.h>


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
    AST_SEQUENCE,
    AST_IF,
    AST_ELSEIF
    AST_LOOP
} ASTKind;

/* Declaracion Adelantada de Nodo */
typedef struct ASTNode ASTNode;

/* ---------------
    CONSTRUCTORES
   --------------- */
ASTNode *new_num(double);
ASTNode *new_string(char *);
ASTNode *new_boolean(int);
ASTNode *new_identifier(char *);
ASTNode *new_binop( char *, ASTNode *, ASTNode *);
ASTNode *new_unop( char *, ASTNode *);
ASTNode *new_assign(char *, ASTNode *);
ASTNode *new_sequence();
void    sequence_add(ASTNode *, ASTNode *);
double  eval_ast( ASTNode * );
double  obtener( char * );
void    guardar( char * , double );

#endif