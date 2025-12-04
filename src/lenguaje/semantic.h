#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "type.h"

extern int sem_errors;
extern int sem_warnings;

/*Tipos semanticos*/
/*No se si vamos a agregar más*/
typedef enum {
    S_TYPE_ERROR = -1,
    S_TYPE_INT,
    S_TYPE_FLOAT,
    S_TYPE_DOUBLE,
    S_TYPE_STRING,
    S_TYPE_BOOL,
    S_TYPE_VOID,
    S_TYPE_ARRAY,
    S_TYPE_LIST
} SType;

/*Struct Symbol
    struct con los siguientes atributos:
    -   name: nombre del simbolo.
    -   type: tipo del simbolo (ya especificados en el enum anterior)
*/
typedef struct {
    char *name;
    SType type;
    int declared_scope_level; /* para debug */
    int is_function;
    ASTNode **params;
    int param_count;
    SType return_type;
    Type *type_reference;
} Symbol;

/*Funciones principales para el analizador semantico*/

/*sem_init se encarga de inicializar todo el analizador semantico y crea el scope global*/
void sem_init(void);
/*sem_destroy limpia todos los scopes creados y libera la memoria usada*/
void sem_destroy(void);
/*Crea un nuevo scope y lo pone en el tope de la pila de scopes*/
void sem_enter_scope(void);
/*cierra el ultimo scope creado y destruye la tabla de simbolos*/
void sem_exit_scope(void);

/*agrega el símbolo en el scope actual (falla si ya existe en dicho scope) */
int sem_add_symbol(const char *name, SType type , Type * type_reference );

int sem_add_function( const char * name , ASTNode ** params , int param_count , SType return_type , Type * return_type_reference );

/*busqueda de un simbolo (desde scope actual hacia afuera), retorna NULL si no existe */
Symbol *sem_lookup(const char *name);

/*Inferencia de tipos, comprobación de expresiones y statements */
SType sem_infer_expr(ASTNode *expr);
void sem_check_statement(ASTNode *stmt);
void sem_check_sequence(ASTNode *seq);

/*chequear todo el programa desde la raíz AST*/
int sem_check_program(ASTNode *root);

/*Para debug*/
const char *stype_to_string(SType t);

#endif
