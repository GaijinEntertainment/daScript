/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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
#define YYBISON_VERSION "3.4.1"

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
#line 50 "ds_parser.ypp"

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
	void das_checkName(const string & name, const LineInfo &at);
    int yylex();
    void yybegin(const char * str);

#line 101 "ds_parser.cpp"

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
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#  define DAS_YYDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DAS_YYDEBUG */
#if DAS_YYDEBUG
extern int das_yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "ds_parser.ypp"

	#include "daScript/misc/platform.h"
	#include "daScript/ast/ast.h"
    #include "daScript/ast/ast_generate.h"
    #include "daScript/ast/ast_expressions.h"

    enum {
        CorM_COPY   = 0,
        CorM_MOVE   = (1<<0),
        CorM_CLONE  = (1<<1)
    };
    
    namespace das {
        extern ProgramPtr			g_Program;
        extern FileAccessPtr        g_Access;
        extern vector<FileInfo *>	g_FileAccessStack;
    }
    using namespace das;
    
    struct VariableDeclaration {
        VariableDeclaration ( const LineInfo & a, vector<string> * n, TypeDecl * t, Expression * i )
            : at(a), pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () {
            if ( pNameList ) delete pNameList;
            delete pTypeDecl;
            if ( pInit ) delete pInit;
            if ( annotation ) delete annotation;
        }
        LineInfo                at;
        vector<string>          *pNameList;
        TypeDecl                *pTypeDecl;
        Expression              *pInit;
        bool                    init_via_move = false;
        bool                    init_via_clone = false;
        bool                    override = false;
        AnnotationArgumentList  *annotation = nullptr;
    };
    
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 191 "ds_parser.cpp"

/* Token type.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
  {
    LEXER_ERROR = 258,
    DAS_STRUCT = 259,
    DAS_LET = 260,
    DAS_DEF = 261,
    DAS_WHILE = 262,
    DAS_IF = 263,
    DAS_ELSE = 264,
    DAS_FOR = 265,
    DAS_CATCH = 266,
    DAS_TRUE = 267,
    DAS_FALSE = 268,
    DAS_NEWT = 269,
    DAS_TYPEINFO = 270,
    DAS_TYPE = 271,
    DAS_IN = 272,
    DAS_IS = 273,
    DAS_ELIF = 274,
    DAS_ARRAY = 275,
    DAS_RETURN = 276,
    DAS_NULL = 277,
    DAS_BREAK = 278,
    DAS_TRY = 279,
    DAS_OPTIONS = 280,
    DAS_TABLE = 281,
    DAS_EXPECT = 282,
    DAS_CONST = 283,
    DAS_REQUIRE = 284,
    DAS_OPERATOR = 285,
    DAS_ENUM = 286,
    DAS_FINALLY = 287,
    DAS_DELETE = 288,
    DAS_DEREF = 289,
    DAS_SCOPE = 290,
    DAS_TYPEDEF = 291,
    DAS_WITH = 292,
    DAS_CAST = 293,
    DAS_OVERRIDE = 294,
    DAS_UPCAST = 295,
    DAS_ITERATOR = 296,
    DAS_VAR = 297,
    DAS_ADDR = 298,
    DAS_CONTINUE = 299,
    DAS_WHERE = 300,
    DAS_PASS = 301,
    DAS_REINTERPRET = 302,
    DAS_MODULE = 303,
    DAS_PUBLIC = 304,
    DAS_LABEL = 305,
    DAS_GOTO = 306,
    DAS_IMPLICIT = 307,
    DAS_TBOOL = 308,
    DAS_TVOID = 309,
    DAS_TSTRING = 310,
    DAS_TAUTO = 311,
    DAS_TINT = 312,
    DAS_TINT2 = 313,
    DAS_TINT3 = 314,
    DAS_TINT4 = 315,
    DAS_TUINT = 316,
    DAS_TUINT2 = 317,
    DAS_TUINT3 = 318,
    DAS_TUINT4 = 319,
    DAS_TFLOAT = 320,
    DAS_TFLOAT2 = 321,
    DAS_TFLOAT3 = 322,
    DAS_TFLOAT4 = 323,
    DAS_TRANGE = 324,
    DAS_TURANGE = 325,
    DAS_TBLOCK = 326,
    DAS_TINT64 = 327,
    DAS_TUINT64 = 328,
    DAS_TDOUBLE = 329,
    DAS_TFUNCTION = 330,
    DAS_TLAMBDA = 331,
    DAS_TINT8 = 332,
    DAS_TUINT8 = 333,
    DAS_TINT16 = 334,
    DAS_TUINT16 = 335,
    DAS_TTUPLE = 336,
    ADDEQU = 337,
    SUBEQU = 338,
    DIVEQU = 339,
    MULEQU = 340,
    MODEQU = 341,
    ANDEQU = 342,
    OREQU = 343,
    XOREQU = 344,
    SHL = 345,
    SHR = 346,
    ADDADD = 347,
    SUBSUB = 348,
    LEEQU = 349,
    SHLEQU = 350,
    SHREQU = 351,
    GREQU = 352,
    EQUEQU = 353,
    NOTEQU = 354,
    RARROW = 355,
    LARROW = 356,
    QQ = 357,
    QDOT = 358,
    LPIPE = 359,
    LBPIPE = 360,
    RPIPE = 361,
    CLONEEQU = 362,
    ROTL = 363,
    ROTR = 364,
    ROTLEQU = 365,
    ROTREQU = 366,
    MAPTO = 367,
    BRABRAB = 368,
    BRACBRB = 369,
    CBRCBRB = 370,
    INTEGER = 371,
    LONG_INTEGER = 372,
    UNSIGNED_INTEGER = 373,
    UNSIGNED_LONG_INTEGER = 374,
    FLOAT = 375,
    DOUBLE = 376,
    NAME = 377,
    BEGIN_STRING = 378,
    STRING_CHARACTER = 379,
    END_STRING = 380,
    BEGIN_STRING_EXPR = 381,
    END_STRING_EXPR = 382,
    UNARY_MINUS = 383,
    UNARY_PLUS = 384,
    PRE_INC = 385,
    PRE_DEC = 386,
    POST_INC = 387,
    POST_DEC = 388,
    DEREF = 389,
    COLCOL = 390
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 76 "ds_parser.ypp"

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

#line 363 "ds_parser.cpp"

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

#endif /* !YY_DAS_YY_DS_PARSER_HPP_INCLUDED  */



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


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#define YYLAST   4901

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  94
/* YYNRULES -- Number of rules.  */
#define YYNRULES  368
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  659

#define YYUNDEFTOK  2
#define YYMAXUTOK   390

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   146,     2,   162,   161,   141,   134,     2,
     154,   155,   139,   138,   128,   137,   151,   140,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   131,   158,
     135,   129,   136,   130,   142,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   152,     2,   153,   133,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   159,   132,   160,   145,     2,     2,     2,
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
     125,   126,   127,   143,   144,   147,   148,   149,   150,   156,
     157
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   359,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   373,   385,   386,   390,   394,   398,   407,
     415,   431,   452,   456,   459,   465,   474,   487,   488,   492,
     496,   497,   501,   504,   510,   516,   519,   525,   526,   527,
     534,   541,   553,   563,   572,   573,   574,   575,   576,   577,
     581,   586,   594,   595,   599,   606,   618,   622,   629,   630,
     634,   635,   636,   640,   644,   651,   655,   656,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   694,   764,   767,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   796,   800,   806,   812,   826,   827,   833,   837,   843,
     846,   849,   853,   859,   863,   867,   870,   873,   878,   881,
     889,   895,   896,   900,   901,   905,   938,   941,   946,   954,
     958,   963,   967,   975,   978,   984,   985,   995,  1000,  1052,
    1053,  1054,  1055,  1056,  1057,  1061,  1062,  1063,  1064,  1065,
    1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1080,  1091,  1096,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,  1148,  1152,  1155,  1158,  1159,  1160,  1161,
    1162,  1163,  1164,  1167,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1186,  1196,  1230,  1231,  1235,  1236,  1240,  1248,  1251,
    1258,  1259,  1263,  1264,  1268,  1274,  1275,  1279,  1280,  1284,
    1289,  1292,  1296,  1302,  1311,  1312,  1313,  1317,  1320,  1325,
    1330,  1337,  1347,  1350,  1357,  1386,  1389,  1398,  1410,  1422,
    1436,  1437,  1441,  1480,  1546,  1553,  1562,  1563,  1564,  1565,
    1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,
    1576,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,
    1589,  1600,  1604,  1614,  1615,  1616,  1617,  1631,  1637,  1641,
    1646,  1651,  1656,  1663,  1668,  1672,  1676,  1681,  1686,  1693,
    1698,  1704,  1709,  1713,  1718,  1727,  1731,  1736,  1745,  1749,
    1754,  1763,  1772,  1773,  1774,  1775,  1779,  1786,  1795,  1800,
    1807,  1812,  1818,  1825,  1836,  1839,  1853,  1862,  1863,  1867,
    1872,  1879,  1884,  1895,  1900,  1907,  1919,  1920,  1924
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"let\"", "\"def\"", "\"while\"", "\"if\"", "\"else\"", "\"for\"",
  "\"catch\"", "\"true\"", "\"false\"", "\"new\"", "\"typeinfo\"",
  "\"type\"", "\"in\"", "\"is\"", "\"elif\"", "\"array\"", "\"return\"",
  "\"null\"", "\"break\"", "\"try\"", "\"options\"", "\"table\"",
  "\"expect\"", "\"const\"", "\"require\"", "\"operator\"", "\"enum\"",
  "\"finally\"", "\"delete\"", "\"deref\"", "\"scope\"", "\"typedef\"",
  "\"with\"", "\"cast\"", "\"override\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"block\"", "\"int64\"",
  "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"",
  "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"", "\"+=\"", "\"-=\"",
  "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"",
  "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"",
  "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"<|\"",
  "\" <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "'.'", "'['", "']'", "'('", "')'", "DEREF", "COLCOL", "';'", "'{'",
  "'}'", "'$'", "'#'", "$accept", "program", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "expression_else", "expression_if_then_else",
  "expression_for_loop", "expression_while_loop", "expression_with",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_try_catch",
  "let_scope", "kwd_let", "expression_let", "expr_cast", "expr_type_info",
  "expr_list", "block_or_simple_block", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_named_call", "expr_method_call", "expr",
  "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "copy_or_move", "variable_declaration",
  "copy_or_move_or_clone", "let_variable_declaration",
  "global_variable_declaration_list", "global_let", "enum_list",
  "alias_declaration", "enum_declaration", "optional_structure_parent",
  "structure_name", "structure_declaration", "variable_name_list",
  "basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "make_decl",
  "make_struct_fields", "make_struct_dim", "make_struct_decl",
  "make_tuple", "make_map_tuple", "make_any_tuple", "make_dim",
  "make_dim_decl", "make_table", "make_table_decl",
  "array_comprehension_where", "array_comprehension", YY_NULLPTR
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
     375,   376,   377,   378,   379,   380,   381,   382,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,    64,   383,   384,   126,    33,   385,   386,   387,
     388,    46,    91,    93,    40,    41,   389,   390,    59,   123,
     125,    36,    35
};
# endif

#define YYPACT_NINF -439

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-439)))

#define YYTABLE_NINF -166

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -439,    23,  -439,  -439,   -80,   -60,   -59,   -40,   -36,  -439,
     -27,    -9,  -439,  -439,  -439,  -439,  -439,   205,  -439,   -51,
    -439,  -439,  -439,  -439,   -30,  -439,     1,    51,    15,  -439,
    -439,     4,  -439,   -42,    56,    55,  -439,    69,   105,    77,
    -439,   -49,  -439,   118,    -3,  -439,    12,   -80,   126,   -60,
    -439,   123,   124,  -439,  -439,  3483,  -439,   125,  -439,   -80,
      -9,  -439,   117,    92,  4755,   223,   225,  -439,   103,   -90,
    -439,  -439,  -439,  -439,  -439,   134,  -439,  -439,  -439,  -439,
    -439,  -439,   -88,   128,   131,   132,  -439,  -439,  -439,   106,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,   133,  -439,  -439,  -439,   135,   136,
    -439,  -439,  -439,  -439,   137,  -439,  -439,  -439,  -439,  4674,
    -439,   -67,  -439,   -89,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,   -24,   138,  -439,  -439,  -439,   -26,  -439,    72,
     -78,  -439,  3483,  3483,  3483,   151,  3413,  3413,  3413,  3483,
    -439,   -16,  -439,  -439,  -439,  -439,  1644,  -439,  -439,  -439,
    -113,   153,  -439,  -439,    -8,  -439,   -37,  3483,   100,  -439,
    -439,   155,  -439,  3483,  2684,  -439,  -439,  2684,  -439,  1006,
    3894,  4035,   127,   138,  4534,   138,  4535,   138,  4575,  -439,
     -96,  4674,   144,  -439,   130,  -439,  -439,  -439,  3343,   145,
    -439,   150,   146,   149,   158,   152,   159,  2684,  2684,   617,
    3483,  2684,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  2684,
    2684,  2684,   -89,  2684,  2684,  -439,  2684,  -439,   139,  -439,
    -439,   -77,  -439,  -439,  -439,  -439,  -439,  -439,  -439,  3881,
     168,  -439,  -439,  -439,  -439,  -439,   -80,  -439,   241,   166,
    -439,  -439,   -21,  -439,  -439,  3483,  2684,  4674,  -439,  -439,
     505,  -439,   183,  2834,  3158,  -439,  3483,  -439,  -439,   154,
    -439,   156,  -439,   162,  -439,  -439,  3483,  -439,  -439,  4586,
    -439,   167,  2684,  3483,  3483,  2684,  3483,  1780,   720,   720,
     153,  1033,  1169,  4172,  -439,    52,    81,   720,   720,  -439,
    -439,   720,   720,  3481,   722,   103,  -439,  1282,  3483,  2684,
    2684,  -439,  -439,  2684,  2684,  2684,  2684,   206,  2684,   207,
    2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,
    2684,  2684,  2684,  2684,  2684,   210,  2684,  -439,  1893,   -14,
    -439,   153,  -439,  -439,     8,  -439,  4022,  -439,  2684,    57,
    2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,  2684,
    2684,  2684,  2684,  2684,  2684,  -439,  -439,  4589,  -439,  -439,
    -439,  -439,  2006,  1395,  3551,  4598,  4627,  3617,  4628,  -439,
    3683,     2,   -72,  2119,  4172,   208,    11,   209,  -439,  -439,
      20,  2232,    64,    65,  2684,  2684,   173,  -439,  2684,   211,
    -439,  -439,  2684,  2684,   153,  1531,  -439,   100,  2684,  2684,
    -439,   180,   224,  2345,  -439,   307,   187,   190,  -439,  -439,
    -439,  -439,  -439,  -439,   201,   202,   203,  -439,  -439,   324,
    -439,   -75,  4116,   138,   240,  -439,   -55,  4364,    41,   442,
     442,   283,   283,  4507,  4507,   213,   720,  -439,   -45,   -45,
     442,   442,  4240,  4420,   217,  4441,   283,   283,    98,    98,
     497,   497,   497,  -439,  3945,  -439,   -44,  -439,  -439,  2684,
    -439,  2921,  -439,  4364,  4364,  4364,  4364,  4364,  4364,  4364,
    4364,  4364,  4364,  4364,  4364,  4364,  4364,  4364,  -439,  -439,
     -32,  3483,   242,  3749,  -439,  2684,  2684,  -439,  2684,  -439,
    2684,  2684,   240,   243,   218,   240,  2684,   219,  2684,   240,
     228,   229,  4364,  -439,  -439,  4296,  2756,  2756,     5,  2684,
    -439,  -439,  3008,   352,  4364,  2756,  -439,   239,   261,  4364,
     231,  -439,  -439,  -439,  -439,  -439,   343,   153,  -439,   -81,
     -63,   -13,  2684,  -439,  2458,  2684,  -439,  -439,  4364,  -439,
    -439,  4665,   248,  -439,   283,   283,   283,   -91,  4364,    31,
     -63,  -439,   208,  4364,  -439,  -439,    74,  -439,  -439,  -439,
    -439,   140,  2684,  -439,  3095,  -439,   100,  -439,  -439,  -439,
    -439,  -439,  -439,  2684,  -439,  -439,   233,  4364,  -439,    44,
    4364,  -439,  2571,  2684,   244,  2684,   245,   100,  2684,  -439,
     -82,  -439,  -439,   883,  4364,  -439,  -439,  3483,  3815,  3221,
    -439,  4364,  -439,  -439,  2756,   100,  -439,  4666,  -439,   348,
     246,   140,  -439,  -439,  2684,   247,  -439,  4364,  -439
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    58,     1,   143,     0,     0,     0,     0,     0,   144,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,     0,    50,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    53,   125,     0,    54,
      56,     0,    52,     0,     0,   272,     0,     0,     0,     0,
      28,     0,     0,    26,   275,     0,    13,     0,   127,     0,
       0,    59,   280,     0,     0,     0,     0,    65,    60,     0,
      48,    49,    46,    47,    45,     0,    44,    51,    33,    31,
      25,    24,     0,     0,     0,     0,   286,   306,   287,   311,
     288,   292,   293,   294,   295,   299,   300,   301,   302,   303,
     304,   305,   307,   308,   332,   291,   298,   309,   335,   338,
     289,   296,   290,   297,     0,   310,   313,   315,   314,   278,
     126,     0,    57,     0,   282,   248,    68,    69,    71,    70,
      72,    73,    74,    75,    94,    95,    92,    93,    85,    96,
      97,    86,    83,    84,    98,    99,   100,   101,    88,    89,
      87,    81,    82,    77,    76,    78,    79,    80,    67,    66,
      90,    91,     0,    63,   284,   274,   273,     0,    14,     0,
       0,   279,     0,     0,     0,     0,    60,    60,    60,     0,
     319,     0,   325,   328,   327,   321,     0,   324,    55,   281,
     243,     0,    61,   252,     0,   250,   259,     0,     0,   265,
     266,     0,   264,     0,     0,    15,    16,     0,   276,     0,
       0,     0,     0,    63,     0,    63,     0,    63,     0,   255,
       0,   254,   320,   323,     0,   326,   189,   190,     0,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   159,   161,   160,   162,   163,   164,    17,     0,
       0,     0,     0,     0,     0,   317,     0,   121,    58,   187,
     157,   185,   236,   235,   234,   239,   186,   238,   237,     0,
       0,   188,   342,   343,   344,   345,     0,   283,   245,     0,
     251,    62,     0,   258,   257,     0,     0,    64,   102,   285,
       0,   271,     0,   165,     0,   329,     0,   331,   312,     0,
     333,     0,   336,     0,   339,   341,     0,   322,   318,   129,
     132,     0,     0,     0,     0,     0,     0,     0,   213,   214,
       0,     0,     0,     0,   363,     0,     0,   194,   193,   226,
     221,   192,   191,     0,     0,    60,   242,     0,     0,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
     246,     0,   249,   253,   260,   263,   262,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   277,     0,   334,   337,
     340,   256,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   125,     0,   354,   348,     0,   357,   358,   359,
       0,     0,     0,     0,     0,     0,     0,    20,     0,    18,
     217,   123,     0,     0,     0,     0,   133,     0,     0,     0,
     134,     0,     0,     0,   105,   103,     0,     0,   116,   112,
     110,   111,   122,   106,     0,     0,     0,   115,   117,   141,
     109,     0,   165,    63,     0,   222,     0,   153,   233,   195,
     196,   208,   209,   206,   207,     0,   231,   220,   240,   241,
     197,   198,     0,   211,   212,   210,   204,   205,   200,   199,
     201,   202,   203,   219,     0,   224,     0,   244,   247,     0,
     269,   165,   124,   172,   173,   175,   174,   176,   169,   170,
     171,   177,   178,   167,   168,   179,   180,   166,   330,   130,
       0,     0,     0,     0,   227,     0,     0,   228,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   364,   365,     0,     0,     0,     0,     0,
     135,   138,   165,     0,   128,     0,   120,     0,     0,    36,
       0,   118,   119,   108,   113,   114,     0,     0,   107,     0,
       0,     0,     0,   223,     0,     0,   218,   225,   261,   268,
     131,     0,     0,   149,   146,   147,   148,     0,   346,     0,
       0,   350,   349,   355,   361,   360,     0,   352,   362,    19,
      42,    37,     0,   139,   165,   136,     0,    43,    34,    35,
     121,   142,   145,     0,   155,   158,     0,   154,   182,     0,
     232,   151,     0,     0,     0,     0,     0,     0,     0,    40,
       0,   137,   140,     0,   156,   181,   183,     0,     0,   366,
     351,   347,   353,    38,     0,     0,   104,     0,   150,     0,
       0,    37,    41,   152,     0,     0,    39,   367,   368
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -439,  -439,  -439,    68,  -439,  -439,  -439,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,   346,  -439,  -439,  -250,  -439,  -439,
    -439,  -439,   355,   -46,  -439,   344,  -439,   148,  -161,  -207,
    -439,  -439,   -57,  -439,  -203,  -199,   -11,  -439,  -439,  -439,
    -439,  -439,  -439,  -439,   402,  -439,  -439,  -439,  -359,  -439,
      30,  -439,  -327,  -439,  -439,    93,  -439,  -439,  -439,  -439,
     129,  -439,   107,  -439,  -194,  -177,   122,  -151,  -439,  -439,
    -439,  -439,  -439,  -439,  -439,  -439,   -68,   -52,  -439,  -439,
     -20,   189,  -438,  -318,  -439,  -439,  -230,  -120,    97,  -439,
    -439,  -439,  -439,  -439
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,    76,   326,   259,    14,    15,    31,
      32,    53,    16,    28,    29,   446,   447,   629,   448,   449,
     450,   451,    25,    26,    39,    40,    41,    17,   163,   198,
      68,    18,   260,   452,   334,   453,   261,   454,   262,   455,
     456,   457,   458,   567,   459,   460,   263,   264,   466,   615,
     265,   266,   292,   267,   268,   467,   278,   371,   279,   190,
     193,   194,   219,   220,   531,   195,   204,   166,    69,    20,
      82,    21,    22,   124,    63,    23,   196,   270,   117,   118,
     221,   271,   415,   416,   272,   417,   418,   419,   420,   273,
     325,   274,   650,   275
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   167,   286,   116,   422,   291,   299,   461,   301,   496,
     303,   324,   222,   121,   280,   213,   215,   217,   191,   530,
      36,   191,   602,     2,    70,    71,   571,    64,     3,   283,
     379,   613,   164,    37,   170,   119,   180,   572,   283,   276,
     305,   181,    24,   520,   115,   336,   572,   277,     4,    42,
       5,   207,     6,    50,     7,   347,    27,   284,   349,     8,
     182,    47,   306,    30,   283,     9,   284,   623,    38,   180,
     165,    10,   171,   572,   181,   199,   645,   337,   257,    60,
     208,   200,    33,   568,   572,    57,    34,   375,   188,    65,
      66,   201,   284,   182,   285,    35,   572,   592,   164,    46,
     573,   164,   201,   202,    61,   203,   365,   366,    45,   283,
     183,   577,   189,    37,    47,   533,   338,    54,   223,    67,
     116,   116,   116,   580,   116,   116,   116,   116,    72,    47,
     201,   192,    73,   201,    74,    75,   224,   284,   184,   497,
     616,   288,   185,    49,    51,   116,   225,   281,    38,   627,
     282,   116,   209,   210,   211,    52,   214,   216,   218,   628,
     186,   115,   115,   115,   534,   115,   115,   115,   115,   535,
     187,   587,   572,   537,   463,    11,   116,   287,   538,   500,
     499,    12,    48,   290,   624,    55,   115,   116,   116,   535,
     341,   342,   115,   186,   498,   543,   205,   206,   347,   636,
     348,   349,   350,   187,   351,   168,   427,   428,   309,    43,
     425,    44,   426,    56,   589,   619,   257,   115,   258,   321,
     322,   596,   535,   538,   540,   541,    57,    58,   115,   115,
     369,    59,   535,   116,   626,   338,   551,   362,   363,   364,
      62,   330,    78,   630,   116,    80,    81,   120,   123,   365,
     366,   125,   411,   160,   116,   161,   569,   162,   168,   257,
     175,   116,   116,   172,   116,   374,   173,   174,   176,   197,
     177,   178,   179,   212,   115,   164,   397,   289,   307,   269,
     370,   313,   298,   308,   314,   115,   116,   316,   379,   403,
     398,    11,   399,   405,   406,   115,   408,   293,   400,   311,
     294,   338,   115,   115,   312,   115,   461,   339,   340,   341,
     342,   343,   315,   317,   344,   345,   346,   347,   468,   348,
     349,   350,   368,   351,   372,   352,   353,   115,   475,   477,
     318,   319,   493,   544,   323,   205,   533,   536,   556,   560,
     557,   566,   327,   328,   329,   561,   331,   332,   562,   333,
     603,   357,   358,   359,   360,   361,   362,   363,   364,   563,
     564,   565,   570,   606,   582,   590,   548,   574,   365,   366,
     608,   591,   594,   339,   340,   341,   342,   609,   611,   376,
     553,   597,   598,   347,   622,   348,   349,   350,   635,   351,
     610,   352,   353,   654,   429,    79,   625,   640,   642,   655,
     658,   656,    77,    19,   122,   404,   335,   633,   407,   502,
     410,   373,   378,   401,   414,   414,   612,   310,   595,   423,
     360,   361,   362,   363,   364,     0,     0,   462,     0,     0,
       0,     0,   469,   470,   365,   366,   471,   472,   473,   474,
       0,   476,     0,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,     0,   494,
     338,     0,     0,     0,     0,     0,     0,     0,     0,   116,
       0,   501,     0,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,     0,   600,
     601,     0,     0,     0,     0,     0,   523,     0,   607,   167,
       0,   581,     0,     0,     0,     0,   333,     0,     0,     0,
     115,     0,   614,     0,   333,   338,     0,   542,   323,     0,
       0,   545,     0,     0,     0,   546,   547,     0,   552,     0,
       0,   554,   555,   180,   341,   342,   559,     0,   181,     0,
       0,     0,   347,     0,   348,   349,   350,     0,   351,   632,
       0,     0,     0,     0,     0,     0,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     643,     0,     0,     0,     0,     0,     0,     0,     0,   360,
     361,   362,   363,   364,     0,   116,     0,   651,   652,   341,
     342,     0,   578,   365,   366,     0,     0,   347,     0,   348,
     349,   350,     0,   351,     0,     0,   199,   183,     0,     0,
       0,     0,   200,     0,     0,     0,     0,   647,   584,   585,
       0,   586,     0,     0,   588,     0,   115,   320,     0,   593,
       0,   414,     0,     0,   202,   184,     0,    83,     0,   185,
       0,     0,   604,    84,     0,     0,     0,     0,   365,   366,
       0,     0,     0,     0,     0,     0,     0,   186,    85,     0,
       0,     0,     0,   377,     0,   617,     0,   187,   620,     0,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,     0,     0,     0,     0,     0,   634,     0,     0,     0,
       0,     0,     0,     0,     0,   638,   639,     0,   641,     0,
       0,   644,     0,   431,     0,     0,   462,     3,     0,   432,
     433,     0,   434,     0,   226,   227,   228,   229,   338,    37,
       0,     0,     0,   435,   230,   436,   437,   657,     0,     0,
       0,     0,     0,     0,     0,   438,   231,     0,     0,   439,
     232,     0,   233,     0,     9,   234,   440,     0,   441,   235,
       0,     0,   442,   443,    38,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   236,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,   342,   237,   238,     0,     0,     0,     0,
     347,     0,     0,   349,   350,     0,   351,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   240,   241,   242,   243,
     244,   245,   246,   247,    37,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,   251,     0,     0,   252,     0,     0,   253,   254,     0,
       0,   365,   366,     0,     0,     0,   256,     0,     0,    38,
     444,   257,   445,   258,   431,     0,     0,     0,     3,     0,
     432,   433,     0,   434,     0,   226,   227,   228,   229,     0,
       0,     0,     0,     0,   435,   230,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   231,     0,     0,
     439,   232,     0,   233,     0,     9,   234,   440,     0,   441,
     235,     0,     0,   442,   443,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   236,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   240,   241,   242,
     243,   244,   245,   246,   247,    37,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,     0,     0,   252,     0,     0,   253,   254,
       0,     0,     0,     0,   180,     0,     0,   256,     0,   181,
      38,   444,   257,   646,   258,   226,   227,   228,   229,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   182,     0,
       0,   180,     0,     0,     0,     0,   181,   231,     0,     0,
       0,   232,     0,   233,     0,     0,   234,     0,     0,     0,
     235,     0,     0,     0,     0,   182,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,   183,   236,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   184,     0,     0,     0,
     185,     0,   295,     0,     0,     0,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   412,   248,     0,   186,     0,
       0,     0,     0,   184,     0,     0,     0,   185,   187,     0,
     249,   250,   251,     0,     0,   252,     0,     0,   253,   254,
       0,   226,   227,   228,   229,   186,     0,   413,     0,     0,
      38,   230,   257,     0,   258,   187,     0,   180,     0,     0,
       0,     0,   181,   231,     0,     0,     0,   232,     0,   233,
       0,     0,   234,     0,     0,     0,   235,     0,     0,     0,
       0,   182,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   236,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,   238,     0,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   412,   248,     0,   226,   227,   228,   229,     0,   184,
       0,     0,     0,   185,   230,     0,   249,   250,   251,     0,
       0,   252,     0,     0,   253,   254,   231,     0,     0,     0,
     232,   186,   233,   421,     0,   234,    38,     0,   257,   235,
     258,   187,     0,     0,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   236,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   240,   241,   242,   243,
     244,   245,   246,   247,    37,   248,     0,   226,   227,   228,
     229,   521,     0,     0,     0,     0,     0,   230,     0,   249,
     250,   251,     0,     0,   252,     0,     0,   253,   254,   231,
       0,     0,     0,   232,   464,   233,   256,   465,   234,    38,
       0,   257,   235,   258,     0,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   236,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,   238,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   240,
     241,   242,   243,   244,   245,   246,   247,    37,   248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     522,     0,   249,   250,   251,     0,     0,   252,     0,     0,
     253,   254,     0,   226,   227,   228,   229,     0,     0,   256,
       0,     0,    38,   230,   257,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,   232,
       0,   233,     0,     0,   234,     0,     0,     0,   235,     0,
       0,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   236,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   238,     0,     0,     0,     0,     0,
       0,     0,   549,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,   240,   241,   242,   243,   244,
     245,   246,   247,    37,   248,     0,   226,   227,   228,   229,
       0,     0,     0,     0,     0,     0,   230,     0,   249,   250,
     251,     0,     0,   252,     0,     0,   253,   254,   231,     0,
       0,     0,   232,     0,   233,   256,     0,   234,    38,   550,
     257,   235,   258,     0,     0,     0,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     236,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,   241,
     242,   243,   244,   245,   246,   247,    37,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,   251,     0,     0,   252,     0,     0,   253,
     254,     0,   226,   227,   228,   229,     0,   255,   256,     0,
       0,    38,   230,   257,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
     233,     0,     0,   234,     0,     0,     0,   235,     0,     0,
       0,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   236,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,   240,   241,   242,   243,   244,   245,
     246,   247,    37,   248,     0,   226,   227,   228,   229,     0,
       0,     0,     0,     0,     0,   230,     0,   249,   250,   251,
       0,     0,   252,     0,     0,   253,   254,   231,     0,     0,
       0,   232,     0,   233,   256,   409,   234,    38,     0,   257,
     235,   258,     0,     0,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   236,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   240,   241,   242,
     243,   244,   245,   246,   247,    37,   248,     0,   226,   227,
     228,   229,     0,     0,     0,     0,     0,     0,   230,     0,
     249,   250,   251,     0,     0,   252,     0,     0,   253,   254,
     231,     0,     0,     0,   232,     0,   233,   256,   495,   234,
      38,     0,   257,   235,   258,     0,     0,     0,     0,    86,
      87,    88,     0,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,     0,   105,   106,
     107,     0,   236,   110,   111,   112,   113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
     240,   241,   242,   243,   244,   245,   246,   247,    37,   248,
       0,   226,   227,   228,   229,     0,     0,     0,     0,     0,
       0,   230,     0,   249,   250,   251,     0,     0,   252,     0,
       0,   253,   254,   231,     0,     0,     0,   232,     0,   233,
     256,   519,   234,    38,     0,   257,   235,   258,     0,     0,
       0,     0,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   236,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,   240,   241,   242,   243,   244,   245,   246,
     247,    37,   248,     0,   226,   227,   228,   229,     0,     0,
       0,     0,     0,     0,   230,     0,   249,   250,   251,     0,
       0,   252,     0,     0,   253,   254,   231,     0,     0,     0,
     232,     0,   233,   256,   532,   234,    38,     0,   257,   235,
     258,     0,     0,     0,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   236,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   240,   241,   242,   243,
     244,   245,   246,   247,    37,   248,     0,   226,   227,   228,
     229,     0,     0,     0,     0,     0,     0,   230,     0,   249,
     250,   251,     0,     0,   252,     0,     0,   253,   254,   231,
       0,     0,     0,   232,     0,   233,   256,   539,   234,    38,
       0,   257,   235,   258,     0,   558,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   236,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,   238,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   240,
     241,   242,   243,   244,   245,   246,   247,    37,   248,     0,
     226,   227,   228,   229,     0,     0,     0,     0,     0,     0,
     230,     0,   249,   250,   251,     0,     0,   252,     0,     0,
     253,   254,   231,     0,     0,     0,   232,     0,   233,   256,
       0,   234,    38,     0,   257,   235,   258,     0,     0,     0,
       0,    86,    87,    88,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
     105,   106,   107,     0,   236,   110,   111,   112,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   240,   241,   242,   243,   244,   245,   246,   247,
      37,   248,     0,   226,   227,   228,   229,   637,     0,     0,
       0,     0,     0,   230,     0,   249,   250,   251,     0,     0,
     252,     0,     0,   253,   254,   231,     0,     0,     0,   232,
       0,   233,   256,   618,   234,    38,     0,   257,   235,   258,
       0,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   236,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,   240,   241,   242,   243,   244,
     245,   246,   247,    37,   248,     0,   226,   227,   228,   229,
       0,     0,     0,     0,     0,     0,   230,     0,   249,   250,
     251,     0,     0,   252,     0,     0,   253,   254,   231,     0,
       0,     0,   232,     0,   233,   256,     0,   234,    38,     0,
     257,   235,   258,     0,     0,     0,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     236,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,     0,   237,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,   241,
     242,   243,   244,   245,   246,   247,    37,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,   251,     0,     0,   252,     0,     0,   253,
     254,     0,     0,     0,     0,     0,     0,     0,   256,     0,
       0,    38,     0,   257,     0,   258,   339,   340,   341,   342,
     343,     0,   338,   344,   345,   346,   347,     0,   348,   349,
     350,     0,   351,     0,   352,   353,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   354,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,   366,     0,
       0,     0,     0,     0,     0,   257,   380,   381,   382,   383,
     384,   385,   386,   387,   339,   340,   341,   342,   343,   388,
     389,   344,   345,   346,   347,   390,   348,   349,   350,   338,
     351,   391,   352,   353,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,   354,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   366,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   339,   340,   341,   342,   343,   388,   389,   344,   345,
     346,   347,   390,   348,   349,   350,   338,   351,   391,   352,
     353,   392,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,   366,     0,     0,     0,     0,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   339,   340,
     341,   342,   343,   388,   389,   344,   345,   346,   347,   390,
     348,   349,   350,   338,   351,   391,   352,   353,   392,   393,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   354,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
     366,     0,     0,     0,     0,     0,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   338,   380,   381,   382,
     383,   384,   385,   386,   387,   339,   340,   341,   342,   343,
     388,   389,   344,   345,   346,   347,   390,   348,   349,   350,
       0,   351,   391,   352,   353,   392,   393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   354,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,     0,   338,
       0,     0,     0,     0,     0,     0,   365,   366,   339,   340,
     341,   342,   343,   631,     0,   344,   345,   346,   347,     0,
     348,   349,   350,     0,   351,     0,   352,   353,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   354,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
     366,   339,   340,   341,   342,   343,   396,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,     0,   352,
     353,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,    83,     0,     0,     0,     0,     0,    84,
       0,     0,   365,   366,     0,     0,     0,     0,     0,   649,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,   239,   240,   241,     0,
       0,     0,     0,     0,     0,    37,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,     0,     0,   338,
      38,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,   162,     0,   338,
      38,   339,   340,   341,   342,   343,     0,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,     0,   352,
     353,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,   366,     0,   338,   430,     0,     0,     0,
      38,   339,   340,   341,   342,   343,     0,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,     0,   352,
     353,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   365,   366,     0,     0,   524,   339,   340,   341,
     342,   343,     0,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   365,   366,
       0,     0,   527,   339,   340,   341,   342,   343,     0,     0,
     344,   345,   346,   347,     0,   348,   349,   350,     0,   351,
       0,   352,   353,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   365,   366,     0,     0,   529,   339,
     340,   341,   342,   343,     0,     0,   344,   345,   346,   347,
       0,   348,   349,   350,     0,   351,     0,   352,   353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   354,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     365,   366,     0,     0,   583,   339,   340,   341,   342,   343,
       0,     0,   344,   345,   346,   347,     0,   348,   349,   350,
       0,   351,   180,   352,   353,     0,     0,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   354,   182,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,     0,     0,
       0,     0,     0,   338,     0,     0,   365,   366,     0,     0,
     648,   339,   340,   341,   342,   343,     0,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,     0,   352,
     353,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,   184,     0,     0,     0,   185,     0,
       0,     0,   365,   366,   367,   339,   340,   341,   342,   343,
     338,     0,   344,   345,   346,   347,   186,   348,   349,   350,
       0,   351,   296,   352,   353,     0,   187,     0,     0,     0,
       0,     0,     0,   180,     0,     0,     0,     0,   181,     0,
       0,     0,     0,     0,     0,   354,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   182,     0,     0,
       0,     0,     0,     0,     0,     0,   365,   366,   576,     0,
       0,     0,     0,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   339,   340,   341,   342,   343,   388,   389,   344,
     345,   346,   347,   390,   348,   349,   350,  -165,   351,   391,
     352,   353,   392,   393,   338,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,   354,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,     0,   184,     0,     0,     0,   185,
       0,   297,     0,   365,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,     0,     0,
     338,     0,     0,     0,     0,     0,     0,   187,   380,   381,
     382,   383,   384,   385,   386,   387,   339,   340,   341,   342,
     343,   388,   389,   344,   345,   346,   347,   390,   348,   349,
     350,     0,   351,   391,   352,   353,   392,   393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   394,   354,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   338,     0,
       0,     0,   339,   340,   341,   342,   343,   365,   366,   344,
     345,   346,   347,     0,   348,   349,   350,     0,   351,     0,
     352,   353,     0,     0,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   354,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   338,     0,     0,     0,     0,     0,
       0,     0,     0,   365,   366,     0,     0,     0,     0,     0,
     339,   340,   341,   342,   343,     0,     0,   344,   345,   346,
     347,     0,   348,   349,   350,     0,   351,     0,   352,   353,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     354,   575,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   338,     0,     0,     0,   339,   340,   341,   342,
     343,   365,   366,   344,   345,   346,   347,     0,   348,   349,
     350,     0,   351,     0,   352,   353,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   599,     0,     0,   354,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   338,     0,
       0,     0,     0,     0,     0,     0,     0,   365,   366,     0,
       0,     0,     0,     0,   339,   340,   341,   342,   343,   338,
       0,   344,   345,   346,   347,     0,   348,   349,   350,     0,
     351,     0,   352,   353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   354,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,     0,     0,     0,     0,
     339,   340,   341,   342,   343,   365,   366,   344,   345,   346,
     347,     0,   348,   349,   350,   338,   351,     0,   352,   353,
       0,   339,   340,   341,   342,   343,     0,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,     0,   352,
     353,     0,     0,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   180,   180,     0,     0,     0,   181,   181,     0,
       0,   365,   366,     0,     0,     0,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,   182,   182,     0,     0,
       0,     0,   365,   366,     0,     0,     0,   339,   340,   341,
     342,   343,     0,   180,   344,     0,     0,   347,   181,   348,
     349,   350,     0,   351,   180,   352,   353,   180,     0,   181,
       0,     0,   181,     0,     0,     0,   180,   182,     0,     0,
       0,   181,     0,     0,     0,     0,   183,   183,   182,     0,
       0,   182,   358,   359,   360,   361,   362,   363,   364,     0,
     182,     0,     0,     0,     0,   180,   180,     0,   365,   366,
     181,   181,     0,     0,   184,   184,     0,     0,   185,   185,
     300,   302,     0,     0,     0,     0,     0,   183,     0,   182,
     182,     0,     0,     0,     0,     0,   186,   186,   183,     0,
       0,   183,     0,   180,   180,     0,   187,   187,   181,   181,
     183,     0,   180,     0,     0,   184,     0,   181,     0,   185,
       0,   304,     0,     0,     0,     0,   184,   182,   182,   184,
     185,     0,     0,   185,     0,   518,   182,   186,   184,   183,
     183,     0,   185,     0,   525,     0,     0,   187,   186,     0,
     402,   186,     0,     0,     0,     0,     0,     0,   187,     0,
     186,   187,     0,     0,     0,     0,     0,   184,   184,     0,
     187,   185,   185,   526,   528,     0,     0,   183,   183,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,   186,
     186,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     187,     0,     0,     0,     0,   184,   184,     0,     0,   185,
     185,     0,     0,     0,   184,     0,     0,     0,   185,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   186,     0,
     621,   653,     0,     0,     0,     0,   186,   187,   187,     0,
       0,     0,     0,     0,     0,     0,   187,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,     0,     0,     0,
       0,     0,     0,   144,   145,   146,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,     0,     0,     0,
     158,   159
};

static const yytype_int16 yycheck[] =
{
      11,    69,   196,    55,   322,   204,   213,   334,   215,   368,
     217,   241,    28,    59,   191,   176,   177,   178,    42,    17,
      29,    42,    17,     0,    12,    13,   464,    30,     5,   101,
     105,   112,   122,   122,   122,    55,    28,   128,   101,   152,
     136,    33,   122,   402,    55,   122,   128,   160,    25,    60,
      27,   129,    29,    49,    31,   100,   116,   129,   103,    36,
      52,   128,   158,   122,   101,    42,   129,   158,   157,    28,
     160,    48,   160,   128,    33,   101,   158,   154,   159,   128,
     158,   107,   122,   158,   128,   157,   122,   286,   155,    92,
      93,   128,   129,    52,   131,   122,   128,   535,   122,   129,
     155,   122,   128,   129,   153,   131,   151,   152,   159,   101,
     102,   155,   123,   122,   128,   128,    18,   159,   134,   122,
     172,   173,   174,   155,   176,   177,   178,   179,   116,   128,
     128,   155,   120,   128,   122,   123,   152,   129,   130,   153,
     153,   198,   134,   128,   140,   197,   162,   155,   157,     9,
     158,   203,   172,   173,   174,   151,   176,   177,   178,    19,
     152,   172,   173,   174,   153,   176,   177,   178,   179,   158,
     162,   530,   128,   153,   335,   152,   228,   197,   158,   378,
     374,   158,   131,   203,   153,   129,   197,   239,   240,   158,
      92,    93,   203,   152,   371,   425,   124,   125,   100,   155,
     102,   103,   104,   162,   106,   124,   125,   126,   228,     4,
     158,     6,   160,   158,   532,   574,   159,   228,   161,   239,
     240,   539,   158,   158,   160,   160,   157,   122,   239,   240,
     276,   154,   158,   285,   160,    18,   435,   139,   140,   141,
     122,   252,   116,   602,   296,   122,   122,   122,   131,   151,
     152,   159,   320,    30,   306,    30,   463,   154,   124,   159,
     154,   313,   314,   135,   316,   285,   135,   135,   135,   131,
     135,   135,   135,   122,   285,   122,   296,   122,   134,   186,
      39,   135,   155,   153,   135,   296,   338,   135,   105,   122,
     136,   152,   136,   313,   314,   306,   316,   204,   136,   154,
     207,    18,   313,   314,   154,   316,   633,    90,    91,    92,
      93,    94,   154,   154,    97,    98,    99,   100,   338,   102,
     103,   104,   154,   106,   158,   108,   109,   338,   122,   122,
     237,   238,   122,   160,   241,   124,   128,   128,   158,    32,
     116,    17,   249,   250,   251,   158,   253,   254,   158,   256,
     549,   134,   135,   136,   137,   138,   139,   140,   141,   158,
     158,   158,   122,    11,   122,   122,   434,   154,   151,   152,
     131,   153,   153,    90,    91,    92,    93,   116,    35,   286,
     437,   153,   153,   100,   136,   102,   103,   104,   155,   106,
     159,   108,   109,    45,   326,    49,   590,   153,   153,   153,
     153,   651,    47,     1,    60,   312,   258,   610,   315,   379,
     317,   282,   290,   306,   321,   322,   567,   228,   538,   322,
     137,   138,   139,   140,   141,    -1,    -1,   334,    -1,    -1,
      -1,    -1,   339,   340,   151,   152,   343,   344,   345,   346,
      -1,   348,    -1,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   521,
      -1,   378,    -1,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,    -1,   546,
     547,    -1,    -1,    -1,    -1,    -1,   403,    -1,   555,   567,
      -1,   521,    -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,
     521,    -1,   569,    -1,   421,    18,    -1,   424,   425,    -1,
      -1,   428,    -1,    -1,    -1,   432,   433,    -1,   435,    -1,
      -1,   438,   439,    28,    92,    93,   443,    -1,    33,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   606,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     627,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,   637,    -1,   644,   645,    92,
      93,    -1,   499,   151,   152,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,   637,   525,   526,
      -1,   528,    -1,    -1,   531,    -1,   637,    10,    -1,   536,
      -1,   538,    -1,    -1,   129,   130,    -1,    20,    -1,   134,
      -1,    -1,   549,    26,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    41,    -1,
      -1,    -1,    -1,   158,    -1,   572,    -1,   162,   575,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   613,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   622,   623,    -1,   625,    -1,
      -1,   628,    -1,     1,    -1,    -1,   633,     5,    -1,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    18,   122,
      -1,    -1,    -1,    21,    22,    23,    24,   654,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    50,    51,   157,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    92,    93,    -1,    -1,    -1,    -1,
     100,    -1,    -1,   103,   104,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    -1,
      -1,   151,   152,    -1,    -1,    -1,   154,    -1,    -1,   157,
     158,   159,   160,   161,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    44,    -1,    46,
      47,    -1,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      -1,    -1,    -1,    -1,    28,    -1,    -1,   154,    -1,    33,
     157,   158,   159,   160,   161,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    52,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    38,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    52,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,   102,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   130,    -1,    -1,    -1,
     134,    -1,   136,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   152,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,   162,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      -1,    12,    13,    14,    15,   152,    -1,   154,    -1,    -1,
     157,    22,   159,    -1,   161,   162,    -1,    28,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    52,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    12,    13,    14,    15,    -1,   130,
      -1,    -1,    -1,   134,    22,    -1,   137,   138,   139,    -1,
      -1,   142,    -1,    -1,   145,   146,    34,    -1,    -1,    -1,
      38,   152,    40,   154,    -1,    43,   157,    -1,   159,    47,
     161,   162,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,
      -1,    -1,    -1,    38,   152,    40,   154,   155,    43,   157,
      -1,   159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,
     145,   146,    -1,    12,    13,    14,    15,    -1,    -1,   154,
      -1,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,
      -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,   138,
     139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,
      -1,    -1,    38,    -1,    40,   154,    -1,    43,   157,   158,
     159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    12,    13,    14,    15,    -1,   153,   154,    -1,
      -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   137,   138,   139,
      -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,    -1,
      -1,    38,    -1,    40,   154,   155,    43,   157,    -1,   159,
      47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      34,    -1,    -1,    -1,    38,    -1,    40,   154,   155,    43,
     157,    -1,   159,    47,   161,    -1,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   137,   138,   139,    -1,    -1,   142,    -1,
      -1,   145,   146,    34,    -1,    -1,    -1,    38,    -1,    40,
     154,   155,    43,   157,    -1,   159,    47,   161,    -1,    -1,
      -1,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   137,   138,   139,    -1,
      -1,   142,    -1,    -1,   145,   146,    34,    -1,    -1,    -1,
      38,    -1,    40,   154,   155,    43,   157,    -1,   159,    47,
     161,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,
      -1,    -1,    -1,    38,    -1,    40,   154,   155,    43,   157,
      -1,   159,    47,   161,    -1,    50,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,
     145,   146,    34,    -1,    -1,    -1,    38,    -1,    40,   154,
      -1,    43,   157,    -1,   159,    47,   161,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   137,   138,   139,    -1,    -1,
     142,    -1,    -1,   145,   146,    34,    -1,    -1,    -1,    38,
      -1,    40,   154,   155,    43,   157,    -1,   159,    47,   161,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,   138,
     139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,
      -1,    -1,    38,    -1,    40,   154,    -1,    43,   157,    -1,
     159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,   157,    -1,   159,    -1,   161,    90,    91,    92,    93,
      94,    -1,    18,    97,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    18,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    18,   106,   107,   108,
     109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    18,   106,   107,   108,   109,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    90,    91,
      92,    93,    94,   158,    -1,    97,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,    90,    91,    92,    93,    94,   158,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    20,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    18,
     157,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    -1,   154,    -1,    18,
     157,    90,    91,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,    -1,    18,   155,    -1,    -1,    -1,
     157,    90,    91,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,   151,   152,    -1,    -1,   155,    90,    91,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,   151,   152,
      -1,    -1,   155,    90,    91,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,   151,   152,    -1,    -1,   155,    90,
      91,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      -1,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
     151,   152,    -1,    -1,   155,    90,    91,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,    28,   108,   109,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    52,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,   151,   152,    -1,    -1,
     155,    90,    91,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   151,   152,   153,    90,    91,    92,    93,    94,
      18,    -1,    97,    98,    99,   100,   152,   102,   103,   104,
      -1,   106,   158,   108,   109,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    18,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,   162,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    18,    -1,
      -1,    -1,    90,    91,    92,    93,    94,   151,   152,    97,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,    -1,
     108,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    18,    -1,    -1,    -1,    90,    91,    92,    93,
      94,   151,   152,    97,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    18,
      -1,    97,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,   151,   152,    97,    98,    99,
     100,    -1,   102,   103,   104,    18,   106,    -1,   108,   109,
      -1,    90,    91,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    28,    28,    -1,    -1,    -1,    33,    33,    -1,
      -1,   151,   152,    -1,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    52,    52,    -1,    -1,
      -1,    -1,   151,   152,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    -1,    28,    97,    -1,    -1,   100,    33,   102,
     103,   104,    -1,   106,    28,   108,   109,    28,    -1,    33,
      -1,    -1,    33,    -1,    -1,    -1,    28,    52,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,   102,   102,    52,    -1,
      -1,    52,   135,   136,   137,   138,   139,   140,   141,    -1,
      52,    -1,    -1,    -1,    -1,    28,    28,    -1,   151,   152,
      33,    33,    -1,    -1,   130,   130,    -1,    -1,   134,   134,
     136,   136,    -1,    -1,    -1,    -1,    -1,   102,    -1,    52,
      52,    -1,    -1,    -1,    -1,    -1,   152,   152,   102,    -1,
      -1,   102,    -1,    28,    28,    -1,   162,   162,    33,    33,
     102,    -1,    28,    -1,    -1,   130,    -1,    33,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,   130,    52,    52,   130,
     134,    -1,    -1,   134,    -1,   136,    52,   152,   130,   102,
     102,    -1,   134,    -1,   136,    -1,    -1,   162,   152,    -1,
     154,   152,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     152,   162,    -1,    -1,    -1,    -1,    -1,   130,   130,    -1,
     162,   134,   134,   136,   136,    -1,    -1,   102,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,   152,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     162,    -1,    -1,    -1,    -1,   130,   130,    -1,    -1,   134,
     134,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   152,    -1,
     155,   155,    -1,    -1,    -1,    -1,   152,   162,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
     145,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   164,     0,     5,    25,    27,    29,    31,    36,    42,
      48,   152,   158,   165,   170,   171,   175,   190,   194,   207,
     232,   234,   235,   238,   122,   185,   186,   116,   176,   177,
     122,   172,   173,   122,   122,   122,    29,   122,   157,   187,
     188,   189,   199,     4,     6,   159,   129,   128,   131,   128,
      49,   140,   151,   174,   159,   129,   158,   157,   122,   154,
     128,   153,   122,   237,    30,    92,    93,   122,   193,   231,
      12,    13,   116,   120,   122,   123,   167,   185,   116,   177,
     122,   122,   233,    20,    26,    41,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,   199,   240,   241,   242,   243,
     122,   186,   188,   131,   236,   159,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   108,   109,   110,   111,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   145,   146,
      30,    30,   154,   191,   122,   160,   230,   239,   124,   166,
     122,   160,   135,   135,   135,   154,   135,   135,   135,   135,
      28,    33,    52,   102,   130,   134,   152,   162,   155,   199,
     222,    42,   155,   223,   224,   228,   239,   131,   192,   101,
     107,   128,   129,   131,   229,   124,   125,   129,   158,   243,
     243,   243,   122,   191,   243,   191,   243,   191,   243,   225,
     226,   243,    28,   134,   152,   162,    12,    13,    14,    15,
      22,    34,    38,    40,    43,    47,    76,    92,    93,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   123,   137,
     138,   139,   142,   145,   146,   153,   154,   159,   161,   169,
     195,   199,   201,   209,   210,   213,   214,   216,   217,   218,
     240,   244,   247,   252,   254,   256,   152,   160,   219,   221,
     228,   155,   158,   101,   129,   131,   227,   243,   195,   122,
     243,   198,   215,   218,   218,   136,   158,   136,   155,   192,
     136,   192,   136,   192,   136,   136,   158,   134,   153,   243,
     244,   154,   154,   135,   135,   154,   135,   154,   218,   218,
      10,   243,   243,   218,   249,   253,   168,   218,   218,   218,
     199,   218,   218,   218,   197,   190,   122,   154,    18,    90,
      91,    92,    93,    94,    97,    98,    99,   100,   102,   103,
     104,   106,   108,   109,   130,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   151,   152,   153,   154,   186,
      39,   220,   158,   223,   243,   198,   218,   158,   229,   105,
      82,    83,    84,    85,    86,    87,    88,    89,    95,    96,
     101,   107,   110,   111,   129,   158,   158,   243,   136,   136,
     136,   225,   154,   122,   218,   243,   243,   218,   243,   155,
     218,   239,   122,   154,   218,   245,   246,   248,   249,   250,
     251,   154,   246,   251,   112,   158,   160,   125,   126,   166,
     155,     1,     7,     8,    10,    21,    23,    24,    33,    37,
      44,    46,    50,    51,   158,   160,   178,   179,   181,   182,
     183,   184,   196,   198,   200,   202,   203,   204,   205,   207,
     208,   215,   218,   191,   152,   155,   211,   218,   243,   218,
     218,   218,   218,   218,   218,   122,   218,   122,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   122,   218,   155,   211,   153,   228,   227,
     198,   218,   213,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   136,   155,
     211,    16,   135,   218,   155,   136,   136,   155,   136,   155,
      17,   227,   155,   128,   153,   158,   128,   153,   158,   155,
     160,   160,   218,   249,   160,   218,   218,   218,   239,   101,
     158,   198,   218,   195,   218,   218,   158,   116,    50,   218,
      32,   158,   158,   158,   158,   158,    17,   206,   158,   192,
     122,   245,   128,   155,   154,   131,   153,   155,   218,   158,
     155,   243,   122,   155,   218,   218,   218,   211,   218,   246,
     122,   153,   245,   218,   153,   250,   246,   153,   153,   127,
     195,   195,    17,   198,   218,   158,    11,   195,   131,   116,
     159,    35,   230,   112,   195,   212,   153,   218,   155,   211,
     218,   155,   136,   158,   153,   227,   160,     9,    19,   180,
     211,   158,   195,   197,   218,   155,   155,    16,   218,   218,
     153,   218,   153,   195,   218,   158,   160,   243,   155,   158,
     255,   195,   195,   155,    45,   153,   180,   218,   153
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   166,   166,   167,   168,   168,   168,
     169,   170,   171,   172,   172,   172,   173,   174,   174,   175,
     176,   176,   177,   177,   178,   179,   179,   180,   180,   180,
     181,   182,   183,   184,   185,   185,   185,   185,   185,   185,
     186,   186,   187,   187,   188,   188,   189,   189,   190,   190,
     191,   191,   191,   192,   192,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   194,   195,   195,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   197,   197,   197,   198,   199,   199,   199,   200,   201,
     201,   201,   201,   202,   203,   204,   204,   204,   204,   204,
     205,   206,   206,   207,   207,   208,   209,   209,   209,   210,
     210,   210,   210,   211,   211,   212,   212,   213,   213,   214,
     214,   214,   214,   214,   214,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   216,   217,   217,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   219,   219,   220,   220,   221,   222,   222,
     223,   223,   224,   224,   225,   226,   226,   227,   227,   228,
     228,   228,   228,   228,   229,   229,   229,   230,   230,   230,
     230,   230,   231,   231,   232,   233,   233,   233,   234,   235,
     236,   236,   237,   238,   239,   239,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     241,   242,   242,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   244,   244,   244,   244,   245,   245,   246,   246,
     247,   247,   247,   247,   248,   248,   249,   250,   250,   251,
     251,   252,   252,   253,   253,   254,   255,   255,   256
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     0,     1,     2,
       1,     3,     1,     3,     3,     3,     2,     0,     2,     4,
       4,     6,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     4,     1,     3,     0,     3,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     6,     3,     7,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     2,     2,
       2,     0,     2,     2,     3,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     2,     3,     4,     2,     3,
       4,     0,     2,     1,     1,     3,     5,     5,     5,     5,
       8,     6,     9,     1,     3,     1,     2,     1,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     5,     6,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     3,
       3,     2,     3,     4,     3,     4,     2,     4,     4,     3,
       4,     3,     5,     3,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     0,     3,     0,     1,     3,     0,     3,
       1,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       3,     5,     3,     3,     1,     1,     1,     4,     6,     5,
       4,     3,     0,     2,     4,     0,     3,     5,     4,     5,
       0,     2,     2,     6,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     1,     1,     4,     3,     4,     2,
       3,     2,     4,     3,     2,     2,     3,     2,     2,     4,
       6,     4,     1,     4,     5,     1,     4,     5,     1,     4,
       5,     4,     1,     1,     1,     1,     3,     5,     1,     3,
       5,     7,     5,     7,     1,     3,     3,     1,     1,     1,
       3,     5,     5,     1,     3,     4,     0,     3,    10
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
    case 122: /* "name"  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2663 "ds_parser.cpp"
        break;

    case 166: /* character_sequence  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2669 "ds_parser.cpp"
        break;

    case 167: /* string_constant  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2675 "ds_parser.cpp"
        break;

    case 168: /* string_builder_body  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2681 "ds_parser.cpp"
        break;

    case 169: /* string_builder  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2687 "ds_parser.cpp"
        break;

    case 172: /* require_module_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2693 "ds_parser.cpp"
        break;

    case 178: /* expression_label  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2699 "ds_parser.cpp"
        break;

    case 179: /* expression_goto  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2705 "ds_parser.cpp"
        break;

    case 180: /* expression_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2711 "ds_parser.cpp"
        break;

    case 181: /* expression_if_then_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2717 "ds_parser.cpp"
        break;

    case 182: /* expression_for_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2723 "ds_parser.cpp"
        break;

    case 183: /* expression_while_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2729 "ds_parser.cpp"
        break;

    case 184: /* expression_with  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2735 "ds_parser.cpp"
        break;

    case 185: /* annotation_argument  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2741 "ds_parser.cpp"
        break;

    case 186: /* annotation_argument_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2747 "ds_parser.cpp"
        break;

    case 187: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2753 "ds_parser.cpp"
        break;

    case 188: /* annotation_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2759 "ds_parser.cpp"
        break;

    case 189: /* annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2765 "ds_parser.cpp"
        break;

    case 190: /* optional_annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2771 "ds_parser.cpp"
        break;

    case 191: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2777 "ds_parser.cpp"
        break;

    case 192: /* optional_function_type  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2783 "ds_parser.cpp"
        break;

    case 193: /* function_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2789 "ds_parser.cpp"
        break;

    case 195: /* expression_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2795 "ds_parser.cpp"
        break;

    case 196: /* expression_any  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2801 "ds_parser.cpp"
        break;

    case 197: /* expressions  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2807 "ds_parser.cpp"
        break;

    case 198: /* expr_pipe  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2813 "ds_parser.cpp"
        break;

    case 199: /* name_in_namespace  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2819 "ds_parser.cpp"
        break;

    case 200: /* expression_delete  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2825 "ds_parser.cpp"
        break;

    case 201: /* expr_new  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2831 "ds_parser.cpp"
        break;

    case 202: /* expression_break  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2837 "ds_parser.cpp"
        break;

    case 203: /* expression_continue  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2843 "ds_parser.cpp"
        break;

    case 204: /* expression_return  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2849 "ds_parser.cpp"
        break;

    case 205: /* expression_try_catch  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2855 "ds_parser.cpp"
        break;

    case 208: /* expression_let  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2861 "ds_parser.cpp"
        break;

    case 209: /* expr_cast  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2867 "ds_parser.cpp"
        break;

    case 210: /* expr_type_info  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2873 "ds_parser.cpp"
        break;

    case 211: /* expr_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2879 "ds_parser.cpp"
        break;

    case 212: /* block_or_simple_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2885 "ds_parser.cpp"
        break;

    case 213: /* expr_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2891 "ds_parser.cpp"
        break;

    case 214: /* expr_numeric_const  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2897 "ds_parser.cpp"
        break;

    case 215: /* expr_assign  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2903 "ds_parser.cpp"
        break;

    case 216: /* expr_named_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2909 "ds_parser.cpp"
        break;

    case 217: /* expr_method_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2915 "ds_parser.cpp"
        break;

    case 218: /* expr  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2921 "ds_parser.cpp"
        break;

    case 219: /* optional_field_annotation  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2927 "ds_parser.cpp"
        break;

    case 221: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2933 "ds_parser.cpp"
        break;

    case 222: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2939 "ds_parser.cpp"
        break;

    case 223: /* function_argument_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2945 "ds_parser.cpp"
        break;

    case 224: /* function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2951 "ds_parser.cpp"
        break;

    case 225: /* tuple_type  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2957 "ds_parser.cpp"
        break;

    case 226: /* tuple_type_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2963 "ds_parser.cpp"
        break;

    case 228: /* variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2969 "ds_parser.cpp"
        break;

    case 230: /* let_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2975 "ds_parser.cpp"
        break;

    case 231: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2981 "ds_parser.cpp"
        break;

    case 233: /* enum_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 2987 "ds_parser.cpp"
        break;

    case 236: /* optional_structure_parent  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2993 "ds_parser.cpp"
        break;

    case 239: /* variable_name_list  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 2999 "ds_parser.cpp"
        break;

    case 241: /* structure_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3005 "ds_parser.cpp"
        break;

    case 242: /* auto_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3011 "ds_parser.cpp"
        break;

    case 243: /* type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3017 "ds_parser.cpp"
        break;

    case 244: /* make_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3023 "ds_parser.cpp"
        break;

    case 245: /* make_struct_fields  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3029 "ds_parser.cpp"
        break;

    case 246: /* make_struct_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3035 "ds_parser.cpp"
        break;

    case 247: /* make_struct_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3041 "ds_parser.cpp"
        break;

    case 248: /* make_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3047 "ds_parser.cpp"
        break;

    case 249: /* make_map_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3053 "ds_parser.cpp"
        break;

    case 250: /* make_any_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3059 "ds_parser.cpp"
        break;

    case 251: /* make_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3065 "ds_parser.cpp"
        break;

    case 252: /* make_dim_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3071 "ds_parser.cpp"
        break;

    case 253: /* make_table  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3077 "ds_parser.cpp"
        break;

    case 254: /* make_table_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3083 "ds_parser.cpp"
        break;

    case 255: /* array_comprehension_where  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3089 "ds_parser.cpp"
        break;

    case 256: /* array_comprehension  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3095 "ds_parser.cpp"
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 13:
#line 373 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3395 "ds_parser.cpp"
    break;

  case 14:
#line 385 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3401 "ds_parser.cpp"
    break;

  case 15:
#line 386 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3407 "ds_parser.cpp"
    break;

  case 16:
#line 390 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3413 "ds_parser.cpp"
    break;

  case 17:
#line 394 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3422 "ds_parser.cpp"
    break;

  case 18:
#line 398 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3436 "ds_parser.cpp"
    break;

  case 19:
#line 407 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3446 "ds_parser.cpp"
    break;

  case 20:
#line 415 "ds_parser.ypp"
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
#line 3464 "ds_parser.cpp"
    break;

  case 21:
#line 431 "ds_parser.ypp"
    {
		if ( g_Program->options.size() ) {
			g_Program->options.insert ( g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
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
#line 3487 "ds_parser.cpp"
    break;

  case 23:
#line 456 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3495 "ds_parser.cpp"
    break;

  case 24:
#line 459 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3506 "ds_parser.cpp"
    break;

  case 25:
#line 465 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3517 "ds_parser.cpp"
    break;

  case 26:
#line 474 "ds_parser.ypp"
    {
        auto modN = getModuleName(*(yyvsp[-1].s));
        if ( auto mod = g_Program->addModule(modN) ) {
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
        } else {
            das_yyerror("required module not found " + *(yyvsp[-1].s),tokAt((yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 3532 "ds_parser.cpp"
    break;

  case 27:
#line 487 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3538 "ds_parser.cpp"
    break;

  case 28:
#line 488 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3544 "ds_parser.cpp"
    break;

  case 32:
#line 501 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3552 "ds_parser.cpp"
    break;

  case 33:
#line 504 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3560 "ds_parser.cpp"
    break;

  case 34:
#line 510 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3568 "ds_parser.cpp"
    break;

  case 35:
#line 516 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3576 "ds_parser.cpp"
    break;

  case 36:
#line 519 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3584 "ds_parser.cpp"
    break;

  case 37:
#line 525 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3590 "ds_parser.cpp"
    break;

  case 38:
#line 526 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3596 "ds_parser.cpp"
    break;

  case 39:
#line 527 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3605 "ds_parser.cpp"
    break;

  case 40:
#line 534 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3614 "ds_parser.cpp"
    break;

  case 41:
#line 541 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3628 "ds_parser.cpp"
    break;

  case 42:
#line 553 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3640 "ds_parser.cpp"
    break;

  case 43:
#line 563 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3651 "ds_parser.cpp"
    break;

  case 44:
#line 572 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3657 "ds_parser.cpp"
    break;

  case 45:
#line 573 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3663 "ds_parser.cpp"
    break;

  case 46:
#line 574 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3669 "ds_parser.cpp"
    break;

  case 47:
#line 575 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3675 "ds_parser.cpp"
    break;

  case 48:
#line 576 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3681 "ds_parser.cpp"
    break;

  case 49:
#line 577 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3687 "ds_parser.cpp"
    break;

  case 50:
#line 581 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3697 "ds_parser.cpp"
    break;

  case 51:
#line 586 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3707 "ds_parser.cpp"
    break;

  case 52:
#line 594 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 3713 "ds_parser.cpp"
    break;

  case 53:
#line 595 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 3719 "ds_parser.cpp"
    break;

  case 54:
#line 599 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3731 "ds_parser.cpp"
    break;

  case 55:
#line 606 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3745 "ds_parser.cpp"
    break;

  case 56:
#line 618 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3754 "ds_parser.cpp"
    break;

  case 57:
#line 622 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3763 "ds_parser.cpp"
    break;

  case 58:
#line 629 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 3769 "ds_parser.cpp"
    break;

  case 59:
#line 630 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3775 "ds_parser.cpp"
    break;

  case 60:
#line 634 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3781 "ds_parser.cpp"
    break;

  case 61:
#line 635 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3787 "ds_parser.cpp"
    break;

  case 62:
#line 636 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3793 "ds_parser.cpp"
    break;

  case 63:
#line 640 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3802 "ds_parser.cpp"
    break;

  case 64:
#line 644 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3811 "ds_parser.cpp"
    break;

  case 65:
#line 651 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3820 "ds_parser.cpp"
    break;

  case 66:
#line 655 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 3826 "ds_parser.cpp"
    break;

  case 67:
#line 656 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 3832 "ds_parser.cpp"
    break;

  case 68:
#line 657 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 3838 "ds_parser.cpp"
    break;

  case 69:
#line 658 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 3844 "ds_parser.cpp"
    break;

  case 70:
#line 659 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 3850 "ds_parser.cpp"
    break;

  case 71:
#line 660 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 3856 "ds_parser.cpp"
    break;

  case 72:
#line 661 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 3862 "ds_parser.cpp"
    break;

  case 73:
#line 662 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 3868 "ds_parser.cpp"
    break;

  case 74:
#line 663 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 3874 "ds_parser.cpp"
    break;

  case 75:
#line 664 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 3880 "ds_parser.cpp"
    break;

  case 76:
#line 665 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 3886 "ds_parser.cpp"
    break;

  case 77:
#line 666 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 3892 "ds_parser.cpp"
    break;

  case 78:
#line 667 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 3898 "ds_parser.cpp"
    break;

  case 79:
#line 668 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 3904 "ds_parser.cpp"
    break;

  case 80:
#line 669 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 3910 "ds_parser.cpp"
    break;

  case 81:
#line 670 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 3916 "ds_parser.cpp"
    break;

  case 82:
#line 671 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 3922 "ds_parser.cpp"
    break;

  case 83:
#line 672 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 3928 "ds_parser.cpp"
    break;

  case 84:
#line 673 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 3934 "ds_parser.cpp"
    break;

  case 85:
#line 674 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 3940 "ds_parser.cpp"
    break;

  case 86:
#line 675 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 3946 "ds_parser.cpp"
    break;

  case 87:
#line 676 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 3952 "ds_parser.cpp"
    break;

  case 88:
#line 677 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 3958 "ds_parser.cpp"
    break;

  case 89:
#line 678 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 3964 "ds_parser.cpp"
    break;

  case 90:
#line 679 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 3970 "ds_parser.cpp"
    break;

  case 91:
#line 680 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 3976 "ds_parser.cpp"
    break;

  case 92:
#line 681 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 3982 "ds_parser.cpp"
    break;

  case 93:
#line 682 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 3988 "ds_parser.cpp"
    break;

  case 94:
#line 683 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 3994 "ds_parser.cpp"
    break;

  case 95:
#line 684 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4000 "ds_parser.cpp"
    break;

  case 96:
#line 685 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4006 "ds_parser.cpp"
    break;

  case 97:
#line 686 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4012 "ds_parser.cpp"
    break;

  case 98:
#line 687 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4018 "ds_parser.cpp"
    break;

  case 99:
#line 688 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4024 "ds_parser.cpp"
    break;

  case 100:
#line 689 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4030 "ds_parser.cpp"
    break;

  case 101:
#line 690 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4036 "ds_parser.cpp"
    break;

  case 102:
#line 695 "ds_parser.ypp"
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
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
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
#line 4107 "ds_parser.cpp"
    break;

  case 103:
#line 764 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4115 "ds_parser.cpp"
    break;

  case 104:
#line 767 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4127 "ds_parser.cpp"
    break;

  case 105:
#line 777 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4133 "ds_parser.cpp"
    break;

  case 106:
#line 778 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4139 "ds_parser.cpp"
    break;

  case 107:
#line 779 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4145 "ds_parser.cpp"
    break;

  case 108:
#line 780 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4151 "ds_parser.cpp"
    break;

  case 109:
#line 781 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4157 "ds_parser.cpp"
    break;

  case 110:
#line 782 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4163 "ds_parser.cpp"
    break;

  case 111:
#line 783 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4169 "ds_parser.cpp"
    break;

  case 112:
#line 784 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4175 "ds_parser.cpp"
    break;

  case 113:
#line 785 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4181 "ds_parser.cpp"
    break;

  case 114:
#line 786 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4187 "ds_parser.cpp"
    break;

  case 115:
#line 787 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4193 "ds_parser.cpp"
    break;

  case 116:
#line 788 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4199 "ds_parser.cpp"
    break;

  case 117:
#line 789 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4205 "ds_parser.cpp"
    break;

  case 118:
#line 790 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4211 "ds_parser.cpp"
    break;

  case 119:
#line 791 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4217 "ds_parser.cpp"
    break;

  case 120:
#line 792 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4223 "ds_parser.cpp"
    break;

  case 121:
#line 796 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4232 "ds_parser.cpp"
    break;

  case 122:
#line 800 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4243 "ds_parser.cpp"
    break;

  case 123:
#line 806 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4251 "ds_parser.cpp"
    break;

  case 124:
#line 812 "ds_parser.ypp"
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
#line 4267 "ds_parser.cpp"
    break;

  case 125:
#line 826 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4273 "ds_parser.cpp"
    break;

  case 126:
#line 827 "ds_parser.ypp"
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4284 "ds_parser.cpp"
    break;

  case 127:
#line 833 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 128:
#line 837 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4298 "ds_parser.cpp"
    break;

  case 129:
#line 843 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4306 "ds_parser.cpp"
    break;

  case 130:
#line 846 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4314 "ds_parser.cpp"
    break;

  case 131:
#line 849 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4323 "ds_parser.cpp"
    break;

  case 132:
#line 853 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4331 "ds_parser.cpp"
    break;

  case 133:
#line 859 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4337 "ds_parser.cpp"
    break;

  case 134:
#line 863 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4343 "ds_parser.cpp"
    break;

  case 135:
#line 867 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4351 "ds_parser.cpp"
    break;

  case 136:
#line 870 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4359 "ds_parser.cpp"
    break;

  case 137:
#line 873 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4369 "ds_parser.cpp"
    break;

  case 138:
#line 878 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4377 "ds_parser.cpp"
    break;

  case 139:
#line 881 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4387 "ds_parser.cpp"
    break;

  case 140:
#line 889 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4395 "ds_parser.cpp"
    break;

  case 141:
#line 895 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4401 "ds_parser.cpp"
    break;

  case 142:
#line 896 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4407 "ds_parser.cpp"
    break;

  case 143:
#line 900 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4413 "ds_parser.cpp"
    break;

  case 144:
#line 901 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4419 "ds_parser.cpp"
    break;

  case 145:
#line 905 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
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
                        pVar->init_via_move = (yyvsp[0].pVarDecl)->init_via_move;
                        pVar->init_via_clone = (yyvsp[0].pVarDecl)->init_via_clone;
                    }
                    if ( (yyvsp[-2].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
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
#line 4454 "ds_parser.cpp"
    break;

  case 146:
#line 938 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4462 "ds_parser.cpp"
    break;

  case 147:
#line 941 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4472 "ds_parser.cpp"
    break;

  case 148:
#line 946 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4482 "ds_parser.cpp"
    break;

  case 149:
#line 954 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4491 "ds_parser.cpp"
    break;

  case 150:
#line 958 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4501 "ds_parser.cpp"
    break;

  case 151:
#line 963 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4510 "ds_parser.cpp"
    break;

  case 152:
#line 967 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4520 "ds_parser.cpp"
    break;

  case 153:
#line 975 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4528 "ds_parser.cpp"
    break;

  case 154:
#line 978 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4536 "ds_parser.cpp"
    break;

  case 155:
#line 984 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4542 "ds_parser.cpp"
    break;

  case 156:
#line 985 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4554 "ds_parser.cpp"
    break;

  case 157:
#line 995 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4564 "ds_parser.cpp"
    break;

  case 158:
#line 1001 "ds_parser.ypp"
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
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
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
#line 4617 "ds_parser.cpp"
    break;

  case 159:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4623 "ds_parser.cpp"
    break;

  case 160:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4629 "ds_parser.cpp"
    break;

  case 161:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4635 "ds_parser.cpp"
    break;

  case 162:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4641 "ds_parser.cpp"
    break;

  case 163:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4647 "ds_parser.cpp"
    break;

  case 164:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4653 "ds_parser.cpp"
    break;

  case 165:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4659 "ds_parser.cpp"
    break;

  case 166:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4665 "ds_parser.cpp"
    break;

  case 167:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4671 "ds_parser.cpp"
    break;

  case 168:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4677 "ds_parser.cpp"
    break;

  case 169:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4683 "ds_parser.cpp"
    break;

  case 170:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4689 "ds_parser.cpp"
    break;

  case 171:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4695 "ds_parser.cpp"
    break;

  case 172:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4701 "ds_parser.cpp"
    break;

  case 173:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4707 "ds_parser.cpp"
    break;

  case 174:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4713 "ds_parser.cpp"
    break;

  case 175:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4719 "ds_parser.cpp"
    break;

  case 176:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4725 "ds_parser.cpp"
    break;

  case 177:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4731 "ds_parser.cpp"
    break;

  case 178:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4737 "ds_parser.cpp"
    break;

  case 179:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4743 "ds_parser.cpp"
    break;

  case 180:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4749 "ds_parser.cpp"
    break;

  case 181:
#line 1080 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4761 "ds_parser.cpp"
    break;

  case 182:
#line 1091 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4771 "ds_parser.cpp"
    break;

  case 183:
#line 1096 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4783 "ds_parser.cpp"
    break;

  case 184:
#line 1106 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4789 "ds_parser.cpp"
    break;

  case 185:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4795 "ds_parser.cpp"
    break;

  case 186:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4801 "ds_parser.cpp"
    break;

  case 187:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4807 "ds_parser.cpp"
    break;

  case 188:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4813 "ds_parser.cpp"
    break;

  case 189:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4819 "ds_parser.cpp"
    break;

  case 190:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4825 "ds_parser.cpp"
    break;

  case 191:
#line 1113 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4831 "ds_parser.cpp"
    break;

  case 192:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4837 "ds_parser.cpp"
    break;

  case 193:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4843 "ds_parser.cpp"
    break;

  case 194:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4849 "ds_parser.cpp"
    break;

  case 195:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4855 "ds_parser.cpp"
    break;

  case 196:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4861 "ds_parser.cpp"
    break;

  case 197:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4867 "ds_parser.cpp"
    break;

  case 198:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4873 "ds_parser.cpp"
    break;

  case 199:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4879 "ds_parser.cpp"
    break;

  case 200:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4885 "ds_parser.cpp"
    break;

  case 201:
#line 1123 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4891 "ds_parser.cpp"
    break;

  case 202:
#line 1124 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4897 "ds_parser.cpp"
    break;

  case 203:
#line 1125 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4903 "ds_parser.cpp"
    break;

  case 204:
#line 1126 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4909 "ds_parser.cpp"
    break;

  case 205:
#line 1127 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4915 "ds_parser.cpp"
    break;

  case 206:
#line 1128 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4921 "ds_parser.cpp"
    break;

  case 207:
#line 1129 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4927 "ds_parser.cpp"
    break;

  case 208:
#line 1130 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4933 "ds_parser.cpp"
    break;

  case 209:
#line 1131 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4939 "ds_parser.cpp"
    break;

  case 210:
#line 1132 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4945 "ds_parser.cpp"
    break;

  case 211:
#line 1133 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4951 "ds_parser.cpp"
    break;

  case 212:
#line 1134 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4957 "ds_parser.cpp"
    break;

  case 213:
#line 1135 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4963 "ds_parser.cpp"
    break;

  case 214:
#line 1136 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4969 "ds_parser.cpp"
    break;

  case 215:
#line 1137 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4975 "ds_parser.cpp"
    break;

  case 216:
#line 1138 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4981 "ds_parser.cpp"
    break;

  case 217:
#line 1139 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4987 "ds_parser.cpp"
    break;

  case 218:
#line 1140 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4993 "ds_parser.cpp"
    break;

  case 219:
#line 1141 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4999 "ds_parser.cpp"
    break;

  case 220:
#line 1142 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5005 "ds_parser.cpp"
    break;

  case 221:
#line 1143 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5011 "ds_parser.cpp"
    break;

  case 222:
#line 1144 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5020 "ds_parser.cpp"
    break;

  case 223:
#line 1148 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5029 "ds_parser.cpp"
    break;

  case 224:
#line 1152 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5037 "ds_parser.cpp"
    break;

  case 225:
#line 1155 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5045 "ds_parser.cpp"
    break;

  case 226:
#line 1158 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5051 "ds_parser.cpp"
    break;

  case 227:
#line 1159 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5057 "ds_parser.cpp"
    break;

  case 228:
#line 1160 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5063 "ds_parser.cpp"
    break;

  case 229:
#line 1161 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5069 "ds_parser.cpp"
    break;

  case 230:
#line 1162 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5075 "ds_parser.cpp"
    break;

  case 231:
#line 1163 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5081 "ds_parser.cpp"
    break;

  case 232:
#line 1164 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5089 "ds_parser.cpp"
    break;

  case 233:
#line 1167 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5097 "ds_parser.cpp"
    break;

  case 234:
#line 1170 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5103 "ds_parser.cpp"
    break;

  case 235:
#line 1171 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5109 "ds_parser.cpp"
    break;

  case 236:
#line 1172 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5115 "ds_parser.cpp"
    break;

  case 237:
#line 1173 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5121 "ds_parser.cpp"
    break;

  case 238:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5127 "ds_parser.cpp"
    break;

  case 239:
#line 1175 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5133 "ds_parser.cpp"
    break;

  case 240:
#line 1176 "ds_parser.ypp"
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
#line 5148 "ds_parser.cpp"
    break;

  case 241:
#line 1186 "ds_parser.ypp"
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
#line 5163 "ds_parser.cpp"
    break;

  case 242:
#line 1196 "ds_parser.ypp"
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
                resEnum = new ExprConstEnumeration(tokAt((yylsp[0])), *(yyvsp[0].s), td);
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
#line 5199 "ds_parser.cpp"
    break;

  case 243:
#line 1230 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5205 "ds_parser.cpp"
    break;

  case 244:
#line 1231 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5211 "ds_parser.cpp"
    break;

  case 245:
#line 1235 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5217 "ds_parser.cpp"
    break;

  case 246:
#line 1236 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5223 "ds_parser.cpp"
    break;

  case 247:
#line 1240 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5233 "ds_parser.cpp"
    break;

  case 248:
#line 1248 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5241 "ds_parser.cpp"
    break;

  case 249:
#line 1251 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5250 "ds_parser.cpp"
    break;

  case 250:
#line 1258 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5256 "ds_parser.cpp"
    break;

  case 251:
#line 1259 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5262 "ds_parser.cpp"
    break;

  case 252:
#line 1263 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5268 "ds_parser.cpp"
    break;

  case 253:
#line 1264 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5274 "ds_parser.cpp"
    break;

  case 254:
#line 1268 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5282 "ds_parser.cpp"
    break;

  case 255:
#line 1274 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5288 "ds_parser.cpp"
    break;

  case 256:
#line 1275 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5294 "ds_parser.cpp"
    break;

  case 257:
#line 1279 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5300 "ds_parser.cpp"
    break;

  case 258:
#line 1280 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5306 "ds_parser.cpp"
    break;

  case 259:
#line 1284 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5316 "ds_parser.cpp"
    break;

  case 260:
#line 1289 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5324 "ds_parser.cpp"
    break;

  case 261:
#line 1292 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5333 "ds_parser.cpp"
    break;

  case 262:
#line 1296 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5344 "ds_parser.cpp"
    break;

  case 263:
#line 1302 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5355 "ds_parser.cpp"
    break;

  case 264:
#line 1311 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5361 "ds_parser.cpp"
    break;

  case 265:
#line 1312 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5367 "ds_parser.cpp"
    break;

  case 266:
#line 1313 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5373 "ds_parser.cpp"
    break;

  case 267:
#line 1317 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5381 "ds_parser.cpp"
    break;

  case 268:
#line 1320 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5391 "ds_parser.cpp"
    break;

  case 269:
#line 1325 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5401 "ds_parser.cpp"
    break;

  case 270:
#line 1330 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5413 "ds_parser.cpp"
    break;

  case 271:
#line 1337 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5425 "ds_parser.cpp"
    break;

  case 272:
#line 1347 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5433 "ds_parser.cpp"
    break;

  case 273:
#line 1350 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5442 "ds_parser.cpp"
    break;

  case 274:
#line 1357 "ds_parser.ypp"
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
                        pVar->init_via_move = pDecl->init_via_move;
                        pVar->init_via_clone = pDecl->init_via_clone;
                    }
                    if ( (yyvsp[-3].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5473 "ds_parser.cpp"
    break;

  case 275:
#line 1386 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5481 "ds_parser.cpp"
    break;

  case 276:
#line 1389 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5495 "ds_parser.cpp"
    break;

  case 277:
#line 1398 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5509 "ds_parser.cpp"
    break;

  case 278:
#line 1410 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5523 "ds_parser.cpp"
    break;

  case 279:
#line 1422 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-4]));
        pEnum->name = *(yyvsp[-3].s); 
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-3].s);
    }
#line 5539 "ds_parser.cpp"
    break;

  case 280:
#line 1436 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5545 "ds_parser.cpp"
    break;

  case 281:
#line 1437 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5551 "ds_parser.cpp"
    break;

  case 282:
#line 1441 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
                pStruct->annotations.clear();
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
#line 5592 "ds_parser.cpp"
    break;

  case 283:
#line 1480 "ds_parser.ypp"
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
                            pStruct->fields.emplace_back(name, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, pDecl->at);
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
                for ( auto pA : *(yyvsp[-5].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror("can't 'touch' with structure annotation\n" + err,
                                    tokAt((yylsp[-4])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-5].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure type annotation", tokAt((yylsp[-4])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-4])),
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
				swap ( pStruct->annotations, *(yyvsp[-5].faList) );
                delete (yyvsp[-5].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5660 "ds_parser.cpp"
    break;

  case 284:
#line 1546 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5672 "ds_parser.cpp"
    break;

  case 285:
#line 1553 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5683 "ds_parser.cpp"
    break;

  case 286:
#line 1562 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 5689 "ds_parser.cpp"
    break;

  case 287:
#line 1563 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 5695 "ds_parser.cpp"
    break;

  case 288:
#line 1564 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 5701 "ds_parser.cpp"
    break;

  case 289:
#line 1565 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 5707 "ds_parser.cpp"
    break;

  case 290:
#line 1566 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 5713 "ds_parser.cpp"
    break;

  case 291:
#line 1567 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 5719 "ds_parser.cpp"
    break;

  case 292:
#line 1568 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 5725 "ds_parser.cpp"
    break;

  case 293:
#line 1569 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 5731 "ds_parser.cpp"
    break;

  case 294:
#line 1570 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 5737 "ds_parser.cpp"
    break;

  case 295:
#line 1571 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 5743 "ds_parser.cpp"
    break;

  case 296:
#line 1572 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 5749 "ds_parser.cpp"
    break;

  case 297:
#line 1573 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 5755 "ds_parser.cpp"
    break;

  case 298:
#line 1574 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 5761 "ds_parser.cpp"
    break;

  case 299:
#line 1575 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 5767 "ds_parser.cpp"
    break;

  case 300:
#line 1576 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 5773 "ds_parser.cpp"
    break;

  case 301:
#line 1577 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 5779 "ds_parser.cpp"
    break;

  case 302:
#line 1578 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 5785 "ds_parser.cpp"
    break;

  case 303:
#line 1579 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 5791 "ds_parser.cpp"
    break;

  case 304:
#line 1580 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 5797 "ds_parser.cpp"
    break;

  case 305:
#line 1581 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 5803 "ds_parser.cpp"
    break;

  case 306:
#line 1582 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 5809 "ds_parser.cpp"
    break;

  case 307:
#line 1583 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 5815 "ds_parser.cpp"
    break;

  case 308:
#line 1584 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 5821 "ds_parser.cpp"
    break;

  case 309:
#line 1585 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 5827 "ds_parser.cpp"
    break;

  case 310:
#line 1589 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5840 "ds_parser.cpp"
    break;

  case 311:
#line 1600 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5849 "ds_parser.cpp"
    break;

  case 312:
#line 1604 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5861 "ds_parser.cpp"
    break;

  case 313:
#line 1614 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5867 "ds_parser.cpp"
    break;

  case 314:
#line 1615 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5873 "ds_parser.cpp"
    break;

  case 315:
#line 1616 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5879 "ds_parser.cpp"
    break;

  case 316:
#line 1617 "ds_parser.ypp"
    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don't get extra infer pass on every array
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
#line 5898 "ds_parser.cpp"
    break;

  case 317:
#line 1631 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5909 "ds_parser.cpp"
    break;

  case 318:
#line 1637 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5918 "ds_parser.cpp"
    break;

  case 319:
#line 1641 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5928 "ds_parser.cpp"
    break;

  case 320:
#line 1646 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5938 "ds_parser.cpp"
    break;

  case 321:
#line 1651 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5948 "ds_parser.cpp"
    break;

  case 322:
#line 1656 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5960 "ds_parser.cpp"
    break;

  case 323:
#line 1663 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5970 "ds_parser.cpp"
    break;

  case 324:
#line 1668 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 5979 "ds_parser.cpp"
    break;

  case 325:
#line 1672 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 5988 "ds_parser.cpp"
    break;

  case 326:
#line 1676 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5998 "ds_parser.cpp"
    break;

  case 327:
#line 1681 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6008 "ds_parser.cpp"
    break;

  case 328:
#line 1686 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6020 "ds_parser.cpp"
    break;

  case 329:
#line 1693 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6030 "ds_parser.cpp"
    break;

  case 330:
#line 1698 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6041 "ds_parser.cpp"
    break;

  case 331:
#line 1704 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6051 "ds_parser.cpp"
    break;

  case 332:
#line 1709 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6060 "ds_parser.cpp"
    break;

  case 333:
#line 1713 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6070 "ds_parser.cpp"
    break;

  case 334:
#line 1718 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6084 "ds_parser.cpp"
    break;

  case 335:
#line 1727 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6093 "ds_parser.cpp"
    break;

  case 336:
#line 1731 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6103 "ds_parser.cpp"
    break;

  case 337:
#line 1736 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6117 "ds_parser.cpp"
    break;

  case 338:
#line 1745 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6126 "ds_parser.cpp"
    break;

  case 339:
#line 1749 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6136 "ds_parser.cpp"
    break;

  case 340:
#line 1754 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6150 "ds_parser.cpp"
    break;

  case 341:
#line 1763 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6161 "ds_parser.cpp"
    break;

  case 342:
#line 1772 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6167 "ds_parser.cpp"
    break;

  case 343:
#line 1773 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6173 "ds_parser.cpp"
    break;

  case 344:
#line 1774 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6179 "ds_parser.cpp"
    break;

  case 345:
#line 1775 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6185 "ds_parser.cpp"
    break;

  case 346:
#line 1779 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6197 "ds_parser.cpp"
    break;

  case 347:
#line 1786 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6208 "ds_parser.cpp"
    break;

  case 348:
#line 1795 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6218 "ds_parser.cpp"
    break;

  case 349:
#line 1800 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6227 "ds_parser.cpp"
    break;

  case 350:
#line 1807 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6237 "ds_parser.cpp"
    break;

  case 351:
#line 1812 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6248 "ds_parser.cpp"
    break;

  case 352:
#line 1818 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6260 "ds_parser.cpp"
    break;

  case 353:
#line 1825 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6273 "ds_parser.cpp"
    break;

  case 354:
#line 1836 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6281 "ds_parser.cpp"
    break;

  case 355:
#line 1839 "ds_parser.ypp"
    {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt((yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6297 "ds_parser.cpp"
    break;

  case 356:
#line 1853 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6308 "ds_parser.cpp"
    break;

  case 357:
#line 1862 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6314 "ds_parser.cpp"
    break;

  case 358:
#line 1863 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6320 "ds_parser.cpp"
    break;

  case 359:
#line 1867 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6330 "ds_parser.cpp"
    break;

  case 360:
#line 1872 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6339 "ds_parser.cpp"
    break;

  case 361:
#line 1879 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6349 "ds_parser.cpp"
    break;

  case 362:
#line 1884 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6362 "ds_parser.cpp"
    break;

  case 363:
#line 1895 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6372 "ds_parser.cpp"
    break;

  case 364:
#line 1900 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6381 "ds_parser.cpp"
    break;

  case 365:
#line 1907 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6395 "ds_parser.cpp"
    break;

  case 366:
#line 1919 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6401 "ds_parser.cpp"
    break;

  case 367:
#line 1920 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6407 "ds_parser.cpp"
    break;

  case 368:
#line 1924 "ds_parser.ypp"
    {
        auto pFor = make_shared<ExprFor>(tokAt((yylsp[-8])));
        pFor->iterators = *(yyvsp[-7].pNameList);
        delete (yyvsp[-7].pNameList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt((yylsp[-8])));
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 6425 "ds_parser.cpp"
    break;


#line 6429 "ds_parser.cpp"

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 1939 "ds_parser.ypp"


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    // printf("ERROR %s\n", error.c_str());
    g_Program->error(error,at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
    // printf("ERROR %s\n", error.c_str());
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
            int count = pDecl->pNameList ? int(pDecl->pNameList->size()) : 1;
            for ( int ai=0; ai!=count; ++ai ) {
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


