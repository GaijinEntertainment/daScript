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

/* Substitute the type names.  */
#define YYSTYPE         DAS_YYSTYPE
#define YYLTYPE         DAS_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das_yyparse
#define yylex           das_yylex
#define yyerror         das_yyerror
#define yydebug         das_yydebug
#define yynerrs         das_yynerrs

#define yylval          das_yylval
#define yychar          das_yychar
#define yylloc          das_yylloc

/* First part of user prologue.  */
#line 36 "src/parser/ds_parser.ypp" /* yacc.c:338  */

	#include "daScript/misc/platform.h"
	#include "daScript/simulate/debug_info.h"
	#include "daScript/ast/compilation_errors.h"

	#ifdef _MSC_VER
	#pragma warning(disable:4262)
	#pragma warning(disable:4127)
	#pragma warning(disable:4702)
	#endif
    
    using namespace das;
    
    void das_yyerror (const string & error, das::CompilationError cerr = das::CompilationError::syntax_error);
    void das_yyerror (const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified);
    int yylex();
    void yybegin(const char * str);

#line 99 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef DAS_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define DAS_YYDEBUG 1
#  else
#   define DAS_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define DAS_YYDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DAS_YYDEBUG */
#if DAS_YYDEBUG
extern int das_yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 174 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */

/* Token type.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
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
    _UPCAST = 294,
    _TBOOL = 295,
    _TVOID = 296,
    _TSTRING = 297,
    _TAUTO = 298,
    _TINT = 299,
    _TINT2 = 300,
    _TINT3 = 301,
    _TINT4 = 302,
    _TUINT = 303,
    _TUINT2 = 304,
    _TUINT3 = 305,
    _TUINT4 = 306,
    _TFLOAT = 307,
    _TFLOAT2 = 308,
    _TFLOAT3 = 309,
    _TFLOAT4 = 310,
    _TRANGE = 311,
    _TURANGE = 312,
    _TBLOCK = 313,
    _TINT64 = 314,
    _TUINT64 = 315,
    _TYPENAME = 316,
    _TDOUBLE = 317,
    _TFUNCTION = 318,
    _TLAMBDA = 319,
    ADDEQU = 320,
    SUBEQU = 321,
    DIVEQU = 322,
    MULEQU = 323,
    MODEQU = 324,
    ANDEQU = 325,
    OREQU = 326,
    XOREQU = 327,
    SHL = 328,
    SHR = 329,
    ADDADD = 330,
    SUBSUB = 331,
    LEEQU = 332,
    SHLEQU = 333,
    SHREQU = 334,
    GREQU = 335,
    EQUEQU = 336,
    NOTEQU = 337,
    RARROW = 338,
    LARROW = 339,
    QQ = 340,
    QDOT = 341,
    LPIPE = 342,
    LBPIPE = 343,
    RPIPE = 344,
    INTEGER = 345,
    LONG_INTEGER = 346,
    UNSIGNED_INTEGER = 347,
    UNSIGNED_LONG_INTEGER = 348,
    FLOAT = 349,
    DOUBLE = 350,
    NAME = 351,
    BEGIN_STRING = 352,
    STRING_CHARACTER = 353,
    END_STRING = 354,
    BEGIN_STRING_EXPR = 355,
    END_STRING_EXPR = 356,
    UNARY_MINUS = 357,
    UNARY_PLUS = 358,
    PRE_INC = 359,
    PRE_DEC = 360,
    POST_INC = 361,
    POST_DEC = 362,
    COLCOL = 363
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 59 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 320 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */
};

typedef union DAS_YYSTYPE DAS_YYSTYPE;
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


extern DAS_YYSTYPE das_yylval;
extern DAS_YYLTYPE das_yylloc;
int das_yyparse (void);

#endif /* !YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   3357

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  135
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  297
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  524

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   363

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,     2,     2,   134,   115,   108,     2,
     128,   129,   113,   112,   102,   111,   125,   114,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   133,
     109,   103,   110,   104,   116,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,   107,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   131,   106,   132,   119,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101,   117,   118,   121,
     122,   123,   124,   130
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   226,   226,   228,   229,   230,   231,   232,   233,   234,
     235,   239,   240,   244,   248,   251,   257,   265,   281,   302,
     306,   307,   311,   321,   325,   326,   330,   333,   339,   340,
     341,   348,   355,   367,   377,   386,   387,   388,   389,   390,
     391,   395,   400,   408,   409,   413,   420,   432,   436,   443,
     444,   448,   449,   450,   454,   458,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   501,
     570,   573,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   598,   602,   608,   614,   628,   629,
     635,   639,   645,   648,   651,   655,   661,   665,   668,   671,
     679,   685,   686,   690,   716,   744,   747,   753,   756,   762,
     765,   771,   774,   780,   785,   836,   837,   838,   839,   840,
     841,   845,   846,   847,   848,   849,   850,   851,   852,   853,
     854,   855,   856,   861,   866,   876,   877,   878,   879,   880,
     881,   882,   883,   884,   885,   886,   887,   888,   889,   890,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   906,   907,   908,   909,   910,
     911,   912,   913,   917,   921,   924,   927,   928,   929,   930,
     931,   934,   935,   936,   937,   938,   939,   940,   950,   960,
     994,   995,   999,  1006,  1009,  1016,  1017,  1021,  1022,  1026,
    1027,  1031,  1035,  1038,  1042,  1047,  1055,  1058,  1062,  1066,
    1071,  1079,  1102,  1105,  1113,  1124,  1135,  1148,  1149,  1153,
    1190,  1247,  1253,  1261,  1262,  1263,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,
    1278,  1279,  1280,  1284,  1294,  1298,  1307,  1308,  1309,  1310,
    1314,  1319,  1324,  1329,  1334,  1341,  1345,  1350,  1356,  1360,
    1365,  1374,  1378,  1383,  1392,  1396,  1401,  1413,  1414,  1418,
    1425,  1434,  1439,  1446,  1451,  1460,  1465,  1472
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEXER_ERROR", "_STRUCT", "_LET", "_DEF",
  "_WHILE", "_IF", "_ELSE", "_FOR", "_CATCH", "_TRUE", "_FALSE", "_SIZEOF",
  "_NEWT", "_TYPE", "_IN", "_ELIF", "_ARRAY", "_RETURN", "_NULL", "_BREAK",
  "_TRY", "_OPTIONS", "_TABLE", "_EXPECT", "_CONST", "_REQUIRE",
  "_OPERATOR", "_ENUM", "_FINALLY", "_DELETE", "_DEREF", "_SCOPE",
  "_TYPEDEF", "_WITH", "_CAST", "_OVERRIDE", "_UPCAST", "_TBOOL", "_TVOID",
  "_TSTRING", "_TAUTO", "_TINT", "_TINT2", "_TINT3", "_TINT4", "_TUINT",
  "_TUINT2", "_TUINT3", "_TUINT4", "_TFLOAT", "_TFLOAT2", "_TFLOAT3",
  "_TFLOAT4", "_TRANGE", "_TURANGE", "_TBLOCK", "_TINT64", "_TUINT64",
  "_TYPENAME", "_TDOUBLE", "_TFUNCTION", "_TLAMBDA", "ADDEQU", "SUBEQU",
  "DIVEQU", "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "SHL", "SHR",
  "ADDADD", "SUBSUB", "LEEQU", "SHLEQU", "SHREQU", "GREQU", "EQUEQU",
  "NOTEQU", "RARROW", "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE",
  "INTEGER", "LONG_INTEGER", "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER",
  "FLOAT", "DOUBLE", "NAME", "BEGIN_STRING", "STRING_CHARACTER",
  "END_STRING", "BEGIN_STRING_EXPR", "END_STRING_EXPR", "','", "'='",
  "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'",
  "'/'", "'%'", "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('",
  "')'", "COLCOL", "'{'", "'}'", "';'", "'$'", "$accept", "program",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_list", "require_module", "expect_declaration", "expect_list",
  "expect_error", "expression_else", "expression_if_then_else",
  "expression_for_loop", "expression_while_loop", "expression_with",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,    64,   357,   358,   126,
      33,   359,   360,   361,   362,    46,    91,    93,    40,    41,
     363,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -313

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-313)))

#define YYTABLE_NINF -142

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -313,    13,  -313,   -72,   -25,    -5,    -1,    11,    24,   -20,
    -313,  -313,  -313,   168,  -313,  -313,  -313,  -313,  -313,  -313,
      39,  -313,    80,    52,    85,  -313,  -313,    93,  -313,    49,
      94,  -313,    79,   121,    91,  -313,   -66,  -313,   130,   126,
     -17,   140,   -25,   128,    -5,    -1,  -313,  2139,   132,  -313,
     -25,   -20,  -313,   133,   100,  2770,   204,   206,  -313,   114,
    -313,  -313,   147,   111,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,   -67,   139,   142,  -313,  -313,
    -313,   118,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,  -313,   143,  -313,  -313,  -313,
     144,   151,  -313,  -313,  -313,  -313,   181,  -313,   -32,  -313,
     -61,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,   -43,   159,  -313,  -313,   102,
    -313,    33,   -53,  -313,  2139,  2139,   174,  2025,  2025,  2025,
    -313,    20,  -313,  -313,   182,  -313,  -313,    -6,  -313,    59,
    -313,  2139,   145,  -313,   175,  -313,  2139,  1929,  -313,  -313,
     184,  -313,    90,    73,   146,   159,   137,   159,   157,   159,
     158,   169,  -313,   152,  -313,  -313,   147,   181,  -313,  -313,
    -313,    57,  -313,  -313,   150,  2082,  -313,   153,   173,   177,
     160,   164,  1929,  1929,  -313,  -313,  -313,  -313,  -313,  -313,
     -62,  -313,  1929,  1929,   -61,  1929,  1929,   161,  1929,   167,
    -313,  -313,  -313,   -45,  -313,  -313,  -313,  -313,  -313,  -313,
     207,  -313,  2309,   166,  -313,  -313,  -313,   165,  -313,  2139,
    -313,   187,  -313,   189,  -313,   192,  -313,  -313,  -313,  -313,
     565,  1929,   411,    50,  -313,  1929,  2139,  2139,   945,  1068,
     457,   457,   112,   457,   457,  -313,   457,   457,  2139,  2477,
     114,  -313,  1191,   -38,  1929,  1929,  1929,  1929,  1929,  1929,
    1929,  1929,  1929,  1929,  -313,  -313,  1929,  1929,  1929,  1929,
    1929,  1929,   209,  1929,  1929,   213,  1929,  1929,  1929,  1929,
    1929,  1929,  1929,  1929,  1929,  1929,  1929,  1929,  1929,  1929,
     214,  1929,  1314,  -313,   227,  -313,  -313,  -313,  -313,    -7,
    1929,  1929,   147,  1437,  -313,   145,  1929,  1929,   281,  -313,
    -313,  -313,  -313,  -313,  -313,  -313,   183,   185,   186,  -313,
    -313,   -55,  2371,  2967,  2139,  2534,  1560,  2591,   296,   385,
    2139,  2648,  -313,  2705,  -313,  1929,   217,   822,  -313,   159,
    -313,   -30,  2967,  -313,  2967,  2967,  2967,  2967,  2967,  2967,
    2967,  2967,  3231,  3231,  3187,  2967,  2967,  3187,  3170,  3170,
     194,  2967,   457,  -313,   -34,   -34,  2967,  2873,  3061,  3108,
    3126,  3187,  3187,   603,   603,   695,   695,   695,  -313,  2818,
    -313,    41,  -313,   286,   147,   147,  2418,  2418,     2,  1929,
    2967,   314,  2967,  2418,   195,  -313,  -313,  -313,  -313,   -10,
    -313,  -313,    42,  -313,  1929,  1929,    -2,  -313,  -313,  2920,
     -40,   -59,  1683,  2967,   225,   -52,    -4,   145,  1929,  -313,
    1806,  1929,  -313,  -313,  -313,    81,  -313,  -313,   122,  -313,
      55,  1929,  2967,   145,  -313,  -313,  -313,  -313,  3187,  3187,
    -313,  -313,  1929,   233,   234,   205,   233,   221,  1929,  -313,
    2967,  -313,    64,  3014,   145,   147,  2139,  1929,   145,  1929,
    -313,   -44,  -313,   699,  2967,   -24,    46,   -24,  -313,   225,
    -313,  2967,  -313,  -313,  -313,    30,  -313,  2171,  -313,  2418,
    -313,  -313,   222,  1929,  -313,  1929,  -313,    55,  -313,  2967,
    -313,  2240,  -313,  -313
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,     0,     0,     0,     0,     0,     0,     0,
       9,     8,     7,     0,     6,     5,    10,     4,     3,   213,
       0,    41,    18,    26,    23,    24,    22,    19,    20,     0,
       0,    44,   108,     0,    45,    47,     0,    43,     0,     0,
     210,     0,     0,     0,     0,     0,   232,     0,     0,   110,
       0,     0,    50,   237,     0,     0,     0,     0,    56,    51,
     211,   231,     0,     0,    39,    40,    37,    38,    36,    11,
      35,    42,    27,    25,    21,     0,     0,     0,   243,   259,
     244,   264,   245,   247,   248,   249,   250,   252,   253,   254,
     255,   256,   257,   258,   260,   261,   278,   246,   251,   262,
     281,   284,   263,   266,   268,   267,   235,   109,     0,    48,
       0,   239,   213,    59,    60,    62,    61,    63,    64,    65,
      66,    85,    86,    83,    84,    76,    87,    88,    77,    74,
      75,    79,    80,    78,    72,    73,    68,    67,    69,    70,
      71,    58,    57,    81,    82,     0,    54,   241,   212,   221,
     214,     0,     0,   236,     0,     0,     0,    51,    51,    51,
     270,     0,   275,   272,     0,    46,   238,   210,    52,     0,
     217,     0,     0,   220,     0,   219,     0,     0,    12,    13,
       0,   233,     0,     0,     0,    54,     0,    54,     0,    54,
       0,   271,   273,     0,   240,    53,     0,    55,   104,    89,
     242,   222,   160,   161,     0,     0,   155,     0,     0,     0,
       0,     0,     0,     0,   135,   137,   136,   138,   139,   140,
     156,    14,     0,     0,     0,     0,     0,     0,     0,    49,
     158,   133,   225,     0,   204,   202,   201,   203,   206,   157,
       0,   205,   224,     0,   159,   287,   288,     0,   276,     0,
     265,     0,   279,     0,   282,     0,   285,   274,   269,   218,
       0,     0,     0,   112,   115,     0,     0,     0,     0,     0,
     183,   184,    11,   165,   164,   191,   163,   162,     0,     0,
      51,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,     0,   280,   283,   286,   106,   121,
       0,     0,     0,   117,   116,     0,     0,     0,    90,    92,
     102,    99,    97,    98,   105,    93,     0,     0,     0,   103,
      96,     0,   141,   223,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,    17,     0,    15,     0,   187,    54,
     192,     0,   131,   107,   146,   147,   149,   148,   150,   143,
     144,   145,   167,   168,   178,   151,   152,   179,   176,   177,
       0,   166,   199,   190,   207,   208,   142,     0,   181,   182,
     180,   174,   175,   170,   169,   171,   172,   173,   189,     0,
     194,     0,   277,     0,     0,     0,     0,     0,     0,     0,
     118,     0,   111,     0,     0,    95,   100,   101,    94,     0,
     127,   113,     0,   196,     0,     0,     0,   129,   198,     0,
     156,     0,     0,   295,   291,     0,     0,     0,     0,   193,
       0,     0,   188,   195,   122,     0,   215,   124,     0,    33,
      28,     0,   119,     0,    34,   104,   128,   114,   125,   126,
     130,    16,     0,     0,     0,     0,     0,     0,     0,   134,
     132,   153,     0,   200,     0,     0,     0,     0,     0,     0,
      31,     0,   120,     0,   289,     0,     0,     0,   293,   292,
     297,   296,   154,   123,   216,     0,   230,   141,    29,     0,
      32,    91,     0,     0,   226,     0,   229,    28,   294,   290,
     228,   141,    30,   227
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -313,  -313,    61,  -313,  -313,  -313,  -313,  -313,  -313,   289,
    -313,  -313,   292,  -165,  -313,  -313,  -313,  -313,   312,   306,
    -313,   308,  -313,   131,  -143,  -178,  -313,  -313,  -167,  -313,
    -108,  -174,    -9,  -313,  -313,  -313,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -310,    89,  -313,  -254,  -313,    78,  -313,
    -313,   249,  -313,  -313,  -147,  -141,  -313,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -312,   -46,  -313,  -313,  -131,   178,
    -103,   -93,  -313,  -313,  -313
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   151,    70,   272,   230,    10,    11,    27,    28,
      12,    24,    25,   490,   340,   341,   342,   343,    21,    22,
      34,    35,    36,    13,   146,   172,    59,    14,   231,   344,
     260,   345,   233,   346,   234,   347,   348,   349,   415,   350,
     235,   236,   237,   371,   238,   239,   240,   241,   372,    62,
      63,    40,   455,   169,   472,   148,   457,    15,    75,    16,
      17,   111,    54,    18,   149,   243,   104,   105,   106,   244,
     444,   445,   245,   446,   246
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,   103,   177,   232,   170,   199,   351,   251,    31,   253,
     413,   255,   411,     2,   185,   187,   189,   160,     3,   461,
     418,    60,   161,   182,   183,   160,   186,   188,   190,   152,
     161,   193,    60,   283,  -108,    32,    51,     4,   102,     5,
     197,     6,    37,     7,   173,   201,   432,   191,     8,   302,
     180,   281,   305,   147,   261,   259,  -108,   160,   448,    19,
     173,    52,   161,   175,   488,   153,  -108,   278,    48,    33,
      42,    20,   448,   489,   263,   475,    32,   160,   428,   175,
     181,   476,   161,   282,   160,    23,   168,   198,  -108,   161,
      48,   320,   321,   198,   162,    26,   229,   165,   163,   449,
     160,   166,   162,   458,   174,   161,   163,    29,   103,   103,
      33,   103,   103,   103,   173,    61,   164,   160,   324,   466,
      30,   414,   161,   477,   164,   103,   194,   470,   192,   478,
     103,   178,   179,   175,   162,   358,   359,   369,   163,     9,
     482,   173,    41,   448,   448,   102,   102,   367,   102,   102,
     102,   491,    64,    65,   162,    55,   164,    43,   163,   103,
     175,   162,   102,   514,   160,   163,   448,   102,   421,   161,
     453,   467,    38,   512,    39,   249,   164,   162,   356,   476,
      46,   163,    42,   164,   160,   160,   173,    44,   195,   161,
     161,   447,   196,   502,   162,    45,   102,    47,   163,   164,
     248,    56,    57,   103,   174,   175,   173,   176,   160,    48,
     484,   364,   365,   161,   485,   275,   164,    49,    72,    50,
     103,   103,    58,   429,   174,   175,    53,   486,   107,   436,
      66,   112,   103,   143,    67,   144,    68,    69,   110,   351,
     102,   162,   145,   147,   150,   163,   156,   252,   154,   459,
     460,   155,   157,   158,   160,   242,   464,   102,   102,   161,
     159,   162,   162,   164,   171,   163,   163,   254,   256,   102,
     184,   200,   193,   456,   247,   250,   198,   257,   262,   258,
     479,   265,   266,   164,   164,   162,   267,   278,   268,   163,
     270,   271,   269,     9,   322,   283,   492,   325,   323,   326,
     273,   274,   327,   276,   277,   390,   279,   164,   103,   393,
     408,   487,   424,   506,   103,   178,   425,   503,   426,   427,
     454,   508,   450,   160,   510,   463,   465,   474,   161,   495,
     497,   162,   498,   366,    74,   163,    73,   412,   352,   353,
     355,   520,   517,   357,   504,   102,   361,   363,   500,   518,
     513,   102,   522,   164,    71,   505,   108,   493,   515,   109,
     280,   167,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   373,   499,   384,   385,   386,   387,   388,   389,
     496,   391,   392,   264,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,     0,   409,
     162,     0,     0,     0,   163,     0,   434,     0,   416,   417,
       0,   420,   160,     0,   422,   423,     0,   161,     0,     0,
       0,     0,   164,   202,   203,   204,   205,   354,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
     103,     0,     0,   439,   207,   443,     0,     0,   208,     0,
     209,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,   210,    99,     0,   211,     0,   102,     0,     0,
       0,     0,     0,     0,     0,     0,   212,   213,     0,   162,
       0,     0,     0,   163,     0,   435,     0,   462,     0,     0,
       0,   214,   215,   216,   217,   218,   219,   220,   221,     0,
       0,   164,   468,   469,     0,     0,     0,     0,     0,     0,
     279,     0,   222,   223,     0,     0,   480,   224,     0,   483,
     225,   226,   294,   295,     0,     0,     0,   227,     0,   228,
     302,    33,   198,   305,   306,   229,   307,     0,     0,     0,
     494,     0,     0,     0,     0,     0,   501,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   328,   509,     0,     0,
     329,   352,   330,   331,     0,   332,     0,   202,   203,   204,
     205,     0,   320,   321,     0,   333,   206,   334,   335,     0,
       0,   519,     0,   521,     0,     0,     0,   336,   207,     0,
       0,   337,   208,     0,   209,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,   210,    99,     0,   211,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,   220,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,   223,   294,   295,
       0,   224,     0,     0,   225,   226,   302,     0,   304,   305,
     306,   227,   307,   228,     0,    33,   198,   338,   339,   229,
     328,     0,     0,     0,   329,     0,   330,   331,     0,   332,
       0,   202,   203,   204,   205,     0,   317,   318,   319,   333,
     206,   334,   335,     0,     0,     0,     0,     0,   320,   321,
       0,   336,   207,     0,     0,   337,   208,     0,   209,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     210,    99,     0,   211,     0,     0,     0,     0,     0,     0,
     294,   295,     0,     0,   212,   213,     0,     0,   302,     0,
     304,   305,   306,     0,   307,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,   220,   221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,   223,     0,     0,     0,   224,     0,     0,   225,   226,
     320,   321,     0,     0,     0,   227,     0,   228,     0,    33,
     198,   511,   339,   229,   202,   203,   204,   205,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,     0,   160,
       0,     0,     0,     0,   161,   207,     0,     0,     0,   208,
       0,   209,    78,    79,    80,     0,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
       0,    97,    98,   210,    99,     0,   211,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,   440,   221,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
     163,     0,     0,   222,   223,     0,     0,     0,   224,     0,
       0,   225,   226,     0,     0,     0,     0,     0,   441,     0,
     442,     0,    33,   198,     0,     0,   229,   202,   203,   204,
     205,   360,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,     0,   208,     0,   209,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,   210,    99,     0,   211,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,   220,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,   223,     0,     0,
       0,   224,     0,     0,   225,   226,     0,     0,     0,     0,
       0,   227,     0,   228,     0,    33,   198,     0,     0,   229,
     202,   203,   204,   205,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,   208,     0,   209,    78,    79,
      80,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,    97,    98,   210,
      99,     0,   211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   214,   215,
     216,   217,   218,   219,   220,   221,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   222,
     223,     0,     0,     0,   224,     0,     0,   225,   226,     0,
       0,     0,     0,     0,   227,     0,   228,   362,    33,   198,
       0,     0,   229,   202,   203,   204,   205,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   207,     0,     0,     0,   208,     0,
     209,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,   210,    99,     0,   211,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,   220,   221,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,   223,     0,     0,     0,   224,     0,     0,
     225,   226,     0,     0,     0,     0,     0,   227,     0,   228,
     370,    33,   198,     0,     0,   229,   202,   203,   204,   205,
       0,     0,     0,     0,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
       0,   208,     0,   209,    78,    79,    80,     0,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    97,    98,   210,    99,     0,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     213,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   214,   215,   216,   217,   218,   219,
     220,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222,   223,     0,     0,     0,
     224,     0,     0,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   228,   410,    33,   198,     0,     0,   229,   202,
     203,   204,   205,     0,     0,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     207,     0,     0,     0,   208,     0,   209,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,   210,    99,
       0,   211,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,     0,     0,     0,     0,     0,     0,
       0,   419,     0,     0,     0,     0,     0,   214,   215,   216,
     217,   218,   219,   220,   221,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,   223,
       0,     0,     0,   224,     0,     0,   225,   226,     0,     0,
       0,     0,     0,   227,     0,   228,     0,    33,   198,     0,
       0,   229,   202,   203,   204,   205,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   207,     0,     0,     0,   208,     0,   209,
      78,    79,    80,     0,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,     0,    97,
      98,   210,    99,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,   220,   221,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   222,   223,     0,     0,     0,   224,     0,     0,   225,
     226,     0,     0,     0,     0,     0,   227,     0,   228,   431,
      33,   198,     0,     0,   229,   202,   203,   204,   205,     0,
       0,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   207,     0,     0,     0,
     208,     0,   209,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,   210,    99,     0,   211,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,   215,   216,   217,   218,   219,   220,
     221,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,   223,     0,     0,     0,   224,
       0,     0,   225,   226,     0,     0,     0,     0,     0,   227,
       0,   228,   473,    33,   198,     0,     0,   229,   202,   203,
     204,   205,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,     0,   208,     0,   209,    78,    79,    80,     0,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,     0,    97,    98,   210,    99,     0,
     211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,   220,   221,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   222,   223,     0,
       0,     0,   224,     0,     0,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   228,   481,    33,   198,     0,     0,
     229,   202,   203,   204,   205,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,     0,   208,     0,   209,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
     210,    99,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,   220,   221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,   223,     0,     0,    76,   224,     0,     0,   225,   226,
      77,     0,     0,     0,     0,   227,     0,   228,     0,    33,
     198,     0,     0,   229,     0,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,    99,   100,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,     0,    99,   100,   101,     0,     0,     0,
       0,     0,     0,   145,     0,    33,     0,     0,    76,     0,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,    99,   100,   101,     0,     0,     0,     0,   227,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,     0,     0,   308,   309,     0,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,     0,
       0,     0,     0,     0,   516,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,     0,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,     0,     0,
       0,     0,     0,   523,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,  -141,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,     0,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   320,   321,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   308,   309,     0,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,     0,     0,     0,
       0,   292,   293,   294,   295,   296,   320,   321,   299,   300,
     301,   302,   303,   304,   305,   306,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   321,     0,     0,     0,     0,   198,
     292,   293,   294,   295,   296,     0,     0,   299,   300,   301,
     302,   303,   304,   305,   306,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,   321,     0,     0,   368,   292,   293,   294,
     295,   296,     0,     0,   299,   300,   301,   302,   303,   304,
     305,   306,     0,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
     321,     0,     0,   430,   292,   293,   294,   295,   296,     0,
       0,   299,   300,   301,   302,   303,   304,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321,     0,     0,
     433,   292,   293,   294,   295,   296,     0,     0,   299,   300,
     301,   302,   303,   304,   305,   306,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   321,     0,     0,   437,   292,   293,
     294,   295,   296,     0,     0,   299,   300,   301,   302,   303,
     304,   305,   306,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,     0,     0,   438,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,     0,     0,     0,   141,
     142,   292,   293,   294,   295,   296,     0,     0,   299,   300,
     301,   302,   303,   304,   305,   306,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   321,   452,   292,   293,   294,   295,
     296,     0,     0,   299,   300,   301,   302,   303,   304,   305,
     306,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   309,   451,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,     0,
       0,     0,     0,   292,   293,   294,   295,   296,   320,   321,
     299,   300,   301,   302,   303,   304,   305,   306,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,   309,     0,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,     0,     0,     0,     0,
     292,   293,   294,   295,   296,   320,   321,   299,   300,   301,
     302,   303,   304,   305,   306,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,     0,     0,     0,     0,   292,   293,   294,
     295,   296,   320,   321,   299,   300,   301,   302,     0,   304,
     305,   306,     0,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
       0,     0,     0,     0,   292,   293,   294,   295,   296,   320,
     321,   299,   300,   301,   302,     0,   304,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
     313,   314,   315,   316,   317,   318,   319,     0,     0,     0,
       0,   292,   293,   294,   295,   296,   320,   321,   299,   300,
     301,   302,     0,   304,   305,   306,     0,   307,     0,   292,
     293,   294,   295,   296,     0,     0,   299,   300,   301,   302,
       0,   304,   305,   306,     0,   307,   312,   313,   314,   315,
     316,   317,   318,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   321,   313,   314,   315,   316,   317,
     318,   319,     0,   292,   293,   294,   295,   296,     0,     0,
     299,   320,   321,   302,     0,   304,   305,   306,     0,   307,
     292,   293,   294,   295,     0,     0,     0,     0,     0,     0,
     302,     0,   304,   305,   306,     0,   307,     0,     0,   313,
     314,   315,   316,   317,   318,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,   321,     0,   315,   316,
     317,   318,   319,     0,     0,     0,   294,   295,     0,     0,
       0,     0,   320,   321,   302,     0,   304,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   321
};

static const yytype_int16 yycheck[] =
{
       9,    47,   149,   177,   145,   172,   260,   185,    28,   187,
      17,   189,   322,     0,   157,   158,   159,    27,     5,    17,
     332,    38,    32,   154,   155,    27,   157,   158,   159,    96,
      32,    90,    38,    88,    96,    96,   102,    24,    47,    26,
     171,    28,    51,    30,    84,   176,   356,    27,    35,    83,
     103,    96,    86,    96,   201,   196,    96,    27,   102,   131,
      84,   127,    32,   103,     9,   132,   128,   126,   130,   130,
     102,    96,   102,    18,   205,   127,    96,    27,   133,   103,
     133,   133,    32,   128,    27,    90,   129,   131,   128,    32,
     130,   125,   126,   131,   104,    96,   134,   129,   108,   129,
      27,   110,   104,   415,   102,    32,   108,    96,   154,   155,
     130,   157,   158,   159,    84,   132,   126,    27,   249,   129,
      96,   128,    32,   127,   126,   171,   132,   129,   108,   133,
     176,    98,    99,   103,   104,   266,   267,   280,   108,   126,
     450,    84,   103,   102,   102,   154,   155,   278,   157,   158,
     159,   461,    12,    13,   104,    29,   126,   105,   108,   205,
     103,   104,   171,   133,    27,   108,   102,   176,   335,    32,
     129,   129,     4,   127,     6,   102,   126,   104,   128,   133,
     131,   108,   102,   126,    27,    27,    84,   102,   129,    32,
      32,   369,   133,   129,   104,   102,   205,   103,   108,   126,
     110,    75,    76,   249,   102,   103,    84,   105,    27,   130,
     129,    99,   100,    32,   133,   224,   126,    96,    90,   128,
     266,   267,    96,   354,   102,   103,    96,   105,    96,   360,
      90,   131,   278,    29,    94,    29,    96,    97,   105,   493,
     249,   104,   128,    96,   133,   108,   128,   110,   109,   416,
     417,   109,   109,   109,    27,   177,   423,   266,   267,    32,
     109,   104,   104,   126,   105,   108,   108,   110,   110,   278,
      96,    96,    90,   414,    90,   129,   131,   108,   128,   127,
     447,   128,   109,   126,   126,   104,   109,   126,   128,   108,
     212,   213,   128,   126,   128,    88,   463,   110,   133,   110,
     222,   223,   110,   225,   226,    96,   228,   126,   354,    96,
      96,   458,    31,   487,   360,    98,   133,   484,   133,   133,
      34,   488,   128,    27,   491,    11,   131,   102,    32,    96,
      96,   104,   127,   272,    45,   108,    44,   110,   260,   261,
     262,   515,   509,   265,   485,   354,   268,   269,   127,   127,
     497,   360,   517,   126,    42,   486,    50,   465,   505,    51,
     229,   112,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   283,   476,   296,   297,   298,   299,   300,   301,
     473,   303,   304,   205,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,    -1,   321,
     104,    -1,    -1,    -1,   108,    -1,   110,    -1,   330,   331,
      -1,   333,    27,    -1,   336,   337,    -1,    32,    -1,    -1,
      -1,    -1,   126,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     486,    -1,    -1,   365,    33,   367,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    -1,    64,    -1,   486,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,   104,
      -1,    -1,    -1,   108,    -1,   110,    -1,   419,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,   126,   434,   435,    -1,    -1,    -1,    -1,    -1,    -1,
     442,    -1,   111,   112,    -1,    -1,   448,   116,    -1,   451,
     119,   120,    75,    76,    -1,    -1,    -1,   126,    -1,   128,
      83,   130,   131,    86,    87,   134,    89,    -1,    -1,    -1,
     472,    -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   487,     1,   489,    -1,    -1,
       5,   493,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    -1,   125,   126,    -1,    20,    21,    22,    23,    -1,
      -1,   513,    -1,   515,    -1,    -1,    -1,    32,    33,    -1,
      -1,    36,    37,    -1,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    75,    76,
      -1,   116,    -1,    -1,   119,   120,    83,    -1,    85,    86,
      87,   126,    89,   128,    -1,   130,   131,   132,   133,   134,
       1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,   113,   114,   115,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,   125,   126,
      -1,    32,    33,    -1,    -1,    36,    37,    -1,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    75,    76,    -1,    -1,    83,    -1,
      85,    86,    87,    -1,    89,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,   116,    -1,    -1,   119,   120,
     125,   126,    -1,    -1,    -1,   126,    -1,   128,    -1,   130,
     131,   132,   133,   134,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,
      -1,    39,    40,    41,    42,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    62,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,    -1,    -1,    -1,   116,    -1,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,    -1,   130,   131,    -1,    -1,   134,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,   116,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,    -1,   130,   131,    -1,    -1,   134,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      62,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,   116,    -1,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,   131,
      -1,    -1,   134,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,   116,    -1,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
     129,   130,   131,    -1,    -1,   134,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    62,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
     116,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
     126,    -1,   128,   129,   130,   131,    -1,    -1,   134,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,    -1,   130,   131,    -1,
      -1,   134,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    62,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,   116,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,
     130,   131,    -1,    -1,   134,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,   116,
      -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,   131,    -1,    -1,   134,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,   126,    -1,   128,   129,   130,   131,    -1,    -1,
     134,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    19,   116,    -1,    -1,   119,   120,
      25,    -1,    -1,    -1,    -1,   126,    -1,   128,    -1,   130,
     131,    -1,    -1,   134,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,   125,   126,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
      73,    74,    75,    76,    77,    -1,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,    -1,    -1,   129,    73,    74,    75,
      76,    77,    -1,    -1,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,    -1,    -1,   129,    73,    74,    75,    76,    77,    -1,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,
     129,    73,    74,    75,    76,    77,    -1,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,    -1,    -1,   129,    73,    74,
      75,    76,    77,    -1,    -1,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,    -1,    -1,   129,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,   119,
     120,    73,    74,    75,    76,    77,    -1,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,    73,    74,    75,    76,
      77,    -1,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,   125,   126,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,   125,   126,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,   125,   126,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,   125,
     126,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,   125,   126,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    89,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    89,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   109,   110,   111,   112,   113,
     114,   115,    -1,    73,    74,    75,    76,    77,    -1,    -1,
      80,   125,   126,    83,    -1,    85,    86,    87,    -1,    89,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    86,    87,    -1,    89,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,    -1,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    75,    76,    -1,    -1,
      -1,    -1,   125,   126,    83,    -1,    85,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   136,     0,     5,    24,    26,    28,    30,    35,   126,
     141,   142,   145,   158,   162,   192,   194,   195,   198,   131,
      96,   153,   154,    90,   146,   147,    96,   143,   144,    96,
      96,    28,    96,   130,   155,   156,   157,   167,     4,     6,
     186,   103,   102,   105,   102,   102,   131,   103,   130,    96,
     128,   102,   127,    96,   197,    29,    75,    76,    96,   161,
      38,   132,   184,   185,    12,    13,    90,    94,    96,    97,
     138,   153,    90,   147,   144,   193,    19,    25,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    62,
      63,    64,   167,   200,   201,   202,   203,    96,   154,   156,
     105,   196,   131,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   119,   120,    29,    29,   128,   159,    96,   190,   199,
     133,   137,    96,   132,   109,   109,   128,   109,   109,   109,
      27,    32,   104,   108,   126,   129,   167,   186,   129,   188,
     190,   105,   160,    84,   102,   103,   105,   189,    98,    99,
     103,   133,   203,   203,    96,   159,   203,   159,   203,   159,
     203,    27,   108,    90,   132,   129,   133,   203,   131,   163,
      96,   203,    12,    13,    14,    15,    21,    33,    37,    39,
      61,    64,    75,    76,    90,    91,    92,    93,    94,    95,
      96,    97,   111,   112,   116,   119,   120,   126,   128,   134,
     140,   163,   166,   167,   169,   175,   176,   177,   179,   180,
     181,   182,   183,   200,   204,   207,   209,    90,   110,   102,
     129,   160,   110,   160,   110,   160,   110,   108,   127,   190,
     165,   189,   128,   203,   204,   128,   109,   109,   128,   128,
     183,   183,   139,   183,   183,   167,   183,   183,   126,   183,
     158,    96,   128,    88,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    89,   103,   104,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     125,   126,   128,   133,   203,   110,   110,   110,     1,     5,
       7,     8,    10,    20,    22,    23,    32,    36,   132,   133,
     149,   150,   151,   152,   164,   166,   168,   170,   171,   172,
     174,   181,   183,   183,    16,   183,   128,   183,   203,   203,
      16,   183,   129,   183,    99,   100,   137,   203,   129,   159,
     129,   178,   183,   179,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
      96,   183,   183,    96,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,    96,   183,
     129,   178,   110,    17,   128,   173,   183,   183,   199,    84,
     183,   163,   183,   183,    31,   133,   133,   133,   133,   203,
     129,   129,   178,   129,   110,   110,   203,   129,   129,   183,
      96,   126,   128,   183,   205,   206,   208,   160,   102,   129,
     128,   105,   127,   129,    34,   187,   190,   191,   199,   163,
     163,    17,   183,    11,   163,   131,   129,   129,   183,   183,
     129,   101,   189,   129,   102,   127,   133,   127,   133,   163,
     183,   129,   178,   183,   129,   133,   105,   189,     9,    18,
     148,   178,   163,   165,   183,    96,   206,    96,   127,   205,
     127,   183,   129,   163,   190,   203,   166,   183,   163,   183,
     163,   132,   127,   189,   133,   189,   133,   163,   127,   183,
     166,   183,   148,   133
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   135,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   137,   137,   138,   139,   139,   139,   140,   141,   142,
     143,   143,   144,   145,   146,   146,   147,   147,   148,   148,
     148,   149,   150,   151,   152,   153,   153,   153,   153,   153,
     153,   154,   154,   155,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   159,   160,   160,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   162,
     163,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   165,   165,   165,   166,   167,   167,
     167,   168,   169,   169,   169,   169,   170,   171,   171,   171,
     172,   173,   173,   174,   174,   175,   175,   176,   176,   177,
     177,   178,   178,   179,   179,   180,   180,   180,   180,   180,
     180,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   182,   182,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   185,   186,   186,   187,   187,   188,   188,   189,
     189,   190,   190,   190,   190,   190,   191,   191,   191,   191,
     191,   192,   193,   193,   193,   194,   195,   196,   196,   197,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   201,   202,   202,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   204,   204,   205,
     205,   206,   206,   207,   207,   208,   208,   209
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     7,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     0,     2,     2,     3,     1,     3,
       2,     2,     2,     4,     5,     2,     1,     1,     2,     3,
       4,     0,     2,     5,     3,     5,     5,     4,     5,     4,
       5,     1,     3,     1,     5,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     6,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     3,
       3,     2,     3,     4,     3,     4,     4,     3,     4,     3,
       5,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     1,     2,     0,     3,     1,     3,     1,     3,     1,
       1,     1,     3,     5,     3,     3,     4,     6,     5,     4,
       3,     4,     0,     3,     5,     4,     5,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     1,     4,
       2,     3,     2,     3,     4,     2,     4,     6,     1,     4,
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
          case 96: /* NAME  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2218 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* character_sequence  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2224 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* string_constant  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2230 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* string_builder_body  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2236 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 140: /* string_builder  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2242 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* expression_else  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2248 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* expression_if_then_else  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2254 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* expression_for_loop  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2260 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expression_while_loop  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2266 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expression_with  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2272 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* annotation_argument  */
#line 90 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2278 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* annotation_argument_list  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2284 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* annotation_declaration_name  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2290 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* annotation_declaration  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2296 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* annotation_list  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2302 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* optional_annotation_list  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2308 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* optional_function_argument_list  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2314 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* optional_function_type  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2320 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* function_name  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2326 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* expression_block  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2332 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* expression_any  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2338 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* expressions  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2344 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* expr_pipe  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2350 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* name_in_namespace  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2356 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expression_delete  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2362 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expr_new  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2368 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_break  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2374 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_return  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2380 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_try_catch  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2386 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expression_let  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2392 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expr_cast  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2398 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expr_sizeof  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2404 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expr_typename  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2410 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* expr_list  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2416 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* expr_block  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2422 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expr_numeric_const  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2428 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expr_assign  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2434 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expr_method_call  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2440 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expr  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2446 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* structure_variable_declaration  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2452 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* struct_variable_declaration_list  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2458 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* variable_declaration_list  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2464 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* function_argument_list  */
#line 87 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2470 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* variable_declaration  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2476 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* let_variable_declaration  */
#line 86 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2482 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* enum_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2488 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* optional_structure_parent  */
#line 84 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2494 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* variable_name_list  */
#line 85 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2500 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* structure_type_declaration  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2506 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* auto_type_declaration  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2512 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* type_declaration  */
#line 88 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2518 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* make_decl  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2524 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* make_struct_fields  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2530 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* make_struct_dim  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2536 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* make_struct_decl  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2542 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* make_dim  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* make_dim_decl  */
#line 89 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2554 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
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
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL
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
#line 239 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); }
#line 2842 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 240 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2848 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 244 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2854 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 248 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2862 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 251 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2873 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 257 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2883 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 17:
#line 265 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2901 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 18:
#line 281 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		if ( g_Program->options.arguments.size() ) {
			g_Program->options.arguments.insert ( g_Program->options.arguments.begin(),
                (yyvsp[0].aaList)->arguments.begin(), (yyvsp[0].aaList)->arguments.end() );
		} else {
			swap ( g_Program->options, *(yyvsp[0].aaList) );
		}
		auto opt = g_Program->options.find("indenting", tInt);
		extern int das_tab_size, das_def_tab_size;
		if (opt)
		{
			if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
				das_tab_size = das_def_tab_size;
			else
				das_tab_size = opt->iValue ? opt->iValue : das_def_tab_size;//0 is default
		}
		delete (yyvsp[0].aaList);
	}
#line 2924 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 22:
#line 311 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2936 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 330 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2944 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 333 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2952 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 339 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 2958 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 340 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2964 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 341 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2973 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 348 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2982 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 355 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 2996 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 367 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3008 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 377 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3019 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 386 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3025 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 387 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3031 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 388 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3037 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 389 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3043 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 390 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3049 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 391 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3055 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 395 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3065 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 400 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3075 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 408 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3081 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 409 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("require"); }
#line 3087 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 413 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3099 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 420 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3113 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 432 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3122 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 436 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3131 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 443 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 3137 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 444 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3143 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 448 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3149 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3155 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 450 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3161 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 454 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3170 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 458 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3179 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 465 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3185 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 466 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 3191 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 467 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 3197 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 468 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 3203 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 469 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 3209 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 470 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 3215 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 471 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 3221 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 472 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 3227 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 473 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 3233 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 474 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 3239 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 475 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 3245 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 476 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 3251 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 477 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 3257 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 478 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 3263 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 479 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 3269 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 3275 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 481 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 3281 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 482 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 3287 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 483 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 3293 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 484 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 3299 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 485 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 3305 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 486 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 3311 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 487 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 3317 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 488 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 3323 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 489 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 3329 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 490 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 3335 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 491 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 3341 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 492 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 3347 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 493 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 3353 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 494 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<"); }
#line 3359 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 495 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>"); }
#line 3365 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 496 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<="); }
#line 3371 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 497 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>="); }
#line 3377 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 502 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                            das_yyerror("function argument is already declared " + name,pDecl->at,
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
                            das_yyerror("can't apply annotation\n" + err,
                                tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("functions are only allowed function annotations",
                            tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( pFunction->annotations, *(yyvsp[-5].faList) );
            delete (yyvsp[-5].faList);
        }
        if ( pFunction->isGeneric() ) {
            if ( !g_Program->addGeneric(pFunction) ) {
                das_yyerror("generic function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        } else {
            if ( !g_Program->addFunction(pFunction) ) {
                das_yyerror("function is already defined " +
                    pFunction->getMangledName(),tokAt((yylsp[-4])),
                        CompilationError::function_already_declared);
                pFunction.reset();
            }
        }
        delete (yyvsp[-3].s);
    }
#line 3447 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 570 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3455 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 573 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3467 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 583 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3473 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 584 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3479 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 585 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3485 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 586 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3491 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 587 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3497 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3503 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 589 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3509 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 590 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3515 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 591 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3521 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3527 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 593 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3533 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 594 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3539 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 598 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = tokAt((yyloc));
    }
#line 3548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 602 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3559 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 608 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3567 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 614 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 3583 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 628 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3589 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 629 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3600 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 635 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3606 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 639 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3614 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 645 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3622 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 648 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3630 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 651 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3639 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 655 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3647 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 661 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3653 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 665 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3661 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 668 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3669 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 671 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3679 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 679 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3687 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 685 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 3693 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 686 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 3699 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 690 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                        das_yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                            CompilationError::local_variable_already_declared);
                    }
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        (yyval.pExpression) = pLet;
    }
#line 3730 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 716 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                    das_yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 3760 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 744 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 3768 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 747 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)), true);
    }
#line 3776 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 753 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3784 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 756 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSizeOf(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3792 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 762 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 3800 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 765 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTypeName(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
#line 3808 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3816 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 774 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3824 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 780 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3834 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                            das_yyerror("block argument is already declared " + name,
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
                            das_yyerror("can't apply annotation\n" + err, tokAt((yylsp[-3])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function annotations", tokAt((yylsp[-3])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-3].faList) );
            delete (yyvsp[-3].faList);
        }
    }
#line 3886 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 836 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3892 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 837 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3898 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 838 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3904 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 839 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3910 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 840 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3916 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 841 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3922 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3928 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 846 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3934 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 847 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3940 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 848 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3946 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 849 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3952 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 850 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3958 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 851 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3964 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 852 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3970 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 853 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3976 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 854 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3982 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 855 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3988 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 856 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3994 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 861 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4004 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 866 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4016 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 876 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4022 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 877 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4028 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 878 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4034 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 879 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4040 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 880 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4046 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 881 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4052 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 882 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4058 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 883 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4064 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 884 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4070 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 885 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4076 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 886 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4082 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 887 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4088 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 888 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4094 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4100 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4106 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4112 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4118 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4124 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4130 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4136 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4142 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4148 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4154 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4160 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4166 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 901 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4172 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 902 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4178 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 903 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4184 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4190 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 905 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4196 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 906 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4202 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 907 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4208 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4214 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 909 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4220 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 910 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4226 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4232 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 912 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4238 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4247 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4256 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 921 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4264 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4272 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 927 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4278 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 928 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4284 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4290 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 930 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4296 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 931 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4304 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 934 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4310 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 935 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4316 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 936 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4322 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4328 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 938 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4334 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 939 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4340 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( (yyvsp[-2].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-2].pExpression);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 4355 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( (yyvsp[0].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[0].pExpression);
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = (yyvsp[0].pExpression);
        } else {
            das_yyerror("can only pipe into a function call",tokAt((yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 4370 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 960 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		ExprConstEnumeration * resEnum = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        if ( enums.size()!=1 ) {
            if ( enums.size() ) {
                string candidates = g_Program->describeCandidates(enums);
                das_yyerror("enumeraiton not found " + *(yyvsp[-1].s) + "\n" + candidates, tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            } else {
                das_yyerror("enumeraiton not found " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                    CompilationError::enumeration_not_found);
            }
        } else {
            auto pEnum = enums.back();
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
				auto td = make_shared<TypeDecl>(pEnum);
                resEnum = new ExprConstEnumeration(tokAt((yylsp[0])), ff.first, td);
            } else {
                das_yyerror("enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt((yylsp[0])),
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
#line 4406 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 994 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4412 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 995 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4418 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 999 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4427 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 1006 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4435 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 1009 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4444 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 1016 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4450 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 1017 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4456 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 1021 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4462 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 1022 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4468 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 1026 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4474 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 1027 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4480 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 1031 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4489 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 1035 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4497 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1038 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4506 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1042 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4516 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1047 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4526 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1055 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4534 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1058 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4543 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1062 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4552 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1066 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4562 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1071 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4572 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1079 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                        das_yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 4597 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1102 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4605 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1105 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4618 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1113 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4631 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1124 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 4644 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1135 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s); 
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-3].s);
    }
#line 4659 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1148 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = nullptr; }
#line 4665 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1149 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4671 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1153 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
            } else if ( structs.size()==0 ) {
                das_yyerror("parent structure not found"+*(yyvsp[0].s),tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                das_yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt((yylsp[0])),
                    CompilationError::structure_not_found);

            }
            delete (yyvsp[0].s);
        } else {
            pStruct = make_shared<Structure>(*(yyvsp[-1].s));
        }
        if ( pStruct ) {
            if ( !g_Program->addStructure(pStruct) ) {
                das_yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt((yylsp[-1])),
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
#line 4710 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1190 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure)->shared_from_this();
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything, "+name,pDecl->at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name, td, init, pDecl->move_to_init, pDecl->at);
                        }
                    } else {
                        if ( pDecl->override ) {
                                auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                                oldFd->init = init;
                                oldFd->parentType = oldFd->type->isAuto();
                        } else {
                            das_yyerror("structure field is already declared "+name
                                +", use override to replace initial value instead",pDecl->at,
                                    CompilationError::invalid_override);
                        }
                    }
                }
            }
            if ( (yyvsp[-5].faList) ) {
                if ( (yyvsp[-5].faList)->size()!=1 ) {
                    das_yyerror("structures are only allowed one annotation", tokAt((yylsp[-4])),
                        CompilationError::invalid_annotation);
                } else {
                    auto & annotation = (yyvsp[-5].faList)->back()->annotation;
                    auto & args = (yyvsp[-5].faList)->back()->arguments;
                    if ( !annotation->rtti_isStructureAnnotation() ) {
                        das_yyerror("structures are only allowed structure annotation", tokAt((yylsp[-4])),
                            CompilationError::invalid_annotation);
                    } else {
                        if ( !g_Program->addStructureHandle(pStruct,
                            static_pointer_cast<StructureTypeAnnotation>(annotation), args) ) {
                                das_yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-4])),
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
#line 4769 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1247 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4780 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1253 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4790 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1261 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 4796 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 244:
#line 1262 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 4802 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 245:
#line 1263 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 4808 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 246:
#line 1264 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 4814 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 247:
#line 1265 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 4820 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 248:
#line 1266 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 4826 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 249:
#line 1267 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 4832 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 250:
#line 1268 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 4838 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 251:
#line 1269 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 4844 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 252:
#line 1270 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 4850 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 253:
#line 1271 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 4856 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 254:
#line 1272 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 4862 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 255:
#line 1273 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 4868 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 256:
#line 1274 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 4874 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 257:
#line 1275 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 4880 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 258:
#line 1276 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 4886 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 259:
#line 1277 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 4892 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 260:
#line 1278 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 4898 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 261:
#line 1279 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 4904 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 262:
#line 1280 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tDouble; }
#line 4910 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 263:
#line 1284 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4922 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 264:
#line 1294 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4931 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 265:
#line 1298 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4942 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 266:
#line 1307 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4948 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 267:
#line 1308 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4954 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 268:
#line 1309 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4960 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 269:
#line 1310 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 4969 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 270:
#line 1314 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4979 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 271:
#line 1319 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 4989 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 272:
#line 1324 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 4999 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 273:
#line 1329 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5009 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 274:
#line 1334 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5021 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 275:
#line 1341 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5030 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 276:
#line 1345 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5040 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 277:
#line 1350 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5051 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 278:
#line 1356 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5060 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 279:
#line 1360 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5070 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 280:
#line 1365 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5084 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 281:
#line 1374 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5093 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 282:
#line 1378 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5103 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 283:
#line 1383 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5117 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 284:
#line 1392 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5126 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 285:
#line 1396 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5136 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 286:
#line 1401 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5150 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 287:
#line 1413 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5156 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 288:
#line 1414 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5162 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 289:
#line 1418 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5174 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 290:
#line 1425 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5185 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 291:
#line 1434 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5195 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 292:
#line 1439 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5204 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 293:
#line 1446 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5214 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 294:
#line 1451 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5225 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 295:
#line 1460 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5235 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 296:
#line 1465 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5244 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 297:
#line 1472 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5254 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 5258 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
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
#line 1479 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
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
        das_yyerror("annotation " + name + " not found", at, CompilationError::annotation_not_found );
        return nullptr;
    } else {
        string candidates = g_Program->describeCandidates(ann);
        das_yyerror("too many options for annotation " + name + "\n" + candidates, at, CompilationError::annotation_not_found );
        return nullptr;
    }
}

void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list ) {
    for ( auto pDecl : *list ) {
        if ( pDecl->pTypeDecl ) {
            for ( size_t ai=0; ai!= pDecl->pNameList->size(); ++ai ) {
                auto pVarType = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    das_yyerror("can't have default values in type declaration",
                    pDecl->at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
            }
        }
    }
}


