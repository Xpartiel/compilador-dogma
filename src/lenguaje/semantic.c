#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------- Tabla de simbolos por scopes (ambitos) ----------*/
#define MAX_SCOPE_DEPTH 128
#define INITIAL_SCOPE_CAP 32


/*Struct para los Scope.
    Contiene los siguientes atributos: 
        -Symbols* symbols: una coleccion de simbolos
        -int count: contador de simbolos actuales en el scope.
        -int cap:   cantidad maxima de simbolos que puede guardar el scope (dinamico).   
*/
typedef struct {
    Symbol *symbols;
    int count;
    int cap;
} Scope;

/*Utilizaremos las siguientes variables para realizar el analisis semantico*/
/*Arreglo estatico de Scopes global*/
static Scope scopes[MAX_SCOPE_DEPTH];
/*tope de la pila de scopes*/
static int scope_top = -1;
/*Contador de errores semanticos*/
static int sem_errors = 0;
/*Contador de advertencias de error semantico*/
static int sem_warnings = 0;


/*Inicializador de un scope*/
static void scope_init(Scope *s){
    s->cap = INITIAL_SCOPE_CAP;
    s->count = 0;
    s->symbols = malloc(sizeof(Symbol) * s->cap);
}

/*Destructor de un scope*/
static void scope_destroy(Scope *s){
    for(int i=0;i<s->count;i++){
        free(s->symbols[i].name);
    }
    free(s->symbols);
    s->symbols = NULL;
    s->count = s->cap = 0;
}

/*Inicializador del analisis semantico.*/
void sem_init(void){
    scope_top = -1;
    sem_errors = 0;
    sem_warnings = 0;
    sem_enter_scope();
}

/*Destructor del analizador semantico
Basicamente, destruye todos los scope uno a uno.
*/
void sem_destroy(void){
    while(scope_top >= 0) sem_exit_scope();
}

/*
    sem_enter_scope:
        -   no recibe nada como parametro.
    postcondiciones:
        Realiza la entrada de un scope a la pila global de scopes, 
        verifica si el limite de scopes ha sido alcanzado, si eso sucede
        regresa un error, sino entonces aumenta el tope de la pila 
        y aumenta el tamaño de dicha pila
*/
void sem_enter_scope(void){
    if(scope_top + 1 >= MAX_SCOPE_DEPTH){
        fprintf(stderr, "FATAL: demasiados scopes\n");
        exit(1);
    }
    scope_top++;
    scope_init(&scopes[scope_top]);
}

/*Destruye el ultimo scope añadido a la pila*/
void sem_exit_scope(void){
    if(scope_top < 0) return;
    scope_destroy(&scopes[scope_top]);
    scope_top--;
}

/* add symbol to current scope; returns 0 on success, -1 if conflict */
int sem_add_symbol(const char *name, SType type){
    
    /*Si el tope de la pila es menor a cero entonces 
    ya no hay pila, regresa un -1 simbolizando un conflicto*/
    if(scope_top < 0) return -1;
    
    /*Obtenemos el apuntador al scope que esta en el tope de la pila*/
    Scope *sc = &scopes[scope_top];

    /*Revisamos si existe un simbolo con el mismo nombre dentro del scope*/
    for(int i=0;i<sc->count;i++){
        if(strcmp(sc->symbols[i].name, name) == 0){
            return -1; /*Hemos encontrado un simbolo duplicado*/
        }
    }

    /*Si las variables actuales en el scope ya han alcanzado el limite de dicho scope*/
    if(sc->count == sc->cap){
        sc->cap *= 2;
        sc->symbols = realloc(sc->symbols, sizeof(Symbol)*sc->cap); /*Aumentamos el tamaño del scope para poder guardar nuevos simbolos*/
    }

    /*Creamos el nuevo simbolo y lo agregamos al scope actual*/
    sc->symbols[sc->count].name = strdup(name);
    sc->symbols[sc->count].type = type;
    sc->symbols[sc->count].declared_scope_level = scope_top;
    sc->count++;
    return 0;
}


/* sem_lookup.
    
    Recibe un char* name como parametro.
    
    Como su propio nombre sugiere, busca dentro de los scopes si hay algun simbolo que 
    coincida con el nombre dado como parametro, para dicha busqueda comienza desde 
    el scope donde se encuentra y luego hacia afuera.

    Regresa el simbolo si es que lo encuentra, sino regresa null.
*/
Symbol *sem_lookup(const char *name){
    for(int s=scope_top; s>=0; s--){
        Scope *sc = &scopes[s];
        for(int i=0;i<sc->count;i++){
            if(strcmp(sc->symbols[i].name, name) == 0){
                return &sc->symbols[i];
            }
        }
    }
    return NULL;
}

/* toString del tipo semantico*/
const char *stype_to_string(SType t){
    switch(t){
        case S_TYPE_DOUBLE: return "double";
        case S_TYPE_STRING: return "string";
        case S_TYPE_BOOL: return "bool";
        case S_TYPE_VOID: return "void";
        default: return "error";
    }
}

/*Aumenta el contador de errores semanticos y muestra en consola los errores semanticos cometidos*/
static void sem_error(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "SEMANTIC ERROR: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    sem_errors++;
}


/*Aumenta el contador de advertencias semanticas y muestra en consola dichas semanticos cometidos*/
static void sem_warning(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "SEMANTIC WARNING: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    sem_warnings++;
}

/*Funcion para la inferencia de tipos.
    Recibe como parametro un ASTNode *expr con la expresion a inferir.
    Regresa un tipo semantico segun sea el caso al que entre.
*/
SType sem_infer_expr(ASTNode *expr){
    if(!expr) return S_TYPE_ERROR;
    switch(expr->kind){
        case AST_NUMBER:
            return S_TYPE_DOUBLE;
        case AST_STRING:
            return S_TYPE_STRING;
        case AST_BOOLEAN:
            return S_TYPE_BOOL;
        case AST_IDENTIFIER: {
            Symbol *sym = sem_lookup(expr->id);
            if(!sym){
                sem_error("Uso de variable no declarada '%s'", expr->id);
                /* para seguir analizando retornamos double como fallback */
                return S_TYPE_DOUBLE;
            }
            return sym->type;
        }
        case AST_UNOP: {
            SType t = sem_infer_expr(expr->unop.expr);
            if(strcmp(expr->unop.op, "-")==0){
                if(t != S_TYPE_DOUBLE){
                    sem_error("Operador unario '-' requiere tipo numerico, se obtuvo %s", stype_to_string(t));
                    return S_TYPE_ERROR;
                }
                return S_TYPE_DOUBLE;
            } else if(strcmp(expr->unop.op, "!")==0){
                if(t != S_TYPE_BOOL){
                    sem_error("Operador '!' requiere bool, se obtuvo %s", stype_to_string(t));
                    return S_TYPE_ERROR;
                }
                return S_TYPE_BOOL;
            } else {
                sem_error("Operador unario desconocido '%s'", expr->unop.op);
                return S_TYPE_ERROR;
            }
        }
        case AST_BINOP: {
            const char *op = expr->binop.op;
            SType L = sem_infer_expr(expr->binop.left);
            SType R = sem_infer_expr(expr->binop.right);

            if(strcmp(op, "+")==0 || strcmp(op, "-")==0 || strcmp(op, "*")==0 || strcmp(op, "/")==0){
                if(L != S_TYPE_DOUBLE || R != S_TYPE_DOUBLE){
                    sem_error("Operador aritmetico '%s' requiere operandos numericos (izq=%s, der=%s)", op, stype_to_string(L), stype_to_string(R));
                    return S_TYPE_ERROR;
                }
                return S_TYPE_DOUBLE;
            }

            /* comparaciones numericas -> bool */
            if(strcmp(op, "<")==0 || strcmp(op, ">")==0 || strcmp(op, "<=")==0 || strcmp(op, ">=")==0 || strcmp(op, "=")==0){
                if(L != S_TYPE_DOUBLE || R != S_TYPE_DOUBLE){
                    sem_error("Operador comparacion '%s' requiere operandos numericos (izq=%s, der=%s)", op, stype_to_string(L), stype_to_string(R));
                    return S_TYPE_ERROR;
                }
                return S_TYPE_BOOL;
            }

            /* logicos */
            if(strcmp(op, "&&")==0 || strcmp(op, "||")==0){
                if(L != S_TYPE_BOOL || R != S_TYPE_BOOL){
                    sem_error("Operador logico '%s' requiere operandos bool (izq=%s, der=%s)", op, stype_to_string(L), stype_to_string(R));
                    return S_TYPE_ERROR;
                }
                return S_TYPE_BOOL;
            }

            sem_error("Operador binario desconocido '%s'", op);
            return S_TYPE_ERROR;
        }
        case AST_ASSIGN: {
            /* inferir RHS y tratar LHS */
            SType rhs = sem_infer_expr(expr->assign.value);
            Symbol *sym = sem_lookup(expr->assign.name);
            if(!sym){
                /* no declarado: lo introducimos en el scope actual con el tipo inferido */
                if(sem_add_symbol(expr->assign.name, rhs) == 0){
                    sem_warning("Variable '%s' no declarada; se declara implícitamente como %s", expr->assign.name, stype_to_string(rhs));
                } else {
                    sem_error("No se pudo declarar '%s' en el scope actual", expr->assign.name);
                }
                return rhs;
            } else {
                /* comparar tipos */
                if(sym->type != rhs){
                    sem_error("Asignacion incompatible: variable '%s' es %s pero se asigna %s", expr->assign.name, stype_to_string(sym->type), stype_to_string(rhs));
                    return S_TYPE_ERROR;
                }
                return sym->type;
            }
        }
        case AST_IF: {
            SType condt = sem_infer_expr(expr->conditional.condition);
            if(condt != S_TYPE_BOOL){
                sem_error("Condicion 'if' debe ser bool (se obtuvo %s)", stype_to_string(condt));
            }
            /* verificar ramas en scopes separados */
            sem_enter_scope();
            sem_check_statement(expr->conditional.if_branch);
            sem_exit_scope();

            ASTNode *eif = expr->conditional.elif_list;
            while(eif){
                sem_enter_scope();
                sem_check_statement(eif->chained_conditional.branch);
                sem_exit_scope();
                eif = eif->chained_conditional.next;
            }

            if(expr->conditional.else_branch){
                sem_enter_scope();
                sem_check_statement(expr->conditional.else_branch);
                sem_exit_scope();
            }
            return S_TYPE_VOID;
        }
        case AST_LOOP: {
            /* body may be sequence */
            sem_enter_scope();
            sem_check_statement(expr->loop_type.body);
            sem_exit_scope();
            return S_TYPE_VOID;
        }
        case AST_SEQUENCE: {
            sem_check_sequence(expr);
            return S_TYPE_VOID;
        }
        default:
            sem_error("Nodo AST no soportado por el analizador semantico (kind=%d)", expr->kind);
            return S_TYPE_ERROR;
    }
}

/* Funcion para que el analizador semantico pueda revisar declaraciones*/
void sem_check_statement(ASTNode *stmt){
    if(!stmt) return; /*Si esta vacio simplemente return*/
    /*Segun el tipo del stmt, vamos a realizar el juicio de tipo*/
    switch(stmt->kind){
        
        case AST_SEQUENCE:
            sem_check_sequence(stmt);
            break;
        case AST_ASSIGN:
        case AST_BINOP:
        case AST_UNOP:
        case AST_NUMBER:
        case AST_STRING:
        case AST_BOOLEAN:
        case AST_IDENTIFIER:
        case AST_IF:
        case AST_LOOP:
            sem_infer_expr(stmt);
            break;
        default:
            sem_error("Sentencia no soportada en el checado semantico (kind=%d)", stmt->kind);
    }
}


/*Funcion auxiliar para realizar un checado de tipo a una secuencia de instrucciones*/
void sem_check_sequence(ASTNode *seq){
    if(!seq) return;
    for(int i=0;i<seq->sequence.count;i++){
        sem_check_statement(seq->sequence.list[i]);
    }
}

/*A partir de la raiz del AST vamos a realizar el analisis semantico*/
int sem_check_program(ASTNode *root){
    sem_init();
    if(!root){ /*Si el programa es vacio*/
        sem_error("Programa vacio");
        sem_destroy();
        return 1;
    }
    sem_check_statement(root);
    /*Cuando el analizador termine, tendremos un conteo de los errores y advertencias (Si es que hubo)*/
    if(sem_errors > 0){
        fprintf(stderr, "Se encontraron %d errores semanticos y %d advertencias.\n", sem_errors, sem_warnings);
    } else if(sem_warnings > 0){
        fprintf(stderr, "Analisis semantico completado con %d advertencias.\n", sem_warnings);
    } else {
        fprintf(stdout, "Analisis semantico completado sin errores.\n");
    }
    sem_destroy();
    return (sem_errors == 0) ? 0 : 1;
}
