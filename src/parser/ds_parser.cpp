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

#line 362 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   4823

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  378
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  672

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   390

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
       0,   360,   360,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   375,   387,   388,   392,   396,   400,   409,
     417,   433,   454,   458,   461,   467,   476,   489,   490,   494,
     498,   499,   503,   506,   512,   518,   521,   527,   528,   529,
     536,   543,   555,   565,   574,   575,   576,   577,   578,   579,
     583,   588,   596,   597,   598,   599,   600,   601,   602,   603,
     609,   622,   638,   639,   643,   650,   662,   666,   673,   674,
     678,   679,   680,   684,   688,   695,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
     723,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   734,   738,   808,   811,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,   840,   844,   850,   856,   870,   871,   877,   881,   887,
     890,   893,   897,   903,   907,   911,   914,   917,   922,   925,
     933,   939,   940,   944,   945,   949,   982,   985,   990,   998,
    1002,  1007,  1011,  1019,  1022,  1028,  1029,  1039,  1044,  1096,
    1097,  1098,  1099,  1100,  1101,  1105,  1106,  1107,  1108,  1109,
    1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,
    1120,  1124,  1135,  1140,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,
    1186,  1187,  1188,  1192,  1196,  1199,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1211,  1214,  1215,  1216,  1217,  1218,  1219,
    1220,  1230,  1240,  1274,  1275,  1279,  1280,  1284,  1292,  1295,
    1302,  1303,  1307,  1308,  1312,  1318,  1319,  1323,  1324,  1328,
    1333,  1336,  1340,  1346,  1355,  1356,  1357,  1361,  1364,  1369,
    1374,  1381,  1391,  1394,  1401,  1430,  1433,  1442,  1454,  1466,
    1480,  1481,  1485,  1524,  1590,  1597,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,
    1620,  1621,  1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,
    1633,  1644,  1648,  1658,  1659,  1660,  1661,  1675,  1681,  1685,
    1690,  1695,  1700,  1707,  1712,  1716,  1720,  1725,  1730,  1737,
    1742,  1748,  1753,  1757,  1762,  1771,  1775,  1780,  1789,  1793,
    1798,  1807,  1816,  1817,  1818,  1819,  1823,  1830,  1839,  1844,
    1851,  1856,  1862,  1869,  1880,  1883,  1897,  1906,  1907,  1911,
    1916,  1923,  1928,  1939,  1944,  1951,  1963,  1964,  1968
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
     375,   376,   377,   378,   379,   380,   381,   382,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,    64,   383,   384,   126,    33,   385,   386,   387,
     388,    46,    91,    93,    40,    41,   389,   390,    59,   123,
     125,    36,    35
};
# endif

#define YYPACT_NINF -454

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-454)))

#define YYTABLE_NINF -176

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -454,    29,  -454,  -454,   -81,   -57,   -48,   -18,    26,  -454,
      61,    -7,  -454,  -454,  -454,  -454,  -454,   143,  -454,   -58,
    -454,  -454,  -454,  -454,   -15,  -454,    31,    32,    75,  -454,
    -454,   -21,  -454,    47,    94,    67,  -454,    70,   102,    76,
    -454,   -71,  -454,   110,    -6,  -454,     2,   -81,   117,   -57,
    -454,   113,   115,  -454,  -454,  3369,  -454,   121,  -454,   -81,
      -7,  -454,   116,    90,  4677,   221,   223,  -454,   105,   -92,
    -454,  -454,  -454,  -454,  -454,   142,   315,  -454,  -454,  -454,
    -454,  -454,  -454,   -91,   132,   133,   134,  -454,  -454,  -454,
     119,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,   135,  -454,  -454,  -454,   140,
     141,  -454,  -454,  -454,  -454,   151,  -454,  -454,  -454,  -454,
    4596,  -454,   -76,  -454,   -85,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,   -10,   146,  -454,  -454,  -454,   -65,  -454,
      96,  -454,  -454,  -454,  -454,  -454,  -454,  -454,   -45,   -74,
    -454,  3369,  3369,  3369,   156,  3299,  3299,  3299,  3369,  -454,
      74,  -454,  -454,  -454,  -454,  1565,  -454,  -454,  -454,   -72,
     165,  -454,  -454,   -47,  -454,    -8,  3369,   129,  -454,  -454,
     170,  -454,  3369,  2605,  -454,  -454,   315,  -454,  2605,  -454,
     927,  3921,  4448,   139,   146,  4461,   146,  4475,   146,  4510,
    -454,   -96,  4596,   162,  -454,   152,  -454,  -454,  -454,  3229,
     145,  -454,   154,   171,   175,   164,   184,   167,  2605,  2605,
    2719,  3369,  2605,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    2605,  2605,  2605,   -85,  2605,  2605,  -454,  2605,  -454,   172,
    -454,  -454,   -84,  -454,  -454,  -454,  -454,  -454,  -454,  -454,
    3767,   168,  -454,  -454,  -454,  -454,  -454,   -81,  -454,   287,
     174,  -454,  -454,   -22,  -454,  -454,  3369,  2605,  4596,  -454,
    -454,   182,  -454,   238,  2720,  -454,  3044,  -454,  3369,  -454,
    -454,   209,  -454,   210,  -454,   237,  -454,  -454,  3369,  -454,
    -454,   179,  -454,   235,  2605,  3369,  3369,  2605,  3369,  1701,
     517,   517,   165,   954,  1090,  4058,  -454,    22,    43,   517,
     517,  -454,  -454,   517,   517,  3367,   643,   105,  -454,  1203,
     360,  2605,  2605,  -454,  -454,  2605,  2605,  2605,  2605,   255,
    2605,   256,  2605,  2605,  2605,  2605,  2605,  2605,  2605,  2605,
    2605,  2605,  2605,  2605,  2605,  2605,  2605,   258,  2605,  -454,
    1814,    36,  -454,   165,  -454,  -454,  3780,  -454,  3908,  -454,
    2605,    41,  2605,  2605,  2605,  2605,  2605,  2605,  2605,  2605,
    2605,  2605,  2605,  2605,  2605,  2605,  2605,  -454,  -454,  4531,
    -454,  -454,  -454,  -454,  1927,  1316,  3437,  4540,  4552,  3503,
    4573,  -454,  3569,    10,   -68,  2040,  4058,   253,  -110,   254,
    -454,  -454,   -61,  2153,    34,    53,  2605,  2605,   224,  -454,
    2605,   259,  -454,  -454,  2605,  2605,   165,  1452,  -454,   129,
    2605,  2605,  -454,   227,   271,  2266,  -454,   374,   249,   250,
    -454,  -454,  -454,  -454,  -454,  -454,   252,   257,   260,  -454,
    -454,   395,  -454,   -80,  4002,   146,   292,  -454,   -38,  4250,
    3369,   198,   198,  4414,  4414,  4393,  4393,   262,   517,  -454,
     -53,   -53,   198,   198,  4126,  4306,  4327,   462,  4414,  4414,
       3,     3,   357,   357,   357,  -454,  3831,  -454,    -2,  -454,
    -454,  2605,  -454,  2807,  -454,  4250,  4250,  4250,  4250,  4250,
    4250,  4250,  4250,  4250,  4250,  4250,  4250,  4250,  4250,  4250,
    -454,  -454,    23,  3369,   295,  3635,  -454,  2605,  2605,  -454,
    2605,  -454,  2605,  2605,   292,   297,   267,   292,  2605,   269,
    2605,   292,   270,   272,  4250,  -454,  -454,  4182,   391,   391,
      18,  2605,  -454,  -454,  2894,   413,  4250,   391,  -454,   299,
     312,  4250,   280,  -454,  -454,  -454,  -454,  -454,   407,   165,
    -454,   -86,   -62,    37,  2605,  -454,  4596,  2379,  2605,  -454,
    -454,  4250,  -454,  -454,  4566,   308,  -454,  4414,  4414,  4414,
     -82,  4250,     4,   -62,  -454,   253,  4250,  -454,  -454,    59,
    -454,  -454,  -454,  -454,    66,  2605,  -454,  2981,  -454,   129,
    -454,  -454,  -454,  -454,  -454,  -454,  2605,  -454,  -454,   290,
    4250,  -454,    33,  4250,  -454,  2492,  2605,   300,  2605,   301,
     129,  2605,  -454,   -77,  -454,  -454,   804,  4250,  -454,  -454,
    3369,  3701,  3107,  -454,  4250,  -454,  -454,   391,   129,  -454,
    4589,  -454,   403,   302,    66,  -454,  -454,  2605,   303,  -454,
    4250,  -454
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
      66,     0,    62,     0,     0,   282,     0,     0,     0,     0,
      28,     0,     0,    26,   285,     0,    13,     0,   137,     0,
       0,    69,   290,     0,     0,     0,     0,    75,    70,     0,
      56,    57,    54,    55,    53,     0,     0,    52,    61,    33,
      31,    25,    24,     0,     0,     0,     0,   296,   316,   297,
     321,   298,   302,   303,   304,   305,   309,   310,   311,   312,
     313,   314,   315,   317,   318,   342,   301,   308,   319,   345,
     348,   299,   306,   300,   307,     0,   320,   323,   325,   324,
     288,   136,     0,    67,     0,   292,   258,    78,    79,    81,
      80,    82,    83,    84,    85,   104,   105,   102,   103,    95,
     106,   107,    96,    93,    94,   108,   109,   110,   111,    98,
      99,    97,    91,    92,    87,    86,    88,    89,    90,    77,
      76,   100,   101,     0,    73,   294,   284,   283,     0,    14,
       0,    48,    49,    46,    47,    45,    44,    50,     0,     0,
     289,     0,     0,     0,     0,    70,    70,    70,     0,   329,
       0,   335,   338,   337,   331,     0,   334,    65,   291,   253,
       0,    71,   262,     0,   260,   269,     0,     0,   275,   276,
       0,   274,     0,     0,    15,    16,     0,    59,     0,   286,
       0,     0,     0,     0,    73,     0,    73,     0,    73,     0,
     265,     0,   264,   330,   333,     0,   336,   199,   200,     0,
       0,   194,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,   171,   170,   172,   173,   174,    17,
       0,     0,     0,     0,     0,     0,   327,     0,   131,    68,
     197,   167,   195,   246,   245,   244,   249,   196,   248,   247,
       0,     0,   198,   352,   353,   354,   355,     0,   293,   255,
       0,   261,    72,     0,   268,   267,     0,     0,    74,   112,
     295,     0,   281,     0,   175,    51,     0,   339,     0,   341,
     322,     0,   343,     0,   346,     0,   349,   351,     0,   332,
     328,   139,   142,     0,     0,     0,     0,     0,     0,     0,
     223,   224,     0,     0,     0,     0,   373,     0,     0,   204,
     203,   236,   231,   202,   201,     0,     0,    70,   252,     0,
       0,     0,     0,   225,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
       0,     0,   256,     0,   259,   263,   270,   273,   272,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,   287,     0,
     344,   347,   350,   266,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,   135,     0,   364,   358,     0,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,    20,
       0,    18,   227,   133,     0,     0,     0,     0,   143,     0,
       0,     0,   144,     0,     0,     0,   115,   113,     0,     0,
     126,   122,   120,   121,   132,   116,     0,     0,     0,   125,
     127,   151,   119,     0,   175,    73,     0,   232,     0,   163,
       0,   205,   206,   218,   219,   216,   217,     0,   241,   230,
     250,   251,   207,   208,     0,   221,   222,   220,   214,   215,
     210,   209,   211,   212,   213,   229,     0,   234,     0,   254,
     257,     0,   279,   175,   134,   182,   183,   185,   184,   186,
     179,   180,   181,   187,   188,   177,   178,   189,   190,   176,
     340,   140,     0,     0,     0,     0,   237,     0,     0,   238,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   374,   375,     0,     0,     0,
       0,     0,   145,   148,   175,     0,   138,     0,   130,     0,
       0,    36,     0,   128,   129,   118,   123,   124,     0,     0,
     117,     0,     0,     0,     0,   233,   243,     0,     0,   228,
     235,   271,   278,   141,     0,     0,   159,   156,   157,   158,
       0,   356,     0,     0,   360,   359,   365,   371,   370,     0,
     362,   372,    19,    42,    37,     0,   149,   175,   146,     0,
      43,    34,    35,   131,   152,   155,     0,   165,   168,     0,
     164,   192,     0,   242,   161,     0,     0,     0,     0,     0,
       0,     0,    40,     0,   147,   150,     0,   166,   191,   193,
       0,     0,   376,   361,   357,   363,    38,     0,     0,   114,
       0,   160,     0,     0,    37,    41,   162,     0,     0,    39,
     377,   378
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -454,  -454,  -454,   114,   412,  -454,  -454,  -454,  -454,  -454,
    -454,  -454,  -454,  -454,   415,  -454,  -454,  -202,  -454,  -454,
    -454,  -454,   251,  -454,   419,   -49,  -454,   408,  -454,   201,
    -168,  -220,  -454,  -454,  -154,  -454,  -152,  -206,   -11,  -454,
    -454,  -454,  -454,  -454,  -454,  -454,   471,  -454,  -454,  -454,
    -369,  -454,    82,  -454,  -343,  -454,  -454,    -4,  -454,  -454,
    -454,  -454,   181,  -454,   157,  -454,  -200,  -184,   176,  -103,
    -454,  -454,  -454,  -454,  -454,  -454,  -454,  -454,   -67,   -54,
    -454,  -454,   -46,   239,  -453,  -322,  -454,  -454,  -239,   -52,
     158,  -454,  -454,  -454,  -454,  -454
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   170,   176,   338,   270,    14,    15,    31,
      32,    53,    16,    28,    29,   458,   459,   642,   460,   461,
     462,   463,   177,   178,    25,    26,    39,    40,    41,    17,
     164,   207,    68,    18,   271,   464,   346,   465,   272,   466,
     273,   467,   468,   469,   470,   579,   471,   472,   274,   275,
     478,   628,   276,   277,   303,   278,   279,   479,   289,   383,
     290,   199,   202,   203,   230,   231,   543,   204,   213,   167,
      69,    20,    83,    21,    22,   125,    63,    23,   205,   281,
     118,   119,   232,   282,   427,   428,   283,   429,   430,   431,
     432,   284,   337,   285,   663,   286
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   117,   168,   473,   311,   297,   313,   302,   315,   120,
     122,   508,   434,   336,    70,    71,   291,   224,   226,   228,
     200,   350,    36,   583,    64,   391,   626,   542,    50,     2,
     165,   179,   200,   294,     3,   615,   208,    37,   348,   294,
     317,    24,   209,   546,   116,   532,   584,   359,   547,    42,
     361,   584,    47,   299,     4,   218,     5,    60,     6,    27,
       7,   295,   318,   210,   211,     8,   212,   295,   166,   180,
     349,     9,    38,   268,    30,   640,   636,    10,   580,   197,
     287,   658,    61,   216,   219,   641,    65,    66,   288,    57,
     584,   387,   549,   294,   605,   353,   354,   550,   377,   378,
     165,    45,   233,   359,    33,   360,   361,   362,   292,   363,
     217,   293,   165,   198,    46,    37,    67,   585,    72,    51,
     210,   295,    73,   296,    74,    75,   584,   117,   117,   117,
      52,   117,   117,   117,   117,   220,   221,   222,   210,   225,
     227,   229,   374,   375,   376,   201,   210,    43,    34,    44,
      38,   584,   117,   590,   377,   378,    76,   637,   117,    47,
     298,   584,   547,    48,    47,   545,   301,   169,   439,   440,
     116,   116,   116,   600,   116,   116,   116,   116,   593,   475,
     437,    11,   438,    35,   512,   117,   511,    12,   649,   509,
     629,   280,   547,   321,   552,   116,   117,   117,   555,   510,
     268,   116,   269,    49,   333,   334,    54,   189,   234,   304,
     189,   550,   190,   553,   306,   190,   350,   547,   632,   639,
     214,   215,   602,    55,    58,    56,   235,    57,   116,   609,
      59,   191,    62,    79,   191,    81,   236,    82,   381,   116,
     116,   563,   117,   121,   330,   331,   643,   124,   335,   126,
     386,   161,   342,   162,   117,   581,   339,   340,   341,   163,
     343,   344,   409,   345,   117,   423,   169,   181,   182,   183,
     185,   117,   117,   184,   117,   186,   187,   206,   223,   417,
     418,   192,   420,   208,   192,   116,   188,   165,   268,   209,
     353,   354,   300,   388,   310,   565,   319,   116,   359,   323,
     360,   361,   362,   473,   363,   320,   325,   116,   324,   193,
     326,   211,   193,   194,   116,   116,   194,   116,   327,   328,
     416,   329,   380,   419,    11,   422,   382,   171,   172,   426,
     426,   195,   384,   414,   195,   372,   373,   374,   375,   376,
     389,   196,   474,   391,   196,   410,   411,   481,   482,   377,
     378,   483,   484,   485,   486,   616,   488,   415,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   412,   506,   350,   480,   487,   489,   560,
     505,   545,   548,   214,   556,   568,   513,   569,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   638,   613,   614,   572,   573,   574,   350,
     575,   535,   578,   620,   582,   576,   587,   595,   577,   603,
     604,   345,   607,   610,   619,   611,   117,   627,   622,   345,
     621,   173,   554,   335,   586,   174,   557,   175,    75,   623,
     558,   559,   624,   564,   635,   648,   566,   567,   667,   353,
     354,   571,   441,   653,   655,   668,   671,   359,    77,   360,
     361,   362,   669,   363,    80,   645,    78,   305,   123,   116,
     347,   646,    19,   514,   385,   413,   625,   390,   322,   117,
     350,   351,   352,   353,   354,   355,   656,   594,   356,   357,
     358,   359,   435,   360,   361,   362,     0,   363,   608,   364,
     365,     0,     0,   664,   665,     0,     0,   591,   377,   378,
       0,     0,   168,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   116,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   597,   598,   350,   599,     0,     0,   601,
       0,     0,   377,   378,   606,     0,   426,     0,     0,     0,
     268,     0,   351,   352,   353,   354,   355,   617,     0,   356,
     357,   358,   359,     0,   360,   361,   362,     0,   363,     0,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,     0,     0,   633,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   117,   370,   371,   372,
     373,   374,   375,   376,   660,     0,     0,     0,     0,   353,
     354,     0,     0,   377,   378,     0,     0,   359,     0,     0,
     361,   362,   647,   363,     0,     0,     0,     0,     0,     0,
       0,   651,   652,     0,   654,     0,     0,   657,     0,   116,
       0,     0,   474,     0,   443,     0,     0,     0,     3,     0,
     444,   445,     0,   446,     0,   237,   238,   239,   240,     0,
       0,     0,     0,   670,   447,   241,   448,   449,   377,   378,
       0,     0,     0,     0,     0,     0,   450,   242,     0,     0,
     451,   243,     0,   244,     0,     9,   245,   452,     0,   453,
     246,     0,     0,   454,   455,     0,    87,    88,    89,     0,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,     0,   106,   107,   108,     0,   247,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   248,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,    37,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,   261,   262,     0,     0,   263,     0,     0,   264,   265,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
      38,   456,   268,   457,   269,   443,     0,     0,     0,     3,
       0,   444,   445,     0,   446,     0,   237,   238,   239,   240,
       0,     0,     0,     0,     0,   447,   241,   448,   449,     0,
       0,     0,     0,     0,     0,     0,     0,   450,   242,     0,
       0,   451,   243,     0,   244,     0,     9,   245,   452,     0,
     453,   246,     0,     0,   454,   455,     0,    87,    88,    89,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     0,   106,   107,   108,     0,
     247,   111,   112,   113,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,    37,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,   261,   262,     0,     0,   263,     0,     0,   264,
     265,     0,     0,     0,     0,   189,     0,     0,   267,     0,
     190,    38,   456,   268,   659,   269,   237,   238,   239,   240,
       0,     0,     0,     0,     0,     0,   241,     0,     0,   191,
       0,     0,   189,     0,     0,     0,     0,   190,   242,     0,
       0,     0,   243,     0,   244,     0,     0,   245,     0,     0,
       0,   246,     0,     0,     0,     0,   191,    87,    88,    89,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     0,   106,   107,   108,   192,
     247,   111,   112,   113,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,     0,     0,   192,   193,     0,     0,
       0,   194,     0,   307,     0,     0,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   424,   259,     0,   195,
       0,     0,     0,     0,   193,     0,     0,     0,   194,   196,
       0,   260,   261,   262,     0,     0,   263,     0,     0,   264,
     265,     0,   237,   238,   239,   240,   195,     0,   425,     0,
       0,    38,   241,   268,     0,   269,   196,     0,   189,     0,
       0,     0,     0,   190,   242,     0,     0,     0,   243,     0,
     244,     0,     0,   245,     0,     0,     0,   246,     0,     0,
       0,     0,   191,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
       0,     0,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   424,   259,     0,   237,   238,   239,   240,     0,
     193,     0,     0,     0,   194,   241,     0,   260,   261,   262,
       0,     0,   263,     0,     0,   264,   265,   242,     0,     0,
       0,   243,   195,   244,   433,     0,   245,    38,     0,   268,
     246,   269,   196,     0,     0,     0,    87,    88,    89,     0,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,     0,   106,   107,   108,     0,   247,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   248,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,    37,   259,     0,   237,   238,
     239,   240,   533,     0,     0,     0,     0,     0,   241,     0,
     260,   261,   262,     0,     0,   263,     0,     0,   264,   265,
     242,     0,     0,     0,   243,   476,   244,   267,   477,   245,
      38,     0,   268,   246,   269,     0,     0,     0,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   247,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   248,   249,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,    37,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,   260,   261,   262,     0,     0,   263,     0,
       0,   264,   265,     0,   237,   238,   239,   240,     0,     0,
     267,     0,     0,    38,   241,   268,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
     243,     0,   244,     0,     0,   245,     0,     0,     0,   246,
       0,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,     0,   561,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,    37,   259,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   260,
     261,   262,     0,     0,   263,     0,     0,   264,   265,   242,
       0,     0,     0,   243,     0,   244,   267,     0,   245,    38,
     562,   268,   246,   269,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,    37,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   260,   261,   262,     0,     0,   263,     0,     0,
     264,   265,     0,   237,   238,   239,   240,     0,   266,   267,
       0,     0,    38,   241,   268,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,     0,   243,
       0,   244,     0,     0,   245,     0,     0,     0,   246,     0,
       0,     0,     0,     0,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   247,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   248,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,    37,   259,     0,   237,   238,   239,   240,
       0,     0,     0,     0,     0,     0,   241,     0,   260,   261,
     262,     0,     0,   263,     0,     0,   264,   265,   242,     0,
       0,     0,   243,     0,   244,   267,   421,   245,    38,     0,
     268,   246,   269,     0,     0,     0,     0,    87,    88,    89,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     0,   106,   107,   108,     0,
     247,   111,   112,   113,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,    37,   259,     0,   237,
     238,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,   260,   261,   262,     0,     0,   263,     0,     0,   264,
     265,   242,     0,     0,     0,   243,     0,   244,   267,   507,
     245,    38,     0,   268,   246,   269,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   248,
     249,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,    37,
     259,     0,   237,   238,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,   260,   261,   262,     0,     0,   263,
       0,     0,   264,   265,   242,     0,     0,     0,   243,     0,
     244,   267,   531,   245,    38,     0,   268,   246,   269,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    37,   259,     0,   237,   238,   239,   240,     0,
       0,     0,     0,     0,     0,   241,     0,   260,   261,   262,
       0,     0,   263,     0,     0,   264,   265,   242,     0,     0,
       0,   243,     0,   244,   267,   544,   245,    38,     0,   268,
     246,   269,     0,     0,     0,     0,    87,    88,    89,     0,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,     0,   106,   107,   108,     0,   247,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   248,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,    37,   259,     0,   237,   238,
     239,   240,     0,     0,     0,     0,     0,     0,   241,     0,
     260,   261,   262,     0,     0,   263,     0,     0,   264,   265,
     242,     0,     0,     0,   243,     0,   244,   267,   551,   245,
      38,     0,   268,   246,   269,     0,   570,     0,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   247,   111,   112,   113,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   248,   249,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,    37,   259,
       0,   237,   238,   239,   240,     0,     0,     0,     0,     0,
       0,   241,     0,   260,   261,   262,     0,     0,   263,     0,
       0,   264,   265,   242,     0,     0,     0,   243,     0,   244,
     267,     0,   245,    38,     0,   268,   246,   269,     0,     0,
       0,     0,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   247,   111,   112,   113,   114,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   248,   249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,    37,   259,     0,   237,   238,   239,   240,   650,     0,
       0,     0,     0,     0,   241,     0,   260,   261,   262,     0,
       0,   263,     0,     0,   264,   265,   242,     0,     0,     0,
     243,     0,   244,   267,   631,   245,    38,     0,   268,   246,
     269,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,    37,   259,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   260,
     261,   262,     0,     0,   263,     0,     0,   264,   265,   242,
       0,     0,     0,   243,     0,   244,   267,     0,   245,    38,
       0,   268,   246,   269,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,    37,   259,   332,
       0,     0,     0,     0,     0,     0,     0,     0,   350,    84,
       0,     0,   260,   261,   262,    85,     0,   263,     0,     0,
     264,   265,     0,     0,     0,     0,     0,     0,     0,   267,
      86,     0,    38,     0,   268,     0,   269,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,   392,   393,   394,   395,   396,   397,   398,   399,
     351,   352,   353,   354,   355,   400,   401,   356,   357,   358,
     359,   402,   360,   361,   362,   350,   363,   403,   364,   365,
     404,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,   406,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,   378,     0,     0,     0,    38,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,   394,   395,   396,   397,   398,   399,   351,   352,   353,
     354,   355,   400,   401,   356,   357,   358,   359,   402,   360,
     361,   362,   350,   363,   403,   364,   365,   404,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   378,
       0,     0,     0,     0,     0,   592,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   393,   394,   395,
     396,   397,   398,   399,   351,   352,   353,   354,   355,   400,
     401,   356,   357,   358,   359,   402,   360,   361,   362,   350,
     363,   403,   364,   365,   404,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   378,     0,     0,     0,
       0,     0,   618,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,   392,   393,   394,   395,   396,   397,   398,
     399,   351,   352,   353,   354,   355,   400,   401,   356,   357,
     358,   359,   402,   360,   361,   362,     0,   363,   403,   364,
     365,   404,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,     0,     0,   350,     0,     0,     0,     0,
       0,     0,   377,   378,   351,   352,   353,   354,   355,   644,
       0,   356,   357,   358,   359,     0,   360,   361,   362,     0,
     363,     0,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   351,   352,   353,
     354,   355,   408,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,   363,     0,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,    84,
       0,     0,     0,     0,     0,    85,     0,     0,   377,   378,
       0,     0,     0,     0,     0,   662,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,   250,   251,   252,     0,     0,     0,     0,     0,
       0,    37,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,     0,     0,     0,   350,    38,     0,     0,    84,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,     0,   163,     0,   350,    38,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,   363,     0,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   378,
       0,   350,   442,     0,     0,     0,    38,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,   363,     0,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,   350,   377,   378,
       0,     0,   536,   351,   352,   353,   354,   355,     0,     0,
     356,   357,   358,   359,     0,   360,   361,   362,     0,   363,
       0,   364,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   377,   378,     0,     0,   539,   351,
     352,   353,   354,   355,     0,     0,   356,   357,   358,   359,
       0,   360,   361,   362,     0,   363,     0,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     377,   378,     0,     0,   541,   351,   352,   353,   354,   355,
       0,     0,   356,   357,   358,   359,     0,   360,   361,   362,
       0,   363,     0,   364,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,   350,   377,   378,     0,     0,
     596,   351,   352,   353,   354,   355,     0,     0,   356,   357,
     358,   359,     0,   360,   361,   362,     0,   363,   189,   364,
     365,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   191,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,     0,     0,     0,     0,     0,     0,   350,
       0,     0,   377,   378,     0,     0,   661,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,   363,     0,   364,   365,     0,     0,     0,
       0,   294,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   295,
     193,     0,     0,     0,   194,     0,     0,     0,   377,   378,
     379,   351,   352,   353,   354,   355,   350,     0,   356,   357,
     358,   359,   195,   360,   361,   362,     0,   363,     0,   364,
     365,     0,   196,     0,     0,     0,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,     0,     0,     0,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   191,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   589,     0,     0,     0,     0,     0,
     392,   393,   394,   395,   396,   397,   398,   399,   351,   352,
     353,   354,   355,   400,   401,   356,   357,   358,   359,   402,
     360,   361,   362,  -175,   363,   403,   364,   365,   404,   405,
     350,     0,     0,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
       0,   193,     0,     0,     0,   194,     0,     0,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   195,     0,     0,   350,     0,     0,   308,
       0,     0,     0,   196,   392,   393,   394,   395,   396,   397,
     398,   399,   351,   352,   353,   354,   355,   400,   401,   356,
     357,   358,   359,   402,   360,   361,   362,     0,   363,   403,
     364,   365,   404,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   350,     0,     0,     0,   351,   352,
     353,   354,   355,   377,   378,   356,   357,   358,   359,     0,
     360,   361,   362,     0,   363,     0,   364,   365,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     350,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,     0,     0,     0,     0,     0,   351,   352,   353,   354,
     355,     0,     0,   356,   357,   358,   359,     0,   360,   361,
     362,     0,   363,     0,   364,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   588,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   350,     0,
       0,     0,   351,   352,   353,   354,   355,   377,   378,   356,
     357,   358,   359,     0,   360,   361,   362,     0,   363,     0,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   612,
       0,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   350,     0,     0,     0,     0,     0,
       0,     0,     0,   377,   378,     0,     0,     0,     0,     0,
     351,   352,   353,   354,   355,   350,     0,   356,   357,   358,
     359,     0,   360,   361,   362,     0,   363,     0,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,     0,     0,     0,     0,   351,   352,   353,   354,
     355,   377,   378,   356,   357,   358,   359,     0,   360,   361,
     362,   350,   363,     0,   364,   365,     0,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,   350,   363,     0,   364,   365,     0,     0,   368,
     369,   370,   371,   372,   373,   374,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,     0,
       0,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,   189,     0,   377,   378,
       0,   190,     0,   351,   352,   353,   354,   355,     0,   189,
     356,     0,     0,   359,   190,   360,   361,   362,     0,   363,
     191,   364,   365,   189,   351,   352,   353,   354,   190,     0,
       0,     0,     0,   191,   359,     0,   360,   361,   362,     0,
     363,     0,   364,   365,     0,     0,     0,   191,   370,   371,
     372,   373,   374,   375,   376,     0,     0,     0,   189,     0,
       0,     0,     0,   190,   377,   378,     0,     0,     0,     0,
     192,   372,   373,   374,   375,   376,     0,     0,     0,   189,
       0,     0,   191,   192,   190,   377,   378,     0,   189,     0,
       0,     0,     0,   190,     0,     0,     0,   192,   193,     0,
     189,     0,   194,   191,   309,   190,     0,     0,     0,     0,
       0,   193,   191,     0,   189,   194,     0,   312,     0,   190,
     195,   189,     0,     0,   191,   193,   190,     0,     0,   194,
     196,   314,   192,   195,     0,     0,     0,   189,   191,     0,
       0,     0,   190,   196,   189,   191,     0,   195,     0,   190,
       0,     0,     0,   192,     0,     0,     0,   196,     0,     0,
     193,   191,   192,     0,   194,     0,   316,     0,   191,     0,
       0,     0,     0,     0,   192,     0,     0,     0,     0,     0,
       0,   193,   195,     0,     0,   194,     0,   530,   192,     0,
     193,     0,   196,     0,   194,   192,   537,     0,     0,     0,
       0,     0,   193,   195,     0,     0,   194,     0,   538,     0,
       0,   192,   195,   196,     0,     0,   193,     0,   192,     0,
     194,     0,   196,   193,   195,     0,     0,   194,     0,   540,
       0,     0,     0,     0,   196,     0,     0,     0,   195,   193,
       0,   634,     0,   194,     0,   195,   193,     0,   196,     0,
     194,     0,     0,     0,     0,   196,     0,     0,     0,     0,
       0,   195,     0,     0,   666,     0,     0,     0,   195,     0,
       0,   196,     0,     0,     0,     0,     0,     0,   196,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,     0,     0,     0,
       0,     0,     0,     0,     0,   145,   146,   147,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,     0,
       0,     0,   159,   160
};

static const yytype_int16 yycheck[] =
{
      11,    55,    69,   346,   224,   205,   226,   213,   228,    55,
      59,   380,   334,   252,    12,    13,   200,   185,   186,   187,
      42,    18,    29,   476,    30,   105,   112,    17,    49,     0,
     122,   122,    42,   101,     5,    17,   101,   122,   122,   101,
     136,   122,   107,   153,    55,   414,   128,   100,   158,    60,
     103,   128,   128,   207,    25,   129,    27,   128,    29,   116,
      31,   129,   158,   128,   129,    36,   131,   129,   160,   160,
     154,    42,   157,   159,   122,     9,   158,    48,   158,   155,
     152,   158,   153,   128,   158,    19,    92,    93,   160,   157,
     128,   297,   153,   101,   547,    92,    93,   158,   151,   152,
     122,   159,    28,   100,   122,   102,   103,   104,   155,   106,
     155,   158,   122,   124,   129,   122,   122,   155,   116,   140,
     128,   129,   120,   131,   122,   123,   128,   181,   182,   183,
     151,   185,   186,   187,   188,   181,   182,   183,   128,   185,
     186,   187,   139,   140,   141,   155,   128,     4,   122,     6,
     157,   128,   206,   155,   151,   152,   154,   153,   212,   128,
     206,   128,   158,   131,   128,   128,   212,   124,   125,   126,
     181,   182,   183,   542,   185,   186,   187,   188,   155,   347,
     158,   152,   160,   122,   390,   239,   386,   158,   155,   153,
     153,   195,   158,   239,   160,   206,   250,   251,   437,   383,
     159,   212,   161,   128,   250,   251,   159,    28,   134,   213,
      28,   158,    33,   160,   218,    33,    18,   158,   587,   160,
     124,   125,   544,   129,   122,   158,   152,   157,   239,   551,
     154,    52,   122,   116,    52,   122,   162,   122,   287,   250,
     251,   447,   296,   122,   248,   249,   615,   131,   252,   159,
     296,    30,   263,    30,   308,   475,   260,   261,   262,   154,
     264,   265,   308,   267,   318,   332,   124,   135,   135,   135,
     135,   325,   326,   154,   328,   135,   135,   131,   122,   325,
     326,   102,   328,   101,   102,   296,   135,   122,   159,   107,
      92,    93,   122,   297,   155,   449,   134,   308,   100,   154,
     102,   103,   104,   646,   106,   153,   135,   318,   154,   130,
     135,   129,   130,   134,   325,   326,   134,   328,   154,   135,
     324,   154,   154,   327,   152,   329,    39,    12,    13,   333,
     334,   152,   158,   154,   152,   137,   138,   139,   140,   141,
     158,   162,   346,   105,   162,   136,   136,   351,   352,   151,
     152,   355,   356,   357,   358,   561,   360,   122,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   136,   378,    18,    16,   122,   122,   446,
     122,   128,   128,   124,   160,   158,   390,   116,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   603,   558,   559,    32,   158,   158,    18,
     158,   415,    17,   567,   122,   158,   154,   122,   158,   122,
     153,   425,   153,   153,    11,   153,   480,   581,   116,   433,
     131,   116,   436,   437,   480,   120,   440,   122,   123,   159,
     444,   445,    35,   447,   136,   155,   450,   451,    45,    92,
      93,   455,   338,   153,   153,   153,   153,   100,    46,   102,
     103,   104,   664,   106,    49,   619,    47,   216,    60,   480,
     269,   623,     1,   391,   293,   318,   579,   301,   239,   533,
      18,    90,    91,    92,    93,    94,   640,   533,    97,    98,
      99,   100,   334,   102,   103,   104,    -1,   106,   550,   108,
     109,    -1,    -1,   657,   658,    -1,    -1,   511,   151,   152,
      -1,    -1,   579,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   533,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   537,   538,    18,   540,    -1,    -1,   543,
      -1,    -1,   151,   152,   548,    -1,   550,    -1,    -1,    -1,
     159,    -1,    90,    91,    92,    93,    94,   561,    -1,    97,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     584,    -1,    -1,    -1,   588,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   650,   135,   136,   137,
     138,   139,   140,   141,   650,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,   151,   152,    -1,    -1,   100,    -1,    -1,
     103,   104,   626,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   635,   636,    -1,   638,    -1,    -1,   641,    -1,   650,
      -1,    -1,   646,    -1,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,   667,    21,    22,    23,    24,   151,   152,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
     157,   158,   159,   160,   161,     1,    -1,    -1,    -1,     5,
      -1,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    44,    -1,
      46,    47,    -1,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    -1,    -1,    -1,    28,    -1,    -1,   154,    -1,
      33,   157,   158,   159,   160,   161,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    52,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    34,    -1,
      -1,    -1,    38,    -1,    40,    -1,    -1,    43,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    52,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,   102,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,   152,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,   162,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    12,    13,    14,    15,   152,    -1,   154,    -1,
      -1,   157,    22,   159,    -1,   161,   162,    -1,    28,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    12,    13,    14,    15,    -1,
     130,    -1,    -1,    -1,   134,    22,    -1,   137,   138,   139,
      -1,    -1,   142,    -1,    -1,   145,   146,    34,    -1,    -1,
      -1,    38,   152,    40,   154,    -1,    43,   157,    -1,   159,
      47,   161,   162,    -1,    -1,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
     137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,
      34,    -1,    -1,    -1,    38,   152,    40,   154,   155,    43,
     157,    -1,   159,    47,   161,    -1,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,    -1,    -1,   142,    -1,
      -1,   145,   146,    -1,    12,    13,    14,    15,    -1,    -1,
     154,    -1,    -1,   157,    22,   159,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      38,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,    34,
      -1,    -1,    -1,    38,    -1,    40,   154,    -1,    43,   157,
     158,   159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,
     145,   146,    -1,    12,    13,    14,    15,    -1,   153,   154,
      -1,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,
      -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
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
     159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   137,   138,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    34,    -1,    -1,    -1,    38,    -1,    40,   154,   155,
      43,   157,    -1,   159,    47,   161,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
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
     157,    -1,   159,    47,   161,    -1,    50,    -1,    -1,    53,
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
     154,    -1,    43,   157,    -1,   159,    47,   161,    -1,    -1,
      -1,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    12,    13,    14,    15,    16,    -1,
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
      -1,    -1,    -1,    38,    -1,    40,   154,    -1,    43,   157,
      -1,   159,    47,   161,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    20,
      -1,    -1,   137,   138,   139,    26,    -1,   142,    -1,    -1,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      41,    -1,   157,    -1,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    18,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,    -1,    -1,    -1,   157,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,    18,   106,   107,   108,   109,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    18,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,    90,    91,    92,    93,    94,   158,
      -1,    97,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,   135,
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
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   129,
     130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   151,   152,
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
      -1,   130,    -1,    -1,    -1,   134,    -1,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    18,    -1,    -1,   158,
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
     103,   104,    18,   106,    -1,   108,   109,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,   151,   152,
      -1,    33,    -1,    90,    91,    92,    93,    94,    -1,    28,
      97,    -1,    -1,   100,    33,   102,   103,   104,    -1,   106,
      52,   108,   109,    28,    90,    91,    92,    93,    33,    -1,
      -1,    -1,    -1,    52,   100,    -1,   102,   103,   104,    -1,
     106,    -1,   108,   109,    -1,    -1,    -1,    52,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,   151,   152,    -1,    -1,    -1,    -1,
     102,   137,   138,   139,   140,   141,    -1,    -1,    -1,    28,
      -1,    -1,    52,   102,    33,   151,   152,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,   102,   130,    -1,
      28,    -1,   134,    52,   136,    33,    -1,    -1,    -1,    -1,
      -1,   130,    52,    -1,    28,   134,    -1,   136,    -1,    33,
     152,    28,    -1,    -1,    52,   130,    33,    -1,    -1,   134,
     162,   136,   102,   152,    -1,    -1,    -1,    28,    52,    -1,
      -1,    -1,    33,   162,    28,    52,    -1,   152,    -1,    33,
      -1,    -1,    -1,   102,    -1,    -1,    -1,   162,    -1,    -1,
     130,    52,   102,    -1,   134,    -1,   136,    -1,    52,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   152,    -1,    -1,   134,    -1,   136,   102,    -1,
     130,    -1,   162,    -1,   134,   102,   136,    -1,    -1,    -1,
      -1,    -1,   130,   152,    -1,    -1,   134,    -1,   136,    -1,
      -1,   102,   152,   162,    -1,    -1,   130,    -1,   102,    -1,
     134,    -1,   162,   130,   152,    -1,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,   152,   130,
      -1,   155,    -1,   134,    -1,   152,   130,    -1,   162,    -1,
     134,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,    -1,    -1,    -1,   152,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,   162,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,   145,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   164,     0,     5,    25,    27,    29,    31,    36,    42,
      48,   152,   158,   165,   170,   171,   175,   192,   196,   209,
     234,   236,   237,   240,   122,   187,   188,   116,   176,   177,
     122,   172,   173,   122,   122,   122,    29,   122,   157,   189,
     190,   191,   201,     4,     6,   159,   129,   128,   131,   128,
      49,   140,   151,   174,   159,   129,   158,   157,   122,   154,
     128,   153,   122,   239,    30,    92,    93,   122,   195,   233,
      12,    13,   116,   120,   122,   123,   154,   167,   187,   116,
     177,   122,   122,   235,    20,    26,    41,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   201,   242,   243,   244,
     245,   122,   188,   190,   131,   238,   159,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   108,   109,   110,   111,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   145,
     146,    30,    30,   154,   193,   122,   160,   232,   241,   124,
     166,    12,    13,   116,   120,   122,   167,   185,   186,   122,
     160,   135,   135,   135,   154,   135,   135,   135,   135,    28,
      33,    52,   102,   130,   134,   152,   162,   155,   201,   224,
      42,   155,   225,   226,   230,   241,   131,   194,   101,   107,
     128,   129,   131,   231,   124,   125,   128,   155,   129,   158,
     245,   245,   245,   122,   193,   245,   193,   245,   193,   245,
     227,   228,   245,    28,   134,   152,   162,    12,    13,    14,
      15,    22,    34,    38,    40,    43,    47,    76,    92,    93,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   123,
     137,   138,   139,   142,   145,   146,   153,   154,   159,   161,
     169,   197,   201,   203,   211,   212,   215,   216,   218,   219,
     220,   242,   246,   249,   254,   256,   258,   152,   160,   221,
     223,   230,   155,   158,   101,   129,   131,   229,   245,   197,
     122,   245,   200,   217,   220,   185,   220,   136,   158,   136,
     155,   194,   136,   194,   136,   194,   136,   136,   158,   134,
     153,   245,   246,   154,   154,   135,   135,   154,   135,   154,
     220,   220,    10,   245,   245,   220,   251,   255,   168,   220,
     220,   220,   201,   220,   220,   220,   199,   192,   122,   154,
      18,    90,    91,    92,    93,    94,    97,    98,    99,   100,
     102,   103,   104,   106,   108,   109,   130,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   151,   152,   153,
     154,   188,    39,   222,   158,   225,   245,   200,   220,   158,
     231,   105,    82,    83,    84,    85,    86,    87,    88,    89,
      95,    96,   101,   107,   110,   111,   129,   158,   158,   245,
     136,   136,   136,   227,   154,   122,   220,   245,   245,   220,
     245,   155,   220,   241,   122,   154,   220,   247,   248,   250,
     251,   252,   253,   154,   248,   253,   112,   158,   160,   125,
     126,   166,   155,     1,     7,     8,    10,    21,    23,    24,
      33,    37,    44,    46,    50,    51,   158,   160,   178,   179,
     181,   182,   183,   184,   198,   200,   202,   204,   205,   206,
     207,   209,   210,   217,   220,   193,   152,   155,   213,   220,
      16,   220,   220,   220,   220,   220,   220,   122,   220,   122,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   122,   220,   155,   213,   153,
     230,   229,   200,   220,   215,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     136,   155,   213,    16,   135,   220,   155,   136,   136,   155,
     136,   155,    17,   229,   155,   128,   153,   158,   128,   153,
     158,   155,   160,   160,   220,   251,   160,   220,   220,   220,
     241,   101,   158,   200,   220,   197,   220,   220,   158,   116,
      50,   220,    32,   158,   158,   158,   158,   158,    17,   208,
     158,   194,   122,   247,   128,   155,   245,   154,   131,   153,
     155,   220,   158,   155,   245,   122,   155,   220,   220,   220,
     213,   220,   248,   122,   153,   247,   220,   153,   252,   248,
     153,   153,   127,   197,   197,    17,   200,   220,   158,    11,
     197,   131,   116,   159,    35,   232,   112,   197,   214,   153,
     220,   155,   213,   220,   155,   136,   158,   153,   229,   160,
       9,    19,   180,   213,   158,   197,   199,   220,   155,   155,
      16,   220,   220,   153,   220,   153,   197,   220,   158,   160,
     245,   155,   158,   257,   197,   197,   155,    45,   153,   180,
     220,   153
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   166,   166,   167,   168,   168,   168,
     169,   170,   171,   172,   172,   172,   173,   174,   174,   175,
     176,   176,   177,   177,   178,   179,   179,   180,   180,   180,
     181,   182,   183,   184,   185,   185,   185,   185,   185,   185,
     186,   186,   187,   187,   187,   187,   187,   187,   187,   187,
     188,   188,   189,   189,   190,   190,   191,   191,   192,   192,
     193,   193,   193,   194,   194,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   196,   197,   197,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   199,   199,   199,   200,   201,   201,   201,   202,   203,
     203,   203,   203,   204,   205,   206,   206,   206,   206,   206,
     207,   208,   208,   209,   209,   210,   211,   211,   211,   212,
     212,   212,   212,   213,   213,   214,   214,   215,   215,   216,
     216,   216,   216,   216,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   219,   219,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   221,   221,   222,   222,   223,   224,   224,
     225,   225,   226,   226,   227,   228,   228,   229,   229,   230,
     230,   230,   230,   230,   231,   231,   231,   232,   232,   232,
     232,   232,   233,   233,   234,   235,   235,   235,   236,   237,
     238,   238,   239,   240,   241,   241,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     243,   244,   244,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   246,   246,   246,   246,   247,   247,   248,   248,
     249,   249,   249,   249,   250,   250,   251,   252,   252,   253,
     253,   254,   254,   255,   255,   256,   257,   257,   258
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
       4,     3,     5,     4,     1,     1,     1,     1,     1,     1,
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
          case 122: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2650 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2656 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2662 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2668 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2674 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2680 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* expression_label  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2686 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* expression_goto  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2692 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2698 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2704 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2710 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2716 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2722 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* annotation_argument_value  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2728 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* annotation_argument_value_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2734 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2740 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2746 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2752 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2758 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2764 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2770 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2776 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2782 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2788 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2794 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2800 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2806 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2812 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2818 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2824 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2830 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2836 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2842 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2848 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2854 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2860 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2866 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2872 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2878 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* block_or_simple_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2884 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2890 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2896 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2902 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2908 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2914 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2920 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2926 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2932 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2938 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2944 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2950 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2956 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2962 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2968 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2974 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2980 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2986 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2992 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2998 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3004 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3010 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 245: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3016 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3022 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3028 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3034 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3040 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3046 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3052 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3058 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3064 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3070 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3076 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3082 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3088 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3094 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 375 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 387 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3396 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 388 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 396 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 400 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 409 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 417 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 433 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 458 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 461 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 467 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 476 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 489 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 490 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 503 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 506 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 512 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 518 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 521 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 529 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3600 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 536 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 543 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 565 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 574 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 575 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 576 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 578 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 588 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 596 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 598 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 599 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 600 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3732 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 601 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 602 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3744 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 603 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 609 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 622 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3844 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 673 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 674 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 678 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3862 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 679 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3868 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 680 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3874 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 684 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 688 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 695 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3901 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 699 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 700 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 701 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3919 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 702 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3925 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 703 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3931 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3943 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 706 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3949 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3955 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3961 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 709 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3967 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 710 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3973 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3979 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3985 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 723 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 724 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4057 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 726 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 727 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4075 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 728 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4081 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 730 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4093 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 732 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 733 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4188 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 808 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4196 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 811 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4208 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 821 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4214 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 822 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 823 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4226 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 825 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4238 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 826 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4244 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 827 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 828 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4256 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 829 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4262 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 830 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 831 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4274 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 832 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 833 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 835 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 836 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4313 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 850 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4332 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 856 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4365 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4371 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4395 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 922 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4458 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 925 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 933 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 940 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 944 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 945 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 949 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 982 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4543 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4572 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1002 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1011 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1022 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1028 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1029 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1039 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1045 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1096 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1098 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1099 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4722 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4728 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4746 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1108 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1109 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1111 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1116 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1117 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1118 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1119 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1140 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1150 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1151 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1154 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1155 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1156 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1158 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4918 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1159 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1160 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1161 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1162 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1163 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1164 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1165 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1166 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1167 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1168 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4978 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1169 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1170 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1171 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1172 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1173 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1174 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1175 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5020 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1176 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1177 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1178 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1179 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1180 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1181 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1182 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1183 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1184 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1185 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5080 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1199 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5170 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5178 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5184 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5190 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5196 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5202 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5208 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5214 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1220 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5229 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5244 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1280 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5314 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1292 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5331 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1302 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5337 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5343 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5349 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5355 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5397 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1346 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1355 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1357 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1361 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1364 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1369 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1391 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1401 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5554 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1430 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1433 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1442 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1454 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1466 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1481 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1485 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1590 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1597 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1606 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1607 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1608 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1609 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1610 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1611 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1612 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1613 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1614 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1615 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1619 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1620 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1621 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1622 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1623 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1624 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1625 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5884 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1626 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5890 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1627 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5896 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1628 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5902 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1629 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1633 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1644 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1648 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5979 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1675 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1681 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1685 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1690 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1695 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1700 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1707 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1712 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1720 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1730 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1737 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1742 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6122 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1748 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1753 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1757 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6151 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1771 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1775 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6184 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1780 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1789 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1816 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1817 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1818 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1819 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1830 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1839 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6299 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1856 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1862 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6341 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1869 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1880 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6362 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1883 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1897 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1906 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6395 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6401 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1911 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1916 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6420 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1923 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1928 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1939 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1944 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1951 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1963 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1964 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1968 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6506 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6510 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1983 "ds_parser.ypp" /* yacc.c:1903  */


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


