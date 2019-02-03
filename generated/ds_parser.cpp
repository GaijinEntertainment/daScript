/* A Bison parser, made by GNU Bison 3.2.4.  */

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
#define YYBISON_VERSION "3.2.4"

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

#line 88 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
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

#line 148 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */

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
    _TBOOL = 286,
    _TVOID = 287,
    _TSTRING = 288,
    _TAUTO = 289,
    _TINT = 290,
    _TINT2 = 291,
    _TINT3 = 292,
    _TINT4 = 293,
    _TUINT = 294,
    _TUINT2 = 295,
    _TUINT3 = 296,
    _TUINT4 = 297,
    _TFLOAT = 298,
    _TFLOAT2 = 299,
    _TFLOAT3 = 300,
    _TFLOAT4 = 301,
    _TRANGE = 302,
    _TURANGE = 303,
    _TBLOCK = 304,
    _TINT64 = 305,
    _TUINT64 = 306,
    _TYPENAME = 307,
    ADDEQU = 308,
    SUBEQU = 309,
    DIVEQU = 310,
    MULEQU = 311,
    MODEQU = 312,
    ANDEQU = 313,
    OREQU = 314,
    XOREQU = 315,
    ADDADD = 316,
    SUBSUB = 317,
    LEEQU = 318,
    GREQU = 319,
    EQUEQU = 320,
    NOTEQU = 321,
    RARROW = 322,
    LARROW = 323,
    QQ = 324,
    QDOT = 325,
    LPIPE = 326,
    LBPIPE = 327,
    RPIPE = 328,
    INTEGER = 329,
    LONG_INTEGER = 330,
    UNSIGNED_INTEGER = 331,
    UNSIGNED_LONG_INTEGER = 332,
    DOUBLE = 333,
    NAME = 334,
    BEGIN_STRING = 335,
    STRING_CHARACTER = 336,
    END_STRING = 337,
    BEGIN_STRING_EXPR = 338,
    END_STRING_EXPR = 339,
    UNARY_MINUS = 340,
    UNARY_PLUS = 341,
    PRE_INC = 342,
    PRE_DEC = 343,
    POST_INC = 344,
    POST_DEC = 345,
    COLCOL = 346
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

#line 275 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */
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

#endif /* !YY_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   2204

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  249
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  428

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   346

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   102,     2,     2,   116,    98,    91,     2,
     110,   111,    96,    95,    85,    94,   107,    97,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    88,   115,
      92,    86,    93,    87,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   108,     2,   109,    90,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   113,    89,   114,   101,     2,     2,     2,
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
      99,   100,   103,   104,   105,   106,   112
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   201,   201,   203,   204,   205,   206,   207,   208,   209,
     213,   214,   218,   222,   225,   231,   239,   255,   276,   280,
     281,   285,   297,   301,   302,   306,   309,   315,   316,   317,
     324,   331,   342,   351,   352,   353,   354,   355,   359,   364,
     372,   379,   391,   395,   402,   403,   407,   408,   409,   413,
     417,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     456,   525,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   542,   543,   549,   555,   569,   570,   576,   580,
     588,   592,   595,   598,   606,   612,   638,   665,   668,   674,
     677,   683,   686,   692,   697,   748,   749,   750,   751,   752,
     756,   757,   758,   759,   760,   761,   762,   763,   764,   765,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   803,   807,   811,   814,   817,   818,
     819,   822,   823,   824,   825,   826,   836,   846,   880,   881,
     885,   886,   890,   891,   895,   896,   900,   904,   907,   911,
     916,   924,   927,   931,   935,   940,   948,   971,   974,   982,
     993,  1006,  1049,  1055,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,  1081,  1085,  1095,  1099,  1108,  1109,  1110,  1111,
    1115,  1120,  1125,  1130,  1135,  1139,  1144,  1150,  1154,  1159,
    1182,  1183,  1187,  1194,  1203,  1208,  1215,  1223,  1228,  1235
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
  "_OPERATOR", "_ENUM", "_TBOOL", "_TVOID", "_TSTRING", "_TAUTO", "_TINT",
  "_TINT2", "_TINT3", "_TINT4", "_TUINT", "_TUINT2", "_TUINT3", "_TUINT4",
  "_TFLOAT", "_TFLOAT2", "_TFLOAT3", "_TFLOAT4", "_TRANGE", "_TURANGE",
  "_TBLOCK", "_TINT64", "_TUINT64", "_TYPENAME", "ADDEQU", "SUBEQU",
  "DIVEQU", "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "ADDADD",
  "SUBSUB", "LEEQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW", "LARROW", "QQ",
  "QDOT", "LPIPE", "LBPIPE", "RPIPE", "INTEGER", "LONG_INTEGER",
  "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER", "DOUBLE", "NAME",
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
     335,   336,   337,   338,   339,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   340,
     341,   126,    33,   342,   343,   344,   345,    46,    91,    93,
      40,    41,   346,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -254

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-254)))

#define YYTABLE_NINF -121

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -254,    11,  -254,   -93,   -41,    48,    55,    72,    17,  -254,
    -254,  -254,   135,  -254,  -254,  -254,  -254,  -254,    41,  -254,
     107,   121,   153,  -254,  -254,   163,  -254,   139,   160,   194,
    -254,     5,   168,   202,   215,   -69,   104,   -41,   211,    48,
      55,  -254,   209,  -254,    17,  -254,   -41,   179,  1691,   264,
     273,  -254,   196,  -254,  -254,   192,   -60,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,   -65,  -254,  -254,
     155,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
      26,   221,  -254,  -254,   236,  -254,   185,  1183,   109,   -81,
    -254,  -254,     1,  -254,   -92,  -254,   185,   205,  -254,   228,
     232,  -254,  -254,  -254,   216,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,   241,
    -254,  -254,  -254,  -254,  -254,  -254,   188,  -254,  -254,   224,
      17,  -254,   226,  1183,  1183,  -254,  -254,  -254,  -254,  -254,
     -29,  -254,  1183,  1183,  1183,  1183,   229,  1183,  -254,   233,
    -254,  -254,  -254,    22,  -254,  -254,  -254,  -254,  -254,   268,
    1373,   237,  -254,  -254,  -254,  -254,  -254,   269,  -254,  -254,
    -254,   263,     4,  -254,   185,   185,   267,    21,  -254,  -254,
    -254,   -12,   278,  1183,   658,  -254,   763,   252,   252,   164,
     252,   252,   252,   252,   185,  1530,   343,   196,  -254,   868,
      86,  1183,  1183,  1183,  1183,  1183,  1183,  1183,  1183,  -254,
    -254,  1183,  1183,  1183,  1183,  -254,  1183,  1183,   275,  1183,
    1183,  1183,  1183,  1183,  1183,  1183,  1183,  1183,  1183,  1183,
    1183,  1183,  1183,   289,  1183,   973,   257,  -254,    16,   152,
     281,   221,   178,  -254,  -254,   290,  1878,   185,  1581,   185,
    1632,  -254,  1183,   319,   448,  -254,  -254,    25,  1183,  1183,
     263,  1078,  -254,   205,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,   286,   287,  -254,  -254,   -68,  1429,   221,  -254,   156,
    1878,  -254,  1878,  1878,  1878,  1878,  1878,  1878,  1878,  1878,
    2096,  2096,  2081,  2081,  1878,   252,  -254,    90,    90,  1878,
    1782,  1974,  2022,  2039,  2096,  2096,   300,   300,   444,   444,
     444,  -254,  1733,  -254,   157,  -254,  -254,   185,  -254,   310,
    -254,  -254,   -14,  -254,    -9,  -254,  1830,   102,   553,   -57,
    1878,   321,   -82,   136,   263,  -254,   242,  1477,  1477,   -11,
    1183,  1878,   399,  -254,  -254,  -254,   205,  1183,  -254,  1183,
    -254,  -254,   322,  -254,  -254,  -254,  -254,  1183,   332,   303,
     335,   307,  1183,    96,  -254,   185,  1183,  -254,    12,  1183,
    1878,   205,  -254,  1878,  1926,  -254,  1878,   339,  -254,   340,
     321,  -254,  1878,   205,   263,   230,  -254,  1247,   205,  1183,
    -254,    67,  -254,  1183,  -254,  -254,  -254,  1183,  -254,  -254,
    1477,  -254,  1878,  -254,  1310,    12,  -254,  -254
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    44,     1,     0,     0,     0,     0,     0,     0,     9,
       8,     7,     0,     6,     5,     4,     3,   178,     0,    38,
      17,    25,    22,    23,    21,    18,    19,     0,    96,     0,
      42,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,   197,     0,    98,     0,    45,     0,     0,     0,     0,
       0,    51,    46,   202,   196,     0,   186,    36,    37,    34,
      35,    10,    33,    39,    26,    24,    20,     0,    97,    43,
       0,   178,    54,    55,    57,    56,    58,    59,    60,    61,
      78,    79,    71,    72,    69,    70,    74,    75,    73,    67,
      68,    63,    62,    64,    65,    66,    53,    52,    76,    77,
       0,    49,   179,   185,     0,   184,     0,     0,     0,     0,
     200,    41,     0,    47,     0,   182,     0,     0,   203,     0,
       0,   204,   220,   205,   224,   206,   208,   209,   210,   211,
     213,   214,   215,   216,   217,   218,   219,   221,   222,   237,
     207,   212,   223,   226,   228,   227,   187,   135,   136,     0,
       0,   130,     0,     0,     0,   115,   117,   116,   118,   119,
     131,    13,     0,     0,     0,     0,     0,     0,    92,    44,
     133,   113,   190,     0,   173,   171,   172,   174,   132,     0,
     189,     0,   134,   240,   241,    11,    12,     0,   198,   201,
      48,     0,    50,    80,     0,     0,     0,    46,   230,   234,
     232,     0,     0,     0,     0,    99,     0,   156,   157,    10,
     140,   139,   138,   137,     0,     0,     0,    46,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
     159,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,    49,     0,   231,   233,     0,   188,     0,     0,     0,
       0,    16,     0,    14,     0,   160,    94,     0,     0,     0,
       0,   101,   100,     0,    81,    82,    90,    87,    86,    93,
      83,     0,     0,    91,    85,     0,   120,    49,   164,     0,
     111,    95,   125,   126,   128,   127,   129,   122,   123,   124,
     151,   152,   149,   150,   141,   169,   163,   175,   176,   121,
       0,   154,   155,   153,   147,   148,   143,   142,   144,   145,
     146,   162,     0,   166,     0,   199,   235,     0,   225,     0,
     238,   229,     0,   107,     0,   109,     0,   131,     0,     0,
     247,   244,     0,     0,     0,   106,     0,     0,     0,     0,
       0,   102,     0,    88,    89,    84,     0,     0,   165,     0,
     161,   167,     0,   239,   108,   110,    15,     0,     0,     0,
       0,     0,     0,     0,   180,     0,     0,    32,    27,     0,
     103,     0,   114,   112,   170,   236,   242,     0,   246,     0,
     245,   249,   248,     0,     0,     0,   195,   120,     0,     0,
      30,     0,   104,     0,   105,   181,   191,     0,   194,    28,
       0,    31,   243,   193,   120,    27,   192,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -254,  -254,   218,  -254,  -254,  -254,  -254,  -254,  -254,   388,
    -254,  -254,   390,     6,  -254,  -254,  -254,   395,   387,   391,
    -254,   265,  -173,  -249,  -254,  -254,  -108,  -254,  -254,  -209,
      -8,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -253,   219,
    -254,   220,   -78,   369,  -254,  -254,  -143,   -99,  -254,  -254,
    -254,  -254,  -254,     7,   -84,  -254,  -254,   -74,  -254,    61,
    -254,  -254,  -254,  -254
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   108,    62,   209,   170,     9,    10,    25,    26,
      11,    22,    23,   410,   286,   287,   288,    19,    20,    30,
      31,    12,   101,   117,    52,    13,   171,   289,   216,   172,
     173,   174,   291,   292,   293,   294,   175,   176,   299,   177,
     178,   179,   300,    35,   383,   114,   107,    55,   355,    14,
      67,    15,    16,    56,   181,   144,   145,   146,   182,   351,
     352,   183,   353,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,   115,   334,   203,   220,   187,   389,   290,   103,   193,
      53,     2,   339,   198,   109,   263,     3,   265,   198,   190,
      17,   408,   143,   191,   261,   104,   105,   379,   106,   180,
     409,   198,   143,   380,   188,     4,    32,     5,    18,     6,
     119,     7,   192,   198,   297,    54,   120,   365,   366,   110,
     -96,   214,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   199,   104,   207,   208,   200,   199,   264,
      53,   -96,   200,    42,   210,   211,   212,   213,   201,   215,
      44,   199,   257,   201,   202,   200,    28,   374,   142,   202,
      28,   218,   375,   199,    53,    53,   201,   200,   142,   336,
     143,   143,   202,   143,    45,   189,    57,    58,   201,     8,
     258,   259,    21,   262,   202,   266,   268,    36,   270,    29,
     143,   100,   219,    29,    24,   354,   411,   113,   296,    33,
     274,    34,   205,   302,   303,   304,   305,   306,   307,   308,
     309,    27,   367,   310,   311,   312,   313,   235,   314,   315,
     238,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   362,   332,   406,    59,   198,
     168,   -96,    60,   143,    61,   143,   142,   142,   377,   142,
     185,   186,    37,   342,   346,   344,   350,   253,   254,   168,
     357,   358,   169,   361,   119,   198,   142,   403,   423,    38,
     120,   404,   -96,   386,    42,   198,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   337,    39,   199,
      37,   367,   367,   200,    48,   381,   271,   272,    40,   387,
     388,   382,    41,   143,   201,   384,   103,   198,   392,   142,
     202,   142,   417,   372,    28,   199,   111,   368,   371,   200,
     213,   340,    42,    43,   105,   199,    49,    50,    46,   200,
     201,    47,   390,   412,   356,    64,   202,   359,    68,   393,
     201,   394,    71,    98,    51,   414,   202,    29,   103,   396,
     419,   143,    99,   421,   402,   415,   100,   102,   407,   116,
     103,   405,   425,   229,   230,   118,   105,   199,   168,   235,
     194,   200,   238,   239,   195,   240,   196,   104,   105,   142,
     385,   420,   201,   197,   204,   422,   206,   214,   202,   424,
     220,     8,    53,   256,   276,   416,   260,   255,   277,   198,
     278,   279,   265,   280,   316,   147,   148,   149,   150,   253,
     254,   229,   230,   281,   151,   282,   283,   235,   331,   237,
     238,   239,   335,   240,   121,   122,   123,   142,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   338,   140,   141,   152,   250,   251,   252,   341,
     185,   363,   364,   373,   153,   154,   378,   253,   254,   199,
     391,   397,   398,   200,   399,   395,   401,   155,   156,   157,
     158,   159,   160,   161,   201,   413,   377,   273,    66,    65,
     202,   427,    63,    70,   217,    69,   295,   162,   163,   301,
     112,   400,     0,     0,   164,   165,     0,     0,     0,     0,
       0,   166,     0,   167,     0,    29,   168,   284,   285,   169,
     147,   148,   149,   150,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,   198,     0,     0,     0,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,   229,   230,     0,     0,   153,
     154,   235,     0,   237,   238,   239,     0,   240,     0,     0,
       0,     0,   155,   156,   157,   158,   159,   347,   161,     0,
       0,     0,     0,     0,     0,   199,     0,     0,     0,   200,
       0,     0,   162,   163,     0,     0,     0,     0,     0,   164,
     348,   253,   254,     0,     0,     0,   349,     0,   167,     0,
      29,   168,     0,     0,   169,   147,   148,   149,   150,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   121,   122,   123,     0,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,     0,   140,   141,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,   162,   163,     0,
       0,     0,     0,     0,   164,   165,     0,     0,     0,     0,
       0,   166,     0,   167,     0,    29,   168,     0,     0,   169,
     147,   148,   149,   150,   267,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,   156,   157,   158,   159,   160,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   163,     0,     0,     0,     0,     0,   164,
     165,     0,     0,     0,     0,     0,   166,     0,   167,     0,
      29,   168,     0,     0,   169,   147,   148,   149,   150,   269,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   123,     0,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,     0,   140,   141,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,   163,     0,
       0,     0,     0,     0,   164,   165,     0,     0,     0,     0,
       0,   166,     0,   167,     0,    29,   168,     0,     0,   169,
     147,   148,   149,   150,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,   156,   157,   158,   159,   160,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   163,     0,     0,     0,     0,     0,   164,
     165,     0,     0,     0,     0,     0,   166,     0,   167,   298,
      29,   168,     0,     0,   169,   147,   148,   149,   150,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   123,     0,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,     0,   140,   141,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,   163,     0,
       0,     0,     0,     0,   164,   165,     0,     0,     0,     0,
       0,   166,     0,   167,   333,    29,   168,     0,     0,   169,
     147,   148,   149,   150,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,     0,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,   140,   141,
     152,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     154,     0,     0,     0,     0,     0,   360,     0,     0,     0,
       0,     0,   155,   156,   157,   158,   159,   160,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   163,     0,     0,     0,     0,     0,   164,
     165,     0,     0,     0,     0,     0,   166,     0,   167,     0,
      29,   168,     0,     0,   169,   147,   148,   149,   150,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   123,     0,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,     0,   140,   141,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,   157,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,   163,     0,
       0,     0,     0,     0,   164,   165,     0,     0,     0,     0,
       0,   166,     0,   167,     0,    29,   168,     0,     0,   169,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,     0,     0,     0,
       0,     0,     0,     0,   253,   254,     0,     0,     0,     0,
       0,     0,   418,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
       0,     0,     0,     0,     0,     0,     0,   253,   254,     0,
       0,     0,     0,     0,     0,   426,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,  -120,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,     0,     0,     0,     0,     0,     0,     0,
     253,   254,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   240,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,     0,
       0,     0,     0,     0,     0,     0,   253,   254,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,     0,     0,     0,
       0,     0,     0,     0,   253,   254,     0,     0,     0,     0,
     168,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
       0,     0,     0,     0,     0,     0,     0,   253,   254,     0,
       0,   275,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,     0,     0,     0,     0,     0,     0,     0,   253,   254,
       0,     0,   343,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,     0,     0,     0,     0,     0,     0,     0,   253,
     254,     0,     0,   345,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
       0,     0,    96,    97,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,     0,     0,     0,     0,     0,     0,     0,
     253,   254,   370,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
     369,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,     0,     0,     0,     0,     0,     0,     0,   253,
     254,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
       0,     0,     0,     0,     0,     0,     0,   253,   254,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,     0,     0,
       0,     0,     0,     0,     0,   253,   254,   229,   230,   231,
     232,   233,   234,   235,     0,   237,   238,   239,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,     0,     0,     0,     0,
       0,     0,     0,   253,   254,   229,   230,   231,   232,   233,
     234,   235,     0,   237,   238,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,     0,     0,     0,     0,     0,     0,
       0,   253,   254,   229,   230,   231,   232,   233,   234,   235,
       0,   237,   238,   239,     0,   240,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,     0,   237,   238,
     239,     0,   240,   245,   246,   247,   248,   249,   250,   251,
     252,     0,     0,     0,     0,     0,     0,     0,     0,   253,
     254,   246,   247,   248,   249,   250,   251,   252,     0,     0,
       0,     0,   229,   230,   231,   232,   253,   254,   235,     0,
     237,   238,   239,     0,   240,     0,     0,   229,   230,     0,
       0,     0,     0,   235,     0,   237,   238,   239,     0,   240,
       0,     0,     0,   246,   247,   248,   249,   250,   251,   252,
       0,     0,     0,     0,     0,     0,     0,     0,   253,   254,
     248,   249,   250,   251,   252,     0,     0,     0,     0,     0,
       0,     0,     0,   253,   254
};

static const yytype_int16 yycheck[] =
{
       8,   100,   255,   146,    72,    86,    17,   216,    68,   117,
      79,     0,   261,    27,    79,    27,     5,    74,    27,   111,
     113,     9,   106,   115,   197,    85,    86,   109,    88,   107,
      18,    27,   116,   115,   115,    24,    44,    26,    79,    28,
      19,    30,   116,    27,   217,   114,    25,   115,   297,   114,
      79,   108,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    87,    85,   153,   154,    91,    87,    91,
      79,   110,    91,   112,   162,   163,   164,   165,   102,   167,
      85,    87,   191,   102,   108,    91,    79,   111,   106,   108,
      79,    79,   111,    87,    79,    79,   102,    91,   116,    93,
     194,   195,   108,   197,   109,   114,    12,    13,   102,   108,
     194,   195,    74,   197,   108,   203,   204,    86,   206,   112,
     214,   110,   110,   112,    79,   110,   389,   111,   216,     4,
     214,     6,   150,   221,   222,   223,   224,   225,   226,   227,
     228,    79,    85,   231,   232,   233,   234,    67,   236,   237,
      70,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   283,   254,   386,    74,    27,
     113,    79,    78,   267,    80,   269,   194,   195,    86,   197,
      81,    82,    85,   267,   272,   269,   274,   107,   108,   113,
     278,   279,   116,   281,    19,    27,   214,   111,   417,    88,
      25,   115,   110,   356,   112,    27,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    85,    85,    87,
      85,    85,    85,    91,    29,   109,    82,    83,    85,   357,
     358,   115,   113,   337,   102,   354,    68,    27,   366,   267,
     108,   269,   405,   337,    79,    87,   111,   111,   111,    91,
     348,    93,   112,    79,    86,    87,    61,    62,   110,    91,
     102,    79,   360,   391,   277,    74,   108,   280,    79,   367,
     102,   369,   113,    29,    79,   403,   108,   112,    68,   377,
     408,   385,    29,   411,   382,   404,   110,   115,   386,    88,
      68,   385,   420,    61,    62,    79,    86,    87,   113,    67,
      92,    91,    70,    71,    92,    73,   110,    85,    86,   337,
      88,   409,   102,    92,   110,   413,   110,   108,   108,   417,
      72,   108,    79,    74,     1,   115,    79,   110,     5,    27,
       7,     8,    74,    10,    79,    12,    13,    14,    15,   107,
     108,    61,    62,    20,    21,    22,    23,    67,    79,    69,
      70,    71,   115,    73,    31,    32,    33,   385,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,   111,    50,    51,    52,    96,    97,    98,   109,
      81,   115,   115,    93,    61,    62,    85,   107,   108,    87,
      11,    79,   109,    91,    79,    93,   109,    74,    75,    76,
      77,    78,    79,    80,   102,    86,    86,   209,    40,    39,
     108,   425,    37,    46,   169,    44,   216,    94,    95,   220,
      71,   380,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,    -1,   112,   113,   114,   115,   116,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    61,
      62,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,   101,
     102,   107,   108,    -1,    -1,    -1,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,
     113,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,   111,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,    -1,   111,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    -1,    -1,   111,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,   107,   108,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    61,    62,    -1,
      -1,    -1,    -1,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   118,     0,     5,    24,    26,    28,    30,   108,   123,
     124,   127,   138,   142,   166,   168,   169,   113,    79,   134,
     135,    74,   128,   129,    79,   125,   126,    79,    79,   112,
     136,   137,   147,     4,     6,   160,    86,    85,    88,    85,
      85,   113,   112,    79,    85,   109,   110,    79,    29,    61,
      62,    79,   141,    79,   114,   164,   170,    12,    13,    74,
      78,    80,   120,   134,    74,   129,   126,   167,    79,   136,
     135,   113,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   101,   102,    29,    29,
     110,   139,   115,    68,    85,    86,    88,   163,   119,    79,
     114,   111,   160,   111,   162,   164,    88,   140,    79,    19,
      25,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,   147,   171,   172,   173,   174,    12,    13,    14,
      15,    21,    52,    61,    62,    74,    75,    76,    77,    78,
      79,    80,    94,    95,   101,   102,   108,   110,   113,   116,
     122,   143,   146,   147,   148,   153,   154,   156,   157,   158,
     159,   171,   175,   178,   180,    81,    82,    86,   115,   114,
     111,   115,   174,   143,    92,    92,   110,    92,    27,    87,
      91,   102,   108,   163,   110,   147,   110,   159,   159,   121,
     159,   159,   159,   159,   108,   159,   145,   138,    79,   110,
      72,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      73,    86,    87,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   107,   108,   110,    74,   164,   174,   174,
      79,   139,   174,    27,    91,    74,   159,    16,   159,    16,
     159,    82,    83,   119,   174,   111,     1,     5,     7,     8,
      10,    20,    22,    23,   114,   115,   131,   132,   133,   144,
     146,   149,   150,   151,   152,   158,   159,   139,   111,   155,
     159,   156,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,    79,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,    79,   159,   111,   155,   115,    93,    85,   111,   140,
      93,   109,   174,   111,   174,   111,   159,    79,   102,   108,
     159,   176,   177,   179,   110,   165,   170,   159,   159,   170,
      68,   159,   143,   115,   115,   115,   140,    85,   111,    88,
     109,   111,   174,    93,   111,   111,    84,    86,    85,   109,
     115,   109,   115,   161,   164,    88,   163,   143,   143,    17,
     159,    11,   143,   159,   159,    93,   159,    79,   109,    79,
     176,   109,   159,   111,   115,   174,   146,   159,     9,    18,
     130,   155,   143,    86,   143,   164,   115,   163,   115,   143,
     159,   143,   159,   146,   159,   143,   115,   130
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     119,   119,   120,   121,   121,   121,   122,   123,   124,   125,
     125,   126,   127,   128,   128,   129,   129,   130,   130,   130,
     131,   132,   133,   134,   134,   134,   134,   134,   135,   135,
     136,   136,   137,   137,   138,   138,   139,   139,   139,   140,
     140,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     142,   143,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   145,   146,   147,   147,   147,   148,
     149,   150,   150,   150,   151,   152,   152,   153,   153,   154,
     154,   155,   155,   156,   156,   157,   157,   157,   157,   157,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   160,   160,
     161,   161,   162,   162,   163,   163,   164,   164,   164,   164,
     164,   165,   165,   165,   165,   165,   166,   167,   167,   167,
     168,   169,   170,   170,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   173,   173,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     175,   175,   176,   176,   177,   177,   178,   179,   179,   180
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
       6,     3,     1,     1,     2,     1,     1,     1,     2,     2,
       1,     1,     0,     2,     2,     3,     1,     3,     2,     2,
       1,     1,     2,     3,     4,     5,     2,     4,     5,     4,
       5,     1,     3,     1,     5,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     3,     3,     3,     4,     3,     4,     2,     3,
       5,     1,     1,     1,     1,     3,     3,     2,     0,     3,
       1,     3,     1,     3,     1,     1,     1,     3,     5,     3,
       3,     4,     6,     5,     4,     3,     4,     0,     3,     5,
       5,     6,     1,     3,     1,     1,     1,     1,     1,     1,
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
          case 79: /* NAME  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1885 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 119: /* character_sequence  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1891 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 120: /* string_constant  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1897 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 121: /* string_builder_body  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1903 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 122: /* string_builder  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1909 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 130: /* expression_else  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1915 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 131: /* expression_if_then_else  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1921 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 132: /* expression_for_loop  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1927 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 133: /* expression_while_loop  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1933 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 134: /* annotation_argument  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 1939 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 135: /* annotation_argument_list  */
#line 81 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 1945 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 136: /* annotation_declaration  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 1951 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* annotation_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1957 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* optional_annotation_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1963 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* optional_function_argument_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 1969 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* optional_function_type  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 1975 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 141: /* function_name  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1981 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* expression_block  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1987 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* expression_any  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1993 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 145: /* expressions  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1999 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* expr_pipe  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2005 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* name_in_namespace  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2011 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* expr_new  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2017 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* expression_break  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2023 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* expression_return  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2029 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expression_try_catch  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2035 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expression_let  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2041 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* expr_sizeof  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2047 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* expr_typename  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2053 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expr_list  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2059 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expr_block  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2065 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* expr_numeric_const  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2071 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expr_assign  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2077 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* expr  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2083 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* struct_variable_declaration_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2089 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* variable_declaration_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2095 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* function_argument_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2101 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* variable_declaration  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2107 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* let_variable_declaration  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2113 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* enum_list  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2119 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* variable_name_list  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2125 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* structure_type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2131 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* auto_type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2137 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2143 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* make_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2149 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* make_struct_fields  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2155 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* make_struct_dim  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2161 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* make_struct_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2167 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* make_dim  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2173 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* make_dim_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2179 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 213 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); }
#line 2467 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 11:
#line 214 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2473 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 218 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2479 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 222 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2487 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 225 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2498 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 231 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2508 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 239 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2526 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 17:
#line 255 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2549 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 21:
#line 285 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( auto mod = Module::require(*(yyvsp[0].s)) ) {
            g_Program->addModule(mod);
        } else {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2563 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 25:
#line 306 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2571 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 309 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2579 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 315 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 2585 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 316 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2591 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 317 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2600 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 324 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2609 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 331 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pFor;
    }
#line 2622 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 342 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile();
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWhile;
    }
#line 2633 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 351 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2639 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 352 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i));  }
#line 2645 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 353 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].d)));  }
#line 2651 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 354 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true);  }
#line 2657 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 355 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false);  }
#line 2663 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 359 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 2673 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 364 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 2683 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 372 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 2695 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 379 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 2709 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 391 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2718 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 395 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2727 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 402 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 2733 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 403 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 2739 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 407 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2745 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 408 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2751 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 409 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 2757 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 413 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(yylloc.first_column,yylloc.first_line);
    }
#line 2766 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 417 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 2775 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 424 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2781 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 425 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 2787 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 426 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 2793 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 427 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 2799 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 428 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 2805 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 429 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 2811 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 430 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 2817 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 431 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 2823 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 432 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 2829 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 433 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 2835 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 434 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 2841 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 435 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 2847 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 436 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 2853 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 437 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 2859 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 438 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 2865 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 439 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 2871 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 2877 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 2883 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 2889 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 443 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 2895 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 444 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 2901 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 2907 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 446 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 2913 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 2919 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 2925 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 2931 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 450 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 2937 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 451 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 2943 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 452 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 2949 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 457 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3019 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 525 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3025 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 529 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3031 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 530 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3037 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 531 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3043 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 532 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3049 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 533 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3055 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 534 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3061 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 535 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3067 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 536 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3073 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 537 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3079 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 538 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3085 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 542 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBlock(); }
#line 3091 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 543 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3102 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 549 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3110 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 555 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3126 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 569 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3132 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 570 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3143 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 576 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3149 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 580 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pTypeDecl = g_Program->makeTypeDeclaration(tokAt((yylsp[-1])), *(yyvsp[0].s));
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr(pTypeDecl));
        delete (yyvsp[0].s);
    }
#line 3159 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3165 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3173 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 595 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3181 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 598 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3191 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 606 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3199 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 612 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3230 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 638 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3259 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 665 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3267 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 668 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3275 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 674 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3283 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 677 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3291 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 683 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3299 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 686 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3307 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 692 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3317 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 698 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3369 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 748 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3375 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 749 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3381 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 750 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3387 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 751 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3393 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 752 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].d)); }
#line 3399 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 756 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3405 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 757 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3411 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 758 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3417 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 759 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3423 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 760 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3429 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 761 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3435 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 762 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3441 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 763 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3447 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 764 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3453 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 765 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3459 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 769 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3465 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 770 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3471 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3477 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 772 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3483 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 773 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3489 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 774 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3495 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 775 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3501 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 776 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3507 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 777 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3513 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 778 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3519 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 779 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3525 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 780 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3531 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 781 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3537 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 782 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3543 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3549 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 784 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3555 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 785 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3561 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3567 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 787 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3573 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 788 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3579 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 789 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3585 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 790 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3591 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 791 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3597 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 792 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3603 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 793 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3609 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 794 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3615 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 795 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3621 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 796 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3627 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 797 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3633 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 798 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3639 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 799 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3645 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 800 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3651 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 801 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3657 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 802 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3663 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 803 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 3672 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 807 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 3681 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 811 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 3689 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 814 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 3697 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 817 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[0])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3703 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 818 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3709 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 819 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 3717 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 822 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3723 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 823 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3729 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 824 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3735 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 825 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3741 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 826 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3756 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 836 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3771 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 846 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3807 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 880 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); }
#line 3813 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 881 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl)); }
#line 3819 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 885 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3825 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 886 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3831 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3837 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3843 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 3849 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 3855 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 3864 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 3872 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 907 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3881 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3891 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 916 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3901 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 3909 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 927 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3918 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 931 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3927 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 935 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3937 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3947 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 948 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3972 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 971 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 3980 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 974 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 3993 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 982 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4006 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 993 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4021 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 1006 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4066 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 1049 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4077 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 1055 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4087 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 1063 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 4093 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 1064 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 4099 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 1065 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 4105 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 1066 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 4111 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 1067 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 4117 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 1068 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 4123 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 1069 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 4129 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 1070 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 4135 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 1071 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 4141 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 1072 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 4147 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 1073 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 4153 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 1074 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 4159 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 1075 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 4165 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 1076 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 4171 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 1077 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 4177 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 1078 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 4183 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 1079 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 4189 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 1080 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 4195 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 1081 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 4201 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1085 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4213 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1095 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4222 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1099 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4233 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1108 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4239 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1109 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4245 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1110 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4251 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1111 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4260 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1115 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4270 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1120 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4280 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1125 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4290 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1130 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4300 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1135 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4309 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1139 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4319 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1144 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4330 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1150 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4339 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1154 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4349 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1159 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4374 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1182 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4380 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1183 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4386 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1187 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 4398 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1194 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 4409 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 244:
#line 1203 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 4419 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 245:
#line 1208 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4428 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 246:
#line 1215 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4438 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 247:
#line 1223 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 4448 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 248:
#line 1228 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4457 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 249:
#line 1235 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4467 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 4471 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
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
#line 1242 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


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


