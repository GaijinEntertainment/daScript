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
#define YYLAST   4794

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  94
/* YYNRULES -- Number of rules.  */
#define YYNRULES  369
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  660

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
     578,   582,   587,   595,   596,   600,   607,   619,   623,   630,
     631,   635,   636,   637,   641,   645,   652,   656,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   691,   695,   765,   768,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   797,   801,   807,   813,   827,   828,   834,   838,
     844,   847,   850,   854,   860,   864,   868,   871,   874,   879,
     882,   890,   896,   897,   901,   902,   906,   939,   942,   947,
     955,   959,   964,   968,   976,   979,   985,   986,   996,  1001,
    1053,  1054,  1055,  1056,  1057,  1058,  1062,  1063,  1064,  1065,
    1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1077,  1081,  1092,  1097,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,  1145,  1149,  1153,  1156,  1159,  1160,  1161,
    1162,  1163,  1164,  1165,  1168,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,  1187,  1197,  1231,  1232,  1236,  1237,  1241,  1249,
    1252,  1259,  1260,  1264,  1265,  1269,  1275,  1276,  1280,  1281,
    1285,  1290,  1293,  1297,  1303,  1312,  1313,  1314,  1318,  1321,
    1326,  1331,  1338,  1348,  1351,  1358,  1387,  1390,  1399,  1411,
    1423,  1437,  1438,  1442,  1481,  1547,  1554,  1563,  1564,  1565,
    1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,
    1576,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,
    1586,  1590,  1601,  1605,  1615,  1616,  1617,  1618,  1632,  1638,
    1642,  1647,  1652,  1657,  1664,  1669,  1673,  1677,  1682,  1687,
    1694,  1699,  1705,  1710,  1714,  1719,  1728,  1732,  1737,  1746,
    1750,  1755,  1764,  1773,  1774,  1775,  1776,  1780,  1787,  1796,
    1801,  1808,  1813,  1819,  1826,  1837,  1840,  1854,  1863,  1864,
    1868,  1873,  1880,  1885,  1896,  1901,  1908,  1920,  1921,  1925
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

#define YYPACT_NINF -447

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-447)))

#define YYTABLE_NINF -167

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -447,    21,  -447,  -447,   -77,   -38,   -37,   -20,   -16,  -447,
      -8,    25,  -447,  -447,  -447,  -447,  -447,    37,  -447,     5,
    -447,  -447,  -447,  -447,   -39,  -447,    44,    68,    79,  -447,
    -447,    18,  -447,    60,    84,    64,  -447,    83,   103,    77,
    -447,   -72,  -447,   114,    -5,  -447,    61,   -77,   135,   -38,
    -447,   125,   134,  -447,  -447,  3389,  -447,   136,  -447,   -77,
      25,  -447,   129,   104,  4648,   237,   238,  -447,   115,  -102,
    -447,  -447,  -447,  -447,  -447,   148,  -447,  -447,  -447,  -447,
    -447,  -447,   -95,   140,   142,   143,  -447,  -447,  -447,   132,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,   144,  -447,  -447,  -447,   156,   162,
    -447,  -447,  -447,  -447,   169,  -447,  -447,  -447,  -447,  4567,
    -447,   -91,  -447,   -86,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,    -7,   176,  -447,  -447,  -447,   117,  -447,   141,
     -74,  -447,  3389,  3389,  3389,   187,  3319,  3319,  3319,  3389,
    -447,    -6,  -447,  -447,  -447,  -447,  1683,  -447,  -447,  -447,
     -41,   191,  -447,  -447,    54,  -447,   242,  3389,   159,  -447,
    -447,   193,  -447,  3389,  2723,  -447,  -447,  2723,  -447,  1045,
     521,  3800,   164,   176,  3941,   176,  4440,   176,  4441,  -447,
      13,  4567,   198,  -447,   168,  -447,  -447,  -447,  3249,   179,
    -447,   180,   201,   202,   186,   210,   192,  2723,  2723,   656,
    3389,  2723,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  2723,
    2723,  2723,   -86,  2723,  2723,  -447,  2723,  -447,   195,  -447,
    -447,   -92,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  3787,
     199,  -447,  -447,  -447,  -447,  -447,   -77,  -447,   313,   196,
    -447,  -447,   -14,  -447,  -447,  3389,  2723,  4567,  -447,  -447,
     399,  -447,   250,  2803,   190,  -447,  3389,  -447,  -447,   220,
    -447,   221,  -447,   229,  -447,  -447,  3389,  -447,  -447,   428,
    -447,   245,  2723,  3389,  3389,  2723,  3389,  1819,   258,   258,
     191,  1072,  1208,  4078,  -447,    -3,   -65,   258,   258,  -447,
    -447,   258,   258,  3387,   761,   115,  -447,  1321,   358,  2723,
    2723,  -447,  -447,  2723,  2723,  2723,  2723,   253,  2723,   254,
    2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,
    2723,  2723,  2723,  2723,  2723,   255,  2723,  -447,  1932,   -56,
    -447,   191,  -447,  -447,   311,  -447,  3928,  -447,  2723,    55,
    2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,  2723,
    2723,  2723,  2723,  2723,  2723,  -447,  -447,  4479,  -447,  -447,
    -447,  -447,  2045,  1434,  3457,  4492,  4504,  3523,  4527,  -447,
    3589,    15,   -78,  2158,  4078,   251,   -40,   265,  -447,  -447,
      52,  2271,    63,    74,  2723,  2723,   218,  -447,  2723,   259,
    -447,  -447,  2723,  2723,   191,  1570,  -447,   159,  2723,  2723,
    -447,   228,   278,  2384,  -447,   364,   241,   247,  -447,  -447,
    -447,  -447,  -447,  -447,   256,   257,   260,  -447,  -447,   386,
    -447,   -81,  4022,   176,   286,  -447,   -89,  4270,  3389,  2865,
    2865,  4413,  4413,   298,   298,   266,   258,  -447,   -69,   -69,
    2865,  2865,  4146,  4326,  4347,     1,  4413,  4413,    98,    98,
     208,   208,   208,  -447,  3851,  -447,   -75,  -447,  -447,  2723,
    -447,  2890,  -447,  4270,  4270,  4270,  4270,  4270,  4270,  4270,
    4270,  4270,  4270,  4270,  4270,  4270,  4270,  4270,  -447,  -447,
     -32,  3389,   289,  3655,  -447,  2723,  2723,  -447,  2723,  -447,
    2723,  2723,   286,   297,   268,   286,  2723,   269,  2723,   286,
     270,   271,  4270,  -447,  -447,  4202,   509,   509,    16,  2723,
    -447,  -447,  2977,   415,  4270,   509,  -447,   299,   315,  4270,
     283,  -447,  -447,  -447,  -447,  -447,   394,   191,  -447,   -83,
     -54,    22,  2723,  -447,  4567,  2497,  2723,  -447,  -447,  4270,
    -447,  -447,  4530,   308,  -447,  4413,  4413,  4413,   -90,  4270,
      53,   -54,  -447,   251,  4270,  -447,  -447,    99,  -447,  -447,
    -447,  -447,   184,  2723,  -447,  3064,  -447,   159,  -447,  -447,
    -447,  -447,  -447,  -447,  2723,  -447,  -447,   292,  4270,  -447,
       4,  4270,  -447,  2610,  2723,   301,  2723,   302,   159,  2723,
    -447,   -88,  -447,  -447,   922,  4270,  -447,  -447,  3389,  3721,
    3127,  -447,  4270,  -447,  -447,   509,   159,  -447,  4556,  -447,
     413,   306,   184,  -447,  -447,  2723,   307,  -447,  4270,  -447
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    59,     1,   144,     0,     0,     0,     0,     0,   145,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,    50,    51,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    54,   126,     0,    55,
      57,     0,    53,     0,     0,   273,     0,     0,     0,     0,
      28,     0,     0,    26,   276,     0,    13,     0,   128,     0,
       0,    60,   281,     0,     0,     0,     0,    66,    61,     0,
      48,    49,    46,    47,    45,     0,    44,    52,    33,    31,
      25,    24,     0,     0,     0,     0,   287,   307,   288,   312,
     289,   293,   294,   295,   296,   300,   301,   302,   303,   304,
     305,   306,   308,   309,   333,   292,   299,   310,   336,   339,
     290,   297,   291,   298,     0,   311,   314,   316,   315,   279,
     127,     0,    58,     0,   283,   249,    69,    70,    72,    71,
      73,    74,    75,    76,    95,    96,    93,    94,    86,    97,
      98,    87,    84,    85,    99,   100,   101,   102,    89,    90,
      88,    82,    83,    78,    77,    79,    80,    81,    68,    67,
      91,    92,     0,    64,   285,   275,   274,     0,    14,     0,
       0,   280,     0,     0,     0,     0,    61,    61,    61,     0,
     320,     0,   326,   329,   328,   322,     0,   325,    56,   282,
     244,     0,    62,   253,     0,   251,   260,     0,     0,   266,
     267,     0,   265,     0,     0,    15,    16,     0,   277,     0,
       0,     0,     0,    64,     0,    64,     0,    64,     0,   256,
       0,   255,   321,   324,     0,   327,   190,   191,     0,     0,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   160,   162,   161,   163,   164,   165,    17,     0,
       0,     0,     0,     0,     0,   318,     0,   122,    59,   188,
     158,   186,   237,   236,   235,   240,   187,   239,   238,     0,
       0,   189,   343,   344,   345,   346,     0,   284,   246,     0,
     252,    63,     0,   259,   258,     0,     0,    65,   103,   286,
       0,   272,     0,   166,     0,   330,     0,   332,   313,     0,
     334,     0,   337,     0,   340,   342,     0,   323,   319,   130,
     133,     0,     0,     0,     0,     0,     0,     0,   214,   215,
       0,     0,     0,     0,   364,     0,     0,   195,   194,   227,
     222,   193,   192,     0,     0,    61,   243,     0,     0,     0,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,     0,
     247,     0,   250,   254,   261,   264,   263,   268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   271,   278,     0,   335,   338,
     341,   257,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,   126,     0,   355,   349,     0,   358,   359,   360,
       0,     0,     0,     0,     0,     0,     0,    20,     0,    18,
     218,   124,     0,     0,     0,     0,   134,     0,     0,     0,
     135,     0,     0,     0,   106,   104,     0,     0,   117,   113,
     111,   112,   123,   107,     0,     0,     0,   116,   118,   142,
     110,     0,   166,    64,     0,   223,     0,   154,     0,   196,
     197,   209,   210,   207,   208,     0,   232,   221,   241,   242,
     198,   199,     0,   212,   213,   211,   205,   206,   201,   200,
     202,   203,   204,   220,     0,   225,     0,   245,   248,     0,
     270,   166,   125,   173,   174,   176,   175,   177,   170,   171,
     172,   178,   179,   168,   169,   180,   181,   167,   331,   131,
       0,     0,     0,     0,   228,     0,     0,   229,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   357,   365,   366,     0,     0,     0,     0,     0,
     136,   139,   166,     0,   129,     0,   121,     0,     0,    36,
       0,   119,   120,   109,   114,   115,     0,     0,   108,     0,
       0,     0,     0,   224,   234,     0,     0,   219,   226,   262,
     269,   132,     0,     0,   150,   147,   148,   149,     0,   347,
       0,     0,   351,   350,   356,   362,   361,     0,   353,   363,
      19,    42,    37,     0,   140,   166,   137,     0,    43,    34,
      35,   122,   143,   146,     0,   156,   159,     0,   155,   183,
       0,   233,   152,     0,     0,     0,     0,     0,     0,     0,
      40,     0,   138,   141,     0,   157,   182,   184,     0,     0,
     367,   352,   348,   354,    38,     0,     0,   105,     0,   151,
       0,     0,    37,    41,   153,     0,     0,    39,   368,   369
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -447,  -447,  -447,   138,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,   417,  -447,  -447,  -190,  -447,  -447,
    -447,  -447,   420,   -46,  -447,   408,  -447,   214,  -161,  -208,
    -447,  -447,    -9,  -447,  -115,  -200,   -11,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,   497,  -447,  -447,  -447,  -360,  -447,
     123,  -447,  -328,  -447,  -447,   131,  -447,  -447,  -447,  -447,
     222,  -447,   197,  -447,  -194,  -177,   215,   -60,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,   -68,   -52,  -447,  -447,
     -43,   280,  -446,  -312,  -447,  -447,  -230,   -12,   209,  -447,
    -447,  -447,  -447,  -447
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,    76,   326,   259,    14,    15,    31,
      32,    53,    16,    28,    29,   446,   447,   630,   448,   449,
     450,   451,    25,    26,    39,    40,    41,    17,   163,   198,
      68,    18,   260,   452,   334,   453,   261,   454,   262,   455,
     456,   457,   458,   567,   459,   460,   263,   264,   466,   616,
     265,   266,   292,   267,   268,   467,   278,   371,   279,   190,
     193,   194,   219,   220,   531,   195,   204,   166,    69,    20,
      82,    21,    22,   124,    63,    23,   196,   270,   117,   118,
     221,   271,   415,   416,   272,   417,   418,   419,   420,   273,
     325,   274,   651,   275
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   167,   286,   116,   291,   299,   461,   301,   496,   303,
     422,   324,   119,   121,   280,   213,   215,   217,   571,   338,
     164,     2,   222,   283,   379,    64,     3,   170,   191,   614,
     336,   347,   530,   603,   349,   191,    37,    47,   572,   572,
     572,    43,   520,    44,   115,    24,     4,   283,     5,    42,
       6,   284,     7,   572,    36,   207,    60,     8,   165,   168,
     427,   428,   337,     9,   188,   171,   573,    50,   624,    10,
     646,    38,    47,    70,    71,   284,   257,   568,    27,    57,
     578,    61,   365,   366,   208,    30,   375,    65,    66,   593,
      46,   339,   340,   341,   342,   343,   572,   497,   344,   345,
     346,   347,    33,   348,   349,   350,    34,   351,   164,   352,
     353,   276,   189,   534,    35,   164,   338,    67,   535,   277,
     116,   116,   116,   581,   116,   116,   116,   116,   223,   209,
     210,   211,   572,   214,   216,   218,   358,   359,   360,   361,
     362,   363,   364,   201,   201,   116,   224,    37,   192,   305,
     533,   116,   365,   366,   287,   425,   225,   426,    51,   637,
     290,   115,   115,   115,    45,   115,   115,   115,   115,    52,
     588,   306,    47,    11,   463,   617,   116,    72,   500,    12,
     499,    73,    38,    74,    75,   309,   115,   116,   116,   288,
     341,   342,   115,   628,   498,   543,   321,   322,   347,    48,
     348,   349,   350,   629,   351,   537,   625,    49,   338,   281,
     538,   535,   282,    55,   257,   620,   258,   115,   199,    54,
     590,   535,    56,   540,   200,    58,   338,   597,   115,   115,
     369,    59,   538,   116,   541,   551,    62,   362,   363,   364,
      57,   330,   374,   631,   116,   201,   202,    80,   203,   365,
     366,    78,   411,   397,   116,   569,    81,   535,   120,   627,
     123,   116,   116,   125,   116,   205,   206,   160,   161,   162,
     405,   406,   168,   408,   115,   172,   338,   173,   174,   176,
     339,   340,   341,   342,   343,   115,   175,   344,   345,   346,
     347,   177,   348,   349,   350,   115,   351,   178,   352,   353,
     341,   342,   115,   115,   179,   115,   461,   197,   347,   212,
     348,   349,   350,   164,   351,   289,   338,   269,   257,   298,
     354,   308,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   307,   311,   312,   293,   313,   314,   294,   180,
     315,   365,   366,   283,   181,   316,   317,    11,   396,   604,
     341,   342,   370,   368,   372,   379,   398,   399,   347,   365,
     366,   349,   350,   182,   351,   400,   548,   403,   318,   319,
     201,   284,   323,   285,   468,   475,   477,   493,   544,   533,
     327,   328,   329,   205,   331,   332,   556,   333,   339,   340,
     341,   342,   343,   536,   557,   344,   560,   626,   347,   561,
     348,   349,   350,   566,   351,   562,   352,   353,   570,   365,
     366,   583,   283,   183,   563,   564,   116,   376,   565,   591,
     575,   592,   595,   598,   599,   574,   607,   180,   553,   612,
     609,   610,   181,   358,   359,   360,   361,   362,   363,   364,
     284,   184,   611,   404,   623,   185,   407,   636,   410,   365,
     366,   182,   414,   414,   641,   643,   180,   115,   655,   656,
     659,   181,   657,   186,   429,   462,    79,    77,   122,   116,
     469,   470,   335,   187,   471,   472,   473,   474,   582,   476,
     182,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   634,   494,    19,   167,
     199,   183,   502,   401,   373,   378,   200,   613,   310,   501,
     115,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   596,   338,   202,   184,
     183,   423,     0,   185,   523,     0,     0,   601,   602,     0,
       0,     0,     0,     0,   333,     0,   608,     0,     0,   180,
       0,   186,   333,     0,   181,   542,   323,   377,   184,   545,
     615,   187,   185,   546,   547,     0,   552,     0,     0,   554,
     555,     0,     0,   182,   559,     0,     0,     0,     0,     0,
     186,     0,   402,     0,     0,     0,   116,     0,     0,     0,
     187,     0,     0,     0,     0,   648,     0,     0,   633,   339,
     340,   341,   342,   343,     0,     0,   344,   345,   346,   347,
       0,   348,   349,   350,     0,   351,     0,   352,   353,   644,
       0,     0,     0,   183,     0,     0,     0,   115,     0,     0,
     579,     0,     0,     0,     0,     0,   652,   653,     0,   354,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   184,     0,     0,     0,   185,   585,   586,     0,   587,
     365,   366,   589,     0,     0,     0,   320,   594,   257,   414,
       0,     0,     0,   186,     0,     0,    83,     0,     0,   296,
     605,     0,    84,   187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,   618,     0,     0,     0,   621,     0,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,     0,
       0,     0,     0,     0,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,   639,   640,     0,   642,     0,     0,
     645,     0,   431,     0,     0,   462,     3,     0,   432,   433,
       0,   434,     0,   226,   227,   228,   229,     0,    37,     0,
       0,     0,   435,   230,   436,   437,   658,     0,     0,     0,
       0,     0,     0,     0,   438,   231,     0,     0,   439,   232,
       0,   233,     0,     9,   234,   440,     0,   441,   235,     0,
       0,   442,   443,    38,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   236,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,   240,   241,   242,   243,   244,
     245,   246,   247,    37,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
     251,     0,     0,   252,     0,     0,   253,   254,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,    38,   444,
     257,   445,   258,   431,     0,     0,     0,     3,     0,   432,
     433,     0,   434,     0,   226,   227,   228,   229,     0,     0,
       0,     0,     0,   435,   230,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   231,     0,     0,   439,
     232,     0,   233,     0,     9,   234,   440,     0,   441,   235,
       0,     0,   442,   443,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   236,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   240,   241,   242,   243,
     244,   245,   246,   247,    37,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,   251,     0,     0,   252,     0,     0,   253,   254,     0,
       0,     0,     0,   180,     0,     0,   256,     0,   181,    38,
     444,   257,   647,   258,   226,   227,   228,   229,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   182,     0,     0,
     180,     0,     0,     0,     0,   181,   231,     0,     0,     0,
     232,     0,   233,     0,     0,   234,     0,     0,     0,   235,
       0,     0,     0,     0,   182,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,   183,   236,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   238,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   184,     0,     0,     0,   185,
       0,   295,     0,     0,     0,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   412,   248,     0,   186,     0,     0,
       0,     0,   184,     0,     0,     0,   185,   187,     0,   249,
     250,   251,     0,     0,   252,     0,     0,   253,   254,     0,
     226,   227,   228,   229,   186,     0,   413,     0,     0,    38,
     230,   257,     0,   258,   187,     0,   180,     0,     0,     0,
       0,   181,   231,     0,     0,     0,   232,     0,   233,     0,
       0,   234,     0,     0,     0,   235,     0,     0,     0,     0,
     182,    86,    87,    88,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
     105,   106,   107,     0,   236,   110,   111,   112,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,   238,     0,     0,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     412,   248,     0,   226,   227,   228,   229,     0,   184,     0,
       0,     0,   185,   230,     0,   249,   250,   251,     0,     0,
     252,     0,     0,   253,   254,   231,     0,     0,     0,   232,
     186,   233,   421,     0,   234,    38,     0,   257,   235,   258,
     187,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   236,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,   240,   241,   242,   243,   244,
     245,   246,   247,    37,   248,     0,   226,   227,   228,   229,
     521,     0,     0,     0,     0,     0,   230,     0,   249,   250,
     251,     0,     0,   252,     0,     0,   253,   254,   231,     0,
       0,     0,   232,   464,   233,   256,   465,   234,    38,     0,
     257,   235,   258,     0,     0,     0,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     236,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,   241,
     242,   243,   244,   245,   246,   247,    37,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   522,
       0,   249,   250,   251,     0,     0,   252,     0,     0,   253,
     254,     0,   226,   227,   228,   229,     0,     0,   256,     0,
       0,    38,   230,   257,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
     233,     0,     0,   234,     0,     0,     0,   235,     0,     0,
       0,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   236,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,   238,     0,     0,     0,     0,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,   240,   241,   242,   243,   244,   245,
     246,   247,    37,   248,     0,   226,   227,   228,   229,     0,
       0,     0,     0,     0,     0,   230,     0,   249,   250,   251,
       0,     0,   252,     0,     0,   253,   254,   231,     0,     0,
       0,   232,     0,   233,   256,     0,   234,    38,   550,   257,
     235,   258,     0,     0,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   236,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   240,   241,   242,
     243,   244,   245,   246,   247,    37,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,     0,     0,   252,     0,     0,   253,   254,
       0,   226,   227,   228,   229,     0,   255,   256,     0,     0,
      38,   230,   257,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,   233,
       0,     0,   234,     0,     0,     0,   235,     0,     0,     0,
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
     232,     0,   233,   256,   409,   234,    38,     0,   257,   235,
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
       0,     0,     0,   232,     0,   233,   256,   495,   234,    38,
       0,   257,   235,   258,     0,     0,     0,     0,    86,    87,
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
     519,   234,    38,     0,   257,   235,   258,     0,     0,     0,
       0,    86,    87,    88,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
     105,   106,   107,     0,   236,   110,   111,   112,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   240,   241,   242,   243,   244,   245,   246,   247,
      37,   248,     0,   226,   227,   228,   229,     0,     0,     0,
       0,     0,     0,   230,     0,   249,   250,   251,     0,     0,
     252,     0,     0,   253,   254,   231,     0,     0,     0,   232,
       0,   233,   256,   532,   234,    38,     0,   257,   235,   258,
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
       0,     0,   232,     0,   233,   256,   539,   234,    38,     0,
     257,   235,   258,     0,   558,     0,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     236,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,   241,
     242,   243,   244,   245,   246,   247,    37,   248,     0,   226,
     227,   228,   229,     0,     0,     0,     0,     0,     0,   230,
       0,   249,   250,   251,     0,     0,   252,     0,     0,   253,
     254,   231,     0,     0,     0,   232,     0,   233,   256,     0,
     234,    38,     0,   257,   235,   258,     0,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   236,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,   240,   241,   242,   243,   244,   245,   246,   247,    37,
     248,     0,   226,   227,   228,   229,   638,     0,     0,     0,
       0,     0,   230,     0,   249,   250,   251,     0,     0,   252,
       0,     0,   253,   254,   231,     0,     0,     0,   232,     0,
     233,   256,   619,   234,    38,     0,   257,   235,   258,     0,
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
       0,   232,     0,   233,   256,     0,   234,    38,     0,   257,
     235,   258,     0,     0,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   236,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,     0,     0,
       0,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   240,   241,   242,
     243,   244,   245,   246,   247,    37,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,     0,     0,   252,     0,     0,   253,   254,
       0,     0,     0,     0,     0,     0,     0,   256,     0,     0,
      38,     0,   257,   338,   258,   380,   381,   382,   383,   384,
     385,   386,   387,   339,   340,   341,   342,   343,   388,   389,
     344,   345,   346,   347,   390,   348,   349,   350,   338,   351,
     391,   352,   353,   392,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,   354,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,   366,     0,   341,   342,     0,
       0,   395,     0,     0,     0,   347,     0,   348,   349,   350,
       0,   351,   380,   381,   382,   383,   384,   385,   386,   387,
     339,   340,   341,   342,   343,   388,   389,   344,   345,   346,
     347,   390,   348,   349,   350,   338,   351,   391,   352,   353,
     392,   393,   360,   361,   362,   363,   364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,   366,     0,   394,
     354,     0,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,   366,     0,     0,     0,     0,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   339,   340,   341,
     342,   343,   388,   389,   344,   345,   346,   347,   390,   348,
     349,   350,   338,   351,   391,   352,   353,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   394,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,   366,
       0,     0,     0,     0,     0,   606,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   380,   381,   382,   383,
     384,   385,   386,   387,   339,   340,   341,   342,   343,   388,
     389,   344,   345,   346,   347,   390,   348,   349,   350,     0,
     351,   391,   352,   353,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,   354,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   366,   339,   340,   341,
     342,   343,   632,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,    83,
       0,     0,     0,     0,     0,    84,     0,     0,   365,   366,
       0,     0,     0,     0,     0,   650,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,     0,   239,   240,   241,     0,     0,     0,     0,     0,
       0,    37,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,     0,   338,    38,     0,     0,    83,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,   162,     0,   338,    38,   339,   340,   341,
     342,   343,     0,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,   366,
       0,   338,   430,     0,     0,     0,    38,   339,   340,   341,
     342,   343,     0,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   365,   366,
       0,     0,   524,   339,   340,   341,   342,   343,     0,     0,
     344,   345,   346,   347,     0,   348,   349,   350,     0,   351,
       0,   352,   353,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   365,   366,     0,     0,   527,   339,
     340,   341,   342,   343,     0,     0,   344,   345,   346,   347,
       0,   348,   349,   350,     0,   351,     0,   352,   353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   354,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     365,   366,     0,     0,   529,   339,   340,   341,   342,   343,
       0,     0,   344,   345,   346,   347,     0,   348,   349,   350,
       0,   351,     0,   352,   353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   354,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   365,   366,     0,     0,
     584,   339,   340,   341,   342,   343,     0,     0,   344,   345,
     346,   347,     0,   348,   349,   350,     0,   351,   180,   352,
     353,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   354,   182,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,     0,     0,     0,   338,
       0,     0,   365,   366,     0,     0,   649,   339,   340,   341,
     342,   343,     0,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
     184,     0,     0,     0,   185,     0,   297,     0,   365,   366,
     367,   339,   340,   341,   342,   343,   338,     0,   344,   345,
     346,   347,   186,   348,   349,   350,     0,   351,     0,   352,
     353,     0,   187,     0,     0,     0,     0,     0,     0,   180,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,   354,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   182,     0,     0,     0,     0,     0,     0,
       0,     0,   365,   366,   577,     0,     0,     0,     0,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   339,   340,
     341,   342,   343,   388,   389,   344,   345,   346,   347,   390,
     348,   349,   350,  -166,   351,   391,   352,   353,   392,   393,
     338,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   354,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
       0,   184,     0,     0,     0,   185,     0,   300,     0,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,     0,   338,     0,     0,     0,
       0,     0,     0,   187,   380,   381,   382,   383,   384,   385,
     386,   387,   339,   340,   341,   342,   343,   388,   389,   344,
     345,   346,   347,   390,   348,   349,   350,     0,   351,   391,
     352,   353,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,   354,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   338,     0,     0,     0,   339,   340,
     341,   342,   343,   365,   366,   344,   345,   346,   347,     0,
     348,   349,   350,     0,   351,     0,   352,   353,     0,     0,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   354,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     338,     0,     0,     0,     0,     0,     0,     0,     0,   365,
     366,     0,     0,     0,     0,     0,   339,   340,   341,   342,
     343,     0,     0,   344,   345,   346,   347,     0,   348,   349,
     350,     0,   351,     0,   352,   353,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   354,   576,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   338,     0,
       0,     0,   339,   340,   341,   342,   343,   365,   366,   344,
     345,   346,   347,     0,   348,   349,   350,     0,   351,     0,
     352,   353,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   354,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   338,     0,     0,     0,     0,     0,
       0,     0,     0,   365,   366,     0,     0,     0,     0,     0,
     339,   340,   341,   342,   343,   338,     0,   344,   345,   346,
     347,     0,   348,   349,   350,     0,   351,     0,   352,   353,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     354,     0,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,     0,     0,     0,     0,   339,   340,   341,   342,
     343,   365,   366,   344,   345,   346,   347,     0,   348,   349,
     350,   338,   351,     0,   352,   353,     0,   339,   340,   341,
     342,   343,     0,     0,   344,   345,   346,   347,     0,   348,
     349,   350,     0,   351,     0,   352,   353,     0,     0,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   180,   180,
       0,     0,     0,   181,   181,     0,     0,   365,   366,     0,
       0,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,   182,   182,     0,     0,     0,     0,   365,   366,
       0,     0,     0,   339,   340,   341,   342,   180,     0,     0,
       0,     0,   181,   347,     0,   348,   349,   350,     0,   351,
     180,   352,   353,     0,     0,   181,     0,     0,     0,     0,
       0,   182,   180,     0,     0,     0,     0,   181,     0,     0,
       0,     0,   183,   183,   182,     0,     0,     0,     0,     0,
     360,   361,   362,   363,   364,   180,   182,     0,   180,     0,
     181,     0,     0,   181,   365,   366,     0,     0,     0,     0,
     184,   184,     0,     0,   185,   185,   302,   304,     0,   182,
       0,   183,   182,     0,   180,     0,     0,     0,     0,   181,
       0,     0,   186,   186,   183,   180,     0,     0,     0,     0,
     181,     0,   187,   187,     0,     0,   183,     0,   182,   184,
       0,     0,     0,   185,     0,   518,     0,     0,     0,   182,
       0,     0,   184,     0,     0,     0,   185,     0,   525,   183,
       0,   186,   183,     0,   184,     0,     0,     0,   185,     0,
     526,   187,     0,     0,   186,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,     0,   186,   184,   183,     0,
     184,   185,     0,   528,   185,     0,   187,     0,     0,   183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
       0,     0,   186,     0,     0,   622,   184,     0,     0,   187,
     185,     0,   187,     0,     0,     0,     0,   184,     0,     0,
       0,   185,     0,     0,     0,     0,     0,     0,   186,     0,
       0,   654,     0,     0,     0,     0,     0,     0,   187,   186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   145,   146,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
       0,     0,     0,   158,   159
};

static const yytype_int16 yycheck[] =
{
      11,    69,   196,    55,   204,   213,   334,   215,   368,   217,
     322,   241,    55,    59,   191,   176,   177,   178,   464,    18,
     122,     0,    28,   101,   105,    30,     5,   122,    42,   112,
     122,   100,    17,    17,   103,    42,   122,   128,   128,   128,
     128,     4,   402,     6,    55,   122,    25,   101,    27,    60,
      29,   129,    31,   128,    29,   129,   128,    36,   160,   124,
     125,   126,   154,    42,   155,   160,   155,    49,   158,    48,
     158,   157,   128,    12,    13,   129,   159,   158,   116,   157,
     155,   153,   151,   152,   158,   122,   286,    92,    93,   535,
     129,    90,    91,    92,    93,    94,   128,   153,    97,    98,
      99,   100,   122,   102,   103,   104,   122,   106,   122,   108,
     109,   152,   123,   153,   122,   122,    18,   122,   158,   160,
     172,   173,   174,   155,   176,   177,   178,   179,   134,   172,
     173,   174,   128,   176,   177,   178,   135,   136,   137,   138,
     139,   140,   141,   128,   128,   197,   152,   122,   155,   136,
     128,   203,   151,   152,   197,   158,   162,   160,   140,   155,
     203,   172,   173,   174,   159,   176,   177,   178,   179,   151,
     530,   158,   128,   152,   335,   153,   228,   116,   378,   158,
     374,   120,   157,   122,   123,   228,   197,   239,   240,   198,
      92,    93,   203,     9,   371,   425,   239,   240,   100,   131,
     102,   103,   104,    19,   106,   153,   153,   128,    18,   155,
     158,   158,   158,   129,   159,   575,   161,   228,   101,   159,
     532,   158,   158,   160,   107,   122,    18,   539,   239,   240,
     276,   154,   158,   285,   160,   435,   122,   139,   140,   141,
     157,   252,   285,   603,   296,   128,   129,   122,   131,   151,
     152,   116,   320,   296,   306,   463,   122,   158,   122,   160,
     131,   313,   314,   159,   316,   124,   125,    30,    30,   154,
     313,   314,   124,   316,   285,   135,    18,   135,   135,   135,
      90,    91,    92,    93,    94,   296,   154,    97,    98,    99,
     100,   135,   102,   103,   104,   306,   106,   135,   108,   109,
      92,    93,   313,   314,   135,   316,   634,   131,   100,   122,
     102,   103,   104,   122,   106,   122,    18,   186,   159,   155,
     130,   153,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   134,   154,   154,   204,   135,   135,   207,    28,
     154,   151,   152,   101,    33,   135,   154,   152,   158,   549,
      92,    93,    39,   154,   158,   105,   136,   136,   100,   151,
     152,   103,   104,    52,   106,   136,   434,   122,   237,   238,
     128,   129,   241,   131,    16,   122,   122,   122,   160,   128,
     249,   250,   251,   124,   253,   254,   158,   256,    90,    91,
      92,    93,    94,   128,   116,    97,    32,   591,   100,   158,
     102,   103,   104,    17,   106,   158,   108,   109,   122,   151,
     152,   122,   101,   102,   158,   158,   468,   286,   158,   122,
     154,   153,   153,   153,   153,   468,    11,    28,   437,    35,
     131,   116,    33,   135,   136,   137,   138,   139,   140,   141,
     129,   130,   159,   312,   136,   134,   315,   155,   317,   151,
     152,    52,   321,   322,   153,   153,    28,   468,    45,   153,
     153,    33,   652,   152,   326,   334,    49,    47,    60,   521,
     339,   340,   258,   162,   343,   344,   345,   346,   521,   348,
      52,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   611,   366,     1,   567,
     101,   102,   379,   306,   282,   290,   107,   567,   228,   378,
     521,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   538,    18,   129,   130,
     102,   322,    -1,   134,   403,    -1,    -1,   546,   547,    -1,
      -1,    -1,    -1,    -1,   413,    -1,   555,    -1,    -1,    28,
      -1,   152,   421,    -1,    33,   424,   425,   158,   130,   428,
     569,   162,   134,   432,   433,    -1,   435,    -1,    -1,   438,
     439,    -1,    -1,    52,   443,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,    -1,    -1,    -1,   638,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,   638,    -1,    -1,   607,    90,
      91,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,    -1,   108,   109,   628,
      -1,    -1,    -1,   102,    -1,    -1,    -1,   638,    -1,    -1,
     499,    -1,    -1,    -1,    -1,    -1,   645,   646,    -1,   130,
      -1,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   130,    -1,    -1,    -1,   134,   525,   526,    -1,   528,
     151,   152,   531,    -1,    -1,    -1,    10,   536,   159,   538,
      -1,    -1,    -1,   152,    -1,    -1,    20,    -1,    -1,   158,
     549,    -1,    26,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,   572,    -1,    -1,    -1,   576,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   614,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   623,   624,    -1,   626,    -1,    -1,
     629,    -1,     1,    -1,    -1,   634,     5,    -1,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,   122,    -1,
      -1,    -1,    21,    22,    23,    24,   655,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    44,    -1,    46,    47,    -1,
      -1,    50,    51,   157,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,   142,    -1,    -1,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,   157,   158,
     159,   160,   161,     1,    -1,    -1,    -1,     5,    -1,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    -1,
      -1,    -1,    -1,    28,    -1,    -1,   154,    -1,    33,   157,
     158,   159,   160,   161,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    52,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      38,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    52,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,   102,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,   152,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,   134,   162,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    -1,
      12,    13,    14,    15,   152,    -1,   154,    -1,    -1,   157,
      22,   159,    -1,   161,   162,    -1,    28,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    38,    -1,    40,    -1,
      -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      52,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    12,    13,    14,    15,    -1,   130,    -1,
      -1,    -1,   134,    22,    -1,   137,   138,   139,    -1,    -1,
     142,    -1,    -1,   145,   146,    34,    -1,    -1,    -1,    38,
     152,    40,   154,    -1,    43,   157,    -1,   159,    47,   161,
     162,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,   138,
     139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,
      -1,    -1,    38,   152,    40,   154,   155,    43,   157,    -1,
     159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    12,    13,    14,    15,    -1,    -1,   154,    -1,
      -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   137,   138,   139,
      -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,    -1,
      -1,    38,    -1,    40,   154,    -1,    43,   157,   158,   159,
      47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      -1,    12,    13,    14,    15,    -1,   153,   154,    -1,    -1,
     157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    -1,
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
      -1,   159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,
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
     155,    43,   157,    -1,   159,    47,   161,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
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
      -1,    -1,    38,    -1,    40,   154,   155,    43,   157,    -1,
     159,    47,   161,    -1,    50,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    34,    -1,    -1,    -1,    38,    -1,    40,   154,    -1,
      43,   157,    -1,   159,    47,   161,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   137,   138,   139,    -1,    -1,   142,
      -1,    -1,   145,   146,    34,    -1,    -1,    -1,    38,    -1,
      40,   154,   155,    43,   157,    -1,   159,    47,   161,    -1,
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
      -1,    38,    -1,    40,   154,    -1,    43,   157,    -1,   159,
      47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
     157,    -1,   159,    18,   161,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    18,   106,
     107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,    -1,    92,    93,    -1,
      -1,   158,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    18,   106,   107,   108,   109,
     110,   111,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,    18,   106,   107,   108,   109,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    90,    91,    92,
      93,    94,   158,    -1,    97,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    20,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    -1,    18,   157,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    -1,   154,    -1,    18,   157,    90,    91,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
      -1,    18,   155,    -1,    -1,    -1,   157,    90,    91,    92,
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
      -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,   151,   152,    -1,    -1,
     155,    90,    91,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,    28,   108,
     109,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    52,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,   151,   152,    -1,    -1,   155,    90,    91,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,    -1,   151,   152,
     153,    90,    91,    92,    93,    94,    18,    -1,    97,    98,
      99,   100,   152,   102,   103,   104,    -1,   106,    -1,   108,
     109,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      18,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    18,    -1,    -1,    -1,    90,    91,
      92,    93,    94,   151,   152,    97,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    18,    -1,
      -1,    -1,    90,    91,    92,    93,    94,   151,   152,    97,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    18,    -1,    97,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,   151,   152,    97,    98,    99,   100,    -1,   102,   103,
     104,    18,   106,    -1,   108,   109,    -1,    90,    91,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,    -1,   108,   109,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    28,    28,
      -1,    -1,    -1,    33,    33,    -1,    -1,   151,   152,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    52,    52,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    90,    91,    92,    93,    28,    -1,    -1,
      -1,    -1,    33,   100,    -1,   102,   103,   104,    -1,   106,
      28,   108,   109,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    52,    28,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,   102,   102,    52,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    28,    52,    -1,    28,    -1,
      33,    -1,    -1,    33,   151,   152,    -1,    -1,    -1,    -1,
     130,   130,    -1,    -1,   134,   134,   136,   136,    -1,    52,
      -1,   102,    52,    -1,    28,    -1,    -1,    -1,    -1,    33,
      -1,    -1,   152,   152,   102,    28,    -1,    -1,    -1,    -1,
      33,    -1,   162,   162,    -1,    -1,   102,    -1,    52,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,    -1,    -1,    52,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,   102,
      -1,   152,   102,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,   162,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   152,   130,   102,    -1,
     130,   134,    -1,   136,   134,    -1,   162,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   152,    -1,    -1,   155,   130,    -1,    -1,   162,
     134,    -1,   162,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,   145,   146
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
     208,   215,   218,   191,   152,   155,   211,   218,    16,   218,
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
     122,   245,   128,   155,   243,   154,   131,   153,   155,   218,
     158,   155,   243,   122,   155,   218,   218,   218,   211,   218,
     246,   122,   153,   245,   218,   153,   250,   246,   153,   153,
     127,   195,   195,    17,   198,   218,   158,    11,   195,   131,
     116,   159,    35,   230,   112,   195,   212,   153,   218,   155,
     211,   218,   155,   136,   158,   153,   227,   160,     9,    19,
     180,   211,   158,   195,   197,   218,   155,   155,    16,   218,
     218,   153,   218,   153,   195,   218,   158,   160,   243,   155,
     158,   255,   195,   195,   155,    45,   153,   180,   218,   153
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   166,   166,   167,   168,   168,   168,
     169,   170,   171,   172,   172,   172,   173,   174,   174,   175,
     176,   176,   177,   177,   178,   179,   179,   180,   180,   180,
     181,   182,   183,   184,   185,   185,   185,   185,   185,   185,
     185,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   191,   191,   191,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   194,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   197,   197,   197,   198,   199,   199,   199,   200,
     201,   201,   201,   201,   202,   203,   204,   204,   204,   204,
     204,   205,   206,   206,   207,   207,   208,   209,   209,   209,
     210,   210,   210,   210,   211,   211,   212,   212,   213,   213,
     214,   214,   214,   214,   214,   214,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   217,   217,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   219,   219,   220,   220,   221,   222,
     222,   223,   223,   224,   224,   225,   226,   226,   227,   227,
     228,   228,   228,   228,   228,   229,   229,   229,   230,   230,
     230,   230,   230,   231,   231,   232,   233,   233,   233,   234,
     235,   236,   236,   237,   238,   239,   239,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   241,   242,   242,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   244,   244,   244,   244,   245,   245,   246,
     246,   247,   247,   247,   247,   248,   248,   249,   250,   250,
     251,   251,   252,   252,   253,   253,   254,   255,   255,   256
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     0,     1,     2,
       1,     3,     1,     3,     3,     3,     2,     0,     2,     4,
       4,     6,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     3,     7,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     4,     0,     2,     1,     1,     3,     5,     5,     5,
       5,     8,     6,     9,     1,     3,     1,     2,     1,     5,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     5,     6,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       3,     3,     2,     3,     4,     3,     4,     2,     4,     4,
       3,     4,     3,     5,     4,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     0,     3,     0,     1,     3,     0,
       3,     1,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     3,     5,     3,     3,     1,     1,     1,     4,     6,
       5,     4,     3,     0,     2,     4,     0,     3,     5,     4,
       5,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     4,     3,     4,
       2,     3,     2,     4,     3,     2,     2,     3,     2,     2,
       4,     6,     4,     1,     4,     5,     1,     4,     5,     1,
       4,     5,     4,     1,     1,     1,     1,     3,     5,     1,
       3,     5,     7,     5,     7,     1,     3,     3,     1,     1,
       1,     3,     5,     5,     1,     3,     4,     0,     3,    10
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
#line 2641 "ds_parser.cpp"
        break;

    case 166: /* character_sequence  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2647 "ds_parser.cpp"
        break;

    case 167: /* string_constant  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2653 "ds_parser.cpp"
        break;

    case 168: /* string_builder_body  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2659 "ds_parser.cpp"
        break;

    case 169: /* string_builder  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2665 "ds_parser.cpp"
        break;

    case 172: /* require_module_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2671 "ds_parser.cpp"
        break;

    case 178: /* expression_label  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2677 "ds_parser.cpp"
        break;

    case 179: /* expression_goto  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2683 "ds_parser.cpp"
        break;

    case 180: /* expression_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2689 "ds_parser.cpp"
        break;

    case 181: /* expression_if_then_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2695 "ds_parser.cpp"
        break;

    case 182: /* expression_for_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2701 "ds_parser.cpp"
        break;

    case 183: /* expression_while_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2707 "ds_parser.cpp"
        break;

    case 184: /* expression_with  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2713 "ds_parser.cpp"
        break;

    case 185: /* annotation_argument  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2719 "ds_parser.cpp"
        break;

    case 186: /* annotation_argument_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2725 "ds_parser.cpp"
        break;

    case 187: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2731 "ds_parser.cpp"
        break;

    case 188: /* annotation_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2737 "ds_parser.cpp"
        break;

    case 189: /* annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2743 "ds_parser.cpp"
        break;

    case 190: /* optional_annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2749 "ds_parser.cpp"
        break;

    case 191: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2755 "ds_parser.cpp"
        break;

    case 192: /* optional_function_type  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2761 "ds_parser.cpp"
        break;

    case 193: /* function_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2767 "ds_parser.cpp"
        break;

    case 195: /* expression_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2773 "ds_parser.cpp"
        break;

    case 196: /* expression_any  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2779 "ds_parser.cpp"
        break;

    case 197: /* expressions  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2785 "ds_parser.cpp"
        break;

    case 198: /* expr_pipe  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2791 "ds_parser.cpp"
        break;

    case 199: /* name_in_namespace  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2797 "ds_parser.cpp"
        break;

    case 200: /* expression_delete  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2803 "ds_parser.cpp"
        break;

    case 201: /* expr_new  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2809 "ds_parser.cpp"
        break;

    case 202: /* expression_break  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2815 "ds_parser.cpp"
        break;

    case 203: /* expression_continue  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2821 "ds_parser.cpp"
        break;

    case 204: /* expression_return  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2827 "ds_parser.cpp"
        break;

    case 205: /* expression_try_catch  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2833 "ds_parser.cpp"
        break;

    case 208: /* expression_let  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2839 "ds_parser.cpp"
        break;

    case 209: /* expr_cast  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2845 "ds_parser.cpp"
        break;

    case 210: /* expr_type_info  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2851 "ds_parser.cpp"
        break;

    case 211: /* expr_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2857 "ds_parser.cpp"
        break;

    case 212: /* block_or_simple_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2863 "ds_parser.cpp"
        break;

    case 213: /* expr_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2869 "ds_parser.cpp"
        break;

    case 214: /* expr_numeric_const  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2875 "ds_parser.cpp"
        break;

    case 215: /* expr_assign  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2881 "ds_parser.cpp"
        break;

    case 216: /* expr_named_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2887 "ds_parser.cpp"
        break;

    case 217: /* expr_method_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2893 "ds_parser.cpp"
        break;

    case 218: /* expr  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2899 "ds_parser.cpp"
        break;

    case 219: /* optional_field_annotation  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2905 "ds_parser.cpp"
        break;

    case 221: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2911 "ds_parser.cpp"
        break;

    case 222: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2917 "ds_parser.cpp"
        break;

    case 223: /* function_argument_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2923 "ds_parser.cpp"
        break;

    case 224: /* function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2929 "ds_parser.cpp"
        break;

    case 225: /* tuple_type  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2935 "ds_parser.cpp"
        break;

    case 226: /* tuple_type_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2941 "ds_parser.cpp"
        break;

    case 228: /* variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2947 "ds_parser.cpp"
        break;

    case 230: /* let_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2953 "ds_parser.cpp"
        break;

    case 231: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2959 "ds_parser.cpp"
        break;

    case 233: /* enum_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 2965 "ds_parser.cpp"
        break;

    case 236: /* optional_structure_parent  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2971 "ds_parser.cpp"
        break;

    case 239: /* variable_name_list  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 2977 "ds_parser.cpp"
        break;

    case 241: /* structure_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2983 "ds_parser.cpp"
        break;

    case 242: /* auto_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2989 "ds_parser.cpp"
        break;

    case 243: /* type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2995 "ds_parser.cpp"
        break;

    case 244: /* make_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3001 "ds_parser.cpp"
        break;

    case 245: /* make_struct_fields  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3007 "ds_parser.cpp"
        break;

    case 246: /* make_struct_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3013 "ds_parser.cpp"
        break;

    case 247: /* make_struct_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3019 "ds_parser.cpp"
        break;

    case 248: /* make_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3025 "ds_parser.cpp"
        break;

    case 249: /* make_map_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3031 "ds_parser.cpp"
        break;

    case 250: /* make_any_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3037 "ds_parser.cpp"
        break;

    case 251: /* make_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3043 "ds_parser.cpp"
        break;

    case 252: /* make_dim_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3049 "ds_parser.cpp"
        break;

    case 253: /* make_table  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3055 "ds_parser.cpp"
        break;

    case 254: /* make_table_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3061 "ds_parser.cpp"
        break;

    case 255: /* array_comprehension_where  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3067 "ds_parser.cpp"
        break;

    case 256: /* array_comprehension  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3073 "ds_parser.cpp"
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
#line 3373 "ds_parser.cpp"
    break;

  case 14:
#line 385 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3379 "ds_parser.cpp"
    break;

  case 15:
#line 386 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3385 "ds_parser.cpp"
    break;

  case 16:
#line 390 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3391 "ds_parser.cpp"
    break;

  case 17:
#line 394 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3400 "ds_parser.cpp"
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
#line 3414 "ds_parser.cpp"
    break;

  case 19:
#line 407 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3424 "ds_parser.cpp"
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
#line 3442 "ds_parser.cpp"
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
#line 3465 "ds_parser.cpp"
    break;

  case 23:
#line 456 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3473 "ds_parser.cpp"
    break;

  case 24:
#line 459 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3484 "ds_parser.cpp"
    break;

  case 25:
#line 465 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3495 "ds_parser.cpp"
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
#line 3510 "ds_parser.cpp"
    break;

  case 27:
#line 487 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3516 "ds_parser.cpp"
    break;

  case 28:
#line 488 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3522 "ds_parser.cpp"
    break;

  case 32:
#line 501 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3530 "ds_parser.cpp"
    break;

  case 33:
#line 504 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3538 "ds_parser.cpp"
    break;

  case 34:
#line 510 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3546 "ds_parser.cpp"
    break;

  case 35:
#line 516 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3554 "ds_parser.cpp"
    break;

  case 36:
#line 519 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3562 "ds_parser.cpp"
    break;

  case 37:
#line 525 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3568 "ds_parser.cpp"
    break;

  case 38:
#line 526 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3574 "ds_parser.cpp"
    break;

  case 39:
#line 527 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3583 "ds_parser.cpp"
    break;

  case 40:
#line 534 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3592 "ds_parser.cpp"
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
#line 3606 "ds_parser.cpp"
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
#line 3618 "ds_parser.cpp"
    break;

  case 43:
#line 563 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3629 "ds_parser.cpp"
    break;

  case 44:
#line 572 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3635 "ds_parser.cpp"
    break;

  case 45:
#line 573 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3641 "ds_parser.cpp"
    break;

  case 46:
#line 574 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3647 "ds_parser.cpp"
    break;

  case 47:
#line 575 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3653 "ds_parser.cpp"
    break;

  case 48:
#line 576 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3659 "ds_parser.cpp"
    break;

  case 49:
#line 577 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3665 "ds_parser.cpp"
    break;

  case 50:
#line 578 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3671 "ds_parser.cpp"
    break;

  case 51:
#line 582 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3681 "ds_parser.cpp"
    break;

  case 52:
#line 587 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3691 "ds_parser.cpp"
    break;

  case 53:
#line 595 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 3697 "ds_parser.cpp"
    break;

  case 54:
#line 596 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 3703 "ds_parser.cpp"
    break;

  case 55:
#line 600 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3715 "ds_parser.cpp"
    break;

  case 56:
#line 607 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3729 "ds_parser.cpp"
    break;

  case 57:
#line 619 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3738 "ds_parser.cpp"
    break;

  case 58:
#line 623 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3747 "ds_parser.cpp"
    break;

  case 59:
#line 630 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 3753 "ds_parser.cpp"
    break;

  case 60:
#line 631 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3759 "ds_parser.cpp"
    break;

  case 61:
#line 635 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3765 "ds_parser.cpp"
    break;

  case 62:
#line 636 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3771 "ds_parser.cpp"
    break;

  case 63:
#line 637 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3777 "ds_parser.cpp"
    break;

  case 64:
#line 641 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3786 "ds_parser.cpp"
    break;

  case 65:
#line 645 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3795 "ds_parser.cpp"
    break;

  case 66:
#line 652 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3804 "ds_parser.cpp"
    break;

  case 67:
#line 656 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 3810 "ds_parser.cpp"
    break;

  case 68:
#line 657 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 3816 "ds_parser.cpp"
    break;

  case 69:
#line 658 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 3822 "ds_parser.cpp"
    break;

  case 70:
#line 659 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 3828 "ds_parser.cpp"
    break;

  case 71:
#line 660 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 3834 "ds_parser.cpp"
    break;

  case 72:
#line 661 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 3840 "ds_parser.cpp"
    break;

  case 73:
#line 662 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 3846 "ds_parser.cpp"
    break;

  case 74:
#line 663 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 3852 "ds_parser.cpp"
    break;

  case 75:
#line 664 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 3858 "ds_parser.cpp"
    break;

  case 76:
#line 665 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 3864 "ds_parser.cpp"
    break;

  case 77:
#line 666 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 3870 "ds_parser.cpp"
    break;

  case 78:
#line 667 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 3876 "ds_parser.cpp"
    break;

  case 79:
#line 668 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 3882 "ds_parser.cpp"
    break;

  case 80:
#line 669 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 3888 "ds_parser.cpp"
    break;

  case 81:
#line 670 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 3894 "ds_parser.cpp"
    break;

  case 82:
#line 671 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 3900 "ds_parser.cpp"
    break;

  case 83:
#line 672 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 3906 "ds_parser.cpp"
    break;

  case 84:
#line 673 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 3912 "ds_parser.cpp"
    break;

  case 85:
#line 674 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 3918 "ds_parser.cpp"
    break;

  case 86:
#line 675 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 3924 "ds_parser.cpp"
    break;

  case 87:
#line 676 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 3930 "ds_parser.cpp"
    break;

  case 88:
#line 677 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 3936 "ds_parser.cpp"
    break;

  case 89:
#line 678 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 3942 "ds_parser.cpp"
    break;

  case 90:
#line 679 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 3948 "ds_parser.cpp"
    break;

  case 91:
#line 680 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 3954 "ds_parser.cpp"
    break;

  case 92:
#line 681 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 3960 "ds_parser.cpp"
    break;

  case 93:
#line 682 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 3966 "ds_parser.cpp"
    break;

  case 94:
#line 683 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 3972 "ds_parser.cpp"
    break;

  case 95:
#line 684 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 3978 "ds_parser.cpp"
    break;

  case 96:
#line 685 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 3984 "ds_parser.cpp"
    break;

  case 97:
#line 686 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 3990 "ds_parser.cpp"
    break;

  case 98:
#line 687 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 3996 "ds_parser.cpp"
    break;

  case 99:
#line 688 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4002 "ds_parser.cpp"
    break;

  case 100:
#line 689 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4008 "ds_parser.cpp"
    break;

  case 101:
#line 690 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4014 "ds_parser.cpp"
    break;

  case 102:
#line 691 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4020 "ds_parser.cpp"
    break;

  case 103:
#line 696 "ds_parser.ypp"
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
#line 4091 "ds_parser.cpp"
    break;

  case 104:
#line 765 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4099 "ds_parser.cpp"
    break;

  case 105:
#line 768 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4111 "ds_parser.cpp"
    break;

  case 106:
#line 778 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4117 "ds_parser.cpp"
    break;

  case 107:
#line 779 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4123 "ds_parser.cpp"
    break;

  case 108:
#line 780 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4129 "ds_parser.cpp"
    break;

  case 109:
#line 781 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4135 "ds_parser.cpp"
    break;

  case 110:
#line 782 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4141 "ds_parser.cpp"
    break;

  case 111:
#line 783 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4147 "ds_parser.cpp"
    break;

  case 112:
#line 784 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4153 "ds_parser.cpp"
    break;

  case 113:
#line 785 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4159 "ds_parser.cpp"
    break;

  case 114:
#line 786 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4165 "ds_parser.cpp"
    break;

  case 115:
#line 787 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4171 "ds_parser.cpp"
    break;

  case 116:
#line 788 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4177 "ds_parser.cpp"
    break;

  case 117:
#line 789 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4183 "ds_parser.cpp"
    break;

  case 118:
#line 790 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4189 "ds_parser.cpp"
    break;

  case 119:
#line 791 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4195 "ds_parser.cpp"
    break;

  case 120:
#line 792 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4201 "ds_parser.cpp"
    break;

  case 121:
#line 793 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4207 "ds_parser.cpp"
    break;

  case 122:
#line 797 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4216 "ds_parser.cpp"
    break;

  case 123:
#line 801 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4227 "ds_parser.cpp"
    break;

  case 124:
#line 807 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4235 "ds_parser.cpp"
    break;

  case 125:
#line 813 "ds_parser.ypp"
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
#line 4251 "ds_parser.cpp"
    break;

  case 126:
#line 827 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4257 "ds_parser.cpp"
    break;

  case 127:
#line 828 "ds_parser.ypp"
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4268 "ds_parser.cpp"
    break;

  case 128:
#line 834 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4274 "ds_parser.cpp"
    break;

  case 129:
#line 838 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4282 "ds_parser.cpp"
    break;

  case 130:
#line 844 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4290 "ds_parser.cpp"
    break;

  case 131:
#line 847 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4298 "ds_parser.cpp"
    break;

  case 132:
#line 850 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4307 "ds_parser.cpp"
    break;

  case 133:
#line 854 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4315 "ds_parser.cpp"
    break;

  case 134:
#line 860 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4321 "ds_parser.cpp"
    break;

  case 135:
#line 864 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4327 "ds_parser.cpp"
    break;

  case 136:
#line 868 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4335 "ds_parser.cpp"
    break;

  case 137:
#line 871 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4343 "ds_parser.cpp"
    break;

  case 138:
#line 874 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4353 "ds_parser.cpp"
    break;

  case 139:
#line 879 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4361 "ds_parser.cpp"
    break;

  case 140:
#line 882 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4371 "ds_parser.cpp"
    break;

  case 141:
#line 890 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4379 "ds_parser.cpp"
    break;

  case 142:
#line 896 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4385 "ds_parser.cpp"
    break;

  case 143:
#line 897 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4391 "ds_parser.cpp"
    break;

  case 144:
#line 901 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4397 "ds_parser.cpp"
    break;

  case 145:
#line 902 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4403 "ds_parser.cpp"
    break;

  case 146:
#line 906 "ds_parser.ypp"
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
#line 4438 "ds_parser.cpp"
    break;

  case 147:
#line 939 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4446 "ds_parser.cpp"
    break;

  case 148:
#line 942 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4456 "ds_parser.cpp"
    break;

  case 149:
#line 947 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4466 "ds_parser.cpp"
    break;

  case 150:
#line 955 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4475 "ds_parser.cpp"
    break;

  case 151:
#line 959 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4485 "ds_parser.cpp"
    break;

  case 152:
#line 964 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4494 "ds_parser.cpp"
    break;

  case 153:
#line 968 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4504 "ds_parser.cpp"
    break;

  case 154:
#line 976 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4512 "ds_parser.cpp"
    break;

  case 155:
#line 979 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4520 "ds_parser.cpp"
    break;

  case 156:
#line 985 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4526 "ds_parser.cpp"
    break;

  case 157:
#line 986 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4538 "ds_parser.cpp"
    break;

  case 158:
#line 996 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4548 "ds_parser.cpp"
    break;

  case 159:
#line 1002 "ds_parser.ypp"
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
#line 4601 "ds_parser.cpp"
    break;

  case 160:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4607 "ds_parser.cpp"
    break;

  case 161:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4613 "ds_parser.cpp"
    break;

  case 162:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4619 "ds_parser.cpp"
    break;

  case 163:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4625 "ds_parser.cpp"
    break;

  case 164:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4631 "ds_parser.cpp"
    break;

  case 165:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4637 "ds_parser.cpp"
    break;

  case 166:
#line 1062 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4643 "ds_parser.cpp"
    break;

  case 167:
#line 1063 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4649 "ds_parser.cpp"
    break;

  case 168:
#line 1064 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4655 "ds_parser.cpp"
    break;

  case 169:
#line 1065 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4661 "ds_parser.cpp"
    break;

  case 170:
#line 1066 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4667 "ds_parser.cpp"
    break;

  case 171:
#line 1067 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4673 "ds_parser.cpp"
    break;

  case 172:
#line 1068 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4679 "ds_parser.cpp"
    break;

  case 173:
#line 1069 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4685 "ds_parser.cpp"
    break;

  case 174:
#line 1070 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4691 "ds_parser.cpp"
    break;

  case 175:
#line 1071 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4697 "ds_parser.cpp"
    break;

  case 176:
#line 1072 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4703 "ds_parser.cpp"
    break;

  case 177:
#line 1073 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4709 "ds_parser.cpp"
    break;

  case 178:
#line 1074 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4715 "ds_parser.cpp"
    break;

  case 179:
#line 1075 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4721 "ds_parser.cpp"
    break;

  case 180:
#line 1076 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4727 "ds_parser.cpp"
    break;

  case 181:
#line 1077 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4733 "ds_parser.cpp"
    break;

  case 182:
#line 1081 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4745 "ds_parser.cpp"
    break;

  case 183:
#line 1092 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4755 "ds_parser.cpp"
    break;

  case 184:
#line 1097 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4767 "ds_parser.cpp"
    break;

  case 185:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4773 "ds_parser.cpp"
    break;

  case 186:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4779 "ds_parser.cpp"
    break;

  case 187:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4785 "ds_parser.cpp"
    break;

  case 188:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4791 "ds_parser.cpp"
    break;

  case 189:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4797 "ds_parser.cpp"
    break;

  case 190:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4803 "ds_parser.cpp"
    break;

  case 191:
#line 1113 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4809 "ds_parser.cpp"
    break;

  case 192:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4815 "ds_parser.cpp"
    break;

  case 193:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4821 "ds_parser.cpp"
    break;

  case 194:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4827 "ds_parser.cpp"
    break;

  case 195:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4833 "ds_parser.cpp"
    break;

  case 196:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4839 "ds_parser.cpp"
    break;

  case 197:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4845 "ds_parser.cpp"
    break;

  case 198:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4851 "ds_parser.cpp"
    break;

  case 199:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4857 "ds_parser.cpp"
    break;

  case 200:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4863 "ds_parser.cpp"
    break;

  case 201:
#line 1123 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4869 "ds_parser.cpp"
    break;

  case 202:
#line 1124 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4875 "ds_parser.cpp"
    break;

  case 203:
#line 1125 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4881 "ds_parser.cpp"
    break;

  case 204:
#line 1126 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4887 "ds_parser.cpp"
    break;

  case 205:
#line 1127 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4893 "ds_parser.cpp"
    break;

  case 206:
#line 1128 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4899 "ds_parser.cpp"
    break;

  case 207:
#line 1129 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4905 "ds_parser.cpp"
    break;

  case 208:
#line 1130 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4911 "ds_parser.cpp"
    break;

  case 209:
#line 1131 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4917 "ds_parser.cpp"
    break;

  case 210:
#line 1132 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4923 "ds_parser.cpp"
    break;

  case 211:
#line 1133 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4929 "ds_parser.cpp"
    break;

  case 212:
#line 1134 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4935 "ds_parser.cpp"
    break;

  case 213:
#line 1135 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4941 "ds_parser.cpp"
    break;

  case 214:
#line 1136 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4947 "ds_parser.cpp"
    break;

  case 215:
#line 1137 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4953 "ds_parser.cpp"
    break;

  case 216:
#line 1138 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4959 "ds_parser.cpp"
    break;

  case 217:
#line 1139 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4965 "ds_parser.cpp"
    break;

  case 218:
#line 1140 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4971 "ds_parser.cpp"
    break;

  case 219:
#line 1141 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4977 "ds_parser.cpp"
    break;

  case 220:
#line 1142 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4983 "ds_parser.cpp"
    break;

  case 221:
#line 1143 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4989 "ds_parser.cpp"
    break;

  case 222:
#line 1144 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4995 "ds_parser.cpp"
    break;

  case 223:
#line 1145 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5004 "ds_parser.cpp"
    break;

  case 224:
#line 1149 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5013 "ds_parser.cpp"
    break;

  case 225:
#line 1153 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5021 "ds_parser.cpp"
    break;

  case 226:
#line 1156 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5029 "ds_parser.cpp"
    break;

  case 227:
#line 1159 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5035 "ds_parser.cpp"
    break;

  case 228:
#line 1160 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5041 "ds_parser.cpp"
    break;

  case 229:
#line 1161 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5047 "ds_parser.cpp"
    break;

  case 230:
#line 1162 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5053 "ds_parser.cpp"
    break;

  case 231:
#line 1163 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5059 "ds_parser.cpp"
    break;

  case 232:
#line 1164 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5065 "ds_parser.cpp"
    break;

  case 233:
#line 1165 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5073 "ds_parser.cpp"
    break;

  case 234:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5081 "ds_parser.cpp"
    break;

  case 235:
#line 1171 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5087 "ds_parser.cpp"
    break;

  case 236:
#line 1172 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5093 "ds_parser.cpp"
    break;

  case 237:
#line 1173 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5099 "ds_parser.cpp"
    break;

  case 238:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5105 "ds_parser.cpp"
    break;

  case 239:
#line 1175 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5111 "ds_parser.cpp"
    break;

  case 240:
#line 1176 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5117 "ds_parser.cpp"
    break;

  case 241:
#line 1177 "ds_parser.ypp"
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
#line 5132 "ds_parser.cpp"
    break;

  case 242:
#line 1187 "ds_parser.ypp"
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
#line 5147 "ds_parser.cpp"
    break;

  case 243:
#line 1197 "ds_parser.ypp"
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
#line 5183 "ds_parser.cpp"
    break;

  case 244:
#line 1231 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5189 "ds_parser.cpp"
    break;

  case 245:
#line 1232 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5195 "ds_parser.cpp"
    break;

  case 246:
#line 1236 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5201 "ds_parser.cpp"
    break;

  case 247:
#line 1237 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5207 "ds_parser.cpp"
    break;

  case 248:
#line 1241 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5217 "ds_parser.cpp"
    break;

  case 249:
#line 1249 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5225 "ds_parser.cpp"
    break;

  case 250:
#line 1252 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5234 "ds_parser.cpp"
    break;

  case 251:
#line 1259 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5240 "ds_parser.cpp"
    break;

  case 252:
#line 1260 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5246 "ds_parser.cpp"
    break;

  case 253:
#line 1264 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5252 "ds_parser.cpp"
    break;

  case 254:
#line 1265 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5258 "ds_parser.cpp"
    break;

  case 255:
#line 1269 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5266 "ds_parser.cpp"
    break;

  case 256:
#line 1275 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5272 "ds_parser.cpp"
    break;

  case 257:
#line 1276 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5278 "ds_parser.cpp"
    break;

  case 258:
#line 1280 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5284 "ds_parser.cpp"
    break;

  case 259:
#line 1281 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5290 "ds_parser.cpp"
    break;

  case 260:
#line 1285 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5300 "ds_parser.cpp"
    break;

  case 261:
#line 1290 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5308 "ds_parser.cpp"
    break;

  case 262:
#line 1293 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5317 "ds_parser.cpp"
    break;

  case 263:
#line 1297 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5328 "ds_parser.cpp"
    break;

  case 264:
#line 1303 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5339 "ds_parser.cpp"
    break;

  case 265:
#line 1312 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5345 "ds_parser.cpp"
    break;

  case 266:
#line 1313 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5351 "ds_parser.cpp"
    break;

  case 267:
#line 1314 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5357 "ds_parser.cpp"
    break;

  case 268:
#line 1318 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5365 "ds_parser.cpp"
    break;

  case 269:
#line 1321 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5375 "ds_parser.cpp"
    break;

  case 270:
#line 1326 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5385 "ds_parser.cpp"
    break;

  case 271:
#line 1331 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5397 "ds_parser.cpp"
    break;

  case 272:
#line 1338 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5409 "ds_parser.cpp"
    break;

  case 273:
#line 1348 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5417 "ds_parser.cpp"
    break;

  case 274:
#line 1351 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5426 "ds_parser.cpp"
    break;

  case 275:
#line 1358 "ds_parser.ypp"
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
#line 5457 "ds_parser.cpp"
    break;

  case 276:
#line 1387 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5465 "ds_parser.cpp"
    break;

  case 277:
#line 1390 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5479 "ds_parser.cpp"
    break;

  case 278:
#line 1399 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5493 "ds_parser.cpp"
    break;

  case 279:
#line 1411 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5507 "ds_parser.cpp"
    break;

  case 280:
#line 1423 "ds_parser.ypp"
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
#line 5523 "ds_parser.cpp"
    break;

  case 281:
#line 1437 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5529 "ds_parser.cpp"
    break;

  case 282:
#line 1438 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5535 "ds_parser.cpp"
    break;

  case 283:
#line 1442 "ds_parser.ypp"
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
#line 5576 "ds_parser.cpp"
    break;

  case 284:
#line 1481 "ds_parser.ypp"
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
#line 5644 "ds_parser.cpp"
    break;

  case 285:
#line 1547 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5656 "ds_parser.cpp"
    break;

  case 286:
#line 1554 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5667 "ds_parser.cpp"
    break;

  case 287:
#line 1563 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 5673 "ds_parser.cpp"
    break;

  case 288:
#line 1564 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 5679 "ds_parser.cpp"
    break;

  case 289:
#line 1565 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 5685 "ds_parser.cpp"
    break;

  case 290:
#line 1566 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 5691 "ds_parser.cpp"
    break;

  case 291:
#line 1567 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 5697 "ds_parser.cpp"
    break;

  case 292:
#line 1568 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 5703 "ds_parser.cpp"
    break;

  case 293:
#line 1569 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 5709 "ds_parser.cpp"
    break;

  case 294:
#line 1570 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 5715 "ds_parser.cpp"
    break;

  case 295:
#line 1571 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 5721 "ds_parser.cpp"
    break;

  case 296:
#line 1572 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 5727 "ds_parser.cpp"
    break;

  case 297:
#line 1573 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 5733 "ds_parser.cpp"
    break;

  case 298:
#line 1574 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 5739 "ds_parser.cpp"
    break;

  case 299:
#line 1575 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 5745 "ds_parser.cpp"
    break;

  case 300:
#line 1576 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 5751 "ds_parser.cpp"
    break;

  case 301:
#line 1577 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 5757 "ds_parser.cpp"
    break;

  case 302:
#line 1578 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 5763 "ds_parser.cpp"
    break;

  case 303:
#line 1579 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 5769 "ds_parser.cpp"
    break;

  case 304:
#line 1580 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 5775 "ds_parser.cpp"
    break;

  case 305:
#line 1581 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 5781 "ds_parser.cpp"
    break;

  case 306:
#line 1582 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 5787 "ds_parser.cpp"
    break;

  case 307:
#line 1583 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 5793 "ds_parser.cpp"
    break;

  case 308:
#line 1584 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 5799 "ds_parser.cpp"
    break;

  case 309:
#line 1585 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 5805 "ds_parser.cpp"
    break;

  case 310:
#line 1586 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 5811 "ds_parser.cpp"
    break;

  case 311:
#line 1590 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5824 "ds_parser.cpp"
    break;

  case 312:
#line 1601 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5833 "ds_parser.cpp"
    break;

  case 313:
#line 1605 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5845 "ds_parser.cpp"
    break;

  case 314:
#line 1615 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5851 "ds_parser.cpp"
    break;

  case 315:
#line 1616 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5857 "ds_parser.cpp"
    break;

  case 316:
#line 1617 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5863 "ds_parser.cpp"
    break;

  case 317:
#line 1618 "ds_parser.ypp"
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
#line 5882 "ds_parser.cpp"
    break;

  case 318:
#line 1632 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5893 "ds_parser.cpp"
    break;

  case 319:
#line 1638 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5902 "ds_parser.cpp"
    break;

  case 320:
#line 1642 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5912 "ds_parser.cpp"
    break;

  case 321:
#line 1647 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5922 "ds_parser.cpp"
    break;

  case 322:
#line 1652 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5932 "ds_parser.cpp"
    break;

  case 323:
#line 1657 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5944 "ds_parser.cpp"
    break;

  case 324:
#line 1664 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5954 "ds_parser.cpp"
    break;

  case 325:
#line 1669 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 5963 "ds_parser.cpp"
    break;

  case 326:
#line 1673 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 5972 "ds_parser.cpp"
    break;

  case 327:
#line 1677 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5982 "ds_parser.cpp"
    break;

  case 328:
#line 1682 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5992 "ds_parser.cpp"
    break;

  case 329:
#line 1687 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6004 "ds_parser.cpp"
    break;

  case 330:
#line 1694 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6014 "ds_parser.cpp"
    break;

  case 331:
#line 1699 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6025 "ds_parser.cpp"
    break;

  case 332:
#line 1705 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6035 "ds_parser.cpp"
    break;

  case 333:
#line 1710 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6044 "ds_parser.cpp"
    break;

  case 334:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6054 "ds_parser.cpp"
    break;

  case 335:
#line 1719 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6068 "ds_parser.cpp"
    break;

  case 336:
#line 1728 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6077 "ds_parser.cpp"
    break;

  case 337:
#line 1732 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6087 "ds_parser.cpp"
    break;

  case 338:
#line 1737 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6101 "ds_parser.cpp"
    break;

  case 339:
#line 1746 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6110 "ds_parser.cpp"
    break;

  case 340:
#line 1750 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6120 "ds_parser.cpp"
    break;

  case 341:
#line 1755 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6134 "ds_parser.cpp"
    break;

  case 342:
#line 1764 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6145 "ds_parser.cpp"
    break;

  case 343:
#line 1773 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6151 "ds_parser.cpp"
    break;

  case 344:
#line 1774 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6157 "ds_parser.cpp"
    break;

  case 345:
#line 1775 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6163 "ds_parser.cpp"
    break;

  case 346:
#line 1776 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6169 "ds_parser.cpp"
    break;

  case 347:
#line 1780 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6181 "ds_parser.cpp"
    break;

  case 348:
#line 1787 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6192 "ds_parser.cpp"
    break;

  case 349:
#line 1796 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6202 "ds_parser.cpp"
    break;

  case 350:
#line 1801 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6211 "ds_parser.cpp"
    break;

  case 351:
#line 1808 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6221 "ds_parser.cpp"
    break;

  case 352:
#line 1813 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6232 "ds_parser.cpp"
    break;

  case 353:
#line 1819 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6244 "ds_parser.cpp"
    break;

  case 354:
#line 1826 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6257 "ds_parser.cpp"
    break;

  case 355:
#line 1837 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6265 "ds_parser.cpp"
    break;

  case 356:
#line 1840 "ds_parser.ypp"
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
#line 6281 "ds_parser.cpp"
    break;

  case 357:
#line 1854 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6292 "ds_parser.cpp"
    break;

  case 358:
#line 1863 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6298 "ds_parser.cpp"
    break;

  case 359:
#line 1864 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6304 "ds_parser.cpp"
    break;

  case 360:
#line 1868 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6314 "ds_parser.cpp"
    break;

  case 361:
#line 1873 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6323 "ds_parser.cpp"
    break;

  case 362:
#line 1880 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6333 "ds_parser.cpp"
    break;

  case 363:
#line 1885 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6346 "ds_parser.cpp"
    break;

  case 364:
#line 1896 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6356 "ds_parser.cpp"
    break;

  case 365:
#line 1901 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6365 "ds_parser.cpp"
    break;

  case 366:
#line 1908 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6379 "ds_parser.cpp"
    break;

  case 367:
#line 1920 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6385 "ds_parser.cpp"
    break;

  case 368:
#line 1921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6391 "ds_parser.cpp"
    break;

  case 369:
#line 1925 "ds_parser.ypp"
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
#line 6409 "ds_parser.cpp"
    break;


#line 6413 "ds_parser.cpp"

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
#line 1940 "ds_parser.ypp"


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


