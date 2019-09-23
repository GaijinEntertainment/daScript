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
#define YYLAST   4289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  155
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  349
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  623

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
       0,   268,   268,   270,   271,   272,   273,   274,   275,   276,
     277,   281,   282,   286,   290,   294,   303,   311,   327,   348,
     352,   353,   357,   360,   369,   380,   384,   385,   389,   392,
     398,   399,   400,   407,   414,   426,   436,   445,   446,   447,
     448,   449,   450,   454,   459,   467,   468,   472,   479,   491,
     495,   502,   503,   507,   508,   509,   513,   517,   524,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   562,   563,   567,   637,   640,   650,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,   667,   671,   677,   683,   697,   698,   704,   708,
     714,   717,   720,   724,   730,   734,   738,   741,   744,   749,
     752,   760,   766,   767,   771,   772,   776,   809,   812,   817,
     825,   829,   834,   838,   846,   849,   855,   860,   912,   913,
     914,   915,   916,   917,   921,   922,   923,   924,   925,   926,
     927,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     940,   951,   956,   966,   967,   968,   969,   970,   971,   972,
     973,   974,   975,   976,   977,   978,   979,   980,   981,   982,
     983,   984,   985,   986,   987,   988,   989,   990,   991,   992,
     993,   994,   995,   996,   997,   998,   999,  1000,  1001,  1002,
    1003,  1004,  1008,  1012,  1015,  1018,  1019,  1020,  1021,  1022,
    1023,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1042,  1052,
    1086,  1087,  1091,  1092,  1096,  1104,  1107,  1114,  1115,  1119,
    1120,  1124,  1130,  1131,  1135,  1136,  1140,  1145,  1148,  1152,
    1158,  1167,  1168,  1169,  1173,  1176,  1181,  1186,  1193,  1203,
    1206,  1213,  1242,  1245,  1254,  1266,  1278,  1292,  1293,  1297,
    1336,  1402,  1409,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
    1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,
    1435,  1436,  1437,  1438,  1439,  1440,  1441,  1445,  1456,  1460,
    1470,  1471,  1472,  1473,  1487,  1493,  1497,  1502,  1507,  1512,
    1517,  1524,  1529,  1534,  1540,  1545,  1549,  1554,  1563,  1567,
    1572,  1581,  1585,  1590,  1599,  1608,  1609,  1610,  1611,  1615,
    1622,  1631,  1636,  1643,  1648,  1657,  1660,  1674,  1683,  1684,
    1688,  1693,  1700,  1705,  1716,  1721,  1728,  1740,  1741,  1745
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEXER_ERROR", "DAS_STRUCT", "DAS_LET",
  "DAS_DEF", "DAS_WHILE", "DAS_IF", "DAS_ELSE", "DAS_FOR", "DAS_CATCH",
  "DAS_TRUE", "DAS_FALSE", "DAS_NEWT", "DAS_TYPEINFO", "DAS_TYPE",
  "DAS_IN", "DAS_ELIF", "DAS_ARRAY", "DAS_RETURN", "DAS_NULL", "DAS_BREAK",
  "DAS_TRY", "DAS_OPTIONS", "DAS_TABLE", "DAS_EXPECT", "DAS_CONST",
  "DAS_REQUIRE", "DAS_OPERATOR", "DAS_ENUM", "DAS_FINALLY", "DAS_DELETE",
  "DAS_DEREF", "DAS_SCOPE", "DAS_TYPEDEF", "DAS_WITH", "DAS_CAST",
  "DAS_OVERRIDE", "DAS_UPCAST", "DAS_ITERATOR", "DAS_VAR", "DAS_ADDR",
  "DAS_CONTINUE", "DAS_WHERE", "DAS_PASS", "DAS_REINTERPRET", "DAS_TBOOL",
  "DAS_TVOID", "DAS_TSTRING", "DAS_TAUTO", "DAS_TINT", "DAS_TINT2",
  "DAS_TINT3", "DAS_TINT4", "DAS_TUINT", "DAS_TUINT2", "DAS_TUINT3",
  "DAS_TUINT4", "DAS_TFLOAT", "DAS_TFLOAT2", "DAS_TFLOAT3", "DAS_TFLOAT4",
  "DAS_TRANGE", "DAS_TURANGE", "DAS_TBLOCK", "DAS_TINT64", "DAS_TUINT64",
  "DAS_TDOUBLE", "DAS_TFUNCTION", "DAS_TLAMBDA", "DAS_TINT8", "DAS_TUINT8",
  "DAS_TINT16", "DAS_TUINT16", "DAS_TTUPLE", "ADDEQU", "SUBEQU", "DIVEQU",
  "MULEQU", "MODEQU", "ANDEQU", "OREQU", "XOREQU", "SHL", "SHR", "ADDADD",
  "SUBSUB", "LEEQU", "SHLEQU", "SHREQU", "GREQU", "EQUEQU", "NOTEQU",
  "RARROW", "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE", "CLONEEQU",
  "ROTL", "ROTR", "ROTLEQU", "ROTREQU", "MAPTO", "BRABRAB", "BRACBRB",
  "CBRCBRB", "INTEGER", "LONG_INTEGER", "UNSIGNED_INTEGER",
  "UNSIGNED_LONG_INTEGER", "FLOAT", "DOUBLE", "NAME", "BEGIN_STRING",
  "STRING_CHARACTER", "END_STRING", "BEGIN_STRING_EXPR", "END_STRING_EXPR",
  "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'",
  "'+'", "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'",
  "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'",
  "'('", "')'", "COLCOL", "';'", "'{'", "'}'", "'$'", "$accept", "program",
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

#define YYPACT_NINF -421

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-421)))

#define YYTABLE_NINF -155

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -421,    22,  -421,  -421,   -71,   -52,   -16,    -4,    -1,  -421,
     -23,  -421,  -421,  -421,   138,  -421,   -91,  -421,  -421,  -421,
    -421,   -48,  -421,    12,    24,    32,  -421,  -421,    54,    37,
    -421,    41,    75,  -421,    60,    88,    68,  -421,   -67,  -421,
     111,   -15,  -421,   398,   -71,   123,   -52,   -16,   121,  -421,
    2639,   129,  -421,   -71,   -23,  -421,   126,   100,  3599,   226,
     229,  -421,   124,   -85,  -421,  -421,  -421,  -421,  -421,   144,
    -421,  -421,  -421,  -421,  -421,  -421,   -80,   137,   140,   142,
    -421,  -421,  -421,   131,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,   145,  -421,
    -421,  -421,   148,   151,  -421,  -421,  -421,  -421,   152,  -421,
    -421,  -421,  -421,    51,  -421,   -83,  -421,   -90,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,   -29,   161,  -421,  -421,
    -421,    -9,  -421,   103,   -86,  -421,  2639,  2639,  2639,   171,
    2569,  2569,  2569,  2639,  -421,    27,  -421,  -421,  1506,  -421,
    -421,   -50,   172,  -421,  -421,    79,  -421,   118,  2639,   139,
    -421,  -421,   173,  -421,  2639,  2320,  -421,  -421,   180,  -421,
      77,     1,   114,   147,   161,   302,   161,   329,   161,   339,
    -421,    13,    51,   169,  -421,   153,  -421,  -421,  2499,   160,
    -421,   176,   198,   199,   182,   203,   185,  2320,  2320,  2429,
    2639,  2320,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  2320,
    2320,   -90,  2320,  2320,  -421,  2320,  -421,   189,  -421,  -421,
     -84,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  3621,   190,
    -421,  -421,  -421,  -421,  -421,   -71,  -421,   316,   204,  -421,
    -421,   -25,  -421,  -421,  2639,  2320,    51,  -421,  -421,   274,
    -421,   258,  2645,   212,  -421,  2639,  -421,  -421,   234,  -421,
     235,  -421,   237,  -421,  -421,  2639,  -421,  -421,    78,  -421,
     254,  2320,  2639,  2639,  2320,  2639,  1636,   457,   457,   172,
     870,  1000,  3749,  -421,    81,   -21,   457,   457,  -421,   457,
     457,  3195,   573,   124,  -421,  1130,  2320,  2320,  -421,  -421,
    2320,  2320,  2320,  2320,   260,  2320,   264,  2320,  2320,  2320,
    2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,
    2320,  2320,   267,  2320,  -421,  1750,   -45,  -421,   172,  -421,
    -421,   299,  -421,  2949,  -421,  2320,   107,  2320,  2320,  2320,
    2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,  2320,
    2320,  2320,  -421,  -421,   346,  -421,  -421,  -421,  -421,  1864,
    1244,  3261,   372,   373,  3327,   381,  -421,  3393,     6,   -74,
    1978,  3749,   252,  -104,   262,  -421,  -421,    38,   112,  2320,
    2320,   233,  -421,  2320,   269,  -421,  -421,  2320,  2320,   172,
    1374,  -421,   139,  2320,  2320,  -421,   240,  -421,   357,  -421,
    -421,  -421,  -421,  -421,  -421,   242,   250,   255,  -421,  -421,
     386,  -421,   -69,  3020,   161,   291,  -421,   -55,  3917,  4128,
    4128,  4086,  4086,  4066,  4066,   261,   457,  -421,   -56,   -56,
    4128,  4128,  3805,  3937,  3993,  4013,  4086,  4086,  4143,  4143,
     452,   452,   452,  -421,  3685,  -421,   -43,  -421,  -421,  2320,
    -421,  2721,  -421,  3917,  3917,  3917,  3917,  3917,  3917,  3917,
    3917,  3917,  3917,  3917,  3917,  3917,  3917,  3917,  -421,  -421,
     -41,  2639,   298,  3459,  -421,  2320,  2320,  -421,  2320,  -421,
    2320,  2320,   291,   300,   268,   291,  2320,   270,  2320,   271,
    3917,  -421,  -421,  3861,  3076,  3076,    18,  2320,  -421,  -421,
    2797,   408,  3917,  3076,  -421,   272,  -421,  -421,  -421,   387,
     172,  -421,   139,   -53,    43,  2320,  -421,  2092,  2320,  -421,
    -421,  3917,  -421,  -421,     2,   301,  -421,  4086,  4086,  4086,
     -66,  3917,    45,   -53,  -421,   252,  3917,  -421,  -421,  -421,
    -421,  -421,    44,  2320,  -421,  2873,  -421,   139,  -421,  -421,
    -421,  -421,  -421,   280,  3917,  -421,   -11,  3917,  -421,  2206,
    2320,   286,  2320,   139,  2320,  -421,   -63,  -421,  -421,   727,
    -421,  -421,  2639,  3525,  3139,  -421,  3917,  -421,  3076,   139,
    -421,    42,  -421,   390,   288,    44,  -421,  -421,  2320,   289,
    -421,  3917,  -421
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    51,     1,   134,     0,     0,     0,     0,     0,   135,
       0,     9,     8,     7,     0,     6,     0,     5,    10,     4,
       3,     0,    43,    18,    28,    25,    26,    22,    19,    24,
      20,     0,     0,    46,   116,     0,    47,    49,     0,    45,
       0,     0,   259,     0,     0,     0,     0,     0,     0,   262,
       0,     0,   118,     0,     0,    52,   267,     0,     0,     0,
       0,    58,    53,     0,    41,    42,    39,    40,    38,     0,
      37,    44,    29,    27,    21,    23,     0,     0,     0,     0,
     273,   293,   274,   298,   275,   279,   280,   281,   282,   286,
     287,   288,   289,   290,   291,   292,   294,   295,   315,   278,
     285,   296,   318,   321,   276,   283,   277,   284,     0,   297,
     300,   302,   301,   265,   117,     0,    50,     0,   269,   235,
      61,    62,    64,    63,    65,    66,    67,    68,    87,    88,
      85,    86,    78,    89,    90,    79,    76,    77,    91,    92,
      93,    94,    81,    82,    80,    74,    75,    70,    69,    71,
      72,    73,    60,    59,    83,    84,     0,    56,   271,   261,
     260,     0,    11,     0,     0,   266,     0,     0,     0,     0,
      53,    53,    53,     0,   306,     0,   311,   308,     0,    48,
     268,   230,     0,    54,   239,     0,   237,   246,     0,     0,
     252,   253,     0,   251,     0,     0,    12,    13,     0,   263,
       0,     0,     0,     0,    56,     0,    56,     0,    56,     0,
     242,     0,   241,   307,   309,     0,   178,   179,     0,     0,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   148,   150,   149,   151,   152,   153,    14,     0,
       0,     0,     0,     0,   304,     0,   112,    51,   176,   146,
     174,   223,   222,   221,   226,   175,   225,   224,     0,     0,
     177,   325,   326,   327,   328,     0,   270,   232,     0,   238,
      55,     0,   245,   244,     0,     0,    57,    95,   272,     0,
     258,     0,   154,     0,   312,     0,   314,   299,     0,   316,
       0,   319,     0,   322,   324,     0,   310,   305,   120,   123,
       0,     0,     0,     0,     0,     0,     0,   202,   203,     0,
       0,     0,     0,   344,     0,     0,   183,   182,   210,   181,
     180,     0,     0,    53,   229,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,     0,     0,   233,     0,   236,
     240,   247,   250,   249,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   257,   264,     0,   317,   320,   323,   243,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,   116,
       0,   335,   331,     0,   338,   339,   340,     0,     0,     0,
       0,     0,    17,     0,    15,   206,   114,     0,     0,     0,
       0,   124,     0,     0,     0,   125,     0,    98,    96,   109,
     105,   103,   104,   113,    99,     0,     0,     0,   108,   110,
     132,   102,     0,   154,    56,     0,   211,     0,   144,   184,
     185,   197,   198,   195,   196,     0,   219,   209,   227,   228,
     186,   187,     0,   200,   201,   199,   193,   194,   189,   188,
     190,   191,   192,   208,     0,   213,     0,   231,   234,     0,
     256,   154,   115,   161,   162,   164,   163,   165,   158,   159,
     160,   166,   167,   156,   157,   168,   169,   155,   313,   121,
       0,     0,     0,     0,   215,     0,     0,   216,     0,   218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,   345,   346,     0,     0,     0,     0,     0,   126,   129,
     154,     0,   119,     0,   111,     0,   101,   106,   107,     0,
       0,   100,     0,     0,     0,     0,   212,     0,     0,   207,
     214,   248,   255,   122,     0,     0,   140,   137,   138,   139,
       0,   329,     0,     0,   333,   332,   336,   342,   341,   343,
      16,    35,    30,     0,   130,   154,   127,     0,    36,   112,
     133,   136,   147,     0,   145,   171,     0,   220,   142,     0,
       0,     0,     0,     0,     0,    33,     0,   128,   131,     0,
     170,   172,     0,     0,   347,   334,   330,    31,     0,     0,
      97,     0,   141,     0,     0,    30,    34,   143,     0,     0,
      32,   348,   349
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -421,  -421,   122,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
     391,  -421,  -421,   393,  -175,  -421,  -421,  -421,  -421,   397,
     -47,  -421,   388,  -421,   200,  -152,  -195,  -421,  -421,  -165,
    -421,  -133,  -191,   -10,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,   448,  -421,  -421,  -421,  -338,    84,  -421,  -315,  -421,
    -421,   -28,  -421,  -421,  -421,  -421,   183,  -421,   163,  -421,
    -184,  -172,   177,   -79,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,   -61,   -49,  -421,  -421,   -35,   244,  -420,   -57,
    -421,  -421,  -223,   -58,   154,  -421,  -421,  -421,  -421,  -421
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   163,    70,   315,   248,    11,    12,    28,    29,
      30,    13,    25,    26,   595,   429,   430,   431,   432,    22,
      23,    36,    37,    38,    14,   157,   189,    62,    15,   249,
     433,   322,   434,   250,   435,   251,   436,   437,   438,   439,
     540,   440,   441,   252,   253,   447,   254,   255,   281,   256,
     257,   448,   267,   358,   268,   181,   184,   185,   210,   211,
     511,   186,   195,   160,    63,    17,    76,    18,    19,   118,
      57,    20,   187,   259,   111,   112,   212,   260,   402,   403,
     261,   404,   405,   406,   407,   262,   314,   263,   614,   264
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   110,   161,   275,   280,    33,   115,   442,   313,   288,
     269,   290,   182,   292,    58,   113,   182,   476,   204,   206,
     208,   272,     2,   510,   277,   544,    34,     3,   174,   174,
     366,   158,   324,   175,   175,   573,   164,   198,   334,    44,
     109,   336,   272,   514,    39,    21,     4,   515,     5,   273,
       6,   500,     7,   593,   213,    54,   545,     8,    24,   545,
      35,    42,   594,     9,   325,   199,   179,   545,   159,   174,
     273,    59,    60,   165,   175,    43,    51,    44,   174,   545,
      55,   545,   541,   175,   362,   590,   190,   158,   609,   352,
     353,   158,   191,    34,   546,   565,   265,   162,   412,   413,
      27,    61,   477,   266,   174,   174,   550,   180,   553,   175,
     175,   545,    31,   192,   193,    32,   194,   110,   110,   110,
     183,   110,   110,   110,   110,   176,   176,    35,   192,   177,
     177,   200,   201,   202,    44,   205,   207,   209,   601,   110,
     192,   174,    40,   294,    41,   110,   175,   178,   178,    45,
     258,   588,   285,   276,    46,   214,   109,   109,   109,   279,
     109,   109,   109,   109,   295,   513,   176,   282,    10,   110,
     177,   444,   560,   215,   480,   176,    47,   479,   109,   177,
     110,   110,    48,   298,   109,   517,   478,   521,   178,   518,
     583,   617,   591,    49,   310,   311,   515,   178,    50,   307,
     308,   176,   176,   312,    52,   177,   177,   284,   109,   586,
      51,   316,   317,   272,   319,   320,    53,   321,   356,   109,
     109,   196,   197,   178,   178,   110,   389,    56,   270,   529,
     271,   318,   410,    72,   411,   596,   110,    75,   176,   361,
     192,   273,   177,   274,   286,   114,   110,   363,   398,   542,
     384,   117,   119,   110,   110,   154,   110,   531,   155,   246,
     178,   247,   162,   518,   109,   519,   166,   392,   393,   167,
     395,   168,   156,   391,   170,   109,   394,   171,   397,   169,
     172,   173,   401,   401,   442,   109,   188,   203,   158,   278,
     283,   246,   109,   109,   443,   109,   287,   296,   449,   450,
     297,   174,   451,   452,   453,   454,   175,   456,   300,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   301,   474,   174,   302,   303,   174,
     304,   175,   305,   306,   175,    10,   574,   481,   355,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   357,   359,   174,   366,   526,   571,
     572,   175,   503,   383,   385,   386,   174,   387,   578,   190,
     390,   175,   321,   174,   513,   191,   455,   582,   175,   592,
     457,   520,   312,   473,   516,   523,   522,   196,   535,   524,
     525,   534,   530,   536,   272,   532,   533,   193,   176,   174,
     174,   537,   177,   539,   175,   175,   538,   543,   174,   547,
      64,    65,   598,   175,   555,   564,   563,   567,   569,   577,
     178,   580,   273,   176,   579,   364,   176,   177,   607,   600,
     177,   589,   289,   605,   618,   619,   622,   414,    74,    73,
     620,    71,   116,   615,   616,   178,   599,   323,   178,    16,
     482,   551,   110,   176,   360,   562,   365,   177,   388,   291,
     568,   581,   299,   176,     0,   408,   554,   177,     0,   293,
     176,     0,     0,     0,   177,   178,   498,   557,   558,   161,
     559,     0,     0,   561,     0,   178,     0,     0,   566,     0,
     401,   109,   178,     0,     0,     0,   176,   176,     0,   575,
     177,   177,   505,   506,     0,   176,     0,     0,    66,   177,
       0,   508,    67,     0,    68,    69,     0,   584,   178,   178,
     587,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   328,   329,
       0,     0,     0,   328,   329,     0,   334,     0,   335,   336,
     337,   334,   338,   110,   336,   337,     0,   338,     0,     0,
       0,   603,   604,     0,   606,     0,   608,   611,     0,     0,
       0,   443,     0,     0,   416,     0,     0,     0,     3,     0,
     417,   418,     0,   419,     0,   216,   217,   218,   219,     0,
     621,     0,   109,   420,   220,   421,   422,   352,   353,     0,
       0,     0,   352,   353,     0,   423,   221,     0,     0,   424,
     222,     0,   223,     0,     9,   224,   425,     0,   426,   225,
      80,    81,    82,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,    99,
     100,   101,     0,   226,   104,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    34,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,   240,     0,     0,     0,   241,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
       0,   245,     0,    35,   427,   246,   428,   247,   416,     0,
       0,     0,     3,     0,   417,   418,     0,   419,     0,   216,
     217,   218,   219,     0,     0,     0,     0,   420,   220,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,   423,
     221,     0,     0,   424,   222,     0,   223,     0,     9,   224,
     425,     0,   426,   225,    80,    81,    82,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,    99,   100,   101,     0,   226,   104,   105,
     106,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,   230,   231,   232,   233,   234,
     235,   236,   237,    34,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   240,
       0,     0,     0,   241,     0,     0,   242,   243,     0,     0,
       0,     0,     0,     0,     0,   245,     0,    35,   427,   246,
     610,   247,   216,   217,   218,   219,     0,     0,     0,     0,
       0,   220,     0,     0,     0,     0,     0,   174,     0,     0,
       0,     0,   175,   221,     0,     0,     0,   222,     0,   223,
       0,     0,   224,     0,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   399,   238,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,   177,     0,
       0,   239,   240,     0,     0,     0,   241,     0,     0,   242,
     243,     0,   216,   217,   218,   219,   178,     0,   400,     0,
      35,   220,   246,     0,   247,     0,     0,   174,     0,     0,
       0,     0,   175,   221,     0,     0,     0,   222,     0,   223,
       0,     0,   224,     0,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,   177,     0,
       0,   239,   240,     0,     0,     0,   241,     0,     0,   242,
     243,     0,   216,   217,   218,   219,   178,     0,   245,     0,
      35,   220,   246,     0,   247,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,     0,     0,   222,     0,   223,
       0,     0,   224,     0,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,     0,
       0,     0,     0,     0,     0,     0,   216,   217,   218,   219,
     501,   239,   240,     0,     0,   220,   241,     0,     0,   242,
     243,     0,     0,     0,     0,     0,   445,   221,   245,   446,
      35,   222,   246,   223,   247,     0,   224,     0,     0,     0,
     225,    80,    81,    82,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,     0,   226,   104,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      34,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,     0,   239,   240,     0,     0,     0,
     241,     0,     0,   242,   243,     0,   216,   217,   218,   219,
       0,     0,   245,     0,    35,   220,   246,     0,   247,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,     0,
       0,   222,     0,   223,     0,     0,   224,     0,     0,     0,
     225,    80,    81,    82,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,     0,   226,   104,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,     0,     0,     0,     0,     0,     0,     0,   527,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      34,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   240,     0,     0,     0,
     241,     0,     0,   242,   243,     0,     0,     0,   216,   217,
     218,   219,   245,     0,    35,   528,   246,   220,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,     0,     0,   222,     0,   223,     0,     0,   224,     0,
       0,     0,   225,    80,    81,    82,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,    99,   100,   101,     0,   226,   104,   105,   106,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    34,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,   240,     0,
       0,     0,   241,     0,     0,   242,   243,     0,   216,   217,
     218,   219,     0,   244,   245,     0,    35,   220,   246,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,     0,     0,   222,     0,   223,     0,     0,   224,     0,
       0,     0,   225,    80,    81,    82,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,    99,   100,   101,     0,   226,   104,   105,   106,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    34,   238,     0,     0,     0,     0,     0,     0,
       0,     0,   216,   217,   218,   219,     0,   239,   240,     0,
       0,   220,   241,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   221,   245,   396,    35,   222,   246,   223,
     247,     0,   224,     0,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,     0,
       0,     0,     0,     0,     0,     0,   216,   217,   218,   219,
       0,   239,   240,     0,     0,   220,   241,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   221,   245,   475,
      35,   222,   246,   223,   247,     0,   224,     0,     0,     0,
     225,    80,    81,    82,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,     0,   226,   104,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      34,   238,     0,     0,     0,     0,     0,     0,     0,     0,
     216,   217,   218,   219,     0,   239,   240,     0,     0,   220,
     241,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   221,   245,   499,    35,   222,   246,   223,   247,     0,
     224,     0,     0,     0,   225,    80,    81,    82,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,    99,   100,   101,     0,   226,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   227,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,   230,   231,   232,   233,
     234,   235,   236,   237,    34,   238,     0,     0,     0,     0,
       0,     0,     0,     0,   216,   217,   218,   219,     0,   239,
     240,     0,     0,   220,   241,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   221,   245,   512,    35,   222,
     246,   223,   247,     0,   224,     0,     0,     0,   225,    80,
      81,    82,     0,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,    99,   100,
     101,     0,   226,   104,   105,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   227,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,   231,   232,   233,   234,   235,   236,   237,    34,   238,
       0,     0,     0,     0,     0,     0,     0,     0,   216,   217,
     218,   219,   602,   239,   240,     0,     0,   220,   241,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   221,
     245,   585,    35,   222,   246,   223,   247,     0,   224,     0,
       0,     0,   225,    80,    81,    82,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,    99,   100,   101,     0,   226,   104,   105,   106,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    34,   238,     0,     0,     0,     0,     0,     0,
       0,     0,   216,   217,   218,   219,     0,   239,   240,     0,
       0,   220,   241,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   221,   245,     0,    35,   222,   246,   223,
     247,     0,   224,     0,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,   239,   240,     0,    78,     0,   241,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,     0,   245,    79,
      35,     0,   246,     0,   247,     0,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,    34,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,   230,   231,    79,
       0,     0,     0,     0,     0,    34,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,    34,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,     0,     0,   156,     0,    35,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   326,
     327,   328,   329,   330,   375,   376,   331,   332,   333,   334,
     377,   335,   336,   337,     0,   338,   378,   339,   340,   379,
     380,     0,     0,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,    35,
     352,   353,     0,     0,     0,     0,   382,   367,   368,   369,
     370,   371,   372,   373,   374,   326,   327,   328,   329,   330,
     375,   376,   331,   332,   333,   334,   377,   335,   336,   337,
       0,   338,   378,   339,   340,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,     0,     0,
       0,     0,   552,   367,   368,   369,   370,   371,   372,   373,
     374,   326,   327,   328,   329,   330,   375,   376,   331,   332,
     333,   334,   377,   335,   336,   337,     0,   338,   378,   339,
     340,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,     0,     0,     0,     0,   576,   367,
     368,   369,   370,   371,   372,   373,   374,   326,   327,   328,
     329,   330,   375,   376,   331,   332,   333,   334,   377,   335,
     336,   337,     0,   338,   378,   339,   340,   379,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,     0,     0,     0,   597,   367,   368,   369,   370,   371,
     372,   373,   374,   326,   327,   328,   329,   330,   375,   376,
     331,   332,   333,   334,   377,   335,   336,   337,  -154,   338,
     378,   339,   340,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,   367,   368,   369,   370,
     371,   372,   373,   374,   326,   327,   328,   329,   330,   375,
     376,   331,   332,   333,   334,   377,   335,   336,   337,     0,
     338,   378,   339,   340,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   341,     0,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,     0,     0,     0,
     326,   327,   328,   329,   330,   352,   353,   331,   332,   333,
     334,     0,   335,   336,   337,     0,   338,     0,   339,   340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     341,     0,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   352,   353,   326,   327,   328,   329,   330,   246,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,   326,
     327,   328,   329,   330,   352,   353,   331,   332,   333,   334,
     613,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,     0,     0,   415,   326,   327,   328,   329,   330,
       0,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,     0,     0,
     504,   326,   327,   328,   329,   330,     0,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   338,     0,   339,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,     0,     0,   507,   326,   327,   328,
     329,   330,     0,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,     0,   509,   326,   327,   328,   329,   330,     0,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,     0,     0,   556,   326,
     327,   328,   329,   330,     0,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,     0,     0,   612,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,     0,     0,     0,     0,     0,     0,     0,
       0,   138,   139,   140,   141,   326,   327,   328,   329,   330,
       0,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,     0,     0,     0,   152,   153,
       0,     0,     0,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,   354,   326,
     327,   328,   329,   330,     0,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,   549,   326,   327,   328,   329,   330,     0,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,     0,     0,   409,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,   326,
     327,   328,   329,   330,   352,   353,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
     548,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,   326,   327,   328,   329,   330,
     352,   353,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,   326,   327,   328,   329,   330,   352,   353,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   338,     0,   339,
     340,   326,   327,   328,   329,   330,     0,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   338,     0,   339,
     340,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,   326,   327,   328,
     329,   330,   352,   353,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,   326,   327,   328,
     329,   330,     0,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,     0,     0,     0,
       0,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,     0,   345,   346,   347,   348,   349,   350,   351,     0,
     326,   327,   328,   329,   330,     0,     0,   331,   352,   353,
     334,     0,   335,   336,   337,     0,   338,     0,   339,   340,
     326,   327,   328,   329,     0,     0,     0,     0,     0,     0,
     334,     0,   335,   336,   337,     0,   338,     0,   339,   340,
       0,     0,     0,     0,     0,   345,   346,   347,   348,   349,
     350,   351,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   352,   353,     0,   328,   329,     0,   347,   348,   349,
     350,   351,   334,     0,   335,   336,   337,     0,   338,   328,
     329,   352,   353,     0,     0,     0,     0,   334,     0,   335,
     336,   337,     0,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     348,   349,   350,   351,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,   353,     0,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353
};

static const yytype_int16 yycheck[] =
{
      10,    50,    63,   187,   195,    28,    53,   322,   231,   204,
     182,   206,    41,   208,    29,    50,    41,   355,   170,   171,
     172,    95,     0,    17,   189,   445,   116,     5,    27,    27,
      99,   116,   116,    32,    32,    17,   116,   123,    94,   122,
      50,    97,    95,   147,    54,   116,    24,   151,    26,   123,
      28,   389,    30,     9,    27,   122,   122,    35,   110,   122,
     150,   152,    18,    41,   148,   151,   149,   122,   153,    27,
     123,    86,    87,   153,    32,   123,   150,   122,    27,   122,
     147,   122,   151,    32,   275,   151,    95,   116,   151,   145,
     146,   116,   101,   116,   149,   515,   146,   118,   119,   120,
     116,   116,   147,   153,    27,    27,   149,   117,   149,    32,
      32,   122,   116,   122,   123,   116,   125,   166,   167,   168,
     149,   170,   171,   172,   173,   124,   124,   150,   122,   128,
     128,   166,   167,   168,   122,   170,   171,   172,   149,   188,
     122,    27,     4,   130,     6,   194,    32,   146,   146,   125,
     178,   149,   151,   188,   122,   128,   166,   167,   168,   194,
     170,   171,   172,   173,   151,   122,   124,   195,   146,   218,
     128,   323,   510,   146,   365,   124,   122,   361,   188,   128,
     229,   230,   145,   218,   194,   147,   358,   410,   146,   151,
     147,   149,   147,   152,   229,   230,   151,   146,   123,   227,
     228,   124,   124,   231,   116,   128,   128,   130,   218,   547,
     150,   239,   240,    95,   242,   243,   148,   245,   265,   229,
     230,   118,   119,   146,   146,   274,   148,   116,   149,   420,
     151,   241,   151,   110,   153,   573,   285,   116,   124,   274,
     122,   123,   128,   125,   130,   116,   295,   275,   309,   444,
     285,   125,   152,   302,   303,    29,   305,   422,    29,   152,
     146,   154,   118,   151,   274,   153,   129,   302,   303,   129,
     305,   129,   148,   301,   129,   285,   304,   129,   306,   148,
     129,   129,   310,   311,   599,   295,   125,   116,   116,   116,
     110,   152,   302,   303,   322,   305,   149,   128,   326,   327,
     147,    27,   330,   331,   332,   333,    32,   335,   148,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   148,   353,    27,   129,   129,    27,
     148,    32,   129,   148,    32,   146,   527,   365,   148,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,    38,   151,    27,    99,   419,   524,
     525,    32,   390,   151,   130,   130,    27,   130,   533,    95,
     116,    32,   400,    27,   122,   101,   116,   542,    32,   563,
     116,   409,   410,   116,   122,   413,   153,   118,    31,   417,
     418,   151,   420,   151,    95,   423,   424,   123,   124,    27,
      27,   151,   128,    17,    32,    32,   151,   116,    27,   148,
      12,    13,   577,    32,   116,   147,   116,   147,   147,    11,
     146,    34,   123,   124,   152,   151,   124,   128,   593,   149,
     128,   130,   130,   147,    44,   147,   147,   315,    47,    46,
     615,    44,    54,   608,   609,   146,   579,   247,   146,     1,
     366,   479,   501,   124,   271,   512,   279,   128,   295,   130,
     518,   540,   218,   124,    -1,   311,   501,   128,    -1,   130,
     124,    -1,    -1,    -1,   128,   146,   130,   505,   506,   540,
     508,    -1,    -1,   511,    -1,   146,    -1,    -1,   516,    -1,
     518,   501,   146,    -1,    -1,    -1,   124,   124,    -1,   527,
     128,   128,   130,   130,    -1,   124,    -1,    -1,   110,   128,
      -1,   130,   114,    -1,   116,   117,    -1,   545,   146,   146,
     548,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    86,    87,    -1,    94,    -1,    96,    97,
      98,    94,   100,   602,    97,    98,    -1,   100,    -1,    -1,
      -1,   589,   590,    -1,   592,    -1,   594,   602,    -1,    -1,
      -1,   599,    -1,    -1,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    -1,
     618,    -1,   602,    20,    21,    22,    23,   145,   146,    -1,
      -1,    -1,   145,   146,    -1,    32,    33,    -1,    -1,    36,
      37,    -1,    39,    -1,    41,    42,    43,    -1,    45,    46,
      47,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,   136,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,   150,   151,   152,   153,   154,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,    42,
      43,    -1,    45,    46,    47,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,    -1,    -1,   136,    -1,    -1,   139,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,   150,   151,   152,
     153,   154,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,   139,
     140,    -1,    12,    13,    14,    15,   146,    -1,   148,    -1,
     150,    21,   152,    -1,   154,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,   139,
     140,    -1,    12,    13,    14,    15,   146,    -1,   148,    -1,
     150,    21,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,
      16,   131,   132,    -1,    -1,    21,   136,    -1,    -1,   139,
     140,    -1,    -1,    -1,    -1,    -1,   146,    33,   148,   149,
     150,    37,   152,    39,   154,    -1,    42,    -1,    -1,    -1,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
     136,    -1,    -1,   139,   140,    -1,    12,    13,    14,    15,
      -1,    -1,   148,    -1,   150,    21,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    -1,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,
     136,    -1,    -1,   139,   140,    -1,    -1,    -1,    12,    13,
      14,    15,   148,    -1,   150,   151,   152,    21,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,
      -1,    -1,   136,    -1,    -1,   139,   140,    -1,    12,    13,
      14,    15,    -1,   147,   148,    -1,   150,    21,   152,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    -1,   131,   132,    -1,
      -1,    21,   136,    -1,    -1,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    33,   148,   149,   150,    37,   152,    39,
     154,    -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,   131,   132,    -1,    -1,    21,   136,    -1,    -1,   139,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    33,   148,   149,
     150,    37,   152,    39,   154,    -1,    42,    -1,    -1,    -1,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,   131,   132,    -1,    -1,    21,
     136,    -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    33,   148,   149,   150,    37,   152,    39,   154,    -1,
      42,    -1,    -1,    -1,    46,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    -1,   131,
     132,    -1,    -1,    21,   136,    -1,    -1,   139,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    33,   148,   149,   150,    37,
     152,    39,   154,    -1,    42,    -1,    -1,    -1,    46,    47,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,   131,   132,    -1,    -1,    21,   136,    -1,
      -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     148,   149,   150,    37,   152,    39,   154,    -1,    42,    -1,
      -1,    -1,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    -1,   131,   132,    -1,
      -1,    21,   136,    -1,    -1,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    33,   148,    -1,   150,    37,   152,    39,
     154,    -1,    42,    -1,    -1,    -1,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,   131,   132,    -1,    25,    -1,   136,    -1,    -1,   139,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    40,
     150,    -1,   152,    -1,   154,    -1,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,   116,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,    40,
      -1,    -1,    -1,    -1,    -1,   116,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,   116,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    -1,   148,    -1,   150,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
     145,   146,    -1,    -1,    -1,    -1,   151,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,
      -1,    -1,   151,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,    -1,    -1,    -1,    -1,   151,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
      -1,    -1,    -1,    -1,   151,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,   145,   146,    91,    92,    93,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    84,    85,    86,    87,    88,   152,    -1,
      91,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,   145,   146,    91,    92,    93,    94,
     151,    96,    97,    98,    -1,   100,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,    -1,    -1,   149,    84,    85,    86,    87,    88,
      -1,    -1,    91,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,
     149,    84,    85,    86,    87,    88,    -1,    -1,    91,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,    -1,    -1,   149,    84,    85,    86,
      87,    88,    -1,    -1,    91,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
      -1,    -1,   149,    84,    85,    86,    87,    88,    -1,    -1,
      91,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,    -1,    -1,   149,    84,
      85,    86,    87,    88,    -1,    -1,    91,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,    -1,    -1,   149,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    84,    85,    86,    87,    88,
      -1,    -1,    91,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,   103,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,   139,   140,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    84,
      85,    86,    87,    88,    -1,    -1,    91,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    84,    85,    86,    87,    88,    -1,    -1,
      91,    92,    93,    94,    -1,    96,    97,    98,    -1,   100,
      -1,   102,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,   145,   146,    91,    92,    93,    94,
      -1,    96,    97,    98,    -1,   100,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
     145,   146,    91,    92,    93,    94,    -1,    96,    97,    98,
      -1,   100,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,   124,    -1,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,   145,   146,    91,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
     103,    84,    85,    86,    87,    88,    -1,    -1,    91,    92,
      93,    94,    -1,    96,    97,    98,    -1,   100,    -1,   102,
     103,   124,    -1,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,   145,   146,    91,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,   103,    84,    85,    86,
      87,    88,    -1,    -1,    91,    92,    93,    94,    -1,    96,
      97,    98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
      -1,    -1,   129,   130,   131,   132,   133,   134,   135,    -1,
      84,    85,    86,    87,    88,    -1,    -1,    91,   145,   146,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,    86,    87,    -1,   131,   132,   133,
     134,   135,    94,    -1,    96,    97,    98,    -1,   100,    86,
      87,   145,   146,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   156,     0,     5,    24,    26,    28,    30,    35,    41,
     146,   161,   162,   166,   179,   183,   196,   220,   222,   223,
     226,   116,   174,   175,   110,   167,   168,   116,   163,   164,
     165,   116,   116,    28,   116,   150,   176,   177,   178,   188,
       4,     6,   152,   123,   122,   125,   122,   122,   145,   152,
     123,   150,   116,   148,   122,   147,   116,   225,    29,    86,
      87,   116,   182,   219,    12,    13,   110,   114,   116,   117,
     158,   174,   110,   168,   165,   116,   221,    19,    25,    40,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,   188,
     228,   229,   230,   231,   116,   175,   177,   125,   224,   152,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,   102,   103,
     104,   105,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   139,   140,    29,    29,   148,   180,   116,   153,
     218,   227,   118,   157,   116,   153,   129,   129,   129,   148,
     129,   129,   129,   129,    27,    32,   124,   128,   146,   149,
     188,   210,    41,   149,   211,   212,   216,   227,   125,   181,
      95,   101,   122,   123,   125,   217,   118,   119,   123,   151,
     231,   231,   231,   116,   180,   231,   180,   231,   180,   231,
     213,   214,   231,    27,   128,   146,    12,    13,    14,    15,
      21,    33,    37,    39,    42,    46,    70,    86,    87,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   117,   131,
     132,   136,   139,   140,   147,   148,   152,   154,   160,   184,
     188,   190,   198,   199,   201,   202,   204,   205,   206,   228,
     232,   235,   240,   242,   244,   146,   153,   207,   209,   216,
     149,   151,    95,   123,   125,   215,   231,   184,   116,   231,
     187,   203,   206,   110,   130,   151,   130,   149,   181,   130,
     181,   130,   181,   130,   130,   151,   128,   147,   231,   232,
     148,   148,   129,   129,   148,   129,   148,   206,   206,    10,
     231,   231,   206,   237,   241,   159,   206,   206,   188,   206,
     206,   206,   186,   179,   116,   148,    84,    85,    86,    87,
      88,    91,    92,    93,    94,    96,    97,    98,   100,   102,
     103,   124,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   145,   146,   147,   148,   175,    38,   208,   151,
     211,   231,   187,   206,   151,   217,    99,    76,    77,    78,
      79,    80,    81,    82,    83,    89,    90,    95,   101,   104,
     105,   123,   151,   151,   231,   130,   130,   130,   213,   148,
     116,   206,   231,   231,   206,   231,   149,   206,   227,   116,
     148,   206,   233,   234,   236,   237,   238,   239,   239,   106,
     151,   153,   119,   120,   157,   149,     1,     7,     8,    10,
      20,    22,    23,    32,    36,    43,    45,   151,   153,   170,
     171,   172,   173,   185,   187,   189,   191,   192,   193,   194,
     196,   197,   203,   206,   180,   146,   149,   200,   206,   206,
     206,   206,   206,   206,   206,   116,   206,   116,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   116,   206,   149,   200,   147,   216,   215,
     187,   206,   201,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   130,   149,
     200,    16,   129,   206,   149,   130,   130,   149,   130,   149,
      17,   215,   149,   122,   147,   151,   122,   147,   151,   153,
     206,   237,   153,   206,   206,   206,   227,    95,   151,   187,
     206,   184,   206,   206,   151,    31,   151,   151,   151,    17,
     195,   151,   181,   116,   233,   122,   149,   148,   125,   147,
     149,   206,   151,   149,   231,   116,   149,   206,   206,   206,
     200,   206,   234,   116,   147,   233,   206,   147,   238,   147,
     121,   184,   184,    17,   187,   206,   151,    11,   184,   152,
      34,   218,   184,   147,   206,   149,   200,   206,   149,   130,
     151,   147,   215,     9,    18,   169,   200,   151,   184,   186,
     149,   149,    16,   206,   206,   147,   206,   184,   206,   151,
     153,   231,   149,   151,   243,   184,   184,   149,    44,   147,
     169,   206,   147
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   155,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   157,   157,   158,   159,   159,   159,   160,   161,   162,
     163,   163,   164,   164,   165,   166,   167,   167,   168,   168,
     169,   169,   169,   170,   171,   172,   173,   174,   174,   174,
     174,   174,   174,   175,   175,   176,   176,   177,   177,   178,
     178,   179,   179,   180,   180,   180,   181,   181,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   183,   184,   184,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   187,   188,   188,   188,   189,
     190,   190,   190,   190,   191,   192,   193,   193,   193,   193,
     193,   194,   195,   195,   196,   196,   197,   198,   198,   198,
     199,   199,   199,   199,   200,   200,   201,   201,   202,   202,
     202,   202,   202,   202,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     204,   205,   205,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     207,   207,   208,   208,   209,   210,   210,   211,   211,   212,
     212,   213,   214,   214,   215,   215,   216,   216,   216,   216,
     216,   217,   217,   217,   218,   218,   218,   218,   218,   219,
     219,   220,   221,   221,   221,   222,   223,   224,   224,   225,
     226,   227,   227,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   230,   230,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   232,   232,   232,   232,   233,
     233,   234,   234,   235,   235,   236,   236,   237,   238,   238,
     239,   239,   240,   240,   241,   241,   242,   243,   243,   244
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     3,     0,     2,     4,     3,     2,     2,
       1,     3,     1,     3,     1,     2,     1,     3,     1,     3,
       0,     2,     4,     4,     6,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     1,     1,     4,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     6,     3,     7,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     2,     0,     2,     2,     3,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     4,     0,     2,     1,     1,     3,     5,     5,     5,
       5,     8,     6,     9,     1,     3,     1,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     5,     6,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     3,     4,     3,     3,
       2,     3,     4,     3,     4,     4,     4,     3,     4,     3,
       5,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     0,     1,     3,     0,     3,     1,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     3,     5,     3,
       3,     1,     1,     1,     4,     6,     5,     4,     3,     0,
       2,     4,     0,     3,     5,     4,     5,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     3,     4,     2,     3,     2,     3,
       4,     2,     4,     6,     4,     1,     4,     5,     1,     4,
       5,     1,     4,     5,     4,     1,     1,     1,     1,     3,
       5,     1,     3,     5,     7,     1,     3,     3,     1,     1,
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
          case 116: /* NAME  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2501 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2507 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2513 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2519 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2525 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2531 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2537 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2543 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2549 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2555 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2561 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2567 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2573 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2579 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2585 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2591 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2597 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2603 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2609 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2615 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2621 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2627 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2633 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2639 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2645 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2651 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2657 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2663 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2669 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2675 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2681 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2687 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2693 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2699 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2705 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2711 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2717 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2723 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2729 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2735 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2741 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2747 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2753 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2759 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2765 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2771 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2777 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2783 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2789 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2795 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2801 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2807 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2813 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2819 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2825 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2831 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2837 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2843 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2849 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2855 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2861 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2867 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2873 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2879 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2885 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2891 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2897 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2903 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2909 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2915 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 281 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 282 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3209 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 286 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 290 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 294 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3238 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 303 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 311 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 327 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 357 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3297 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 360 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 369 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto modN = getModuleName(*(yyvsp[0].s));
        if ( !g_Program->addModule(modN) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 389 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 392 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3337 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3343 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3349 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 400 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 407 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3367 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 414 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 426 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 436 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 445 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 446 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3416 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 447 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 448 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 449 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3434 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 450 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 454 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 459 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 467 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 468 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 491 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 495 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3516 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 502 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 503 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3528 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 507 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3534 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 508 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3540 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 509 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 513 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 524 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3597 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 534 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 535 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 536 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 537 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3633 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3639 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 540 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3651 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3657 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 542 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3663 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 543 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3669 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 544 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3675 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 545 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 546 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 547 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3693 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 548 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3699 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3705 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3711 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 551 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 552 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3723 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 553 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 554 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 556 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3747 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 557 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 558 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 559 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 560 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 562 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 563 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3789 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 568 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3868 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3880 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3904 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3910 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3922 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3934 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3940 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3973 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 671 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 677 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 683 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 697 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 698 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 714 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4064 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 724 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 730 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 738 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4100 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4128 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4148 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4154 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4160 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 809 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 812 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 817 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4223 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 825 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 829 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4251 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 838 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4269 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 849 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 855 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4287 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 861 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 912 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 913 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 914 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 915 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 916 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 922 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 923 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 924 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 925 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 926 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 927 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 928 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 929 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 930 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 931 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 932 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 933 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 934 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 935 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 936 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 951 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 967 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4518 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 968 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4530 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 971 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 973 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4554 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 974 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4560 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 975 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4566 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4572 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 977 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 978 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 979 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 980 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4596 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 981 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 982 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 983 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 984 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 986 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 987 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 988 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 989 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 990 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 991 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 992 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 993 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 994 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 997 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1000 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1001 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1002 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4743 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1018 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1020 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4786 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1021 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1022 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4798 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1023 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1026 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1027 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1028 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1029 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1030 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4857 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1042 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1052 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1087 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1096 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4959 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1119 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4977 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1130 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1140 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1145 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1148 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1158 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5064 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1167 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5070 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1168 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5076 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1169 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1173 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1176 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5100 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1181 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5122 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5134 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5151 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5190 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1266 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1292 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1293 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1297 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1402 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1409 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5392 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1418 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1419 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1421 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5416 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1422 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1423 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1424 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5434 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1425 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1426 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5446 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1427 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5452 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1428 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5458 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1429 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5464 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1430 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5470 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1431 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1432 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1433 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1434 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1435 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1436 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1437 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5518 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1440 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5530 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1441 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1445 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5549 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1456 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1460 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5570 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1470 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1471 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1472 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1473 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1487 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5618 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1493 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1497 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1507 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5657 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1529 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5699 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1540 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1545 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1549 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1554 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1563 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5762 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1567 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5786 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1581 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1585 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1590 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5819 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1608 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1609 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1610 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1611 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1615 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1622 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1631 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1636 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5896 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1643 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1648 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1657 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5925 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1660 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1674 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1683 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1684 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1688 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1693 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1700 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1705 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6006 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6016 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1721 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1728 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1745 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6069 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6073 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1760 "ds_parser.ypp" /* yacc.c:1903  */


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


