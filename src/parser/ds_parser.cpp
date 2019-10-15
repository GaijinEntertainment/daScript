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
    DAS_PUBLIC = 303,
    DAS_TBOOL = 304,
    DAS_TVOID = 305,
    DAS_TSTRING = 306,
    DAS_TAUTO = 307,
    DAS_TINT = 308,
    DAS_TINT2 = 309,
    DAS_TINT3 = 310,
    DAS_TINT4 = 311,
    DAS_TUINT = 312,
    DAS_TUINT2 = 313,
    DAS_TUINT3 = 314,
    DAS_TUINT4 = 315,
    DAS_TFLOAT = 316,
    DAS_TFLOAT2 = 317,
    DAS_TFLOAT3 = 318,
    DAS_TFLOAT4 = 319,
    DAS_TRANGE = 320,
    DAS_TURANGE = 321,
    DAS_TBLOCK = 322,
    DAS_TINT64 = 323,
    DAS_TUINT64 = 324,
    DAS_TDOUBLE = 325,
    DAS_TFUNCTION = 326,
    DAS_TLAMBDA = 327,
    DAS_TINT8 = 328,
    DAS_TUINT8 = 329,
    DAS_TINT16 = 330,
    DAS_TUINT16 = 331,
    DAS_TTUPLE = 332,
    ADDEQU = 333,
    SUBEQU = 334,
    DIVEQU = 335,
    MULEQU = 336,
    MODEQU = 337,
    ANDEQU = 338,
    OREQU = 339,
    XOREQU = 340,
    SHL = 341,
    SHR = 342,
    ADDADD = 343,
    SUBSUB = 344,
    LEEQU = 345,
    SHLEQU = 346,
    SHREQU = 347,
    GREQU = 348,
    EQUEQU = 349,
    NOTEQU = 350,
    RARROW = 351,
    LARROW = 352,
    QQ = 353,
    QDOT = 354,
    LPIPE = 355,
    LBPIPE = 356,
    RPIPE = 357,
    CLONEEQU = 358,
    ROTL = 359,
    ROTR = 360,
    ROTLEQU = 361,
    ROTREQU = 362,
    MAPTO = 363,
    BRABRAB = 364,
    BRACBRB = 365,
    CBRCBRB = 366,
    INTEGER = 367,
    LONG_INTEGER = 368,
    UNSIGNED_INTEGER = 369,
    UNSIGNED_LONG_INTEGER = 370,
    FLOAT = 371,
    DOUBLE = 372,
    NAME = 373,
    BEGIN_STRING = 374,
    STRING_CHARACTER = 375,
    END_STRING = 376,
    BEGIN_STRING_EXPR = 377,
    END_STRING_EXPR = 378,
    UNARY_MINUS = 379,
    UNARY_PLUS = 380,
    PRE_INC = 381,
    PRE_DEC = 382,
    POST_INC = 383,
    POST_DEC = 384,
    DEREF = 385,
    COLCOL = 386
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

#line 358 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   4192

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  158
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  354
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  631

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   386

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   142,     2,     2,   157,   137,   130,     2,
     150,   151,   135,   134,   124,   133,   147,   136,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   127,   154,
     131,   125,   132,   126,   138,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   148,     2,   149,   129,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   155,   128,   156,   141,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   121,   122,   123,   139,
     140,   143,   144,   145,   146,   152,   153
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   350,   350,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   365,   377,   378,   382,   386,   390,   399,
     407,   423,   444,   448,   451,   457,   466,   479,   480,   484,
     488,   489,   493,   496,   502,   503,   504,   511,   518,   530,
     540,   549,   550,   551,   552,   553,   554,   558,   563,   571,
     572,   576,   583,   595,   599,   606,   607,   611,   612,   613,
     617,   621,   628,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   671,
     741,   744,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763,   764,   765,   766,   767,   771,   775,   781,   787,
     801,   802,   808,   812,   818,   821,   824,   828,   834,   838,
     842,   845,   848,   853,   856,   864,   870,   871,   875,   876,
     880,   913,   916,   921,   929,   933,   938,   942,   950,   953,
     959,   964,  1016,  1017,  1018,  1019,  1020,  1021,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,
    1037,  1038,  1039,  1040,  1044,  1055,  1060,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,  1104,  1105,  1106,  1107,  1108,  1112,  1116,  1119,  1122,
    1123,  1124,  1125,  1126,  1127,  1128,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1147,  1157,  1191,  1192,  1196,  1197,  1201,
    1209,  1212,  1219,  1220,  1224,  1225,  1229,  1235,  1236,  1240,
    1241,  1245,  1250,  1253,  1257,  1263,  1272,  1273,  1274,  1278,
    1281,  1286,  1291,  1298,  1308,  1311,  1318,  1347,  1350,  1359,
    1371,  1383,  1397,  1398,  1402,  1441,  1507,  1514,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,
    1535,  1536,  1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,
    1545,  1546,  1550,  1561,  1565,  1575,  1576,  1577,  1578,  1592,
    1598,  1602,  1607,  1612,  1617,  1622,  1629,  1634,  1639,  1645,
    1650,  1654,  1659,  1668,  1672,  1677,  1686,  1690,  1695,  1704,
    1713,  1714,  1715,  1716,  1720,  1727,  1736,  1741,  1748,  1753,
    1762,  1765,  1779,  1788,  1789,  1793,  1798,  1805,  1810,  1821,
    1826,  1833,  1845,  1846,  1850
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
  "\"module\"", "\"public\"", "\"bool\"", "\"void\"", "\"string\"",
  "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"",
  "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"", "\"block\"",
  "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"",
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"<|\"", "\" <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "'.'", "'['", "']'", "'('", "')'", "DEREF", "COLCOL", "';'", "'{'",
  "'}'", "'$'", "$accept", "program", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
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
     375,   376,   377,   378,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,    64,   379,
     380,   126,    33,   381,   382,   383,   384,    46,    91,    93,
      40,    41,   385,   386,    59,   123,   125,    36
};
# endif

#define YYPACT_NINF -435

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-435)))

#define YYTABLE_NINF -159

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -435,    30,  -435,  -435,   -75,   -62,   -21,     3,    20,  -435,
      35,   -16,  -435,  -435,  -435,  -435,  -435,    60,  -435,    32,
    -435,  -435,  -435,  -435,    33,  -435,   -56,    75,    96,  -435,
    -435,    38,  -435,    76,    97,    79,  -435,    82,   128,    92,
    -435,   -71,  -435,   135,    -7,  -435,   307,   -75,   147,   -62,
    -435,   150,   151,  -435,  -435,  2651,  -435,   155,  -435,   -75,
     -16,  -435,   152,   119,  3557,   249,   254,  -435,   139,   -97,
    -435,  -435,  -435,  -435,  -435,   165,  -435,  -435,  -435,  -435,
    -435,  -435,   -95,   163,   164,   166,  -435,  -435,  -435,   157,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,   169,  -435,  -435,  -435,   186,   187,
    -435,  -435,  -435,  -435,   201,  -435,  -435,  -435,  -435,   124,
    -435,   -79,  -435,   -90,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,   -15,   200,  -435,  -435,  -435,   218,  -435,   141,
     -92,  -435,  2651,  2651,  2651,   215,  2581,  2581,  2581,  2651,
    -435,     4,  -435,  -435,  1537,  -435,  -435,    73,   216,  -435,
    -435,   -66,  -435,   -10,  2651,   183,  -435,  -435,   221,  -435,
    2651,  2328,  -435,  -435,   228,  -435,    67,     0,    78,   195,
     200,   196,   200,   225,   200,   239,  -435,  -103,   124,   217,
    -435,   203,  -435,  -435,  2509,   199,  -435,   204,   219,   232,
     206,   233,   229,  2328,  2328,  2439,  2651,  2328,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  2328,  2328,  2328,   -90,  2328,
    2328,  -435,  2328,  -435,   211,  -435,  -435,   -93,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  3579,   231,  -435,  -435,  -435,
    -435,  -435,   -75,  -435,   328,   230,  -435,  -435,   -26,  -435,
    -435,  2651,  2328,   124,  -435,  -435,    47,  -435,   267,  2658,
     237,  -435,  2651,  -435,  -435,   243,  -435,   253,  -435,   260,
    -435,  -435,  2651,  -435,  -435,    68,  -435,   268,  2328,  2651,
    2651,  2328,  2651,  1668,   429,   429,   216,   945,  1076,  3707,
    -435,   -47,   209,   429,   429,  -435,  -435,   429,   429,   502,
     657,   139,  -435,  1186,  2328,  2328,  -435,  -435,  2328,  2328,
    2328,  2328,   283,  2328,   285,  2328,  2328,  2328,  2328,  2328,
    2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,
     303,  2328,  -435,  1778,   -48,  -435,   216,  -435,  -435,   115,
    -435,  2966,  -435,  2328,   -51,  2328,  2328,  2328,  2328,  2328,
    2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,  2328,
    -435,  -435,   248,  -435,  -435,  -435,  -435,  1888,  1296,  3219,
     326,   356,  3285,   363,  -435,  3351,    17,   -73,  1998,  3707,
     298,    10,   300,  -435,  -435,    51,    55,  2328,  2328,   271,
    -435,  2328,   308,  -435,  -435,  2328,  2328,   216,  1427,  -435,
     183,  2328,  2328,  -435,   275,  -435,   399,  -435,  -435,  -435,
    -435,  -435,  -435,   277,   279,   295,  -435,  -435,   436,  -435,
     -81,  3037,   200,   336,  -435,   -55,  3875,   373,   373,  4044,
    4044,  4024,  4024,   310,   429,  -435,   -57,   -57,   373,   373,
    3763,  3895,  3951,  3971,  4044,  4044,   188,   188,   214,   214,
     214,  -435,  3643,  -435,    -8,  -435,  -435,  2328,  -435,  2735,
    -435,  3875,  3875,  3875,  3875,  3875,  3875,  3875,  3875,  3875,
    3875,  3875,  3875,  3875,  3875,  3875,  -435,  -435,    52,  2651,
     345,  3417,  -435,  2328,  2328,  -435,  2328,  -435,  2328,  2328,
     336,   347,   317,   336,  2328,   321,  2328,   329,  3875,  -435,
    -435,  3819,  3093,  3093,    21,  2328,  -435,  -435,  2812,   468,
    3875,  3093,  -435,   337,  -435,  -435,  -435,   449,   216,  -435,
     183,   -50,   -41,  2328,  -435,  2108,  2328,  -435,  -435,  3875,
    -435,  -435,     9,   362,  -435,  4044,  4044,  4044,   -87,  3875,
      90,   -50,  -435,   298,  3875,  -435,  -435,  -435,  -435,  -435,
      37,  2328,  -435,  2889,  -435,   183,  -435,  -435,  -435,  -435,
    -435,   346,  3875,  -435,    83,  3875,  -435,  2218,  2328,   350,
    2328,   183,  2328,  -435,   -84,  -435,  -435,   814,  -435,  -435,
    2651,  3483,  3156,  -435,  3875,  -435,  3093,   183,  -435,    66,
    -435,   452,   351,    37,  -435,  -435,  2328,   352,  -435,  3875,
    -435
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    55,     1,   138,     0,     0,     0,     0,     0,   139,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,     0,    47,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    50,   120,     0,    51,
      53,     0,    49,     0,     0,   264,     0,     0,     0,     0,
      28,     0,     0,    26,   267,     0,    13,     0,   122,     0,
       0,    56,   272,     0,     0,     0,     0,    62,    57,     0,
      45,    46,    43,    44,    42,     0,    41,    48,    33,    31,
      25,    24,     0,     0,     0,     0,   278,   298,   279,   303,
     280,   284,   285,   286,   287,   291,   292,   293,   294,   295,
     296,   297,   299,   300,   320,   283,   290,   301,   323,   326,
     281,   288,   282,   289,     0,   302,   305,   307,   306,   270,
     121,     0,    54,     0,   274,   240,    65,    66,    68,    67,
      69,    70,    71,    72,    91,    92,    89,    90,    82,    93,
      94,    83,    80,    81,    95,    96,    97,    98,    85,    86,
      84,    78,    79,    74,    73,    75,    76,    77,    64,    63,
      87,    88,     0,    60,   276,   266,   265,     0,    14,     0,
       0,   271,     0,     0,     0,     0,    57,    57,    57,     0,
     311,     0,   316,   313,     0,    52,   273,   235,     0,    58,
     244,     0,   242,   251,     0,     0,   257,   258,     0,   256,
       0,     0,    15,    16,     0,   268,     0,     0,     0,     0,
      60,     0,    60,     0,    60,     0,   247,     0,   246,   312,
     314,     0,   182,   183,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,   154,
     153,   155,   156,   157,    17,     0,     0,     0,     0,     0,
       0,   309,     0,   116,    55,   180,   150,   178,   228,   227,
     226,   231,   179,   230,   229,     0,     0,   181,   330,   331,
     332,   333,     0,   275,   237,     0,   243,    59,     0,   250,
     249,     0,     0,    61,    99,   277,     0,   263,     0,   158,
       0,   317,     0,   319,   304,     0,   321,     0,   324,     0,
     327,   329,     0,   315,   310,   124,   127,     0,     0,     0,
       0,     0,     0,     0,   206,   207,     0,     0,     0,     0,
     349,     0,     0,   187,   186,   219,   214,   185,   184,     0,
       0,    57,   234,     0,     0,     0,   208,   209,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,     0,     0,   238,     0,   241,   245,   252,
     255,   254,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   269,     0,   322,   325,   328,   248,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,   120,     0,   340,
     336,     0,   343,   344,   345,     0,     0,     0,     0,     0,
      20,     0,    18,   210,   118,     0,     0,     0,     0,   128,
       0,     0,     0,   129,     0,   102,   100,   113,   109,   107,
     108,   117,   103,     0,     0,     0,   112,   114,   136,   106,
       0,   158,    60,     0,   215,     0,   148,   188,   189,   201,
     202,   199,   200,     0,   224,   213,   232,   233,   190,   191,
       0,   204,   205,   203,   197,   198,   193,   192,   194,   195,
     196,   212,     0,   217,     0,   236,   239,     0,   261,   158,
     119,   165,   166,   168,   167,   169,   162,   163,   164,   170,
     171,   160,   161,   172,   173,   159,   318,   125,     0,     0,
       0,     0,   220,     0,     0,   221,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,   350,
     351,     0,     0,     0,     0,     0,   130,   133,   158,     0,
     123,     0,   115,     0,   105,   110,   111,     0,     0,   104,
       0,     0,     0,     0,   216,     0,     0,   211,   218,   253,
     260,   126,     0,     0,   144,   141,   142,   143,     0,   334,
       0,     0,   338,   337,   341,   347,   346,   348,    19,    39,
      34,     0,   134,   158,   131,     0,    40,   116,   137,   140,
     151,     0,   149,   175,     0,   225,   146,     0,     0,     0,
       0,     0,     0,    37,     0,   132,   135,     0,   174,   176,
       0,     0,   352,   339,   335,    35,     0,     0,   101,     0,
     145,     0,     0,    34,    38,   147,     0,     0,    36,   353,
     354
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -435,  -435,  -435,   180,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,   456,  -111,  -435,  -435,  -435,  -435,
     466,   -53,  -435,   454,  -435,   261,  -160,  -201,  -435,  -435,
     -82,  -435,   -65,  -198,   -11,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,   515,  -435,  -435,  -435,  -349,   149,  -435,  -325,
    -435,  -435,    59,  -435,  -435,  -435,  -435,   246,  -435,   224,
    -435,  -189,  -180,   241,   -18,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,   -67,   -54,  -435,  -435,   -45,   309,  -434,
      12,  -435,  -435,  -230,    11,   220,  -435,  -435,  -435,  -435,
    -435
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,    76,   322,   255,    14,    15,    31,
      32,    53,    16,    28,    29,   603,   437,   438,   439,   440,
      25,    26,    39,    40,    41,    17,   163,   195,    68,    18,
     256,   441,   330,   442,   257,   443,   258,   444,   445,   446,
     447,   548,   448,   449,   259,   260,   455,   261,   262,   288,
     263,   264,   456,   274,   366,   275,   187,   190,   191,   216,
     217,   519,   192,   201,   166,    69,    20,    82,    21,    22,
     124,    63,    23,   193,   266,   117,   118,   218,   267,   410,
     411,   268,   412,   413,   414,   415,   269,   321,   270,   622,
     271
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   116,   167,   287,   282,   450,   121,   320,   276,   295,
     119,   297,    36,   299,   484,   188,   210,   212,   214,   552,
     374,   164,    64,   170,   279,   332,   188,   180,    37,   301,
       2,   219,   181,   204,   518,     3,   180,   553,   581,   342,
     553,   181,   344,    24,   115,    47,   601,   279,   508,    42,
      27,   302,   280,    60,     4,   602,     5,   333,     6,   165,
       7,   171,   205,    38,    43,     8,    44,   598,    47,   553,
     617,     9,   185,   549,   180,   280,    47,    10,    61,   181,
      57,    65,    66,   521,   370,   277,    50,   279,   278,   573,
     360,   361,   164,   180,   180,   180,   554,    30,   181,   181,
     181,   485,    37,   164,   253,   180,   254,   418,   591,   419,
     181,    67,   186,   284,   198,   280,   553,   281,   116,   116,
     116,    33,   116,   116,   116,   116,   182,   206,   207,   208,
     183,   211,   213,   215,   220,   182,   189,    38,    34,   183,
     116,   198,   180,   558,   196,   198,   116,   181,   184,   283,
     197,   180,   221,    35,   292,   286,   181,   184,    46,   522,
     596,   115,   115,   115,   523,   115,   115,   115,   115,   568,
     116,   452,   199,   182,    51,   488,   553,   183,    11,   305,
     487,   116,   116,   115,    12,    52,   486,    45,   529,   115,
     317,   318,   182,   182,   182,   184,   183,   183,   183,   291,
     525,   372,    48,   561,   182,   526,   594,   553,   183,   526,
     293,   527,   279,   115,   184,   184,   184,   625,   397,   364,
      49,   272,    55,   180,   115,   115,   184,   116,   181,   273,
     537,    54,   604,    56,   609,    57,   369,   326,   116,   599,
     280,   182,    59,   265,   523,   183,    58,   392,   116,   406,
     182,   550,   180,    62,   183,   116,   116,   181,   116,    78,
     289,   202,   203,   184,   400,   401,   180,   403,    80,    81,
     115,   181,   184,   120,   125,   180,   336,   337,   160,   123,
     181,   115,   450,   161,   342,   168,   343,   344,   345,   162,
     346,   115,   314,   315,   172,   173,   319,   174,   115,   115,
     176,   115,   336,   337,   323,   324,   325,   175,   327,   328,
     342,   329,   343,   344,   345,   196,   346,   177,   178,    70,
      71,   197,   182,   357,   358,   359,   183,   194,   296,   168,
     420,   421,   179,   209,   164,   360,   361,   582,   253,   285,
     290,   371,   198,   199,   184,   200,   294,   303,   539,   307,
     309,   182,   304,   180,   308,   183,   311,   298,   181,    11,
     534,   360,   361,   310,   312,   182,   365,   399,   374,   183,
     402,   300,   405,   184,   182,   393,   409,   409,   183,   313,
     506,   363,   600,   180,   367,   394,   398,   184,   181,   451,
     180,   391,   395,   457,   458,   181,   184,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,    72,
     482,   481,   521,    73,   524,    74,    75,   530,   202,   542,
     543,   544,   489,   545,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   546,
     579,   580,   182,   547,   551,   116,   183,   511,   513,   586,
     555,   336,   337,   563,   562,   571,   572,   329,   590,   342,
     575,   343,   344,   345,   184,   346,   528,   319,   577,   585,
     531,   167,   182,   588,   532,   533,   183,   538,   514,   182,
     540,   541,   587,   183,   597,   516,   626,   608,   115,   613,
     627,   630,   422,   606,   184,    79,   355,   356,   357,   358,
     359,   184,   628,    77,   122,   331,    19,   336,   337,   615,
     360,   361,   607,   490,   368,   342,   396,   373,   344,   345,
     589,   346,   570,   306,   623,   624,     0,   576,   416,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,     0,     0,     0,
       0,     0,     0,     0,     0,   619,     0,     0,     0,     0,
       0,     0,   565,   566,     0,   567,   360,   361,   569,     0,
       0,     0,     0,   574,     0,   409,     0,     0,   334,   335,
     336,   337,   338,     0,   583,   339,   340,   341,   342,   115,
     343,   344,   345,     0,   346,     0,   347,   348,     0,     0,
       0,     0,   592,     0,     0,   595,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   360,
     361,     0,     0,   423,     0,     0,   611,   612,   424,   614,
       0,   616,     3,     0,   425,   426,   451,   427,     0,   222,
     223,   224,   225,     0,     0,     0,     0,   428,   226,   429,
     430,     0,     0,     0,     0,   629,     0,     0,     0,   431,
     227,     0,     0,   432,   228,     0,   229,     0,     9,   230,
     433,     0,   434,   231,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   232,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,   236,   237,   238,
     239,   240,   241,   242,   243,    37,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,     0,     0,   248,     0,     0,   249,   250,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
      38,   435,   253,   436,   254,   424,     0,     0,     0,     3,
       0,   425,   426,     0,   427,     0,   222,   223,   224,   225,
       0,     0,     0,     0,   428,   226,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   227,     0,     0,
     432,   228,     0,   229,     0,     9,   230,   433,     0,   434,
     231,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   232,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   233,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,    37,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,   246,   247,
       0,     0,   248,     0,     0,   249,   250,   222,   223,   224,
     225,     0,     0,     0,   252,     0,   226,    38,   435,   253,
     618,   254,   180,     0,     0,     0,     0,   181,   227,     0,
       0,     0,   228,     0,   229,     0,     0,   230,     0,     0,
       0,   231,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   232,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   407,   244,     0,     0,     0,     0,     0,
       0,   182,     0,     0,     0,   183,     0,     0,   245,   246,
     247,     0,     0,   248,     0,     0,   249,   250,   222,   223,
     224,   225,     0,   184,     0,   408,     0,   226,    38,     0,
     253,     0,   254,   180,     0,     0,     0,     0,   181,   227,
       0,     0,     0,   228,     0,   229,     0,     0,   230,     0,
       0,     0,   231,     0,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   232,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,    37,   244,     0,     0,   222,   223,
     224,   225,   182,     0,     0,     0,   183,   226,     0,   245,
     246,   247,     0,     0,   248,     0,     0,   249,   250,   227,
       0,     0,     0,   228,   184,   229,   252,     0,   230,    38,
       0,   253,   231,   254,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   232,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,    37,   244,     0,     0,   222,   223,
     224,   225,   509,     0,     0,     0,     0,   226,     0,   245,
     246,   247,     0,     0,   248,     0,     0,   249,   250,   227,
       0,     0,     0,   228,   453,   229,   252,   454,   230,    38,
       0,   253,   231,   254,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   232,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,    37,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   510,     0,   245,
     246,   247,     0,     0,   248,     0,     0,   249,   250,   222,
     223,   224,   225,     0,     0,     0,   252,     0,   226,    38,
       0,   253,     0,   254,     0,     0,     0,     0,     0,     0,
     227,     0,     0,     0,   228,     0,   229,     0,     0,   230,
       0,     0,     0,   231,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   232,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
       0,     0,     0,     0,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,   236,   237,   238,
     239,   240,   241,   242,   243,    37,   244,     0,     0,   222,
     223,   224,   225,     0,     0,     0,     0,     0,   226,     0,
     245,   246,   247,     0,     0,   248,     0,     0,   249,   250,
     227,     0,     0,     0,   228,     0,   229,   252,     0,   230,
      38,   536,   253,   231,   254,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   232,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,   236,   237,   238,
     239,   240,   241,   242,   243,    37,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,     0,     0,   248,     0,     0,   249,   250,
     222,   223,   224,   225,     0,     0,   251,   252,     0,   226,
      38,     0,   253,     0,   254,     0,     0,     0,     0,     0,
       0,   227,     0,     0,     0,   228,     0,   229,     0,     0,
     230,     0,     0,     0,   231,     0,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,     0,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,   404,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,     0,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,   483,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,     0,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,   507,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,     0,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,   520,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,   610,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,   593,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,     0,
     222,   223,   224,   225,     0,     0,     0,     0,     0,   226,
       0,   245,   246,   247,     0,     0,   248,     0,     0,   249,
     250,   227,     0,     0,     0,   228,     0,   229,   252,     0,
     230,    38,     0,   253,   231,   254,     0,    86,    87,    88,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,   105,   106,   107,     0,
     232,   110,   111,   112,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,    37,   244,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
       0,   245,   246,   247,    84,     0,   248,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,   252,    85,
       0,    38,     0,   253,     0,   254,     0,     0,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    37,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   235,   236,
     237,    85,     0,     0,     0,     0,     0,    37,     0,     0,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    37,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,   162,     0,     0,    38,     0,   375,   376,   377,   378,
     379,   380,   381,   382,   334,   335,   336,   337,   338,   383,
     384,   339,   340,   341,   342,   385,   343,   344,   345,     0,
     346,   386,   347,   348,   387,   388,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   389,   349,     0,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,     0,     0,     0,     0,
       0,     0,     0,     0,    38,   360,   361,     0,     0,     0,
       0,     0,   390,   375,   376,   377,   378,   379,   380,   381,
     382,   334,   335,   336,   337,   338,   383,   384,   339,   340,
     341,   342,   385,   343,   344,   345,     0,   346,   386,   347,
     348,   387,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     389,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,   361,     0,     0,     0,     0,     0,   560,
     375,   376,   377,   378,   379,   380,   381,   382,   334,   335,
     336,   337,   338,   383,   384,   339,   340,   341,   342,   385,
     343,   344,   345,     0,   346,   386,   347,   348,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   389,   349,     0,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   360,
     361,     0,     0,     0,     0,     0,   584,   375,   376,   377,
     378,   379,   380,   381,   382,   334,   335,   336,   337,   338,
     383,   384,   339,   340,   341,   342,   385,   343,   344,   345,
       0,   346,   386,   347,   348,   387,   388,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   389,   349,     0,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   360,   361,     0,     0,
       0,     0,     0,   605,   375,   376,   377,   378,   379,   380,
     381,   382,   334,   335,   336,   337,   338,   383,   384,   339,
     340,   341,   342,   385,   343,   344,   345,  -158,   346,   386,
     347,   348,   387,   388,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,   349,     0,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   360,   361,   375,   376,   377,   378,   379,
     380,   381,   382,   334,   335,   336,   337,   338,   383,   384,
     339,   340,   341,   342,   385,   343,   344,   345,     0,   346,
     386,   347,   348,   387,   388,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,   349,     0,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,     0,     0,     0,     0,   334,
     335,   336,   337,   338,   360,   361,   339,   340,   341,   342,
       0,   343,   344,   345,     0,   346,     0,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     360,   361,   334,   335,   336,   337,   338,     0,   253,   339,
     340,   341,   342,     0,   343,   344,   345,     0,   346,     0,
     347,   348,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,     0,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   360,   361,   334,   335,   336,   337,   338,
     621,     0,   339,   340,   341,   342,     0,   343,   344,   345,
       0,   346,     0,   347,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   349,     0,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   360,   361,     0,     0,
     512,   334,   335,   336,   337,   338,     0,     0,   339,   340,
     341,   342,     0,   343,   344,   345,     0,   346,     0,   347,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,   361,     0,     0,   515,   334,   335,   336,
     337,   338,     0,     0,   339,   340,   341,   342,     0,   343,
     344,   345,     0,   346,     0,   347,   348,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   360,   361,
       0,     0,   517,   334,   335,   336,   337,   338,     0,     0,
     339,   340,   341,   342,     0,   343,   344,   345,     0,   346,
       0,   347,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   349,     0,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   360,   361,     0,     0,   564,   334,
     335,   336,   337,   338,     0,     0,   339,   340,   341,   342,
       0,   343,   344,   345,     0,   346,     0,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     360,   361,     0,     0,   620,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,     0,     0,     0,     0,     0,     0,     0,
       0,   144,   145,   146,   147,   334,   335,   336,   337,   338,
       0,     0,   339,   340,   341,   342,     0,   343,   344,   345,
       0,   346,     0,   347,   348,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,     0,     0,     0,   158,   159,
       0,     0,     0,     0,     0,   349,     0,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   360,   361,   362,   334,
     335,   336,   337,   338,     0,     0,   339,   340,   341,   342,
       0,   343,   344,   345,     0,   346,     0,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     360,   361,   557,   334,   335,   336,   337,   338,     0,     0,
     339,   340,   341,   342,     0,   343,   344,   345,     0,   346,
       0,   347,   348,     0,     0,   417,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   349,     0,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,     0,     0,     0,     0,   334,
     335,   336,   337,   338,   360,   361,   339,   340,   341,   342,
       0,   343,   344,   345,     0,   346,     0,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
     556,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,     0,     0,     0,     0,   334,   335,   336,   337,   338,
     360,   361,   339,   340,   341,   342,     0,   343,   344,   345,
       0,   346,     0,   347,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   578,     0,     0,   349,     0,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,     0,     0,     0,
       0,   334,   335,   336,   337,   338,   360,   361,   339,   340,
     341,   342,     0,   343,   344,   345,     0,   346,     0,   347,
     348,   334,   335,   336,   337,   338,     0,     0,   339,   340,
     341,   342,     0,   343,   344,   345,     0,   346,     0,   347,
     348,   349,     0,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,   361,   351,   352,   353,   354,   355,   356,
     357,   358,   359,     0,     0,     0,     0,   334,   335,   336,
     337,   338,   360,   361,   339,   340,   341,   342,     0,   343,
     344,   345,     0,   346,     0,   347,   348,   334,   335,   336,
     337,   338,     0,     0,   339,   340,   341,   342,     0,   343,
     344,   345,     0,   346,     0,   347,   348,     0,     0,     0,
       0,   352,   353,   354,   355,   356,   357,   358,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   360,   361,
       0,     0,   353,   354,   355,   356,   357,   358,   359,     0,
     334,   335,   336,   337,   338,     0,     0,   339,   360,   361,
     342,     0,   343,   344,   345,     0,   346,     0,   347,   348,
     334,   335,   336,   337,     0,     0,     0,     0,     0,     0,
     342,     0,   343,   344,   345,     0,   346,     0,   347,   348,
       0,     0,     0,     0,     0,   353,   354,   355,   356,   357,
     358,   359,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   360,   361,     0,     0,     0,     0,   355,   356,   357,
     358,   359,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   360,   361
};

static const yytype_int16 yycheck[] =
{
      11,    55,    69,   201,   193,   330,    59,   237,   188,   210,
      55,   212,    28,   214,   363,    41,   176,   177,   178,   453,
     101,   118,    29,   118,    97,   118,    41,    27,   118,   132,
       0,    27,    32,   125,    17,     5,    27,   124,    17,    96,
     124,    32,    99,   118,    55,   124,     9,    97,   397,    60,
     112,   154,   125,   124,    24,    18,    26,   150,    28,   156,
      30,   156,   154,   153,     4,    35,     6,   154,   124,   124,
     154,    41,   151,   154,    27,   125,   124,    47,   149,    32,
     153,    88,    89,   124,   282,   151,    48,    97,   154,   523,
     147,   148,   118,    27,    27,    27,   151,   118,    32,    32,
      32,   149,   118,   118,   155,    27,   157,   154,   149,   156,
      32,   118,   123,   195,   124,   125,   124,   127,   172,   173,
     174,   118,   176,   177,   178,   179,   126,   172,   173,   174,
     130,   176,   177,   178,   130,   126,   151,   153,   118,   130,
     194,   124,    27,   151,    97,   124,   200,    32,   148,   194,
     103,    27,   148,   118,   154,   200,    32,   148,   125,   149,
     151,   172,   173,   174,   154,   176,   177,   178,   179,   518,
     224,   331,   125,   126,   136,   373,   124,   130,   148,   224,
     369,   235,   236,   194,   154,   147,   366,   155,   418,   200,
     235,   236,   126,   126,   126,   148,   130,   130,   130,   132,
     149,   154,   127,   151,   126,   154,   555,   124,   130,   154,
     132,   156,    97,   224,   148,   148,   148,   151,   150,   272,
     124,   148,   125,    27,   235,   236,   148,   281,    32,   156,
     428,   155,   581,   154,   151,   153,   281,   248,   292,   149,
     125,   126,   150,   184,   154,   130,   118,   292,   302,   316,
     126,   452,    27,   118,   130,   309,   310,    32,   312,   112,
     201,   120,   121,   148,   309,   310,    27,   312,   118,   118,
     281,    32,   148,   118,   155,    27,    88,    89,    29,   127,
      32,   292,   607,    29,    96,   120,    98,    99,   100,   150,
     102,   302,   233,   234,   131,   131,   237,   131,   309,   310,
     131,   312,    88,    89,   245,   246,   247,   150,   249,   250,
      96,   252,    98,    99,   100,    97,   102,   131,   131,    12,
      13,   103,   126,   135,   136,   137,   130,   127,   132,   120,
     121,   122,   131,   118,   118,   147,   148,   535,   155,   118,
     112,   282,   124,   125,   148,   127,   151,   130,   430,   150,
     131,   126,   149,    27,   150,   130,   150,   132,    32,   148,
     427,   147,   148,   131,   131,   126,    38,   308,   101,   130,
     311,   132,   313,   148,   126,   132,   317,   318,   130,   150,
     132,   150,   571,    27,   154,   132,   118,   148,    32,   330,
      27,   154,   132,   334,   335,    32,   148,   338,   339,   340,
     341,   118,   343,   118,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   112,
     361,   118,   124,   116,   124,   118,   119,   156,   120,   154,
      31,   154,   373,   154,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   154,
     532,   533,   126,    17,   118,   509,   130,   398,   132,   541,
     150,    88,    89,   118,   509,   118,   149,   408,   550,    96,
     149,    98,    99,   100,   148,   102,   417,   418,   149,    11,
     421,   548,   126,    34,   425,   426,   130,   428,   132,   126,
     431,   432,   155,   130,   132,   132,    44,   151,   509,   149,
     149,   149,   322,   585,   148,    49,   133,   134,   135,   136,
     137,   148,   623,    47,    60,   254,     1,    88,    89,   601,
     147,   148,   587,   374,   278,    96,   302,   286,    99,   100,
     548,   102,   520,   224,   616,   617,    -1,   526,   318,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   487,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   610,    -1,    -1,    -1,    -1,
      -1,    -1,   513,   514,    -1,   516,   147,   148,   519,    -1,
      -1,    -1,    -1,   524,    -1,   526,    -1,    -1,    86,    87,
      88,    89,    90,    -1,   535,    93,    94,    95,    96,   610,
      98,    99,   100,    -1,   102,    -1,   104,   105,    -1,    -1,
      -1,    -1,   553,    -1,    -1,   556,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,    -1,    -1,   151,    -1,    -1,   597,   598,     1,   600,
      -1,   602,     5,    -1,     7,     8,   607,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,   626,    -1,    -1,    -1,    32,
      33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
     153,   154,   155,   156,   157,     1,    -1,    -1,    -1,     5,
      -1,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      36,    37,    -1,    39,    -1,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,   138,    -1,    -1,   141,   142,    12,    13,    14,
      15,    -1,    -1,    -1,   150,    -1,    21,   153,   154,   155,
     156,   157,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,    -1,   133,   134,
     135,    -1,    -1,   138,    -1,    -1,   141,   142,    12,    13,
      14,    15,    -1,   148,    -1,   150,    -1,    21,   153,    -1,
     155,    -1,   157,    27,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    12,    13,
      14,    15,   126,    -1,    -1,    -1,   130,    21,    -1,   133,
     134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,    33,
      -1,    -1,    -1,    37,   148,    39,   150,    -1,    42,   153,
      -1,   155,    46,   157,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,   133,
     134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,    33,
      -1,    -1,    -1,    37,   148,    39,   150,   151,    42,   153,
      -1,   155,    46,   157,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,    12,
      13,    14,    15,    -1,    -1,    -1,   150,    -1,    21,   153,
      -1,   155,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
     133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,
      33,    -1,    -1,    -1,    37,    -1,    39,   150,    -1,    42,
     153,   154,   155,    46,   157,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,   142,
      12,    13,    14,    15,    -1,    -1,   149,   150,    -1,    21,
     153,    -1,   155,    -1,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,   151,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,   151,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,   151,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,   151,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,   151,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,   133,   134,   135,    -1,    -1,   138,    -1,    -1,   141,
     142,    33,    -1,    -1,    -1,    37,    -1,    39,   150,    -1,
      42,   153,    -1,   155,    46,   157,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,   133,   134,   135,    25,    -1,   138,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    40,
      -1,   153,    -1,   155,    -1,   157,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,    40,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,   150,    -1,    -1,   153,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,   107,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   147,   148,    -1,    -1,    -1,
      -1,    -1,   154,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,   154,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,    -1,    -1,    -1,    -1,    -1,   154,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,    -1,    -1,
      -1,    -1,    -1,   154,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,   147,   148,    93,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,    86,    87,    88,    89,    90,    -1,   155,    93,
      94,    95,    96,    -1,    98,    99,   100,    -1,   102,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,    86,    87,    88,    89,    90,
     154,    -1,    93,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,    -1,    -1,
     151,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,    -1,    -1,   151,    86,    87,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
      -1,    -1,   151,    86,    87,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,    -1,    -1,   151,    86,
      87,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,    -1,    -1,   151,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    86,    87,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,    -1,   104,   105,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,   126,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,    86,
      87,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    86,    87,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
      -1,   104,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,   147,   148,    93,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
     147,   148,    93,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,   126,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,   147,   148,    93,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,    -1,   104,
     105,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,    -1,   104,
     105,   126,    -1,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,   147,   148,    93,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,    -1,   104,   105,    86,    87,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,    -1,   104,   105,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
      -1,    -1,   131,   132,   133,   134,   135,   136,   137,    -1,
      86,    87,    88,    89,    90,    -1,    -1,    93,   147,   148,
      96,    -1,    98,    99,   100,    -1,   102,    -1,   104,   105,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   159,     0,     5,    24,    26,    28,    30,    35,    41,
      47,   148,   154,   160,   165,   166,   170,   183,   187,   200,
     224,   226,   227,   230,   118,   178,   179,   112,   171,   172,
     118,   167,   168,   118,   118,   118,    28,   118,   153,   180,
     181,   182,   192,     4,     6,   155,   125,   124,   127,   124,
      48,   136,   147,   169,   155,   125,   154,   153,   118,   150,
     124,   149,   118,   229,    29,    88,    89,   118,   186,   223,
      12,    13,   112,   116,   118,   119,   162,   178,   112,   172,
     118,   118,   225,    19,    25,    40,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   192,   232,   233,   234,   235,
     118,   179,   181,   127,   228,   155,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,   104,   105,   106,   107,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   141,   142,
      29,    29,   150,   184,   118,   156,   222,   231,   120,   161,
     118,   156,   131,   131,   131,   150,   131,   131,   131,   131,
      27,    32,   126,   130,   148,   151,   192,   214,    41,   151,
     215,   216,   220,   231,   127,   185,    97,   103,   124,   125,
     127,   221,   120,   121,   125,   154,   235,   235,   235,   118,
     184,   235,   184,   235,   184,   235,   217,   218,   235,    27,
     130,   148,    12,    13,    14,    15,    21,    33,    37,    39,
      42,    46,    72,    88,    89,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   133,   134,   135,   138,   141,
     142,   149,   150,   155,   157,   164,   188,   192,   194,   202,
     203,   205,   206,   208,   209,   210,   232,   236,   239,   244,
     246,   248,   148,   156,   211,   213,   220,   151,   154,    97,
     125,   127,   219,   235,   188,   118,   235,   191,   207,   210,
     112,   132,   154,   132,   151,   185,   132,   185,   132,   185,
     132,   132,   154,   130,   149,   235,   236,   150,   150,   131,
     131,   150,   131,   150,   210,   210,    10,   235,   235,   210,
     241,   245,   163,   210,   210,   210,   192,   210,   210,   210,
     190,   183,   118,   150,    86,    87,    88,    89,    90,    93,
      94,    95,    96,    98,    99,   100,   102,   104,   105,   126,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     147,   148,   149,   150,   179,    38,   212,   154,   215,   235,
     191,   210,   154,   221,   101,    78,    79,    80,    81,    82,
      83,    84,    85,    91,    92,    97,   103,   106,   107,   125,
     154,   154,   235,   132,   132,   132,   217,   150,   118,   210,
     235,   235,   210,   235,   151,   210,   231,   118,   150,   210,
     237,   238,   240,   241,   242,   243,   243,   108,   154,   156,
     121,   122,   161,   151,     1,     7,     8,    10,    20,    22,
      23,    32,    36,    43,    45,   154,   156,   174,   175,   176,
     177,   189,   191,   193,   195,   196,   197,   198,   200,   201,
     207,   210,   184,   148,   151,   204,   210,   210,   210,   210,
     210,   210,   210,   118,   210,   118,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   118,   210,   151,   204,   149,   220,   219,   191,   210,
     205,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   132,   151,   204,    16,
     131,   210,   151,   132,   132,   151,   132,   151,    17,   219,
     151,   124,   149,   154,   124,   149,   154,   156,   210,   241,
     156,   210,   210,   210,   231,    97,   154,   191,   210,   188,
     210,   210,   154,    31,   154,   154,   154,    17,   199,   154,
     185,   118,   237,   124,   151,   150,   127,   149,   151,   210,
     154,   151,   235,   118,   151,   210,   210,   210,   204,   210,
     238,   118,   149,   237,   210,   149,   242,   149,   123,   188,
     188,    17,   191,   210,   154,    11,   188,   155,    34,   222,
     188,   149,   210,   151,   204,   210,   151,   132,   154,   149,
     219,     9,    18,   173,   204,   154,   188,   190,   151,   151,
      16,   210,   210,   149,   210,   188,   210,   154,   156,   235,
     151,   154,   247,   188,   188,   151,    44,   149,   173,   210,
     149
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   158,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   160,   161,   161,   162,   163,   163,   163,
     164,   165,   166,   167,   167,   167,   168,   169,   169,   170,
     171,   171,   172,   172,   173,   173,   173,   174,   175,   176,
     177,   178,   178,   178,   178,   178,   178,   179,   179,   180,
     180,   181,   181,   182,   182,   183,   183,   184,   184,   184,
     185,   185,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   187,
     188,   188,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   191,
     192,   192,   192,   193,   194,   194,   194,   194,   195,   196,
     197,   197,   197,   197,   197,   198,   199,   199,   200,   200,
     201,   202,   202,   202,   203,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   206,   206,   206,   206,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   208,   209,   209,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   211,   211,   212,   212,   213,
     214,   214,   215,   215,   216,   216,   217,   218,   218,   219,
     219,   220,   220,   220,   220,   220,   221,   221,   221,   222,
     222,   222,   222,   222,   223,   223,   224,   225,   225,   225,
     226,   227,   228,   228,   229,   230,   231,   231,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   234,   234,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     236,   236,   236,   236,   237,   237,   238,   238,   239,   239,
     240,   240,   241,   242,   242,   243,   243,   244,   244,   245,
     245,   246,   247,   247,   248
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     0,     1,     2,
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
       3,     4,     3,     3,     2,     3,     4,     3,     4,     2,
       4,     4,     3,     4,     3,     5,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     3,     0,     1,     3,
       0,     3,     1,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     3,     5,     3,     3,     1,     1,     1,     4,
       6,     5,     4,     3,     0,     2,     4,     0,     3,     5,
       4,     5,     0,     2,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     4,     3,
       4,     2,     3,     2,     3,     4,     2,     4,     6,     4,
       1,     4,     5,     1,     4,     5,     1,     4,     5,     4,
       1,     1,     1,     1,     3,     5,     1,     3,     5,     7,
       1,     3,     3,     1,     1,     1,     3,     5,     5,     1,
       3,     4,     0,     3,    10
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
          case 118: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2497 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2503 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2509 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2515 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2521 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2527 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2533 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2539 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2545 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2551 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2557 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2563 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2569 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2575 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2581 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2587 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2593 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2599 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2605 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2611 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2617 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2623 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2629 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2635 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2641 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2647 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2653 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2659 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2665 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2671 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2677 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2683 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2689 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2695 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2701 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2707 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2713 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2719 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2725 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2731 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2737 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2743 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2749 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2755 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2761 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2767 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2773 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2779 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2785 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2791 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2797 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2803 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2809 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2815 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2821 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2827 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2833 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2839 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2845 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2851 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2857 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2863 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2869 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2875 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2881 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2887 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 245: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2893 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2899 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2905 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2911 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 365 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 377 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 378 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 382 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 386 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 390 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 407 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 423 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3299 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 448 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3307 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 451 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 457 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 466 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3344 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3350 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3356 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 493 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 496 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 502 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 503 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 504 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 511 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 518 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3416 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 540 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3451 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 551 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 552 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 553 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3469 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 554 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3475 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 558 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 563 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 572 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 576 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 595 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 607 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 611 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 612 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 613 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 628 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 632 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 633 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 635 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 636 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 647 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 648 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 649 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3746 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 672 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3939 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3969 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3975 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3981 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3987 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4043 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 801 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 802 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 808 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 812 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 818 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 821 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 828 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 838 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 842 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4127 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 845 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 848 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 853 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 856 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4163 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 864 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 913 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4238 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 916 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 929 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 933 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 938 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 942 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 953 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 959 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 965 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 1018 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 1020 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1021 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1025 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1026 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1027 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1028 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1029 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1030 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1033 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1034 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1035 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1036 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1037 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1038 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1039 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1040 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1044 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1055 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4529 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1060 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1070 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1076 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1077 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1079 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1080 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1082 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1083 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1085 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1087 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1088 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1089 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1090 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1093 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1094 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1096 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1098 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1099 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1102 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1103 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1108 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4778 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1116 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1119 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4803 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1122 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4809 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1123 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4815 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4821 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1125 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4827 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1126 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4833 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1127 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4839 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1128 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4853 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1137 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1147 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4949 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1191 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4955 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4961 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4967 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4973 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1201 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5006 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1220 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5012 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1224 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5018 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5094 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1272 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1273 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1281 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1286 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5151 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1291 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5163 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5223 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1350 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1359 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1371 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1397 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5295 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1402 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1441 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1507 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1514 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1525 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5451 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1526 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5469 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5475 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5481 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5493 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5499 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5505 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1535 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5511 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1536 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1537 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5529 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1540 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1541 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1543 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1544 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1545 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1546 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1550 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1565 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1576 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1578 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5648 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1592 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1598 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1602 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5678 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1607 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1612 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1617 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1622 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1629 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5730 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1634 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1639 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1645 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1650 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1654 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1659 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1668 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5803 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1672 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1677 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5827 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1686 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1690 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1695 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1704 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5889 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1720 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1727 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5918 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1736 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1741 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1748 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1753 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1779 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1805 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1810 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1821 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6057 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1826 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1833 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6080 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1845 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1846 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1850 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6110 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6114 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1865 "ds_parser.ypp" /* yacc.c:1903  */


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


