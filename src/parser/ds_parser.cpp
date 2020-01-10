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
    DAS_GENERATOR = 337,
    DAS_YIELD = 338,
    ADDEQU = 339,
    SUBEQU = 340,
    DIVEQU = 341,
    MULEQU = 342,
    MODEQU = 343,
    ANDEQU = 344,
    OREQU = 345,
    XOREQU = 346,
    SHL = 347,
    SHR = 348,
    ADDADD = 349,
    SUBSUB = 350,
    LEEQU = 351,
    SHLEQU = 352,
    SHREQU = 353,
    GREQU = 354,
    EQUEQU = 355,
    NOTEQU = 356,
    RARROW = 357,
    LARROW = 358,
    QQ = 359,
    QDOT = 360,
    LPIPE = 361,
    LBPIPE = 362,
    RPIPE = 363,
    CLONEEQU = 364,
    ROTL = 365,
    ROTR = 366,
    ROTLEQU = 367,
    ROTREQU = 368,
    MAPTO = 369,
    BRABRAB = 370,
    BRACBRB = 371,
    CBRCBRB = 372,
    INTEGER = 373,
    LONG_INTEGER = 374,
    UNSIGNED_INTEGER = 375,
    UNSIGNED_LONG_INTEGER = 376,
    FLOAT = 377,
    DOUBLE = 378,
    NAME = 379,
    BEGIN_STRING = 380,
    STRING_CHARACTER = 381,
    END_STRING = 382,
    BEGIN_STRING_EXPR = 383,
    END_STRING_EXPR = 384,
    UNARY_MINUS = 385,
    UNARY_PLUS = 386,
    PRE_INC = 387,
    PRE_DEC = 388,
    POST_INC = 389,
    POST_DEC = 390,
    DEREF = 391,
    COLCOL = 392
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

#line 365 "ds_parser.cpp"

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
#define YYLAST   5046

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  165
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  380
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  677

#define YYUNDEFTOK  2
#define YYMAXUTOK   392

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
       2,     2,     2,   148,     2,   164,   163,   143,   136,     2,
     156,   157,   141,   140,   130,   139,   153,   142,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   133,   160,
     137,   131,   138,   132,   144,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   154,     2,   155,   135,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   161,   134,   162,   147,     2,     2,     2,
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
     125,   126,   127,   128,   129,   145,   146,   149,   150,   151,
     152,   158,   159
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   363,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   377,   389,   390,   394,   398,   402,   411,
     419,   435,   456,   460,   463,   469,   478,   491,   492,   496,
     500,   501,   505,   508,   514,   520,   523,   529,   530,   531,
     538,   545,   557,   567,   576,   577,   578,   579,   580,   581,
     585,   590,   598,   599,   600,   601,   602,   603,   604,   605,
     611,   624,   640,   641,   645,   652,   664,   668,   675,   676,
     680,   681,   682,   686,   690,   697,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   740,   810,   813,   823,   824,   825,   826,   827,
     828,   829,   830,   831,   832,   833,   834,   835,   836,   837,
     838,   842,   846,   852,   858,   872,   873,   879,   883,   889,
     892,   895,   899,   905,   909,   913,   916,   919,   924,   927,
     935,   941,   942,   946,   947,   951,   984,   987,   992,  1000,
    1004,  1009,  1013,  1021,  1024,  1030,  1031,  1041,  1046,  1098,
    1099,  1100,  1101,  1102,  1103,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1126,  1137,  1142,  1152,  1153,  1154,  1155,  1156,  1157,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,
    1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,
    1188,  1189,  1190,  1194,  1198,  1201,  1204,  1205,  1206,  1207,
    1208,  1209,  1210,  1211,  1212,  1215,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1234,  1244,  1278,  1279,  1283,  1284,  1288,
    1296,  1299,  1306,  1307,  1311,  1312,  1316,  1322,  1323,  1327,
    1328,  1332,  1337,  1340,  1344,  1350,  1359,  1360,  1361,  1365,
    1368,  1373,  1378,  1385,  1395,  1398,  1405,  1434,  1437,  1446,
    1458,  1470,  1484,  1485,  1489,  1528,  1594,  1601,  1610,  1611,
    1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,  1631,
    1632,  1633,  1637,  1648,  1652,  1662,  1663,  1664,  1665,  1679,
    1685,  1689,  1694,  1699,  1704,  1711,  1716,  1720,  1724,  1729,
    1734,  1741,  1746,  1752,  1757,  1761,  1766,  1775,  1779,  1784,
    1793,  1797,  1802,  1811,  1820,  1821,  1822,  1823,  1827,  1834,
    1843,  1848,  1855,  1860,  1866,  1873,  1884,  1887,  1901,  1910,
    1911,  1915,  1920,  1927,  1932,  1943,  1948,  1955,  1967,  1968,
    1972
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
  "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"", "\"generator\"",
  "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"<|\"", "\" <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
  "annotation_argument_value", "annotation_argument_value_list",
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
      44,    61,    63,    58,   124,    94,    38,    60,    62,    45,
      43,    42,    47,    37,    64,   385,   386,   126,    33,   387,
     388,   389,   390,    46,    91,    93,    40,    41,   391,   392,
      59,   123,   125,    36,    35
};
# endif

#define YYPACT_NINF -460

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-460)))

#define YYTABLE_NINF -176

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -460,    26,  -460,  -460,   -66,   -55,    20,    39,    40,  -460,
      45,   -13,  -460,  -460,  -460,  -460,  -460,   244,  -460,   -16,
    -460,  -460,  -460,  -460,    28,  -460,    69,   -56,    83,  -460,
    -460,   -27,  -460,    55,   102,    57,  -460,    66,   115,    88,
    -460,   -76,  -460,   123,     6,  -460,    25,   -66,   161,   -55,
    -460,   165,   167,  -460,  -460,  3612,  -460,   168,  -460,   -66,
     -13,  -460,   152,   132,  4898,   265,   267,  -460,   143,   -97,
    -460,  -460,  -460,  -460,  -460,   174,    86,  -460,  -460,  -460,
    -460,  -460,  -460,   -95,   164,   166,   170,  -460,  -460,  -460,
     146,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,  -460,   172,  -460,  -460,  -460,   173,
     175,  -460,  -460,  -460,  -460,   176,  -460,  -460,  -460,  -460,
    4817,  -460,   -60,  -460,   -90,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,   -19,   171,  -460,  -460,  -460,   -43,  -460,
     -24,  -460,  -460,  -460,  -460,  -460,  -460,  -460,   -45,   -84,
    -460,  3612,  3612,  3612,   181,  3540,  3540,  3540,  3612,  -460,
      67,  -460,  -460,  -460,  -460,  1584,  -460,  -460,  -460,    61,
     187,  -460,  -460,    27,  -460,   157,  3612,   153,  -460,  -460,
     193,  -460,  3612,  2757,  -460,  -460,    86,  -460,  2757,  -460,
    4023,  3827,  4166,   162,   171,  4167,   171,  4720,   171,  4740,
    -460,   -77,  4817,   184,  -460,   178,  -460,  -460,  -460,  3468,
     169,  -460,   179,   185,   186,   180,   191,   182,   183,  2757,
    2757,  2873,  3612,  2757,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  2757,  2757,  2757,   -90,  2757,  2757,  -460,  2757,  -460,
     188,  -460,  -460,   -85,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  4010,   189,  -460,  -460,  -460,  -460,  -460,   -66,  -460,
     290,   177,  -460,  -460,   -30,  -460,  -460,  3612,  2757,  4817,
    -460,  -460,  3761,  -460,   223,  2951,  -460,  3281,  -460,  3612,
    -460,  -460,   196,  -460,   202,  -460,   203,  -460,  -460,  3612,
    -460,  -460,   470,  -460,   219,  2757,  3612,  3612,  2757,  3612,
    1722,  1837,   124,   124,   187,   963,  1101,  4305,  -460,    94,
     110,   124,   124,  -460,  -460,   124,   124,   476,   648,   143,
    -460,  1216,   330,  2757,  2757,  -460,  -460,  2757,  2757,  2757,
    2757,   224,  2757,   227,  2757,  2757,  2757,  2757,  2757,  2757,
    2757,  2757,  2757,  2757,  2757,  2757,  2757,  2757,  2757,   228,
    2757,  -460,  1952,   -38,  -460,   187,  -460,  -460,    58,  -460,
    4153,  -460,  2757,   107,  2757,  2757,  2757,  2757,  2757,  2757,
    2757,  2757,  2757,  2757,  2757,  2757,  2757,  2757,  2757,  -460,
    -460,  4773,  -460,  -460,  -460,  -460,  2067,  1331,  3608,  4786,
    4787,  3680,  4794,  -460,  3746,  -460,  3812,    -7,   -79,  2182,
    4305,   217,  -115,   229,  -460,  -460,  -114,  2297,   109,   114,
    2757,  2757,   213,  -460,  2757,   251,  -460,  -460,  2757,  2757,
     187,  1469,  -460,   153,  2757,  2757,  -460,   218,   261,  2412,
    -460,   348,   221,   222,  -460,  -460,  -460,  -460,  -460,  -460,
     225,   226,   245,  -460,  -460,   370,  -460,   -82,  4249,   171,
     285,  -460,    21,  4473,  3612,   451,   451,    14,    14,  4694,
    4694,   233,   124,  -460,   -72,   -72,   451,   451,  4361,  4529,
    4585,  4641,    14,    14,   366,   366,   646,   646,   646,  -460,
    4074,  -460,    35,  -460,  -460,  2757,  -460,  3040,  -460,  4473,
    4473,  4473,  4473,  4473,  4473,  4473,  4473,  4473,  4473,  4473,
    4473,  4473,  4473,  4473,  -460,  -460,    43,  3612,   286,  3878,
    -460,  2757,  2757,  -460,  2757,  -460,  -460,  2757,  2757,   285,
     287,   257,   285,  2757,   259,  2757,   285,   262,   263,  4473,
    -460,  -460,  4417,  2866,  2866,    18,  2757,  -460,  -460,  3129,
     405,  4473,  2866,  -460,   288,   301,  4473,   266,  -460,  -460,
    -460,  -460,  -460,   385,   187,  -460,   -86,   -47,   -34,  2757,
    -460,  4817,  2527,  2757,  -460,  -460,  4473,  -460,  -460,  2859,
     284,  -460,    14,    14,    14,   -88,  4473,   -96,   -47,  -460,
     217,  4473,  -460,  -460,   122,  -460,  -460,  -460,  -460,    95,
    2757,  -460,  3218,  -460,   153,  -460,  -460,  -460,  -460,  -460,
    -460,  2757,  -460,  -460,   269,  4473,  -460,    77,  4473,  -460,
    2642,  2757,   268,  2757,   273,   153,  2757,  -460,   -87,  -460,
    -460,   811,  4473,  -460,  -460,  3612,  3944,  3344,  -460,  4473,
    -460,  -460,  2866,   153,  -460,  3602,  -460,   379,   276,    95,
    -460,  -460,  2757,   277,  -460,  4473,  -460
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    68,     1,   153,     0,     0,     0,     0,     0,   154,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,    58,    60,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    63,   135,     0,    64,
      66,     0,    62,     0,     0,   284,     0,     0,     0,     0,
      28,     0,     0,    26,   287,     0,    13,     0,   137,     0,
       0,    69,   292,     0,     0,     0,     0,    75,    70,     0,
      56,    57,    54,    55,    53,     0,     0,    52,    61,    33,
      31,    25,    24,     0,     0,     0,     0,   298,   318,   299,
     323,   300,   304,   305,   306,   307,   311,   312,   313,   314,
     315,   316,   317,   319,   320,   344,   303,   310,   321,   347,
     350,   301,   308,   302,   309,     0,   322,   325,   327,   326,
     290,   136,     0,    67,     0,   294,   260,    78,    79,    81,
      80,    82,    83,    84,    85,   104,   105,   102,   103,    95,
     106,   107,    96,    93,    94,   108,   109,   110,   111,    98,
      99,    97,    91,    92,    87,    86,    88,    89,    90,    77,
      76,   100,   101,     0,    73,   296,   286,   285,     0,    14,
       0,    48,    49,    46,    47,    45,    44,    50,     0,     0,
     291,     0,     0,     0,     0,    70,    70,    70,     0,   331,
       0,   337,   340,   339,   333,     0,   336,    65,   293,   255,
       0,    71,   264,     0,   262,   271,     0,     0,   277,   278,
       0,   276,     0,     0,    15,    16,     0,    59,     0,   288,
       0,     0,     0,     0,    73,     0,    73,     0,    73,     0,
     267,     0,   266,   332,   335,     0,   338,   199,   200,     0,
       0,   194,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   169,   171,   170,   172,   173,   174,
      17,     0,     0,     0,     0,     0,     0,   329,     0,   131,
      68,   197,   167,   195,   248,   247,   246,   251,   196,   250,
     249,     0,     0,   198,   354,   355,   356,   357,     0,   295,
     257,     0,   263,    72,     0,   270,   269,     0,     0,    74,
     112,   297,     0,   283,     0,   175,    51,     0,   341,     0,
     343,   324,     0,   345,     0,   348,     0,   351,   353,     0,
     334,   330,   139,   142,     0,     0,     0,     0,     0,     0,
       0,     0,   223,   224,     0,     0,     0,     0,   375,     0,
       0,   204,   203,   236,   231,   202,   201,     0,     0,    70,
     254,     0,     0,     0,     0,   225,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,     0,     0,   258,     0,   261,   265,   272,   275,
     274,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     289,     0,   346,   349,   352,   268,     0,     0,     0,     0,
       0,     0,     0,   239,     0,   241,     0,     0,   135,     0,
     366,   360,     0,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    18,   227,   133,     0,     0,
       0,     0,   143,     0,     0,     0,   144,     0,     0,     0,
     115,   113,     0,     0,   126,   122,   120,   121,   132,   116,
       0,     0,     0,   125,   127,   151,   119,     0,   175,    73,
       0,   232,     0,   163,     0,   205,   206,   218,   219,   216,
     217,     0,   243,   230,   252,   253,   207,   208,     0,   221,
     222,   220,   214,   215,   210,   209,   211,   212,   213,   229,
       0,   234,     0,   256,   259,     0,   281,   175,   134,   182,
     183,   185,   184,   186,   179,   180,   181,   187,   188,   177,
     178,   189,   190,   176,   342,   140,     0,     0,     0,     0,
     237,     0,     0,   238,     0,   240,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   368,
     376,   377,     0,     0,     0,     0,     0,   145,   148,   175,
       0,   138,     0,   130,     0,     0,    36,     0,   128,   129,
     118,   123,   124,     0,     0,   117,     0,     0,     0,     0,
     233,   245,     0,     0,   228,   235,   273,   280,   141,     0,
       0,   159,   156,   157,   158,     0,   358,     0,     0,   362,
     361,   367,   373,   372,     0,   364,   374,    19,    42,    37,
       0,   149,   175,   146,     0,    43,    34,    35,   131,   152,
     155,     0,   165,   168,     0,   164,   192,     0,   244,   161,
       0,     0,     0,     0,     0,     0,     0,    40,     0,   147,
     150,     0,   166,   191,   193,     0,     0,   378,   363,   359,
     365,    38,     0,     0,   114,     0,   160,     0,     0,    37,
      41,   162,     0,     0,    39,   379,   380
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -460,  -460,  -460,    99,   388,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,   386,  -460,  -460,  -228,  -460,  -460,
    -460,  -460,   230,  -460,   395,   -53,  -460,   383,  -460,   192,
    -167,  -213,  -460,  -460,  -157,  -460,  -180,  -209,   -11,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,   448,  -460,  -460,  -460,
    -368,  -460,    59,  -460,  -345,  -460,  -460,    -4,  -460,  -460,
    -460,  -460,   159,  -460,   135,  -460,  -200,  -183,   154,  -127,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,   -67,   -54,
    -460,  -460,   -46,   220,  -459,  -329,  -460,  -460,  -245,   -92,
     128,  -460,  -460,  -460,  -460,  -460
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   170,   176,   340,   271,    14,    15,    31,
      32,    53,    16,    28,    29,   462,   463,   647,   464,   465,
     466,   467,   177,   178,    25,    26,    39,    40,    41,    17,
     164,   207,    68,    18,   272,   468,   348,   469,   273,   470,
     274,   471,   472,   473,   474,   584,   475,   476,   275,   276,
     482,   633,   277,   278,   304,   279,   280,   483,   290,   385,
     291,   199,   202,   203,   230,   231,   548,   204,   213,   167,
      69,    20,    83,    21,    22,   125,    63,    23,   205,   282,
     118,   119,   232,   283,   431,   432,   284,   433,   434,   435,
     436,   285,   339,   286,   668,   287
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   117,   168,   477,   303,   298,   122,   438,   338,   120,
     547,   312,   200,   314,   512,   316,    36,   292,   224,   226,
     228,   588,    50,   200,   295,   393,     2,   165,   631,   179,
     361,     3,   352,   363,    37,   620,    64,    70,    71,   350,
     551,   554,   589,   589,   116,   552,   555,   218,   536,    42,
     300,     4,   296,     5,    60,     6,   295,     7,    24,   642,
     208,   318,     8,    27,   552,   166,   209,   180,     9,    38,
      47,   351,   641,   663,    10,   269,   219,    48,   585,    61,
      57,   379,   380,   319,   296,   216,   189,   210,   211,   389,
     212,   190,    47,   610,   165,   233,   550,   197,   171,   172,
      65,    66,   214,   215,   645,   165,   353,   354,   355,   356,
     191,    37,   217,   198,   646,    51,   361,   513,   362,   363,
     364,   634,   365,   210,   366,   367,    52,   117,   117,   117,
      67,   117,   117,   117,   117,   220,   221,   222,   201,   225,
     227,   229,   352,    72,    30,    45,    38,    73,   210,    74,
      75,   589,   117,   374,   375,   376,   377,   378,   117,    46,
     299,   295,   192,    33,    34,   589,   302,   379,   380,    35,
     116,   116,   116,   589,   116,   116,   116,   116,   590,   605,
      11,    76,   479,   516,   293,   117,    12,   294,   515,   296,
     193,   281,   595,   322,   194,   116,   560,   117,   117,    47,
     598,   116,   514,   234,   173,   335,   336,   589,   174,   305,
     175,    75,   195,    49,   307,   288,    54,    56,   355,   356,
     607,   235,   196,   289,   637,    57,   361,   614,   116,   363,
     364,   236,   365,    55,   654,   383,   169,   443,   444,    58,
     116,   116,   568,   117,    59,   332,   333,    62,    43,   337,
      44,   388,   648,   344,   441,   117,   442,   341,   342,   343,
     295,   345,   346,   411,   347,   117,   586,   427,   269,   552,
     270,   557,   117,   117,   555,   117,   558,   379,   380,    79,
     419,   420,   552,   422,   644,   124,   116,   210,   296,    81,
     297,    82,   121,   126,   390,   161,   570,   162,   116,   163,
     169,   181,   184,   182,   206,   223,   477,   183,   116,   185,
     186,   165,   187,   188,   269,   116,   116,   301,   116,   311,
     320,   418,   326,   327,   421,   324,   424,   426,   329,   384,
     393,   430,   430,   321,   412,   325,   328,   386,   330,   331,
     413,   414,    11,   417,   478,   382,   484,   550,   491,   485,
     486,   493,   509,   487,   488,   489,   490,   621,   492,   553,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   561,   510,   214,   573,   574,
     577,   578,   579,   565,   352,   580,   581,   583,   517,   592,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   582,   618,   619,   643,   587,
     600,   608,   609,   539,   612,   625,   624,   615,   616,   627,
     629,   626,   640,   658,   672,   347,   653,   628,   660,   632,
     117,   673,   676,   347,    77,    80,   559,   337,   591,   445,
     562,   674,    78,   123,   563,   564,   306,   569,   651,    19,
     571,   572,   518,   387,   415,   576,   392,   630,     0,   323,
     355,   356,   349,   613,   439,     0,     0,   650,   361,   352,
     362,   363,   364,   116,   365,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,     0,     0,   661,     0,
       0,   599,     0,     0,   352,     0,     0,     0,   189,     0,
       0,     0,     0,   190,     0,   669,   670,   376,   377,   378,
       0,   596,     0,     0,     0,     0,     0,   168,     0,   379,
     380,     0,   191,     0,     0,     0,   116,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   602,   603,     0,
     604,     0,     0,     0,   606,   355,   356,     0,     0,   611,
       0,   430,     0,   361,     0,   362,   363,   364,     0,   365,
       0,     0,   622,     0,     0,     0,     0,     0,   353,   354,
     355,   356,   357,     0,   192,   358,   359,   360,   361,     0,
     362,   363,   364,     0,   365,   635,   366,   367,     0,   638,
     374,   375,   376,   377,   378,     0,     0,     0,     0,     0,
       0,   117,   193,     0,   379,   380,   194,     0,   368,   665,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
       0,     0,     0,     0,   195,     0,   416,   652,     0,   379,
     380,     0,     0,   446,   196,     0,   656,   657,     0,   659,
       0,     0,   662,     0,   116,     0,     0,   478,     0,   447,
       0,     0,     0,     3,     0,   448,   449,     0,   450,     0,
     237,   238,   239,   240,   352,     0,     0,     0,   675,   451,
     241,   452,   453,     0,     0,     0,     0,     0,     0,     0,
       0,   454,   242,     0,     0,   455,   243,     0,   244,     0,
       9,   245,   456,     0,   457,   246,     0,     0,   458,   459,
       0,    87,    88,    89,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,     0,
     106,   107,   108,     0,   247,   111,   112,   113,   114,     0,
     248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,   356,   249,   250,     0,     0,     0,     0,   361,     0,
     362,   363,   364,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,   251,   252,   253,   254,   255,   256,   257,
     258,   259,    37,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   262,   263,
       0,     0,   264,     0,     0,   265,   266,     0,     0,   379,
     380,     0,     0,     0,   268,     0,     0,    38,   460,   269,
     461,   270,   447,     0,     0,     0,     3,     0,   448,   449,
       0,   450,     0,   237,   238,   239,   240,     0,     0,     0,
       0,     0,   451,   241,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   242,     0,     0,   455,   243,
       0,   244,     0,     9,   245,   456,     0,   457,   246,     0,
       0,   458,   459,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   247,   111,   112,
     113,   114,     0,   248,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,   252,   253,   254,
     255,   256,   257,   258,   259,    37,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,   263,     0,     0,   264,     0,     0,   265,   266,
       0,     0,     0,     0,     0,     0,     0,   268,     0,     0,
      38,   460,   269,   664,   270,   237,   238,   239,   240,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,   189,     0,     0,     0,     0,   190,   242,     0,     0,
       0,   243,     0,   244,     0,     0,   245,     0,     0,     0,
     246,     0,     0,     0,     0,   191,    87,    88,    89,     0,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,     0,   106,   107,   108,     0,   247,
     111,   112,   113,   114,     0,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   428,   260,     0,
       0,     0,     0,     0,     0,   193,     0,     0,     0,   194,
       0,     0,   261,   262,   263,     0,     0,   264,     0,     0,
     265,   266,     0,   237,   238,   239,   240,   195,     0,   429,
       0,     0,    38,   241,   269,     0,   270,   196,     0,   189,
       0,     0,     0,     0,   190,   242,     0,     0,     0,   243,
       0,   244,     0,     0,   245,     0,     0,     0,   246,     0,
       0,     0,     0,   191,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   247,   111,   112,
     113,   114,     0,   248,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   428,   260,     0,   237,   238,
     239,   240,     0,   193,     0,     0,     0,   194,   241,     0,
     261,   262,   263,     0,     0,   264,     0,     0,   265,   266,
     242,     0,     0,     0,   243,   195,   244,   437,     0,   245,
      38,     0,   269,   246,   270,   196,     0,     0,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   247,   111,   112,   113,   114,     0,   248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,   252,   253,   254,   255,   256,   257,   258,   259,
      37,   260,     0,   237,   238,   239,   240,   537,     0,     0,
       0,     0,     0,   241,     0,   261,   262,   263,     0,     0,
     264,     0,     0,   265,   266,   242,     0,     0,     0,   243,
     480,   244,   268,   481,   245,    38,     0,   269,   246,   270,
       0,     0,     0,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   247,   111,   112,
     113,   114,     0,   248,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,   252,   253,   254,
     255,   256,   257,   258,   259,    37,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,     0,
     261,   262,   263,     0,     0,   264,     0,     0,   265,   266,
       0,   237,   238,   239,   240,     0,     0,   268,     0,     0,
      38,   241,   269,     0,   270,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,   243,     0,   244,
       0,     0,   245,     0,     0,     0,   246,     0,     0,     0,
       0,     0,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   247,   111,   112,   113,   114,
       0,   248,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   249,   250,     0,     0,     0,     0,     0,
       0,     0,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   252,   253,   254,   255,   256,
     257,   258,   259,    37,   260,     0,   237,   238,   239,   240,
       0,     0,     0,     0,     0,     0,   241,     0,   261,   262,
     263,     0,     0,   264,     0,     0,   265,   266,   242,     0,
       0,     0,   243,     0,   244,   268,     0,   245,    38,   567,
     269,   246,   270,     0,     0,     0,     0,    87,    88,    89,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     0,   106,   107,   108,     0,
     247,   111,   112,   113,   114,     0,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
     252,   253,   254,   255,   256,   257,   258,   259,    37,   260,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,   262,   263,     0,     0,   264,     0,
       0,   265,   266,     0,   237,   238,   239,   240,     0,   267,
     268,     0,     0,    38,   241,   269,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
     243,     0,   244,     0,     0,   245,     0,     0,     0,   246,
       0,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,   242,     0,     0,     0,   243,     0,   244,   268,   423,
     245,    38,     0,   269,   246,   270,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,     0,   237,   238,   239,   240,     0,     0,
       0,     0,     0,     0,   241,     0,   261,   262,   263,     0,
       0,   264,     0,     0,   265,   266,   242,     0,     0,     0,
     243,     0,   244,   268,   425,   245,    38,     0,   269,   246,
     270,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,   242,     0,     0,     0,   243,     0,   244,   268,   511,
     245,    38,     0,   269,   246,   270,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,     0,   237,   238,   239,   240,     0,     0,
       0,     0,     0,     0,   241,     0,   261,   262,   263,     0,
       0,   264,     0,     0,   265,   266,   242,     0,     0,     0,
     243,     0,   244,   268,   535,   245,    38,     0,   269,   246,
     270,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,   242,     0,     0,     0,   243,     0,   244,   268,   549,
     245,    38,     0,   269,   246,   270,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,     0,   237,   238,   239,   240,     0,     0,
       0,     0,     0,     0,   241,     0,   261,   262,   263,     0,
       0,   264,     0,     0,   265,   266,   242,     0,     0,     0,
     243,     0,   244,   268,   556,   245,    38,     0,   269,   246,
     270,     0,   575,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,   242,     0,     0,     0,   243,     0,   244,   268,     0,
     245,    38,     0,   269,   246,   270,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,     0,   237,   238,   239,   240,   655,     0,
       0,     0,     0,     0,   241,     0,   261,   262,   263,     0,
       0,   264,     0,     0,   265,   266,   242,     0,     0,     0,
     243,     0,   244,   268,   636,   245,    38,     0,   269,   246,
     270,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,   242,     0,     0,     0,   243,     0,   244,   268,     0,
     245,    38,     0,   269,   246,   270,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,   334,   352,     0,     0,   189,     0,     0,
       0,     0,   190,    84,     0,     0,   261,   262,   263,    85,
       0,   264,     0,     0,   265,   266,     0,     0,     0,     0,
       0,   191,     0,   268,    86,     0,    38,     0,   269,     0,
     270,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     0,     0,     0,   353,   354,
     355,   356,   357,   192,     0,   358,   359,   360,   361,   352,
     362,   363,   364,     0,   365,     0,   366,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   193,     0,     0,     0,   194,     0,    37,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
       0,     0,     0,   195,     0,     0,   639,     0,     0,   379,
     380,     0,     0,   196,     0,     0,     0,   269,     0,     0,
       0,     0,    38,     0,     0,   394,   395,   396,   397,   398,
     399,   400,   401,   353,   354,   355,   356,   357,   402,   403,
     358,   359,   360,   361,   404,   362,   363,   364,   352,   365,
     405,   366,   367,   406,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   408,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,   409,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   395,   396,   397,   398,   399,
     400,   401,   353,   354,   355,   356,   357,   402,   403,   358,
     359,   360,   361,   404,   362,   363,   364,   352,   365,   405,
     366,   367,   406,   407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,     0,     0,     0,     0,
     597,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,   395,   396,   397,   398,   399,   400,
     401,   353,   354,   355,   356,   357,   402,   403,   358,   359,
     360,   361,   404,   362,   363,   364,   352,   365,   405,   366,
     367,   406,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     408,   368,     0,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,     0,     0,     0,     0,   623,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
       0,     0,   394,   395,   396,   397,   398,   399,   400,   401,
     353,   354,   355,   356,   357,   402,   403,   358,   359,   360,
     361,   404,   362,   363,   364,     0,   365,   405,   366,   367,
     406,   407,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   352,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,   353,   354,   355,   356,   357,   649,     0,
     358,   359,   360,   361,     0,   362,   363,   364,     0,   365,
       0,   366,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,   353,   354,   355,   356,
     357,   410,     0,   358,   359,   360,   361,     0,   362,   363,
     364,     0,   365,     0,   366,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,    84,     0,
       0,     0,     0,     0,    85,     0,     0,   379,   380,     0,
       0,     0,     0,     0,   667,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,     0,   251,   252,   253,     0,     0,     0,     0,
       0,     0,    37,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,   352,    38,     0,     0,
     189,     0,    84,     0,     0,   190,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,   163,     0,   352,    38,
     353,   354,   355,   356,   357,     0,   192,   358,   359,   360,
     361,     0,   362,   363,   364,     0,   365,     0,   366,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   193,     0,    37,     0,   194,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,     0,     0,     0,     0,   195,     0,     0,   671,
       0,   379,   380,     0,   352,   540,   196,     0,     0,     0,
       0,    38,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,   361,     0,   362,   363,   364,     0,   365,   189,
     366,   367,     0,     0,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,   191,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,     0,     0,     0,     0,     0,     0,
     352,     0,     0,   379,   380,     0,     0,   543,   353,   354,
     355,   356,   357,     0,     0,   358,   359,   360,   361,     0,
     362,   363,   364,     0,   365,   189,   366,   367,     0,     0,
     190,     0,     0,     0,   208,   192,     0,     0,     0,     0,
     209,     0,     0,     0,     0,     0,     0,     0,   368,   191,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
       0,     0,   211,   193,     0,     0,   352,   194,     0,   379,
     380,     0,     0,   545,   353,   354,   355,   356,   357,     0,
       0,   358,   359,   360,   361,   195,   362,   363,   364,     0,
     365,   391,   366,   367,     0,   196,     0,     0,     0,     0,
       0,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,     0,     0,     0,   193,
       0,     0,   352,   194,     0,   379,   380,     0,     0,   546,
     353,   354,   355,   356,   357,     0,     0,   358,   359,   360,
     361,   195,   362,   363,   364,     0,   365,   309,   366,   367,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,     0,     0,     0,     0,     0,     0,   352,     0,
       0,   379,   380,     0,     0,   601,   353,   354,   355,   356,
     357,     0,     0,   358,   359,   360,   361,     0,   362,   363,
     364,   189,   365,     0,   366,   367,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,     0,     0,
       0,     0,   352,     0,     0,     0,     0,   379,   380,     0,
       0,   666,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,   361,     0,   362,   363,   364,     0,   365,     0,
     366,   367,     0,     0,     0,     0,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,     0,   193,     0,     0,     0,   194,
       0,   308,     0,   379,   380,   381,   353,   354,   355,   356,
     357,   352,     0,   358,   359,   360,   361,   195,   362,   363,
     364,     0,   365,     0,   366,   367,     0,   196,     0,     0,
       0,     0,     0,     0,   189,   189,     0,     0,     0,   190,
     190,     0,     0,     0,     0,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   191,   191,
       0,     0,     0,     0,     0,     0,     0,   379,   380,   594,
       0,     0,     0,     0,     0,     0,     0,   394,   395,   396,
     397,   398,   399,   400,   401,   353,   354,   355,   356,   357,
     402,   403,   358,   359,   360,   361,   404,   362,   363,   364,
    -175,   365,   405,   366,   367,   406,   407,   352,     0,     0,
     192,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   408,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,     0,   193,   193,
       0,     0,   194,   194,   310,   313,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     195,   195,     0,   352,     0,     0,     0,     0,     0,     0,
     196,   196,     0,   394,   395,   396,   397,   398,   399,   400,
     401,   353,   354,   355,   356,   357,   402,   403,   358,   359,
     360,   361,   404,   362,   363,   364,     0,   365,   405,   366,
     367,   406,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
     408,   368,     0,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,     0,     0,     0,     0,   353,   354,   355,
     356,   357,   379,   380,   358,   359,   360,   361,     0,   362,
     363,   364,     0,   365,     0,   366,   367,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   352,     0,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,     0,
       0,     0,     0,   353,   354,   355,   356,   357,   379,   380,
     358,   359,   360,   361,     0,   362,   363,   364,     0,   365,
       0,   366,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   352,     0,   368,   593,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,     0,     0,     0,     0,   353,
     354,   355,   356,   357,   379,   380,   358,   359,   360,   361,
       0,   362,   363,   364,     0,   365,     0,   366,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   617,   352,     0,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,     0,     0,     0,     0,   353,   354,   355,   356,   357,
     379,   380,   358,   359,   360,   361,     0,   362,   363,   364,
       0,   365,     0,   366,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,     0,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,     0,     0,     0,
       0,   353,   354,   355,   356,   357,   379,   380,   358,   359,
     360,   361,     0,   362,   363,   364,     0,   365,     0,   366,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
       0,     0,     0,     0,   370,   371,   372,   373,   374,   375,
     376,   377,   378,     0,     0,     0,     0,   353,   354,   355,
     356,   357,   379,   380,   358,   359,   360,   361,     0,   362,
     363,   364,     0,   365,     0,   366,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,     0,     0,     0,     0,     0,     0,     0,
       0,   371,   372,   373,   374,   375,   376,   377,   378,     0,
       0,     0,     0,   353,   354,   355,   356,   357,   379,   380,
     358,   359,   360,   361,     0,   362,   363,   364,   189,   365,
       0,   366,   367,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,   191,   190,     0,     0,     0,     0,   372,   373,
     374,   375,   376,   377,   378,     0,   353,   354,   355,   356,
     357,     0,   191,   358,   379,   380,   361,     0,   362,   363,
     364,   189,   365,     0,   366,   367,   190,     0,     0,     0,
       0,     0,     0,     0,   189,   189,     0,     0,     0,   190,
     190,     0,   189,     0,   192,   191,     0,   190,     0,     0,
       0,   372,   373,   374,   375,   376,   377,   378,   191,   191,
       0,     0,     0,     0,   192,   189,   191,   379,   380,     0,
     190,     0,   193,     0,     0,     0,   194,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
       0,     0,   193,     0,   195,     0,   194,   192,   317,     0,
       0,     0,     0,     0,   196,     0,     0,     0,     0,     0,
     192,   192,     0,     0,   195,     0,     0,     0,   192,     0,
       0,     0,     0,     0,   196,   193,     0,     0,     0,   194,
       0,   534,     0,     0,     0,     0,     0,     0,   193,   193,
       0,   192,   194,   194,   541,   542,   193,   195,     0,     0,
     194,     0,   544,     0,     0,     0,     0,   196,     0,     0,
     195,   195,     0,     0,     0,     0,     0,     0,   195,   193,
     196,   196,     0,   194,     0,     0,     0,     0,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   196,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   146,
     147,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,     0,     0,     0,   159,   160
};

static const yytype_int16 yycheck[] =
{
      11,    55,    69,   348,   213,   205,    59,   336,   253,    55,
      17,   224,    42,   226,   382,   228,    29,   200,   185,   186,
     187,   480,    49,    42,   103,   107,     0,   124,   114,   124,
     102,     5,    18,   105,   124,    17,    30,    12,    13,   124,
     155,   155,   130,   130,    55,   160,   160,   131,   416,    60,
     207,    25,   131,    27,   130,    29,   103,    31,   124,   155,
     103,   138,    36,   118,   160,   162,   109,   162,    42,   159,
     130,   156,   160,   160,    48,   161,   160,   133,   160,   155,
     159,   153,   154,   160,   131,   130,    28,   130,   131,   298,
     133,    33,   130,   552,   124,    28,   130,   157,    12,    13,
      94,    95,   126,   127,     9,   124,    92,    93,    94,    95,
      52,   124,   157,   124,    19,   142,   102,   155,   104,   105,
     106,   155,   108,   130,   110,   111,   153,   181,   182,   183,
     124,   185,   186,   187,   188,   181,   182,   183,   157,   185,
     186,   187,    18,   118,   124,   161,   159,   122,   130,   124,
     125,   130,   206,   139,   140,   141,   142,   143,   212,   131,
     206,   103,   104,   124,   124,   130,   212,   153,   154,   124,
     181,   182,   183,   130,   185,   186,   187,   188,   157,   547,
     154,   156,   349,   392,   157,   239,   160,   160,   388,   131,
     132,   195,   157,   239,   136,   206,   441,   251,   252,   130,
     157,   212,   385,   136,   118,   251,   252,   130,   122,   213,
     124,   125,   154,   130,   218,   154,   161,   160,    94,    95,
     549,   154,   164,   162,   592,   159,   102,   556,   239,   105,
     106,   164,   108,   131,   157,   288,   126,   127,   128,   124,
     251,   252,   451,   297,   156,   249,   250,   124,     4,   253,
       6,   297,   620,   264,   160,   309,   162,   261,   262,   263,
     103,   265,   266,   309,   268,   319,   479,   334,   161,   160,
     163,   162,   326,   327,   160,   329,   162,   153,   154,   118,
     326,   327,   160,   329,   162,   133,   297,   130,   131,   124,
     133,   124,   124,   161,   298,    30,   453,    30,   309,   156,
     126,   137,   156,   137,   133,   124,   651,   137,   319,   137,
     137,   124,   137,   137,   161,   326,   327,   124,   329,   157,
     136,   325,   137,   137,   328,   156,   330,   331,   137,    39,
     107,   335,   336,   155,   138,   156,   156,   160,   156,   156,
     138,   138,   154,   124,   348,   156,    16,   130,   124,   353,
     354,   124,   124,   357,   358,   359,   360,   566,   362,   130,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   162,   380,   126,   160,   118,
      32,   160,   160,   450,    18,   160,   160,    17,   392,   156,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   160,   563,   564,   608,   124,
     124,   124,   155,   417,   155,   572,    11,   155,   155,   118,
      35,   133,   138,   155,    45,   429,   157,   161,   155,   586,
     484,   155,   155,   437,    46,    49,   440,   441,   484,   340,
     444,   669,    47,    60,   448,   449,   216,   451,   628,     1,
     454,   455,   393,   294,   319,   459,   302,   584,    -1,   239,
      94,    95,   270,   555,   336,    -1,    -1,   624,   102,    18,
     104,   105,   106,   484,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   537,    -1,    -1,    -1,    -1,   645,    -1,
      -1,   537,    -1,    -1,    18,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,   662,   663,   141,   142,   143,
      -1,   515,    -1,    -1,    -1,    -1,    -1,   584,    -1,   153,
     154,    -1,    52,    -1,    -1,    -1,   537,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   541,   542,    -1,
     544,    -1,    -1,    -1,   548,    94,    95,    -1,    -1,   553,
      -1,   555,    -1,   102,    -1,   104,   105,   106,    -1,   108,
      -1,    -1,   566,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    -1,   104,    99,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   589,   110,   111,    -1,   593,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,   655,   132,    -1,   153,   154,   136,    -1,   132,   655,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   154,    -1,   156,   631,    -1,   153,
     154,    -1,    -1,   157,   164,    -1,   640,   641,    -1,   643,
      -1,    -1,   646,    -1,   655,    -1,    -1,   651,    -1,     1,
      -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,
      12,    13,    14,    15,    18,    -1,    -1,    -1,   672,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    44,    -1,    46,    47,    -1,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    94,    95,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,   144,    -1,    -1,   147,   148,    -1,    -1,   153,
     154,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,   163,     1,    -1,    -1,    -1,     5,    -1,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    44,    -1,    46,    47,    -1,
      -1,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
     159,   160,   161,   162,   163,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    38,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    52,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,
      -1,    -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,
     147,   148,    -1,    12,    13,    14,    15,   154,    -1,   156,
      -1,    -1,   159,    22,   161,    -1,   163,   164,    -1,    28,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,
      -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    52,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    12,    13,
      14,    15,    -1,   132,    -1,    -1,    -1,   136,    22,    -1,
     139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,
      34,    -1,    -1,    -1,    38,   154,    40,   156,    -1,    43,
     159,    -1,   161,    47,   163,   164,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,
     144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,
     154,    40,   156,   157,    43,   159,    -1,   161,    47,   163,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,
      -1,    12,    13,    14,    15,    -1,    -1,   156,    -1,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,   140,
     141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,    -1,
      -1,    -1,    38,    -1,    40,   156,    -1,    43,   159,   160,
     161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,   144,    -1,
      -1,   147,   148,    -1,    12,    13,    14,    15,    -1,   155,
     156,    -1,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      38,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    34,    -1,    -1,    -1,    38,    -1,    40,   156,   157,
      43,   159,    -1,   161,    47,   163,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,
      38,    -1,    40,   156,   157,    43,   159,    -1,   161,    47,
     163,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    34,    -1,    -1,    -1,    38,    -1,    40,   156,   157,
      43,   159,    -1,   161,    47,   163,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,
      38,    -1,    40,   156,   157,    43,   159,    -1,   161,    47,
     163,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    34,    -1,    -1,    -1,    38,    -1,    40,   156,   157,
      43,   159,    -1,   161,    47,   163,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,
      38,    -1,    40,   156,   157,    43,   159,    -1,   161,    47,
     163,    -1,    50,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    34,    -1,    -1,    -1,    38,    -1,    40,   156,    -1,
      43,   159,    -1,   161,    47,   163,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,
      38,    -1,    40,   156,   157,    43,   159,    -1,   161,    47,
     163,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    34,    -1,    -1,    -1,    38,    -1,    40,   156,    -1,
      43,   159,    -1,   161,    47,   163,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    10,    18,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    33,    20,    -1,    -1,   139,   140,   141,    26,
      -1,   144,    -1,    -1,   147,   148,    -1,    -1,    -1,    -1,
      -1,    52,    -1,   156,    41,    -1,   159,    -1,   161,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    92,    93,
      94,    95,    96,   104,    -1,    99,   100,   101,   102,    18,
     104,   105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,    -1,   124,   132,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,    -1,    -1,   154,    -1,    -1,   157,    -1,    -1,   153,
     154,    -1,    -1,   164,    -1,    -1,    -1,   161,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    18,   108,
     109,   110,   111,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    18,   108,   109,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    18,   108,   109,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,    92,    93,    94,    95,    96,   160,    -1,
      99,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    92,    93,    94,    95,
      96,   160,    -1,    99,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    20,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,   153,   154,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    -1,    18,   159,    -1,    -1,
      28,    -1,    20,    -1,    -1,    33,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    -1,   156,    -1,    18,   159,
      92,    93,    94,    95,    96,    -1,   104,    99,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,   124,    -1,   136,    -1,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   154,    -1,    -1,   157,
      -1,   153,   154,    -1,    18,   157,   164,    -1,    -1,    -1,
      -1,   159,    92,    93,    94,    95,    96,    -1,    -1,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,    28,
     110,   111,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    52,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,   153,   154,    -1,    -1,   157,    92,    93,
      94,    95,    96,    -1,    -1,    99,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,    28,   110,   111,    -1,    -1,
      33,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    52,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,    -1,   131,   132,    -1,    -1,    18,   136,    -1,   153,
     154,    -1,    -1,   157,    92,    93,    94,    95,    96,    -1,
      -1,    99,   100,   101,   102,   154,   104,   105,   106,    -1,
     108,   160,   110,   111,    -1,   164,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,   132,
      -1,    -1,    18,   136,    -1,   153,   154,    -1,    -1,   157,
      92,    93,    94,    95,    96,    -1,    -1,    99,   100,   101,
     102,   154,   104,   105,   106,    -1,   108,   160,   110,   111,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,   153,   154,    -1,    -1,   157,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,   101,   102,    -1,   104,   105,
     106,    28,   108,    -1,   110,   111,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   153,   154,    -1,
      -1,   157,    92,    93,    94,    95,    96,    -1,    -1,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,   132,    -1,    -1,    -1,   136,
      -1,   138,    -1,   153,   154,   155,    92,    93,    94,    95,
      96,    18,    -1,    99,   100,   101,   102,   154,   104,   105,
     106,    -1,   108,    -1,   110,   111,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    28,    -1,    -1,    -1,    33,
      33,    -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    52,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    18,    -1,    -1,
     104,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,   132,   132,
      -1,    -1,   136,   136,   138,   138,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   154,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
     164,   164,    -1,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
     131,   132,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,   153,   154,    99,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,   153,   154,
      99,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,   153,   154,    99,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    18,    -1,   132,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
     153,   154,    99,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,   153,   154,    99,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,   153,   154,    99,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,   153,   154,
      99,   100,   101,   102,    -1,   104,   105,   106,    28,   108,
      -1,   110,   111,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    52,    33,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,    -1,    92,    93,    94,    95,
      96,    -1,    52,    99,   153,   154,   102,    -1,   104,   105,
     106,    28,   108,    -1,   110,   111,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    28,    -1,    -1,    -1,    33,
      33,    -1,    28,    -1,   104,    52,    -1,    33,    -1,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,    52,    52,
      -1,    -1,    -1,    -1,   104,    28,    52,   153,   154,    -1,
      33,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,   132,    -1,   154,    -1,   136,   104,   138,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
     104,   104,    -1,    -1,   154,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,   164,   132,    -1,    -1,    -1,   136,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,   132,   132,
      -1,   104,   136,   136,   138,   138,   132,   154,    -1,    -1,
     136,    -1,   138,    -1,    -1,    -1,    -1,   164,    -1,    -1,
     154,   154,    -1,    -1,    -1,    -1,    -1,    -1,   154,   132,
     164,   164,    -1,   136,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,   147,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   166,     0,     5,    25,    27,    29,    31,    36,    42,
      48,   154,   160,   167,   172,   173,   177,   194,   198,   211,
     236,   238,   239,   242,   124,   189,   190,   118,   178,   179,
     124,   174,   175,   124,   124,   124,    29,   124,   159,   191,
     192,   193,   203,     4,     6,   161,   131,   130,   133,   130,
      49,   142,   153,   176,   161,   131,   160,   159,   124,   156,
     130,   155,   124,   241,    30,    94,    95,   124,   197,   235,
      12,    13,   118,   122,   124,   125,   156,   169,   189,   118,
     179,   124,   124,   237,    20,    26,    41,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   203,   244,   245,   246,
     247,   124,   190,   192,   133,   240,   161,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   110,   111,   112,   113,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   147,
     148,    30,    30,   156,   195,   124,   162,   234,   243,   126,
     168,    12,    13,   118,   122,   124,   169,   187,   188,   124,
     162,   137,   137,   137,   156,   137,   137,   137,   137,    28,
      33,    52,   104,   132,   136,   154,   164,   157,   203,   226,
      42,   157,   227,   228,   232,   243,   133,   196,   103,   109,
     130,   131,   133,   233,   126,   127,   130,   157,   131,   160,
     247,   247,   247,   124,   195,   247,   195,   247,   195,   247,
     229,   230,   247,    28,   136,   154,   164,    12,    13,    14,
      15,    22,    34,    38,    40,    43,    47,    76,    82,    94,
      95,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     125,   139,   140,   141,   144,   147,   148,   155,   156,   161,
     163,   171,   199,   203,   205,   213,   214,   217,   218,   220,
     221,   222,   244,   248,   251,   256,   258,   260,   154,   162,
     223,   225,   232,   157,   160,   103,   131,   133,   231,   247,
     199,   124,   247,   202,   219,   222,   187,   222,   138,   160,
     138,   157,   196,   138,   196,   138,   196,   138,   138,   160,
     136,   155,   247,   248,   156,   156,   137,   137,   156,   137,
     156,   156,   222,   222,    10,   247,   247,   222,   253,   257,
     170,   222,   222,   222,   203,   222,   222,   222,   201,   194,
     124,   156,    18,    92,    93,    94,    95,    96,    99,   100,
     101,   102,   104,   105,   106,   108,   110,   111,   132,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   153,
     154,   155,   156,   190,    39,   224,   160,   227,   247,   202,
     222,   160,   233,   107,    84,    85,    86,    87,    88,    89,
      90,    91,    97,    98,   103,   109,   112,   113,   131,   160,
     160,   247,   138,   138,   138,   229,   156,   124,   222,   247,
     247,   222,   247,   157,   222,   157,   222,   243,   124,   156,
     222,   249,   250,   252,   253,   254,   255,   156,   250,   255,
     114,   160,   162,   127,   128,   168,   157,     1,     7,     8,
      10,    21,    23,    24,    33,    37,    44,    46,    50,    51,
     160,   162,   180,   181,   183,   184,   185,   186,   200,   202,
     204,   206,   207,   208,   209,   211,   212,   219,   222,   195,
     154,   157,   215,   222,    16,   222,   222,   222,   222,   222,
     222,   124,   222,   124,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   124,
     222,   157,   215,   155,   232,   231,   202,   222,   217,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   138,   157,   215,    16,   137,   222,
     157,   138,   138,   157,   138,   157,   157,    17,   231,   157,
     130,   155,   160,   130,   155,   160,   157,   162,   162,   222,
     253,   162,   222,   222,   222,   243,   103,   160,   202,   222,
     199,   222,   222,   160,   118,    50,   222,    32,   160,   160,
     160,   160,   160,    17,   210,   160,   196,   124,   249,   130,
     157,   247,   156,   133,   155,   157,   222,   160,   157,   247,
     124,   157,   222,   222,   222,   215,   222,   250,   124,   155,
     249,   222,   155,   254,   250,   155,   155,   129,   199,   199,
      17,   202,   222,   160,    11,   199,   133,   118,   161,    35,
     234,   114,   199,   216,   155,   222,   157,   215,   222,   157,
     138,   160,   155,   231,   162,     9,    19,   182,   215,   160,
     199,   201,   222,   157,   157,    16,   222,   222,   155,   222,
     155,   199,   222,   160,   162,   247,   157,   160,   259,   199,
     199,   157,    45,   155,   182,   222,   155
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   165,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   167,   168,   168,   169,   170,   170,   170,
     171,   172,   173,   174,   174,   174,   175,   176,   176,   177,
     178,   178,   179,   179,   180,   181,   181,   182,   182,   182,
     183,   184,   185,   186,   187,   187,   187,   187,   187,   187,
     188,   188,   189,   189,   189,   189,   189,   189,   189,   189,
     190,   190,   191,   191,   192,   192,   193,   193,   194,   194,
     195,   195,   195,   196,   196,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   198,   199,   199,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   201,   201,   201,   202,   203,   203,   203,   204,   205,
     205,   205,   205,   206,   207,   208,   208,   208,   208,   208,
     209,   210,   210,   211,   211,   212,   213,   213,   213,   214,
     214,   214,   214,   215,   215,   216,   216,   217,   217,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   220,   221,   221,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   223,   224,   224,   225,
     226,   226,   227,   227,   228,   228,   229,   230,   230,   231,
     231,   232,   232,   232,   232,   232,   233,   233,   233,   234,
     234,   234,   234,   234,   235,   235,   236,   237,   237,   237,
     238,   239,   240,   240,   241,   242,   243,   243,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   246,   246,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   248,   248,   248,   248,   249,   249,
     250,   250,   251,   251,   251,   251,   252,   252,   253,   254,
     254,   255,   255,   256,   256,   257,   257,   258,   259,   259,
     260
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     0,     1,     2,
       1,     3,     1,     3,     3,     3,     2,     0,     2,     4,
       4,     6,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     5,
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
       4,     3,     4,     3,     5,     4,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     3,     0,     1,     3,
       0,     3,     1,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     3,     5,     3,     3,     1,     1,     1,     4,
       6,     5,     4,     3,     0,     2,     4,     0,     3,     5,
       4,     5,     0,     2,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     4,     3,
       4,     2,     3,     2,     4,     3,     2,     2,     3,     2,
       2,     4,     6,     4,     1,     4,     5,     1,     4,     5,
       1,     4,     5,     4,     1,     1,     1,     1,     3,     5,
       1,     3,     5,     7,     5,     7,     1,     3,     3,     1,
       1,     1,     3,     5,     5,     1,     3,     4,     0,     3,
      10
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
    case 124: /* "name"  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2706 "ds_parser.cpp"
        break;

    case 168: /* character_sequence  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2712 "ds_parser.cpp"
        break;

    case 169: /* string_constant  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2718 "ds_parser.cpp"
        break;

    case 170: /* string_builder_body  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2724 "ds_parser.cpp"
        break;

    case 171: /* string_builder  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2730 "ds_parser.cpp"
        break;

    case 174: /* require_module_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2736 "ds_parser.cpp"
        break;

    case 180: /* expression_label  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2742 "ds_parser.cpp"
        break;

    case 181: /* expression_goto  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2748 "ds_parser.cpp"
        break;

    case 182: /* expression_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2754 "ds_parser.cpp"
        break;

    case 183: /* expression_if_then_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2760 "ds_parser.cpp"
        break;

    case 184: /* expression_for_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2766 "ds_parser.cpp"
        break;

    case 185: /* expression_while_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2772 "ds_parser.cpp"
        break;

    case 186: /* expression_with  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2778 "ds_parser.cpp"
        break;

    case 187: /* annotation_argument_value  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2784 "ds_parser.cpp"
        break;

    case 188: /* annotation_argument_value_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2790 "ds_parser.cpp"
        break;

    case 189: /* annotation_argument  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2796 "ds_parser.cpp"
        break;

    case 190: /* annotation_argument_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2802 "ds_parser.cpp"
        break;

    case 191: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2808 "ds_parser.cpp"
        break;

    case 192: /* annotation_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2814 "ds_parser.cpp"
        break;

    case 193: /* annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2820 "ds_parser.cpp"
        break;

    case 194: /* optional_annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2826 "ds_parser.cpp"
        break;

    case 195: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2832 "ds_parser.cpp"
        break;

    case 196: /* optional_function_type  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2838 "ds_parser.cpp"
        break;

    case 197: /* function_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2844 "ds_parser.cpp"
        break;

    case 199: /* expression_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2850 "ds_parser.cpp"
        break;

    case 200: /* expression_any  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2856 "ds_parser.cpp"
        break;

    case 201: /* expressions  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2862 "ds_parser.cpp"
        break;

    case 202: /* expr_pipe  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2868 "ds_parser.cpp"
        break;

    case 203: /* name_in_namespace  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2874 "ds_parser.cpp"
        break;

    case 204: /* expression_delete  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2880 "ds_parser.cpp"
        break;

    case 205: /* expr_new  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2886 "ds_parser.cpp"
        break;

    case 206: /* expression_break  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2892 "ds_parser.cpp"
        break;

    case 207: /* expression_continue  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2898 "ds_parser.cpp"
        break;

    case 208: /* expression_return  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2904 "ds_parser.cpp"
        break;

    case 209: /* expression_try_catch  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2910 "ds_parser.cpp"
        break;

    case 212: /* expression_let  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2916 "ds_parser.cpp"
        break;

    case 213: /* expr_cast  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2922 "ds_parser.cpp"
        break;

    case 214: /* expr_type_info  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2928 "ds_parser.cpp"
        break;

    case 215: /* expr_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2934 "ds_parser.cpp"
        break;

    case 216: /* block_or_simple_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2940 "ds_parser.cpp"
        break;

    case 217: /* expr_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2946 "ds_parser.cpp"
        break;

    case 218: /* expr_numeric_const  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2952 "ds_parser.cpp"
        break;

    case 219: /* expr_assign  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2958 "ds_parser.cpp"
        break;

    case 220: /* expr_named_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2964 "ds_parser.cpp"
        break;

    case 221: /* expr_method_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2970 "ds_parser.cpp"
        break;

    case 222: /* expr  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2976 "ds_parser.cpp"
        break;

    case 223: /* optional_field_annotation  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2982 "ds_parser.cpp"
        break;

    case 225: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2988 "ds_parser.cpp"
        break;

    case 226: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2994 "ds_parser.cpp"
        break;

    case 227: /* function_argument_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3000 "ds_parser.cpp"
        break;

    case 228: /* function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3006 "ds_parser.cpp"
        break;

    case 229: /* tuple_type  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3012 "ds_parser.cpp"
        break;

    case 230: /* tuple_type_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3018 "ds_parser.cpp"
        break;

    case 232: /* variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3024 "ds_parser.cpp"
        break;

    case 234: /* let_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3030 "ds_parser.cpp"
        break;

    case 235: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3036 "ds_parser.cpp"
        break;

    case 237: /* enum_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3042 "ds_parser.cpp"
        break;

    case 240: /* optional_structure_parent  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3048 "ds_parser.cpp"
        break;

    case 243: /* variable_name_list  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3054 "ds_parser.cpp"
        break;

    case 245: /* structure_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3060 "ds_parser.cpp"
        break;

    case 246: /* auto_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3066 "ds_parser.cpp"
        break;

    case 247: /* type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3072 "ds_parser.cpp"
        break;

    case 248: /* make_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3078 "ds_parser.cpp"
        break;

    case 249: /* make_struct_fields  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3084 "ds_parser.cpp"
        break;

    case 250: /* make_struct_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3090 "ds_parser.cpp"
        break;

    case 251: /* make_struct_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3096 "ds_parser.cpp"
        break;

    case 252: /* make_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3102 "ds_parser.cpp"
        break;

    case 253: /* make_map_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3108 "ds_parser.cpp"
        break;

    case 254: /* make_any_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3114 "ds_parser.cpp"
        break;

    case 255: /* make_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3120 "ds_parser.cpp"
        break;

    case 256: /* make_dim_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3126 "ds_parser.cpp"
        break;

    case 257: /* make_table  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp"
        break;

    case 258: /* make_table_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3138 "ds_parser.cpp"
        break;

    case 259: /* array_comprehension_where  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3144 "ds_parser.cpp"
        break;

    case 260: /* array_comprehension  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3150 "ds_parser.cpp"
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
#line 377 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3450 "ds_parser.cpp"
    break;

  case 14:
#line 389 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3456 "ds_parser.cpp"
    break;

  case 15:
#line 390 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3462 "ds_parser.cpp"
    break;

  case 16:
#line 394 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3468 "ds_parser.cpp"
    break;

  case 17:
#line 398 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3477 "ds_parser.cpp"
    break;

  case 18:
#line 402 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3491 "ds_parser.cpp"
    break;

  case 19:
#line 411 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3501 "ds_parser.cpp"
    break;

  case 20:
#line 419 "ds_parser.ypp"
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
#line 3519 "ds_parser.cpp"
    break;

  case 21:
#line 435 "ds_parser.ypp"
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
#line 3542 "ds_parser.cpp"
    break;

  case 23:
#line 460 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3550 "ds_parser.cpp"
    break;

  case 24:
#line 463 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3561 "ds_parser.cpp"
    break;

  case 25:
#line 469 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3572 "ds_parser.cpp"
    break;

  case 26:
#line 478 "ds_parser.ypp"
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-1].s), g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.first) ) {
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
        } else {
            das_yyerror("required module not found " + *(yyvsp[-1].s),tokAt((yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 3587 "ds_parser.cpp"
    break;

  case 27:
#line 491 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3593 "ds_parser.cpp"
    break;

  case 28:
#line 492 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3599 "ds_parser.cpp"
    break;

  case 32:
#line 505 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3607 "ds_parser.cpp"
    break;

  case 33:
#line 508 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3615 "ds_parser.cpp"
    break;

  case 34:
#line 514 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3623 "ds_parser.cpp"
    break;

  case 35:
#line 520 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3631 "ds_parser.cpp"
    break;

  case 36:
#line 523 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3639 "ds_parser.cpp"
    break;

  case 37:
#line 529 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3645 "ds_parser.cpp"
    break;

  case 38:
#line 530 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3651 "ds_parser.cpp"
    break;

  case 39:
#line 531 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3660 "ds_parser.cpp"
    break;

  case 40:
#line 538 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3669 "ds_parser.cpp"
    break;

  case 41:
#line 545 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3683 "ds_parser.cpp"
    break;

  case 42:
#line 557 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3695 "ds_parser.cpp"
    break;

  case 43:
#line 567 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3706 "ds_parser.cpp"
    break;

  case 44:
#line 576 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3712 "ds_parser.cpp"
    break;

  case 45:
#line 577 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3718 "ds_parser.cpp"
    break;

  case 46:
#line 578 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3724 "ds_parser.cpp"
    break;

  case 47:
#line 579 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3730 "ds_parser.cpp"
    break;

  case 48:
#line 580 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3736 "ds_parser.cpp"
    break;

  case 49:
#line 581 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3742 "ds_parser.cpp"
    break;

  case 50:
#line 585 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3752 "ds_parser.cpp"
    break;

  case 51:
#line 590 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3762 "ds_parser.cpp"
    break;

  case 52:
#line 598 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3768 "ds_parser.cpp"
    break;

  case 53:
#line 599 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3774 "ds_parser.cpp"
    break;

  case 54:
#line 600 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3780 "ds_parser.cpp"
    break;

  case 55:
#line 601 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3786 "ds_parser.cpp"
    break;

  case 56:
#line 602 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3792 "ds_parser.cpp"
    break;

  case 57:
#line 603 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3798 "ds_parser.cpp"
    break;

  case 58:
#line 604 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3804 "ds_parser.cpp"
    break;

  case 59:
#line 605 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3812 "ds_parser.cpp"
    break;

  case 60:
#line 611 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
		if ( (yyvsp[0].aa)->type==Type::none ) {
			for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
				sarg.name = (yyvsp[0].aa)->name;
				(yyval.aaList)->push_back(sarg);
			}
			delete (yyvsp[0].aa)->aList;
		} else {
			(yyval.aaList)->push_back(*(yyvsp[0].aa));
		}
        delete (yyvsp[0].aa);
    }
#line 3830 "ds_parser.cpp"
    break;

  case 61:
#line 624 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
			if ( (yyvsp[0].aa)->type==Type::none ) {
				for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
					sarg.name = (yyvsp[0].aa)->name;
					(yyval.aaList)->push_back(sarg);
				}
				delete (yyvsp[0].aa)->aList;
			} else {
				(yyval.aaList)->push_back(*(yyvsp[0].aa));
			}
            delete (yyvsp[0].aa);
    }
#line 3848 "ds_parser.cpp"
    break;

  case 62:
#line 640 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 3854 "ds_parser.cpp"
    break;

  case 63:
#line 641 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 3860 "ds_parser.cpp"
    break;

  case 64:
#line 645 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3872 "ds_parser.cpp"
    break;

  case 65:
#line 652 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3886 "ds_parser.cpp"
    break;

  case 66:
#line 664 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3895 "ds_parser.cpp"
    break;

  case 67:
#line 668 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3904 "ds_parser.cpp"
    break;

  case 68:
#line 675 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 3910 "ds_parser.cpp"
    break;

  case 69:
#line 676 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3916 "ds_parser.cpp"
    break;

  case 70:
#line 680 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3922 "ds_parser.cpp"
    break;

  case 71:
#line 681 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3928 "ds_parser.cpp"
    break;

  case 72:
#line 682 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3934 "ds_parser.cpp"
    break;

  case 73:
#line 686 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3943 "ds_parser.cpp"
    break;

  case 74:
#line 690 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3952 "ds_parser.cpp"
    break;

  case 75:
#line 697 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3961 "ds_parser.cpp"
    break;

  case 76:
#line 701 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 3967 "ds_parser.cpp"
    break;

  case 77:
#line 702 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 3973 "ds_parser.cpp"
    break;

  case 78:
#line 703 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 3979 "ds_parser.cpp"
    break;

  case 79:
#line 704 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 3985 "ds_parser.cpp"
    break;

  case 80:
#line 705 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 3991 "ds_parser.cpp"
    break;

  case 81:
#line 706 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 3997 "ds_parser.cpp"
    break;

  case 82:
#line 707 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4003 "ds_parser.cpp"
    break;

  case 83:
#line 708 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4009 "ds_parser.cpp"
    break;

  case 84:
#line 709 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4015 "ds_parser.cpp"
    break;

  case 85:
#line 710 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4021 "ds_parser.cpp"
    break;

  case 86:
#line 711 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4027 "ds_parser.cpp"
    break;

  case 87:
#line 712 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4033 "ds_parser.cpp"
    break;

  case 88:
#line 713 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4039 "ds_parser.cpp"
    break;

  case 89:
#line 714 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4045 "ds_parser.cpp"
    break;

  case 90:
#line 715 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4051 "ds_parser.cpp"
    break;

  case 91:
#line 716 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4057 "ds_parser.cpp"
    break;

  case 92:
#line 717 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4063 "ds_parser.cpp"
    break;

  case 93:
#line 718 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4069 "ds_parser.cpp"
    break;

  case 94:
#line 719 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4075 "ds_parser.cpp"
    break;

  case 95:
#line 720 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4081 "ds_parser.cpp"
    break;

  case 96:
#line 721 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4087 "ds_parser.cpp"
    break;

  case 97:
#line 722 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4093 "ds_parser.cpp"
    break;

  case 98:
#line 723 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4099 "ds_parser.cpp"
    break;

  case 99:
#line 724 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4105 "ds_parser.cpp"
    break;

  case 100:
#line 725 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4111 "ds_parser.cpp"
    break;

  case 101:
#line 726 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4117 "ds_parser.cpp"
    break;

  case 102:
#line 727 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4123 "ds_parser.cpp"
    break;

  case 103:
#line 728 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4129 "ds_parser.cpp"
    break;

  case 104:
#line 729 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4135 "ds_parser.cpp"
    break;

  case 105:
#line 730 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4141 "ds_parser.cpp"
    break;

  case 106:
#line 731 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4147 "ds_parser.cpp"
    break;

  case 107:
#line 732 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4153 "ds_parser.cpp"
    break;

  case 108:
#line 733 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4159 "ds_parser.cpp"
    break;

  case 109:
#line 734 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4165 "ds_parser.cpp"
    break;

  case 110:
#line 735 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4171 "ds_parser.cpp"
    break;

  case 111:
#line 736 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4177 "ds_parser.cpp"
    break;

  case 112:
#line 741 "ds_parser.ypp"
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
#line 4248 "ds_parser.cpp"
    break;

  case 113:
#line 810 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4256 "ds_parser.cpp"
    break;

  case 114:
#line 813 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4268 "ds_parser.cpp"
    break;

  case 115:
#line 823 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4274 "ds_parser.cpp"
    break;

  case 116:
#line 824 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4280 "ds_parser.cpp"
    break;

  case 117:
#line 825 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4286 "ds_parser.cpp"
    break;

  case 118:
#line 826 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4292 "ds_parser.cpp"
    break;

  case 119:
#line 827 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4298 "ds_parser.cpp"
    break;

  case 120:
#line 828 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4304 "ds_parser.cpp"
    break;

  case 121:
#line 829 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4310 "ds_parser.cpp"
    break;

  case 122:
#line 830 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4316 "ds_parser.cpp"
    break;

  case 123:
#line 831 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4322 "ds_parser.cpp"
    break;

  case 124:
#line 832 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4328 "ds_parser.cpp"
    break;

  case 125:
#line 833 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4334 "ds_parser.cpp"
    break;

  case 126:
#line 834 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4340 "ds_parser.cpp"
    break;

  case 127:
#line 835 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4346 "ds_parser.cpp"
    break;

  case 128:
#line 836 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4352 "ds_parser.cpp"
    break;

  case 129:
#line 837 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4358 "ds_parser.cpp"
    break;

  case 130:
#line 838 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4364 "ds_parser.cpp"
    break;

  case 131:
#line 842 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4373 "ds_parser.cpp"
    break;

  case 132:
#line 846 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4384 "ds_parser.cpp"
    break;

  case 133:
#line 852 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4392 "ds_parser.cpp"
    break;

  case 134:
#line 858 "ds_parser.ypp"
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
#line 4408 "ds_parser.cpp"
    break;

  case 135:
#line 872 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4414 "ds_parser.cpp"
    break;

  case 136:
#line 873 "ds_parser.ypp"
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4425 "ds_parser.cpp"
    break;

  case 137:
#line 879 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4431 "ds_parser.cpp"
    break;

  case 138:
#line 883 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4439 "ds_parser.cpp"
    break;

  case 139:
#line 889 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4447 "ds_parser.cpp"
    break;

  case 140:
#line 892 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4455 "ds_parser.cpp"
    break;

  case 141:
#line 895 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4464 "ds_parser.cpp"
    break;

  case 142:
#line 899 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4472 "ds_parser.cpp"
    break;

  case 143:
#line 905 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4478 "ds_parser.cpp"
    break;

  case 144:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4484 "ds_parser.cpp"
    break;

  case 145:
#line 913 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4492 "ds_parser.cpp"
    break;

  case 146:
#line 916 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4500 "ds_parser.cpp"
    break;

  case 147:
#line 919 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4510 "ds_parser.cpp"
    break;

  case 148:
#line 924 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4518 "ds_parser.cpp"
    break;

  case 149:
#line 927 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4528 "ds_parser.cpp"
    break;

  case 150:
#line 935 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4536 "ds_parser.cpp"
    break;

  case 151:
#line 941 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4542 "ds_parser.cpp"
    break;

  case 152:
#line 942 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4548 "ds_parser.cpp"
    break;

  case 153:
#line 946 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4554 "ds_parser.cpp"
    break;

  case 154:
#line 947 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4560 "ds_parser.cpp"
    break;

  case 155:
#line 951 "ds_parser.ypp"
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
#line 4595 "ds_parser.cpp"
    break;

  case 156:
#line 984 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4603 "ds_parser.cpp"
    break;

  case 157:
#line 987 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4613 "ds_parser.cpp"
    break;

  case 158:
#line 992 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4623 "ds_parser.cpp"
    break;

  case 159:
#line 1000 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4632 "ds_parser.cpp"
    break;

  case 160:
#line 1004 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4642 "ds_parser.cpp"
    break;

  case 161:
#line 1009 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4651 "ds_parser.cpp"
    break;

  case 162:
#line 1013 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4661 "ds_parser.cpp"
    break;

  case 163:
#line 1021 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4669 "ds_parser.cpp"
    break;

  case 164:
#line 1024 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4677 "ds_parser.cpp"
    break;

  case 165:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4683 "ds_parser.cpp"
    break;

  case 166:
#line 1031 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4695 "ds_parser.cpp"
    break;

  case 167:
#line 1041 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4705 "ds_parser.cpp"
    break;

  case 168:
#line 1047 "ds_parser.ypp"
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
#line 4758 "ds_parser.cpp"
    break;

  case 169:
#line 1098 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4764 "ds_parser.cpp"
    break;

  case 170:
#line 1099 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4770 "ds_parser.cpp"
    break;

  case 171:
#line 1100 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4776 "ds_parser.cpp"
    break;

  case 172:
#line 1101 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4782 "ds_parser.cpp"
    break;

  case 173:
#line 1102 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4788 "ds_parser.cpp"
    break;

  case 174:
#line 1103 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4794 "ds_parser.cpp"
    break;

  case 175:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4800 "ds_parser.cpp"
    break;

  case 176:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4806 "ds_parser.cpp"
    break;

  case 177:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4812 "ds_parser.cpp"
    break;

  case 178:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4818 "ds_parser.cpp"
    break;

  case 179:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4824 "ds_parser.cpp"
    break;

  case 180:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4830 "ds_parser.cpp"
    break;

  case 181:
#line 1113 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4836 "ds_parser.cpp"
    break;

  case 182:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4842 "ds_parser.cpp"
    break;

  case 183:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4848 "ds_parser.cpp"
    break;

  case 184:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4854 "ds_parser.cpp"
    break;

  case 185:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4860 "ds_parser.cpp"
    break;

  case 186:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4866 "ds_parser.cpp"
    break;

  case 187:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4872 "ds_parser.cpp"
    break;

  case 188:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4878 "ds_parser.cpp"
    break;

  case 189:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4884 "ds_parser.cpp"
    break;

  case 190:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4890 "ds_parser.cpp"
    break;

  case 191:
#line 1126 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4902 "ds_parser.cpp"
    break;

  case 192:
#line 1137 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4912 "ds_parser.cpp"
    break;

  case 193:
#line 1142 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4924 "ds_parser.cpp"
    break;

  case 194:
#line 1152 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4930 "ds_parser.cpp"
    break;

  case 195:
#line 1153 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4936 "ds_parser.cpp"
    break;

  case 196:
#line 1154 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4942 "ds_parser.cpp"
    break;

  case 197:
#line 1155 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4948 "ds_parser.cpp"
    break;

  case 198:
#line 1156 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4954 "ds_parser.cpp"
    break;

  case 199:
#line 1157 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4960 "ds_parser.cpp"
    break;

  case 200:
#line 1158 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4966 "ds_parser.cpp"
    break;

  case 201:
#line 1159 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4972 "ds_parser.cpp"
    break;

  case 202:
#line 1160 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4978 "ds_parser.cpp"
    break;

  case 203:
#line 1161 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4984 "ds_parser.cpp"
    break;

  case 204:
#line 1162 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4990 "ds_parser.cpp"
    break;

  case 205:
#line 1163 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4996 "ds_parser.cpp"
    break;

  case 206:
#line 1164 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5002 "ds_parser.cpp"
    break;

  case 207:
#line 1165 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5008 "ds_parser.cpp"
    break;

  case 208:
#line 1166 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5014 "ds_parser.cpp"
    break;

  case 209:
#line 1167 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5020 "ds_parser.cpp"
    break;

  case 210:
#line 1168 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5026 "ds_parser.cpp"
    break;

  case 211:
#line 1169 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5032 "ds_parser.cpp"
    break;

  case 212:
#line 1170 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5038 "ds_parser.cpp"
    break;

  case 213:
#line 1171 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5044 "ds_parser.cpp"
    break;

  case 214:
#line 1172 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5050 "ds_parser.cpp"
    break;

  case 215:
#line 1173 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5056 "ds_parser.cpp"
    break;

  case 216:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5062 "ds_parser.cpp"
    break;

  case 217:
#line 1175 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5068 "ds_parser.cpp"
    break;

  case 218:
#line 1176 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5074 "ds_parser.cpp"
    break;

  case 219:
#line 1177 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5080 "ds_parser.cpp"
    break;

  case 220:
#line 1178 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5086 "ds_parser.cpp"
    break;

  case 221:
#line 1179 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5092 "ds_parser.cpp"
    break;

  case 222:
#line 1180 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5098 "ds_parser.cpp"
    break;

  case 223:
#line 1181 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5104 "ds_parser.cpp"
    break;

  case 224:
#line 1182 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5110 "ds_parser.cpp"
    break;

  case 225:
#line 1183 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5116 "ds_parser.cpp"
    break;

  case 226:
#line 1184 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5122 "ds_parser.cpp"
    break;

  case 227:
#line 1185 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5128 "ds_parser.cpp"
    break;

  case 228:
#line 1186 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5134 "ds_parser.cpp"
    break;

  case 229:
#line 1187 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5140 "ds_parser.cpp"
    break;

  case 230:
#line 1188 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5146 "ds_parser.cpp"
    break;

  case 231:
#line 1189 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5152 "ds_parser.cpp"
    break;

  case 232:
#line 1190 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5161 "ds_parser.cpp"
    break;

  case 233:
#line 1194 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5170 "ds_parser.cpp"
    break;

  case 234:
#line 1198 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5178 "ds_parser.cpp"
    break;

  case 235:
#line 1201 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5186 "ds_parser.cpp"
    break;

  case 236:
#line 1204 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5192 "ds_parser.cpp"
    break;

  case 237:
#line 1205 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5198 "ds_parser.cpp"
    break;

  case 238:
#line 1206 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5204 "ds_parser.cpp"
    break;

  case 239:
#line 1207 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5210 "ds_parser.cpp"
    break;

  case 240:
#line 1208 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5216 "ds_parser.cpp"
    break;

  case 241:
#line 1209 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-2]))); }
#line 5222 "ds_parser.cpp"
    break;

  case 242:
#line 1210 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5228 "ds_parser.cpp"
    break;

  case 243:
#line 1211 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5234 "ds_parser.cpp"
    break;

  case 244:
#line 1212 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5242 "ds_parser.cpp"
    break;

  case 245:
#line 1215 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5250 "ds_parser.cpp"
    break;

  case 246:
#line 1218 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5256 "ds_parser.cpp"
    break;

  case 247:
#line 1219 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5262 "ds_parser.cpp"
    break;

  case 248:
#line 1220 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5268 "ds_parser.cpp"
    break;

  case 249:
#line 1221 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5274 "ds_parser.cpp"
    break;

  case 250:
#line 1222 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5280 "ds_parser.cpp"
    break;

  case 251:
#line 1223 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5286 "ds_parser.cpp"
    break;

  case 252:
#line 1224 "ds_parser.ypp"
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
#line 5301 "ds_parser.cpp"
    break;

  case 253:
#line 1234 "ds_parser.ypp"
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
#line 5316 "ds_parser.cpp"
    break;

  case 254:
#line 1244 "ds_parser.ypp"
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
#line 5352 "ds_parser.cpp"
    break;

  case 255:
#line 1278 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5358 "ds_parser.cpp"
    break;

  case 256:
#line 1279 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5364 "ds_parser.cpp"
    break;

  case 257:
#line 1283 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5370 "ds_parser.cpp"
    break;

  case 258:
#line 1284 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5376 "ds_parser.cpp"
    break;

  case 259:
#line 1288 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5386 "ds_parser.cpp"
    break;

  case 260:
#line 1296 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5394 "ds_parser.cpp"
    break;

  case 261:
#line 1299 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5403 "ds_parser.cpp"
    break;

  case 262:
#line 1306 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5409 "ds_parser.cpp"
    break;

  case 263:
#line 1307 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5415 "ds_parser.cpp"
    break;

  case 264:
#line 1311 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5421 "ds_parser.cpp"
    break;

  case 265:
#line 1312 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5427 "ds_parser.cpp"
    break;

  case 266:
#line 1316 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5435 "ds_parser.cpp"
    break;

  case 267:
#line 1322 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5441 "ds_parser.cpp"
    break;

  case 268:
#line 1323 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5447 "ds_parser.cpp"
    break;

  case 269:
#line 1327 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5453 "ds_parser.cpp"
    break;

  case 270:
#line 1328 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5459 "ds_parser.cpp"
    break;

  case 271:
#line 1332 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5469 "ds_parser.cpp"
    break;

  case 272:
#line 1337 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5477 "ds_parser.cpp"
    break;

  case 273:
#line 1340 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5486 "ds_parser.cpp"
    break;

  case 274:
#line 1344 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5497 "ds_parser.cpp"
    break;

  case 275:
#line 1350 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5508 "ds_parser.cpp"
    break;

  case 276:
#line 1359 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5514 "ds_parser.cpp"
    break;

  case 277:
#line 1360 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5520 "ds_parser.cpp"
    break;

  case 278:
#line 1361 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5526 "ds_parser.cpp"
    break;

  case 279:
#line 1365 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5534 "ds_parser.cpp"
    break;

  case 280:
#line 1368 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5544 "ds_parser.cpp"
    break;

  case 281:
#line 1373 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5554 "ds_parser.cpp"
    break;

  case 282:
#line 1378 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5566 "ds_parser.cpp"
    break;

  case 283:
#line 1385 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5578 "ds_parser.cpp"
    break;

  case 284:
#line 1395 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5586 "ds_parser.cpp"
    break;

  case 285:
#line 1398 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5595 "ds_parser.cpp"
    break;

  case 286:
#line 1405 "ds_parser.ypp"
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
#line 5626 "ds_parser.cpp"
    break;

  case 287:
#line 1434 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5634 "ds_parser.cpp"
    break;

  case 288:
#line 1437 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5648 "ds_parser.cpp"
    break;

  case 289:
#line 1446 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5662 "ds_parser.cpp"
    break;

  case 290:
#line 1458 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5676 "ds_parser.cpp"
    break;

  case 291:
#line 1470 "ds_parser.ypp"
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
#line 5692 "ds_parser.cpp"
    break;

  case 292:
#line 1484 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5698 "ds_parser.cpp"
    break;

  case 293:
#line 1485 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5704 "ds_parser.cpp"
    break;

  case 294:
#line 1489 "ds_parser.ypp"
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
#line 5745 "ds_parser.cpp"
    break;

  case 295:
#line 1528 "ds_parser.ypp"
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
#line 5813 "ds_parser.cpp"
    break;

  case 296:
#line 1594 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5825 "ds_parser.cpp"
    break;

  case 297:
#line 1601 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5836 "ds_parser.cpp"
    break;

  case 298:
#line 1610 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 5842 "ds_parser.cpp"
    break;

  case 299:
#line 1611 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 5848 "ds_parser.cpp"
    break;

  case 300:
#line 1612 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 5854 "ds_parser.cpp"
    break;

  case 301:
#line 1613 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 5860 "ds_parser.cpp"
    break;

  case 302:
#line 1614 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 5866 "ds_parser.cpp"
    break;

  case 303:
#line 1615 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 5872 "ds_parser.cpp"
    break;

  case 304:
#line 1616 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 5878 "ds_parser.cpp"
    break;

  case 305:
#line 1617 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 5884 "ds_parser.cpp"
    break;

  case 306:
#line 1618 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 5890 "ds_parser.cpp"
    break;

  case 307:
#line 1619 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 5896 "ds_parser.cpp"
    break;

  case 308:
#line 1620 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 5902 "ds_parser.cpp"
    break;

  case 309:
#line 1621 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 5908 "ds_parser.cpp"
    break;

  case 310:
#line 1622 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 5914 "ds_parser.cpp"
    break;

  case 311:
#line 1623 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 5920 "ds_parser.cpp"
    break;

  case 312:
#line 1624 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 5926 "ds_parser.cpp"
    break;

  case 313:
#line 1625 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 5932 "ds_parser.cpp"
    break;

  case 314:
#line 1626 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 5938 "ds_parser.cpp"
    break;

  case 315:
#line 1627 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 5944 "ds_parser.cpp"
    break;

  case 316:
#line 1628 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 5950 "ds_parser.cpp"
    break;

  case 317:
#line 1629 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 5956 "ds_parser.cpp"
    break;

  case 318:
#line 1630 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 5962 "ds_parser.cpp"
    break;

  case 319:
#line 1631 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 5968 "ds_parser.cpp"
    break;

  case 320:
#line 1632 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 5974 "ds_parser.cpp"
    break;

  case 321:
#line 1633 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 5980 "ds_parser.cpp"
    break;

  case 322:
#line 1637 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5993 "ds_parser.cpp"
    break;

  case 323:
#line 1648 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6002 "ds_parser.cpp"
    break;

  case 324:
#line 1652 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6014 "ds_parser.cpp"
    break;

  case 325:
#line 1662 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6020 "ds_parser.cpp"
    break;

  case 326:
#line 1663 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6026 "ds_parser.cpp"
    break;

  case 327:
#line 1664 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6032 "ds_parser.cpp"
    break;

  case 328:
#line 1665 "ds_parser.ypp"
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
#line 6051 "ds_parser.cpp"
    break;

  case 329:
#line 1679 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6062 "ds_parser.cpp"
    break;

  case 330:
#line 1685 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6071 "ds_parser.cpp"
    break;

  case 331:
#line 1689 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6081 "ds_parser.cpp"
    break;

  case 332:
#line 1694 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6091 "ds_parser.cpp"
    break;

  case 333:
#line 1699 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6101 "ds_parser.cpp"
    break;

  case 334:
#line 1704 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6113 "ds_parser.cpp"
    break;

  case 335:
#line 1711 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6123 "ds_parser.cpp"
    break;

  case 336:
#line 1716 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6132 "ds_parser.cpp"
    break;

  case 337:
#line 1720 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6141 "ds_parser.cpp"
    break;

  case 338:
#line 1724 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6151 "ds_parser.cpp"
    break;

  case 339:
#line 1729 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6161 "ds_parser.cpp"
    break;

  case 340:
#line 1734 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6173 "ds_parser.cpp"
    break;

  case 341:
#line 1741 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6183 "ds_parser.cpp"
    break;

  case 342:
#line 1746 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6194 "ds_parser.cpp"
    break;

  case 343:
#line 1752 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6204 "ds_parser.cpp"
    break;

  case 344:
#line 1757 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6213 "ds_parser.cpp"
    break;

  case 345:
#line 1761 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6223 "ds_parser.cpp"
    break;

  case 346:
#line 1766 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6237 "ds_parser.cpp"
    break;

  case 347:
#line 1775 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6246 "ds_parser.cpp"
    break;

  case 348:
#line 1779 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6256 "ds_parser.cpp"
    break;

  case 349:
#line 1784 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6270 "ds_parser.cpp"
    break;

  case 350:
#line 1793 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6279 "ds_parser.cpp"
    break;

  case 351:
#line 1797 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6289 "ds_parser.cpp"
    break;

  case 352:
#line 1802 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6303 "ds_parser.cpp"
    break;

  case 353:
#line 1811 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6314 "ds_parser.cpp"
    break;

  case 354:
#line 1820 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6320 "ds_parser.cpp"
    break;

  case 355:
#line 1821 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6326 "ds_parser.cpp"
    break;

  case 356:
#line 1822 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6332 "ds_parser.cpp"
    break;

  case 357:
#line 1823 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6338 "ds_parser.cpp"
    break;

  case 358:
#line 1827 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6350 "ds_parser.cpp"
    break;

  case 359:
#line 1834 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6361 "ds_parser.cpp"
    break;

  case 360:
#line 1843 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6371 "ds_parser.cpp"
    break;

  case 361:
#line 1848 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6380 "ds_parser.cpp"
    break;

  case 362:
#line 1855 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6390 "ds_parser.cpp"
    break;

  case 363:
#line 1860 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6401 "ds_parser.cpp"
    break;

  case 364:
#line 1866 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6413 "ds_parser.cpp"
    break;

  case 365:
#line 1873 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6426 "ds_parser.cpp"
    break;

  case 366:
#line 1884 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6434 "ds_parser.cpp"
    break;

  case 367:
#line 1887 "ds_parser.ypp"
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
#line 6450 "ds_parser.cpp"
    break;

  case 368:
#line 1901 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6461 "ds_parser.cpp"
    break;

  case 369:
#line 1910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6467 "ds_parser.cpp"
    break;

  case 370:
#line 1911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6473 "ds_parser.cpp"
    break;

  case 371:
#line 1915 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6483 "ds_parser.cpp"
    break;

  case 372:
#line 1920 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6492 "ds_parser.cpp"
    break;

  case 373:
#line 1927 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6502 "ds_parser.cpp"
    break;

  case 374:
#line 1932 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6515 "ds_parser.cpp"
    break;

  case 375:
#line 1943 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6525 "ds_parser.cpp"
    break;

  case 376:
#line 1948 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6534 "ds_parser.cpp"
    break;

  case 377:
#line 1955 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6548 "ds_parser.cpp"
    break;

  case 378:
#line 1967 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6554 "ds_parser.cpp"
    break;

  case 379:
#line 1968 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6560 "ds_parser.cpp"
    break;

  case 380:
#line 1972 "ds_parser.ypp"
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
#line 6578 "ds_parser.cpp"
    break;


#line 6582 "ds_parser.cpp"

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
#line 1987 "ds_parser.ypp"


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


