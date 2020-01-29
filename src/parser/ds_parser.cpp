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
    DAS_SHARED = 308,
    DAS_TBOOL = 309,
    DAS_TVOID = 310,
    DAS_TSTRING = 311,
    DAS_TAUTO = 312,
    DAS_TINT = 313,
    DAS_TINT2 = 314,
    DAS_TINT3 = 315,
    DAS_TINT4 = 316,
    DAS_TUINT = 317,
    DAS_TUINT2 = 318,
    DAS_TUINT3 = 319,
    DAS_TUINT4 = 320,
    DAS_TFLOAT = 321,
    DAS_TFLOAT2 = 322,
    DAS_TFLOAT3 = 323,
    DAS_TFLOAT4 = 324,
    DAS_TRANGE = 325,
    DAS_TURANGE = 326,
    DAS_TBLOCK = 327,
    DAS_TINT64 = 328,
    DAS_TUINT64 = 329,
    DAS_TDOUBLE = 330,
    DAS_TFUNCTION = 331,
    DAS_TLAMBDA = 332,
    DAS_TINT8 = 333,
    DAS_TUINT8 = 334,
    DAS_TINT16 = 335,
    DAS_TUINT16 = 336,
    DAS_TTUPLE = 337,
    DAS_GENERATOR = 338,
    DAS_YIELD = 339,
    ADDEQU = 340,
    SUBEQU = 341,
    DIVEQU = 342,
    MULEQU = 343,
    MODEQU = 344,
    ANDEQU = 345,
    OREQU = 346,
    XOREQU = 347,
    SHL = 348,
    SHR = 349,
    ADDADD = 350,
    SUBSUB = 351,
    LEEQU = 352,
    SHLEQU = 353,
    SHREQU = 354,
    GREQU = 355,
    EQUEQU = 356,
    NOTEQU = 357,
    RARROW = 358,
    LARROW = 359,
    QQ = 360,
    QDOT = 361,
    LPIPE = 362,
    LBPIPE = 363,
    RPIPE = 364,
    CLONEEQU = 365,
    ROTL = 366,
    ROTR = 367,
    ROTLEQU = 368,
    ROTREQU = 369,
    MAPTO = 370,
    BRABRAB = 371,
    BRACBRB = 372,
    CBRCBRB = 373,
    INTEGER = 374,
    LONG_INTEGER = 375,
    UNSIGNED_INTEGER = 376,
    UNSIGNED_LONG_INTEGER = 377,
    FLOAT = 378,
    DOUBLE = 379,
    NAME = 380,
    BEGIN_STRING = 381,
    STRING_CHARACTER = 382,
    END_STRING = 383,
    BEGIN_STRING_EXPR = 384,
    END_STRING_EXPR = 385,
    UNARY_MINUS = 386,
    UNARY_PLUS = 387,
    PRE_INC = 388,
    PRE_DEC = 389,
    POST_INC = 390,
    POST_DEC = 391,
    DEREF = 392,
    COLCOL = 393
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

#line 365 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   5573

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  166
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  98
/* YYNRULES -- Number of rules.  */
#define YYNRULES  389
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  696

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   393

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   149,     2,   165,   164,   144,   137,     2,
     157,   158,   142,   141,   131,   140,   154,   143,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   134,   161,
     138,   132,   139,   133,   145,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   155,     2,   156,   136,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   162,   135,   163,   148,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   146,   147,   150,   151,
     152,   153,   159,   160
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   365,   365,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   380,   392,   393,   397,   401,   405,   414,
     422,   438,   459,   463,   466,   472,   481,   494,   495,   499,
     503,   504,   508,   511,   517,   523,   526,   532,   533,   534,
     541,   548,   560,   570,   579,   580,   581,   582,   583,   584,
     588,   593,   601,   602,   603,   604,   605,   606,   607,   608,
     614,   627,   643,   644,   648,   655,   667,   671,   678,   679,
     683,   684,   685,   689,   693,   700,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   743,   813,   816,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   835,   836,   837,   838,   839,   840,
     841,   842,   846,   850,   856,   862,   876,   877,   883,   887,
     893,   896,   899,   903,   909,   913,   917,   920,   923,   928,
     931,   939,   942,   947,   950,   958,   964,   965,   969,   970,
     974,  1007,  1010,  1015,  1023,  1027,  1032,  1038,  1042,  1047,
    1056,  1059,  1065,  1066,  1076,  1081,  1133,  1134,  1135,  1136,
    1137,  1138,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,
    1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,  1161,  1172,
    1177,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1229,  1233,  1236,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1250,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1269,  1279,  1313,  1314,  1318,  1319,  1323,  1331,  1334,  1341,
    1342,  1346,  1347,  1351,  1357,  1358,  1362,  1363,  1367,  1372,
    1375,  1379,  1385,  1394,  1395,  1396,  1400,  1403,  1408,  1413,
    1420,  1430,  1433,  1440,  1441,  1445,  1475,  1478,  1487,  1499,
    1511,  1525,  1526,  1530,  1569,  1635,  1642,  1651,  1652,  1653,
    1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1665,  1666,  1667,  1668,  1669,  1670,  1671,  1672,  1673,
    1674,  1678,  1689,  1693,  1703,  1704,  1705,  1706,  1720,  1726,
    1730,  1735,  1740,  1745,  1752,  1757,  1761,  1765,  1770,  1775,
    1782,  1787,  1793,  1798,  1802,  1807,  1816,  1820,  1825,  1834,
    1838,  1843,  1852,  1861,  1862,  1863,  1864,  1868,  1875,  1884,
    1889,  1896,  1901,  1907,  1914,  1925,  1928,  1942,  1951,  1952,
    1956,  1961,  1968,  1973,  1984,  1989,  1996,  2008,  2009,  2013
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
  "\"implicit\"", "\"shared\"", "\"bool\"", "\"void\"", "\"string\"",
  "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"",
  "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"", "\"block\"",
  "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"",
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"",
  "\"generator\"", "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
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
  "expression_while_loop", "expression_with", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
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
  "optional_shared", "global_let", "enum_list", "alias_declaration",
  "enum_declaration", "optional_structure_parent", "structure_name",
  "structure_declaration", "variable_name_list", "basic_type_declaration",
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
     385,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,    64,   386,   387,   126,    33,
     388,   389,   390,   391,    46,    91,    93,    40,    41,   392,
     393,    59,   123,   125,    36,    35
};
# endif

#define YYPACT_NINF -435

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-435)))

#define YYTABLE_NINF -183

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -435,    22,  -435,  -435,   -86,   -65,   -23,    31,    34,  -435,
      38,   -19,  -435,  -435,  -435,  -435,  -435,   236,  -435,    69,
    -435,  -435,  -435,  -435,    93,  -435,    95,   107,    98,  -435,
    -435,    -3,  -435,   108,   106,   119,  -435,   131,   163,   135,
    -435,    76,  -435,   173,   160,  -435,   138,    29,   -86,   182,
     -65,  -435,   178,   179,  -435,  -435,  4015,  -435,   184,  -435,
     -86,   -19,  -435,   180,   145,  5424,   286,   287,  -435,   161,
    -435,  -435,  -435,  -435,  -435,  -435,   192,   298,  -435,  -435,
    -435,  -435,  -435,  -435,  -102,   183,   185,   187,  -435,  -435,
    -435,   165,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,   188,  -435,  -435,  -435,
     193,   194,  -435,  -435,  -435,  -435,   195,  -435,  -435,  -435,
    -435,  5343,  -435,    51,  -435,   -57,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,   -13,   201,   -58,  -435,    19,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,    53,   -12,  -435,  4015,
    4015,  4015,   211,  3943,  3943,  3943,  4015,  -435,    24,  -435,
    -435,  -435,  -435,  1686,  -435,  -435,  -435,    80,   213,  -435,
    -435,  -435,  -118,  -435,   -69,  4015,   199,  -435,  -435,   162,
    -435,  -435,   298,  -435,  3101,  -435,   169,  1138,  4702,   186,
     201,  4703,   201,  5219,   201,  5268,  -435,  -105,  5343,   205,
    -435,   209,  -435,  -435,  -435,  3871,   198,  -435,   210,   232,
     233,   215,   241,   224,   225,  3101,  3101,    18,  4015,  3101,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  3101,  3101,  3101,
     -57,  3101,  3101,  -435,  3101,  -435,   228,  -435,  -435,   -88,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,  4545,   229,  -435,
    -435,  -435,  -435,  -435,   -86,  -435,   310,   226,  -435,  -435,
      -9,  -435,   266,  -435,  4015,  3101,  5343,  -435,  -435,  -435,
    -435,  4015,  3101,  -435,  3694,  -435,  4015,  -435,  -435,   263,
    -435,   267,  -435,   268,  -435,  -435,  4015,  -435,  -435,  4005,
    -435,   283,  3101,  4015,  4015,  3101,  4015,  1825,  1941,   109,
     109,   213,  1060,  1199,  4842,  -435,  -106,   -18,   109,   109,
    -435,  -435,   109,   109,  4011,   743,   161,  -435,  1315,   393,
    3101,  3101,  -435,  -435,  3101,  3101,  3101,  3101,   285,  3101,
     290,  3101,  3101,  3101,  3101,  3101,  3101,  3101,  3101,  3101,
    3101,  3101,  3101,  3101,  3101,  3101,   291,  3101,  -435,  2057,
      88,  -435,   213,  -435,  -435,  -435,   352,  -435,   312,  4689,
     580,  -435,  3181,  -435,  5285,  -435,  -435,  -435,  -435,  2173,
    1431,  4083,  5305,  5308,  4149,  5316,  -435,  4215,  -435,  4281,
       4,    67,  2289,  4842,   294,   125,   295,  -435,  -435,   126,
    2405,    87,    97,  3101,  3101,   264,  -435,  3101,   302,  -435,
    -435,  3101,  3101,   213,  1570,  -435,   199,  3101,  3101,  -435,
     269,   313,  2521,  2637,  -435,   399,   272,   273,  -435,  -435,
    -435,  -435,  -435,  -435,   274,   277,   278,  -435,  -435,  -435,
     424,  -435,   -60,  4786,   201,   317,  -435,    64,  5010,  4015,
     385,   385,   234,   234,  5188,  5188,   288,    12,  -435,    59,
      59,   385,   385,  4898,  5081,  5102,  5167,   234,   234,   493,
     493,    12,    12,    12,  -435,  4609,  -435,    65,  -435,  -435,
    3101,   103,  3101,  3101,  3101,  3101,  3101,  3101,  3101,  3101,
    3101,  3101,  3101,  3101,  3101,  3101,  3101,  -435,  3101,  -435,
    -435,  -435,    72,  4015,   318,  4347,  -435,  3101,  3101,  -435,
    3101,  -435,  -435,  3101,  3101,   317,   319,   303,   317,  3101,
     304,  3101,   317,   305,   306,  5010,  -435,  -435,  4954,   257,
     257,     8,  3101,  -435,  -435,  3271,   435,  5010,   257,  -435,
     329,   328,  5010,  3101,  -435,  3361,   289,  -435,  -435,  -435,
    -435,  -435,   429,   213,  -435,   -91,   -72,    89,  3101,  -435,
    5343,  2753,  3101,  -435,  -435,  5010,  -435,  5010,  5010,  5010,
    5010,  5010,  5010,  5010,  5010,  5010,  5010,  5010,  5010,  5010,
    5010,  5010,  -435,  3451,  -435,  1764,     3,  -435,   234,   234,
     234,   -95,  5010,   128,   -72,  -435,   294,  5010,  -435,  -435,
     136,  -435,  -435,  -435,  -435,   104,  3101,  -435,  3541,  -435,
     199,  -435,  -435,  -435,  -435,  3631,  -435,  -435,  -435,  -435,
    3101,  -435,  -435,   307,  5010,  -435,    75,  5010,  -435,  -435,
    2869,   341,  3101,   311,  3101,   314,   199,  3101,  -435,    26,
    -435,  -435,  -435,   907,  5010,  -435,  -435,  4015,  4413,   330,
    3763,  -435,  5010,  -435,  -435,   257,   199,  -435,  5036,  -435,
    2985,   426,   316,   104,  -435,  -435,  4015,  4479,  3101,   321,
    -435,  5331,  -435,  5010,  -435,  -435
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    68,     1,   158,     0,     0,     0,     0,     0,   159,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   293,
       6,    11,     5,     4,    58,    60,    21,    32,    29,    30,
      23,    27,    22,     0,     0,     0,    63,   136,     0,    64,
      66,     0,    62,     0,     0,   294,     0,     0,     0,     0,
       0,    28,     0,     0,    26,   296,     0,    13,     0,   138,
       0,     0,    69,   301,     0,     0,     0,     0,    75,    70,
     291,    56,    57,    54,    55,    53,     0,     0,    52,    61,
      33,    31,    25,    24,     0,     0,     0,     0,   307,   327,
     308,   332,   309,   313,   314,   315,   316,   320,   321,   322,
     323,   324,   325,   326,   328,   329,   353,   312,   319,   330,
     356,   359,   310,   317,   311,   318,     0,   331,   334,   336,
     335,   299,   137,     0,    67,     0,   303,   267,    78,    79,
      81,    80,    82,    83,    84,    85,   104,   105,   102,   103,
      95,   106,   107,    96,    93,    94,   108,   109,   110,   111,
      98,    99,    97,    91,    92,    87,    86,    88,    89,    90,
      77,    76,   100,   101,     0,    73,     0,    14,     0,    48,
      49,    46,    47,    45,    44,    50,     0,     0,   300,     0,
       0,     0,     0,    70,    70,    70,     0,   340,     0,   346,
     349,   348,   342,     0,   345,    65,   302,   262,     0,   305,
      71,   271,     0,   269,   278,     0,     0,   295,   292,     0,
      15,    16,     0,    59,     0,   297,     0,     0,     0,     0,
      73,     0,    73,     0,    73,     0,   274,     0,   273,   341,
     344,     0,   347,   206,   207,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,   178,   177,   179,   180,   181,    17,     0,     0,     0,
       0,     0,     0,   338,     0,   132,    68,   204,   174,   202,
     255,   254,   253,   258,   203,   257,   256,     0,     0,   205,
     363,   364,   365,   366,     0,   304,   264,     0,   270,    72,
       0,   277,     0,   276,     0,     0,    74,   112,   284,   285,
     283,     0,     0,    51,     0,   350,     0,   352,   333,     0,
     354,     0,   357,     0,   360,   362,     0,   343,   339,   140,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   230,
     231,     0,     0,     0,     0,   384,     0,     0,   211,   210,
     243,   238,   209,   208,     0,     0,    70,   261,     0,     0,
       0,     0,   232,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   337,     0,
       0,   265,     0,   268,   272,   306,   279,   282,     0,   281,
       0,   290,   182,   298,     0,   355,   358,   361,   275,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   248,     0,
       0,   136,     0,   375,   369,     0,   378,   379,   380,     0,
       0,     0,     0,     0,     0,     0,    20,     0,    18,   234,
     134,     0,     0,     0,     0,   144,     0,     0,     0,   145,
       0,     0,     0,     0,   115,   113,     0,     0,   127,   122,
     120,   121,   133,   116,     0,     0,     0,   125,   126,   128,
     156,   119,     0,   182,    73,     0,   239,     0,   170,     0,
     212,   213,   225,   226,   223,   224,     0,   250,   237,   259,
     260,   214,   215,     0,   228,   229,   227,   221,   222,   217,
     216,   218,   219,   220,   236,     0,   241,     0,   263,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   286,     0,   289,
     351,   141,     0,     0,     0,     0,   244,     0,     0,   245,
       0,   247,   249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   385,   386,     0,     0,
       0,     0,     0,   146,   149,   182,     0,   139,     0,   131,
       0,     0,    36,     0,   153,   182,     0,   129,   130,   118,
     123,   124,     0,     0,   117,     0,     0,     0,     0,   240,
     252,     0,     0,   235,   242,   280,   135,   189,   190,   192,
     191,   193,   186,   187,   188,   194,   195,   184,   185,   196,
     197,   183,   288,   182,   142,     0,     0,   164,   161,   162,
     163,     0,   367,     0,     0,   371,   370,   376,   382,   381,
       0,   373,   383,    19,    42,    37,     0,   150,   182,   147,
       0,    43,    34,    35,   154,   182,   151,   132,   157,   160,
       0,   172,   175,     0,   171,   199,     0,   251,   287,   167,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
     148,   155,   152,     0,   173,   198,   200,     0,     0,     0,
     387,   372,   368,   374,    38,     0,     0,   114,     0,   165,
       0,     0,     0,    37,    41,   168,     0,     0,     0,     0,
      39,     0,   166,   388,   389,   169
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -435,  -435,  -435,   141,   428,  -435,  -435,  -435,  -435,  -435,
    -435,  -435,  -435,  -435,   432,  -435,  -435,  -204,  -435,  -435,
    -435,  -435,   271,  -435,   439,   -53,  -435,   434,  -435,   230,
    -165,  -207,  -435,  -435,  -202,  -435,  -140,  -290,   -11,  -435,
    -435,  -435,  -435,  -435,  -435,  -435,  -435,   497,  -435,  -435,
    -435,  -373,  -435,    -2,  -435,  -343,  -435,  -435,   191,  -435,
    -435,  -435,  -435,   212,  -435,   189,  -435,  -201,  -184,   110,
     -70,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,  -435,
    -155,   -55,  -435,  -435,   -47,   275,  -434,  -325,  -435,  -435,
    -233,   -40,   171,  -435,  -435,  -435,  -435,  -435
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   168,   174,   337,   267,    14,    15,    31,
      32,    54,    16,    28,    29,   446,   447,   658,   448,   449,
     450,   451,   175,   176,    25,    26,    39,    40,    41,    17,
     165,   206,    69,    18,   268,   452,   345,   453,   269,   454,
     270,   455,   456,   457,   458,   459,   573,   460,   461,   271,
     272,   467,   642,   273,   274,   388,   275,   276,   468,   286,
     382,   287,   197,   201,   202,   226,   227,   534,   203,   302,
     208,   166,    46,    20,    84,    21,    22,   126,    64,    23,
     204,   278,   119,   120,   228,   279,   414,   415,   280,   416,
     417,   418,   419,   281,   336,   282,   682,   283
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   118,   462,   295,   297,   387,   497,   123,   421,   121,
      36,   209,   391,   309,   288,   311,   335,   313,   220,   222,
     224,   533,     2,   177,   640,   626,   522,     3,   331,   198,
     349,   577,   291,   198,   315,   291,   578,   347,    85,    24,
     289,    71,    72,   290,    86,   117,    51,     4,   501,     5,
      42,     6,   229,     7,    27,   424,   316,   425,     8,    87,
     293,   178,   292,   293,     9,   294,   652,   199,    37,   348,
      10,   265,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   574,    30,    38,   616,   207,    37,   352,   353,   167,
     426,   427,   199,   656,   196,   358,   199,   359,   360,   361,
     214,   362,    45,   657,   118,   118,   118,   349,   118,   118,
     118,   118,   216,   217,   218,   292,   221,   223,   225,   292,
      52,    38,   650,    37,   554,   200,   210,   211,    73,   215,
     118,    53,    74,   564,    75,    76,    33,   578,   296,    34,
     611,   230,   358,    35,   651,   360,   376,   377,   117,   117,
     117,   291,   117,   117,   117,   117,   410,    11,    38,   231,
     118,   464,    48,    12,   212,   500,    77,   676,   319,   232,
      65,   546,   118,   118,   117,   578,   578,   187,   499,   293,
     332,   333,   188,   578,   352,   353,   578,    61,   646,   195,
     613,   213,   358,   376,   377,   360,   361,   620,   362,    48,
     536,   189,   579,   584,   117,    47,    48,    58,   602,    50,
     604,   380,    62,   666,   556,   284,   117,   117,    56,   118,
      43,    49,    44,   285,   498,   643,   118,   386,   538,   341,
     543,   118,   349,   659,   390,    66,    67,   575,   541,   394,
     544,   118,   627,   376,   377,   265,   298,   266,   118,   118,
      55,   118,   299,   634,   190,   349,   402,   403,   551,   405,
      57,   537,   540,   117,   653,    68,   538,   541,    59,   538,
     117,    58,    60,   292,   300,   117,   301,   538,    63,   655,
      70,    80,   191,    82,    83,   117,   192,   127,   305,   122,
     169,   170,   117,   117,   125,   117,   162,   163,   164,   167,
     462,   179,   182,   180,   193,   181,   183,   350,   351,   352,
     353,   184,   185,   186,   194,   205,   219,   358,   199,   359,
     360,   361,   317,   362,   308,   363,   364,   624,   625,   381,
     350,   351,   352,   353,   354,   321,   631,   355,   356,   357,
     358,   265,   359,   360,   361,   318,   362,   322,   363,   364,
     323,   324,   325,   641,   371,   372,   373,   374,   375,   326,
     187,   327,   328,    11,   277,   188,   379,   383,   376,   377,
     365,   385,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   395,   349,   189,   304,   396,   397,   400,   469,
     476,   376,   377,   654,   118,   478,   494,   171,   209,   265,
     501,   172,   580,   173,    76,   536,   539,   547,   661,   210,
     559,   566,   560,   567,   568,   569,   329,   330,   570,   571,
     334,   572,   576,   606,   614,   581,   630,   633,   338,   339,
     340,   637,   342,   343,   674,   344,   291,   190,   117,   615,
     618,   621,   622,   632,   638,   665,   669,   671,   118,   680,
     673,   688,   689,   683,   684,    78,   605,   694,   428,   690,
     352,   353,    81,   303,   293,   191,   389,    79,   358,   192,
     359,   360,   361,   392,   362,   124,   346,   663,    19,   586,
     518,   619,   384,   639,   422,   398,     0,   193,     0,     0,
     320,   349,   117,   401,     0,     0,   404,   194,   407,   409,
       0,     0,     0,   413,   413,   371,   372,   373,   374,   375,
       0,     0,     0,     0,     0,     0,   463,     0,     0,   376,
     377,   470,   471,     0,     0,   472,   473,   474,   475,     0,
     477,     0,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,     0,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   353,
       0,   525,     0,     0,     0,     0,   358,     0,   359,   360,
     361,     0,   362,   344,     0,     0,     0,     0,   187,     0,
       0,   344,   118,   188,   545,   334,     0,     0,   548,     0,
     678,     0,   549,   550,     0,   555,     0,     0,   557,   558,
       0,   118,   189,   562,   565,   373,   374,   375,     0,   691,
       0,     0,     0,     0,     0,     0,     0,   376,   377,     0,
       0,     0,     0,     0,     0,     0,   117,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   117,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   190,     0,     0,     0,     0,
     299,   585,     0,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,     0,   603,
       0,     0,   300,   191,     0,     0,     0,   192,   608,   609,
       0,   610,     0,     0,     0,   612,     0,     0,     0,     0,
     617,     0,   413,     0,     0,   193,     0,     0,     0,     0,
       0,   517,     0,   628,   430,   194,     0,     0,     3,     0,
     431,   432,     0,   433,   635,   233,   234,   235,   236,     0,
       0,     0,     0,     0,   434,   237,   435,   436,     0,   644,
       0,     0,     0,   647,     0,     0,   437,   238,     0,     0,
     438,   239,     0,   240,     0,     9,   241,   439,     0,   440,
     242,     0,     0,   441,   442,     0,     0,    88,    89,    90,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,     0,   107,   108,   109,     0,
     243,   112,   113,   114,   115,     0,   244,   443,     0,     0,
       0,   664,     0,     0,     0,     0,     0,     0,   245,   246,
       0,   668,     0,   670,     0,   672,     0,     0,   675,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,    37,   256,
       0,   687,     0,     0,     0,     0,     0,     0,     0,   693,
       0,     0,     0,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,    38,   444,   265,   445,   266,   430,     0,
       0,     0,     3,     0,   431,   432,     0,   433,     0,   233,
     234,   235,   236,     0,     0,     0,     0,     0,   434,   237,
     435,   436,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   238,     0,     0,   438,   239,     0,   240,     0,     9,
     241,   439,     0,   440,   242,     0,     0,   441,   442,     0,
       0,    88,    89,    90,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     0,
     107,   108,   109,     0,   243,   112,   113,   114,   115,     0,
     244,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,    37,   256,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,    38,   444,   265,
     677,   266,   233,   234,   235,   236,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   187,     0,
       0,     0,     0,   188,   238,     0,     0,     0,   239,     0,
     240,     0,     0,   241,     0,     0,     0,   242,     0,     0,
       0,     0,   189,     0,    88,    89,    90,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     0,   107,   108,   109,     0,   243,   112,   113,
     114,   115,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   246,     0,     0,     0,
       0,     0,     0,     0,     0,   190,   187,     0,     0,     0,
       0,   188,     0,     0,     0,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   411,   256,     0,     0,     0,
     189,     0,     0,   191,     0,     0,     0,   192,     0,     0,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   233,   234,   235,   236,   193,     0,   412,     0,     0,
      38,   237,   265,     0,   266,   194,     0,   187,     0,     0,
       0,     0,   188,   238,     0,     0,     0,   239,     0,   240,
       0,     0,   241,   190,     0,     0,   242,     0,     0,     0,
       0,   189,     0,    88,    89,    90,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   191,   107,   108,   109,   192,   243,   112,   113,   114,
     115,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,   245,   246,     0,     0,     0,   306,
       0,     0,     0,   194,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   411,   256,     0,   233,   234,   235,
     236,     0,   191,     0,     0,     0,   192,   237,     0,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,   238,
       0,     0,     0,   239,   193,   240,   420,     0,   241,    38,
       0,   265,   242,   266,   194,     0,     0,     0,     0,    88,
      89,    90,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,   107,   108,
     109,     0,   243,   112,   113,   114,   115,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      37,   256,     0,   233,   234,   235,   236,   523,     0,     0,
       0,     0,     0,   237,     0,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,   238,     0,     0,     0,   239,
     465,   240,   264,   466,   241,    38,     0,   265,   242,   266,
       0,     0,     0,     0,     0,    88,    89,    90,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,   107,   108,   109,     0,   243,   112,
     113,   114,   115,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,    37,   256,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   524,
       0,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   233,   234,   235,   236,     0,     0,   264,     0,
       0,    38,   237,   265,     0,   266,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,     0,   239,     0,
     240,     0,     0,   241,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,    88,    89,    90,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     0,   107,   108,   109,     0,   243,   112,   113,
     114,   115,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   246,     0,     0,     0,
       0,     0,     0,     0,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,    37,   256,     0,   233,   234,
     235,   236,     0,     0,     0,     0,     0,     0,   237,     0,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
     238,     0,     0,     0,   239,     0,   240,   264,     0,   241,
      38,   553,   265,   242,   266,     0,     0,     0,     0,     0,
      88,    89,    90,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,   107,
     108,   109,     0,   243,   112,   113,   114,   115,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,     0,     0,     0,     0,   188,     0,     0,
       0,     0,   247,   248,   249,   250,   251,   252,   253,   254,
     255,    37,   256,     0,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   233,   234,   235,
     236,     0,   263,   264,     0,     0,    38,   237,   265,     0,
     266,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,     0,   239,     0,   240,     0,     0,   241,   190,
       0,     0,   242,     0,     0,     0,     0,     0,     0,    88,
      89,    90,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   191,   107,   108,
     109,   192,   243,   112,   113,   114,   115,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   193,
     245,   246,   649,     0,     0,     0,     0,     0,     0,   194,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      37,   256,     0,   233,   234,   235,   236,     0,     0,     0,
       0,     0,     0,   237,     0,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,   238,     0,     0,     0,   239,
       0,   240,   264,   406,   241,    38,     0,   265,   242,   266,
       0,     0,     0,     0,     0,    88,    89,    90,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,   107,   108,   109,     0,   243,   112,
     113,   114,   115,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,    37,   256,     0,   233,
     234,   235,   236,     0,     0,     0,     0,     0,     0,   237,
       0,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,   238,     0,     0,     0,   239,     0,   240,   264,   408,
     241,    38,     0,   265,   242,   266,     0,     0,     0,     0,
       0,    88,    89,    90,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     0,
     107,   108,   109,     0,   243,   112,   113,   114,   115,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,    37,   256,     0,   233,   234,   235,   236,     0,
       0,     0,     0,     0,     0,   237,     0,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,   238,     0,     0,
       0,   239,     0,   240,   264,   496,   241,    38,     0,   265,
     242,   266,     0,     0,     0,     0,     0,    88,    89,    90,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,     0,   107,   108,   109,     0,
     243,   112,   113,   114,   115,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,    37,   256,
       0,   233,   234,   235,   236,     0,     0,     0,     0,     0,
       0,   237,     0,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,   238,     0,     0,     0,   239,     0,   240,
     264,   521,   241,    38,     0,   265,   242,   266,     0,     0,
       0,     0,     0,    88,    89,    90,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,     0,   107,   108,   109,     0,   243,   112,   113,   114,
     115,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,    37,   256,     0,   233,   234,   235,
     236,     0,     0,     0,     0,     0,     0,   237,     0,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,   238,
       0,     0,     0,   239,     0,   240,   264,   535,   241,    38,
       0,   265,   242,   266,     0,     0,     0,     0,     0,    88,
      89,    90,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,   107,   108,
     109,     0,   243,   112,   113,   114,   115,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      37,   256,     0,   233,   234,   235,   236,     0,     0,     0,
       0,     0,     0,   237,     0,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,   238,     0,     0,     0,   239,
       0,   240,   264,   542,   241,    38,     0,   265,   242,   266,
       0,   561,     0,     0,     0,    88,    89,    90,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,   107,   108,   109,     0,   243,   112,
     113,   114,   115,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,    37,   256,     0,   233,
     234,   235,   236,     0,     0,     0,     0,     0,     0,   237,
       0,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,   238,     0,     0,     0,   239,     0,   240,   264,     0,
     241,    38,     0,   265,   242,   266,     0,     0,     0,     0,
       0,    88,    89,    90,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     0,
     107,   108,   109,     0,   243,   112,   113,   114,   115,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,     0,
       0,   563,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,    37,   256,     0,   233,   234,   235,   236,     0,
       0,     0,     0,     0,     0,   237,     0,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,   238,     0,     0,
       0,   239,     0,   240,   264,     0,   241,    38,     0,   265,
     242,   266,     0,     0,     0,     0,     0,    88,    89,    90,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,     0,   107,   108,   109,     0,
     243,   112,   113,   114,   115,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
     248,   249,   250,   251,   252,   253,   254,   255,    37,   256,
       0,   233,   234,   235,   236,   667,     0,     0,     0,     0,
       0,   237,     0,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,   238,     0,     0,     0,   239,     0,   240,
     264,   645,   241,    38,     0,   265,   242,   266,     0,     0,
       0,     0,     0,    88,    89,    90,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,     0,   107,   108,   109,     0,   243,   112,   113,   114,
     115,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,    37,   256,     0,   233,   234,   235,
     236,   686,     0,     0,     0,     0,     0,   237,     0,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,   238,
       0,     0,     0,   239,     0,   240,   264,     0,   241,    38,
       0,   265,   242,   266,     0,     0,     0,     0,     0,    88,
      89,    90,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     0,   107,   108,
     109,     0,   243,   112,   113,   114,   115,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      37,   256,     0,   233,   234,   235,   236,     0,     0,     0,
       0,     0,     0,   237,     0,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,   238,     0,     0,     0,   239,
       0,   240,   264,     0,   241,    38,     0,   265,   242,   266,
       0,     0,     0,     0,     0,    88,    89,    90,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,     0,   107,   108,   109,     0,   243,   112,
     113,   114,   115,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,     0,   349,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,   255,    37,   256,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,    38,     0,   265,     0,   266,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,   349,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,     0,
       0,     0,   519,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,   349,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,     0,
       0,     0,   629,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,   349,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,     0,
       0,     0,   636,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,   349,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,     0,
       0,     0,   648,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,   349,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,     0,
       0,     0,   660,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,     0,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   509,   350,   351,   352,   353,   354,   510,
     511,   355,   356,   357,   358,   512,   359,   360,   361,     0,
     362,   513,   363,   364,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,   349,     0,     0,     0,   376,   377,   350,   351,   352,
     353,   354,   662,     0,   355,   356,   357,   358,     0,   359,
     360,   361,     0,   362,     0,   363,   364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,   377,
       0,     0,     0,     0,     0,   393,   350,   351,   352,   353,
     354,     0,     0,   355,   356,   357,   358,     0,   359,   360,
     361,     0,   362,     0,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,   365,    86,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,     0,     0,
       0,     0,    87,     0,     0,     0,     0,   376,   377,     0,
       0,     0,     0,     0,   681,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,   247,   248,   249,
       0,     0,     0,     0,     0,     0,    37,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,     0,     0,   349,
       0,    38,     0,   187,     0,    85,     0,     0,   188,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,     0,
     164,   349,     0,    38,   350,   351,   352,   353,   354,     0,
     190,   355,   356,   357,   358,     0,   359,   360,   361,     0,
     362,     0,   363,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   191,     0,
      37,     0,   192,     0,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
     193,     0,   399,     0,     0,   376,   377,   349,     0,   429,
     194,     0,     0,     0,     0,    38,   350,   351,   352,   353,
     354,     0,     0,   355,   356,   357,   358,     0,   359,   360,
     361,     0,   362,     0,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,     0,     0,
       0,     0,     0,   349,     0,     0,     0,   376,   377,     0,
       0,   526,   350,   351,   352,   353,   354,     0,     0,   355,
     356,   357,   358,     0,   359,   360,   361,     0,   362,     0,
     363,   364,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,     0,     0,     0,     0,     0,   349,
       0,     0,     0,   376,   377,     0,     0,   529,   350,   351,
     352,   353,   354,     0,     0,   355,   356,   357,   358,     0,
     359,   360,   361,     0,   362,     0,   363,   364,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
       0,     0,     0,     0,     0,   349,     0,     0,     0,   376,
     377,     0,     0,   531,   350,   351,   352,   353,   354,     0,
       0,   355,   356,   357,   358,     0,   359,   360,   361,     0,
     362,     0,   363,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,   349,     0,     0,     0,   376,   377,     0,     0,   532,
     350,   351,   352,   353,   354,     0,     0,   355,   356,   357,
     358,     0,   359,   360,   361,     0,   362,     0,   363,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,   349,     0,     0,
       0,   376,   377,     0,     0,   607,   350,   351,   352,   353,
     354,     0,     0,   355,   356,   357,   358,     0,   359,   360,
     361,     0,   362,     0,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,     0,     0,
       0,     0,     0,   349,     0,     0,     0,   376,   377,     0,
       0,   679,   350,   351,   352,   353,   354,     0,     0,   355,
     356,   357,   358,     0,   359,   360,   361,     0,   362,     0,
     363,   364,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,     0,     0,     0,   349,     0,     0,
       0,     0,     0,   376,   377,     0,     0,   692,   350,   351,
     352,   353,   354,     0,     0,   355,   356,   357,   358,     0,
     359,   360,   361,     0,   362,     0,   363,   364,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
     377,   378,   350,   351,   352,   353,   354,   349,     0,   355,
     356,   357,   358,     0,   359,   360,   361,     0,   362,     0,
     363,   364,     0,     0,     0,     0,     0,     0,     0,     0,
     187,   187,     0,     0,     0,   188,   188,     0,     0,     0,
       0,     0,   365,     0,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   189,   189,     0,     0,     0,     0,
       0,     0,     0,   376,   377,   583,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,   505,   506,   507,
     508,   509,   350,   351,   352,   353,   354,   510,   511,   355,
     356,   357,   358,   512,   359,   360,   361,  -182,   362,   513,
     363,   364,   514,   515,   349,     0,     0,   190,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   365,     0,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,     0,   191,   191,     0,     0,   192,
     192,   307,   310,   376,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   193,   193,     0,
     349,     0,     0,     0,     0,     0,     0,   194,   194,     0,
       0,   502,   503,   504,   505,   506,   507,   508,   509,   350,
     351,   352,   353,   354,   510,   511,   355,   356,   357,   358,
     512,   359,   360,   361,     0,   362,   513,   363,   364,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,     0,   516,   365,
       0,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,     0,     0,     0,     0,   350,   351,   352,   353,   354,
     376,   377,   355,   356,   357,   358,     0,   359,   360,   361,
       0,   362,     0,   363,   364,     0,     0,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,     0,     0,   365,     0,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,     0,     0,     0,
       0,   350,   351,   352,   353,   354,   376,   377,   355,   356,
     357,   358,     0,   359,   360,   361,     0,   362,     0,   363,
     364,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,   365,   582,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,     0,     0,     0,     0,   350,   351,   352,
     353,   354,   376,   377,   355,   356,   357,   358,     0,   359,
     360,   361,     0,   362,   187,   363,   364,     0,     0,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   623,     0,     0,   365,   189,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   349,
       0,     0,     0,   350,   351,   352,   353,   354,   376,   377,
     355,   356,   357,   358,     0,   359,   360,   361,     0,   362,
     349,   363,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,   365,     0,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,   377,     0,     0,     0,   191,
       0,     0,     0,   192,   350,   351,   352,   353,   354,     0,
       0,   355,   356,   357,   358,   349,   359,   360,   361,     0,
     362,   193,   363,   364,   685,   350,   351,   352,   353,   354,
       0,   194,   355,   356,   357,   358,   349,   359,   360,   361,
       0,   362,     0,   363,   364,     0,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,   377,     0,     0,   368,
     369,   370,   371,   372,   373,   374,   375,   187,     0,     0,
       0,     0,   188,     0,     0,     0,   376,   377,     0,     0,
     350,   351,   352,   353,   354,     0,     0,   355,   356,   357,
     358,   189,   359,   360,   361,     0,   362,     0,   363,   364,
       0,   350,   351,   352,   353,   354,     0,     0,   355,     0,
       0,   358,     0,   359,   360,   361,   187,   362,     0,   363,
     364,   188,     0,     0,     0,   369,   370,   371,   372,   373,
     374,   375,     0,   187,     0,     0,     0,     0,   188,     0,
     189,   376,   377,     0,   190,     0,   369,   370,   371,   372,
     373,   374,   375,   187,     0,     0,   187,   189,   188,     0,
       0,   188,   376,   377,   187,     0,     0,     0,     0,   188,
       0,     0,   191,     0,     0,     0,   192,   189,   312,   187,
     189,     0,     0,     0,   188,     0,     0,     0,   189,     0,
       0,   187,     0,   190,   193,     0,   188,     0,     0,     0,
       0,     0,     0,   189,   194,     0,     0,     0,     0,     0,
     190,     0,     0,     0,     0,   189,     0,     0,     0,     0,
       0,   191,     0,     0,     0,   192,     0,   314,     0,     0,
     190,     0,     0,   190,     0,     0,     0,     0,   191,     0,
       0,   190,   192,   193,   520,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,     0,   190,     0,   191,     0,
     193,   191,   192,     0,   527,   192,     0,   528,   190,   191,
     194,     0,     0,   192,     0,   530,     0,     0,     0,     0,
     193,     0,     0,   193,   191,     0,     0,     0,   192,     0,
     194,   193,     0,   194,     0,     0,   191,     0,     0,     0,
     192,   194,     0,     0,     0,     0,   193,     0,     0,   695,
       0,     0,     0,     0,     0,     0,   194,     0,   193,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   194,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   147,   148,   149,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,     0,
       0,     0,   160,   161
};

static const yytype_int16 yycheck[] =
{
      11,    56,   345,   204,   206,   295,   379,    60,   333,    56,
      29,   166,   302,   220,   198,   222,   249,   224,   183,   184,
     185,    17,     0,   125,   115,    17,   399,     5,    10,    42,
      18,   465,   104,    42,   139,   104,   131,   125,    20,   125,
     158,    12,    13,   161,    26,    56,    49,    25,   108,    27,
      61,    29,    28,    31,   119,   161,   161,   163,    36,    41,
     132,   163,   131,   132,    42,   134,   161,   125,   125,   157,
      48,   162,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   161,   125,   160,   538,   163,   125,    95,    96,   127,
     128,   129,   125,     9,   125,   103,   125,   105,   106,   107,
     132,   109,    53,    19,   179,   180,   181,    18,   183,   184,
     185,   186,   179,   180,   181,   131,   183,   184,   185,   131,
     143,   160,   139,   125,   434,   158,   127,   128,   119,   161,
     205,   154,   123,   443,   125,   126,   125,   131,   205,   125,
     533,   137,   103,   125,   161,   106,   154,   155,   179,   180,
     181,   104,   183,   184,   185,   186,   331,   155,   160,   155,
     235,   346,   131,   161,   131,   386,   157,   161,   235,   165,
      30,   424,   247,   248,   205,   131,   131,    28,   382,   132,
     247,   248,    33,   131,    95,    96,   131,   131,   581,   158,
     535,   158,   103,   154,   155,   106,   107,   542,   109,   131,
     131,    52,   158,   158,   235,   132,   131,   160,   518,   131,
     158,   284,   156,   158,   436,   155,   247,   248,   132,   294,
       4,   134,     6,   163,   156,   156,   301,   294,   161,   260,
     163,   306,    18,   626,   301,    95,    96,   464,   161,   306,
     163,   316,   552,   154,   155,   162,   104,   164,   323,   324,
     162,   326,   110,   563,   105,    18,   323,   324,   433,   326,
     161,   156,   156,   294,   156,   125,   161,   161,   125,   161,
     301,   160,   157,   131,   132,   306,   134,   161,   125,   163,
     162,   119,   133,   125,   125,   316,   137,   162,   139,   125,
      12,    13,   323,   324,   134,   326,    30,    30,   157,   127,
     663,   138,   157,   138,   155,   138,   138,    93,    94,    95,
      96,   138,   138,   138,   165,   134,   125,   103,   125,   105,
     106,   107,   137,   109,   158,   111,   112,   549,   550,    39,
      93,    94,    95,    96,    97,   157,   558,   100,   101,   102,
     103,   162,   105,   106,   107,   156,   109,   157,   111,   112,
     138,   138,   157,   575,   140,   141,   142,   143,   144,   138,
      28,   157,   157,   155,   193,    33,   157,   161,   154,   155,
     133,   125,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   139,    18,    52,   214,   139,   139,   125,    16,
     125,   154,   155,   614,   469,   125,   125,   119,   573,   162,
     108,   123,   469,   125,   126,   131,   131,   163,   630,   127,
     161,    32,   119,   161,   161,   161,   245,   246,   161,   161,
     249,    17,   125,   125,   125,   157,    11,   119,   257,   258,
     259,   162,   261,   262,   656,   264,   104,   105,   469,   156,
     156,   156,   156,   134,    35,   158,   125,   156,   523,   139,
     156,    45,   156,   675,   676,    47,   523,   156,   337,   683,
      95,    96,    50,   212,   132,   133,   295,    48,   103,   137,
     105,   106,   107,   302,   109,    61,   266,   637,     1,   501,
     390,   541,   290,   573,   333,   316,    -1,   155,    -1,    -1,
     235,    18,   523,   322,    -1,    -1,   325,   165,   327,   328,
      -1,    -1,    -1,   332,   333,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,    -1,    -1,   345,    -1,    -1,   154,
     155,   350,   351,    -1,    -1,   354,   355,   356,   357,    -1,
     359,    -1,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    -1,   377,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,   400,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   412,    -1,    -1,    -1,    -1,    28,    -1,
      -1,   420,   667,    33,   423,   424,    -1,    -1,   427,    -1,
     667,    -1,   431,   432,    -1,   434,    -1,    -1,   437,   438,
      -1,   686,    52,   442,   443,   142,   143,   144,    -1,   686,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   667,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   686,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
     110,   500,    -1,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,    -1,   518,
      -1,    -1,   132,   133,    -1,    -1,    -1,   137,   527,   528,
      -1,   530,    -1,    -1,    -1,   534,    -1,    -1,    -1,    -1,
     539,    -1,   541,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   552,     1,   165,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,   563,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    22,    23,    24,    -1,   578,
      -1,    -1,    -1,   582,    -1,    -1,    33,    34,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    44,    -1,    46,
      47,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,   640,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,   650,    -1,   652,    -1,   654,    -1,    -1,   657,    -1,
      -1,    -1,    -1,    -1,   663,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,   680,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   688,
      -1,    -1,    -1,   140,   141,   142,    -1,    -1,   145,    -1,
      -1,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,   164,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    44,    -1,    46,    47,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
      -1,    -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,   164,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    28,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,    -1,    -1,
      52,    -1,    -1,   133,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,   149,
      -1,    12,    13,    14,    15,   155,    -1,   157,    -1,    -1,
     160,    22,   162,    -1,   164,   165,    -1,    28,    -1,    -1,
      -1,    -1,    33,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    43,   105,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    52,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,   133,    73,    74,    75,   137,    77,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    95,    96,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    12,    13,    14,
      15,    -1,   133,    -1,    -1,    -1,   137,    22,    -1,   140,
     141,   142,    -1,    -1,   145,    -1,    -1,   148,   149,    34,
      -1,    -1,    -1,    38,   155,    40,   157,    -1,    43,   160,
      -1,   162,    47,   164,   165,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   140,   141,   142,    -1,    -1,
     145,    -1,    -1,   148,   149,    34,    -1,    -1,    -1,    38,
     155,    40,   157,   158,    43,   160,    -1,   162,    47,   164,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,
     149,    -1,    12,    13,    14,    15,    -1,    -1,   157,    -1,
      -1,   160,    22,   162,    -1,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,
      40,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
     140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,   149,
      34,    -1,    -1,    -1,    38,    -1,    40,   157,    -1,    43,
     160,   161,   162,    47,   164,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,
      -1,   145,    -1,    -1,   148,   149,    -1,    12,    13,    14,
      15,    -1,   156,   157,    -1,    -1,   160,    22,   162,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    38,    -1,    40,    -1,    -1,    43,   105,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,   133,    73,    74,
      75,   137,    77,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      95,    96,   158,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   140,   141,   142,    -1,    -1,
     145,    -1,    -1,   148,   149,    34,    -1,    -1,    -1,    38,
      -1,    40,   157,   158,    43,   160,    -1,   162,    47,   164,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,
     149,    34,    -1,    -1,    -1,    38,    -1,    40,   157,   158,
      43,   160,    -1,   162,    47,   164,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   140,   141,   142,
      -1,    -1,   145,    -1,    -1,   148,   149,    34,    -1,    -1,
      -1,    38,    -1,    40,   157,   158,    43,   160,    -1,   162,
      47,   164,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   140,   141,   142,    -1,    -1,   145,    -1,
      -1,   148,   149,    34,    -1,    -1,    -1,    38,    -1,    40,
     157,   158,    43,   160,    -1,   162,    47,   164,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   140,
     141,   142,    -1,    -1,   145,    -1,    -1,   148,   149,    34,
      -1,    -1,    -1,    38,    -1,    40,   157,   158,    43,   160,
      -1,   162,    47,   164,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   140,   141,   142,    -1,    -1,
     145,    -1,    -1,   148,   149,    34,    -1,    -1,    -1,    38,
      -1,    40,   157,   158,    43,   160,    -1,   162,    47,   164,
      -1,    50,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,
     149,    34,    -1,    -1,    -1,    38,    -1,    40,   157,    -1,
      43,   160,    -1,   162,    47,   164,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   140,   141,   142,
      -1,    -1,   145,    -1,    -1,   148,   149,    34,    -1,    -1,
      -1,    38,    -1,    40,   157,    -1,    43,   160,    -1,   162,
      47,   164,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   140,   141,   142,    -1,    -1,   145,    -1,
      -1,   148,   149,    34,    -1,    -1,    -1,    38,    -1,    40,
     157,   158,    43,   160,    -1,   162,    47,   164,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   140,
     141,   142,    -1,    -1,   145,    -1,    -1,   148,   149,    34,
      -1,    -1,    -1,    38,    -1,    40,   157,    -1,    43,   160,
      -1,   162,    47,   164,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   140,   141,   142,    -1,    -1,
     145,    -1,    -1,   148,   149,    34,    -1,    -1,    -1,    38,
      -1,    40,   157,    -1,    43,   160,    -1,   162,    47,   164,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,   145,    -1,    -1,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,    -1,   162,    -1,   164,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    18,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    18,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    18,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    18,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    18,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,   154,   155,    93,    94,    95,
      96,    97,   161,    -1,   100,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,   161,    93,    94,    95,    96,
      97,    -1,    -1,   100,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,   133,    26,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,   161,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    -1,    -1,    18,
      -1,   160,    -1,    28,    -1,    20,    -1,    -1,    33,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    -1,
     157,    18,    -1,   160,    93,    94,    95,    96,    97,    -1,
     105,   100,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
     125,    -1,   137,    -1,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     155,    -1,   157,    -1,    -1,   154,   155,    18,    -1,   158,
     165,    -1,    -1,    -1,    -1,   160,    93,    94,    95,    96,
      97,    -1,    -1,   100,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,   154,   155,    -1,
      -1,   158,    93,    94,    95,    96,    97,    -1,    -1,   100,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,   154,   155,    -1,    -1,   158,    93,    94,
      95,    96,    97,    -1,    -1,   100,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,   154,
     155,    -1,    -1,   158,    93,    94,    95,    96,    97,    -1,
      -1,   100,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,   154,   155,    -1,    -1,   158,
      93,    94,    95,    96,    97,    -1,    -1,   100,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,   154,   155,    -1,    -1,   158,    93,    94,    95,    96,
      97,    -1,    -1,   100,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,   154,   155,    -1,
      -1,   158,    93,    94,    95,    96,    97,    -1,    -1,   100,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,   154,   155,    -1,    -1,   158,    93,    94,
      95,    96,    97,    -1,    -1,   100,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,   156,    93,    94,    95,    96,    97,    18,    -1,   100,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    28,    -1,    -1,    -1,    33,    33,    -1,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    52,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    18,    -1,    -1,   105,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,   133,   133,    -1,    -1,   137,
     137,   139,   139,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   155,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,   165,   165,    -1,
      -1,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,   132,   133,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
     154,   155,   100,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,   133,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,   154,   155,   100,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,   154,   155,   100,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,    28,   111,   112,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   133,    52,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    18,
      -1,    -1,    -1,    93,    94,    95,    96,    97,   154,   155,
     100,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
      18,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   133,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,    93,    94,    95,    96,    97,    -1,
      -1,   100,   101,   102,   103,    18,   105,   106,   107,    -1,
     109,   155,   111,   112,   158,    93,    94,    95,    96,    97,
      -1,   165,   100,   101,   102,   103,    18,   105,   106,   107,
      -1,   109,    -1,   111,   112,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    28,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,   154,   155,    -1,    -1,
      93,    94,    95,    96,    97,    -1,    -1,   100,   101,   102,
     103,    52,   105,   106,   107,    -1,   109,    -1,   111,   112,
      -1,    93,    94,    95,    96,    97,    -1,    -1,   100,    -1,
      -1,   103,    -1,   105,   106,   107,    28,   109,    -1,   111,
     112,    33,    -1,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,
      52,   154,   155,    -1,   105,    -1,   138,   139,   140,   141,
     142,   143,   144,    28,    -1,    -1,    28,    52,    33,    -1,
      -1,    33,   154,   155,    28,    -1,    -1,    -1,    -1,    33,
      -1,    -1,   133,    -1,    -1,    -1,   137,    52,   139,    28,
      52,    -1,    -1,    -1,    33,    -1,    -1,    -1,    52,    -1,
      -1,    28,    -1,   105,   155,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    52,   165,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,    -1,
     105,    -1,    -1,   105,    -1,    -1,    -1,    -1,   133,    -1,
      -1,   105,   137,   155,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,   105,    -1,   133,    -1,
     155,   133,   137,    -1,   139,   137,    -1,   139,   105,   133,
     165,    -1,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,
     155,    -1,    -1,   155,   133,    -1,    -1,    -1,   137,    -1,
     165,   155,    -1,   165,    -1,    -1,   133,    -1,    -1,    -1,
     137,   165,    -1,    -1,    -1,    -1,   155,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,   148,   149
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   167,     0,     5,    25,    27,    29,    31,    36,    42,
      48,   155,   161,   168,   173,   174,   178,   195,   199,   213,
     239,   241,   242,   245,   125,   190,   191,   119,   179,   180,
     125,   175,   176,   125,   125,   125,    29,   125,   160,   192,
     193,   194,   204,     4,     6,    53,   238,   132,   131,   134,
     131,    49,   143,   154,   177,   162,   132,   161,   160,   125,
     157,   131,   156,   125,   244,    30,    95,    96,   125,   198,
     162,    12,    13,   119,   123,   125,   126,   157,   170,   190,
     119,   180,   125,   125,   240,    20,    26,    41,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,   204,   247,   248,
     249,   250,   125,   191,   193,   134,   243,   162,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   111,   112,   113,   114,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     148,   149,    30,    30,   157,   196,   237,   127,   169,    12,
      13,   119,   123,   125,   170,   188,   189,   125,   163,   138,
     138,   138,   157,   138,   138,   138,   138,    28,    33,    52,
     105,   133,   137,   155,   165,   158,   204,   228,    42,   125,
     158,   229,   230,   234,   246,   134,   197,   163,   236,   246,
     127,   128,   131,   158,   132,   161,   250,   250,   250,   125,
     196,   250,   196,   250,   196,   250,   231,   232,   250,    28,
     137,   155,   165,    12,    13,    14,    15,    22,    34,    38,
      40,    43,    47,    77,    83,    95,    96,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   126,   140,   141,   142,
     145,   148,   149,   156,   157,   162,   164,   172,   200,   204,
     206,   215,   216,   219,   220,   222,   223,   224,   247,   251,
     254,   259,   261,   263,   155,   163,   225,   227,   234,   158,
     161,   104,   131,   132,   134,   233,   250,   200,   104,   110,
     132,   134,   235,   188,   224,   139,   161,   139,   158,   197,
     139,   197,   139,   197,   139,   139,   161,   137,   156,   250,
     251,   157,   157,   138,   138,   157,   138,   157,   157,   224,
     224,    10,   250,   250,   224,   256,   260,   171,   224,   224,
     224,   204,   224,   224,   224,   202,   195,   125,   157,    18,
      93,    94,    95,    96,    97,   100,   101,   102,   103,   105,
     106,   107,   109,   111,   112,   133,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   154,   155,   156,   157,
     191,    39,   226,   161,   229,   125,   250,   203,   221,   224,
     250,   203,   224,   161,   250,   139,   139,   139,   231,   157,
     125,   224,   250,   250,   224,   250,   158,   224,   158,   224,
     246,   125,   157,   224,   252,   253,   255,   256,   257,   258,
     157,   253,   258,   115,   161,   163,   128,   129,   169,   158,
       1,     7,     8,    10,    21,    23,    24,    33,    37,    44,
      46,    50,    51,    84,   161,   163,   181,   182,   184,   185,
     186,   187,   201,   203,   205,   207,   208,   209,   210,   211,
     213,   214,   221,   224,   196,   155,   158,   217,   224,    16,
     224,   224,   224,   224,   224,   224,   125,   224,   125,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   125,   224,   158,   217,   156,   234,
     233,   108,    85,    86,    87,    88,    89,    90,    91,    92,
      98,    99,   104,   110,   113,   114,   132,   161,   235,   161,
     139,   158,   217,    16,   138,   224,   158,   139,   139,   158,
     139,   158,   158,    17,   233,   158,   131,   156,   161,   131,
     156,   161,   158,   163,   163,   224,   256,   163,   224,   224,
     224,   246,   104,   161,   203,   224,   200,   224,   224,   161,
     119,    50,   224,   104,   203,   224,    32,   161,   161,   161,
     161,   161,    17,   212,   161,   197,   125,   252,   131,   158,
     250,   157,   134,   156,   158,   224,   219,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   203,   224,   158,   250,   125,   158,   224,   224,
     224,   217,   224,   253,   125,   156,   252,   224,   156,   257,
     253,   156,   156,   130,   200,   200,    17,   203,   224,   161,
      11,   200,   134,   119,   203,   224,   161,   162,    35,   236,
     115,   200,   218,   156,   224,   158,   217,   224,   161,   158,
     139,   161,   161,   156,   233,   163,     9,    19,   183,   217,
     161,   200,   161,   202,   224,   158,   158,    16,   224,   125,
     224,   156,   224,   156,   200,   224,   161,   163,   250,   158,
     139,   161,   262,   200,   200,   158,    16,   224,    45,   156,
     183,   250,   158,   224,   156,   158
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   166,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   169,   169,   170,   171,   171,   171,
     172,   173,   174,   175,   175,   175,   176,   177,   177,   178,
     179,   179,   180,   180,   181,   182,   182,   183,   183,   183,
     184,   185,   186,   187,   188,   188,   188,   188,   188,   188,
     189,   189,   190,   190,   190,   190,   190,   190,   190,   190,
     191,   191,   192,   192,   193,   193,   194,   194,   195,   195,
     196,   196,   196,   197,   197,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   199,   200,   200,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   202,   202,   202,   203,   204,   204,   204,   205,
     206,   206,   206,   206,   207,   208,   209,   209,   209,   209,
     209,   210,   210,   210,   210,   211,   212,   212,   213,   213,
     214,   215,   215,   215,   216,   216,   216,   216,   216,   216,
     217,   217,   218,   218,   219,   219,   220,   220,   220,   220,
     220,   220,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   223,
     223,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   226,   226,   227,   228,   228,   229,
     229,   230,   230,   231,   232,   232,   233,   233,   234,   234,
     234,   234,   234,   235,   235,   235,   236,   236,   236,   236,
     236,   237,   237,   238,   238,   239,   240,   240,   240,   241,
     242,   243,   243,   244,   245,   246,   246,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   248,   249,   249,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   251,   251,   251,   251,   252,   252,   253,
     253,   254,   254,   254,   254,   255,   255,   256,   257,   257,
     258,   258,   259,   259,   260,   260,   261,   262,   262,   263
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
       3,     0,     2,     0,     1,     5,     0,     3,     5,     4,
       5,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     4,     3,     4,
       2,     3,     2,     4,     3,     2,     2,     3,     2,     2,
       4,     6,     4,     1,     4,     5,     1,     4,     5,     1,
       4,     5,     4,     1,     1,     1,     1,     3,     5,     1,
       3,     5,     7,     5,     7,     1,     3,     3,     1,     1,
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
          case 125: /* "name"  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2812 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* character_sequence  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2818 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* string_constant  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2824 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* string_builder_body  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2830 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* string_builder  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2836 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* require_module_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2842 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expression_label  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2848 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_goto  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2854 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2860 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_if_then_else  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2866 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_for_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2872 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_while_loop  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2878 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_with  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2884 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* annotation_argument_value  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2890 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* annotation_argument_value_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2896 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2902 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_argument_list  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2908 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_declaration_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2914 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_declaration  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2920 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2926 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* optional_annotation_list  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2932 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* optional_function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2938 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* optional_function_type  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2944 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* function_name  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2950 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expression_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2956 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expression_any  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2962 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expressions  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2968 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_pipe  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2974 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* name_in_namespace  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2980 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expression_delete  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2986 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expr_new  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2992 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_break  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2998 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expression_continue  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3004 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_return  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3010 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_yield  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3016 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_try_catch  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3022 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expression_let  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3028 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expr_cast  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3034 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expr_type_info  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3040 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_list  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3046 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* block_or_simple_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3052 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_block  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3058 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expr_numeric_const  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3064 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expr_assign  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3070 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_named_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3076 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_method_call  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3082 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3088 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* optional_field_annotation  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3094 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* structure_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3100 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* struct_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3106 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* function_argument_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3112 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* function_argument_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3118 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* tuple_type  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3124 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* tuple_type_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3130 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3136 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* let_variable_declaration  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3142 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* global_variable_declaration_list  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3148 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* enum_list  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3154 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* optional_structure_parent  */
#line 101 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3160 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* variable_name_list  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3166 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* structure_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3172 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* auto_type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3178 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* type_declaration  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3184 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* make_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3190 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* make_struct_fields  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3196 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* make_struct_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3202 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* make_struct_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3208 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* make_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3214 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_map_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3220 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_any_tuple  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3226 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_dim  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3232 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_dim_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3238 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* make_table  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3244 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* make_table_decl  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3250 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* array_comprehension_where  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3256 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* array_comprehension  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3262 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 380 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[-1].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[-1].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[-1])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[-1].s);
    }
#line 3558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3564 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 393 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3570 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 397 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 401 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 405 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 414 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 422 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 438 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 463 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 466 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3669 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 481 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 494 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 495 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 508 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 511 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3723 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3731 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 523 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 526 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3747 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 534 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 548 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 560 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3803 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3814 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3820 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 580 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3832 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3844 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 588 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 601 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 602 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 603 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 604 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 605 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 607 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 608 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 614 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 627 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 648 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 655 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 671 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4012 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 678 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4018 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 679 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 683 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 684 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4036 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 689 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 693 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 700 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4075 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4081 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 706 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4093 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 709 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 710 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 723 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 724 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 726 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 727 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 728 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 730 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4237 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 732 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 733 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4249 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 735 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 738 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4356 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 813 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 816 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 826 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 827 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 828 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 829 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 830 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 831 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 832 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 833 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 834 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 835 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 836 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 837 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 838 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 839 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 840 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 841 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 842 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 850 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 856 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 862 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4528 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 883 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4578 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 913 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4606 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 920 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 923 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4624 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 928 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 931 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4642 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4650 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 942 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4660 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 947 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4678 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 958 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 964 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 965 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 974 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1010 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4773 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1023 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1027 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4803 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1038 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1042 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1047 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4833 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1056 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1059 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4849 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1065 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1066 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4867 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1076 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1082 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1135 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1137 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1138 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1142 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1143 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4978 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1144 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1145 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1146 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1147 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1148 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1149 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1150 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5020 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1151 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1154 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1155 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1156 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1161 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1172 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1177 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1190 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1191 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1199 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1201 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5186 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1210 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1220 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1221 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1224 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5333 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5350 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 5358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1239 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-2]))); }
#line 5394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeGenerator(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5428 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5434 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5446 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5452 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5458 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5530 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5566 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1334 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1341 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1342 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1346 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1351 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1357 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1358 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1362 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1363 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1367 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5669 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1385 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5692 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1400 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5706 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1403 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5716 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1408 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1413 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1430 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5758 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1433 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5767 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1440 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5773 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1441 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1445 "ds_parser.ypp" /* yacc.c:1660  */
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
                    if ( (yyvsp[-4].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pVar->global_shared = (yyvsp[-3].b);
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1475 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5819 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1478 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5833 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1487 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1499 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1511 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1525 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1526 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5889 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1569 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1635 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6010 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1642 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1651 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1653 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1654 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6057 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1657 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1658 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6069 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6075 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6081 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6093 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1668 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1669 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1670 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1671 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1672 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1673 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1674 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1678 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6178 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1689 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1693 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1703 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6205 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6211 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1706 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1720 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6247 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1726 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6256 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1730 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1735 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1740 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1745 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1752 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1757 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6317 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1761 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6326 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1770 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1775 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6368 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1802 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6422 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1816 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1820 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1825 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1834 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6464 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1838 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1843 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6488 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1852 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6499 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6505 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1862 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6511 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1868 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6535 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1875 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1884 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1889 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1896 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1901 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1907 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1925 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1928 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1942 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1951 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1952 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1956 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1961 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1968 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1973 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6700 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1984 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 1989 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 1996 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2009 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2013 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6763 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6767 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2028 "ds_parser.ypp" /* yacc.c:1903  */


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


