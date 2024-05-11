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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/client/syntax.y"

    #include <stdlib.h>
    #include <stdio.h>

    #include "lex.h"

#line 78 "src/client/syntax.c"

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

#include "syntax.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LBRACE = 3,                     /* LBRACE  */
  YYSYMBOL_RBRACE = 4,                     /* RBRACE  */
  YYSYMBOL_DOT = 5,                        /* DOT  */
  YYSYMBOL_COMMA = 6,                      /* COMMA  */
  YYSYMBOL_ASTERISK = 7,                   /* ASTERISK  */
  YYSYMBOL_SEMICOLON = 8,                  /* SEMICOLON  */
  YYSYMBOL_FROM = 9,                       /* FROM  */
  YYSYMBOL_IN = 10,                        /* IN  */
  YYSYMBOL_WHERE = 11,                     /* WHERE  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_INSERT = 13,                    /* INSERT  */
  YYSYMBOL_DELETE = 14,                    /* DELETE  */
  YYSYMBOL_UPDATE = 15,                    /* UPDATE  */
  YYSYMBOL_JOIN = 16,                      /* JOIN  */
  YYSYMBOL_ON = 17,                        /* ON  */
  YYSYMBOL_CREATE = 18,                    /* CREATE  */
  YYSYMBOL_VARIABLE = 19,                  /* VARIABLE  */
  YYSYMBOL_BOOL_CONSTANT = 20,             /* BOOL_CONSTANT  */
  YYSYMBOL_INTEGER_CONSTANT = 21,          /* INTEGER_CONSTANT  */
  YYSYMBOL_FLOAT_CONSTANT = 22,            /* FLOAT_CONSTANT  */
  YYSYMBOL_STRING_CONSTANT = 23,           /* STRING_CONSTANT  */
  YYSYMBOL_BOOL_TYPE = 24,                 /* BOOL_TYPE  */
  YYSYMBOL_UINT_8_TYPE = 25,               /* UINT_8_TYPE  */
  YYSYMBOL_UINT_16_TYPE = 26,              /* UINT_16_TYPE  */
  YYSYMBOL_UINT_32_TYPE = 27,              /* UINT_32_TYPE  */
  YYSYMBOL_INT_32_TYPE = 28,               /* INT_32_TYPE  */
  YYSYMBOL_FLOAT_TYPE = 29,                /* FLOAT_TYPE  */
  YYSYMBOL_STRING_TYPE = 30,               /* STRING_TYPE  */
  YYSYMBOL_ASSIGN = 31,                    /* ASSIGN  */
  YYSYMBOL_AND = 32,                       /* AND  */
  YYSYMBOL_OR = 33,                        /* OR  */
  YYSYMBOL_EQUAL = 34,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUAL = 35,                 /* NOT_EQUAL  */
  YYSYMBOL_LESS = 36,                      /* LESS  */
  YYSYMBOL_NOT_LESS = 37,                  /* NOT_LESS  */
  YYSYMBOL_GREATER = 38,                   /* GREATER  */
  YYSYMBOL_NOT_GREATER = 39,               /* NOT_GREATER  */
  YYSYMBOL_SUBSTR = 40,                    /* SUBSTR  */
  YYSYMBOL_NOT = 41,                       /* NOT  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_query = 43,                     /* query  */
  YYSYMBOL_column_ptr = 44,                /* column_ptr  */
  YYSYMBOL_projection = 45,                /* projection  */
  YYSYMBOL_constant = 46,                  /* constant  */
  YYSYMBOL_operand = 47,                   /* operand  */
  YYSYMBOL_expression = 48,                /* expression  */
  YYSYMBOL_table_data_source = 49,         /* table_data_source  */
  YYSYMBOL_data_source = 50,               /* data_source  */
  YYSYMBOL_filter = 51,                    /* filter  */
  YYSYMBOL_type = 52,                      /* type  */
  YYSYMBOL_column_def = 53,                /* column_def  */
  YYSYMBOL_columns_def_list = 54,          /* columns_def_list  */
  YYSYMBOL_assignment = 55,                /* assignment  */
  YYSYMBOL_assignments_list = 56,          /* assignments_list  */
  YYSYMBOL_constants_list = 57,            /* constants_list  */
  YYSYMBOL_data_tuple = 58,                /* data_tuple  */
  YYSYMBOL_data_tuples_list = 59,          /* data_tuples_list  */
  YYSYMBOL_select_query = 60,              /* select_query  */
  YYSYMBOL_delete_query = 61,              /* delete_query  */
  YYSYMBOL_insert_query = 62,              /* insert_query  */
  YYSYMBOL_update_query = 63,              /* update_query  */
  YYSYMBOL_create_table_query = 64,        /* create_table_query  */
  YYSYMBOL_delete_table_query = 65         /* delete_table_query  */
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
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   186

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  128

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    71,    71,    75,    79,    83,    87,    91,    98,   109,
     113,   121,   129,   137,   141,   145,   149,   156,   160,   167,
     171,   180,   189,   198,   207,   216,   225,   234,   242,   251,
     260,   268,   279,   284,   288,   300,   310,   314,   318,   322,
     326,   330,   334,   341,   352,   360,   365,   376,   387,   395,
     406,   414,   425,   433,   441,   446,   457,   466,   478,   486,
     497,   508,   517,   529,   540
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
  "\"end of file\"", "error", "\"invalid token\"", "LBRACE", "RBRACE",
  "DOT", "COMMA", "ASTERISK", "SEMICOLON", "FROM", "IN", "WHERE", "SELECT",
  "INSERT", "DELETE", "UPDATE", "JOIN", "ON", "CREATE", "VARIABLE",
  "BOOL_CONSTANT", "INTEGER_CONSTANT", "FLOAT_CONSTANT", "STRING_CONSTANT",
  "BOOL_TYPE", "UINT_8_TYPE", "UINT_16_TYPE", "UINT_32_TYPE",
  "INT_32_TYPE", "FLOAT_TYPE", "STRING_TYPE", "ASSIGN", "AND", "OR",
  "EQUAL", "NOT_EQUAL", "LESS", "NOT_LESS", "GREATER", "NOT_GREATER",
  "SUBSTR", "NOT", "$accept", "query", "column_ptr", "projection",
  "constant", "operand", "expression", "table_data_source", "data_source",
  "filter", "type", "column_def", "columns_def_list", "assignment",
  "assignments_list", "constants_list", "data_tuple", "data_tuples_list",
  "select_query", "delete_query", "insert_query", "update_query",
  "create_table_query", "delete_table_query", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-66)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      47,     9,   -15,    -4,    11,    39,   -66,   -66,   -66,   -66,
     -66,   -66,     9,    52,   -66,   106,    65,   111,     8,   -66,
       2,   123,     0,    10,   142,   143,   144,   129,   161,   -66,
       8,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   149,   -66,
      58,   -66,   -66,     0,   160,   -66,   -66,   -66,   -66,     0,
     -66,   -66,   -66,    36,    10,   -66,   -66,    71,   158,   136,
     -66,   139,   152,    10,   151,   143,     4,   -66,   140,   145,
     -66,    73,   -66,    48,   155,   -66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   143,   -66,   -66,   104,
     143,   -66,     0,   147,   163,   148,   -66,   153,   154,   169,
     -66,   -66,   -66,   -66,   -66,    56,    70,    76,    92,    96,
      15,   -31,   133,   -66,   -66,   -66,   -66,   -66,    36,   -66,
     -66,   -66,   -66,   117,   -66,   117,   -66,   -66
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     2,     3,     4,     5,
       6,     7,     0,     0,    33,     0,     0,     0,     0,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    36,    37,    38,    39,    40,    41,    42,     0,    44,
       0,    32,    31,     0,     0,    13,    14,    15,    16,     0,
      17,    18,    19,    35,     0,     9,    10,     0,     0,     0,
      48,     0,     0,     0,     0,     0,     0,    53,     0,     0,
      43,     0,    63,     0,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    58,     0,
       0,    61,     0,     0,     0,     0,    50,     0,     0,     0,
      60,    45,    46,    30,     8,    28,    29,    20,    21,    22,
      23,    24,    25,    26,    12,    11,    47,    49,    34,    57,
      59,    62,    52,     0,    54,     0,    55,    51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -66,   -66,   -23,   -49,   -65,    86,   -33,   150,   165,   -66,
     -66,   107,   156,    89,   115,   -66,    82,   116,   -66,   -66,
     -66,   -66,   -66,   -66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     5,    50,    57,    51,    52,    53,    14,    15,    27,
      38,    39,    40,    60,    61,    97,    67,    68,     6,     7,
       8,     9,    10,    11
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      56,    96,    59,    43,    16,    85,    41,    66,    83,    84,
      73,    30,    12,    54,    93,    17,    75,    55,    26,    44,
      45,    46,    47,    48,    45,    46,    47,    48,    13,    44,
      18,    56,    31,    32,    33,    34,    35,    36,    37,    19,
      56,    49,    59,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   103,    82,    83,    84,     1,     2,   127,   118,
      96,     3,    21,   115,    71,     4,    72,    59,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    86,    28,    87,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    77,
      78,    79,    80,    81,    82,    83,    84,    31,    32,    33,
      34,    35,    36,    37,    78,    79,    80,    81,    82,    83,
      84,    79,    80,    81,    82,    83,    84,    22,    23,    29,
      24,    25,    26,    44,    45,    46,    47,    48,    80,    81,
      82,    83,    84,    81,    82,    83,    84,    45,    46,    47,
      48,    63,    42,    64,    65,    90,    99,    91,   100,   101,
     114,    71,    86,    86,    90,   119,   121,   122,   124,   123,
      99,    58,    44,    13,    66,    74,    88,    89,    70,    92,
      94,   120,   125,    84,   104,   116,    62,    20,   102,   117,
      95,   126,    98,     0,     0,     0,    69
};

static const yytype_int8 yycheck[] =
{
      23,    66,    25,     3,    19,    54,     4,     3,    39,    40,
      43,     3,     3,     3,    63,    19,    49,     7,    16,    19,
      20,    21,    22,    23,    20,    21,    22,    23,    19,    19,
      19,    54,    24,    25,    26,    27,    28,    29,    30,     0,
      63,    41,    65,    76,    77,    78,    79,    80,    81,    82,
      83,    84,     4,    38,    39,    40,     9,    10,   123,    92,
     125,    14,    10,    86,     6,    18,     8,    90,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     6,    13,     8,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    33,
      34,    35,    36,    37,    38,    39,    40,    24,    25,    26,
      27,    28,    29,    30,    34,    35,    36,    37,    38,    39,
      40,    35,    36,    37,    38,    39,    40,    11,    12,     8,
      14,    15,    16,    19,    20,    21,    22,    23,    36,    37,
      38,    39,    40,    37,    38,    39,    40,    20,    21,    22,
      23,    12,    19,    14,    15,     6,     6,     8,     8,     4,
       4,     6,     6,     6,     6,     8,     8,     4,     4,     6,
       6,    19,    19,    19,     3,     5,     8,    31,    19,    17,
      19,     8,     3,    40,    19,    89,    26,    12,    71,    90,
      65,    99,    66,    -1,    -1,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    10,    14,    18,    43,    60,    61,    62,    63,
      64,    65,     3,    19,    49,    50,    19,    19,    19,     0,
      50,    10,    11,    12,    14,    15,    16,    51,    13,     8,
       3,    24,    25,    26,    27,    28,    29,    30,    52,    53,
      54,     4,    19,     3,    19,    20,    21,    22,    23,    41,
      44,    46,    47,    48,     3,     7,    44,    45,    19,    44,
      55,    56,    49,    12,    14,    15,     3,    58,    59,    54,
      19,     6,     8,    48,     5,    48,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    45,     6,     8,     8,    31,
       6,     8,    17,    45,    19,    56,    46,    57,    59,     6,
       8,     4,    53,     4,    19,    48,    48,    48,    48,    48,
      48,    48,    48,    48,     4,    44,    47,    55,    48,     8,
       8,     8,     4,     6,     4,     3,    58,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    43,    43,    43,    43,    43,    44,    45,
      45,    45,    45,    46,    46,    46,    46,    47,    47,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    49,    50,    50,    50,    51,    52,    52,    52,    52,
      52,    52,    52,    53,    54,    54,    54,    55,    56,    56,
      57,    57,    58,    59,    59,    59,    60,    60,    61,    61,
      62,    63,    63,    64,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     1,     5,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     5,     6,     5,     6,
       5,     5,     6,     4,     3
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
        yyerror (automata, text, result_node, YY_("syntax error: cannot back up")); \
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
                  Kind, Value, automata, text, result_node); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (automata);
  YY_USE (text);
  YY_USE (result_node);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, automata, text, result_node);
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
                 int yyrule, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], automata, text, result_node);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, automata, text, result_node); \
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
/* Parser data structure.  */
struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;
    /* Whether this instance has not started parsing yet.
     * If 2, it corresponds to a finished parsing.  */
    int yynew;
  };






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
  YY_USE (yyvaluep);
  YY_USE (automata);
  YY_USE (text);
  YY_USE (result_node);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}





int
yyparse (struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
  yypstate *yyps = yypstate_new ();
  if (!yyps)
    {
      yyerror (automata, text, result_node, YY_("memory exhausted"));
      return 2;
    }
  int yystatus = yypull_parse (yyps, automata, text, result_node);
  yypstate_delete (yyps);
  return yystatus;
}

int
yypull_parse (yypstate *yyps, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
  YY_ASSERT (yyps);
  int yystatus;
  do {
    YYSTYPE yylval;
    int yychar = yylex (&yylval, automata, text);
    yystatus = yypush_parse (yyps, yychar, &yylval, automata, text, result_node);
  } while (yystatus == YYPUSH_MORE);
  return yystatus;
}

#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yystacksize yyps->yystacksize

/* Initialize the parser data structure.  */
static void
yypstate_clear (yypstate *yyps)
{
  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yyssp = yyss;
  yyvsp = yyvs;

  /* Initialize the state stack, in case yypcontext_expected_tokens is
     called before the first call to yyparse. */
  *yyssp = 0;
  yyps->yynew = 1;
}

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, YYMALLOC (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yystacksize = YYINITDEPTH;
  yyss = yyssa;
  yyvs = yyvsa;
  yypstate_clear (yyps);
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (yyss != yyssa)
        YYSTACK_FREE (yyss);
#endif
      YYFREE (yyps);
    }
}



/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps,
              int yypushed_char, YYSTYPE const *yypushed_val, struct FiniteAutomata *automata, struct Text *text, struct Node **result_node)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

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

  switch (yyps->yynew)
    {
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;

    case 2:
      yypstate_clear (yyps);
      break;

    default:
      break;
    }

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
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
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
  case 2: /* query: select_query  */
#line 72 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1332 "src/client/syntax.c"
    break;

  case 3: /* query: delete_query  */
#line 76 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1340 "src/client/syntax.c"
    break;

  case 4: /* query: insert_query  */
#line 80 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1348 "src/client/syntax.c"
    break;

  case 5: /* query: update_query  */
#line 84 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1356 "src/client/syntax.c"
    break;

  case 6: /* query: create_table_query  */
#line 88 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1364 "src/client/syntax.c"
    break;

  case 7: /* query: delete_table_query  */
#line 92 "src/client/syntax.y"
    {
        *result_node = yyvsp[0];
    }
#line 1372 "src/client/syntax.c"
    break;

  case 8: /* column_ptr: VARIABLE DOT VARIABLE  */
#line 99 "src/client/syntax.y"
    { 
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_COLUMN_PTR;
        yyval->column_ptr.table_name = yyvsp[-2];
        yyval->column_ptr.column_name = yyvsp[0];
        *result_node = yyval;
    }
#line 1384 "src/client/syntax.c"
    break;

  case 9: /* projection: ASTERISK  */
#line 110 "src/client/syntax.y"
    {
        yyval = 0;
    }
#line 1392 "src/client/syntax.c"
    break;

  case 10: /* projection: column_ptr  */
#line 114 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_PROJECTION;
        yyval->projection.column_ptr = yyvsp[0];
        yyval->projection.next = 0;
        *result_node = yyval;
    }
#line 1404 "src/client/syntax.c"
    break;

  case 11: /* projection: projection COMMA column_ptr  */
#line 122 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_PROJECTION;
        yyval->projection.column_ptr = yyvsp[0];
        yyval->projection.next = yyvsp[-2];
        *result_node = yyval;
    }
#line 1416 "src/client/syntax.c"
    break;

  case 12: /* projection: LBRACE projection RBRACE  */
#line 130 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1425 "src/client/syntax.c"
    break;

  case 13: /* constant: BOOL_CONSTANT  */
#line 138 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1433 "src/client/syntax.c"
    break;

  case 14: /* constant: INTEGER_CONSTANT  */
#line 142 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1441 "src/client/syntax.c"
    break;

  case 15: /* constant: FLOAT_CONSTANT  */
#line 146 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1449 "src/client/syntax.c"
    break;

  case 16: /* constant: STRING_CONSTANT  */
#line 150 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1457 "src/client/syntax.c"
    break;

  case 17: /* operand: column_ptr  */
#line 157 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1465 "src/client/syntax.c"
    break;

  case 18: /* operand: constant  */
#line 161 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1473 "src/client/syntax.c"
    break;

  case 19: /* expression: operand  */
#line 168 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1481 "src/client/syntax.c"
    break;

  case 20: /* expression: expression EQUAL expression  */
#line 172 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_EQUAL;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1494 "src/client/syntax.c"
    break;

  case 21: /* expression: expression NOT_EQUAL expression  */
#line 181 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_NOT_EQUAL;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1507 "src/client/syntax.c"
    break;

  case 22: /* expression: expression LESS expression  */
#line 190 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_LESS;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1520 "src/client/syntax.c"
    break;

  case 23: /* expression: expression NOT_LESS expression  */
#line 199 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_NOT_LESS;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1533 "src/client/syntax.c"
    break;

  case 24: /* expression: expression GREATER expression  */
#line 208 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_GREATER;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1546 "src/client/syntax.c"
    break;

  case 25: /* expression: expression NOT_GREATER expression  */
#line 217 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_NOT_GREATER;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1559 "src/client/syntax.c"
    break;

  case 26: /* expression: expression SUBSTR expression  */
#line 226 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_SUBSTR;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1572 "src/client/syntax.c"
    break;

  case 27: /* expression: NOT expression  */
#line 235 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_UNARY_EXPRESSION;
        yyval->unary_expression.op = OP_NOT;
        yyval->unary_expression.operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1584 "src/client/syntax.c"
    break;

  case 28: /* expression: expression AND expression  */
#line 243 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_AND;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1597 "src/client/syntax.c"
    break;

  case 29: /* expression: expression OR expression  */
#line 252 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_BINARY_EXPRESSION;
        yyval->binary_expression.op = OP_OR;
        yyval->binary_expression.left_operand = yyvsp[-2];
        yyval->binary_expression.right_operand = yyvsp[0];
        *result_node = yyval;
    }
#line 1610 "src/client/syntax.c"
    break;

  case 30: /* expression: LBRACE expression RBRACE  */
#line 261 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1619 "src/client/syntax.c"
    break;

  case 31: /* table_data_source: VARIABLE IN VARIABLE  */
#line 269 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_TABLE_DATA_SOURCE;
        yyval->table_data_source.table = yyvsp[0];
        yyval->table_data_source.variable = yyvsp[-2];
        *result_node = yyval;
    }
#line 1631 "src/client/syntax.c"
    break;

  case 32: /* data_source: LBRACE data_source RBRACE  */
#line 280 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1640 "src/client/syntax.c"
    break;

  case 33: /* data_source: table_data_source  */
#line 285 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1648 "src/client/syntax.c"
    break;

  case 34: /* data_source: data_source JOIN table_data_source ON expression  */
#line 289 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_JOIN_DATA_SOURCE;
        yyval->join_data_source.left_data_source = yyvsp[-4];
        yyval->join_data_source.right_data_source = yyvsp[-2];
        yyval->join_data_source.expression = yyvsp[0];
        *result_node = yyval;
    }
#line 1661 "src/client/syntax.c"
    break;

  case 35: /* filter: WHERE expression  */
#line 301 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_FILTER;
        yyval->filter.expression = yyvsp[0];
        *result_node = yyval;
    }
#line 1672 "src/client/syntax.c"
    break;

  case 36: /* type: BOOL_TYPE  */
#line 311 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1680 "src/client/syntax.c"
    break;

  case 37: /* type: UINT_8_TYPE  */
#line 315 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1688 "src/client/syntax.c"
    break;

  case 38: /* type: UINT_16_TYPE  */
#line 319 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1696 "src/client/syntax.c"
    break;

  case 39: /* type: UINT_32_TYPE  */
#line 323 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1704 "src/client/syntax.c"
    break;

  case 40: /* type: INT_32_TYPE  */
#line 327 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1712 "src/client/syntax.c"
    break;

  case 41: /* type: FLOAT_TYPE  */
#line 331 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1720 "src/client/syntax.c"
    break;

  case 42: /* type: STRING_TYPE  */
#line 335 "src/client/syntax.y"
    {
        *result_node = yyval;
    }
#line 1728 "src/client/syntax.c"
    break;

  case 43: /* column_def: type VARIABLE  */
#line 342 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_COLUMN_DEF;
        yyval->column_def.type = yyvsp[-1];
        yyval->column_def.name = yyvsp[0];
        *result_node = yyval;
    }
#line 1740 "src/client/syntax.c"
    break;

  case 44: /* columns_def_list: column_def  */
#line 353 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_COLUMNS_DEF_LIST;
        yyval->columns_def_list.def = yyvsp[0];
        yyval->columns_def_list.next = 0;
        *result_node = yyval;
    }
#line 1752 "src/client/syntax.c"
    break;

  case 45: /* columns_def_list: LBRACE columns_def_list RBRACE  */
#line 361 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1761 "src/client/syntax.c"
    break;

  case 46: /* columns_def_list: columns_def_list COMMA column_def  */
#line 366 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_COLUMNS_DEF_LIST;
        yyval->columns_def_list.def = yyvsp[0];
        yyval->columns_def_list.next = yyvsp[-2];
        *result_node = yyval;
    }
#line 1773 "src/client/syntax.c"
    break;

  case 47: /* assignment: column_ptr ASSIGN operand  */
#line 377 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_ASSIGNMENT;
        yyval->assignment.to = yyvsp[-2];
        yyval->assignment.what = yyvsp[0];
        *result_node = yyval;
    }
#line 1785 "src/client/syntax.c"
    break;

  case 48: /* assignments_list: assignment  */
#line 388 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_ASSIGNMENTS_LIST;
        yyval->assignments_list.assignment = yyvsp[0];
        yyval->assignments_list.next = 0;
        *result_node = yyval;
    }
#line 1797 "src/client/syntax.c"
    break;

  case 49: /* assignments_list: assignments_list COMMA assignment  */
#line 396 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_ASSIGNMENTS_LIST;
        yyval->assignments_list.assignment = yyvsp[0];
        yyval->assignments_list.next = yyvsp[-2];
        *result_node = yyval;
    }
#line 1809 "src/client/syntax.c"
    break;

  case 50: /* constants_list: constant  */
#line 407 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_CONSTANTS_LIST;
        yyval->constants_list.constant = yyvsp[0];
        yyval->constants_list.next = 0;
        *result_node = yyval;
    }
#line 1821 "src/client/syntax.c"
    break;

  case 51: /* constants_list: constants_list COMMA constant  */
#line 415 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_CONSTANTS_LIST;
        yyval->constants_list.constant = yyvsp[0];
        yyval->constants_list.next = yyvsp[-2];
        *result_node = yyval;
    }
#line 1833 "src/client/syntax.c"
    break;

  case 52: /* data_tuple: LBRACE constants_list RBRACE  */
#line 426 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1842 "src/client/syntax.c"
    break;

  case 53: /* data_tuples_list: data_tuple  */
#line 434 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_DATA_TUPLES_LIST;
        yyval->data_tuples_list.data_tuple = yyvsp[0];
        yyval->data_tuples_list.next = 0;
        *result_node = yyval;
    }
#line 1854 "src/client/syntax.c"
    break;

  case 54: /* data_tuples_list: LBRACE data_tuples_list RBRACE  */
#line 442 "src/client/syntax.y"
    {
        yyval = yyvsp[-1];
        *result_node = yyval;
    }
#line 1863 "src/client/syntax.c"
    break;

  case 55: /* data_tuples_list: data_tuples_list COMMA data_tuple  */
#line 447 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_DATA_TUPLES_LIST;
        yyval->data_tuples_list.data_tuple = yyvsp[0];
        yyval->data_tuples_list.next = yyvsp[-2];
        *result_node = yyval;
    }
#line 1875 "src/client/syntax.c"
    break;

  case 56: /* select_query: FROM data_source SELECT projection SEMICOLON  */
#line 458 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_SELECT_QUERY;
        yyval->select_query.data_source = yyvsp[-3];
        yyval->select_query.filter = 0;
        yyval->select_query.projection = yyvsp[-1];
        *result_node = yyval;
    }
#line 1888 "src/client/syntax.c"
    break;

  case 57: /* select_query: FROM data_source filter SELECT projection SEMICOLON  */
#line 467 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_SELECT_QUERY;
        yyval->select_query.data_source = yyvsp[-4];
        yyval->select_query.filter = yyvsp[-3];
        yyval->select_query.projection = yyvsp[-1];
        *result_node = yyval;
    }
#line 1901 "src/client/syntax.c"
    break;

  case 58: /* delete_query: FROM data_source DELETE VARIABLE SEMICOLON  */
#line 479 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_DELETE_QUERY;
        yyval->delete_query.data_source = yyvsp[-3];
        yyval->delete_query.filter = 0;
        *result_node = yyval;
    }
#line 1913 "src/client/syntax.c"
    break;

  case 59: /* delete_query: FROM data_source filter DELETE VARIABLE SEMICOLON  */
#line 487 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_DELETE_QUERY;
        yyval->delete_query.data_source = yyvsp[-4];
        yyval->delete_query.filter = yyvsp[-3];
        *result_node = yyval;
    }
#line 1925 "src/client/syntax.c"
    break;

  case 60: /* insert_query: IN VARIABLE INSERT data_tuples_list SEMICOLON  */
#line 498 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_INSERT_QUERY;
        yyval->insert_query.table_name = yyvsp[-3];
        yyval->insert_query.data_tuples = yyvsp[-1];
        *result_node = yyval;
    }
#line 1937 "src/client/syntax.c"
    break;

  case 61: /* update_query: FROM data_source UPDATE assignments_list SEMICOLON  */
#line 509 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_UPDATE_QUERY;
        yyval->update_query.data_source = yyvsp[-3];
        yyval->update_query.filter = 0;
        yyval->update_query.assignments = yyvsp[-1];
        *result_node = yyval;
    }
#line 1950 "src/client/syntax.c"
    break;

  case 62: /* update_query: FROM data_source filter UPDATE assignments_list SEMICOLON  */
#line 518 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_UPDATE_QUERY;
        yyval->update_query.data_source = yyvsp[-4];
        yyval->update_query.filter = yyvsp[-3];
        yyval->update_query.assignments = yyvsp[-1];
        *result_node = yyval;
    }
#line 1963 "src/client/syntax.c"
    break;

  case 63: /* create_table_query: CREATE VARIABLE columns_def_list SEMICOLON  */
#line 530 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_CREATE_TABLE_QUERY;
        yyval->create_table_query.table_name = yyvsp[-2];
        yyval->create_table_query.columns_def = yyvsp[-1];
        *result_node = yyval;
    }
#line 1975 "src/client/syntax.c"
    break;

  case 64: /* delete_table_query: DELETE VARIABLE SEMICOLON  */
#line 541 "src/client/syntax.y"
    {
        yyval = malloc(sizeof(struct Node));
        yyval->type = NODE_DELETE_TABLE_QUERY;
        yyval->delete_table_query.table_name = yyvsp[-1];
        *result_node = yyval;
    }
#line 1986 "src/client/syntax.c"
    break;


#line 1990 "src/client/syntax.c"

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
      yyerror (automata, text, result_node, YY_("syntax error"));
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
                      yytoken, &yylval, automata, text, result_node);
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, automata, text, result_node);
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
  yyerror (automata, text, result_node, YY_("memory exhausted"));
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
                  yytoken, &yylval, automata, text, result_node);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, automata, text, result_node);
      YYPOPSTACK (1);
    }
  yyps->yynew = 2;
  goto yypushreturn;


/*-------------------------.
| yypushreturn -- return.  |
`-------------------------*/
yypushreturn:

  return yyresult;
}
#undef yynerrs
#undef yystate
#undef yyerrstatus
#undef yyssa
#undef yyss
#undef yyssp
#undef yyvsa
#undef yyvs
#undef yyvsp
#undef yystacksize
