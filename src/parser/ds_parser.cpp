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

#line 361 "ds_parser.cpp"

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
#define YYLAST   4358

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  160
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  360
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  643

#define YYUNDEFTOK  2
#define YYMAXUTOK   388

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
       2,     2,     2,   144,     2,     2,   159,   139,   132,     2,
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
       0,   355,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   369,   381,   382,   386,   390,   394,   403,
     411,   427,   448,   452,   455,   461,   470,   483,   484,   488,
     492,   493,   497,   500,   506,   512,   515,   521,   522,   523,
     530,   537,   549,   559,   568,   569,   570,   571,   572,   573,
     577,   582,   590,   591,   595,   602,   614,   618,   625,   626,
     630,   631,   632,   636,   640,   647,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   686,   690,   760,   763,   773,   774,   775,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   792,   796,   802,   808,   822,   823,   829,   833,   839,
     842,   845,   849,   855,   859,   863,   866,   869,   874,   877,
     885,   891,   892,   896,   897,   901,   934,   937,   942,   950,
     954,   959,   963,   971,   974,   980,   985,  1037,  1038,  1039,
    1040,  1041,  1042,  1046,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1065,
    1076,  1081,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,
    1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,
    1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,
    1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,
    1129,  1133,  1137,  1140,  1143,  1144,  1145,  1146,  1147,  1148,
    1149,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1168,  1178,
    1212,  1213,  1217,  1218,  1222,  1230,  1233,  1240,  1241,  1245,
    1246,  1250,  1256,  1257,  1261,  1262,  1266,  1271,  1274,  1278,
    1284,  1293,  1294,  1295,  1299,  1302,  1307,  1312,  1319,  1329,
    1332,  1339,  1368,  1371,  1380,  1392,  1404,  1418,  1419,  1423,
    1462,  1528,  1535,  1544,  1545,  1546,  1547,  1548,  1549,  1550,
    1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,
    1561,  1562,  1563,  1564,  1565,  1566,  1567,  1571,  1582,  1586,
    1596,  1597,  1598,  1599,  1613,  1619,  1623,  1628,  1633,  1638,
    1643,  1650,  1655,  1662,  1667,  1673,  1678,  1682,  1687,  1696,
    1700,  1705,  1714,  1718,  1723,  1732,  1741,  1742,  1743,  1744,
    1748,  1755,  1764,  1769,  1776,  1781,  1790,  1793,  1807,  1816,
    1817,  1821,  1826,  1833,  1838,  1849,  1854,  1861,  1873,  1874,
    1878
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
  "')'", "DEREF", "COLCOL", "';'", "'{'", "'}'", "'$'", "$accept",
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
     375,   376,   377,   378,   379,   380,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
      64,   381,   382,   126,    33,   383,   384,   385,   386,    46,
      91,    93,    40,    41,   387,   388,    59,   123,   125,    36
};
# endif

#define YYPACT_NINF -440

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-440)))

#define YYTABLE_NINF -164

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -440,    38,  -440,  -440,   -25,   -68,   -20,    -6,     1,  -440,
      89,   -16,  -440,  -440,  -440,  -440,  -440,    27,  -440,    53,
    -440,  -440,  -440,  -440,   103,  -440,   -66,    94,   110,  -440,
    -440,   -33,  -440,   101,   119,    95,  -440,    99,   141,   111,
    -440,   -79,  -440,   144,   122,  -440,    75,   -25,   155,   -68,
    -440,   150,   154,  -440,  -440,  2814,  -440,   158,  -440,   -25,
     -16,  -440,   146,   123,  3723,   254,   255,  -440,   133,   -97,
    -440,  -440,  -440,  -440,  -440,   165,  -440,  -440,  -440,  -440,
    -440,  -440,   -93,   156,   157,   161,  -440,  -440,  -440,   136,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,   162,  -440,  -440,  -440,   163,   164,
    -440,  -440,  -440,  -440,   170,  -440,  -440,  -440,  -440,   245,
    -440,   -73,  -440,   -96,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,    -9,   177,  -440,  -440,  -440,   -36,  -440,    52,
     -99,  -440,  2814,  2814,  2814,   189,  2744,  2744,  2744,  2814,
    -440,     2,  -440,  -440,  -440,  1574,  -440,  -440,    20,   192,
    -440,  -440,    64,  -440,   -43,  2814,   160,  -440,  -440,   194,
    -440,  2814,  2491,  -440,  -440,   201,  -440,   176,    -2,   179,
     166,   177,   220,   177,   228,   177,   235,  -440,    42,   245,
     186,  -440,   171,  -440,  -440,  2674,   172,  -440,   175,   199,
     200,   184,   204,   187,  2491,  2491,  2604,  2814,  2491,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  2491,  2491,  2491,   -96,
    2491,  2491,  -440,  2491,  -440,   188,  -440,  -440,   -94,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  3745,   198,  -440,  -440,
    -440,  -440,  -440,   -25,  -440,   302,   185,  -440,  -440,   -21,
    -440,  -440,  2814,  2491,   245,  -440,  -440,     8,  -440,   250,
    2821,   202,  -440,  2814,  -440,  -440,   221,  -440,   223,  -440,
     230,  -440,  -440,  2814,  -440,  -440,   173,  -440,   246,  2491,
    2814,  2814,  2491,  2814,  1707,   676,   676,   192,   972,  1105,
    3873,  -440,    -3,   121,   676,   676,  -440,  -440,   676,   676,
    3319,   680,   133,  -440,  1217,  2491,  2491,  -440,  -440,  2491,
    2491,  2491,  2491,   248,  2491,   251,  2491,  2491,  2491,  2491,
    2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,
    2491,   252,  2491,  -440,  1819,   -74,  -440,   192,  -440,  -440,
      10,  -440,  3129,  -440,  2491,     0,  2491,  2491,  2491,  2491,
    2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,  2491,
    2491,  -440,  -440,   259,  -440,  -440,  -440,  -440,  1931,  1329,
    3385,   289,   392,  3451,   402,  -440,  3517,    17,   -77,  2043,
    3873,   239,    65,   249,  -440,  -440,    73,    77,  2491,  2491,
     216,  -440,  2491,   258,  -440,  -440,  2491,  2491,   192,  1462,
    -440,   160,  2491,  2491,  -440,   226,   262,  2155,  -440,   352,
     232,   236,  -440,  -440,  -440,  -440,  -440,  -440,   238,   240,
     241,  -440,  -440,   367,  -440,   -82,  3200,   177,   266,  -440,
     -71,  4041,  4208,  4208,   533,   533,  4190,  4190,   247,   676,
    -440,   -47,   -47,  4208,  4208,  3929,  4061,  4117,  4137,   533,
     533,  2617,  2617,   492,   492,   492,  -440,  3809,  -440,   -56,
    -440,  -440,  2491,  -440,  2898,  -440,  4041,  4041,  4041,  4041,
    4041,  4041,  4041,  4041,  4041,  4041,  4041,  4041,  4041,  4041,
    4041,  -440,  -440,    78,  2814,   278,  3583,  -440,  2491,  2491,
    -440,  2491,  -440,  2491,  2491,   266,   280,   253,   266,  2491,
     256,  2491,   260,  4041,  -440,  -440,  3985,   469,   469,    19,
    2491,  -440,  -440,  2975,   390,  4041,   469,  -440,   273,   292,
    4041,   261,  -440,  -440,  -440,  -440,  -440,   369,   192,  -440,
     160,   -60,   -34,  2491,  -440,  2267,  2491,  -440,  -440,  4041,
    -440,  -440,    49,   279,  -440,   533,   533,   533,   -85,  4041,
      76,   -60,  -440,   239,  4041,  -440,  -440,  -440,  -440,  -440,
      97,  2491,  -440,  3052,  -440,   160,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,   263,  4041,  -440,    81,  4041,  -440,  2379,
    2491,   269,  2491,   160,  2491,  -440,   -81,  -440,  -440,   839,
    -440,  -440,  2814,  3649,  3256,  -440,  4041,  -440,   469,   160,
    -440,    69,  -440,   368,   271,    97,  -440,  -440,  2491,   274,
    -440,  4041,  -440
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
      56,     0,    52,     0,     0,   269,     0,     0,     0,     0,
      28,     0,     0,    26,   272,     0,    13,     0,   127,     0,
       0,    59,   277,     0,     0,     0,     0,    65,    60,     0,
      48,    49,    46,    47,    45,     0,    44,    51,    33,    31,
      25,    24,     0,     0,     0,     0,   283,   303,   284,   308,
     285,   289,   290,   291,   292,   296,   297,   298,   299,   300,
     301,   302,   304,   305,   326,   288,   295,   306,   329,   332,
     286,   293,   287,   294,     0,   307,   310,   312,   311,   275,
     126,     0,    57,     0,   279,   245,    68,    69,    71,    70,
      72,    73,    74,    75,    94,    95,    92,    93,    85,    96,
      97,    86,    83,    84,    98,    99,   100,   101,    88,    89,
      87,    81,    82,    77,    76,    78,    79,    80,    67,    66,
      90,    91,     0,    63,   281,   271,   270,     0,    14,     0,
       0,   276,     0,     0,     0,     0,    60,    60,    60,     0,
     316,     0,   322,   321,   318,     0,    55,   278,   240,     0,
      61,   249,     0,   247,   256,     0,     0,   262,   263,     0,
     261,     0,     0,    15,    16,     0,   273,     0,     0,     0,
       0,    63,     0,    63,     0,    63,     0,   252,     0,   251,
     317,   319,     0,   187,   188,     0,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
     159,   158,   160,   161,   162,    17,     0,     0,     0,     0,
       0,     0,   314,     0,   121,    58,   185,   155,   183,   233,
     232,   231,   236,   184,   235,   234,     0,     0,   186,   336,
     337,   338,   339,     0,   280,   242,     0,   248,    62,     0,
     255,   254,     0,     0,    64,   102,   282,     0,   268,     0,
     163,     0,   323,     0,   325,   309,     0,   327,     0,   330,
       0,   333,   335,     0,   320,   315,   129,   132,     0,     0,
       0,     0,     0,     0,     0,   211,   212,     0,     0,     0,
       0,   355,     0,     0,   192,   191,   224,   219,   190,   189,
       0,     0,    60,   239,     0,     0,     0,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,   243,     0,   246,   250,
     257,   260,   259,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   267,   274,     0,   328,   331,   334,   253,     0,     0,
       0,     0,     0,     0,     0,   227,     0,     0,   125,     0,
     346,   342,     0,   349,   350,   351,     0,     0,     0,     0,
       0,    20,     0,    18,   215,   123,     0,     0,     0,     0,
     133,     0,     0,     0,   134,     0,     0,     0,   105,   103,
       0,     0,   116,   112,   110,   111,   122,   106,     0,     0,
       0,   115,   117,   141,   109,     0,   163,    63,     0,   220,
       0,   153,   193,   194,   206,   207,   204,   205,     0,   229,
     218,   237,   238,   195,   196,     0,   209,   210,   208,   202,
     203,   198,   197,   199,   200,   201,   217,     0,   222,     0,
     241,   244,     0,   266,   163,   124,   170,   171,   173,   172,
     174,   167,   168,   169,   175,   176,   165,   166,   177,   178,
     164,   324,   130,     0,     0,     0,     0,   225,     0,     0,
     226,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   356,   357,     0,     0,     0,     0,
       0,   135,   138,   163,     0,   128,     0,   120,     0,     0,
      36,     0,   118,   119,   108,   113,   114,     0,     0,   107,
       0,     0,     0,     0,   221,     0,     0,   216,   223,   258,
     265,   131,     0,     0,   149,   146,   147,   148,     0,   340,
       0,     0,   344,   343,   347,   353,   352,   354,    19,    42,
      37,     0,   139,   163,   136,     0,    43,    34,    35,   121,
     142,   145,   156,     0,   154,   180,     0,   230,   151,     0,
       0,     0,     0,     0,     0,    40,     0,   137,   140,     0,
     179,   181,     0,     0,   358,   345,   341,    38,     0,     0,
     104,     0,   150,     0,     0,    37,    41,   152,     0,     0,
      39,   359,   360
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -440,  -440,  -440,   105,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,   377,  -440,  -440,  -205,  -440,  -440,
    -440,  -440,   386,   -55,  -440,   381,  -440,   206,  -160,  -204,
    -440,  -440,   -50,  -440,  -142,  -189,   -11,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,   458,  -440,  -440,  -440,  -350,    87,
    -440,  -326,  -440,  -440,    96,  -440,  -440,  -440,  -440,   210,
    -440,   168,  -440,  -191,  -181,   178,   -95,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,   -67,   -54,  -440,  -440,   -45,
     242,  -439,   -61,  -440,  -440,  -232,   -65,   149,  -440,  -440,
    -440,  -440,  -440
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,    76,   323,   256,    14,    15,    31,
      32,    53,    16,    28,    29,   440,   441,   615,   442,   443,
     444,   445,    25,    26,    39,    40,    41,    17,   163,   196,
      68,    18,   257,   446,   331,   447,   258,   448,   259,   449,
     450,   451,   452,   558,   453,   454,   260,   261,   460,   262,
     263,   289,   264,   265,   461,   275,   367,   276,   188,   191,
     192,   217,   218,   524,   193,   202,   166,    69,    20,    82,
      21,    22,   124,    63,    23,   194,   267,   117,   118,   219,
     268,   411,   412,   269,   413,   414,   415,   416,   270,   322,
     271,   634,   272
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   116,   167,   283,   121,   455,   321,   296,   277,   298,
     119,   300,    36,   288,   489,    50,   211,   213,   215,   562,
     189,   375,   280,   164,    37,   180,   333,   170,   205,   220,
     181,    43,   189,    44,   523,   180,   591,   180,     2,   280,
     181,   563,   181,     3,   115,   563,    27,    60,   513,    42,
     281,   343,    47,    47,   345,   563,   280,   206,   334,    38,
      47,   165,     4,   197,     5,   171,     6,   281,     7,   198,
     563,   610,    61,     8,   559,   629,   180,   490,    57,     9,
     186,   181,   564,   199,   281,    10,   282,    70,    71,   583,
     199,   200,   526,   201,   371,    24,   180,   568,   182,   164,
      30,   181,   361,   362,    37,    51,   613,   197,   182,   280,
     182,   164,   187,   198,    33,   614,    52,   603,   116,   116,
     116,    34,   116,   116,   116,   116,   183,   207,   208,   209,
     184,   212,   214,   216,   221,   200,   183,   281,   183,    38,
     184,   116,   184,   199,   190,   199,   285,   116,   185,   182,
     284,    64,   222,   419,   293,   420,   287,   254,   185,   255,
     185,   115,   115,   115,   373,   115,   115,   115,   115,   182,
     273,   116,   457,   578,   203,   204,   302,   183,   274,   492,
     306,   184,   116,   116,   115,   493,   491,   534,    11,    72,
     115,   318,   319,    73,    12,    74,    75,   183,   303,   185,
     180,   184,   608,   180,   563,   181,   180,   563,   181,    35,
      45,   181,    65,    66,   115,   606,   527,   278,   365,   185,
     279,   528,   637,    48,   530,   115,   115,   611,   116,   531,
      46,   571,   528,   531,   621,   532,    49,   370,   327,   116,
     542,   616,    67,   168,   421,   422,    55,   180,   393,   116,
     407,    56,   181,   560,    57,   180,   116,   116,    54,   116,
     181,    58,   180,    59,    62,   401,   402,   181,   404,    78,
      80,   115,   180,   182,    81,   123,   182,   181,   120,   182,
     125,   266,   115,   160,   161,   162,   180,   168,   175,   172,
     173,   181,   115,   455,   174,   176,   177,   178,   290,   115,
     115,   183,   115,   179,   183,   184,   195,   183,   184,   210,
     292,   184,   164,   294,   286,   291,   180,   254,   304,   295,
     182,   181,   305,   185,   308,   398,   185,   309,   182,   185,
     315,   316,   310,   311,   320,   182,   312,   313,    11,   314,
     366,   368,   324,   325,   326,   182,   328,   329,   183,   330,
     364,   592,   184,   375,   297,   394,   183,   395,   392,   182,
     184,   539,   299,   183,   396,   526,   399,   184,   468,   301,
     185,   470,   486,   183,   535,   529,   548,   184,   185,   372,
     203,   544,   547,   551,   557,   185,   561,   183,   552,   182,
     612,   184,   553,   511,   554,   185,   555,   556,   573,   565,
     581,   595,   597,   600,   582,   400,   598,   585,   403,   185,
     406,   587,   638,   609,   410,   410,   620,   183,   599,   180,
     625,   184,   639,   518,   181,   642,    79,   456,   423,   180,
     640,   462,   463,    77,   181,   464,   465,   466,   467,   185,
     469,   122,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   619,   487,    19,
     116,   332,   495,   601,   580,   374,   586,   307,   417,   572,
     494,   397,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   589,   590,   369,
       0,   167,   182,     0,     0,   516,   596,     0,     0,     0,
       0,     0,   182,   115,     0,   330,     0,     0,     0,     0,
     602,     0,     0,     0,   533,   320,     0,     0,   536,     0,
     183,     0,   537,   538,   184,   543,   519,     0,   545,   546,
     183,     0,     0,   550,   184,     0,   521,     0,     0,     0,
       0,     0,   185,     0,     0,   618,     0,     0,     0,     0,
       0,     0,   185,     0,     0,     0,     0,   335,   336,   337,
     338,   339,     0,   627,   340,   341,   342,   343,   116,   344,
     345,   346,     0,   347,     0,   348,   349,   631,   635,   636,
       0,     0,   337,   338,     0,     0,     0,     0,   569,     0,
     343,     0,   344,   345,   346,     0,   347,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,   115,     0,     0,   575,   576,     0,   577,   361,   362,
     579,   335,   336,   337,   338,   584,   254,   410,     0,     0,
       0,   343,     0,   344,   345,   346,   593,   347,     0,   348,
     349,   361,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   607,     0,     0,     0,     0,     0,   356,   357,
     358,   359,   360,     0,     0,     0,     0,     0,     0,     0,
       0,   425,   361,   362,     0,     3,     0,   426,   427,     0,
     428,     0,   223,   224,   225,   226,     0,     0,     0,     0,
     429,   227,   430,   431,     0,   623,   624,     0,   626,     0,
     628,     0,   432,   228,     0,   456,   433,   229,     0,   230,
       0,     9,   231,   434,     0,   435,   232,     0,     0,   436,
     437,    86,    87,    88,   641,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
     105,   106,   107,     0,   233,   110,   111,   112,   113,     0,
       0,     0,     0,     0,     0,     0,   337,   338,     0,     0,
     234,   235,     0,     0,   343,     0,     0,   345,   346,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,   237,   238,   239,   240,   241,   242,   243,   244,
      37,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,   248,     0,     0,
     249,     0,     0,   250,   251,   361,   362,     0,     0,     0,
       0,     0,   253,     0,     0,    38,   438,   254,   439,   255,
     425,     0,     0,     0,     3,     0,   426,   427,     0,   428,
       0,   223,   224,   225,   226,     0,     0,     0,     0,   429,
     227,   430,   431,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   228,     0,     0,   433,   229,     0,   230,     0,
       9,   231,   434,     0,   435,   232,     0,     0,   436,   437,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   233,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   244,    37,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,   248,     0,     0,   249,
       0,     0,   250,   251,   223,   224,   225,   226,     0,     0,
       0,   253,     0,   227,    38,   438,   254,   630,   255,   180,
       0,     0,     0,     0,   181,   228,     0,     0,     0,   229,
       0,   230,     0,     0,   231,     0,     0,     0,   232,     0,
       0,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   233,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   408,   245,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   184,     0,     0,   246,   247,   248,
       0,     0,   249,     0,     0,   250,   251,   223,   224,   225,
     226,     0,   185,     0,   409,     0,   227,    38,     0,   254,
       0,   255,   180,     0,     0,     0,     0,   181,   228,     0,
       0,     0,   229,     0,   230,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   233,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,   235,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,   241,   242,   243,   244,    37,   245,     0,     0,   223,
     224,   225,   226,   183,     0,     0,     0,   184,   227,     0,
     246,   247,   248,     0,     0,   249,     0,     0,   250,   251,
     228,     0,     0,     0,   229,   185,   230,   253,     0,   231,
      38,     0,   254,   232,   255,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   233,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,   237,
     238,   239,   240,   241,   242,   243,   244,    37,   245,     0,
       0,   223,   224,   225,   226,   514,     0,     0,     0,     0,
     227,     0,   246,   247,   248,     0,     0,   249,     0,     0,
     250,   251,   228,     0,     0,     0,   229,   458,   230,   253,
     459,   231,    38,     0,   254,   232,   255,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   233,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   244,    37,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   515,     0,   246,   247,   248,     0,     0,   249,
       0,     0,   250,   251,   223,   224,   225,   226,     0,     0,
       0,   253,     0,   227,    38,     0,   254,     0,   255,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,   229,
       0,   230,     0,     0,   231,     0,     0,     0,   232,     0,
       0,     0,     0,    86,    87,    88,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,   105,   106,   107,     0,   233,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   235,     0,     0,     0,     0,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,   238,   239,   240,   241,   242,
     243,   244,    37,   245,     0,     0,   223,   224,   225,   226,
       0,     0,     0,     0,     0,   227,     0,   246,   247,   248,
       0,     0,   249,     0,     0,   250,   251,   228,     0,     0,
       0,   229,     0,   230,   253,     0,   231,    38,   541,   254,
     232,   255,     0,     0,     0,    86,    87,    88,     0,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,     0,   233,   110,
     111,   112,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   234,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   237,   238,   239,   240,
     241,   242,   243,   244,    37,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,   248,     0,     0,   249,     0,     0,   250,   251,   223,
     224,   225,   226,     0,     0,   252,   253,     0,   227,    38,
       0,   254,     0,   255,     0,     0,     0,     0,     0,     0,
     228,     0,     0,     0,   229,     0,   230,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   233,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,   237,
     238,   239,   240,   241,   242,   243,   244,    37,   245,     0,
       0,   223,   224,   225,   226,     0,     0,     0,     0,     0,
     227,     0,   246,   247,   248,     0,     0,   249,     0,     0,
     250,   251,   228,     0,     0,     0,   229,     0,   230,   253,
     405,   231,    38,     0,   254,   232,   255,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   233,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   244,    37,
     245,     0,     0,   223,   224,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   246,   247,   248,     0,     0,   249,
       0,     0,   250,   251,   228,     0,     0,     0,   229,     0,
     230,   253,   488,   231,    38,     0,   254,   232,   255,     0,
       0,     0,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   233,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,   238,   239,   240,   241,   242,   243,
     244,    37,   245,     0,     0,   223,   224,   225,   226,     0,
       0,     0,     0,     0,   227,     0,   246,   247,   248,     0,
       0,   249,     0,     0,   250,   251,   228,     0,     0,     0,
     229,     0,   230,   253,   512,   231,    38,     0,   254,   232,
     255,     0,     0,     0,    86,    87,    88,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,   105,   106,   107,     0,   233,   110,   111,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,    37,   245,     0,     0,   223,   224,   225,
     226,     0,     0,     0,     0,     0,   227,     0,   246,   247,
     248,     0,     0,   249,     0,     0,   250,   251,   228,     0,
       0,     0,   229,     0,   230,   253,   525,   231,    38,     0,
     254,   232,   255,     0,   549,     0,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,   105,   106,   107,     0,   233,
     110,   111,   112,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,   241,   242,   243,   244,    37,   245,     0,     0,   223,
     224,   225,   226,     0,     0,     0,     0,     0,   227,     0,
     246,   247,   248,     0,     0,   249,     0,     0,   250,   251,
     228,     0,     0,     0,   229,     0,   230,   253,     0,   231,
      38,     0,   254,   232,   255,     0,     0,     0,    86,    87,
      88,     0,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,   105,   106,   107,
       0,   233,   110,   111,   112,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,   237,
     238,   239,   240,   241,   242,   243,   244,    37,   245,     0,
       0,   223,   224,   225,   226,   622,     0,     0,     0,     0,
     227,     0,   246,   247,   248,     0,     0,   249,     0,     0,
     250,   251,   228,     0,     0,     0,   229,     0,   230,   253,
     605,   231,    38,     0,   254,   232,   255,     0,     0,     0,
      86,    87,    88,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,   105,
     106,   107,     0,   233,   110,   111,   112,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   244,    37,
     245,     0,     0,   223,   224,   225,   226,     0,     0,     0,
       0,     0,   227,     0,   246,   247,   248,     0,     0,   249,
       0,     0,   250,   251,   228,     0,     0,     0,   229,     0,
     230,   253,     0,   231,    38,     0,   254,   232,   255,     0,
       0,     0,    86,    87,    88,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,   105,   106,   107,     0,   233,   110,   111,   112,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,   238,   239,   240,   241,   242,   243,
     244,    37,   245,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,   246,   247,   248,    84,
       0,   249,     0,     0,   250,   251,     0,     0,     0,     0,
       0,     0,     0,   253,    85,     0,    38,     0,   254,     0,
     255,     0,     0,     0,     0,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,   337,   338,     0,
       0,     0,     0,     0,    85,   343,     0,   344,   345,   346,
       0,   347,     0,     0,    37,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   358,   359,   360,     0,     0,    38,
       0,     0,     0,    83,     0,     0,   361,   362,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,   236,   237,   238,     0,     0,
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
       0,   376,   377,   378,   379,   380,   381,   382,   383,   335,
     336,   337,   338,   339,   384,   385,   340,   341,   342,   343,
     386,   344,   345,   346,     0,   347,   387,   348,   349,   388,
     389,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   390,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,    38,
     361,   362,     0,     0,     0,     0,     0,   391,   376,   377,
     378,   379,   380,   381,   382,   383,   335,   336,   337,   338,
     339,   384,   385,   340,   341,   342,   343,   386,   344,   345,
     346,     0,   347,   387,   348,   349,   388,   389,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   390,   350,     0,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,   362,     0,
       0,     0,     0,     0,   570,   376,   377,   378,   379,   380,
     381,   382,   383,   335,   336,   337,   338,   339,   384,   385,
     340,   341,   342,   343,   386,   344,   345,   346,     0,   347,
     387,   348,   349,   388,   389,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   362,     0,     0,     0,     0,
       0,   594,   376,   377,   378,   379,   380,   381,   382,   383,
     335,   336,   337,   338,   339,   384,   385,   340,   341,   342,
     343,   386,   344,   345,   346,     0,   347,   387,   348,   349,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     350,     0,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,   362,     0,     0,     0,     0,     0,   617,   376,
     377,   378,   379,   380,   381,   382,   383,   335,   336,   337,
     338,   339,   384,   385,   340,   341,   342,   343,   386,   344,
     345,   346,  -163,   347,   387,   348,   349,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   390,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     376,   377,   378,   379,   380,   381,   382,   383,   335,   336,
     337,   338,   339,   384,   385,   340,   341,   342,   343,   386,
     344,   345,   346,     0,   347,   387,   348,   349,   388,   389,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   390,   350,     0,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,     0,     0,     0,   335,   336,   337,   338,   339,   361,
     362,   340,   341,   342,   343,     0,   344,   345,   346,     0,
     347,     0,   348,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   362,   335,   336,   337,
     338,   339,   633,     0,   340,   341,   342,   343,     0,   344,
     345,   346,     0,   347,     0,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,   424,   335,   336,   337,   338,   339,     0,     0,
     340,   341,   342,   343,     0,   344,   345,   346,     0,   347,
       0,   348,   349,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   362,     0,     0,   517,   335,
     336,   337,   338,   339,     0,     0,   340,   341,   342,   343,
       0,   344,   345,   346,     0,   347,     0,   348,   349,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,   520,   335,   336,   337,   338,   339,
       0,     0,   340,   341,   342,   343,     0,   344,   345,   346,
       0,   347,     0,   348,   349,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,     0,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,     0,     0,
     522,   335,   336,   337,   338,   339,     0,     0,   340,   341,
     342,   343,     0,   344,   345,   346,     0,   347,     0,   348,
     349,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   361,   362,     0,     0,   574,   335,   336,   337,
     338,   339,     0,     0,   340,   341,   342,   343,     0,   344,
     345,   346,     0,   347,     0,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,   632,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   144,
     145,   146,   147,   335,   336,   337,   338,   339,     0,     0,
     340,   341,   342,   343,     0,   344,   345,   346,     0,   347,
       0,   348,   349,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,     0,     0,     0,   158,   159,     0,     0,
       0,     0,     0,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   362,   363,   335,   336,   337,
     338,   339,     0,     0,   340,   341,   342,   343,     0,   344,
     345,   346,     0,   347,     0,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     567,   335,   336,   337,   338,   339,     0,     0,   340,   341,
     342,   343,     0,   344,   345,   346,     0,   347,     0,   348,
     349,     0,     0,   418,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,     0,     0,     0,   335,   336,   337,
     338,   339,   361,   362,   340,   341,   342,   343,     0,   344,
     345,   346,     0,   347,     0,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,   566,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,   335,   336,   337,   338,   339,   361,   362,
     340,   341,   342,   343,     0,   344,   345,   346,     0,   347,
       0,   348,   349,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     588,     0,     0,   350,     0,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,     0,     0,     0,     0,   335,
     336,   337,   338,   339,   361,   362,   340,   341,   342,   343,
       0,   344,   345,   346,     0,   347,     0,   348,   349,   335,
     336,   337,   338,   339,     0,     0,   340,   341,   342,   343,
       0,   344,   345,   346,     0,   347,     0,   348,   349,   350,
       0,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,     0,     0,     0,   335,   336,   337,   338,   339,
     361,   362,   340,   341,   342,   343,     0,   344,   345,   346,
       0,   347,     0,   348,   349,   335,   336,   337,   338,   339,
       0,     0,   340,   341,   342,   343,     0,   344,   345,   346,
       0,   347,     0,   348,   349,     0,     0,     0,     0,   353,
     354,   355,   356,   357,   358,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,     0,     0,
     354,   355,   356,   357,   358,   359,   360,     0,   335,   336,
     337,   338,   339,     0,     0,   340,   361,   362,   343,     0,
     344,   345,   346,     0,   347,     0,   348,   349,   337,   338,
       0,     0,     0,     0,     0,     0,   343,     0,   344,   345,
     346,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,   355,   356,   357,   358,   359,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,   356,   357,   358,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,   362
};

static const yytype_int16 yycheck[] =
{
      11,    55,    69,   194,    59,   331,   238,   211,   189,   213,
      55,   215,    28,   202,   364,    48,   176,   177,   178,   458,
      41,   103,    99,   120,   120,    27,   120,   120,   127,    27,
      32,     4,    41,     6,    17,    27,    17,    27,     0,    99,
      32,   126,    32,     5,    55,   126,   114,   126,   398,    60,
     127,    98,   126,   126,   101,   126,    99,   156,   152,   155,
     126,   158,    24,    99,    26,   158,    28,   127,    30,   105,
     126,   156,   151,    35,   156,   156,    27,   151,   155,    41,
     153,    32,   153,   126,   127,    47,   129,    12,    13,   528,
     126,   127,   126,   129,   283,   120,    27,   153,   100,   120,
     120,    32,   149,   150,   120,   138,     9,    99,   100,    99,
     100,   120,   123,   105,   120,    18,   149,   151,   172,   173,
     174,   120,   176,   177,   178,   179,   128,   172,   173,   174,
     132,   176,   177,   178,   132,   127,   128,   127,   128,   155,
     132,   195,   132,   126,   153,   126,   196,   201,   150,   100,
     195,    29,   150,   156,   156,   158,   201,   157,   150,   159,
     150,   172,   173,   174,   156,   176,   177,   178,   179,   100,
     150,   225,   332,   523,   122,   123,   134,   128,   158,   370,
     225,   132,   236,   237,   195,   374,   367,   419,   150,   114,
     201,   236,   237,   118,   156,   120,   121,   128,   156,   150,
      27,   132,   153,    27,   126,    32,    27,   126,    32,   120,
     157,    32,    90,    91,   225,   565,   151,   153,   273,   150,
     156,   156,   153,   129,   151,   236,   237,   151,   282,   156,
     127,   153,   156,   156,   153,   158,   126,   282,   249,   293,
     429,   591,   120,   122,   123,   124,   127,    27,   293,   303,
     317,   156,    32,   457,   155,    27,   310,   311,   157,   313,
      32,   120,    27,   152,   120,   310,   311,    32,   313,   114,
     120,   282,    27,   100,   120,   129,   100,    32,   120,   100,
     157,   185,   293,    29,    29,   152,    27,   122,   152,   133,
     133,    32,   303,   619,   133,   133,   133,   133,   202,   310,
     311,   128,   313,   133,   128,   132,   129,   128,   132,   120,
     134,   132,   120,   134,   120,   114,    27,   157,   132,   153,
     100,    32,   151,   150,   152,   152,   150,   152,   100,   150,
     234,   235,   133,   133,   238,   100,   152,   133,   150,   152,
      38,   156,   246,   247,   248,   100,   250,   251,   128,   253,
     152,   540,   132,   103,   134,   134,   128,   134,   156,   100,
     132,   428,   134,   128,   134,   126,   120,   132,   120,   134,
     150,   120,   120,   128,   158,   126,   114,   132,   150,   283,
     122,   431,   156,    31,    17,   150,   120,   128,   156,   100,
     581,   132,   156,   134,   156,   150,   156,   156,   120,   152,
     120,    11,   129,    34,   151,   309,   114,   151,   312,   150,
     314,   151,    44,   134,   318,   319,   153,   128,   157,    27,
     151,   132,   151,   134,    32,   151,    49,   331,   323,    27,
     635,   335,   336,    47,    32,   339,   340,   341,   342,   150,
     344,    60,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   599,   362,     1,
     514,   255,   375,   558,   525,   287,   531,   225,   319,   514,
     374,   303,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   537,   538,   279,
      -1,   558,   100,    -1,    -1,   399,   546,    -1,    -1,    -1,
      -1,    -1,   100,   514,    -1,   409,    -1,    -1,    -1,    -1,
     560,    -1,    -1,    -1,   418,   419,    -1,    -1,   422,    -1,
     128,    -1,   426,   427,   132,   429,   134,    -1,   432,   433,
     128,    -1,    -1,   437,   132,    -1,   134,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,   595,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,    -1,   613,    95,    96,    97,    98,   622,   100,
     101,   102,    -1,   104,    -1,   106,   107,   622,   628,   629,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,   492,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,   622,    -1,    -1,   518,   519,    -1,   521,   149,   150,
     524,    88,    89,    90,    91,   529,   157,   531,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,   540,   104,    -1,   106,
     107,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   563,
      -1,    -1,   566,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,   149,   150,    -1,     5,    -1,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    -1,   609,   610,    -1,   612,    -1,
     614,    -1,    32,    33,    -1,   619,    36,    37,    -1,    39,
      -1,    41,    42,    43,    -1,    45,    46,    -1,    -1,    49,
      50,    51,    52,    53,   638,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      90,    91,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,   144,   149,   150,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,   159,
       1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    36,    37,    -1,    39,    -1,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   136,   137,    -1,    -1,   140,
      -1,    -1,   143,   144,    12,    13,    14,    15,    -1,    -1,
      -1,   152,    -1,    21,   155,   156,   157,   158,   159,    27,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,    -1,   135,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,   144,    12,    13,    14,
      15,    -1,   150,    -1,   152,    -1,    21,   155,    -1,   157,
      -1,   159,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    12,
      13,    14,    15,   128,    -1,    -1,    -1,   132,    21,    -1,
     135,   136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,
      33,    -1,    -1,    -1,    37,   150,    39,   152,    -1,    42,
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
     143,   144,    33,    -1,    -1,    -1,    37,   150,    39,   152,
     153,    42,   155,    -1,   157,    46,   159,    -1,    -1,    -1,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,   137,    -1,    -1,   140,
      -1,    -1,   143,   144,    12,    13,    14,    15,    -1,    -1,
      -1,   152,    -1,    21,   155,    -1,   157,    -1,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    51,    52,    53,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,   135,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,   144,    33,    -1,    -1,
      -1,    37,    -1,    39,   152,    -1,    42,   155,   156,   157,
      46,   159,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,    -1,    -1,   140,    -1,    -1,   143,   144,    12,
      13,    14,    15,    -1,    -1,   151,   152,    -1,    21,   155,
      -1,   157,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    51,    52,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    40,    98,    -1,   100,   101,   102,
      -1,   104,    -1,    -1,   120,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,   137,   138,   139,    -1,    -1,   155,
      -1,    -1,    -1,    19,    -1,    -1,   149,   150,    -1,    25,
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
      91,    92,   156,    -1,    95,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,   153,    88,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,    -1,   153,    88,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,    -1,   106,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,   153,    88,    89,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
     153,    88,    89,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,   153,    88,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,   153,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,    88,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
      -1,   106,   107,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,   143,   144,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,    88,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,    88,    89,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,    -1,   106,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    92,   149,   150,    95,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,   149,   150,
      95,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    92,   149,   150,    95,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,    -1,   106,   107,    88,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,    -1,   106,   107,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
     149,   150,    95,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,    -1,   106,   107,    88,    89,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,    -1,   106,   107,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
     133,   134,   135,   136,   137,   138,   139,    -1,    88,    89,
      90,    91,    92,    -1,    -1,    95,   149,   150,    98,    -1,
     100,   101,   102,    -1,   104,    -1,   106,   107,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   161,     0,     5,    24,    26,    28,    30,    35,    41,
      47,   150,   156,   162,   167,   168,   172,   187,   191,   204,
     228,   230,   231,   234,   120,   182,   183,   114,   173,   174,
     120,   169,   170,   120,   120,   120,    28,   120,   155,   184,
     185,   186,   196,     4,     6,   157,   127,   126,   129,   126,
      48,   138,   149,   171,   157,   127,   156,   155,   120,   152,
     126,   151,   120,   233,    29,    90,    91,   120,   190,   227,
      12,    13,   114,   118,   120,   121,   164,   182,   114,   174,
     120,   120,   229,    19,    25,    40,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,   196,   236,   237,   238,   239,
     120,   183,   185,   129,   232,   157,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   106,   107,   108,   109,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   143,   144,
      29,    29,   152,   188,   120,   158,   226,   235,   122,   163,
     120,   158,   133,   133,   133,   152,   133,   133,   133,   133,
      27,    32,   100,   128,   132,   150,   153,   196,   218,    41,
     153,   219,   220,   224,   235,   129,   189,    99,   105,   126,
     127,   129,   225,   122,   123,   127,   156,   239,   239,   239,
     120,   188,   239,   188,   239,   188,   239,   221,   222,   239,
      27,   132,   150,    12,    13,    14,    15,    21,    33,    37,
      39,    42,    46,    74,    90,    91,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   121,   135,   136,   137,   140,
     143,   144,   151,   152,   157,   159,   166,   192,   196,   198,
     206,   207,   209,   210,   212,   213,   214,   236,   240,   243,
     248,   250,   252,   150,   158,   215,   217,   224,   153,   156,
      99,   127,   129,   223,   239,   192,   120,   239,   195,   211,
     214,   114,   134,   156,   134,   153,   189,   134,   189,   134,
     189,   134,   134,   156,   132,   151,   239,   240,   152,   152,
     133,   133,   152,   133,   152,   214,   214,    10,   239,   239,
     214,   245,   249,   165,   214,   214,   214,   196,   214,   214,
     214,   194,   187,   120,   152,    88,    89,    90,    91,    92,
      95,    96,    97,    98,   100,   101,   102,   104,   106,   107,
     128,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   149,   150,   151,   152,   183,    38,   216,   156,   219,
     239,   195,   214,   156,   225,   103,    80,    81,    82,    83,
      84,    85,    86,    87,    93,    94,    99,   105,   108,   109,
     127,   156,   156,   239,   134,   134,   134,   221,   152,   120,
     214,   239,   239,   214,   239,   153,   214,   235,   120,   152,
     214,   241,   242,   244,   245,   246,   247,   247,   110,   156,
     158,   123,   124,   163,   153,     1,     7,     8,    10,    20,
      22,    23,    32,    36,    43,    45,    49,    50,   156,   158,
     175,   176,   178,   179,   180,   181,   193,   195,   197,   199,
     200,   201,   202,   204,   205,   211,   214,   188,   150,   153,
     208,   214,   214,   214,   214,   214,   214,   214,   120,   214,
     120,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   120,   214,   153,   208,
     151,   224,   223,   195,   214,   209,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   134,   153,   208,    16,   133,   214,   153,   134,   134,
     153,   134,   153,    17,   223,   153,   126,   151,   156,   126,
     151,   156,   158,   214,   245,   158,   214,   214,   214,   235,
      99,   156,   195,   214,   192,   214,   214,   156,   114,    49,
     214,    31,   156,   156,   156,   156,   156,    17,   203,   156,
     189,   120,   241,   126,   153,   152,   129,   151,   153,   214,
     156,   153,   239,   120,   153,   214,   214,   214,   208,   214,
     242,   120,   151,   241,   214,   151,   246,   151,   125,   192,
     192,    17,   195,   214,   156,    11,   192,   129,   114,   157,
      34,   226,   192,   151,   214,   153,   208,   214,   153,   134,
     156,   151,   223,     9,    18,   177,   208,   156,   192,   194,
     153,   153,    16,   214,   214,   151,   214,   192,   214,   156,
     158,   239,   153,   156,   251,   192,   192,   153,    44,   151,
     177,   214,   151
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   160,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   163,   163,   164,   165,   165,   165,
     166,   167,   168,   169,   169,   169,   170,   171,   171,   172,
     173,   173,   174,   174,   175,   176,   176,   177,   177,   177,
     178,   179,   180,   181,   182,   182,   182,   182,   182,   182,
     183,   183,   184,   184,   185,   185,   186,   186,   187,   187,
     188,   188,   188,   189,   189,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   192,   192,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   194,   194,   194,   195,   196,   196,   196,   197,   198,
     198,   198,   198,   199,   200,   201,   201,   201,   201,   201,
     202,   203,   203,   204,   204,   205,   206,   206,   206,   207,
     207,   207,   207,   208,   208,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   212,
     213,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     215,   215,   216,   216,   217,   218,   218,   219,   219,   220,
     220,   221,   222,   222,   223,   223,   224,   224,   224,   224,
     224,   225,   225,   225,   226,   226,   226,   226,   226,   227,
     227,   228,   229,   229,   229,   230,   231,   232,   232,   233,
     234,   235,   235,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   238,   238,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   240,   240,
     241,   241,   242,   242,   243,   243,   244,   244,   245,   246,
     246,   247,   247,   248,   248,   249,   249,   250,   251,   251,
     252
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
       8,     6,     9,     1,     3,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       5,     6,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     3,     3,     2,
       3,     4,     3,     4,     2,     4,     4,     3,     4,     3,
       5,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     0,     1,     3,     0,     3,     1,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     3,     5,     3,
       3,     1,     1,     1,     4,     6,     5,     4,     3,     0,
       2,     4,     0,     3,     5,     4,     5,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     3,     4,     2,     3,     2,     3,
       4,     2,     2,     4,     6,     4,     1,     4,     5,     1,
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
    case 120: /* "name"  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2545 "ds_parser.cpp"
        break;

    case 163: /* character_sequence  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2551 "ds_parser.cpp"
        break;

    case 164: /* string_constant  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2557 "ds_parser.cpp"
        break;

    case 165: /* string_builder_body  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2563 "ds_parser.cpp"
        break;

    case 166: /* string_builder  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2569 "ds_parser.cpp"
        break;

    case 169: /* require_module_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2575 "ds_parser.cpp"
        break;

    case 175: /* expression_label  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2581 "ds_parser.cpp"
        break;

    case 176: /* expression_goto  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2587 "ds_parser.cpp"
        break;

    case 177: /* expression_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2593 "ds_parser.cpp"
        break;

    case 178: /* expression_if_then_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2599 "ds_parser.cpp"
        break;

    case 179: /* expression_for_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2605 "ds_parser.cpp"
        break;

    case 180: /* expression_while_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2611 "ds_parser.cpp"
        break;

    case 181: /* expression_with  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2617 "ds_parser.cpp"
        break;

    case 182: /* annotation_argument  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2623 "ds_parser.cpp"
        break;

    case 183: /* annotation_argument_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2629 "ds_parser.cpp"
        break;

    case 184: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2635 "ds_parser.cpp"
        break;

    case 185: /* annotation_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2641 "ds_parser.cpp"
        break;

    case 186: /* annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2647 "ds_parser.cpp"
        break;

    case 187: /* optional_annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2653 "ds_parser.cpp"
        break;

    case 188: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2659 "ds_parser.cpp"
        break;

    case 189: /* optional_function_type  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2665 "ds_parser.cpp"
        break;

    case 190: /* function_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2671 "ds_parser.cpp"
        break;

    case 192: /* expression_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2677 "ds_parser.cpp"
        break;

    case 193: /* expression_any  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2683 "ds_parser.cpp"
        break;

    case 194: /* expressions  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2689 "ds_parser.cpp"
        break;

    case 195: /* expr_pipe  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2695 "ds_parser.cpp"
        break;

    case 196: /* name_in_namespace  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2701 "ds_parser.cpp"
        break;

    case 197: /* expression_delete  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2707 "ds_parser.cpp"
        break;

    case 198: /* expr_new  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2713 "ds_parser.cpp"
        break;

    case 199: /* expression_break  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2719 "ds_parser.cpp"
        break;

    case 200: /* expression_continue  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2725 "ds_parser.cpp"
        break;

    case 201: /* expression_return  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2731 "ds_parser.cpp"
        break;

    case 202: /* expression_try_catch  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2737 "ds_parser.cpp"
        break;

    case 205: /* expression_let  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2743 "ds_parser.cpp"
        break;

    case 206: /* expr_cast  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2749 "ds_parser.cpp"
        break;

    case 207: /* expr_type_info  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2755 "ds_parser.cpp"
        break;

    case 208: /* expr_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2761 "ds_parser.cpp"
        break;

    case 209: /* expr_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2767 "ds_parser.cpp"
        break;

    case 210: /* expr_numeric_const  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2773 "ds_parser.cpp"
        break;

    case 211: /* expr_assign  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2779 "ds_parser.cpp"
        break;

    case 212: /* expr_named_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2785 "ds_parser.cpp"
        break;

    case 213: /* expr_method_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2791 "ds_parser.cpp"
        break;

    case 214: /* expr  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2797 "ds_parser.cpp"
        break;

    case 215: /* optional_field_annotation  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2803 "ds_parser.cpp"
        break;

    case 217: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2809 "ds_parser.cpp"
        break;

    case 218: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2815 "ds_parser.cpp"
        break;

    case 219: /* function_argument_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2821 "ds_parser.cpp"
        break;

    case 220: /* function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2827 "ds_parser.cpp"
        break;

    case 221: /* tuple_type  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2833 "ds_parser.cpp"
        break;

    case 222: /* tuple_type_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2839 "ds_parser.cpp"
        break;

    case 224: /* variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2845 "ds_parser.cpp"
        break;

    case 226: /* let_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2851 "ds_parser.cpp"
        break;

    case 227: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2857 "ds_parser.cpp"
        break;

    case 229: /* enum_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 2863 "ds_parser.cpp"
        break;

    case 232: /* optional_structure_parent  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2869 "ds_parser.cpp"
        break;

    case 235: /* variable_name_list  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 2875 "ds_parser.cpp"
        break;

    case 237: /* structure_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2881 "ds_parser.cpp"
        break;

    case 238: /* auto_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2887 "ds_parser.cpp"
        break;

    case 239: /* type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2893 "ds_parser.cpp"
        break;

    case 240: /* make_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2899 "ds_parser.cpp"
        break;

    case 241: /* make_struct_fields  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 2905 "ds_parser.cpp"
        break;

    case 242: /* make_struct_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2911 "ds_parser.cpp"
        break;

    case 243: /* make_struct_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2917 "ds_parser.cpp"
        break;

    case 244: /* make_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2923 "ds_parser.cpp"
        break;

    case 245: /* make_map_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2929 "ds_parser.cpp"
        break;

    case 246: /* make_any_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2935 "ds_parser.cpp"
        break;

    case 247: /* make_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2941 "ds_parser.cpp"
        break;

    case 248: /* make_dim_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2947 "ds_parser.cpp"
        break;

    case 249: /* make_table  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2953 "ds_parser.cpp"
        break;

    case 250: /* make_table_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2959 "ds_parser.cpp"
        break;

    case 251: /* array_comprehension_where  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2965 "ds_parser.cpp"
        break;

    case 252: /* array_comprehension  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2971 "ds_parser.cpp"
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
#line 369 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3271 "ds_parser.cpp"
    break;

  case 14:
#line 381 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3277 "ds_parser.cpp"
    break;

  case 15:
#line 382 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3283 "ds_parser.cpp"
    break;

  case 16:
#line 386 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3289 "ds_parser.cpp"
    break;

  case 17:
#line 390 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3298 "ds_parser.cpp"
    break;

  case 18:
#line 394 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3312 "ds_parser.cpp"
    break;

  case 19:
#line 403 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3322 "ds_parser.cpp"
    break;

  case 20:
#line 411 "ds_parser.ypp"
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
#line 3340 "ds_parser.cpp"
    break;

  case 21:
#line 427 "ds_parser.ypp"
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
#line 3363 "ds_parser.cpp"
    break;

  case 23:
#line 452 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3371 "ds_parser.cpp"
    break;

  case 24:
#line 455 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3382 "ds_parser.cpp"
    break;

  case 25:
#line 461 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3393 "ds_parser.cpp"
    break;

  case 26:
#line 470 "ds_parser.ypp"
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
#line 3408 "ds_parser.cpp"
    break;

  case 27:
#line 483 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3414 "ds_parser.cpp"
    break;

  case 28:
#line 484 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3420 "ds_parser.cpp"
    break;

  case 32:
#line 497 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3428 "ds_parser.cpp"
    break;

  case 33:
#line 500 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3436 "ds_parser.cpp"
    break;

  case 34:
#line 506 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3444 "ds_parser.cpp"
    break;

  case 35:
#line 512 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3452 "ds_parser.cpp"
    break;

  case 36:
#line 515 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3460 "ds_parser.cpp"
    break;

  case 37:
#line 521 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3466 "ds_parser.cpp"
    break;

  case 38:
#line 522 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3472 "ds_parser.cpp"
    break;

  case 39:
#line 523 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3481 "ds_parser.cpp"
    break;

  case 40:
#line 530 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3490 "ds_parser.cpp"
    break;

  case 41:
#line 537 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3504 "ds_parser.cpp"
    break;

  case 42:
#line 549 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3516 "ds_parser.cpp"
    break;

  case 43:
#line 559 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3527 "ds_parser.cpp"
    break;

  case 44:
#line 568 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3533 "ds_parser.cpp"
    break;

  case 45:
#line 569 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3539 "ds_parser.cpp"
    break;

  case 46:
#line 570 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3545 "ds_parser.cpp"
    break;

  case 47:
#line 571 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3551 "ds_parser.cpp"
    break;

  case 48:
#line 572 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3557 "ds_parser.cpp"
    break;

  case 49:
#line 573 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3563 "ds_parser.cpp"
    break;

  case 50:
#line 577 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3573 "ds_parser.cpp"
    break;

  case 51:
#line 582 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3583 "ds_parser.cpp"
    break;

  case 52:
#line 590 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 3589 "ds_parser.cpp"
    break;

  case 53:
#line 591 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 3595 "ds_parser.cpp"
    break;

  case 54:
#line 595 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3607 "ds_parser.cpp"
    break;

  case 55:
#line 602 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3621 "ds_parser.cpp"
    break;

  case 56:
#line 614 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3630 "ds_parser.cpp"
    break;

  case 57:
#line 618 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3639 "ds_parser.cpp"
    break;

  case 58:
#line 625 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 3645 "ds_parser.cpp"
    break;

  case 59:
#line 626 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3651 "ds_parser.cpp"
    break;

  case 60:
#line 630 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3657 "ds_parser.cpp"
    break;

  case 61:
#line 631 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3663 "ds_parser.cpp"
    break;

  case 62:
#line 632 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3669 "ds_parser.cpp"
    break;

  case 63:
#line 636 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3678 "ds_parser.cpp"
    break;

  case 64:
#line 640 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3687 "ds_parser.cpp"
    break;

  case 65:
#line 647 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3696 "ds_parser.cpp"
    break;

  case 66:
#line 651 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 3702 "ds_parser.cpp"
    break;

  case 67:
#line 652 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 3708 "ds_parser.cpp"
    break;

  case 68:
#line 653 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 3714 "ds_parser.cpp"
    break;

  case 69:
#line 654 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 3720 "ds_parser.cpp"
    break;

  case 70:
#line 655 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 3726 "ds_parser.cpp"
    break;

  case 71:
#line 656 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 3732 "ds_parser.cpp"
    break;

  case 72:
#line 657 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 3738 "ds_parser.cpp"
    break;

  case 73:
#line 658 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 3744 "ds_parser.cpp"
    break;

  case 74:
#line 659 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 3750 "ds_parser.cpp"
    break;

  case 75:
#line 660 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 3756 "ds_parser.cpp"
    break;

  case 76:
#line 661 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 3762 "ds_parser.cpp"
    break;

  case 77:
#line 662 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 3768 "ds_parser.cpp"
    break;

  case 78:
#line 663 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 3774 "ds_parser.cpp"
    break;

  case 79:
#line 664 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 3780 "ds_parser.cpp"
    break;

  case 80:
#line 665 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 3786 "ds_parser.cpp"
    break;

  case 81:
#line 666 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 3792 "ds_parser.cpp"
    break;

  case 82:
#line 667 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 3798 "ds_parser.cpp"
    break;

  case 83:
#line 668 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 3804 "ds_parser.cpp"
    break;

  case 84:
#line 669 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 3810 "ds_parser.cpp"
    break;

  case 85:
#line 670 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 3816 "ds_parser.cpp"
    break;

  case 86:
#line 671 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 3822 "ds_parser.cpp"
    break;

  case 87:
#line 672 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 3828 "ds_parser.cpp"
    break;

  case 88:
#line 673 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 3834 "ds_parser.cpp"
    break;

  case 89:
#line 674 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 3840 "ds_parser.cpp"
    break;

  case 90:
#line 675 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 3846 "ds_parser.cpp"
    break;

  case 91:
#line 676 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 3852 "ds_parser.cpp"
    break;

  case 92:
#line 677 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 3858 "ds_parser.cpp"
    break;

  case 93:
#line 678 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 3864 "ds_parser.cpp"
    break;

  case 94:
#line 679 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 3870 "ds_parser.cpp"
    break;

  case 95:
#line 680 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 3876 "ds_parser.cpp"
    break;

  case 96:
#line 681 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 3882 "ds_parser.cpp"
    break;

  case 97:
#line 682 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 3888 "ds_parser.cpp"
    break;

  case 98:
#line 683 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 3894 "ds_parser.cpp"
    break;

  case 99:
#line 684 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 3900 "ds_parser.cpp"
    break;

  case 100:
#line 685 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 3906 "ds_parser.cpp"
    break;

  case 101:
#line 686 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 3912 "ds_parser.cpp"
    break;

  case 102:
#line 691 "ds_parser.ypp"
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
#line 3983 "ds_parser.cpp"
    break;

  case 103:
#line 760 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3991 "ds_parser.cpp"
    break;

  case 104:
#line 763 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4003 "ds_parser.cpp"
    break;

  case 105:
#line 773 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4009 "ds_parser.cpp"
    break;

  case 106:
#line 774 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4015 "ds_parser.cpp"
    break;

  case 107:
#line 775 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4021 "ds_parser.cpp"
    break;

  case 108:
#line 776 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4027 "ds_parser.cpp"
    break;

  case 109:
#line 777 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4033 "ds_parser.cpp"
    break;

  case 110:
#line 778 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4039 "ds_parser.cpp"
    break;

  case 111:
#line 779 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4045 "ds_parser.cpp"
    break;

  case 112:
#line 780 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4051 "ds_parser.cpp"
    break;

  case 113:
#line 781 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4057 "ds_parser.cpp"
    break;

  case 114:
#line 782 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4063 "ds_parser.cpp"
    break;

  case 115:
#line 783 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4069 "ds_parser.cpp"
    break;

  case 116:
#line 784 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4075 "ds_parser.cpp"
    break;

  case 117:
#line 785 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4081 "ds_parser.cpp"
    break;

  case 118:
#line 786 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4087 "ds_parser.cpp"
    break;

  case 119:
#line 787 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4093 "ds_parser.cpp"
    break;

  case 120:
#line 788 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4099 "ds_parser.cpp"
    break;

  case 121:
#line 792 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4108 "ds_parser.cpp"
    break;

  case 122:
#line 796 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4119 "ds_parser.cpp"
    break;

  case 123:
#line 802 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4127 "ds_parser.cpp"
    break;

  case 124:
#line 808 "ds_parser.ypp"
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
#line 4143 "ds_parser.cpp"
    break;

  case 125:
#line 822 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4149 "ds_parser.cpp"
    break;

  case 126:
#line 823 "ds_parser.ypp"
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4160 "ds_parser.cpp"
    break;

  case 127:
#line 829 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4166 "ds_parser.cpp"
    break;

  case 128:
#line 833 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4174 "ds_parser.cpp"
    break;

  case 129:
#line 839 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4182 "ds_parser.cpp"
    break;

  case 130:
#line 842 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4190 "ds_parser.cpp"
    break;

  case 131:
#line 845 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4199 "ds_parser.cpp"
    break;

  case 132:
#line 849 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4207 "ds_parser.cpp"
    break;

  case 133:
#line 855 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4213 "ds_parser.cpp"
    break;

  case 134:
#line 859 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4219 "ds_parser.cpp"
    break;

  case 135:
#line 863 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4227 "ds_parser.cpp"
    break;

  case 136:
#line 866 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4235 "ds_parser.cpp"
    break;

  case 137:
#line 869 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4245 "ds_parser.cpp"
    break;

  case 138:
#line 874 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4253 "ds_parser.cpp"
    break;

  case 139:
#line 877 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4263 "ds_parser.cpp"
    break;

  case 140:
#line 885 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4271 "ds_parser.cpp"
    break;

  case 141:
#line 891 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4277 "ds_parser.cpp"
    break;

  case 142:
#line 892 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4283 "ds_parser.cpp"
    break;

  case 143:
#line 896 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4289 "ds_parser.cpp"
    break;

  case 144:
#line 897 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4295 "ds_parser.cpp"
    break;

  case 145:
#line 901 "ds_parser.ypp"
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
#line 4330 "ds_parser.cpp"
    break;

  case 146:
#line 934 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4338 "ds_parser.cpp"
    break;

  case 147:
#line 937 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4348 "ds_parser.cpp"
    break;

  case 148:
#line 942 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4358 "ds_parser.cpp"
    break;

  case 149:
#line 950 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4367 "ds_parser.cpp"
    break;

  case 150:
#line 954 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4377 "ds_parser.cpp"
    break;

  case 151:
#line 959 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4386 "ds_parser.cpp"
    break;

  case 152:
#line 963 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4396 "ds_parser.cpp"
    break;

  case 153:
#line 971 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4404 "ds_parser.cpp"
    break;

  case 154:
#line 974 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4412 "ds_parser.cpp"
    break;

  case 155:
#line 980 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4422 "ds_parser.cpp"
    break;

  case 156:
#line 986 "ds_parser.ypp"
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
#line 4475 "ds_parser.cpp"
    break;

  case 157:
#line 1037 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4481 "ds_parser.cpp"
    break;

  case 158:
#line 1038 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4487 "ds_parser.cpp"
    break;

  case 159:
#line 1039 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4493 "ds_parser.cpp"
    break;

  case 160:
#line 1040 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4499 "ds_parser.cpp"
    break;

  case 161:
#line 1041 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4505 "ds_parser.cpp"
    break;

  case 162:
#line 1042 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4511 "ds_parser.cpp"
    break;

  case 163:
#line 1046 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4517 "ds_parser.cpp"
    break;

  case 164:
#line 1047 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4523 "ds_parser.cpp"
    break;

  case 165:
#line 1048 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4529 "ds_parser.cpp"
    break;

  case 166:
#line 1049 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4535 "ds_parser.cpp"
    break;

  case 167:
#line 1050 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4541 "ds_parser.cpp"
    break;

  case 168:
#line 1051 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4547 "ds_parser.cpp"
    break;

  case 169:
#line 1052 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4553 "ds_parser.cpp"
    break;

  case 170:
#line 1053 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4559 "ds_parser.cpp"
    break;

  case 171:
#line 1054 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4565 "ds_parser.cpp"
    break;

  case 172:
#line 1055 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4571 "ds_parser.cpp"
    break;

  case 173:
#line 1056 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4577 "ds_parser.cpp"
    break;

  case 174:
#line 1057 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4583 "ds_parser.cpp"
    break;

  case 175:
#line 1058 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4589 "ds_parser.cpp"
    break;

  case 176:
#line 1059 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4595 "ds_parser.cpp"
    break;

  case 177:
#line 1060 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4601 "ds_parser.cpp"
    break;

  case 178:
#line 1061 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4607 "ds_parser.cpp"
    break;

  case 179:
#line 1065 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4619 "ds_parser.cpp"
    break;

  case 180:
#line 1076 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4629 "ds_parser.cpp"
    break;

  case 181:
#line 1081 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4641 "ds_parser.cpp"
    break;

  case 182:
#line 1091 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4647 "ds_parser.cpp"
    break;

  case 183:
#line 1092 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4653 "ds_parser.cpp"
    break;

  case 184:
#line 1093 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4659 "ds_parser.cpp"
    break;

  case 185:
#line 1094 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4665 "ds_parser.cpp"
    break;

  case 186:
#line 1095 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4671 "ds_parser.cpp"
    break;

  case 187:
#line 1096 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4677 "ds_parser.cpp"
    break;

  case 188:
#line 1097 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4683 "ds_parser.cpp"
    break;

  case 189:
#line 1098 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4689 "ds_parser.cpp"
    break;

  case 190:
#line 1099 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4695 "ds_parser.cpp"
    break;

  case 191:
#line 1100 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4701 "ds_parser.cpp"
    break;

  case 192:
#line 1101 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4707 "ds_parser.cpp"
    break;

  case 193:
#line 1102 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4713 "ds_parser.cpp"
    break;

  case 194:
#line 1103 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4719 "ds_parser.cpp"
    break;

  case 195:
#line 1104 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4725 "ds_parser.cpp"
    break;

  case 196:
#line 1105 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4731 "ds_parser.cpp"
    break;

  case 197:
#line 1106 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4737 "ds_parser.cpp"
    break;

  case 198:
#line 1107 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4743 "ds_parser.cpp"
    break;

  case 199:
#line 1108 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4749 "ds_parser.cpp"
    break;

  case 200:
#line 1109 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4755 "ds_parser.cpp"
    break;

  case 201:
#line 1110 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4761 "ds_parser.cpp"
    break;

  case 202:
#line 1111 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4767 "ds_parser.cpp"
    break;

  case 203:
#line 1112 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4773 "ds_parser.cpp"
    break;

  case 204:
#line 1113 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4779 "ds_parser.cpp"
    break;

  case 205:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4785 "ds_parser.cpp"
    break;

  case 206:
#line 1115 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4791 "ds_parser.cpp"
    break;

  case 207:
#line 1116 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4797 "ds_parser.cpp"
    break;

  case 208:
#line 1117 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4803 "ds_parser.cpp"
    break;

  case 209:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4809 "ds_parser.cpp"
    break;

  case 210:
#line 1119 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4815 "ds_parser.cpp"
    break;

  case 211:
#line 1120 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4821 "ds_parser.cpp"
    break;

  case 212:
#line 1121 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4827 "ds_parser.cpp"
    break;

  case 213:
#line 1122 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4833 "ds_parser.cpp"
    break;

  case 214:
#line 1123 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4839 "ds_parser.cpp"
    break;

  case 215:
#line 1124 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4845 "ds_parser.cpp"
    break;

  case 216:
#line 1125 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4851 "ds_parser.cpp"
    break;

  case 217:
#line 1126 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4857 "ds_parser.cpp"
    break;

  case 218:
#line 1127 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4863 "ds_parser.cpp"
    break;

  case 219:
#line 1128 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4869 "ds_parser.cpp"
    break;

  case 220:
#line 1129 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4878 "ds_parser.cpp"
    break;

  case 221:
#line 1133 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4887 "ds_parser.cpp"
    break;

  case 222:
#line 1137 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4895 "ds_parser.cpp"
    break;

  case 223:
#line 1140 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4903 "ds_parser.cpp"
    break;

  case 224:
#line 1143 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4909 "ds_parser.cpp"
    break;

  case 225:
#line 1144 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4915 "ds_parser.cpp"
    break;

  case 226:
#line 1145 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4921 "ds_parser.cpp"
    break;

  case 227:
#line 1146 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4927 "ds_parser.cpp"
    break;

  case 228:
#line 1147 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4933 "ds_parser.cpp"
    break;

  case 229:
#line 1148 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4939 "ds_parser.cpp"
    break;

  case 230:
#line 1149 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4947 "ds_parser.cpp"
    break;

  case 231:
#line 1152 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4953 "ds_parser.cpp"
    break;

  case 232:
#line 1153 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4959 "ds_parser.cpp"
    break;

  case 233:
#line 1154 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4965 "ds_parser.cpp"
    break;

  case 234:
#line 1155 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4971 "ds_parser.cpp"
    break;

  case 235:
#line 1156 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4977 "ds_parser.cpp"
    break;

  case 236:
#line 1157 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4983 "ds_parser.cpp"
    break;

  case 237:
#line 1158 "ds_parser.ypp"
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
#line 4998 "ds_parser.cpp"
    break;

  case 238:
#line 1168 "ds_parser.ypp"
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
#line 5013 "ds_parser.cpp"
    break;

  case 239:
#line 1178 "ds_parser.ypp"
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
#line 5049 "ds_parser.cpp"
    break;

  case 240:
#line 1212 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5055 "ds_parser.cpp"
    break;

  case 241:
#line 1213 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5061 "ds_parser.cpp"
    break;

  case 242:
#line 1217 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5067 "ds_parser.cpp"
    break;

  case 243:
#line 1218 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5073 "ds_parser.cpp"
    break;

  case 244:
#line 1222 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5083 "ds_parser.cpp"
    break;

  case 245:
#line 1230 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5091 "ds_parser.cpp"
    break;

  case 246:
#line 1233 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5100 "ds_parser.cpp"
    break;

  case 247:
#line 1240 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5106 "ds_parser.cpp"
    break;

  case 248:
#line 1241 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5112 "ds_parser.cpp"
    break;

  case 249:
#line 1245 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5118 "ds_parser.cpp"
    break;

  case 250:
#line 1246 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5124 "ds_parser.cpp"
    break;

  case 251:
#line 1250 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5132 "ds_parser.cpp"
    break;

  case 252:
#line 1256 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5138 "ds_parser.cpp"
    break;

  case 253:
#line 1257 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5144 "ds_parser.cpp"
    break;

  case 254:
#line 1261 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5150 "ds_parser.cpp"
    break;

  case 255:
#line 1262 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5156 "ds_parser.cpp"
    break;

  case 256:
#line 1266 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5166 "ds_parser.cpp"
    break;

  case 257:
#line 1271 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5174 "ds_parser.cpp"
    break;

  case 258:
#line 1274 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5183 "ds_parser.cpp"
    break;

  case 259:
#line 1278 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5194 "ds_parser.cpp"
    break;

  case 260:
#line 1284 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5205 "ds_parser.cpp"
    break;

  case 261:
#line 1293 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5211 "ds_parser.cpp"
    break;

  case 262:
#line 1294 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5217 "ds_parser.cpp"
    break;

  case 263:
#line 1295 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5223 "ds_parser.cpp"
    break;

  case 264:
#line 1299 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5231 "ds_parser.cpp"
    break;

  case 265:
#line 1302 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5241 "ds_parser.cpp"
    break;

  case 266:
#line 1307 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5251 "ds_parser.cpp"
    break;

  case 267:
#line 1312 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5263 "ds_parser.cpp"
    break;

  case 268:
#line 1319 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5275 "ds_parser.cpp"
    break;

  case 269:
#line 1329 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5283 "ds_parser.cpp"
    break;

  case 270:
#line 1332 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5292 "ds_parser.cpp"
    break;

  case 271:
#line 1339 "ds_parser.ypp"
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
#line 5323 "ds_parser.cpp"
    break;

  case 272:
#line 1368 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5331 "ds_parser.cpp"
    break;

  case 273:
#line 1371 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5345 "ds_parser.cpp"
    break;

  case 274:
#line 1380 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5359 "ds_parser.cpp"
    break;

  case 275:
#line 1392 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5373 "ds_parser.cpp"
    break;

  case 276:
#line 1404 "ds_parser.ypp"
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
#line 5389 "ds_parser.cpp"
    break;

  case 277:
#line 1418 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5395 "ds_parser.cpp"
    break;

  case 278:
#line 1419 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5401 "ds_parser.cpp"
    break;

  case 279:
#line 1423 "ds_parser.ypp"
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
#line 5442 "ds_parser.cpp"
    break;

  case 280:
#line 1462 "ds_parser.ypp"
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
#line 5510 "ds_parser.cpp"
    break;

  case 281:
#line 1528 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5522 "ds_parser.cpp"
    break;

  case 282:
#line 1535 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5533 "ds_parser.cpp"
    break;

  case 283:
#line 1544 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 5539 "ds_parser.cpp"
    break;

  case 284:
#line 1545 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 5545 "ds_parser.cpp"
    break;

  case 285:
#line 1546 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 5551 "ds_parser.cpp"
    break;

  case 286:
#line 1547 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 5557 "ds_parser.cpp"
    break;

  case 287:
#line 1548 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 5563 "ds_parser.cpp"
    break;

  case 288:
#line 1549 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 5569 "ds_parser.cpp"
    break;

  case 289:
#line 1550 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 5575 "ds_parser.cpp"
    break;

  case 290:
#line 1551 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 5581 "ds_parser.cpp"
    break;

  case 291:
#line 1552 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 5587 "ds_parser.cpp"
    break;

  case 292:
#line 1553 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 5593 "ds_parser.cpp"
    break;

  case 293:
#line 1554 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 5599 "ds_parser.cpp"
    break;

  case 294:
#line 1555 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 5605 "ds_parser.cpp"
    break;

  case 295:
#line 1556 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 5611 "ds_parser.cpp"
    break;

  case 296:
#line 1557 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 5617 "ds_parser.cpp"
    break;

  case 297:
#line 1558 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 5623 "ds_parser.cpp"
    break;

  case 298:
#line 1559 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 5629 "ds_parser.cpp"
    break;

  case 299:
#line 1560 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 5635 "ds_parser.cpp"
    break;

  case 300:
#line 1561 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 5641 "ds_parser.cpp"
    break;

  case 301:
#line 1562 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 5647 "ds_parser.cpp"
    break;

  case 302:
#line 1563 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 5653 "ds_parser.cpp"
    break;

  case 303:
#line 1564 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 5659 "ds_parser.cpp"
    break;

  case 304:
#line 1565 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 5665 "ds_parser.cpp"
    break;

  case 305:
#line 1566 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 5671 "ds_parser.cpp"
    break;

  case 306:
#line 1567 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 5677 "ds_parser.cpp"
    break;

  case 307:
#line 1571 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 5690 "ds_parser.cpp"
    break;

  case 308:
#line 1582 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5699 "ds_parser.cpp"
    break;

  case 309:
#line 1586 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5711 "ds_parser.cpp"
    break;

  case 310:
#line 1596 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5717 "ds_parser.cpp"
    break;

  case 311:
#line 1597 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5723 "ds_parser.cpp"
    break;

  case 312:
#line 1598 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5729 "ds_parser.cpp"
    break;

  case 313:
#line 1599 "ds_parser.ypp"
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
#line 5748 "ds_parser.cpp"
    break;

  case 314:
#line 1613 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5759 "ds_parser.cpp"
    break;

  case 315:
#line 1619 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5768 "ds_parser.cpp"
    break;

  case 316:
#line 1623 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5778 "ds_parser.cpp"
    break;

  case 317:
#line 1628 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5788 "ds_parser.cpp"
    break;

  case 318:
#line 1633 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5798 "ds_parser.cpp"
    break;

  case 319:
#line 1638 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5808 "ds_parser.cpp"
    break;

  case 320:
#line 1643 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5820 "ds_parser.cpp"
    break;

  case 321:
#line 1650 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5830 "ds_parser.cpp"
    break;

  case 322:
#line 1655 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5842 "ds_parser.cpp"
    break;

  case 323:
#line 1662 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5852 "ds_parser.cpp"
    break;

  case 324:
#line 1667 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5863 "ds_parser.cpp"
    break;

  case 325:
#line 1673 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5873 "ds_parser.cpp"
    break;

  case 326:
#line 1678 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5882 "ds_parser.cpp"
    break;

  case 327:
#line 1682 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5892 "ds_parser.cpp"
    break;

  case 328:
#line 1687 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5906 "ds_parser.cpp"
    break;

  case 329:
#line 1696 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5915 "ds_parser.cpp"
    break;

  case 330:
#line 1700 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5925 "ds_parser.cpp"
    break;

  case 331:
#line 1705 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5939 "ds_parser.cpp"
    break;

  case 332:
#line 1714 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5948 "ds_parser.cpp"
    break;

  case 333:
#line 1718 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5958 "ds_parser.cpp"
    break;

  case 334:
#line 1723 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5972 "ds_parser.cpp"
    break;

  case 335:
#line 1732 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5983 "ds_parser.cpp"
    break;

  case 336:
#line 1741 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5989 "ds_parser.cpp"
    break;

  case 337:
#line 1742 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5995 "ds_parser.cpp"
    break;

  case 338:
#line 1743 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6001 "ds_parser.cpp"
    break;

  case 339:
#line 1744 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6007 "ds_parser.cpp"
    break;

  case 340:
#line 1748 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6019 "ds_parser.cpp"
    break;

  case 341:
#line 1755 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6030 "ds_parser.cpp"
    break;

  case 342:
#line 1764 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6040 "ds_parser.cpp"
    break;

  case 343:
#line 1769 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6049 "ds_parser.cpp"
    break;

  case 344:
#line 1776 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6059 "ds_parser.cpp"
    break;

  case 345:
#line 1781 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6070 "ds_parser.cpp"
    break;

  case 346:
#line 1790 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6078 "ds_parser.cpp"
    break;

  case 347:
#line 1793 "ds_parser.ypp"
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
#line 6094 "ds_parser.cpp"
    break;

  case 348:
#line 1807 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6105 "ds_parser.cpp"
    break;

  case 349:
#line 1816 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6111 "ds_parser.cpp"
    break;

  case 350:
#line 1817 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6117 "ds_parser.cpp"
    break;

  case 351:
#line 1821 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6127 "ds_parser.cpp"
    break;

  case 352:
#line 1826 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6136 "ds_parser.cpp"
    break;

  case 353:
#line 1833 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6146 "ds_parser.cpp"
    break;

  case 354:
#line 1838 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6159 "ds_parser.cpp"
    break;

  case 355:
#line 1849 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6169 "ds_parser.cpp"
    break;

  case 356:
#line 1854 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6178 "ds_parser.cpp"
    break;

  case 357:
#line 1861 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6192 "ds_parser.cpp"
    break;

  case 358:
#line 1873 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6198 "ds_parser.cpp"
    break;

  case 359:
#line 1874 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6204 "ds_parser.cpp"
    break;

  case 360:
#line 1878 "ds_parser.ypp"
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
#line 6222 "ds_parser.cpp"
    break;


#line 6226 "ds_parser.cpp"

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
#line 1893 "ds_parser.ypp"


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


