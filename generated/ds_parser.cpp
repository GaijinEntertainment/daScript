/* A Bison parser, made by GNU Bison 3.2.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.2.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 28 "src/parser/ds_parser.ypp" /* yacc.c:338  */

	#include "daScript/misc/platform.h"
	#include "daScript/simulate/debug_info.h"
	#include "daScript/ast/compilation_errors.h"

	#ifdef _MSC_VER
	#pragma warning(disable:4262)
	#pragma warning(disable:4127)
	#pragma warning(disable:4702)
	#endif
    
    using namespace das;
    
    void yyerror (const string & error, das::CompilationError cerr = das::CompilationError::syntax_error);
    void yyerror (const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified);
    int yylex();
    void yybegin(const char * str);

#line 88 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:338  */
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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ds_parser.hpp".  */
#ifndef YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser/ds_parser.ypp" /* yacc.c:353  */

	#include "daScript/misc/platform.h"
	#include "daScript/ast/ast.h"
    
    namespace das { extern ProgramPtr g_Program; }
    using namespace das;
    
    struct VariableDeclaration {
        VariableDeclaration ( const LineInfo & a, vector<string> * n, TypeDecl * t, Expression * i )
            : at(a), pNameList(n), pTypeDecl(t), pInit(i) {}
        ~VariableDeclaration () { delete pNameList; delete pTypeDecl; delete pInit; }
        LineInfo        at;
        vector<string>  *pNameList;
        TypeDecl        *pTypeDecl;
        Expression      *pInit;
        bool            move_to_init = false;
    };
    
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 148 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LEXER_ERROR = 258,
    _STRUCT = 259,
    _LET = 260,
    _DEF = 261,
    _WHILE = 262,
    _IF = 263,
    _ELSE = 264,
    _FOR = 265,
    _CATCH = 266,
    _TRUE = 267,
    _FALSE = 268,
    _SIZEOF = 269,
    _NEWT = 270,
    _TYPE = 271,
    _IN = 272,
    _ELIF = 273,
    _ARRAY = 274,
    _RETURN = 275,
    _NULL = 276,
    _BREAK = 277,
    _TRY = 278,
    _OPTIONS = 279,
    _TABLE = 280,
    _EXPECT = 281,
    _CONST = 282,
    _REQUIRE = 283,
    _OPERATOR = 284,
    _ENUM = 285,
    _FINALLY = 286,
    _TBOOL = 287,
    _TVOID = 288,
    _TSTRING = 289,
    _TAUTO = 290,
    _TINT = 291,
    _TINT2 = 292,
    _TINT3 = 293,
    _TINT4 = 294,
    _TUINT = 295,
    _TUINT2 = 296,
    _TUINT3 = 297,
    _TUINT4 = 298,
    _TFLOAT = 299,
    _TFLOAT2 = 300,
    _TFLOAT3 = 301,
    _TFLOAT4 = 302,
    _TRANGE = 303,
    _TURANGE = 304,
    _TBLOCK = 305,
    _TINT64 = 306,
    _TUINT64 = 307,
    _TYPENAME = 308,
    _TDOUBLE = 309,
    ADDEQU = 310,
    SUBEQU = 311,
    DIVEQU = 312,
    MULEQU = 313,
    MODEQU = 314,
    ANDEQU = 315,
    OREQU = 316,
    XOREQU = 317,
    ADDADD = 318,
    SUBSUB = 319,
    LEEQU = 320,
    GREQU = 321,
    EQUEQU = 322,
    NOTEQU = 323,
    RARROW = 324,
    LARROW = 325,
    QQ = 326,
    QDOT = 327,
    LPIPE = 328,
    LBPIPE = 329,
    RPIPE = 330,
    INTEGER = 331,
    LONG_INTEGER = 332,
    UNSIGNED_INTEGER = 333,
    UNSIGNED_LONG_INTEGER = 334,
    FLOAT = 335,
    DOUBLE = 336,
    NAME = 337,
    BEGIN_STRING = 338,
    STRING_CHARACTER = 339,
    END_STRING = 340,
    BEGIN_STRING_EXPR = 341,
    END_STRING_EXPR = 342,
    UNARY_MINUS = 343,
    UNARY_PLUS = 344,
    PRE_INC = 345,
    PRE_DEC = 346,
    POST_INC = 347,
    POST_DEC = 348,
    COLCOL = 349
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 51 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 279 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

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
#define YYLAST   2261

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  120
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  251
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  433

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   349

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   105,     2,     2,   119,   101,    94,     2,
     113,   114,    99,    98,    88,    97,   110,   100,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    91,   118,
      95,    89,    96,    90,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   111,     2,   112,    93,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   116,    92,   117,   104,     2,     2,     2,
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
      85,    86,    87,   102,   103,   106,   107,   108,   109,   115
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   203,   203,   205,   206,   207,   208,   209,   210,   211,
     215,   216,   220,   224,   227,   233,   241,   257,   278,   282,
     283,   287,   299,   303,   304,   308,   311,   317,   318,   319,
     326,   333,   345,   355,   356,   357,   358,   359,   363,   368,
     376,   383,   395,   399,   406,   407,   411,   412,   413,   417,
     421,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     460,   529,   532,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   554,   555,   561,   567,   581,   582,   588,
     592,   600,   604,   607,   610,   618,   624,   650,   677,   680,
     686,   689,   695,   698,   704,   709,   760,   761,   762,   763,
     764,   765,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   820,   824,   827,
     830,   831,   832,   835,   836,   837,   838,   839,   849,   859,
     893,   894,   898,   899,   903,   904,   908,   909,   913,   917,
     920,   924,   929,   937,   940,   944,   948,   953,   961,   984,
     987,   995,  1006,  1019,  1062,  1068,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1098,  1108,  1112,  1121,  1122,
    1123,  1124,  1128,  1133,  1138,  1143,  1148,  1152,  1157,  1163,
    1167,  1172,  1195,  1196,  1200,  1207,  1216,  1221,  1228,  1236,
    1241,  1248
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEXER_ERROR", "_STRUCT", "_LET", "_DEF",
  "_WHILE", "_IF", "_ELSE", "_FOR", "_CATCH", "_TRUE", "_FALSE", "_SIZEOF",
  "_NEWT", "_TYPE", "_IN", "_ELIF", "_ARRAY", "_RETURN", "_NULL", "_BREAK",
  "_TRY", "_OPTIONS", "_TABLE", "_EXPECT", "_CONST", "_REQUIRE",
  "_OPERATOR", "_ENUM", "_FINALLY", "_TBOOL", "_TVOID", "_TSTRING",
  "_TAUTO", "_TINT", "_TINT2", "_TINT3", "_TINT4", "_TUINT", "_TUINT2",
  "_TUINT3", "_TUINT4", "_TFLOAT", "_TFLOAT2", "_TFLOAT3", "_TFLOAT4",
  "_TRANGE", "_TURANGE", "_TBLOCK", "_TINT64", "_TUINT64", "_TYPENAME",
  "_TDOUBLE", "ADDEQU", "SUBEQU", "DIVEQU", "MULEQU", "MODEQU", "ANDEQU",
  "OREQU", "XOREQU", "ADDADD", "SUBSUB", "LEEQU", "GREQU", "EQUEQU",
  "NOTEQU", "RARROW", "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE",
  "INTEGER", "LONG_INTEGER", "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER",
  "FLOAT", "DOUBLE", "NAME", "BEGIN_STRING", "STRING_CHARACTER",
  "END_STRING", "BEGIN_STRING_EXPR", "END_STRING_EXPR", "','", "'='",
  "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'",
  "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC",
  "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('", "')'",
  "COLCOL", "'{'", "'}'", "';'", "'$'", "$accept", "program",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_list", "require_module", "expect_declaration", "expect_list",
  "expect_error", "expression_else", "expression_if_then_else",
  "expression_for_loop", "expression_while_loop", "annotation_argument",
  "annotation_argument_list", "annotation_declaration", "annotation_list",
  "optional_annotation_list", "optional_function_argument_list",
  "optional_function_type", "function_name", "function_declaration",
  "expression_block", "expression_any", "expressions", "expr_pipe",
  "name_in_namespace", "expr_new", "expression_break", "expression_return",
  "expression_try_catch", "expression_let", "expr_sizeof", "expr_typename",
  "expr_list", "expr_block", "expr_numeric_const", "expr_assign", "expr",
  "struct_variable_declaration_list", "variable_declaration_list",
  "function_argument_list", "copy_or_move", "variable_declaration",
  "let_variable_declaration", "global_let", "enum_list",
  "enum_declaration", "structure_declaration", "variable_name_list",
  "basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "make_decl",
  "make_struct_fields", "make_struct_dim", "make_struct_decl", "make_dim",
  "make_dim_decl", YY_NULLPTR
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
     335,   336,   337,   338,   339,   340,   341,   342,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,   343,   344,   126,    33,   345,   346,   347,   348,
      46,    91,    93,    40,    41,   349,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -253

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-253)))

#define YYTABLE_NINF -123

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -253,    18,  -253,   -89,   -65,   -43,    13,    45,   -66,  -253,
    -253,  -253,   193,  -253,  -253,  -253,  -253,  -253,    43,  -253,
      72,    87,    91,  -253,  -253,   104,  -253,    82,   131,   120,
    -253,    29,    95,   167,   346,   -70,   106,   -65,   174,   -43,
      13,  -253,   172,  -253,   -66,  -253,   -65,   139,  1764,   238,
     240,  -253,   146,  -253,  -253,   156,   259,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,    -3,  -253,  -253,
     125,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
       1,   180,  -253,  -253,   198,  -253,   186,  1247,    52,   -81,
    -253,  -253,    14,  -253,   -84,  -253,   186,   165,  -253,   189,
     190,  -253,  -253,  -253,   162,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,   194,
    -253,  -253,  -253,  -253,  -253,  -253,   341,  -253,  -253,   184,
     -66,  -253,   185,  1247,  1247,  -253,  -253,  -253,  -253,  -253,
    -253,    -9,  -253,  1247,  1247,  1247,  1247,   192,  1247,  -253,
     195,  -253,  -253,  -253,    93,  -253,  -253,  -253,  -253,  -253,
     222,  1440,   205,  -253,  -253,  -253,  -253,  -253,   219,  -253,
    -253,  -253,   245,   230,  -253,   186,   186,   246,    20,  -253,
    -253,  -253,   -16,   255,  1247,   787,  -253,   895,   461,   461,
     159,   461,   461,   461,   461,   186,  1600,   344,   146,  -253,
     983,   -97,  1247,  1247,  1247,  1247,  1247,  1247,  1247,  1247,
    -253,  -253,  1247,  1247,  1247,  1247,  -253,  1247,  1247,   252,
    1247,  1247,  1247,  1247,  1247,  1247,  1247,  1247,  1247,  1247,
    1247,  1247,  1247,  1247,   254,  1247,  1071,   225,  -253,    11,
     153,   226,   180,   182,  -253,  -253,   232,  1928,   186,  1652,
     186,  1704,  -253,  1247,   262,   571,  -253,  -253,    99,  1247,
    1247,   245,  1159,  -253,   165,   324,  -253,  -253,  -253,  -253,
    -253,  -253,   244,   251,  -253,  -253,   -67,  1497,   180,  -253,
     128,  1928,  -253,  1928,  1928,  1928,  1928,  1928,  1928,  1928,
    1928,  2150,  2150,  2135,  2135,  1928,   461,  -253,   -29,   -29,
    1928,  1830,  2026,  2075,  2092,  2150,  2150,   418,   418,   250,
     250,   250,  -253,  1780,  -253,   152,  -253,  -253,   186,  -253,
     267,  -253,  -253,   -14,  -253,    -2,  -253,  1879,   102,   679,
      23,  1928,   282,   -92,    39,   245,  -253,   283,  1546,  1546,
     -11,  1247,  1928,   362,   278,  -253,  -253,  -253,   165,  1247,
    -253,  1247,  -253,  -253,   183,  -253,  -253,  -253,  -253,  1247,
     316,   287,   318,   289,  1247,   -24,  -253,   186,  1247,  -253,
       6,  1247,  1928,   165,  -253,  -253,  1928,  1977,  -253,  1928,
     313,  -253,   314,   282,  -253,  1928,   165,   245,   221,  -253,
    1312,   165,  1247,  -253,    36,  -253,   463,  1247,  -253,  -253,
    -253,  1247,  -253,  -253,  1546,  -253,  -253,  1928,  -253,  1376,
       6,  -253,  -253
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    44,     1,     0,     0,     0,     0,     0,     0,     9,
       8,     7,     0,     6,     5,     4,     3,   180,     0,    38,
      17,    25,    22,    23,    21,    18,    19,     0,    97,     0,
      42,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,   199,     0,    99,     0,    45,     0,     0,     0,     0,
       0,    51,    46,   204,   198,     0,   188,    36,    37,    34,
      35,    10,    33,    39,    26,    24,    20,     0,    98,    43,
       0,   180,    54,    55,    57,    56,    58,    59,    60,    61,
      78,    79,    71,    72,    69,    70,    74,    75,    73,    67,
      68,    63,    62,    64,    65,    66,    53,    52,    76,    77,
       0,    49,   181,   187,     0,   186,     0,     0,     0,     0,
     202,    41,     0,    47,     0,   184,     0,     0,   205,     0,
       0,   206,   222,   207,   226,   208,   210,   211,   212,   213,
     215,   216,   217,   218,   219,   220,   221,   223,   224,   239,
     209,   214,   225,   228,   230,   229,   189,   137,   138,     0,
       0,   132,     0,     0,     0,   116,   118,   117,   119,   120,
     121,   133,    13,     0,     0,     0,     0,     0,     0,    93,
      44,   135,   114,   192,     0,   175,   173,   174,   176,   134,
       0,   191,     0,   136,   242,   243,    11,    12,     0,   200,
     203,    48,     0,    50,    80,     0,     0,     0,    46,   232,
     236,   234,     0,     0,     0,     0,   100,     0,   158,   159,
      10,   142,   141,   140,   139,     0,     0,     0,    46,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,   161,     0,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,     0,    49,     0,   233,   235,     0,   190,     0,     0,
       0,     0,    16,     0,    14,     0,   162,    95,     0,     0,
       0,     0,   102,   101,     0,    81,    83,    91,    88,    87,
      94,    84,     0,     0,    92,    86,     0,   122,    49,   166,
       0,   112,    96,   127,   128,   130,   129,   131,   124,   125,
     126,   153,   154,   151,   152,   143,   171,   165,   177,   178,
     123,     0,   156,   157,   155,   149,   150,   145,   144,   146,
     147,   148,   164,     0,   168,     0,   201,   237,     0,   227,
       0,   240,   231,     0,   108,     0,   110,     0,   133,     0,
       0,   249,   246,     0,     0,     0,   107,     0,     0,     0,
       0,     0,   103,     0,     0,    89,    90,    85,     0,     0,
     167,     0,   163,   169,     0,   241,   109,   111,    15,     0,
       0,     0,     0,     0,     0,     0,   182,     0,     0,    32,
      27,     0,   104,     0,    93,   115,   113,   172,   238,   244,
       0,   248,     0,   247,   251,   250,     0,     0,     0,   197,
     122,     0,     0,    30,     0,   105,     0,     0,   106,   183,
     193,     0,   196,    28,     0,    31,    82,   245,   195,   122,
      27,   194,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -253,  -253,   196,  -253,  -253,  -253,  -253,  -253,  -253,   364,
    -253,  -253,   366,   -18,  -253,  -253,  -253,   376,   368,   371,
    -253,   247,  -189,  -248,  -253,  -253,  -107,  -253,    22,  -105,
      -8,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -252,   197,
    -253,  -212,   -79,   348,  -253,  -253,  -143,   -99,  -253,  -253,
    -253,  -253,  -253,  -246,   -85,  -253,  -253,   -75,  -253,    47,
    -253,  -253,  -253,  -253
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   108,    62,   210,   171,     9,    10,    25,    26,
      11,    22,    23,   413,   287,   288,   289,    19,    20,    30,
      31,    12,   101,   117,    52,    13,   172,   290,   217,   291,
     174,   175,   292,   293,   294,   295,   176,   177,   300,   178,
     179,   180,   301,    35,   385,   114,   107,    55,   356,    14,
      67,    15,    16,    56,   182,   144,   145,   146,   183,   352,
     353,   184,   354,   185
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,   115,   173,   204,   335,   296,   391,   221,   188,   262,
     194,   264,    53,   199,   340,   411,    28,    18,     2,   169,
     381,   143,   170,     3,   412,   199,   382,    17,   181,   298,
     191,   143,   357,    21,   192,   360,    32,   189,   199,   119,
     236,   193,     4,   239,     5,   120,     6,    54,     7,    29,
     368,   367,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   -97,   208,   209,   200,   104,   265,   109,
     201,   254,   255,    53,   211,   212,   213,   214,   200,   216,
     406,   202,   201,   258,   407,    24,    53,   203,   142,   266,
     376,   200,    28,   202,   -97,   201,    42,   337,   142,   203,
     143,   143,   377,   143,   110,   113,   202,    44,    57,    58,
     259,   260,   203,   263,   369,   267,   269,    27,   271,     8,
     143,   190,    36,   100,   215,    29,   186,   187,   297,   414,
     275,    45,   206,   303,   304,   305,   306,   307,   308,   309,
     310,   383,   169,   311,   312,   313,   314,   384,   315,   316,
      37,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   219,   333,   363,    38,    39,
     199,    53,    59,   143,   -97,   143,    60,   142,   142,    61,
     142,   379,    40,   343,   347,   345,   351,    33,    41,    34,
     358,   359,    43,   362,   296,   119,   220,   142,    46,   199,
     199,   120,   355,    37,   388,   -97,   369,    42,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   111,
     369,   338,   370,   200,   272,   273,    42,   201,   199,    47,
      64,   389,   390,   143,    68,    71,   386,   199,   202,   100,
     142,   395,   142,   374,   203,   421,   373,    98,    28,    99,
     214,   116,   200,   200,   102,   197,   201,   201,   341,   398,
     118,   169,   392,   409,   195,   196,   415,   202,   202,   198,
     396,   103,   397,   203,   203,   257,   221,   205,   207,   418,
     399,    29,   143,   215,   423,   405,     8,   425,   419,   410,
     105,   200,   408,   230,   231,   201,   428,   430,   256,   236,
     200,   238,   239,   240,   201,   241,   202,    53,   261,   103,
     142,   266,   203,   424,   317,   202,   332,   297,   427,   420,
     339,   203,   429,   336,   342,   277,   186,   104,   105,   278,
     106,   279,   280,   103,   281,   364,   147,   148,   149,   150,
     254,   255,   365,   375,   282,   151,   283,   284,   199,   366,
     380,   104,   105,   393,   387,    48,   121,   122,   123,   142,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   394,   140,   141,   152,   400,   401,
     402,   404,   417,   379,    66,    65,   274,   153,   154,    49,
      50,   103,   432,    63,    70,    69,   416,   218,   302,   112,
     155,   156,   157,   158,   159,   160,   161,   162,    51,   403,
     105,   200,     0,     0,     0,   201,     0,     0,     0,     0,
       0,   163,   164,     0,     0,     0,   202,     0,   165,   166,
       0,     0,   203,     0,     0,   167,     0,   168,     0,    29,
     169,   285,   286,   170,   277,     0,     0,     0,   278,     0,
     279,   280,     0,   281,     0,   147,   148,   149,   150,     0,
       0,   230,   231,   282,   151,   283,   284,   236,     0,   238,
     239,   240,     0,   241,     0,   121,   122,   123,     0,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,     0,   140,   141,   152,   251,   252,   253,
       0,     0,     0,     0,   230,   231,   153,   154,   254,   255,
     236,     0,     0,   239,   240,     0,   241,     0,     0,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   164,     0,     0,     0,     0,     0,   165,   166,     0,
       0,   254,   255,     0,   167,     0,   168,     0,    29,   169,
     426,   286,   170,   147,   148,   149,   150,     0,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,   199,     0,
       0,     0,     0,   121,   122,   123,     0,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,     0,   140,   141,   152,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   348,   162,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,   201,     0,     0,   163,   164,
       0,     0,     0,     0,     0,   165,   349,     0,     0,     0,
       0,     0,   350,     0,   168,     0,    29,   169,     0,     0,
     170,   147,   148,   149,   150,     0,     0,     0,     0,     0,
     151,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   121,   122,   123,     0,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
     140,   141,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,     0,   163,   164,     0,     0,
       0,     0,     0,   165,   166,     0,     0,     0,     0,     0,
     167,     0,   168,     0,    29,   169,     0,     0,   170,   147,
     148,   149,   150,   268,     0,     0,     0,     0,   151,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     153,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   164,     0,     0,     0,     0,
       0,   165,   166,     0,     0,     0,     0,     0,   167,     0,
     168,     0,    29,   169,     0,     0,   170,   147,   148,   149,
     150,   270,     0,     0,     0,     0,   151,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,   123,
       0,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,     0,   140,   141,   152,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,   154,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   155,   156,   157,   158,   159,   160,   161,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,   164,     0,   147,   148,   149,   150,   165,
     166,     0,     0,     0,   151,     0,   167,     0,   168,     0,
      29,   169,     0,     0,   170,   121,   122,   123,     0,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,     0,   140,   141,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   164,     0,   147,   148,   149,   150,   165,   166,     0,
       0,     0,   151,     0,   167,     0,   168,   299,    29,   169,
       0,     0,   170,   121,   122,   123,     0,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,     0,   140,   141,   152,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   161,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   164,
       0,   147,   148,   149,   150,   165,   166,     0,     0,     0,
     151,     0,   167,     0,   168,   334,    29,   169,     0,     0,
     170,   121,   122,   123,     0,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
     140,   141,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   153,   154,     0,     0,     0,     0,     0,   361,
       0,     0,     0,     0,     0,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,   164,     0,   147,
     148,   149,   150,   165,   166,     0,     0,     0,   151,     0,
     167,     0,   168,     0,    29,   169,     0,     0,   170,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     153,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   164,     0,     0,     0,     0,
       0,   165,   166,     0,     0,     0,     0,     0,   167,     0,
     168,     0,    29,   169,     0,     0,   170,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,   243,     0,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,     0,     0,     0,     0,     0,
       0,     0,   254,   255,     0,     0,     0,     0,     0,     0,
     422,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,   243,     0,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   254,   255,     0,     0,
       0,     0,     0,     0,   431,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,  -122,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   254,   255,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   254,   255,     0,     0,
       0,     0,   169,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255,     0,     0,   276,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,     0,     0,     0,     0,     0,
       0,     0,   254,   255,     0,     0,   344,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,     0,     0,     0,
       0,     0,     0,     0,   254,   255,     0,     0,   346,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   241,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,     0,    96,    97,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255,   372,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   371,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,     0,     0,     0,     0,     0,     0,     0,   254,
     255,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,     0,     0,     0,     0,     0,     0,     0,   254,   255,
     230,   231,   232,   233,   234,   235,   236,     0,   238,   239,
     240,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   254,   255,   230,
     231,   232,   233,   234,   235,   236,     0,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   254,   255,   230,   231,
     232,   233,   234,   235,   236,     0,   238,   239,   240,     0,
     241,     0,     0,     0,     0,   230,   231,   232,   233,   234,
     235,   236,     0,   238,   239,   240,     0,   241,     0,   246,
     247,   248,   249,   250,   251,   252,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   254,   255,   247,   248,   249,
     250,   251,   252,   253,     0,     0,     0,     0,   230,   231,
     232,   233,   254,   255,   236,     0,   238,   239,   240,     0,
     241,     0,     0,   230,   231,     0,     0,     0,     0,   236,
       0,   238,   239,   240,     0,   241,     0,     0,     0,     0,
     247,   248,   249,   250,   251,   252,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   254,   255,   249,   250,   251,
     252,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     254,   255
};

static const yytype_int16 yycheck[] =
{
       8,   100,   107,   146,   256,   217,    17,    74,    89,   198,
     117,    27,    82,    27,   262,     9,    82,    82,     0,   116,
     112,   106,   119,     5,    18,    27,   118,   116,   107,   218,
     114,   116,   278,    76,   118,   281,    44,   118,    27,    19,
      69,   116,    24,    72,    26,    25,    28,   117,    30,   115,
     298,   118,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    82,   153,   154,    90,    88,    94,    82,
      94,   110,   111,    82,   163,   164,   165,   166,    90,   168,
     114,   105,    94,   192,   118,    82,    82,   111,   106,    76,
     114,    90,    82,   105,   113,    94,   115,    96,   116,   111,
     195,   196,   114,   198,   117,   114,   105,    88,    12,    13,
     195,   196,   111,   198,    88,   204,   205,    82,   207,   111,
     215,   117,    89,   113,   111,   115,    84,    85,   217,   391,
     215,   112,   150,   222,   223,   224,   225,   226,   227,   228,
     229,   112,   116,   232,   233,   234,   235,   118,   237,   238,
      88,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,    82,   255,   284,    91,    88,
      27,    82,    76,   268,    82,   270,    80,   195,   196,    83,
     198,    89,    88,   268,   273,   270,   275,     4,   116,     6,
     279,   280,    82,   282,   416,    19,   113,   215,   113,    27,
      27,    25,   113,    88,   357,   113,    88,   115,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,   114,
      88,    88,   114,    90,    85,    86,   115,    94,    27,    82,
      76,   358,   359,   338,    82,   116,   355,    27,   105,   113,
     268,   368,   270,   338,   111,   408,   114,    29,    82,    29,
     349,    91,    90,    90,   118,   113,    94,    94,    96,    96,
      82,   116,   361,   388,    95,    95,   393,   105,   105,    95,
     369,    70,   371,   111,   111,    76,    74,   113,   113,   406,
     379,   115,   387,   111,   411,   384,   111,   414,   407,   388,
      89,    90,   387,    63,    64,    94,   421,   424,   113,    69,
      90,    71,    72,    73,    94,    75,   105,    82,    82,    70,
     338,    76,   111,   412,    82,   105,    82,   416,   417,   118,
     114,   111,   421,   118,   112,     1,    84,    88,    89,     5,
      91,     7,     8,    70,    10,    31,    12,    13,    14,    15,
     110,   111,   118,    96,    20,    21,    22,    23,    27,   118,
      88,    88,    89,    11,    91,    29,    32,    33,    34,   387,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,   116,    51,    52,    53,    82,   112,
      82,   112,    89,    89,    40,    39,   210,    63,    64,    63,
      64,    70,   430,    37,    46,    44,   394,   170,   221,    71,
      76,    77,    78,    79,    80,    81,    82,    83,    82,   382,
      89,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,   105,    -1,   104,   105,
      -1,    -1,   111,    -1,    -1,   111,    -1,   113,    -1,   115,
     116,   117,   118,   119,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    63,    64,    20,    21,    22,    23,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    52,    53,    99,   100,   101,
      -1,    -1,    -1,    -1,    63,    64,    63,    64,   110,   111,
      69,    -1,    -1,    72,    73,    -1,    75,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,   110,   111,    -1,   111,    -1,   113,    -1,   115,   116,
     117,   118,   119,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,    -1,
     119,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,    -1,   119,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   115,   116,    -1,    -1,   119,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,    12,    13,    14,    15,   104,
     105,    -1,    -1,    -1,    21,    -1,   111,    -1,   113,    -1,
     115,   116,    -1,    -1,   119,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,    12,    13,    14,    15,   104,   105,    -1,
      -1,    -1,    21,    -1,   111,    -1,   113,   114,   115,   116,
      -1,    -1,   119,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,    12,    13,    14,    15,   104,   105,    -1,    -1,    -1,
      21,    -1,   111,    -1,   113,   114,   115,   116,    -1,    -1,
     119,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    12,
      13,    14,    15,   104,   105,    -1,    -1,    -1,    21,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,    -1,   119,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   115,   116,    -1,    -1,   119,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,   116,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,   114,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    -1,    -1,   114,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    -1,    -1,   114,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,   110,   111,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   121,     0,     5,    24,    26,    28,    30,   111,   126,
     127,   130,   141,   145,   169,   171,   172,   116,    82,   137,
     138,    76,   131,   132,    82,   128,   129,    82,    82,   115,
     139,   140,   150,     4,     6,   163,    89,    88,    91,    88,
      88,   116,   115,    82,    88,   112,   113,    82,    29,    63,
      64,    82,   144,    82,   117,   167,   173,    12,    13,    76,
      80,    83,   123,   137,    76,   132,   129,   170,    82,   139,
     138,   116,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   104,   105,    29,    29,
     113,   142,   118,    70,    88,    89,    91,   166,   122,    82,
     117,   114,   163,   114,   165,   167,    91,   143,    82,    19,
      25,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,   150,   174,   175,   176,   177,    12,    13,    14,
      15,    21,    53,    63,    64,    76,    77,    78,    79,    80,
      81,    82,    83,    97,    98,   104,   105,   111,   113,   116,
     119,   125,   146,   149,   150,   151,   156,   157,   159,   160,
     161,   162,   174,   178,   181,   183,    84,    85,    89,   118,
     117,   114,   118,   177,   146,    95,    95,   113,    95,    27,
      90,    94,   105,   111,   166,   113,   150,   113,   162,   162,
     124,   162,   162,   162,   162,   111,   162,   148,   141,    82,
     113,    74,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    75,    89,    90,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   110,   111,   113,    76,   167,   177,
     177,    82,   142,   177,    27,    94,    76,   162,    16,   162,
      16,   162,    85,    86,   122,   177,   114,     1,     5,     7,
       8,    10,    20,    22,    23,   117,   118,   134,   135,   136,
     147,   149,   152,   153,   154,   155,   161,   162,   142,   114,
     158,   162,   159,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,    82,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,    82,   162,   114,   158,   118,    96,    88,   114,
     143,    96,   112,   177,   114,   177,   114,   162,    82,   105,
     111,   162,   179,   180,   182,   113,   168,   173,   162,   162,
     173,    70,   162,   146,    31,   118,   118,   118,   143,    88,
     114,    91,   112,   114,   177,    96,   114,   114,    87,    89,
      88,   112,   118,   112,   118,   164,   167,    91,   166,   146,
     146,    17,   162,    11,   116,   146,   162,   162,    96,   162,
      82,   112,    82,   179,   112,   162,   114,   118,   177,   149,
     162,     9,    18,   133,   158,   146,   148,    89,   146,   167,
     118,   166,   118,   146,   162,   146,   117,   162,   149,   162,
     146,   118,   133
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   120,   121,   121,   121,   121,   121,   121,   121,   121,
     122,   122,   123,   124,   124,   124,   125,   126,   127,   128,
     128,   129,   130,   131,   131,   132,   132,   133,   133,   133,
     134,   135,   136,   137,   137,   137,   137,   137,   138,   138,
     139,   139,   140,   140,   141,   141,   142,   142,   142,   143,
     143,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   146,   146,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   148,   148,   148,   149,   150,   150,   150,
     151,   152,   153,   153,   153,   154,   155,   155,   156,   156,
     157,   157,   158,   158,   159,   159,   160,   160,   160,   160,
     160,   160,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     163,   163,   164,   164,   165,   165,   166,   166,   167,   167,
     167,   167,   167,   168,   168,   168,   168,   168,   169,   170,
     170,   170,   171,   172,   173,   173,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   175,   176,   176,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   178,   178,   179,   179,   180,   180,   181,   182,
     182,   183
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       0,     2,     3,     0,     2,     4,     3,     2,     2,     1,
       3,     1,     2,     1,     3,     1,     3,     0,     2,     4,
       4,     5,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       6,     3,     7,     1,     1,     2,     1,     1,     1,     2,
       2,     1,     1,     0,     2,     2,     3,     1,     3,     2,
       2,     1,     1,     2,     3,     4,     5,     2,     4,     5,
       4,     5,     1,     3,     1,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     3,     3,     3,     4,     3,     4,
       2,     3,     5,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     1,     3,     1,     3,     1,     1,     1,     3,
       5,     3,     3,     4,     6,     5,     4,     3,     4,     0,
       3,     5,     5,     6,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     4,     2,     3,     2,     3,     2,     4,     6,     1,
       4,     5,     1,     1,     3,     5,     1,     3,     6,     1,
       3,     6
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
      yyerror (YY_("syntax error: cannot back up")); \
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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
    default: /* Avoid compiler warnings. */
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 82: /* NAME  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1907 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 122: /* character_sequence  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1913 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 123: /* string_constant  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1919 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 124: /* string_builder_body  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1925 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 125: /* string_builder  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1931 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 133: /* expression_else  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1937 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 134: /* expression_if_then_else  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1943 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 135: /* expression_for_loop  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1949 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 136: /* expression_while_loop  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1955 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* annotation_argument  */
#line 81 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 1961 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* annotation_argument_list  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 1967 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* annotation_declaration  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 1973 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* annotation_list  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1979 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 141: /* optional_annotation_list  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1985 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 142: /* optional_function_argument_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 1991 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* optional_function_type  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 1997 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* function_name  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2003 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* expression_block  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2009 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* expression_any  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2015 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* expressions  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2021 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* expr_pipe  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2027 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* name_in_namespace  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2033 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expr_new  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2039 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expression_break  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2045 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* expression_return  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2051 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* expression_try_catch  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2057 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expression_let  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2063 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expr_sizeof  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2069 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* expr_typename  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2075 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expr_list  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2081 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* expr_block  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2087 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* expr_numeric_const  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2093 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* expr_assign  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2099 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* expr  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2105 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* struct_variable_declaration_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2111 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* variable_declaration_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2117 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* function_argument_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2123 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* variable_declaration  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2129 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* let_variable_declaration  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2135 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* enum_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2141 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* variable_name_list  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2147 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* structure_type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2153 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* auto_type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2159 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2165 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* make_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2171 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* make_struct_fields  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2177 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* make_struct_dim  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2183 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* make_struct_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2189 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* make_dim  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2195 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* make_dim_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2201 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

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
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
                  (unsigned long) yystacksize));

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
      yychar = yylex ();
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 10:
#line 215 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); }
#line 2489 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 11:
#line 216 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2495 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 220 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2501 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 224 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2509 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 227 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2520 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 233 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2530 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 241 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto strb = static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression));
        if ( strb->elements.size()==0 ) {
            (yyval.pExpression) = new ExprConstString(strb->at,"");
            delete (yyvsp[-1].pExpression);
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_pointer_cast<ExprConstString>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(strb->at,sconst->text);
            delete (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    }
#line 2548 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 17:
#line 257 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		if ( g_Program->options.arguments.size() ) {
			g_Program->options.arguments.insert ( g_Program->options.arguments.begin(),
                (yyvsp[0].aaList)->arguments.begin(), (yyvsp[0].aaList)->arguments.end() );
		} else {
			swap ( g_Program->options, *(yyvsp[0].aaList) );
		}
		auto opt = g_Program->options.find("indenting", tInt);
		extern int tab_size, def_tab_size;
		if (opt)
		{
			if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
				tab_size = def_tab_size;
			else
				tab_size = opt->iValue ? opt->iValue : def_tab_size;//0 is default
		}
		delete (yyvsp[0].aaList);
	}
#line 2571 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 21:
#line 287 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( auto mod = Module::require(*(yyvsp[0].s)) ) {
            g_Program->addModule(mod);
        } else {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2585 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 25:
#line 308 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2593 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 311 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2601 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 317 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 2607 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 318 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2613 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 319 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2622 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 326 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2631 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 333 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 2645 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 345 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile();
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 2657 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 355 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2663 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 356 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i));  }
#line 2669 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 357 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)));  }
#line 2675 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 358 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true);  }
#line 2681 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 359 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false);  }
#line 2687 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 363 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 2697 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 368 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 2707 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 376 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 2719 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 383 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 2733 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 395 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2742 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 399 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2751 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 406 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 2757 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 407 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 2763 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 411 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2769 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 412 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2775 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 413 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 2781 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 417 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(yylloc.first_column,yylloc.first_line);
    }
#line 2790 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 421 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 2799 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 428 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2805 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 429 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 2811 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 430 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 2817 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 431 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 2823 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 432 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 2829 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 433 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 2835 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 434 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 2841 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 435 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 2847 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 436 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 2853 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 437 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 2859 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 438 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 2865 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 439 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 2871 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 2877 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 2883 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 2889 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 443 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 2895 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 444 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 2901 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 2907 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 446 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 2913 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 2919 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 2925 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 2931 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 450 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 2937 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 451 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 2943 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 452 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 2949 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 453 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 2955 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 454 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 2961 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 455 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 2967 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 456 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 2973 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 461 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFunction = make_shared<Function>();
        pFunction->at = tokAt((yylsp[-4]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name) ) {
                            VariablePtr pVar = make_shared<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->move_to_init = pDecl->move_to_init;
                            }
                            pFunction->arguments.push_back(pVar);
                        } else {
                            yyerror("function argument is already declared " + name,pDecl->at,
                                CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        if ( (yyvsp[-5].faList) ) {
            for ( auto pA : *(yyvsp[-5].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(pFunction, pA->arguments, err) ) {
                            yyerror("can't apply annotation\n" + err,
                                tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                        }
                    } else {
                        yyerror("functions are only allowed function annotations",
                            tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( pFunction->annotations, *(yyvsp[-5].faList) );
            delete (yyvsp[-5].faList);
        }
        if ( pFunction->isGeneric() ) {
            if ( !g_Program->addGeneric(pFunction) ) {
                yyerror("generic function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        } else {
            if ( !g_Program->addFunction(pFunction) ) {
                yyerror("function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        }
        delete (yyvsp[-3].s);
    }
#line 3043 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 529 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3051 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 532 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
    }
#line 3062 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 541 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3068 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 542 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3074 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 543 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3080 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 544 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3086 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 545 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3092 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 546 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3098 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 547 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3104 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 548 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3110 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 549 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3116 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 550 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3122 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 554 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBlock(); }
#line 3128 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 555 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3139 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 561 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3147 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 567 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCall() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            yyerror("can only pipe into function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 3163 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 581 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3169 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 582 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3180 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3186 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pTypeDecl = g_Program->makeTypeDeclaration(tokAt((yylsp[-1])), *(yyvsp[0].s));
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr(pTypeDecl));
        delete (yyvsp[0].s);
    }
#line 3196 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 600 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3202 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 604 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3210 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 607 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3218 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 610 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3228 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 618 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3236 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 624 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pLet = new ExprLet();
        pLet->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    if ( !pLet->find(name) ) {
                        VariablePtr pVar = make_shared<Variable>();
                        pVar->name = name;
                        pVar->at = pDecl->at;
                        pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                        if ( pDecl->pInit ) {
                            pVar->init = pDecl->pInit->clone();
                            pVar->move_to_init = pDecl->move_to_init;
                        }
                        pLet->variables.push_back(pVar);
                    } else {
                        yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                            CompilationError::local_variable_already_declared);
                    }
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        (yyval.pExpression) = pLet;
    }
#line 3267 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 650 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pLet = new ExprLet();
        pLet->scoped = false;
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name) ) {
                    VariablePtr pVar = make_shared<Variable>();
                    pVar->name = name;
                    pVar->at = (yyvsp[0].pVarDecl)->at;
                    pVar->type = make_shared<TypeDecl>(*(yyvsp[0].pVarDecl)->pTypeDecl);
                    if ( (yyvsp[0].pVarDecl)->pInit ) {
                        pVar->init = (yyvsp[0].pVarDecl)->pInit->clone();
                        pVar->move_to_init = (yyvsp[0].pVarDecl)->move_to_init;
                    }
                    pLet->variables.push_back(pVar);
                } else {
                    yyerror("local variable is already declared " + name,tokAt((yylsp[-1])),
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 3296 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 677 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3304 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 680 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3312 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 686 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3320 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 689 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3328 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 695 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3336 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 698 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3344 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 704 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3354 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 710 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name) ) {
                            VariablePtr pVar = make_shared<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->move_to_init = pDecl->move_to_init;
                            }
                            closure->arguments.push_back(pVar);
                        } else {
                            yyerror("block argument is already declared " + name,
                                pDecl->at,CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
            if ( (yyvsp[-3].faList) ) {
                for ( auto pA : *(yyvsp[-3].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                            auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->apply(closure, pA->arguments, err) ) {
                                yyerror("can't apply annotation\n" + err, tokAt((yylsp[-3])),
                                    CompilationError::invalid_annotation);
                            }
                        } else {
                            yyerror("blocks are only allowed function annotations", tokAt((yylsp[-3])),
                                CompilationError::invalid_annotation);
                        }
                    }
                }
                swap ( closure->annotations, *(yyvsp[-3].faList) );
                delete (yyvsp[-3].faList);
            }
        }
    }
#line 3406 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 760 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3412 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 761 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3418 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 762 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3424 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 763 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3430 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 764 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3436 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 765 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3442 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 769 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3448 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 770 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3454 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3460 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 772 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3466 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 773 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3472 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 774 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3478 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 775 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3484 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 776 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3490 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 777 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3496 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 778 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3502 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 782 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3508 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3514 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 784 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3520 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 785 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3526 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3532 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 787 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3538 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 788 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3544 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 789 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3550 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 790 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3556 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 791 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3562 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 792 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3568 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 793 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3574 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 794 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3580 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 795 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3586 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 796 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3592 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 797 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3598 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 798 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3604 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 799 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3610 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 800 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3616 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 801 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3622 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 802 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3628 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 803 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3634 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 804 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3640 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 805 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3646 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 806 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3652 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 807 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3658 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 808 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3664 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 809 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3670 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 810 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3676 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 811 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3682 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 812 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3688 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 813 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3694 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 814 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3700 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 815 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3706 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 816 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 3715 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 820 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 3724 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 824 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 3732 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 827 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 3740 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 830 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[0])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3746 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 831 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3752 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 832 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 3760 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 835 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3766 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 836 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3772 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 837 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3778 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 838 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3784 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 839 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( (yyvsp[-2].pExpression)->rtti_isCall() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-2].pExpression);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 3799 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 849 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( (yyvsp[0].pExpression)->rtti_isCall() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[0].pExpression);
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = (yyvsp[0].pExpression);
        } else {
            yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 3814 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 859 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		ExprConstEnumeration * resEnum = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        if ( enums.size()!=1 ) {
            if ( enums.size() ) {
                string candidates = g_Program->describeCandidates(enums);
                yyerror("enumeraiton not found " + *(yyvsp[-1].s) + "\n" + candidates, tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            } else {
                yyerror("enumeraiton not found " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            }
        } else {
            auto pEnum = enums.back();
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
				auto td = make_shared<TypeDecl>(pEnum);
                resEnum = new ExprConstEnumeration(tokAt((yylsp[0])), ff.first, td);
            } else {
                yyerror("enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt((yylsp[0])),
                    CompilationError::enumeration_not_found);
            }
        }
		if ( resEnum ) {
			(yyval.pExpression) = resEnum;
		} else {
			(yyval.pExpression) = new ExprConstInt(0);	// dummy
		}
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 3850 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); }
#line 3856 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl)); }
#line 3862 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3868 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3874 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 903 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3880 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3886 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 3892 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 909 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 3898 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 3907 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 3915 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 920 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3924 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3934 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3944 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 3952 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3961 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 944 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3970 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 948 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3980 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 953 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3990 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 961 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    VariablePtr pVar = make_shared<Variable>();
                    pVar->name = name;
                    pVar->at = pDecl->at;
                    pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                    if ( pDecl->pInit ) {
                        pVar->init = pDecl->pInit->clone();
                        pVar->move_to_init = pDecl->move_to_init;
                    }
                    if ( !g_Program->addVariable(pVar) )
                        yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 4015 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 984 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4023 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 987 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4036 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 995 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4049 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 1006 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s); 
        if ( !g_Program->addEnumeration(pEnum) ) {
            yyerror("enumeration is alredy defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-3].s);
    }
#line 4064 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 1019 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pStruct = make_shared<Structure>(*(yyvsp[-3].s));
        pStruct->at = tokAt((yylsp[-4]));
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            for ( const auto & name : *pDecl->pNameList ) {
                if ( !pStruct->findField(name) ) {
                    auto td = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                    auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                    pStruct->fields.emplace_back(name, td, init, pDecl->at);
                } else {
                    yyerror("structure field is already declared "+name,pDecl->at,
                        CompilationError::structure_field_already_declared);
                }
            }
        }
        if ( (yyvsp[-5].faList) ) {
            if ( (yyvsp[-5].faList)->size()!=1 ) {
                yyerror("structures are only allowed one annotation", tokAt((yylsp[-4])),
                    CompilationError::invalid_annotation);
            } else {
                auto & annotation = (yyvsp[-5].faList)->back()->annotation;
                auto & args = (yyvsp[-5].faList)->back()->arguments;
                if ( !annotation->rtti_isStructureAnnotation() ) {
                    yyerror("structures are only allowed structure annotation", tokAt((yylsp[-4])),
                        CompilationError::invalid_annotation);
                } else if ( !g_Program->addStructureHandle(pStruct,
                        static_pointer_cast<StructureTypeAnnotation>(annotation), args) ) {
                    yyerror("handled structure is alredy defined "+*(yyvsp[-3].s),tokAt((yylsp[-4])),
                        CompilationError::structure_already_declared);
                }
            }
            delete (yyvsp[-5].faList);
        } else {
            if ( !g_Program->addStructure(pStruct) )
                yyerror("structure is alredy defined "+*(yyvsp[-3].s),tokAt((yylsp[-4])),
                    CompilationError::structure_already_declared);
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        delete((yyvsp[-3].s));
    }
#line 4109 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 1062 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4120 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 1068 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4130 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 1076 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 4136 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 1077 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 4142 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 1078 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 4148 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 1079 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 4154 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 1080 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 4160 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 1081 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 4166 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 1082 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 4172 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 1083 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 4178 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 1084 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 4184 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 1085 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 4190 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 1086 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 4196 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 1087 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 4202 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 1088 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 4208 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 1089 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 4214 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 1090 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 4220 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 1091 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 4226 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 1092 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 4232 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1093 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 4238 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1094 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 4244 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1098 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4256 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1108 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4265 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1112 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4276 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1121 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4282 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1122 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4288 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1123 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4294 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1124 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4303 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1128 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4313 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1133 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4323 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1138 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4333 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1143 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4343 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1148 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4352 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1152 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4362 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1157 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4373 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1163 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4382 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1167 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4392 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1172 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( size_t ai=0; ai!= pDecl->pNameList->size(); ++ai ) {
                        auto pVarType = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                        if ( pDecl->pInit ) {
                            yyerror("can't have default values in block type declaration",
                                pDecl->at,CompilationError::cant_initialize);
                        }
                        (yyval.pTypeDecl)->argTypes.push_back(pVarType);
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 4417 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1195 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4423 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1196 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4429 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 244:
#line 1200 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 4441 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 245:
#line 1207 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 4452 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 246:
#line 1216 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 4462 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 247:
#line 1221 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4471 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 248:
#line 1228 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4481 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 249:
#line 1236 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 4491 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 250:
#line 1241 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4500 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 251:
#line 1248 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4510 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 4514 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
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
        yyerror (yymsgp);
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
                      yytoken, &yylval, &yylloc);
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
                  yystos[yystate], yyvsp, yylsp);
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
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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
#line 1255 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


void yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,at,cerr);
}

void yyerror(const string & error, CompilationError cerr) {
    g_Program->error(error,LineInfo(yylloc.first_column,yylloc.first_line),cerr);
}

vector<ExpressionPtr> sequenceToList ( Expression * arguments ) {
    vector<ExpressionPtr> argList;
    auto arg = arguments;
    if ( arg->rtti_isSequence() ) {
        while ( arg->rtti_isSequence() ) {
            auto pSeq = static_cast<ExprSequence *>(arg);
            assert(!pSeq->right->rtti_isSequence());
            argList.push_back(pSeq->right);
            arg = pSeq->left.get();
        }
        argList.push_back(arg->shared_from_this());
        reverse(argList.begin(),argList.end());
        delete arguments;
    } else {
        argList.push_back(ExpressionPtr(arg));
    }
    return argList;
}

ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments ) {
    pCall->arguments = sequenceToList(arguments);
    return pCall;
}

void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list ) {
    if ( !list ) return;
    for ( auto pD : *list )
        delete pD;
    delete list;
}

LineInfo tokAt ( const struct YYLTYPE & li ) {
    return LineInfo(li.first_column,li.first_line);
}

Annotation * findAnnotation ( const string & name, const LineInfo & at ) {
    auto ann = g_Program->findAnnotation(name);
    if ( ann.size()==1 ) {
        return ann.back().get();
    } else if ( ann.size()==0 ) {
        yyerror("annotation " + name + " not found", at, CompilationError::annotation_not_found );
        return nullptr;
    } else {
        string candidates = g_Program->describeCandidates(ann);
        yyerror("too many options for annotation " + name + "\n" + candidates, at, CompilationError::annotation_not_found );
        return nullptr;
    }
}


