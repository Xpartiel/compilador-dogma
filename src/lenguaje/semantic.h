#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/*Tipos semanticos*/
/*No se si vamos a agregar más*/
typedef enum {
    S_TYPE_ERROR = -1,
    S_TYPE_DOUBLE,
    S_TYPE_STRING,
    S_TYPE_BOOL,
    S_TYPE_VOID
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
int sem_add_symbol(const char *name, SType type);

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
