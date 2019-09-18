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
    DAS_REINTERPRET = 300,
    DAS_TBOOL = 301,
    DAS_TVOID = 302,
    DAS_TSTRING = 303,
    DAS_TAUTO = 304,
    DAS_TINT = 305,
    DAS_TINT2 = 306,
    DAS_TINT3 = 307,
    DAS_TINT4 = 308,
    DAS_TUINT = 309,
    DAS_TUINT2 = 310,
    DAS_TUINT3 = 311,
    DAS_TUINT4 = 312,
    DAS_TFLOAT = 313,
    DAS_TFLOAT2 = 314,
    DAS_TFLOAT3 = 315,
    DAS_TFLOAT4 = 316,
    DAS_TRANGE = 317,
    DAS_TURANGE = 318,
    DAS_TBLOCK = 319,
    DAS_TINT64 = 320,
    DAS_TUINT64 = 321,
    DAS_TDOUBLE = 322,
    DAS_TFUNCTION = 323,
    DAS_TLAMBDA = 324,
    DAS_TINT8 = 325,
    DAS_TUINT8 = 326,
    DAS_TINT16 = 327,
    DAS_TUINT16 = 328,
    DAS_TTUPLE = 329,
    ADDEQU = 330,
    SUBEQU = 331,
    DIVEQU = 332,
    MULEQU = 333,
    MODEQU = 334,
    ANDEQU = 335,
    OREQU = 336,
    XOREQU = 337,
    SHL = 338,
    SHR = 339,
    ADDADD = 340,
    SUBSUB = 341,
    LEEQU = 342,
    SHLEQU = 343,
    SHREQU = 344,
    GREQU = 345,
    EQUEQU = 346,
    NOTEQU = 347,
    RARROW = 348,
    LARROW = 349,
    QQ = 350,
    QDOT = 351,
    LPIPE = 352,
    LBPIPE = 353,
    RPIPE = 354,
    CLONEEQU = 355,
    ROTL = 356,
    ROTR = 357,
    ROTLEQU = 358,
    ROTREQU = 359,
    MAPTO = 360,
    BRABRAB = 361,
    BRACBRB = 362,
    CBRCBRB = 363,
    INTEGER = 364,
    LONG_INTEGER = 365,
    UNSIGNED_INTEGER = 366,
    UNSIGNED_LONG_INTEGER = 367,
    FLOAT = 368,
    DOUBLE = 369,
    NAME = 370,
    BEGIN_STRING = 371,
    STRING_CHARACTER = 372,
    END_STRING = 373,
    BEGIN_STRING_EXPR = 374,
    END_STRING_EXPR = 375,
    UNARY_MINUS = 376,
    UNARY_PLUS = 377,
    PRE_INC = 378,
    PRE_DEC = 379,
    POST_INC = 380,
    POST_DEC = 381,
    COLCOL = 382
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

#line 355 "ds_parser.cpp"

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
#define YYLAST   4231

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  348
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  621

#define YYUNDEFTOK  2
#define YYMAXUTOK   382

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
       2,     2,     2,   139,     2,     2,   153,   134,   127,     2,
     147,   148,   132,   131,   121,   130,   144,   133,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   124,   150,
     128,   122,   129,   123,   135,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   145,     2,   146,   126,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   151,   125,   152,   138,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   136,   137,   140,   141,
     142,   143,   149
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   269,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   281,   282,   286,   290,   293,   302,   310,   326,   347,
     351,   352,   356,   359,   368,   379,   383,   384,   388,   391,
     397,   398,   399,   406,   413,   425,   435,   444,   445,   446,
     447,   448,   449,   453,   458,   466,   467,   471,   478,   490,
     494,   501,   502,   506,   507,   508,   512,   516,   523,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   566,   636,   639,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   665,   668,   677,   683,   697,   698,   704,   708,   714,
     717,   720,   724,   730,   734,   738,   741,   744,   749,   752,
     760,   766,   767,   771,   772,   776,   809,   812,   817,   825,
     829,   834,   838,   846,   849,   855,   860,   912,   913,   914,
     915,   916,   917,   921,   922,   923,   924,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   934,   935,   936,   940,
     951,   956,   966,   967,   968,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,
    1004,  1008,  1012,  1015,  1018,  1019,  1020,  1021,  1022,  1023,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1042,  1052,  1086,
    1087,  1091,  1092,  1096,  1104,  1107,  1114,  1115,  1119,  1120,
    1124,  1130,  1131,  1135,  1136,  1140,  1144,  1147,  1151,  1156,
    1164,  1165,  1166,  1170,  1173,  1178,  1183,  1189,  1198,  1201,
    1208,  1237,  1240,  1249,  1261,  1273,  1287,  1288,  1292,  1331,
    1397,  1404,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1440,  1450,  1454,  1464,
    1465,  1466,  1467,  1481,  1487,  1491,  1496,  1501,  1506,  1511,
    1518,  1522,  1527,  1533,  1538,  1542,  1547,  1556,  1560,  1565,
    1574,  1578,  1583,  1592,  1601,  1602,  1603,  1604,  1608,  1615,
    1624,  1629,  1636,  1641,  1650,  1653,  1667,  1676,  1677,  1681,
    1686,  1693,  1698,  1709,  1714,  1721,  1733,  1734,  1738
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
  "DAS_CONTINUE", "DAS_WHERE", "DAS_REINTERPRET", "DAS_TBOOL", "DAS_TVOID",
  "DAS_TSTRING", "DAS_TAUTO", "DAS_TINT", "DAS_TINT2", "DAS_TINT3",
  "DAS_TINT4", "DAS_TUINT", "DAS_TUINT2", "DAS_TUINT3", "DAS_TUINT4",
  "DAS_TFLOAT", "DAS_TFLOAT2", "DAS_TFLOAT3", "DAS_TFLOAT4", "DAS_TRANGE",
  "DAS_TURANGE", "DAS_TBLOCK", "DAS_TINT64", "DAS_TUINT64", "DAS_TDOUBLE",
  "DAS_TFUNCTION", "DAS_TLAMBDA", "DAS_TINT8", "DAS_TUINT8", "DAS_TINT16",
  "DAS_TUINT16", "DAS_TTUPLE", "ADDEQU", "SUBEQU", "DIVEQU", "MULEQU",
  "MODEQU", "ANDEQU", "OREQU", "XOREQU", "SHL", "SHR", "ADDADD", "SUBSUB",
  "LEEQU", "SHLEQU", "SHREQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW",
  "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE", "CLONEEQU", "ROTL",
  "ROTR", "ROTLEQU", "ROTREQU", "MAPTO", "BRABRAB", "BRACBRB", "CBRCBRB",
  "INTEGER", "LONG_INTEGER", "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER",
  "FLOAT", "DOUBLE", "NAME", "BEGIN_STRING", "STRING_CHARACTER",
  "END_STRING", "BEGIN_STRING_EXPR", "END_STRING_EXPR", "','", "'='",
  "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'",
  "'/'", "'%'", "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
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
     375,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,    64,   376,   377,   126,    33,
     378,   379,   380,   381,    46,    91,    93,    40,    41,   382,
      59,   123,   125,    36
};
# endif

#define YYPACT_NINF -429

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-429)))

#define YYTABLE_NINF -154

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -429,    22,  -429,  -429,   -43,    -6,    -2,    40,    53,  -429,
     -15,  -429,  -429,  -429,   108,  -429,   -90,  -429,  -429,  -429,
    -429,    57,  -429,    21,    72,    84,  -429,  -429,   103,    96,
    -429,    81,   121,  -429,   106,   129,   112,  -429,   -48,  -429,
     151,    -5,  -429,   309,   -43,   163,    -6,    -2,   158,  -429,
    2702,   159,  -429,   -43,   -15,  -429,   152,   126,  3596,   250,
     252,  -429,   136,   -84,  -429,  -429,  -429,  -429,  -429,   169,
    -429,  -429,  -429,  -429,  -429,  -429,   -83,   160,   161,   168,
    -429,  -429,  -429,   140,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,   170,  -429,
    -429,  -429,   171,   173,  -429,  -429,  -429,  -429,   176,  -429,
    -429,  -429,  -429,   334,  -429,    23,  -429,   -82,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,    25,   185,  -429,  -429,
    -429,   -47,  -429,   -22,   -79,  -429,  2702,  2702,  2702,   182,
    2632,  2632,  2632,  2702,  -429,     1,  -429,  -429,  1577,  -429,
    -429,   -46,   195,  -429,  -429,   -92,  -429,   -39,  2702,   164,
    -429,  -429,   197,  -429,  2702,  2384,  -429,  -429,   207,  -429,
      77,     2,   155,   175,   185,   184,   185,   191,   185,   201,
    -429,   -88,   334,   190,  -429,   179,  -429,  -429,  2562,   172,
    -429,   180,   198,   203,   187,   204,   193,  2384,  2384,  2492,
    2702,  2384,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  2384,
    2384,   -82,  2384,  2384,  -429,  2384,  -429,   196,  -429,  -429,
     -77,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  3618,   205,
    -429,  -429,  -429,  -429,  -429,   -43,  -429,   304,   200,  -429,
    -429,   -27,  -429,  -429,  2702,  2384,   334,  -429,  -429,    -7,
    -429,   247,  2708,   213,  -429,  2702,  -429,  -429,   219,  -429,
     222,  -429,   225,  -429,  -429,  2702,  -429,  -429,    62,  -429,
     253,  2384,  2702,  2702,  2384,  2702,  1706,   478,   478,   195,
     946,  1075,  3746,  -429,    99,    95,   478,   478,  -429,   478,
     478,   501,   651,   136,  -429,  1204,  2384,  2384,  -429,  -429,
    2384,  2384,  2384,  2384,   255,  2384,   256,  2384,  2384,  2384,
    2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,
    2384,  2384,   257,  2384,  -429,  1819,    45,  -429,   195,  -429,
    -429,    70,  -429,  3012,  -429,  2384,   110,  2384,  2384,  2384,
    2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,  2384,
    2384,  2384,  -429,  -429,   210,  -429,  -429,  -429,  -429,  1932,
    1317,  3258,   220,   230,  3324,   233,  -429,  3390,     6,   -73,
    2045,  3746,   258,   -45,   259,  -429,  -429,    52,   119,  2384,
    2384,   224,  -429,  2384,   264,  -429,  -429,  2384,  2384,   195,
    1446,  -429,   164,  2384,  2384,  -429,  -429,   351,  -429,  -429,
    -429,  -429,  -429,  -429,   234,   236,   237,  -429,  -429,   366,
    -429,   -72,  3083,   185,   273,  -429,    28,  3914,  4081,  4081,
    4063,  4063,  4010,  4010,   244,   478,  -429,   -54,   -54,  4081,
    4081,  3802,  3934,   415,  3990,  4063,  4063,  4086,  4086,   437,
     437,   437,  -429,  3682,  -429,    69,  -429,  -429,  2384,  -429,
    2784,  -429,  3914,  3914,  3914,  3914,  3914,  3914,  3914,  3914,
    3914,  3914,  3914,  3914,  3914,  3914,  3914,  -429,  -429,    78,
    2702,   277,  3456,  -429,  2384,  2384,  -429,  2384,  -429,  2384,
    2384,   273,   282,   269,   273,  2384,   271,  2384,   274,  3914,
    -429,  -429,  3858,  3139,  3139,    20,  2384,  -429,  -429,  2860,
     388,  3914,  3139,   268,  -429,  -429,  -429,   387,   195,  -429,
     164,   -30,    89,  2384,  -429,  2158,  2384,  -429,  -429,  3914,
    -429,  -429,     3,   294,  -429,  4063,  4063,  4063,   -85,  3914,
      88,   -30,  -429,   258,  3914,  -429,  -429,  -429,  -429,  -429,
      42,  2384,  -429,  2936,  -429,   164,  -429,  -429,  -429,  -429,
    -429,   278,  3914,  -429,    82,  3914,  -429,  2271,  2384,   281,
    2384,   164,  2384,  -429,   -42,  -429,  -429,   804,  -429,  -429,
    2702,  3522,  3202,  -429,  3914,  -429,  3139,   164,  -429,    27,
    -429,   385,   301,    42,  -429,  -429,  2384,   302,  -429,  3914,
    -429
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    51,     1,   133,     0,     0,     0,     0,     0,   134,
       0,     9,     8,     7,     0,     6,     0,     5,    10,     4,
       3,     0,    43,    18,    28,    25,    26,    22,    19,    24,
      20,     0,     0,    46,   115,     0,    47,    49,     0,    45,
       0,     0,   258,     0,     0,     0,     0,     0,     0,   261,
       0,     0,   117,     0,     0,    52,   266,     0,     0,     0,
       0,    58,    53,     0,    41,    42,    39,    40,    38,     0,
      37,    44,    29,    27,    21,    23,     0,     0,     0,     0,
     272,   292,   273,   297,   274,   278,   279,   280,   281,   285,
     286,   287,   288,   289,   290,   291,   293,   294,   314,   277,
     284,   295,   317,   320,   275,   282,   276,   283,     0,   296,
     299,   301,   300,   264,   116,     0,    50,     0,   268,   234,
      61,    62,    64,    63,    65,    66,    67,    68,    87,    88,
      85,    86,    78,    89,    90,    79,    76,    77,    91,    92,
      93,    94,    81,    82,    80,    74,    75,    70,    69,    71,
      72,    73,    60,    59,    83,    84,     0,    56,   270,   260,
     259,     0,    11,     0,     0,   265,     0,     0,     0,     0,
      53,    53,    53,     0,   305,     0,   310,   307,     0,    48,
     267,   229,     0,    54,   238,     0,   236,   245,     0,     0,
     251,   252,     0,   250,     0,     0,    12,    13,     0,   262,
       0,     0,     0,     0,    56,     0,    56,     0,    56,     0,
     241,     0,   240,   306,   308,     0,   177,   178,     0,     0,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   149,   148,   150,   151,   152,    14,     0,
       0,     0,     0,     0,   303,     0,   111,    51,   175,   145,
     173,   222,   221,   220,   225,   174,   224,   223,     0,     0,
     176,   324,   325,   326,   327,     0,   269,   231,     0,   237,
      55,     0,   244,   243,     0,     0,    57,    95,   271,     0,
     257,     0,   153,     0,   311,     0,   313,   298,     0,   315,
       0,   318,     0,   321,   323,     0,   309,   304,   119,   122,
       0,     0,     0,     0,     0,     0,     0,   201,   202,     0,
       0,     0,     0,   343,     0,     0,   182,   181,   209,   180,
     179,     0,     0,    53,   228,     0,     0,     0,   203,   204,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,     0,   232,     0,   235,
     239,   246,   249,   248,   253,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,   263,     0,   316,   319,   322,   242,     0,
       0,     0,     0,     0,     0,     0,   216,     0,     0,   115,
       0,   334,   330,     0,   337,   338,   339,     0,     0,     0,
       0,     0,    17,     0,    15,   205,   113,     0,     0,     0,
       0,   123,     0,     0,     0,   124,    98,    96,   109,   105,
     103,   104,   112,    99,     0,     0,     0,   108,   110,   131,
     102,     0,   153,    56,     0,   210,     0,   143,   183,   184,
     196,   197,   194,   195,     0,   218,   208,   226,   227,   185,
     186,     0,   199,   200,   198,   192,   193,   188,   187,   189,
     190,   191,   207,     0,   212,     0,   230,   233,     0,   255,
     153,   114,   160,   161,   163,   162,   164,   157,   158,   159,
     165,   166,   155,   156,   167,   168,   154,   312,   120,     0,
       0,     0,     0,   214,     0,     0,   215,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     344,   345,     0,     0,     0,     0,     0,   125,   128,   153,
       0,   118,     0,     0,   101,   106,   107,     0,     0,   100,
       0,     0,     0,     0,   211,     0,     0,   206,   213,   247,
     254,   121,     0,     0,   139,   136,   137,   138,     0,   328,
       0,     0,   332,   331,   335,   341,   340,   342,    16,    35,
      30,     0,   129,   153,   126,     0,    36,   111,   132,   135,
     146,     0,   144,   170,     0,   219,   141,     0,     0,     0,
       0,     0,     0,    33,     0,   127,   130,     0,   169,   171,
       0,     0,   346,   333,   329,    31,     0,     0,    97,     0,
     140,     0,     0,    30,    34,   142,     0,     0,    32,   347,
     348
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -429,  -429,   134,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
     403,  -429,  -429,   406,  -158,  -429,  -429,  -429,  -429,   412,
     -44,  -429,   404,  -429,   212,  -153,  -198,  -429,  -429,   -78,
    -429,  -117,  -191,   -10,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,   463,  -429,  -429,  -429,  -344,   100,  -429,  -317,  -429,
    -429,    63,  -429,  -429,  -429,  -429,   199,  -429,   174,  -429,
    -184,  -170,   188,   -70,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,   -61,   -49,  -429,  -429,   -35,   260,  -428,   -40,
    -429,  -429,  -224,   -33,   177,  -429,  -429,  -429,  -429,  -429
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   163,    70,   315,   248,    11,    12,    28,    29,
      30,    13,    25,    26,   593,   428,   429,   430,   431,    22,
      23,    36,    37,    38,    14,   157,   189,    62,    15,   249,
     432,   322,   433,   250,   434,   251,   435,   436,   437,   438,
     538,   439,   440,   252,   253,   446,   254,   255,   281,   256,
     257,   447,   267,   358,   268,   181,   184,   185,   210,   211,
     510,   186,   195,   160,    63,    17,    76,    18,    19,   118,
      57,    20,   187,   259,   111,   112,   212,   260,   402,   403,
     261,   404,   405,   406,   407,   262,   314,   263,   612,   264
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   110,   161,   275,   280,   441,   288,   313,   290,   115,
     292,   475,   269,    33,   182,   113,   542,   204,   206,   208,
     174,   272,     2,   509,    58,   175,   366,     3,   213,   174,
     174,   158,   164,    34,   175,   175,   543,   571,   324,   334,
     109,   294,   336,   198,    39,   499,     4,   190,     5,   273,
       6,   591,     7,   191,   174,   272,   270,     8,   271,   175,
     592,    42,   295,     9,   272,   588,   182,    35,   159,   165,
     325,   199,    21,    54,   192,   193,    51,   194,   539,   543,
      59,    60,   192,   273,   362,   274,   563,   190,   158,   174,
     352,   353,   273,   191,   175,   196,   197,   174,    55,   265,
      34,   513,   175,    24,   174,   514,   266,   180,   607,   175,
      61,   277,    40,    27,    41,   193,   176,   110,   110,   110,
     177,   110,   110,   110,   110,   176,   176,   192,   214,   177,
     177,   200,   201,   202,    35,   205,   207,   209,   178,   110,
     158,   192,    44,   364,    44,   110,   215,   178,   178,   543,
     176,   586,   285,   276,   177,    31,   109,   109,   109,   279,
     109,   109,   109,   109,   272,   558,    44,    10,    32,   110,
     443,   179,   178,   183,   479,   615,   544,   478,   109,    43,
     110,   110,   174,   298,   109,   176,   520,   175,   477,   177,
     543,   476,   273,   176,   310,   311,    45,   177,   516,   543,
     176,   584,   517,   543,   177,    46,   284,   178,   109,   389,
     512,   174,   162,   412,   413,   178,   175,   548,   174,   109,
     109,   356,   178,   175,    47,   110,   551,   594,   174,   528,
     599,   318,    49,   175,   589,   581,   110,   174,   514,   361,
      48,   258,   175,    50,    52,   540,   110,   174,   398,   410,
     384,   411,   175,   110,   110,    51,   110,   174,   282,    53,
     174,   246,   175,   247,   109,   175,    56,   392,   393,   517,
     395,   518,    72,    75,   114,   109,   117,   119,   176,   154,
     441,   155,   177,   156,   286,   109,   162,   169,   166,   167,
     307,   308,   109,   109,   312,   109,   168,   203,   170,   171,
     178,   172,   316,   317,   173,   319,   320,   176,   321,   188,
     158,   177,   278,   289,   176,   246,   283,   296,   177,   300,
     291,    64,    65,   287,   176,   297,   302,   301,   177,   178,
     293,   303,   305,   176,   304,   572,   178,   177,   363,   497,
     306,    10,   357,   176,   530,   366,   178,   177,   385,   504,
     359,   386,   355,   176,   387,   178,   176,   177,   525,   505,
     177,   174,   507,   383,   391,   178,   175,   394,   390,   397,
     454,   456,   472,   401,   401,   178,   521,   590,   178,   512,
     515,   196,   533,   537,   534,   442,   535,   536,   541,   448,
     449,   545,   553,   450,   451,   452,   453,   561,   455,   575,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   562,   473,   565,    66,   577,
     567,   578,    67,   587,    68,    69,   598,   603,   480,   616,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   569,   570,   617,   620,   414,
      74,   110,    73,   502,   576,   618,    71,   176,   116,   323,
     597,   177,   580,   321,    16,   552,   481,   365,   579,   388,
     360,   560,   519,   312,     0,     0,   522,   161,   299,   178,
     523,   524,     0,   529,   566,     0,   531,   532,   408,     0,
     109,     0,     0,     0,     0,     0,     0,   596,   326,   327,
     328,   329,   330,     0,     0,   331,   332,   333,   334,     0,
     335,   336,   337,   605,   338,     0,   339,   340,     0,     0,
       0,     0,   328,   329,     0,     0,     0,     0,   613,   614,
     334,     0,   335,   336,   337,     0,   338,     0,     0,     0,
       0,   549,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   110,     0,     0,     0,     0,     0,     0,     0,   352,
     353,     0,     0,   328,   329,   609,     0,   555,   556,     0,
     557,   334,     0,   559,   336,   337,     0,   338,   564,     0,
     401,   352,   353,     0,   326,   327,   328,   329,   330,   573,
     109,   331,   332,   333,   334,     0,   335,   336,   337,     0,
     338,     0,   339,   340,     0,     0,   582,     0,     0,   585,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,   341,     0,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   352,   353,     0,     0,   415,
     601,   602,   416,   604,     0,   606,     3,     0,   417,   418,
     442,   419,     0,   216,   217,   218,   219,     0,     0,     0,
       0,   420,   220,   421,   422,     0,     0,     0,     0,   619,
       0,     0,     0,   423,   221,     0,     0,   424,   222,     0,
     223,     0,     9,   224,   425,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   240,     0,     0,     0,   241,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,     0,   245,     0,
      35,   426,   246,   427,   247,   416,     0,     0,     0,     3,
       0,   417,   418,     0,   419,     0,   216,   217,   218,   219,
       0,     0,     0,     0,   420,   220,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,   423,   221,     0,     0,
     424,   222,     0,   223,     0,     9,   224,   425,     0,   225,
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
       0,   245,     0,    35,   426,   246,   608,   247,   216,   217,
     218,   219,     0,     0,     0,     0,     0,   220,     0,     0,
       0,     0,     0,   174,     0,     0,     0,     0,   175,   221,
       0,     0,     0,   222,     0,   223,     0,     0,   224,     0,
       0,   225,    80,    81,    82,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    99,   100,   101,     0,   226,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   399,   238,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   177,     0,     0,   239,   240,     0,     0,
       0,   241,     0,     0,   242,   243,     0,   216,   217,   218,
     219,   178,     0,   400,     0,    35,   220,   246,     0,   247,
       0,     0,   174,     0,     0,     0,     0,   175,   221,     0,
       0,     0,   222,     0,   223,     0,     0,   224,     0,     0,
     225,    80,    81,    82,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,     0,   226,   104,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
      34,   238,     0,     0,     0,     0,     0,     0,   176,     0,
       0,     0,   177,     0,     0,   239,   240,     0,     0,     0,
     241,     0,     0,   242,   243,     0,   216,   217,   218,   219,
     178,     0,   245,     0,    35,   220,   246,     0,   247,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,     0,
       0,   222,     0,   223,     0,     0,   224,     0,     0,   225,
      80,    81,    82,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,    99,
     100,   101,     0,   226,   104,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    34,
     238,     0,     0,     0,     0,     0,     0,     0,     0,   216,
     217,   218,   219,   500,   239,   240,     0,     0,   220,   241,
       0,     0,   242,   243,     0,     0,     0,     0,     0,   444,
     221,   245,   445,    35,   222,   246,   223,   247,     0,   224,
       0,     0,   225,    80,    81,    82,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,    99,   100,   101,     0,   226,   104,   105,   106,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,    34,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,     0,   239,   240,     0,
       0,     0,   241,     0,     0,   242,   243,     0,   216,   217,
     218,   219,     0,     0,   245,     0,    35,   220,   246,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,     0,     0,   222,     0,   223,     0,     0,   224,     0,
       0,   225,    80,    81,    82,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    99,   100,   101,     0,   226,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   228,     0,     0,     0,     0,     0,     0,     0,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    34,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   240,     0,     0,
       0,   241,     0,     0,   242,   243,     0,     0,     0,   216,
     217,   218,   219,   245,     0,    35,   527,   246,   220,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,     0,     0,   222,     0,   223,     0,     0,   224,
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
       0,   225,    80,    81,    82,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    99,   100,   101,     0,   226,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,    34,   238,     0,     0,     0,     0,     0,     0,     0,
       0,   216,   217,   218,   219,     0,   239,   240,     0,     0,
     220,   241,     0,     0,   242,   243,     0,     0,     0,     0,
       0,     0,   221,   245,   396,    35,   222,   246,   223,   247,
       0,   224,     0,     0,   225,    80,    81,    82,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,    99,   100,   101,     0,   226,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   227,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,   230,   231,   232,   233,
     234,   235,   236,   237,    34,   238,     0,     0,     0,     0,
       0,     0,     0,     0,   216,   217,   218,   219,     0,   239,
     240,     0,     0,   220,   241,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   221,   245,   474,    35,   222,
     246,   223,   247,     0,   224,     0,     0,   225,    80,    81,
      82,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,    99,   100,   101,
       0,   226,   104,   105,   106,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   227,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    34,   238,     0,
       0,     0,     0,     0,     0,     0,     0,   216,   217,   218,
     219,     0,   239,   240,     0,     0,   220,   241,     0,     0,
     242,   243,     0,     0,     0,     0,     0,     0,   221,   245,
     498,    35,   222,   246,   223,   247,     0,   224,     0,     0,
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
       0,   221,   245,   511,    35,   222,   246,   223,   247,     0,
     224,     0,     0,   225,    80,    81,    82,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,    99,   100,   101,     0,   226,   104,   105,
     106,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,   230,   231,   232,   233,   234,
     235,   236,   237,    34,   238,     0,     0,     0,     0,     0,
       0,     0,     0,   216,   217,   218,   219,   600,   239,   240,
       0,     0,   220,   241,     0,     0,   242,   243,     0,     0,
       0,     0,     0,     0,   221,   245,   583,    35,   222,   246,
     223,   247,     0,   224,     0,     0,   225,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     226,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,    34,   238,     0,     0,
       0,     0,     0,     0,     0,     0,   216,   217,   218,   219,
       0,   239,   240,     0,     0,   220,   241,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   221,   245,     0,
      35,   222,   246,   223,   247,     0,   224,     0,     0,   225,
      80,    81,    82,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,    99,
     100,   101,     0,   226,   104,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,   236,   237,    34,
     238,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,    77,     0,     0,   239,   240,     0,    78,     0,   241,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
       0,   245,    79,    35,     0,   246,     0,   247,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     0,     0,    34,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   230,
     231,     0,    79,     0,     0,     0,     0,    34,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     0,     0,    34,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,   156,
       0,    35,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   326,   327,   328,   329,   330,   375,   376,   331,   332,
     333,   334,   377,   335,   336,   337,     0,   338,   378,   339,
     340,   379,   380,     0,     0,     0,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,    35,   352,   353,     0,     0,     0,     0,   382,   367,
     368,   369,   370,   371,   372,   373,   374,   326,   327,   328,
     329,   330,   375,   376,   331,   332,   333,   334,   377,   335,
     336,   337,     0,   338,   378,   339,   340,   379,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,     0,     0,     0,   550,   367,   368,   369,   370,   371,
     372,   373,   374,   326,   327,   328,   329,   330,   375,   376,
     331,   332,   333,   334,   377,   335,   336,   337,     0,   338,
     378,   339,   340,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,     0,     0,     0,     0,
     574,   367,   368,   369,   370,   371,   372,   373,   374,   326,
     327,   328,   329,   330,   375,   376,   331,   332,   333,   334,
     377,   335,   336,   337,     0,   338,   378,   339,   340,   379,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,     0,     0,     0,     0,   595,   367,   368,   369,
     370,   371,   372,   373,   374,   326,   327,   328,   329,   330,
     375,   376,   331,   332,   333,   334,   377,   335,   336,   337,
    -153,   338,   378,   339,   340,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,   367,   368,
     369,   370,   371,   372,   373,   374,   326,   327,   328,   329,
     330,   375,   376,   331,   332,   333,   334,   377,   335,   336,
     337,     0,   338,   378,   339,   340,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   341,     0,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,     0,
       0,     0,   326,   327,   328,   329,   330,   352,   353,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   338,     0,
     339,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,     0,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,   353,   326,   327,   328,   329,   330,
     246,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,   326,   327,   328,   329,   330,   352,   353,   331,   332,
     333,   334,   611,   335,   336,   337,     0,   338,     0,   339,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,     0,     0,   503,   326,   327,   328,
     329,   330,     0,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,     0,   506,   326,   327,   328,   329,   330,     0,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,     0,     0,   508,   326,
     327,   328,   329,   330,     0,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,     0,     0,   554,   326,   327,   328,   329,   330,
       0,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,     0,     0,
     610,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,     0,
       0,     0,     0,     0,     0,     0,     0,   138,   139,   140,
     141,   326,   327,   328,   329,   330,     0,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   338,     0,   339,
     340,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,     0,     0,     0,   152,   153,     0,     0,     0,     0,
       0,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   353,   354,   326,   327,   328,   329,   330,
       0,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,   547,   326,
     327,   328,   329,   330,     0,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,   338,     0,   339,   340,     0,
       0,   409,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,     0,     0,     0,   326,   327,   328,   329,   330,
     352,   353,   331,   332,   333,   334,     0,   335,   336,   337,
       0,   338,     0,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,   546,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,     0,     0,
       0,   326,   327,   328,   329,   330,   352,   353,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   338,     0,   339,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   568,     0,
       0,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,     0,     0,   326,   327,   328,
     329,   330,   352,   353,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,   326,   327,   328,
     329,   330,     0,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   338,     0,   339,   340,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
       0,     0,     0,   326,   327,   328,   329,   330,   352,   353,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,   326,   327,   328,   329,   330,     0,     0,
     331,     0,     0,   334,     0,   335,   336,   337,     0,   338,
       0,   339,   340,     0,     0,     0,     0,     0,   345,   346,
     347,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,     0,     0,   345,   346,
     347,   348,   349,   350,   351,     0,   326,   327,   328,   329,
       0,     0,     0,     0,   352,   353,   334,     0,   335,   336,
     337,     0,   338,     0,   339,   340,   328,   329,     0,     0,
       0,   328,   329,     0,   334,     0,   335,   336,   337,   334,
     338,   335,   336,   337,     0,   338,     0,     0,     0,     0,
       0,     0,     0,   347,   348,   349,   350,   351,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   352,   353,     0,
       0,   347,   348,   349,   350,   351,     0,     0,   349,   350,
     351,     0,     0,     0,     0,   352,   353,     0,     0,     0,
     352,   353
};

static const yytype_int16 yycheck[] =
{
      10,    50,    63,   187,   195,   322,   204,   231,   206,    53,
     208,   355,   182,    28,    41,    50,   444,   170,   171,   172,
      27,    94,     0,    17,    29,    32,    98,     5,    27,    27,
      27,   115,   115,   115,    32,    32,   121,    17,   115,    93,
      50,   129,    96,   122,    54,   389,    24,    94,    26,   122,
      28,     9,    30,   100,    27,    94,   148,    35,   150,    32,
      18,   151,   150,    41,    94,   150,    41,   149,   152,   152,
     147,   150,   115,   121,   121,   122,   149,   124,   150,   121,
      85,    86,   121,   122,   275,   124,   514,    94,   115,    27,
     144,   145,   122,   100,    32,   117,   118,    27,   146,   145,
     115,   146,    32,   109,    27,   150,   152,   117,   150,    32,
     115,   189,     4,   115,     6,   122,   123,   166,   167,   168,
     127,   170,   171,   172,   173,   123,   123,   121,   127,   127,
     127,   166,   167,   168,   149,   170,   171,   172,   145,   188,
     115,   121,   121,   150,   121,   194,   145,   145,   145,   121,
     123,   148,   150,   188,   127,   115,   166,   167,   168,   194,
     170,   171,   172,   173,    94,   509,   121,   145,   115,   218,
     323,   148,   145,   148,   365,   148,   148,   361,   188,   122,
     229,   230,    27,   218,   194,   123,   410,    32,   358,   127,
     121,   146,   122,   123,   229,   230,   124,   127,   146,   121,
     123,   545,   150,   121,   127,   121,   129,   145,   218,   147,
     121,    27,   117,   118,   119,   145,    32,   148,    27,   229,
     230,   265,   145,    32,   121,   274,   148,   571,    27,   420,
     148,   241,   151,    32,   146,   146,   285,    27,   150,   274,
     144,   178,    32,   122,   115,   443,   295,    27,   309,   150,
     285,   152,    32,   302,   303,   149,   305,    27,   195,   147,
      27,   151,    32,   153,   274,    32,   115,   302,   303,   150,
     305,   152,   109,   115,   115,   285,   124,   151,   123,    29,
     597,    29,   127,   147,   129,   295,   117,   147,   128,   128,
     227,   228,   302,   303,   231,   305,   128,   115,   128,   128,
     145,   128,   239,   240,   128,   242,   243,   123,   245,   124,
     115,   127,   115,   129,   123,   151,   109,   127,   127,   147,
     129,    12,    13,   148,   123,   146,   128,   147,   127,   145,
     129,   128,   128,   123,   147,   526,   145,   127,   275,   129,
     147,   145,    38,   123,   422,    98,   145,   127,   129,   129,
     150,   129,   147,   123,   129,   145,   123,   127,   419,   129,
     127,    27,   129,   150,   301,   145,    32,   304,   115,   306,
     115,   115,   115,   310,   311,   145,   152,   561,   145,   121,
     121,   117,    31,    17,   150,   322,   150,   150,   115,   326,
     327,   147,   115,   330,   331,   332,   333,   115,   335,    11,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   146,   353,   146,   109,   151,
     146,    34,   113,   129,   115,   116,   148,   146,   365,    44,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   523,   524,   146,   146,   315,
      47,   500,    46,   390,   532,   613,    44,   123,    54,   247,
     577,   127,   540,   400,     1,   500,   366,   279,   538,   295,
     271,   511,   409,   410,    -1,    -1,   413,   538,   218,   145,
     417,   418,    -1,   420,   517,    -1,   423,   424,   311,    -1,
     500,    -1,    -1,    -1,    -1,    -1,    -1,   575,    83,    84,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,   591,    99,    -1,   101,   102,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,   606,   607,
      93,    -1,    95,    96,    97,    -1,    99,    -1,    -1,    -1,
      -1,   478,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,   600,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,    85,    86,   600,    -1,   504,   505,    -1,
     507,    93,    -1,   510,    96,    97,    -1,    99,   515,    -1,
     517,   144,   145,    -1,    83,    84,    85,    86,    87,   526,
     600,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,   543,    -1,    -1,   546,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   148,
     587,   588,     1,   590,    -1,   592,     5,    -1,     7,     8,
     597,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    -1,    -1,    -1,    -1,   616,
      -1,    -1,    -1,    32,    33,    -1,    -1,    36,    37,    -1,
      39,    -1,    41,    42,    43,    -1,    45,    46,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
     149,   150,   151,   152,   153,     1,    -1,    -1,    -1,     5,
      -1,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      36,    37,    -1,    39,    -1,    41,    42,    43,    -1,    45,
      46,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,   135,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,   149,   150,   151,   152,   153,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,   127,    -1,    -1,   130,   131,    -1,    -1,
      -1,   135,    -1,    -1,   138,   139,    -1,    12,    13,    14,
      15,   145,    -1,   147,    -1,   149,    21,   151,    -1,   153,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,   127,    -1,    -1,   130,   131,    -1,    -1,    -1,
     135,    -1,    -1,   138,   139,    -1,    12,    13,    14,    15,
     145,    -1,   147,    -1,   149,    21,   151,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      46,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    14,    15,    16,   130,   131,    -1,    -1,    21,   135,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,   145,
      33,   147,   148,   149,    37,   151,    39,   153,    -1,    42,
      -1,    -1,    45,    46,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,
      -1,    -1,   135,    -1,    -1,   138,   139,    -1,    12,    13,
      14,    15,    -1,    -1,   147,    -1,   149,    21,   151,    -1,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,   135,    -1,    -1,   138,   139,    -1,    -1,    -1,    12,
      13,    14,    15,   147,    -1,   149,   150,   151,    21,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    46,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,   135,    -1,    -1,   138,   139,    -1,    12,    13,
      14,    15,    -1,   146,   147,    -1,   149,    21,   151,    -1,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,   130,   131,    -1,    -1,
      21,   135,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    33,   147,   148,   149,    37,   151,    39,   153,
      -1,    42,    -1,    -1,    45,    46,    47,    48,    -1,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    -1,   130,
     131,    -1,    -1,    21,   135,    -1,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    33,   147,   148,   149,    37,
     151,    39,   153,    -1,    42,    -1,    -1,    45,    46,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,   130,   131,    -1,    -1,    21,   135,    -1,    -1,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    33,   147,
     148,   149,    37,   151,    39,   153,    -1,    42,    -1,    -1,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    -1,   130,   131,    -1,    -1,    21,
     135,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    33,   147,   148,   149,    37,   151,    39,   153,    -1,
      42,    -1,    -1,    45,    46,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    16,   130,   131,
      -1,    -1,    21,   135,    -1,    -1,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   147,   148,   149,    37,   151,
      39,   153,    -1,    42,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,   130,   131,    -1,    -1,    21,   135,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    33,   147,    -1,
     149,    37,   151,    39,   153,    -1,    42,    -1,    -1,    45,
      46,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   130,   131,    -1,    25,    -1,   135,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    40,   149,    -1,   151,    -1,   153,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   115,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,    -1,    40,    -1,    -1,    -1,    -1,   115,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   115,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,   147,
      -1,   149,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   144,   145,    -1,    -1,    -1,    -1,   150,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,    -1,    -1,    -1,   150,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,    -1,
     150,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,    -1,    -1,    -1,    -1,   150,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,   144,   145,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,    83,    84,    85,    86,    87,
     151,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,   144,   145,    90,    91,
      92,    93,   150,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,    -1,   148,    83,    84,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
      -1,    -1,   148,    83,    84,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   148,    83,
      84,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,    -1,    -1,   148,    83,    84,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,
     148,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    83,    84,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    83,    84,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,    83,
      84,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
     144,   145,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,   144,   145,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,   144,   145,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    83,    84,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,   144,   145,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    83,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,    -1,    83,    84,    85,    86,
      -1,    -1,    -1,    -1,   144,   145,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    85,    86,    -1,    -1,
      -1,    85,    86,    -1,    93,    -1,    95,    96,    97,    93,
      99,    95,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,
     144,   145
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   155,     0,     5,    24,    26,    28,    30,    35,    41,
     145,   160,   161,   165,   178,   182,   195,   219,   221,   222,
     225,   115,   173,   174,   109,   166,   167,   115,   162,   163,
     164,   115,   115,    28,   115,   149,   175,   176,   177,   187,
       4,     6,   151,   122,   121,   124,   121,   121,   144,   151,
     122,   149,   115,   147,   121,   146,   115,   224,    29,    85,
      86,   115,   181,   218,    12,    13,   109,   113,   115,   116,
     157,   173,   109,   167,   164,   115,   220,    19,    25,    40,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,   187,
     227,   228,   229,   230,   115,   174,   176,   124,   223,   151,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,   101,   102,
     103,   104,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   138,   139,    29,    29,   147,   179,   115,   152,
     217,   226,   117,   156,   115,   152,   128,   128,   128,   147,
     128,   128,   128,   128,    27,    32,   123,   127,   145,   148,
     187,   209,    41,   148,   210,   211,   215,   226,   124,   180,
      94,   100,   121,   122,   124,   216,   117,   118,   122,   150,
     230,   230,   230,   115,   179,   230,   179,   230,   179,   230,
     212,   213,   230,    27,   127,   145,    12,    13,    14,    15,
      21,    33,    37,    39,    42,    45,    69,    85,    86,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   116,   130,
     131,   135,   138,   139,   146,   147,   151,   153,   159,   183,
     187,   189,   197,   198,   200,   201,   203,   204,   205,   227,
     231,   234,   239,   241,   243,   145,   152,   206,   208,   215,
     148,   150,    94,   122,   124,   214,   230,   183,   115,   230,
     186,   202,   205,   109,   129,   150,   129,   148,   180,   129,
     180,   129,   180,   129,   129,   150,   127,   146,   230,   231,
     147,   147,   128,   128,   147,   128,   147,   205,   205,    10,
     230,   230,   205,   236,   240,   158,   205,   205,   187,   205,
     205,   205,   185,   178,   115,   147,    83,    84,    85,    86,
      87,    90,    91,    92,    93,    95,    96,    97,    99,   101,
     102,   123,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   144,   145,   146,   147,   174,    38,   207,   150,
     210,   230,   186,   205,   150,   216,    98,    75,    76,    77,
      78,    79,    80,    81,    82,    88,    89,    94,   100,   103,
     104,   122,   150,   150,   230,   129,   129,   129,   212,   147,
     115,   205,   230,   230,   205,   230,   148,   205,   226,   115,
     147,   205,   232,   233,   235,   236,   237,   238,   238,   105,
     150,   152,   118,   119,   156,   148,     1,     7,     8,    10,
      20,    22,    23,    32,    36,    43,   150,   152,   169,   170,
     171,   172,   184,   186,   188,   190,   191,   192,   193,   195,
     196,   202,   205,   179,   145,   148,   199,   205,   205,   205,
     205,   205,   205,   205,   115,   205,   115,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   115,   205,   148,   199,   146,   215,   214,   186,
     205,   200,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   129,   148,   199,
      16,   128,   205,   148,   129,   129,   148,   129,   148,    17,
     214,   148,   121,   146,   150,   121,   146,   150,   152,   205,
     236,   152,   205,   205,   205,   226,    94,   150,   186,   205,
     183,   205,   205,    31,   150,   150,   150,    17,   194,   150,
     180,   115,   232,   121,   148,   147,   124,   146,   148,   205,
     150,   148,   230,   115,   148,   205,   205,   205,   199,   205,
     233,   115,   146,   232,   205,   146,   237,   146,   120,   183,
     183,    17,   186,   205,   150,    11,   183,   151,    34,   217,
     183,   146,   205,   148,   199,   205,   148,   129,   150,   146,
     214,     9,    18,   168,   199,   150,   183,   185,   148,   148,
      16,   205,   205,   146,   205,   183,   205,   150,   152,   230,
     148,   150,   242,   183,   183,   148,    44,   146,   168,   205,
     146
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   154,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   158,   158,   158,   159,   160,   161,
     162,   162,   163,   163,   164,   165,   166,   166,   167,   167,
     168,   168,   168,   169,   170,   171,   172,   173,   173,   173,
     173,   173,   173,   174,   174,   175,   175,   176,   176,   177,
     177,   178,   178,   179,   179,   179,   180,   180,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   182,   183,   183,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   185,   185,   185,   186,   187,   187,   187,   188,   189,
     189,   189,   189,   190,   191,   192,   192,   192,   192,   192,
     193,   194,   194,   195,   195,   196,   197,   197,   197,   198,
     198,   198,   198,   199,   199,   200,   200,   201,   201,   201,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   203,
     204,   204,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   206,
     206,   207,   207,   208,   209,   209,   210,   210,   211,   211,
     212,   213,   213,   214,   214,   215,   215,   215,   215,   215,
     216,   216,   216,   217,   217,   217,   217,   217,   218,   218,
     219,   220,   220,   220,   221,   222,   223,   223,   224,   225,
     226,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   228,   229,   229,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   231,   231,   231,   231,   232,   232,
     233,   233,   234,   234,   235,   235,   236,   237,   237,   238,
     238,   239,   239,   240,   240,   241,   242,   242,   243
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
       1,     0,     2,     2,     3,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     2,     3,     4,     2,     3,
       4,     0,     2,     1,     1,     3,     5,     5,     5,     5,
       8,     6,     9,     1,     3,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       5,     6,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     3,     3,     2,
       3,     4,     3,     4,     4,     4,     3,     4,     3,     5,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       3,     0,     1,     3,     0,     3,     1,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     3,     5,     3,     3,
       1,     1,     1,     4,     6,     5,     4,     3,     0,     2,
       4,     0,     3,     5,     4,     5,     0,     2,     2,     6,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       1,     1,     4,     3,     4,     2,     3,     2,     3,     4,
       2,     4,     6,     4,     1,     4,     5,     1,     4,     5,
       1,     4,     5,     4,     1,     1,     1,     1,     3,     5,
       1,     3,     5,     7,     1,     3,     3,     1,     1,     1,
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
    case 115: /* NAME  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2497 "ds_parser.cpp"
        break;

    case 156: /* character_sequence  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2503 "ds_parser.cpp"
        break;

    case 157: /* string_constant  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2509 "ds_parser.cpp"
        break;

    case 158: /* string_builder_body  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2515 "ds_parser.cpp"
        break;

    case 159: /* string_builder  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2521 "ds_parser.cpp"
        break;

    case 163: /* require_module_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2527 "ds_parser.cpp"
        break;

    case 168: /* expression_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2533 "ds_parser.cpp"
        break;

    case 169: /* expression_if_then_else  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2539 "ds_parser.cpp"
        break;

    case 170: /* expression_for_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2545 "ds_parser.cpp"
        break;

    case 171: /* expression_while_loop  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2551 "ds_parser.cpp"
        break;

    case 172: /* expression_with  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2557 "ds_parser.cpp"
        break;

    case 173: /* annotation_argument  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2563 "ds_parser.cpp"
        break;

    case 174: /* annotation_argument_list  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2569 "ds_parser.cpp"
        break;

    case 175: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2575 "ds_parser.cpp"
        break;

    case 176: /* annotation_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2581 "ds_parser.cpp"
        break;

    case 177: /* annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2587 "ds_parser.cpp"
        break;

    case 178: /* optional_annotation_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2593 "ds_parser.cpp"
        break;

    case 179: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2599 "ds_parser.cpp"
        break;

    case 180: /* optional_function_type  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2605 "ds_parser.cpp"
        break;

    case 181: /* function_name  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2611 "ds_parser.cpp"
        break;

    case 183: /* expression_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2617 "ds_parser.cpp"
        break;

    case 184: /* expression_any  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2623 "ds_parser.cpp"
        break;

    case 185: /* expressions  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2629 "ds_parser.cpp"
        break;

    case 186: /* expr_pipe  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2635 "ds_parser.cpp"
        break;

    case 187: /* name_in_namespace  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2641 "ds_parser.cpp"
        break;

    case 188: /* expression_delete  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2647 "ds_parser.cpp"
        break;

    case 189: /* expr_new  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2653 "ds_parser.cpp"
        break;

    case 190: /* expression_break  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2659 "ds_parser.cpp"
        break;

    case 191: /* expression_continue  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2665 "ds_parser.cpp"
        break;

    case 192: /* expression_return  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2671 "ds_parser.cpp"
        break;

    case 193: /* expression_try_catch  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2677 "ds_parser.cpp"
        break;

    case 196: /* expression_let  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2683 "ds_parser.cpp"
        break;

    case 197: /* expr_cast  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2689 "ds_parser.cpp"
        break;

    case 198: /* expr_type_info  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2695 "ds_parser.cpp"
        break;

    case 199: /* expr_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2701 "ds_parser.cpp"
        break;

    case 200: /* expr_block  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2707 "ds_parser.cpp"
        break;

    case 201: /* expr_numeric_const  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2713 "ds_parser.cpp"
        break;

    case 202: /* expr_assign  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2719 "ds_parser.cpp"
        break;

    case 203: /* expr_named_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2725 "ds_parser.cpp"
        break;

    case 204: /* expr_method_call  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2731 "ds_parser.cpp"
        break;

    case 205: /* expr  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2737 "ds_parser.cpp"
        break;

    case 206: /* optional_field_annotation  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2743 "ds_parser.cpp"
        break;

    case 208: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2749 "ds_parser.cpp"
        break;

    case 209: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2755 "ds_parser.cpp"
        break;

    case 210: /* function_argument_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2761 "ds_parser.cpp"
        break;

    case 211: /* function_argument_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2767 "ds_parser.cpp"
        break;

    case 212: /* tuple_type  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2773 "ds_parser.cpp"
        break;

    case 213: /* tuple_type_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2779 "ds_parser.cpp"
        break;

    case 215: /* variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2785 "ds_parser.cpp"
        break;

    case 217: /* let_variable_declaration  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 2791 "ds_parser.cpp"
        break;

    case 218: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2797 "ds_parser.cpp"
        break;

    case 220: /* enum_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 2803 "ds_parser.cpp"
        break;

    case 223: /* optional_structure_parent  */
#line 101 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2809 "ds_parser.cpp"
        break;

    case 226: /* variable_name_list  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 2815 "ds_parser.cpp"
        break;

    case 228: /* structure_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2821 "ds_parser.cpp"
        break;

    case 229: /* auto_type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2827 "ds_parser.cpp"
        break;

    case 230: /* type_declaration  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2833 "ds_parser.cpp"
        break;

    case 231: /* make_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2839 "ds_parser.cpp"
        break;

    case 232: /* make_struct_fields  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 2845 "ds_parser.cpp"
        break;

    case 233: /* make_struct_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2851 "ds_parser.cpp"
        break;

    case 234: /* make_struct_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2857 "ds_parser.cpp"
        break;

    case 235: /* make_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2863 "ds_parser.cpp"
        break;

    case 236: /* make_map_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2869 "ds_parser.cpp"
        break;

    case 237: /* make_any_tuple  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2875 "ds_parser.cpp"
        break;

    case 238: /* make_dim  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2881 "ds_parser.cpp"
        break;

    case 239: /* make_dim_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2887 "ds_parser.cpp"
        break;

    case 240: /* make_table  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2893 "ds_parser.cpp"
        break;

    case 241: /* make_table_decl  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2899 "ds_parser.cpp"
        break;

    case 242: /* array_comprehension_where  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2905 "ds_parser.cpp"
        break;

    case 243: /* array_comprehension  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2911 "ds_parser.cpp"
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
  case 11:
#line 281 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3203 "ds_parser.cpp"
    break;

  case 12:
#line 282 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3209 "ds_parser.cpp"
    break;

  case 13:
#line 286 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3215 "ds_parser.cpp"
    break;

  case 14:
#line 290 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 3223 "ds_parser.cpp"
    break;

  case 15:
#line 293 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3237 "ds_parser.cpp"
    break;

  case 16:
#line 302 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3247 "ds_parser.cpp"
    break;

  case 17:
#line 310 "ds_parser.ypp"
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
#line 3265 "ds_parser.cpp"
    break;

  case 18:
#line 326 "ds_parser.ypp"
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
#line 3288 "ds_parser.cpp"
    break;

  case 22:
#line 356 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3296 "ds_parser.cpp"
    break;

  case 23:
#line 359 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3307 "ds_parser.cpp"
    break;

  case 24:
#line 368 "ds_parser.ypp"
    {
        auto modN = getModuleName(*(yyvsp[0].s));
        if ( !g_Program->addModule(modN) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3320 "ds_parser.cpp"
    break;

  case 28:
#line 388 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3328 "ds_parser.cpp"
    break;

  case 29:
#line 391 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3336 "ds_parser.cpp"
    break;

  case 30:
#line 397 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3342 "ds_parser.cpp"
    break;

  case 31:
#line 398 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3348 "ds_parser.cpp"
    break;

  case 32:
#line 399 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3357 "ds_parser.cpp"
    break;

  case 33:
#line 406 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3366 "ds_parser.cpp"
    break;

  case 34:
#line 413 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3380 "ds_parser.cpp"
    break;

  case 35:
#line 425 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3392 "ds_parser.cpp"
    break;

  case 36:
#line 435 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3403 "ds_parser.cpp"
    break;

  case 37:
#line 444 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3409 "ds_parser.cpp"
    break;

  case 38:
#line 445 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3415 "ds_parser.cpp"
    break;

  case 39:
#line 446 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3421 "ds_parser.cpp"
    break;

  case 40:
#line 447 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3427 "ds_parser.cpp"
    break;

  case 41:
#line 448 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3433 "ds_parser.cpp"
    break;

  case 42:
#line 449 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3439 "ds_parser.cpp"
    break;

  case 43:
#line 453 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3449 "ds_parser.cpp"
    break;

  case 44:
#line 458 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3459 "ds_parser.cpp"
    break;

  case 45:
#line 466 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 3465 "ds_parser.cpp"
    break;

  case 46:
#line 467 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 3471 "ds_parser.cpp"
    break;

  case 47:
#line 471 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3483 "ds_parser.cpp"
    break;

  case 48:
#line 478 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3497 "ds_parser.cpp"
    break;

  case 49:
#line 490 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3506 "ds_parser.cpp"
    break;

  case 50:
#line 494 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3515 "ds_parser.cpp"
    break;

  case 51:
#line 501 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 3521 "ds_parser.cpp"
    break;

  case 52:
#line 502 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3527 "ds_parser.cpp"
    break;

  case 53:
#line 506 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3533 "ds_parser.cpp"
    break;

  case 54:
#line 507 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 3539 "ds_parser.cpp"
    break;

  case 55:
#line 508 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3545 "ds_parser.cpp"
    break;

  case 56:
#line 512 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3554 "ds_parser.cpp"
    break;

  case 57:
#line 516 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3563 "ds_parser.cpp"
    break;

  case 58:
#line 523 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3572 "ds_parser.cpp"
    break;

  case 59:
#line 527 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 3578 "ds_parser.cpp"
    break;

  case 60:
#line 528 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 3584 "ds_parser.cpp"
    break;

  case 61:
#line 529 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 3590 "ds_parser.cpp"
    break;

  case 62:
#line 530 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 3596 "ds_parser.cpp"
    break;

  case 63:
#line 531 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 3602 "ds_parser.cpp"
    break;

  case 64:
#line 532 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 3608 "ds_parser.cpp"
    break;

  case 65:
#line 533 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 3614 "ds_parser.cpp"
    break;

  case 66:
#line 534 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 3620 "ds_parser.cpp"
    break;

  case 67:
#line 535 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 3626 "ds_parser.cpp"
    break;

  case 68:
#line 536 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 3632 "ds_parser.cpp"
    break;

  case 69:
#line 537 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 3638 "ds_parser.cpp"
    break;

  case 70:
#line 538 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 3644 "ds_parser.cpp"
    break;

  case 71:
#line 539 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 3650 "ds_parser.cpp"
    break;

  case 72:
#line 540 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 3656 "ds_parser.cpp"
    break;

  case 73:
#line 541 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 3662 "ds_parser.cpp"
    break;

  case 74:
#line 542 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 3668 "ds_parser.cpp"
    break;

  case 75:
#line 543 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 3674 "ds_parser.cpp"
    break;

  case 76:
#line 544 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 3680 "ds_parser.cpp"
    break;

  case 77:
#line 545 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 3686 "ds_parser.cpp"
    break;

  case 78:
#line 546 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 3692 "ds_parser.cpp"
    break;

  case 79:
#line 547 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 3698 "ds_parser.cpp"
    break;

  case 80:
#line 548 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 3704 "ds_parser.cpp"
    break;

  case 81:
#line 549 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 3710 "ds_parser.cpp"
    break;

  case 82:
#line 550 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 3716 "ds_parser.cpp"
    break;

  case 83:
#line 551 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 3722 "ds_parser.cpp"
    break;

  case 84:
#line 552 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 3728 "ds_parser.cpp"
    break;

  case 85:
#line 553 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 3734 "ds_parser.cpp"
    break;

  case 86:
#line 554 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 3740 "ds_parser.cpp"
    break;

  case 87:
#line 555 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 3746 "ds_parser.cpp"
    break;

  case 88:
#line 556 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 3752 "ds_parser.cpp"
    break;

  case 89:
#line 557 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 3758 "ds_parser.cpp"
    break;

  case 90:
#line 558 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 3764 "ds_parser.cpp"
    break;

  case 91:
#line 559 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 3770 "ds_parser.cpp"
    break;

  case 92:
#line 560 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 3776 "ds_parser.cpp"
    break;

  case 93:
#line 561 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 3782 "ds_parser.cpp"
    break;

  case 94:
#line 562 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 3788 "ds_parser.cpp"
    break;

  case 95:
#line 567 "ds_parser.ypp"
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
#line 3859 "ds_parser.cpp"
    break;

  case 96:
#line 636 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3867 "ds_parser.cpp"
    break;

  case 97:
#line 639 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3879 "ds_parser.cpp"
    break;

  case 98:
#line 649 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3885 "ds_parser.cpp"
    break;

  case 99:
#line 650 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3891 "ds_parser.cpp"
    break;

  case 100:
#line 651 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3897 "ds_parser.cpp"
    break;

  case 101:
#line 652 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3903 "ds_parser.cpp"
    break;

  case 102:
#line 653 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3909 "ds_parser.cpp"
    break;

  case 103:
#line 654 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3915 "ds_parser.cpp"
    break;

  case 104:
#line 655 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3921 "ds_parser.cpp"
    break;

  case 105:
#line 656 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3927 "ds_parser.cpp"
    break;

  case 106:
#line 657 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3933 "ds_parser.cpp"
    break;

  case 107:
#line 658 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3939 "ds_parser.cpp"
    break;

  case 108:
#line 659 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3945 "ds_parser.cpp"
    break;

  case 109:
#line 660 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3951 "ds_parser.cpp"
    break;

  case 110:
#line 661 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3957 "ds_parser.cpp"
    break;

  case 111:
#line 665 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
    }
#line 3965 "ds_parser.cpp"
    break;

  case 112:
#line 668 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
			if ( !(yyvsp[-1].pExpression)->at.line ) {
				(yyvsp[-1].pExpression)->at = (yyvsp[0].pExpression)->at;
			}
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3979 "ds_parser.cpp"
    break;

  case 113:
#line 677 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3987 "ds_parser.cpp"
    break;

  case 114:
#line 683 "ds_parser.ypp"
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
#line 4003 "ds_parser.cpp"
    break;

  case 115:
#line 697 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4009 "ds_parser.cpp"
    break;

  case 116:
#line 698 "ds_parser.ypp"
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4020 "ds_parser.cpp"
    break;

  case 117:
#line 704 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4026 "ds_parser.cpp"
    break;

  case 118:
#line 708 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4034 "ds_parser.cpp"
    break;

  case 119:
#line 714 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4042 "ds_parser.cpp"
    break;

  case 120:
#line 717 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4050 "ds_parser.cpp"
    break;

  case 121:
#line 720 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4059 "ds_parser.cpp"
    break;

  case 122:
#line 724 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4067 "ds_parser.cpp"
    break;

  case 123:
#line 730 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4073 "ds_parser.cpp"
    break;

  case 124:
#line 734 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4079 "ds_parser.cpp"
    break;

  case 125:
#line 738 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4087 "ds_parser.cpp"
    break;

  case 126:
#line 741 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4095 "ds_parser.cpp"
    break;

  case 127:
#line 744 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4105 "ds_parser.cpp"
    break;

  case 128:
#line 749 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4113 "ds_parser.cpp"
    break;

  case 129:
#line 752 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4123 "ds_parser.cpp"
    break;

  case 130:
#line 760 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4131 "ds_parser.cpp"
    break;

  case 131:
#line 766 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4137 "ds_parser.cpp"
    break;

  case 132:
#line 767 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4143 "ds_parser.cpp"
    break;

  case 133:
#line 771 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4149 "ds_parser.cpp"
    break;

  case 134:
#line 772 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4155 "ds_parser.cpp"
    break;

  case 135:
#line 776 "ds_parser.ypp"
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
#line 4190 "ds_parser.cpp"
    break;

  case 136:
#line 809 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4198 "ds_parser.cpp"
    break;

  case 137:
#line 812 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4208 "ds_parser.cpp"
    break;

  case 138:
#line 817 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4218 "ds_parser.cpp"
    break;

  case 139:
#line 825 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4227 "ds_parser.cpp"
    break;

  case 140:
#line 829 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4237 "ds_parser.cpp"
    break;

  case 141:
#line 834 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4246 "ds_parser.cpp"
    break;

  case 142:
#line 838 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4256 "ds_parser.cpp"
    break;

  case 143:
#line 846 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4264 "ds_parser.cpp"
    break;

  case 144:
#line 849 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4272 "ds_parser.cpp"
    break;

  case 145:
#line 855 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4282 "ds_parser.cpp"
    break;

  case 146:
#line 861 "ds_parser.ypp"
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
#line 4335 "ds_parser.cpp"
    break;

  case 147:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4341 "ds_parser.cpp"
    break;

  case 148:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4347 "ds_parser.cpp"
    break;

  case 149:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4353 "ds_parser.cpp"
    break;

  case 150:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4359 "ds_parser.cpp"
    break;

  case 151:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4365 "ds_parser.cpp"
    break;

  case 152:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4371 "ds_parser.cpp"
    break;

  case 153:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4377 "ds_parser.cpp"
    break;

  case 154:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4383 "ds_parser.cpp"
    break;

  case 155:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4389 "ds_parser.cpp"
    break;

  case 156:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4395 "ds_parser.cpp"
    break;

  case 157:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4401 "ds_parser.cpp"
    break;

  case 158:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4407 "ds_parser.cpp"
    break;

  case 159:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4413 "ds_parser.cpp"
    break;

  case 160:
#line 928 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4419 "ds_parser.cpp"
    break;

  case 161:
#line 929 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4425 "ds_parser.cpp"
    break;

  case 162:
#line 930 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4431 "ds_parser.cpp"
    break;

  case 163:
#line 931 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4437 "ds_parser.cpp"
    break;

  case 164:
#line 932 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4443 "ds_parser.cpp"
    break;

  case 165:
#line 933 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4449 "ds_parser.cpp"
    break;

  case 166:
#line 934 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4455 "ds_parser.cpp"
    break;

  case 167:
#line 935 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4461 "ds_parser.cpp"
    break;

  case 168:
#line 936 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4467 "ds_parser.cpp"
    break;

  case 169:
#line 940 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4479 "ds_parser.cpp"
    break;

  case 170:
#line 951 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4489 "ds_parser.cpp"
    break;

  case 171:
#line 956 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4501 "ds_parser.cpp"
    break;

  case 172:
#line 966 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4507 "ds_parser.cpp"
    break;

  case 173:
#line 967 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4513 "ds_parser.cpp"
    break;

  case 174:
#line 968 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4519 "ds_parser.cpp"
    break;

  case 175:
#line 969 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4525 "ds_parser.cpp"
    break;

  case 176:
#line 970 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4531 "ds_parser.cpp"
    break;

  case 177:
#line 971 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4537 "ds_parser.cpp"
    break;

  case 178:
#line 972 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4543 "ds_parser.cpp"
    break;

  case 179:
#line 973 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4549 "ds_parser.cpp"
    break;

  case 180:
#line 974 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4555 "ds_parser.cpp"
    break;

  case 181:
#line 975 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4561 "ds_parser.cpp"
    break;

  case 182:
#line 976 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4567 "ds_parser.cpp"
    break;

  case 183:
#line 977 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4573 "ds_parser.cpp"
    break;

  case 184:
#line 978 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4579 "ds_parser.cpp"
    break;

  case 185:
#line 979 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4585 "ds_parser.cpp"
    break;

  case 186:
#line 980 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4591 "ds_parser.cpp"
    break;

  case 187:
#line 981 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4597 "ds_parser.cpp"
    break;

  case 188:
#line 982 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4603 "ds_parser.cpp"
    break;

  case 189:
#line 983 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4609 "ds_parser.cpp"
    break;

  case 190:
#line 984 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4615 "ds_parser.cpp"
    break;

  case 191:
#line 985 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4621 "ds_parser.cpp"
    break;

  case 192:
#line 986 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4627 "ds_parser.cpp"
    break;

  case 193:
#line 987 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4633 "ds_parser.cpp"
    break;

  case 194:
#line 988 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4639 "ds_parser.cpp"
    break;

  case 195:
#line 989 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4645 "ds_parser.cpp"
    break;

  case 196:
#line 990 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4651 "ds_parser.cpp"
    break;

  case 197:
#line 991 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4657 "ds_parser.cpp"
    break;

  case 198:
#line 992 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4663 "ds_parser.cpp"
    break;

  case 199:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4669 "ds_parser.cpp"
    break;

  case 200:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4675 "ds_parser.cpp"
    break;

  case 201:
#line 995 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4681 "ds_parser.cpp"
    break;

  case 202:
#line 996 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4687 "ds_parser.cpp"
    break;

  case 203:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4693 "ds_parser.cpp"
    break;

  case 204:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4699 "ds_parser.cpp"
    break;

  case 205:
#line 999 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4705 "ds_parser.cpp"
    break;

  case 206:
#line 1000 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4711 "ds_parser.cpp"
    break;

  case 207:
#line 1001 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4717 "ds_parser.cpp"
    break;

  case 208:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4723 "ds_parser.cpp"
    break;

  case 209:
#line 1003 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4729 "ds_parser.cpp"
    break;

  case 210:
#line 1004 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4738 "ds_parser.cpp"
    break;

  case 211:
#line 1008 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4747 "ds_parser.cpp"
    break;

  case 212:
#line 1012 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4755 "ds_parser.cpp"
    break;

  case 213:
#line 1015 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4763 "ds_parser.cpp"
    break;

  case 214:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4769 "ds_parser.cpp"
    break;

  case 215:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4775 "ds_parser.cpp"
    break;

  case 216:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4781 "ds_parser.cpp"
    break;

  case 217:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4787 "ds_parser.cpp"
    break;

  case 218:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4793 "ds_parser.cpp"
    break;

  case 219:
#line 1023 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4801 "ds_parser.cpp"
    break;

  case 220:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4807 "ds_parser.cpp"
    break;

  case 221:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4813 "ds_parser.cpp"
    break;

  case 222:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4819 "ds_parser.cpp"
    break;

  case 223:
#line 1029 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4825 "ds_parser.cpp"
    break;

  case 224:
#line 1030 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4831 "ds_parser.cpp"
    break;

  case 225:
#line 1031 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4837 "ds_parser.cpp"
    break;

  case 226:
#line 1032 "ds_parser.ypp"
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
#line 4852 "ds_parser.cpp"
    break;

  case 227:
#line 1042 "ds_parser.ypp"
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
#line 4867 "ds_parser.cpp"
    break;

  case 228:
#line 1052 "ds_parser.ypp"
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
#line 4903 "ds_parser.cpp"
    break;

  case 229:
#line 1086 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 4909 "ds_parser.cpp"
    break;

  case 230:
#line 1087 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4915 "ds_parser.cpp"
    break;

  case 231:
#line 1091 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4921 "ds_parser.cpp"
    break;

  case 232:
#line 1092 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4927 "ds_parser.cpp"
    break;

  case 233:
#line 1096 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4937 "ds_parser.cpp"
    break;

  case 234:
#line 1104 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4945 "ds_parser.cpp"
    break;

  case 235:
#line 1107 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4954 "ds_parser.cpp"
    break;

  case 236:
#line 1114 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4960 "ds_parser.cpp"
    break;

  case 237:
#line 1115 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4966 "ds_parser.cpp"
    break;

  case 238:
#line 1119 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4972 "ds_parser.cpp"
    break;

  case 239:
#line 1120 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4978 "ds_parser.cpp"
    break;

  case 240:
#line 1124 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4986 "ds_parser.cpp"
    break;

  case 241:
#line 1130 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4992 "ds_parser.cpp"
    break;

  case 242:
#line 1131 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4998 "ds_parser.cpp"
    break;

  case 243:
#line 1135 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5004 "ds_parser.cpp"
    break;

  case 244:
#line 1136 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5010 "ds_parser.cpp"
    break;

  case 245:
#line 1140 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5019 "ds_parser.cpp"
    break;

  case 246:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5027 "ds_parser.cpp"
    break;

  case 247:
#line 1147 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5036 "ds_parser.cpp"
    break;

  case 248:
#line 1151 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5046 "ds_parser.cpp"
    break;

  case 249:
#line 1156 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5056 "ds_parser.cpp"
    break;

  case 250:
#line 1164 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5062 "ds_parser.cpp"
    break;

  case 251:
#line 1165 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5068 "ds_parser.cpp"
    break;

  case 252:
#line 1166 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5074 "ds_parser.cpp"
    break;

  case 253:
#line 1170 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5082 "ds_parser.cpp"
    break;

  case 254:
#line 1173 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5092 "ds_parser.cpp"
    break;

  case 255:
#line 1178 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5102 "ds_parser.cpp"
    break;

  case 256:
#line 1183 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5113 "ds_parser.cpp"
    break;

  case 257:
#line 1189 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5124 "ds_parser.cpp"
    break;

  case 258:
#line 1198 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5132 "ds_parser.cpp"
    break;

  case 259:
#line 1201 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5141 "ds_parser.cpp"
    break;

  case 260:
#line 1208 "ds_parser.ypp"
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
#line 5172 "ds_parser.cpp"
    break;

  case 261:
#line 1237 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5180 "ds_parser.cpp"
    break;

  case 262:
#line 1240 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5194 "ds_parser.cpp"
    break;

  case 263:
#line 1249 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5208 "ds_parser.cpp"
    break;

  case 264:
#line 1261 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5222 "ds_parser.cpp"
    break;

  case 265:
#line 1273 "ds_parser.ypp"
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
#line 5238 "ds_parser.cpp"
    break;

  case 266:
#line 1287 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5244 "ds_parser.cpp"
    break;

  case 267:
#line 1288 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5250 "ds_parser.cpp"
    break;

  case 268:
#line 1292 "ds_parser.ypp"
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
#line 5291 "ds_parser.cpp"
    break;

  case 269:
#line 1331 "ds_parser.ypp"
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
#line 5359 "ds_parser.cpp"
    break;

  case 270:
#line 1397 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5371 "ds_parser.cpp"
    break;

  case 271:
#line 1404 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5382 "ds_parser.cpp"
    break;

  case 272:
#line 1413 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 5388 "ds_parser.cpp"
    break;

  case 273:
#line 1414 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 5394 "ds_parser.cpp"
    break;

  case 274:
#line 1415 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 5400 "ds_parser.cpp"
    break;

  case 275:
#line 1416 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 5406 "ds_parser.cpp"
    break;

  case 276:
#line 1417 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 5412 "ds_parser.cpp"
    break;

  case 277:
#line 1418 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 5418 "ds_parser.cpp"
    break;

  case 278:
#line 1419 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 5424 "ds_parser.cpp"
    break;

  case 279:
#line 1420 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 5430 "ds_parser.cpp"
    break;

  case 280:
#line 1421 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 5436 "ds_parser.cpp"
    break;

  case 281:
#line 1422 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 5442 "ds_parser.cpp"
    break;

  case 282:
#line 1423 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 5448 "ds_parser.cpp"
    break;

  case 283:
#line 1424 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 5454 "ds_parser.cpp"
    break;

  case 284:
#line 1425 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 5460 "ds_parser.cpp"
    break;

  case 285:
#line 1426 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 5466 "ds_parser.cpp"
    break;

  case 286:
#line 1427 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 5472 "ds_parser.cpp"
    break;

  case 287:
#line 1428 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 5478 "ds_parser.cpp"
    break;

  case 288:
#line 1429 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 5484 "ds_parser.cpp"
    break;

  case 289:
#line 1430 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 5490 "ds_parser.cpp"
    break;

  case 290:
#line 1431 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 5496 "ds_parser.cpp"
    break;

  case 291:
#line 1432 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 5502 "ds_parser.cpp"
    break;

  case 292:
#line 1433 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 5508 "ds_parser.cpp"
    break;

  case 293:
#line 1434 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 5514 "ds_parser.cpp"
    break;

  case 294:
#line 1435 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 5520 "ds_parser.cpp"
    break;

  case 295:
#line 1436 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 5526 "ds_parser.cpp"
    break;

  case 296:
#line 1440 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 5538 "ds_parser.cpp"
    break;

  case 297:
#line 1450 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5547 "ds_parser.cpp"
    break;

  case 298:
#line 1454 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5559 "ds_parser.cpp"
    break;

  case 299:
#line 1464 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5565 "ds_parser.cpp"
    break;

  case 300:
#line 1465 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5571 "ds_parser.cpp"
    break;

  case 301:
#line 1466 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5577 "ds_parser.cpp"
    break;

  case 302:
#line 1467 "ds_parser.ypp"
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
#line 5596 "ds_parser.cpp"
    break;

  case 303:
#line 1481 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5607 "ds_parser.cpp"
    break;

  case 304:
#line 1487 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5616 "ds_parser.cpp"
    break;

  case 305:
#line 1491 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5626 "ds_parser.cpp"
    break;

  case 306:
#line 1496 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5636 "ds_parser.cpp"
    break;

  case 307:
#line 1501 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5646 "ds_parser.cpp"
    break;

  case 308:
#line 1506 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5656 "ds_parser.cpp"
    break;

  case 309:
#line 1511 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5668 "ds_parser.cpp"
    break;

  case 310:
#line 1518 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5677 "ds_parser.cpp"
    break;

  case 311:
#line 1522 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5687 "ds_parser.cpp"
    break;

  case 312:
#line 1527 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5698 "ds_parser.cpp"
    break;

  case 313:
#line 1533 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5708 "ds_parser.cpp"
    break;

  case 314:
#line 1538 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5717 "ds_parser.cpp"
    break;

  case 315:
#line 1542 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5727 "ds_parser.cpp"
    break;

  case 316:
#line 1547 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5741 "ds_parser.cpp"
    break;

  case 317:
#line 1556 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5750 "ds_parser.cpp"
    break;

  case 318:
#line 1560 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5760 "ds_parser.cpp"
    break;

  case 319:
#line 1565 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5774 "ds_parser.cpp"
    break;

  case 320:
#line 1574 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5783 "ds_parser.cpp"
    break;

  case 321:
#line 1578 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5793 "ds_parser.cpp"
    break;

  case 322:
#line 1583 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5807 "ds_parser.cpp"
    break;

  case 323:
#line 1592 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5818 "ds_parser.cpp"
    break;

  case 324:
#line 1601 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5824 "ds_parser.cpp"
    break;

  case 325:
#line 1602 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5830 "ds_parser.cpp"
    break;

  case 326:
#line 1603 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5836 "ds_parser.cpp"
    break;

  case 327:
#line 1604 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5842 "ds_parser.cpp"
    break;

  case 328:
#line 1608 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5854 "ds_parser.cpp"
    break;

  case 329:
#line 1615 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5865 "ds_parser.cpp"
    break;

  case 330:
#line 1624 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5875 "ds_parser.cpp"
    break;

  case 331:
#line 1629 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5884 "ds_parser.cpp"
    break;

  case 332:
#line 1636 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5894 "ds_parser.cpp"
    break;

  case 333:
#line 1641 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5905 "ds_parser.cpp"
    break;

  case 334:
#line 1650 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5913 "ds_parser.cpp"
    break;

  case 335:
#line 1653 "ds_parser.ypp"
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
#line 5929 "ds_parser.cpp"
    break;

  case 336:
#line 1667 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5940 "ds_parser.cpp"
    break;

  case 337:
#line 1676 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5946 "ds_parser.cpp"
    break;

  case 338:
#line 1677 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5952 "ds_parser.cpp"
    break;

  case 339:
#line 1681 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5962 "ds_parser.cpp"
    break;

  case 340:
#line 1686 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5971 "ds_parser.cpp"
    break;

  case 341:
#line 1693 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5981 "ds_parser.cpp"
    break;

  case 342:
#line 1698 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 5994 "ds_parser.cpp"
    break;

  case 343:
#line 1709 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6004 "ds_parser.cpp"
    break;

  case 344:
#line 1714 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6013 "ds_parser.cpp"
    break;

  case 345:
#line 1721 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6027 "ds_parser.cpp"
    break;

  case 346:
#line 1733 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6033 "ds_parser.cpp"
    break;

  case 347:
#line 1734 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6039 "ds_parser.cpp"
    break;

  case 348:
#line 1738 "ds_parser.ypp"
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
#line 6057 "ds_parser.cpp"
    break;


#line 6061 "ds_parser.cpp"

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
#line 1753 "ds_parser.ypp"


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


