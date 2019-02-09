/* A Bison parser, made by GNU Bison 3.2.1.  */

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
#define YYBISON_VERSION "3.2.1"

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

#line 88 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
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

#line 148 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

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
    _DELETE = 287,
    _DEREF = 288,
    _SCOPE = 289,
    _TBOOL = 290,
    _TVOID = 291,
    _TSTRING = 292,
    _TAUTO = 293,
    _TINT = 294,
    _TINT2 = 295,
    _TINT3 = 296,
    _TINT4 = 297,
    _TUINT = 298,
    _TUINT2 = 299,
    _TUINT3 = 300,
    _TUINT4 = 301,
    _TFLOAT = 302,
    _TFLOAT2 = 303,
    _TFLOAT3 = 304,
    _TFLOAT4 = 305,
    _TRANGE = 306,
    _TURANGE = 307,
    _TBLOCK = 308,
    _TINT64 = 309,
    _TUINT64 = 310,
    _TYPENAME = 311,
    _TDOUBLE = 312,
    ADDEQU = 313,
    SUBEQU = 314,
    DIVEQU = 315,
    MULEQU = 316,
    MODEQU = 317,
    ANDEQU = 318,
    OREQU = 319,
    XOREQU = 320,
    ADDADD = 321,
    SUBSUB = 322,
    LEEQU = 323,
    GREQU = 324,
    EQUEQU = 325,
    NOTEQU = 326,
    RARROW = 327,
    LARROW = 328,
    QQ = 329,
    QDOT = 330,
    LPIPE = 331,
    LBPIPE = 332,
    RPIPE = 333,
    INTEGER = 334,
    LONG_INTEGER = 335,
    UNSIGNED_INTEGER = 336,
    UNSIGNED_LONG_INTEGER = 337,
    FLOAT = 338,
    DOUBLE = 339,
    NAME = 340,
    BEGIN_STRING = 341,
    STRING_CHARACTER = 342,
    END_STRING = 343,
    BEGIN_STRING_EXPR = 344,
    END_STRING_EXPR = 345,
    UNARY_MINUS = 346,
    UNARY_PLUS = 347,
    PRE_INC = 348,
    PRE_DEC = 349,
    POST_INC = 350,
    POST_DEC = 351,
    COLCOL = 352
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

#line 282 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
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

#endif /* !YY_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   2228

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  123
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  259
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  447

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   352

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   108,     2,     2,   122,   104,    97,     2,
     116,   117,   102,   101,    91,   100,   113,   103,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    94,   121,
      98,    92,    99,    93,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   114,     2,   115,    96,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   119,    95,   120,   107,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90,   105,   106,   109,   110,
     111,   112,   118
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   207,   207,   209,   210,   211,   212,   213,   214,   215,
     219,   220,   224,   228,   231,   237,   245,   261,   282,   286,
     287,   291,   303,   307,   308,   312,   315,   321,   322,   323,
     330,   337,   349,   359,   360,   361,   362,   363,   364,   368,
     373,   381,   382,   386,   393,   405,   409,   416,   417,   421,
     422,   423,   427,   431,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   470,   539,   542,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   565,   566,   572,
     578,   592,   593,   599,   603,   609,   615,   619,   622,   625,
     633,   639,   640,   644,   670,   698,   701,   707,   710,   716,
     719,   725,   730,   781,   782,   783,   784,   785,   786,   790,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   841,   845,   848,   851,   852,   853,
     856,   857,   858,   859,   860,   870,   880,   914,   915,   919,
     920,   924,   925,   929,   930,   934,   938,   941,   945,   950,
     958,   961,   965,   969,   974,   982,  1005,  1008,  1016,  1027,
    1040,  1083,  1089,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,
    1114,  1115,  1116,  1120,  1130,  1134,  1143,  1144,  1145,  1146,
    1150,  1155,  1160,  1165,  1170,  1174,  1179,  1185,  1189,  1194,
    1217,  1218,  1222,  1229,  1238,  1243,  1250,  1258,  1263,  1270
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
  "_OPERATOR", "_ENUM", "_FINALLY", "_DELETE", "_DEREF", "_SCOPE",
  "_TBOOL", "_TVOID", "_TSTRING", "_TAUTO", "_TINT", "_TINT2", "_TINT3",
  "_TINT4", "_TUINT", "_TUINT2", "_TUINT3", "_TUINT4", "_TFLOAT",
  "_TFLOAT2", "_TFLOAT3", "_TFLOAT4", "_TRANGE", "_TURANGE", "_TBLOCK",
  "_TINT64", "_TUINT64", "_TYPENAME", "_TDOUBLE", "ADDEQU", "SUBEQU",
  "DIVEQU", "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "ADDADD",
  "SUBSUB", "LEEQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW", "LARROW", "QQ",
  "QDOT", "LPIPE", "LBPIPE", "RPIPE", "INTEGER", "LONG_INTEGER",
  "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER", "FLOAT", "DOUBLE", "NAME",
  "BEGIN_STRING", "STRING_CHARACTER", "END_STRING", "BEGIN_STRING_EXPR",
  "END_STRING_EXPR", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "'.'", "'['", "']'", "'('", "')'", "COLCOL", "'{'", "'}'", "';'", "'$'",
  "$accept", "program", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "options_declaration",
  "require_declaration", "require_list", "require_module",
  "expect_declaration", "expect_list", "expect_error", "expression_else",
  "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break", "expression_return",
  "expression_try_catch", "let_scope", "expression_let", "expr_sizeof",
  "expr_typename", "expr_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr", "struct_variable_declaration_list",
  "variable_declaration_list", "function_argument_list", "copy_or_move",
  "variable_declaration", "let_variable_declaration", "global_let",
  "enum_list", "enum_declaration", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration",
  "type_declaration", "make_decl", "make_struct_fields", "make_struct_dim",
  "make_struct_decl", "make_dim", "make_dim_decl", YY_NULLPTR
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
     345,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,   346,   347,   126,    33,   348,
     349,   350,   351,    46,    91,    93,    40,    41,   352,   123,
     125,    59,    36
};
# endif

#define YYPACT_NINF -276

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-276)))

#define YYTABLE_NINF -130

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -276,     6,  -276,  -102,   -57,    19,    37,    40,   -21,  -276,
    -276,  -276,   135,  -276,  -276,  -276,  -276,  -276,    48,  -276,
      18,    51,    89,  -276,  -276,    95,  -276,    77,  -276,    88,
     126,   102,  -276,   -16,  -276,   160,    52,   -48,    -3,   -57,
     161,    19,    37,  -276,   162,  -276,   -57,   -21,  -276,   129,
    1754,   217,   221,  -276,   143,  -276,  -276,   130,   169,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
      -6,  -276,   -76,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,   -15,   172,  -276,  -276,   187,  -276,   184,
    1185,   127,    91,  -276,  -276,    10,  -276,   -95,  -276,   184,
     156,  -276,   176,   178,  -276,  -276,  -276,   163,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,   180,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
     189,  -276,  -276,   164,   184,  -276,   174,   175,  1185,  1185,
    -276,  -276,  -276,  -276,  -276,  -276,   -47,  -276,  1185,  1185,
    1185,  1185,   171,  1185,  -276,   182,  -276,  -276,  -276,     0,
    -276,  -276,  -276,  -276,  -276,   211,  1378,   192,  -276,  -276,
    -276,  -276,  -276,   213,  -276,  -276,  -276,   209,   463,  -276,
     184,   184,   210,     8,  -276,  -276,  -276,   -13,   226,  1185,
     735,   463,  1185,   825,   416,   416,   155,   416,   416,   416,
     416,   184,  1538,   343,   143,  -276,   915,    72,  1185,  1185,
    1185,  1185,  1185,  1185,  1185,  1185,  -276,  -276,  1185,  1185,
    1185,  1185,  1185,  1185,   215,  1185,  1185,  1185,  1185,  1185,
    1185,  1185,  1185,  1185,  1185,  1185,  1185,  1185,  1185,   231,
    1185,  1005,   196,  -276,   322,   340,   194,   172,   341,  -276,
    -276,   205,  1918,   184,  1590,  1642,   184,  1694,  -276,  1185,
     234,   555,  -276,  -276,   -12,  1185,  1185,   209,  1095,  -276,
     156,  1185,   291,  -276,  -276,  -276,  -276,  -276,  -276,   203,
     207,   208,  -276,  -276,   -54,  1435,   172,  -276,   -75,  1918,
    -276,  1918,  1918,  1918,  1918,  1918,  1918,  1918,  1918,  2114,
    2114,  2099,  2099,  1918,   416,  -276,   -40,   -40,  1918,  1820,
    2000,  2041,  2058,  2114,  2114,  1291,  1291,   259,   259,   259,
    -276,  1770,  -276,    26,  -276,  -276,   184,  -276,   242,  -276,
    -276,   190,  -276,  -276,   222,  -276,  1869,    71,   645,    24,
    1918,   252,   -97,   -96,   311,   209,   209,  1484,  1484,     3,
    1185,  1918,   349,  1918,   233,  -276,  -276,  -276,  -276,   156,
    1185,  -276,  1185,  -276,  -276,   444,  -276,  -276,  -276,  -276,
    1185,   269,   246,   277,   254,  1185,  -276,    80,  -276,  -276,
     179,  -276,   181,  1185,  1918,   156,  -276,  -276,  1918,  1959,
    -276,  1918,   278,  -276,   279,   252,  -276,  1918,   156,   209,
     184,  1185,   156,  1185,  -276,    17,  -276,   465,  1185,  -276,
    -276,    13,  -276,  1250,  -276,  1484,  -276,  -276,  1918,  -276,
    1185,  -276,   181,  -276,  1314,  -276,  -276
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    47,     1,     0,     0,     0,     0,     0,     0,     9,
       8,     7,     0,     6,     5,     4,     3,   187,     0,    39,
      17,    25,    22,    23,    21,    18,    19,     0,    42,   101,
       0,    43,    45,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,   103,     0,     0,    48,     0,
       0,     0,     0,    54,    49,   211,   205,     0,   195,    37,
      38,    35,    36,    34,    10,    33,    40,    26,    24,    20,
       0,   102,     0,    46,   187,    57,    58,    60,    59,    61,
      62,    63,    64,    81,    82,    74,    75,    72,    73,    77,
      78,    76,    70,    71,    66,    65,    67,    68,    69,    56,
      55,    79,    80,     0,    52,   188,   194,     0,   193,     0,
       0,     0,     0,   209,    44,     0,    50,     0,   191,     0,
       0,   212,     0,     0,   213,   229,   214,   234,   215,   217,
     218,   219,   220,   222,   223,   224,   225,   226,   227,   228,
     230,   231,   247,   216,   221,   232,   233,   236,   238,   237,
     196,   144,   145,     0,     0,   139,     0,     0,     0,     0,
     123,   125,   124,   126,   127,   128,   140,    13,     0,     0,
       0,     0,     0,     0,    97,    47,   142,   121,   199,     0,
     182,   180,   181,   183,   141,     0,   198,     0,   143,   250,
     251,    11,    12,     0,   207,   210,    51,     0,    53,    83,
       0,     0,     0,    49,   240,   244,   242,     0,     0,     0,
       0,   105,     0,     0,   165,   166,    10,   149,   148,   147,
     146,     0,     0,     0,    49,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   192,     0,     0,     0,    52,     0,   241,
     243,     0,   197,     0,     0,     0,     0,     0,    16,     0,
      14,     0,   169,    99,   111,     0,     0,     0,   107,   106,
       0,     0,    84,    86,    95,    92,    91,    98,    87,     0,
       0,     0,    96,    90,     0,   129,    52,   173,     0,   119,
     100,   134,   135,   137,   136,   138,   131,   132,   133,   160,
     161,   158,   159,   150,   178,   172,   184,   185,   130,     0,
     163,   164,   162,   156,   157,   152,   151,   153,   154,   155,
     171,     0,   175,     0,   208,   245,     0,   235,     0,   248,
     239,     0,   115,   177,     0,   117,     0,   140,     0,     0,
     257,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,     0,   104,     0,    89,    93,    94,    88,     0,
       0,   174,     0,   170,   176,     0,   249,   116,   118,    15,
       0,     0,     0,     0,     0,     0,   112,     0,   189,   114,
       0,    32,    27,     0,   109,     0,    97,   122,   120,   179,
     246,   252,     0,   256,     0,   255,   259,   258,     0,     0,
       0,     0,     0,     0,    30,     0,   110,     0,     0,   113,
     190,     0,   204,   129,    28,     0,    31,    85,   253,   200,
       0,   203,    27,   202,   129,    29,   201
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -276,  -276,   158,  -276,  -276,  -276,  -276,  -276,  -276,   335,
    -276,  -276,   355,   -61,  -276,  -276,  -276,   363,   357,  -276,
     358,  -276,   229,   -80,  -199,  -276,  -276,  -112,  -276,     1,
    -109,    -8,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -248,   186,  -276,  -219,   -81,   332,  -276,  -276,  -147,
    -101,  -276,  -276,  -276,  -276,  -276,  -275,   -88,  -276,  -276,
     -19,  -276,    15,  -276,  -276,  -276,  -276
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   111,    65,   216,   176,     9,    10,    25,    26,
      11,    22,    23,   424,   294,   295,   296,    19,    20,    31,
      32,    33,    12,   104,   120,    54,    13,   177,   297,   223,
     298,   179,   299,   180,   300,   301,   302,   366,   303,   181,
     182,   308,   183,   184,   185,   309,    37,   397,   117,   110,
      57,   399,    14,    70,    15,    16,    58,   187,   148,   149,
     150,   188,   361,   362,   189,   363,   190
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,   178,   118,   209,   304,   364,     2,    28,   199,    59,
      60,     3,   369,   343,   269,    39,   380,    17,   392,   394,
     403,   147,   196,   227,   393,   395,   197,   122,    18,   186,
       4,   147,     5,   123,     6,   244,     7,    55,  -101,    34,
     204,   114,   381,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    29,   145,   147,   378,   348,  -101,
      55,    44,    56,   259,   260,    47,    61,   214,   215,   112,
      62,    50,    63,    64,   270,   225,   106,   217,   218,   219,
     220,   400,   222,    29,   107,    55,   263,    30,    21,    48,
     198,   146,   116,   271,   365,   108,   205,   379,   380,    39,
     206,   146,   147,   147,   113,   147,   226,   380,    51,    52,
       8,   207,    24,   267,   103,    27,    30,   208,   272,   274,
     195,   275,   277,   147,   439,   211,   174,    53,   221,    35,
      38,    36,   305,   384,   306,    40,   146,   311,   312,   313,
     314,   315,   316,   317,   318,   425,  -101,   319,   320,   321,
     322,   323,   324,   390,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   372,   341,
      41,   264,   265,   193,   268,   147,    42,  -101,   147,    44,
     422,   174,   146,   146,   175,   146,    43,   418,   356,   423,
     360,   419,   281,   122,   367,   368,    44,   371,   304,   123,
     373,    45,   194,   146,   191,   192,   204,   204,    46,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
      67,   145,   106,   278,   279,    49,   101,    71,    74,   204,
     102,   105,   106,   421,   351,   401,   402,   354,   147,   103,
     107,   108,   106,   109,   398,   146,   119,   407,   146,    29,
     107,   108,   121,   420,   200,   174,   201,   220,   203,   202,
     210,   108,   205,   205,   440,   221,   206,   206,   227,   404,
     212,   213,   262,   426,    55,   266,     8,   207,   207,   408,
     325,   409,    30,   208,   208,   271,   429,   387,   261,   411,
     434,   347,   432,   436,   417,   205,   340,   344,   430,   206,
     350,   191,   374,   442,   375,   236,   237,   385,   376,   377,
     207,   443,   147,   243,   244,   245,   208,   246,   146,   388,
     433,   386,   435,   391,   283,   396,   305,   438,   284,   204,
     285,   286,   406,   287,   412,   151,   152,   153,   154,   444,
     405,   413,   414,   288,   155,   289,   290,   204,   204,   416,
     428,   390,   259,   260,   280,   291,   156,    69,   124,   125,
     126,   445,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    68,   143,   144,   157,
     145,   431,    66,    72,   224,    73,   115,   427,   415,   158,
     159,     0,   146,   310,     0,   205,     0,     0,     0,   206,
       0,   345,   160,   161,   162,   163,   164,   165,   166,   167,
     207,   346,     0,   205,   205,     0,   208,   206,   206,     0,
     349,     0,     0,   168,   169,     0,     0,     0,   207,   207,
     170,   171,     0,     0,   208,   208,     0,   172,     0,   173,
       0,    30,   174,   292,   293,   175,   283,     0,     0,     0,
     284,   204,   285,   286,     0,   287,     0,   151,   152,   153,
     154,     0,   236,   237,     0,   288,   155,   289,   290,     0,
     204,   244,   245,     0,   246,     0,     0,   291,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,   259,
     260,   158,   159,     0,     0,     0,     0,   205,     0,     0,
       0,   206,     0,   410,   160,   161,   162,   163,   164,   165,
     166,   167,   207,     0,     0,     0,   205,     0,   208,     0,
     206,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,   207,   170,   171,     0,     0,   155,   208,     0,   172,
       0,   173,   204,    30,   174,   437,   293,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     357,   167,     0,     0,     0,     0,     0,     0,   205,     0,
       0,     0,   206,     0,     0,   168,   169,   151,   152,   153,
     154,     0,   170,   358,     0,     0,   155,     0,     0,   359,
       0,   173,   269,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,   168,   169,   151,   152,   153,
     154,   273,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,     0,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,   276,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,     0,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,     0,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,     0,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,     0,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,   307,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,     0,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,   342,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,   370,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   151,   152,   153,
     154,     0,   170,   171,     0,     0,   155,     0,     0,   172,
       0,   173,     0,    30,   174,     0,     0,   175,   156,     0,
     124,   125,   126,     0,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,   143,
     144,   157,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,   169,     0,     0,     0,
       0,     0,   170,   171,     0,     0,     0,     0,     0,   172,
       0,   173,     0,    30,   174,     0,     0,   175,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   242,   243,   244,   245,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,     0,   236,   237,     0,
       0,     0,     0,   259,   260,   243,   244,   245,     0,   246,
       0,   441,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   242,   243,   244,
     245,     0,   246,   256,   257,   258,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   260,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,     0,
       0,     0,     0,     0,     0,   446,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   242,   243,   244,   245,  -129,   246,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,     0,   242,   243,
     244,   245,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   260,
     236,   237,   238,   239,   240,   241,     0,   242,   243,   244,
     245,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,     0,
       0,     0,     0,   174,   236,   237,   238,   239,   240,   241,
       0,   242,   243,   244,   245,     0,   246,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,   282,   236,   237,   238,   239,
     240,   241,     0,   242,   243,   244,   245,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,     0,     0,   352,   236,   237,
     238,   239,   240,   241,     0,   242,   243,   244,   245,     0,
     246,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   260,     0,     0,   353,
     236,   237,   238,   239,   240,   241,     0,   242,   243,   244,
     245,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,     0,
       0,   355,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,   241,     0,   242,   243,   244,   245,     0,   246,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,     0,
       0,    99,   100,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,   383,   236,   237,   238,   239,
     240,   241,     0,   242,   243,   244,   245,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   248,   382,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,   236,   237,   238,   239,   240,
     241,     0,   242,   243,   244,   245,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   389,
       0,     0,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   236,   237,   238,   239,   240,   241,
       0,   242,   243,   244,   245,     0,   246,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,     0,     0,   236,   237,   238,   239,   240,
     241,   259,   260,   243,   244,   245,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,     0,     0,   236,   237,   238,   239,
     240,   241,   259,   260,   243,   244,   245,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,     0,   236,   237,   238,
     239,   240,   241,   259,   260,   243,   244,   245,     0,   246,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
       0,     0,   243,   244,   245,     0,   246,     0,   251,   252,
     253,   254,   255,   256,   257,   258,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   260,   252,   253,   254,   255,
     256,   257,   258,     0,     0,   236,   237,   238,   239,     0,
       0,   259,   260,   243,   244,   245,     0,   246,     0,     0,
     236,   237,     0,     0,     0,     0,     0,     0,   243,   244,
     245,     0,   246,     0,     0,     0,     0,   252,   253,   254,
     255,   256,   257,   258,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   254,   255,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260
};

static const yytype_int16 yycheck[] =
{
       8,   110,   103,   150,   223,    17,     0,    28,   120,    12,
      13,     5,   287,   261,    27,    91,    91,   119,   115,   115,
      17,   109,   117,    77,   121,   121,   121,    19,    85,   110,
      24,   119,    26,    25,    28,    75,    30,    85,    85,    47,
      27,   117,   117,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    85,    57,   154,   121,   267,   116,
      85,   118,   120,   113,   114,    91,    79,   158,   159,    85,
      83,    29,    85,    86,    97,    85,    73,   168,   169,   170,
     171,   366,   173,    85,    91,    85,   197,   118,    79,   115,
     119,   109,   117,    79,   116,    92,    93,   306,    91,    91,
      97,   119,   200,   201,   120,   203,   116,    91,    66,    67,
     114,   108,    85,   203,   116,    85,   118,   114,   209,   210,
     120,   212,   213,   221,   121,   154,   119,    85,   114,     4,
      92,     6,   223,   117,   224,    94,   154,   228,   229,   230,
     231,   232,   233,   234,   235,   403,    85,   238,   239,   240,
     241,   242,   243,    92,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   290,   260,
      91,   200,   201,    92,   203,   273,    91,   116,   276,   118,
       9,   119,   200,   201,   122,   203,   119,   117,   279,    18,
     281,   121,   221,    19,   285,   286,   118,   288,   427,    25,
     291,    85,   121,   221,    87,    88,    27,    27,   116,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      79,    57,    73,    88,    89,    85,    29,    85,   119,    27,
      29,   121,    73,   400,   273,   367,   368,   276,   346,   116,
      91,    92,    73,    94,   365,   273,    94,   379,   276,    85,
      91,    92,    85,    94,    98,   119,    98,   358,    98,   116,
     116,    92,    93,    93,   431,   114,    97,    97,    77,   370,
     116,   116,    79,   405,    85,    85,   114,   108,   108,   380,
      85,   382,   118,   114,   114,    79,   418,   117,   116,   390,
     422,   117,   421,   425,   395,    93,    85,   121,   419,    97,
     115,    87,    31,   435,   121,    66,    67,   346,   121,   121,
     108,   440,   420,    74,    75,    76,   114,    78,   346,   117,
     421,    99,   423,    91,     1,    34,   427,   428,     5,    27,
       7,     8,   119,    10,    85,    12,    13,    14,    15,   440,
      11,   115,    85,    20,    21,    22,    23,    27,    27,   115,
      92,    92,   113,   114,   216,    32,    33,    42,    35,    36,
      37,   442,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    41,    54,    55,    56,
      57,   420,    39,    46,   175,    47,    74,   406,   393,    66,
      67,    -1,   420,   227,    -1,    93,    -1,    -1,    -1,    97,
      -1,    99,    79,    80,    81,    82,    83,    84,    85,    86,
     108,    91,    -1,    93,    93,    -1,   114,    97,    97,    -1,
      99,    -1,    -1,   100,   101,    -1,    -1,    -1,   108,   108,
     107,   108,    -1,    -1,   114,   114,    -1,   114,    -1,   116,
      -1,   118,   119,   120,   121,   122,     1,    -1,    -1,    -1,
       5,    27,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    -1,    66,    67,    -1,    20,    21,    22,    23,    -1,
      27,    75,    76,    -1,    78,    -1,    -1,    32,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    66,    67,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    99,    79,    80,    81,    82,    83,    84,
      85,    86,   108,    -1,    -1,    -1,    93,    -1,   114,    -1,
      97,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,   108,   107,   108,    -1,    -1,    21,   114,    -1,   114,
      -1,   116,    27,   118,   119,   120,   121,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,   100,   101,    12,    13,    14,
      15,    -1,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,    27,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,   100,   101,    12,    13,    14,
      15,    16,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,    -1,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,    16,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,    -1,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,    -1,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,    -1,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,    -1,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,   117,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,    -1,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,   117,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    12,    13,    14,
      15,    -1,   107,   108,    -1,    -1,    21,    -1,    -1,   114,
      -1,   116,    -1,   118,   119,    -1,    -1,   122,    33,    -1,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,   114,
      -1,   116,    -1,   118,   119,    -1,    -1,   122,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,   113,   114,    74,    75,    76,    -1,    78,
      -1,   121,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    -1,    78,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,   119,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,   117,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,   117,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,   117,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,   117,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    -1,    78,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,   107,   108,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    66,    67,    68,    69,    70,
      71,   113,   114,    74,    75,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    66,    67,    68,    69,
      70,    71,   113,   114,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    66,    67,    68,
      69,    70,    71,   113,   114,    74,    75,    76,    -1,    78,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      -1,    -1,    74,    75,    76,    -1,    78,    -1,    97,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,   113,   114,    74,    75,    76,    -1,    78,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
      76,    -1,    78,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   124,     0,     5,    24,    26,    28,    30,   114,   129,
     130,   133,   145,   149,   175,   177,   178,   119,    85,   140,
     141,    79,   134,   135,    85,   131,   132,    85,    28,    85,
     118,   142,   143,   144,   154,     4,     6,   169,    92,    91,
      94,    91,    91,   119,   118,    85,   116,    91,   115,    85,
      29,    66,    67,    85,   148,    85,   120,   173,   179,    12,
      13,    79,    83,    85,    86,   126,   140,    79,   135,   132,
     176,    85,   141,   143,   119,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   107,
     108,    29,    29,   116,   146,   121,    73,    91,    92,    94,
     172,   125,    85,   120,   117,   169,   117,   171,   173,    94,
     147,    85,    19,    25,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    57,   154,   180,   181,   182,
     183,    12,    13,    14,    15,    21,    33,    56,    66,    67,
      79,    80,    81,    82,    83,    84,    85,    86,   100,   101,
     107,   108,   114,   116,   119,   122,   128,   150,   153,   154,
     156,   162,   163,   165,   166,   167,   168,   180,   184,   187,
     189,    87,    88,    92,   121,   120,   117,   121,   183,   150,
      98,    98,   116,    98,    27,    93,    97,   108,   114,   172,
     116,   183,   116,   116,   168,   168,   127,   168,   168,   168,
     168,   114,   168,   152,   145,    85,   116,    77,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    73,    74,    75,    76,    78,    92,    93,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   113,
     114,   116,    79,   173,   183,   183,    85,   146,   183,    27,
      97,    79,   168,    16,   168,   168,    16,   168,    88,    89,
     125,   183,   117,     1,     5,     7,     8,    10,    20,    22,
      23,    32,   120,   121,   137,   138,   139,   151,   153,   155,
     157,   158,   159,   161,   167,   168,   146,   117,   164,   168,
     165,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,    85,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
      85,   168,   117,   164,   121,    99,    91,   117,   147,    99,
     115,   183,   117,   117,   183,   117,   168,    85,   108,   114,
     168,   185,   186,   188,    17,   116,   160,   168,   168,   179,
      73,   168,   150,   168,    31,   121,   121,   121,   121,   147,
      91,   117,    94,   115,   117,   183,    99,   117,   117,    90,
      92,    91,   115,   121,   115,   121,    34,   170,   173,   174,
     179,   150,   150,    17,   168,    11,   119,   150,   168,   168,
      99,   168,    85,   115,    85,   185,   115,   168,   117,   121,
      94,   172,     9,    18,   136,   164,   150,   152,    92,   150,
     173,   183,   153,   168,   150,   168,   150,   120,   168,   121,
     172,   121,   150,   153,   168,   136,   121
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   123,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   126,   127,   127,   127,   128,   129,   130,   131,
     131,   132,   133,   134,   134,   135,   135,   136,   136,   136,
     137,   138,   139,   140,   140,   140,   140,   140,   140,   141,
     141,   142,   142,   143,   143,   144,   144,   145,   145,   146,
     146,   146,   147,   147,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   149,   150,   150,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   152,   152,   152,
     153,   154,   154,   154,   155,   156,   157,   158,   158,   158,
     159,   160,   160,   161,   161,   162,   162,   163,   163,   164,
     164,   165,   165,   166,   166,   166,   166,   166,   166,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   169,   169,   170,
     170,   171,   171,   172,   172,   173,   173,   173,   173,   173,
     174,   174,   174,   174,   174,   175,   176,   176,   176,   177,
     178,   179,   179,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   182,   182,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   185,   185,   186,   186,   187,   188,   188,   189
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       0,     2,     3,     0,     2,     4,     3,     2,     2,     1,
       3,     1,     2,     1,     3,     1,     3,     0,     2,     4,
       4,     5,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     1,     1,     4,     1,     3,     0,     3,     0,
       2,     3,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     3,     7,     1,     1,     2,     2,
       1,     1,     1,     2,     2,     1,     1,     0,     2,     2,
       3,     1,     3,     2,     2,     2,     1,     1,     2,     3,
       4,     0,     2,     5,     3,     4,     5,     4,     5,     1,
       3,     1,     5,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     3,     3,     3,     4,     3,     4,     4,     3,     5,
       1,     1,     1,     1,     3,     3,     2,     0,     3,     1,
       3,     1,     3,     1,     1,     1,     3,     5,     3,     3,
       4,     6,     5,     4,     3,     4,     0,     3,     5,     5,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     1,     4,
       2,     3,     2,     3,     2,     4,     6,     1,     4,     5,
       1,     1,     3,     5,     1,     3,     6,     1,     3,     6
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
          case 85: /* NAME  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1909 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 125: /* character_sequence  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1915 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 126: /* string_constant  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1921 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 127: /* string_builder_body  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1927 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 128: /* string_builder  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1933 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 136: /* expression_else  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1939 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* expression_if_then_else  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1945 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* expression_for_loop  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1951 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* expression_while_loop  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1957 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* annotation_argument  */
#line 81 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 1963 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 141: /* annotation_argument_list  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 1969 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 142: /* annotation_declaration_name  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1975 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* annotation_declaration  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 1981 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* annotation_list  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1987 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 145: /* optional_annotation_list  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1993 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* optional_function_argument_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 1999 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* optional_function_type  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2005 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* function_name  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2011 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* expression_block  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2017 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expression_any  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2023 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expressions  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2029 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* expr_pipe  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2035 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* name_in_namespace  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2041 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expression_delete  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2047 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expr_new  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2053 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* expression_break  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2059 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expression_return  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2065 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* expression_try_catch  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2071 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* expression_let  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2077 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* expr_sizeof  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2083 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* expr_typename  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2089 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* expr_list  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2095 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* expr_block  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2101 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* expr_numeric_const  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2107 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expr_assign  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2113 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expr  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2119 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* struct_variable_declaration_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2125 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* variable_declaration_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2131 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* function_argument_list  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2137 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* variable_declaration  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2143 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* let_variable_declaration  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2149 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* enum_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2155 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* variable_name_list  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2161 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* structure_type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2167 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* auto_type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2173 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* type_declaration  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2179 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* make_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2185 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* make_struct_fields  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2191 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* make_struct_dim  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2197 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* make_struct_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2203 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* make_dim  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2209 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* make_dim_decl  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2215 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 219 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); }
#line 2503 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 11:
#line 220 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2509 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 224 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2515 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 228 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2523 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 231 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2534 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 237 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2544 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 245 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2562 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 261 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2585 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 291 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( auto mod = Module::require(*(yyvsp[0].s)) ) {
            g_Program->addModule(mod);
        } else {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2599 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 312 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2607 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 315 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2615 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 321 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 2621 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 322 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2627 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 323 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2636 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 330 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2645 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 337 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 2659 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 349 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile();
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 2671 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 359 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2677 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 360 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2683 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 361 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i));  }
#line 2689 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 362 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)));  }
#line 2695 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 363 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true);  }
#line 2701 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 364 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false);  }
#line 2707 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 368 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 2717 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 373 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 2727 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 381 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2733 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 382 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 2739 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 386 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 2751 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 393 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 2765 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 405 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2774 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 409 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2783 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 416 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 2789 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 417 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 2795 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 421 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2801 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 422 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2807 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 423 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 2813 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 427 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(yylloc.first_column,yylloc.first_line);
    }
#line 2822 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 431 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 2831 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 438 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2837 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 439 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 2843 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 2849 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 2855 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 2861 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 443 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 2867 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 444 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 2873 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 2879 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 446 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 2885 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 2891 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 2897 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 2903 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 450 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 2909 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 451 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 2915 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 452 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 2921 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 453 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 2927 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 454 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 2933 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 455 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 2939 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 456 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 2945 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 457 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 2951 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 458 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 2957 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 459 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 2963 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 460 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 2969 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 461 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 2975 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 462 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 2981 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 463 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 2987 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 2993 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 465 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 2999 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 466 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3005 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 471 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3075 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 539 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3083 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 542 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
    }
#line 3094 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 551 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3100 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 552 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3106 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 553 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3112 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 554 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3118 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 555 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3124 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 556 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3130 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 557 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3136 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 558 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3142 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 559 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3148 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 560 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3154 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 561 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3160 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 565 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBlock(); }
#line 3166 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 566 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3177 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 572 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3185 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 578 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3201 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3207 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 593 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3218 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 599 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3224 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 603 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3232 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 609 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 3240 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 615 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3246 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 619 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3254 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 622 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3262 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 625 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3272 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 633 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3280 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 639 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3286 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 640 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3292 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 644 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3323 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 670 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pLet = new ExprLet();
        pLet->scoped = false;
        pLet->inScope = (yyvsp[-1].b);
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
                    yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 3353 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 698 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3361 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 701 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3369 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 707 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3377 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 710 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3385 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 716 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3393 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 719 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3401 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 725 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3411 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 731 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3463 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 781 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3469 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 782 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3475 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3481 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 784 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3487 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 785 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3493 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3499 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 790 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3505 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 791 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3511 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 792 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3517 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 793 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3523 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 794 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3529 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 795 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3535 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 796 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3541 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 797 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3547 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 798 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3553 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 799 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3559 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 803 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3565 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 804 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3571 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 805 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3577 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 806 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3583 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 807 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3589 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 808 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3595 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 809 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3601 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 810 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3607 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 811 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3613 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 812 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3619 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 813 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3625 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 814 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3631 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 815 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3637 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 816 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3643 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 817 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3649 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 818 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3655 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 819 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3661 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 820 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3667 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 821 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3673 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 822 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3679 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 823 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3685 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 824 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3691 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 825 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3697 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 826 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3703 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 827 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3709 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 828 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3715 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 829 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3721 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 830 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3727 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 831 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3733 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 832 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3739 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 833 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3745 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 834 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3751 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 835 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3757 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 836 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3763 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 837 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 3772 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 841 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 3781 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 3789 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 848 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 3797 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 851 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3803 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 852 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3809 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 853 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 3817 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 856 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3823 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 857 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3829 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 858 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3835 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 859 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3841 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 860 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3856 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 870 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3871 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 880 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3907 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 914 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); }
#line 3913 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 915 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl)); }
#line 3919 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 919 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3925 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 920 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3931 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3937 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 925 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3943 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3949 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 930 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3955 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 934 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 3964 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 938 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 3972 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 941 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3981 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 945 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3991 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4001 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 958 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4009 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 961 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4018 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 965 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4027 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 969 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4037 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 974 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4047 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 982 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4072 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1005 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4080 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1008 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4093 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1016 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4106 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1027 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4121 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1040 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4166 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1083 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4177 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1089 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4187 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1097 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 4193 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1098 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 4199 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1099 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 4205 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1100 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 4211 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1101 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 4217 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1102 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 4223 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1103 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 4229 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1104 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 4235 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1105 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 4241 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1106 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 4247 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1107 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 4253 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1108 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 4259 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1109 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 4265 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1110 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 4271 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1111 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 4277 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1112 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 4283 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1113 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 4289 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1114 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 4295 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1115 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 4301 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1116 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 4307 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1120 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4319 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1130 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4328 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1134 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4339 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1143 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4345 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1144 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4351 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1145 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4357 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1146 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4366 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1150 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4376 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1155 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4386 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1160 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4396 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1165 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4406 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1170 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4415 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1174 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4425 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1179 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4436 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1185 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4445 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1189 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4455 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1194 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4480 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1217 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4486 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1218 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4492 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1222 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 4504 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1229 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 4515 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1238 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 4525 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1243 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4534 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1250 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4544 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1258 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 4554 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1263 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4563 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1270 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4573 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 4577 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
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
#line 1277 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


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


