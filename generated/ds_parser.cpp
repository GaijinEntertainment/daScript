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
#line 32 "src/parser/ds_parser.ypp" /* yacc.c:338  */

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
        extern vector<FileInfoPtr>	g_AccessStack;
    }
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

#line 152 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

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
    _TBOOL = 292,
    _TVOID = 293,
    _TSTRING = 294,
    _TAUTO = 295,
    _TINT = 296,
    _TINT2 = 297,
    _TINT3 = 298,
    _TINT4 = 299,
    _TUINT = 300,
    _TUINT2 = 301,
    _TUINT3 = 302,
    _TUINT4 = 303,
    _TFLOAT = 304,
    _TFLOAT2 = 305,
    _TFLOAT3 = 306,
    _TFLOAT4 = 307,
    _TRANGE = 308,
    _TURANGE = 309,
    _TBLOCK = 310,
    _TINT64 = 311,
    _TUINT64 = 312,
    _TYPENAME = 313,
    _TDOUBLE = 314,
    _TFUNCTION = 315,
    ADDEQU = 316,
    SUBEQU = 317,
    DIVEQU = 318,
    MULEQU = 319,
    MODEQU = 320,
    ANDEQU = 321,
    OREQU = 322,
    XOREQU = 323,
    ADDADD = 324,
    SUBSUB = 325,
    LEEQU = 326,
    GREQU = 327,
    EQUEQU = 328,
    NOTEQU = 329,
    RARROW = 330,
    LARROW = 331,
    QQ = 332,
    QDOT = 333,
    LPIPE = 334,
    LBPIPE = 335,
    RPIPE = 336,
    INTEGER = 337,
    LONG_INTEGER = 338,
    UNSIGNED_INTEGER = 339,
    UNSIGNED_LONG_INTEGER = 340,
    FLOAT = 341,
    DOUBLE = 342,
    NAME = 343,
    BEGIN_STRING = 344,
    STRING_CHARACTER = 345,
    END_STRING = 346,
    BEGIN_STRING_EXPR = 347,
    END_STRING_EXPR = 348,
    UNARY_MINUS = 349,
    UNARY_PLUS = 350,
    PRE_INC = 351,
    PRE_DEC = 352,
    POST_INC = 353,
    POST_DEC = 354,
    COLCOL = 355
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 55 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 290 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   3089

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  277
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  488

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   355

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   112,     2,     2,   126,   107,   100,     2,
     120,   121,   105,   104,    94,   103,   117,   106,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    97,   125,
     101,    95,   102,    96,   108,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   118,     2,   119,    99,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   123,    98,   124,   111,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90,    91,    92,    93,   109,
     110,   113,   114,   115,   116,   122
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   217,   217,   219,   220,   221,   222,   223,   224,   225,
     226,   230,   231,   235,   239,   242,   248,   256,   272,   293,
     297,   298,   302,   312,   316,   317,   321,   324,   330,   331,
     332,   339,   346,   358,   368,   377,   378,   379,   380,   381,
     382,   386,   391,   399,   400,   404,   411,   423,   427,   434,
     435,   439,   440,   441,   445,   449,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   488,   557,   560,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     584,   585,   591,   597,   611,   612,   618,   622,   628,   631,
     634,   638,   644,   648,   651,   654,   662,   668,   669,   673,
     699,   727,   730,   736,   739,   745,   748,   754,   759,   810,
     842,   843,   844,   845,   846,   847,   851,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   865,   870,   880,   881,
     882,   883,   884,   885,   886,   887,   888,   889,   890,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   919,   923,   926,   929,   930,   931,
     934,   935,   936,   937,   938,   939,   940,   950,   960,   994,
     995,   999,  1000,  1004,  1005,  1009,  1010,  1014,  1018,  1021,
    1025,  1030,  1038,  1041,  1045,  1049,  1054,  1062,  1085,  1088,
    1096,  1107,  1118,  1131,  1145,  1189,  1195,  1203,  1204,  1205,
    1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1226,  1236,  1240,
    1249,  1250,  1251,  1252,  1256,  1261,  1266,  1271,  1276,  1280,
    1285,  1291,  1295,  1300,  1320,  1324,  1329,  1352,  1353,  1357,
    1364,  1373,  1378,  1385,  1390,  1399,  1404,  1411
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
  "_TYPEDEF", "_WITH", "_TBOOL", "_TVOID", "_TSTRING", "_TAUTO", "_TINT",
  "_TINT2", "_TINT3", "_TINT4", "_TUINT", "_TUINT2", "_TUINT3", "_TUINT4",
  "_TFLOAT", "_TFLOAT2", "_TFLOAT3", "_TFLOAT4", "_TRANGE", "_TURANGE",
  "_TBLOCK", "_TINT64", "_TUINT64", "_TYPENAME", "_TDOUBLE", "_TFUNCTION",
  "ADDEQU", "SUBEQU", "DIVEQU", "MULEQU", "MODEQU", "ANDEQU", "OREQU",
  "XOREQU", "ADDADD", "SUBSUB", "LEEQU", "GREQU", "EQUEQU", "NOTEQU",
  "RARROW", "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE", "INTEGER",
  "LONG_INTEGER", "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER", "FLOAT",
  "DOUBLE", "NAME", "BEGIN_STRING", "STRING_CHARACTER", "END_STRING",
  "BEGIN_STRING_EXPR", "END_STRING_EXPR", "','", "'='", "'?'", "':'",
  "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('", "')'", "COLCOL",
  "'{'", "'}'", "';'", "'$'", "$accept", "program", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "options_declaration", "require_declaration", "require_list",
  "require_module", "expect_declaration", "expect_list", "expect_error",
  "expression_else", "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break", "expression_return",
  "expression_try_catch", "let_scope", "expression_let", "expr_sizeof",
  "expr_typename", "expr_list", "expr_block", "expr_lambda",
  "expr_numeric_const", "expr_assign", "expr_method_call", "expr",
  "struct_variable_declaration_list", "variable_declaration_list",
  "function_argument_list", "copy_or_move", "variable_declaration",
  "let_variable_declaration", "global_let", "enum_list",
  "alias_declaration", "enum_declaration", "structure_name",
  "structure_declaration", "variable_name_list", "basic_type_declaration",
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
     345,   346,   347,   348,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,    64,   349,
     350,   126,    33,   351,   352,   353,   354,    46,    91,    93,
      40,    41,   355,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -308

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-308)))

#define YYTABLE_NINF -137

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -308,     9,  -308,   -87,   -45,   -20,    41,    64,    71,     2,
    -308,  -308,  -308,   145,  -308,  -308,  -308,  -308,  -308,  -308,
      78,  -308,    91,   108,   123,  -308,  -308,   125,  -308,   106,
     136,  -308,   112,   158,   128,  -308,    14,  -308,   162,   138,
     -78,   275,   -45,   170,   -20,    41,  -308,  2059,   165,  -308,
     -45,     2,  -308,  -308,   137,  2600,   227,   232,  -308,   143,
    -308,  -308,   144,   119,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,   -70,   171,   175,  -308,  -308,
    -308,   160,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,   184,  -308,  -308,  -308,
     188,  -308,  -308,  -308,  -308,   174,  -308,    37,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,   -32,   178,
    -308,  -308,   202,  -308,  2059,  1917,   149,   -37,  -308,  2059,
    2059,   205,    26,    26,  -308,  -308,  -308,    -5,   214,  -308,
     -69,  -308,   -28,  -308,  2059,   185,  -308,    92,   143,  -308,
    -308,   177,  2007,  -308,   187,   193,  1917,  1917,  -308,  -308,
    -308,  -308,  -308,  -308,    49,  -308,  1917,  1917,   -61,  1917,
    1917,   180,  1917,  -308,   215,  -308,  -308,  -308,   -36,  -308,
    -308,  -308,  -308,  -308,  -308,   255,  -308,  2217,   216,  -308,
    -308,  -308,  -308,  -308,   257,  -308,    94,   103,   219,   178,
     147,   178,   155,  -308,  -308,   222,  -308,  -308,   254,   174,
    -308,  1917,   178,   982,     5,  -308,  1917,  1097,   460,   460,
     150,   460,   460,  -308,   460,   460,  2059,  2380,   511,   143,
    -308,  1212,    13,  1917,  1917,  1917,  1917,  1917,  1917,  1917,
    1917,  -308,  -308,  1917,  1917,  1917,  1917,   256,  1917,  1917,
     259,  1917,  1917,  1917,  1917,  1917,  1917,  1917,  1917,  1917,
    1917,  1917,  1917,  1917,  1917,   260,  1917,  1330,   218,  -308,
    2059,  -308,   248,  -308,   249,  -308,  -308,  -308,  2795,   185,
    2059,  2433,  1448,  2486,  2059,  2539,  -308,  1917,   264,   752,
    -308,  -308,   -10,  1917,  1917,   254,  1566,  -308,   185,  1917,
    1917,   336,  -308,  -308,  -308,  -308,  -308,  -308,  -308,   243,
     244,   247,  -308,  -308,   -76,  2275,   178,  -308,    68,  2795,
    -308,  2795,  2795,  2795,  2795,  2795,  2795,  2795,  2795,  2957,
    2957,   386,   386,   253,  2795,   460,  -308,   -18,   -18,  2795,
    2695,  2845,  2895,  2913,  2957,  2957,  2971,  2971,   301,   301,
     301,  -308,  2644,  -308,    81,  -308,   166,  -308,  -308,  -308,
      60,  -308,  -308,    90,  -308,    65,  -308,  2745,   261,   867,
     -59,  1681,  2795,   280,    28,    45,   341,   254,   254,  2325,
    2325,     0,  1917,  2795,   373,  2795,  2325,   262,  -308,  -308,
    -308,  -308,   185,  1917,  -308,  1799,  1917,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  1917,   299,   300,   270,   299,   271,
    1917,  -308,    29,  -308,  -308,   133,  -308,    32,  1917,  2795,
     185,  -308,  -308,  -308,  2795,  -308,   104,   326,  2795,   -42,
      99,   -42,  -308,   280,  -308,  2795,   185,   254,  2059,  1917,
     185,  1917,  -308,    19,  -308,   637,  -308,   272,  1917,  -308,
    -308,    20,  -308,  2087,  -308,  2325,  -308,  -308,  -308,  2795,
    -308,  1917,  -308,    32,  -308,  2152,  -308,  -308
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,     0,     0,     0,     0,     0,     0,     0,
       9,     8,     7,     0,     6,     5,    10,     4,     3,   199,
       0,    41,    18,    26,    23,    24,    22,    19,    20,     0,
       0,    44,   104,     0,    45,    47,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,   218,     0,     0,   106,
       0,     0,    50,   223,     0,     0,     0,     0,    56,    51,
     225,   217,     0,   207,    39,    40,    37,    38,    36,    11,
      35,    42,    27,    25,    21,     0,     0,     0,   227,   243,
     228,   248,   229,   231,   232,   233,   234,   236,   237,   238,
     239,   240,   241,   242,   244,   245,   261,   230,   235,   246,
     264,   247,   250,   252,   251,   221,   105,     0,    48,   199,
      59,    60,    62,    61,    63,    64,    65,    66,    83,    84,
      76,    77,    74,    75,    79,    80,    78,    72,    73,    68,
      67,    69,    70,    71,    58,    57,    81,    82,     0,    54,
     200,   206,     0,   205,     0,     0,     0,     0,   222,     0,
       0,     0,    51,    51,   254,   258,   256,     0,     0,    46,
       0,    52,     0,   203,     0,     0,   226,   208,    51,   153,
     154,     0,     0,   148,     0,     0,     0,     0,   130,   132,
     131,   133,   134,   135,   149,    14,     0,     0,     0,     0,
       0,     0,     0,   100,    49,   151,   127,   211,     0,   192,
     190,   191,   195,   194,   150,     0,   193,   210,     0,   152,
     267,   268,    12,    13,     0,   219,     0,     0,     0,    54,
       0,    54,     0,   255,   257,     0,   224,    53,     0,    55,
      85,     0,    54,     0,   108,   111,     0,     0,   174,   175,
      11,   158,   157,   182,   156,   155,     0,     0,     0,    51,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   249,     0,   262,     0,   265,   253,   204,   209,     0,
       0,     0,     0,     0,     0,     0,    17,     0,    15,     0,
     178,   102,   117,     0,     0,     0,   113,   112,     0,     0,
       0,    86,    88,    98,    95,    93,    94,   101,    89,     0,
       0,     0,    99,    92,     0,   136,    54,   183,     0,   125,
     103,   141,   142,   144,   143,   145,   138,   139,   140,   169,
     170,   167,   168,     0,   159,   188,   181,   196,   197,   137,
       0,   172,   173,   171,   165,   166,   161,   160,   162,   163,
     164,   180,     0,   185,     0,   220,     0,   263,   266,   129,
       0,   121,   109,     0,   187,     0,   123,     0,   149,     0,
       0,     0,   275,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,   107,     0,     0,    91,    96,
      97,    90,     0,     0,   184,     0,     0,   179,   186,   260,
     122,   110,   124,    16,     0,     0,     0,     0,     0,     0,
       0,   118,     0,   201,   120,     0,    33,    28,     0,   115,
       0,    34,   100,   128,   126,   146,     0,   189,   269,     0,
       0,     0,   273,   272,   277,   276,     0,     0,     0,     0,
       0,     0,    31,     0,   116,     0,   147,     0,     0,   119,
     202,     0,   216,   136,    29,     0,    32,    87,   274,   270,
     212,     0,   215,    28,   214,   136,    30,   213
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -308,  -308,   152,  -308,  -308,  -308,  -308,  -308,  -308,   349,
    -308,  -308,   364,   -73,  -308,  -308,  -308,  -308,   369,   365,
    -308,   363,  -308,   223,  -137,  -208,  -308,  -308,   -54,  -308,
     -26,  -143,    -9,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -281,   168,  -308,  -308,  -245,  -308,    46,   314,
    -308,  -308,   -58,  -126,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -307,   -46,  -308,  -308,  -124,   266,     6,    11,  -308,
    -308,  -308
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   146,    70,   240,   195,    10,    11,    27,    28,
      12,    24,    25,   462,   323,   324,   325,   326,    21,    22,
      34,    35,    36,    13,   139,   165,    59,    14,   196,   327,
     248,   328,   198,   329,   199,   330,   331,   332,   398,   333,
     200,   201,   338,   202,   203,   204,   205,   206,   339,    40,
     432,   162,   424,    62,   434,    15,    75,    16,    17,    54,
      18,    63,   208,   103,   104,   105,   209,   393,   394,   210,
     395,   211
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,   102,   197,   334,   252,   145,   374,   396,   401,     2,
      60,   292,   163,   294,     3,   219,   221,   438,   147,    60,
     167,   383,   223,   225,   299,   216,   217,    32,   220,   222,
      31,   232,   154,     4,   141,     5,    19,     6,   101,     7,
     229,   460,    37,    20,     8,    76,    61,   154,   234,   411,
     461,    77,   250,   143,   148,   226,    60,   267,   214,   246,
     270,    33,    23,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   251,    99,   100,   154,   215,   161,
      32,   435,   154,   227,   142,   224,   141,   228,   102,   285,
     286,   155,   297,   102,   102,   156,   102,   102,    51,   231,
     397,   230,   336,   413,    32,   143,   155,   157,   102,   154,
     156,   154,   309,   158,    33,   302,   102,     9,   412,    26,
     154,    42,   157,    52,   446,   101,   193,  -104,   158,   194,
     101,   101,   193,   101,   101,   480,   138,   427,    33,    38,
     456,    39,    29,   428,   457,   101,   155,   463,   159,    30,
     156,   155,   413,   101,   429,   156,   376,    55,   141,  -104,
     430,    48,   157,    41,   154,   413,   380,   157,   158,   243,
     385,   420,   154,   158,   413,    42,   422,   143,   155,   414,
     155,   207,   156,   154,   156,   141,   289,   290,   413,   155,
     102,   154,   418,   156,   157,    43,   157,    56,    57,   141,
     158,   421,   158,   142,   143,   157,   144,    44,   467,    45,
     334,   158,   238,   239,   428,   466,    58,   142,   143,    46,
     458,    47,   241,   242,    48,   244,   245,   101,   247,   212,
     213,   306,   307,   155,   102,   379,    49,   156,    50,   293,
      53,   155,    72,   106,   102,   156,   136,   295,   102,   157,
     109,   137,   155,   138,   404,   158,   156,   157,   419,   140,
     155,   433,   149,   158,   156,   164,   150,   298,   157,   301,
     151,   101,   303,   305,   158,   152,   157,    64,    65,   153,
     166,   101,   158,   218,   335,   101,   225,   233,   246,   341,
     342,   343,   344,   345,   346,   347,   348,   236,   193,   349,
     350,   351,   352,   237,   354,   355,   472,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   470,   372,     9,   471,   252,   287,   141,   484,   288,
     291,   296,    60,   375,   353,   436,   437,   356,   371,  -104,
     377,   378,   441,   387,   212,   392,   143,    66,   443,   399,
     400,    67,   403,    68,    69,   405,   406,   407,   408,   409,
     261,   262,   410,   415,   426,   431,   267,   459,   269,   270,
     271,  -104,   272,    48,   440,   442,   464,   449,   451,   452,
     454,   478,   308,   468,    74,   261,   262,   263,   264,   265,
     266,   267,   469,   269,   270,   271,   474,   272,    73,   476,
     486,    71,   102,   481,   108,   107,   465,   249,   285,   286,
     340,   483,   274,   160,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   453,   245,   450,   247,   235,     0,
       0,     0,     0,   285,   286,     0,     0,     0,   439,   101,
       0,     0,     0,     0,     0,   261,   262,   263,   264,   444,
       0,   267,   447,   269,   270,   271,     0,   272,     0,     0,
     448,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,   279,   280,
     281,   282,   283,   284,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   285,   286,   473,     0,   475,     0,     0,
       0,   335,   311,     0,   479,     0,   312,   168,   313,   314,
       0,   315,     0,   169,   170,   171,   172,   485,     0,   261,
     262,   316,   173,   317,   318,   267,     0,     0,   270,   271,
       0,   272,     0,   319,   174,     0,     0,   320,    78,    79,
      80,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,    97,    98,   175,
      99,     0,     0,     0,     0,     0,     0,   285,   286,     0,
     176,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,   179,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,     0,     0,     0,   188,
       0,     0,   189,   190,     0,     0,     0,     0,     0,   191,
       0,   192,     0,    33,   193,   321,   322,   194,   311,     0,
       0,     0,   312,   168,   313,   314,     0,   315,     0,   169,
     170,   171,   172,     0,     0,     0,     0,   316,   173,   317,
     318,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     174,     0,     0,   320,    78,    79,    80,     0,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    97,    98,   175,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
     179,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,     0,     0,     0,   188,     0,     0,   189,   190,
       0,     0,     0,     0,     0,   191,     0,   192,   168,    33,
     193,   477,   322,   194,   169,   170,   171,   172,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,   154,
       0,     0,     0,     0,     0,   174,     0,     0,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     175,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,   179,   180,   181,   182,   183,
     388,   185,     0,     0,     0,     0,     0,     0,   155,     0,
       0,     0,   156,     0,     0,   186,   187,     0,     0,     0,
     188,     0,     0,   189,   389,     0,     0,     0,     0,     0,
     390,     0,   391,   168,    33,   193,     0,     0,   194,   169,
     170,   171,   172,     0,     0,     0,     0,     0,   173,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
     174,     0,     0,     0,    78,    79,    80,     0,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    97,    98,   175,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
     179,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     186,   187,     0,     0,     0,   188,     0,     0,   189,   190,
       0,     0,     0,     0,     0,   191,     0,   192,   168,    33,
     193,     0,     0,   194,   169,   170,   171,   172,   300,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     175,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,   179,   180,   181,   182,   183,
     184,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   187,     0,     0,     0,
     188,     0,     0,   189,   190,     0,     0,     0,     0,     0,
     191,     0,   192,   168,    33,   193,     0,     0,   194,   169,
     170,   171,   172,   304,     0,     0,     0,     0,   173,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     174,     0,     0,     0,    78,    79,    80,     0,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    97,    98,   175,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
     179,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,     0,     0,     0,   188,     0,     0,   189,   190,
       0,     0,     0,     0,     0,   191,     0,   192,   168,    33,
     193,     0,     0,   194,   169,   170,   171,   172,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     175,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,   179,   180,   181,   182,   183,
     184,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   187,     0,     0,     0,
     188,     0,     0,   189,   190,     0,     0,     0,     0,     0,
     191,     0,   192,   337,    33,   193,   168,     0,   194,     0,
       0,     0,   169,   170,   171,   172,     0,     0,     0,     0,
       0,   173,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   174,     0,     0,     0,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,   175,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,   179,   180,   181,   182,   183,   184,   185,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   187,     0,     0,     0,   188,     0,
       0,   189,   190,     0,     0,     0,     0,     0,   191,     0,
     192,   373,    33,   193,   168,     0,   194,     0,     0,     0,
     169,   170,   171,   172,     0,     0,     0,     0,     0,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   174,     0,     0,     0,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,   175,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,   179,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,     0,     0,     0,   188,     0,     0,   189,
     190,     0,     0,     0,     0,     0,   191,     0,   192,   382,
      33,   193,   168,     0,   194,     0,     0,     0,   169,   170,
     171,   172,     0,     0,     0,     0,     0,   173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
       0,     0,     0,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,   175,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   176,   177,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,   178,   179,
     180,   181,   182,   183,   184,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
     187,     0,     0,     0,   188,     0,     0,   189,   190,     0,
       0,     0,     0,     0,   191,     0,   192,   168,    33,   193,
       0,     0,   194,   169,   170,   171,   172,     0,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   174,     0,     0,     0,    78,    79,
      80,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,    97,    98,   175,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,   179,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,     0,     0,     0,   188,
       0,     0,   189,   190,     0,     0,     0,     0,     0,   191,
       0,   192,   425,    33,   193,   168,     0,   194,     0,     0,
       0,   169,   170,   171,   172,     0,     0,     0,     0,     0,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   174,     0,     0,     0,    78,    79,    80,     0,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,     0,    97,    98,   175,    99,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,   179,   180,   181,   182,   183,   184,   185,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   187,     0,     0,     0,   188,     0,     0,
     189,   190,     0,     0,     0,     0,     0,   191,     0,   192,
     445,    33,   193,   168,     0,   194,     0,     0,     0,   169,
     170,   171,   172,     0,     0,     0,     0,     0,   173,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     174,     0,     0,     0,    78,    79,    80,     0,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    97,    98,   175,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
     179,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   187,     0,     0,     0,   188,    76,     0,   189,   190,
       0,     0,    77,     0,     0,   191,     0,   192,     0,    33,
     193,     0,     0,   194,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,     0,    99,   100,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,     0,    99,   100,
       0,     0,     0,     0,     0,   191,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   285,   286,     0,     0,     0,     0,
       0,     0,   482,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   285,
     286,     0,     0,     0,     0,     0,     0,   487,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,  -136,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   285,   286,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,     0,     0,     0,     0,   193,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   285,   286,     0,
       0,   310,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     285,   286,     0,     0,   381,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,     0,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   285,   286,     0,     0,   384,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   285,   286,     0,     0,
     386,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,     0,
       0,   134,   135,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   285,   286,   417,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   416,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   423,     0,
       0,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   261,   262,   263,   264,   265,   266,
     267,     0,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   261,   262,   263,   264,   265,   266,
     267,     0,   269,   270,   271,     0,   272,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,     0,
     269,   270,   271,     0,   272,   277,   278,   279,   280,   281,
     282,   283,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   285,   286,   278,   279,   280,   281,   282,   283,
     284,     0,     0,     0,     0,     0,   261,   262,     0,     0,
     285,   286,   267,     0,   269,   270,   271,     0,   272,     0,
     261,   262,     0,     0,     0,     0,   267,     0,   269,   270,
     271,     0,   272,     0,     0,     0,     0,     0,     0,     0,
     280,   281,   282,   283,   284,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   285,   286,   282,   283,   284,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   285,   286
};

static const yytype_int16 yycheck[] =
{
       9,    47,   145,   248,    80,    63,   287,    17,   315,     0,
      88,   219,   138,   221,     5,   152,   153,    17,    88,    88,
     144,   302,    27,    82,   232,   149,   150,    88,   152,   153,
      28,   168,    27,    24,    76,    26,   123,    28,    47,    30,
     164,     9,    51,    88,    35,    19,   124,    27,   172,   125,
      18,    25,    88,    95,   124,   124,    88,    75,    95,   118,
      78,   122,    82,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,   120,    59,    60,    27,   125,   121,
      88,   398,    27,   121,    94,   100,    76,   125,   144,   117,
     118,    96,   228,   149,   150,   100,   152,   153,    94,   167,
     120,   165,   249,    94,    88,    95,    96,   112,   164,    27,
     100,    27,   246,   118,   122,   120,   172,   118,   336,    88,
      27,    94,   112,   119,   415,   144,   123,    88,   118,   126,
     149,   150,   123,   152,   153,   125,   120,   119,   122,     4,
     121,     6,    88,   125,   125,   164,    96,   438,   121,    88,
     100,    96,    94,   172,   119,   100,   290,    29,    76,   120,
     125,   122,   112,    95,    27,    94,   300,   112,   118,   188,
     304,   121,    27,   118,    94,    94,   121,    95,    96,   121,
      96,   145,   100,    27,   100,    76,   102,    94,    94,    96,
     246,    27,   121,   100,   112,    97,   112,    69,    70,    76,
     118,   121,   118,    94,    95,   112,    97,    94,   119,    94,
     465,   118,   176,   177,   125,   121,    88,    94,    95,   123,
      97,    95,   186,   187,   122,   189,   190,   246,   192,    90,
      91,    91,    92,    96,   290,   299,    88,   100,   120,   102,
      88,    96,    82,    88,   300,   100,    29,   102,   304,   112,
     123,    29,    96,   120,   318,   118,   100,   112,   102,   125,
      96,   397,   101,   118,   100,    97,   101,   231,   112,   233,
     120,   290,   236,   237,   118,   101,   112,    12,    13,   101,
      88,   300,   118,    88,   248,   304,    82,   120,   118,   253,
     254,   255,   256,   257,   258,   259,   260,   120,   123,   263,
     264,   265,   266,   120,   268,   269,   459,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   457,   286,   118,   458,    80,   120,    76,   481,    82,
     121,   119,    88,   125,    88,   399,   400,    88,    88,    88,
     102,   102,   406,   307,    90,   309,    95,    82,   412,   313,
     314,    86,   316,    88,    89,   319,   320,    31,   125,   125,
      69,    70,   125,   120,    94,    34,    75,   435,    77,    78,
      79,   120,    81,   122,    11,   123,   440,    88,    88,   119,
     119,   119,   240,   451,    45,    69,    70,    71,    72,    73,
      74,    75,   456,    77,    78,    79,   460,    81,    44,   463,
     483,    42,   458,   471,    51,    50,   442,   194,   117,   118,
     252,   475,    96,   109,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   428,   389,   425,   391,   172,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,   402,   458,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,   413,
      -1,    75,   416,    77,    78,    79,    -1,    81,    -1,    -1,
     424,    -1,    -1,    -1,    -1,    -1,   430,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,   459,    -1,   461,    -1,    -1,
      -1,   465,     1,    -1,   468,    -1,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,   481,    -1,    69,
      70,    20,    21,    22,    23,    75,    -1,    -1,    78,    79,
      -1,    81,    -1,    32,    33,    -1,    -1,    36,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,   122,   123,   124,   125,   126,     1,    -1,
      -1,    -1,     5,     6,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,     6,   122,
     123,   124,   125,   126,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,   100,    -1,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,     6,   122,   123,    -1,    -1,   126,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,     6,   122,
     123,    -1,    -1,   126,    12,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,     6,   122,   123,    -1,    -1,   126,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,     6,   122,
     123,    -1,    -1,   126,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,     6,    -1,   126,    -1,
      -1,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,     6,    -1,   126,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,   121,
     122,   123,     6,    -1,   126,    -1,    -1,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,     6,   122,   123,
      -1,    -1,   126,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,   121,   122,   123,     6,    -1,   126,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,     6,    -1,   126,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,   108,    19,    -1,   111,   112,
      -1,    -1,    25,    -1,    -1,   118,    -1,   120,    -1,   122,
     123,    -1,    -1,   126,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,   125,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,   123,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,   121,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,   121,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,   121,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
     121,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,    -1,    -1,
      -1,   111,   112,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    81,   100,   101,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   101,   102,   103,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
     117,   118,    75,    -1,    77,    78,    79,    -1,    81,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,   105,   106,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   128,     0,     5,    24,    26,    28,    30,    35,   118,
     133,   134,   137,   150,   154,   182,   184,   185,   187,   123,
      88,   145,   146,    82,   138,   139,    88,   135,   136,    88,
      88,    28,    88,   122,   147,   148,   149,   159,     4,     6,
     176,    95,    94,    97,    94,    94,   123,    95,   122,    88,
     120,    94,   119,    88,   186,    29,    69,    70,    88,   153,
      88,   124,   180,   188,    12,    13,    82,    86,    88,    89,
     130,   145,    82,   139,   136,   183,    19,    25,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    59,
      60,   159,   189,   190,   191,   192,    88,   146,   148,   123,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   111,   112,    29,    29,   120,   151,
     125,    76,    94,    95,    97,   179,   129,    88,   124,   101,
     101,   120,   101,   101,    27,    96,   100,   112,   118,   121,
     176,   121,   178,   180,    97,   152,    88,   192,     6,    12,
      13,    14,    15,    21,    33,    58,    69,    70,    82,    83,
      84,    85,    86,    87,    88,    89,   103,   104,   108,   111,
     112,   118,   120,   123,   126,   132,   155,   158,   159,   161,
     167,   168,   170,   171,   172,   173,   174,   175,   189,   193,
     196,   198,    90,    91,    95,   125,   192,   192,    88,   151,
     192,   151,   192,    27,   100,    82,   124,   121,   125,   192,
     155,   179,   151,   120,   192,   193,   120,   120,   175,   175,
     131,   175,   175,   159,   175,   175,   118,   175,   157,   150,
      88,   120,    80,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    81,    95,    96,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   117,   118,   120,    82,   102,
      94,   121,   152,   102,   152,   102,   119,   180,   175,   152,
      16,   175,   120,   175,    16,   175,    91,    92,   129,   192,
     121,     1,     5,     7,     8,    10,    20,    22,    23,    32,
      36,   124,   125,   141,   142,   143,   144,   156,   158,   160,
     162,   163,   164,   166,   173,   175,   151,   121,   169,   175,
     170,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,    88,   175,   175,    88,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,    88,   175,   121,   169,   125,   192,   102,   102,   155,
     192,   121,   121,   169,   121,   192,   121,   175,    88,   112,
     118,   120,   175,   194,   195,   197,    17,   120,   165,   175,
     175,   188,    76,   175,   155,   175,   175,    31,   125,   125,
     125,   125,   152,    94,   121,   120,    97,   119,   121,   102,
     121,   121,   121,    93,   179,   121,    94,   119,   125,   119,
     125,    34,   177,   180,   181,   188,   155,   155,    17,   175,
      11,   155,   123,   155,   175,   121,   169,   175,   175,    88,
     195,    88,   119,   194,   119,   175,   121,   125,    97,   179,
       9,    18,   140,   169,   155,   157,   121,   119,   179,   155,
     180,   192,   158,   175,   155,   175,   155,   124,   119,   175,
     125,   179,   125,   155,   158,   175,   140,   125
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   127,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   129,   129,   130,   131,   131,   131,   132,   133,   134,
     135,   135,   136,   137,   138,   138,   139,   139,   140,   140,
     140,   141,   142,   143,   144,   145,   145,   145,   145,   145,
     145,   146,   146,   147,   147,   148,   148,   149,   149,   150,
     150,   151,   151,   151,   152,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   154,   155,   155,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     157,   157,   157,   158,   159,   159,   159,   160,   161,   161,
     161,   161,   162,   163,   163,   163,   164,   165,   165,   166,
     166,   167,   167,   168,   168,   169,   169,   170,   170,   171,
     172,   172,   172,   172,   172,   172,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   174,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   176,
     176,   177,   177,   178,   178,   179,   179,   180,   180,   180,
     180,   180,   181,   181,   181,   181,   181,   182,   183,   183,
     183,   184,   185,   186,   187,   188,   188,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   190,   191,   191,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   193,   193,   194,
     194,   195,   195,   196,   196,   197,   197,   198
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
       3,     4,     5,     4,     5,     1,     3,     1,     5,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       3,     3,     2,     3,     4,     3,     4,     4,     3,     5,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       3,     1,     3,     1,     3,     1,     1,     1,     3,     5,
       3,     3,     4,     6,     5,     4,     3,     4,     0,     3,
       5,     4,     5,     1,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     2,     3,     2,     3,     2,     4,
       6,     1,     4,     5,     1,     4,     5,     1,     1,     3,
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
          case 88: /* NAME  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2110 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 129: /* character_sequence  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2116 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 130: /* string_constant  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2122 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 131: /* string_builder_body  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2128 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 132: /* string_builder  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2134 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* expression_else  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2140 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 141: /* expression_if_then_else  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2146 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 142: /* expression_for_loop  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2152 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* expression_while_loop  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2158 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* expression_with  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2164 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 145: /* annotation_argument  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2170 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* annotation_argument_list  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2176 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* annotation_declaration_name  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2182 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* annotation_declaration  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2188 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* annotation_list  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2194 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* optional_annotation_list  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2200 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* optional_function_argument_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2206 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* optional_function_type  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2212 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* function_name  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2218 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expression_block  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2224 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expression_any  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2230 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* expressions  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2236 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expr_pipe  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2242 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* name_in_namespace  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2248 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* expression_delete  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2254 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* expr_new  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2260 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* expression_break  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2266 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* expression_return  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2272 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* expression_try_catch  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2278 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* expression_let  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2284 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expr_sizeof  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2290 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expr_typename  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2296 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expr_list  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2302 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expr_block  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2308 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expr_lambda  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2314 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expr_numeric_const  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2320 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expr_assign  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2326 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expr_method_call  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2332 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expr  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2338 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* struct_variable_declaration_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2344 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* variable_declaration_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2350 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* function_argument_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2356 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* variable_declaration  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2362 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* let_variable_declaration  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2368 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* enum_list  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2374 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* variable_name_list  */
#line 81 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2380 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* structure_type_declaration  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2386 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* auto_type_declaration  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2392 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* type_declaration  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2398 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* make_decl  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2404 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* make_struct_fields  */
#line 90 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2410 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* make_struct_dim  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2416 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* make_struct_decl  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2422 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* make_dim  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2428 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* make_dim_decl  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2434 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 230 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); }
#line 2722 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 231 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2728 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 235 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2734 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 239 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2742 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 242 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2753 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 248 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2763 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 256 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2781 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 272 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 2804 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 302 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2816 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 321 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2824 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 324 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2832 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 330 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 2838 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 331 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2844 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 332 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2853 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 339 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2862 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 346 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 2876 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 358 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 2888 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 368 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 2899 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 377 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2905 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 378 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2911 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 379 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i));  }
#line 2917 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 380 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)));  }
#line 2923 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 381 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true);  }
#line 2929 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 382 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false);  }
#line 2935 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 386 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 2945 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 391 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 2955 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 399 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2961 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 400 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 2967 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 404 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 2979 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 411 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 2993 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 423 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3002 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 427 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3011 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 434 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3017 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 435 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3023 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 439 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3029 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3035 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3041 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_AccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3050 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3059 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 456 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3065 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 457 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3071 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 458 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3077 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 459 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3083 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 460 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3089 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 461 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3095 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 462 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3101 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 463 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3107 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3113 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 465 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3119 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 466 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3125 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 467 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3131 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 468 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3137 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 469 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3143 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 470 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3149 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 471 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3155 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 472 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3161 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 473 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3167 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 474 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3173 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 475 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3179 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 476 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3185 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 477 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3191 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 478 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3197 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 479 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3203 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3209 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 481 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3215 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 482 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3221 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 483 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3227 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 484 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3233 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 489 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3303 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 557 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3311 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 560 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
    }
#line 3322 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 569 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3328 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 570 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3334 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 571 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3340 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 572 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3346 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 573 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3352 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 574 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3358 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 575 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3364 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 576 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3370 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 577 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3376 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 578 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3382 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 579 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3388 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 580 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3394 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 584 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBlock(); }
#line 3400 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 585 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3411 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 591 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3419 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 597 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3435 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 611 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3441 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 612 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3452 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 618 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3458 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 622 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3466 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 628 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3474 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 631 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3482 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 634 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3491 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 638 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3499 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 644 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3505 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 648 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3513 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 651 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3521 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 654 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3531 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 662 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3539 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 668 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3545 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 669 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3551 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 673 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3582 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 699 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3612 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 727 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3620 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 730 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3628 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 736 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3636 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 739 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3644 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 745 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3652 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 748 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3660 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 754 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3670 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 760 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
    }
#line 3722 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 811 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
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
                            yyerror("closure argument is already declared " + name,
                                pDecl->at,CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 3755 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 842 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3761 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 843 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3767 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 844 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3773 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3779 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 846 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3785 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 847 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3791 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 851 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3797 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 852 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3803 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 853 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3809 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 854 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3815 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 855 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3821 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 856 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3827 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 857 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3833 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 858 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3839 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 859 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3845 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 860 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3851 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 865 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 3861 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 870 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 3873 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 880 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3879 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 881 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3885 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 882 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3891 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 883 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3897 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 884 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3903 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 885 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3909 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 886 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3915 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 887 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3921 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 888 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3927 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3933 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3939 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3945 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3951 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3957 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3963 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3969 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3975 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3981 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3987 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3993 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3999 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 901 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4005 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 902 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4011 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 903 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4017 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4023 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 905 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4029 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 906 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4035 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 907 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4041 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4047 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 909 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4053 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 910 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4059 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4065 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 912 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4071 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4077 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 914 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4083 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 915 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4092 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 919 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4101 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 923 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4109 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 926 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4117 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4123 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 930 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4129 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 931 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4137 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 934 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4143 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 935 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4149 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 936 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4155 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4161 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 938 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4167 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 939 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4173 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4188 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4203 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 960 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4239 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 994 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); }
#line 4245 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 995 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl)); }
#line 4251 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 999 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4257 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1000 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4263 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1004 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4269 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1005 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4275 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1009 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4281 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1010 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4287 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1014 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4296 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1018 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4304 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1021 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4313 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1025 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4323 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1030 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4333 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1038 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4341 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1041 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4350 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1045 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4359 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1049 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4369 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1054 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4379 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1062 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4404 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1085 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4412 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1088 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4425 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1096 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4438 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1107 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 4451 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1118 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4466 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1131 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pStruct = make_shared<Structure>(*(yyvsp[0].s));
        if ( !g_Program->addStructure(pStruct) ) {
            yyerror("structure is already defined "+*(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::structure_already_declared);
            (yyval.pStructure) = nullptr;
        } else {
            (yyval.pStructure) = pStruct.get();
        }
        delete (yyvsp[0].s);
    }
#line 4482 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1145 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure)->shared_from_this();
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    if ( !pStruct->findField(name) ) {
                        auto td = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                        auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                        pStruct->fields.emplace_back(name, td, init, pDecl->move_to_init, pDecl->at);
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
#line 4528 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1189 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4539 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1195 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4549 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1203 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 4555 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1204 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 4561 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1205 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 4567 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1206 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 4573 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1207 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 4579 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1208 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 4585 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1209 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 4591 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1210 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 4597 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1211 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 4603 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1212 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 4609 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1213 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 4615 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1214 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 4621 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1215 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 4627 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1216 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 4633 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1217 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 4639 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1218 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 4645 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1219 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 4651 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1220 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 4657 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1221 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 4663 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1222 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 4669 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1226 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4681 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1236 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4690 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1240 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4701 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1249 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4707 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1250 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4713 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1251 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4719 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1252 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4728 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1256 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4738 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1261 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4748 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1266 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4758 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1271 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4768 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1276 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4777 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1280 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4787 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1285 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4798 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1291 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4807 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1295 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4817 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1300 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4842 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1320 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4851 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1324 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4861 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1329 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( size_t ai=0; ai!= pDecl->pNameList->size(); ++ai ) {
                        auto pVarType = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                        if ( pDecl->pInit ) {
                            yyerror("can't have default values in function type declaration",
                                pDecl->at,CompilationError::cant_initialize);
                        }
                        (yyval.pTypeDecl)->argTypes.push_back(pVarType);
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 4886 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1352 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4892 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1353 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4898 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1357 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 4910 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1364 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 4921 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1373 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 4931 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1378 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4940 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1385 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4950 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1390 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4961 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1399 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 4971 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1404 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4980 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1411 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4990 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 4994 "/Users/borisbatkin/work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
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
#line 1418 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


void yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,at,cerr);
}

void yyerror(const string & error, CompilationError cerr) {
    g_Program->error(error,LineInfo(g_AccessStack.back(),yylloc.first_column,yylloc.first_line),cerr);
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
    return LineInfo(g_AccessStack.back(), li.first_column,li.first_line);
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


