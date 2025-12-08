#include "codegen.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* codegen.c
 *
 * Implementación del generador de código intermedio para FIS-25.
 *
 * Requiere:
 *  - ast.h que define ASTNode y sus campos (kind, binop, unop, assign, sequence, conditional, ...)
 *  - stdarg.h para la función emit()
 *
 * Notas:
 *  - Las funciones que devuelven char* devuelven cadenas con malloc; el llamador
 *    debe liberarlas (free) cuando ya no las necesite.
 *  - Se imprime "VAR <name>" la primera vez que aparece una variable/temporal.
 *  - Este módulo asume que FIS-25 usa instrucciones del estilo:
 *      ADD a b T1
 *      SUB a b T2
 *      MUL a b T3
 *      DIV a b T4
 *      LT a b Tn    (dest = (a < b) ? 1 : 0)
 *      GT, LTE, GTE, EQ, NEQ
 *      ASSIGN src dest
 *      IFFALSE cond GOTO LABEL
 *      GOTO LABEL
 *      LABEL Lx
 *
 *  - Traducción de booleanos:
 *      && -> MUL (porque usamos 0/1)
 *      || -> ADD + GT 0   (sencillo, no short-circuit)
 */


/* ---- configuración ---- */
#define TEMP_NAME_LEN 32
#define LABEL_NAME_LEN 32

/* ---- estado interno ---- */
static FILE *out_f = NULL;
static int temp_counter = 0;
static int label_counter = 0;

/* tabla simple de strings ya declarados (VAR) para evitar redeclarar */
typedef struct {
    char **items;
    int count;
    int cap;
} StrVec;

static StrVec declared;

typedef struct LoopContext {
    char *label_break;
    char *label_continue;
    struct LoopContext *next;
} LoopContext;

static LoopContext *loop_stack = NULL;

/* ----- utilidades para StrVec ----- */
static void svec_init(StrVec *v){
    v->cap = 64;
    v->count = 0;
    v->items = malloc(sizeof(char*) * v->cap);
    if(!v->items){ perror("malloc"); exit(1); }
}
static void svec_destroy(StrVec *v){
    if(!v->items) return;
    for(int i=0;i<v->count;i++) free(v->items[i]);
    free(v->items);
    v->items = NULL;
    v->count = v->cap = 0;
}
static int svec_find(StrVec *v, const char *s){
    for(int i=0;i<v->count;i++) if(strcmp(v->items[i], s) == 0) return i;
    return -1;
}
static void svec_push(StrVec *v, const char *s){
    if(svec_find(v,s) != -1) return;
    if(v->count == v->cap){
        v->cap *= 2;
        v->items = realloc(v->items, sizeof(char*) * v->cap);
        if(!v->items){ perror("realloc"); exit(1); }
    }
    v->items[v->count++] = strdup(s);
}

/* ----- emit: wrapper para escribir líneas al archivo de salida ----- */
/* Uso: emit("ADD %s %s %s", a, b, t);  -> escribe "ADD a b t\n" en out_f */
static void emit(const char *fmt, ...){
    if(!out_f) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(out_f, fmt, ap);
    va_end(ap);
    fprintf(out_f, "\n");
}

/* ----- asegurar que una variable/temporal está declarada (VAR name) ----- */
static void ensure_declared(const char *name){
    if(!name) return;
    if(svec_find(&declared, name) == -1){
        emit("VAR %s", name);
        svec_push(&declared, name);
    }
}

/* ----- temporales y etiquetas ----- */
char *codegen_new_temp(void){
    char buf[TEMP_NAME_LEN];
    temp_counter++;
    snprintf(buf, TEMP_NAME_LEN, "T%d", temp_counter);
    return strdup(buf);
}

char *codegen_new_label(void){
    char buf[LABEL_NAME_LEN];
    label_counter++;
    snprintf(buf, LABEL_NAME_LEN, "L%d", label_counter);
    return strdup(buf);
}

/* ----- loop stack helper procedures ----- */
static void loop_push( const char *label_break , const char *label_continue ){
    LoopContext *c = malloc( sizeof(LoopContext) );
    c->label_break = strdup( label_break );
    c->label_continue = strdup( label_continue );
    c->next = loop_stack;
    loop_stack = c;
}

static void loop_pop(){
    if( !loop_stack ){
        return;
    }
    LoopContext *c = loop_stack;
    loop_stack = c->next;
    free( c->label_break );
    free( c->label_continue );
    free(c);
}

static LoopContext *loop_top(){
    return loop_stack;
}

/* ----- inicialización / finalización pública ----- */
void codegen_init(FILE *out){
    out_f = out;
    temp_counter = 0;
    label_counter = 0;
    svec_init(&declared);

    ensure_declared("RVAL");
    emit("; FIS-25 generated code");
}

void codegen_finish(void){
    /* mensaje final opcional */
    emit("PRINT \"--- program finished ---\"");
    /* liberar recursos */
    svec_destroy(&declared);
    out_f = NULL;
}



/* ----- mapeo de operadores a instrucciones FIS-25 ----- */
static const char* binop_to_instr(const char *op){
    if(strcmp(op,"+")==0) return "ADD";
    if(strcmp(op,"-")==0) return "SUB";
    if(strcmp(op,"*")==0) return "MUL";
    if(strcmp(op,"/")==0) return "DIV";
    if(strcmp(op,"=")==0) return "EQ";  /* igualdad en tu AST se representaba "=" */
    if(strcmp(op,"<")==0) return "LT";
    if(strcmp(op,">")==0) return "GT";
    if(strcmp(op,"<=")==0) return "LTE";
    if(strcmp(op,">=")==0) return "GTE";
    /* && and || se implementan como patrones (no mapeo directo) */
    return NULL;
}

/* ----- GENERACIÓN DE EXPRESIONES -----
 * Devuelve una cadena malloc'd con el nombre que contiene el resultado:
 *  - literales: "5", "\"hola\"", "1"
 *  - identificadores: "x"   (asegura VAR x la primera vez)
 *  - temporales: "Tn" (creados y declarados en el proceso)
 *
 * El llamador es responsable de free() la cadena devuelta cuando corresponda.
 */
char *codegen_gen_expr(ASTNode *expr){
    if(!expr) return strdup("0");

    switch(expr->kind){
        case AST_INTEGER:{
            char temporal_buffer[64];
            char *t = codegen_new_temp();
            ensure_declared(t);

            snprintf( temporal_buffer , sizeof(temporal_buffer) , "%.0d", expr->integer.num );
            emit("ASSIGN %s %s",temporal_buffer , t);
            return t;
        }
        case AST_FLOAT: {
            char buf[64];
            char *t = codegen_new_temp();

            ensure_declared(t);
            snprintf(buf, sizeof(buf), "%g", expr->floating.num);
            
            emit("ASSIGN %s %s", buf, t);
            return t;
        }

        case AST_STRING: {
            /* ponemos comillas para PRINT si se usan */
            size_t n = strlen(expr->str) + 3;
            char *lit = malloc(n);
            snprintf(lit, n, "\"%s\"", expr->str);

            char *t = codegen_new_temp();
            ensure_declared(t);
            emit("ASSIGN %s %s", lit, t);
            free(lit);

            return t;
        }

        case AST_BOOLEAN: {
            char *t = codegen_new_temp();
            ensure_declared(t);
            emit("ASSIGN %s %s", expr->boolean ? "1" : "0", t);
            return t;
        }

        case AST_IDENTIFIER: {
            ensure_declared(expr->id);
            return strdup(expr->id);
        }

        case AST_UNOP: {
            /* Unario: '-' o '!' */
            char *sub = codegen_gen_expr(expr->unop.expr);
            if(strcmp(expr->unop.op, "-")==0){
                char *t = codegen_new_temp();
                ensure_declared(t);

                emit("SUB 0 %s %s", sub, t);   /* t = 0 - sub */
                free(sub);
                return t;
            } else if(strcmp(expr->unop.op, "!")==0){
                char *t = codegen_new_temp();
                ensure_declared(t);
                emit("EQ %s 0 %s", sub, t);    /* t = (sub == 0) ? 1 : 0 */
                free(sub);
                return t;
            } else {
                /* operador desconocido: devolver 0 como fallback */
                char *t = codegen_new_temp();
                ensure_declared(t);
                emit("ASSIGN 0 %s", t);
                free(sub);
                return t;
            }
        }

        case AST_BINOP: {
            const char *op = expr->binop.op;

            /* manejos especiales */
            if(strcmp(op, "&&")==0){
                /* a && b  => MUL a b t  (0/1 semantics) */
                char *L = codegen_gen_expr(expr->binop.left);
                char *R = codegen_gen_expr(expr->binop.right);
                char *t = codegen_new_temp();
                ensure_declared(t);
                emit("MUL %s %s %s", L, R, t);
                free(L); free(R);
                return t;
            }

            if(strcmp(op, "||")==0){
                /* a || b => ADD a b tmp ; GT tmp 0 t */
                char *L = codegen_gen_expr(expr->binop.left);
                char *R = codegen_gen_expr(expr->binop.right);
                char *tmp = codegen_new_temp();
                ensure_declared(tmp);
                emit("ADD %s %s %s", L, R, tmp);
                char *t = codegen_new_temp();
                ensure_declared(t);
                emit("GT %s 0 %s", tmp, t);
                free(L); free(R); free(tmp);
                return t;
            }

            /* mapeo directo para operación binaria aritmética o comparaciones */
            char *L = codegen_gen_expr(expr->binop.left);
            char *R = codegen_gen_expr(expr->binop.right);
            char *t = codegen_new_temp();
            ensure_declared(t);

            const char *instr = binop_to_instr(op);
            if(instr){
                emit("%s %s %s %s", instr, L, R, t);
            } else {
                /* operador desconocido */
                emit("ASSIGN 0 %s", t);
            }
            free(L); free(R);
            return t;
        }

        case AST_ASSIGN: {
            /* generamos RHS y emitimos ASSIGN rhs name */
            char *rhs = codegen_gen_expr(expr->assign.value);
            ensure_declared(expr->assign.name);
            emit("ASSIGN %s %s", rhs, expr->assign.name);
            free(rhs);
            /* convención: assignment devuelve el nombre destino */
            return strdup(expr->assign.name);
        }

        case AST_INIT_LIST:
            /* inicializadores no producen un valor directo; generar errores si se usa en expresion */
            emit("; WARNING: init-list used in expr context");
            return strdup("0");

        case AST_FUNCTION_CALL: {

             const char *fname = expr->function_call.name;

            /*Checamos si es una funcion nativa de la FIS*/
            if (strcmp(fname, "PRINT") == 0) {
                char *a = codegen_gen_expr(expr->function_call.args[0]);
                emit("PRINT %s", a);
                free(a);
                return strdup("0");
            }

            if (strcmp(fname, "INPUT") == 0) {
                char *dest = codegen_gen_expr(expr->function_call.args[0]);
                ensure_declared(dest);
                emit("INPUT %s", dest);
                return dest;
            }

            if (strcmp(fname, "KEY") == 0) {
                char *code = codegen_gen_expr(expr->function_call.args[0]);
                char *dest = codegen_gen_expr(expr->function_call.args[1]);
                ensure_declared(dest);
                emit("KEY %s %s", code, dest);
                free(code);
                return dest;
            }

            if (strcmp(fname, "PIXEL") == 0) {
                char *x = codegen_gen_expr(expr->function_call.args[0]);
                char *y = codegen_gen_expr(expr->function_call.args[1]);
                char *c = codegen_gen_expr(expr->function_call.args[2]);
                emit("PIXEL %s %s %s", x, y, c);
                free(x); free(y); free(c);
                return strdup("0");
            }

            /*Si no es nativa, entonces la tenemos que buscar*/
            /*Iteramos los argumentos de la funcion*/
            for(int i=0;i<expr->function_call.arg_count;i++){
                char *a = codegen_gen_expr(expr->function_call.args[i]);
                /*Escribimos en el archivo*/
                emit("PARAM %s", a);
                free(a);
            }

            /*Escribimos en el archivo*/
            emit("GOSUB %s", expr->function_call.name);
            
            /*Para la variable de retorno*/
            char *t = codegen_new_temp();
            ensure_declared(t);
            /*Escribimos en el archivo*/
            emit("ASSIGN RVAL %s", t); 
            return t;
        }

        

        case AST_ARRAY:
        case AST_LIST:
            /* construcción de arrays/listas no implementada en detalle aquí */
            emit("; TODO: array/list constructor not implemented");
            return strdup("0");

        case AST_ELSEIF: {
            /* por precaución: handled by AST_IF; si llega aquí, generamos branch */
            codegen_gen_statement(expr->chained_conditional.branch);
            return strdup("0");
        }

        

        default: {
            /* nodo no soportado: generar temporal con 0 */
            char *t = codegen_new_temp();
            ensure_declared(t);
            emit("ASSIGN 0 %s", t);
            return t;
        }
    }
}

/* ----- GENERACIÓN DE SENTENCIAS ----- */
bool codegen_gen_statement(ASTNode *stmt){
    if(!stmt) return false;

    switch(stmt->kind){
        
        case AST_SEQUENCE:
            return codegen_gen_sequence(stmt);

        case AST_DECLARATION:
            ensure_declared(stmt->declaration.name);
            if(stmt->declaration.init){
                char *r = codegen_gen_expr(stmt->declaration.init);
                emit("ASSIGN %s %s", r, stmt->declaration.name);
                free(r);
            }
            return false;

        case AST_ASSIGN: {
            /* codegen_gen_expr ya emite ASSIGN para AST_ASSIGN */
            char *r = codegen_gen_expr(stmt);
            free(r);
            return false;
        }

        case AST_IF: {
            /* Generamos: cond -> IFFALSE cond GOTO L_else ; then ; GOTO L_end ; LABEL L_else ; else ; LABEL L_end */
            char *cond = codegen_gen_expr(stmt->conditional.condition);
            char *L_else = codegen_new_label();
            char *L_end  = codegen_new_label();
            /* no usamos ensure_declared en labels para VAR; pero si tu VM requiere VAR para labels, podrías. */
            emit("IFFALSE %s GOTO %s", cond, L_else);
            free(cond);

            /* then branch */
            bool thenReturned=codegen_gen_statement(stmt->conditional.if_branch);
            if(!thenReturned) emit("GOTO %s", L_end);
            emit("LABEL %s", L_else);

            bool allElifsReturn = true;
            /* elif chain: cada nodo chained_conditional contiene condition & branch & next */
            ASTNode *eif = stmt->conditional.elif_list;
            while(eif){
                /* eif: cadena de condicionales para else if seguidos */
                char *c = codegen_gen_expr(eif->chained_conditional.condition);
                char *L_next = codegen_new_label();
                emit("IFFALSE %s GOTO %s", c, L_next);
                free(c);
                
                bool elifReturned=codegen_gen_statement(eif->chained_conditional.branch);
                if(!elifReturned) emit("GOTO %s", L_end);
                emit("LABEL %s", L_next);
                
                if(!elifReturned) allElifsReturn = false;
                eif = eif->chained_conditional.next;
            }

            bool elseExists = (stmt->conditional.else_branch != NULL);
            bool elseReturned = false;

            if(elseExists){
                elseReturned = codegen_gen_statement(stmt->conditional.else_branch);
            }

            emit("LABEL %s", L_end);

            bool ifGuarantees =
                thenReturned &&
                allElifsReturn &&
                (elseExists ? elseReturned : true);

            return ifGuarantees;
        }

        case AST_LOOP: {
            /* Tu AST actual representa loops sin condición (infinite). */
            char *L_start = codegen_new_label();
            char *L_end   = codegen_new_label();
            emit("LABEL %s", L_start);

            loop_push( L_end , L_start );

            (void)codegen_gen_statement(stmt->loop_type.body);
            emit("GOTO %s", L_start);
            emit("LABEL %s", L_end);

            loop_pop();

            free(L_start);
            free(L_end);
            return false;
        }

        case AST_BREAK: {
            LoopContext *context = loop_top();
            if(!context){
                emit("// SEMANTIC WARNING: break outside loop");
            } else {
                emit("GOTO %s", context->label_break);
            }
            return false;
        }

        case AST_CONTINUE: {
            LoopContext *context = loop_top();
            if(!context){
                emit("// SEMANTIC WARNING: continue outside loop");
            } else {
                emit("GOTO %s", context->label_continue);
            }
            return false;
        }

        case AST_RETURN: {
            /* if has value, compute it and assign to RVAL, then RETURN */
            if(stmt->result.value){
                char *r = codegen_gen_expr(stmt->result.value);
                emit("ASSIGN %s RVAL", r);
                free(r);
            } else {
                /* no value: set RVAL to 0 by default */
                emit("ASSIGN 0 RVAL");
            }
            emit("RETURN");
            return true;
        }

        case AST_FUNCTION_DECL: {
            /* function label */
            const char *fname = stmt->function_declaration.name;
            emit("LABEL %s", fname);

            /* declare parameters as variables and retrieve them with PARAM_GET */
            int pcount = stmt->function_declaration.param_count;
            ASTNode **params = stmt->function_declaration.params;
            for(int i=0;i<pcount;i++){
                ASTNode *pnode = params[i]; /* param_decl AST_DECLARATION node */
                const char *pname = pnode->declaration.name;
                ensure_declared(pname);
                /* retrieve argument i into pname */
                emit("PARAM_GET %d %s", i, pname);
            }

            /* generate function body */
            bool returned= codegen_gen_statement(stmt->function_declaration.body);

            /* if execution reaches end of function without RETURN, ensure RETURN */
            /* ensure RVAL exists (maybe set to 0) */
            if(!returned){
                emit("ASSIGN 0 RVAL");
                emit("RETURN");
            }
            return false;
        }
        case AST_FUNCTION_CALL: {
            /* functions used as statements: generate call and discard RVAL */
            /* push params */
            for(int i=0;i<stmt->function_call.arg_count;i++){
                char *a = codegen_gen_expr(stmt->function_call.args[i]);
                emit("PARAM %s", a);
                free(a);
            }
            emit("GOSUB %s", stmt->function_call.name);
            /* optionally discard RVAL or ignore */
            return false;
        }
        default:
            /* expresiones sueltas: evaluarlas y descartar resultado */
            {
                char *r = codegen_gen_expr(stmt);
                free(r);
                return false;
            }
    }
}

bool codegen_gen_sequence(ASTNode *seq){
    if(!seq) return false;
    for(int i=0;i<seq->sequence.count;i++){
        //codegen_gen_statement(seq->sequence.list[i]);
        bool ret = codegen_gen_statement(seq->sequence.list[i]);
        if(ret) return true;     //Si algún statement garantiza RETURN, paramos
    }
    return false;
}
