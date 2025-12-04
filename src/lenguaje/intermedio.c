/* 
   Generador de Código Intermedio (FIS-25) a partir del AST.

   Uso esperado:
     cg_init();
     cg_generate_program(root); // root: AST root (sequence)
     cg_dump_stdout();          // o cg_dump_file("out.fis")
     cg_free();

   Notas:
    - Para retorno de funciones usamos variable "ret_<fname>".
    - Para arrays/lists inicializados hay comentarios y una posible extensión a heap.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "type.h"

/* ------------------ Configuración interna ------------------ */
#define INITIAL_CODE_CAP 256
#define MAX_NAME 128

/* Buffer de código: lista dinámica de líneas */
static char **code_lines = NULL;
static int code_count = 0;
static int code_cap = 0;

/* Contadores para temps y labels */
static int temp_counter = 0;
static int label_counter = 0;

/* Variables globales ya declaradas para evitar redeclarar VAR */
typedef struct {
    char **names;
    int count;
    int cap;
} NameSet;

static NameSet declared_vars = {NULL, 0, 0};

/* Helper para añadir una línea de código */
static void emit_line(const char *fmt, ...) {
    if (code_lines == NULL) {
        code_cap = INITIAL_CODE_CAP;
        code_lines = malloc(sizeof(char*) * code_cap);
        code_count = 0;
    }
    if (code_count == code_cap) {
        code_cap *= 2;
        code_lines = realloc(code_lines, sizeof(char*) * code_cap);
    }
    va_list ap;
    va_start(ap, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    code_lines[code_count++] = strdup(buf);
}

/* Helpers de temporales/labels */
static char *new_temp(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "t%d", temp_counter++);
    return strdup(buf);
}

static char *new_label(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "L%d", label_counter++);
    return strdup(buf);
}

/* NameSet helpers */
static void nameset_init(NameSet *s) {
    s->cap = 32;
    s->count = 0;
    s->names = malloc(sizeof(char*) * s->cap);
}
static void nameset_free(NameSet *s) {
    for(int i=0;i<s->count;i++) free(s->names[i]);
    free(s->names);
    s->names = NULL; s->count = s->cap = 0;
}
static int nameset_has(NameSet *s, const char *name) {
    for(int i=0;i<s->count;i++) if(strcmp(s->names[i], name)==0) return 1;
    return 0;
}
static void nameset_add(NameSet *s, const char *name) {
    if(nameset_has(s, name)) return;
    if(s->names == NULL) nameset_init(s);
    if(s->count == s->cap) {
        s->cap *= 2;
        s->names = realloc(s->names, sizeof(char*) * s->cap);
    }
    s->names[s->count++] = strdup(name);
}

/* Helper para concatenar código (se asume que los subnodos ya emitieron sus líneas) */
/* En este diseño las funciones generan instrucciones directamente en el buffer global,
   y devuelven 'place' (nombre del temporal o literal) por retorno. */

/* Convierte un literal numérico a string (place) */
static char *place_from_number(double v) {
    char buf[64];
    /* imprimimos como entero si es entero */
    if (floor(v) == v) snprintf(buf, sizeof(buf), "%d", (int)v);
    else snprintf(buf, sizeof(buf), "%g", v);
    return strdup(buf);
}

/* Escapa y devuelve string literal (con comillas) */
static char *place_from_string(const char *s) {
    int len = strlen(s);
    char *out = malloc(len + 3);
    out[0] = '"';
    strcpy(out+1, s);
    out[len+1] = '"';
    out[len+2] = '\0';
    return out;
}

/* ------------------ Generación de expresiones ------------------ */

/* Genera código para una expresión y retorna un 'place' (char*). 
   El caller es responsable de free() del place devuelto. */
static char *cg_generate_expr(ASTNode *expr);

/* Mapea operador binario a instrucción FIS-25. Retorna NULL si es comparación lógica manejada aparte. */
static const char *binop_to_instr(const char *op) {
    if(strcmp(op, "+")==0) return "ADD";
    if(strcmp(op, "-")==0) return "SUB";
    if(strcmp(op, "*")==0) return "MUL";
    if(strcmp(op, "/")==0) return "DIV";
    if(strcmp(op, "%")==0) return "MOD";
    if(strcmp(op, "^")==0) return "POW";
    /* comparaciones -> handled with EQ/NEQ/LT/LTE/GT/GTE */
    if(strcmp(op, "=")==0) return "EQ";
    if(strcmp(op, "!=")==0) return "NEQ";
    if(strcmp(op, "<")==0) return "LT";
    if(strcmp(op, "<=")==0) return "LTE";
    if(strcmp(op, ">")==0) return "GT";
    if(strcmp(op, ">=")==0) return "GTE";
    if(strcmp(op, "&&")==0) return "AND";
    if(strcmp(op, "||")==0) return "OR";
    return NULL;
}

static char *cg_generate_expr(ASTNode *expr) {
    if(!expr) return strdup("0");
    switch(expr->kind) {
        case AST_NUMBER: {
            return place_from_number(expr->num);
        }
        case AST_STRING: {
            return place_from_string(expr->str);
        }
        case AST_BOOLEAN: {
            return strdup(expr->boolean ? "1" : "0");
        }
        case AST_IDENTIFIER: {
            /* variable name is the place */
            return strdup(expr->id);
        }
        case AST_UNOP: {
            char *sub = cg_generate_expr(expr->unop.expr);
            if(strcmp(expr->unop.op, "-")==0) {
                char *t = new_temp();
                emit_line("SUB 0 %s %s", sub, t);
                free(sub);
                return t;
            } else if(strcmp(expr->unop.op, "!")==0) {
                char *t = new_temp();
                /* UNARY NOT: compute EQ sub 0 -> t (1 if equal to zero -> true?), we want !x => (x == 0)?1:0 */
                emit_line("EQ %s 0 %s", sub, t); /* t = (sub == 0) */
                free(sub);
                return t;
            } else {
                /* unknown unop */
                free(sub);
                return strdup("0");
            }
        }
        case AST_BINOP: {
            const char *instr = binop_to_instr(expr->binop.op);
            char *L = cg_generate_expr(expr->binop.left);
            char *R = cg_generate_expr(expr->binop.right);
            char *t = new_temp();
            if(instr) {
                emit_line("%s %s %s %s", instr, L, R, t);
            } else {
                /* fallback: try to handle logical ops */
                if(strcmp(expr->binop.op, "&&")==0) {
                    /* a && b -> compute a, if false short-circuit; simple implementation: compute both and AND */
                    emit_line("AND %s %s %s", L, R, t);
                } else if(strcmp(expr->binop.op, "||")==0) {
                    emit_line("OR %s %s %s", L, R, t);
                } else {
                    /* unknown */
                    emit_line("# UNKNOWN_BINOP %s", expr->binop.op);
                    emit_line("ASSIGN 0 %s", t);
                }
            }
            free(L); free(R);
            return t;
        }
        case AST_ASSIGN: {
            /* assign.name <- value */
            char *val = cg_generate_expr(expr->assign.value);
            /* generate assignment */
            emit_line("ASSIGN %s %s", val, expr->assign.name);
            nameset_add(&declared_vars, expr->assign.name); /* ensure declared to avoid missing VAR */
            free(val);
            /* return destination as place (so assignment can be expression value if needed) */
            return strdup(expr->assign.name);
        }
        case AST_FUNCTION_CALL: {
            /* Push params */
            for(int i=0;i<expr->function_call.arg_count;i++){
                char *argp = cg_generate_expr(expr->function_call.args[i]);
                emit_line("PARAM %s", argp);
                free(argp);
            }
            /* call */
            emit_line("GOSUB %s", expr->function_call.name);
            /* convention: read return from ret_<funcname> into new temp */
            char tmpname[MAX_NAME];
            snprintf(tmpname, sizeof(tmpname), "ret_%s", expr->function_call.name);
            /* ensure ret variable exists (in case caller used before declaration) */
            nameset_add(&declared_vars, tmpname);
            char *t = new_temp();
            emit_line("ASSIGN %s %s", tmpname, t);
            return t;
        }
        case AST_INIT_LIST: {
            /* produce temporals for elements and leave a comment for runtime construction */
            /* We'll evaluate elements and then emit a comment; more advanced: allocate heap and store pointer */
            char *tmp = new_temp();
            emit_line("# INIT_LIST start -> %s (elements: %d)", tmp, expr->initializator.count);
            for(int i=0;i<expr->initializator.count;i++){
                char *p = cg_generate_expr(expr->initializator.elements[i]);
                emit_line("#   element %d -> %s", i, p);
                free(p);
            }
            emit_line("# INIT_LIST end -> %s", tmp);
            /* The tmp acts as handle placeholder */
            return tmp;
        }
        case AST_ARRAY: {
            /* new array(type, size_expr) */
            char *sizep = cg_generate_expr(expr->array.size_expr);
            char *tmp = new_temp();
            emit_line("# NEW_ARRAY of type (placeholder) -> %s", tmp);
            emit_line("# array size expr -> %s", sizep);
            /* advanced: call runtime ALLOC, omitted */
            emit_line("ASSIGN 0 %s  # placeholder handle for array", tmp);
            free(sizep);
            return tmp;
        }
        case AST_LIST: {
            char *sizep = cg_generate_expr(expr->list.size_expr);
            char *tmp = new_temp();
            emit_line("# NEW_LIST (placeholder) -> %s", tmp);
            emit_line("# list size expr -> %s", sizep);
            emit_line("ASSIGN 0 %s  # placeholder handle for list", tmp);
            free(sizep);
            return tmp;
        }
        default: {
            /* default: not supported expression type */
            emit_line("# Unsupported expr kind %d", expr->kind);
            return strdup("0");
        }
    }
}

/* ------------------ Generación de statements ------------------ */

/* Forward declarations */
static void cg_generate_statement(ASTNode *stmt);

static void cg_generate_sequence(ASTNode *seq) {
    if(!seq) return;
    for(int i=0;i<seq->sequence.count;i++){
        cg_generate_statement(seq->sequence.list[i]);
    }
}

static void cg_generate_statement(ASTNode *stmt) {
    if(!stmt) return;
    switch(stmt->kind) {
        case AST_SEQUENCE:
            cg_generate_sequence(stmt);
            break;
        case AST_DECLARATION: {
            /* emit VAR <name> and optional init */
            const char *name = stmt->declaration.name;
            if(!nameset_has(&declared_vars, name)) {
                emit_line("VAR %s", name);
                nameset_add(&declared_vars, name);
            } else {
                /* if already declared, skip */
            }
            if(stmt->declaration.init) {
                char *place = cg_generate_expr(stmt->declaration.init);
                emit_line("ASSIGN %s %s", place, name);
                free(place);
            }
            break;
        }
        case AST_ASSIGN: {
            /* In your grammar assignment also appears as statement via assign_expr SEQUENCE_SEPARATOR */
            char *place = cg_generate_expr(stmt); /* cg_generate_expr handles ASSIGN node and emits ASSIGN */
            free(place);
            break;
        }
        case AST_RETURN: {
            /* Evaluate return value if exists and assign to ret_<funcname>, then emit RETURN.
               We need to know current function name; since AST node doesn't hold it, we assume:
               - The codegen caller sets a global 'current_function_name' before generating the body.
               For simplicity we used naming convention: function's return variable is ret_<fname>.
               So this case is usually processed while generating a function body (see function decl below).
            */
            /* This default behavior will only emit RETURN, but function decl generation sets ret var itself. */
            if(stmt->result.value) {
                char *val = cg_generate_expr(stmt->result.value);
                /* current function return var must be set by caller code (we keep a stack) */
                /* We will search the latest "current_func_retvar" from a simple stack */
                emit_line("# RETURN expr -> %s (no retvar bound)", val);
                free(val);
            } else {
                emit_line("RETURN");
            }
            break;
        }
        case AST_IF: {
            /* cond IF block (maybe elif_chain and else_part saved inside AST) */
            char *condp = cg_generate_expr(stmt->conditional.condition);
            char *Lfalse = new_label();
            char *Lend = new_label();
            /* if ... then */
            emit_line("IFFALSE %s GOTO %s", condp, Lfalse);
            free(condp);
            /* then branch: it's a block (sequence) */
            cg_generate_sequence(stmt->conditional.if_branch);
            /* after then, if there is elif or else, we must jump to end; else fall through */
            if(stmt->conditional.elif_list || stmt->conditional.else_branch) {
                emit_line("GOTO %s", Lend);
            }
            /* else-if chain */
            emit_line("LABEL %s", Lfalse);
            ASTNode *eif = stmt->conditional.elif_list;
            while(eif) {
                /* each elseif: has condition and branch */
                char *c = cg_generate_expr(eif->chained_conditional.condition);
                char *Lnext = new_label();
                emit_line("IFFALSE %s GOTO %s", c, Lnext);
                free(c);
                cg_generate_sequence(eif->chained_conditional.branch);
                emit_line("GOTO %s", Lend);
                emit_line("LABEL %s", Lnext);
                eif = eif->chained_conditional.next;
            }
            /* else branch */
            if(stmt->conditional.else_branch) {
                cg_generate_sequence(stmt->conditional.else_branch);
            }
            emit_line("LABEL %s", Lend);
            free(Lfalse); free(Lend);
            break;
        }
        case AST_LOOP: {
            /* BEGIN_LOOP statement_list END_LOOP SEQUENCE_SEPARATOR
               We treat as while(true) looping over the body.
               A more elaborate loop with condition could be adapted.
            */
            char *Lstart = new_label();
            char *Lend = new_label();
            emit_line("LABEL %s", Lstart);
            /* body */
            cg_generate_sequence(stmt->loop_type.body);
            emit_line("GOTO %s", Lstart);
            emit_line("LABEL %s", Lend);
            free(Lstart); free(Lend);
            break;
        }
        case AST_FUNCTION_DECL: {
            /* function_declaration.name, params (ASTNode**), param_count, return_type (Type*), body (sequence) */
            const char *fname = stmt->function_declaration.name;
            /* declare a return variable ret_<fname> at global scope */
            char retvar[MAX_NAME];
            snprintf(retvar, sizeof(retvar), "ret_%s", fname);
            if(!nameset_has(&declared_vars, retvar)) {
                emit_line("VAR %s", retvar);
                nameset_add(&declared_vars, retvar);
            }
            /* Label for function entry */
            emit_line("LABEL %s", fname);
            /* PARAM_GET <paramname> for each parameter (convention) */
            for(int i=0;i<stmt->function_declaration.param_count;i++){
                ASTNode *pdecl = stmt->function_declaration.params[i];
                char *pname = pdecl->declaration.name;
                /* declare local param variable */
                if(!nameset_has(&declared_vars, pname)) {
                    /* NOTE: parameters typically local to function; here we keep global VAR to simplify runtime.
                       In a more advanced system you'd manage stack frames and local variables separately.
                    */
                    emit_line("VAR %s", pname);
                    nameset_add(&declared_vars, pname);
                }
                emit_line("PARAM_GET %s", pname);
            }
            /* Generate body: we want RETURN statements to assign to retvar before RETURN.
               To do that, we temporarily set a global pointer telling cg_generate_statement
               which retvar to use. We'll implement a simple mechanism using a static variable.
            */
            /* push current function retvar */
            /* simple approach: set a global variable current_retvar_name used by return handling */
            /* We'll implement minimal support: scan body for RETURN nodes and handle them here by
               generating ASSIGN <expr> retvar followed by RETURN. To keep it simple, we call a helper that
               uses 'retvar' as the expected return target.*/
            /* To keep code small, we temporarily set a static char* used by return generation. */
            /* We will implement a tiny scope of 'current_retvar' here. */
            extern char *cg_current_retvar;
            char *prev_ret = cg_current_retvar;
            cg_current_retvar = strdup(retvar);
            /* generate body */
            cg_generate_sequence(stmt->function_declaration.body);
            /* if function falls through without explicit return, we emit RETURN */
            emit_line("RETURN");
            /* restore prev */
            free(cg_current_retvar);
            cg_current_retvar = prev_ret;
            break;
        }
        case AST_FUNCTION_CALL: {
            /* function call as statement: same as expr but we may not capture return */
            char *res = cg_generate_expr(stmt);
            free(res);
            break;
        }
        case AST_BREAK:
            /* Break handling requires loop targets; not implemented in this simple generator.
               We can emit a comment.
            */
            emit_line("# BREAK (not implemented)");
            break;
        case AST_CONTINUE:
            emit_line("# CONTINUE (not implemented)");
            break;
        case AST_STRING:
        case AST_NUMBER:
        case AST_BOOLEAN:
            /* expression as statement -> evaluate and drop */
            {
                char *p = cg_generate_expr(stmt);
                free(p);
            }
            break;
        default:
            emit_line("# Unsupported statement kind %d", stmt->kind);
            break;
    }
}

/* global var used by function-decl handling for RETURN */
char *cg_current_retvar = NULL;

/* We override handling of AST_RETURN to use cg_current_retvar if set.
   Provide a separate helper that should be called when encountering a RETURN node
   inside a function body generation. We'll patch cg_generate_statement to use the global. */
static void cg_handle_return_node(ASTNode *ret) {
    if(!ret) return;
    if(ret->result.value) {
        char *val = cg_generate_expr(ret->result.value);
        if(cg_current_retvar) {
            emit_line("ASSIGN %s %s", val, cg_current_retvar);
        } else {
            emit_line("# RETURN with value but no current retvar -> %s", val);
        }
        free(val);
    } else {
        /* return without value: just do nothing special for retvar */
    }
    emit_line("RETURN");
}

/* Patch: modify cg_generate_statement handling of AST_RETURN to call cg_handle_return_node.
   We'll replace earlier case by checking here:
*/
#undef cg_generate_statement /* ensure we re-declare below to include RETURN handling properly */

/* Re-declare cg_generate_statement with RETURN handling */
static void cg_generate_statement(ASTNode *stmt) {
    if(!stmt) return;
    switch(stmt->kind) {
        case AST_SEQUENCE:
            cg_generate_sequence(stmt);
            break;
        case AST_DECLARATION: {
            const char *name = stmt->declaration.name;
            if(!nameset_has(&declared_vars, name)) {
                emit_line("VAR %s", name);
                nameset_add(&declared_vars, name);
            }
            if(stmt->declaration.init) {
                char *place = cg_generate_expr(stmt->declaration.init);
                emit_line("ASSIGN %s %s", place, name);
                free(place);
            }
            break;
        }
        case AST_ASSIGN: {
            char *place = cg_generate_expr(stmt);
            free(place);
            break;
        }
        case AST_RETURN: {
            /* handle return with current retvar convention */
            cg_handle_return_node(stmt);
            break;
        }
        case AST_IF: {
            char *condp = cg_generate_expr(stmt->conditional.condition);
            char *Lfalse = new_label();
            char *Lend = new_label();
            emit_line("IFFALSE %s GOTO %s", condp, Lfalse);
            free(condp);
            cg_generate_sequence(stmt->conditional.if_branch);
            if(stmt->conditional.elif_list || stmt->conditional.else_branch) {
                emit_line("GOTO %s", Lend);
            }
            emit_line("LABEL %s", Lfalse);
            ASTNode *eif = stmt->conditional.elif_list;
            while(eif) {
                char *c = cg_generate_expr(eif->chained_conditional.condition);
                char *Lnext = new_label();
                emit_line("IFFALSE %s GOTO %s", c, Lnext);
                free(c);
                cg_generate_sequence(eif->chained_conditional.branch);
                emit_line("GOTO %s", Lend);
                emit_line("LABEL %s", Lnext);
                eif = eif->chained_conditional.next;
            }
            if(stmt->conditional.else_branch) {
                cg_generate_sequence(stmt->conditional.else_branch);
            }
            emit_line("LABEL %s", Lend);
            free(Lfalse); free(Lend);
            break;
        }
        case AST_LOOP: {
            char *Lstart = new_label();
            char *Lend = new_label();
            emit_line("LABEL %s", Lstart);
            cg_generate_sequence(stmt->loop_type.body);
            emit_line("GOTO %s", Lstart);
            emit_line("LABEL %s", Lend);
            free(Lstart); free(Lend);
            break;
        }
        case AST_FUNCTION_DECL: {
            const char *fname = stmt->function_declaration.name;
            char retvar[MAX_NAME];
            snprintf(retvar, sizeof(retvar), "ret_%s", fname);
            if(!nameset_has(&declared_vars, retvar)) {
                emit_line("VAR %s", retvar);
                nameset_add(&declared_vars, retvar);
            }
            emit_line("LABEL %s", fname);
            for(int i=0;i<stmt->function_declaration.param_count;i++){
                ASTNode *pdecl = stmt->function_declaration.params[i];
                char *pname = pdecl->declaration.name;
                if(!nameset_has(&declared_vars, pname)) {
                    emit_line("VAR %s", pname);
                    nameset_add(&declared_vars, pname);
                }
                emit_line("PARAM_GET %s", pname);
            }
            /* set current retvar and generate body */
            char *prev = cg_current_retvar;
            cg_current_retvar = strdup(retvar);
            cg_generate_sequence(stmt->function_declaration.body);
            /* ensure a RETURN at end */
            emit_line("RETURN");
            free(cg_current_retvar);
            cg_current_retvar = prev;
            break;
        }
        case AST_FUNCTION_CALL: {
            char *res = cg_generate_expr(stmt);
            free(res);
            break;
        }
        case AST_BREAK:
            emit_line("# BREAK (not implemented)");
            break;
        case AST_CONTINUE:
            emit_line("# CONTINUE (not implemented)");
            break;
        case AST_STRING:
        case AST_NUMBER:
        case AST_BOOLEAN: {
            char *p = cg_generate_expr(stmt);
            free(p);
            break;
        }
        default:
            emit_line("# Unsupported statement kind %d", stmt->kind);
            break;
    }
}

/* ------------------ API público ------------------ */

void cg_init(void) {
    /* reset everything */
    if(code_lines) {
        for(int i=0;i<code_count;i++) free(code_lines[i]);
        free(code_lines);
    }
    code_lines = NULL;
    code_count = 0;
    code_cap = 0;
    temp_counter = 0;
    label_counter = 0;
    nameset_free(&declared_vars);
    declared_vars.names = NULL; declared_vars.count = declared_vars.cap = 0;
    cg_current_retvar = NULL;
}

void cg_free(void) {
    if(code_lines) {
        for(int i=0;i<code_count;i++) free(code_lines[i]);
        free(code_lines);
    }
    code_lines = NULL; code_count = code_cap = 0;
    nameset_free(&declared_vars);
    if(cg_current_retvar) { free(cg_current_retvar); cg_current_retvar = NULL; }
}

/* Genera código para todo el programa (root debe ser AST_SEQUENCE) */
void cg_generate_program(ASTNode *root) {
    if(!root) return;
    cg_init();
    /* walk top-level sequence */
    cg_generate_sequence(root);
}

/* Dump to stdout */
void cg_dump_stdout(void) {
    for(int i=0;i<code_count;i++){
        printf("%s\n", code_lines[i]);
    }
}

/* Dump to file */
int cg_dump_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if(!f) return -1;
    for(int i=0;i<code_count;i++){
        fprintf(f, "%s\n", code_lines[i]);
    }
    fclose(f);
    return 0;
}

/* ------------------ Ejemplo de main (comentado) ------------------
#include "ast.h"
int main() {
    ASTNode *root = ...; // obtén AST
    cg_generate_program(root);
    cg_dump_stdout();
    cg_free();
    return 0;
}
------------------------------------------------------------------ */

