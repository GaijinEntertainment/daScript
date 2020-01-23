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

#line 364 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   5593

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  165
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  97
/* YYNRULES -- Number of rules.  */
#define YYNRULES  387
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  694

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   392

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
       0,   363,   363,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   378,   390,   391,   395,   399,   403,   412,
     420,   436,   457,   461,   464,   470,   479,   492,   493,   497,
     501,   502,   506,   509,   515,   521,   524,   530,   531,   532,
     539,   546,   558,   568,   577,   578,   579,   580,   581,   582,
     586,   591,   599,   600,   601,   602,   603,   604,   605,   606,
     612,   625,   641,   642,   646,   653,   665,   669,   676,   677,
     681,   682,   683,   687,   691,   698,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,   723,   724,   725,
     726,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   741,   811,   814,   824,   825,   826,   827,   828,
     829,   830,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   844,   848,   854,   860,   874,   875,   881,   885,
     891,   894,   897,   901,   907,   911,   915,   918,   921,   926,
     929,   937,   940,   945,   948,   956,   962,   963,   967,   968,
     972,  1005,  1008,  1013,  1021,  1025,  1030,  1036,  1040,  1045,
    1054,  1057,  1063,  1064,  1074,  1079,  1131,  1132,  1133,  1134,
    1135,  1136,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1159,  1170,
    1175,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,
    1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,
    1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,
    1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,
    1227,  1231,  1234,  1237,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1248,  1251,  1252,  1253,  1254,  1255,  1256,  1257,
    1267,  1277,  1311,  1312,  1316,  1317,  1321,  1329,  1332,  1339,
    1340,  1344,  1345,  1349,  1355,  1356,  1360,  1361,  1365,  1370,
    1373,  1377,  1383,  1392,  1393,  1394,  1398,  1401,  1406,  1411,
    1418,  1428,  1431,  1438,  1467,  1470,  1479,  1491,  1503,  1517,
    1518,  1522,  1561,  1627,  1634,  1643,  1644,  1645,  1646,  1647,
    1648,  1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1670,
    1681,  1685,  1695,  1696,  1697,  1698,  1712,  1718,  1722,  1727,
    1732,  1737,  1744,  1749,  1753,  1757,  1762,  1767,  1774,  1779,
    1785,  1790,  1794,  1799,  1808,  1812,  1817,  1826,  1830,  1835,
    1844,  1853,  1854,  1855,  1856,  1860,  1867,  1876,  1881,  1888,
    1893,  1899,  1906,  1917,  1920,  1934,  1943,  1944,  1948,  1953,
    1960,  1965,  1976,  1981,  1988,  2000,  2001,  2005
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
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "expr_type_info", "expr_list", "block_or_simple_block",
  "expr_block", "expr_numeric_const", "expr_assign", "expr_named_call",
  "expr_method_call", "expr", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "let_variable_declaration", "global_variable_declaration_list",
  "global_let", "enum_list", "alias_declaration", "enum_declaration",
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
      44,    61,    63,    58,   124,    94,    38,    60,    62,    45,
      43,    42,    47,    37,    64,   385,   386,   126,    33,   387,
     388,   389,   390,    46,    91,    93,    40,    41,   391,   392,
      59,   123,   125,    36,    35
};
# endif

#define YYPACT_NINF -445

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-445)))

#define YYTABLE_NINF -183

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -445,    28,  -445,  -445,   -70,   -81,   -64,   -17,    -8,  -445,
       0,   -16,  -445,  -445,  -445,  -445,  -445,   186,  -445,    46,
    -445,  -445,  -445,  -445,    17,  -445,    27,   123,   131,  -445,
    -445,   -28,  -445,   109,   147,    88,  -445,   129,   165,   126,
    -445,    69,  -445,   166,   115,  -445,   108,   -70,   173,   -81,
    -445,   168,   175,  -445,  -445,  4054,  -445,   180,  -445,   -70,
     -16,  -445,   177,   159,  5445,   289,   292,  -445,   174,   -97,
    -445,  -445,  -445,  -445,  -445,   203,   210,  -445,  -445,  -445,
    -445,  -445,  -445,   -90,   208,   209,   211,  -445,  -445,  -445,
     191,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,  -445,   212,  -445,  -445,  -445,   215,
     216,  -445,  -445,  -445,  -445,   220,  -445,  -445,  -445,  -445,
     659,  -445,   -89,  -445,   -88,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,   -13,   232,  -445,  -445,  -445,   277,  -445,
     158,  -445,  -445,  -445,  -445,  -445,  -445,  -445,   -15,    -1,
    -445,  4054,  4054,  4054,   242,  3982,  3982,  3982,  4054,  -445,
      14,  -445,  -445,  -445,  -445,  1732,  -445,  -445,  -445,  -104,
     243,  -445,  -445,    78,  -445,    84,  4054,   207,  -445,  -445,
     252,  -445,  4054,  3135,  -445,  -445,   210,  -445,  3135,  -445,
     521,  4203,  4597,   224,   232,  4740,   232,  4741,   232,  5257,
    -445,   -99,   659,   246,  -445,   229,  -445,  -445,  -445,  3910,
     233,  -445,   236,   248,   256,   238,   259,   241,   249,  3135,
    3135,    25,  4054,  3135,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  3135,  3135,  3135,   -88,  3135,  3135,  -445,  3135,  -445,
     244,  -445,  -445,   -93,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  4584,   253,  -445,  -445,  -445,  -445,  -445,   -70,  -445,
     373,   254,  -445,  -445,   -12,  -445,  -445,  4054,  3135,   659,
    -445,  -445,   420,  -445,   306,  3215,  -445,  3723,  -445,  4054,
    -445,  -445,   278,  -445,   279,  -445,   280,  -445,  -445,  4054,
    -445,  -445,   298,  -445,   291,  3135,  4054,  4054,  3135,  4054,
    1870,  1985,   269,   269,   243,  1111,  1249,  4879,  -445,    92,
     118,   269,   269,  -445,  -445,   269,   269,  4050,   796,   174,
    -445,  1364,   403,  3135,  3135,  -445,  -445,  3135,  3135,  3135,
    3135,   296,  3135,   297,  3135,  3135,  3135,  3135,  3135,  3135,
    3135,  3135,  3135,  3135,  3135,  3135,  3135,  3135,  3135,   300,
    3135,  -445,  2100,    95,  -445,   243,  -445,  -445,    15,  -445,
    4727,  -445,  3135,    97,  3135,  3135,  3135,  3135,  3135,  3135,
    3135,  3135,  3135,  3135,  3135,  3135,  3135,  3135,  3135,  -445,
    -445,  5258,  -445,  -445,  -445,  -445,  2215,  1479,  4122,  5311,
    5331,  4188,  5348,  -445,  4254,  -445,  4320,    -7,    52,  2330,
    4879,   295,  -108,   299,  -445,  -445,   -38,  2445,   106,   114,
    3135,  3135,   264,  -445,  3135,   301,  -445,  -445,  3135,  3135,
     243,  1617,  -445,   207,  3135,  3135,  -445,   268,   313,  2560,
    2675,  -445,   405,   275,   281,  -445,  -445,  -445,  -445,  -445,
    -445,   282,   283,   284,  -445,  -445,  -445,   421,  -445,   -87,
    4823,   232,   315,  -445,    32,  5047,  4054,   219,   219,  5232,
    5232,  5212,  5212,   290,   269,  -445,   -79,   -79,   219,   219,
    4935,   201,  5103,  5159,  5232,  5232,   464,   464,    59,    59,
      59,  -445,  4648,  -445,    61,  -445,  -445,  3135,  -445,  3304,
    -445,  5047,  5047,  5047,  5047,  5047,  5047,  5047,  5047,  5047,
    5047,  5047,  5047,  5047,  5047,  5047,  -445,  -445,    72,  4054,
     321,  4386,  -445,  3135,  3135,  -445,  3135,  -445,  -445,  3135,
    3135,   315,   323,   294,   315,  3135,   302,  3135,   315,   304,
     305,  5047,  -445,  -445,  4991,   608,   608,     8,  3135,  -445,
    -445,  3393,   439,  5047,   608,  -445,   318,   337,  5047,  3135,
    -445,  3482,   303,  -445,  -445,  -445,  -445,  -445,   426,   243,
    -445,   -92,    77,   104,  3135,  -445,   659,  2790,  3135,  -445,
    -445,  5047,  -445,  -445,  4044,    56,  -445,  5232,  5232,  5232,
     -98,  5047,    87,    77,  -445,   295,  5047,  -445,  -445,   117,
    -445,  -445,  -445,  -445,    37,  3135,  -445,  3571,  -445,   207,
    -445,  -445,  -445,  -445,  3660,  -445,  -445,  -445,  -445,  3135,
    -445,  -445,   308,  5047,  -445,    74,  5047,  -445,  2905,   332,
    3135,   312,  3135,   316,   207,  3135,  -445,    -4,  -445,  -445,
    -445,   959,  5047,  -445,  -445,  4054,  4452,   338,  3786,  -445,
    5047,  -445,  -445,   608,   207,  -445,  5351,  -445,  3020,   425,
     324,    37,  -445,  -445,  4054,  4518,  3135,   325,  -445,  5364,
    -445,  5047,  -445,  -445
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    68,     1,   158,     0,     0,     0,     0,     0,   159,
       0,     0,    12,     3,    10,     9,     8,     0,     7,     0,
       6,    11,     5,     4,    58,    60,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    63,   136,     0,    64,
      66,     0,    62,     0,     0,   291,     0,     0,     0,     0,
      28,     0,     0,    26,   294,     0,    13,     0,   138,     0,
       0,    69,   299,     0,     0,     0,     0,    75,    70,     0,
      56,    57,    54,    55,    53,     0,     0,    52,    61,    33,
      31,    25,    24,     0,     0,     0,     0,   305,   325,   306,
     330,   307,   311,   312,   313,   314,   318,   319,   320,   321,
     322,   323,   324,   326,   327,   351,   310,   317,   328,   354,
     357,   308,   315,   309,   316,     0,   329,   332,   334,   333,
     297,   137,     0,    67,     0,   301,   267,    78,    79,    81,
      80,    82,    83,    84,    85,   104,   105,   102,   103,    95,
     106,   107,    96,    93,    94,   108,   109,   110,   111,    98,
      99,    97,    91,    92,    87,    86,    88,    89,    90,    77,
      76,   100,   101,     0,    73,   303,   293,   292,     0,    14,
       0,    48,    49,    46,    47,    45,    44,    50,     0,     0,
     298,     0,     0,     0,     0,    70,    70,    70,     0,   338,
       0,   344,   347,   346,   340,     0,   343,    65,   300,   262,
       0,    71,   271,     0,   269,   278,     0,     0,   284,   285,
       0,   283,     0,     0,    15,    16,     0,    59,     0,   295,
       0,     0,     0,     0,    73,     0,    73,     0,    73,     0,
     274,     0,   273,   339,   342,     0,   345,   206,   207,     0,
       0,   201,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,   178,   177,   179,   180,   181,
      17,     0,     0,     0,     0,     0,     0,   336,     0,   132,
      68,   204,   174,   202,   255,   254,   253,   258,   203,   257,
     256,     0,     0,   205,   361,   362,   363,   364,     0,   302,
     264,     0,   270,    72,     0,   277,   276,     0,     0,    74,
     112,   304,     0,   290,     0,   182,    51,     0,   348,     0,
     350,   331,     0,   352,     0,   355,     0,   358,   360,     0,
     341,   337,   140,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,     0,     0,     0,     0,   382,     0,
       0,   211,   210,   243,   238,   209,   208,     0,     0,    70,
     261,     0,     0,     0,     0,   232,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,   265,     0,   268,   272,   279,   282,
     281,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     296,     0,   353,   356,   359,   275,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   248,     0,     0,   136,     0,
     373,   367,     0,   376,   377,   378,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    18,   234,   134,     0,     0,
       0,     0,   144,     0,     0,     0,   145,     0,     0,     0,
       0,   115,   113,     0,     0,   127,   122,   120,   121,   133,
     116,     0,     0,     0,   125,   126,   128,   156,   119,     0,
     182,    73,     0,   239,     0,   170,     0,   212,   213,   225,
     226,   223,   224,     0,   250,   237,   259,   260,   214,   215,
       0,   228,   229,   227,   221,   222,   217,   216,   218,   219,
     220,   236,     0,   241,     0,   263,   266,     0,   288,   182,
     135,   189,   190,   192,   191,   193,   186,   187,   188,   194,
     195,   184,   185,   196,   197,   183,   349,   141,     0,     0,
       0,     0,   244,     0,     0,   245,     0,   247,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   383,   384,     0,     0,     0,     0,     0,   146,
     149,   182,     0,   139,     0,   131,     0,     0,    36,     0,
     153,   182,     0,   129,   130,   118,   123,   124,     0,     0,
     117,     0,     0,     0,     0,   240,   252,     0,     0,   235,
     242,   280,   287,   142,     0,     0,   164,   161,   162,   163,
       0,   365,     0,     0,   369,   368,   374,   380,   379,     0,
     371,   381,    19,    42,    37,     0,   150,   182,   147,     0,
      43,    34,    35,   154,   182,   151,   132,   157,   160,     0,
     172,   175,     0,   171,   199,     0,   251,   167,     0,     0,
       0,     0,     0,     0,     0,     0,    40,     0,   148,   155,
     152,     0,   173,   198,   200,     0,     0,     0,   385,   370,
     366,   372,    38,     0,     0,   114,     0,   165,     0,     0,
       0,    37,    41,   168,     0,     0,     0,     0,    39,     0,
     166,   386,   387,   169
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -445,  -445,  -445,   141,   437,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,   435,  -445,  -445,  -194,  -445,  -445,
    -445,  -445,   272,  -445,   442,   -52,  -445,   430,  -445,   231,
    -168,  -212,  -445,  -445,  -196,  -445,  -142,  -189,   -11,  -445,
    -445,  -445,  -445,  -445,  -445,  -445,  -445,   498,  -445,  -445,
    -445,  -376,  -445,   124,  -445,  -344,  -445,  -445,   138,  -445,
    -445,  -445,  -445,   225,  -445,   202,  -445,  -202,  -185,   218,
     -63,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,   -67,
     -54,  -445,  -445,   -46,   286,  -444,  -331,  -445,  -445,  -245,
     -30,   195,  -445,  -445,  -445,  -445,  -445
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   170,   176,   340,   271,    14,    15,    31,
      32,    53,    16,    28,    29,   463,   464,   656,   465,   466,
     467,   468,   177,   178,    25,    26,    39,    40,    41,    17,
     164,   207,    68,    18,   272,   469,   348,   470,   273,   471,
     274,   472,   473,   474,   475,   476,   589,   477,   478,   275,
     276,   484,   641,   277,   278,   304,   279,   280,   485,   290,
     385,   291,   199,   202,   203,   230,   231,   550,   204,   213,
     167,    69,    20,    83,    21,    22,   125,    63,    23,   205,
     282,   118,   119,   232,   283,   431,   432,   284,   433,   434,
     435,   436,   285,   339,   286,   680,   287
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   117,   168,   298,   479,   438,   514,   122,   338,   120,
     549,   300,   312,    36,   314,   292,   316,   224,   226,   228,
     393,    50,   639,   361,   303,   625,   363,   165,     2,   200,
     200,   350,   594,     3,   179,   334,    37,    27,   593,   318,
     538,    47,   233,   189,   116,    84,   654,   553,   190,    42,
     288,    85,   554,     4,    24,     5,   655,     6,   289,     7,
      30,   319,   650,   351,     8,   166,    86,   191,   197,   269,
       9,    38,   180,   590,   379,   380,    10,   352,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    33,    37,   389,
     615,   165,   165,   198,    51,   216,    34,   556,   295,   192,
      70,    71,   557,   210,    35,    52,   594,   117,   117,   117,
     218,   117,   117,   117,   117,   220,   221,   222,   210,   225,
     227,   229,   217,    38,   201,    64,   296,   193,    46,    37,
     234,   194,   117,   355,   356,   295,   674,    47,   117,   219,
     299,   361,   594,   362,   363,   364,   302,   365,   235,   195,
     116,   116,   116,   610,   116,   116,   116,   116,   236,   196,
     295,   481,    11,   296,    38,   117,   517,   295,    12,   595,
      43,   594,    44,   322,   648,   116,   562,   117,   117,    60,
     516,   116,   594,   518,   594,   335,   336,    45,   296,    65,
      66,    57,   379,   380,   210,   296,   649,   297,   600,   352,
     612,   645,   171,   172,    61,    47,    72,   619,   116,   603,
      73,   664,    74,    75,   552,   293,   383,   352,   294,    67,
     116,   116,   651,   117,   169,   443,   444,   554,    56,   657,
     515,   388,   441,   344,   442,   117,    48,   572,   269,   642,
     270,    49,   570,   411,    76,   117,   554,   427,   559,   591,
      54,   580,   117,   117,   557,   117,   560,   554,    55,   653,
     419,   420,    59,   422,   214,   215,   116,   352,    57,    58,
      62,    79,    81,   353,   354,   355,   356,   357,   116,    82,
     358,   359,   360,   361,   121,   362,   363,   364,   116,   365,
     124,   366,   367,   355,   356,   116,   116,   479,   116,   161,
     126,   361,   162,   362,   363,   364,   189,   365,   173,   169,
     163,   190,   174,   281,   175,    75,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   181,   182,   184,   183,   185,
     191,   305,   186,   187,   379,   380,   307,   188,   374,   375,
     376,   377,   378,   355,   356,   206,   223,   165,   269,   623,
     624,   361,   379,   380,   363,   364,   301,   365,   630,   626,
     208,   311,   320,   567,   321,   326,   209,   332,   333,   324,
     633,   337,   325,   327,   328,   640,   329,   330,    11,   341,
     342,   343,   192,   345,   346,   331,   347,   210,   211,   382,
     212,   652,   384,   393,   386,   417,   412,   413,   414,   486,
     493,   495,   379,   380,   511,   552,   563,   214,   575,   555,
     193,   576,   117,   659,   194,   583,   390,   582,   588,   592,
     596,   584,   585,   586,   587,   605,   597,   613,   189,   614,
     629,   631,   195,   190,   416,   632,   667,   617,   672,   620,
     621,   637,   196,   418,   636,   663,   421,   669,   424,   426,
     686,   671,   191,   430,   430,   116,   678,   681,   682,   687,
     692,   445,   352,    77,    80,   117,   480,   688,   306,    78,
     123,   487,   488,   604,   661,   489,   490,   491,   492,    19,
     494,   349,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   520,   512,   387,
     392,   415,   168,   208,   192,   323,   638,   618,   116,   209,
     519,   439,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,     0,     0,   189,
       0,   211,   193,     0,   190,   541,   194,     0,   355,   356,
       0,     0,     0,     0,     0,     0,   361,   347,   362,   363,
     364,     0,   365,   191,   195,   347,     0,     0,   561,   337,
     391,     0,   564,     0,   196,     0,   565,   566,     0,   571,
       0,     0,   573,   574,     0,     0,     0,   578,   581,     0,
       0,     0,     0,     0,     0,   376,   377,   378,     0,     0,
       0,   117,     0,     0,     0,     0,     0,   379,   380,   676,
       0,     0,     0,     0,     0,   192,   352,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,   689,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,   116,   601,     0,   194,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,     0,   195,     0,     0,     0,     0,
       0,   607,   608,     0,   609,   196,     0,   189,   611,     0,
       0,     0,   190,   616,     0,   430,     0,     0,     0,     0,
     353,   354,   355,   356,   357,     0,   627,   358,   359,   360,
     361,   191,   362,   363,   364,     0,   365,   634,   366,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   643,     0,     0,     0,   646,     0,     0,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,   192,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,   662,     0,     0,
       0,     0,     0,     0,     0,     0,   666,     0,   668,     0,
     670,   193,     0,   673,     0,   194,     0,   447,     0,   480,
       0,     3,     0,   448,   449,     0,   450,     0,   237,   238,
     239,   240,     0,   195,     0,     0,   685,   451,   241,   452,
     453,     0,     0,   196,   691,     0,     0,     0,     0,   454,
     242,     0,     0,   455,   243,     0,   244,     0,     9,   245,
     456,     0,   457,   246,     0,     0,   458,   459,     0,    87,
      88,    89,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,     0,   106,   107,
     108,     0,   247,   111,   112,   113,   114,     0,   248,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,   252,   253,   254,   255,   256,   257,   258,   259,
      37,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   261,   262,   263,     0,     0,
     264,     0,     0,   265,   266,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,    38,   461,   269,   462,   270,
     447,     0,     0,     0,     3,     0,   448,   449,     0,   450,
       0,   237,   238,   239,   240,     0,     0,     0,     0,     0,
     451,   241,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   242,     0,     0,   455,   243,     0,   244,
       0,     9,   245,   456,     0,   457,   246,     0,     0,   458,
     459,     0,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   247,   111,   112,   113,   114,
       0,   248,   460,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   249,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   252,   253,   254,   255,   256,
     257,   258,   259,    37,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,   262,
     263,     0,     0,   264,     0,     0,   265,   266,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,    38,   461,
     269,   675,   270,   237,   238,   239,   240,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,   189,
       0,     0,     0,     0,   190,   242,     0,     0,     0,   243,
       0,   244,     0,     0,   245,     0,     0,     0,   246,     0,
       0,     0,     0,   191,    87,    88,    89,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,     0,   247,   111,   112,
     113,   114,     0,   248,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   428,   260,     0,     0,     0,
       0,     0,     0,   193,     0,     0,     0,   194,     0,     0,
     261,   262,   263,     0,     0,   264,     0,     0,   265,   266,
       0,   237,   238,   239,   240,   195,     0,   429,     0,     0,
      38,   241,   269,     0,   270,   196,     0,   189,     0,     0,
       0,     0,   190,   242,     0,     0,     0,   243,     0,   244,
       0,     0,   245,     0,     0,     0,   246,     0,     0,     0,
       0,   191,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   247,   111,   112,   113,   114,
       0,   248,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   249,   250,     0,     0,     0,     0,     0,
       0,     0,     0,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   428,   260,     0,   237,   238,   239,   240,
       0,   193,     0,     0,     0,   194,   241,     0,   261,   262,
     263,     0,     0,   264,     0,     0,   265,   266,   242,     0,
       0,     0,   243,   195,   244,   437,     0,   245,    38,     0,
     269,   246,   270,   196,     0,     0,     0,    87,    88,    89,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     0,   106,   107,   108,     0,
     247,   111,   112,   113,   114,     0,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
     252,   253,   254,   255,   256,   257,   258,   259,    37,   260,
       0,   237,   238,   239,   240,   539,     0,     0,     0,     0,
       0,   241,     0,   261,   262,   263,     0,     0,   264,     0,
       0,   265,   266,   242,     0,     0,     0,   243,   482,   244,
     268,   483,   245,    38,     0,   269,   246,   270,     0,     0,
       0,     0,    87,    88,    89,     0,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
       0,   106,   107,   108,     0,   247,   111,   112,   113,   114,
       0,   248,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   249,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   252,   253,   254,   255,   256,
     257,   258,   259,    37,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   540,     0,   261,   262,
     263,     0,     0,   264,     0,     0,   265,   266,     0,   237,
     238,   239,   240,     0,     0,   268,     0,     0,    38,   241,
     269,     0,   270,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,     0,   243,     0,   244,     0,     0,
     245,     0,     0,     0,   246,     0,     0,     0,     0,     0,
      87,    88,    89,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,     0,   106,
     107,   108,     0,   247,   111,   112,   113,   114,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,     0,     0,     0,     0,     0,     0,     0,
     568,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,   252,   253,   254,   255,   256,   257,   258,
     259,    37,   260,     0,   237,   238,   239,   240,     0,     0,
       0,     0,     0,     0,   241,     0,   261,   262,   263,     0,
       0,   264,     0,     0,   265,   266,   242,     0,     0,     0,
     243,     0,   244,   268,     0,   245,    38,   569,   269,   246,
     270,     0,     0,     0,     0,    87,    88,    89,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,     0,   106,   107,   108,     0,   247,   111,
     112,   113,   114,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,   252,   253,
     254,   255,   256,   257,   258,   259,    37,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   262,   263,     0,     0,   264,     0,     0,   265,
     266,     0,   237,   238,   239,   240,     0,   267,   268,     0,
       0,    38,   241,   269,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,   243,     0,
     244,     0,     0,   245,     0,     0,     0,   246,     0,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,   423,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,   237,   238,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,   242,     0,     0,     0,   243,     0,
     244,   268,   425,   245,    38,     0,   269,   246,   270,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,   513,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,   237,   238,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,   242,     0,     0,     0,   243,     0,
     244,   268,   537,   245,    38,     0,   269,   246,   270,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,   551,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,   237,   238,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,   242,     0,     0,     0,   243,     0,
     244,   268,   558,   245,    38,     0,   269,   246,   270,     0,
     577,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,     0,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,   579,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,   237,   238,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,   242,     0,     0,     0,   243,     0,
     244,   268,     0,   245,    38,     0,   269,   246,   270,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,   665,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,   644,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,   237,   238,   239,   240,   684,     0,     0,     0,
       0,     0,   241,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,   242,     0,     0,     0,   243,     0,
     244,   268,     0,   245,    38,     0,   269,   246,   270,     0,
       0,     0,     0,    87,    88,    89,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   106,   107,   108,     0,   247,   111,   112,   113,
     114,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,   252,   253,   254,   255,
     256,   257,   258,   259,    37,   260,     0,   237,   238,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,   261,
     262,   263,     0,     0,   264,     0,     0,   265,   266,   242,
       0,     0,     0,   243,     0,   244,   268,     0,   245,    38,
       0,   269,   246,   270,     0,     0,     0,     0,    87,    88,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,     0,   106,   107,   108,
       0,   247,   111,   112,   113,   114,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
     250,     0,     0,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   252,   253,   254,   255,   256,   257,   258,   259,    37,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   262,   263,     0,     0,   264,
       0,     0,   265,   266,     0,     0,     0,     0,     0,     0,
       0,   268,     0,     0,    38,     0,   269,     0,   270,   394,
     395,   396,   397,   398,   399,   400,   401,   353,   354,   355,
     356,   357,   402,   403,   358,   359,   360,   361,   404,   362,
     363,   364,   352,   365,   405,   366,   367,   406,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   408,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,     0,     0,     0,     0,   409,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,   395,
     396,   397,   398,   399,   400,   401,   353,   354,   355,   356,
     357,   402,   403,   358,   359,   360,   361,   404,   362,   363,
     364,   352,   365,   405,   366,   367,   406,   407,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   408,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
       0,     0,     0,     0,   602,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   395,   396,
     397,   398,   399,   400,   401,   353,   354,   355,   356,   357,
     402,   403,   358,   359,   360,   361,   404,   362,   363,   364,
     352,   365,   405,   366,   367,   406,   407,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   408,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,     0,   628,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   394,   395,   396,   397,
     398,   399,   400,   401,   353,   354,   355,   356,   357,   402,
     403,   358,   359,   360,   361,   404,   362,   363,   364,   352,
     365,   405,   366,   367,   406,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,     0,     0,
       0,     0,   635,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   394,   395,   396,   397,   398,
     399,   400,   401,   353,   354,   355,   356,   357,   402,   403,
     358,   359,   360,   361,   404,   362,   363,   364,   352,   365,
     405,   366,   367,   406,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   408,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,   658,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   352,     0,     0,   394,   395,   396,   397,   398,   399,
     400,   401,   353,   354,   355,   356,   357,   402,   403,   358,
     359,   360,   361,   404,   362,   363,   364,     0,   365,   405,
     366,   367,   406,   407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   352,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,   353,   354,   355,   356,   357,
     660,     0,   358,   359,   360,   361,     0,   362,   363,   364,
       0,   365,     0,   366,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,   353,   354,
     355,   356,   357,   410,     0,   358,   359,   360,   361,     0,
     362,   363,   364,     0,   365,     0,   366,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
      84,     0,     0,     0,     0,     0,    85,     0,     0,   379,
     380,     0,     0,     0,     0,     0,   679,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,     0,   251,   252,   253,     0,     0,
       0,     0,     0,     0,    37,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,   352,    38,
       0,     0,   189,     0,    84,     0,     0,   190,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,     0,     0,   163,     0,
     352,    38,   353,   354,   355,   356,   357,     0,   192,   358,
     359,   360,   361,     0,   362,   363,   364,     0,   365,     0,
     366,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   193,     0,    37,     0,
     194,     0,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,     0,     0,     0,     0,   195,     0,
       0,   647,     0,   379,   380,     0,   352,   446,   196,     0,
       0,     0,     0,    38,   353,   354,   355,   356,   357,     0,
       0,   358,   359,   360,   361,     0,   362,   363,   364,     0,
     365,   189,   366,   367,     0,     0,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,   191,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,     0,     0,     0,     0,
       0,     0,   352,     0,     0,   379,   380,     0,     0,   542,
     353,   354,   355,   356,   357,     0,     0,   358,   359,   360,
     361,     0,   362,   363,   364,     0,   365,     0,   366,   367,
       0,     0,     0,     0,     0,     0,     0,   192,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,     0,     0,     0,   193,     0,     0,   352,   194,
       0,   379,   380,     0,     0,   545,   353,   354,   355,   356,
     357,     0,     0,   358,   359,   360,   361,   195,   362,   363,
     364,     0,   365,   309,   366,   367,     0,   196,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,     0,     0,
       0,     0,     0,     0,   352,     0,     0,   379,   380,     0,
       0,   547,   353,   354,   355,   356,   357,     0,     0,   358,
     359,   360,   361,     0,   362,   363,   364,     0,   365,     0,
     366,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,     0,     0,     0,     0,     0,     0,
     352,     0,     0,   379,   380,     0,     0,   548,   353,   354,
     355,   356,   357,     0,     0,   358,   359,   360,   361,     0,
     362,   363,   364,     0,   365,     0,   366,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
       0,     0,     0,     0,     0,     0,   352,     0,     0,   379,
     380,     0,     0,   606,   353,   354,   355,   356,   357,     0,
       0,   358,   359,   360,   361,     0,   362,   363,   364,     0,
     365,     0,   366,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,     0,     0,     0,     0,
       0,     0,   352,     0,     0,   379,   380,     0,     0,   677,
     353,   354,   355,   356,   357,     0,     0,   358,   359,   360,
     361,     0,   362,   363,   364,   189,   365,     0,   366,   367,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,     0,     0,     0,     0,   352,     0,     0,     0,
       0,   379,   380,     0,     0,   690,   353,   354,   355,   356,
     357,     0,     0,   358,   359,   360,   361,     0,   362,   363,
     364,     0,   365,     0,   366,   367,     0,     0,     0,     0,
       0,   192,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,     0,   193,
       0,     0,     0,   194,     0,   310,     0,   379,   380,   381,
     353,   354,   355,   356,   357,   352,     0,   358,   359,   360,
     361,   195,   362,   363,   364,     0,   365,     0,   366,   367,
       0,   196,     0,     0,     0,     0,     0,     0,   189,   189,
       0,     0,     0,   190,   190,     0,     0,     0,     0,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   191,   191,     0,     0,     0,     0,     0,     0,
       0,   379,   380,   599,     0,     0,     0,     0,     0,     0,
       0,   394,   395,   396,   397,   398,   399,   400,   401,   353,
     354,   355,   356,   357,   402,   403,   358,   359,   360,   361,
     404,   362,   363,   364,  -182,   365,   405,   366,   367,   406,
     407,   352,     0,     0,   192,   192,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,     0,   193,   193,     0,     0,   194,   194,   313,   315,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   195,   195,     0,   352,     0,     0,
       0,     0,     0,     0,   196,   196,     0,   394,   395,   396,
     397,   398,   399,   400,   401,   353,   354,   355,   356,   357,
     402,   403,   358,   359,   360,   361,   404,   362,   363,   364,
       0,   365,   405,   366,   367,   406,   407,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,   408,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,     0,     0,     0,
       0,   353,   354,   355,   356,   357,   379,   380,   358,   359,
     360,   361,     0,   362,   363,   364,     0,   365,     0,   366,
     367,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   352,
       0,   368,     0,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,     0,     0,     0,     0,   353,   354,   355,
     356,   357,   379,   380,   358,   359,   360,   361,     0,   362,
     363,   364,     0,   365,     0,   366,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   352,     0,   368,   598,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,     0,
       0,     0,     0,   353,   354,   355,   356,   357,   379,   380,
     358,   359,   360,   361,     0,   362,   363,   364,     0,   365,
       0,   366,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     622,   352,     0,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,     0,     0,     0,     0,   353,
     354,   355,   356,   357,   379,   380,   358,   359,   360,   361,
       0,   362,   363,   364,     0,   365,     0,   366,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   352,     0,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,     0,     0,     0,     0,   353,   354,   355,   356,   357,
     379,   380,   358,   359,   360,   361,     0,   362,   363,   364,
       0,   365,     0,   366,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,   373,   374,   375,   376,   377,   378,     0,     0,     0,
     352,   353,   354,   355,   356,   357,   379,   380,   358,   359,
     360,   361,     0,   362,   363,   364,     0,   365,     0,   366,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   189,     0,     0,     0,
     190,   190,     0,     0,     0,     0,   372,   373,   374,   375,
     376,   377,   378,     0,   353,   354,   355,   356,   357,   191,
     191,   358,   379,   380,   361,     0,   362,   363,   364,     0,
     365,     0,   366,   367,   353,   354,   355,   356,     0,     0,
       0,     0,     0,     0,   361,     0,   362,   363,   364,   189,
     365,     0,   366,   367,   190,     0,     0,     0,     0,   372,
     373,   374,   375,   376,   377,   378,     0,     0,     0,   189,
       0,   192,   192,   191,   190,   379,   380,     0,     0,     0,
       0,   374,   375,   376,   377,   378,   189,     0,     0,   189,
       0,   190,     0,   191,   190,   379,   380,     0,     0,   193,
     193,     0,   189,   194,   194,   317,   536,   190,     0,     0,
     191,     0,     0,   191,     0,     0,     0,     0,     0,     0,
       0,   195,   195,     0,     0,   192,   191,     0,     0,     0,
       0,   196,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   192,     0,     0,     0,     0,
       0,     0,     0,   193,     0,     0,     0,   194,     0,   543,
       0,     0,   192,     0,     0,   192,     0,     0,     0,     0,
       0,     0,     0,   193,     0,   195,     0,   194,   192,   544,
       0,     0,     0,     0,     0,   196,     0,     0,     0,     0,
     193,     0,     0,   193,   194,   195,   546,   194,     0,     0,
       0,     0,     0,     0,     0,   196,   193,     0,     0,     0,
     194,     0,   195,     0,     0,   195,     0,     0,   683,     0,
       0,     0,   196,     0,     0,   196,     0,     0,   195,     0,
       0,   693,     0,     0,     0,     0,     0,     0,   196,   127,
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
      11,    55,    69,   205,   348,   336,   382,    59,   253,    55,
      17,   207,   224,    29,   226,   200,   228,   185,   186,   187,
     107,    49,   114,   102,   213,    17,   105,   124,     0,    42,
      42,   124,   130,     5,   124,    10,   124,   118,   482,   138,
     416,   130,    28,    28,    55,    20,     9,   155,    33,    60,
     154,    26,   160,    25,   124,    27,    19,    29,   162,    31,
     124,   160,   160,   156,    36,   162,    41,    52,   157,   161,
      42,   159,   162,   160,   153,   154,    48,    18,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,   124,   124,   298,
     554,   124,   124,   124,   142,   130,   124,   155,   103,   104,
      12,    13,   160,   130,   124,   153,   130,   181,   182,   183,
     131,   185,   186,   187,   188,   181,   182,   183,   130,   185,
     186,   187,   157,   159,   157,    30,   131,   132,   131,   124,
     136,   136,   206,    94,    95,   103,   160,   130,   212,   160,
     206,   102,   130,   104,   105,   106,   212,   108,   154,   154,
     181,   182,   183,   549,   185,   186,   187,   188,   164,   164,
     103,   349,   154,   131,   159,   239,   388,   103,   160,   157,
       4,   130,     6,   239,   138,   206,   441,   251,   252,   130,
     385,   212,   130,   392,   130,   251,   252,   161,   131,    94,
      95,   159,   153,   154,   130,   131,   160,   133,   157,    18,
     551,   597,    12,    13,   155,   130,   118,   558,   239,   157,
     122,   157,   124,   125,   130,   157,   288,    18,   160,   124,
     251,   252,   155,   297,   126,   127,   128,   160,   160,   625,
     155,   297,   160,   264,   162,   309,   133,   453,   161,   155,
     163,   130,   451,   309,   156,   319,   160,   334,   162,   481,
     161,   460,   326,   327,   160,   329,   162,   160,   131,   162,
     326,   327,   156,   329,   126,   127,   297,    18,   159,   124,
     124,   118,   124,    92,    93,    94,    95,    96,   309,   124,
      99,   100,   101,   102,   124,   104,   105,   106,   319,   108,
     133,   110,   111,    94,    95,   326,   327,   661,   329,    30,
     161,   102,    30,   104,   105,   106,    28,   108,   118,   126,
     156,    33,   122,   195,   124,   125,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   137,   137,   156,   137,   137,
      52,   213,   137,   137,   153,   154,   218,   137,   139,   140,
     141,   142,   143,    94,    95,   133,   124,   124,   161,   565,
     566,   102,   153,   154,   105,   106,   124,   108,   574,   568,
     103,   157,   136,   450,   155,   137,   109,   249,   250,   156,
     579,   253,   156,   137,   156,   591,   137,   156,   154,   261,
     262,   263,   104,   265,   266,   156,   268,   130,   131,   156,
     133,   613,    39,   107,   160,   124,   138,   138,   138,    16,
     124,   124,   153,   154,   124,   130,   162,   126,   160,   130,
     132,   118,   486,   629,   136,   160,   298,    32,    17,   124,
     486,   160,   160,   160,   160,   124,   156,   124,    28,   155,
      11,   133,   154,    33,   156,   118,   124,   155,   654,   155,
     155,    35,   164,   325,   161,   157,   328,   155,   330,   331,
      45,   155,    52,   335,   336,   486,   138,   673,   674,   155,
     155,   340,    18,    46,    49,   539,   348,   681,   216,    47,
      60,   353,   354,   539,   636,   357,   358,   359,   360,     1,
     362,   270,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   393,   380,   294,
     302,   319,   589,   103,   104,   239,   589,   557,   539,   109,
     392,   336,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,    -1,    -1,    28,
      -1,   131,   132,    -1,    33,   417,   136,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   429,   104,   105,
     106,    -1,   108,    52,   154,   437,    -1,    -1,   440,   441,
     160,    -1,   444,    -1,   164,    -1,   448,   449,    -1,   451,
      -1,    -1,   454,   455,    -1,    -1,    -1,   459,   460,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   665,    -1,    -1,    -1,    -1,    -1,   153,   154,   665,
      -1,    -1,    -1,    -1,    -1,   104,    18,    -1,    -1,    -1,
     684,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   684,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   665,   517,    -1,   136,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   684,    -1,   154,    -1,    -1,    -1,    -1,
      -1,   543,   544,    -1,   546,   164,    -1,    28,   550,    -1,
      -1,    -1,    33,   555,    -1,   557,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    -1,   568,    99,   100,   101,
     102,    52,   104,   105,   106,    -1,   108,   579,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   594,    -1,    -1,    -1,   598,    -1,    -1,    -1,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,   104,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   639,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   648,    -1,   650,    -1,
     652,   132,    -1,   655,    -1,   136,    -1,     1,    -1,   661,
      -1,     5,    -1,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    -1,   154,    -1,    -1,   678,    21,    22,    23,
      24,    -1,    -1,   164,   686,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      44,    -1,    46,    47,    -1,    -1,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    -1,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
     144,    -1,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,   163,
       1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    -1,    46,    47,    -1,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,   144,    -1,    -1,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,
     161,   162,   163,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,
      -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    52,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,    -1,
     139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,
      -1,    12,    13,    14,    15,   154,    -1,   156,    -1,    -1,
     159,    22,   161,    -1,   163,   164,    -1,    28,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    52,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    12,    13,    14,    15,
      -1,   132,    -1,    -1,    -1,   136,    22,    -1,   139,   140,
     141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,    -1,
      -1,    -1,    38,   154,    40,   156,    -1,    43,   159,    -1,
     161,    47,   163,   164,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,    -1,
      -1,   147,   148,    34,    -1,    -1,    -1,    38,   154,    40,
     156,   157,    43,   159,    -1,   161,    47,   163,    -1,    -1,
      -1,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,
     141,    -1,    -1,   144,    -1,    -1,   147,   148,    -1,    12,
      13,    14,    15,    -1,    -1,   156,    -1,    -1,   159,    22,
     161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    38,    -1,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,   147,   148,    34,    -1,    -1,    -1,
      38,    -1,    40,   156,    -1,    43,   159,   160,   161,    47,
     163,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,   144,    -1,    -1,   147,
     148,    -1,    12,    13,    14,    15,    -1,   155,   156,    -1,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,   157,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,    -1,
      40,   156,   157,    43,   159,    -1,   161,    47,   163,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,   157,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,    -1,
      40,   156,   157,    43,   159,    -1,   161,    47,   163,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,   157,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,    -1,
      40,   156,   157,    43,   159,    -1,   161,    47,   163,    -1,
      50,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,    -1,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,    -1,
      40,   156,    -1,    43,   159,    -1,   161,    47,   163,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,   157,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    34,    -1,    -1,    -1,    38,    -1,
      40,   156,    -1,    43,   159,    -1,   161,    47,   163,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   139,
     140,   141,    -1,    -1,   144,    -1,    -1,   147,   148,    34,
      -1,    -1,    -1,    38,    -1,    40,   156,    -1,    43,   159,
      -1,   161,    47,   163,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,   144,
      -1,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,    -1,   161,    -1,   163,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,    18,   108,   109,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    18,   108,   109,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      18,   108,   109,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    18,
     108,   109,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   131,   132,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    18,   108,
     109,   110,   111,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,    92,    93,    94,    95,    96,
     160,    -1,    99,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    92,    93,
      94,    95,    96,   160,    -1,    99,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      20,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,   153,
     154,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    -1,    18,   159,
      -1,    -1,    28,    -1,    20,    -1,    -1,    33,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    -1,   156,    -1,
      18,   159,    92,    93,    94,    95,    96,    -1,   104,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,   124,    -1,
     136,    -1,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   154,    -1,
      -1,   157,    -1,   153,   154,    -1,    18,   157,   164,    -1,
      -1,    -1,    -1,   159,    92,    93,    94,    95,    96,    -1,
      -1,    99,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,    28,   110,   111,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    52,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,   153,   154,    -1,    -1,   157,
      92,    93,    94,    95,    96,    -1,    -1,    99,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,   132,    -1,    -1,    18,   136,
      -1,   153,   154,    -1,    -1,   157,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,   101,   102,   154,   104,   105,
     106,    -1,   108,   160,   110,   111,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,   153,   154,    -1,
      -1,   157,    92,    93,    94,    95,    96,    -1,    -1,    99,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,   153,   154,    -1,    -1,   157,    92,    93,
      94,    95,    96,    -1,    -1,    99,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   153,
     154,    -1,    -1,   157,    92,    93,    94,    95,    96,    -1,
      -1,    99,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,   153,   154,    -1,    -1,   157,
      92,    93,    94,    95,    96,    -1,    -1,    99,   100,   101,
     102,    -1,   104,   105,   106,    28,   108,    -1,   110,   111,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   153,   154,    -1,    -1,   157,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,    -1,   132,
      -1,    -1,    -1,   136,    -1,   138,    -1,   153,   154,   155,
      92,    93,    94,    95,    96,    18,    -1,    99,   100,   101,
     102,   154,   104,   105,   106,    -1,   108,    -1,   110,   111,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    28,    28,
      -1,    -1,    -1,    33,    33,    -1,    -1,    -1,    -1,    -1,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    52,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    18,    -1,    -1,   104,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,   132,   132,    -1,    -1,   136,   136,   138,   138,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   154,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,   164,   164,    -1,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,   153,   154,    99,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,    -1,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,   132,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,   153,   154,    99,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,   153,   154,
      99,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    18,    -1,   132,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,   153,   154,    99,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,   132,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
     153,   154,    99,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      18,    92,    93,    94,    95,    96,   153,   154,    99,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    28,    -1,    -1,    -1,
      33,    33,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,    -1,    92,    93,    94,    95,    96,    52,
      52,    99,   153,   154,   102,    -1,   104,   105,   106,    -1,
     108,    -1,   110,   111,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    28,
     108,    -1,   110,   111,    33,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    28,
      -1,   104,   104,    52,    33,   153,   154,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    28,    -1,    -1,    28,
      -1,    33,    -1,    52,    33,   153,   154,    -1,    -1,   132,
     132,    -1,    28,   136,   136,   138,   138,    33,    -1,    -1,
      52,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,   154,    -1,    -1,   104,    52,    -1,    -1,    -1,
      -1,   164,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,
      -1,    -1,   104,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,   154,    -1,   136,   104,   138,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
     132,    -1,    -1,   132,   136,   154,   138,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,   132,    -1,    -1,    -1,
     136,    -1,   154,    -1,    -1,   154,    -1,    -1,   157,    -1,
      -1,    -1,   164,    -1,    -1,   164,    -1,    -1,   154,    -1,
      -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,   164,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    -1,
      -1,    -1,   147,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   166,     0,     5,    25,    27,    29,    31,    36,    42,
      48,   154,   160,   167,   172,   173,   177,   194,   198,   212,
     237,   239,   240,   243,   124,   189,   190,   118,   178,   179,
     124,   174,   175,   124,   124,   124,    29,   124,   159,   191,
     192,   193,   203,     4,     6,   161,   131,   130,   133,   130,
      49,   142,   153,   176,   161,   131,   160,   159,   124,   156,
     130,   155,   124,   242,    30,    94,    95,   124,   197,   236,
      12,    13,   118,   122,   124,   125,   156,   169,   189,   118,
     179,   124,   124,   238,    20,    26,    41,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   203,   245,   246,   247,
     248,   124,   190,   192,   133,   241,   161,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   110,   111,   112,   113,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   147,
     148,    30,    30,   156,   195,   124,   162,   235,   244,   126,
     168,    12,    13,   118,   122,   124,   169,   187,   188,   124,
     162,   137,   137,   137,   156,   137,   137,   137,   137,    28,
      33,    52,   104,   132,   136,   154,   164,   157,   203,   227,
      42,   157,   228,   229,   233,   244,   133,   196,   103,   109,
     130,   131,   133,   234,   126,   127,   130,   157,   131,   160,
     248,   248,   248,   124,   195,   248,   195,   248,   195,   248,
     230,   231,   248,    28,   136,   154,   164,    12,    13,    14,
      15,    22,    34,    38,    40,    43,    47,    76,    82,    94,
      95,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     125,   139,   140,   141,   144,   147,   148,   155,   156,   161,
     163,   171,   199,   203,   205,   214,   215,   218,   219,   221,
     222,   223,   245,   249,   252,   257,   259,   261,   154,   162,
     224,   226,   233,   157,   160,   103,   131,   133,   232,   248,
     199,   124,   248,   202,   220,   223,   187,   223,   138,   160,
     138,   157,   196,   138,   196,   138,   196,   138,   138,   160,
     136,   155,   248,   249,   156,   156,   137,   137,   156,   137,
     156,   156,   223,   223,    10,   248,   248,   223,   254,   258,
     170,   223,   223,   223,   203,   223,   223,   223,   201,   194,
     124,   156,    18,    92,    93,    94,    95,    96,    99,   100,
     101,   102,   104,   105,   106,   108,   110,   111,   132,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   153,
     154,   155,   156,   190,    39,   225,   160,   228,   248,   202,
     223,   160,   234,   107,    84,    85,    86,    87,    88,    89,
      90,    91,    97,    98,   103,   109,   112,   113,   131,   160,
     160,   248,   138,   138,   138,   230,   156,   124,   223,   248,
     248,   223,   248,   157,   223,   157,   223,   244,   124,   156,
     223,   250,   251,   253,   254,   255,   256,   156,   251,   256,
     114,   160,   162,   127,   128,   168,   157,     1,     7,     8,
      10,    21,    23,    24,    33,    37,    44,    46,    50,    51,
      83,   160,   162,   180,   181,   183,   184,   185,   186,   200,
     202,   204,   206,   207,   208,   209,   210,   212,   213,   220,
     223,   195,   154,   157,   216,   223,    16,   223,   223,   223,
     223,   223,   223,   124,   223,   124,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   124,   223,   157,   216,   155,   233,   232,   202,   223,
     218,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   138,   157,   216,    16,
     137,   223,   157,   138,   138,   157,   138,   157,   157,    17,
     232,   157,   130,   155,   160,   130,   155,   160,   157,   162,
     162,   223,   254,   162,   223,   223,   223,   244,   103,   160,
     202,   223,   199,   223,   223,   160,   118,    50,   223,   103,
     202,   223,    32,   160,   160,   160,   160,   160,    17,   211,
     160,   196,   124,   250,   130,   157,   248,   156,   133,   155,
     157,   223,   160,   157,   248,   124,   157,   223,   223,   223,
     216,   223,   251,   124,   155,   250,   223,   155,   255,   251,
     155,   155,   129,   199,   199,    17,   202,   223,   160,    11,
     199,   133,   118,   202,   223,   160,   161,    35,   235,   114,
     199,   217,   155,   223,   157,   216,   223,   157,   138,   160,
     160,   155,   232,   162,     9,    19,   182,   216,   160,   199,
     160,   201,   223,   157,   157,    16,   223,   124,   223,   155,
     223,   155,   199,   223,   160,   162,   248,   157,   138,   160,
     260,   199,   199,   157,    16,   223,    45,   155,   182,   248,
     157,   223,   155,   157
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
     200,   200,   201,   201,   201,   202,   203,   203,   203,   204,
     205,   205,   205,   205,   206,   207,   208,   208,   208,   208,
     208,   209,   209,   209,   209,   210,   211,   211,   212,   212,
     213,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     216,   216,   217,   217,   218,   218,   219,   219,   219,   219,
     219,   219,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   221,   222,
     222,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   225,   225,   226,   227,   227,   228,
     228,   229,   229,   230,   231,   231,   232,   232,   233,   233,
     233,   233,   233,   234,   234,   234,   235,   235,   235,   235,
     235,   236,   236,   237,   238,   238,   238,   239,   240,   241,
     241,   242,   243,   244,   244,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   246,
     247,   247,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   249,   249,   249,   249,   250,   250,   251,   251,   252,
     252,   252,   252,   253,   253,   254,   255,   255,   256,   256,
     257,   257,   258,   258,   259,   260,   260,   261
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
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     3,     4,     2,     3,     4,     0,     2,     1,     1,
       3,     5,     5,     5,     5,     8,    10,     6,     9,    11,
       1,     3,     1,     2,     1,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     6,     5,
       6,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     3,     4,     3,     3,     2,     3,
       4,     3,     4,     2,     4,     4,     3,     4,     3,     4,
       3,     5,     4,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     0,     3,     0,     1,     3,     0,     3,     1,
       2,     1,     3,     1,     1,     3,     1,     1,     1,     3,
       5,     3,     3,     1,     1,     1,     4,     6,     5,     4,
       3,     0,     2,     4,     0,     3,     5,     4,     5,     0,
       2,     2,     6,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     4,     3,     4,     2,     3,
       2,     4,     3,     2,     2,     3,     2,     2,     4,     6,
       4,     1,     4,     5,     1,     4,     5,     1,     4,     5,
       4,     1,     1,     1,     1,     3,     5,     1,     3,     5,
       7,     5,     7,     1,     3,     3,     1,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10
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
          case 124: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2815 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2821 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2827 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2833 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2839 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2845 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expression_label  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2851 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expression_goto  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2857 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2863 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2869 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2875 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2881 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2887 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* annotation_argument_value  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2893 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* annotation_argument_value_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2899 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2905 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2911 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2917 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2923 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2929 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2935 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2941 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2947 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2953 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2959 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2965 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2971 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2977 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2983 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2989 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2995 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3001 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3007 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3013 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_yield  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3019 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3025 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3031 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3037 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3043 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3049 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* block_or_simple_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3055 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3061 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3067 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3073 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3079 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3085 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3091 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3097 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3103 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3109 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3115 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3121 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3127 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3133 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3139 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3145 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3151 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3157 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3163 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3169 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3175 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3181 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3187 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3193 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3199 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3205 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3211 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3217 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3223 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3229 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3235 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3241 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3247 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3253 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3259 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3265 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 378 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 390 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 391 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 403 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 412 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3612 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 420 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3630 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 436 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 461 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 464 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3672 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 470 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 492 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 493 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 506 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3718 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 509 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 515 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3734 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 521 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3742 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 524 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3756 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3762 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 546 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 558 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 568 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 578 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 580 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3853 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 586 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3863 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 591 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 599 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 600 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 601 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 602 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 603 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 604 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 605 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3923 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 612 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 625 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3959 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 653 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4006 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 669 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 676 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 677 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 682 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 683 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 687 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 691 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 698 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 702 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 703 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 706 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 709 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 710 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4186 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 723 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 724 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 726 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 727 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 728 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 730 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 732 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 733 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 735 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4359 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 811 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4367 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 814 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4385 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 825 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 826 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4397 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 827 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 828 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 829 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 830 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 831 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 832 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 833 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4439 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 835 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4451 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 836 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 837 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 838 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4469 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 839 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4475 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 840 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4481 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 848 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 854 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 860 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 881 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 885 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4572 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 915 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 918 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 926 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 929 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 937 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4663 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 962 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 967 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 968 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4748 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1005 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4756 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1013 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4776 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1021 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4785 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1025 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1030 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1036 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4815 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1040 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4825 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1045 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4844 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1057 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1063 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4858 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1064 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4880 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1080 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4939 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4969 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1140 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4975 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1141 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4981 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1142 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4987 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1143 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1144 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1145 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1146 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5011 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1147 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5017 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1148 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1149 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1150 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5035 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1151 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1154 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1155 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1159 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1170 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1175 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1185 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1190 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1191 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1199 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1201 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5237 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5249 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1210 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5297 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5303 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5309 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1220 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1221 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5353 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5361 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5367 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5373 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1239 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5385 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-2]))); }
#line 5397 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1248 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1252 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1267 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1339 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5596 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1345 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1349 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1355 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5622 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1360 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1361 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1365 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1377 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5672 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1393 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1398 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1401 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1406 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1411 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1418 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1428 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1431 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1467 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5809 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1470 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1479 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1491 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5851 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5867 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1518 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1561 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1627 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1634 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6011 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6017 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6035 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1647 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1648 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1649 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1650 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1651 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6071 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6095 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6143 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6149 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6155 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1670 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1681 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1685 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1695 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1696 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1697 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1698 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6226 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1712 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6237 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1718 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1722 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6256 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1727 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1732 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1737 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1744 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1749 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6307 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1753 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1757 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6326 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1767 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1774 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1779 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1785 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1790 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1794 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1808 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1812 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1817 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1826 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1830 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6464 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1835 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1853 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1856 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6513 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1867 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1876 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1888 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1893 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1899 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1906 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1917 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1920 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6636 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1943 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6642 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1944 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6648 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1953 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1960 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1965 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6690 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1976 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6700 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1981 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1988 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6723 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2000 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2001 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2005 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6753 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6757 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2020 "ds_parser.ypp" /* yacc.c:1903  */


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


