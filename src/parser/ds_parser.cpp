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
#line 43 "ds_parser.ypp" /* yacc.c:338  */

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
# define YYERROR_VERBOSE 0
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
        bool                    move_to_init = false;
        bool                    override = false;
        AnnotationArgumentList  *annotation = nullptr;
    };
    
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 182 "ds_parser.cpp" /* yacc.c:353  */

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
#line 67 "ds_parser.ypp" /* yacc.c:353  */

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

#line 347 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   3988

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  88
/* YYNRULES -- Number of rules.  */
#define YYNRULES  341
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  613

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   382

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
       0,   256,   256,   258,   259,   260,   261,   262,   263,   264,
     265,   269,   270,   274,   278,   281,   290,   298,   314,   335,
     339,   340,   344,   347,   356,   367,   371,   372,   376,   379,
     385,   386,   387,   394,   401,   413,   423,   432,   433,   434,
     435,   436,   437,   441,   446,   454,   455,   459,   466,   478,
     482,   489,   490,   494,   495,   496,   500,   504,   511,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   554,   623,   626,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,   647,
     648,   652,   655,   664,   670,   684,   685,   691,   695,   701,
     704,   707,   711,   717,   721,   725,   728,   731,   739,   745,
     746,   750,   751,   755,   787,   790,   795,   803,   807,   812,
     816,   824,   827,   833,   838,   889,   890,   891,   892,   893,
     894,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   917,   928,   933,
     943,   944,   945,   946,   947,   948,   949,   950,   951,   952,
     953,   954,   955,   956,   957,   958,   959,   960,   961,   962,
     963,   964,   965,   966,   967,   968,   969,   970,   971,   972,
     973,   974,   975,   976,   977,   978,   979,   980,   981,   985,
     989,   992,   995,   996,   997,   998,   999,  1000,  1003,  1004,
    1005,  1006,  1007,  1008,  1009,  1019,  1029,  1063,  1064,  1068,
    1069,  1073,  1081,  1084,  1091,  1092,  1096,  1097,  1101,  1107,
    1108,  1112,  1113,  1117,  1121,  1124,  1128,  1133,  1141,  1144,
    1148,  1152,  1157,  1165,  1193,  1196,  1205,  1217,  1229,  1243,
    1244,  1248,  1287,  1353,  1360,  1369,  1370,  1371,  1372,  1373,
    1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,
    1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1396,
    1406,  1410,  1420,  1421,  1422,  1423,  1437,  1443,  1447,  1452,
    1457,  1462,  1467,  1474,  1478,  1483,  1489,  1494,  1498,  1503,
    1512,  1516,  1521,  1530,  1534,  1539,  1548,  1557,  1558,  1559,
    1560,  1564,  1571,  1580,  1585,  1592,  1597,  1606,  1609,  1623,
    1632,  1633,  1637,  1642,  1649,  1654,  1665,  1670,  1677,  1689,
    1690,  1694
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 0
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
  "let_variable_declaration", "global_let", "enum_list",
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

#define YYPACT_NINF -381

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-381)))

#define YYTABLE_NINF -152

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -381,    26,  -381,  -381,   -86,    -9,    22,    28,   120,  -381,
     -13,  -381,  -381,  -381,    39,  -381,    99,  -381,  -381,  -381,
    -381,   -17,  -381,   123,   147,   170,  -381,  -381,   176,   141,
    -381,   151,   181,  -381,   156,   195,   172,  -381,   -91,  -381,
     203,    29,  -381,   300,   -86,   211,    -9,    22,   206,  -381,
    2569,   209,  -381,   -86,   -13,  -381,   198,   174,  3314,   297,
     298,  -381,   190,  -120,  -381,  -381,  -381,  -381,  -381,   218,
    -381,  -381,  -381,  -381,  -381,  -381,   -99,   210,   214,   217,
    -381,  -381,  -381,   192,  -381,  -381,  -381,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,   219,  -381,
    -381,  -381,   220,   221,  -381,  -381,  -381,  -381,   223,  -381,
    -381,  -381,  -381,     7,  -381,   -97,  -381,   -92,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,    -6,   229,   -86,  -381,
     317,   216,  -381,   164,  -104,  -381,  2569,  2569,  2569,   242,
    2499,  2499,  2499,  2569,  -381,    20,  -381,  -381,  1471,  -381,
    -381,    93,   252,  -381,  -381,  -381,   127,  -381,    70,  2569,
     225,   107,  -381,   252,  -381,  -381,  -381,   259,  -381,   171,
      72,   188,   213,   229,   207,   229,   227,   229,   235,  -381,
     -88,     7,   243,  -381,   232,  -381,  -381,   595,   237,  -381,
     238,   251,   254,   239,   255,   240,  2391,  2391,    23,  2569,
    2391,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  2391,  2391,
     -92,  2391,  2391,  -381,  2391,  -381,   244,  -381,  -381,   -87,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  3336,   247,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,   -14,  -381,   275,
    -381,  2569,  2391,     7,  -381,  -381,  -381,   253,  -381,  2569,
    -381,  -381,   264,  -381,   281,  -381,   282,  -381,  -381,  2569,
    -381,  -381,   119,  -381,   304,  2391,  2569,  2569,  2391,  2569,
    1600,   427,   427,   252,   971,  1100,  3464,  -381,   124,    66,
     427,   427,  -381,   427,   427,  2910,   676,   190,  -381,  1229,
    2391,  2391,  -381,  -381,  2391,  2391,  2391,  2391,   305,  2391,
     309,  2391,  2391,  2391,  2391,  2391,  2391,  2391,  2391,  2391,
    2391,  2391,  2391,  2391,  2391,  2391,   310,  2391,  -381,  1713,
    -381,  -381,   161,  -381,   314,  2727,  -381,   236,  -381,  -381,
    -381,  -381,  1826,  1342,  2976,   246,   269,  3042,   272,  -381,
    3108,     0,    51,  1939,  3464,   311,     3,   327,  -381,  -381,
      87,   140,  2391,  2391,   299,  -381,  2391,   333,  -381,  -381,
    2391,  2391,   252,  2052,  -381,   225,  2391,  2391,  -381,  -381,
     421,  -381,  -381,  -381,  -381,  -381,  -381,   303,   306,   307,
     308,  -381,   437,  -381,   -84,  2798,   229,   340,  -381,    27,
    3632,   494,   494,  3801,  3801,  3781,  3781,   319,   427,  -381,
     -28,   -28,   494,   494,  3520,  3652,  3708,  3728,  3801,  3801,
    3843,  3843,   672,   672,   672,  -381,  3400,  -381,    53,  2391,
     142,  2391,  2391,  2391,  2391,  2391,  2391,  2391,  2391,  2391,
    2391,  2391,  2391,  2391,  2391,  2391,  -381,  -381,    88,  2569,
     356,  3174,  -381,  2391,  2391,  -381,  2391,  -381,  2391,  2391,
     340,   362,   334,   340,  2391,   335,  2391,   336,  3632,  -381,
    -381,  3576,   377,   377,    19,  2391,  3632,   472,  3632,   377,
     338,  -381,  -381,  -381,  -381,   452,   252,  -381,   225,   103,
     126,  2391,  -381,  2165,  2391,  -381,  -381,  3632,  -381,  3632,
    3632,  3632,  3632,  3632,  3632,  3632,  3632,  3632,  3632,  3632,
    3632,  3632,  3632,  3632,  -381,    79,   358,  -381,  3801,  3801,
    3801,   -11,  3632,   106,   103,  -381,   311,  3632,  -381,  -381,
    -381,  -381,  -381,   178,  2391,  3632,   225,  -381,  -381,  -381,
    -381,    92,  -381,   342,  3632,  -381,   122,  3632,  -381,  2278,
    2391,   345,  2391,   225,  2391,  -381,    55,  -381,   829,  2569,
    2391,  -381,  -381,  2569,  3240,  2854,  -381,  3632,  -381,   377,
     225,  -381,    32,  -381,  2575,    81,  -381,   450,   350,   178,
    -381,  -381,  2391,  -381,  -381,  2391,   351,  -381,  -381,  2651,
    3632,  -381,  -381
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    51,     1,   131,     0,     0,     0,     0,     0,   132,
       0,     9,     8,     7,     0,     6,     0,     5,    10,     4,
       3,     0,    43,    18,    28,    25,    26,    22,    19,    24,
      20,     0,     0,    46,   115,     0,    47,    49,     0,    45,
       0,     0,   232,     0,     0,     0,     0,     0,     0,   254,
       0,     0,   117,     0,     0,    52,   259,     0,     0,     0,
       0,    58,    53,   227,    41,    42,    39,    40,    38,     0,
      37,    44,    29,    27,    21,    23,     0,     0,     0,     0,
     265,   285,   266,   290,   267,   271,   272,   273,   274,   278,
     279,   280,   281,   282,   283,   284,   286,   287,   307,   270,
     277,   288,   310,   313,   268,   275,   269,   276,     0,   289,
     292,   294,   293,   257,   116,     0,    50,     0,   261,   232,
      61,    62,    64,    63,    65,    66,    67,    68,    87,    88,
      85,    86,    78,    89,    90,    79,    76,    77,    91,    92,
      93,    94,    81,    82,    80,    74,    75,    70,    69,    71,
      72,    73,    60,    59,    83,    84,     0,    56,     0,   253,
     229,     0,    11,     0,     0,   258,     0,     0,     0,     0,
      53,    53,    53,     0,   298,     0,   303,   300,     0,    48,
     260,   227,     0,   263,    54,   236,     0,   234,   243,     0,
       0,     0,   230,     0,   233,    12,    13,     0,   255,     0,
       0,     0,     0,    56,     0,    56,     0,    56,     0,   239,
       0,   238,   299,   301,     0,   175,   176,     0,     0,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,   147,   146,   148,   149,   150,    14,     0,     0,
       0,     0,     0,   296,     0,   111,    51,   173,   143,   171,
     220,   219,   218,   223,   172,   222,   221,     0,     0,   174,
     317,   318,   319,   320,   262,   235,    55,     0,   242,     0,
     241,     0,     0,    57,    95,   228,   231,     0,   304,     0,
     306,   291,     0,   308,     0,   311,     0,   314,   316,     0,
     302,   297,   119,   122,     0,     0,     0,     0,     0,     0,
       0,   199,   200,     0,     0,     0,     0,   336,     0,     0,
     180,   179,   207,   178,   177,     0,     0,    53,   226,     0,
       0,     0,   201,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,     0,
     237,   264,   244,   247,     0,   246,   256,     0,   309,   312,
     315,   240,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,   115,     0,   327,   323,     0,   330,   331,   332,
       0,     0,     0,     0,     0,    17,     0,    15,   203,   113,
       0,     0,     0,   125,   123,     0,     0,     0,   124,    98,
      96,   109,   105,   103,   104,   112,    99,     0,     0,     0,
       0,   110,   129,   102,     0,   151,    56,     0,   208,     0,
     141,   181,   182,   194,   195,   192,   193,     0,   216,   206,
     224,   225,   183,   184,     0,   197,   198,   196,   190,   191,
     186,   185,   187,   188,   189,   205,     0,   210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   120,     0,     0,
       0,     0,   212,     0,     0,   213,     0,   215,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   329,   337,
     338,     0,     0,     0,     0,     0,   126,     0,   118,     0,
       0,   101,   106,   107,   108,     0,     0,   100,     0,     0,
       0,     0,   209,     0,     0,   204,   211,   245,   114,   158,
     159,   161,   160,   162,   155,   156,   157,   163,   164,   153,
     154,   165,   166,   152,   121,     0,     0,   137,   134,   135,
     136,     0,   321,     0,     0,   325,   324,   328,   334,   333,
     335,    16,    35,    30,     0,   127,     0,    36,   111,   130,
     133,     0,   144,     0,   142,   168,     0,   217,   139,     0,
       0,     0,     0,     0,     0,    33,     0,   128,     0,     0,
       0,   167,   169,     0,     0,   339,   326,   322,    31,     0,
       0,    97,     0,   252,   151,     0,   138,     0,     0,    30,
      34,   248,     0,   251,   140,     0,     0,    32,   250,   151,
     340,   341,   249
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -381,  -381,   205,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
     454,  -381,  -381,   469,   -83,  -381,  -381,  -381,  -381,   473,
     -46,  -381,   464,  -381,   273,  -150,  -195,  -381,  -381,  -185,
    -381,   -33,  -271,   -10,  -381,  -381,  -381,  -381,  -381,  -381,
    -381,   526,  -381,  -381,  -381,  -343,    80,  -381,  -313,  -381,
    -381,   102,  -381,  -381,  -381,   410,   265,  -381,   245,  -381,
    -184,  -144,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
    -294,   -48,  -381,  -381,   -39,   316,  -380,    56,  -381,  -381,
    -217,    52,   234,  -381,  -381,  -381,  -381,  -381
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   163,    70,   309,   247,    11,    12,    28,    29,
      30,    13,    25,    26,   575,   401,   402,   403,   404,    22,
      23,    36,    37,    38,    14,   157,   190,    62,    15,   248,
     405,   316,   406,   249,   407,   250,   408,   409,   410,   411,
     506,   412,   413,   251,   252,   419,   253,   254,   354,   255,
     256,   420,   160,   193,   161,    63,   185,   186,   209,   210,
     479,   187,   560,    17,    76,    18,    19,   118,    57,    20,
     188,   258,   111,   112,   211,   259,   375,   376,   260,   377,
     378,   379,   380,   261,   308,   262,   598,   263
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   353,   110,   414,   272,   274,   448,   115,   282,   371,
     284,   113,   286,   307,   450,    33,   164,   478,   197,   468,
     203,   205,   207,    34,    44,   158,     2,   182,   318,    21,
      54,     3,   159,   303,   174,   182,   554,   510,   265,   175,
     109,   288,    77,    40,    39,    41,   198,   212,    78,   276,
       4,   179,     5,   165,     6,    55,     7,    35,    58,   174,
     319,     8,   289,    79,   175,   328,   507,     9,   330,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   494,   174,
      24,   183,    34,   546,   175,    43,   174,   180,   174,   183,
     511,   175,   191,   175,    59,    60,   346,   347,   110,   110,
     110,   269,   110,   110,   110,   110,   268,   199,   200,   201,
     176,   204,   206,   208,   177,   541,    35,    27,    34,   570,
     269,   110,   184,    31,    61,   268,   174,   213,   511,   482,
     273,   175,   178,   483,   270,   176,   109,   109,   109,   177,
     109,   109,   109,   109,   268,   214,   489,   416,   449,   110,
     566,    10,    35,   270,   511,   512,   511,   178,   292,   109,
     110,   110,   601,   162,   385,   386,   268,   573,   174,   304,
     305,   269,   270,   175,   271,   176,   574,   268,   174,   177,
      51,   516,   176,   175,   176,   590,   177,   109,   177,   511,
     497,   576,   561,   269,   270,   174,   579,   178,   109,   109,
     175,   508,   279,   110,   178,   270,   178,   568,    44,   604,
     312,   110,   352,   485,   174,    32,   534,   486,   158,   175,
     357,   110,   176,   511,    44,   264,   177,   481,   110,   110,
      42,   110,   571,   275,   174,   268,   483,   365,   366,   175,
     368,   109,   174,   174,   178,   414,   362,   175,   175,   109,
     582,    45,   563,   174,   383,   266,   384,   267,   175,   109,
     257,   195,   196,   270,   176,    48,   109,   109,   177,   109,
     486,    46,   487,   245,   176,   246,   174,    47,   177,   174,
     278,   175,    49,    50,   175,    51,   178,   552,   553,   593,
      52,   176,    64,    65,   557,   177,   178,   280,    56,    53,
      72,    75,   117,   562,   114,   119,   154,   155,   301,   302,
     176,   608,   306,   178,   177,   162,   283,   156,   166,   169,
     310,   311,   167,   313,   314,   168,   315,   170,   171,   172,
     176,   173,   178,   189,   177,   192,   285,   202,   176,   176,
     572,   281,   177,   177,   287,   466,   194,   183,   277,   176,
     290,   577,   178,   177,   355,   473,   245,   580,   291,   296,
     178,   178,   297,   299,   294,   295,   298,   300,   588,    10,
     351,   178,   176,   358,   349,   176,   177,   364,   474,   177,
     367,   476,   370,   356,   599,   600,   374,   374,   602,    66,
     359,   360,   450,    67,   178,    68,    69,   178,   415,   363,
     427,   110,   421,   422,   429,   445,   423,   424,   425,   426,
     535,   428,   481,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   484,   446,
     195,   490,   500,   501,   505,   509,   502,   503,   504,   109,
     320,   321,   322,   323,   324,   471,   513,   325,   326,   327,
     328,   536,   329,   330,   331,   315,   332,   544,   333,   334,
     545,   548,   550,   556,   488,   306,   559,   569,   491,   558,
     581,   586,   492,   493,   605,   496,   606,   611,   498,   499,
     335,    74,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   322,   323,   387,    73,   607,    71,   116,   317,
     328,   346,   347,   330,   331,   578,   332,    16,   245,   181,
     518,   110,   350,   293,   361,   110,   543,     0,   549,   381,
     592,     0,     0,     0,   595,     0,     0,     0,     0,     0,
       0,   517,     0,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,   109,
       0,   346,   347,   109,     0,   538,   539,     0,   540,   322,
     323,   542,     0,     0,     0,     0,   547,   328,   374,   329,
     330,   331,     0,   332,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   564,    77,     0,   567,     0,     0,     0,
      78,     0,     0,     0,   341,   342,   343,   344,   345,     0,
       0,     0,     0,     0,     0,    79,     0,     0,   346,   347,
       0,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
       0,   584,   585,     0,   587,     0,   589,   389,     0,     0,
     415,     3,   594,   390,   391,     0,   392,     0,   215,   216,
     217,   218,     0,     0,     0,     0,   393,   219,   394,   395,
       0,   228,   229,   230,   609,     0,     0,   610,   396,   220,
      34,     0,   397,   221,     0,   222,     0,     9,   223,   398,
       0,   224,    80,    81,    82,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    99,   100,   101,    35,   225,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,   322,   323,     0,
       0,   226,   227,     0,     0,   328,     0,   329,   330,   331,
       0,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    34,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,   239,     0,     0,
       0,   240,     0,     0,   241,   242,   346,   347,     0,     0,
       0,     0,     0,   244,     0,    35,   399,   245,   400,   246,
     389,     0,     0,     0,     3,     0,   390,   391,     0,   392,
       0,   215,   216,   217,   218,     0,     0,     0,     0,   393,
     219,   394,   395,     0,     0,     0,     0,     0,     0,     0,
       0,   396,   220,     0,     0,   397,   221,     0,   222,     0,
       9,   223,   398,     0,   224,    80,    81,    82,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,    99,   100,   101,     0,   225,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   226,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    34,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
     239,     0,     0,     0,   240,     0,     0,   241,   242,     0,
       0,     0,     0,     0,     0,     0,   244,     0,    35,   399,
     245,   591,   246,   215,   216,   217,   218,     0,     0,     0,
       0,     0,   219,     0,     0,     0,     0,     0,   174,     0,
       0,     0,     0,   175,   220,     0,     0,     0,   221,     0,
     222,     0,     0,   223,     0,     0,   224,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     225,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   227,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   372,   237,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,   177,     0,
       0,   238,   239,     0,     0,     0,   240,     0,     0,   241,
     242,     0,   215,   216,   217,   218,   178,     0,   373,     0,
      35,   219,   245,     0,   246,     0,     0,   174,     0,     0,
       0,     0,   175,   220,     0,     0,     0,   221,     0,   222,
       0,     0,   223,     0,     0,   224,    80,    81,    82,     0,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,    99,   100,   101,     0,   225,
     104,   105,   106,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   226,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    34,   237,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,   177,     0,     0,
     238,   239,     0,     0,     0,   240,     0,     0,   241,   242,
       0,   215,   216,   217,   218,   178,     0,   244,     0,    35,
     219,   245,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,   220,     0,     0,     0,   221,     0,   222,     0,
       0,   223,     0,     0,   224,    80,    81,    82,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,    99,   100,   101,     0,   225,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   226,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    34,   237,     0,     0,     0,     0,
       0,     0,     0,     0,   215,   216,   217,   218,   469,   238,
     239,     0,     0,   219,   240,     0,     0,   241,   242,     0,
       0,     0,     0,     0,   417,   220,   244,   418,    35,   221,
     245,   222,   246,     0,   223,     0,     0,   224,    80,    81,
      82,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,    99,   100,   101,
       0,   225,   104,   105,   106,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   226,   227,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    34,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     470,     0,   238,   239,     0,     0,     0,   240,     0,     0,
     241,   242,     0,   215,   216,   217,   218,     0,     0,   244,
       0,    35,   219,   245,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,   221,     0,
     222,     0,     0,   223,     0,     0,   224,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     225,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   227,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    34,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,   239,     0,     0,     0,   240,     0,     0,   241,
     242,     0,   215,   216,   217,   218,     0,   243,   244,     0,
      35,   219,   245,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,     0,   221,     0,   222,
       0,     0,   223,     0,     0,   224,    80,    81,    82,     0,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,    99,   100,   101,     0,   225,
     104,   105,   106,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   226,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    34,   237,     0,     0,     0,
       0,     0,     0,     0,     0,   215,   216,   217,   218,     0,
     238,   239,     0,     0,   219,   240,     0,     0,   241,   242,
       0,     0,     0,     0,     0,     0,   220,   244,   369,    35,
     221,   245,   222,   246,     0,   223,     0,     0,   224,    80,
      81,    82,     0,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,    99,   100,
     101,     0,   225,   104,   105,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   226,   227,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    34,   237,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,     0,   238,   239,     0,     0,   219,   240,     0,
       0,   241,   242,     0,     0,     0,     0,     0,     0,   220,
     244,   447,    35,   221,   245,   222,   246,     0,   223,     0,
       0,   224,    80,    81,    82,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,    99,   100,   101,     0,   225,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    34,   237,     0,     0,     0,     0,     0,     0,     0,
       0,   215,   216,   217,   218,     0,   238,   239,     0,     0,
     219,   240,     0,     0,   241,   242,     0,     0,     0,     0,
       0,     0,   220,   244,   467,    35,   221,   245,   222,   246,
       0,   223,     0,     0,   224,    80,    81,    82,     0,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,    99,   100,   101,     0,   225,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   226,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    34,   237,     0,     0,     0,     0,
       0,     0,     0,     0,   215,   216,   217,   218,     0,   238,
     239,     0,     0,   219,   240,     0,     0,   241,   242,     0,
       0,     0,     0,     0,     0,   220,   244,   480,    35,   221,
     245,   222,   246,     0,   223,     0,     0,   224,    80,    81,
      82,     0,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,    99,   100,   101,
       0,   225,   104,   105,   106,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   226,   227,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    34,   237,     0,
       0,     0,     0,     0,     0,     0,     0,   215,   216,   217,
     218,     0,   238,   239,     0,     0,   219,   240,     0,     0,
     241,   242,     0,     0,     0,     0,     0,     0,   220,   244,
       0,    35,   221,   245,   222,   246,     0,   223,     0,     0,
     224,    80,    81,    82,     0,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,     0,   225,   104,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     226,   227,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      34,   237,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   216,   217,   218,   583,   238,   239,     0,     0,   219,
     240,     0,     0,   241,   242,     0,     0,     0,     0,     0,
       0,   220,   244,   565,    35,   221,   245,   222,   246,     0,
     223,     0,     0,   224,    80,    81,    82,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,    99,   100,   101,     0,   225,   104,   105,
     106,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   226,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    34,   237,     0,     0,     0,     0,     0,
       0,     0,     0,   215,   216,   217,   218,     0,   238,   239,
       0,     0,   219,   240,     0,     0,   241,   242,     0,     0,
       0,     0,     0,     0,   220,   244,     0,    35,   221,   245,
     222,   246,     0,   223,     0,     0,   224,    80,    81,    82,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,    99,   100,   101,     0,
     225,   104,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   227,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    34,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,   238,   239,     0,    78,     0,   240,     0,     0,   241,
     242,     0,     0,     0,     0,     0,     0,     0,   244,    79,
      35,     0,   245,     0,   246,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,     0,     0,     0,    34,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,     0,     0,   156,     0,    35,     0,
     451,   452,   453,   454,   455,   456,   457,   458,   320,   321,
     322,   323,   324,   459,   460,   325,   326,   327,   328,   461,
     329,   330,   331,     0,   332,   462,   333,   334,   463,   464,
       0,     0,     0,     0,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   335,     0,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
       0,     0,     0,     0,     0,     0,     0,     0,    35,   346,
     347,     0,     0,     0,     0,   603,   451,   452,   453,   454,
     455,   456,   457,   458,   320,   321,   322,   323,   324,   459,
     460,   325,   326,   327,   328,   461,   329,   330,   331,     0,
     332,   462,   333,   334,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   335,     0,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,   347,     0,     0,     0,
       0,   612,   451,   452,   453,   454,   455,   456,   457,   458,
     320,   321,   322,   323,   324,   459,   460,   325,   326,   327,
     328,   461,   329,   330,   331,  -151,   332,   462,   333,   334,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     335,     0,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,   347,   451,   452,   453,   454,   455,   456,   457,
     458,   320,   321,   322,   323,   324,   459,   460,   325,   326,
     327,   328,   461,   329,   330,   331,     0,   332,   462,   333,
     334,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   335,     0,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   346,   347,   325,   326,   327,   328,     0,   329,
     330,   331,     0,   332,     0,   333,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,     0,
       0,     0,     0,   320,   321,   322,   323,   324,   346,   347,
     325,   326,   327,   328,   597,   329,   330,   331,     0,   332,
       0,   333,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,     0,   388,   320,
     321,   322,   323,   324,     0,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   332,     0,   333,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   335,
       0,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   347,     0,     0,   472,   320,   321,   322,   323,   324,
       0,     0,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   332,     0,   333,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   335,     0,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,     0,     0,
     475,   320,   321,   322,   323,   324,     0,     0,   325,   326,
     327,   328,     0,   329,   330,   331,     0,   332,     0,   333,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   347,     0,     0,   477,   320,   321,   322,
     323,   324,     0,     0,   325,   326,   327,   328,     0,   329,
     330,   331,     0,   332,     0,   333,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,   347,
       0,     0,   537,   320,   321,   322,   323,   324,     0,     0,
     325,   326,   327,   328,     0,   329,   330,   331,     0,   332,
       0,   333,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,     0,   596,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,     0,     0,     0,
       0,     0,     0,     0,     0,   138,   139,   140,   141,   320,
     321,   322,   323,   324,     0,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   332,     0,   333,   334,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,     0,
       0,     0,   152,   153,     0,     0,     0,     0,     0,   335,
       0,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   347,   348,   320,   321,   322,   323,   324,     0,     0,
     325,   326,   327,   328,     0,   329,   330,   331,     0,   332,
       0,   333,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,     0,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   515,   320,   321,   322,
     323,   324,     0,     0,   325,   326,   327,   328,     0,   329,
     330,   331,     0,   332,     0,   333,   334,     0,     0,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     0,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,     0,
       0,     0,     0,   320,   321,   322,   323,   324,   346,   347,
     325,   326,   327,   328,     0,   329,   330,   331,     0,   332,
       0,   333,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,   514,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   346,   347,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   332,     0,   333,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   551,     0,     0,   335,
       0,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,     0,     0,     0,     0,   320,   321,   322,   323,   324,
     346,   347,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   332,     0,   333,   334,   320,   321,   322,   323,   324,
       0,     0,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   332,     0,   333,   334,   335,     0,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,   337,   338,
     339,   340,   341,   342,   343,   344,   345,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   346,   347,   325,   326,
     327,   328,     0,   329,   330,   331,     0,   332,     0,   333,
     334,   320,   321,   322,   323,   324,     0,     0,   325,   326,
     327,   328,     0,   329,   330,   331,     0,   332,     0,   333,
     334,     0,     0,     0,     0,   338,   339,   340,   341,   342,
     343,   344,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   347,     0,     0,   339,   340,   341,   342,
     343,   344,   345,     0,   320,   321,   322,   323,   324,     0,
       0,   325,   346,   347,   328,     0,   329,   330,   331,     0,
     332,     0,   333,   334,   320,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   328,     0,   329,   330,   331,     0,
     332,     0,   333,   334,     0,     0,     0,     0,     0,   339,
     340,   341,   342,   343,   344,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,   347,     0,   322,   323,
       0,   341,   342,   343,   344,   345,   328,     0,   329,   330,
     331,     0,   332,     0,     0,   346,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,   347
};

static const yytype_int16 yycheck[] =
{
      10,   272,    50,   316,   188,   190,   349,    53,   203,   303,
     205,    50,   207,   230,    98,    28,   115,    17,   122,   362,
     170,   171,   172,   115,   121,   145,     0,    41,   115,   115,
     121,     5,   152,    10,    27,    41,    17,   417,   182,    32,
      50,   129,    19,     4,    54,     6,   150,    27,    25,   193,
      24,   148,    26,   152,    28,   146,    30,   149,    29,    27,
     147,    35,   150,    40,    32,    93,   150,    41,    96,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,   392,    27,
     109,   115,   115,   483,    32,   122,    27,   117,    27,   115,
     121,    32,   158,    32,    85,    86,   144,   145,   166,   167,
     168,   121,   170,   171,   172,   173,    94,   166,   167,   168,
     123,   170,   171,   172,   127,   478,   149,   115,   115,   150,
     121,   189,   148,   115,   115,    94,    27,   127,   121,   146,
     189,    32,   145,   150,   122,   123,   166,   167,   168,   127,
     170,   171,   172,   173,    94,   145,   383,   317,   352,   217,
     513,   145,   149,   122,   121,   148,   121,   145,   217,   189,
     228,   229,   150,   117,   118,   119,    94,     9,    27,   228,
     229,   121,   122,    32,   124,   123,    18,    94,    27,   127,
     149,   148,   123,    32,   123,   150,   127,   217,   127,   121,
     395,   554,   506,   121,   122,    27,   124,   145,   228,   229,
      32,   416,   150,   271,   145,   122,   145,   148,   121,   148,
     240,   279,   271,   146,    27,   115,   148,   150,   145,    32,
     279,   289,   123,   121,   121,   152,   127,   121,   296,   297,
     151,   299,   146,   146,    27,    94,   150,   296,   297,    32,
     299,   271,    27,    27,   145,   578,   147,    32,    32,   279,
     148,   124,   146,    27,   150,   148,   152,   150,    32,   289,
     178,   117,   118,   122,   123,   144,   296,   297,   127,   299,
     150,   121,   152,   151,   123,   153,    27,   121,   127,    27,
     129,    32,   151,   122,    32,   149,   145,   492,   493,   580,
     115,   123,    12,    13,   499,   127,   145,   129,   115,   147,
     109,   115,   124,   508,   115,   151,    29,    29,   226,   227,
     123,   602,   230,   145,   127,   117,   129,   147,   128,   147,
     238,   239,   128,   241,   242,   128,   244,   128,   128,   128,
     123,   128,   145,   124,   127,    38,   129,   115,   123,   123,
     544,   148,   127,   127,   129,   129,   150,   115,   109,   123,
     127,   556,   145,   127,   272,   129,   151,   561,   146,   128,
     145,   145,   128,   128,   147,   147,   147,   147,   573,   145,
     115,   145,   123,   129,   147,   123,   127,   295,   129,   127,
     298,   129,   300,   150,   589,   590,   304,   305,   592,   109,
     129,   129,    98,   113,   145,   115,   116,   145,   316,   115,
     115,   469,   320,   321,   115,   115,   324,   325,   326,   327,
     469,   329,   121,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   121,   347,
     117,   152,    31,   150,    17,   115,   150,   150,   150,   469,
      83,    84,    85,    86,    87,   363,   147,    90,    91,    92,
      93,   115,    95,    96,    97,   373,    99,   115,   101,   102,
     146,   146,   146,    11,   382,   383,    34,   129,   386,   151,
     148,   146,   390,   391,    44,   393,   146,   146,   396,   397,
     123,    47,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    85,    86,   309,    46,   599,    44,    54,   246,
      93,   144,   145,    96,    97,   558,    99,     1,   151,   119,
     450,   579,   267,   217,   289,   583,   480,    -1,   486,   305,
     579,    -1,    -1,    -1,   583,    -1,    -1,    -1,    -1,    -1,
      -1,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,    -1,   579,
      -1,   144,   145,   583,    -1,   473,   474,    -1,   476,    85,
      86,   479,    -1,    -1,    -1,    -1,   484,    93,   486,    95,
      96,    97,    -1,    99,    -1,    -1,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   511,    19,    -1,   514,    -1,    -1,    -1,
      25,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,   144,   145,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,   569,   570,    -1,   572,    -1,   574,     1,    -1,    -1,
     578,     5,   580,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      -1,   106,   107,   108,   602,    -1,    -1,   605,    32,    33,
     115,    -1,    36,    37,    -1,    39,    -1,    41,    42,    43,
      -1,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,   149,    69,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    85,    86,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,   135,    -1,    -1,   138,   139,   144,   145,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,   150,   151,   152,   153,
       1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    36,    37,    -1,    39,    -1,
      41,    42,    43,    -1,    45,    46,    47,    48,    -1,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,    -1,    -1,   135,    -1,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,   150,
     151,   152,   153,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,   127,    -1,
      -1,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,   138,
     139,    -1,    12,    13,    14,    15,   145,    -1,   147,    -1,
     149,    21,   151,    -1,   153,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    46,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,   127,    -1,    -1,
     130,   131,    -1,    -1,    -1,   135,    -1,    -1,   138,   139,
      -1,    12,    13,    14,    15,   145,    -1,   147,    -1,   149,
      21,   151,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    46,    47,    48,    -1,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    16,   130,
     131,    -1,    -1,    21,   135,    -1,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,   145,    33,   147,   148,   149,    37,
     151,    39,   153,    -1,    42,    -1,    -1,    45,    46,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,
     138,   139,    -1,    12,    13,    14,    15,    -1,    -1,   147,
      -1,   149,    21,   151,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,   138,
     139,    -1,    12,    13,    14,    15,    -1,   146,   147,    -1,
     149,    21,   151,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    46,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,    -1,
     130,   131,    -1,    -1,    21,   135,    -1,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    33,   147,   148,   149,
      37,   151,    39,   153,    -1,    42,    -1,    -1,    45,    46,
      47,    48,    -1,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    -1,   130,   131,    -1,    -1,    21,   135,    -1,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     147,   148,   149,    37,   151,    39,   153,    -1,    42,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,    -1,   130,   131,    -1,    -1,    21,   135,    -1,    -1,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    33,   147,
      -1,   149,    37,   151,    39,   153,    -1,    42,    -1,    -1,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    16,   130,   131,    -1,    -1,    21,
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
      -1,    -1,    -1,    12,    13,    14,    15,    -1,   130,   131,
      -1,    -1,    21,   135,    -1,    -1,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    33,   147,    -1,   149,    37,   151,
      39,   153,    -1,    42,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,   130,   131,    -1,    25,    -1,   135,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    40,
     149,    -1,   151,    -1,   153,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,   115,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,   147,    -1,   149,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   144,
     145,    -1,    -1,    -1,    -1,   150,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,    -1,
      -1,   150,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    83,    84,    85,
      86,    87,   144,   145,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,   144,   145,
      90,    91,    92,    93,   150,    95,    96,    97,    -1,    99,
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
     148,    83,    84,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
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
      -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   148,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    83,
      84,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,    83,    84,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,    83,    84,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    83,    84,    85,    86,    87,   144,   145,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    83,
      84,    85,    86,    87,   144,   145,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    83,    84,    85,    86,    87,
     144,   145,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    83,    84,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,   144,   145,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    83,    84,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,    83,    84,    85,    86,    87,    -1,
      -1,    90,   144,   145,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    83,    84,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    85,    86,
      -1,   130,   131,   132,   133,   134,    93,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   155,     0,     5,    24,    26,    28,    30,    35,    41,
     145,   160,   161,   165,   178,   182,   195,   217,   219,   220,
     223,   115,   173,   174,   109,   166,   167,   115,   162,   163,
     164,   115,   115,    28,   115,   149,   175,   176,   177,   187,
       4,     6,   151,   122,   121,   124,   121,   121,   144,   151,
     122,   149,   115,   147,   121,   146,   115,   222,    29,    85,
      86,   115,   181,   209,    12,    13,   109,   113,   115,   116,
     157,   173,   109,   167,   164,   115,   218,    19,    25,    40,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,   187,
     225,   226,   227,   228,   115,   174,   176,   124,   221,   151,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,   101,   102,
     103,   104,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   138,   139,    29,    29,   147,   179,   145,   152,
     206,   208,   117,   156,   115,   152,   128,   128,   128,   147,
     128,   128,   128,   128,    27,    32,   123,   127,   145,   148,
     187,   209,    41,   115,   148,   210,   211,   215,   224,   124,
     180,   174,    38,   207,   150,   117,   118,   122,   150,   228,
     228,   228,   115,   179,   228,   179,   228,   179,   228,   212,
     213,   228,    27,   127,   145,    12,    13,    14,    15,    21,
      33,    37,    39,    42,    45,    69,    85,    86,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   116,   130,   131,
     135,   138,   139,   146,   147,   151,   153,   159,   183,   187,
     189,   197,   198,   200,   201,   203,   204,   205,   225,   229,
     232,   237,   239,   241,   152,   215,   148,   150,    94,   121,
     122,   124,   214,   228,   183,   146,   215,   109,   129,   150,
     129,   148,   180,   129,   180,   129,   180,   129,   129,   150,
     127,   146,   228,   229,   147,   147,   128,   128,   147,   128,
     147,   205,   205,    10,   228,   228,   205,   234,   238,   158,
     205,   205,   187,   205,   205,   205,   185,   178,   115,   147,
      83,    84,    85,    86,    87,    90,    91,    92,    93,    95,
      96,    97,    99,   101,   102,   123,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   144,   145,   146,   147,
     210,   115,   228,   186,   202,   205,   150,   228,   129,   129,
     129,   212,   147,   115,   205,   228,   228,   205,   228,   148,
     205,   224,   115,   147,   205,   230,   231,   233,   234,   235,
     236,   236,   105,   150,   152,   118,   119,   156,   148,     1,
       7,     8,    10,    20,    22,    23,    32,    36,    43,   150,
     152,   169,   170,   171,   172,   184,   186,   188,   190,   191,
     192,   193,   195,   196,   202,   205,   179,   145,   148,   199,
     205,   205,   205,   205,   205,   205,   205,   115,   205,   115,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   115,   205,   148,   199,   214,
      98,    75,    76,    77,    78,    79,    80,    81,    82,    88,
      89,    94,   100,   103,   104,   122,   129,   148,   199,    16,
     128,   205,   148,   129,   129,   148,   129,   148,    17,   214,
     148,   121,   146,   150,   121,   146,   150,   152,   205,   234,
     152,   205,   205,   205,   224,    94,   205,   183,   205,   205,
      31,   150,   150,   150,   150,    17,   194,   150,   180,   115,
     230,   121,   148,   147,   124,   146,   148,   205,   200,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   148,   228,   115,   148,   205,   205,
     205,   199,   205,   231,   115,   146,   230,   205,   146,   235,
     146,   120,   183,   183,    17,   205,    11,   183,   151,    34,
     216,   224,   183,   146,   205,   148,   199,   205,   148,   129,
     150,   146,   214,     9,    18,   168,   199,   183,   185,   124,
     214,   148,   148,    16,   205,   205,   146,   205,   183,   205,
     150,   152,   228,   186,   205,   228,   148,   150,   240,   183,
     183,   150,   214,   150,   148,    44,   146,   168,   186,   205,
     205,   146,   150
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
     189,   189,   189,   190,   191,   192,   192,   192,   193,   194,
     194,   195,   195,   196,   197,   197,   197,   198,   198,   198,
     198,   199,   199,   200,   200,   201,   201,   201,   201,   201,
     201,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   203,   204,   204,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   206,   206,   207,
     207,   208,   209,   209,   210,   210,   211,   211,   212,   213,
     213,   214,   214,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   216,   217,   218,   218,   218,   219,   220,   221,
     221,   222,   223,   224,   224,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   226,
     227,   227,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   229,
     229,   230,   230,   231,   231,   232,   232,   233,   233,   234,
     235,   235,   236,   236,   237,   237,   238,   238,   239,   240,
     240,   241
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
       2,     2,     1,     1,     1,     1,     2,     2,     2,     1,
       1,     0,     2,     2,     3,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     1,     2,     3,     4,     0,
       2,     1,     1,     3,     5,     5,     5,     5,     8,     6,
       9,     1,     3,     1,     5,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     5,     6,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     3,     4,     3,     3,     2,     3,     4,
       3,     4,     4,     4,     3,     4,     3,     5,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     0,     3,     0,
       1,     3,     0,     3,     1,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     3,     5,     3,     3,     4,     6,
       5,     4,     3,     4,     0,     3,     5,     4,     5,     0,
       2,     2,     6,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     4,     3,     4,     2,     3,
       2,     3,     4,     2,     4,     6,     4,     1,     4,     5,
       1,     4,     5,     1,     4,     5,     4,     1,     1,     1,
       1,     3,     5,     1,     3,     5,     7,     1,     3,     3,
       1,     1,     1,     3,     5,     5,     1,     3,     4,     0,
       3,    10
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
          case 115: /* NAME  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2429 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* character_sequence  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2435 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* string_constant  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2441 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* string_builder_body  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2447 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* string_builder  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2453 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* require_module_name  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2459 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expression_else  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2465 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_if_then_else  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2471 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_for_loop  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2477 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_while_loop  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2483 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_with  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2489 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* annotation_argument  */
#line 98 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2495 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* annotation_argument_list  */
#line 99 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2501 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* annotation_declaration_name  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2507 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* annotation_declaration  */
#line 100 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2513 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* annotation_list  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2519 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* optional_annotation_list  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2525 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* optional_function_argument_list  */
#line 95 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2531 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* optional_function_type  */
#line 96 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2537 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* function_name  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2543 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_block  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2549 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_any  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2555 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expressions  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2561 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expr_pipe  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2567 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* name_in_namespace  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2573 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_delete  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2579 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expr_new  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2585 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expression_break  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2591 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_continue  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2597 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_return  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2603 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_try_catch  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2609 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expression_let  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2615 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expr_cast  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2621 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expr_type_info  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2627 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expr_list  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2633 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_block  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2639 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expr_numeric_const  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2645 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_assign  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2651 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_named_call  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2657 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_method_call  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2663 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2669 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* optional_field_annotation  */
#line 99 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2675 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* structure_variable_declaration  */
#line 94 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2681 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* struct_variable_declaration_list  */
#line 95 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2687 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* function_argument_declaration  */
#line 94 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2693 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* function_argument_list  */
#line 95 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2699 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* tuple_type  */
#line 94 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2705 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* tuple_type_list  */
#line 95 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2711 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* variable_declaration  */
#line 94 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2717 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* let_variable_declaration  */
#line 94 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2723 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* enum_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2729 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* optional_structure_parent  */
#line 92 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2735 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* variable_name_list  */
#line 93 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2741 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* structure_type_declaration  */
#line 96 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2747 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* auto_type_declaration  */
#line 96 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2753 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* type_declaration  */
#line 96 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2759 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* make_decl  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2765 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* make_struct_fields  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2771 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* make_struct_dim  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2777 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* make_struct_decl  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2783 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* make_tuple  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2789 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* make_map_tuple  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2795 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* make_any_tuple  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2801 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_dim  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2807 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_dim_decl  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2813 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_table  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2819 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* make_table_decl  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2825 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* array_comprehension_where  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2831 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* array_comprehension  */
#line 97 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2837 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 278 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 3145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 281 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 290 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3169 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 298 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 314 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 344 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 347 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3229 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 356 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto modN = getModuleName(*(yyvsp[0].s));
        if ( !g_Program->addModule(modN) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 376 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 379 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 385 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 386 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 387 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 394 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 401 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3302 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 413 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3314 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 423 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3325 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 432 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3331 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 433 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3337 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 434 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3343 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 435 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3349 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 436 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3355 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 437 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3361 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 441 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3371 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 446 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 454 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 455 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 459 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 466 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 478 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 489 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 490 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 494 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 495 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 496 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 500 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 504 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 511 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 515 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 516 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 517 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 518 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3518 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 519 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 520 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3530 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 521 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 522 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3554 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 525 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3560 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 526 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3566 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3572 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3590 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3596 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 534 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 535 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 536 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 537 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 540 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 542 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 543 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 544 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 545 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 546 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 547 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 3692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 548 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 3698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 3704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 3710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
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
                                pVar->move_to_init = pDecl->move_to_init;
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
#line 3780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 623 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 626 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 636 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 647 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 648 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
    }
#line 3886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
			if ( !(yyvsp[-1].pExpression)->at.line ) {
				(yyvsp[-1].pExpression)->at = (yyvsp[0].pExpression)->at;
			}
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 670 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 684 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 691 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 695 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3955 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 701 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 711 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 4008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 728 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4016 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4058 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
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
                        pVar->move_to_init = (yyvsp[0].pVarDecl)->move_to_init;
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
#line 4092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4100 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 795 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 803 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 807 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4139 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 812 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4148 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 816 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4158 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4166 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 827 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 833 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4184 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 839 "ds_parser.ypp" /* yacc.c:1660  */
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
                                pVar->move_to_init = pDecl->move_to_init;
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
#line 4236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 892 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4272 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4284 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4290 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 902 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4302 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 904 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4314 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4320 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 906 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4326 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4332 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 908 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4338 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4344 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 910 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4350 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4356 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 912 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4362 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 913 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4368 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4380 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 928 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 933 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 943 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 944 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 945 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4420 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 946 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 947 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 948 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 949 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 951 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 952 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 953 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 954 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 955 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4480 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4486 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 957 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4492 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 958 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 959 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4504 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 960 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4510 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 961 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4516 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 962 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4528 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 964 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4534 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 965 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4540 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 967 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4552 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 968 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4570 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 971 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 973 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 974 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4594 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 975 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4600 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4606 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 977 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4612 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 978 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4618 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 979 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4624 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 980 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4630 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 981 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4639 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4648 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 989 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 992 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 997 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4694 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1000 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1005 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1006 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4732 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1029 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4804 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1063 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1064 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4816 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1068 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1069 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4867 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1096 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1108 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4911 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1117 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1128 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1141 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1144 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1148 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 5003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1165 "ds_parser.ypp" /* yacc.c:1660  */
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
                        pVar->move_to_init = pDecl->move_to_init;
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
#line 5033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1248 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5152 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1287 "ds_parser.ypp" /* yacc.c:1660  */
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
                                pDecl->move_to_init, pDecl->at);
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
#line 5220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1353 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1360 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1369 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 5249 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 5255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1371 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 5267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 5273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1377 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5297 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1378 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5303 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 5309 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1380 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 5315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1382 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5333 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1384 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5339 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1385 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1386 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5351 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1387 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1388 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1389 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1390 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1391 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 5399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1406 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1410 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5420 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1421 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1422 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1423 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1437 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1443 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1447 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1452 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1462 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1467 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5529 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1474 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1478 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1483 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1489 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1494 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1498 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1516 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1521 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5654 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1539 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1548 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1557 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1558 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1559 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1560 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1564 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1571 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1580 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1585 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1592 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1606 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1609 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5790 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1623 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1632 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1633 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1637 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1642 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5832 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1649 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1654 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 5855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1665 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1670 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5874 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1677 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 5888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1689 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 5894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1690 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1694 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5918 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 5922 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 1709 "ds_parser.ypp" /* yacc.c:1903  */


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


