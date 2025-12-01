/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 8 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Tabla simple de variables (la que compartiste) */
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

/* Forward del lexer */
int yylex(void);

/* Reporte de errores sintácticos */
void yyerror(const char *s) {
    fprintf(stderr, "Error Sintactico: %s\n", s);
}

/* ========================
    DEFINICION DE TIPOS AST
   ======================== */

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
    AST_LOOP,
    AST_RETURN
} ASTKind;

typedef struct ASTNode ASTNode;

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
    ASTNode** list;
    int count;
    int cap;
} AST_Sequence;

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
        AST_Sequence sequence;
    };
};

/* CONSTRUCTORES */
// TODO malloc revision
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
    n->assign.name = id;
    n->assign.value = expr;
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

/* Liberar AST (recursivo) */
void free_ast(ASTNode* n){
    if(!n) return;
    switch(n->kind){
        case AST_NUMBER: break;
        case AST_STRING: free(n->str); break;
        case AST_BOOLEAN: break;
        case AST_IDENTIFIER: free(n->id); break;
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
        /*
        case AST_IF:
            free_ast(n->if_stmt.cond);
            free_ast(n->if_stmt.then_branch);
            free_ast(n->if_stmt.else_branch);
            break;
        case AST_LOOP:
            free_ast(n->loop_stmt.cond);
            free_ast(n->loop_stmt.body);
            break;
        case AST_RETURN:
            free_ast(n->ret.value);
            break;
        */
        case AST_SEQUENCE:
            /* Si implementas secuencias con listas, liberarlas aquí */
            break;
    }
    free(n);
}


/* Evaluador simple: devuelve double (0.0 = false, !=0 = true).
   Ejecuta ASSIGN (usa guardar()) y devuelve el valor asignado. */
double eval_ast(ASTNode* n){
    if(!n) return 0.0;
    switch(n->kind){
        case AST_NUMBER: return n->num;
        case AST_STRING: /* strings no se usan en eval numérico */ return 0.0;
        case AST_BOOLEAN: return n->boolean ? 1.0 : 0.0;
        case AST_IDENTIFIER: return obtener(n->id);
        case AST_ASSIGN: {
            double v = eval_ast(n->assign.value);
            guardar(n->assign.name, v);
            return v;
        }
        case AST_UNOP: {
            double v = eval_ast(n->unop.expr);
            if(strcmp(n->unop.op, "-") == 0) return -v;
            if(strcmp(n->unop.op, "!") == 0) return (v == 0.0) ? 1.0 : 0.0;
            return 0.0;
        }
        case AST_BINOP: {
            double L = eval_ast(n->binop.left);
            double R = eval_ast(n->binop.right);
            const char* op = n->binop.op;
            if(strcmp(op, "+") == 0) return L + R;
            if(strcmp(op, "-") == 0) return L - R;
            if(strcmp(op, "*") == 0) return L * R;
            if(strcmp(op, "/") == 0) { if(R == 0.0) { yyerror("division por cero"); return 0.0; } return L / R; }
            //if(strcmp(op, "==") == 0) return (L == R) ? 1.0 : 0.0;
            if(strcmp(op, "=") == 0) return (L == R) ? 1.0 : 0.0; /* igualdad sintáctica si la usas */
            if(strcmp(op, "<") == 0) return (L < R) ? 1.0 : 0.0;
            if(strcmp(op, ">") == 0) return (L > R) ? 1.0 : 0.0;
            if(strcmp(op, "<=") == 0) return (L <= R) ? 1.0 : 0.0;
            if(strcmp(op, ">=") == 0) return (L >= R) ? 1.0 : 0.0;
            if(strcmp(op, "&&") == 0) return ( (L!=0.0) && (R!=0.0) ) ? 1.0 : 0.0;
            if(strcmp(op, "||") == 0) return ( (L!=0.0) || (R!=0.0) ) ? 1.0 : 0.0;
            return 0.0;
        }
        default:
            return 0.0;
    }
}



#line 335 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_LIT_STRING = 4,                 /* LIT_STRING  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_ASSIGN = 6,                     /* ASSIGN  */
  YYSYMBOL_BEGIN_LOOP = 7,                 /* BEGIN_LOOP  */
  YYSYMBOL_END_LOOP = 8,                   /* END_LOOP  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 10,                  /* CONTINUE  */
  YYSYMBOL_RETURN_VALUE = 11,              /* RETURN_VALUE  */
  YYSYMBOL_RETURN_TYPE = 12,               /* RETURN_TYPE  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE_IF = 14,                   /* ELSE_IF  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_TYPE_DOUBLE = 16,               /* TYPE_DOUBLE  */
  YYSYMBOL_TYPE_FLOAT = 17,                /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_INTEGER = 18,              /* TYPE_INTEGER  */
  YYSYMBOL_TYPE_STRING = 19,               /* TYPE_STRING  */
  YYSYMBOL_TYPE_BOOLEAN = 20,              /* TYPE_BOOLEAN  */
  YYSYMBOL_TYPE_LIST = 21,                 /* TYPE_LIST  */
  YYSYMBOL_TYPE_ARRAY = 22,                /* TYPE_ARRAY  */
  YYSYMBOL_ERROR = 23,                     /* ERROR  */
  YYSYMBOL_BEGIN_SEQUENCE = 24,            /* BEGIN_SEQUENCE  */
  YYSYMBOL_SEQUENCE_SEPARATOR = 25,        /* SEQUENCE_SEPARATOR  */
  YYSYMBOL_END_SEQUENCE = 26,              /* END_SEQUENCE  */
  YYSYMBOL_OP_EQUALS = 27,                 /* OP_EQUALS  */
  YYSYMBOL_OP_LESSER = 28,                 /* OP_LESSER  */
  YYSYMBOL_OP_GREATER = 29,                /* OP_GREATER  */
  YYSYMBOL_OP_GREATER_EQUAL = 30,          /* OP_GREATER_EQUAL  */
  YYSYMBOL_OP_LESSER_EQUAL = 31,           /* OP_LESSER_EQUAL  */
  YYSYMBOL_BOOL_AND = 32,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 33,                   /* BOOL_OR  */
  YYSYMBOL_BOOL_NOT = 34,                  /* BOOL_NOT  */
  YYSYMBOL_35_ = 35,                       /* '+'  */
  YYSYMBOL_36_ = 36,                       /* '-'  */
  YYSYMBOL_37_ = 37,                       /* '*'  */
  YYSYMBOL_38_ = 38,                       /* '/'  */
  YYSYMBOL_UMINUS = 39,                    /* UMINUS  */
  YYSYMBOL_40_ = 40,                       /* '('  */
  YYSYMBOL_41_ = 41,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_statement_list = 44,            /* statement_list  */
  YYSYMBOL_statement = 45,                 /* statement  */
  YYSYMBOL_expresion = 46                  /* expresion  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   128

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  5
/* YYNRULES -- Number of rules.  */
#define YYNRULES  24
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  44

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,    37,    35,     2,    36,     2,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   323,   323,   324,   328,   329,   333,   334,   344,   345,
     346,   347,   348,   349,   350,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "LIT_STRING",
  "ID", "ASSIGN", "BEGIN_LOOP", "END_LOOP", "BREAK", "CONTINUE",
  "RETURN_VALUE", "RETURN_TYPE", "IF", "ELSE_IF", "ELSE", "TYPE_DOUBLE",
  "TYPE_FLOAT", "TYPE_INTEGER", "TYPE_STRING", "TYPE_BOOLEAN", "TYPE_LIST",
  "TYPE_ARRAY", "ERROR", "BEGIN_SEQUENCE", "SEQUENCE_SEPARATOR",
  "END_SEQUENCE", "OP_EQUALS", "OP_LESSER", "OP_GREATER",
  "OP_GREATER_EQUAL", "OP_LESSER_EQUAL", "BOOL_AND", "BOOL_OR", "BOOL_NOT",
  "'+'", "'-'", "'*'", "'/'", "UMINUS", "'('", "')'", "$accept", "program",
  "statement_list", "statement", "expresion", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-22)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -3,   -22,   -22,     7,     0,     0,     0,    25,    -3,   -22,
      29,     0,   -22,    55,   -22,   -21,   -22,   -22,   -22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   -22,    90,   -14,   -14,   -14,   -14,    67,    79,   -19,
     -19,   -22,   -22,   -22
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     8,     9,    10,     0,     0,     0,     0,     3,     4,
       0,     0,    10,    22,    23,     0,     1,     5,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    15,    16,    17,    19,    18,    20,    21,    11,
      12,    13,    14,     7
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -22,   -22,   -22,    18,    24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     7,     8,     9,    10
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       1,     2,     3,     1,     2,    12,    19,    20,    21,    22,
      23,    24,    25,    11,    26,    27,    28,    29,    28,    29,
      31,    26,    27,    28,    29,    16,    17,     0,    13,    14,
      15,     4,     0,     5,     4,    30,     5,     6,     0,     0,
       6,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    18,     0,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,    28,    29,    43,     0,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
      28,    29,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,    28,    29,    19,    20,    21,    22,    23,     0,
      25,     0,    26,    27,    28,    29,    19,    20,    21,    22,
      23,     0,     0,     0,    26,    27,    28,    29,    20,    21,
      22,    23,     0,     0,     0,    26,    27,    28,    29
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     3,     4,     5,    27,    28,    29,    30,
      31,    32,    33,     6,    35,    36,    37,    38,    37,    38,
      41,    35,    36,    37,    38,     0,     8,    -1,     4,     5,
       6,    34,    -1,    36,    34,    11,    36,    40,    -1,    -1,
      40,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    27,    28,    29,    30,    31,    -1,
      33,    -1,    35,    36,    37,    38,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    28,    29,
      30,    31,    -1,    -1,    -1,    35,    36,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    34,    36,    40,    43,    44,    45,
      46,     6,     5,    46,    46,    46,     0,    45,    25,    27,
      28,    29,    30,    31,    32,    33,    35,    36,    37,    38,
      46,    41,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    43,    44,    44,    45,    45,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     4,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: %empty  */
#line 323 "parser.y"
                     { (yyval.node) = new_sequence(); }
#line 1407 "parser.tab.c"
    break;

  case 3: /* program: statement_list  */
#line 324 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1413 "parser.tab.c"
    break;

  case 4: /* statement_list: statement  */
#line 328 "parser.y"
                { (yyval.node) = new_sequence(); sequence_add( (yyval.node) , (yyvsp[0].node) ); }
#line 1419 "parser.tab.c"
    break;

  case 5: /* statement_list: statement_list statement  */
#line 329 "parser.y"
                               { sequence_add( (yyvsp[-1].node) , (yyvsp[0].node) ); (yyval.node) = (yyvsp[-1].node); }
#line 1425 "parser.tab.c"
    break;

  case 6: /* statement: expresion SEQUENCE_SEPARATOR  */
#line 333 "parser.y"
                                    { (yyval.node) = (yyvsp[-1].node); }
#line 1431 "parser.tab.c"
    break;

  case 7: /* statement: ID ASSIGN expresion SEQUENCE_SEPARATOR  */
#line 334 "parser.y"
                                             {
            ASTNode *node = new_assign((yyvsp[-3].id), (yyvsp[-1].node));
            double val = eval_ast((yyvsp[-1].node));
            printf("Asignación: %s = %g\n", (yyvsp[-3].id), val);
            guardar((yyvsp[-3].id), val);
            (yyval.node) = node;
        }
#line 1443 "parser.tab.c"
    break;

  case 8: /* expresion: NUMBER  */
#line 344 "parser.y"
            { (yyval.node) = new_num( (yyvsp[0].num) ); printf("Numero reconocido: %g\n", (yyvsp[0].num)); }
#line 1449 "parser.tab.c"
    break;

  case 9: /* expresion: LIT_STRING  */
#line 345 "parser.y"
                    { (yyval.node) = new_string((yyvsp[0].str)); printf("String reconocida: %s\n" , (yyvsp[0].str) ); }
#line 1455 "parser.tab.c"
    break;

  case 10: /* expresion: ID  */
#line 346 "parser.y"
            { (yyval.node) = new_identifier( (yyvsp[0].id) ); printf( "Variable %s" , (yyvsp[0].id) ); }
#line 1461 "parser.tab.c"
    break;

  case 11: /* expresion: expresion '+' expresion  */
#line 347 "parser.y"
                                { (yyval.node) = new_binop( "+" , (yyvsp[-2].node) , (yyvsp[0].node) ); printf( "Suma resulta en: %g" , eval_ast((yyval.node)) ); }
#line 1467 "parser.tab.c"
    break;

  case 12: /* expresion: expresion '-' expresion  */
#line 348 "parser.y"
                                { (yyval.node) = new_binop( "-" , (yyvsp[-2].node) , (yyvsp[0].node) ); printf( "Resta resulta en: %g" , eval_ast((yyval.node)) ); }
#line 1473 "parser.tab.c"
    break;

  case 13: /* expresion: expresion '*' expresion  */
#line 349 "parser.y"
                                { (yyval.node) = new_binop( "*" , (yyvsp[-2].node) , (yyvsp[0].node) ); printf( "Suma resulta en: %g" , eval_ast((yyval.node)) ); }
#line 1479 "parser.tab.c"
    break;

  case 14: /* expresion: expresion '/' expresion  */
#line 350 "parser.y"
                                {
                                    if( eval_ast((yyvsp[0].node)) == 0.0 ){
                                        yyerror("division por 0");
                                        (yyval.node) = new_num(0.0);
                                    }else{
                                        (yyval.node) = new_binop( "/" , (yyvsp[-2].node) , (yyvsp[0].node) );
                                        printf( "Division resulta en: %g" , eval_ast((yyval.node)) );
                                    }
                                }
#line 1493 "parser.tab.c"
    break;

  case 15: /* expresion: expresion OP_EQUALS expresion  */
#line 359 "parser.y"
                                    { (yyval.node) = new_binop( "=" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1499 "parser.tab.c"
    break;

  case 16: /* expresion: expresion OP_LESSER expresion  */
#line 360 "parser.y"
                                    { (yyval.node) = new_binop( "<" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1505 "parser.tab.c"
    break;

  case 17: /* expresion: expresion OP_GREATER expresion  */
#line 361 "parser.y"
                                        { (yyval.node) = new_binop( ">" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1511 "parser.tab.c"
    break;

  case 18: /* expresion: expresion OP_LESSER_EQUAL expresion  */
#line 362 "parser.y"
                                            { (yyval.node) = new_binop( "<=" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1517 "parser.tab.c"
    break;

  case 19: /* expresion: expresion OP_GREATER_EQUAL expresion  */
#line 363 "parser.y"
                                            { (yyval.node) = new_binop( ">=" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1523 "parser.tab.c"
    break;

  case 20: /* expresion: expresion BOOL_AND expresion  */
#line 364 "parser.y"
                                    { (yyval.node) = new_binop( "&&" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1529 "parser.tab.c"
    break;

  case 21: /* expresion: expresion BOOL_OR expresion  */
#line 365 "parser.y"
                                    { (yyval.node) = new_binop( "||" , (yyvsp[-2].node) , (yyvsp[0].node) ); }
#line 1535 "parser.tab.c"
    break;

  case 22: /* expresion: BOOL_NOT expresion  */
#line 366 "parser.y"
                            { (yyval.node) = new_unop( "!" , (yyvsp[0].node) ); }
#line 1541 "parser.tab.c"
    break;

  case 23: /* expresion: '-' expresion  */
#line 367 "parser.y"
                                    { (yyval.node) = new_unop( "neg" , (yyvsp[0].node)); }
#line 1547 "parser.tab.c"
    break;

  case 24: /* expresion: '(' expresion ')'  */
#line 368 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1553 "parser.tab.c"
    break;


#line 1557 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 370 "parser.y"


/* ============================
   SECCIÓN 4: C epilogue (main opcional)
   ============================ */

int main(int argc, char** argv){
    printf("Parser AST - inicia. Introduce sentencias terminadas en ';'\\n");
    yyparse();
    return 0;
}
