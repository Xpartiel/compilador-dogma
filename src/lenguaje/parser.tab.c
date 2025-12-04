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
#line 5 "parser.y"

#include "ast.h"
#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



/* Forward del lexer */
int yylex(void);


/* Reporte de errores sintácticos */
void yyerror(const char *s) {
    fprintf(stderr, "Error Sintactico: %s\n", s);
}


#line 92 "parser.tab.c"

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
  YYSYMBOL_PARENTHESES_OPEN = 7,           /* PARENTHESES_OPEN  */
  YYSYMBOL_PARENTHESES_CLOSE = 8,          /* PARENTHESES_CLOSE  */
  YYSYMBOL_BEGIN_LOOP = 9,                 /* BEGIN_LOOP  */
  YYSYMBOL_END_LOOP = 10,                  /* END_LOOP  */
  YYSYMBOL_BREAK = 11,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 12,                  /* CONTINUE  */
  YYSYMBOL_RETURN_VALUE = 13,              /* RETURN_VALUE  */
  YYSYMBOL_RETURN_TYPE = 14,               /* RETURN_TYPE  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ELSE_IF = 16,                   /* ELSE_IF  */
  YYSYMBOL_ELSE = 17,                      /* ELSE  */
  YYSYMBOL_TYPE_DOUBLE = 18,               /* TYPE_DOUBLE  */
  YYSYMBOL_TYPE_FLOAT = 19,                /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_INTEGER = 20,              /* TYPE_INTEGER  */
  YYSYMBOL_TYPE_STRING = 21,               /* TYPE_STRING  */
  YYSYMBOL_TYPE_BOOLEAN = 22,              /* TYPE_BOOLEAN  */
  YYSYMBOL_TYPE_LIST = 23,                 /* TYPE_LIST  */
  YYSYMBOL_TYPE_ARRAY = 24,                /* TYPE_ARRAY  */
  YYSYMBOL_ERROR = 25,                     /* ERROR  */
  YYSYMBOL_LIST_SEPARATOR = 26,            /* LIST_SEPARATOR  */
  YYSYMBOL_BEGIN_SEQUENCE = 27,            /* BEGIN_SEQUENCE  */
  YYSYMBOL_SEQUENCE_SEPARATOR = 28,        /* SEQUENCE_SEPARATOR  */
  YYSYMBOL_END_SEQUENCE = 29,              /* END_SEQUENCE  */
  YYSYMBOL_OP_EQUALS = 30,                 /* OP_EQUALS  */
  YYSYMBOL_OP_LESSER = 31,                 /* OP_LESSER  */
  YYSYMBOL_OP_GREATER = 32,                /* OP_GREATER  */
  YYSYMBOL_OP_GREATER_EQUAL = 33,          /* OP_GREATER_EQUAL  */
  YYSYMBOL_OP_LESSER_EQUAL = 34,           /* OP_LESSER_EQUAL  */
  YYSYMBOL_OP_PLUS = 35,                   /* OP_PLUS  */
  YYSYMBOL_OP_MINUS = 36,                  /* OP_MINUS  */
  YYSYMBOL_OP_TIMES = 37,                  /* OP_TIMES  */
  YYSYMBOL_OP_DIVIDE = 38,                 /* OP_DIVIDE  */
  YYSYMBOL_BOOL_AND = 39,                  /* BOOL_AND  */
  YYSYMBOL_BOOL_OR = 40,                   /* BOOL_OR  */
  YYSYMBOL_BOOL_NOT = 41,                  /* BOOL_NOT  */
  YYSYMBOL_UMINUS = 42,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 43,                  /* $accept  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_statement_list = 45,            /* statement_list  */
  YYSYMBOL_block = 46,                     /* block  */
  YYSYMBOL_continue_expr = 47,             /* continue_expr  */
  YYSYMBOL_break_expr = 48,                /* break_expr  */
  YYSYMBOL_return_expr = 49,               /* return_expr  */
  YYSYMBOL_statement = 50,                 /* statement  */
  YYSYMBOL_expresion = 51,                 /* expresion  */
  YYSYMBOL_if_statement = 52,              /* if_statement  */
  YYSYMBOL_elif_chain = 53,                /* elif_chain  */
  YYSYMBOL_else_part = 54,                 /* else_part  */
  YYSYMBOL_loop_expr = 55,                 /* loop_expr  */
  YYSYMBOL_type = 56,                      /* type  */
  YYSYMBOL_var_init = 57,                  /* var_init  */
  YYSYMBOL_var_declaration = 58,           /* var_declaration  */
  YYSYMBOL_init_data_structure = 59,       /* init_data_structure  */
  YYSYMBOL_list_item = 60,                 /* list_item  */
  YYSYMBOL_param_decl = 61,                /* param_decl  */
  YYSYMBOL_param_list = 62,                /* param_list  */
  YYSYMBOL_optional_param_list = 63,       /* optional_param_list  */
  YYSYMBOL_fun_declaration = 64,           /* fun_declaration  */
  YYSYMBOL_arg_list = 65,                  /* arg_list  */
  YYSYMBOL_optional_arg_list = 66,         /* optional_arg_list  */
  YYSYMBOL_function_call = 67              /* function_call  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   325

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   297


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    94,    94,    95,    99,   102,   110,   116,   120,   124,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   141,
     144,   147,   150,   153,   156,   158,   161,   164,   167,   174,
     177,   180,   183,   186,   189,   192,   195,   197,   199,   204,
     207,   210,   213,   219,   222,   228,   235,   241,   244,   247,
     250,   253,   256,   260,   269,   270,   273,   282,   298,   303,
     306,   312,   316,   319,   325,   326,   330,   342,   345,   351,
     352,   356
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
  "ID", "ASSIGN", "PARENTHESES_OPEN", "PARENTHESES_CLOSE", "BEGIN_LOOP",
  "END_LOOP", "BREAK", "CONTINUE", "RETURN_VALUE", "RETURN_TYPE", "IF",
  "ELSE_IF", "ELSE", "TYPE_DOUBLE", "TYPE_FLOAT", "TYPE_INTEGER",
  "TYPE_STRING", "TYPE_BOOLEAN", "TYPE_LIST", "TYPE_ARRAY", "ERROR",
  "LIST_SEPARATOR", "BEGIN_SEQUENCE", "SEQUENCE_SEPARATOR", "END_SEQUENCE",
  "OP_EQUALS", "OP_LESSER", "OP_GREATER", "OP_GREATER_EQUAL",
  "OP_LESSER_EQUAL", "OP_PLUS", "OP_MINUS", "OP_TIMES", "OP_DIVIDE",
  "BOOL_AND", "BOOL_OR", "BOOL_NOT", "UMINUS", "$accept", "program",
  "statement_list", "block", "continue_expr", "break_expr", "return_expr",
  "statement", "expresion", "if_statement", "elif_chain", "else_part",
  "loop_expr", "type", "var_init", "var_declaration",
  "init_data_structure", "list_item", "param_decl", "param_list",
  "optional_param_list", "fun_declaration", "arg_list",
  "optional_arg_list", "function_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-45)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     164,   -45,   -45,    13,    39,   164,    -7,    10,    39,   -45,
     -45,   -45,   -45,   -45,   216,   216,   164,    39,    39,    36,
     164,   -45,   -45,   -45,   -45,   -45,   227,   -45,   -45,    55,
     -45,   -45,   -45,    39,   216,   216,   181,   103,   -45,   -45,
     254,   216,   216,    34,    54,   129,   -20,   287,   -45,   -45,
      37,   -45,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    59,    61,   265,    41,    60,    34,    54,
     -45,    42,   -45,   -45,   -45,    39,    39,   -45,     7,   161,
     -20,   -20,   -20,   -20,   -35,   -35,   -45,   -45,   276,   287,
       4,    45,    67,    39,   -45,   -45,   193,   214,    39,    37,
     -45,   -16,    48,    39,   265,   -45,   -45,   216,   265,   -45,
     -45,   241,   -45,   -45,    49,   -45,   265,     8,    73,   -45,
      53,    75,    37,   -45,    39,   -45,   -45,   216,    37,    65,
     265,   -45,    56,   -45,   -45
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,    19,    20,    21,     0,     0,     0,     0,     0,    49,
      48,    47,    51,    50,     0,     0,     0,     0,     0,     0,
       3,    18,    13,    14,    12,     4,     0,    16,    17,     0,
      15,    10,    36,    69,     0,     0,     0,     0,     8,     7,
       0,     0,     0,    53,    52,     0,    23,    22,     1,     5,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,     0,    67,    70,     0,     0,     0,
      24,     0,     9,    53,    52,     0,     0,     6,     0,    29,
      30,    31,    33,    32,    25,    26,    27,    28,    34,    35,
       0,     0,     0,     0,    71,    46,     0,     0,     0,     0,
      39,     0,     0,     0,    55,    56,    57,    64,    68,    38,
      37,     0,    45,    40,     0,    41,    59,     0,     0,    62,
      65,     0,     0,    42,     0,    58,    61,     0,     0,    43,
      60,    63,     0,    44,    66
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,    14,   -40,   -45,   -45,   -45,     2,    -4,   -45,
     -44,   -14,   -45,    -9,   -45,   -45,   -45,   -45,   -37,   -45,
     -45,   -45,   -45,   -45,   -45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
     101,   102,    28,    29,    91,    30,   105,   117,   119,   120,
     121,    31,    66,    67,    32
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      36,    99,    59,    60,    40,    43,    44,     1,     2,     3,
      78,     4,   113,    46,    47,    57,    58,    59,    60,    37,
      33,    38,    49,    98,    99,    68,    69,    34,    35,    65,
      45,   103,    73,    74,   124,   100,    48,   125,    39,    49,
      17,    75,     1,     2,     3,    18,     4,    49,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,   112,
      63,    76,    34,    35,    16,    90,    92,    93,    94,    64,
      95,    96,    97,   106,   107,    17,   115,   123,   126,   127,
      18,    98,   129,   128,   134,   133,   104,   114,   132,   108,
     131,     0,     0,     0,   111,     0,     0,     0,   118,   116,
       0,     0,     0,     0,     0,     0,     1,     2,     3,     0,
       4,     0,     5,    71,     6,     7,     8,     0,   118,     0,
     130,     9,    10,    11,    12,    13,    14,    15,     0,     0,
      16,     0,     1,     2,     3,     0,     4,     0,     5,    17,
       6,     7,     8,     0,    18,     0,     0,     9,    10,    11,
      12,    13,    14,    15,     0,     0,    16,     0,    77,     0,
       0,     0,     0,     0,     0,    17,     0,     1,     2,     3,
      18,     4,     0,     5,     0,     6,     7,     8,     0,     0,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    70,
       0,    16,    53,    54,    55,    56,    57,    58,    59,    60,
      17,   109,     0,     0,     0,    18,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,   110,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,     9,    10,    11,    12,    13,    41,
      42,     0,    50,     0,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    51,   122,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,     0,     0,
       0,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    72,     0,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    52,    53,    54,    55,
      56,    57,    58,    59,    60,     0,    62,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

static const yytype_int16 yycheck[] =
{
       4,    17,    37,    38,     8,    14,    15,     3,     4,     5,
      50,     7,    28,    17,    18,    35,    36,    37,    38,     5,
       7,    28,    20,    16,    17,    34,    35,    23,    24,    33,
      16,    27,    41,    42,    26,    28,     0,    29,    28,    37,
      36,     7,     3,     4,     5,    41,     7,    45,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    99,
       5,     7,    23,    24,    27,     6,     5,    26,     8,    14,
      28,    75,    76,    28,     7,    36,    28,    28,     5,    26,
      41,    16,   122,     8,    28,   129,    90,   101,   128,    93,
     127,    -1,    -1,    -1,    98,    -1,    -1,    -1,   107,   103,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,     9,    10,    11,    12,    13,    -1,   127,    -1,
     124,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    -1,     3,     4,     5,    -1,     7,    -1,     9,    36,
      11,    12,    13,    -1,    41,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    27,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,     3,     4,     5,
      41,     7,    -1,     9,    -1,    11,    12,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,     8,
      -1,    27,    31,    32,    33,    34,    35,    36,    37,    38,
      36,     8,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     8,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    18,    19,    20,    21,    22,    23,
      24,    -1,    15,    -1,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    28,    15,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    28,    -1,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    -1,    40,    30,    31,    32,
      33,    34,    35,    36,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,     9,    11,    12,    13,    18,
      19,    20,    21,    22,    23,    24,    27,    36,    41,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    55,    56,
      58,    64,    67,     7,    23,    24,    51,    45,    28,    28,
      51,    23,    24,    56,    56,    45,    51,    51,     0,    50,
      15,    28,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     5,    14,    51,    65,    66,    56,    56,
       8,    10,    28,    56,    56,     7,     7,    29,    46,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
       6,    57,     5,    26,     8,    28,    51,    51,    16,    17,
      28,    53,    54,    27,    51,    59,    28,     7,    51,     8,
       8,    51,    46,    28,    54,    28,    51,    60,    56,    61,
      62,    63,    15,    28,    26,    29,     5,    26,     8,    46,
      51,    61,    46,    53,    28
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    44,    45,    45,    46,    47,    48,    49,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    52,
      52,    52,    52,    53,    53,    54,    55,    56,    56,    56,
      56,    56,    56,    56,    57,    57,    57,    58,    59,    60,
      60,    61,    62,    62,    63,    63,    64,    65,    65,    66,
      66,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     2,     2,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     5,     5,     4,
       5,     5,     6,     4,     5,     2,     4,     1,     1,     1,
       1,     1,     2,     2,     0,     2,     2,     4,     3,     1,
       3,     2,     1,     3,     0,     1,     8,     1,     3,     0,
       1,     4
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
#line 94 "parser.y"
                     { (yyval.node) = new_sequence(); }
#line 1277 "parser.tab.c"
    break;

  case 3: /* program: statement_list  */
#line 95 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1283 "parser.tab.c"
    break;

  case 4: /* statement_list: statement  */
#line 99 "parser.y"
                {
        (yyval.node) = new_sequence();
        sequence_add( (yyval.node) , (yyvsp[0].node) ); }
#line 1291 "parser.tab.c"
    break;

  case 5: /* statement_list: statement_list statement  */
#line 102 "parser.y"
                               {
        sequence_add( (yyvsp[-1].node) , (yyvsp[0].node) );
        (yyval.node) = (yyvsp[-1].node); }
#line 1299 "parser.tab.c"
    break;

  case 6: /* block: BEGIN_SEQUENCE statement_list END_SEQUENCE  */
#line 110 "parser.y"
                                                {
        (yyval.node) = (yyvsp[-1].node);
        printf("[CODE BLOCK]\n");}
#line 1307 "parser.tab.c"
    break;

  case 7: /* continue_expr: CONTINUE SEQUENCE_SEPARATOR  */
#line 116 "parser.y"
                                { (yyval.node) = new_continue(); }
#line 1313 "parser.tab.c"
    break;

  case 8: /* break_expr: BREAK SEQUENCE_SEPARATOR  */
#line 120 "parser.y"
                                { (yyval.node) = new_break(); }
#line 1319 "parser.tab.c"
    break;

  case 9: /* return_expr: RETURN_VALUE expresion SEQUENCE_SEPARATOR  */
#line 124 "parser.y"
                                              { (yyval.node) = new_return((yyvsp[-1].node)); }
#line 1325 "parser.tab.c"
    break;

  case 10: /* statement: fun_declaration  */
#line 128 "parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1331 "parser.tab.c"
    break;

  case 11: /* statement: expresion SEQUENCE_SEPARATOR  */
#line 129 "parser.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 1337 "parser.tab.c"
    break;

  case 12: /* statement: return_expr  */
#line 130 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1343 "parser.tab.c"
    break;

  case 13: /* statement: continue_expr  */
#line 131 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1349 "parser.tab.c"
    break;

  case 14: /* statement: break_expr  */
#line 132 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1355 "parser.tab.c"
    break;

  case 15: /* statement: var_declaration  */
#line 133 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1361 "parser.tab.c"
    break;

  case 16: /* statement: if_statement  */
#line 134 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1367 "parser.tab.c"
    break;

  case 17: /* statement: loop_expr  */
#line 135 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1373 "parser.tab.c"
    break;

  case 18: /* statement: block  */
#line 136 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1379 "parser.tab.c"
    break;

  case 19: /* expresion: NUMBER  */
#line 141 "parser.y"
            {
        (yyval.node) = new_num( (yyvsp[0].num) );
        printf("[Numero]: %g\n", (yyvsp[0].num)); }
#line 1387 "parser.tab.c"
    break;

  case 20: /* expresion: LIT_STRING  */
#line 144 "parser.y"
                    {
        (yyval.node) = new_string( (yyvsp[0].str) );
        printf("String reconocida: %s\n" , (yyvsp[0].str) ); }
#line 1395 "parser.tab.c"
    break;

  case 21: /* expresion: ID  */
#line 147 "parser.y"
            {
        (yyval.node) = new_identifier( (yyvsp[0].id) );
        printf( "Variable %s" , (yyvsp[0].id) );/*TODO REMOVE*/ }
#line 1403 "parser.tab.c"
    break;

  case 22: /* expresion: BOOL_NOT expresion  */
#line 150 "parser.y"
                            {
        (yyval.node) = new_unop( "!" , (yyvsp[0].node) );
        printf( "Negacion NOT: %g" , eval_ast((yyval.node)) );/*TODO REMOVE*/}
#line 1411 "parser.tab.c"
    break;

  case 23: /* expresion: OP_MINUS expresion  */
#line 153 "parser.y"
                                        {
        (yyval.node) = new_unop("-", (yyvsp[0].node));
        printf("Unario negativo aplicado: %g\n", eval_ast((yyval.node)));/*TODO REMOVE*/ }
#line 1419 "parser.tab.c"
    break;

  case 24: /* expresion: PARENTHESES_OPEN expresion PARENTHESES_CLOSE  */
#line 156 "parser.y"
                                                   {
        (yyval.node) = (yyvsp[-1].node); }
#line 1426 "parser.tab.c"
    break;

  case 25: /* expresion: expresion OP_PLUS expresion  */
#line 158 "parser.y"
                                    {
        (yyval.node) = new_binop( "+" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Suma resulta en: %g\n" , eval_ast((yyval.node)) ); /*TODO REMOVE*/ }
#line 1434 "parser.tab.c"
    break;

  case 26: /* expresion: expresion OP_MINUS expresion  */
#line 161 "parser.y"
                                    {
        (yyval.node) = new_binop( "-" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Resta resulta en: %g\n" , eval_ast((yyval.node)) ); /*TODO REMOVE*/ }
#line 1442 "parser.tab.c"
    break;

  case 27: /* expresion: expresion OP_TIMES expresion  */
#line 164 "parser.y"
                                     {
        (yyval.node) = new_binop( "*" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Producto resulta en: %g\n" , eval_ast((yyval.node)) );/*TODO REMOVE*/ }
#line 1450 "parser.tab.c"
    break;

  case 28: /* expresion: expresion OP_DIVIDE expresion  */
#line 167 "parser.y"
                                      {
        if( eval_ast((yyvsp[0].node)) == 0.0 ){
            yyerror("division por 0"); /* // TODO REMOVE*/
            (yyval.node) = new_num(0.0);
        }else{
            (yyval.node) = new_binop( "/" , (yyvsp[-2].node) , (yyvsp[0].node) );
            printf( "Division resulta en: %g\n" , eval_ast((yyval.node)) ); }}
#line 1462 "parser.tab.c"
    break;

  case 29: /* expresion: expresion OP_EQUALS expresion  */
#line 174 "parser.y"
                                    {
        (yyval.node) = new_binop( "=" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Comparacion de Igualdad: %g\n" , eval_ast((yyval.node)) );}
#line 1470 "parser.tab.c"
    break;

  case 30: /* expresion: expresion OP_LESSER expresion  */
#line 177 "parser.y"
                                    {
        (yyval.node) = new_binop( "<" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Comparacion menor que: %g\n" , eval_ast((yyval.node)) );}
#line 1478 "parser.tab.c"
    break;

  case 31: /* expresion: expresion OP_GREATER expresion  */
#line 180 "parser.y"
                                        {
        (yyval.node) = new_binop( ">" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Comparacion mayor que: %g\n" , eval_ast((yyval.node)) );}
#line 1486 "parser.tab.c"
    break;

  case 32: /* expresion: expresion OP_LESSER_EQUAL expresion  */
#line 183 "parser.y"
                                            {
        (yyval.node) = new_binop( "<=" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Comparacion menor o igual que: %g\n" , eval_ast((yyval.node)) );}
#line 1494 "parser.tab.c"
    break;

  case 33: /* expresion: expresion OP_GREATER_EQUAL expresion  */
#line 186 "parser.y"
                                            {
        (yyval.node) = new_binop( ">=" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Comparacion menor o igual que: %g\n" , eval_ast((yyval.node)) );}
#line 1502 "parser.tab.c"
    break;

  case 34: /* expresion: expresion BOOL_AND expresion  */
#line 189 "parser.y"
                                    {
        (yyval.node) = new_binop( "&&" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Operacion AND: %g\n" ,eval_ast((yyval.node)) ); }
#line 1510 "parser.tab.c"
    break;

  case 35: /* expresion: expresion BOOL_OR expresion  */
#line 192 "parser.y"
                                    {
        (yyval.node) = new_binop( "||" , (yyvsp[-2].node) , (yyvsp[0].node) );
        printf( "Operacion OR: %g\n" ,eval_ast((yyval.node)) ); }
#line 1518 "parser.tab.c"
    break;

  case 36: /* expresion: function_call  */
#line 195 "parser.y"
                    {
        (yyval.node) = (yyvsp[0].node); }
#line 1525 "parser.tab.c"
    break;

  case 37: /* expresion: TYPE_ARRAY type PARENTHESES_OPEN expresion PARENTHESES_CLOSE  */
#line 197 "parser.y"
                                                                   {
        (yyval.node) = new_array((yyvsp[-3].type), (yyvsp[-1].node)); }
#line 1532 "parser.tab.c"
    break;

  case 38: /* expresion: TYPE_LIST type PARENTHESES_OPEN expresion PARENTHESES_CLOSE  */
#line 199 "parser.y"
                                                                  {
        (yyval.node) = new_list((yyvsp[-3].type), (yyvsp[-1].node));  }
#line 1539 "parser.tab.c"
    break;

  case 39: /* if_statement: expresion IF block SEQUENCE_SEPARATOR  */
#line 204 "parser.y"
                                          {
        (yyval.node) = new_if((yyvsp[-3].node), (yyvsp[-1].node), NULL, NULL);
        printf("[SIMPLE CONDITIONAL]\n");   }
#line 1547 "parser.tab.c"
    break;

  case 40: /* if_statement: expresion IF block elif_chain SEQUENCE_SEPARATOR  */
#line 207 "parser.y"
                                                       {
        (yyval.node) = new_if((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[-1].node), NULL);
        printf("[ELSE_IF CONDITIONAL]\n");  }
#line 1555 "parser.tab.c"
    break;

  case 41: /* if_statement: expresion IF block else_part SEQUENCE_SEPARATOR  */
#line 210 "parser.y"
                                                        {
        (yyval.node) = new_if((yyvsp[-4].node), (yyvsp[-2].node), NULL, (yyvsp[-1].node));
        printf("[ELSE CONDICIONAL]\n"); }
#line 1563 "parser.tab.c"
    break;

  case 42: /* if_statement: expresion IF block elif_chain else_part SEQUENCE_SEPARATOR  */
#line 213 "parser.y"
                                                                   {
        (yyval.node) = new_if((yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node));
        printf("[ELSE_IF & ELSE CONDICIONAL ]\n");  }
#line 1571 "parser.tab.c"
    break;

  case 43: /* elif_chain: ELSE_IF expresion IF block  */
#line 219 "parser.y"
                                {
        (yyval.node) = new_elseif((yyvsp[-2].node), (yyvsp[0].node), NULL);
        printf("[LAST ELSE_IF-BRANCH]\n");  }
#line 1579 "parser.tab.c"
    break;

  case 44: /* elif_chain: ELSE_IF expresion IF block elif_chain  */
#line 222 "parser.y"
                                            {
        (yyval.node) = new_elseif((yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));
        printf("[ELSE_IF BRANCH]\n");   }
#line 1587 "parser.tab.c"
    break;

  case 45: /* else_part: ELSE block  */
#line 228 "parser.y"
                {
        (yyval.node) = (yyvsp[0].node);
        printf("[ELSE BRANCH]\n");
    }
#line 1596 "parser.tab.c"
    break;

  case 46: /* loop_expr: BEGIN_LOOP statement_list END_LOOP SEQUENCE_SEPARATOR  */
#line 235 "parser.y"
                                                            {
        (yyval.node) = new_loop((yyvsp[-2].node));
        printf("El loop se asigno correctamente %g",eval_ast((yyval.node)));  }
#line 1604 "parser.tab.c"
    break;

  case 47: /* type: TYPE_INTEGER  */
#line 241 "parser.y"
                    {
        (yyval.type) = new_type( T_integer , NULL );
        printf("[TYPE - INTEGER - #]\n");}
#line 1612 "parser.tab.c"
    break;

  case 48: /* type: TYPE_FLOAT  */
#line 244 "parser.y"
                    {
        (yyval.type) = new_type( T_float , NULL );
        printf("[TYPE - FLOAT - #.]\n");}
#line 1620 "parser.tab.c"
    break;

  case 49: /* type: TYPE_DOUBLE  */
#line 247 "parser.y"
                    {
        (yyval.type) = new_type( T_double , NULL );
        printf("[TYPE - DOUBLE - #..]\n");}
#line 1628 "parser.tab.c"
    break;

  case 50: /* type: TYPE_BOOLEAN  */
#line 250 "parser.y"
                    { 
        (yyval.type) = new_type( T_boolean , NULL);
        printf("[TYPE - BOOLEAN - ?']\n");}
#line 1636 "parser.tab.c"
    break;

  case 51: /* type: TYPE_STRING  */
#line 253 "parser.y"
                    { 
        (yyval.type) = new_type( T_string , NULL );
        printf("[TYPE - STRING - @]\n");}
#line 1644 "parser.tab.c"
    break;

  case 52: /* type: TYPE_ARRAY type  */
#line 256 "parser.y"
                        { 
        (yyval.type) = new_type( T_array , (yyvsp[0].type) );
        printf("[TYPE - ARRAY - []]\n");
        print_type((yyvsp[0].type)); }
#line 1653 "parser.tab.c"
    break;

  case 53: /* type: TYPE_LIST type  */
#line 260 "parser.y"
                        {
        (yyval.type) = new_type( T_list , (yyvsp[0].type) );
        printf("[TYPE - LIST - [>]]\n");
        print_type((yyvsp[0].type)); }
#line 1662 "parser.tab.c"
    break;

  case 54: /* var_init: %empty  */
#line 269 "parser.y"
                   { (yyval.node) = NULL; }
#line 1668 "parser.tab.c"
    break;

  case 55: /* var_init: ASSIGN expresion  */
#line 270 "parser.y"
                        {
        /* asignación normal */
        (yyval.node) = (yyvsp[0].node); }
#line 1676 "parser.tab.c"
    break;

  case 56: /* var_init: ASSIGN init_data_structure  */
#line 273 "parser.y"
                                 {
        /* array/lista con inicialización */
        (yyval.node) = (yyvsp[0].node); }
#line 1684 "parser.tab.c"
    break;

  case 57: /* var_declaration: type ID var_init SEQUENCE_SEPARATOR  */
#line 282 "parser.y"
                                        {
        /* $1 = type, $2 = ID, $3 = var_init (puede ser NULL) */
        (yyval.node) = new_declaration((yyvsp[-2].id), (yyvsp[-3].type), (yyvsp[-1].node));

        /* Optional: depuración */
        if ((yyvsp[-1].node) == NULL) {
            printf("[DECLARATION - NO ASSIGN]\n");
        } else {
            printf("[DECLARATION - WITH ASSIGN]\n");
        }
    }
#line 1700 "parser.tab.c"
    break;

  case 58: /* init_data_structure: BEGIN_SEQUENCE list_item END_SEQUENCE  */
#line 298 "parser.y"
                                          {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1708 "parser.tab.c"
    break;

  case 59: /* list_item: expresion  */
#line 303 "parser.y"
              {
        (yyval.node) = new_init_data_structure();
        init_data_structure_add( (yyval.node) , (yyvsp[0].node) ); }
#line 1716 "parser.tab.c"
    break;

  case 60: /* list_item: list_item LIST_SEPARATOR expresion  */
#line 306 "parser.y"
                                        {
        init_data_structure_add( (yyvsp[-2].node) , (yyvsp[0].node) );
        (yyval.node) = (yyvsp[-2].node);
    }
#line 1725 "parser.tab.c"
    break;

  case 61: /* param_decl: type ID  */
#line 312 "parser.y"
           { (yyval.node) = new_declaration((yyvsp[0].id), (yyvsp[-1].type), NULL);}
#line 1731 "parser.tab.c"
    break;

  case 62: /* param_list: param_decl  */
#line 316 "parser.y"
                { 
        (yyval.node) = new_sequence();
        sequence_add((yyval.node), (yyvsp[0].node)); }
#line 1739 "parser.tab.c"
    break;

  case 63: /* param_list: param_list LIST_SEPARATOR param_decl  */
#line 319 "parser.y"
                                           {
        sequence_add((yyvsp[-2].node), (yyvsp[0].node));
        (yyval.node) = (yyvsp[-2].node); }
#line 1747 "parser.tab.c"
    break;

  case 64: /* optional_param_list: %empty  */
#line 325 "parser.y"
                         { (yyval.node) = new_sequence(); }
#line 1753 "parser.tab.c"
    break;

  case 65: /* optional_param_list: param_list  */
#line 326 "parser.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1759 "parser.tab.c"
    break;

  case 66: /* fun_declaration: type RETURN_TYPE ID PARENTHESES_OPEN optional_param_list PARENTHESES_CLOSE block SEQUENCE_SEPARATOR  */
#line 330 "parser.y"
                                                                                                       {
        /* nombreFuncion ( args ) -> type { ... }; */
        int param_count = sequence_length((yyvsp[-3].node));
        ASTNode **params_array = sequence_to_array((yyvsp[-3].node));
        (yyval.node) = new_function_declaration((yyvsp[-5].id), params_array, param_count, (yyvsp[-7].type), (yyvsp[-1].node));
        printf("[FUNCTION DECLARATION]");
        print_type( (yyvsp[-7].type) );
        printf("\n");
    }
#line 1773 "parser.tab.c"
    break;

  case 67: /* arg_list: expresion  */
#line 342 "parser.y"
                {
        (yyval.node) = new_sequence();
        sequence_add((yyval.node), (yyvsp[0].node));   }
#line 1781 "parser.tab.c"
    break;

  case 68: /* arg_list: arg_list LIST_SEPARATOR expresion  */
#line 345 "parser.y"
                                        {
        sequence_add((yyvsp[-2].node), (yyvsp[0].node));
        (yyval.node) = (yyvsp[-2].node);    }
#line 1789 "parser.tab.c"
    break;

  case 69: /* optional_arg_list: %empty  */
#line 351 "parser.y"
                 { (yyval.node) = new_sequence(); }
#line 1795 "parser.tab.c"
    break;

  case 70: /* optional_arg_list: arg_list  */
#line 352 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1801 "parser.tab.c"
    break;

  case 71: /* function_call: ID PARENTHESES_OPEN optional_arg_list PARENTHESES_CLOSE  */
#line 356 "parser.y"
                                                             {
        /* nombreFuncion ( args ) */
        int arg_count = sequence_length((yyvsp[-1].node)); 
        ASTNode **args_array = sequence_to_array((yyvsp[-1].node));
        (yyval.node) = new_function_call((yyvsp[-3].id), args_array, arg_count);  }
#line 1811 "parser.tab.c"
    break;


#line 1815 "parser.tab.c"

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

#line 364 "parser.y"


/* ============================
   SECCIÓN 4: C epilogue (main opcional)
   ============================ */

int main(int argc, char** argv){
    printf("Parser AST - inicia. Introduce sentencias terminadas en ';'\n");
    yyparse();
    return 0;
}
