#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "semantic.h"
#include "type.h"

int sem_errors = 0;
int sem_warnings = 0;

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
//static int sem_errors = 0;


/*Contador de advertencias de error semantico*/
//static int sem_warnings = 0;

/* contexto de función actual (para checar returns) */
static int in_function = 0;
static SType current_function_return_type = S_TYPE_VOID;
static Type *current_function_return_type_reference = NULL;
/* contador de profundidad de loops (para validar break/continue) */
static int loop_depth = 0;


/*Inicializador de un scope*/
static void scope_init(Scope *s){
    s->cap = INITIAL_SCOPE_CAP;
    s->count = 0;
    s->symbols = malloc(sizeof(Symbol) * s->cap);

    if(!s->symbols){
        fprintf(stderr, "FATAL: malloc fallo en scope_init\n");
        exit(1);
    }
}

/*Destructor de un scope*/
static void scope_destroy(Scope *s){
    for(int i=0;i<s->count;i++){
        free(s->symbols[i].name);
        if(s->symbols[i].is_function){
            /* si params pointer fue asignado dinámicamente al registrar función,
               NO lo liberamos aquí a menos que se haya duplicado explícitamente */
        }
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
    in_function = 0;
    current_function_return_type = S_TYPE_VOID;
    current_function_return_type_reference = NULL;
    loop_depth = 0;
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

/* Afrefa un symbolo al scope actual, regresa 0 si lo logra, en cualquier otro caso regresa -1*/
int sem_add_symbol(const char *name, SType type , Type *type_reference){
    
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
        if( !sc->symbols ){
            fprintf(stderr, "FATAL: realloc fallo en sem_add_symbol\n");
            exit(1);
        }
    }

    /*Creamos el nuevo simbolo y lo agregamos al scope actual*/
    sc->symbols[sc->count].name = strdup(name);
    sc->symbols[sc->count].type = type;
    sc->symbols[sc->count].declared_scope_level = scope_top;
    sc->symbols[sc->count].is_function = 0;
    sc->symbols[sc->count].params = NULL;
    sc->symbols[sc->count].param_count = 0;
    sc->symbols[sc->count].return_type = S_TYPE_VOID;
    sc->symbols[sc->count].type_reference = type_reference;
    sc->count++;
    return 0;
}

/* sem_add_function: añade una función (nombre, params array, param_count, return_type, optional Type* ref)
   devuelve 0 en éxito, -1 en conflicto */
int sem_add_function(const char *name, ASTNode **params, int param_count, SType return_type, Type *return_type_reference){
    if(scope_top < 0) return -1;
    Scope *sc = &scopes[scope_top];

    for(int i=0;i<sc->count;i++){
        if(strcmp(sc->symbols[i].name, name) == 0){
            return -1; /* ya existe */
        }
    }

    if(sc->count == sc->cap){
        sc->cap *= 2;
        sc->symbols = realloc(sc->symbols, sizeof(Symbol)*sc->cap);
        if(!sc->symbols){
            fprintf(stderr, "FATAL: realloc fallo en sem_add_function\n");
            exit(1);
        }
    }

    sc->symbols[sc->count].name = strdup(name);
    sc->symbols[sc->count].type = S_TYPE_VOID; /* variable-type unused for functions */
    sc->symbols[sc->count].declared_scope_level = scope_top;
    sc->symbols[sc->count].is_function = 1;
    sc->symbols[sc->count].params = params;
    sc->symbols[sc->count].param_count = param_count;
    sc->symbols[sc->count].return_type = return_type;
    sc->symbols[sc->count].type_reference = return_type_reference;
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
        case S_TYPE_INT: return "int";
        case S_TYPE_FLOAT: return "float";
        case S_TYPE_STRING: return "string";
        case S_TYPE_BOOL: return "bool";
        case S_TYPE_VOID: return "void";
        case S_TYPE_ARRAY: return "array";
        case S_TYPE_LIST: return "list";
        case S_TYPE_ERROR: return "error";
        default: return "error: unknown stype";
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

/* Convierte Type* (AST) a SType.
   - T_integer -> S_TYPE_INT
   - T_float -> S_TYPE_FLOAT
   - T_boolean -> S_TYPE_BOOL
   - T_string -> S_TYPE_STRING
   - T_array -> S_TYPE_ARRAY
   - T_list -> S_TYPE_LIST
*/
static SType type_to_stype_full(Type *t){
    if(!t) return S_TYPE_ERROR;
    switch(t->base){
        case T_integer: return S_TYPE_INT;
        case T_float:   return S_TYPE_FLOAT;
        case T_boolean: return S_TYPE_BOOL;
        case T_string:  return S_TYPE_STRING;
        case T_array:   return S_TYPE_ARRAY;
        case T_list:    return S_TYPE_LIST;
        default: return S_TYPE_ERROR;
    }
}

/* Compara types estructurales (Type*), incluyendo subtipos para arrays/lists */
static int type_equals(Type *a, Type *b){
    if(a == NULL || b == NULL) return 0;
    if(a->base != b->base) return 0;
    if(a->base == T_array || a->base == T_list){
        if(a->inner == NULL || b->inner == NULL) return 0;
        return type_equals(a->inner, b->inner);
    }
    return 1;
}

/* Devuelve el SType base de un Type* compuesto (si corresponde) */
static SType type_base_stype(Type *t){
    if(!t) return S_TYPE_ERROR;
    if(t->base == T_array || t->base == T_list){
        return type_to_stype_full(t->inner);
    } else {
        return type_to_stype_full(t);
    }
}

/*Funcion para reconocer funciones nativas de la FIS-25*/
static int is_native(const char *name, const char *native){
    return strcmp(name, native) == 0;
}

/* Declaraciones adelantadas para facilitar uso */
SType sem_infer_expr(ASTNode *expr);
void sem_check_statement(ASTNode *stmt);
void sem_check_sequence(ASTNode *seq);

/*Funcion para la inferencia de tipos.
    Recibe como parametro un ASTNode *expr con la expresion a inferir.
    Regresa un tipo semantico segun sea el caso al que entre.
*/
SType sem_infer_expr(ASTNode *expr){
    if(!expr) return S_TYPE_ERROR;
    switch(expr->kind){
        case AST_INTEGER:
            return S_TYPE_INT;
        case AST_FLOAT:
            return S_TYPE_FLOAT;
        case AST_STRING:
            return S_TYPE_STRING;
        case AST_BOOLEAN:
            return S_TYPE_BOOL;
        case AST_IDENTIFIER: {
            Symbol *sym = sem_lookup(expr->id);
            if(!sym){
                sem_error("Uso de variable no declarada '%s'", expr->id);
                /* para seguir analizando retornamos float como fallback */
                return S_TYPE_ERROR;
            }
            if( sym->is_function ){
                sem_error("Nombre de función '%s' usado sin llamada", expr->id);
                return S_TYPE_ERROR;
            }
            return sym->type;
        }
        case AST_UNOP: {
            SType t = sem_infer_expr(expr->unop.expr);
            if(strcmp(expr->unop.op, "-")==0){
                if( t == S_TYPE_FLOAT ){
                    return S_TYPE_FLOAT;
                }
                if( t == S_TYPE_INT ){
                    return S_TYPE_INT;
                }
                sem_error("Operador unario '-' requiere tipo numerico, se obtuvo %s", stype_to_string(t));
                return S_TYPE_ERROR;
            } else if(strcmp(expr->unop.op, "!") == 0 ){
                if( t != S_TYPE_BOOL ){
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

            if( strcmp(op, "+")==0 ||
                strcmp(op, "-")==0 ||
                strcmp(op, "*")==0 ||
                strcmp(op, "/")==0) {
                
                if( ( L == S_TYPE_FLOAT ) && ( R == S_TYPE_FLOAT ) ){
                    return S_TYPE_FLOAT;
                }
                if( (L == S_TYPE_INT ) && ( R == S_TYPE_INT) ){
                    return S_TYPE_INT;
                }
                sem_error("Operador aritmetico '%s' requiere operandos numericos (izq=%s, der=%s)", op, stype_to_string(L), stype_to_string(R));
                return S_TYPE_ERROR;
            }

            /* operaciones relacionales */
            if( strcmp(op, "<")==0 ||
                strcmp(op, ">")==0 ||
                strcmp(op, "<=")==0 ||
                strcmp(op, ">=")==0 ||
                strcmp(op, "=")==0) {

                if( (L==S_TYPE_INT || L==S_TYPE_FLOAT) && ( R==S_TYPE_INT || R==S_TYPE_FLOAT) ){
                    return S_TYPE_BOOL;
                }
                sem_error("Operador comparacion '%s' requiere operandos numericos, encontrados (izq=%s, der=%s)", op, stype_to_string(L), stype_to_string(R));
                return S_TYPE_ERROR;
            }

            /* logicos */
            if(strcmp(op, "&&")==0 || strcmp(op, "||")==0){
                if( L != S_TYPE_BOOL || R != S_TYPE_BOOL ) {
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
            if( !sym ){
                /* no declarado: lo introducimos en el scope actual con el tipo inferido */
                if(sem_add_symbol(expr->assign.name, rhs,NULL) == 0){
                    sem_warning("Variable '%s' sin declarar; se declara implícitamente como %s", expr->assign.name, stype_to_string(rhs));
                } else {
                    sem_error("No pudo declararse '%s' en el scope actual", expr->assign.name);
                }
                return rhs;
            } else {
                if( sym->is_function ){
                    sem_error("Intento de asignar a nombre de funcion '%s'", expr->assign.name);
                    return S_TYPE_ERROR;
                }else{
                    if( sym->type == rhs ){
                        return sym->type;
                    }
                    sem_error("Asignacion incompatible: variable '%s' es %s pero se asigna %s", expr->assign.name, stype_to_string(sym->type), stype_to_string(rhs));
                    return S_TYPE_ERROR;
                }
            }
        }
        case AST_IF: {
            SType condt = sem_infer_expr(expr->conditional.condition);
            if( condt != S_TYPE_BOOL ){
                sem_error("Condicional 'IF' debe ser bool (se obtuvo %s)", stype_to_string(condt));
            }
            /* verificar ramas en scopes separados */
            sem_enter_scope();
            sem_check_statement(expr->conditional.if_branch);
            sem_exit_scope();

            ASTNode *eif = expr->conditional.elif_list;
            while(eif){
                SType cond = sem_infer_expr(eif->chained_conditional.condition);
                if(cond != S_TYPE_BOOL){
                    sem_error("Condicion 'else if' debe ser bool (se obtuvo %s)", stype_to_string(cond));
                }
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
            loop_depth++;
            sem_enter_scope();
            sem_check_statement(expr->loop_type.body);
            sem_exit_scope();
            loop_depth--;
            return S_TYPE_VOID;
        }
        case AST_SEQUENCE: {
            sem_check_sequence(expr);
            return S_TYPE_VOID;
        }
        case AST_INIT_LIST: {
            if(expr->initializator.count == 0){
                sem_warning("Inicializador de estructura vacio");
                return S_TYPE_ERROR;
            }
            SType base = sem_infer_expr(expr->initializator.elements[0]);
            for(int i=1;i<expr->initializator.count;i++){
                SType t = sem_infer_expr(expr->initializator.elements[i]);
                if(t != base){
                    sem_error("Elementos del inicializador tienen tipos distintos (%s vs %s)", stype_to_string(base), stype_to_string(t));
                    return S_TYPE_ERROR;
                }
            }
            return base;
        }
        case AST_ARRAY: {
            if(expr->array.size_expr){
                SType s = sem_infer_expr(expr->array.size_expr);
                if( s != S_TYPE_INT ){
                    sem_error("Declarar arreglo con espacio requiere INTEGER, %s encontrado", stype_to_string(s));
                }
            }
            /* Representamos arrays como S_TYPE_ARRAY in contexts where type is known; here return ERROR */
            return S_TYPE_ERROR;
        }
        case AST_LIST: {
            if(expr->list.size_expr){
                SType s = sem_infer_expr(expr->list.size_expr);
                if( s != S_TYPE_INT ){
                    sem_error("Size de lista debe ser INTEGER, %s encontrado", stype_to_string(s));
                }
            }
            return S_TYPE_ERROR;
        }
        case AST_FUNCTION_CALL: {
            /*Primero debemos revisar que la funcion llamada no sea alguna nativa de la FIS */
            const char *fname = expr->function_call.name;
            int argc = expr->function_call.arg_count;

            /* PRINT(expr) para est funcion solo se imprime algo (se encarga la FIS), y luego aqui se devuelve VOID */
            if (is_native(fname, "PRINT")) {
                if (argc != 1)
                    sem_error("PRINT espera 1 argumento, recibió %d", argc);
                else {
                    /*Aqui validamos que el argumento tenga un tipo válido (auqnue acepte cualquier tipo, es
                    necesario hacer la validación)*/
                    sem_infer_expr(expr->function_call.args[0]); 
                }
                return S_TYPE_VOID;
            }

            /* INPUT(dest) lee entrada y la guarda en la variable dest*/
            if (is_native(fname, "INPUT")) {
                if (argc != 1) {
                    sem_error("INPUT espera un argumento");
                    return S_TYPE_ERROR;
                }

                ASTNode *dest = expr->function_call.args[0];
                if (dest->kind != AST_IDENTIFIER) {
                    sem_error("INPUT requiere un identificador como destino");
                    return S_TYPE_ERROR;
                }

                /*Declaracion implicita si no existe*/
                if (!sem_lookup(dest->id)) {
                    sem_add_symbol(dest->id, S_TYPE_STRING, NULL);
                }

                return S_TYPE_STRING; /*INPUT produce un string*/
            }

            /* KEY(code, dest) recibe un codigo numerico y guarda el estado en una variable dest (si esta pulsada o no)*/
            if (is_native(fname, "KEY")) {
                if (argc != 2) {
                    sem_error("KEY espera 2 argumentos: KEY(code, dest)");
                    return S_TYPE_ERROR;
                }

                SType ct = sem_infer_expr(expr->function_call.args[0]);
                if (ct != S_TYPE_INT && ct != S_TYPE_FLOAT)
                    sem_error("KEY: el código de tecla debe ser numérico");

                ASTNode *dest = expr->function_call.args[1];
                if (dest->kind != AST_IDENTIFIER) {
                    sem_error("KEY: el segundo argumento debe ser un identificador destino");
                } else if (!sem_lookup(dest->id)) {
                    sem_add_symbol(dest->id, S_TYPE_INT, NULL);
                }

                return S_TYPE_INT;
            }

            /* PIXEL(x, y, c)  Recibe 3 valores numericos, los primeros 2 para las coordenadnas y el ultimo para saber
            si se enciende el pixel o no, 0 = NO, 1 o mas= si*/
            if (is_native(fname, "PIXEL")) {
                if (argc != 3) {
                    sem_error("PIXEL espera 3 argumentos: PIXEL(x, y, color)");
                    return S_TYPE_ERROR;
                }

                SType tx = sem_infer_expr(expr->function_call.args[0]);
                SType ty = sem_infer_expr(expr->function_call.args[1]);
                SType tc = sem_infer_expr(expr->function_call.args[2]);

                if (!(tx == S_TYPE_INT || tx == S_TYPE_FLOAT))
                    sem_error("PIXEL: x debe ser numérico");
                if (!(ty == S_TYPE_INT || ty == S_TYPE_FLOAT))
                    sem_error("PIXEL: y debe ser numérico");
                if (!(tc == S_TYPE_INT || tc == S_TYPE_FLOAT))
                    sem_error("PIXEL: color debe ser numérico");

                return S_TYPE_VOID;
            }

            /*Si no es una funcion nativa de la FIS entonces buscamos su declaracion*/
            Symbol *f = sem_lookup(expr->function_call.name);
            if( !f || !f->is_function ){
                sem_error("Funcion '%s' no declarada", expr->function_call.name);
                return S_TYPE_ERROR;
            }
            if(f->param_count != expr->function_call.arg_count){
                sem_error("Numero de argumentos incorrecto en llamada a '%s' (espera %d, recibe %d)", expr->function_call.name, f->param_count, expr->function_call.arg_count);
            }
            int n = (f->param_count < expr->function_call.arg_count) ? f->param_count : expr->function_call.arg_count;
            for(int i=0;i<n;i++){
                ASTNode *param_decl = f->params[i];
                Type *param_type_reference = param_decl->declaration.type; /* Type* */
                SType expected = type_to_stype_full(param_type_reference);
                SType actual = sem_infer_expr(expr->function_call.args[i]);

                if(expected == S_TYPE_ARRAY || expected == S_TYPE_LIST) {
                    ASTNode *arg = expr->function_call.args[i];
                    if(arg->kind == AST_INIT_LIST){
                        SType elem_t = sem_infer_expr(arg);
                        SType base = type_base_stype(param_type_reference);
                        if(elem_t != base){
                            sem_error("Tipo de elemento en argumento %d de '%s' incompatible (esperado %s, obtenido %s)", i+1, expr->function_call.name, stype_to_string(base), stype_to_string(elem_t));
                        }
                    } else {
                        sem_warning("No se pudo verificar completamente argumento %d de '%s' (tipos compuestos)", i+1, expr->function_call.name);
                    }
                } else if(actual != expected){
                    sem_error("Tipo de argumento %d en llamada a '%s' incompatible (esperado %s, obtenido %s)", i+1, expr->function_call.name, stype_to_string(expected), stype_to_string(actual));
                }
            }
            return f->return_type;
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
        case AST_DECLARATION: {
            char *name = stmt->declaration.name;
            
            Type *t = stmt->declaration.type;
            SType st = type_to_stype_full(t);
            Type *type_reference = NULL;
            if(st == S_TYPE_ARRAY || st == S_TYPE_LIST){
                type_reference = t;
            }

            if( sem_add_symbol(name, st, type_reference) != 0 ){
                sem_error("Variable '%s' ya declarada en este scope", name);
            }

            /* inicializador */
            ASTNode *init = stmt->declaration.init;
            if(init){
                if(init->kind == AST_INIT_LIST){
                    SType elems = sem_infer_expr(init);
                    if(st == S_TYPE_ARRAY || st == S_TYPE_LIST){
                        SType base = type_base_stype(t);
                        if(elems != base){
                            sem_error("Inicializador de '%s' tiene elementos de tipo %s, pero la variable declara elementos %s",
                                      name, stype_to_string(elems), stype_to_string(base));
                        }
                    } else {
                        sem_error("Inicializacion por lista en variable no-compuesta '%s'", name);
                    }
                } else {
                    SType it = sem_infer_expr(init);
                    if(st == S_TYPE_ARRAY || st == S_TYPE_LIST){
                        sem_error("Inicializador de variable compuesta '%s' debe ser lista o constructo apropiado", name);
                    } else {
                        /* handle numeric promotions */
                        if( st == S_TYPE_FLOAT ){
                            if( !( it==S_TYPE_INT || it==S_TYPE_FLOAT )){
                                sem_error("Inicializacion de '%s' incompatible (esperado %s, obtenido %s)", name, stype_to_string(st), stype_to_string(it));
                            }
                        }else {
                            if(it != st){
                                sem_error("Inicializacion de '%s' incompatible (esperado %s, obtenido %s)", name, stype_to_string(st), stype_to_string(it));
                            }
                        }
                    }
                }
            }
            break;
        }
        case AST_ASSIGN:
        case AST_BINOP:
        case AST_UNOP:
        case AST_INTEGER:
        case AST_FLOAT:
        case AST_STRING:
        case AST_BOOLEAN:
        case AST_IDENTIFIER:
            sem_infer_expr(stmt);
            break;
        case AST_IF:
            sem_infer_expr(stmt);
            break;
        case AST_LOOP:
            sem_infer_expr(stmt);
            break;
        case AST_FUNCTION_DECL: {
            char *fname = stmt->function_declaration.name;
            ASTNode **params = stmt->function_declaration.params;
            int pcount = stmt->function_declaration.param_count;
            Type *rett = stmt->function_declaration.return_type;

            SType ret_st = S_TYPE_VOID;
            Type *ret_ref = NULL;
            if(rett){
                ret_st = type_to_stype_full(rett);
                if(ret_st == S_TYPE_ARRAY || ret_st == S_TYPE_LIST) ret_ref = rett;
            }

            if(sem_add_function(fname, params, pcount, ret_st, ret_ref) != 0){
                sem_error("Funcion '%s' ya declarada en este scope", fname);
            }

            /* check body in new scope with params inserted */
            sem_enter_scope();
            for(int i=0;i<pcount;i++){
                ASTNode *pnode = params[i];
                char *pname = pnode->declaration.name;
                Type *pt = pnode->declaration.type;
                SType pst = type_to_stype_full(pt);
                Type *pref = (pst==S_TYPE_ARRAY || pst==S_TYPE_LIST) ? pt : NULL;
                if(sem_add_symbol(pname, pst, pref) != 0){
                    sem_error("Parametro '%s' duplicado en funcion '%s'", pname, fname);
                }
            }

            int prev_in_function = in_function;
            SType prev_return = current_function_return_type;
            Type *prev_return_ref = current_function_return_type_reference;

            in_function = 1;
            current_function_return_type = ret_st;
            current_function_return_type_reference = ret_ref;

            sem_check_statement(stmt->function_declaration.body);

            in_function = prev_in_function;
            current_function_return_type = prev_return;
            current_function_return_type_reference = prev_return_ref;

            sem_exit_scope();
            break;
        }
        case AST_FUNCTION_CALL:
            sem_infer_expr(stmt);
            break;
        case AST_RETURN: {
            if(!in_function){
                sem_error("Sentencia 'return' fuera de funcion");
            } else {
                ASTNode *val = stmt->result.value;
                if(val == NULL){
                    if(current_function_return_type != S_TYPE_VOID){
                        sem_error("Return sin expresion en funcion que espera %s", stype_to_string(current_function_return_type));
                    }
                } else {
                    SType rt = sem_infer_expr(val);
                    if(current_function_return_type == S_TYPE_ARRAY || current_function_return_type == S_TYPE_LIST){
                        /* esperamos que val sea init-list o compatible; si es init-list comparamos elemento base */
                        if(val->kind == AST_INIT_LIST){
                            SType elem_t = sem_infer_expr(val);
                            SType expected_base = (current_function_return_type_reference) ? type_base_stype(current_function_return_type_reference) : S_TYPE_ERROR;
                            if(elem_t != expected_base){
                                sem_error("Tipo de return incompatible (elementos): esperado %s, obtenido %s", stype_to_string(expected_base), stype_to_string(elem_t));
                            }
                        } else {
                            sem_warning("No se pudo verificar completamente return con tipo compuesto");
                        }
                    } else {
                        if( current_function_return_type == S_TYPE_FLOAT ){
                            if( !(rt==S_TYPE_INT || rt==S_TYPE_FLOAT ) ){
                                sem_error("Tipo de return incompatible: se esperaba %s, se obtuvo %s", stype_to_string(current_function_return_type), stype_to_string(rt));
                            }
                        } else {
                            if(rt != current_function_return_type){
                                sem_error("Tipo de return incompatible: se esperaba %s, se obtuvo %s", stype_to_string(current_function_return_type), stype_to_string(rt));
                            }
                        }
                    }
                }
            }
            break;
        }
        case AST_BREAK: {
            if(loop_depth == 0){
                sem_error("Sentencia 'break' fuera de loop");
            }
            break;
        }
        case AST_CONTINUE: {
            if(loop_depth == 0){
                sem_error("Sentencia 'continue' fuera de loop");
            }
            break;
        }
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