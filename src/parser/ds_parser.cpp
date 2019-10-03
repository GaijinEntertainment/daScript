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
    DAS_MODULE = 302,
    DAS_TBOOL = 303,
    DAS_TVOID = 304,
    DAS_TSTRING = 305,
    DAS_TAUTO = 306,
    DAS_TINT = 307,
    DAS_TINT2 = 308,
    DAS_TINT3 = 309,
    DAS_TINT4 = 310,
    DAS_TUINT = 311,
    DAS_TUINT2 = 312,
    DAS_TUINT3 = 313,
    DAS_TUINT4 = 314,
    DAS_TFLOAT = 315,
    DAS_TFLOAT2 = 316,
    DAS_TFLOAT3 = 317,
    DAS_TFLOAT4 = 318,
    DAS_TRANGE = 319,
    DAS_TURANGE = 320,
    DAS_TBLOCK = 321,
    DAS_TINT64 = 322,
    DAS_TUINT64 = 323,
    DAS_TDOUBLE = 324,
    DAS_TFUNCTION = 325,
    DAS_TLAMBDA = 326,
    DAS_TINT8 = 327,
    DAS_TUINT8 = 328,
    DAS_TINT16 = 329,
    DAS_TUINT16 = 330,
    DAS_TTUPLE = 331,
    ADDEQU = 332,
    SUBEQU = 333,
    DIVEQU = 334,
    MULEQU = 335,
    MODEQU = 336,
    ANDEQU = 337,
    OREQU = 338,
    XOREQU = 339,
    SHL = 340,
    SHR = 341,
    ADDADD = 342,
    SUBSUB = 343,
    LEEQU = 344,
    SHLEQU = 345,
    SHREQU = 346,
    GREQU = 347,
    EQUEQU = 348,
    NOTEQU = 349,
    RARROW = 350,
    LARROW = 351,
    QQ = 352,
    QDOT = 353,
    LPIPE = 354,
    LBPIPE = 355,
    RPIPE = 356,
    CLONEEQU = 357,
    ROTL = 358,
    ROTR = 359,
    ROTLEQU = 360,
    ROTREQU = 361,
    MAPTO = 362,
    BRABRAB = 363,
    BRACBRB = 364,
    CBRCBRB = 365,
    INTEGER = 366,
    LONG_INTEGER = 367,
    UNSIGNED_INTEGER = 368,
    UNSIGNED_LONG_INTEGER = 369,
    FLOAT = 370,
    DOUBLE = 371,
    NAME = 372,
    BEGIN_STRING = 373,
    STRING_CHARACTER = 374,
    END_STRING = 375,
    BEGIN_STRING_EXPR = 376,
    END_STRING_EXPR = 377,
    UNARY_MINUS = 378,
    UNARY_PLUS = 379,
    PRE_INC = 380,
    PRE_DEC = 381,
    POST_INC = 382,
    POST_DEC = 383,
    COLCOL = 384
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

#line 356 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   4304

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  156
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  353
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  630

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   384

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   141,     2,     2,   155,   136,   129,     2,
     149,   150,   134,   133,   123,   132,   146,   135,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   126,   152,
     130,   124,   131,   125,   137,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   147,     2,   148,   128,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   153,   127,   154,   140,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   122,   138,   139,
     142,   143,   144,   145,   151
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   348,   348,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   363,   375,   376,   380,   384,   388,   397,
     405,   421,   442,   446,   447,   451,   454,   460,   469,   482,
     486,   487,   491,   494,   500,   501,   502,   509,   516,   528,
     538,   547,   548,   549,   550,   551,   552,   556,   561,   569,
     570,   574,   581,   593,   597,   604,   605,   609,   610,   611,
     615,   619,   626,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,   664,   665,   669,
     739,   742,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   769,   773,   779,   785,
     799,   800,   806,   810,   816,   819,   822,   826,   832,   836,
     840,   843,   846,   851,   854,   862,   868,   869,   873,   874,
     878,   911,   914,   919,   927,   931,   936,   940,   948,   951,
     957,   962,  1014,  1015,  1016,  1017,  1018,  1019,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,
    1035,  1036,  1037,  1038,  1042,  1053,  1058,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1110,  1114,  1117,  1120,
    1121,  1122,  1123,  1124,  1125,  1128,  1129,  1130,  1131,  1132,
    1133,  1134,  1144,  1154,  1188,  1189,  1193,  1194,  1198,  1206,
    1209,  1216,  1217,  1221,  1222,  1226,  1232,  1233,  1237,  1238,
    1242,  1247,  1250,  1254,  1260,  1269,  1270,  1271,  1275,  1278,
    1283,  1288,  1295,  1305,  1308,  1315,  1344,  1347,  1356,  1368,
    1380,  1394,  1395,  1399,  1438,  1504,  1511,  1520,  1521,  1522,
    1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,
    1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,  1542,
    1543,  1547,  1558,  1562,  1572,  1573,  1574,  1575,  1589,  1595,
    1599,  1604,  1609,  1614,  1619,  1626,  1631,  1636,  1642,  1647,
    1651,  1656,  1665,  1669,  1674,  1683,  1687,  1692,  1701,  1710,
    1711,  1712,  1713,  1717,  1724,  1733,  1738,  1745,  1750,  1759,
    1762,  1776,  1785,  1786,  1790,  1795,  1802,  1807,  1818,  1823,
    1830,  1842,  1843,  1847
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
  "\"module\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
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
  "'.'", "'['", "']'", "'('", "')'", "COLCOL", "';'", "'{'", "'}'", "'$'",
  "$accept", "program", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "options_declaration", "require_declaration", "require_list",
  "require_module_name", "require_module", "expect_declaration",
  "expect_list", "expect_error", "expression_else",
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
     375,   376,   377,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,    64,   378,   379,
     126,    33,   380,   381,   382,   383,    46,    91,    93,    40,
      41,   384,    59,   123,   125,    36
};
# endif

#define YYPACT_NINF -431

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-431)))

#define YYTABLE_NINF -159

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -431,    26,  -431,  -431,   -62,   -48,    -1,    29,    34,  -431,
      35,   -13,  -431,  -431,  -431,  -431,  -431,   106,  -431,  -114,
    -431,  -431,  -431,  -431,   -43,  -431,    42,    70,    86,  -431,
    -431,   120,   -88,  -431,    59,    98,    81,  -431,   122,   143,
     112,  -431,   -83,  -431,   154,   216,  -431,    93,   -62,   168,
     -48,    -1,   170,   171,  -431,  2707,  -431,   172,  -431,   -62,
     -13,  -431,   167,   141,  3667,   266,   267,  -431,   151,   -85,
    -431,  -431,  -431,  -431,  -431,   186,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,   -69,   176,   177,   179,  -431,  -431,  -431,
     162,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,   182,  -431,  -431,  -431,   184,
     185,  -431,  -431,  -431,  -431,   190,  -431,  -431,  -431,  -431,
      14,  -431,   -14,  -431,   -87,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,    -6,   192,  -431,  -431,  -431,   -36,  -431,
     -26,   -82,  -431,  2707,  2707,  2707,   204,  2637,  2637,  2637,
    2707,  -431,     6,  -431,  -431,  1565,  -431,  -431,     1,   206,
    -431,  -431,     7,  -431,   -24,  2707,   174,  -431,  -431,   207,
    -431,  2707,  2386,  -431,  -431,   214,  -431,    76,     2,   161,
     180,   192,   188,   192,   197,   192,   270,  -431,   -93,    14,
     200,  -431,   183,  -431,  -431,  2566,   191,  -431,   193,   202,
     213,   198,   218,   201,  2386,  2386,  2496,  2707,  2386,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  2386,  2386,   -87,  2386,
    2386,  -431,  2386,  -431,   187,  -431,  -431,   -81,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  3689,   203,  -431,  -431,  -431,
    -431,  -431,   -62,  -431,   299,   199,  -431,  -431,   -20,  -431,
    -431,  2707,  2386,    14,  -431,  -431,    -7,  -431,   249,  2713,
     208,  -431,  2707,  -431,  -431,   222,  -431,   223,  -431,   224,
    -431,  -431,  2707,  -431,  -431,    69,  -431,   239,  2386,  2707,
    2707,  2386,  2707,  1696,   432,   432,   206,   924,  1055,  3817,
    -431,    20,   164,   432,   432,  -431,   432,   432,  3263,   625,
     151,  -431,  1186,  2386,  2386,  -431,  -431,  2386,  2386,  2386,
    2386,   247,  2386,   252,  2386,  2386,  2386,  2386,  2386,  2386,
    2386,  2386,  2386,  2386,  2386,  2386,  2386,  2386,  2386,   254,
    2386,  -431,  1811,    80,  -431,   206,  -431,  -431,   110,  -431,
    3017,  -431,  2386,    87,  2386,  2386,  2386,  2386,  2386,  2386,
    2386,  2386,  2386,  2386,  2386,  2386,  2386,  2386,  2386,  -431,
    -431,   331,  -431,  -431,  -431,  -431,  1926,  1301,  3329,   364,
     365,  3395,   397,  -431,  3461,    -9,   -73,  2041,  3817,   271,
     -95,   275,  -431,  -431,   102,    99,  2386,  2386,   235,  -431,
    2386,   274,  -431,  -431,  2386,  2386,   206,  1432,  -431,   174,
    2386,  2386,  -431,   238,  -431,   389,  -431,  -431,  -431,  -431,
    -431,  -431,   269,   276,   278,  -431,  -431,   405,  -431,   -72,
    3088,   192,   306,  -431,    37,  3985,  4152,  4152,  4134,  4134,
     471,   471,   277,   432,  -431,   -71,   -71,  4152,  4152,  3873,
    4005,  4061,  4081,  4134,  4134,  4157,  4157,   378,   378,   378,
    -431,  3753,  -431,    96,  -431,  -431,  2386,  -431,  2789,  -431,
    3985,  3985,  3985,  3985,  3985,  3985,  3985,  3985,  3985,  3985,
    3985,  3985,  3985,  3985,  3985,  -431,  -431,   109,  2707,   314,
    3527,  -431,  2386,  2386,  -431,  2386,  -431,  2386,  2386,   306,
     315,   285,   306,  2386,   286,  2386,   289,  3985,  -431,  -431,
    3929,  3144,  3144,    -8,  2386,  -431,  -431,  2865,   427,  3985,
    3144,  -431,   287,  -431,  -431,  -431,   407,   206,  -431,   174,
     -53,   121,  2386,  -431,  2156,  2386,  -431,  -431,  3985,  -431,
    -431,    50,   308,  -431,  4134,  4134,  4134,   -78,  3985,   124,
     -53,  -431,   271,  3985,  -431,  -431,  -431,  -431,  -431,    89,
    2386,  -431,  2941,  -431,   174,  -431,  -431,  -431,  -431,  -431,
     292,  3985,  -431,   118,  3985,  -431,  2271,  2386,   295,  2386,
     174,  2386,  -431,   -61,  -431,  -431,   780,  -431,  -431,  2707,
    3593,  3207,  -431,  3985,  -431,  3144,   174,  -431,    52,  -431,
     402,   301,    89,  -431,  -431,  2386,   302,  -431,  3985,  -431
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    55,     1,   138,     0,     0,     0,     0,     0,   139,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,     0,    47,    21,    32,    29,    30,
      25,    22,    28,    23,     0,     0,     0,    50,   120,     0,
      51,    53,     0,    49,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,   266,     0,    13,     0,   122,     0,
       0,    56,   271,     0,     0,     0,     0,    62,    57,     0,
      45,    46,    43,    44,    42,     0,    41,    48,    33,    31,
      24,    27,    26,     0,     0,     0,     0,   277,   297,   278,
     302,   279,   283,   284,   285,   286,   290,   291,   292,   293,
     294,   295,   296,   298,   299,   319,   282,   289,   300,   322,
     325,   280,   287,   281,   288,     0,   301,   304,   306,   305,
     269,   121,     0,    54,     0,   273,   239,    65,    66,    68,
      67,    69,    70,    71,    72,    91,    92,    89,    90,    82,
      93,    94,    83,    80,    81,    95,    96,    97,    98,    85,
      86,    84,    78,    79,    74,    73,    75,    76,    77,    64,
      63,    87,    88,     0,    60,   275,   265,   264,     0,    14,
       0,     0,   270,     0,     0,     0,     0,    57,    57,    57,
       0,   310,     0,   315,   312,     0,    52,   272,   234,     0,
      58,   243,     0,   241,   250,     0,     0,   256,   257,     0,
     255,     0,     0,    15,    16,     0,   267,     0,     0,     0,
       0,    60,     0,    60,     0,    60,     0,   246,     0,   245,
     311,   313,     0,   182,   183,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   152,
     154,   153,   155,   156,   157,    17,     0,     0,     0,     0,
       0,   308,     0,   116,    55,   180,   150,   178,   227,   226,
     225,   230,   179,   229,   228,     0,     0,   181,   329,   330,
     331,   332,     0,   274,   236,     0,   242,    59,     0,   249,
     248,     0,     0,    61,    99,   276,     0,   262,     0,   158,
       0,   316,     0,   318,   303,     0,   320,     0,   323,     0,
     326,   328,     0,   314,   309,   124,   127,     0,     0,     0,
       0,     0,     0,     0,   206,   207,     0,     0,     0,     0,
     348,     0,     0,   187,   186,   214,   185,   184,     0,     0,
      57,   233,     0,     0,     0,   208,   209,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,   237,     0,   240,   244,   251,   254,
     253,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   261,
     268,     0,   321,   324,   327,   247,     0,     0,     0,     0,
       0,     0,     0,   221,     0,     0,   120,     0,   339,   335,
       0,   342,   343,   344,     0,     0,     0,     0,     0,    20,
       0,    18,   210,   118,     0,     0,     0,     0,   128,     0,
       0,     0,   129,     0,   102,   100,   113,   109,   107,   108,
     117,   103,     0,     0,     0,   112,   114,   136,   106,     0,
     158,    60,     0,   215,     0,   148,   188,   189,   201,   202,
     199,   200,     0,   223,   213,   231,   232,   190,   191,     0,
     204,   205,   203,   197,   198,   193,   192,   194,   195,   196,
     212,     0,   217,     0,   235,   238,     0,   260,   158,   119,
     165,   166,   168,   167,   169,   162,   163,   164,   170,   171,
     160,   161,   172,   173,   159,   317,   125,     0,     0,     0,
       0,   219,     0,     0,   220,     0,   222,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   349,   350,
       0,     0,     0,     0,     0,   130,   133,   158,     0,   123,
       0,   115,     0,   105,   110,   111,     0,     0,   104,     0,
       0,     0,     0,   216,     0,     0,   211,   218,   252,   259,
     126,     0,     0,   144,   141,   142,   143,     0,   333,     0,
       0,   337,   336,   340,   346,   345,   347,    19,    39,    34,
       0,   134,   158,   131,     0,    40,   116,   137,   140,   151,
       0,   149,   175,     0,   224,   146,     0,     0,     0,     0,
       0,     0,    37,     0,   132,   135,     0,   174,   176,     0,
       0,   351,   338,   334,    35,     0,     0,   101,     0,   145,
       0,     0,    34,    38,   147,     0,     0,    36,   352,   353
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -431,  -431,  -431,   125,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,   400,  -431,  -431,   411,  -165,  -431,  -431,  -431,  -431,
     416,   -55,  -431,   408,  -431,   215,  -160,  -199,  -431,  -431,
    -113,  -431,  -119,  -196,   -11,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,   469,  -431,  -431,  -431,  -359,   101,  -431,  -324,
    -431,  -431,    28,  -431,  -431,  -431,  -431,   194,  -431,   181,
    -431,  -183,  -176,   195,   -65,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,   -67,   -54,  -431,  -431,   -45,   259,  -430,
     -34,  -431,  -431,  -231,   -39,   173,  -431,  -431,  -431,  -431,
    -431
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   170,    76,   322,   255,    14,    15,    31,
      32,    33,    16,    28,    29,   602,   436,   437,   438,   439,
      25,    26,    40,    41,    42,    17,   164,   196,    68,    18,
     256,   440,   329,   441,   257,   442,   258,   443,   444,   445,
     446,   547,   447,   448,   259,   260,   454,   261,   262,   288,
     263,   264,   455,   274,   365,   275,   188,   191,   192,   217,
     218,   518,   193,   202,   167,    69,    20,    83,    21,    22,
     125,    63,    23,   194,   266,   118,   119,   219,   267,   409,
     410,   268,   411,   412,   413,   414,   269,   321,   270,   621,
     271
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,   117,   168,   483,   122,   449,   287,   320,   517,   580,
     120,   282,   295,   276,   297,    37,   299,   211,   213,   215,
     181,   189,   551,   279,   341,   182,     2,   343,   373,   181,
      38,     3,   165,   220,   182,   189,   331,   507,   301,    46,
      60,   181,   205,   279,   116,   552,   182,    52,   171,    43,
       4,   280,     5,   521,     6,    24,     7,   522,    53,   302,
     197,     8,   552,    27,    39,    61,   198,     9,   332,   166,
     206,   280,   279,    10,   597,   359,   360,   181,    57,   181,
     548,    47,   182,   284,   182,   172,   369,   199,   200,   197,
     201,   616,   572,   203,   204,   198,   181,   165,   600,   199,
     280,   182,   281,   181,    38,    70,    71,   601,   182,    48,
      44,   165,    45,   187,   199,   199,    30,   200,   183,   117,
     117,   117,   184,   117,   117,   117,   117,   183,   207,   208,
     209,   184,   212,   214,   216,   221,   186,   181,    39,   183,
     185,   117,   182,   184,   190,   371,    34,   117,   272,   185,
     283,    35,    36,   222,   292,   273,   286,   277,   567,   278,
     552,   185,   116,   116,   116,    48,   116,   116,   116,   116,
     451,   117,   417,    11,   418,   183,   487,   183,    12,   184,
     305,   184,   117,   117,   116,   486,   528,   553,   181,   485,
     116,   317,   318,   182,   183,   593,    49,   185,   184,   185,
     595,   183,   624,    48,    72,   184,   279,   291,    73,    50,
      74,    75,    54,   265,   116,   181,   185,   363,   396,   552,
     182,   603,    55,   185,   181,   116,   116,   117,   484,   182,
     289,   536,   552,    56,   280,   183,   368,   325,   117,   184,
     253,   552,   254,    51,   520,    64,   557,   391,   117,   405,
     524,   525,   549,   526,   525,   117,   117,   185,   117,   560,
      58,    59,   314,   315,   399,   400,   319,   402,   608,   590,
     116,    62,   598,    57,   323,   324,   522,   326,   327,    78,
     328,   116,   449,   169,   419,   420,   183,    81,    82,   121,
     184,   116,   293,   124,   126,   161,   162,   181,   116,   116,
     163,   116,   182,    65,    66,   169,   173,   174,   185,   175,
     370,   176,   177,   183,   178,   179,   538,   184,   195,   296,
     180,   210,   183,   165,   285,   290,   184,   253,   298,   303,
     294,   304,   309,    67,    11,   185,   398,   364,   581,   401,
     307,   404,   308,   310,   185,   408,   408,   311,   312,   373,
     313,   366,   362,   392,   393,   394,   397,   450,   181,   533,
     390,   456,   457,   182,   462,   458,   459,   460,   461,   464,
     463,   480,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   599,   481,   529,
     541,   181,   181,   203,   520,   183,   182,   182,   523,   184,
     488,   300,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   185,   578,   579,
     542,   543,   546,   550,   181,   510,   554,   585,   544,   182,
     545,   562,   570,   571,   574,   328,   589,   576,   584,   596,
     586,   587,   607,   612,   527,   319,   625,   421,   530,   626,
     629,    80,   531,   532,   117,   537,   183,   627,   539,   540,
     184,    79,   505,   561,    77,   335,   336,   606,   123,   330,
      19,   605,   367,   341,   489,   342,   343,   344,   185,   345,
     168,   372,   588,   395,   306,   569,   575,   614,     0,   183,
     183,   415,     0,   184,   184,   512,   513,   116,     0,     0,
       0,     0,   622,   623,     0,     0,     0,     0,     0,     0,
       0,   185,   185,     0,   558,     0,     0,     0,     0,   335,
     336,     0,   183,     0,   359,   360,   184,   341,   515,     0,
     343,   344,     0,   345,     0,     0,     0,     0,     0,     0,
     564,   565,     0,   566,   185,     0,   568,     0,     0,     0,
       0,   573,     0,   408,     0,   117,   333,   334,   335,   336,
     337,     0,   582,   338,   618,     0,   341,     0,   342,   343,
     344,     0,   345,     0,   346,   347,     0,     0,   359,   360,
     591,     0,     0,   594,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,     0,
       0,   352,   353,   354,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,     0,     0,     0,   610,   611,   423,   613,     0,   615,
       3,     0,   424,   425,   450,   426,     0,   223,   224,   225,
     226,     0,     0,     0,     0,   427,   227,   428,   429,     0,
       0,     0,     0,   628,     0,     0,     0,   430,   228,     0,
       0,   431,   229,     0,   230,     0,     9,   231,   432,     0,
     433,   232,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   233,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,   238,   239,   240,   241,   242,
     243,   244,    38,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,     0,
       0,     0,   248,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,     0,   252,     0,    39,   434,   253,   435,
     254,   423,     0,     0,     0,     3,     0,   424,   425,     0,
     426,     0,   223,   224,   225,   226,     0,     0,     0,     0,
     427,   227,   428,   429,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   228,     0,     0,   431,   229,     0,   230,
       0,     9,   231,   432,     0,   433,   232,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   233,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,   237,
     238,   239,   240,   241,   242,   243,   244,    38,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,   247,     0,     0,     0,   248,     0,     0,
     249,   250,     0,     0,     0,     0,     0,     0,     0,   252,
       0,    39,   434,   253,   617,   254,   223,   224,   225,   226,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,   181,     0,     0,     0,     0,   182,   228,     0,     0,
       0,   229,     0,   230,     0,     0,   231,     0,     0,     0,
     232,     0,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   233,   111,   112,   113,   114,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   406,   245,     0,     0,     0,     0,     0,     0,   183,
       0,     0,     0,   184,     0,     0,   246,   247,     0,     0,
       0,   248,     0,     0,   249,   250,     0,   223,   224,   225,
     226,   185,     0,   407,     0,    39,   227,   253,     0,   254,
       0,     0,   181,     0,     0,     0,     0,   182,   228,     0,
       0,     0,   229,     0,   230,     0,     0,   231,     0,     0,
       0,   232,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   233,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,   238,   239,   240,   241,   242,
     243,   244,    38,   245,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   184,     0,     0,   246,   247,     0,
       0,     0,   248,     0,     0,   249,   250,     0,   223,   224,
     225,   226,   185,     0,   252,     0,    39,   227,   253,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,   228,
       0,     0,     0,   229,     0,   230,     0,     0,   231,     0,
       0,     0,   232,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   233,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    38,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,   225,   226,   508,   246,   247,
       0,     0,   227,   248,     0,     0,   249,   250,     0,     0,
       0,     0,     0,   452,   228,   252,   453,    39,   229,   253,
     230,   254,     0,   231,     0,     0,     0,   232,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   233,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,   238,   239,   240,   241,   242,   243,   244,    38,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   509,     0,   246,   247,     0,     0,     0,   248,     0,
       0,   249,   250,     0,   223,   224,   225,   226,     0,     0,
     252,     0,    39,   227,   253,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,   229,
       0,   230,     0,     0,   231,     0,     0,     0,   232,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   233,   111,   112,   113,   114,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,     0,     0,   534,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   244,    38,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,   248,
       0,     0,   249,   250,     0,     0,     0,   223,   224,   225,
     226,   252,     0,    39,   535,   253,   227,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,     0,
       0,     0,   229,     0,   230,     0,     0,   231,     0,     0,
       0,   232,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   233,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,   238,   239,   240,   241,   242,
     243,   244,    38,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,     0,
       0,     0,   248,     0,     0,   249,   250,     0,   223,   224,
     225,   226,     0,   251,   252,     0,    39,   227,   253,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,   228,
       0,     0,     0,   229,     0,   230,     0,     0,   231,     0,
       0,     0,   232,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   233,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    38,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,   225,   226,     0,   246,   247,
       0,     0,   227,   248,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,   228,   252,   403,    39,   229,   253,
     230,   254,     0,   231,     0,     0,     0,   232,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   233,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,   238,   239,   240,   241,   242,   243,   244,    38,   245,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
     225,   226,     0,   246,   247,     0,     0,   227,   248,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,   228,
     252,   482,    39,   229,   253,   230,   254,     0,   231,     0,
       0,     0,   232,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   233,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    38,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,   225,   226,     0,   246,   247,
       0,     0,   227,   248,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,   228,   252,   506,    39,   229,   253,
     230,   254,     0,   231,     0,     0,     0,   232,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   233,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,   238,   239,   240,   241,   242,   243,   244,    38,   245,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
     225,   226,     0,   246,   247,     0,     0,   227,   248,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,   228,
     252,   519,    39,   229,   253,   230,   254,     0,   231,     0,
       0,     0,   232,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   233,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    38,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   223,   224,   225,   226,   609,   246,   247,
       0,     0,   227,   248,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,   228,   252,   592,    39,   229,   253,
     230,   254,     0,   231,     0,     0,     0,   232,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   233,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,   238,   239,   240,   241,   242,   243,   244,    38,   245,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
     225,   226,     0,   246,   247,     0,     0,   227,   248,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,   228,
     252,     0,    39,   229,   253,   230,   254,     0,   231,     0,
       0,     0,   232,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   233,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    38,   245,     0,   316,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,   246,   247,
       0,    85,     0,   248,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,   252,    86,    39,     0,   253,
       0,   254,     0,     0,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,    38,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,    86,     0,     0,
       0,     0,     0,    38,     0,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,    38,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,   163,     0,    39,     0,
     374,   375,   376,   377,   378,   379,   380,   381,   333,   334,
     335,   336,   337,   382,   383,   338,   339,   340,   341,   384,
     342,   343,   344,     0,   345,   385,   346,   347,   386,   387,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
       0,     0,     0,     0,     0,     0,     0,     0,    39,   359,
     360,     0,     0,     0,     0,   389,   374,   375,   376,   377,
     378,   379,   380,   381,   333,   334,   335,   336,   337,   382,
     383,   338,   339,   340,   341,   384,   342,   343,   344,     0,
     345,   385,   346,   347,   386,   387,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   388,   348,     0,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,   559,   374,   375,   376,   377,   378,   379,   380,   381,
     333,   334,   335,   336,   337,   382,   383,   338,   339,   340,
     341,   384,   342,   343,   344,     0,   345,   385,   346,   347,
     386,   387,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
     348,     0,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,   360,     0,     0,     0,     0,   583,   374,   375,
     376,   377,   378,   379,   380,   381,   333,   334,   335,   336,
     337,   382,   383,   338,   339,   340,   341,   384,   342,   343,
     344,     0,   345,   385,   346,   347,   386,   387,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   388,   348,     0,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,     0,     0,   604,   374,   375,   376,   377,   378,   379,
     380,   381,   333,   334,   335,   336,   337,   382,   383,   338,
     339,   340,   341,   384,   342,   343,   344,  -158,   345,   385,
     346,   347,   386,   387,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   388,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,   360,   374,   375,   376,   377,   378,
     379,   380,   381,   333,   334,   335,   336,   337,   382,   383,
     338,   339,   340,   341,   384,   342,   343,   344,     0,   345,
     385,   346,   347,   386,   387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,   348,     0,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,     0,     0,     0,     0,   333,
     334,   335,   336,   337,   359,   360,   338,   339,   340,   341,
       0,   342,   343,   344,     0,   345,     0,   346,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
       0,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   360,   333,   334,   335,   336,   337,   253,     0,   338,
     339,   340,   341,     0,   342,   343,   344,     0,   345,     0,
     346,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,     0,     0,     0,     0,   333,   334,
     335,   336,   337,   359,   360,   338,   339,   340,   341,   620,
     342,   343,   344,     0,   345,     0,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,     0,     0,   422,   333,   334,   335,   336,   337,     0,
       0,   338,   339,   340,   341,     0,   342,   343,   344,     0,
     345,     0,   346,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,   511,
     333,   334,   335,   336,   337,     0,     0,   338,   339,   340,
     341,     0,   342,   343,   344,     0,   345,     0,   346,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,   360,     0,     0,   514,   333,   334,   335,   336,
     337,     0,     0,   338,   339,   340,   341,     0,   342,   343,
     344,     0,   345,     0,   346,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,   516,   333,   334,   335,   336,   337,     0,     0,   338,
     339,   340,   341,     0,   342,   343,   344,     0,   345,     0,
     346,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,   360,     0,     0,   563,   333,   334,
     335,   336,   337,     0,     0,   338,   339,   340,   341,     0,
     342,   343,   344,     0,   345,     0,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,     0,     0,   619,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,     0,     0,     0,     0,     0,     0,     0,     0,
     145,   146,   147,   148,   333,   334,   335,   336,   337,     0,
       0,   338,   339,   340,   341,     0,   342,   343,   344,     0,
     345,     0,   346,   347,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,     0,     0,     0,   159,   160,     0,
       0,     0,     0,     0,   348,     0,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,   361,   333,   334,
     335,   336,   337,     0,     0,   338,   339,   340,   341,     0,
     342,   343,   344,     0,   345,     0,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,   556,   333,   334,   335,   336,   337,     0,     0,   338,
     339,   340,   341,     0,   342,   343,   344,     0,   345,     0,
     346,   347,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   348,     0,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,     0,     0,     0,     0,   333,   334,
     335,   336,   337,   359,   360,   338,   339,   340,   341,     0,
     342,   343,   344,     0,   345,     0,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,   555,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
       0,     0,     0,     0,   333,   334,   335,   336,   337,   359,
     360,   338,   339,   340,   341,     0,   342,   343,   344,     0,
     345,     0,   346,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   577,     0,     0,   348,     0,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,     0,     0,     0,     0,
     333,   334,   335,   336,   337,   359,   360,   338,   339,   340,
     341,     0,   342,   343,   344,     0,   345,     0,   346,   347,
     333,   334,   335,   336,   337,     0,     0,   338,   339,   340,
     341,     0,   342,   343,   344,     0,   345,     0,   346,   347,
     348,     0,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,   360,   350,   351,   352,   353,   354,   355,   356,
     357,   358,     0,     0,     0,     0,   333,   334,   335,   336,
     337,   359,   360,   338,   339,   340,   341,     0,   342,   343,
     344,     0,   345,     0,   346,   347,   333,   334,   335,   336,
     337,     0,     0,   338,   339,   340,   341,     0,   342,   343,
     344,     0,   345,     0,   346,   347,     0,     0,     0,     0,
     351,   352,   353,   354,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,   352,   353,   354,   355,   356,   357,   358,     0,   333,
     334,   335,   336,     0,     0,     0,     0,   359,   360,   341,
       0,   342,   343,   344,     0,   345,     0,   346,   347,   335,
     336,     0,     0,     0,   335,   336,     0,   341,     0,   342,
     343,   344,   341,   345,   342,   343,   344,     0,   345,     0,
       0,     0,     0,     0,     0,     0,   354,   355,   356,   357,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   360,     0,     0,   354,   355,   356,   357,   358,     0,
       0,   356,   357,   358,     0,     0,     0,     0,   359,   360,
       0,     0,     0,   359,   360
};

static const yytype_int16 yycheck[] =
{
      11,    55,    69,   362,    59,   329,   202,   238,    17,    17,
      55,   194,   211,   189,   213,    28,   215,   177,   178,   179,
      27,    41,   452,    96,    95,    32,     0,    98,   100,    27,
     117,     5,   117,    27,    32,    41,   117,   396,   131,   153,
     123,    27,   124,    96,    55,   123,    32,   135,   117,    60,
      24,   124,    26,   148,    28,   117,    30,   152,   146,   152,
      96,    35,   123,   111,   151,   148,   102,    41,   149,   154,
     152,   124,    96,    47,   152,   146,   147,    27,   151,    27,
     152,   124,    32,   196,    32,   154,   282,   123,   124,    96,
     126,   152,   522,   119,   120,   102,    27,   117,     9,   123,
     124,    32,   126,    27,   117,    12,    13,    18,    32,   123,
       4,   117,     6,   124,   123,   123,   117,   124,   125,   173,
     174,   175,   129,   177,   178,   179,   180,   125,   173,   174,
     175,   129,   177,   178,   179,   129,   150,    27,   151,   125,
     147,   195,    32,   129,   150,   152,   117,   201,   147,   147,
     195,   117,   117,   147,   152,   154,   201,   150,   517,   152,
     123,   147,   173,   174,   175,   123,   177,   178,   179,   180,
     330,   225,   152,   147,   154,   125,   372,   125,   152,   129,
     225,   129,   236,   237,   195,   368,   417,   150,    27,   365,
     201,   236,   237,    32,   125,   554,   126,   147,   129,   147,
     150,   125,   150,   123,   111,   129,    96,   131,   115,   123,
     117,   118,   153,   185,   225,    27,   147,   272,   149,   123,
      32,   580,   124,   147,    27,   236,   237,   281,   148,    32,
     202,   427,   123,   152,   124,   125,   281,   248,   292,   129,
     153,   123,   155,   123,   123,    29,   150,   292,   302,   316,
     148,   152,   451,   154,   152,   309,   310,   147,   312,   150,
     117,   149,   234,   235,   309,   310,   238,   312,   150,   148,
     281,   117,   148,   151,   246,   247,   152,   249,   250,   111,
     252,   292,   606,   119,   120,   121,   125,   117,   117,   117,
     129,   302,   131,   126,   153,    29,    29,    27,   309,   310,
     149,   312,    32,    87,    88,   119,   130,   130,   147,   130,
     282,   149,   130,   125,   130,   130,   429,   129,   126,   131,
     130,   117,   125,   117,   117,   111,   129,   153,   131,   129,
     150,   148,   130,   117,   147,   147,   308,    38,   534,   311,
     149,   313,   149,   130,   147,   317,   318,   149,   130,   100,
     149,   152,   149,   131,   131,   131,   117,   329,    27,   426,
     152,   333,   334,    32,   117,   337,   338,   339,   340,   117,
     342,   117,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   570,   360,   154,
     152,    27,    27,   119,   123,   125,    32,    32,   123,   129,
     372,   131,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   147,   531,   532,
      31,   152,    17,   117,    27,   397,   149,   540,   152,    32,
     152,   117,   117,   148,   148,   407,   549,   148,    11,   131,
     153,    34,   150,   148,   416,   417,    44,   322,   420,   148,
     148,    51,   424,   425,   508,   427,   125,   622,   430,   431,
     129,    50,   131,   508,    48,    87,    88,   586,    60,   254,
       1,   584,   278,    95,   373,    97,    98,    99,   147,   101,
     547,   286,   547,   302,   225,   519,   525,   600,    -1,   125,
     125,   318,    -1,   129,   129,   131,   131,   508,    -1,    -1,
      -1,    -1,   615,   616,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   147,    -1,   486,    -1,    -1,    -1,    -1,    87,
      88,    -1,   125,    -1,   146,   147,   129,    95,   131,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
     512,   513,    -1,   515,   147,    -1,   518,    -1,    -1,    -1,
      -1,   523,    -1,   525,    -1,   609,    85,    86,    87,    88,
      89,    -1,   534,    92,   609,    -1,    95,    -1,    97,    98,
      99,    -1,   101,    -1,   103,   104,    -1,    -1,   146,   147,
     552,    -1,    -1,   555,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   609,    -1,
      -1,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
      -1,    -1,    -1,    -1,   596,   597,     1,   599,    -1,   601,
       5,    -1,     7,     8,   606,    10,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,   625,    -1,    -1,    -1,    32,    33,    -1,
      -1,    36,    37,    -1,    39,    -1,    41,    42,    43,    -1,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,     1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    36,    37,    -1,    39,
      -1,    41,    42,    43,    -1,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    -1,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,   129,    -1,    -1,   132,   133,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,    -1,    12,    13,    14,
      15,   147,    -1,   149,    -1,   151,    21,   153,    -1,   155,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,   129,    -1,    -1,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,   140,   141,    -1,    12,    13,
      14,    15,   147,    -1,   149,    -1,   151,    21,   153,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    16,   132,   133,
      -1,    -1,    21,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,   147,    33,   149,   150,   151,    37,   153,
      39,   155,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,   137,    -1,
      -1,   140,   141,    -1,    12,    13,    14,    15,    -1,    -1,
     149,    -1,   151,    21,   153,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,    -1,   137,
      -1,    -1,   140,   141,    -1,    -1,    -1,    12,    13,    14,
      15,   149,    -1,   151,   152,   153,    21,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,   137,    -1,    -1,   140,   141,    -1,    12,    13,
      14,    15,    -1,   148,   149,    -1,   151,    21,   153,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,   132,   133,
      -1,    -1,    21,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   149,   150,   151,    37,   153,
      39,   155,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    -1,   132,   133,    -1,    -1,    21,   137,    -1,
      -1,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     149,   150,   151,    37,   153,    39,   155,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    -1,   132,   133,
      -1,    -1,    21,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   149,   150,   151,    37,   153,
      39,   155,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    -1,   132,   133,    -1,    -1,    21,   137,    -1,
      -1,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     149,   150,   151,    37,   153,    39,   155,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    16,   132,   133,
      -1,    -1,    21,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   149,   150,   151,    37,   153,
      39,   155,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    -1,   132,   133,    -1,    -1,    21,   137,    -1,
      -1,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     149,    -1,   151,    37,   153,    39,   155,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   132,   133,
      -1,    25,    -1,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    40,   151,    -1,   153,
      -1,   155,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,    40,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,   149,    -1,   151,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   146,
     147,    -1,    -1,    -1,    -1,   152,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,    -1,
      -1,   152,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,    -1,    -1,    -1,    -1,   152,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
      -1,    -1,    -1,   152,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,   104,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,   146,   147,    92,    93,    94,    95,
      -1,    97,    98,    99,    -1,   101,    -1,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,    85,    86,    87,    88,    89,   153,    -1,    92,
      93,    94,    95,    -1,    97,    98,    99,    -1,   101,    -1,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,   146,   147,    92,    93,    94,    95,   152,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,    -1,    -1,   150,    85,    86,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    97,    98,    99,    -1,
     101,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,   150,
      85,    86,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    97,    98,    99,    -1,   101,    -1,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,    -1,    -1,   150,    85,    86,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    97,    98,
      99,    -1,   101,    -1,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
      -1,   150,    85,    86,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    97,    98,    99,    -1,   101,    -1,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,    -1,    -1,   150,    85,    86,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,    -1,    -1,   150,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    85,    86,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    97,    98,    99,    -1,
     101,    -1,   103,   104,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,   140,   141,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,    85,    86,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,    85,    86,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    97,    98,    99,    -1,   101,    -1,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,   146,   147,    92,    93,    94,    95,    -1,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,   146,
     147,    92,    93,    94,    95,    -1,    97,    98,    99,    -1,
     101,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,   146,   147,    92,    93,    94,
      95,    -1,    97,    98,    99,    -1,   101,    -1,   103,   104,
      85,    86,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    97,    98,    99,    -1,   101,    -1,   103,   104,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,   146,   147,    92,    93,    94,    95,    -1,    97,    98,
      99,    -1,   101,    -1,   103,   104,    85,    86,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    97,    98,
      99,    -1,   101,    -1,   103,   104,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,    -1,
      -1,   130,   131,   132,   133,   134,   135,   136,    -1,    85,
      86,    87,    88,    -1,    -1,    -1,    -1,   146,   147,    95,
      -1,    97,    98,    99,    -1,   101,    -1,   103,   104,    87,
      88,    -1,    -1,    -1,    87,    88,    -1,    95,    -1,    97,
      98,    99,    95,   101,    97,    98,    99,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,    -1,   146,   147,
      -1,    -1,    -1,   146,   147
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   157,     0,     5,    24,    26,    28,    30,    35,    41,
      47,   147,   152,   158,   163,   164,   168,   181,   185,   198,
     222,   224,   225,   228,   117,   176,   177,   111,   169,   170,
     117,   165,   166,   167,   117,   117,   117,    28,   117,   151,
     178,   179,   180,   190,     4,     6,   153,   124,   123,   126,
     123,   123,   135,   146,   153,   124,   152,   151,   117,   149,
     123,   148,   117,   227,    29,    87,    88,   117,   184,   221,
      12,    13,   111,   115,   117,   118,   160,   176,   111,   170,
     167,   117,   117,   223,    19,    25,    40,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,   190,   230,   231,   232,
     233,   117,   177,   179,   126,   226,   153,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,   103,   104,   105,   106,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   140,
     141,    29,    29,   149,   182,   117,   154,   220,   229,   119,
     159,   117,   154,   130,   130,   130,   149,   130,   130,   130,
     130,    27,    32,   125,   129,   147,   150,   190,   212,    41,
     150,   213,   214,   218,   229,   126,   183,    96,   102,   123,
     124,   126,   219,   119,   120,   124,   152,   233,   233,   233,
     117,   182,   233,   182,   233,   182,   233,   215,   216,   233,
      27,   129,   147,    12,    13,    14,    15,    21,    33,    37,
      39,    42,    46,    71,    87,    88,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   118,   132,   133,   137,   140,
     141,   148,   149,   153,   155,   162,   186,   190,   192,   200,
     201,   203,   204,   206,   207,   208,   230,   234,   237,   242,
     244,   246,   147,   154,   209,   211,   218,   150,   152,    96,
     124,   126,   217,   233,   186,   117,   233,   189,   205,   208,
     111,   131,   152,   131,   150,   183,   131,   183,   131,   183,
     131,   131,   152,   129,   148,   233,   234,   149,   149,   130,
     130,   149,   130,   149,   208,   208,    10,   233,   233,   208,
     239,   243,   161,   208,   208,   190,   208,   208,   208,   188,
     181,   117,   149,    85,    86,    87,    88,    89,    92,    93,
      94,    95,    97,    98,    99,   101,   103,   104,   125,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   146,
     147,   148,   149,   177,    38,   210,   152,   213,   233,   189,
     208,   152,   219,   100,    77,    78,    79,    80,    81,    82,
      83,    84,    90,    91,    96,   102,   105,   106,   124,   152,
     152,   233,   131,   131,   131,   215,   149,   117,   208,   233,
     233,   208,   233,   150,   208,   229,   117,   149,   208,   235,
     236,   238,   239,   240,   241,   241,   107,   152,   154,   120,
     121,   159,   150,     1,     7,     8,    10,    20,    22,    23,
      32,    36,    43,    45,   152,   154,   172,   173,   174,   175,
     187,   189,   191,   193,   194,   195,   196,   198,   199,   205,
     208,   182,   147,   150,   202,   208,   208,   208,   208,   208,
     208,   208,   117,   208,   117,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     117,   208,   150,   202,   148,   218,   217,   189,   208,   203,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   131,   150,   202,    16,   130,
     208,   150,   131,   131,   150,   131,   150,    17,   217,   150,
     123,   148,   152,   123,   148,   152,   154,   208,   239,   154,
     208,   208,   208,   229,    96,   152,   189,   208,   186,   208,
     208,   152,    31,   152,   152,   152,    17,   197,   152,   183,
     117,   235,   123,   150,   149,   126,   148,   150,   208,   152,
     150,   233,   117,   150,   208,   208,   208,   202,   208,   236,
     117,   148,   235,   208,   148,   240,   148,   122,   186,   186,
      17,   189,   208,   152,    11,   186,   153,    34,   220,   186,
     148,   208,   150,   202,   208,   150,   131,   152,   148,   217,
       9,    18,   171,   202,   152,   186,   188,   150,   150,    16,
     208,   208,   148,   208,   186,   208,   152,   154,   233,   150,
     152,   245,   186,   186,   150,    44,   148,   171,   208,   148
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   156,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   158,   159,   159,   160,   161,   161,   161,
     162,   163,   164,   165,   165,   166,   166,   166,   167,   168,
     169,   169,   170,   170,   171,   171,   171,   172,   173,   174,
     175,   176,   176,   176,   176,   176,   176,   177,   177,   178,
     178,   179,   179,   180,   180,   181,   181,   182,   182,   182,
     183,   183,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   185,
     186,   186,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   188,   188,   188,   189,
     190,   190,   190,   191,   192,   192,   192,   192,   193,   194,
     195,   195,   195,   195,   195,   196,   197,   197,   198,   198,
     199,   200,   200,   200,   201,   201,   201,   201,   202,   202,
     203,   203,   204,   204,   204,   204,   204,   204,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   206,   207,   207,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   209,   209,   210,   210,   211,   212,
     212,   213,   213,   214,   214,   215,   216,   216,   217,   217,
     218,   218,   218,   218,   218,   219,   219,   219,   220,   220,
     220,   220,   220,   221,   221,   222,   223,   223,   223,   224,
     225,   226,   226,   227,   228,   229,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   231,   232,   232,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   240,   240,   241,   241,   242,   242,   243,   243,
     244,   245,   245,   246
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     1,     3,     3,     1,     2,
       1,     3,     1,     3,     0,     2,     4,     4,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       1,     1,     4,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     7,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     2,     0,     2,     2,     3,
       1,     3,     2,     2,     2,     4,     5,     2,     1,     1,
       2,     3,     4,     2,     3,     4,     0,     2,     1,     1,
       3,     5,     5,     5,     5,     8,     6,     9,     1,     3,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     5,     6,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     3,     3,     2,     3,     4,     3,     4,     4,
       4,     3,     4,     3,     5,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     0,     3,     0,     1,     3,     0,
       3,     1,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     3,     5,     3,     3,     1,     1,     1,     4,     6,
       5,     4,     3,     0,     2,     4,     0,     3,     5,     4,
       5,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     4,     3,     4,
       2,     3,     2,     3,     4,     2,     4,     6,     4,     1,
       4,     5,     1,     4,     5,     1,     4,     5,     4,     1,
       1,     1,     1,     3,     5,     1,     3,     5,     7,     1,
       3,     3,     1,     1,     1,     3,     5,     5,     1,     3,
       4,     0,     3,    10
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
          case 117: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2514 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2520 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2526 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2532 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2538 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2544 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2550 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2556 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2562 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2568 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2574 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2580 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2586 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2592 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2598 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2604 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2610 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2616 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2622 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2628 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2634 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2640 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2646 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2652 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2658 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2664 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2670 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2676 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2682 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2688 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2694 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2700 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2706 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2712 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2718 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2724 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2730 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2736 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2742 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2748 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2754 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2760 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2766 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2772 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2778 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2784 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2790 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2796 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2802 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2808 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2814 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2820 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2826 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2832 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2838 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2844 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2850 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2856 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2862 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2868 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2874 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2880 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2886 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2892 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2898 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2904 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2910 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2916 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 245: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2922 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2928 "ds_parser.cpp" /* yacc.c:1254  */
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
        case 13:
#line 363 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 376 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 380 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 384 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3251 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 388 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3265 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 397 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3275 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 405 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3293 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 421 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 451 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 454 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3335 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 460 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 469 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3361 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 491 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 494 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3377 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 500 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3383 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 501 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 502 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 509 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 516 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 538 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 547 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 548 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 551 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 552 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3480 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 556 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 569 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 574 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 604 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 605 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 609 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 610 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 611 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 615 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 619 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 626 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 630 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 631 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 632 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 633 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 635 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 636 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 647 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 648 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 649 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3781 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3799 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 670 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3944 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3986 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4004 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 800 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 806 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4071 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 810 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 816 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 819 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4095 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 822 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4104 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 826 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4112 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 832 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 836 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4124 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 843 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4140 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4158 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 854 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 862 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4176 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4188 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 878 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4253 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 919 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4263 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 927 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4272 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 931 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 936 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 940 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4309 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 951 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4317 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 957 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4380 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 1014 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4386 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4392 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 1018 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4416 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1023 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1024 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1025 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4434 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1026 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1027 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4446 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1028 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4452 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1029 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4458 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1030 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4464 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4470 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1033 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1034 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1035 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1036 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1037 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1038 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1042 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1053 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4534 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1058 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1068 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4552 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1069 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1070 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4570 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4594 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1076 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4600 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1077 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4606 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4612 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1079 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4618 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1080 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4624 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4630 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1082 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4636 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1083 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4642 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4648 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1085 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4654 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4660 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1087 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4666 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1088 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4672 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1089 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4678 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1090 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4684 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4690 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4696 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1093 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1094 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1096 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1098 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4732 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1099 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4744 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1102 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4756 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1103 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4762 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1117 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4808 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4814 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4820 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1122 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1123 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4832 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1125 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1128 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1129 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4858 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1130 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1144 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1154 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5011 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1221 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5017 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5043 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5073 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5093 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1260 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5104 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5116 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1271 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5122 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5130 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5140 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1283 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1288 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5191 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5244 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1368 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5272 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1380 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1399 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5341 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1504 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1511 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1520 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1521 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1525 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1526 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5480 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5486 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5492 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5504 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5510 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5516 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1535 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5528 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1536 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5534 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1537 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5540 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5552 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1540 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1541 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5570 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1543 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1547 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1558 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1562 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1573 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5622 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1574 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1589 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1595 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1604 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1609 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1614 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1619 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1626 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1631 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1636 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1642 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1647 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1651 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1656 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1665 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5802 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1669 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1674 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1683 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1687 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1692 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1701 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1710 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1717 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1724 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1733 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1738 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1745 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1750 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1759 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5981 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1776 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6004 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1790 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1795 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1802 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1818 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1830 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1842 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1843 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1847 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6109 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6113 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1862 "ds_parser.ypp" /* yacc.c:1903  */


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


