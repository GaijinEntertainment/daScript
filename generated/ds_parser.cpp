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
#line 35 "src/parser/ds_parser.ypp" /* yacc.c:338  */

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
    #include "daScript/ast/ast_generate.h"
    
    namespace das {
        extern ProgramPtr			g_Program;
        extern FileAccessPtr        g_Access;
        extern vector<FileInfo *>	g_FileAccessStack;
    }
    using namespace das;
    
    struct VariableDeclaration {
        VariableDeclaration ( const LineInfo & a, vector<string> * n, TypeDecl * t, Expression * i )
            : at(a), pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () { delete pNameList; delete pTypeDecl; delete pInit; }
        LineInfo        at;
        vector<string>  *pNameList;
        TypeDecl        *pTypeDecl;
        Expression      *pInit;
        bool            move_to_init = false;
        bool            override = false;
    };
    
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 155 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

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
    _TYPEDEF = 290,
    _WITH = 291,
    _CAST = 292,
    _OVERRIDE = 293,
    _TBOOL = 294,
    _TVOID = 295,
    _TSTRING = 296,
    _TAUTO = 297,
    _TINT = 298,
    _TINT2 = 299,
    _TINT3 = 300,
    _TINT4 = 301,
    _TUINT = 302,
    _TUINT2 = 303,
    _TUINT3 = 304,
    _TUINT4 = 305,
    _TFLOAT = 306,
    _TFLOAT2 = 307,
    _TFLOAT3 = 308,
    _TFLOAT4 = 309,
    _TRANGE = 310,
    _TURANGE = 311,
    _TBLOCK = 312,
    _TINT64 = 313,
    _TUINT64 = 314,
    _TYPENAME = 315,
    _TDOUBLE = 316,
    _TFUNCTION = 317,
    _TLAMBDA = 318,
    ADDEQU = 319,
    SUBEQU = 320,
    DIVEQU = 321,
    MULEQU = 322,
    MODEQU = 323,
    ANDEQU = 324,
    OREQU = 325,
    XOREQU = 326,
    ADDADD = 327,
    SUBSUB = 328,
    LEEQU = 329,
    GREQU = 330,
    EQUEQU = 331,
    NOTEQU = 332,
    RARROW = 333,
    LARROW = 334,
    QQ = 335,
    QDOT = 336,
    LPIPE = 337,
    LBPIPE = 338,
    RPIPE = 339,
    INTEGER = 340,
    LONG_INTEGER = 341,
    UNSIGNED_INTEGER = 342,
    UNSIGNED_LONG_INTEGER = 343,
    FLOAT = 344,
    DOUBLE = 345,
    NAME = 346,
    BEGIN_STRING = 347,
    STRING_CHARACTER = 348,
    END_STRING = 349,
    BEGIN_STRING_EXPR = 350,
    END_STRING_EXPR = 351,
    UNARY_MINUS = 352,
    UNARY_PLUS = 353,
    PRE_INC = 354,
    PRE_DEC = 355,
    POST_INC = 356,
    POST_DEC = 357,
    COLCOL = 358
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 58 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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
    Structure *                     pStructure;

#line 296 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   3328

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  287
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  507

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   358

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   115,     2,     2,   129,   110,   103,     2,
     123,   124,   108,   107,    97,   106,   120,   109,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   100,   128,
     104,    98,   105,    99,   111,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   121,     2,   122,   102,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   126,   101,   127,   114,     2,     2,     2,
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
      95,    96,   112,   113,   116,   117,   118,   119,   125
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   224,   224,   226,   227,   228,   229,   230,   231,   232,
     233,   237,   238,   242,   246,   249,   255,   263,   279,   300,
     304,   305,   309,   319,   323,   324,   328,   331,   337,   338,
     339,   346,   353,   365,   375,   384,   385,   386,   387,   388,
     389,   393,   398,   406,   407,   411,   418,   430,   434,   441,
     442,   446,   447,   448,   452,   456,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   495,   564,   567,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     592,   596,   602,   608,   622,   623,   629,   633,   639,   642,
     645,   649,   655,   659,   662,   665,   673,   679,   680,   684,
     710,   738,   744,   747,   753,   756,   762,   765,   771,   776,
     827,   828,   829,   830,   831,   832,   836,   837,   838,   839,
     840,   841,   842,   843,   844,   845,   850,   855,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   904,   908,   911,   914,   915,   916,
     917,   918,   921,   922,   923,   924,   925,   926,   927,   937,
     947,   981,   982,   986,   993,   996,  1003,  1004,  1008,  1009,
    1013,  1014,  1018,  1022,  1025,  1029,  1034,  1042,  1045,  1049,
    1053,  1058,  1066,  1089,  1092,  1100,  1111,  1122,  1135,  1136,
    1140,  1176,  1238,  1244,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,  1271,  1275,  1285,  1289,  1298,  1299,  1300,
    1301,  1305,  1310,  1315,  1320,  1325,  1329,  1334,  1340,  1344,
    1349,  1358,  1362,  1367,  1376,  1380,  1385,  1397,  1398,  1402,
    1409,  1418,  1423,  1430,  1435,  1444,  1449,  1456
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
  "_TYPEDEF", "_WITH", "_CAST", "_OVERRIDE", "_TBOOL", "_TVOID",
  "_TSTRING", "_TAUTO", "_TINT", "_TINT2", "_TINT3", "_TINT4", "_TUINT",
  "_TUINT2", "_TUINT3", "_TUINT4", "_TFLOAT", "_TFLOAT2", "_TFLOAT3",
  "_TFLOAT4", "_TRANGE", "_TURANGE", "_TBLOCK", "_TINT64", "_TUINT64",
  "_TYPENAME", "_TDOUBLE", "_TFUNCTION", "_TLAMBDA", "ADDEQU", "SUBEQU",
  "DIVEQU", "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "ADDADD",
  "SUBSUB", "LEEQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW", "LARROW", "QQ",
  "QDOT", "LPIPE", "LBPIPE", "RPIPE", "INTEGER", "LONG_INTEGER",
  "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER", "FLOAT", "DOUBLE", "NAME",
  "BEGIN_STRING", "STRING_CHARACTER", "END_STRING", "BEGIN_STRING_EXPR",
  "END_STRING_EXPR", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "'.'", "'['", "']'", "'('", "')'", "COLCOL", "'{'", "'}'", "';'", "'$'",
  "$accept", "program", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "options_declaration",
  "require_declaration", "require_list", "require_module",
  "expect_declaration", "expect_list", "expect_error", "expression_else",
  "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break", "expression_return",
  "expression_try_catch", "let_scope", "expression_let", "expr_cast",
  "expr_sizeof", "expr_typename", "expr_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_method_call", "expr",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "variable_declaration_list",
  "function_argument_list", "copy_or_move", "variable_declaration",
  "let_variable_declaration", "global_let", "enum_list",
  "alias_declaration", "enum_declaration", "optional_structure_parent",
  "structure_name", "structure_declaration", "variable_name_list",
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
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    44,    61,    63,
      58,   124,    94,    38,    60,    62,    45,    43,    42,    47,
      37,    64,   352,   353,   126,    33,   354,   355,   356,   357,
      46,    91,    93,    40,    41,   358,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -304

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-304)))

#define YYTABLE_NINF -137

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -304,    32,  -304,  -102,   -21,   -26,    47,    77,   100,    -8,
    -304,  -304,  -304,   169,  -304,  -304,  -304,  -304,  -304,  -304,
     -16,  -304,    59,    48,    79,  -304,  -304,    99,  -304,    52,
     109,  -304,    91,   130,   103,  -304,    37,  -304,   132,    -1,
     -27,   139,   -21,   144,   -26,    47,  -304,  2166,   151,  -304,
     -21,    -8,  -304,   145,   108,  2740,   221,   228,  -304,   148,
    -304,  -304,   176,   152,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,   -70,   172,   177,  -304,  -304,
    -304,   159,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,   181,  -304,  -304,  -304,
     183,   194,  -304,  -304,  -304,  -304,    87,  -304,    25,  -304,
     -38,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,   -40,   190,  -304,  -304,   143,  -304,    35,   -29,  -304,
    2166,  2166,   204,  2059,  2059,  2059,  -304,  -304,  -304,     8,
     216,  -304,  -304,    -7,  -304,    81,  -304,  2166,   178,  -304,
     212,  -304,  2166,  1968,  -304,  -304,   220,  -304,    66,    96,
     182,   190,    98,   190,   133,   190,   134,  -304,  -304,   185,
    -304,  -304,   176,    87,  -304,  -304,  -304,    85,  -304,  -304,
     193,  2112,  -304,   195,   205,   196,   198,  1968,  1968,  -304,
    -304,  -304,  -304,  -304,  -304,   -57,  -304,  1968,  1968,   -38,
    1968,  1968,   201,  1968,   202,  -304,  -304,  -304,   -28,  -304,
    -304,  -304,  -304,  -304,  -304,   231,  -304,  2304,   206,  -304,
    -304,  -304,   207,  -304,  2166,  -304,   219,  -304,   238,  -304,
     239,  -304,  -304,  -304,   541,  1968,   394,    51,  -304,  1968,
    2166,  1024,  1142,   191,   191,   165,   191,   191,  -304,   191,
     191,  2166,  2467,   148,  -304,  1260,   -35,  1968,  1968,  1968,
    1968,  1968,  1968,  1968,  1968,  -304,  -304,  1968,  1968,  1968,
    1968,   245,  1968,  1968,   255,  1968,  1968,  1968,  1968,  1968,
    1968,  1968,  1968,  1968,  1968,  1968,  1968,  1968,  1968,   256,
    1968,  1378,  -304,   153,  -304,  -304,  -304,  -304,   -10,  1968,
    1968,   176,  1496,  -304,   178,  1968,  1968,   325,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,   229,   234,   237,  -304,  -304,
     -64,  2362,  2935,  2166,  2520,  1614,  2573,   283,  2166,  2626,
    -304,  2679,  -304,  1968,   287,   788,  -304,   190,  -304,    33,
    2935,  -304,  2935,  2935,  2935,  2935,  2935,  2935,  2935,  2935,
    3163,  3163,  3147,  3147,   260,  2935,   191,  -304,   -45,   -45,
    2935,  2835,  3035,  3085,  3103,  3163,  3163,  3207,  3207,   -32,
     -32,   -32,  -304,  2784,  -304,    65,  -304,   350,   176,   176,
    2412,  2412,    -5,  1968,  2935,   374,  2935,  2412,   261,  -304,
    -304,  -304,  -304,     3,  -304,  -304,    70,  -304,  1968,    16,
    -304,  -304,  2885,   217,   906,   -56,  1732,  2935,   292,    11,
      57,   178,  1968,  -304,  1850,  1968,  -304,  -304,  -304,   142,
    -304,  -304,   186,  -304,    68,  1968,  2935,   178,  -304,  -304,
    -304,  -304,  3163,  -304,  -304,  1968,   301,   303,   273,   301,
     277,  1968,  -304,  2935,  -304,    80,  2985,   178,   176,  2166,
    1968,   178,  1968,  -304,   -52,  -304,   670,  2935,     0,    90,
       0,  -304,   292,  -304,  2935,  -304,  -304,  -304,   -18,  -304,
    2174,  -304,  2412,  -304,  -304,   278,  1968,  -304,  1968,  -304,
      68,  -304,  2935,  -304,  2239,  -304,  -304
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,     0,     0,     0,     0,     0,     0,     0,
       9,     8,     7,     0,     6,     5,    10,     4,     3,   204,
       0,    41,    18,    26,    23,    24,    22,    19,    20,     0,
       0,    44,   104,     0,    45,    47,     0,    43,     0,     0,
     201,     0,     0,     0,     0,     0,   223,     0,     0,   106,
       0,     0,    50,   228,     0,     0,     0,     0,    56,    51,
     202,   222,     0,     0,    39,    40,    37,    38,    36,    11,
      35,    42,    27,    25,    21,     0,     0,     0,   234,   250,
     235,   255,   236,   238,   239,   240,   241,   243,   244,   245,
     246,   247,   248,   249,   251,   252,   268,   237,   242,   253,
     271,   274,   254,   257,   259,   258,   226,   105,     0,    48,
       0,   230,   204,    59,    60,    62,    61,    63,    64,    65,
      66,    83,    84,    76,    77,    74,    75,    79,    80,    78,
      72,    73,    68,    67,    69,    70,    71,    58,    57,    81,
      82,     0,    54,   232,   203,   212,   205,     0,     0,   227,
       0,     0,     0,    51,    51,    51,   261,   265,   263,     0,
       0,    46,   229,   201,    52,     0,   208,     0,     0,   211,
       0,   210,     0,     0,    12,    13,     0,   224,     0,     0,
       0,    54,     0,    54,     0,    54,     0,   262,   264,     0,
     231,    53,     0,    55,   100,    85,   233,   213,   153,   154,
       0,     0,   148,     0,     0,     0,     0,     0,     0,   130,
     132,   131,   133,   134,   135,   149,    14,     0,     0,     0,
       0,     0,     0,     0,    49,   151,   128,   216,     0,   195,
     193,   192,   194,   197,   150,     0,   196,   215,     0,   152,
     277,   278,     0,   266,     0,   256,     0,   269,     0,   272,
       0,   275,   260,   209,     0,     0,     0,   108,   111,     0,
       0,     0,     0,   174,   175,    11,   158,   157,   182,   156,
     155,     0,     0,    51,   200,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   176,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,   270,   273,   276,   102,   117,     0,
       0,     0,   113,   112,     0,     0,     0,    86,    88,    98,
      95,    93,    94,   101,    89,     0,     0,     0,    99,    92,
       0,   136,   214,     0,     0,     0,     0,     0,     0,     0,
     188,     0,    17,     0,    15,     0,   178,    54,   183,     0,
     126,   103,   141,   142,   144,   143,   145,   138,   139,   140,
     169,   170,   167,   168,     0,   159,   190,   181,   198,   199,
     137,     0,   172,   173,   171,   165,   166,   161,   160,   162,
     163,   164,   180,     0,   185,     0,   267,     0,     0,     0,
       0,     0,     0,     0,   114,     0,   107,     0,     0,    91,
      96,    97,    90,     0,   122,   109,     0,   187,     0,     0,
     124,   189,     0,   149,     0,     0,     0,   285,   281,     0,
       0,     0,     0,   184,     0,     0,   179,   186,   118,     0,
     206,   120,     0,    33,    28,     0,   115,     0,    34,   100,
     123,   110,   121,   125,    16,     0,     0,     0,     0,     0,
       0,     0,   129,   127,   146,     0,   191,     0,     0,     0,
       0,     0,     0,    31,     0,   116,     0,   279,     0,     0,
       0,   283,   282,   287,   286,   147,   119,   207,     0,   221,
     136,    29,     0,    32,    87,     0,     0,   217,     0,   220,
      28,   284,   280,   219,   136,    30,   218
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -304,  -304,   136,  -304,  -304,  -304,  -304,  -304,  -304,   357,
    -304,  -304,   359,   -95,  -304,  -304,  -304,  -304,   369,   362,
    -304,   363,  -304,   189,  -138,  -175,  -304,  -304,  -154,  -304,
     -33,  -170,    -9,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -298,   141,  -304,  -249,  -304,    71,  -304,
    -304,   306,  -304,  -304,  -143,  -137,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -303,   -46,  -304,  -304,  -128,   218,
     -39,   -34,  -304,  -304,  -304
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   147,    70,   265,   225,    10,    11,    27,    28,
      12,    24,    25,   473,   329,   330,   331,   332,    21,    22,
      34,    35,    36,    13,   142,   168,    59,    14,   226,   333,
     254,   334,   228,   335,   229,   336,   337,   338,   399,   339,
     230,   231,   232,   359,   233,   234,   235,   236,   360,    62,
      63,    40,   439,   165,   455,   144,   441,    15,    75,    16,
      17,   111,    54,    18,   145,   238,   104,   105,   106,   239,
     428,   429,   240,   430,   241
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,   103,   173,   227,   166,   340,   246,   397,   248,   156,
     250,    60,   445,   395,   195,   181,   183,   185,   402,   276,
      31,   148,   178,   179,    19,   182,   184,   186,    55,   189,
     156,    60,     2,   291,  -104,   187,   294,     3,   102,   193,
     285,   286,    37,   156,   197,   432,   291,   416,   293,   294,
     295,   143,   296,    32,   255,   253,     4,   149,     5,    23,
       6,   169,     7,   274,   412,   271,  -104,     8,    48,   176,
      20,    56,    57,   257,   194,   309,   310,   471,   156,   169,
     171,   157,    41,    32,   164,   158,   472,    33,   309,   310,
      58,   194,   170,   156,   224,   275,   442,   159,   171,   177,
      61,   162,   157,   160,   103,   103,   158,   103,   103,   103,
     497,   188,   156,   398,   156,   157,   313,    33,   159,   158,
     190,   103,    42,   156,   160,   156,   103,   450,   174,   175,
     432,   159,   347,   458,    51,   357,   465,   160,    26,   459,
     453,   102,   102,   355,   102,   102,   102,   474,    43,   161,
     157,    64,    65,     9,   158,   103,    42,   433,   102,    52,
     156,   156,   432,   102,   169,   157,   159,   432,    29,   158,
     405,   243,   160,    38,   345,    39,    44,   432,    46,   460,
     156,   159,   431,   171,   157,   461,   157,   160,   158,   437,
     158,    30,   102,   244,   451,   157,    45,   157,   103,   158,
     159,   158,   159,   247,   485,   191,   160,    47,   160,   192,
     268,   159,   495,   159,   103,   413,    48,   160,   459,   160,
     419,    49,   169,    53,    66,   103,    50,   340,    67,    72,
      68,    69,   157,   157,   112,   102,   158,   158,   249,   251,
     170,   171,   107,   172,   237,   110,   443,   444,   159,   159,
     139,   102,   157,   448,   160,   160,   158,   140,   396,   352,
     353,   440,   102,   285,   286,   169,   467,   143,   159,   291,
     468,   141,   294,   295,   160,   296,   150,   462,   263,   264,
     146,   151,   152,   170,   171,   153,   469,   154,   266,   267,
     167,   269,   270,   475,   272,   180,   169,   103,   155,   470,
     489,   189,   103,   196,   194,   242,   245,   252,  -104,   260,
     156,   309,   310,   486,   276,   171,   256,   491,   259,   261,
     493,   262,   271,     9,   314,   341,   342,   344,   503,   311,
     346,   487,   349,   351,   102,   312,   374,   496,   500,   102,
    -104,   488,    48,   315,   316,   498,   377,   392,   362,   363,
     364,   365,   366,   367,   368,   369,   408,   409,   370,   371,
     372,   373,   410,   375,   376,   411,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     174,   393,   157,   434,   438,   447,   158,   449,   418,   457,
     400,   401,   478,   404,   480,   481,   406,   407,   159,   483,
     501,   354,    74,    73,   160,   505,   198,   199,   200,   201,
     343,    71,   108,   273,   109,   202,   476,   361,   163,   258,
     482,     0,   479,   103,   422,     0,   427,   203,     0,     0,
       0,   204,     0,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,   205,    99,     0,   206,     0,     0,
     102,     0,     0,     0,     0,     0,   207,   208,     0,     0,
       0,     0,     0,     0,   446,     0,     0,     0,     0,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,   452,
       0,     0,     0,     0,     0,   270,     0,   272,     0,     0,
     217,   218,     0,   463,     0,   219,   466,     0,   220,   221,
       0,     0,     0,     0,     0,   222,     0,   223,     0,    33,
     194,     0,     0,   224,     0,     0,   477,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   317,   492,     0,     0,   318,   341,   319,   320,
       0,   321,     0,   198,   199,   200,   201,     0,     0,     0,
       0,   322,   202,   323,   324,     0,     0,   502,     0,   504,
       0,     0,     0,   325,   203,     0,     0,   326,   204,     0,
      78,    79,    80,     0,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,     0,    97,
      98,   205,    99,     0,   206,     0,     0,     0,     0,     0,
       0,     0,     0,   207,   208,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209,   210,   211,   212,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   217,   218,     0,
       0,     0,   219,     0,     0,   220,   221,     0,     0,     0,
       0,     0,   222,     0,   223,     0,    33,   194,   327,   328,
     224,   317,     0,     0,     0,   318,     0,   319,   320,     0,
     321,     0,   198,   199,   200,   201,     0,     0,     0,     0,
     322,   202,   323,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   203,     0,     0,   326,   204,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     205,    99,     0,   206,     0,     0,     0,     0,     0,     0,
       0,     0,   207,   208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   218,     0,     0,
       0,   219,     0,     0,   220,   221,     0,     0,     0,     0,
       0,   222,     0,   223,     0,    33,   194,   494,   328,   224,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,   203,     0,     0,     0,   204,     0,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,   205,    99,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   211,   212,   213,   214,   423,
     216,     0,     0,     0,     0,     0,     0,   157,     0,     0,
       0,   158,     0,     0,   217,   218,     0,     0,     0,   219,
       0,     0,   220,   424,     0,     0,     0,     0,     0,   425,
       0,   426,     0,    33,   194,     0,     0,   224,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,   187,     0,     0,     0,     0,     0,   203,
       0,     0,     0,   204,     0,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,   205,    99,     0,   206,
       0,     0,     0,     0,     0,     0,     0,     0,   207,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   188,
       0,     0,   217,   218,     0,     0,     0,   219,     0,     0,
     220,   221,     0,     0,     0,     0,     0,   222,     0,   223,
       0,    33,   194,     0,     0,   224,   198,   199,   200,   201,
     348,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   203,     0,     0,
       0,   204,     0,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,   205,    99,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,   207,   208,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,   218,     0,     0,     0,   219,     0,     0,   220,   221,
       0,     0,     0,     0,     0,   222,     0,   223,     0,    33,
     194,     0,     0,   224,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,     0,     0,   204,
       0,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,   205,    99,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   209,   210,   211,
     212,   213,   214,   215,   216,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   218,
       0,     0,     0,   219,     0,     0,   220,   221,     0,     0,
       0,     0,     0,   222,     0,   223,   350,    33,   194,     0,
       0,   224,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   203,     0,     0,     0,   204,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     205,    99,     0,   206,     0,     0,     0,     0,     0,     0,
       0,     0,   207,   208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   218,     0,     0,
       0,   219,     0,     0,   220,   221,     0,     0,     0,     0,
       0,   222,     0,   223,   358,    33,   194,     0,     0,   224,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,   204,     0,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,   205,    99,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   218,     0,     0,     0,   219,
       0,     0,   220,   221,     0,     0,     0,     0,     0,   222,
       0,   223,   394,    33,   194,     0,     0,   224,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   203,
       0,     0,     0,   204,     0,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,   205,    99,     0,   206,
       0,     0,     0,     0,     0,     0,     0,     0,   207,   208,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,   209,   210,   211,   212,   213,   214,   215,   216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   218,     0,     0,     0,   219,     0,     0,
     220,   221,     0,     0,     0,     0,     0,   222,     0,   223,
       0,    33,   194,     0,     0,   224,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   203,     0,     0,
       0,   204,     0,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,   205,    99,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,   207,   208,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,   218,     0,     0,     0,   219,     0,     0,   220,   221,
       0,     0,     0,     0,     0,   222,     0,   223,   415,    33,
     194,     0,     0,   224,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,     0,     0,   204,
       0,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,   205,    99,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   209,   210,   211,
     212,   213,   214,   215,   216,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   218,
       0,     0,     0,   219,     0,     0,   220,   221,     0,     0,
       0,     0,     0,   222,     0,   223,   456,    33,   194,     0,
       0,   224,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   203,     0,     0,     0,   204,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     205,    99,     0,   206,     0,     0,     0,     0,     0,     0,
       0,     0,   207,   208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   218,     0,     0,
       0,   219,     0,     0,   220,   221,     0,     0,     0,     0,
       0,   222,     0,   223,   464,    33,   194,     0,     0,   224,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,   204,     0,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,   205,    99,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   218,     0,     0,    76,   219,
       0,     0,   220,   221,    77,     0,     0,     0,     0,   222,
       0,   223,     0,    33,   194,     0,     0,   224,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,     0,
      99,   100,   101,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,    99,   100,   101,     0,     0,     0,     0,
       0,     0,   141,     0,    33,    76,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,     0,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,    99,   100,   101,
       0,     0,     0,   222,     0,     0,     0,    33,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,    32,   296,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,   298,     0,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,     0,     0,     0,     0,     0,
       0,    33,     0,     0,   309,   310,     0,     0,     0,     0,
       0,     0,   499,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,     0,   296,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   298,     0,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,     0,     0,     0,     0,   506,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,  -136,   296,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,   298,     0,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   310,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,     0,   296,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,   298,     0,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   310,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,     0,   296,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   310,     0,     0,     0,     0,   194,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
       0,   296,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   310,     0,
       0,   356,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,     0,   296,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   310,     0,     0,   414,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,     0,   296,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   310,     0,     0,   417,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,     0,
     296,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   310,     0,     0,
     420,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,     0,   296,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     310,     0,     0,   421,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,     0,   296,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   310,   436,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   435,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,   298,     0,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   285,   286,   287,
     288,   289,   290,   291,     0,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   285,   286,   287,
     288,   289,   290,   291,     0,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   300,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   285,   286,   287,
     288,   289,   290,   291,     0,   293,   294,   295,     0,   296,
       0,     0,     0,     0,     0,   285,   286,   287,   288,   289,
     290,   291,     0,   293,   294,   295,     0,   296,   301,   302,
     303,   304,   305,   306,   307,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   310,   302,   303,   304,
     305,   306,   307,   308,     0,     0,     0,     0,     0,   285,
     286,   287,   288,   309,   310,   291,     0,   293,   294,   295,
       0,   296,     0,     0,     0,   285,   286,     0,     0,     0,
       0,   291,     0,   293,   294,   295,     0,   296,     0,     0,
       0,   302,   303,   304,   305,   306,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   310,   304,
     305,   306,   307,   308,     0,     0,     0,     0,     0,   285,
     286,     0,     0,   309,   310,   291,     0,   293,   294,   295,
       0,   296,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   310
};

static const yytype_int16 yycheck[] =
{
       9,    47,   145,   173,   141,   254,   181,    17,   183,    27,
     185,    38,    17,   311,   168,   153,   154,   155,   321,    83,
      28,    91,   150,   151,   126,   153,   154,   155,    29,    85,
      27,    38,     0,    78,    91,    27,    81,     5,    47,   167,
      72,    73,    51,    27,   172,    97,    78,   345,    80,    81,
      82,    91,    84,    91,   197,   192,    24,   127,    26,    85,
      28,    79,    30,    91,   128,   121,   123,    35,   125,    98,
      91,    72,    73,   201,   126,   120,   121,     9,    27,    79,
      98,    99,    98,    91,   124,   103,    18,   125,   120,   121,
      91,   126,    97,    27,   129,   123,   399,   115,    98,   128,
     127,   110,    99,   121,   150,   151,   103,   153,   154,   155,
     128,   103,    27,   123,    27,    99,   244,   125,   115,   103,
     127,   167,    97,    27,   121,    27,   172,   124,    93,    94,
      97,   115,   260,   122,    97,   273,   434,   121,    91,   128,
     124,   150,   151,   271,   153,   154,   155,   445,   100,   124,
      99,    12,    13,   121,   103,   201,    97,   124,   167,   122,
      27,    27,    97,   172,    79,    99,   115,    97,    91,   103,
     324,   105,   121,     4,   123,     6,    97,    97,   126,   122,
      27,   115,   357,    98,    99,   128,    99,   121,   103,   124,
     103,    91,   201,    97,   124,    99,    97,    99,   244,   103,
     115,   103,   115,   105,   124,   124,   121,    98,   121,   128,
     219,   115,   122,   115,   260,   343,   125,   121,   128,   121,
     348,    91,    79,    91,    85,   271,   123,   476,    89,    85,
      91,    92,    99,    99,   126,   244,   103,   103,   105,   105,
      97,    98,    91,   100,   173,   100,   400,   401,   115,   115,
      29,   260,    99,   407,   121,   121,   103,    29,   105,    94,
      95,   398,   271,    72,    73,    79,   124,    91,   115,    78,
     128,   123,    81,    82,   121,    84,   104,   431,   207,   208,
     128,   104,   123,    97,    98,   104,   100,   104,   217,   218,
     100,   220,   221,   447,   223,    91,    79,   343,   104,   442,
     470,    85,   348,    91,   126,    85,   124,   122,    91,   104,
      27,   120,   121,   467,    83,    98,   123,   471,   123,   123,
     474,   123,   121,   121,   105,   254,   255,   256,   498,   123,
     259,   468,   261,   262,   343,   128,    91,   480,   492,   348,
     123,   469,   125,   105,   105,   488,    91,    91,   277,   278,
     279,   280,   281,   282,   283,   284,    31,   128,   287,   288,
     289,   290,   128,   292,   293,   128,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
      93,   310,    99,   123,    34,    11,   103,   126,   105,    97,
     319,   320,    91,   322,    91,   122,   325,   326,   115,   122,
     122,   265,    45,    44,   121,   500,    12,    13,    14,    15,
      16,    42,    50,   224,    51,    21,   449,   276,   112,   201,
     459,    -1,   456,   469,   353,    -1,   355,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    61,    -1,    63,    -1,    -1,
     469,    -1,    -1,    -1,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,   403,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,   418,
      -1,    -1,    -1,    -1,    -1,   424,    -1,   426,    -1,    -1,
     106,   107,    -1,   432,    -1,   111,   435,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,    -1,   129,    -1,    -1,   455,    -1,    -1,    -1,
      -1,    -1,   461,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   470,     1,   472,    -1,    -1,     5,   476,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    -1,    -1,   496,    -1,   498,
      -1,    -1,    -1,    32,    33,    -1,    -1,    36,    37,    -1,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,   126,   127,   128,
     129,     1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    36,    37,    -1,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,   127,   128,   129,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    61,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,    -1,   129,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    61,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,   106,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,    -1,   129,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    61,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,    -1,   129,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,    -1,
      -1,   129,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,    -1,    -1,   129,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    61,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,    -1,    -1,   129,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    61,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,    -1,   129,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    61,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,    -1,    -1,   129,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,    -1,
      -1,   129,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,    -1,    -1,   129,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    61,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    19,   111,
      -1,    -1,   114,   115,    25,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,    -1,   129,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    91,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,   128,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
      -1,   124,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,    -1,   124,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,    -1,    -1,   124,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
     124,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,    -1,   124,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,    -1,   114,   115,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    99,    -1,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,    72,    73,    74,
      75,    76,    77,    78,    -1,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      77,    78,    -1,    80,    81,    82,    -1,    84,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,   120,   121,    78,    -1,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    -1,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   106,
     107,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    72,
      73,    -1,    -1,   120,   121,    78,    -1,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   131,     0,     5,    24,    26,    28,    30,    35,   121,
     136,   137,   140,   153,   157,   187,   189,   190,   193,   126,
      91,   148,   149,    85,   141,   142,    91,   138,   139,    91,
      91,    28,    91,   125,   150,   151,   152,   162,     4,     6,
     181,    98,    97,   100,    97,    97,   126,    98,   125,    91,
     123,    97,   122,    91,   192,    29,    72,    73,    91,   156,
      38,   127,   179,   180,    12,    13,    85,    89,    91,    92,
     133,   148,    85,   142,   139,   188,    19,    25,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    61,
      62,    63,   162,   195,   196,   197,   198,    91,   149,   151,
     100,   191,   126,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   114,   115,    29,
      29,   123,   154,    91,   185,   194,   128,   132,    91,   127,
     104,   104,   123,   104,   104,   104,    27,    99,   103,   115,
     121,   124,   162,   181,   124,   183,   185,   100,   155,    79,
      97,    98,   100,   184,    93,    94,    98,   128,   198,   198,
      91,   154,   198,   154,   198,   154,   198,    27,   103,    85,
     127,   124,   128,   198,   126,   158,    91,   198,    12,    13,
      14,    15,    21,    33,    37,    60,    63,    72,    73,    85,
      86,    87,    88,    89,    90,    91,    92,   106,   107,   111,
     114,   115,   121,   123,   129,   135,   158,   161,   162,   164,
     170,   171,   172,   174,   175,   176,   177,   178,   195,   199,
     202,   204,    85,   105,    97,   124,   155,   105,   155,   105,
     155,   105,   122,   185,   160,   184,   123,   198,   199,   123,
     104,   123,   123,   178,   178,   134,   178,   178,   162,   178,
     178,   121,   178,   153,    91,   123,    83,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    84,    98,    99,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   120,
     121,   123,   128,   198,   105,   105,   105,     1,     5,     7,
       8,    10,    20,    22,    23,    32,    36,   127,   128,   144,
     145,   146,   147,   159,   161,   163,   165,   166,   167,   169,
     176,   178,   178,    16,   178,   123,   178,   198,    16,   178,
     124,   178,    94,    95,   132,   198,   124,   154,   124,   173,
     178,   174,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,    91,   178,   178,    91,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,    91,   178,   124,   173,   105,    17,   123,   168,
     178,   178,   194,    79,   178,   158,   178,   178,    31,   128,
     128,   128,   128,   198,   124,   124,   173,   124,   105,   198,
     124,   124,   178,    91,   115,   121,   123,   178,   200,   201,
     203,   155,    97,   124,   123,   100,   122,   124,    34,   182,
     185,   186,   194,   158,   158,    17,   178,    11,   158,   126,
     124,   124,   178,   124,    96,   184,   124,    97,   122,   128,
     122,   128,   158,   178,   124,   173,   178,   124,   128,   100,
     184,     9,    18,   143,   173,   158,   160,   178,    91,   201,
      91,   122,   200,   122,   178,   124,   158,   185,   198,   161,
     178,   158,   178,   158,   127,   122,   184,   128,   184,   128,
     158,   122,   178,   161,   178,   143,   128
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   130,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   132,   132,   133,   134,   134,   134,   135,   136,   137,
     138,   138,   139,   140,   141,   141,   142,   142,   143,   143,
     143,   144,   145,   146,   147,   148,   148,   148,   148,   148,
     148,   149,   149,   150,   150,   151,   151,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   157,   158,   158,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     160,   160,   160,   161,   162,   162,   162,   163,   164,   164,
     164,   164,   165,   166,   166,   166,   167,   168,   168,   169,
     169,   170,   171,   171,   172,   172,   173,   173,   174,   174,
     175,   175,   175,   175,   175,   175,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   177,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   180,   181,   181,   182,   182,   183,   183,
     184,   184,   185,   185,   185,   185,   185,   186,   186,   186,
     186,   186,   187,   188,   188,   188,   189,   190,   191,   191,
     192,   193,   194,   194,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   196,   197,   197,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   199,   199,   200,
     200,   201,   201,   202,   202,   203,   203,   204
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     0,     2,     3,     0,     2,     4,     3,     2,     2,
       1,     3,     1,     2,     1,     3,     1,     3,     0,     2,
       4,     4,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     6,     3,     7,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     1,     1,
       0,     2,     2,     3,     1,     3,     2,     2,     2,     4,
       5,     2,     1,     1,     2,     3,     4,     0,     2,     5,
       3,     5,     4,     5,     4,     5,     1,     3,     1,     5,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       3,     3,     2,     3,     4,     3,     4,     4,     3,     4,
       3,     5,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     1,     2,     0,     3,     1,     3,     1,     3,
       1,     1,     1,     3,     5,     3,     3,     4,     6,     5,
       4,     3,     4,     0,     3,     5,     4,     5,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     1,     1,
       4,     2,     3,     2,     3,     2,     4,     6,     1,     4,
       5,     1,     4,     5,     1,     4,     5,     1,     1,     3,
       5,     1,     3,     6,     8,     1,     3,     6
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
          case 91: /* NAME  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2176 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 132: /* character_sequence  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2182 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 133: /* string_constant  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2188 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 134: /* string_builder_body  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2194 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 135: /* string_builder  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2200 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* expression_else  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2206 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* expression_if_then_else  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2212 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 145: /* expression_for_loop  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2218 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* expression_while_loop  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2224 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* expression_with  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2230 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* annotation_argument  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2236 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* annotation_argument_list  */
#line 90 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2242 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* annotation_declaration_name  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2248 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* annotation_declaration  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2254 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* annotation_list  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2260 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* optional_annotation_list  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2266 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* optional_function_argument_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2272 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* optional_function_type  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2278 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* function_name  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2284 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expression_block  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2290 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* expression_any  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2296 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* expressions  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2302 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* expr_pipe  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2308 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* name_in_namespace  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2314 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* expression_delete  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2320 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* expr_new  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2326 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* expression_break  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2332 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* expression_return  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2338 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expression_try_catch  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2344 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_let  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2350 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expr_cast  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2356 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expr_sizeof  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2362 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expr_typename  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2368 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expr_list  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2374 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expr_block  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2380 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expr_numeric_const  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2386 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expr_assign  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2392 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expr_method_call  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2398 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* expr  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2404 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* structure_variable_declaration  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2410 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* struct_variable_declaration_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2416 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* variable_declaration_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2422 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* function_argument_list  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2428 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* variable_declaration  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2434 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* let_variable_declaration  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2440 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* enum_list  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2446 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* optional_structure_parent  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2452 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* variable_name_list  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2458 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* structure_type_declaration  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2464 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* auto_type_declaration  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2470 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* type_declaration  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2476 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* make_decl  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2482 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* make_struct_fields  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2488 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* make_struct_dim  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2494 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* make_struct_decl  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2500 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* make_dim  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2506 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* make_dim_decl  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2512 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
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
        case 11:
#line 237 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); }
#line 2800 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 238 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2806 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 242 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2812 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 246 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2820 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 249 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2831 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 255 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2841 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 263 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2859 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 279 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2882 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 309 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2894 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 328 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2902 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 331 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2910 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 337 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 2916 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 338 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2922 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 339 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2931 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 346 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2940 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 353 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 2954 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 365 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 2966 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 375 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 2977 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 384 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 2983 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 385 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 2989 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 386 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 2995 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 387 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3001 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 388 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3007 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 389 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3013 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 393 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3023 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 398 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3033 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 406 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3039 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 407 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3045 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 411 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3057 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 418 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3071 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 430 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3080 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 434 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3089 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3095 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3101 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 446 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3107 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3113 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3119 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 452 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3128 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 456 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3137 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 463 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3143 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3149 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 465 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3155 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 466 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3161 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 467 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3167 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 468 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3173 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 469 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3179 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 470 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3185 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 471 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3191 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 472 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3197 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 473 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3203 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 474 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3209 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 475 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3215 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 476 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3221 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 477 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3227 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 478 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3233 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 479 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3239 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3245 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 481 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3251 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 482 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3257 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 483 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3263 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 484 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3269 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 485 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3275 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 486 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3281 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 487 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3287 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 488 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3293 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 489 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3299 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 490 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3305 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 491 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3311 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 496 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
                        if ( !ann->apply(pFunction, *g_Program->thisModuleGroup, pA->arguments, err) ) {
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
#line 3381 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 564 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3389 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 567 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3401 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 577 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3407 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 578 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3413 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 579 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3419 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 580 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3425 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 581 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3431 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 582 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3437 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 583 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3443 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 584 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3449 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 585 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3455 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 586 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3461 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 587 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3467 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3473 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = tokAt((yyloc));
    }
#line 3482 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 596 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3493 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 602 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3501 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 608 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3517 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 622 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3523 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 623 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3534 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 629 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3540 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 633 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3548 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 639 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3556 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 642 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3564 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 645 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3573 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 649 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3581 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 655 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3587 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 659 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3595 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 662 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3603 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 665 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3613 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 673 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3621 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 679 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3627 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 680 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3633 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 684 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3664 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 710 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3694 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 738 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 3702 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 744 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3710 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 747 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3718 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 753 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3726 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 756 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3734 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 762 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3742 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 765 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3750 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3760 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 777 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
        }
        if ( (yyvsp[-3].faList) ) {
            for ( auto pA : *(yyvsp[-3].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *g_Program->thisModuleGroup, pA->arguments, err) ) {
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
#line 3812 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 827 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3818 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 828 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3824 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 829 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3830 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 830 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3836 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 831 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3842 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 832 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3848 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 836 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3854 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 837 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3860 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 838 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3866 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 839 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3872 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 840 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3878 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 841 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3884 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 842 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3890 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 843 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3896 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 844 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3902 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3908 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 850 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 3918 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 855 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 3930 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 865 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3936 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 866 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3942 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 867 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3948 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 868 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3954 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 869 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3960 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 870 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3966 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 871 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3972 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 872 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3978 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 873 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3984 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 874 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3990 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 875 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3996 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 876 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4002 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 877 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4008 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 878 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4014 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 879 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4020 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 880 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4026 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 881 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4032 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 882 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4038 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 883 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4044 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 884 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4050 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 885 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4056 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 886 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4062 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 887 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4068 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 888 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4074 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4080 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4086 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4092 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4098 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4104 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4110 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4116 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4122 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4128 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4134 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4140 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4149 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4158 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4166 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4174 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 914 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4180 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 915 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4186 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 916 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4192 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4198 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 918 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4206 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 921 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4212 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 922 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4218 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 923 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4224 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4230 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 925 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4236 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 926 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4242 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 927 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4257 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4272 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 947 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4308 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 981 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4314 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 982 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4320 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 986 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4329 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 993 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4337 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 996 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4346 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1003 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4352 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1004 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4358 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1008 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4364 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1009 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4370 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1013 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4376 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1014 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4382 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1018 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4391 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1022 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4399 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1025 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4408 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1029 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4418 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1034 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4428 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1042 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4436 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1045 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4445 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1049 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4454 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1053 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4464 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1058 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4474 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1066 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4499 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1089 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4507 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1092 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4520 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1100 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4533 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1111 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 4546 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1122 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s); 
        if ( !g_Program->addEnumeration(pEnum) ) {
            yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-3].s);
    }
#line 4561 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1135 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 4567 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1136 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4573 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1140 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
            } else if ( structs.size()==0 ) {
                yyerror("parent structure not found"+*(yyvsp[0].s),tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt((yylsp[0])),
                    CompilationError::structure_not_found);

            }
            delete (yyvsp[0].s);
        } else {
            pStruct = make_shared<Structure>(*(yyvsp[-1].s));
        }
        if ( pStruct ) {
            if ( !g_Program->addStructure(pStruct) ) {
                yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt((yylsp[-1])),
                    CompilationError::structure_already_declared);
                (yyval.pStructure) = nullptr;
            } else {
                (yyval.pStructure) = pStruct.get();
            }
        } else {
            (yyval.pStructure) = nullptr;
        }
        delete (yyvsp[-1].s);
    }
#line 4611 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1176 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure)->shared_from_this();
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            yyerror("structure field is not overriding anything, "+name,pDecl->at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name, td, init, pDecl->move_to_init, pDecl->at);
                        }
                    } else {
                        if ( pDecl->override ) {
                            if ( oldFd->type->isSameType(*pDecl->pTypeDecl,true,true) ) {
                                auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                                oldFd->init = init;
                            } else {
                                yyerror("structure field "+name+" override type mismatch ("+
                                    oldFd->type->describe()+") vs ("+pDecl->pTypeDecl->describe()+")",pDecl->at,
                                CompilationError::invalid_override);
                            }
                        } else {
                            yyerror("structure field is already declared "+name
                                +", use override to replace initial value instead",pDecl->at,
                                    CompilationError::invalid_override);
                        }
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
                    } else {
                        if ( !g_Program->addStructureHandle(pStruct,
                            static_pointer_cast<StructureTypeAnnotation>(annotation), args) ) {
                                yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-4])),
                                CompilationError::structure_already_declared);
                        } else {
                            pStruct->module->removeStructure(pStruct);
                        }
                    }
                }
                delete (yyvsp[-5].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 4675 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1238 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4686 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1244 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4696 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1252 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 4702 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1253 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 4708 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1254 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 4714 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1255 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 4720 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1256 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 4726 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1257 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 4732 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1258 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 4738 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1259 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 4744 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1260 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 4750 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1261 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 4756 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1262 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 4762 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1263 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 4768 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1264 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 4774 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1265 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 4780 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1266 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 4786 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1267 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 4792 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1268 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 4798 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1269 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 4804 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1270 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 4810 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1271 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 4816 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1275 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4828 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1285 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4837 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1289 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4848 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1298 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4854 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1299 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4860 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1300 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4866 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1301 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4875 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1305 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4885 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1310 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4895 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1315 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4905 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1320 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4915 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1325 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4924 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1329 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4934 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1334 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4945 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1340 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4954 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1344 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4964 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1349 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 4978 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1358 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4987 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1362 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4997 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1367 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5011 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1376 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5020 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1380 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5030 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1385 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5044 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1397 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5050 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1398 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5056 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1402 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5068 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1409 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5079 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1418 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5089 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1423 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5098 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1430 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5108 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1435 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5119 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1444 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5129 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1449 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5138 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1456 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5148 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 5152 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
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
#line 1463 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


void yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,at,cerr);
}

void yyerror(const string & error, CompilationError cerr) {
    g_Program->error(error,LineInfo(g_FileAccessStack.back(),
        yylloc.first_column,yylloc.first_line),cerr);
}

vector<ExpressionPtr> sequenceToList ( Expression * arguments ) {
    vector<ExpressionPtr> argList;
    auto arg = arguments;
    if ( arg->rtti_isSequence() ) {
        while ( arg->rtti_isSequence() ) {
            auto pSeq = static_cast<ExprSequence *>(arg);
            DAS_ASSERT(!pSeq->right->rtti_isSequence());
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
    return LineInfo(g_FileAccessStack.back(), li.first_column,li.first_line);
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

void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list ) {
    for ( auto pDecl : *list ) {
        if ( pDecl->pTypeDecl ) {
            for ( size_t ai=0; ai!= pDecl->pNameList->size(); ++ai ) {
                auto pVarType = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    yyerror("can't have default values in type declaration",
                    pDecl->at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
            }
        }
    }
}


