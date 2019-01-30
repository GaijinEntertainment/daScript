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
    _TBOOL = 285,
    _TVOID = 286,
    _TSTRING = 287,
    _TAUTO = 288,
    _TINT = 289,
    _TINT2 = 290,
    _TINT3 = 291,
    _TINT4 = 292,
    _TUINT = 293,
    _TUINT2 = 294,
    _TUINT3 = 295,
    _TUINT4 = 296,
    _TFLOAT = 297,
    _TFLOAT2 = 298,
    _TFLOAT3 = 299,
    _TFLOAT4 = 300,
    _TRANGE = 301,
    _TURANGE = 302,
    _TBLOCK = 303,
    _TINT64 = 304,
    _TUINT64 = 305,
    _TYPENAME = 306,
    ADDEQU = 307,
    SUBEQU = 308,
    DIVEQU = 309,
    MULEQU = 310,
    MODEQU = 311,
    ANDEQU = 312,
    OREQU = 313,
    XOREQU = 314,
    ADDADD = 315,
    SUBSUB = 316,
    LEEQU = 317,
    GREQU = 318,
    EQUEQU = 319,
    NOTEQU = 320,
    RARROW = 321,
    LARROW = 322,
    QQ = 323,
    QDOT = 324,
    LPIPE = 325,
    LBPIPE = 326,
    RPIPE = 327,
    INTEGER = 328,
    LONG_INTEGER = 329,
    UNSIGNED_INTEGER = 330,
    UNSIGNED_LONG_INTEGER = 331,
    DOUBLE = 332,
    NAME = 333,
    BEGIN_STRING = 334,
    STRING_CHARACTER = 335,
    END_STRING = 336,
    BEGIN_STRING_EXPR = 337,
    END_STRING_EXPR = 338,
    UNARY_MINUS = 339,
    UNARY_PLUS = 340,
    PRE_INC = 341,
    PRE_DEC = 342,
    POST_INC = 343,
    POST_DEC = 344,
    COLCOL = 345
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
    

#line 274 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   2185

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  243
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  416

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   345

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   101,     2,     2,   115,    97,    90,     2,
     109,   110,    95,    94,    84,    93,   106,    96,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    87,   114,
      91,    85,    92,    86,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   107,     2,   108,    89,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   112,    88,   113,   100,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    98,
      99,   102,   103,   104,   105,   111
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   198,   198,   200,   201,   202,   203,   204,   205,   209,
     210,   214,   218,   221,   227,   235,   251,   272,   276,   277,
     281,   293,   297,   298,   302,   305,   311,   312,   313,   320,
     327,   338,   347,   348,   349,   350,   351,   355,   360,   368,
     375,   387,   391,   398,   399,   403,   404,   405,   409,   413,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   452,
     521,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   538,   539,   545,   551,   565,   566,   572,   576,   584,
     588,   591,   594,   602,   608,   634,   661,   664,   670,   673,
     679,   682,   688,   693,   744,   745,   746,   747,   748,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   761,   765,
     766,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   803,   807,   810,   813,   814,   815,
     818,   819,   820,   821,   822,   832,   845,   846,   850,   851,
     855,   856,   860,   861,   865,   869,   872,   876,   881,   889,
     892,   896,   900,   905,   913,   936,   979,   985,   993,   994,
     995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,
    1005,  1006,  1007,  1008,  1009,  1010,  1011,  1015,  1025,  1029,
    1038,  1039,  1040,  1041,  1045,  1050,  1055,  1060,  1065,  1069,
    1074,  1080,  1084,  1089,  1112,  1113,  1117,  1124,  1133,  1138,
    1145,  1153,  1158,  1165
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
  "_OPERATOR", "_TBOOL", "_TVOID", "_TSTRING", "_TAUTO", "_TINT", "_TINT2",
  "_TINT3", "_TINT4", "_TUINT", "_TUINT2", "_TUINT3", "_TUINT4", "_TFLOAT",
  "_TFLOAT2", "_TFLOAT3", "_TFLOAT4", "_TRANGE", "_TURANGE", "_TBLOCK",
  "_TINT64", "_TUINT64", "_TYPENAME", "ADDEQU", "SUBEQU", "DIVEQU",
  "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "ADDADD", "SUBSUB",
  "LEEQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW", "LARROW", "QQ", "QDOT",
  "LPIPE", "LBPIPE", "RPIPE", "INTEGER", "LONG_INTEGER",
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
  "let_variable_declaration", "global_let", "structure_declaration",
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
     335,   336,   337,   338,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   339,   340,
     126,    33,   341,   342,   343,   344,    46,    91,    93,    40,
      41,   345,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -250

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-250)))

#define YYTABLE_NINF -120

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -250,    10,  -250,  -100,   -53,    25,    -4,    17,  -250,  -250,
    -250,   102,  -250,  -250,  -250,  -250,    27,  -250,    35,    95,
      47,  -250,  -250,   106,  -250,    84,   127,  -250,   -76,   100,
     154,   207,     9,    30,   -53,   165,    25,    -4,   162,  -250,
      17,  -250,   -53,   138,  1673,   228,   234,  -250,   160,  -250,
    -250,   158,   226,  -250,  -250,  -250,  -250,  -250,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,   -65,  -250,  -250,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,   -75,   184,  -250,  -250,   195,
    -250,   181,  1165,    44,  -250,    56,  -250,   -88,  -250,   181,
     163,  -250,   186,   188,  -250,  -250,  -250,   171,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -250,
    -250,  -250,   193,  -250,  -250,  -250,  -250,  -250,  -250,   219,
    -250,  -250,   178,    17,  -250,   179,  1165,  1165,  -250,  -250,
    -250,  -250,  -250,    99,  -250,  1165,  1165,  1165,  1165,   187,
    1165,  -250,   190,  -250,  -250,  -250,   192,  -250,  -250,  -250,
    -250,  -250,   235,  1355,   199,  -250,  -250,  -250,  -250,  -250,
    -250,  -250,   237,   -11,  -250,   181,   181,   246,    21,  -250,
    -250,  -250,   -13,   239,  1165,   645,  -250,   749,   353,   353,
     153,   353,   353,   353,   353,   181,  1512,   333,   160,   853,
      60,  1165,  1165,  1165,  1165,  1165,  1165,  1165,  1165,  -250,
    -250,  1165,  1165,  1165,  1165,  -250,  1165,  1165,   247,  1165,
    1165,  1165,  1165,  1165,  1165,  1165,  1165,  1165,  1165,  1165,
    1165,  1165,  1165,   250,  1165,   957,  -250,   147,   157,   211,
     184,   243,  -250,  -250,   222,  1860,   181,  1563,   181,  1614,
    -250,  1165,   252,   437,  -250,  -250,   -60,  1165,  1165,   237,
    1061,  -250,   163,  -250,  -250,  -250,  -250,  -250,  -250,  -250,
     223,   225,  -250,  -250,   -67,  1411,   184,  -250,     1,  1860,
    -250,  1860,  1860,  1860,  1860,  1860,  1860,  1860,  1860,  2078,
    2078,  2063,  2063,  1860,   353,  -250,    85,    85,  1860,  1764,
    1956,  2004,  2021,  2078,  2078,   602,   602,   329,   329,   329,
    -250,  1715,  -250,     4,  -250,   181,  -250,   244,  -250,  -250,
     -10,  -250,     3,  -250,  1812,    92,   541,   -66,  1860,   258,
     -87,   -85,   237,  -250,   232,  1459,  1459,    -6,  1165,  1860,
     338,  -250,  -250,  -250,   163,  1165,  -250,  1165,  -250,  -250,
     295,  -250,  -250,  -250,  -250,  1165,   273,   249,   274,   251,
    1165,    -9,  -250,   181,  1165,  -250,   167,  1165,  1860,   163,
    -250,  1860,  1908,  -250,  1860,   275,  -250,   276,   258,  -250,
    1860,   163,   237,   175,  -250,  1229,   163,  1165,  -250,   -64,
    -250,  1165,  -250,  -250,  -250,  1165,  -250,  -250,  1459,  -250,
    1860,  -250,  1292,   167,  -250,  -250
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    43,     1,     0,     0,     0,     0,     0,     8,     7,
       6,     0,     5,     4,     3,   176,     0,    37,    16,    24,
      21,    22,    20,    17,    18,    95,     0,    41,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,    44,     0,     0,     0,     0,     0,    50,    45,   196,
     194,     0,   184,    35,    36,    33,    34,     9,    32,    38,
      25,    23,    19,    96,    42,     0,   176,    53,    54,    56,
      55,    57,    58,    59,    60,    77,    78,    70,    71,    68,
      69,    73,    74,    72,    66,    67,    62,    61,    63,    64,
      65,    52,    51,    75,    76,     0,    48,   177,   183,     0,
     182,     0,     0,     0,    40,     0,    46,     0,   180,     0,
       0,   197,     0,     0,   198,   214,   199,   218,   200,   202,
     203,   204,   205,   207,   208,   209,   210,   211,   212,   213,
     215,   216,   231,   201,   206,   217,   220,   222,   221,   185,
     134,   135,     0,     0,   129,     0,     0,     0,   114,   116,
     115,   117,   118,   130,    12,     0,     0,     0,     0,     0,
       0,    91,    43,   132,   112,   188,     0,   172,   170,   171,
     173,   131,     0,   187,     0,   133,   234,   235,    10,    11,
     195,    47,     0,    49,    79,     0,     0,     0,    45,   224,
     228,   226,     0,     0,     0,     0,    98,     0,   155,   156,
       9,   139,   138,   137,   136,     0,     0,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
     158,     0,     0,     0,     0,   167,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,     0,     0,
      48,     0,   225,   227,     0,   186,     0,     0,     0,     0,
      15,     0,    13,     0,   159,    93,     0,     0,     0,     0,
     100,    99,     0,    80,    81,    89,    86,    85,    92,    82,
       0,     0,    90,    84,     0,   119,    48,   163,     0,   110,
      94,   124,   125,   127,   126,   128,   121,   122,   123,   150,
     151,   148,   149,   140,   168,   162,   174,   175,   120,     0,
     153,   154,   152,   146,   147,   142,   141,   143,   144,   145,
     161,     0,   165,     0,   229,     0,   219,     0,   232,   223,
       0,   106,     0,   108,     0,   130,     0,     0,   241,   238,
       0,     0,     0,   105,     0,     0,     0,     0,     0,   101,
       0,    87,    88,    83,     0,     0,   164,     0,   160,   166,
       0,   233,   107,   109,    14,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,    31,    26,     0,   102,     0,
     113,   111,   169,   230,   236,     0,   240,     0,   239,   243,
     242,     0,     0,     0,   193,   119,     0,     0,    29,     0,
     103,     0,   104,   179,   189,     0,   192,    27,     0,    30,
     237,   191,   119,    26,   190,    28
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -250,  -250,   191,  -250,  -250,  -250,  -250,  -250,  -250,   321,
    -250,  -250,   326,   -27,  -250,  -250,  -250,   354,   350,   360,
    -250,   241,  -164,  -249,  -250,  -250,  -101,  -250,  -250,  -201,
      -7,  -250,  -250,  -250,  -250,  -250,  -250,  -250,  -232,   194,
    -250,   198,   -74,   349,  -250,  -250,  -137,   -90,  -250,  -250,
    -250,   -86,   -70,  -250,  -250,   -59,  -250,    48,  -250,  -250,
    -250,  -250
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   103,    58,   200,   163,     8,     9,    23,    24,
      10,    20,    21,   398,   275,   276,   277,    17,    18,    27,
      28,    11,    96,   110,    48,    12,   164,   278,   207,   165,
     166,   167,   280,   281,   282,   283,   168,   169,   288,   170,
     171,   172,   289,    32,   371,   107,   102,    51,   343,    13,
      14,    52,   174,   137,   138,   139,   175,   339,   340,   176,
     341,   177
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      29,   327,   194,    49,   210,   108,   279,   254,    40,   184,
       2,   377,    15,   323,   252,     3,   189,   189,    49,    34,
     355,   367,   181,   369,   250,    16,   182,   368,   173,   370,
     189,   136,    41,    29,     4,   106,     5,   354,     6,   136,
     112,   205,    53,    54,   286,   104,   113,   353,   161,   342,
     183,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   198,   199,    22,   190,   190,   253,    99,   191,
     191,   201,   202,   203,   204,   355,   206,    49,   355,   190,
     192,   192,   246,   191,   135,    25,   193,   193,    19,    25,
     362,   391,   135,    55,   192,   392,    30,    56,    31,    57,
     193,   356,    33,   363,   359,   136,   136,     7,   136,    34,
     255,   257,    50,   259,   178,   179,   247,   248,    26,   251,
      95,    36,    26,   285,    49,   136,   196,   291,   292,   293,
     294,   295,   296,   297,   298,   399,   263,   299,   300,   301,
     302,   225,   303,   304,   228,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   180,
     321,   350,   161,   394,   189,   162,   396,   365,   135,   135,
     344,   135,    35,   347,   189,   397,   136,   334,   136,   338,
      37,   243,   244,   345,   346,    38,   349,   330,   135,   332,
     112,   -95,   189,    38,   411,    39,   113,   374,   -95,    42,
      38,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    43,   190,   260,   261,    44,   191,    60,   324,
      63,   325,    98,   190,   375,   376,   189,   191,   192,   135,
      66,   135,   372,   380,   193,   136,   405,    93,   192,    25,
     100,   190,   204,    94,   193,   191,   360,    45,    46,    95,
     189,   109,    97,   111,   378,   161,   192,   185,   400,   186,
     187,   381,   193,   382,   188,    47,    98,   195,   197,   404,
     402,   384,    26,    98,   205,   407,   390,     7,   409,    98,
     395,   209,   403,   136,   100,   190,   210,   413,   245,   191,
      99,   100,   254,   101,   393,    49,    99,   100,   135,   373,
     192,   326,   189,   408,   249,   305,   193,   410,   320,   190,
     329,   412,   178,   191,   265,   328,   361,   351,   266,   352,
     267,   268,   366,   269,   192,   140,   141,   142,   143,   379,
     193,   385,   387,   270,   144,   271,   272,   386,    62,   389,
     401,   365,    61,   114,   115,   116,   135,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   190,   133,   134,   145,   191,   415,   383,    59,   219,
     220,   262,    65,   146,   147,   225,   192,   227,   228,   229,
      64,   230,   193,   208,   290,   284,   148,   149,   150,   151,
     152,   153,   154,   219,   220,   105,   388,     0,     0,   225,
       0,     0,   228,   229,     0,   230,   155,   156,     0,     0,
       0,     0,     0,   157,   158,   243,   244,     0,     0,     0,
     159,     0,   160,     0,    26,   161,   273,   274,   162,   140,
     141,   142,   143,     0,     0,     0,     0,     0,   144,   243,
     244,     0,     0,     0,   189,     0,     0,   114,   115,   116,
       0,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   133,   134,   145,     0,
       0,     0,     0,     0,     0,     0,     0,   146,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,   151,   152,   335,   154,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   191,     0,     0,
     155,   156,     0,     0,     0,     0,     0,   157,   336,     0,
       0,     0,     0,     0,   337,     0,   160,     0,    26,   161,
       0,     0,   162,   140,   141,   142,   143,     0,     0,     0,
       0,     0,   144,     0,     0,     0,     0,     0,   252,     0,
       0,   114,   115,   116,     0,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     133,   134,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   148,   149,   150,   151,   152,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,     0,     0,   155,   156,     0,     0,     0,     0,
       0,   157,   158,     0,     0,     0,     0,     0,   159,     0,
     160,     0,    26,   161,     0,     0,   162,   140,   141,   142,
     143,   256,   219,   220,     0,     0,   144,     0,   225,     0,
     227,   228,   229,     0,   230,   114,   115,   116,     0,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   133,   134,   145,   240,   241,   242,
       0,     0,     0,     0,     0,   146,   147,     0,   243,   244,
       0,     0,     0,     0,     0,     0,     0,     0,   148,   149,
     150,   151,   152,   153,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,   156,
       0,     0,     0,     0,     0,   157,   158,     0,     0,     0,
       0,     0,   159,     0,   160,     0,    26,   161,     0,     0,
     162,   140,   141,   142,   143,   258,     0,     0,     0,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     115,   116,     0,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,   133,   134,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   148,   149,   150,   151,   152,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,   156,     0,     0,     0,     0,     0,   157,
     158,     0,     0,     0,     0,     0,   159,     0,   160,     0,
      26,   161,     0,     0,   162,   140,   141,   142,   143,     0,
       0,     0,     0,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,   133,   134,   145,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,   149,   150,   151,
     152,   153,   154,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,   156,     0,     0,
       0,     0,     0,   157,   158,     0,     0,     0,     0,     0,
     159,     0,   160,   287,    26,   161,     0,     0,   162,   140,
     141,   142,   143,     0,     0,     0,     0,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
       0,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   133,   134,   145,     0,
       0,     0,     0,     0,     0,     0,     0,   146,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,   151,   152,   153,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,   156,     0,     0,     0,     0,     0,   157,   158,     0,
       0,     0,     0,     0,   159,     0,   160,   322,    26,   161,
       0,     0,   162,   140,   141,   142,   143,     0,     0,     0,
       0,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,     0,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     133,   134,   145,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   147,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,   148,   149,   150,   151,   152,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,   156,     0,     0,     0,     0,
       0,   157,   158,     0,     0,     0,     0,     0,   159,     0,
     160,     0,    26,   161,     0,     0,   162,   140,   141,   142,
     143,     0,     0,     0,     0,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,     0,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   133,   134,   145,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   148,   149,
     150,   151,   152,   153,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,   156,
       0,     0,     0,     0,     0,   157,   158,     0,     0,     0,
       0,     0,   159,     0,   160,     0,    26,   161,     0,     0,
     162,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   231,   232,     0,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,     0,     0,     0,
       0,     0,     0,   406,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,     0,   230,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   231,   232,     0,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,     0,     0,     0,     0,     0,   414,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,  -119,   230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,     0,   230,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,   232,     0,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,     0,     0,     0,
       0,   161,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,     0,   230,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,     0,   264,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,     0,   230,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,     0,     0,   331,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   244,     0,     0,   333,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,     0,     0,    91,    92,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,     0,   230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,   358,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,   357,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   244,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,     0,   230,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   364,     0,     0,   232,     0,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,     0,   230,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   219,   220,
     221,   222,   223,   224,   225,     0,   227,   228,   229,     0,
     230,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,     0,     0,     0,
       0,     0,     0,     0,   243,   244,   219,   220,   221,   222,
     223,   224,   225,     0,   227,   228,   229,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,     0,     0,     0,     0,     0,
       0,     0,   243,   244,   219,   220,   221,   222,   223,   224,
     225,     0,   227,   228,   229,     0,   230,     0,     0,     0,
       0,   219,   220,   221,   222,   223,   224,   225,     0,   227,
     228,   229,     0,   230,   235,   236,   237,   238,   239,   240,
     241,   242,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   244,   236,   237,   238,   239,   240,   241,   242,     0,
       0,     0,     0,   219,   220,   221,   222,   243,   244,   225,
       0,   227,   228,   229,     0,   230,     0,     0,   219,   220,
       0,     0,     0,     0,   225,     0,   227,   228,   229,     0,
     230,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,   238,   239,   240,   241,   242,     0,     0,     0,     0,
       0,     0,     0,     0,   243,   244
};

static const yytype_int16 yycheck[] =
{
       7,   250,   139,    78,    71,    95,   207,    73,    84,   110,
       0,    17,   112,   245,    27,     5,    27,    27,    78,    84,
      84,   108,   110,   108,   188,    78,   114,   114,   102,   114,
      27,   101,   108,    40,    24,   110,    26,   286,    28,   109,
      19,   107,    12,    13,   208,   110,    25,   114,   112,   109,
     109,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,   146,   147,    78,    86,    86,    90,    84,    90,
      90,   155,   156,   157,   158,    84,   160,    78,    84,    86,
     101,   101,   182,    90,   101,    78,   107,   107,    73,    78,
     110,   110,   109,    73,   101,   114,     4,    77,     6,    79,
     107,   110,    85,   110,   110,   185,   186,   107,   188,    84,
     194,   195,   113,   197,    80,    81,   185,   186,   111,   188,
     109,    84,   111,   207,    78,   205,   143,   211,   212,   213,
     214,   215,   216,   217,   218,   377,   205,   221,   222,   223,
     224,    66,   226,   227,    69,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   113,
     244,   272,   112,   374,    27,   115,     9,    85,   185,   186,
     266,   188,    87,   269,    27,    18,   256,   261,   258,   263,
      84,   106,   107,   267,   268,   111,   270,   256,   205,   258,
      19,   109,    27,   111,   405,    78,    25,   344,   109,   109,
     111,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    78,    86,    81,    82,    29,    90,    73,    92,
      78,    84,    67,    86,   345,   346,    27,    90,   101,   256,
     112,   258,   342,   354,   107,   325,   393,    29,   101,    78,
      85,    86,   336,    29,   107,    90,   325,    60,    61,   109,
      27,    87,   114,    78,   348,   112,   101,    91,   379,    91,
     109,   355,   107,   357,    91,    78,    67,   109,   109,   114,
     391,   365,   111,    67,   107,   396,   370,   107,   399,    67,
     374,   109,   392,   373,    85,    86,    71,   408,   109,    90,
      84,    85,    73,    87,   373,    78,    84,    85,   325,    87,
     101,   110,    27,   397,    78,    78,   107,   401,    78,    86,
     108,   405,    80,    90,     1,    92,    92,   114,     5,   114,
       7,     8,    84,    10,   101,    12,    13,    14,    15,    11,
     107,    78,    78,    20,    21,    22,    23,   108,    37,   108,
      85,    85,    36,    30,    31,    32,   373,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    86,    49,    50,    51,    90,   413,    92,    34,    60,
      61,   200,    42,    60,    61,    66,   101,    68,    69,    70,
      40,    72,   107,   162,   210,   207,    73,    74,    75,    76,
      77,    78,    79,    60,    61,    66,   368,    -1,    -1,    66,
      -1,    -1,    69,    70,    -1,    72,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   106,   107,    -1,    -1,    -1,
     107,    -1,   109,    -1,   111,   112,   113,   114,   115,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,   106,
     107,    -1,    -1,    -1,    27,    -1,    -1,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,    -1,   111,   112,
      -1,    -1,   115,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,    -1,   111,   112,    -1,    -1,   115,    12,    13,    14,
      15,    16,    60,    61,    -1,    -1,    21,    -1,    66,    -1,
      68,    69,    70,    -1,    72,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    60,    61,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,    -1,   111,   112,    -1,    -1,
     115,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,    -1,
     111,   112,    -1,    -1,   115,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,   112,    -1,    -1,   115,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,   112,
      -1,    -1,   115,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,    -1,   111,   112,    -1,    -1,   115,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,    -1,   111,   112,    -1,    -1,
     115,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,    -1,
      -1,   112,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      -1,    -1,   110,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,    -1,    -1,   110,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    -1,    -1,   110,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,   100,   101,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    70,    -1,    72,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,   106,   107,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    -1,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,    93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   117,     0,     5,    24,    26,    28,   107,   122,   123,
     126,   137,   141,   165,   166,   112,    78,   133,   134,    73,
     127,   128,    78,   124,   125,    78,   111,   135,   136,   146,
       4,     6,   159,    85,    84,    87,    84,    84,   111,    78,
      84,   108,   109,    78,    29,    60,    61,    78,   140,    78,
     113,   163,   167,    12,    13,    73,    77,    79,   119,   133,
      73,   128,   125,    78,   135,   134,   112,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,   100,   101,    29,    29,   109,   138,   114,    67,    84,
      85,    87,   162,   118,   110,   159,   110,   161,   163,    87,
     139,    78,    19,    25,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   146,   168,   169,   170,   171,
      12,    13,    14,    15,    21,    51,    60,    61,    73,    74,
      75,    76,    77,    78,    79,    93,    94,   100,   101,   107,
     109,   112,   115,   121,   142,   145,   146,   147,   152,   153,
     155,   156,   157,   158,   168,   172,   175,   177,    80,    81,
     113,   110,   114,   171,   142,    91,    91,   109,    91,    27,
      86,    90,   101,   107,   162,   109,   146,   109,   158,   158,
     120,   158,   158,   158,   158,   107,   158,   144,   137,   109,
      71,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      72,    85,    86,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,   106,   107,   109,   163,   171,   171,    78,
     138,   171,    27,    90,    73,   158,    16,   158,    16,   158,
      81,    82,   118,   171,   110,     1,     5,     7,     8,    10,
      20,    22,    23,   113,   114,   130,   131,   132,   143,   145,
     148,   149,   150,   151,   157,   158,   138,   110,   154,   158,
     155,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,    78,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
      78,   158,   110,   154,    92,    84,   110,   139,    92,   108,
     171,   110,   171,   110,   158,    78,   101,   107,   158,   173,
     174,   176,   109,   164,   167,   158,   158,   167,    67,   158,
     142,   114,   114,   114,   139,    84,   110,    87,   108,   110,
     171,    92,   110,   110,    83,    85,    84,   108,   114,   108,
     114,   160,   163,    87,   162,   142,   142,    17,   158,    11,
     142,   158,   158,    92,   158,    78,   108,    78,   173,   108,
     158,   110,   114,   171,   145,   158,     9,    18,   129,   154,
     142,    85,   142,   163,   114,   162,   114,   142,   158,   142,
     158,   145,   158,   142,   114,   129
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   117,   117,   117,   117,   117,   117,   117,   118,
     118,   119,   120,   120,   120,   121,   122,   123,   124,   124,
     125,   126,   127,   127,   128,   128,   129,   129,   129,   130,
     131,   132,   133,   133,   133,   133,   133,   134,   134,   135,
     135,   136,   136,   137,   137,   138,   138,   138,   139,   139,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   141,
     142,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   144,   144,   144,   145,   146,   146,   146,   147,   148,
     149,   149,   149,   150,   151,   151,   152,   152,   153,   153,
     154,   154,   155,   155,   156,   156,   156,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   162,   162,   163,   163,   163,   163,   163,   164,
     164,   164,   164,   164,   165,   166,   167,   167,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   169,   170,   170,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   172,   172,   173,   173,   174,   174,
     175,   176,   176,   177
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     0,
       2,     3,     0,     2,     4,     3,     2,     2,     1,     3,
       1,     2,     1,     3,     1,     3,     0,     2,     4,     4,
       5,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       4,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     1,     1,     2,     1,     1,     1,     2,     2,     1,
       1,     0,     2,     2,     3,     1,     3,     2,     2,     1,
       1,     2,     3,     4,     5,     2,     4,     5,     4,     5,
       1,     3,     1,     5,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     3,     3,     3,     4,     3,     4,     2,     3,     5,
       1,     1,     1,     1,     3,     3,     0,     3,     1,     3,
       1,     3,     1,     1,     1,     3,     5,     3,     3,     4,
       6,     5,     4,     3,     4,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     2,     3,     2,     3,     2,     4,
       6,     1,     4,     5,     1,     1,     3,     5,     1,     3,
       6,     1,     3,     6
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
          case 78: /* NAME  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1876 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 118: /* character_sequence  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1882 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 119: /* string_constant  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1888 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 120: /* string_builder_body  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1894 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 121: /* string_builder  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1900 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 129: /* expression_else  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1906 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 130: /* expression_if_then_else  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1912 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 131: /* expression_for_loop  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1918 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 132: /* expression_while_loop  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1924 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 133: /* annotation_argument  */
#line 80 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 1930 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 134: /* annotation_argument_list  */
#line 81 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 1936 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 135: /* annotation_declaration  */
#line 82 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 1942 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 136: /* annotation_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1948 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* optional_annotation_list  */
#line 83 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 1954 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* optional_function_argument_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 1960 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* optional_function_type  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 1966 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* function_name  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 1972 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 142: /* expression_block  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1978 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* expression_any  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1984 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* expressions  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1990 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 145: /* expr_pipe  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 1996 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 146: /* name_in_namespace  */
#line 74 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2002 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* expr_new  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2008 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* expression_break  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2014 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* expression_return  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2020 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* expression_try_catch  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2026 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expression_let  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2032 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expr_sizeof  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2038 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* expr_typename  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2044 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* expr_list  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2050 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expr_block  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2056 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expr_numeric_const  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2062 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* expr_assign  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2068 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* expr  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2074 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* struct_variable_declaration_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2080 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* variable_declaration_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2086 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* function_argument_list  */
#line 77 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2092 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* variable_declaration  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2098 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* let_variable_declaration  */
#line 76 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2104 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* variable_name_list  */
#line 75 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2110 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* structure_type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2116 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* auto_type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2122 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* type_declaration  */
#line 78 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2128 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* make_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2134 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* make_struct_fields  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2140 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* make_struct_dim  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2146 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* make_struct_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2152 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* make_dim  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2158 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* make_dim_decl  */
#line 79 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2164 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
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
        case 9:
#line 209 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); }
#line 2452 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 10:
#line 210 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2458 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 11:
#line 214 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2464 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 218 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2472 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 221 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2483 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 227 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2493 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 235 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2511 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 251 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2534 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 20:
#line 281 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( auto mod = Module::require(*(yyvsp[0].s)) ) {
            g_Program->addModule(mod);
        } else {
            yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 24:
#line 302 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2556 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 25:
#line 305 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2564 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 311 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 2570 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 312 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2576 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 313 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2585 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 320 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2594 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 327 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pFor;
    }
#line 2607 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 338 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile();
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWhile;
    }
#line 2618 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 347 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 2624 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 348 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i));  }
#line 2630 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 349 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].d)));  }
#line 2636 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 350 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true);  }
#line 2642 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 351 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false);  }
#line 2648 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 355 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 2658 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 360 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 2668 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 368 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 2680 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 375 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 2694 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 387 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2703 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 391 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 2712 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 398 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 2718 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 399 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 2724 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 403 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2730 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 404 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 2736 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 405 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 2742 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 409 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(yylloc.first_column,yylloc.first_line);
    }
#line 2751 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 413 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 2760 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 420 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 2766 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 421 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 2772 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 422 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 2778 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 423 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 2784 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 424 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 2790 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 425 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 2796 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 426 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 2802 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 427 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 2808 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 428 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 2814 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 429 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 2820 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 430 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 2826 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 431 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 2832 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 432 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 2838 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 433 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 2844 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 434 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 2850 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 435 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 2856 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 436 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 2862 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 437 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 2868 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 438 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 2874 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 439 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 2880 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 2886 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 2892 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 2898 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 443 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 2904 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 444 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 2910 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 2916 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 446 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 2922 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 2928 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 2934 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 453 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3004 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 521 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3010 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 525 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3016 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 526 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3022 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 527 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3028 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 528 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3034 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 529 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3040 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 530 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3046 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 531 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3052 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 532 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3058 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 533 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3064 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 534 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3070 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 538 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBlock(); }
#line 3076 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 539 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3087 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 545 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3095 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 551 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3111 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 565 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3117 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 566 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3128 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 572 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3134 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 576 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pTypeDecl = g_Program->makeTypeDeclaration(tokAt((yylsp[-1])), *(yyvsp[0].s));
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr(pTypeDecl));
        delete (yyvsp[0].s);
    }
#line 3144 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 584 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3150 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3158 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 591 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3166 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 594 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3176 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 602 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3184 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 608 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3215 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 634 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3244 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 661 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3252 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 664 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3260 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 670 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3268 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 673 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3276 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 679 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3284 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 682 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3292 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 688 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3302 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 694 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3354 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 744 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3360 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 745 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3366 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 746 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3372 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 747 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3378 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 748 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].d)); }
#line 3384 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 752 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3390 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 753 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3396 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 754 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3402 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 755 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3408 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 756 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3414 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 757 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3420 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 758 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3426 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 759 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3432 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 760 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3438 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 761 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3444 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 765 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 3450 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 766 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3456 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 767 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3462 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 768 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3468 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 769 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3474 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 770 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 3480 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 3486 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 772 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3492 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 773 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3498 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 774 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3504 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 775 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 3510 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 776 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3516 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 777 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3522 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 778 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3528 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 779 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3534 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 780 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3540 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 781 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3546 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 782 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3552 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3558 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 784 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3564 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 785 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3570 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3576 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 787 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3582 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 788 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3588 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 789 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3594 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 790 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3600 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 791 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3606 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 792 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 3612 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 793 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3618 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 794 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3624 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 795 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3630 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 796 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3636 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 797 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3642 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 798 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3648 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 799 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 3657 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 803 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 3666 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 807 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 3674 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 810 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 3682 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 813 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[0])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 3688 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 814 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3694 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 815 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 3702 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 818 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3708 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 819 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3714 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 820 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3720 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 821 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3726 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 822 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3741 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 832 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3756 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); }
#line 3762 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 846 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl)); }
#line 3768 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 850 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3774 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 851 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3780 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 855 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3786 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 856 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 3792 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 860 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 3798 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 861 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 3804 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 865 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 3813 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 869 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 3821 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 872 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3830 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 876 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3840 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 881 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3850 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 3858 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3867 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3876 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 3886 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 905 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 3896 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3921 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 936 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3966 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 979 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 3977 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 985 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 3987 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 993 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 3993 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 994 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 3999 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 995 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 4005 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 996 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 4011 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 997 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 4017 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 998 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 4023 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 999 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 4029 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 1000 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 4035 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 1001 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 4041 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 1002 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 4047 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 1003 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 4053 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 1004 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 4059 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 1005 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 4065 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 1006 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 4071 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 1007 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 4077 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 1008 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 4083 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 1009 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 4089 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 1010 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 4095 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 1011 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 4101 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 1015 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4113 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 1025 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4122 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 1029 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4133 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 1038 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4139 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 1039 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4145 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 1040 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4151 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1041 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4160 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1045 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4170 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1050 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4180 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1055 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4190 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1060 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4200 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1065 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4209 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1069 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4219 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1074 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4230 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1080 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4239 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1084 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 4249 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1089 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4274 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1112 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4280 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1113 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4286 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1117 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 4298 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1124 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 4309 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1133 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 4319 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1138 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4328 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1145 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4338 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1153 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 4348 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1158 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4357 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1165 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 4367 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 4371 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
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
#line 1172 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


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
        yyerror("too many options for annotation " + name, at, CompilationError::annotation_not_found );
        return nullptr;
    }
}


