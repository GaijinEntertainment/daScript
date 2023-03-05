/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         DAS_YYSTYPE
#define YYLTYPE         DAS_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das_yyparse
#define yylex           das_yylex
#define yyerror         das_yyerror
#define yydebug         das_yydebug
#define yynerrs         das_yynerrs


/* Copy the first part of user declarations.  */


    #include "daScript/misc/platform.h"
    #include "daScript/simulate/debug_info.h"
    #include "daScript/ast/compilation_errors.h"

    #ifdef _MSC_VER
    #pragma warning(disable:4262)
    #pragma warning(disable:4127)
    #pragma warning(disable:4702)
    #endif

    using namespace das;

    union DAS_YYSTYPE;
    struct DAS_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex.yy.h"

    void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS_YYSTYPE *lvalp, DAS_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das_yybegin_reader ( yyscan_t yyscanner );
    void das_yyend_reader ( yyscan_t yyscanner );

    namespace das { class Module; }
    void das_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

    #undef yyextra
    #define yyextra (*((das::DasParserState **)(scanner)))



# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ds_parser.hpp".  */
#ifndef YY_DAS_YY_DS_PARSER_HPP_INCLUDED
# define YY_DAS_YY_DS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef DAS_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define DAS_YYDEBUG 1
#  else
#   define DAS_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define DAS_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DAS_YYDEBUG */
#if DAS_YYDEBUG
extern int das_yydebug;
#endif
/* "%code requires" blocks.  */


    #include "daScript/misc/platform.h"
    #include "daScript/ast/ast.h"
    #include "daScript/ast/ast_generate.h"
    #include "daScript/ast/ast_expressions.h"

    #include "parser_state.h"

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(DAS_RELWITHDEBINFO)
        #pragma optimize( "s", on )
    #endif
#endif

    enum {
        CorM_COPY   = 0,
        CorM_MOVE   = (1<<0),
        CorM_CLONE  = (1<<1)
    };

    using namespace das;

    #include "parser_impl.h"

    LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li );
    LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie );

    struct TypePair {
        TypeDecl * firstType;
        TypeDecl * secondType;
    };



/* Token type.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
  {
    LEXER_ERROR = 258,
    DAS_STRUCT = 259,
    DAS_CLASS = 260,
    DAS_LET = 261,
    DAS_DEF = 262,
    DAS_WHILE = 263,
    DAS_IF = 264,
    DAS_STATIC_IF = 265,
    DAS_ELSE = 266,
    DAS_FOR = 267,
    DAS_CATCH = 268,
    DAS_TRUE = 269,
    DAS_FALSE = 270,
    DAS_NEWT = 271,
    DAS_TYPEINFO = 272,
    DAS_TYPE = 273,
    DAS_IN = 274,
    DAS_IS = 275,
    DAS_AS = 276,
    DAS_ELIF = 277,
    DAS_STATIC_ELIF = 278,
    DAS_ARRAY = 279,
    DAS_RETURN = 280,
    DAS_NULL = 281,
    DAS_BREAK = 282,
    DAS_TRY = 283,
    DAS_OPTIONS = 284,
    DAS_TABLE = 285,
    DAS_EXPECT = 286,
    DAS_CONST = 287,
    DAS_REQUIRE = 288,
    DAS_OPERATOR = 289,
    DAS_ENUM = 290,
    DAS_FINALLY = 291,
    DAS_DELETE = 292,
    DAS_DEREF = 293,
    DAS_TYPEDEF = 294,
    DAS_WITH = 295,
    DAS_AKA = 296,
    DAS_ASSUME = 297,
    DAS_CAST = 298,
    DAS_OVERRIDE = 299,
    DAS_ABSTRACT = 300,
    DAS_UPCAST = 301,
    DAS_ITERATOR = 302,
    DAS_VAR = 303,
    DAS_ADDR = 304,
    DAS_CONTINUE = 305,
    DAS_WHERE = 306,
    DAS_PASS = 307,
    DAS_REINTERPRET = 308,
    DAS_MODULE = 309,
    DAS_PUBLIC = 310,
    DAS_LABEL = 311,
    DAS_GOTO = 312,
    DAS_IMPLICIT = 313,
    DAS_EXPLICIT = 314,
    DAS_SHARED = 315,
    DAS_PRIVATE = 316,
    DAS_SMART_PTR = 317,
    DAS_UNSAFE = 318,
    DAS_INSCOPE = 319,
    DAS_TBOOL = 320,
    DAS_TVOID = 321,
    DAS_TSTRING = 322,
    DAS_TAUTO = 323,
    DAS_TINT = 324,
    DAS_TINT2 = 325,
    DAS_TINT3 = 326,
    DAS_TINT4 = 327,
    DAS_TUINT = 328,
    DAS_TBITFIELD = 329,
    DAS_TUINT2 = 330,
    DAS_TUINT3 = 331,
    DAS_TUINT4 = 332,
    DAS_TFLOAT = 333,
    DAS_TFLOAT2 = 334,
    DAS_TFLOAT3 = 335,
    DAS_TFLOAT4 = 336,
    DAS_TRANGE = 337,
    DAS_TURANGE = 338,
    DAS_TBLOCK = 339,
    DAS_TINT64 = 340,
    DAS_TUINT64 = 341,
    DAS_TDOUBLE = 342,
    DAS_TFUNCTION = 343,
    DAS_TLAMBDA = 344,
    DAS_TINT8 = 345,
    DAS_TUINT8 = 346,
    DAS_TINT16 = 347,
    DAS_TUINT16 = 348,
    DAS_TTUPLE = 349,
    DAS_TVARIANT = 350,
    DAS_GENERATOR = 351,
    DAS_YIELD = 352,
    DAS_SEALED = 353,
    ADDEQU = 354,
    SUBEQU = 355,
    DIVEQU = 356,
    MULEQU = 357,
    MODEQU = 358,
    ANDEQU = 359,
    OREQU = 360,
    XOREQU = 361,
    SHL = 362,
    SHR = 363,
    ADDADD = 364,
    SUBSUB = 365,
    LEEQU = 366,
    SHLEQU = 367,
    SHREQU = 368,
    GREQU = 369,
    EQUEQU = 370,
    NOTEQU = 371,
    RARROW = 372,
    LARROW = 373,
    QQ = 374,
    QDOT = 375,
    QBRA = 376,
    LPIPE = 377,
    LBPIPE = 378,
    LAPIPE = 379,
    LFPIPE = 380,
    RPIPE = 381,
    CLONEEQU = 382,
    ROTL = 383,
    ROTR = 384,
    ROTLEQU = 385,
    ROTREQU = 386,
    MAPTO = 387,
    COLCOL = 388,
    ANDAND = 389,
    OROR = 390,
    XORXOR = 391,
    ANDANDEQU = 392,
    OROREQU = 393,
    XORXOREQU = 394,
    DOTDOT = 395,
    MTAG_E = 396,
    MTAG_I = 397,
    MTAG_V = 398,
    MTAG_B = 399,
    MTAG_A = 400,
    MTAG_T = 401,
    MTAG_C = 402,
    MTAG_F = 403,
    MTAG_DOTDOTDOT = 404,
    BRABRAB = 405,
    BRACBRB = 406,
    CBRCBRB = 407,
    INTEGER = 408,
    LONG_INTEGER = 409,
    UNSIGNED_INTEGER = 410,
    UNSIGNED_LONG_INTEGER = 411,
    FLOAT = 412,
    DOUBLE = 413,
    NAME = 414,
    KEYWORD = 415,
    BEGIN_STRING = 416,
    STRING_CHARACTER = 417,
    STRING_CHARACTER_ESC = 418,
    END_STRING = 419,
    BEGIN_STRING_EXPR = 420,
    END_STRING_EXPR = 421,
    END_OF_READ = 422,
    UNARY_MINUS = 423,
    UNARY_PLUS = 424,
    PRE_INC = 425,
    PRE_DEC = 426,
    POST_INC = 427,
    POST_DEC = 428,
    DEREF = 429
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
typedef union DAS_YYSTYPE DAS_YYSTYPE;
union DAS_YYSTYPE
{


    char                            ch;
    bool                            b;
    int32_t                         i;
    uint32_t                        ui;
    int64_t                         i64;
    uint64_t                        ui64;
    double                          d;
    double                          fd;
    string *                        s;
    vector<string> *                pNameList;
    vector<VariableNameAndPosition> * pNameWithPosList;
    VariableDeclaration *           pVarDecl;
    vector<VariableDeclaration*> *  pVarDeclList;
    TypeDecl *                      pTypeDecl;
    Expression *                    pExpression;
    Type                            type;
    AnnotationArgument *            aa;
    AnnotationArgumentList *        aaList;
    AnnotationDeclaration *         fa;
    AnnotationList *                faList;
    MakeStruct *                    pMakeStruct;
    Enumeration *                   pEnum;
    Structure *                     pStructure;
    Function *                      pFuncDecl;
    CaptureEntry *                  pCapt;
    vector<CaptureEntry> *          pCaptList;
    TypePair                        aTypePair;


};
# define DAS_YYSTYPE_IS_TRIVIAL 1
# define DAS_YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined DAS_YYLTYPE && ! defined DAS_YYLTYPE_IS_DECLARED
typedef struct DAS_YYLTYPE DAS_YYLTYPE;
struct DAS_YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define DAS_YYLTYPE_IS_DECLARED 1
# define DAS_YYLTYPE_IS_TRIVIAL 1
#endif



int das_yyparse (yyscan_t scanner);

#endif /* !YY_DAS_YY_DS_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL \
             && defined DAS_YYSTYPE_IS_TRIVIAL && DAS_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11389

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  202
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  216
/* YYNRULES -- Number of rules.  */
#define YYNRULES  692
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1220

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   429

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   185,     2,   201,   196,   181,   174,     2,
     194,   195,   179,   178,   168,   177,   191,   180,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   171,   198,
     175,   169,   176,   170,   197,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   192,     2,   193,   173,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   199,   172,   200,   184,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   182,   183,   186,   187,   188,   189,   190
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   486,   486,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   503,   504,   505,   509,   510,
     514,   532,   533,   534,   535,   539,   543,   548,   557,   565,
     581,   586,   594,   594,   624,   645,   649,   650,   654,   657,
     661,   667,   676,   679,   685,   686,   690,   694,   695,   699,
     702,   708,   714,   717,   723,   724,   728,   729,   730,   739,
     740,   744,   745,   745,   751,   752,   753,   754,   755,   759,
     765,   765,   771,   777,   785,   795,   804,   804,   811,   812,
     813,   814,   815,   816,   820,   825,   833,   834,   835,   839,
     840,   841,   842,   843,   844,   845,   846,   852,   855,   861,
     862,   863,   867,   875,   888,   891,   899,   910,   921,   932,
     938,   942,   949,   950,   954,   955,   956,   960,   963,   970,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,
    1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1057,
    1079,  1080,  1081,  1085,  1091,  1091,  1108,  1112,  1123,  1124,
    1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1147,
    1152,  1158,  1164,  1175,  1175,  1175,  1186,  1220,  1223,  1229,
    1230,  1241,  1245,  1248,  1256,  1256,  1256,  1259,  1265,  1268,
    1271,  1275,  1281,  1285,  1289,  1292,  1295,  1303,  1306,  1309,
    1317,  1320,  1328,  1331,  1334,  1342,  1348,  1349,  1353,  1354,
    1358,  1364,  1364,  1364,  1367,  1367,  1367,  1372,  1372,  1372,
    1380,  1380,  1380,  1386,  1396,  1407,  1422,  1425,  1431,  1432,
    1439,  1450,  1451,  1452,  1456,  1457,  1458,  1459,  1463,  1468,
    1476,  1477,  1481,  1486,  1493,  1494,  1495,  1496,  1497,  1498,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1524,
    1525,  1526,  1527,  1531,  1538,  1550,  1555,  1565,  1569,  1576,
    1579,  1579,  1579,  1584,  1584,  1584,  1597,  1601,  1605,  1605,
    1605,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,
    1621,  1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,
    1631,  1632,  1633,  1634,  1635,  1636,  1637,  1638,  1639,  1640,
    1641,  1642,  1643,  1644,  1645,  1646,  1647,  1653,  1654,  1655,
    1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,
    1669,  1673,  1676,  1679,  1680,  1681,  1682,  1685,  1688,  1689,
    1692,  1692,  1692,  1695,  1700,  1704,  1708,  1708,  1708,  1713,
    1716,  1720,  1720,  1720,  1725,  1728,  1729,  1730,  1731,  1732,
    1733,  1734,  1735,  1736,  1737,  1738,  1743,  1747,  1748,  1749,
    1750,  1751,  1752,  1753,  1757,  1761,  1765,  1769,  1773,  1777,
    1781,  1785,  1789,  1796,  1797,  1801,  1802,  1803,  1807,  1808,
    1812,  1813,  1814,  1818,  1828,  1831,  1831,  1850,  1849,  1864,
    1863,  1876,  1885,  1891,  1896,  1906,  1907,  1911,  1914,  1923,
    1924,  1928,  1937,  1938,  1943,  1944,  1948,  1954,  1960,  1963,
    1967,  1973,  1982,  1983,  1984,  1988,  1989,  1993,  2000,  2005,
    2014,  2020,  2031,  2034,  2039,  2044,  2052,  2063,  2066,  2066,
    2086,  2087,  2091,  2092,  2093,  2097,  2100,  2100,  2119,  2122,
    2131,  2144,  2144,  2165,  2166,  2170,  2171,  2175,  2176,  2177,
    2181,  2191,  2198,  2208,  2209,  2213,  2214,  2218,  2224,  2225,
    2229,  2230,  2231,  2235,  2240,  2235,  2250,  2257,  2262,  2271,
    2277,  2288,  2289,  2290,  2291,  2292,  2293,  2294,  2295,  2296,
    2297,  2298,  2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,
    2307,  2308,  2309,  2310,  2311,  2312,  2316,  2317,  2318,  2319,
    2320,  2321,  2325,  2336,  2340,  2347,  2359,  2366,  2375,  2375,
    2375,  2388,  2392,  2399,  2400,  2401,  2402,  2403,  2417,  2423,
    2427,  2431,  2436,  2441,  2446,  2451,  2455,  2459,  2464,  2468,
    2472,  2477,  2477,  2477,  2483,  2490,  2490,  2490,  2495,  2495,
    2495,  2501,  2501,  2501,  2506,  2510,  2510,  2510,  2515,  2515,
    2515,  2524,  2528,  2528,  2528,  2533,  2533,  2533,  2542,  2546,
    2546,  2546,  2551,  2551,  2551,  2560,  2560,  2560,  2566,  2566,
    2566,  2575,  2578,  2589,  2605,  2605,  2605,  2629,  2629,  2649,
    2650,  2651,  2652,  2656,  2663,  2670,  2676,  2685,  2690,  2697,
    2698,  2702,  2708,  2715,  2723,  2730,  2738,  2750,  2753,  2759,
    2773,  2779,  2785,  2790,  2797,  2802,  2812,  2817,  2824,  2836,
    2837,  2841,  2844
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"class\"", "\"let\"", "\"def\"", "\"while\"", "\"if\"",
  "\"static_if\"", "\"else\"", "\"for\"", "\"recover\"", "\"true\"",
  "\"false\"", "\"new\"", "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"",
  "\"as\"", "\"elif\"", "\"static_elif\"", "\"array\"", "\"return\"",
  "\"null\"", "\"break\"", "\"try\"", "\"options\"", "\"table\"",
  "\"expect\"", "\"const\"", "\"require\"", "\"operator\"", "\"enum\"",
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"with\"",
  "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"", "\"abstract\"",
  "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"",
  "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"",
  "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"bool\"",
  "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"",
  "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"",
  "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"",
  "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"keyword\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner", "$@2",
  "expression_if_one_liner", "expression_if_then_else", "$@3",
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "$@4",
  "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_keyword", "expression_keyword", "$@6", "$@7",
  "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@8", "$@9", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "expression_let", "expr_cast", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16", "$@17",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@18", "$@19", "$@20", "$@21",
  "expr_field", "$@22", "$@23", "expr", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@30", "$@31", "$@32", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "single_alias", "$@35", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@36", "$@37", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@38", "$@39", "table_type_pair",
  "type_declaration_no_options", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "type_declaration", "variant_alias_declaration", "$@64", "$@65",
  "bitfield_alias_declaration", "$@66", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,   423,   424,   126,    33,   425,   426,   427,   428,
     429,    46,    91,    93,    40,    41,    36,    64,    59,   123,
     125,    35
};
# endif

#define YYPACT_NINF -1010

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1010)))

#define YYTABLE_NINF -643

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-643)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1010,    20, -1010, -1010,    14,   -82,   -78,     2, -1010,   127,
   -1010, -1010,   131, -1010, -1010, -1010, -1010, -1010,   298, -1010,
      29, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
      50, -1010,    54,    83,   114, -1010, -1010, -1010,   -78, -1010,
      37, -1010, -1010,   165, -1010, -1010, -1010,    29,   205,   226,
   -1010, -1010,   228,   159,   131,   131,   200, -1010,   356,   199,
   -1010, -1010, -1010,   166,   383,   438, -1010,   445,    21,    14,
     177,   -82,   245,   318, -1010,   421,   421, -1010,   346,   325,
     -90,   447,   343, -1010, -1010,   388, -1010,   -71,    14,   131,
     131,   131,   131, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
     393, -1010, -1010, -1010, -1010, -1010,   368, -1010, -1010, -1010,
   -1010, -1010,   398,   104, -1010, -1010, -1010, -1010,   481, -1010,
   -1010, 11050, -1010, -1010,   357, -1010, -1010, -1010,   418,   414,
   -1010, -1010,    28, -1010,   -51,   480,   356, 11230, -1010,    63,
     523, -1010,   482, -1010, -1010,   401, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010,    84, -1010,   456,   459,   460,   461, -1010,
   -1010, -1010,   443, -1010, -1010, -1010, -1010, -1010,   466, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,   467, -1010,
   -1010, -1010,   468,   470, -1010, -1010, -1010, -1010,   472,   473,
     462, -1010, -1010, -1010, -1010, -1010,  1248,   478, -1010, -1010,
     455,   501, -1010,  9790, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010,   637,   638, -1010,
     479,   475,   382, -1010, -1010,   516, -1010,   476,    14,   282,
   -1010, -1010, -1010,   104, -1010, -1010, -1010, -1010, -1010,   518,
   -1010,   259,   287,   300, -1010, -1010,  6435, -1010, -1010, -1010,
       5, -1010, -1010, -1010,    59,  3687, -1010,  1149,   -87,   508,
   -1010,   484,   524,   526, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010,   493, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,   666, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010,   495, -1010, -1010,   -38,   519, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010,   490,   -73,   520,   494, -1010,
     482,   209,   500,   654,   258, -1010, -1010, 11050, 11050, 11050,
   11050,   503,   418, 11050,   479, 11050,   479, 11050,   479, 11145,
     501, -1010, -1010,   262,   502,   525, -1010,   505,   527,   528,
     507,   531,   513, -1010,   533,  6435,  6435,   515,   517,   521,
     522,   529,   530, -1010, 10853, 10955,  6435, -1010, -1010, -1010,
   -1010, -1010, -1010,   535, -1010,  6435,  6435,  6435,   136,  6435,
    6435,  6435, -1010,   534, -1010, -1010, -1010, -1010,   -92, -1010,
   -1010, -1010, -1010,   540, -1010, -1010, -1010, -1010, -1010, -1010,
    6731, -1010,   543, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010,   532, -1010, -1010,  9215, -1010,  1248, -1010, -1010,
   11050,   -66, -1010, -1010, -1010,   553, -1010,   548, -1010,    -7,
   -1010,   133, 11050, -1010,  1714, -1010,    40, -1010,   136, -1010,
   -1010,   282,   550,  6435,   554,   555, 11050, -1010,    87,   225,
     541,   178,   331,   350, -1010,   117,   352,   519,   353,   519,
     358,   519,     0, -1010,   220,   478,   266, -1010,   552, -1010,
   -1010,   136, -1010,  6435, -1010, -1010,  6435, -1010,  6435, 11050,
     187,   187,  6435,  6435,  6435,  6435,  6435,  6435,   338,  2089,
     338,  2261,  9887, -1010,   316, -1010,   411,   187,   187,   -22,
   -1010,   187,   187,  6823,   152, -1010,  3068,   568,  6715,  6899,
    6435,  6435, -1010, -1010,  6435,  6435,  6435,  6435,   560,  6435,
      -1,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,
    3836,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,
    6435,   129,  6435, -1010,  3985, -1010, -1010,   478, -1010, -1010,
   -1010,  6435,   338,   556,   679, -1010,   251, -1010,   -55,   478,
   -1010,  6435, -1010, -1010,   338,  2770, -1010,   475,  4157,  6435,
     563, -1010,   546,   596,  4306,   240,  2919,   360,   360,  4455,
   -1010,   715,   558,   562,  6435,   743, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010,   569,   571,   572,   573, -1010,
     574, -1010, -1010,   694, -1010,   -61, -1010,   489,   -58,  6435,
   -1010, -1010,    24, -1010, -1010,  6915, -1010,   725,   646, -1010,
   -1010, -1010,  3240, -1010, -1010, 11050, -1010, -1010, -1010,   615,
   -1010,   599, -1010,   600, -1010,   602, 11050, -1010, 11145, -1010,
     501, 11050,  4627,  4799, 11050,  7007, 11050, 11050,  7099, 11050,
    7191,   -11,  7283,  7375,  7467,  7559,  7651,  7743,    15,   360,
     173,  2433,  4971,  9966,   611,   -23,   588,   614,   272,    26,
    5143,   -23,   371,  6435,  6435,   584, 11050, -1010,  6435,   420,
     623, -1010,   592,   593,   307, -1010, -1010,   629, -1010,   106,
   10251,    -2,   479,   626,   608, -1010, -1010,   628,   610, -1010,
   -1010,    67,    67,   340,   340, 10785, 10785,   612,    34,   613,
   -1010,  9307,   -60,   -60,    67,    67, 10555,   742, 10463, 10330,
    7083, 10069, 10578, 10670, 10693,   340,   340,    19,    19,    34,
      34,    34,   336,  6435,   618, -1010,   337,  6435,   789,  9399,
   -1010,   115,  7835, -1010,  6435,   649, -1010,   650, -1010, 11050,
   -1010,  3240, -1010,   821,    27,  3240, -1010,   682,  9686,   801,
    6435, 10251,   821,   648, -1010,   651,   668, 10251, -1010,  3240,
   -1010,  9686,   622, -1010, -1010,   821,   625, -1010, -1010,   821,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010,   282,   360, -1010,
    6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,  6435,
    3389,  6435,  6435,  6435,  6435,  6435,  6435,  3538, -1010,  1519,
     131, -1010,   818,   482, -1010,   667, -1010,  3240, -1010,  1380,
   -1010, -1010,   478, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010,   478, -1010, -1010, -1010,   361, -1010,   119,   669,
    7927,   363, -1010,   312,   922, -1010,  1080, -1010,   568, -1010,
   -1010, -1010, -1010, -1010,   633,  6435, -1010,  6435,  6435,   -13,
    6435,   670,   629,   639,   640,  6435,   641,  6435,  6435,   629,
     630,   642, 10251, -1010, -1010,  1232, 10148, -1010,   674,  6435,
    6435, 11050,   479,   157,   212,  5315, -1010,   672,   680,   681,
     684, -1010,   216,   519, -1010,  6435, -1010,  6435,  5472,  6435,
   -1010,   671,   660, -1010, -1010,  6435,   661, -1010,  9491,  6435,
     673, -1010,  9583, -1010, -1010,  6435, -1010, -1010,  8019, -1010,
     804,   -52, -1010,  9686, -1010,  6435, -1010,  9686,  6435,  6435,
     475, 10251, -1010, -1010, -1010, -1010, -1010,  9686, -1010, -1010,
   -1010,   377,  6435, -1010, -1010, 10251, 10251, 10251, 10251, 10251,
   10251, 10251, 10251, 10251, 10251,   360,   360, 10251, 10251, 10251,
   10251, 10251, 10251, 10251,   360,   360, 10251, -1010,   222,   450,
      32,   662, -1010, -1010,  6536, -1010, -1010, -1010, -1010, -1010,
   -1010,   269, -1010, -1010, -1010, -1010, -1010,   675,  5644,   -98,
   10251, 10251,   -23,   655, 10251,   171,   611,   686, -1010, 10251,
   -1010,   614,   -93,   -23,   687, -1010, -1010, -1010, -1010,  8111,
    8203,  1262,   519,   677,   629, 10251, -1010, -1010, -1010, -1010,
      -2,   688,   -76, 11050,  8295, 11050,  8387, -1010,   144,  8479,
   -1010,  6435, 10433,  6435, -1010,  8571,  6435, -1010, -1010, -1010,
     706,  6435,   -84, -1010,  6435,  1914,   475, -1010, -1010,  6435,
   -1010,   619, -1010, -1010, -1010, -1010,   676, -1010, -1010,    52,
   -1010, -1010,   450, -1010, -1010, -1010,  6435,   707, -1010,  6435,
    6435,  6435,  5816, -1010,   161,  6435,   689,   690,  6435,  6435,
   -1010,  6435,   685, -1010,  6435, -1010, -1010, -1010,   697, -1010,
     230, -1010, -1010,  5988, -1010, -1010,  1413, -1010,   369, -1010,
   -1010, -1010, 11050,  8663,  8755, -1010,  8847, -1010, 10251,   475,
   10251, -1010, -1010,   821, -1010,   692, -1010, -1010,   843,   338,
    8939,   701,   340,   340,   340, -1010,  9031, -1010,  6639,   691,
   -1010, 10251, 10251,  6639,   693,   340,   232, -1010,   696,  6435,
   10330, -1010, -1010,   378, -1010, -1010, -1010, -1010,   377,  2621,
   -1010, 11230, -1010, -1010, -1010, -1010,  6435, -1010,   841,   700,
   -1010,   695, -1010, -1010,   232, -1010, 10433, -1010, -1010, -1010,
   -1010,  6137,  6286, -1010, -1010, -1010, -1010, -1010, 10251,   698,
   11230,  9123,  6435,   704,   708, -1010, -1010,  6435, 10251,  6435,
   10251, -1010,   475, -1010, 10251, -1010, -1010, 10251, 10251, -1010
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,   112,     1,   266,     0,     0,     0,     0,   267,     0,
     657,   654,     0,    14,     3,    10,     9,     8,     0,     7,
     510,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   521,     0,   526,    19,    18,   510,     0,     0,
     100,   101,     0,   239,     0,     0,   102,   104,   110,     0,
      99,   539,   538,   200,   527,   540,   511,   512,     0,     0,
       0,     0,    39,     0,    45,     0,     0,    42,     0,     0,
       0,    15,     0,   655,   241,     0,   105,     0,     0,     0,
       0,     0,     0,   113,   202,   201,   204,   199,   529,   528,
       0,   542,   541,   543,   514,   513,   516,    93,    94,    91,
      92,    90,     0,     0,    89,    98,    50,    48,    44,    41,
      40,     0,   523,   525,     0,    16,    17,    20,     0,     0,
     240,   109,     0,   106,   107,   108,   111,     0,   530,     0,
     535,   507,   453,    21,    22,     0,    82,    83,    80,    81,
      79,    78,    84,     0,    43,     0,     0,     0,     0,   551,
     571,   552,   583,   553,   557,   558,   559,   560,   575,   564,
     565,   566,   567,   568,   569,   570,   572,   573,   624,   556,
     563,   574,   631,   638,   554,   561,   555,   562,     0,     0,
       0,   582,   593,   596,   594,   595,   651,   522,   524,   586,
       0,     0,   103,     0,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,   119,
     114,     0,     0,   518,   536,     0,   544,   508,     0,     0,
      23,    24,    25,     0,    96,   615,   618,   621,   611,     0,
     588,   625,   632,   639,   645,   648,     0,   601,   606,   600,
       0,   614,   610,   603,     0,     0,   605,     0,     0,     0,
     482,     0,   170,   171,   168,   122,   123,   125,   124,   126,
     127,   128,   129,   155,   156,   153,   154,   146,   157,   158,
     147,   144,   145,   169,   166,     0,   167,   159,   160,   161,
     162,   133,   134,   135,   130,   131,   132,   143,     0,   149,
     150,   148,   141,   142,   137,   136,   138,   139,   140,   121,
     120,   165,     0,   151,   152,   453,   117,   229,   205,   576,
     579,   577,   580,   578,   581,     0,     0,   533,     0,   515,
     453,     0,     0,   497,   495,   517,    85,     0,     0,     0,
       0,     0,     0,     0,   114,     0,   114,     0,   114,     0,
       0,   357,   358,     0,     0,     0,   351,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,   304,   306,   305,
     307,   308,   309,     0,    26,     0,     0,     0,     0,     0,
       0,     0,   291,   292,   355,   354,   302,   436,   352,   428,
     427,   426,   425,   112,   431,   353,   430,   429,   398,   359,
       0,   360,     0,   356,   659,   660,   661,   662,   608,   609,
     602,   604,     0,   607,   598,     0,   653,   652,   587,   658,
       0,     0,   172,   173,   164,   174,   163,     0,   115,     0,
     475,     0,     0,   203,     0,   518,     0,   531,     0,   537,
     464,     0,     0,     0,     0,     0,     0,   496,     0,     0,
       0,   591,     0,     0,   584,     0,     0,   117,     0,   117,
       0,   117,   239,   479,     0,   477,     0,   244,   248,   247,
     251,     0,   280,     0,   271,   274,     0,   277,     0,     0,
     387,   388,     0,     0,     0,     0,     0,     0,     0,   669,
       0,     0,   681,   686,     0,   233,     0,   364,   363,   403,
      32,   362,   361,     0,   293,   434,     0,   300,     0,     0,
       0,     0,   389,   390,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,     0,   585,     0,   599,   597,   481,   656,   483,
     175,     0,     0,     0,   546,   472,   486,   116,   453,   118,
     231,     0,    59,    60,     0,   254,   252,     0,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   206,     0,     0,     0,     0,   224,   219,   216,   215,
     217,   218,   230,   210,   209,     0,    67,    68,    65,   222,
      66,   223,   225,   269,   214,     0,   211,   310,     0,     0,
     519,   534,   465,   509,   454,     0,   499,   500,     0,   493,
     494,   492,     0,   616,   619,     0,   622,   612,   589,     0,
     626,     0,   633,     0,   640,     0,     0,   646,     0,   649,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,   677,   667,   669,     0,   682,     0,     0,
       0,   669,     0,     0,     0,     0,     0,    29,     0,    27,
       0,   391,     0,     0,   340,   337,   339,     0,   399,     0,
     286,     0,   114,     0,     0,   414,   413,     0,     0,   415,
     419,   365,   366,   378,   379,   376,   377,     0,   408,     0,
     396,     0,   432,   433,   367,   368,   383,   384,   385,   386,
       0,     0,   381,   382,   380,   374,   375,   370,   369,   371,
     372,   373,     0,     0,     0,   346,     0,     0,     0,     0,
     401,     0,     0,   473,     0,     0,   485,     0,   484,     0,
     487,     0,   476,     0,     0,     0,   258,     0,   255,     0,
       0,   242,     0,     0,   228,     0,     0,    53,    73,     0,
     263,   260,   292,   237,   238,     0,     0,   226,   227,     0,
      70,   213,   220,   221,   257,   262,   268,     0,     0,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   532,     0,
       0,   545,     0,   453,   498,     0,   502,     0,   506,   310,
     617,   620,   592,   623,   613,   590,   627,   629,   634,   636,
     641,   643,   478,   647,   480,   650,     0,   249,     0,     0,
       0,     0,   404,     0,     0,   405,     0,   435,   300,   437,
     438,   439,   440,   441,     0,     0,   670,     0,     0,   669,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   680,   687,   688,     0,     0,    30,    33,     0,
       0,     0,   114,     0,     0,     0,   400,     0,     0,     0,
       0,   298,     0,   117,   410,     0,   416,     0,     0,     0,
     394,     0,     0,   420,   424,     0,     0,   397,     0,     0,
       0,   347,     0,   349,   392,     0,   402,   474,     0,   548,
     549,   488,   491,   490,    74,     0,   259,   256,     0,     0,
       0,   243,    75,    76,    51,    52,   264,   261,   293,   232,
     229,    56,     0,   270,   236,   320,   321,   323,   322,   324,
     314,   315,   316,   325,   326,     0,     0,   312,   313,   327,
     328,   317,   318,   319,     0,     0,   311,   520,     0,   460,
     455,     0,   501,   504,   310,   505,   630,   637,   644,   245,
     250,     0,   283,   281,   272,   275,   278,     0,     0,     0,
     664,   663,   669,     0,   678,     0,   668,     0,   672,   679,
     684,   683,     0,   669,     0,   685,   234,    28,    31,     0,
       0,     0,   117,     0,     0,   287,   296,   297,   295,   294,
       0,     0,     0,     0,     0,     0,     0,   335,     0,     0,
     421,     0,   409,     0,   395,     0,     0,   393,   350,   547,
       0,     0,     0,   265,     0,     0,     0,    54,    55,     0,
      69,    61,   331,   332,   329,   330,   113,   461,   462,   455,
     456,   457,   460,   466,   503,   246,     0,     0,   282,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
     671,     0,     0,   675,     0,   338,   452,   341,     0,   333,
       0,   299,   301,     0,   288,   303,     0,   451,     0,   449,
     336,   446,     0,     0,     0,   445,     0,   550,   489,     0,
      77,   207,    57,     0,    62,     0,   471,   467,   458,     0,
       0,     0,   273,   276,   279,   406,     0,   444,   689,     0,
     673,   666,   665,   689,     0,   235,     0,   344,     0,     0,
     289,   411,   417,     0,   450,   448,   447,    72,    56,     0,
      71,     0,   459,   469,   463,   284,     0,   407,     0,     0,
     674,     0,   676,   342,     0,   334,   290,   412,   418,   422,
      58,   254,     0,    63,    67,    68,    65,    66,    64,     0,
       0,     0,     0,     0,     0,   345,   423,     0,   255,     0,
     260,   468,     0,   285,   690,   691,   692,   256,   261,   470
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1010, -1010, -1010, -1010, -1010,   386,   826, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010,   510,   861, -1010,   785, -1010, -1010,
     834, -1010, -1010, -1010,  -262, -1010, -1010, -1010,  -259, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010,   683, -1010, -1010,
     855,   -70, -1010, -1010,   263,   109,  -390,  -344,  -470, -1010,
   -1010, -1010, -1009, -1010, -1010,  -229, -1010,   -16, -1010, -1010,
   -1010, -1010,  -569,   -12, -1010, -1010, -1010, -1010, -1010,  -224,
    -223,  -221, -1010,  -218, -1010, -1010,   951, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
    -556, -1010, -1010,   -81, -1010,    85,  -568, -1010,  -453, -1010,
   -1010, -1010,  -903, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010,   512, -1010, -1010, -1010, -1010, -1010, -1010, -1010,  -136,
    -121, -1010,  -122, -1010, -1010, -1010, -1010, -1010,   384, -1010,
     305, -1010,  -417,   604,  -559,  -567,   327, -1010, -1010,  -448,
   -1010, -1010,   920, -1010, -1010, -1010,   514,   105, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010,  -458,  -118, -1010,   607, -1010,   620, -1010, -1010,
   -1010, -1010,  -263, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010, -1010,
   -1010, -1010, -1010, -1010, -1010, -1010, -1010,   -77, -1010, -1010,
   -1010, -1010, -1010,   616,  -685,  -496,  -666, -1010,    86,   277,
     463, -1010, -1010, -1010,  -177, -1010
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   127,    47,    14,   145,   151,   516,   404,   898,
     405,   700,    15,    16,    39,    40,    41,    77,    17,    34,
      35,   602,   603,  1069,  1070,   604,  1135,  1169,   605,   606,
     962,   607,   608,   609,   610,   611,  1064,   152,   153,    30,
      31,    32,    56,    57,    58,    59,    18,   326,   453,   230,
      19,    96,   231,    97,   137,   406,   612,   454,   613,   407,
     696,  1104,   614,   408,   615,   488,   661,  1085,   409,   616,
     617,   618,   619,   620,   621,   622,   623,   807,   624,   410,
     666,  1089,   667,  1090,   669,  1091,   411,   664,  1088,   412,
     709,  1115,   413,   911,   912,   712,   414,   415,   777,   626,
     416,   417,   706,   418,   901,  1156,   902,  1184,   419,   758,
    1058,   710,  1043,  1187,  1045,  1188,  1122,  1206,   421,   449,
    1082,  1173,  1079,   991,   632,   833,  1171,  1200,   450,   451,
     483,   484,   270,   271,   878,   575,   642,   468,   344,   345,
     237,   340,    67,   106,    21,   142,   336,    44,    78,    80,
      22,   100,   139,    23,   459,   235,   236,    65,   103,    24,
     140,   338,   576,   422,   335,   193,   194,   200,   195,   352,
     845,   470,   196,   350,   844,   347,   840,   348,   841,   349,
     843,   353,   846,   354,   996,   355,   848,   356,   997,   357,
     850,   358,   998,   359,   853,   360,   855,   485,    25,    49,
     129,    26,    48,   423,   684,   685,   686,   424,   687,   513,
     688,   425,   514,   426,  1179,   427
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   625,   328,   192,   437,   763,   239,   651,   761,   653,
     477,   655,   479,   633,   481,   691,   776,   771,   132,   883,
       2,   257,   904,   527,   569,   890,     3,   790,   679,   793,
     794,  -112,    27,    28,   875,   107,   108,   428,   679,   528,
     529,   572,    60,    60,   197,   888,   945,   258,   259,     4,
     678,     5,   689,     6,   528,   529,  1113,   538,    73,     7,
     540,   541,   808,    89,    90,    91,   766,   525,     8,    42,
     935,    33,   438,   838,     9,   935,  1080,    60,    60,    60,
      60,    36,    37,    89,   935,    91,   456,   528,   529,    66,
     447,   430,    74,   269,    10,   238,  1080,  1137,   540,   541,
    1095,   456,   526,    38,   260,  1101,   858,   447,   261,   191,
     123,   876,   238,   439,  1129,    11,   907,   768,   146,   147,
     267,   509,   511,   327,   131,   908,   774,   457,   532,   533,
    1081,   561,   562,    85,   568,   573,   538,   809,   539,   540,
     541,   542,   828,   532,   533,   543,   903,   729,    79,   192,
    1081,   538,   574,   539,   540,   541,   542,   448,   730,   262,
     543,    42,  1199,   263,    50,   868,   264,   909,   341,   561,
     562,   656,   910,    29,   109,   882,   532,   533,   110,   429,
     111,   265,   112,   767,   538,   124,   539,   540,   541,   542,
     266,  1212,    51,   543,   767,   767,    69,  1016,   558,   559,
     560,    43,   942,   638,   461,   639,   946,   528,   529,   629,
     561,   562,    12,  1013,   640,   113,   830,    75,    13,    68,
     956,    94,    69,   202,   831,   561,   562,    95,    76,   192,
     192,   192,   192,   431,   232,   192,   671,   192,   630,   192,
     964,   192,   832,   569,   556,   557,   558,   559,   560,   752,
     753,   432,   243,  1183,    70,   191,   641,   148,   561,   562,
     433,   149,   233,   150,    52,   112,   192,   192,   993,    52,
     469,   471,   472,   473,   905,   766,   476,   754,   478,   244,
     480,  1205,    71,   935,   877,    52,    45,   935,   755,   766,
      53,   766,    85,   648,   702,    53,   532,   533,  1098,   703,
     877,   906,    61,    62,   538,    63,    85,   540,   541,   542,
     936,    53,   935,   543,  1000,   649,    54,    86,    87,  1009,
     756,   757,   192,    46,    42,    55,   768,   704,   577,   935,
     116,   578,  1022,    64,   192,   191,   191,   191,   191,  1120,
     768,   191,   768,   191,   257,   191,  1096,   191,   192,  1110,
     267,   191,   133,   134,   135,   136,  1147,  1102,   779,   963,
     528,   529,  1048,   567,    82,    52,   788,    92,   913,   766,
     258,   259,   191,   191,   702,   579,   645,    69,   561,   562,
     881,   192,  1061,  1012,  1040,    83,   520,    84,  1066,  1062,
      92,    53,    93,  1023,    88,    52,   657,   267,   881,  1067,
    1068,   643,   462,   863,   864,  1033,   866,  1072,  1073,  1041,
     716,   720,   384,   385,   386,  1076,  1074,  1075,   658,   767,
     768,    53,   769,  1158,   342,   770,   465,   260,   191,   466,
    -628,   261,   467,   895,   498,  -628,    76,   487,    98,   327,
     191,   343,   659,  1042,    99,  1086,   631,   530,   531,   532,
     533,   329,  1094,  -628,   191,   330,  1099,   538,  -635,   539,
     540,   541,   542,  -635,   660,   886,   543,  1087,   544,   545,
     887,  -642,   331,   332,   333,   334,  -642,   118,  -343,   663,
     573,  -635,   262,  -343,   926,   930,   263,   191,  1004,   264,
      89,    90,    91,   101,  -642,   927,   931,   574,   -64,   102,
     104,  -343,   125,   267,   265,  1077,   105,   646,   126,   528,
     529,  1078,   705,   266,   694,   121,   695,   556,   557,   558,
     559,   560,   267,   122,   267,   267,   647,   192,   650,   652,
     267,   561,   562,   267,   654,   267,    74,   999,   192,  1003,
     192,   267,   128,   192,   944,  1162,   192,   130,   192,   192,
     267,   192,   138,   952,  1189,   198,   402,   792,  1032,   327,
     143,   144,  1108,   240,   241,   242,   959,   141,   842,   887,
     961,   891,  1174,   143,   144,   697,   698,   199,   192,   852,
      36,    37,   240,   241,   856,   119,   120,   861,   810,   811,
     812,   813,   814,   815,   816,   817,   530,   531,   532,   533,
     534,   818,   819,   535,   536,   537,   538,   820,   539,   540,
     541,   542,   625,   201,    89,   543,   821,   544,   545,   822,
     823,   234,   924,   546,   547,   548,   824,   825,   826,   549,
    1134,   245,   238,   191,   246,   247,   248,   249,  1031,   528,
     529,   250,   251,   252,   191,   253,   191,   254,   255,   191,
     267,   192,   191,   268,   191,   191,   256,   191,   827,   550,
     269,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   323,   324,   325,   327,   337,   339,   351,   257,   440,
     561,   562,   441,   442,   191,   443,   444,   445,   446,   455,
     452,   458,   941,   460,   463,   464,   491,   990,   474,   493,
     492,   496,   494,   495,   258,   259,   497,   498,   499,   502,
     515,   503,   570,   636,   637,   504,   505,   644,   711,   727,
     765,  1063,   783,   506,   507,   565,   530,   531,   532,   533,
     534,   524,    12,   535,   536,   537,   538,   564,   539,   540,
     541,   542,   571,   634,   784,   543,   662,   544,   545,   785,
     764,   796,   800,   546,   547,   548,   797,   191,   806,   549,
     798,   260,   528,   529,   639,   261,   835,   801,   420,   802,
     803,   804,   805,   640,   438,   847,   849,   435,   851,   881,
    1116,   884,   885,   192,   894,   897,   899,   900,   903,   550,
     933,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   914,   915,   916,   917,   808,   918,   919,   939,   940,
     561,   562,   929,  1114,   950,   641,   262,   953,    60,   958,
     263,   955,   954,   264,   960,   989,   992,  1008,  1001,  1015,
    1024,  1036,  1017,  1018,  1020,  1025,  1028,  1132,   265,  1037,
    1038,   528,   529,  1039,   836,  1060,  1050,   266,  1097,   530,
     531,   532,   533,   534,  1051,  1053,   535,   536,   537,   538,
    1083,   539,   540,   541,   542,  1127,  1141,  1056,   543,  1092,
     544,   545,  1109,  1157,  1136,  1172,   546,  1176,   548,  1100,
    1103,  1112,  1149,  1150,  1180,  1154,  1182,   500,   501,   191,
    1170,  1185,  1202,  1203,   114,  1204,  1211,  1215,   512,    72,
    1167,  1216,   699,   154,  1168,   117,  1190,   517,   518,   519,
    1193,   521,   522,   523,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   115,   192,   346,   192,   530,   531,
     532,   533,   534,   561,   562,   535,   536,   537,   538,   988,
     539,   540,   541,   542,  1065,  1194,  1195,   543,  1196,   544,
     545,  1197,    20,  1007,   257,   546,   547,   548,  1138,  1111,
    1139,   549,   772,   854,   486,   837,   627,    81,  1118,   628,
     489,   893,   475,  1021,   692,   635,  1181,     0,     0,   490,
     258,   259,     0,  1219,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,   192,   665,     0,     0,   668,     0,
     670,     0,   561,   562,   672,   673,   674,   675,   676,   677,
     327,   683,     0,   683,     0,     0,     0,     0,     0,     0,
       0,   191,     0,   191,     0,     0,     0,   260,     0,     0,
       0,   261,   721,   722,     0,  1163,   723,   724,   725,   726,
       0,   728,     0,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   741,   742,   743,   744,   745,   746,   747,   748,
     749,   750,   751,     0,   759,     0,     0,     0,     0,     0,
       0,     0,     0,   762,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   773,     0,     0,   263,   778,  1005,   264,
     781,   782,     0,     0,     0,     0,   787,     0,   791,     0,
     191,   795,   257,     0,   265,     0,   799,     0,     0,     0,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,   829,     0,     0,   705,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   839,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   705,   155,     0,   860,     0,     0,     0,   156,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   517,   523,   260,   157,     0,     0,   261,
       0,     0,   523,     0,     0,   892,   512,     0,     0,     0,
     896,   158,     0,     0,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,     0,     0,
     262,     0,     0,     0,   263,     0,  1006,   264,     0,     0,
       0,     0,     0,     0,   257,   928,     0,     0,     0,   932,
       0,     0,   265,     0,     0,     0,   938,     0,     0,     0,
     257,   266,    52,   943,     0,     0,     0,   947,     0,     0,
     258,   259,   951,     0,   257,   190,     0,     0,     0,     0,
       0,   957,     0,     0,     0,     0,   258,   259,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   259,   965,   966,   967,   968,   969,   970,   971,   972,
     973,   974,   977,   978,   979,   980,   981,   982,   983,   986,
       0,     0,     0,     0,     0,     0,     0,   260,     0,   994,
     436,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,  1010,
    1011,     0,  1014,     0,     0,     0,     0,  1019,     0,   683,
     528,   529,   262,     0,     0,     0,   263,     0,  1026,   264,
       0,  1029,  1030,     0,     0,     0,     0,  1035,   262,     0,
       0,     0,   263,     0,   265,   264,     0,  1044,     0,  1046,
       0,  1049,   262,   266,     0,     0,   263,  1052,  1107,   264,
     265,  1055,     0,     0,     0,   257,     0,  1035,     0,   266,
       0,     0,     0,     0,   265,     0,     0,     0,     0,     0,
     977,   986,     0,   266,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,  1071,     0,     0,     0,     0,   810,
     811,   812,   813,   814,   815,   816,   817,   530,   531,   532,
     533,   534,   818,   819,   535,   536,   537,   538,   948,   539,
     540,   541,   542,     0,     0,     0,   543,   821,   544,   545,
     822,   823,     0,     0,   546,   547,   548,   824,   825,   826,
     549,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,   528,
     529,     0,     0,     0,     0,     0,     0,     0,     0,   949,
     550,     0,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,     0,  1123,     0,  1124,     0,     0,  1126,     0,
       0,   561,   562,  1128,     0,     0,  1130,   627,   995,     0,
       0,  1133,     0,   262,     0,     0,     0,   263,     0,  1161,
     264,     0,     0,     0,     0,     0,     0,     0,  1140,     0,
       0,  1142,  1143,  1144,  1146,   265,     0,  1148,     0,     0,
    1151,  1152,     0,  1153,   266,     0,  1155,     0,     0,     0,
       0,     0,     0,     0,     0,  1160,   530,   531,   532,   533,
     534,     0,     0,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1198,     0,     0,     0,     0,     0,     0,  1201,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,  1208,  1210,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1214,   580,     0,   987,     0,  1217,
       3,  1218,   581,   582,   583,     0,   584,     0,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,   585,
     366,   586,   587,     0,     0,     0,     0,     0,     0,     0,
       0,   588,   367,     0,   589,     0,   590,   368,     0,     0,
     369,     0,     8,   370,   591,     0,   592,   371,     0,     0,
     593,   594,     0,     0,     0,     0,     0,   595,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,   596,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   597,   598,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   599,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
     402,   403,   600,   327,   601,   580,     0,     0,     0,     0,
       3,     0,   581,   582,   583,     0,   584,     0,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,   585,
     366,   586,   587,     0,     0,     0,     0,     0,     0,     0,
       0,   588,   367,     0,   589,     0,   590,   368,     0,     0,
     369,     0,     8,   370,   591,     0,   592,   371,     0,     0,
     593,   594,     0,     0,     0,     0,     0,   595,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,   596,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   597,   598,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   599,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
       0,     0,     0,   361,   362,   363,   364,   365,   401,     0,
     402,   403,   600,   327,  1131,   366,     0,     0,     0,     0,
       0,   257,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,   368,     0,     0,   369,     0,     0,   370,     0,
     679,     0,   371,     0,     0,     0,     0,   258,   259,     0,
       0,     0,   372,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   376,
       0,     0,     0,     0,   260,     0,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     377,   378,   379,   380,   381,     0,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   680,   393,
     394,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,   263,     0,     0,   681,   396,   397,     0,
     398,     0,     0,   399,   400,   361,   362,   363,   364,   365,
       0,   265,     0,   682,     0,   402,   403,   366,   327,     0,
     266,     0,     0,   257,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,   368,     0,     0,   369,     0,     0,
     370,     0,     0,     0,   371,     0,     0,     0,     0,   258,
     259,     0,     0,     0,   372,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,   260,     0,     0,     0,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     680,   393,   394,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,   263,     0,     0,   681,   396,
     397,     0,   398,     0,     0,   399,   400,   361,   362,   363,
     364,   365,     0,   265,     0,   690,     0,   402,   403,   366,
     327,     0,   266,     0,     0,   430,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   368,     0,     0,   369,
       0,     0,   370,     0,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,   393,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
     395,   396,   397,     0,   398,     0,     0,   399,   400,     0,
       0,     0,     0,     0,     0,   432,     0,   401,     0,   402,
     403,     0,   327,     0,   433,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,  1191,   366,   586,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,   368,     0,     0,   369,     0,     0,
     370,   591,     0,     0,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   372,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,  1192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,   393,   394,     0,   361,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,   366,     0,   395,   396,
     397,     0,   398,     0,     0,   399,   400,     0,   367,     0,
       0,     0,     0,   368,     0,   401,   369,   402,   403,   370,
     327,     0,     0,   371,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,   775,     0,
       0,     0,     0,     0,   597,   598,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
     393,   394,     0,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,   366,     0,   395,   396,   397,
       0,   398,     0,     0,   399,   400,     0,   367,     0,     0,
       0,     0,   368,     0,   401,   369,   402,   403,   370,   327,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   376,
       0,     0,     0,     0,     0,     0,     0,   789,     0,     0,
       0,     0,     0,   597,   598,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     377,   378,   379,   380,   381,     0,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,    53,   393,
     394,     0,   361,   362,   363,   364,   365,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   395,   396,   397,     0,
     398,     0,     0,   399,   400,     0,   367,     0,     0,     0,
       0,   368,     0,   401,   369,   402,   403,   370,   327,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,   361,   362,   363,   364,   365,     0,
     707,     0,   401,   708,   402,   403,   366,   327,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,   368,     0,     0,   369,     0,     0,   370,
       0,     0,     0,   371,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   597,   598,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
     393,   394,     0,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,   366,     0,   395,   396,   397,
       0,   398,     0,     0,   399,   400,     0,   367,     0,     0,
       0,     0,   368,     0,   401,   369,   402,   403,   370,   327,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   975,   976,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     377,   378,   379,   380,   381,     0,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,    53,   393,
     394,     0,   361,   362,   363,   364,   365,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   395,   396,   397,     0,
     398,     0,     0,   399,   400,     0,   367,     0,     0,     0,
       0,   368,     0,   401,   369,   402,   403,   370,   327,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   984,   985,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,     0,   367,     0,     0,     0,     0,
     368,     0,   401,   369,   402,   403,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,   740,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
     434,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,   361,   362,   363,   364,   365,     0,     0,     0,   401,
     760,   402,   403,   366,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     368,     0,     0,   369,     0,     0,   370,     0,     0,     0,
     371,     0,     0,     0,     0,     0,   780,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
       0,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,   786,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     515,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,   361,   362,   363,   364,   365,     0,     0,     0,   401,
       0,   402,   403,   366,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     368,     0,     0,   369,     0,     0,   370,     0,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,   361,   362,   363,   364,   365,     0,     0,
       0,   401,   857,   402,   403,   366,   327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,   368,     0,     0,   369,     0,     0,   370,     0,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     377,   378,   379,   380,   381,     0,   382,     0,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,    53,   393,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   859,     0,   395,   396,   397,     0,
     398,     0,     0,   399,   400,   361,   362,   363,   364,   365,
       0,     0,     0,   401,     0,   402,   403,   366,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,   368,     0,     0,   369,     0,     0,
     370,     0,     0,     0,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   372,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,   393,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,   396,
     397,     0,   398,     0,     0,   399,   400,   361,   362,   363,
     364,   365,     0,     0,     0,   401,   879,   402,   403,   366,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   368,     0,     0,   369,
       0,     0,   370,     0,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,   393,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,   396,   397,     0,   398,     0,     0,   399,   400,   361,
     362,   363,   364,   365,     0,     0,     0,   401,   889,   402,
     403,   366,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,   368,     0,
       0,   369,     0,     0,   370,     0,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,     0,   395,   396,   397,     0,   398,     0,   366,   399,
     400,     0,     0,     0,     0,     0,     0,  1034,     0,   401,
     367,   402,   403,     0,   327,   368,     0,     0,   369,     0,
       0,   370,     0,     0,     0,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,   393,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     396,   397,     0,   398,     0,     0,   399,   400,   361,   362,
     363,   364,   365,     0,     0,     0,   401,  1047,   402,   403,
     366,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   368,     0,     0,
     369,     0,     0,   370,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   393,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
     361,   362,   363,   364,   365,     0,     0,     0,   401,  1093,
     402,   403,   366,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,   361,   362,   363,   364,   365,     0,     0,     0,
     401,  1145,   402,   403,   366,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,  1159,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,     0,   367,     0,     0,     0,     0,
     368,     0,   401,   369,   402,   403,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,  1207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
       0,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,  1209,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,   402,   403,     0,   327,   810,   811,   812,   813,   814,
     815,   816,   817,   530,   531,   532,   533,   534,   818,   819,
     535,   536,   537,   538,   948,   539,   540,   541,   542,   528,
     529,     0,   543,   821,   544,   545,   822,   823,     0,     0,
     546,   547,   548,   824,   825,   826,   549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   949,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,   713,  1084,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,     0,     0,     0,  1178,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,   714,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,   715,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,   717,     0,     0,
       0,     0,   561,   562,     0,     0,   563,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   701,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,   718,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,   719,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,   921,     0,     0,     0,     0,   561,   562,     0,     0,
     834,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,   862,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,   922,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,   923,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,   865,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,   867,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   869,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
     870,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,   871,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,   872,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,   873,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   874,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
     937,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1002,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1059,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1105,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1106,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
    1117,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1119,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1121,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1125,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1164,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
    1165,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1166,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1175,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1177,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1213,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,   566,     0,
       0,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
     920,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,   934,     0,     0,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,  1054,     0,     0,     0,     0,     0,
     530,   531,   532,   533,   534,     0,     0,   535,   536,   537,
     538,     0,   539,   540,   541,   542,   528,   529,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,  1057,     0,     0,     0,
       0,     0,     0,     0,     0,   810,   811,   812,   813,   814,
     815,   816,   817,   530,   531,   532,   533,   534,   818,   819,
     535,   536,   537,   538,   948,   539,   540,   541,   542,  -310,
     272,   273,   543,   821,   544,   545,   822,   823,     0,     0,
     546,   547,   548,   824,   825,   826,   549,   274,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   949,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   528,   529,   293,
     294,   295,     0,     0,     0,     0,     0,   296,   297,   298,
     299,   300,     0,     0,   301,   302,   303,   304,   305,   306,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,     0,     0,   319,   320,     0,     0,     0,     0,
       0,   321,   322,     0,     0,     0,   528,   529,     0,     0,
       0,     0,     0,     0,   530,   531,   532,   533,   534,     0,
       0,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,   693,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,   530,   531,   532,   533,   534,   561,   562,
     535,   536,   537,   538,     0,   539,   540,   541,   542,   528,
     529,     0,   543,     0,   544,   545,     0,     0,   880,     0,
     546,   547,   548,     0,     0,     0,   549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,     0,     0,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
     925,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,   530,   531,   532,   533,   534,
     561,   562,   535,   536,   537,   538,     0,   539,   540,   541,
     542,   528,   529,     0,   543,     0,   544,   545,     0,     0,
       0,     0,   546,   547,   548,     0,     0,     0,   549,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1027,     0,     0,     0,   550,     0,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     562,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,   529,     0,     0,     0,     0,     0,     0,   530,   531,
     532,   533,   534,     0,     0,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,   530,   531,   532,
     533,   534,   561,   562,   535,   536,   537,   538,     0,   539,
     540,   541,   542,   528,   529,     0,   543,     0,   544,   545,
       0,     0,     0,     0,   546,   547,   548,     0,     0,     0,
    -643,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     550,     0,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   562,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,   533,   534,     0,     0,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   528,   529,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,     0,     0,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,   530,   531,   532,   533,   534,
     528,   529,   535,   536,   537,   538,     0,   539,   540,   541,
     542,     0,     0,     0,   543,     0,   544,   545,     0,     0,
       0,     0,     0,   528,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
       0,   552,   553,   554,   555,   556,   557,   558,   559,   560,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     562,     0,     0,     0,     0,     0,     0,   530,   531,   532,
     533,   534,     0,     0,   535,   536,   537,   538,     0,   539,
     540,   541,   542,     0,     0,     0,   543,     0,   544,   545,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   553,   554,   555,   556,   557,   558,
     559,   560,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   562,     0,     0,   508,     0,     0,   554,   555,
     556,   557,   558,   559,   560,     0,     0,   155,     0,     0,
       0,     0,     0,   156,   561,   562,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,     0,     0,   535,
     157,     0,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,   158,     0,     0,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     554,   555,   556,   557,   558,   559,   560,   510,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,   155,
       0,     0,     0,     0,     0,   156,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,   158,     0,     0,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,     0,    53,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,     0,    53,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,   204,     0,   205,     0,   206,
     207,   208,   209,   210,   482,   211,   212,   213,   214,   215,
     216,   217,   218,   219,     0,   220,   221,   222,     0,     0,
     223,   224,   225,   226,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229
};

static const yytype_int16 yycheck[] =
{
      12,   454,   231,   121,   267,   572,   142,   477,   564,   479,
     354,   481,   356,   461,   358,   511,   585,   576,    88,   685,
       0,    32,   707,   413,   441,   691,     6,   596,    51,   597,
     598,     7,    18,    19,    19,    14,    15,    32,    51,    20,
      21,    48,    54,    55,   121,    19,    19,    58,    59,    29,
     508,    31,   510,    33,    20,    21,   132,   117,    21,    39,
     120,   121,   123,   134,   135,   136,   118,   159,    48,   159,
     168,   153,   159,   642,    54,   168,    44,    89,    90,    91,
      92,   159,   160,   134,   168,   136,   159,    20,    21,    60,
     145,    32,    55,   159,    74,   150,    44,    45,   120,   121,
     198,   159,   194,   181,   115,   198,   662,   145,   119,   121,
     200,   679,   150,   200,   198,    95,   118,   169,    14,    15,
     172,   384,   385,   199,   195,   127,   584,   200,   109,   110,
      98,   191,   192,   133,   200,   142,   117,   198,   119,   120,
     121,   122,   200,   109,   110,   126,   159,   148,    43,   267,
      98,   117,   159,   119,   120,   121,   122,   195,   159,   170,
     126,   159,  1171,   174,    33,   176,   177,   169,   238,   191,
     192,   171,   174,   159,   153,   198,   109,   110,   157,   174,
     159,   192,   161,   168,   117,    80,   119,   120,   121,   122,
     201,  1200,    61,   126,   168,   168,   168,   882,   179,   180,
     181,   199,   771,   466,   340,   118,   775,    20,    21,   169,
     191,   192,   192,   879,   127,   194,   192,   180,   198,   169,
     789,    55,   168,   195,   200,   191,   192,    61,   191,   347,
     348,   349,   350,   174,   171,   353,   499,   355,   198,   357,
     808,   359,   632,   660,   177,   178,   179,   180,   181,   120,
     121,   192,   168,  1156,   171,   267,   169,   153,   191,   192,
     201,   157,   199,   159,   133,   161,   384,   385,   837,   133,
     347,   348,   349,   350,   168,   118,   353,   148,   355,   195,
     357,  1184,   168,   168,   127,   133,   159,   168,   159,   118,
     159,   118,   133,   176,   142,   159,   109,   110,   127,   147,
     127,   195,     4,     5,   117,     7,   133,   120,   121,   122,
     195,   159,   168,   126,   195,   198,   185,    54,    55,   875,
     191,   192,   440,   196,   159,   194,   169,   175,   195,   168,
     153,   198,   888,    35,   452,   347,   348,   349,   350,   195,
     169,   353,   169,   355,    32,   357,  1012,   359,   466,  1034,
     172,   363,    89,    90,    91,    92,   195,  1023,   587,   807,
      20,    21,   918,   440,   159,   133,   595,   168,   712,   118,
      58,    59,   384,   385,   142,   452,   198,   168,   191,   192,
     168,   499,   941,   879,   168,   159,   398,   159,    11,   945,
     168,   159,   193,   889,   194,   133,   176,   172,   168,    22,
      23,   176,   193,   666,   667,   193,   669,   975,   976,   193,
     528,   529,   150,   151,   152,   193,   984,   985,   198,   168,
     169,   159,   171,   193,   142,   174,   168,   115,   440,   171,
     171,   119,   174,   696,   194,   176,   191,   175,    55,   199,
     452,   159,   176,   913,    61,   176,   458,   107,   108,   109,
     110,    69,  1008,   194,   466,    73,  1015,   117,   171,   119,
     120,   121,   122,   176,   198,   193,   126,   198,   128,   129,
     198,   171,    90,    91,    92,    93,   176,   159,   171,   491,
     142,   194,   170,   176,   148,   148,   174,   499,   176,   177,
     134,   135,   136,    55,   194,   159,   159,   159,     9,    61,
      55,   194,    55,   172,   192,    55,    61,   176,    61,    20,
      21,    61,   524,   201,   198,   169,   200,   177,   178,   179,
     180,   181,   172,   198,   172,   172,   176,   645,   176,   176,
     172,   191,   192,   172,   176,   172,    55,   176,   656,   176,
     658,   172,   199,   661,   773,   176,   664,   159,   666,   667,
     172,   669,   159,   782,   176,   198,   196,   197,   902,   199,
     162,   163,  1032,   162,   163,   164,   795,   199,   645,   198,
     799,   200,  1139,   162,   163,   164,   165,   159,   696,   656,
     159,   160,   162,   163,   661,    75,    76,   664,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,  1065,   199,   134,   126,   127,   128,   129,   130,
     131,    98,   740,   134,   135,   136,   137,   138,   139,   140,
      11,   175,   150,   645,   175,   175,   175,   194,   901,    20,
      21,   175,   175,   175,   656,   175,   658,   175,   175,   661,
     172,   769,   664,   198,   666,   667,   194,   669,   169,   170,
     159,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    34,    34,   194,   199,   159,   200,   159,    32,   171,
     191,   192,   198,   159,   696,   159,   193,    21,   193,   199,
     171,   171,   769,   199,   194,    41,   194,   833,   195,   194,
     175,   194,   175,   175,    58,    59,   175,   194,   175,   194,
     175,   194,   159,   159,   159,   194,   194,   176,   150,   159,
      41,   950,   159,   194,   194,   193,   107,   108,   109,   110,
     111,   197,   192,   114,   115,   116,   117,   194,   119,   120,
     121,   122,   194,   193,   198,   126,   194,   128,   129,   153,
     194,    36,     9,   134,   135,   136,   198,   769,    64,   140,
     198,   115,    20,    21,   118,   119,    41,   198,   256,   198,
     198,   198,   198,   127,   159,   176,   176,   265,   176,   168,
    1043,   193,   168,   901,   200,   162,   194,   194,   159,   170,
       1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   175,   194,   175,   194,   123,   194,   194,   159,   159,
     191,   192,   194,  1042,    13,   169,   170,   169,   830,   197,
     174,   153,   171,   177,   199,     7,   159,   194,   159,   159,
     200,   159,   193,   193,   193,   193,   162,  1066,   192,   159,
     159,    20,    21,   159,   198,    41,   175,   201,   193,   107,
     108,   109,   110,   111,   194,   194,   114,   115,   116,   117,
     198,   119,   120,   121,   122,   159,   159,   194,   126,   194,
     128,   129,   195,   176,   198,    32,   134,   176,   136,   193,
     193,   193,   193,   193,   193,   200,   193,   375,   376,   901,
     198,   195,    51,   193,    68,   200,   198,   193,   386,    38,
    1129,   193,   516,   118,  1133,    71,  1168,   395,   396,   397,
    1169,   399,   400,   401,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,    69,  1043,   243,  1045,   107,   108,
     109,   110,   111,   191,   192,   114,   115,   116,   117,   830,
     119,   120,   121,   122,   960,  1169,  1169,   126,  1169,   128,
     129,  1169,     1,   868,    32,   134,   135,   136,  1079,  1040,
    1082,   140,   578,   658,   360,   638,   454,    47,  1045,   455,
     363,   694,   352,   887,   511,   463,  1153,    -1,    -1,   363,
      58,    59,    -1,  1212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,  1122,   493,    -1,    -1,   496,    -1,
     498,    -1,   191,   192,   502,   503,   504,   505,   506,   507,
     199,   509,    -1,   511,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1043,    -1,  1045,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,   530,   531,    -1,  1122,   534,   535,   536,   537,
      -1,   539,    -1,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,    -1,   562,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   571,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,   581,    -1,    -1,   174,   585,   176,   177,
     588,   589,    -1,    -1,    -1,    -1,   594,    -1,   596,    -1,
    1122,   599,    32,    -1,   192,    -1,   604,    -1,    -1,    -1,
      -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,   629,    -1,    -1,  1156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   642,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1184,    24,    -1,   663,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   681,   682,   115,    47,    -1,    -1,   119,
      -1,    -1,   690,    -1,    -1,   693,   694,    -1,    -1,    -1,
     698,    62,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,   174,    -1,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   753,    -1,    -1,    -1,   757,
      -1,    -1,   192,    -1,    -1,    -1,   764,    -1,    -1,    -1,
      32,   201,   133,   771,    -1,    -1,    -1,   775,    -1,    -1,
      58,    59,   780,    -1,    32,   146,    -1,    -1,    -1,    -1,
      -1,   789,    -1,    -1,    -1,    -1,    58,    59,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,   837,
     201,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   877,
     878,    -1,   880,    -1,    -1,    -1,    -1,   885,    -1,   887,
      20,    21,   170,    -1,    -1,    -1,   174,    -1,   176,   177,
      -1,   899,   900,    -1,    -1,    -1,    -1,   905,   170,    -1,
      -1,    -1,   174,    -1,   192,   177,    -1,   915,    -1,   917,
      -1,   919,   170,   201,    -1,    -1,   174,   925,   176,   177,
     192,   929,    -1,    -1,    -1,    32,    -1,   935,    -1,   201,
      -1,    -1,    -1,    -1,   192,    -1,    -1,    -1,    -1,    -1,
     948,   949,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,   962,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,  1051,    -1,  1053,    -1,    -1,  1056,    -1,
      -1,   191,   192,  1061,    -1,    -1,  1064,  1065,   198,    -1,
      -1,  1069,    -1,   170,    -1,    -1,    -1,   174,    -1,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1086,    -1,
      -1,  1089,  1090,  1091,  1092,   192,    -1,  1095,    -1,    -1,
    1098,  1099,    -1,  1101,   201,    -1,  1104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1113,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1169,    -1,    -1,    -1,    -1,    -1,    -1,  1176,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,  1191,  1192,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,  1202,     1,    -1,   198,    -1,  1207,
       6,  1209,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,
     196,   197,   198,   199,   200,     1,    -1,    -1,    -1,    -1,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      -1,    -1,    -1,    14,    15,    16,    17,    18,   194,    -1,
     196,   197,   198,   199,   200,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      51,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,   174,    -1,    -1,   177,   178,   179,    -1,
     181,    -1,    -1,   184,   185,    14,    15,    16,    17,    18,
      -1,   192,    -1,   194,    -1,   196,   197,    26,   199,    -1,
     201,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,   174,    -1,    -1,   177,   178,
     179,    -1,   181,    -1,    -1,   184,   185,    14,    15,    16,
      17,    18,    -1,   192,    -1,   194,    -1,   196,   197,    26,
     199,    -1,   201,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
     177,   178,   179,    -1,   181,    -1,    -1,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,    -1,   194,    -1,   196,
     197,    -1,   199,    -1,   201,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   177,   178,
     179,    -1,   181,    -1,    -1,   184,   185,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   194,    46,   196,   197,    49,
     199,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   177,   178,   179,
      -1,   181,    -1,    -1,   184,   185,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   194,    46,   196,   197,    49,   199,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,
     181,    -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   194,    46,   196,   197,    49,   199,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    14,    15,    16,    17,    18,    -1,
     192,    -1,   194,   195,   196,   197,    26,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   177,   178,   179,
      -1,   181,    -1,    -1,   184,   185,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   194,    46,   196,   197,    49,   199,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,
     181,    -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   194,    46,   196,   197,    49,   199,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   194,    46,   196,   197,    49,   199,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
     193,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    14,    15,    16,    17,    18,    -1,    -1,    -1,   194,
     195,   196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    14,    15,    16,    17,    18,    -1,    -1,    -1,   194,
      -1,   196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   194,   195,   196,   197,    26,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,    -1,
     181,    -1,    -1,   184,   185,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   194,    -1,   196,   197,    26,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,    -1,   181,    -1,    -1,   184,   185,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   194,   195,   196,   197,    26,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,    -1,   181,    -1,    -1,   184,   185,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   194,   195,   196,
     197,    26,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   177,   178,   179,    -1,   181,    -1,    26,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,   194,
      38,   196,   197,    -1,   199,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,    -1,   181,    -1,    -1,   184,   185,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   194,   195,   196,   197,
      26,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   194,   195,
     196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     194,   195,   196,   197,    26,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   194,    46,   196,   197,    49,   199,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
      -1,   196,   197,    -1,   199,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    20,
      21,    -1,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    18,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,    -1,    -1,    -1,   198,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,   148,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,   148,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,   148,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    -1,    -1,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    20,    21,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      20,    21,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,    -1,
      -1,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,   132,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   191,   192,
     114,   115,   116,   117,    -1,   119,   120,   121,   122,    20,
      21,    -1,   126,    -1,   128,   129,    -1,    -1,   132,    -1,
     134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
     191,   192,   114,   115,   116,   117,    -1,   119,   120,   121,
     122,    20,    21,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,   170,    -1,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    -1,    -1,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   191,   192,   114,   115,   116,   117,    -1,   119,
     120,   121,   122,    20,    21,    -1,   126,    -1,   128,   129,
      -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    -1,    -1,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    20,    21,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    -1,    -1,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,   107,   108,   109,   110,   111,
      20,    21,   114,   115,   116,   117,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
      -1,   173,   174,   175,   176,   177,   178,   179,   180,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,    -1,    -1,   114,   115,   116,   117,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    12,    -1,    -1,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    -1,    -1,   114,
      47,    -1,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    62,    -1,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,   177,   178,   179,   180,   181,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,   159,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   159,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
      70,    71,    72,    73,   159,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   203,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    95,   192,   198,   206,   214,   215,   220,   248,   252,
     278,   346,   352,   355,   361,   400,   403,    18,    19,   159,
     241,   242,   243,   153,   221,   222,   159,   160,   181,   216,
     217,   218,   159,   199,   349,   159,   196,   205,   404,   401,
      33,    61,   133,   159,   185,   194,   244,   245,   246,   247,
     265,     4,     5,     7,    35,   359,    60,   344,   169,   168,
     171,   168,   217,    21,    55,   180,   191,   219,   350,   349,
     351,   344,   159,   159,   159,   133,   246,   246,   194,   134,
     135,   136,   168,   193,    55,    61,   253,   255,    55,    61,
     353,    55,    61,   360,    55,    61,   345,    14,    15,   153,
     157,   159,   161,   194,   208,   242,   153,   222,   159,   216,
     216,   169,   198,   200,   349,    55,    61,   204,   199,   402,
     159,   195,   243,   246,   246,   246,   246,   256,   159,   354,
     362,   199,   347,   162,   163,   207,    14,    15,   153,   157,
     159,   208,   239,   240,   219,    24,    30,    47,    62,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
     146,   265,   365,   367,   368,   370,   374,   399,   198,   159,
     369,   199,   195,    34,    65,    67,    69,    70,    71,    72,
      73,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      85,    86,    87,    90,    91,    92,    93,   109,   110,   159,
     251,   254,   171,   199,    98,   357,   358,   342,   150,   321,
     162,   163,   164,   168,   195,   175,   175,   175,   175,   194,
     175,   175,   175,   175,   175,   175,   194,    32,    58,    59,
     115,   119,   170,   174,   177,   192,   201,   172,   198,   159,
     334,   335,    20,    21,    37,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   119,   120,   121,   127,   128,   129,   130,
     131,   134,   135,   136,   137,   138,   139,   140,   170,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   184,
     185,   191,   192,    34,    34,   194,   249,   199,   257,    69,
      73,    90,    91,    92,    93,   366,   348,   159,   363,   200,
     343,   243,   142,   159,   340,   341,   239,   377,   379,   381,
     375,   159,   371,   383,   385,   387,   389,   391,   393,   395,
     397,    14,    15,    16,    17,    18,    26,    38,    43,    46,
      49,    53,    63,    74,    96,   109,   110,   141,   142,   143,
     144,   145,   147,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   160,   161,   177,   178,   179,   181,   184,
     185,   194,   196,   197,   210,   212,   257,   261,   265,   270,
     281,   288,   291,   294,   298,   299,   302,   303,   305,   310,
     313,   320,   365,   405,   409,   413,   415,   417,    32,   174,
      32,   174,   192,   201,   193,   313,   201,   374,   159,   200,
     171,   198,   159,   159,   193,    21,   193,   145,   195,   321,
     330,   331,   171,   250,   259,   199,   159,   200,   171,   356,
     199,   321,   193,   194,    41,   168,   171,   174,   339,   399,
     373,   399,   399,   399,   195,   369,   399,   249,   399,   249,
     399,   249,   159,   332,   333,   399,   335,   175,   267,   367,
     405,   194,   175,   194,   175,   175,   194,   175,   194,   175,
     313,   313,   194,   194,   194,   194,   194,   194,    12,   374,
      12,   374,   313,   411,   414,   175,   209,   313,   313,   313,
     265,   313,   313,   313,   197,   159,   194,   248,    20,    21,
     107,   108,   109,   110,   111,   114,   115,   116,   117,   119,
     120,   121,   122,   126,   128,   129,   134,   135,   136,   140,
     170,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   191,   192,   195,   194,   193,   193,   399,   200,   334,
     159,   194,    48,   142,   159,   337,   364,   195,   198,   399,
       1,     8,     9,    10,    12,    25,    27,    28,    37,    40,
      42,    50,    52,    56,    57,    63,    97,   124,   125,   160,
     198,   200,   223,   224,   227,   230,   231,   233,   234,   235,
     236,   237,   258,   260,   264,   266,   271,   272,   273,   274,
     275,   276,   277,   278,   280,   300,   301,   313,   348,   169,
     198,   265,   326,   341,   193,   313,   159,   159,   374,   118,
     127,   169,   338,   176,   176,   198,   176,   176,   176,   198,
     176,   250,   176,   250,   176,   250,   171,   176,   198,   176,
     198,   268,   194,   265,   289,   313,   282,   284,   313,   286,
     313,   374,   313,   313,   313,   313,   313,   313,   364,    51,
     159,   177,   194,   313,   406,   407,   408,   410,   412,   364,
     194,   407,   412,   132,   198,   200,   262,   164,   165,   207,
     213,   195,   142,   147,   175,   265,   304,   192,   195,   292,
     313,   150,   297,    18,   148,   159,   365,    18,   148,   159,
     365,   313,   313,   313,   313,   313,   313,   159,   313,   148,
     159,   313,   313,   313,   313,   313,   313,   313,   313,   313,
      21,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   120,   121,   148,   159,   191,   192,   311,   313,
     195,   292,   313,   337,   194,    41,   118,   168,   169,   171,
     174,   336,   330,   313,   364,   118,   264,   300,   313,   257,
      59,   313,   313,   159,   198,   153,    56,   313,   257,   118,
     264,   313,   197,   298,   298,   313,    36,   198,   198,   313,
       9,   198,   198,   198,   198,   198,    64,   279,   123,   198,
      99,   100,   101,   102,   103,   104,   105,   106,   112,   113,
     118,   127,   130,   131,   137,   138,   139,   169,   200,   313,
     192,   200,   248,   327,   195,    41,   198,   338,   264,   313,
     378,   380,   399,   382,   376,   372,   384,   176,   388,   176,
     392,   176,   399,   396,   332,   398,   399,   195,   292,   175,
     313,   399,   195,   374,   374,   195,   374,   195,   176,   195,
     195,   195,   195,   195,   195,    19,   298,   127,   336,   195,
     132,   168,   198,   408,   193,   168,   193,   198,    19,   195,
     408,   200,   313,   411,   200,   374,   313,   162,   211,   194,
     194,   306,   308,   159,   406,   168,   195,   118,   127,   169,
     174,   295,   296,   249,   175,   194,   175,   194,   194,   194,
     193,    18,   148,   159,   365,   171,   148,   159,   313,   194,
     148,   159,   313,     1,   193,   168,   195,   195,   313,   159,
     159,   399,   264,   313,   257,    19,   264,   313,   118,   169,
      13,   313,   257,   169,   171,   153,   264,   313,   197,   257,
     199,   257,   232,   341,   298,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   124,   125,   313,   313,   313,
     313,   313,   313,   313,   124,   125,   313,   198,   247,     7,
     321,   325,   159,   264,   313,   198,   386,   390,   394,   176,
     195,   159,   195,   176,   176,   176,   176,   297,   194,   292,
     313,   313,   407,   408,   313,   159,   406,   193,   193,   313,
     193,   410,   292,   407,   200,   193,   176,   166,   162,   313,
     313,   374,   249,   193,   192,   313,   159,   159,   159,   159,
     168,   193,   250,   314,   313,   316,   313,   195,   292,   313,
     175,   194,   313,   194,   193,   313,   194,   193,   312,   195,
      41,   336,   292,   257,   238,   259,    11,    22,    23,   225,
     226,   313,   298,   298,   298,   298,   193,    55,    61,   324,
      44,    98,   322,   198,   198,   269,   176,   198,   290,   283,
     285,   287,   194,   195,   292,   198,   408,   193,   127,   336,
     193,   198,   408,   193,   263,   195,   195,   176,   250,   195,
     406,   295,   193,   132,   257,   293,   374,   195,   399,   195,
     195,   195,   318,   313,   313,   195,   313,   159,   313,   198,
     313,   200,   257,   313,    11,   228,   198,    45,   322,   324,
     313,   159,   313,   313,   313,   195,   313,   195,   313,   193,
     193,   313,   313,   313,   200,   313,   307,   176,   193,   118,
     313,   176,   176,   399,   195,   195,   195,   257,   257,   229,
     198,   328,    32,   323,   337,   195,   176,   195,   198,   416,
     193,   416,   193,   304,   309,   195,   313,   315,   317,   176,
     226,    25,    97,   230,   271,   272,   273,   275,   313,   254,
     329,   313,    51,   193,   200,   304,   319,   118,   313,   118,
     313,   198,   254,   195,   313,   193,   193,   313,   313,   257
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   202,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   205,   205,
     206,   207,   207,   207,   207,   208,   209,   209,   209,   210,
     211,   211,   213,   212,   214,   215,   216,   216,   217,   217,
     217,   217,   218,   218,   219,   219,   220,   221,   221,   222,
     222,   223,   224,   224,   225,   225,   226,   226,   226,   227,
     227,   228,   229,   228,   230,   230,   230,   230,   230,   231,
     232,   231,   233,   234,   235,   236,   238,   237,   239,   239,
     239,   239,   239,   239,   240,   240,   241,   241,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   246,   246,   246,   246,   246,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   252,
     253,   253,   253,   254,   256,   255,   257,   257,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   259,
     259,   259,   260,   262,   263,   261,   264,   264,   264,   265,
     265,   265,   266,   266,   268,   269,   267,   267,   270,   270,
     270,   270,   271,   272,   273,   273,   273,   274,   274,   274,
     275,   275,   276,   276,   276,   277,   278,   278,   279,   279,
     280,   282,   283,   281,   284,   285,   281,   286,   287,   281,
     289,   290,   288,   291,   291,   291,   292,   292,   293,   293,
     293,   294,   294,   294,   295,   295,   295,   295,   296,   296,
     297,   297,   298,   298,   299,   299,   299,   299,   299,   299,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   301,
     301,   301,   301,   302,   302,   303,   303,   304,   304,   305,
     306,   307,   305,   308,   309,   305,   310,   310,   311,   312,
     310,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     314,   315,   313,   313,   313,   313,   316,   317,   313,   313,
     313,   318,   319,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   321,   321,   322,   322,   322,   323,   323,
     324,   324,   324,   325,   326,   327,   326,   328,   326,   329,
     326,   326,   330,   330,   330,   331,   331,   332,   332,   333,
     333,   334,   335,   335,   336,   336,   337,   337,   337,   337,
     337,   337,   338,   338,   338,   339,   339,   340,   340,   340,
     340,   340,   341,   341,   341,   341,   341,   342,   343,   342,
     344,   344,   345,   345,   345,   346,   347,   346,   348,   348,
     348,   350,   349,   351,   351,   352,   352,   353,   353,   353,
     354,   355,   355,   356,   356,   357,   357,   358,   359,   359,
     360,   360,   360,   362,   363,   361,   364,   364,   364,   364,
     364,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   366,   366,   366,   366,
     366,   366,   367,   368,   368,   368,   369,   369,   371,   372,
     370,   373,   373,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   375,   376,   374,   374,   377,   378,   374,   379,   380,
     374,   381,   382,   374,   374,   383,   384,   374,   385,   386,
     374,   374,   387,   388,   374,   389,   390,   374,   374,   391,
     392,   374,   393,   394,   374,   395,   396,   374,   397,   398,
     374,   399,   399,   399,   401,   402,   400,   404,   403,   405,
     405,   405,   405,   406,   406,   406,   406,   407,   407,   408,
     408,   409,   409,   409,   409,   409,   409,   410,   410,   410,
     411,   411,   412,   412,   413,   413,   414,   414,   415,   416,
     416,   417,   417
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     1,     1,     2,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     0,     0,     3,     1,     1,     1,     1,     1,     4,
       0,     6,     6,     2,     3,     3,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     0,     4,     3,     7,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     0,     0,     7,     3,     2,     2,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     4,
       5,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     1,     0,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     1,     3,     1,     2,
       3,     1,     1,     2,     2,     2,     2,     2,     1,     3,
       0,     4,     1,     6,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     5,     4,     5,     3,     4,     1,     3,
       4,     3,     4,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     4,     1,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     4,     0,     1,     1,     0,     1,
       0,     1,     1,     4,     0,     0,     4,     0,     8,     0,
       9,     5,     2,     3,     4,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     1,     1,     2,     3,     5,
       3,     3,     1,     1,     1,     0,     1,     1,     4,     3,
       3,     5,     4,     6,     5,     5,     4,     0,     0,     4,
       0,     1,     0,     1,     1,     6,     0,     6,     0,     3,
       5,     0,     4,     2,     3,     4,     2,     0,     1,     1,
       1,     7,     9,     0,     2,     0,     1,     3,     1,     1,
       0,     1,     1,     0,     0,     9,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     0,
       6,     1,     3,     1,     1,     1,     1,     4,     3,     4,
       2,     2,     3,     2,     3,     2,     2,     3,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       6,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     1,     3,     3,     0,     0,     8,     0,     7,     1,
       1,     1,     1,     3,     3,     5,     5,     1,     3,     0,
       2,     6,     5,     7,     8,     6,     8,     1,     3,     3,
       3,     1,     1,     3,     5,     5,     1,     3,     4,     0,
       3,    10,    10
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if DAS_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !DAS_YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DAS_YYDEBUG */


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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 159: /* "name"  */

      { delete ((*yyvaluep).s); }

        break;

    case 160: /* "keyword"  */

      { delete ((*yyvaluep).s); }

        break;

    case 205: /* module_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 207: /* character_sequence  */

      { delete ((*yyvaluep).s); }

        break;

    case 208: /* string_constant  */

      { delete ((*yyvaluep).s); }

        break;

    case 209: /* string_builder_body  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 210: /* string_builder  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 212: /* expr_reader  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 216: /* keyword_or_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 217: /* require_module_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 223: /* expression_label  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 224: /* expression_goto  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 226: /* expression_else  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 228: /* expression_else_one_liner  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 230: /* expression_if_one_liner  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 231: /* expression_if_then_else  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 233: /* expression_for_loop  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 234: /* expression_unsafe  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 235: /* expression_while_loop  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 236: /* expression_with  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 237: /* expression_with_alias  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 239: /* annotation_argument_value  */

      { delete ((*yyvaluep).aa); }

        break;

    case 240: /* annotation_argument_value_list  */

      { delete ((*yyvaluep).aaList); }

        break;

    case 241: /* annotation_argument_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 242: /* annotation_argument  */

      { delete ((*yyvaluep).aa); }

        break;

    case 243: /* annotation_argument_list  */

      { delete ((*yyvaluep).aaList); }

        break;

    case 244: /* annotation_declaration_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 245: /* annotation_declaration_basic  */

      { delete ((*yyvaluep).fa); }

        break;

    case 246: /* annotation_declaration  */

      { delete ((*yyvaluep).fa); }

        break;

    case 247: /* annotation_list  */

      { delete ((*yyvaluep).faList); }

        break;

    case 248: /* optional_annotation_list  */

      { delete ((*yyvaluep).faList); }

        break;

    case 249: /* optional_function_argument_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 250: /* optional_function_type  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 251: /* function_name  */

      { delete ((*yyvaluep).s); }

        break;

    case 254: /* function_declaration_header  */

      { ((*yyvaluep).pFuncDecl)->delRef(); }

        break;

    case 255: /* function_declaration  */

      { ((*yyvaluep).pFuncDecl)->delRef(); }

        break;

    case 257: /* expression_block  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 258: /* expression_any  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 259: /* expressions  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 260: /* expr_keyword  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 261: /* expression_keyword  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 264: /* expr_pipe  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 265: /* name_in_namespace  */

      { delete ((*yyvaluep).s); }

        break;

    case 266: /* expression_delete  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 267: /* new_type_declaration  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 270: /* expr_new  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 271: /* expression_break  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 272: /* expression_continue  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 273: /* expression_return_no_pipe  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 274: /* expression_return  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 275: /* expression_yield_no_pipe  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 276: /* expression_yield  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 277: /* expression_try_catch  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 280: /* expression_let  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 281: /* expr_cast  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 288: /* expr_type_decl  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 291: /* expr_type_info  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 292: /* expr_list  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 293: /* block_or_simple_block  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 295: /* capture_entry  */

      { delete ((*yyvaluep).pCapt); }

        break;

    case 296: /* capture_list  */

      { delete ((*yyvaluep).pCaptList); }

        break;

    case 297: /* optional_capture_list  */

      { delete ((*yyvaluep).pCaptList); }

        break;

    case 298: /* expr_block  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 299: /* expr_numeric_const  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 300: /* expr_assign  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 301: /* expr_assign_pipe  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 302: /* expr_named_call  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 303: /* expr_method_call  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 304: /* func_addr_name  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 305: /* func_addr_expr  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 310: /* expr_field  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 313: /* expr  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 320: /* expr_mtag  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 321: /* optional_field_annotation  */

      { delete ((*yyvaluep).aaList); }

        break;

    case 325: /* structure_variable_declaration  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 326: /* struct_variable_declaration_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 330: /* function_argument_declaration  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 331: /* function_argument_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 332: /* tuple_type  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 333: /* tuple_type_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 334: /* variant_type  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 335: /* variant_type_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 337: /* variable_declaration  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 340: /* let_variable_name_with_pos_list  */

      { delete ((*yyvaluep).pNameWithPosList); }

        break;

    case 341: /* let_variable_declaration  */

      { delete ((*yyvaluep).pVarDecl); }

        break;

    case 342: /* global_variable_declaration_list  */

      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }

        break;

    case 348: /* enum_list  */

      { if ( ((*yyvaluep).pEnum)->use_count()==1 ) delete ((*yyvaluep).pEnum); }

        break;

    case 354: /* enum_name  */

      { if ( ((*yyvaluep).pEnum)->use_count()==1 ) delete ((*yyvaluep).pEnum); }

        break;

    case 356: /* optional_structure_parent  */

      { delete ((*yyvaluep).s); }

        break;

    case 364: /* variable_name_with_pos_list  */

      { delete ((*yyvaluep).pNameWithPosList); }

        break;

    case 367: /* structure_type_declaration  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 368: /* auto_type_declaration  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 369: /* bitfield_bits  */

      { delete ((*yyvaluep).pNameList); }

        break;

    case 370: /* bitfield_type_declaration  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 373: /* table_type_pair  */

      { delete ((*yyvaluep).aTypePair).firstType; delete ((*yyvaluep).aTypePair).secondType; }

        break;

    case 374: /* type_declaration_no_options  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 399: /* type_declaration  */

      { delete ((*yyvaluep).pTypeDecl); }

        break;

    case 405: /* make_decl  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 406: /* make_struct_fields  */

      { delete ((*yyvaluep).pMakeStruct); }

        break;

    case 407: /* make_struct_dim  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 408: /* optional_block  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 409: /* make_struct_decl  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 410: /* make_tuple  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 411: /* make_map_tuple  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 412: /* make_dim  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 413: /* make_dim_decl  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 414: /* make_table  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 415: /* make_table_decl  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 416: /* array_comprehension_where  */

      { delete ((*yyvaluep).pExpression); }

        break;

    case 417: /* array_comprehension  */

      { delete ((*yyvaluep).pExpression); }

        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 15:

    { (yyval.b) = yyextra->g_Program->policies.default_module_public; }

    break;

  case 16:

    { (yyval.b) = true; }

    break;

  case 17:

    { (yyval.b) = false; }

    break;

  case 18:

    { (yyval.s) = new string("$"); }

    break;

  case 19:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 20:

    {
        yyextra->g_Program->thisModuleName = *(yyvsp[-2].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[0].b);
        yyextra->g_Program->thisModule->isModule = true;
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }

    break;

  case 21:

    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }

    break;

  case 22:

    { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }

    break;

  case 23:

    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }

    break;

  case 24:

    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }

    break;

  case 25:

    { (yyval.s) = (yyvsp[-1].s); }

    break;

  case 26:

    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }

    break;

  case 27:

    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }

    break;

  case 28:

    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }

    break;

  case 29:

    {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }

    break;

  case 30:

    {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }

    break;

  case 31:

    {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }

    break;

  case 32:

    {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das_yybegin_reader(scanner);
        }
    }

    break;

  case 33:

    {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }

    break;

  case 34:

    {
        if ( yyextra->g_Program->options.size() ) {
            yyextra->g_Program->options.insert ( yyextra->g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        auto opt = yyextra->g_Program->options.find("indenting", tInt);
        if (opt)
        {
            if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
                yyextra->das_tab_size = yyextra->das_def_tab_size;
            else
                yyextra->das_tab_size = opt->iValue ? opt->iValue : yyextra->das_def_tab_size;//0 is default
            yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
        }
        delete (yyvsp[0].aaList);
    }

    break;

  case 36:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 37:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 38:

    {
        (yyval.s) = (yyvsp[0].s);
    }

    break;

  case 39:

    {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }

    break;

  case 40:

    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }

    break;

  case 41:

    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }

    break;

  case 42:

    {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }

    break;

  case 43:

    {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }

    break;

  case 44:

    { (yyval.b) = false; }

    break;

  case 45:

    { (yyval.b) = true; }

    break;

  case 49:

    {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }

    break;

  case 50:

    {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }

    break;

  case 51:

    {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }

    break;

  case 52:

    {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }

    break;

  case 53:

    {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 54:

    { (yyval.b) = false; }

    break;

  case 55:

    { (yyval.b) = true; }

    break;

  case 56:

    { (yyval.pExpression) = nullptr; }

    break;

  case 57:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 58:

    {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }

    break;

  case 59:

    { (yyval.b) = false; }

    break;

  case 60:

    { (yyval.b) = true; }

    break;

  case 61:

    { (yyval.pExpression) = nullptr; }

    break;

  case 62:

    { yyextra->das_need_oxford_comma = true; }

    break;

  case 63:

    {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 64:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 65:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 66:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 67:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 68:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 69:

    {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }

    break;

  case 70:

    { yyextra->das_need_oxford_comma = true; }

    break;

  case 71:

    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }

    break;

  case 72:

    {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }

    break;

  case 73:

    {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }

    break;

  case 74:

    {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }

    break;

  case 75:

    {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }

    break;

  case 76:

    { yyextra->das_need_oxford_comma=true; }

    break;

  case 77:

    {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }

    break;

  case 78:

    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }

    break;

  case 79:

    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }

    break;

  case 80:

    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }

    break;

  case 81:

    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }

    break;

  case 82:

    { (yyval.aa) = new AnnotationArgument("",true); }

    break;

  case 83:

    { (yyval.aa) = new AnnotationArgument("",false); }

    break;

  case 84:

    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }

    break;

  case 85:

    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }

    break;

  case 86:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 87:

    { (yyval.s) = new string("type"); }

    break;

  case 88:

    { (yyval.s) = new string("in"); }

    break;

  case 89:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }

    break;

  case 90:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }

    break;

  case 91:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }

    break;

  case 92:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }

    break;

  case 93:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }

    break;

  case 94:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }

    break;

  case 95:

    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }

    break;

  case 96:

    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }

    break;

  case 97:

    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }

    break;

  case 98:

    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }

    break;

  case 99:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 100:

    { (yyval.s) = new string("require"); }

    break;

  case 101:

    { (yyval.s) = new string("private"); }

    break;

  case 102:

    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }

    break;

  case 103:

    {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }

    break;

  case 104:

    {
        (yyval.fa) = (yyvsp[0].fa);
    }

    break;

  case 105:

    {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }

    break;

  case 106:

    {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }

    break;

  case 107:

    {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }

    break;

  case 108:

    {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }

    break;

  case 109:

    {
        (yyval.fa) = (yyvsp[-1].fa);
    }

    break;

  case 110:

    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }

    break;

  case 111:

    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }

    break;

  case 112:

    { (yyval.faList) = nullptr; }

    break;

  case 113:

    { (yyval.faList) = (yyvsp[-1].faList); }

    break;

  case 114:

    { (yyval.pVarDeclList) = nullptr; }

    break;

  case 115:

    { (yyval.pVarDeclList) = nullptr; }

    break;

  case 116:

    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }

    break;

  case 117:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }

    break;

  case 118:

    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }

    break;

  case 119:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }

    break;

  case 120:

    { (yyval.s) = new string("!"); }

    break;

  case 121:

    { (yyval.s) = new string("~"); }

    break;

  case 122:

    { (yyval.s) = new string("+="); }

    break;

  case 123:

    { (yyval.s) = new string("-="); }

    break;

  case 124:

    { (yyval.s) = new string("*="); }

    break;

  case 125:

    { (yyval.s) = new string("/="); }

    break;

  case 126:

    { (yyval.s) = new string("%="); }

    break;

  case 127:

    { (yyval.s) = new string("&="); }

    break;

  case 128:

    { (yyval.s) = new string("|="); }

    break;

  case 129:

    { (yyval.s) = new string("^="); }

    break;

  case 130:

    { (yyval.s) = new string("&&="); }

    break;

  case 131:

    { (yyval.s) = new string("||="); }

    break;

  case 132:

    { (yyval.s) = new string("^^="); }

    break;

  case 133:

    { (yyval.s) = new string("&&"); }

    break;

  case 134:

    { (yyval.s) = new string("||"); }

    break;

  case 135:

    { (yyval.s) = new string("^^"); }

    break;

  case 136:

    { (yyval.s) = new string("+"); }

    break;

  case 137:

    { (yyval.s) = new string("-"); }

    break;

  case 138:

    { (yyval.s) = new string("*"); }

    break;

  case 139:

    { (yyval.s) = new string("/"); }

    break;

  case 140:

    { (yyval.s) = new string("%"); }

    break;

  case 141:

    { (yyval.s) = new string("<"); }

    break;

  case 142:

    { (yyval.s) = new string(">"); }

    break;

  case 143:

    { (yyval.s) = new string("interval"); }

    break;

  case 144:

    { (yyval.s) = new string("=="); }

    break;

  case 145:

    { (yyval.s) = new string("!="); }

    break;

  case 146:

    { (yyval.s) = new string("<="); }

    break;

  case 147:

    { (yyval.s) = new string(">="); }

    break;

  case 148:

    { (yyval.s) = new string("&"); }

    break;

  case 149:

    { (yyval.s) = new string("|"); }

    break;

  case 150:

    { (yyval.s) = new string("^"); }

    break;

  case 151:

    { (yyval.s) = new string("++"); }

    break;

  case 152:

    { (yyval.s) = new string("--"); }

    break;

  case 153:

    { (yyval.s) = new string("+++"); }

    break;

  case 154:

    { (yyval.s) = new string("---"); }

    break;

  case 155:

    { (yyval.s) = new string("<<"); }

    break;

  case 156:

    { (yyval.s) = new string(">>"); }

    break;

  case 157:

    { (yyval.s) = new string("<<="); }

    break;

  case 158:

    { (yyval.s) = new string(">>="); }

    break;

  case 159:

    { (yyval.s) = new string("<<<"); }

    break;

  case 160:

    { (yyval.s) = new string(">>>"); }

    break;

  case 161:

    { (yyval.s) = new string("<<<="); }

    break;

  case 162:

    { (yyval.s) = new string(">>>="); }

    break;

  case 163:

    { (yyval.s) = new string("[]"); }

    break;

  case 164:

    { (yyval.s) = new string("?[]"); }

    break;

  case 165:

    { (yyval.s) = new string("."); }

    break;

  case 166:

    { (yyval.s) = new string("?."); }

    break;

  case 167:

    { (yyval.s) = new string("clone"); }

    break;

  case 168:

    { (yyval.s) = new string("finalize"); }

    break;

  case 169:

    { (yyval.s) = new string("??"); }

    break;

  case 170:

    { (yyval.s) = new string("`is"); }

    break;

  case 171:

    { (yyval.s) = new string("`as"); }

    break;

  case 172:

    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }

    break;

  case 173:

    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }

    break;

  case 174:

    { (yyval.s) = new string("?as"); }

    break;

  case 175:

    { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }

    break;

  case 176:

    { (yyval.s) = new string("bool"); }

    break;

  case 177:

    { (yyval.s) = new string("string"); }

    break;

  case 178:

    { (yyval.s) = new string("int"); }

    break;

  case 179:

    { (yyval.s) = new string("int2"); }

    break;

  case 180:

    { (yyval.s) = new string("int3"); }

    break;

  case 181:

    { (yyval.s) = new string("int4"); }

    break;

  case 182:

    { (yyval.s) = new string("uint"); }

    break;

  case 183:

    { (yyval.s) = new string("uint2"); }

    break;

  case 184:

    { (yyval.s) = new string("uint3"); }

    break;

  case 185:

    { (yyval.s) = new string("uint4"); }

    break;

  case 186:

    { (yyval.s) = new string("float"); }

    break;

  case 187:

    { (yyval.s) = new string("float2"); }

    break;

  case 188:

    { (yyval.s) = new string("float3"); }

    break;

  case 189:

    { (yyval.s) = new string("float4"); }

    break;

  case 190:

    { (yyval.s) = new string("range"); }

    break;

  case 191:

    { (yyval.s) = new string("urange"); }

    break;

  case 192:

    { (yyval.s) = new string("int64"); }

    break;

  case 193:

    { (yyval.s) = new string("uint64"); }

    break;

  case 194:

    { (yyval.s) = new string("double"); }

    break;

  case 195:

    { (yyval.s) = new string("int8"); }

    break;

  case 196:

    { (yyval.s) = new string("uint8"); }

    break;

  case 197:

    { (yyval.s) = new string("int16"); }

    break;

  case 198:

    { (yyval.s) = new string("uint16"); }

    break;

  case 199:

    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            if ( !yyextra->g_Program->addGeneric((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"generic function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }

    break;

  case 200:

    { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }

    break;

  case 201:

    { (yyval.b) = false; }

    break;

  case 202:

    { (yyval.b) = true; }

    break;

  case 203:

    {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }

    break;

  case 204:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }

    break;

  case 205:

    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-3].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
        }
    }

    break;

  case 206:

    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }

    break;

  case 207:

    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }

    break;

  case 208:

    { (yyval.pExpression) = nullptr; }

    break;

  case 209:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 210:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 211:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 212:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 213:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 214:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 215:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 216:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 217:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 218:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 219:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 220:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 221:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 222:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 223:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 224:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 225:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 226:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 227:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 228:

    { (yyval.pExpression) = nullptr; }

    break;

  case 229:

    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }

    break;

  case 230:

    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }

    break;

  case 231:

    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }

    break;

  case 232:

    {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        auto resT = new TypeDecl(Type::autoinfer);
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])));
        pCall->arguments.push_back(ExpressionPtr(blk));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }

    break;

  case 233:

    { yyextra->das_arrow_depth ++; }

    break;

  case 234:

    { yyextra->das_arrow_depth --; }

    break;

  case 235:

    {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }

    break;

  case 236:

    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            if ( !(yyvsp[-2].pExpression)->swap_tail(pVar,pCall) ) {
                delete pVar;
                (yyval.pExpression) = pCall;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        } else if ( pipeCall->rtti_isMakeStruct() ) {
            auto pMS = (ExprMakeStruct *) pipeCall;
            if ( pMS->block ) {
                das_yyerror(scanner,"can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror(scanner,"can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }

    break;

  case 237:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 238:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 239:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 240:

    {
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-2].s));
            if ( ita == yyextra->das_module_alias.end() ) {
                *(yyvsp[-2].s) += "::";
            } else {
                *(yyvsp[-2].s) = ita->second + "::";
            }
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }

    break;

  case 241:

    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }

    break;

  case 242:

    {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 243:

    {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }

    break;

  case 244:

    { yyextra->das_arrow_depth ++; }

    break;

  case 245:

    { yyextra->das_arrow_depth --; }

    break;

  case 246:

    {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 247:

    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }

    break;

  case 248:

    {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }

    break;

  case 249:

    {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }

    break;

  case 250:

    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }

    break;

  case 251:

    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 252:

    { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }

    break;

  case 253:

    { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }

    break;

  case 254:

    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }

    break;

  case 255:

    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 256:

    {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }

    break;

  case 257:

    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }

    break;

  case 258:

    {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 259:

    {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }

    break;

  case 260:

    {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 261:

    {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }

    break;

  case 262:

    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }

    break;

  case 263:

    {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 264:

    {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }

    break;

  case 265:

    {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 266:

    { (yyval.b) = true; }

    break;

  case 267:

    { (yyval.b) = false; }

    break;

  case 268:

    { (yyval.b) = true; }

    break;

  case 269:

    { (yyval.b) = false; }

    break;

  case 270:

    {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }

    break;

  case 271:

    { yyextra->das_arrow_depth ++; }

    break;

  case 272:

    { yyextra->das_arrow_depth --; }

    break;

  case 273:

    {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }

    break;

  case 274:

    { yyextra->das_arrow_depth ++; }

    break;

  case 275:

    { yyextra->das_arrow_depth --; }

    break;

  case 276:

    {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }

    break;

  case 277:

    { yyextra->das_arrow_depth ++; }

    break;

  case 278:

    { yyextra->das_arrow_depth --; }

    break;

  case 279:

    {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }

    break;

  case 280:

    { yyextra->das_arrow_depth ++; }

    break;

  case 281:

    { yyextra->das_arrow_depth --; }

    break;

  case 282:

    {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }

    break;

  case 283:

    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-2].s);
    }

    break;

  case 284:

    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }

    break;

  case 285:

    {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }

    break;

  case 286:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 287:

    {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }

    break;

  case 288:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 289:

    {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }

    break;

  case 290:

    {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }

    break;

  case 291:

    { (yyval.i) = 0;   /* block */  }

    break;

  case 292:

    { (yyval.i) = 1;   /* lambda */ }

    break;

  case 293:

    { (yyval.i) = 2;   /* local function */ }

    break;

  case 294:

    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }

    break;

  case 295:

    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }

    break;

  case 296:

    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }

    break;

  case 297:

    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }

    break;

  case 298:

    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }

    break;

  case 299:

    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }

    break;

  case 300:

    { (yyval.pCaptList) = nullptr; }

    break;

  case 301:

    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }

    break;

  case 302:

    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }

    break;

  case 303:

    {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }

    break;

  case 304:

    { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }

    break;

  case 305:

    { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }

    break;

  case 306:

    { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }

    break;

  case 307:

    { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }

    break;

  case 308:

    { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }

    break;

  case 309:

    { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }

    break;

  case 310:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 311:

    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 312:

    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 313:

    { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 314:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 315:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 316:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 317:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 318:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 319:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 320:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 321:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 322:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 323:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 324:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 325:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 326:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 327:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 328:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 329:

    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 330:

    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 331:

    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 332:

    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 333:

    {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }

    break;

  case 334:

    {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }

    break;

  case 335:

    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }

    break;

  case 336:

    {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }

    break;

  case 337:

    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }

    break;

  case 338:

    {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }

    break;

  case 339:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 340:

    { yyextra->das_arrow_depth ++; }

    break;

  case 341:

    { yyextra->das_arrow_depth --; }

    break;

  case 342:

    {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 343:

    { yyextra->das_arrow_depth ++; }

    break;

  case 344:

    { yyextra->das_arrow_depth --; }

    break;

  case 345:

    {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 346:

    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }

    break;

  case 347:

    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }

    break;

  case 348:

    { yyextra->das_supress_errors=true; }

    break;

  case 349:

    { yyextra->das_supress_errors=false; }

    break;

  case 350:

    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }

    break;

  case 351:

    { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }

    break;

  case 352:

    { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }

    break;

  case 353:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 354:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 355:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 356:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 357:

    { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }

    break;

  case 358:

    { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }

    break;

  case 359:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 360:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 361:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 362:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 363:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 364:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 365:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 366:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 367:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 368:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 369:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 370:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 371:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 372:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 373:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 374:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 375:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 376:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 377:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 378:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 379:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 380:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 381:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 382:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 383:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 384:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 385:

    { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 386:

    {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }

    break;

  case 387:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 388:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 389:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 390:

    { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 391:

    { (yyval.pExpression) = (yyvsp[-1].pExpression); }

    break;

  case 392:

    { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 393:

    { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }

    break;

  case 394:

    { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 395:

    { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }

    break;

  case 396:

    { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }

    break;

  case 397:

    { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }

    break;

  case 398:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 399:

    {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }

    break;

  case 400:

    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }

    break;

  case 401:

    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }

    break;

  case 402:

    {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }

    break;

  case 403:

    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 404:

    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 405:

    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }

    break;

  case 406:

    {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }

    break;

  case 407:

    {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }

    break;

  case 408:

    { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }

    break;

  case 409:

    {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }

    break;

  case 410:

    { yyextra->das_arrow_depth ++; }

    break;

  case 411:

    { yyextra->das_arrow_depth --; }

    break;

  case 412:

    {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }

    break;

  case 413:

    {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }

    break;

  case 414:

    {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }

    break;

  case 415:

    {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }

    break;

  case 416:

    { yyextra->das_arrow_depth ++; }

    break;

  case 417:

    { yyextra->das_arrow_depth --; }

    break;

  case 418:

    {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }

    break;

  case 419:

    {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }

    break;

  case 420:

    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }

    break;

  case 421:

    { yyextra->das_arrow_depth ++; }

    break;

  case 422:

    { yyextra->das_arrow_depth --; }

    break;

  case 423:

    {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }

    break;

  case 424:

    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }

    break;

  case 425:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 426:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 427:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 428:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 429:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 430:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 431:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 432:

    { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }

    break;

  case 433:

    { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }

    break;

  case 434:

    { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }

    break;

  case 435:

    {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }

    break;

  case 436:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 437:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }

    break;

  case 438:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }

    break;

  case 439:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }

    break;

  case 440:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }

    break;

  case 441:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }

    break;

  case 442:

    { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }

    break;

  case 443:

    {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }

    break;

  case 444:

    {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }

    break;

  case 445:

    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 446:

    {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 447:

    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 448:

    {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 449:

    {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 450:

    {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 451:

    {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }

    break;

  case 452:

    {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }

    break;

  case 453:

    { (yyval.aaList) = nullptr; }

    break;

  case 454:

    { (yyval.aaList) = (yyvsp[-2].aaList); }

    break;

  case 455:

    { (yyval.i) = OVERRIDE_NONE; }

    break;

  case 456:

    { (yyval.i) = OVERRIDE_OVERRIDE; }

    break;

  case 457:

    { (yyval.i) = OVERRIDE_SEALED; }

    break;

  case 458:

    { (yyval.b) = false; }

    break;

  case 459:

    { (yyval.b) = true; }

    break;

  case 460:

    { (yyval.b) = false; }

    break;

  case 461:

    { (yyval.b) = false; }

    break;

  case 462:

    { (yyval.b) = true; }

    break;

  case 463:

    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }

    break;

  case 464:

    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }

    break;

  case 465:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }

    break;

  case 466:

    {
        (yyval.pVarDeclList) = (yyvsp[-3].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-3].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) ) {
                    crd->afterStructureField(nl.name.c_str(), nl.at);
                }
            }
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructureFields(tak);
        }
    }

    break;

  case 467:

    {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }

    break;

  case 468:

    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }

    break;

  case 469:

    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }

    break;

  case 470:

    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }

    break;

  case 471:

    {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }

    break;

  case 472:

    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }

    break;

  case 473:

    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }

    break;

  case 474:

    {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }

    break;

  case 475:

    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 476:

    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 477:

    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }

    break;

  case 478:

    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }

    break;

  case 479:

    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 480:

    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 481:

    {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }

    break;

  case 482:

    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 483:

    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }

    break;

  case 484:

    { (yyval.b) = false; }

    break;

  case 485:

    { (yyval.b) = true; }

    break;

  case 486:

    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }

    break;

  case 487:

    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }

    break;

  case 488:

    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }

    break;

  case 489:

    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }

    break;

  case 490:

    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }

    break;

  case 491:

    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }

    break;

  case 492:

    { (yyval.i) = CorM_COPY; }

    break;

  case 493:

    { (yyval.i) = CorM_MOVE; }

    break;

  case 494:

    { (yyval.i) = CorM_CLONE; }

    break;

  case 495:

    { (yyval.b) = false; }

    break;

  case 496:

    { (yyval.b) = true; }

    break;

  case 497:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }

    break;

  case 498:

    {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }

    break;

  case 499:

    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }

    break;

  case 500:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }

    break;

  case 501:

    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }

    break;

  case 502:

    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }

    break;

  case 503:

    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }

    break;

  case 504:

    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }

    break;

  case 505:

    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }

    break;

  case 506:

    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }

    break;

  case 507:

    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }

    break;

  case 508:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }

    break;

  case 509:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders )
                for ( auto & nl : *((yyvsp[0].pVarDecl)->pNameList) )
                    crd->afterGlobalVariable(nl.name.c_str(),tak);
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterGlobalVariables(tak);
        }
        (yyval.pVarDeclList) = (yyvsp[-3].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-3].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }

    break;

  case 510:

    { (yyval.b) = false; }

    break;

  case 511:

    { (yyval.b) = true; }

    break;

  case 512:

    { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }

    break;

  case 513:

    { (yyval.b) = false; }

    break;

  case 514:

    { (yyval.b) = true; }

    break;

  case 515:

    {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }

    break;

  case 516:

    {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }

    break;

  case 517:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders )
                for ( auto & nl : *((yyvsp[0].pVarDecl)->pNameList) )
                    crd->afterGlobalVariable(nl.name.c_str(),tak);
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterGlobalVariables(tak);
        }
        ast_globalLet(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].aaList),(yyvsp[0].pVarDecl));
    }

    break;

  case 518:

    {
        (yyval.pEnum) = new Enumeration();
    }

    break;

  case 519:

    {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }

    break;

  case 520:

    {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }

    break;

  case 521:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }

    break;

  case 522:

    {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }

    break;

  case 527:

    { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }

    break;

  case 528:

    { (yyval.b) = false; }

    break;

  case 529:

    { (yyval.b) = true; }

    break;

  case 530:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }

    break;

  case 531:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }

    break;

  case 532:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }

    break;

  case 533:

    { (yyval.s) = nullptr; }

    break;

  case 534:

    { (yyval.s) = (yyvsp[0].s); }

    break;

  case 535:

    { (yyval.b) = false; }

    break;

  case 536:

    { (yyval.b) = true; }

    break;

  case 537:

    {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }

    break;

  case 538:

    { (yyval.b) = true; }

    break;

  case 539:

    { (yyval.b) = false; }

    break;

  case 540:

    { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }

    break;

  case 541:

    { (yyval.b) = false; }

    break;

  case 542:

    { (yyval.b) = true; }

    break;

  case 543:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }

    break;

  case 544:

    { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }

    break;

  case 545:

    {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }

    break;

  case 546:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }

    break;

  case 547:

    {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }

    break;

  case 548:

    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }

    break;

  case 549:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }

    break;

  case 550:

    {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }

    break;

  case 551:

    { (yyval.type) = Type::tBool; }

    break;

  case 552:

    { (yyval.type) = Type::tString; }

    break;

  case 553:

    { (yyval.type) = Type::tInt; }

    break;

  case 554:

    { (yyval.type) = Type::tInt8; }

    break;

  case 555:

    { (yyval.type) = Type::tInt16; }

    break;

  case 556:

    { (yyval.type) = Type::tInt64; }

    break;

  case 557:

    { (yyval.type) = Type::tInt2; }

    break;

  case 558:

    { (yyval.type) = Type::tInt3; }

    break;

  case 559:

    { (yyval.type) = Type::tInt4; }

    break;

  case 560:

    { (yyval.type) = Type::tUInt; }

    break;

  case 561:

    { (yyval.type) = Type::tUInt8; }

    break;

  case 562:

    { (yyval.type) = Type::tUInt16; }

    break;

  case 563:

    { (yyval.type) = Type::tUInt64; }

    break;

  case 564:

    { (yyval.type) = Type::tUInt2; }

    break;

  case 565:

    { (yyval.type) = Type::tUInt3; }

    break;

  case 566:

    { (yyval.type) = Type::tUInt4; }

    break;

  case 567:

    { (yyval.type) = Type::tFloat; }

    break;

  case 568:

    { (yyval.type) = Type::tFloat2; }

    break;

  case 569:

    { (yyval.type) = Type::tFloat3; }

    break;

  case 570:

    { (yyval.type) = Type::tFloat4; }

    break;

  case 571:

    { (yyval.type) = Type::tVoid; }

    break;

  case 572:

    { (yyval.type) = Type::tRange; }

    break;

  case 573:

    { (yyval.type) = Type::tURange; }

    break;

  case 574:

    { (yyval.type) = Type::tDouble; }

    break;

  case 575:

    { (yyval.type) = Type::tBitfield; }

    break;

  case 576:

    { (yyval.type) = Type::tInt; }

    break;

  case 577:

    { (yyval.type) = Type::tInt8; }

    break;

  case 578:

    { (yyval.type) = Type::tInt16; }

    break;

  case 579:

    { (yyval.type) = Type::tUInt; }

    break;

  case 580:

    { (yyval.type) = Type::tUInt8; }

    break;

  case 581:

    { (yyval.type) = Type::tUInt16; }

    break;

  case 582:

    {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }

    break;

  case 583:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }

    break;

  case 584:

    {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }

    break;

  case 585:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->dimExpr.push_back((yyvsp[-1].pExpression));
    }

    break;

  case 586:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }

    break;

  case 587:

    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }

    break;

  case 588:

    { yyextra->das_arrow_depth ++; }

    break;

  case 589:

    { yyextra->das_arrow_depth --; }

    break;

  case 590:

    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }

    break;

  case 591:

    {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }

    break;

  case 592:

    {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }

    break;

  case 593:

    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }

    break;

  case 594:

    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }

    break;

  case 595:

    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }

    break;

  case 596:

    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }

    break;

  case 597:

    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don`t get extra infer pass on every array
            auto cI = (ExprConst *) (yyvsp[-1].pExpression);
            auto bt = cI->baseType;
            if ( bt==Type::tInt || bt==Type::tUInt ) {
                dI = cast<int32_t>::to(cI->value);
            }
        }
        (yyvsp[-3].pTypeDecl)->dim.push_back(dI);
        (yyvsp[-3].pTypeDecl)->dimExpr.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyvsp[-3].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }

    break;

  case 598:

    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 599:

    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }

    break;

  case 600:

    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }

    break;

  case 601:

    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }

    break;

  case 602:

    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 603:

    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }

    break;

  case 604:

    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 605:

    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }

    break;

  case 606:

    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }

    break;

  case 607:

    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 608:

    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 609:

    {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }

    break;

  case 610:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }

    break;

  case 611:

    { yyextra->das_arrow_depth ++; }

    break;

  case 612:

    { yyextra->das_arrow_depth --; }

    break;

  case 613:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 614:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }

    break;

  case 615:

    { yyextra->das_arrow_depth ++; }

    break;

  case 616:

    { yyextra->das_arrow_depth --; }

    break;

  case 617:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 618:

    { yyextra->das_arrow_depth ++; }

    break;

  case 619:

    { yyextra->das_arrow_depth --; }

    break;

  case 620:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }

    break;

  case 621:

    { yyextra->das_arrow_depth ++; }

    break;

  case 622:

    { yyextra->das_arrow_depth --; }

    break;

  case 623:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 624:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }

    break;

  case 625:

    { yyextra->das_arrow_depth ++; }

    break;

  case 626:

    { yyextra->das_arrow_depth --; }

    break;

  case 627:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 628:

    { yyextra->das_arrow_depth ++; }

    break;

  case 629:

    { yyextra->das_arrow_depth --; }

    break;

  case 630:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }

    break;

  case 631:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }

    break;

  case 632:

    { yyextra->das_arrow_depth ++; }

    break;

  case 633:

    { yyextra->das_arrow_depth --; }

    break;

  case 634:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 635:

    { yyextra->das_arrow_depth ++; }

    break;

  case 636:

    { yyextra->das_arrow_depth --; }

    break;

  case 637:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }

    break;

  case 638:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }

    break;

  case 639:

    { yyextra->das_arrow_depth ++; }

    break;

  case 640:

    { yyextra->das_arrow_depth --; }

    break;

  case 641:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }

    break;

  case 642:

    { yyextra->das_arrow_depth ++; }

    break;

  case 643:

    { yyextra->das_arrow_depth --; }

    break;

  case 644:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }

    break;

  case 645:

    { yyextra->das_arrow_depth ++; }

    break;

  case 646:

    { yyextra->das_arrow_depth --; }

    break;

  case 647:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }

    break;

  case 648:

    { yyextra->das_arrow_depth ++; }

    break;

  case 649:

    { yyextra->das_arrow_depth --; }

    break;

  case 650:

    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }

    break;

  case 651:

    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }

    break;

  case 652:

    {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        }
    }

    break;

  case 653:

    {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(make_smart<TypeDecl>(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option);
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(make_smart<TypeDecl>(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }

    break;

  case 654:

    { yyextra->das_need_oxford_comma=false; }

    break;

  case 655:

    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }

    break;

  case 656:

    {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-5].s);
        vtype->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-5].s)->c_str(),atvname);
        }
        delete (yyvsp[-5].s);
    }

    break;

  case 657:

    { yyextra->das_need_oxford_comma=false; }

    break;

  case 658:

    {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }

    break;

  case 659:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 660:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 661:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 662:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 663:

    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }

    break;

  case 664:

    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }

    break;

  case 665:

    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }

    break;

  case 666:

    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }

    break;

  case 667:

    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }

    break;

  case 668:

    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }

    break;

  case 669:

    { (yyval.pExpression) = nullptr; }

    break;

  case 670:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 671:

    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }

    break;

  case 672:

    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }

    break;

  case 673:

    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }

    break;

  case 674:

    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }

    break;

  case 675:

    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }

    break;

  case 676:

    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }

    break;

  case 677:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 678:

    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }

    break;

  case 679:

    {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }

    break;

  case 680:

    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }

    break;

  case 681:

    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }

    break;

  case 682:

    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }

    break;

  case 683:

    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }

    break;

  case 684:

    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }

    break;

  case 685:

    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }

    break;

  case 686:

    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }

    break;

  case 687:

    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }

    break;

  case 688:

    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }

    break;

  case 689:

    { (yyval.pExpression) = nullptr; }

    break;

  case 690:

    { (yyval.pExpression) = (yyvsp[0].pExpression); }

    break;

  case 691:

    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }

    break;

  case 692:

    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false);
    }

    break;



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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, scanner);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}



void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_supress_errors ) {
        yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
                CompilationError::syntax_error);
    }
}

LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
}


