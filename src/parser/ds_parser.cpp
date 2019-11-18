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
    DAS_LABEL = 304,
    DAS_GOTO = 305,
    DAS_TBOOL = 306,
    DAS_TVOID = 307,
    DAS_TSTRING = 308,
    DAS_TAUTO = 309,
    DAS_TINT = 310,
    DAS_TINT2 = 311,
    DAS_TINT3 = 312,
    DAS_TINT4 = 313,
    DAS_TUINT = 314,
    DAS_TUINT2 = 315,
    DAS_TUINT3 = 316,
    DAS_TUINT4 = 317,
    DAS_TFLOAT = 318,
    DAS_TFLOAT2 = 319,
    DAS_TFLOAT3 = 320,
    DAS_TFLOAT4 = 321,
    DAS_TRANGE = 322,
    DAS_TURANGE = 323,
    DAS_TBLOCK = 324,
    DAS_TINT64 = 325,
    DAS_TUINT64 = 326,
    DAS_TDOUBLE = 327,
    DAS_TFUNCTION = 328,
    DAS_TLAMBDA = 329,
    DAS_TINT8 = 330,
    DAS_TUINT8 = 331,
    DAS_TINT16 = 332,
    DAS_TUINT16 = 333,
    DAS_TTUPLE = 334,
    ADDEQU = 335,
    SUBEQU = 336,
    DIVEQU = 337,
    MULEQU = 338,
    MODEQU = 339,
    ANDEQU = 340,
    OREQU = 341,
    XOREQU = 342,
    SHL = 343,
    SHR = 344,
    ADDADD = 345,
    SUBSUB = 346,
    LEEQU = 347,
    SHLEQU = 348,
    SHREQU = 349,
    GREQU = 350,
    EQUEQU = 351,
    NOTEQU = 352,
    RARROW = 353,
    LARROW = 354,
    QQ = 355,
    QDOT = 356,
    LPIPE = 357,
    LBPIPE = 358,
    RPIPE = 359,
    CLONEEQU = 360,
    ROTL = 361,
    ROTR = 362,
    ROTLEQU = 363,
    ROTREQU = 364,
    MAPTO = 365,
    BRABRAB = 366,
    BRACBRB = 367,
    CBRCBRB = 368,
    INTEGER = 369,
    LONG_INTEGER = 370,
    UNSIGNED_INTEGER = 371,
    UNSIGNED_LONG_INTEGER = 372,
    FLOAT = 373,
    DOUBLE = 374,
    NAME = 375,
    BEGIN_STRING = 376,
    STRING_CHARACTER = 377,
    END_STRING = 378,
    BEGIN_STRING_EXPR = 379,
    END_STRING_EXPR = 380,
    UNARY_MINUS = 381,
    UNARY_PLUS = 382,
    PRE_INC = 383,
    PRE_DEC = 384,
    POST_INC = 385,
    POST_DEC = 386,
    DEREF = 387,
    COLCOL = 388
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

#line 360 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   4585

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  161
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  95
/* YYNRULES -- Number of rules.  */
#define YYNRULES  370
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  661

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   388

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   144,     2,   160,   159,   139,   132,     2,
     152,   153,   137,   136,   126,   135,   149,   138,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   129,   156,
     133,   127,   134,   128,   140,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   150,     2,   151,   131,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   157,   130,   158,   143,     2,     2,     2,
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
     125,   141,   142,   145,   146,   147,   148,   154,   155
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   356,   356,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   371,   383,   384,   388,   392,   396,   405,
     413,   429,   450,   454,   457,   463,   472,   485,   486,   490,
     494,   495,   499,   502,   508,   514,   517,   523,   524,   525,
     532,   539,   551,   561,   570,   571,   572,   573,   574,   575,
     579,   584,   592,   593,   597,   604,   616,   620,   627,   628,
     632,   633,   634,   638,   642,   649,   653,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   692,   762,   765,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   794,   798,   804,   810,   824,   825,   831,   835,   841,
     844,   847,   851,   857,   861,   865,   868,   871,   876,   879,
     887,   893,   894,   898,   899,   903,   936,   939,   944,   952,
     956,   961,   965,   973,   976,   982,   983,   993,   998,  1050,
    1051,  1052,  1053,  1054,  1055,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1074,  1078,  1089,  1094,  1104,  1105,  1106,  1107,  1108,  1109,
    1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,
    1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,
    1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,
    1140,  1141,  1142,  1146,  1150,  1153,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1165,  1166,  1167,  1168,  1169,  1170,  1171,
    1181,  1191,  1225,  1226,  1230,  1231,  1235,  1243,  1246,  1253,
    1254,  1258,  1259,  1263,  1269,  1270,  1274,  1275,  1279,  1284,
    1287,  1291,  1297,  1306,  1307,  1308,  1312,  1315,  1320,  1325,
    1332,  1342,  1345,  1352,  1381,  1382,  1383,  1384,  1388,  1391,
    1400,  1412,  1424,  1438,  1439,  1443,  1482,  1548,  1555,  1564,
    1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,
    1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,
    1585,  1586,  1587,  1591,  1602,  1606,  1616,  1617,  1618,  1619,
    1633,  1639,  1643,  1648,  1653,  1658,  1665,  1670,  1674,  1679,
    1684,  1691,  1696,  1702,  1707,  1711,  1716,  1725,  1729,  1734,
    1743,  1747,  1752,  1761,  1770,  1771,  1772,  1773,  1777,  1784,
    1793,  1798,  1805,  1810,  1816,  1823,  1834,  1837,  1851,  1860,
    1861,  1865,  1870,  1877,  1882,  1893,  1898,  1905,  1917,  1918,
    1922
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
  "\"module\"", "\"public\"", "\"label\"", "\"goto\"", "\"bool\"",
  "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"",
  "\"int4\"", "\"uint\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"<|\"", "\" <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "\"end of the string\"", "\"{\"", "\"}\"", "','",
  "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'",
  "'*'", "'/'", "'%'", "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('",
  "')'", "DEREF", "COLCOL", "';'", "'{'", "'}'", "'$'", "'#'", "$accept",
  "program", "module_declaration", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "options_declaration",
  "require_declaration", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "expression_else",
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
  "expr_list", "block_or_simple_block", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_named_call", "expr_method_call", "expr",
  "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "copy_or_move", "variable_declaration",
  "copy_or_move_or_clone", "let_variable_declaration",
  "global_variable_declaration_list", "global_let", "enum_const_value",
  "enum_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration",
  "type_declaration", "make_decl", "make_struct_fields", "make_struct_dim",
  "make_struct_decl", "make_tuple", "make_map_tuple", "make_any_tuple",
  "make_dim", "make_dim_decl", "make_table", "make_table_decl",
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
     375,   376,   377,   378,   379,   380,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
      64,   381,   382,   126,    33,   383,   384,   385,   386,    46,
      91,    93,    40,    41,   387,   388,    59,   123,   125,    36,
      35
};
# endif

#define YYPACT_NINF -441

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-441)))

#define YYTABLE_NINF -166

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -441,    34,  -441,  -441,   -67,   -26,   -25,   -22,   -18,  -441,
     -13,   -12,  -441,  -441,  -441,  -441,  -441,    88,  -441,  -110,
    -441,  -441,  -441,  -441,   -16,  -441,    19,    10,    27,  -441,
    -441,   -34,  -441,    -8,    33,    15,  -441,    -1,    52,    36,
    -441,   -41,  -441,    75,   -14,  -441,   360,   -67,    82,   -26,
    -441,    84,    91,  -441,  -441,  2934,  -441,    97,  -441,   -67,
     -12,  -441,    83,    61,  3906,   193,   200,  -441,    86,   -92,
    -441,  -441,  -441,  -441,  -441,   117,  -441,  -441,  -441,  -441,
    -441,  -441,   -91,   124,   135,   138,  -441,  -441,  -441,   123,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,   143,  -441,  -441,  -441,   146,   149,
    -441,  -441,  -441,  -441,   151,  -441,  -441,  -441,  -441,   477,
    -441,   -75,  -441,   -90,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,   -24,   156,  -441,  -441,  -441,   187,  -441,    13,
     -82,  -441,  2934,  2934,  2934,   175,  2864,  2864,  2864,  2934,
    -441,    -2,  -441,  -441,  -441,  1582,  -441,  -441,  -441,   -49,
     178,  -441,  -441,  -116,  -441,   -56,  2934,   142,  -441,  -441,
     180,  -441,  2934,  2611,  -441,  -441,    24,  -441,    23,   105,
     183,   153,   156,   203,   156,   218,   156,   385,  -441,     0,
     477,   176,  -441,   158,  -441,  -441,  -441,  2794,   160,  -441,
     169,   177,   190,   172,   192,   182,  2611,  2611,  2724,  2934,
    2611,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  2611,  2611,
    2611,   -90,  2611,  2611,  -441,  2611,  -441,   179,  -441,  -441,
     -84,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  3928,   184,
    -441,  -441,  -441,  -441,  -441,   -67,  -441,   289,   174,  -441,
    -441,   -20,  -441,  -441,  2934,  2611,   477,  -441,  -441,    14,
    -441,   225,  2941,  -441,  -441,   109,   185,  -441,  2934,  -441,
    -441,   198,  -441,   204,  -441,   205,  -441,  -441,  2934,  -441,
    -441,   386,  -441,   220,  2611,  2934,  2934,  2611,  2934,  1715,
     961,   961,   178,   964,  1113,  4056,  -441,    78,    85,   961,
     961,  -441,  -441,   961,   961,  3502,   672,    86,  -441,  1225,
    2611,  2611,  -441,  -441,  2611,  2611,  2611,  2611,   222,  2611,
     224,  2611,  2611,  2611,  2611,  2611,  2611,  2611,  2611,  2611,
    2611,  2611,  2611,  2611,  2611,  2611,   227,  2611,  -441,  1827,
     -33,  -441,   178,  -441,  -441,   120,  -441,  3249,  -441,  2611,
      92,  2611,  2611,  2611,  2611,  2611,  2611,  2611,  2611,  2611,
    2611,  2611,  2611,  2611,  2611,  2611,  -441,  -441,  -441,  -441,
     416,  -441,  -441,  -441,  -441,  1939,  1337,  3568,   423,   461,
    3634,   462,  -441,  3700,     5,   -73,  2051,  4056,   219,    38,
     223,  -441,  -441,    46,  2163,   104,   111,  2611,  2611,   196,
    -441,  2611,   226,  -441,  -441,  2611,  2611,   178,  1470,  -441,
     142,  2611,  2611,  -441,   195,   242,  2275,  -441,   326,   210,
     211,  -441,  -441,  -441,  -441,  -441,  -441,   233,   235,   251,
    -441,  -441,   342,  -441,   -72,  3320,   156,   254,  -441,   -70,
    4224,  4435,  4435,  4393,  4393,  4373,  4373,   228,   961,  -441,
     -63,   -63,  4435,  4435,  4112,  4244,  4300,  4320,  4393,  4393,
     551,   551,   668,   668,   668,  -441,  3992,  -441,   -54,  -441,
    -441,  2611,  -441,  3018,  -441,  4224,  4224,  4224,  4224,  4224,
    4224,  4224,  4224,  4224,  4224,  4224,  4224,  4224,  4224,  4224,
    -441,  -441,   -37,  2934,   255,  3766,  -441,  2611,  2611,  -441,
    2611,  -441,  2611,  2611,   254,   256,   232,   254,  2611,   259,
    2611,   254,   260,   263,  4224,  -441,  -441,  4168,  3376,  3376,
       7,  2611,  -441,  -441,  3095,   368,  4224,  3376,  -441,   286,
     302,  4224,   262,  -441,  -441,  -441,  -441,  -441,   403,   178,
    -441,   -78,   -47,   -23,  2611,  -441,  2387,  2611,  -441,  -441,
    4224,  -441,  -441,   470,   304,  -441,  4393,  4393,  4393,   -93,
    4224,    47,   -47,  -441,   219,  4224,  -441,  -441,   116,  -441,
    -441,  -441,  -441,    39,  2611,  -441,  3172,  -441,   142,  -441,
    -441,  -441,  -441,  -441,  -441,  2611,  -441,  -441,   287,  4224,
    -441,   -36,  4224,  -441,  2499,  2611,   288,  2611,   290,   142,
    2611,  -441,   -65,  -441,  -441,   831,  4224,  -441,  -441,  2934,
    3832,  3439,  -441,  4224,  -441,  -441,  3376,   142,  -441,   497,
    -441,   400,   294,    39,  -441,  -441,  2611,   295,  -441,  4224,
    -441
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
      56,     0,    52,     0,     0,   271,     0,     0,     0,     0,
      28,     0,     0,    26,   278,     0,    13,     0,   127,     0,
       0,    59,   283,     0,     0,     0,     0,    65,    60,     0,
      48,    49,    46,    47,    45,     0,    44,    51,    33,    31,
      25,    24,     0,     0,     0,     0,   289,   309,   290,   314,
     291,   295,   296,   297,   298,   302,   303,   304,   305,   306,
     307,   308,   310,   311,   334,   294,   301,   312,   337,   340,
     292,   299,   293,   300,     0,   313,   316,   318,   317,   281,
     126,     0,    57,     0,   285,   247,    68,    69,    71,    70,
      72,    73,    74,    75,    94,    95,    92,    93,    85,    96,
      97,    86,    83,    84,    98,    99,   100,   101,    88,    89,
      87,    81,    82,    77,    76,    78,    79,    80,    67,    66,
      90,    91,     0,    63,   287,   273,   272,     0,    14,     0,
       0,   282,     0,     0,     0,     0,    60,    60,    60,     0,
     322,     0,   330,   329,   324,     0,   327,    55,   284,   242,
       0,    61,   251,     0,   249,   258,     0,     0,   264,   265,
       0,   263,     0,     0,    15,    16,     0,   279,     0,     0,
       0,     0,    63,     0,    63,     0,    63,     0,   254,     0,
     253,   323,   326,     0,   328,   189,   190,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   159,   161,   160,   162,   163,   164,    17,     0,     0,
       0,     0,     0,     0,   320,     0,   121,    58,   187,   157,
     185,   235,   234,   233,   238,   186,   237,   236,     0,     0,
     188,   344,   345,   346,   347,     0,   286,   244,     0,   250,
      62,     0,   257,   256,     0,     0,    64,   102,   288,     0,
     270,     0,   165,   274,   276,     0,     0,   331,     0,   333,
     315,     0,   335,     0,   338,     0,   341,   343,     0,   325,
     321,   129,   132,     0,     0,     0,     0,     0,     0,     0,
     213,   214,     0,     0,     0,     0,   365,     0,     0,   194,
     193,   226,   221,   192,   191,     0,     0,    60,   241,     0,
       0,     0,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
       0,   245,     0,   248,   252,   259,   262,   261,   266,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   269,   275,   277,   280,
       0,   336,   339,   342,   255,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,   125,     0,   356,   350,     0,
     359,   360,   361,     0,     0,     0,     0,     0,     0,     0,
      20,     0,    18,   217,   123,     0,     0,     0,     0,   133,
       0,     0,     0,   134,     0,     0,     0,   105,   103,     0,
       0,   116,   112,   110,   111,   122,   106,     0,     0,     0,
     115,   117,   141,   109,     0,   165,    63,     0,   222,     0,
     153,   195,   196,   208,   209,   206,   207,     0,   231,   220,
     239,   240,   197,   198,     0,   211,   212,   210,   204,   205,
     200,   199,   201,   202,   203,   219,     0,   224,     0,   243,
     246,     0,   268,   165,   124,   172,   173,   175,   174,   176,
     169,   170,   171,   177,   178,   167,   168,   179,   180,   166,
     332,   130,     0,     0,     0,     0,   227,     0,     0,   228,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,   366,   367,     0,     0,     0,
       0,     0,   135,   138,   165,     0,   128,     0,   120,     0,
       0,    36,     0,   118,   119,   108,   113,   114,     0,     0,
     107,     0,     0,     0,     0,   223,     0,     0,   218,   225,
     260,   267,   131,     0,     0,   149,   146,   147,   148,     0,
     348,     0,     0,   352,   351,   357,   363,   362,     0,   354,
     364,    19,    42,    37,     0,   139,   165,   136,     0,    43,
      34,    35,   121,   142,   145,     0,   155,   158,     0,   154,
     182,     0,   232,   151,     0,     0,     0,     0,     0,     0,
       0,    40,     0,   137,   140,     0,   156,   181,   183,     0,
       0,   368,   353,   349,   355,    38,     0,     0,   104,     0,
     150,     0,     0,    37,    41,   152,     0,     0,    39,   369,
     370
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -441,  -441,  -441,   121,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,   402,  -441,  -441,  -201,  -441,  -441,
    -441,  -441,   406,   -51,  -441,   394,  -441,   199,  -158,  -207,
    -441,  -441,  -187,  -441,  -154,  -180,   -11,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,   460,  -441,  -441,  -441,  -363,  -441,
      87,  -441,  -334,  -441,  -441,    41,  -441,  -441,  -441,  -441,
     181,  -441,   155,  -441,  -183,  -179,   186,  -105,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,   -68,   -52,  -441,
    -441,     4,   239,  -440,  -320,  -441,  -441,  -227,   -50,   167,
    -441,  -441,  -441,  -441,  -441
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,    76,   328,   258,    14,    15,    31,
      32,    53,    16,    28,    29,   449,   450,   631,   451,   452,
     453,   454,    25,    26,    39,    40,    41,    17,   163,   197,
      68,    18,   259,   455,   336,   456,   260,   457,   261,   458,
     459,   460,   461,   569,   462,   463,   262,   263,   469,   617,
     264,   265,   291,   266,   267,   470,   277,   372,   278,   189,
     192,   193,   218,   219,   533,   194,   203,   166,    69,    20,
     296,    82,    21,    22,   124,    63,    23,   195,   269,   117,
     118,   220,   270,   418,   419,   271,   420,   421,   422,   423,
     272,   327,   273,   652,   274
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   167,   464,   116,   425,   301,   498,   303,   121,   305,
     287,   279,   285,   326,    50,    64,    36,   190,   212,   214,
     216,   190,   532,   290,   604,   221,   282,   573,   164,   170,
      37,   380,   615,   574,     2,   348,   338,   280,   350,     3,
     281,   180,   522,   282,   115,   206,   181,    45,   629,    42,
     180,    47,   282,    24,   283,   181,   574,   630,     4,   119,
       5,   574,     6,   625,     7,    38,   165,   171,   339,     8,
     200,   283,   574,   284,   207,     9,    65,    66,   187,   256,
     283,    10,    57,   575,   570,    60,   366,   367,    27,   574,
     574,   647,    43,    47,    44,    30,   164,   594,    33,   579,
     164,   275,    34,   535,    51,   376,    67,    35,    37,   276,
      61,    46,   188,   198,   182,    52,   582,   638,   499,   199,
     116,   116,   116,   182,   116,   116,   116,   116,   618,   191,
     222,   200,   180,   200,   307,   204,   205,   181,   293,    48,
     294,   201,   183,    38,   116,    47,   184,   180,   223,    54,
     116,   183,   181,    49,    57,   184,   308,   297,   224,   295,
      55,   115,   115,   115,   185,   115,   115,   115,   115,   589,
     378,    56,    58,   185,   186,   116,   208,   209,   210,   466,
     213,   215,   217,   186,    11,   115,   116,   116,    59,   536,
      12,   115,   501,   500,   537,    62,    78,   539,   626,   502,
     286,   545,   540,   537,    80,   182,   289,   168,   430,   431,
     180,    81,   123,   621,   591,   181,   115,   120,   125,   282,
     182,   598,   160,   397,   370,   398,   268,   115,   115,   161,
     180,   311,   116,   183,   428,   181,   429,   184,   162,   168,
     332,   632,   323,   324,   292,   180,   116,   283,   183,   256,
     181,   257,   184,   555,   414,   185,   116,   172,   553,   571,
     537,   298,   542,   116,   116,   186,   116,   540,   173,   543,
     185,   174,   537,   115,   628,   175,   176,   320,   321,   177,
     186,   325,   178,   182,   179,   196,   198,   115,   375,   329,
     330,   331,   199,   333,   334,   211,   335,   115,   164,   256,
     288,   464,   400,   182,   115,   115,   300,   115,   309,   310,
     315,   183,   313,   200,   201,   184,   202,   299,   182,   408,
     409,   314,   411,   316,   317,   318,   377,   371,   380,    11,
     373,   183,   401,   185,   319,   184,   369,   302,   402,   403,
     406,   399,   477,   186,   479,   535,   183,   495,   204,   538,
     184,   558,   304,   185,   546,   407,   559,   562,   410,   568,
     413,   602,   603,   186,   417,   417,   563,   564,   185,   550,
     609,   605,    70,    71,   572,   584,   592,   465,   186,   608,
     576,   471,   472,   593,   616,   473,   474,   475,   476,   565,
     478,   566,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   567,   496,   627,
     596,   599,   180,   180,   600,   610,   611,   181,   181,   612,
     503,   634,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   613,   624,   642,
     637,   644,   645,   180,   656,   657,   660,   525,   181,   432,
     180,    79,   658,    77,   122,   181,   337,   335,   635,   653,
     654,    19,   374,   404,   614,   335,   312,   504,   544,   325,
       0,   116,   547,     0,    72,   379,   548,   549,    73,   554,
      74,    75,   556,   557,     0,   182,   182,   561,   180,   180,
     597,   426,     0,   181,   181,     0,     0,   180,     0,     0,
       0,   167,   181,     0,   180,     0,     0,     0,     0,   181,
       0,     0,   115,   183,   183,     0,   182,   184,   184,   306,
       0,     0,     0,   182,   180,     0,     0,   583,     0,   181,
       0,     0,     0,     0,     0,   185,   185,     0,   405,     0,
       0,     0,   580,     0,   183,   186,   186,     0,   184,     0,
     520,   183,     0,     0,     0,   184,     0,   527,     0,     0,
       0,   182,   182,     0,     0,     0,   185,     0,   586,   587,
     182,   588,     0,   185,   590,     0,   186,   182,     0,   595,
       0,   417,     0,   186,     0,     0,     0,   116,     0,   183,
     183,     0,   606,   184,   184,   528,   530,   182,   183,     0,
       0,     0,   184,     0,     0,   183,     0,     0,     0,   184,
       0,   185,   185,     0,     0,   619,     0,     0,   622,     0,
     185,   186,   186,   623,     0,   183,     0,   185,   115,   184,
     186,     0,     0,     0,     0,     0,     0,   186,     0,     0,
       0,   342,   343,   649,     0,     0,     0,   185,     0,   348,
     655,   349,   350,   351,     0,   352,   636,   186,     0,     0,
       0,     0,     0,     0,     0,   640,   641,     0,   643,     0,
       0,   646,     0,   434,     0,     0,   465,     3,     0,   435,
     436,     0,   437,     0,   225,   226,   227,   228,   363,   364,
     365,     0,   438,   229,   439,   440,     0,   659,     0,     0,
     366,   367,     0,     0,   441,   230,     0,     0,   442,   231,
       0,   232,     0,     9,   233,   443,     0,   444,   234,     0,
       0,   445,   446,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   235,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,   342,   343,
       0,     0,   236,   237,     0,     0,   348,     0,   349,   350,
     351,     0,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   239,   240,   241,   242,   243,   244,
     245,   246,    37,   247,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,   249,   250,
       0,     0,   251,     0,     0,   252,   253,   366,   367,     0,
       0,     0,     0,     0,   255,     0,     0,    38,   447,   256,
     448,   257,   434,     0,     0,     0,     3,     0,   435,   436,
       0,   437,     0,   225,   226,   227,   228,     0,     0,     0,
       0,   438,   229,   439,   440,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   230,     0,     0,   442,   231,     0,
     232,     0,     9,   233,   443,     0,   444,   234,     0,     0,
     445,   446,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   235,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,   239,   240,   241,   242,   243,   244,   245,
     246,    37,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,   250,     0,
       0,   251,     0,     0,   252,   253,   225,   226,   227,   228,
       0,     0,     0,   255,     0,   229,    38,   447,   256,   648,
     257,   180,     0,     0,     0,     0,   181,   230,     0,     0,
       0,   231,     0,   232,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   235,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,   342,   343,     0,   236,   237,     0,     0,     0,   348,
       0,     0,   350,   351,   182,   352,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   415,   247,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   184,     0,     0,   248,
     249,   250,     0,     0,   251,     0,     0,   252,   253,     0,
     366,   367,     0,     0,   185,     0,   416,     0,     0,    38,
       0,   256,     0,   257,   186,   225,   226,   227,   228,     0,
       0,     0,     0,     0,   229,     0,     0,     0,     0,     0,
     180,     0,     0,     0,     0,   181,   230,     0,     0,     0,
     231,     0,   232,     0,     0,   233,     0,     0,     0,   234,
       0,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   235,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,     0,     0,     0,     0,     0,
       0,     0,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   415,   247,     0,     0,   225,   226,   227,
     228,   183,     0,     0,     0,   184,   229,     0,   248,   249,
     250,     0,     0,   251,     0,     0,   252,   253,   230,     0,
       0,     0,   231,   185,   232,   424,     0,   233,    38,     0,
     256,   234,   257,   186,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   235,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,   239,   240,   241,
     242,   243,   244,   245,   246,    37,   247,     0,     0,   225,
     226,   227,   228,   523,     0,     0,     0,     0,   229,     0,
     248,   249,   250,     0,     0,   251,     0,     0,   252,   253,
     230,     0,     0,     0,   231,   467,   232,   255,   468,   233,
      38,     0,   256,   234,   257,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   235,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,   239,
     240,   241,   242,   243,   244,   245,   246,    37,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     524,     0,   248,   249,   250,     0,     0,   251,     0,     0,
     252,   253,   225,   226,   227,   228,     0,     0,     0,   255,
       0,   229,    38,     0,   256,     0,   257,     0,     0,     0,
       0,     0,     0,   230,     0,     0,     0,   231,     0,   232,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,    86,    87,    88,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
     105,   106,   107,     0,   235,   110,   111,   112,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,     0,     0,     0,     0,     0,     0,     0,   551,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,   239,   240,   241,   242,   243,   244,   245,   246,
      37,   247,     0,     0,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   248,   249,   250,     0,     0,
     251,     0,     0,   252,   253,   230,     0,     0,     0,   231,
       0,   232,   255,     0,   233,    38,   552,   256,   234,   257,
       0,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   235,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   239,   240,   241,   242,   243,   244,
     245,   246,    37,   247,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,   249,   250,
       0,     0,   251,     0,     0,   252,   253,   225,   226,   227,
     228,     0,     0,   254,   255,     0,   229,    38,     0,   256,
       0,   257,     0,     0,     0,     0,     0,     0,   230,     0,
       0,     0,   231,     0,   232,     0,     0,   233,     0,     0,
       0,   234,     0,     0,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   235,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,   239,   240,   241,
     242,   243,   244,   245,   246,    37,   247,     0,     0,   225,
     226,   227,   228,     0,     0,     0,     0,     0,   229,     0,
     248,   249,   250,     0,     0,   251,     0,     0,   252,   253,
     230,     0,     0,     0,   231,     0,   232,   255,   412,   233,
      38,     0,   256,   234,   257,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   235,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,   239,
     240,   241,   242,   243,   244,   245,   246,    37,   247,     0,
       0,   225,   226,   227,   228,     0,     0,     0,     0,     0,
     229,     0,   248,   249,   250,     0,     0,   251,     0,     0,
     252,   253,   230,     0,     0,     0,   231,     0,   232,   255,
     497,   233,    38,     0,   256,   234,   257,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   235,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   239,   240,   241,   242,   243,   244,   245,   246,    37,
     247,     0,     0,   225,   226,   227,   228,     0,     0,     0,
       0,     0,   229,     0,   248,   249,   250,     0,     0,   251,
       0,     0,   252,   253,   230,     0,     0,     0,   231,     0,
     232,   255,   521,   233,    38,     0,   256,   234,   257,     0,
       0,     0,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   235,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,   239,   240,   241,   242,   243,   244,   245,
     246,    37,   247,     0,     0,   225,   226,   227,   228,     0,
       0,     0,     0,     0,   229,     0,   248,   249,   250,     0,
       0,   251,     0,     0,   252,   253,   230,     0,     0,     0,
     231,     0,   232,   255,   534,   233,    38,     0,   256,   234,
     257,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   235,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,   239,   240,   241,   242,   243,
     244,   245,   246,    37,   247,     0,     0,   225,   226,   227,
     228,     0,     0,     0,     0,     0,   229,     0,   248,   249,
     250,     0,     0,   251,     0,     0,   252,   253,   230,     0,
       0,     0,   231,     0,   232,   255,   541,   233,    38,     0,
     256,   234,   257,     0,   560,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   235,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,   239,   240,   241,
     242,   243,   244,   245,   246,    37,   247,     0,     0,   225,
     226,   227,   228,     0,     0,     0,     0,     0,   229,     0,
     248,   249,   250,     0,     0,   251,     0,     0,   252,   253,
     230,     0,     0,     0,   231,     0,   232,   255,     0,   233,
      38,     0,   256,   234,   257,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   235,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,   239,
     240,   241,   242,   243,   244,   245,   246,    37,   247,     0,
       0,   225,   226,   227,   228,   639,     0,     0,     0,     0,
     229,     0,   248,   249,   250,     0,     0,   251,     0,     0,
     252,   253,   230,     0,     0,     0,   231,     0,   232,   255,
     620,   233,    38,     0,   256,   234,   257,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   235,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   239,   240,   241,   242,   243,   244,   245,   246,    37,
     247,     0,     0,   225,   226,   227,   228,     0,     0,     0,
       0,     0,   229,     0,   248,   249,   250,     0,     0,   251,
       0,     0,   252,   253,   230,     0,     0,     0,   231,     0,
     232,   255,     0,   233,    38,     0,   256,   234,   257,     0,
       0,     0,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   235,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,   239,   240,   241,   242,   243,   244,   245,
     246,    37,   247,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,   248,   249,   250,    84,
       0,   251,     0,     0,   252,   253,     0,     0,     0,     0,
       0,     0,     0,   255,    85,     0,    38,     0,   256,     0,
     257,     0,     0,     0,     0,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,     0,     0,     0,    38,
       0,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,   238,   239,   240,     0,     0,
       0,     0,     0,     0,    37,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,     0,     0,     0,    38,
       0,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,   162,     0,     0,    38,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   340,
     341,   342,   343,   344,   389,   390,   345,   346,   347,   348,
     391,   349,   350,   351,     0,   352,   392,   353,   354,   393,
     394,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,   355,
       0,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,     0,     0,     0,     0,     0,     0,     0,     0,    38,
     366,   367,     0,     0,     0,     0,     0,   396,   381,   382,
     383,   384,   385,   386,   387,   388,   340,   341,   342,   343,
     344,   389,   390,   345,   346,   347,   348,   391,   349,   350,
     351,     0,   352,   392,   353,   354,   393,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,   355,     0,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
       0,     0,     0,     0,   581,   381,   382,   383,   384,   385,
     386,   387,   388,   340,   341,   342,   343,   344,   389,   390,
     345,   346,   347,   348,   391,   349,   350,   351,     0,   352,
     392,   353,   354,   393,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   355,     0,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367,     0,     0,     0,     0,
       0,   607,   381,   382,   383,   384,   385,   386,   387,   388,
     340,   341,   342,   343,   344,   389,   390,   345,   346,   347,
     348,   391,   349,   350,   351,     0,   352,   392,   353,   354,
     393,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,     0,     0,     0,     0,   633,   381,
     382,   383,   384,   385,   386,   387,   388,   340,   341,   342,
     343,   344,   389,   390,   345,   346,   347,   348,   391,   349,
     350,   351,  -165,   352,   392,   353,   354,   393,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   355,     0,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   367,
     381,   382,   383,   384,   385,   386,   387,   388,   340,   341,
     342,   343,   344,   389,   390,   345,   346,   347,   348,   391,
     349,   350,   351,     0,   352,   392,   353,   354,   393,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,   355,     0,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,   340,   341,   342,   343,   344,   366,
     367,   345,   346,   347,   348,     0,   349,   350,   351,     0,
     352,     0,   353,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,   340,   341,   342,
     343,   344,     0,   256,   345,   346,   347,   348,     0,   349,
     350,   351,     0,   352,     0,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   355,     0,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   367,
     340,   341,   342,   343,   344,   651,     0,   345,   346,   347,
     348,     0,   349,   350,   351,     0,   352,     0,   353,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,     0,   433,   340,   341,   342,   343,
     344,     0,     0,   345,   346,   347,   348,     0,   349,   350,
     351,     0,   352,     0,   353,   354,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   355,     0,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,     0,
       0,   526,   340,   341,   342,   343,   344,     0,     0,   345,
     346,   347,   348,     0,   349,   350,   351,     0,   352,     0,
     353,   354,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   355,     0,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,     0,     0,   529,   340,   341,
     342,   343,   344,     0,     0,   345,   346,   347,   348,     0,
     349,   350,   351,     0,   352,     0,   353,   354,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   355,     0,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,     0,   531,   340,   341,   342,   343,   344,     0,
       0,   345,   346,   347,   348,     0,   349,   350,   351,     0,
     352,     0,   353,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   367,     0,     0,   585,
     340,   341,   342,   343,   344,     0,     0,   345,   346,   347,
     348,     0,   349,   350,   351,     0,   352,     0,   353,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,     0,     0,   650,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   145,   146,   147,   340,   341,   342,   343,
     344,     0,     0,   345,   346,   347,   348,     0,   349,   350,
     351,     0,   352,     0,   353,   354,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,     0,     0,     0,   158,
     159,     0,     0,     0,     0,     0,   355,     0,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   367,   368,
     340,   341,   342,   343,   344,     0,     0,   345,   346,   347,
     348,     0,   349,   350,   351,     0,   352,     0,   353,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,     0,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   367,   578,   340,   341,   342,   343,   344,     0,
       0,   345,   346,   347,   348,     0,   349,   350,   351,     0,
     352,     0,   353,   354,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   355,     0,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,     0,     0,     0,     0,
     340,   341,   342,   343,   344,   366,   367,   345,   346,   347,
     348,     0,   349,   350,   351,     0,   352,     0,   353,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,   577,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,     0,     0,     0,   340,   341,   342,   343,
     344,   366,   367,   345,   346,   347,   348,     0,   349,   350,
     351,     0,   352,     0,   353,   354,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   601,     0,     0,   355,     0,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,   340,   341,   342,   343,   344,   366,   367,   345,
     346,   347,   348,     0,   349,   350,   351,     0,   352,     0,
     353,   354,   340,   341,   342,   343,   344,     0,     0,   345,
     346,   347,   348,     0,   349,   350,   351,     0,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   367,   357,   358,   359,   360,   361,
     362,   363,   364,   365,     0,     0,     0,     0,   340,   341,
     342,   343,   344,   366,   367,   345,   346,   347,   348,     0,
     349,   350,   351,     0,   352,     0,   353,   354,   340,   341,
     342,   343,   344,     0,     0,   345,   346,   347,   348,     0,
     349,   350,   351,     0,   352,     0,   353,   354,     0,     0,
       0,     0,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     367,     0,     0,   359,   360,   361,   362,   363,   364,   365,
       0,   340,   341,   342,   343,   344,     0,     0,   345,   366,
     367,   348,     0,   349,   350,   351,     0,   352,     0,   353,
     354,   340,   341,   342,   343,     0,     0,     0,     0,     0,
       0,   348,     0,   349,   350,   351,     0,   352,     0,   353,
     354,     0,     0,     0,     0,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,   367,     0,   342,   343,     0,   361,   362,
     363,   364,   365,   348,     0,   349,   350,   351,     0,   352,
       0,     0,   366,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   367
};

static const yytype_int16 yycheck[] =
{
      11,    69,   336,    55,   324,   212,   369,   214,    59,   216,
     197,   190,   195,   240,    48,    29,    28,    41,   176,   177,
     178,    41,    17,   203,    17,    27,    99,   467,   120,   120,
     120,   103,   110,   126,     0,    98,   120,   153,   101,     5,
     156,    27,   405,    99,    55,   127,    32,   157,     9,    60,
      27,   126,    99,   120,   127,    32,   126,    18,    24,    55,
      26,   126,    28,   156,    30,   155,   158,   158,   152,    35,
     126,   127,   126,   129,   156,    41,    90,    91,   153,   157,
     127,    47,   155,   153,   156,   126,   149,   150,   114,   126,
     126,   156,     4,   126,     6,   120,   120,   537,   120,   153,
     120,   150,   120,   126,   138,   285,   120,   120,   120,   158,
     151,   127,   123,    99,   100,   149,   153,   153,   151,   105,
     172,   173,   174,   100,   176,   177,   178,   179,   151,   153,
     132,   126,    27,   126,   134,   122,   123,    32,   114,   129,
     116,   127,   128,   155,   196,   126,   132,    27,   150,   157,
     202,   128,    32,   126,   155,   132,   156,   134,   160,   135,
     127,   172,   173,   174,   150,   176,   177,   178,   179,   532,
     156,   156,   120,   150,   160,   227,   172,   173,   174,   337,
     176,   177,   178,   160,   150,   196,   238,   239,   152,   151,
     156,   202,   375,   372,   156,   120,   114,   151,   151,   379,
     196,   428,   156,   156,   120,   100,   202,   122,   123,   124,
      27,   120,   129,   576,   534,    32,   227,   120,   157,    99,
     100,   541,    29,   114,   275,   116,   185,   238,   239,    29,
      27,   227,   284,   128,   156,    32,   158,   132,   152,   122,
     251,   604,   238,   239,   203,    27,   298,   127,   128,   157,
      32,   159,   132,   440,   322,   150,   308,   133,   438,   466,
     156,   156,   158,   315,   316,   160,   318,   156,   133,   158,
     150,   133,   156,   284,   158,   152,   133,   236,   237,   133,
     160,   240,   133,   100,   133,   129,    99,   298,   284,   248,
     249,   250,   105,   252,   253,   120,   255,   308,   120,   157,
     120,   635,   298,   100,   315,   316,   153,   318,   132,   151,
     133,   128,   152,   126,   127,   132,   129,   134,   100,   315,
     316,   152,   318,   133,   152,   133,   285,    38,   103,   150,
     156,   128,   134,   150,   152,   132,   152,   134,   134,   134,
     120,   156,   120,   160,   120,   126,   128,   120,   122,   126,
     132,   156,   134,   150,   158,   314,   114,    31,   317,    17,
     319,   548,   549,   160,   323,   324,   156,   156,   150,   437,
     557,   551,    12,    13,   120,   120,   120,   336,   160,    11,
     152,   340,   341,   151,   571,   344,   345,   346,   347,   156,
     349,   156,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   156,   367,   592,
     151,   151,    27,    27,   151,   129,   114,    32,    32,   157,
     379,   608,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,    34,   134,   151,
     153,   151,   629,    27,    44,   151,   151,   406,    32,   328,
      27,    49,   653,    47,    60,    32,   257,   416,   612,   646,
     647,     1,   281,   308,   569,   424,   227,   380,   427,   428,
      -1,   523,   431,    -1,   114,   289,   435,   436,   118,   438,
     120,   121,   441,   442,    -1,   100,   100,   446,    27,    27,
     540,   324,    -1,    32,    32,    -1,    -1,    27,    -1,    -1,
      -1,   569,    32,    -1,    27,    -1,    -1,    -1,    -1,    32,
      -1,    -1,   523,   128,   128,    -1,   100,   132,   132,   134,
      -1,    -1,    -1,   100,    27,    -1,    -1,   523,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   150,   150,    -1,   152,    -1,
      -1,    -1,   501,    -1,   128,   160,   160,    -1,   132,    -1,
     134,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,    -1,
      -1,   100,   100,    -1,    -1,    -1,   150,    -1,   527,   528,
     100,   530,    -1,   150,   533,    -1,   160,   100,    -1,   538,
      -1,   540,    -1,   160,    -1,    -1,    -1,   639,    -1,   128,
     128,    -1,   551,   132,   132,   134,   134,   100,   128,    -1,
      -1,    -1,   132,    -1,    -1,   128,    -1,    -1,    -1,   132,
      -1,   150,   150,    -1,    -1,   574,    -1,    -1,   577,    -1,
     150,   160,   160,   153,    -1,   128,    -1,   150,   639,   132,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
      -1,    90,    91,   639,    -1,    -1,    -1,   150,    -1,    98,
     153,   100,   101,   102,    -1,   104,   615,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   624,   625,    -1,   627,    -1,
      -1,   630,    -1,     1,    -1,    -1,   635,     5,    -1,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,   137,   138,
     139,    -1,    20,    21,    22,    23,    -1,   656,    -1,    -1,
     149,   150,    -1,    -1,    32,    33,    -1,    -1,    36,    37,
      -1,    39,    -1,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    49,    50,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    90,    91,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,   144,   149,   150,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,   159,     1,    -1,    -1,    -1,     5,    -1,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    36,    37,    -1,
      39,    -1,    41,    42,    43,    -1,    45,    46,    -1,    -1,
      49,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,    -1,
      -1,   140,    -1,    -1,   143,   144,    12,    13,    14,    15,
      -1,    -1,    -1,   152,    -1,    21,   155,   156,   157,   158,
     159,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    90,    91,    -1,    -1,    -1,    98,
      -1,    -1,   101,   102,   100,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,    -1,   135,
     136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,    -1,
     149,   150,    -1,    -1,   150,    -1,   152,    -1,    -1,   155,
      -1,   157,    -1,   159,   160,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    12,    13,    14,
      15,   128,    -1,    -1,    -1,   132,    21,    -1,   135,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,   144,    33,    -1,
      -1,    -1,    37,   150,    39,   152,    -1,    42,   155,    -1,
     157,    46,   159,   160,    -1,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,
     135,   136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,
      33,    -1,    -1,    -1,    37,   150,    39,   152,   153,    42,
     155,    -1,   157,    46,   159,    -1,    -1,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,   137,    -1,    -1,   140,    -1,    -1,
     143,   144,    12,    13,    14,    15,    -1,    -1,    -1,   152,
      -1,    21,   155,    -1,   157,    -1,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    51,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,   135,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,   144,    33,    -1,    -1,    -1,    37,
      -1,    39,   152,    -1,    42,   155,   156,   157,    46,   159,
      -1,    -1,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,   144,    12,    13,    14,
      15,    -1,    -1,   151,   152,    -1,    21,   155,    -1,   157,
      -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
     135,   136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,
      33,    -1,    -1,    -1,    37,    -1,    39,   152,   153,    42,
     155,    -1,   157,    46,   159,    -1,    -1,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,   135,   136,   137,    -1,    -1,   140,    -1,    -1,
     143,   144,    33,    -1,    -1,    -1,    37,    -1,    39,   152,
     153,    42,   155,    -1,   157,    46,   159,    -1,    -1,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,   135,   136,   137,    -1,    -1,   140,
      -1,    -1,   143,   144,    33,    -1,    -1,    -1,    37,    -1,
      39,   152,   153,    42,   155,    -1,   157,    46,   159,    -1,
      -1,    -1,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,   135,   136,   137,    -1,
      -1,   140,    -1,    -1,   143,   144,    33,    -1,    -1,    -1,
      37,    -1,    39,   152,   153,    42,   155,    -1,   157,    46,
     159,    -1,    -1,    -1,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,   135,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,   144,    33,    -1,
      -1,    -1,    37,    -1,    39,   152,   153,    42,   155,    -1,
     157,    46,   159,    -1,    49,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
     135,   136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,
      33,    -1,    -1,    -1,    37,    -1,    39,   152,    -1,    42,
     155,    -1,   157,    46,   159,    -1,    -1,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,    -1,   135,   136,   137,    -1,    -1,   140,    -1,    -1,
     143,   144,    33,    -1,    -1,    -1,    37,    -1,    39,   152,
     153,    42,   155,    -1,   157,    46,   159,    -1,    -1,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,   135,   136,   137,    -1,    -1,   140,
      -1,    -1,   143,   144,    33,    -1,    -1,    -1,    37,    -1,
      39,   152,    -1,    42,   155,    -1,   157,    46,   159,    -1,
      -1,    -1,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,   135,   136,   137,    25,
      -1,   140,    -1,    -1,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    40,    -1,   155,    -1,   157,    -1,
     159,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,   111,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    -1,   152,    -1,    -1,   155,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     149,   150,    -1,    -1,    -1,    -1,    -1,   156,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,    -1,    -1,   156,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,    -1,
      -1,   156,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,    -1,    -1,   156,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,   149,
     150,    95,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    88,    89,    90,
      91,    92,    -1,   157,    95,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      88,    89,    90,    91,    92,   156,    -1,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,   153,    88,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,   153,    88,    89,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,    -1,   153,    88,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,   153,    88,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,   153,
      88,    89,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,   153,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,    88,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,    -1,   106,   107,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
      88,    89,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,    88,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,    -1,   106,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,   149,   150,    95,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,   149,   150,    95,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,   149,   150,    95,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,    -1,
     106,   107,    88,    89,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,    -1,
     106,   107,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,   149,   150,    95,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,    -1,   106,   107,    88,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    88,    89,    90,    91,    92,    -1,    -1,    95,   149,
     150,    98,    -1,   100,   101,   102,    -1,   104,    -1,   106,
     107,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,    -1,    90,    91,    -1,   135,   136,
     137,   138,   139,    98,    -1,   100,   101,   102,    -1,   104,
      -1,    -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   162,     0,     5,    24,    26,    28,    30,    35,    41,
      47,   150,   156,   163,   168,   169,   173,   188,   192,   205,
     230,   233,   234,   237,   120,   183,   184,   114,   174,   175,
     120,   170,   171,   120,   120,   120,    28,   120,   155,   185,
     186,   187,   197,     4,     6,   157,   127,   126,   129,   126,
      48,   138,   149,   172,   157,   127,   156,   155,   120,   152,
     126,   151,   120,   236,    29,    90,    91,   120,   191,   229,
      12,    13,   114,   118,   120,   121,   165,   183,   114,   175,
     120,   120,   232,    19,    25,    40,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,   197,   239,   240,   241,   242,
     120,   184,   186,   129,   235,   157,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   106,   107,   108,   109,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   143,   144,
      29,    29,   152,   189,   120,   158,   228,   238,   122,   164,
     120,   158,   133,   133,   133,   152,   133,   133,   133,   133,
      27,    32,   100,   128,   132,   150,   160,   153,   197,   220,
      41,   153,   221,   222,   226,   238,   129,   190,    99,   105,
     126,   127,   129,   227,   122,   123,   127,   156,   242,   242,
     242,   120,   189,   242,   189,   242,   189,   242,   223,   224,
     242,    27,   132,   150,   160,    12,    13,    14,    15,    21,
      33,    37,    39,    42,    46,    74,    90,    91,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   121,   135,   136,
     137,   140,   143,   144,   151,   152,   157,   159,   167,   193,
     197,   199,   207,   208,   211,   212,   214,   215,   216,   239,
     243,   246,   251,   253,   255,   150,   158,   217,   219,   226,
     153,   156,    99,   127,   129,   225,   242,   193,   120,   242,
     196,   213,   216,   114,   116,   135,   231,   134,   156,   134,
     153,   190,   134,   190,   134,   190,   134,   134,   156,   132,
     151,   242,   243,   152,   152,   133,   133,   152,   133,   152,
     216,   216,    10,   242,   242,   216,   248,   252,   166,   216,
     216,   216,   197,   216,   216,   216,   195,   188,   120,   152,
      88,    89,    90,    91,    92,    95,    96,    97,    98,   100,
     101,   102,   104,   106,   107,   128,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   149,   150,   151,   152,
     184,    38,   218,   156,   221,   242,   196,   216,   156,   227,
     103,    80,    81,    82,    83,    84,    85,    86,    87,    93,
      94,    99,   105,   108,   109,   127,   156,   114,   116,   156,
     242,   134,   134,   134,   223,   152,   120,   216,   242,   242,
     216,   242,   153,   216,   238,   120,   152,   216,   244,   245,
     247,   248,   249,   250,   152,   245,   250,   110,   156,   158,
     123,   124,   164,   153,     1,     7,     8,    10,    20,    22,
      23,    32,    36,    43,    45,    49,    50,   156,   158,   176,
     177,   179,   180,   181,   182,   194,   196,   198,   200,   201,
     202,   203,   205,   206,   213,   216,   189,   150,   153,   209,
     216,   216,   216,   216,   216,   216,   216,   120,   216,   120,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   120,   216,   153,   209,   151,
     226,   225,   196,   216,   211,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     134,   153,   209,    16,   133,   216,   153,   134,   134,   153,
     134,   153,    17,   225,   153,   126,   151,   156,   126,   151,
     156,   153,   158,   158,   216,   248,   158,   216,   216,   216,
     238,    99,   156,   196,   216,   193,   216,   216,   156,   114,
      49,   216,    31,   156,   156,   156,   156,   156,    17,   204,
     156,   190,   120,   244,   126,   153,   152,   129,   151,   153,
     216,   156,   153,   242,   120,   153,   216,   216,   216,   209,
     216,   245,   120,   151,   244,   216,   151,   249,   245,   151,
     151,   125,   193,   193,    17,   196,   216,   156,    11,   193,
     129,   114,   157,    34,   228,   110,   193,   210,   151,   216,
     153,   209,   216,   153,   134,   156,   151,   225,   158,     9,
      18,   178,   209,   156,   193,   195,   216,   153,   153,    16,
     216,   216,   151,   216,   151,   193,   216,   156,   158,   242,
     153,   156,   254,   193,   193,   153,    44,   151,   178,   216,
     151
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   161,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   163,   164,   164,   165,   166,   166,   166,
     167,   168,   169,   170,   170,   170,   171,   172,   172,   173,
     174,   174,   175,   175,   176,   177,   177,   178,   178,   178,
     179,   180,   181,   182,   183,   183,   183,   183,   183,   183,
     184,   184,   185,   185,   186,   186,   187,   187,   188,   188,
     189,   189,   189,   190,   190,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   192,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   195,   195,   195,   196,   197,   197,   197,   198,   199,
     199,   199,   199,   200,   201,   202,   202,   202,   202,   202,
     203,   204,   204,   205,   205,   206,   207,   207,   207,   208,
     208,   208,   208,   209,   209,   210,   210,   211,   211,   212,
     212,   212,   212,   212,   212,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   215,   215,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   220,   220,   221,
     221,   222,   222,   223,   224,   224,   225,   225,   226,   226,
     226,   226,   226,   227,   227,   227,   228,   228,   228,   228,
     228,   229,   229,   230,   231,   231,   231,   231,   232,   232,
     232,   233,   234,   235,   235,   236,   237,   238,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   240,   241,   241,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   243,   243,   243,   244,   244,
     245,   245,   246,   246,   246,   246,   247,   247,   248,   249,
     249,   250,   250,   251,   251,   252,   252,   253,   254,   254,
     255
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
       4,     3,     5,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     0,     3,     0,     1,     3,     0,     3,     1,
       2,     1,     3,     1,     1,     3,     1,     1,     1,     3,
       5,     3,     3,     1,     1,     1,     4,     6,     5,     4,
       3,     0,     2,     4,     1,     2,     1,     2,     0,     3,
       5,     4,     5,     0,     2,     2,     6,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     1,     4,
       3,     4,     2,     3,     2,     4,     3,     2,     3,     2,
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
          case 120: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2594 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2600 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2606 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2612 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2618 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2624 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expression_label  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2630 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expression_goto  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2636 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2642 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2648 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2654 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2660 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2666 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2672 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2678 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2684 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2690 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2696 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2702 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2708 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2714 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2720 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2726 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2732 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2738 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2744 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2750 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2756 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2762 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2768 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2774 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2780 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2786 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2792 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2798 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2804 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2810 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* block_or_simple_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2816 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2822 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2828 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2834 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2840 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2846 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2852 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2858 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2864 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2870 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2876 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2882 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2888 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2894 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2900 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2906 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2912 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2918 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2924 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2930 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2936 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2942 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2948 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2954 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2960 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 245: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2966 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2972 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2978 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2984 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2990 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2996 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3002 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3008 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3014 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3020 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3026 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 371 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 383 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 384 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 388 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 392 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3349 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 396 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 405 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3373 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 413 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 429 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 454 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 457 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 463 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 472 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 485 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 486 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 499 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 502 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 508 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 514 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3511 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 525 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 551 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 572 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3596 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 573 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 574 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 575 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3624 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 592 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3640 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 604 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3672 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 616 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 620 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3690 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 627 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3696 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 628 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 632 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 633 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 649 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3747 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3789 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3819 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3825 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 668 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3843 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 669 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3849 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 670 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 671 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 672 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3867 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 673 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 674 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 675 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 676 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 677 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 678 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 679 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 680 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 682 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 683 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 684 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3939 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 686 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 687 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 688 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 693 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 784 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 794 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4170 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 804 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4178 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 810 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 825 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4211 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 831 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 835 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 841 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4241 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 847 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 857 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 865 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4314 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 936 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 944 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 952 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 961 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 965 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 973 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 982 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4469 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 983 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4481 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 993 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1050 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1051 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1052 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1053 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1055 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1059 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1060 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1061 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1062 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1063 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1064 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1065 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4622 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1066 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1067 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1068 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4640 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1069 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1070 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1089 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1094 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1108 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1109 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4746 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1111 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1116 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1117 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1118 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1119 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1122 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1123 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1125 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1126 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1127 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1128 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1129 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1130 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4884 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4890 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4896 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4902 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1137 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1138 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1139 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1140 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1141 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1142 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1146 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1150 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1156 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4978 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1158 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1159 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1160 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1161 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1162 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5016 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1165 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5022 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1166 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5028 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1167 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1168 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1169 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1170 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1171 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1181 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1191 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5124 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5130 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5152 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5160 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5169 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5193 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1287 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1291 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5263 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1297 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5274 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5320 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5332 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5344 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1342 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1345 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5361 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1352 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5392 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1382 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = -(yyvsp[0].i); }
#line 5404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = int32_t((yyvsp[0].ui)); }
#line 5410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1384 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = -int32_t((yyvsp[0].ui)); }
#line 5416 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1388 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1391 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1400 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5452 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1412 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1424 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1443 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1482 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1548 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1555 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1564 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1565 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1566 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1567 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1568 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1569 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1573 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1574 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1576 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1578 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1579 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1580 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1581 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1583 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5746 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1584 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1585 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1586 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1587 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1591 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1602 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1606 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5804 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5816 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1619 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1633 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1639 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1643 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1648 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1653 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1658 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1665 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1670 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 5922 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1674 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1679 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1684 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1691 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1696 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5975 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1702 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5985 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1707 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1711 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6004 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6018 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1729 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1734 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1743 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1747 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6070 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1752 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1761 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6095 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1777 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1784 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6152 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6161 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1805 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1810 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1816 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1834 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1837 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1865 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1870 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6283 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1882 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1893 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1898 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1905 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1917 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6335 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1918 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6341 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1922 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6359 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6363 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1937 "ds_parser.ypp" /* yacc.c:1903  */


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


