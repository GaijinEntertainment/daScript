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
#line 50 "ds_parser.ypp" /* yacc.c:338  */

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

#line 100 "ds_parser.cpp" /* yacc.c:338  */
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

/* In a future release of Bison, this section will be replaced
   by #include "ds_parser.hpp".  */
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
#line 2 "ds_parser.ypp" /* yacc.c:353  */

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

#line 189 "ds_parser.cpp" /* yacc.c:353  */

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
    DAS_ELIF = 273,
    DAS_ARRAY = 274,
    DAS_RETURN = 275,
    DAS_NULL = 276,
    DAS_BREAK = 277,
    DAS_TRY = 278,
    DAS_OPTIONS = 279,
    DAS_TABLE = 280,
    DAS_EXPECT = 281,
    DAS_CONST = 282,
    DAS_REQUIRE = 283,
    DAS_OPERATOR = 284,
    DAS_ENUM = 285,
    DAS_FINALLY = 286,
    DAS_DELETE = 287,
    DAS_DEREF = 288,
    DAS_SCOPE = 289,
    DAS_TYPEDEF = 290,
    DAS_WITH = 291,
    DAS_CAST = 292,
    DAS_OVERRIDE = 293,
    DAS_UPCAST = 294,
    DAS_ITERATOR = 295,
    DAS_VAR = 296,
    DAS_ADDR = 297,
    DAS_CONTINUE = 298,
    DAS_WHERE = 299,
    DAS_PASS = 300,
    DAS_REINTERPRET = 301,
    DAS_TBOOL = 302,
    DAS_TVOID = 303,
    DAS_TSTRING = 304,
    DAS_TAUTO = 305,
    DAS_TINT = 306,
    DAS_TINT2 = 307,
    DAS_TINT3 = 308,
    DAS_TINT4 = 309,
    DAS_TUINT = 310,
    DAS_TUINT2 = 311,
    DAS_TUINT3 = 312,
    DAS_TUINT4 = 313,
    DAS_TFLOAT = 314,
    DAS_TFLOAT2 = 315,
    DAS_TFLOAT3 = 316,
    DAS_TFLOAT4 = 317,
    DAS_TRANGE = 318,
    DAS_TURANGE = 319,
    DAS_TBLOCK = 320,
    DAS_TINT64 = 321,
    DAS_TUINT64 = 322,
    DAS_TDOUBLE = 323,
    DAS_TFUNCTION = 324,
    DAS_TLAMBDA = 325,
    DAS_TINT8 = 326,
    DAS_TUINT8 = 327,
    DAS_TINT16 = 328,
    DAS_TUINT16 = 329,
    DAS_TTUPLE = 330,
    ADDEQU = 331,
    SUBEQU = 332,
    DIVEQU = 333,
    MULEQU = 334,
    MODEQU = 335,
    ANDEQU = 336,
    OREQU = 337,
    XOREQU = 338,
    SHL = 339,
    SHR = 340,
    ADDADD = 341,
    SUBSUB = 342,
    LEEQU = 343,
    SHLEQU = 344,
    SHREQU = 345,
    GREQU = 346,
    EQUEQU = 347,
    NOTEQU = 348,
    RARROW = 349,
    LARROW = 350,
    QQ = 351,
    QDOT = 352,
    LPIPE = 353,
    LBPIPE = 354,
    RPIPE = 355,
    CLONEEQU = 356,
    ROTL = 357,
    ROTR = 358,
    ROTLEQU = 359,
    ROTREQU = 360,
    MAPTO = 361,
    BRABRAB = 362,
    BRACBRB = 363,
    CBRCBRB = 364,
    INTEGER = 365,
    LONG_INTEGER = 366,
    UNSIGNED_INTEGER = 367,
    UNSIGNED_LONG_INTEGER = 368,
    FLOAT = 369,
    DOUBLE = 370,
    NAME = 371,
    BEGIN_STRING = 372,
    STRING_CHARACTER = 373,
    END_STRING = 374,
    BEGIN_STRING_EXPR = 375,
    END_STRING_EXPR = 376,
    UNARY_MINUS = 377,
    UNARY_PLUS = 378,
    PRE_INC = 379,
    PRE_DEC = 380,
    POST_INC = 381,
    POST_DEC = 382,
    COLCOL = 383
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 76 "ds_parser.ypp" /* yacc.c:353  */

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

#line 355 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   4246

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  155
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  350
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  624

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   383

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   140,     2,     2,   154,   135,   128,     2,
     148,   149,   133,   132,   122,   131,   145,   134,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   125,   151,
     129,   123,   130,   124,   136,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   146,     2,   147,   127,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   152,   126,   153,   139,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   137,   138,   141,
     142,   143,   144,   150
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   347,   347,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   361,   362,   366,   370,   374,   383,   391,   407,
     428,   432,   433,   437,   440,   449,   462,   466,   467,   471,
     474,   480,   481,   482,   489,   496,   508,   518,   527,   528,
     529,   530,   531,   532,   536,   541,   549,   550,   554,   561,
     573,   577,   584,   585,   589,   590,   591,   595,   599,   606,
     610,   611,   612,   613,   614,   615,   616,   617,   618,   619,
     620,   621,   622,   623,   624,   625,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,   644,   645,   649,   719,   722,   732,
     733,   734,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   749,   753,   759,   765,   779,   780,   786,
     790,   796,   799,   802,   806,   812,   816,   820,   823,   826,
     831,   834,   842,   848,   849,   853,   854,   858,   891,   894,
     899,   907,   911,   916,   920,   928,   931,   937,   942,   994,
     995,   996,   997,   998,   999,  1003,  1004,  1005,  1006,  1007,
    1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,
    1018,  1022,  1033,  1038,  1048,  1049,  1050,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1090,  1094,  1097,  1100,  1101,  1102,  1103,
    1104,  1105,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1124,
    1134,  1168,  1169,  1173,  1174,  1178,  1186,  1189,  1196,  1197,
    1201,  1202,  1206,  1212,  1213,  1217,  1218,  1222,  1227,  1230,
    1234,  1240,  1249,  1250,  1251,  1255,  1258,  1263,  1268,  1275,
    1285,  1288,  1295,  1324,  1327,  1336,  1348,  1360,  1374,  1375,
    1379,  1418,  1484,  1491,  1500,  1501,  1502,  1503,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1527,  1538,
    1542,  1552,  1553,  1554,  1555,  1569,  1575,  1579,  1584,  1589,
    1594,  1599,  1606,  1611,  1616,  1622,  1627,  1631,  1636,  1645,
    1649,  1654,  1663,  1667,  1672,  1681,  1690,  1691,  1692,  1693,
    1697,  1704,  1713,  1718,  1725,  1730,  1739,  1742,  1756,  1765,
    1766,  1770,  1775,  1782,  1787,  1798,  1803,  1810,  1822,  1823,
    1827
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
  "\"type\"", "\"in\"", "\"elif\"", "\"array\"", "\"return\"", "\"null\"",
  "\"break\"", "\"try\"", "\"options\"", "\"table\"", "\"expect\"",
  "\"const\"", "\"require\"", "\"operator\"", "\"enum\"", "\"finally\"",
  "\"delete\"", "\"deref\"", "\"scope\"", "\"typedef\"", "\"with\"",
  "\"cast\"", "\"override\"", "\"upcast\"", "\"iterator\"", "\"var\"",
  "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"",
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"+=\"", "\"-=\"", "\"/=\"",
  "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"",
  "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"",
  "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"<|\"", "\" <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"", "','",
  "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'",
  "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('",
  "')'", "COLCOL", "';'", "'{'", "'}'", "'$'", "$accept", "program",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_list", "require_module_name", "require_module",
  "expect_declaration", "expect_list", "expect_error", "expression_else",
  "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "function_declaration", "expression_block",
  "expression_any", "expressions", "expr_pipe", "name_in_namespace",
  "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_try_catch",
  "let_scope", "kwd_let", "expression_let", "expr_cast", "expr_type_info",
  "expr_list", "expr_block", "expr_numeric_const", "expr_assign",
  "expr_named_call", "expr_method_call", "expr",
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
     375,   376,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,    64,   377,   378,   126,
      33,   379,   380,   381,   382,    46,    91,    93,    40,    41,
     383,    59,   123,   125,    36
};
# endif

#define YYPACT_NINF -425

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-425)))

#define YYTABLE_NINF -156

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -425,    20,  -425,  -425,   -86,   -56,   -52,   -44,    -1,  -425,
     -13,  -425,  -425,  -425,  -425,   103,  -425,   -63,  -425,  -425,
    -425,  -425,    28,  -425,    38,    42,    71,  -425,  -425,    97,
      82,  -425,    83,   100,  -425,    93,   135,   120,  -425,   -49,
    -425,   139,    -5,  -425,    26,   -86,   157,   -56,   -52,   153,
    -425,  2650,   154,  -425,   -86,   -13,  -425,   147,   121,  3610,
     245,   246,  -425,   129,   -95,  -425,  -425,  -425,  -425,  -425,
     160,  -425,  -425,  -425,  -425,  -425,  -425,   -90,   151,   152,
     158,  -425,  -425,  -425,   136,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,   159,
    -425,  -425,  -425,   161,   168,  -425,  -425,  -425,  -425,   169,
    -425,  -425,  -425,  -425,   352,  -425,   -79,  -425,   -88,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,   -14,   164,  -425,
    -425,  -425,   -35,  -425,    56,   -58,  -425,  2650,  2650,  2650,
     183,  2580,  2580,  2580,  2650,  -425,     6,  -425,  -425,  1587,
    -425,  -425,   -33,   184,  -425,  -425,    -3,  -425,   -66,  2650,
     150,  -425,  -425,   189,  -425,  2650,  2401,  -425,  -425,   198,
    -425,    68,     4,   155,   162,   164,   190,   164,   239,   164,
     300,  -425,   -77,   352,   182,  -425,   165,  -425,  -425,  2510,
     167,  -425,   171,   187,   188,   173,   193,   175,  2401,  2401,
     573,  2650,  2401,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    2401,  2401,   -88,  2401,  2401,  -425,  2401,  -425,   178,  -425,
    -425,   -81,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  3632,
     177,  -425,  -425,  -425,  -425,  -425,   -86,  -425,   275,   180,
    -425,  -425,   -25,  -425,  -425,  2650,  2401,   352,  -425,  -425,
     106,  -425,   227,  2656,   191,  -425,  2650,  -425,  -425,   203,
    -425,   204,  -425,   208,  -425,  -425,  2650,  -425,  -425,    78,
    -425,   212,  2401,  2650,  2650,  2401,  2650,  1717,   243,   243,
     184,   951,  1081,  3760,  -425,    57,   119,   243,   243,  -425,
     243,   243,  3206,   654,   129,  -425,  1211,  2401,  2401,  -425,
    -425,  2401,  2401,  2401,  2401,   228,  2401,   230,  2401,  2401,
    2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,
    2401,  2401,  2401,   231,  2401,  -425,  1831,   -46,  -425,   184,
    -425,  -425,   117,  -425,  2960,  -425,  2401,    95,  2401,  2401,
    2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,  2401,
    2401,  2401,  2401,  -425,  -425,   325,  -425,  -425,  -425,  -425,
    1945,  1325,  3272,   339,   366,  3338,   389,  -425,  3404,    -6,
     -72,  2059,  3760,   226,    52,   229,  -425,  -425,    53,   107,
    2401,  2401,   196,  -425,  2401,   232,  -425,  -425,  2401,  2401,
     184,  1455,  -425,   150,  2401,  2401,  -425,   202,  -425,   323,
    -425,  -425,  -425,  -425,  -425,  -425,   205,   209,   210,  -425,
    -425,   338,  -425,   -67,  3031,   164,   248,  -425,   -70,  3928,
    4095,  4095,   454,   454,  4077,  4077,   211,   243,  -425,   -60,
     -60,  4095,  4095,  3816,  3948,  4004,  4024,   454,   454,  4100,
    4100,   428,   428,   428,  -425,  3696,  -425,   -45,  -425,  -425,
    2401,  -425,  2732,  -425,  3928,  3928,  3928,  3928,  3928,  3928,
    3928,  3928,  3928,  3928,  3928,  3928,  3928,  3928,  3928,  -425,
    -425,    -8,  2650,   256,  3470,  -425,  2401,  2401,  -425,  2401,
    -425,  2401,  2401,   248,   257,   215,   248,  2401,   234,  2401,
     235,  3928,  -425,  -425,  3872,  3087,  3087,    32,  2401,  -425,
    -425,  2808,   365,  3928,  3087,  -425,   225,  -425,  -425,  -425,
     344,   184,  -425,   150,   -27,    43,  2401,  -425,  2173,  2401,
    -425,  -425,  3928,  -425,  -425,    48,   253,  -425,   454,   454,
     454,   -82,  3928,    64,   -27,  -425,   226,  3928,  -425,  -425,
    -425,  -425,  -425,    88,  2401,  -425,  2884,  -425,   150,  -425,
    -425,  -425,  -425,  -425,   238,  3928,  -425,    34,  3928,  -425,
    2287,  2401,   271,  2401,   150,  2401,  -425,   -80,  -425,  -425,
     808,  -425,  -425,  2650,  3536,  3150,  -425,  3928,  -425,  3087,
     150,  -425,    67,  -425,   348,   272,    88,  -425,  -425,  2401,
     273,  -425,  3928,  -425
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    52,     1,   135,     0,     0,     0,     0,     0,   136,
       0,    11,     9,     8,     7,     0,     6,     0,     5,    10,
       4,     3,     0,    44,    19,    29,    26,    27,    23,    20,
      25,    21,     0,     0,    47,   117,     0,    48,    50,     0,
      46,     0,     0,   260,     0,     0,     0,     0,     0,     0,
     263,     0,     0,   119,     0,     0,    53,   268,     0,     0,
       0,     0,    59,    54,     0,    42,    43,    40,    41,    39,
       0,    38,    45,    30,    28,    22,    24,     0,     0,     0,
       0,   274,   294,   275,   299,   276,   280,   281,   282,   283,
     287,   288,   289,   290,   291,   292,   293,   295,   296,   316,
     279,   286,   297,   319,   322,   277,   284,   278,   285,     0,
     298,   301,   303,   302,   266,   118,     0,    51,     0,   270,
     236,    62,    63,    65,    64,    66,    67,    68,    69,    88,
      89,    86,    87,    79,    90,    91,    80,    77,    78,    92,
      93,    94,    95,    82,    83,    81,    75,    76,    71,    70,
      72,    73,    74,    61,    60,    84,    85,     0,    57,   272,
     262,   261,     0,    12,     0,     0,   267,     0,     0,     0,
       0,    54,    54,    54,     0,   307,     0,   312,   309,     0,
      49,   269,   231,     0,    55,   240,     0,   238,   247,     0,
       0,   253,   254,     0,   252,     0,     0,    13,    14,     0,
     264,     0,     0,     0,     0,    57,     0,    57,     0,    57,
       0,   243,     0,   242,   308,   310,     0,   179,   180,     0,
       0,   174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,   151,   150,   152,   153,   154,    15,
       0,     0,     0,     0,     0,   305,     0,   113,    52,   177,
     147,   175,   224,   223,   222,   227,   176,   226,   225,     0,
       0,   178,   326,   327,   328,   329,     0,   271,   233,     0,
     239,    56,     0,   246,   245,     0,     0,    58,    96,   273,
       0,   259,     0,   155,     0,   313,     0,   315,   300,     0,
     317,     0,   320,     0,   323,   325,     0,   311,   306,   121,
     124,     0,     0,     0,     0,     0,     0,     0,   203,   204,
       0,     0,     0,     0,   345,     0,     0,   184,   183,   211,
     182,   181,     0,     0,    54,   230,     0,     0,     0,   205,
     206,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,     0,   234,     0,
     237,   241,   248,   251,   250,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   265,     0,   318,   321,   324,   244,
       0,     0,     0,     0,     0,     0,     0,   218,     0,     0,
     117,     0,   336,   332,     0,   339,   340,   341,     0,     0,
       0,     0,     0,    18,     0,    16,   207,   115,     0,     0,
       0,     0,   125,     0,     0,     0,   126,     0,    99,    97,
     110,   106,   104,   105,   114,   100,     0,     0,     0,   109,
     111,   133,   103,     0,   155,    57,     0,   212,     0,   145,
     185,   186,   198,   199,   196,   197,     0,   220,   210,   228,
     229,   187,   188,     0,   201,   202,   200,   194,   195,   190,
     189,   191,   192,   193,   209,     0,   214,     0,   232,   235,
       0,   257,   155,   116,   162,   163,   165,   164,   166,   159,
     160,   161,   167,   168,   157,   158,   169,   170,   156,   314,
     122,     0,     0,     0,     0,   216,     0,     0,   217,     0,
     219,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   346,   347,     0,     0,     0,     0,     0,   127,
     130,   155,     0,   120,     0,   112,     0,   102,   107,   108,
       0,     0,   101,     0,     0,     0,     0,   213,     0,     0,
     208,   215,   249,   256,   123,     0,     0,   141,   138,   139,
     140,     0,   330,     0,     0,   334,   333,   337,   343,   342,
     344,    17,    36,    31,     0,   131,   155,   128,     0,    37,
     113,   134,   137,   148,     0,   146,   172,     0,   221,   143,
       0,     0,     0,     0,     0,     0,    34,     0,   129,   132,
       0,   171,   173,     0,     0,   348,   335,   331,    32,     0,
       0,    98,     0,   142,     0,     0,    31,    35,   144,     0,
       0,    33,   349,   350
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -425,  -425,    84,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
     374,  -425,  -425,   376,  -191,  -425,  -425,  -425,  -425,   381,
     -48,  -425,   372,  -425,   213,  -154,  -195,  -425,  -425,   -78,
    -425,  -130,  -193,   -10,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,   450,  -425,  -425,  -425,  -343,    90,  -425,  -318,  -425,
    -425,    63,  -425,  -425,  -425,  -425,   186,  -425,   163,  -425,
    -184,  -175,   192,   -75,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,   -62,   -50,  -425,  -425,   -42,   249,  -424,   -51,
    -425,  -425,  -225,   -41,   174,  -425,  -425,  -425,  -425,  -425
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   164,    71,   316,   249,    12,    13,    29,    30,
      31,    14,    26,    27,   596,   430,   431,   432,   433,    23,
      24,    37,    38,    39,    15,   158,   190,    63,    16,   250,
     434,   323,   435,   251,   436,   252,   437,   438,   439,   440,
     541,   441,   442,   253,   254,   448,   255,   256,   282,   257,
     258,   449,   268,   359,   269,   182,   185,   186,   211,   212,
     512,   187,   196,   161,    64,    18,    77,    19,    20,   119,
      58,    21,   188,   260,   112,   113,   213,   261,   403,   404,
     262,   405,   406,   407,   408,   263,   315,   264,   615,   265
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,   111,   162,   281,   276,   443,   116,   314,   270,   114,
     289,   511,   291,   477,   293,    34,   183,   205,   207,   209,
       2,   159,   545,   273,    59,     3,   165,   183,    35,   273,
      22,   175,   367,   214,   335,   325,   176,   337,    65,    66,
     546,   110,   546,    45,     4,    40,     5,   501,     6,   574,
       7,   274,   546,   295,    25,     8,   193,   274,   160,   275,
     191,     9,    36,   166,    28,   199,   192,   326,   273,   591,
     180,   610,    32,    55,   296,   175,    45,   546,    52,   547,
     176,    60,    61,   363,   542,   353,   354,   193,   194,    43,
     195,   159,   566,   200,   175,   175,   274,   594,    56,   176,
     176,   478,   159,    35,   551,   175,   595,    41,   181,    42,
     176,    62,   278,   266,   546,    33,   193,   111,   111,   111,
     267,   111,   111,   111,   111,   201,   202,   203,   177,   206,
     208,   210,   178,   175,   215,   184,    67,    36,   176,   111,
      68,   554,    69,    70,   175,   111,   271,   277,   272,   176,
     179,    44,   216,   280,   193,   286,   546,   110,   110,   110,
      45,   110,   110,   110,   110,   514,    10,    46,   561,   111,
     445,    11,   177,   481,   197,   198,   178,   299,   480,   110,
     111,   111,   175,   602,   479,   110,   522,   176,   311,   312,
     584,   177,   177,    47,   179,   178,   178,   589,   285,   515,
     518,   191,   177,   516,   519,   587,   178,   192,   411,   110,
     412,   592,   273,   179,   179,   516,   618,   175,   357,    48,
     110,   110,   176,    51,   179,   111,   390,    49,   530,   194,
     177,   597,   319,   362,   178,    50,   111,   163,   413,   414,
     274,   177,   259,    52,   385,   178,   111,   247,   399,   248,
     543,    53,   179,   111,   111,    57,   111,   365,   519,   283,
     520,   393,   394,   179,   396,   110,   175,    73,    54,    76,
     115,   176,   118,   120,   155,   156,   110,   157,   163,   177,
     167,   168,   443,   178,   170,   287,   110,   169,   171,   189,
     172,   308,   309,   110,   110,   313,   110,   173,   174,   204,
     159,   179,   247,   317,   318,   279,   320,   321,   284,   322,
     297,   288,   298,   358,   177,   301,   303,   304,   178,   302,
     290,   305,   306,   307,    10,   356,   367,   175,   391,   329,
     330,   360,   176,   386,   387,   575,   179,   335,   388,   364,
     337,   338,   384,   339,   456,   532,   458,   474,   514,   523,
     197,   517,   175,   535,   536,   540,   537,   176,   527,   548,
     538,   539,   565,   177,   544,   392,   175,   178,   395,   292,
     398,   176,   556,   564,   402,   402,   578,   580,   581,   175,
     593,   568,   570,   590,   176,   179,   444,   601,   353,   354,
     450,   451,   619,   175,   452,   453,   454,   455,   176,   457,
     415,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   175,   475,   606,   620,
     623,   176,    75,    74,   177,   621,    72,   117,   178,   482,
     294,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   179,   572,   573,   177,
     600,    17,   111,   178,   504,   499,   579,   483,   361,   389,
     555,   324,   563,   177,   322,   583,   582,   178,   300,   506,
       0,   179,   366,   521,   313,     0,   177,   524,   569,   162,
     178,   525,   526,     0,   531,   179,   409,   533,   534,     0,
     177,     0,   110,     0,   178,     0,   507,     0,   179,     0,
     599,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   179,   177,   329,   330,   608,   178,     0,   509,
       0,     0,   335,     0,   336,   337,   338,     0,   339,     0,
       0,   616,   617,     0,     0,   179,     0,     0,   327,   328,
     329,   330,     0,   552,     0,     0,     0,     0,   335,     0,
     336,   337,   338,   111,   339,     0,   340,   341,     0,     0,
       0,   612,     0,     0,     0,     0,     0,     0,     0,   558,
     559,     0,   560,   353,   354,   562,     0,     0,     0,     0,
     567,     0,   402,   310,     0,   348,   349,   350,   351,   352,
       0,   576,    78,   110,     0,     0,     0,     0,    79,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,   585,
       0,     0,   588,    80,     0,     0,     0,     0,     0,     0,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,   604,   605,   417,   607,     0,   609,     3,
       0,   418,   419,   444,   420,     0,   217,   218,   219,   220,
       0,     0,     0,     0,   421,   221,   422,   423,     0,     0,
       0,     0,   622,     0,     0,     0,   424,   222,     0,    35,
     425,   223,     0,   224,     0,     9,   225,   426,     0,   427,
     226,    81,    82,    83,     0,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,     0,
     100,   101,   102,    36,   227,   105,   106,   107,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     228,   229,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,   231,   232,   233,   234,   235,   236,   237,   238,
      35,   239,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   240,   241,     0,     0,     0,
     242,     0,     0,   243,   244,     0,     0,     0,     0,     0,
       0,     0,   246,     0,    36,   428,   247,   429,   248,   417,
       0,     0,     0,     3,     0,   418,   419,     0,   420,     0,
     217,   218,   219,   220,     0,     0,     0,     0,   421,   221,
     422,   423,     0,     0,     0,     0,     0,     0,     0,     0,
     424,   222,     0,     0,   425,   223,     0,   224,     0,     9,
     225,   426,     0,   427,   226,    81,    82,    83,     0,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,     0,   100,   101,   102,     0,   227,   105,
     106,   107,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   228,   229,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,   231,   232,   233,   234,
     235,   236,   237,   238,    35,   239,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   240,
     241,     0,     0,     0,   242,     0,     0,   243,   244,     0,
       0,     0,     0,     0,     0,     0,   246,     0,    36,   428,
     247,   611,   248,   217,   218,   219,   220,     0,     0,     0,
       0,     0,   221,     0,     0,     0,     0,     0,   175,     0,
       0,     0,     0,   176,   222,     0,     0,     0,   223,     0,
     224,     0,     0,   225,     0,     0,     0,   226,    81,    82,
      83,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,   100,   101,   102,
       0,   227,   105,   106,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   400,   239,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,   178,
       0,     0,   240,   241,     0,     0,     0,   242,     0,     0,
     243,   244,     0,   217,   218,   219,   220,   179,     0,   401,
       0,    36,   221,   247,     0,   248,     0,     0,   175,     0,
       0,     0,     0,   176,   222,     0,     0,     0,   223,     0,
     224,     0,     0,   225,     0,     0,     0,   226,    81,    82,
      83,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,   100,   101,   102,
       0,   227,   105,   106,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    35,   239,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,   178,
       0,     0,   240,   241,     0,     0,     0,   242,     0,     0,
     243,   244,     0,   217,   218,   219,   220,   179,     0,   246,
       0,    36,   221,   247,     0,   248,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,     0,   223,     0,
     224,     0,     0,   225,     0,     0,     0,   226,    81,    82,
      83,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,   100,   101,   102,
       0,   227,   105,   106,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    35,   239,     0,
       0,     0,     0,     0,     0,     0,     0,   217,   218,   219,
     220,   502,   240,   241,     0,     0,   221,   242,     0,     0,
     243,   244,     0,     0,     0,     0,     0,   446,   222,   246,
     447,    36,   223,   247,   224,   248,     0,   225,     0,     0,
       0,   226,    81,    82,    83,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,   100,   101,   102,     0,   227,   105,   106,   107,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,   232,   233,   234,   235,   236,   237,
     238,    35,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   503,     0,   240,   241,     0,     0,
       0,   242,     0,     0,   243,   244,     0,   217,   218,   219,
     220,     0,     0,   246,     0,    36,   221,   247,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
       0,     0,   223,     0,   224,     0,     0,   225,     0,     0,
       0,   226,    81,    82,    83,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,   100,   101,   102,     0,   227,   105,   106,   107,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,     0,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,   232,   233,   234,   235,   236,   237,
     238,    35,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   240,   241,     0,     0,
       0,   242,     0,     0,   243,   244,     0,     0,     0,   217,
     218,   219,   220,   246,     0,    36,   529,   247,   221,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,     0,     0,   223,     0,   224,     0,     0,   225,
       0,     0,     0,   226,    81,    82,    83,     0,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,   100,   101,   102,     0,   227,   105,   106,
     107,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,   238,    35,   239,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,   242,     0,     0,   243,   244,     0,   217,
     218,   219,   220,     0,   245,   246,     0,    36,   221,   247,
       0,   248,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,     0,     0,   223,     0,   224,     0,     0,   225,
       0,     0,     0,   226,    81,    82,    83,     0,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,   100,   101,   102,     0,   227,   105,   106,
     107,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,   238,    35,   239,     0,     0,     0,     0,     0,
       0,     0,     0,   217,   218,   219,   220,     0,   240,   241,
       0,     0,   221,   242,     0,     0,   243,   244,     0,     0,
       0,     0,     0,     0,   222,   246,   397,    36,   223,   247,
     224,   248,     0,   225,     0,     0,     0,   226,    81,    82,
      83,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,   100,   101,   102,
       0,   227,   105,   106,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    35,   239,     0,
       0,     0,     0,     0,     0,     0,     0,   217,   218,   219,
     220,     0,   240,   241,     0,     0,   221,   242,     0,     0,
     243,   244,     0,     0,     0,     0,     0,     0,   222,   246,
     476,    36,   223,   247,   224,   248,     0,   225,     0,     0,
       0,   226,    81,    82,    83,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,   100,   101,   102,     0,   227,   105,   106,   107,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,   232,   233,   234,   235,   236,   237,
     238,    35,   239,     0,     0,     0,     0,     0,     0,     0,
       0,   217,   218,   219,   220,     0,   240,   241,     0,     0,
     221,   242,     0,     0,   243,   244,     0,     0,     0,     0,
       0,     0,   222,   246,   500,    36,   223,   247,   224,   248,
       0,   225,     0,     0,     0,   226,    81,    82,    83,     0,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,     0,   100,   101,   102,     0,   227,
     105,   106,   107,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,   231,   232,   233,
     234,   235,   236,   237,   238,    35,   239,     0,     0,     0,
       0,     0,     0,     0,     0,   217,   218,   219,   220,     0,
     240,   241,     0,     0,   221,   242,     0,     0,   243,   244,
       0,     0,     0,     0,     0,     0,   222,   246,   513,    36,
     223,   247,   224,   248,     0,   225,     0,     0,     0,   226,
      81,    82,    83,     0,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,     0,   100,
     101,   102,     0,   227,   105,   106,   107,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,   231,   232,   233,   234,   235,   236,   237,   238,    35,
     239,     0,     0,     0,     0,     0,     0,     0,     0,   217,
     218,   219,   220,   603,   240,   241,     0,     0,   221,   242,
       0,     0,   243,   244,     0,     0,     0,     0,     0,     0,
     222,   246,   586,    36,   223,   247,   224,   248,     0,   225,
       0,     0,     0,   226,    81,    82,    83,     0,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,   100,   101,   102,     0,   227,   105,   106,
     107,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,   238,    35,   239,     0,     0,     0,     0,     0,
       0,     0,     0,   217,   218,   219,   220,     0,   240,   241,
       0,     0,   221,   242,     0,     0,   243,   244,     0,     0,
       0,     0,     0,     0,   222,   246,     0,    36,   223,   247,
     224,   248,     0,   225,     0,     0,     0,   226,    81,    82,
      83,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,   100,   101,   102,
       0,   227,   105,   106,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    35,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,   240,   241,     0,    79,     0,   242,     0,     0,
     243,   244,     0,     0,     0,     0,     0,     0,     0,   246,
      80,    36,     0,   247,     0,   248,     0,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,   231,   232,
      80,     0,     0,     0,     0,     0,    35,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,     0,     0,     0,     0,    35,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,   157,     0,
      36,     0,   368,   369,   370,   371,   372,   373,   374,   375,
     327,   328,   329,   330,   331,   376,   377,   332,   333,   334,
     335,   378,   336,   337,   338,     0,   339,   379,   340,   341,
     380,   381,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   382,
     342,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
      36,   353,   354,     0,     0,     0,     0,   383,   368,   369,
     370,   371,   372,   373,   374,   375,   327,   328,   329,   330,
     331,   376,   377,   332,   333,   334,   335,   378,   336,   337,
     338,     0,   339,   379,   340,   341,   380,   381,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   382,   342,     0,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,   354,     0,
       0,     0,     0,   553,   368,   369,   370,   371,   372,   373,
     374,   375,   327,   328,   329,   330,   331,   376,   377,   332,
     333,   334,   335,   378,   336,   337,   338,     0,   339,   379,
     340,   341,   380,   381,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   382,   342,     0,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,   354,     0,     0,     0,     0,   577,
     368,   369,   370,   371,   372,   373,   374,   375,   327,   328,
     329,   330,   331,   376,   377,   332,   333,   334,   335,   378,
     336,   337,   338,     0,   339,   379,   340,   341,   380,   381,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   382,   342,     0,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
     354,     0,     0,     0,     0,   598,   368,   369,   370,   371,
     372,   373,   374,   375,   327,   328,   329,   330,   331,   376,
     377,   332,   333,   334,   335,   378,   336,   337,   338,  -155,
     339,   379,   340,   341,   380,   381,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   382,   342,     0,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   353,   354,   368,   369,   370,
     371,   372,   373,   374,   375,   327,   328,   329,   330,   331,
     376,   377,   332,   333,   334,   335,   378,   336,   337,   338,
       0,   339,   379,   340,   341,   380,   381,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   382,   342,     0,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,     0,     0,
       0,   327,   328,   329,   330,   331,   353,   354,   332,   333,
     334,   335,     0,   336,   337,   338,     0,   339,     0,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,   354,   327,   328,   329,   330,   331,   247,
       0,   332,   333,   334,   335,     0,   336,   337,   338,     0,
     339,     0,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,     0,     0,     0,
     327,   328,   329,   330,   331,   353,   354,   332,   333,   334,
     335,   614,   336,   337,   338,     0,   339,     0,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   353,   354,     0,     0,   416,   327,   328,   329,   330,
     331,     0,     0,   332,   333,   334,   335,     0,   336,   337,
     338,     0,   339,     0,   340,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,     0,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,   354,     0,
       0,   505,   327,   328,   329,   330,   331,     0,     0,   332,
     333,   334,   335,     0,   336,   337,   338,     0,   339,     0,
     340,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,   354,     0,     0,   508,   327,   328,
     329,   330,   331,     0,     0,   332,   333,   334,   335,     0,
     336,   337,   338,     0,   339,     0,   340,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,     0,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
     354,     0,     0,   510,   327,   328,   329,   330,   331,     0,
       0,   332,   333,   334,   335,     0,   336,   337,   338,     0,
     339,     0,   340,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   353,   354,     0,     0,   557,
     327,   328,   329,   330,   331,     0,     0,   332,   333,   334,
     335,     0,   336,   337,   338,     0,   339,     0,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   353,   354,     0,     0,   613,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,     0,     0,     0,
       0,     0,   139,   140,   141,   142,   327,   328,   329,   330,
     331,     0,     0,   332,   333,   334,   335,     0,   336,   337,
     338,     0,   339,     0,   340,   341,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,     0,     0,     0,   153,
     154,     0,     0,     0,     0,     0,   342,     0,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,   354,   355,
     327,   328,   329,   330,   331,     0,     0,   332,   333,   334,
     335,     0,   336,   337,   338,     0,   339,     0,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   353,   354,   550,   327,   328,   329,   330,   331,     0,
       0,   332,   333,   334,   335,     0,   336,   337,   338,     0,
     339,     0,   340,   341,     0,     0,   410,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,     0,     0,     0,
     327,   328,   329,   330,   331,   353,   354,   332,   333,   334,
     335,     0,   336,   337,   338,     0,   339,     0,   340,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,   549,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,     0,     0,     0,   327,   328,   329,   330,
     331,   353,   354,   332,   333,   334,   335,     0,   336,   337,
     338,     0,   339,     0,   340,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   571,     0,     0,   342,     0,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,     0,
       0,     0,   327,   328,   329,   330,   331,   353,   354,   332,
     333,   334,   335,     0,   336,   337,   338,     0,   339,     0,
     340,   341,   327,   328,   329,   330,   331,     0,     0,   332,
     333,   334,   335,     0,   336,   337,   338,     0,   339,     0,
     340,   341,   342,     0,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,   354,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,     0,     0,     0,   327,   328,
     329,   330,   331,   353,   354,   332,   333,   334,   335,     0,
     336,   337,   338,     0,   339,     0,   340,   341,   327,   328,
     329,   330,   331,     0,     0,   332,   333,   334,   335,     0,
     336,   337,   338,     0,   339,     0,   340,   341,     0,     0,
       0,     0,   345,   346,   347,   348,   349,   350,   351,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
     354,     0,     0,   346,   347,   348,   349,   350,   351,   352,
       0,   327,   328,   329,   330,   331,     0,     0,   332,   353,
     354,   335,     0,   336,   337,   338,     0,   339,     0,   340,
     341,   329,   330,     0,     0,     0,   329,   330,     0,   335,
       0,   336,   337,   338,   335,   339,   336,   337,   338,     0,
     339,     0,     0,     0,     0,     0,   346,   347,   348,   349,
     350,   351,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,   354,     0,     0,   348,   349,   350,   351,
     352,     0,     0,   350,   351,   352,     0,     0,     0,     0,
     353,   354,     0,     0,     0,   353,   354
};

static const yytype_int16 yycheck[] =
{
      10,    51,    64,   196,   188,   323,    54,   232,   183,    51,
     205,    17,   207,   356,   209,    28,    41,   171,   172,   173,
       0,   116,   446,    95,    29,     5,   116,    41,   116,    95,
     116,    27,    99,    27,    94,   116,    32,    97,    12,    13,
     122,    51,   122,   122,    24,    55,    26,   390,    28,    17,
      30,   123,   122,   130,   110,    35,   122,   123,   153,   125,
      95,    41,   150,   153,   116,   123,   101,   148,    95,   151,
     149,   151,   116,   122,   151,    27,   122,   122,   150,   149,
      32,    86,    87,   276,   151,   145,   146,   122,   123,   152,
     125,   116,   516,   151,    27,    27,   123,     9,   147,    32,
      32,   147,   116,   116,   149,    27,    18,     4,   118,     6,
      32,   116,   190,   146,   122,   116,   122,   167,   168,   169,
     153,   171,   172,   173,   174,   167,   168,   169,   124,   171,
     172,   173,   128,    27,   128,   149,   110,   150,    32,   189,
     114,   149,   116,   117,    27,   195,   149,   189,   151,    32,
     146,   123,   146,   195,   122,   151,   122,   167,   168,   169,
     122,   171,   172,   173,   174,   122,   146,   125,   511,   219,
     324,   151,   124,   366,   118,   119,   128,   219,   362,   189,
     230,   231,    27,   149,   359,   195,   411,    32,   230,   231,
     147,   124,   124,   122,   146,   128,   128,   149,   130,   147,
     147,    95,   124,   151,   151,   548,   128,   101,   151,   219,
     153,   147,    95,   146,   146,   151,   149,    27,   266,   122,
     230,   231,    32,   123,   146,   275,   148,   145,   421,   123,
     124,   574,   242,   275,   128,   152,   286,   118,   119,   120,
     123,   124,   179,   150,   286,   128,   296,   152,   310,   154,
     445,   116,   146,   303,   304,   116,   306,   151,   151,   196,
     153,   303,   304,   146,   306,   275,    27,   110,   148,   116,
     116,    32,   125,   152,    29,    29,   286,   148,   118,   124,
     129,   129,   600,   128,   148,   130,   296,   129,   129,   125,
     129,   228,   229,   303,   304,   232,   306,   129,   129,   116,
     116,   146,   152,   240,   241,   116,   243,   244,   110,   246,
     128,   149,   147,    38,   124,   148,   129,   129,   128,   148,
     130,   148,   129,   148,   146,   148,    99,    27,   116,    86,
      87,   151,    32,   130,   130,   528,   146,    94,   130,   276,
      97,    98,   151,   100,   116,   423,   116,   116,   122,   153,
     118,   122,    27,   151,    31,    17,   151,    32,   420,   148,
     151,   151,   147,   124,   116,   302,    27,   128,   305,   130,
     307,    32,   116,   116,   311,   312,    11,   152,    34,    27,
     564,   147,   147,   130,    32,   146,   323,   149,   145,   146,
     327,   328,    44,    27,   331,   332,   333,   334,    32,   336,
     316,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,    27,   354,   147,   147,
     147,    32,    48,    47,   124,   616,    45,    55,   128,   366,
     130,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   146,   525,   526,   124,
     580,     1,   502,   128,   391,   130,   534,   367,   272,   296,
     502,   248,   513,   124,   401,   543,   541,   128,   219,   130,
      -1,   146,   280,   410,   411,    -1,   124,   414,   519,   541,
     128,   418,   419,    -1,   421,   146,   312,   424,   425,    -1,
     124,    -1,   502,    -1,   128,    -1,   130,    -1,   146,    -1,
     578,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   124,    86,    87,   594,   128,    -1,   130,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,    -1,
      -1,   609,   610,    -1,    -1,   146,    -1,    -1,    84,    85,
      86,    87,    -1,   480,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,   603,   100,    -1,   102,   103,    -1,    -1,
      -1,   603,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   506,
     507,    -1,   509,   145,   146,   512,    -1,    -1,    -1,    -1,
     517,    -1,   519,    10,    -1,   131,   132,   133,   134,   135,
      -1,   528,    19,   603,    -1,    -1,    -1,    -1,    25,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   546,
      -1,    -1,   549,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      -1,    -1,    -1,   590,   591,     1,   593,    -1,   595,     5,
      -1,     7,     8,   600,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,   619,    -1,    -1,    -1,    32,    33,    -1,   116,
      36,    37,    -1,    39,    -1,    41,    42,    43,    -1,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,   150,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,
     136,    -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,   150,   151,   152,   153,   154,     1,
      -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,
      42,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,    -1,    -1,    -1,   136,    -1,    -1,   139,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,   150,   151,
     152,   153,   154,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,
     139,   140,    -1,    12,    13,    14,    15,   146,    -1,   148,
      -1,   150,    21,   152,    -1,   154,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,
     139,   140,    -1,    12,    13,    14,    15,   146,    -1,   148,
      -1,   150,    21,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,    16,   131,   132,    -1,    -1,    21,   136,    -1,    -1,
     139,   140,    -1,    -1,    -1,    -1,    -1,   146,    33,   148,
     149,   150,    37,   152,    39,   154,    -1,    42,    -1,    -1,
      -1,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,   136,    -1,    -1,   139,   140,    -1,    12,    13,    14,
      15,    -1,    -1,   148,    -1,   150,    21,   152,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,
      -1,   136,    -1,    -1,   139,   140,    -1,    -1,    -1,    12,
      13,    14,    15,   148,    -1,   150,   151,   152,    21,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    -1,    46,    47,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,    -1,    -1,   136,    -1,    -1,   139,   140,    -1,    12,
      13,    14,    15,    -1,   147,   148,    -1,   150,    21,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    -1,    46,    47,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,   131,   132,
      -1,    -1,    21,   136,    -1,    -1,   139,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   148,   149,   150,    37,   152,
      39,   154,    -1,    42,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,   131,   132,    -1,    -1,    21,   136,    -1,    -1,
     139,   140,    -1,    -1,    -1,    -1,    -1,    -1,    33,   148,
     149,   150,    37,   152,    39,   154,    -1,    42,    -1,    -1,
      -1,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,   131,   132,    -1,    -1,
      21,   136,    -1,    -1,   139,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    33,   148,   149,   150,    37,   152,    39,   154,
      -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
     131,   132,    -1,    -1,    21,   136,    -1,    -1,   139,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    33,   148,   149,   150,
      37,   152,    39,   154,    -1,    42,    -1,    -1,    -1,    46,
      47,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    14,    15,    16,   131,   132,    -1,    -1,    21,   136,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      33,   148,   149,   150,    37,   152,    39,   154,    -1,    42,
      -1,    -1,    -1,    46,    47,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,   131,   132,
      -1,    -1,    21,   136,    -1,    -1,   139,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   148,    -1,   150,    37,   152,
      39,   154,    -1,    42,    -1,    -1,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   131,   132,    -1,    25,    -1,   136,    -1,    -1,
     139,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      40,   150,    -1,   152,    -1,   154,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
      40,    -1,    -1,    -1,    -1,    -1,   116,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,   116,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,   148,    -1,
     150,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   145,   146,    -1,    -1,    -1,    -1,   151,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,
      -1,    -1,    -1,   151,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,    -1,    -1,    -1,    -1,   151,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,    -1,    -1,   151,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,   145,   146,    91,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,    84,    85,    86,    87,    88,   152,
      -1,    91,    92,    93,    94,    -1,    96,    97,    98,    -1,
     100,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,   145,   146,    91,    92,    93,
      94,   151,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,    -1,   149,    84,    85,    86,    87,
      88,    -1,    -1,    91,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,
      -1,   149,    84,    85,    86,    87,    88,    -1,    -1,    91,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,    -1,    -1,   149,    84,    85,
      86,    87,    88,    -1,    -1,    91,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,   149,    84,    85,    86,    87,    88,    -1,
      -1,    91,    92,    93,    94,    -1,    96,    97,    98,    -1,
     100,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,   149,
      84,    85,    86,    87,    88,    -1,    -1,    91,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,    -1,   149,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,   104,   105,    84,    85,    86,    87,
      88,    -1,    -1,    91,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,   103,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,   139,
     140,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
      84,    85,    86,    87,    88,    -1,    -1,    91,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,    84,    85,    86,    87,    88,    -1,
      -1,    91,    92,    93,    94,    -1,    96,    97,    98,    -1,
     100,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,   145,   146,    91,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    84,    85,    86,    87,
      88,   145,   146,    91,    92,    93,    94,    -1,    96,    97,
      98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,   124,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    84,    85,    86,    87,    88,   145,   146,    91,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,   103,    84,    85,    86,    87,    88,    -1,    -1,    91,
      92,    93,    94,    -1,    96,    97,    98,    -1,   100,    -1,
     102,   103,   124,    -1,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,   145,   146,    91,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,   103,    84,    85,
      86,    87,    88,    -1,    -1,    91,    92,    93,    94,    -1,
      96,    97,    98,    -1,   100,    -1,   102,   103,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
      -1,    84,    85,    86,    87,    88,    -1,    -1,    91,   145,
     146,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
     103,    86,    87,    -1,    -1,    -1,    86,    87,    -1,    94,
      -1,    96,    97,    98,    94,   100,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
     145,   146,    -1,    -1,    -1,   145,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   156,     0,     5,    24,    26,    28,    30,    35,    41,
     146,   151,   161,   162,   166,   179,   183,   196,   220,   222,
     223,   226,   116,   174,   175,   110,   167,   168,   116,   163,
     164,   165,   116,   116,    28,   116,   150,   176,   177,   178,
     188,     4,     6,   152,   123,   122,   125,   122,   122,   145,
     152,   123,   150,   116,   148,   122,   147,   116,   225,    29,
      86,    87,   116,   182,   219,    12,    13,   110,   114,   116,
     117,   158,   174,   110,   168,   165,   116,   221,    19,    25,
      40,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
     188,   228,   229,   230,   231,   116,   175,   177,   125,   224,
     152,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,   102,
     103,   104,   105,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   139,   140,    29,    29,   148,   180,   116,
     153,   218,   227,   118,   157,   116,   153,   129,   129,   129,
     148,   129,   129,   129,   129,    27,    32,   124,   128,   146,
     149,   188,   210,    41,   149,   211,   212,   216,   227,   125,
     181,    95,   101,   122,   123,   125,   217,   118,   119,   123,
     151,   231,   231,   231,   116,   180,   231,   180,   231,   180,
     231,   213,   214,   231,    27,   128,   146,    12,    13,    14,
      15,    21,    33,    37,    39,    42,    46,    70,    86,    87,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   117,
     131,   132,   136,   139,   140,   147,   148,   152,   154,   160,
     184,   188,   190,   198,   199,   201,   202,   204,   205,   206,
     228,   232,   235,   240,   242,   244,   146,   153,   207,   209,
     216,   149,   151,    95,   123,   125,   215,   231,   184,   116,
     231,   187,   203,   206,   110,   130,   151,   130,   149,   181,
     130,   181,   130,   181,   130,   130,   151,   128,   147,   231,
     232,   148,   148,   129,   129,   148,   129,   148,   206,   206,
      10,   231,   231,   206,   237,   241,   159,   206,   206,   188,
     206,   206,   206,   186,   179,   116,   148,    84,    85,    86,
      87,    88,    91,    92,    93,    94,    96,    97,    98,   100,
     102,   103,   124,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   145,   146,   147,   148,   175,    38,   208,
     151,   211,   231,   187,   206,   151,   217,    99,    76,    77,
      78,    79,    80,    81,    82,    83,    89,    90,    95,   101,
     104,   105,   123,   151,   151,   231,   130,   130,   130,   213,
     148,   116,   206,   231,   231,   206,   231,   149,   206,   227,
     116,   148,   206,   233,   234,   236,   237,   238,   239,   239,
     106,   151,   153,   119,   120,   157,   149,     1,     7,     8,
      10,    20,    22,    23,    32,    36,    43,    45,   151,   153,
     170,   171,   172,   173,   185,   187,   189,   191,   192,   193,
     194,   196,   197,   203,   206,   180,   146,   149,   200,   206,
     206,   206,   206,   206,   206,   206,   116,   206,   116,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   116,   206,   149,   200,   147,   216,
     215,   187,   206,   201,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   130,
     149,   200,    16,   129,   206,   149,   130,   130,   149,   130,
     149,    17,   215,   149,   122,   147,   151,   122,   147,   151,
     153,   206,   237,   153,   206,   206,   206,   227,    95,   151,
     187,   206,   184,   206,   206,   151,    31,   151,   151,   151,
      17,   195,   151,   181,   116,   233,   122,   149,   148,   125,
     147,   149,   206,   151,   149,   231,   116,   149,   206,   206,
     206,   200,   206,   234,   116,   147,   233,   206,   147,   238,
     147,   121,   184,   184,    17,   187,   206,   151,    11,   184,
     152,    34,   218,   184,   147,   206,   149,   200,   206,   149,
     130,   151,   147,   215,     9,    18,   169,   200,   151,   184,
     186,   149,   149,    16,   206,   206,   147,   206,   184,   206,
     151,   153,   231,   149,   151,   243,   184,   184,   149,    44,
     147,   169,   206,   147
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   155,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   157,   158,   159,   159,   159,   160,   161,
     162,   163,   163,   164,   164,   165,   166,   167,   167,   168,
     168,   169,   169,   169,   170,   171,   172,   173,   174,   174,
     174,   174,   174,   174,   175,   175,   176,   176,   177,   177,
     178,   178,   179,   179,   180,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   186,   186,   186,   187,   188,   188,   188,
     189,   190,   190,   190,   190,   191,   192,   193,   193,   193,
     193,   193,   194,   195,   195,   196,   196,   197,   198,   198,
     198,   199,   199,   199,   199,   200,   200,   201,   201,   202,
     202,   202,   202,   202,   202,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   204,   205,   205,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   208,   208,   209,   210,   210,   211,   211,
     212,   212,   213,   214,   214,   215,   215,   216,   216,   216,
     216,   216,   217,   217,   217,   218,   218,   218,   218,   218,
     219,   219,   220,   221,   221,   221,   222,   223,   224,   224,
     225,   226,   227,   227,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   230,
     230,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   232,   232,   232,   232,
     233,   233,   234,   234,   235,   235,   236,   236,   237,   238,
     238,   239,   239,   240,   240,   241,   241,   242,   243,   243,
     244
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     2,     3,     0,     2,     4,     3,     2,
       2,     1,     3,     1,     3,     1,     2,     1,     3,     1,
       3,     0,     2,     4,     4,     6,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     1,     1,     1,     4,
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     6,     3,     7,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     2,     0,     2,     2,     3,     1,     3,     2,
       2,     2,     4,     5,     2,     1,     1,     2,     3,     4,
       2,     3,     4,     0,     2,     1,     1,     3,     5,     5,
       5,     5,     8,     6,     9,     1,     3,     1,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     5,     6,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     3,
       3,     2,     3,     4,     3,     4,     4,     4,     3,     4,
       3,     5,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     3,     0,     1,     3,     0,     3,     1,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     3,     5,
       3,     3,     1,     1,     1,     4,     6,     5,     4,     3,
       0,     2,     4,     0,     3,     5,     4,     5,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     4,     3,     4,     2,     3,     2,
       3,     4,     2,     4,     6,     4,     1,     4,     5,     1,
       4,     5,     1,     4,     5,     4,     1,     1,     1,     1,
       3,     5,     1,     3,     5,     7,     1,     3,     3,     1,
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
          case 116: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2498 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2504 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2510 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2516 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2522 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2528 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2534 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2540 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2546 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2552 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2558 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2564 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2570 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2576 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2582 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2588 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2594 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2600 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2606 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2612 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2618 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2624 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2630 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2636 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2642 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2648 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2654 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2660 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2666 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2672 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2678 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2684 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2690 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2696 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2702 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2708 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2714 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2720 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2726 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2732 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2738 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2744 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2750 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2756 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2762 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2768 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2774 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2780 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2786 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2792 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2798 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2804 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2810 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2816 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2822 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2828 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2834 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2840 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2846 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2852 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2858 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2864 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2870 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2876 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2882 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2888 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2894 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2900 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2906 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2912 "ds_parser.cpp" /* yacc.c:1254  */
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
        case 12:
#line 361 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 362 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 366 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 370 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 374 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 383 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 391 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3263 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 407 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 437 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 440 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3305 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 449 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto modN = getModuleName(*(yyvsp[0].s));
        if ( auto mod = g_Program->addModule(modN) ) {
            g_Program->thisModule->requireModule.insert(mod);
        } else {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3320 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 471 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 474 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 481 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 489 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 496 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3380 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 508 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3392 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 518 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 536 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 554 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 573 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 585 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 589 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 590 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 591 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 595 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3554 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3572 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 610 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 611 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 612 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 613 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3596 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 614 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 615 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 619 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 620 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 622 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 623 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 624 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 625 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 626 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 627 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 628 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 629 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 630 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 631 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 632 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 633 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 635 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 636 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3746 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3867 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 732 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 733 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 735 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 738 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3939 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 799 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 802 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 806 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4071 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 812 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 816 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 820 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 826 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 831 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4127 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 842 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 848 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 849 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 853 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 858 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4202 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4241 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 916 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 920 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 928 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 931 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 937 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 943 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4339 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 994 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4351 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 997 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1005 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1006 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1010 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1011 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1013 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1014 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1018 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1022 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1033 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4493 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1038 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4505 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1048 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4511 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1049 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1050 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1051 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4529 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1052 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1053 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1055 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1056 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1057 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1058 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1059 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1060 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1061 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1062 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1063 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1064 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1065 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1066 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1067 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1068 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1069 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1070 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1076 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1077 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1079 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1080 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1082 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1083 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1085 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4742 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1090 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1094 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4767 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4773 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1102 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4785 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1103 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4797 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1108 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1109 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1111 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1168 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1169 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4919 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1173 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4925 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1174 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4931 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1178 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4949 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1201 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1249 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5075 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5081 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1268 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5121 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1285 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1288 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1348 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1360 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5247 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5253 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1418 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5368 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1484 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5380 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1491 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1500 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5397 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1501 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1504 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1505 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1506 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1507 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1508 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1509 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5451 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1510 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1511 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5469 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1513 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5475 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1514 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5481 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1515 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1516 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5493 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5499 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1518 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5505 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1519 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5511 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1520 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1521 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5529 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1527 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1538 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1552 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1553 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1554 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1555 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5606 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1569 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1579 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5636 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1584 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1589 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1594 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5666 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5678 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1606 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1611 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1616 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1622 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1627 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1631 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1636 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1645 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1649 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1654 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5785 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1663 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1667 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5804 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1672 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1681 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1690 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1691 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1692 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1693 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5853 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1697 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1704 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1713 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1718 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1730 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1739 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1742 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5940 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1756 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1770 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5973 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1775 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1803 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1810 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1822 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1827 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6068 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6072 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1842 "ds_parser.ypp" /* yacc.c:1903  */


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


