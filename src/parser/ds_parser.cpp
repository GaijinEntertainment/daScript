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
#line 51 "ds_parser.ypp" /* yacc.c:338  */

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
        extern ProgramPtr			    g_Program;
        extern FileAccessPtr            g_Access;
        extern vector<FileInfo *>	    g_FileAccessStack;
        extern das_map<string,string>   das_module_alias;
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

#line 190 "ds_parser.cpp" /* yacc.c:353  */

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
    DAS_AS = 274,
    DAS_ELIF = 275,
    DAS_ARRAY = 276,
    DAS_RETURN = 277,
    DAS_NULL = 278,
    DAS_BREAK = 279,
    DAS_TRY = 280,
    DAS_OPTIONS = 281,
    DAS_TABLE = 282,
    DAS_EXPECT = 283,
    DAS_CONST = 284,
    DAS_REQUIRE = 285,
    DAS_OPERATOR = 286,
    DAS_ENUM = 287,
    DAS_FINALLY = 288,
    DAS_DELETE = 289,
    DAS_DEREF = 290,
    DAS_SCOPE = 291,
    DAS_TYPEDEF = 292,
    DAS_WITH = 293,
    DAS_CAST = 294,
    DAS_OVERRIDE = 295,
    DAS_UPCAST = 296,
    DAS_ITERATOR = 297,
    DAS_VAR = 298,
    DAS_ADDR = 299,
    DAS_CONTINUE = 300,
    DAS_WHERE = 301,
    DAS_PASS = 302,
    DAS_REINTERPRET = 303,
    DAS_MODULE = 304,
    DAS_PUBLIC = 305,
    DAS_LABEL = 306,
    DAS_GOTO = 307,
    DAS_IMPLICIT = 308,
    DAS_SHARED = 309,
    DAS_TBOOL = 310,
    DAS_TVOID = 311,
    DAS_TSTRING = 312,
    DAS_TAUTO = 313,
    DAS_TINT = 314,
    DAS_TINT2 = 315,
    DAS_TINT3 = 316,
    DAS_TINT4 = 317,
    DAS_TUINT = 318,
    DAS_TUINT2 = 319,
    DAS_TUINT3 = 320,
    DAS_TUINT4 = 321,
    DAS_TFLOAT = 322,
    DAS_TFLOAT2 = 323,
    DAS_TFLOAT3 = 324,
    DAS_TFLOAT4 = 325,
    DAS_TRANGE = 326,
    DAS_TURANGE = 327,
    DAS_TBLOCK = 328,
    DAS_TINT64 = 329,
    DAS_TUINT64 = 330,
    DAS_TDOUBLE = 331,
    DAS_TFUNCTION = 332,
    DAS_TLAMBDA = 333,
    DAS_TINT8 = 334,
    DAS_TUINT8 = 335,
    DAS_TINT16 = 336,
    DAS_TUINT16 = 337,
    DAS_TTUPLE = 338,
    DAS_GENERATOR = 339,
    DAS_YIELD = 340,
    ADDEQU = 341,
    SUBEQU = 342,
    DIVEQU = 343,
    MULEQU = 344,
    MODEQU = 345,
    ANDEQU = 346,
    OREQU = 347,
    XOREQU = 348,
    SHL = 349,
    SHR = 350,
    ADDADD = 351,
    SUBSUB = 352,
    LEEQU = 353,
    SHLEQU = 354,
    SHREQU = 355,
    GREQU = 356,
    EQUEQU = 357,
    NOTEQU = 358,
    RARROW = 359,
    LARROW = 360,
    QQ = 361,
    QDOT = 362,
    LPIPE = 363,
    LBPIPE = 364,
    LAPIPE = 365,
    RPIPE = 366,
    CLONEEQU = 367,
    ROTL = 368,
    ROTR = 369,
    ROTLEQU = 370,
    ROTREQU = 371,
    MAPTO = 372,
    BRABRAB = 373,
    BRACBRB = 374,
    CBRCBRB = 375,
    INTEGER = 376,
    LONG_INTEGER = 377,
    UNSIGNED_INTEGER = 378,
    UNSIGNED_LONG_INTEGER = 379,
    FLOAT = 380,
    DOUBLE = 381,
    NAME = 382,
    BEGIN_STRING = 383,
    STRING_CHARACTER = 384,
    END_STRING = 385,
    BEGIN_STRING_EXPR = 386,
    END_STRING_EXPR = 387,
    UNARY_MINUS = 388,
    UNARY_PLUS = 389,
    PRE_INC = 390,
    PRE_DEC = 391,
    POST_INC = 392,
    POST_DEC = 393,
    DEREF = 394,
    COLCOL = 395
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 77 "ds_parser.ypp" /* yacc.c:353  */

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

#line 368 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   6003

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  100
/* YYNRULES -- Number of rules.  */
#define YYNRULES  394
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  708

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   395

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   150,     2,   167,   165,   146,   139,     2,
     158,   159,   144,   143,   133,   142,   155,   145,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   136,   162,
     140,   134,   141,   135,   166,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   156,     2,   157,   138,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   163,   137,   164,   149,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   132,   147,   148,
     151,   152,   153,   154,   160,   161
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   370,   370,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   385,   397,   398,   402,   406,   410,   419,
     427,   443,   464,   468,   471,   477,   486,   507,   530,   531,
     535,   539,   540,   544,   547,   553,   559,   562,   568,   569,
     570,   577,   584,   596,   606,   615,   616,   617,   618,   619,
     620,   624,   629,   637,   638,   639,   640,   641,   642,   643,
     644,   650,   663,   679,   680,   684,   691,   703,   707,   714,
     715,   719,   720,   721,   725,   729,   736,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   779,   849,   852,   862,   863,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     876,   877,   878,   879,   883,   887,   893,   899,   910,   916,
     917,   928,   932,   938,   941,   944,   948,   954,   958,   962,
     965,   968,   973,   976,   984,   987,   992,   995,  1003,  1009,
    1010,  1014,  1015,  1019,  1053,  1056,  1061,  1069,  1073,  1078,
    1084,  1088,  1093,  1102,  1105,  1111,  1112,  1122,  1123,  1127,
    1132,  1184,  1185,  1186,  1187,  1188,  1189,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1212,  1213,  1217,  1228,  1233,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1281,  1285,  1289,  1292,
    1295,  1296,  1297,  1298,  1303,  1308,  1309,  1312,  1315,  1316,
    1317,  1318,  1319,  1320,  1321,  1331,  1341,  1375,  1376,  1380,
    1381,  1385,  1393,  1396,  1403,  1404,  1408,  1409,  1413,  1419,
    1420,  1424,  1425,  1429,  1434,  1437,  1441,  1447,  1456,  1457,
    1458,  1462,  1465,  1470,  1475,  1482,  1492,  1495,  1502,  1503,
    1507,  1537,  1540,  1549,  1561,  1573,  1587,  1588,  1592,  1631,
    1697,  1704,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1733,  1734,  1735,  1736,  1740,  1751,  1755,  1765,
    1766,  1767,  1768,  1782,  1788,  1792,  1797,  1802,  1807,  1814,
    1819,  1823,  1827,  1832,  1837,  1844,  1849,  1855,  1860,  1864,
    1869,  1878,  1882,  1887,  1896,  1900,  1905,  1914,  1923,  1924,
    1925,  1926,  1930,  1937,  1946,  1951,  1958,  1963,  1969,  1976,
    1987,  1990,  2004,  2013,  2014,  2018,  2023,  2030,  2035,  2046,
    2051,  2058,  2070,  2071,  2075
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
  "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"", "\"array\"",
  "\"return\"", "\"null\"", "\"break\"", "\"try\"", "\"options\"",
  "\"table\"", "\"expect\"", "\"const\"", "\"require\"", "\"operator\"",
  "\"enum\"", "\"finally\"", "\"delete\"", "\"deref\"", "\"scope\"",
  "\"typedef\"", "\"with\"", "\"cast\"", "\"override\"", "\"upcast\"",
  "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"", "\"where\"",
  "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"",
  "\"goto\"", "\"implicit\"", "\"shared\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"",
  "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"generator\"", "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"",
  "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"",
  "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"",
  "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"<|\"", "\" <|\"",
  "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS",
  "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'", "'['",
  "']'", "'('", "')'", "DEREF", "COLCOL", "';'", "'{'", "'}'", "'$'",
  "'@'", "'#'", "$accept", "program", "module_declaration",
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
  "block_or_lambda", "expr_block", "expr_numeric_const", "expr_assign",
  "expr_assign_pipe", "expr_named_call", "expr_method_call", "expr",
  "optional_field_annotation", "optional_override",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "copy_or_move", "variable_declaration",
  "copy_or_move_or_clone", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared", "global_let",
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   388,   389,   126,
      33,   390,   391,   392,   393,    46,    91,    93,    40,    41,
     394,   395,    59,   123,   125,    36,    64,    35
};
# endif

#define YYPACT_NINF -425

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-425)))

#define YYTABLE_NINF -188

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -425,    32,  -425,  -425,   -91,   -72,   -66,   -50,    20,  -425,
      29,    -7,  -425,  -425,  -425,  -425,  -425,   159,  -425,    17,
    -425,  -425,  -425,  -425,   -10,  -425,   106,   109,   132,  -425,
    -425,     5,  -425,   116,   137,  -425,  -425,   121,   157,   128,
    -425,   -74,  -425,   160,    87,  -425,   125,    21,   -91,   168,
     -72,   166,  -425,   169,   172,  -425,  -425,  5084,   176,  -425,
     -91,    -7,  -425,   170,   141,  5853,   277,   278,  -425,   152,
    -425,  -425,  -425,  -425,  -425,  -425,   183,    41,  -425,  -425,
    -425,  -425,   263,  -425,  -425,   -85,   179,   180,   181,  -425,
    -425,  -425,   158,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,   182,  -425,  -425,
    -425,   185,   186,  -425,  -425,  -425,  -425,   187,  -425,  -425,
    -425,  -425,  3159,  -425,   -81,  -425,   -96,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,  -425,    -4,   193,   -84,  -425,   120,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,   -77,  -425,   -86,
    -425,  5084,  5084,  5084,   203,  5011,  5011,  5011,  5084,  -425,
      -3,  -425,  -425,  -425,  -425,  1965,  -425,  -425,  -425,    52,
     204,  -425,  -425,  -425,    97,  -425,   -89,  5084,   171,  -425,
    -425,   110,  -425,  -425,    41,  -425,  3706,  -425,   345,   438,
    1097,   174,   193,  1780,   193,  1829,   193,  2046,  -425,   -78,
    3159,   202,  -425,   191,  -425,  -425,  -425,  4938,   184,  -425,
     195,   211,   212,   196,   217,   218,  3706,  3706,    30,  5084,
    3706,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  3706,  3706,
    3706,  3706,  3706,  -425,  3706,  -425,  -425,   177,  -425,  -425,
     -92,  -425,  -425,  -425,   188,  -425,  -425,  -425,  -425,  5079,
     201,  -425,  -425,  -425,  -425,  -425,   -91,  -425,   323,   205,
    -425,  -425,   -13,  -425,   237,  -425,  5084,  2098,  3159,  -425,
    -425,  -425,  -425,  5084,  2098,  -425,  4305,  -425,  5084,  -425,
    -425,   224,  -425,   225,  -425,   229,  -425,  -425,  5084,  -425,
    -425,   473,  -425,   248,  3706,  5084,  5084,  3706,  5084,  5084,
     461,   461,   204,  1149,  1288,  5372,  -425,    55,    74,   461,
     461,  -425,   461,   461,  4437,   828,   -96,  -425,  1560,   152,
     361,  3706,  3706,  -425,  -425,  3706,  3706,  3706,  3706,   253,
    3706,   255,  3706,  3706,  3706,  3706,  3706,  3706,  3706,  3706,
    3706,  3706,  3706,  3706,  3706,  3706,  3706,   256,  3706,  -425,
    2231,    53,  -425,   204,  -425,  -425,  -425,   189,    88,  -425,
     275,  5224,   447,  -425,  3787,  -425,  2095,  -425,  -425,  -425,
    -425,  2364,  1699,  4503,  2312,  2578,  4569,  2844,  2893,     8,
      68,  2497,  5372,   252,    70,   259,  -425,  -425,    71,  2630,
      99,   104,  3706,  3706,   222,  -425,  3706,   264,  -425,  -425,
    3706,  3706,   204,  1427,  -425,   171,  3706,  3706,  -425,   240,
     282,  2763,  1832,  -425,   372,   246,   247,  -425,  -425,  -425,
    -425,  -425,  -425,   251,   254,   257,  -425,  -425,  -425,   401,
    -425,   -88,  -425,  5316,  -425,   301,  -425,    47,  5540,   193,
    5084,   194,   194,  5782,  5782,  5761,  5761,   283,   265,  -425,
     -34,   -34,   194,   194,  5428,   293,  5596,  5652,  5782,  5782,
     350,   350,   265,   265,   265,  -425,  5152,  -425,    65,  -425,
    -425,  3706,  -425,  -425,    88,  3706,  3706,  3706,  3706,  3706,
    3706,  3706,  3706,  3706,  3706,  3706,  3706,  3706,  3706,  3706,
    -425,  2098,  -425,  -425,  -425,    66,  5084,   313,  4635,  -425,
    3706,  3706,  -425,  3706,   284,  3706,  3706,   301,   316,   287,
     301,  3706,   288,  3706,   301,   295,   296,  5540,  -425,  -425,
    5484,   620,   620,    11,  2098,  -425,  -425,  3878,   439,  5540,
     620,  -425,   319,   342,  5540,  2098,  -425,  3969,   302,  -425,
    -425,  -425,  -425,  -425,   428,   204,  -425,  2896,  3029,   -67,
      64,  3706,  -425,   -95,  3159,  3162,  3706,  -425,  -425,  5540,
    -425,  5540,  5540,  5540,  5540,  5540,  5540,  5540,  5540,  5540,
    5540,  5540,  5540,  5540,  5540,  5540,  -425,  4060,  -425,   635,
     -22,  -425,  5782,  5782,  5782,  3295,    25,  5540,    79,   -67,
    -425,   252,  5540,  -425,  -425,   112,  -425,  -425,  -425,  -425,
     108,  3706,  -425,  4151,  -425,   171,  -425,  -425,  -425,  -425,
    4242,  -425,  -425,  -425,  -425,  3706,  3706,   307,  5540,  3706,
    -425,  -425,  -425,    76,  5540,  -425,  -425,  3434,   341,  -425,
    4701,  3706,   314,  3706,   317,   171,  3706,  -425,    45,  -425,
    -425,  -425,   994,  5708,  5708,  -425,  5540,  -425,  5084,  4767,
     332,  -425,  4374,  -425,  5540,  -425,  -425,   620,   171,  -425,
    3243,  -425,  3573,   429,   320,   108,  -425,  -425,  5084,  4833,
    3706,   328,  -425,  3654,  -425,  5540,  -425,  -425
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    69,     1,   161,     0,     0,     0,     0,     0,   162,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   298,
       6,    11,     5,     4,    59,    61,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    64,   139,     0,    65,
      67,     0,    63,     0,     0,   299,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,   301,     0,     0,   141,
       0,     0,    70,   306,     0,     0,     0,     0,    76,    71,
     296,    57,    58,    55,    56,    54,     0,     0,    53,    62,
      34,    32,    28,    25,    24,     0,     0,     0,     0,   312,
     332,   313,   337,   314,   318,   319,   320,   321,   325,   326,
     327,   328,   329,   330,   331,   333,   334,   358,   317,   324,
     335,   361,   364,   315,   322,   316,   323,     0,   336,   339,
     341,   340,   304,   140,     0,    68,     0,   308,   272,    79,
      80,    82,    81,    83,    84,    85,    86,   105,   106,   103,
     104,    96,   107,   108,    97,    94,    95,   109,   110,   111,
     112,    99,   100,    98,    92,    93,    88,    87,    89,    90,
      91,    78,    77,   101,   102,     0,    74,     0,    14,     0,
      49,    50,    47,    48,    46,    45,    51,     0,    27,     0,
     305,     0,     0,     0,     0,    71,    71,    71,     0,   345,
       0,   351,   354,   353,   347,     0,   350,    66,   307,   267,
       0,   310,    72,   276,     0,   274,   283,     0,     0,   300,
     297,     0,    15,    16,     0,    60,     0,   302,     0,     0,
       0,     0,    74,     0,    74,     0,    74,     0,   279,     0,
     278,   346,   349,     0,   352,   213,   214,     0,     0,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,   183,   182,   184,   185,   186,    17,     0,     0,
       0,     0,     0,   343,     0,   134,   177,   178,   211,   179,
     209,   260,   259,   258,    69,   263,   210,   262,   261,     0,
       0,   212,   368,   369,   370,   371,     0,   309,   269,     0,
     275,    73,     0,   282,     0,   281,     0,     0,    75,   113,
     289,   290,   288,     0,     0,    52,     0,   355,     0,   357,
     338,     0,   359,     0,   362,     0,   365,   367,     0,   348,
     344,   143,   146,     0,     0,     0,     0,     0,     0,     0,
     237,   238,     0,     0,     0,     0,   389,     0,     0,   218,
     217,   250,   216,   215,     0,     0,     0,   266,     0,    71,
       0,     0,     0,   239,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,   270,     0,   273,   277,   311,   284,     0,   287,
       0,   286,     0,   295,   187,   303,     0,   360,   363,   366,
     280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,   380,   374,     0,   383,   384,   385,     0,     0,
       0,     0,     0,     0,     0,    20,     0,    18,   241,   136,
       0,     0,     0,     0,   147,     0,     0,     0,   148,     0,
       0,     0,     0,   116,   114,     0,     0,   129,   124,   122,
     123,   135,   117,     0,     0,     0,   127,   128,   130,   159,
     121,     0,   118,   187,   245,     0,   246,     0,   173,    74,
       0,   219,   220,   232,   233,   230,   231,     0,   255,   244,
     264,   265,   221,   222,     0,   235,   236,   234,   228,   229,
     224,   223,   225,   226,   227,   243,     0,   248,     0,   268,
     271,     0,   178,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,   294,   356,   144,     0,     0,     0,     0,   251,
       0,     0,   252,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   382,   390,   391,
       0,     0,     0,     0,     0,   149,   152,   187,     0,   142,
       0,   133,     0,     0,    37,     0,   156,   187,     0,   131,
     132,   120,   125,   126,     0,     0,   119,     0,     0,     0,
       0,     0,   247,     0,   257,     0,     0,   242,   249,   285,
     137,   194,   195,   197,   196,   198,   191,   192,   193,   199,
     200,   189,   190,   201,   202,   188,   293,   187,   145,     0,
       0,   167,   164,   165,   166,     0,     0,   372,     0,     0,
     376,   375,   381,   387,   386,     0,   378,   388,    19,    43,
      38,     0,   153,   187,   150,     0,    44,    35,    36,   157,
     187,   154,   134,   160,   163,     0,     0,     0,   174,     0,
     175,   180,   206,     0,   256,   292,   170,     0,     0,   253,
       0,     0,     0,     0,     0,     0,     0,    41,     0,   151,
     158,   155,     0,   204,   203,   205,   176,   207,     0,     0,
       0,   254,   392,   377,   373,   379,    39,     0,     0,   115,
       0,   168,     0,     0,     0,    38,    42,   171,     0,     0,
       0,     0,    40,     0,   169,   393,   394,   172
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -425,  -425,  -425,   149,   441,  -425,  -425,  -425,  -425,  -425,
    -425,   408,  -425,  -425,   443,  -425,  -425,  -198,  -425,  -425,
    -425,  -425,   289,  -425,   456,   -52,  -425,   448,  -425,   234,
    -167,  -211,  -425,  -425,  -205,  -425,  -131,  -290,   -11,  -425,
    -425,  -425,  -425,  -425,  -425,  -425,  -425,   512,  -425,  -425,
    -425,  -374,  -425,  -425,  -359,  -425,  -340,  -425,  -425,  -425,
     165,  -425,  -425,  -425,  -425,   226,  -425,   206,  -425,  -202,
    -188,   122,   -33,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -165,   -56,  -425,  -425,   -48,   308,  -424,  -324,
    -425,  -425,  -233,   -24,   213,  -425,  -425,  -425,  -425,  -425
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,   175,   338,   268,    14,    15,    31,
      32,    55,    16,    28,    29,   445,   446,   667,   447,   448,
     449,   450,   176,   177,    25,    26,    39,    40,    41,    17,
     166,   208,    69,    18,   269,   451,   345,   452,   270,   453,
     271,   454,   455,   456,   457,   458,   575,   459,   460,   272,
     273,   467,   651,   274,   275,   276,   390,   462,   277,   278,
     468,   288,   383,   289,   199,   203,   204,   228,   229,   536,
     205,   304,   210,   167,    46,    20,    85,    21,    22,   127,
      64,    23,   206,   280,   120,   121,   230,   281,   413,   414,
     282,   415,   416,   417,   418,   283,   337,   284,   694,   285
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   119,   211,   299,   297,   461,   498,   389,   124,   122,
     420,   311,   290,   313,   393,   315,   293,   336,   222,   224,
     226,   504,   649,    36,    51,   535,   231,   525,   631,   503,
     200,    37,     2,    71,    72,   347,    24,     3,   293,   200,
     332,   580,   179,   201,   294,   295,   118,   296,   216,    27,
      42,    86,    48,   170,   171,    52,   214,    87,     4,    61,
       5,    30,     6,   317,     7,    38,   348,   295,   265,     8,
     359,    45,    88,   361,   576,     9,   217,    33,   197,   180,
     209,    10,   215,    62,   318,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   201,   198,   621,   665,    65,   657,
      37,   377,   378,   201,    47,   119,   119,   119,   666,   119,
     119,   119,   119,   218,   219,   220,   232,   223,   225,   227,
     658,   294,    73,   556,   294,   590,    74,    34,    75,    76,
      53,   119,   566,   233,    38,   202,    35,    37,   581,   298,
      54,   616,   172,    43,   234,    44,   173,   409,   174,    76,
     118,   118,   118,   293,   118,   118,   118,   118,   581,    77,
     581,   119,   469,    66,    67,   501,    48,   661,    11,   321,
     548,    38,   119,   119,    12,   500,   118,   538,   581,   581,
     333,   334,   295,   168,   425,   426,   582,   688,   286,   581,
     499,   653,   350,   618,    68,   300,   287,   423,   189,   424,
     625,   647,   301,   190,   588,   608,   118,   539,   542,    58,
     558,   606,   540,   543,   381,   677,   662,   118,   118,    48,
     119,   540,   191,   294,   302,    49,   303,   119,   387,   212,
     213,   265,   119,   266,   502,   392,   291,   668,   583,   292,
     396,   540,   119,   545,   632,    50,   543,   553,   546,   119,
     119,    57,   119,   119,   540,   639,   664,   404,   405,    56,
     407,   408,    58,   350,    59,   118,    60,    63,    70,    80,
     353,   354,   118,    82,   293,   192,    83,   118,   359,    84,
     360,   361,   362,   123,   128,   363,   126,   118,   163,   164,
     165,   350,   168,    52,   118,   118,   184,   118,   118,   181,
     182,   183,   185,   295,   193,   186,   187,   188,   194,   207,
     221,   201,   461,   310,   265,   464,   372,   373,   374,   375,
     376,   319,   323,   346,    11,   195,   629,   630,   320,   377,
     378,   325,   326,   324,   327,   636,   196,   328,   329,   380,
     279,   353,   354,   382,   386,   397,   398,   384,   350,   359,
     399,   360,   361,   362,   189,   402,   363,   470,   650,   190,
     477,   306,   479,   495,   504,   538,   549,   351,   352,   353,
     354,   355,   541,   212,   356,   357,   358,   359,   191,   360,
     361,   362,   561,   562,   363,   568,   364,   365,   569,   570,
     211,   330,   331,   571,   119,   335,   572,   663,   574,   573,
     377,   378,   584,   339,   340,   341,   342,   343,   579,   344,
     670,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     610,   585,   615,   619,   620,   623,   353,   354,   377,   378,
     635,   192,   626,   627,   359,   637,   360,   361,   362,   118,
     686,   363,   391,   638,   643,   642,   675,   189,   680,   394,
     119,   683,   190,   692,   685,   700,   189,   701,   609,   350,
     193,   190,   695,   696,   194,   706,   307,   427,    78,   403,
     178,   191,   406,    81,   374,   375,   376,   702,   412,   412,
     191,   195,   189,   305,    79,   377,   378,   190,   349,   125,
     463,   672,   196,    19,   521,   118,   471,   472,   385,   624,
     473,   474,   475,   476,   400,   478,   191,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   644,   496,   192,   322,     0,   421,     0,     0,
       0,     0,   300,   192,     0,     0,     0,   353,   354,   301,
       0,     0,     0,     0,     0,   359,     0,   528,   361,   362,
       0,     0,   363,   193,     0,     0,   344,   194,     0,   192,
       0,   302,   193,     0,   344,     0,   194,   547,   335,     0,
       0,   550,     0,     0,   195,   551,   552,     0,   557,     0,
     308,   559,   560,   195,     0,   196,   564,   567,   193,   520,
       0,     0,   194,     0,   196,     0,   377,   378,     0,     0,
       0,     0,   119,     0,     0,     0,     0,     0,     0,   195,
     690,   401,     0,     0,     0,     0,     0,     0,   350,     0,
     196,     0,   119,     0,     0,     0,     0,     0,     0,     0,
     703,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,   589,   118,     0,   190,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,     0,   607,   118,   191,     0,
       0,     0,     0,     0,     0,   612,   613,     0,   614,     0,
       0,   617,     0,     0,     0,     0,   622,     0,   412,     0,
       0,     0,     0,     0,   351,   352,   353,   354,   355,   633,
       0,   356,   357,   358,   359,     0,   360,   361,   362,     0,
     640,   363,     0,   364,   365,     0,     0,     0,     0,     0,
       0,   192,   601,   605,     0,     0,   648,     0,     0,     0,
       0,   654,     0,     0,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,     0,     0,     0,
     193,     0,     0,     0,   194,   377,   378,     0,     0,     0,
     660,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,   195,     0,     0,   656,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,     0,     0,     0,
     673,   674,     0,     0,   676,     0,     0,     0,     0,     0,
       0,     0,   679,     0,     0,     0,   682,     0,   684,   429,
       0,   687,     0,     3,     0,   430,   431,   463,   432,     0,
     235,   236,   237,   238,     0,     0,     0,     0,     0,     0,
     433,   239,   434,   435,     0,     0,     0,   699,     0,     0,
       0,     0,   436,   240,     0,   705,   437,   241,     0,   242,
       0,     9,   243,   438,     0,   439,   244,     0,     0,   440,
     441,     0,     0,    89,    90,    91,     0,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   108,   109,   110,     0,     0,   113,   114,   115,
     116,     0,   245,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,   248,   249,   250,   251,
     252,   253,   254,   255,   256,    37,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   259,   260,     0,     0,     0,     0,   261,   262,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,    38,
     443,   265,   444,   266,   267,   429,     0,     0,     0,     3,
       0,   430,   431,     0,   432,     0,   235,   236,   237,   238,
       0,     0,     0,     0,     0,     0,   433,   239,   434,   435,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   240,
       0,     0,   437,   241,     0,   242,     0,     9,   243,   438,
       0,   439,   244,     0,     0,   440,   441,     0,     0,    89,
      90,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   108,   109,
     110,     0,     0,   113,   114,   115,   116,     0,   245,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   388,     0,     0,     0,     0,     0,
       0,     0,   248,   249,   250,   251,   252,   253,   254,   255,
     256,    37,   257,     0,     0,     0,   189,     0,     0,     0,
       0,   190,     0,     0,     0,     0,   258,   259,   260,     0,
       0,     0,     0,   261,   262,     0,     0,     0,     0,     0,
     191,     0,   264,     0,     0,    38,   443,   265,   689,   266,
     267,   235,   236,   237,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,   190,   240,     0,     0,     0,   241,     0,
     242,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,   191,   192,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,     0,   113,   114,
     115,   116,   193,   245,     0,     0,   194,     0,   309,     0,
       0,     0,     0,     0,     0,   246,   247,     0,     0,     0,
       0,     0,     0,   195,     0,   192,     0,     0,     0,     0,
       0,     0,     0,     0,   196,     0,     0,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   410,   257,     0,     0,
       0,     0,     0,     0,   193,     0,     0,     0,   194,     0,
       0,   258,   259,   260,     0,     0,     0,     0,   261,   262,
     235,   236,   237,   238,     0,   195,     0,   411,     0,     0,
      38,   239,   265,     0,   266,   267,   196,   189,     0,     0,
       0,     0,   190,   240,     0,     0,     0,   241,     0,   242,
       0,     0,   243,     0,     0,     0,   244,     0,     0,     0,
       0,   191,     0,    89,    90,    91,     0,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   108,   109,   110,     0,     0,   113,   114,   115,
     116,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,     0,
       0,     0,     0,     0,   192,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   410,   257,     0,     0,     0,
       0,     0,     0,   193,     0,     0,     0,   194,     0,     0,
     258,   259,   260,     0,     0,     0,     0,   261,   262,   235,
     236,   237,   238,     0,   195,     0,   419,     0,     0,    38,
     239,   265,     0,   266,   267,   196,     0,     0,     0,     0,
       0,     0,   240,     0,     0,     0,   241,     0,   242,     0,
       0,   243,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,    89,    90,    91,     0,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       0,   108,   109,   110,     0,     0,   113,   114,   115,   116,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,     0,     0,
       0,     0,   554,     0,     0,     0,     0,   388,     0,     0,
       0,     0,     0,     0,     0,   248,   249,   250,   251,   252,
     253,   254,   255,   256,    37,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   258,
     259,   260,   235,   236,   237,   238,   261,   262,     0,     0,
       0,     0,     0,   239,     0,   264,     0,     0,    38,   555,
     265,     0,   266,   267,     0,   240,     0,     0,     0,   241,
       0,   242,     0,     0,   243,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,    89,    90,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   108,   109,   110,     0,     0,   113,
     114,   115,   116,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   248,   249,
     250,   251,   252,   253,   254,   255,   256,    37,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,   260,     0,     0,     0,     0,   261,
     262,   235,   236,   237,   238,   526,   465,     0,   264,   466,
       0,    38,   239,   265,     0,   266,   267,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,   241,     0,
     242,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,     0,   113,   114,
     115,   116,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,   248,   249,   250,
     251,   252,   253,   254,   255,   256,    37,   257,     0,     0,
       0,     0,     0,   191,     0,     0,     0,     0,     0,   527,
       0,   258,   259,   260,   235,   236,   237,   238,   261,   262,
       0,     0,     0,     0,     0,   239,     0,   264,   189,     0,
      38,     0,   265,   190,   266,   267,     0,   240,     0,     0,
       0,   241,     0,   242,     0,     0,   243,     0,     0,     0,
     244,     0,   191,     0,     0,     0,   192,    89,    90,    91,
       0,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,     0,   108,   109,   110,     0,
       0,   113,   114,   115,   116,   193,   245,     0,     0,   194,
       0,   312,     0,     0,     0,     0,     0,     0,   246,   247,
       0,     0,     0,     0,     0,   192,   195,   565,     0,     0,
       0,     0,   388,     0,     0,     0,     0,   196,     0,     0,
     248,   249,   250,   251,   252,   253,   254,   255,   256,    37,
     257,     0,     0,     0,   193,     0,     0,     0,   194,     0,
     314,     0,     0,     0,   258,   259,   260,   235,   236,   237,
     238,   261,   262,     0,     0,   195,     0,     0,   239,     0,
     264,     0,     0,    38,     0,   265,   196,   266,   267,     0,
     240,     0,     0,     0,   241,     0,   242,     0,     0,   243,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,     0,   113,   114,   115,   116,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,     0,     0,     0,
     190,     0,     0,   248,   249,   250,   251,   252,   253,   254,
     255,   256,    37,   257,     0,     0,     0,     0,     0,   191,
       0,     0,     0,     0,     0,     0,     0,   258,   259,   260,
     235,   236,   237,   238,   261,   262,     0,     0,     0,     0,
       0,   239,   263,   264,   189,     0,    38,     0,   265,   190,
     266,   267,     0,   240,     0,     0,     0,   241,     0,   242,
       0,     0,   243,     0,     0,     0,   244,     0,   191,     0,
       0,     0,   192,    89,    90,    91,     0,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   108,   109,   110,     0,     0,   113,   114,   115,
     116,   193,   245,     0,     0,   194,     0,   316,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,     0,
       0,   192,   195,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,   196,     0,     0,   248,   249,   250,   251,
     252,   253,   254,   255,   256,    37,   257,     0,     0,     0,
     193,     0,     0,     0,   194,     0,   523,     0,     0,     0,
     258,   259,   260,   235,   236,   237,   238,   261,   262,     0,
       0,   195,     0,     0,   239,     0,   264,     0,     0,    38,
       0,   265,   196,   266,   267,     0,   240,     0,     0,     0,
     241,     0,   242,     0,     0,   243,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,    89,    90,    91,     0,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   108,   109,   110,     0,     0,
     113,   114,   115,   116,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,     0,     0,     0,   190,     0,     0,   248,
     249,   250,   251,   252,   253,   254,   255,   256,    37,   257,
       0,     0,     0,     0,     0,   191,     0,     0,     0,     0,
       0,     0,     0,   258,   259,   260,   235,   236,   237,   238,
     261,   262,     0,     0,     0,     0,     0,   239,     0,   264,
     497,     0,    38,     0,   265,     0,   266,   267,     0,   240,
       0,     0,     0,   241,     0,   242,     0,     0,   243,     0,
       0,     0,   244,     0,     0,     0,     0,     0,   192,    89,
      90,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   108,   109,
     110,     0,     0,   113,   114,   115,   116,   193,   245,     0,
       0,   194,     0,   530,     0,     0,     0,     0,     0,     0,
     246,   247,     0,     0,     0,     0,     0,     0,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   196,
       0,     0,   248,   249,   250,   251,   252,   253,   254,   255,
     256,    37,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,   260,   235,
     236,   237,   238,   261,   262,     0,     0,     0,     0,     0,
     239,     0,   264,   524,     0,    38,     0,   265,     0,   266,
     267,     0,   240,     0,     0,     0,   241,     0,   242,     0,
       0,   243,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,    89,    90,    91,     0,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       0,   108,   109,   110,     0,     0,   113,   114,   115,   116,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,     0,   190,     0,     0,   248,   249,   250,   251,   252,
     253,   254,   255,   256,    37,   257,     0,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,   258,
     259,   260,   235,   236,   237,   238,   261,   262,     0,     0,
       0,     0,     0,   239,     0,   264,   537,     0,    38,     0,
     265,     0,   266,   267,     0,   240,     0,     0,     0,   241,
       0,   242,     0,     0,   243,     0,     0,     0,   244,     0,
       0,     0,     0,     0,   192,    89,    90,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   108,   109,   110,     0,     0,   113,
     114,   115,   116,   193,   245,     0,     0,   194,     0,   531,
       0,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,     0,     0,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,   248,   249,
     250,   251,   252,   253,   254,   255,   256,    37,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,   260,   235,   236,   237,   238,   261,
     262,     0,     0,     0,     0,     0,   239,     0,   264,   544,
       0,    38,     0,   265,     0,   266,   267,     0,   240,     0,
       0,     0,   241,     0,   242,     0,     0,   243,     0,     0,
       0,   244,     0,     0,   563,     0,     0,     0,    89,    90,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   108,   109,   110,
       0,     0,   113,   114,   115,   116,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,     0,     0,     0,   190,     0,
       0,   248,   249,   250,   251,   252,   253,   254,   255,   256,
      37,   257,     0,     0,     0,     0,     0,   191,     0,     0,
       0,     0,     0,     0,     0,   258,   259,   260,   235,   236,
     237,   238,   261,   262,     0,     0,     0,     0,     0,   239,
       0,   264,   189,     0,    38,     0,   265,   190,   266,   267,
       0,   240,     0,     0,     0,   241,     0,   242,     0,     0,
     243,     0,     0,     0,   244,     0,   191,     0,     0,     0,
     192,    89,    90,    91,     0,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
     108,   109,   110,     0,     0,   113,   114,   115,   116,   193,
     245,     0,     0,   194,     0,   533,     0,     0,     0,     0,
       0,     0,   246,   247,     0,     0,     0,     0,     0,   192,
     195,     0,     0,     0,     0,     0,   645,     0,     0,     0,
       0,   196,     0,     0,   248,   249,   250,   251,   252,   253,
     254,   255,   256,    37,   257,     0,     0,     0,   193,     0,
       0,     0,   194,     0,   534,     0,     0,     0,   258,   259,
     260,   235,   236,   237,   238,   261,   262,     0,     0,   195,
       0,     0,   239,     0,   264,     0,     0,    38,     0,   265,
     196,   266,   267,     0,   240,     0,     0,     0,   241,     0,
     242,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,     0,   113,   114,
     115,   116,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   646,
       0,     0,     0,     0,     0,     0,     0,   248,   249,   250,
     251,   252,   253,   254,   255,   256,    37,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,   260,   235,   236,   237,   238,   261,   262,
       0,     0,     0,     0,     0,   239,     0,   264,   189,     0,
      38,     0,   265,   190,   266,   267,     0,   240,     0,     0,
       0,   241,     0,   242,     0,     0,   243,     0,     0,     0,
     244,     0,   191,     0,     0,     0,     0,    89,    90,    91,
       0,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,     0,   108,   109,   110,     0,
       0,   113,   114,   115,   116,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,   247,
       0,     0,     0,     0,     0,   192,     0,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,   190,     0,     0,
     248,   249,   250,   251,   252,   253,   254,   255,   256,    37,
     257,     0,     0,     0,   193,     0,   191,     0,   194,     0,
       0,     0,     0,     0,   258,   259,   260,   235,   236,   237,
     238,   261,   262,     0,     0,   195,     0,     0,   239,     0,
     264,   652,     0,    38,     0,   265,   196,   266,   267,     0,
     240,     0,     0,     0,   241,     0,   242,     0,     0,   243,
       0,     0,     0,   244,     0,     0,     0,     0,     0,   192,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,     0,   113,   114,   115,   116,   193,   245,
       0,     0,   194,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,     0,     0,     0,     0,     0,   195,
       0,     0,   697,     0,     0,     0,     0,     0,     0,     0,
     196,     0,     0,   248,   249,   250,   251,   252,   253,   254,
     255,   256,    37,   257,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,   260,
       0,     0,     0,     0,   261,   262,   235,   236,   237,   238,
     678,     0,     0,   264,   659,     0,    38,   239,   265,     0,
     266,   267,     0,     0,     0,     0,     0,     0,     0,   240,
       0,     0,     0,   241,     0,   242,     0,     0,   243,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,    89,
      90,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   108,   109,
     110,     0,     0,   113,   114,   115,   116,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248,   249,   250,   251,   252,   253,   254,   255,
     256,    37,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,   260,     0,
       0,     0,     0,   261,   262,   235,   236,   237,   238,   698,
       0,     0,   264,     0,     0,    38,   239,   265,     0,   266,
     267,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,   241,     0,   242,     0,     0,   243,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,    89,    90,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   108,   109,   110,
       0,     0,   113,   114,   115,   116,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,     0,     0,     0,   190,     0,
       0,   248,   249,   250,   251,   252,   253,   254,   255,   256,
      37,   257,     0,     0,     0,     0,     0,   191,     0,     0,
       0,     0,     0,     0,     0,   258,   259,   260,   235,   236,
     237,   238,   261,   262,     0,     0,     0,     0,     0,   239,
       0,   264,     0,     0,    38,     0,   265,     0,   266,   267,
       0,   240,     0,     0,     0,   241,     0,   242,     0,     0,
     243,     0,     0,     0,   244,     0,     0,     0,     0,     0,
     192,    89,    90,    91,     0,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,     0,
     108,   109,   110,     0,     0,   113,   114,   115,   116,   193,
     245,     0,     0,   194,     0,     0,     0,     0,     0,     0,
       0,     0,   246,   247,     0,   350,     0,     0,     0,     0,
     195,     0,     0,   707,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,   248,   249,   250,   251,   252,   253,
     254,   255,   256,    37,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
     260,     0,     0,     0,     0,   261,   262,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,    38,     0,   265,
       0,   266,   267,   505,   506,   507,   508,   509,   510,   511,
     512,   351,   352,   353,   354,   355,   513,   514,   356,   357,
     358,   359,   515,   360,   361,   362,   350,     0,   363,   516,
     364,   365,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   519,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,     0,     0,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   505,   506,   507,   508,   509,   510,
     511,   512,   351,   352,   353,   354,   355,   513,   514,   356,
     357,   358,   359,   515,   360,   361,   362,   350,     0,   363,
     516,   364,   365,   517,   518,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   519,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,   377,   378,     0,     0,     0,     0,     0,
     634,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,   506,   507,   508,   509,
     510,   511,   512,   351,   352,   353,   354,   355,   513,   514,
     356,   357,   358,   359,   515,   360,   361,   362,   350,     0,
     363,   516,   364,   365,   517,   518,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   519,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   378,     0,     0,     0,     0,
       0,   641,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,   506,   507,   508,
     509,   510,   511,   512,   351,   352,   353,   354,   355,   513,
     514,   356,   357,   358,   359,   515,   360,   361,   362,   350,
       0,   363,   516,   364,   365,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   378,     0,     0,     0,
       0,     0,   655,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   505,   506,   507,
     508,   509,   510,   511,   512,   351,   352,   353,   354,   355,
     513,   514,   356,   357,   358,   359,   515,   360,   361,   362,
     350,     0,   363,   516,   364,   365,   517,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   519,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   378,     0,     0,
       0,     0,     0,   669,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,   505,   506,
     507,   508,   509,   510,   511,   512,   351,   352,   353,   354,
     355,   513,   514,   356,   357,   358,   359,   515,   360,   361,
     362,     0,     0,   363,   516,   364,   365,   517,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   519,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,   350,     0,     0,     0,     0,   377,   378,   351,
     352,   353,   354,   355,   671,     0,   356,   357,   358,   359,
       0,   360,   361,   362,     0,     0,   363,     0,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,     0,     0,     0,   350,     0,     0,     0,     0,
     377,   378,     0,     0,     0,     0,     0,   395,   351,   352,
     353,   354,   355,     0,     0,   356,   357,   358,   359,     0,
     360,   361,   362,     0,     0,   363,     0,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   350,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   351,   352,   353,   354,   355,   693,     0,   356,   357,
     358,   359,     0,   360,   361,   362,     0,     0,   363,     0,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,     0,     0,     0,   350,     0,     0,
       0,     0,   377,   378,     0,     0,   428,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,     0,   363,     0,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
       0,     0,     0,   350,     0,     0,     0,     0,   377,   378,
       0,     0,   529,   351,   352,   353,   354,   355,     0,     0,
     356,   357,   358,   359,     0,   360,   361,   362,     0,     0,
     363,     0,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,     0,     0,     0,   350,
       0,     0,     0,     0,   377,   378,     0,     0,   532,   351,
     352,   353,   354,   355,     0,     0,   356,   357,   358,   359,
       0,   360,   361,   362,     0,     0,   363,     0,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,     0,     0,     0,   350,     0,     0,     0,     0,
     377,   378,     0,     0,   611,   351,   352,   353,   354,   355,
       0,     0,   356,   357,   358,   359,     0,   360,   361,   362,
       0,     0,   363,     0,   364,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,     0,     0,
       0,   350,     0,     0,     0,     0,   377,   378,     0,     0,
     681,   351,   352,   353,   354,   355,     0,     0,   356,   357,
     358,   359,     0,   360,   361,   362,     0,     0,   363,     0,
     364,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,     0,     0,   691,   351,   352,   353,
     354,   355,     0,     0,   356,   357,   358,   359,     0,   360,
     361,   362,     0,     0,   363,     0,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,    87,     0,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
      88,     0,     0,     0,     0,     0,     0,     0,   377,   378,
       0,     0,   704,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,   248,   249,   250,     0,
       0,     0,     0,     0,     0,    37,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,   350,     0,    38,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,   165,
     350,     0,    38,   351,   352,   353,   354,   355,     0,     0,
     356,   357,   358,   359,     0,   360,   361,   362,     0,     0,
     363,     0,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,     0,     0,     0,
       0,     0,   350,     0,     0,    38,   351,   352,   353,   354,
     355,     0,     0,   356,   357,   358,   359,     0,   360,   361,
     362,     0,     0,   363,     0,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   587,
     505,   506,   507,   508,   509,   510,   511,   512,   351,   352,
     353,   354,   355,   513,   514,   356,   357,   358,   359,   515,
     360,   361,   362,  -187,   350,   363,   516,   364,   365,   517,
     518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   519,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,   508,   509,   510,   511,   512,
     351,   352,   353,   354,   355,   513,   514,   356,   357,   358,
     359,   577,   360,   361,   362,     0,     0,   363,   516,   364,
     365,   517,   518,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
     578,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,     0,     0,     0,   351,   352,   353,   354,
     355,   377,   378,   356,   357,   358,   359,     0,   360,   361,
     362,     0,     0,   363,     0,   364,   365,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,     0,
       0,     0,   351,   352,   353,   354,   355,   377,   378,   356,
     357,   358,   359,     0,   360,   361,   362,     0,     0,   363,
       0,   364,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,   366,   586,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,     0,     0,     0,   351,   352,
     353,   354,   355,   377,   378,   356,   357,   358,   359,     0,
     360,   361,   362,     0,     0,   363,     0,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,   628,     0,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,     0,     0,     0,   351,   352,   353,   354,   355,   377,
     378,   356,   357,   358,   359,     0,   360,   361,   362,     0,
       0,   363,     0,   364,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,     0,     0,     0,
     351,   352,   353,   354,   355,   377,   378,   356,   357,   358,
     359,     0,   360,   361,   362,     0,     0,   363,     0,   364,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,   369,   370,   371,   372,   373,
     374,   375,   376,     0,     0,     0,   351,   352,   353,   354,
     355,   377,   378,   356,   357,   358,   359,     0,   360,   361,
     362,     0,     0,   363,     0,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   371,   372,   373,   374,   375,   376,     0,
     350,     0,   351,   352,     0,     0,   355,   377,   378,   356,
     357,   358,   359,     0,   360,   361,   362,     0,     0,   363,
       0,   364,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   367,   368,   369,   370,   371,
       0,     0,     0,   375,   376,   351,   352,   353,   354,   355,
       0,     0,   356,   377,   378,   359,     0,   360,   361,   362,
       0,     0,   363,     0,   364,   365,   351,   352,   353,   354,
       0,     0,     0,     0,     0,     0,   359,     0,   360,   361,
     362,     0,     0,   363,     0,   364,   365,     0,     0,     0,
       0,   370,   371,   372,   373,   374,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   378,     0,     0,
       0,     0,     0,     0,   372,   373,   374,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   148,   149,   150,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
       0,     0,   161,   162
};

static const yytype_int16 yycheck[] =
{
      11,    57,   167,   208,   206,   345,   380,   297,    60,    57,
     334,   222,   200,   224,   304,   226,   105,   250,   185,   186,
     187,   109,   117,    30,    19,    17,    29,   401,    17,   388,
      43,   127,     0,    12,    13,   127,   127,     5,   105,    43,
      10,   465,   127,   127,   133,   134,    57,   136,   134,   121,
      61,    21,   133,    12,    13,    50,   133,    27,    26,   133,
      28,   127,    30,   141,    32,   161,   158,   134,   163,    37,
     104,    54,    42,   107,   162,    43,   162,   127,   159,   164,
     164,    49,   159,   157,   162,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,   127,   126,   540,     9,    31,   141,
     127,   155,   156,   127,   134,   181,   182,   183,    20,   185,
     186,   187,   188,   181,   182,   183,   139,   185,   186,   187,
     162,   133,   121,   433,   133,   504,   125,   127,   127,   128,
     145,   207,   442,   156,   161,   159,   127,   127,   133,   207,
     155,   535,   121,     4,   167,     6,   125,   332,   127,   128,
     181,   182,   183,   105,   185,   186,   187,   188,   133,   158,
     133,   237,   349,    96,    97,   387,   133,   162,   156,   237,
     423,   161,   248,   249,   162,   383,   207,   133,   133,   133,
     248,   249,   134,   129,   130,   131,   159,   162,   156,   133,
     157,   585,    18,   537,   127,   105,   164,   162,    29,   164,
     544,   157,   112,    34,   159,   159,   237,   157,   157,   161,
     435,   521,   162,   162,   286,   159,   157,   248,   249,   133,
     296,   162,    53,   133,   134,   136,   136,   303,   296,   129,
     130,   163,   308,   165,   166,   303,   159,   631,   469,   162,
     308,   162,   318,   164,   554,   133,   162,   432,   164,   325,
     326,   134,   328,   329,   162,   565,   164,   325,   326,   163,
     328,   329,   161,    18,   127,   296,   158,   127,   163,   121,
      96,    97,   303,   127,   105,   106,   127,   308,   104,   127,
     106,   107,   108,   127,   163,   111,   136,   318,    31,    31,
     158,    18,   129,    50,   325,   326,   158,   328,   329,   140,
     140,   140,   140,   134,   135,   140,   140,   140,   139,   136,
     127,   127,   672,   159,   163,   346,   142,   143,   144,   145,
     146,   139,   158,   166,   156,   156,   551,   552,   157,   155,
     156,   140,   140,   158,   158,   560,   167,   140,   140,   158,
     195,    96,    97,    40,   127,   141,   141,   162,    18,   104,
     141,   106,   107,   108,    29,   127,   111,    16,   583,    34,
     127,   216,   127,   127,   109,   133,   164,    94,    95,    96,
      97,    98,   133,   129,   101,   102,   103,   104,    53,   106,
     107,   108,   162,   121,   111,    33,   113,   114,   162,   162,
     575,   246,   247,   162,   470,   250,   162,   619,    17,   162,
     155,   156,   470,   258,   259,   260,   261,   262,   127,   264,
     635,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     127,   158,   158,   127,   157,   157,    96,    97,   155,   156,
      11,   106,   157,   157,   104,   136,   106,   107,   108,   470,
     665,   111,   297,   121,    36,   163,   159,    29,   127,   304,
     526,   157,    34,   141,   157,    46,    29,   157,   526,    18,
     135,    34,   687,   688,   139,   157,   141,   338,    47,   324,
      82,    53,   327,    50,   144,   145,   146,   695,   333,   334,
      53,   156,    29,   214,    48,   155,   156,    34,   274,    61,
     345,   642,   167,     1,   392,   526,   351,   352,   292,   543,
     355,   356,   357,   358,   318,   360,    53,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   575,   378,   106,   237,    -1,   334,    -1,    -1,
      -1,    -1,   105,   106,    -1,    -1,    -1,    96,    97,   112,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   402,   107,   108,
      -1,    -1,   111,   135,    -1,    -1,   411,   139,    -1,   106,
      -1,   134,   135,    -1,   419,    -1,   139,   422,   423,    -1,
      -1,   426,    -1,    -1,   156,   430,   431,    -1,   433,    -1,
     162,   436,   437,   156,    -1,   167,   441,   442,   135,   162,
      -1,    -1,   139,    -1,   167,    -1,   155,   156,    -1,    -1,
      -1,    -1,   678,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     678,   158,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
     167,    -1,   698,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     698,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,   501,   678,    -1,    34,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,    -1,   521,   698,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   530,   531,    -1,   533,    -1,
      -1,   536,    -1,    -1,    -1,    -1,   541,    -1,   543,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,   554,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
     565,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   577,   578,    -1,    -1,   581,    -1,    -1,    -1,
      -1,   586,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
     135,    -1,    -1,    -1,   139,   155,   156,    -1,    -1,    -1,
     615,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     645,   646,    -1,    -1,   649,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   657,    -1,    -1,    -1,   661,    -1,   663,     1,
      -1,   666,    -1,     5,    -1,     7,     8,   672,    10,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    -1,    -1,    -1,   692,    -1,    -1,
      -1,    -1,    34,    35,    -1,   700,    38,    39,    -1,    41,
      -1,    43,    44,    45,    -1,    47,    48,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,   165,   166,     1,    -1,    -1,    -1,     5,
      -1,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    45,
      -1,    47,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,   150,    -1,    -1,    -1,    -1,    -1,
      53,    -1,   158,    -1,    -1,   161,   162,   163,   164,   165,
     166,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    53,   106,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,   135,    84,    -1,    -1,   139,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,    -1,   149,   150,
      12,    13,    14,    15,    -1,   156,    -1,   158,    -1,    -1,
     161,    23,   163,    -1,   165,   166,   167,    29,    -1,    -1,
      -1,    -1,    34,    35,    -1,    -1,    -1,    39,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    53,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,   150,    12,
      13,    14,    15,    -1,   156,    -1,   158,    -1,    -1,   161,
      23,   163,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,    12,    13,    14,    15,   149,   150,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,   165,   166,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
     150,    12,    13,    14,    15,    16,   156,    -1,   158,   159,
      -1,   161,    23,   163,    -1,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,   144,    12,    13,    14,    15,   149,   150,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   158,    29,    -1,
     161,    -1,   163,    34,   165,   166,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    53,    -1,    -1,    -1,   106,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,   135,    84,    -1,    -1,   139,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,   106,   156,   105,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,
     141,    -1,    -1,    -1,   142,   143,   144,    12,    13,    14,
      15,   149,   150,    -1,    -1,   156,    -1,    -1,    23,    -1,
     158,    -1,    -1,   161,    -1,   163,   167,   165,   166,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
      12,    13,    14,    15,   149,   150,    -1,    -1,    -1,    -1,
      -1,    23,   157,   158,    29,    -1,   161,    -1,   163,    34,
     165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    53,    -1,
      -1,    -1,   106,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,   135,    84,    -1,    -1,   139,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,   106,   156,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,   167,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
     135,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,    -1,
     142,   143,   144,    12,    13,    14,    15,   149,   150,    -1,
      -1,   156,    -1,    -1,    23,    -1,   158,    -1,    -1,   161,
      -1,   163,   167,   165,   166,    -1,    35,    -1,    -1,    -1,
      39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,    12,    13,    14,    15,
     149,   150,    -1,    -1,    -1,    -1,    -1,    23,    -1,   158,
     159,    -1,   161,    -1,   163,    -1,   165,   166,    -1,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   106,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,   135,    84,    -1,
      -1,   139,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,    12,
      13,    14,    15,   149,   150,    -1,    -1,    -1,    -1,    -1,
      23,    -1,   158,   159,    -1,   161,    -1,   163,    -1,   165,
     166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    34,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,    12,    13,    14,    15,   149,   150,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   158,   159,    -1,   161,    -1,
     163,    -1,   165,   166,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   106,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,   135,    84,    -1,    -1,   139,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,    12,    13,    14,    15,   149,
     150,    -1,    -1,    -1,    -1,    -1,    23,    -1,   158,   159,
      -1,   161,    -1,   163,    -1,   165,   166,    -1,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,
      14,    15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,
      -1,   158,    29,    -1,   161,    -1,   163,    34,   165,   166,
      -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    53,    -1,    -1,    -1,
     106,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,   135,
      84,    -1,    -1,   139,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   106,
     156,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,   167,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,   135,    -1,
      -1,    -1,   139,    -1,   141,    -1,    -1,    -1,   142,   143,
     144,    12,    13,    14,    15,   149,   150,    -1,    -1,   156,
      -1,    -1,    23,    -1,   158,    -1,    -1,   161,    -1,   163,
     167,   165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,    12,    13,    14,    15,   149,   150,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   158,    29,    -1,
     161,    -1,   163,    34,   165,   166,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    53,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,   135,    -1,    53,    -1,   139,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,    14,
      15,   149,   150,    -1,    -1,   156,    -1,    -1,    23,    -1,
     158,   159,    -1,   161,    -1,   163,   167,   165,   166,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   106,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,   135,    84,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,   150,    12,    13,    14,    15,
      16,    -1,    -1,   158,   159,    -1,   161,    23,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,   150,    12,    13,    14,    15,    16,
      -1,    -1,   158,    -1,    -1,   161,    23,   163,    -1,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,
      14,    15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,
      -1,   158,    -1,    -1,   161,    -1,   163,    -1,   165,   166,
      -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
     106,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,   135,
      84,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    18,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,    -1,   163,
      -1,   165,   166,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    18,    -1,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    18,    -1,   111,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    18,    -1,
     111,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    18,
      -1,   111,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
      18,    -1,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    -1,    -1,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   155,   156,    94,
      95,    96,    97,    98,   162,    -1,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,   162,    94,    95,
      96,    97,    98,    -1,    -1,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    94,    95,    96,    97,    98,   162,    -1,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,   159,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,
      95,    96,    97,    98,    -1,    -1,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,   159,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
     159,    94,    95,    96,    97,    98,    -1,    -1,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,   159,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    18,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,   158,
      18,    -1,   161,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,   161,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    18,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    94,    95,    96,    97,
      98,   155,   156,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    94,    95,    96,    97,    98,   155,   156,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    94,    95,
      96,    97,    98,   155,   156,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,   132,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    94,    95,    96,    97,    98,   155,
     156,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      94,    95,    96,    97,    98,   155,   156,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    94,    95,    96,    97,
      98,   155,   156,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,    -1,
      18,    -1,    94,    95,    -1,    -1,    98,   155,   156,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,   145,   146,    94,    95,    96,    97,    98,
      -1,    -1,   101,   155,   156,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,   113,   114,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,   149,   150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   169,     0,     5,    26,    28,    30,    32,    37,    43,
      49,   156,   162,   170,   175,   176,   180,   197,   201,   215,
     243,   245,   246,   249,   127,   192,   193,   121,   181,   182,
     127,   177,   178,   127,   127,   127,    30,   127,   161,   194,
     195,   196,   206,     4,     6,    54,   242,   134,   133,   136,
     133,    19,    50,   145,   155,   179,   163,   134,   161,   127,
     158,   133,   157,   127,   248,    31,    96,    97,   127,   200,
     163,    12,    13,   121,   125,   127,   128,   158,   172,   192,
     121,   182,   127,   127,   127,   244,    21,    27,    42,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   206,   251,
     252,   253,   254,   127,   193,   195,   136,   247,   163,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   113,   114,   115,
     116,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   149,   150,    31,    31,   158,   198,   241,   129,   171,
      12,    13,   121,   125,   127,   172,   190,   191,   179,   127,
     164,   140,   140,   140,   158,   140,   140,   140,   140,    29,
      34,    53,   106,   135,   139,   156,   167,   159,   206,   232,
      43,   127,   159,   233,   234,   238,   250,   136,   199,   164,
     240,   250,   129,   130,   133,   159,   134,   162,   254,   254,
     254,   127,   198,   254,   198,   254,   198,   254,   235,   236,
     254,    29,   139,   156,   167,    12,    13,    14,    15,    23,
      35,    39,    41,    44,    48,    84,    96,    97,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   128,   142,   143,
     144,   149,   150,   157,   158,   163,   165,   166,   174,   202,
     206,   208,   217,   218,   221,   222,   223,   226,   227,   228,
     251,   255,   258,   263,   265,   267,   156,   164,   229,   231,
     238,   159,   162,   105,   133,   134,   136,   237,   254,   202,
     105,   112,   134,   136,   239,   190,   228,   141,   162,   141,
     159,   199,   141,   199,   141,   199,   141,   141,   162,   139,
     157,   254,   255,   158,   158,   140,   140,   158,   140,   140,
     228,   228,    10,   254,   254,   228,   260,   264,   173,   228,
     228,   228,   228,   228,   228,   204,   166,   127,   158,   197,
      18,    94,    95,    96,    97,    98,   101,   102,   103,   104,
     106,   107,   108,   111,   113,   114,   135,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   155,   156,   157,
     158,   193,    40,   230,   162,   233,   127,   254,   110,   205,
     224,   228,   254,   205,   228,   162,   254,   141,   141,   141,
     235,   158,   127,   228,   254,   254,   228,   254,   254,   250,
     127,   158,   228,   256,   257,   259,   260,   261,   262,   158,
     257,   262,   117,   162,   164,   130,   131,   171,   159,     1,
       7,     8,    10,    22,    24,    25,    34,    38,    45,    47,
      51,    52,    85,   162,   164,   183,   184,   186,   187,   188,
     189,   203,   205,   207,   209,   210,   211,   212,   213,   215,
     216,   224,   225,   228,   206,   156,   159,   219,   228,   198,
      16,   228,   228,   228,   228,   228,   228,   127,   228,   127,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   127,   228,   159,   219,   157,
     238,   237,   166,   222,   109,    86,    87,    88,    89,    90,
      91,    92,    93,    99,   100,   105,   112,   115,   116,   134,
     162,   239,   162,   141,   159,   219,    16,   140,   228,   159,
     141,   141,   159,   141,   141,    17,   237,   159,   133,   157,
     162,   133,   157,   162,   159,   164,   164,   228,   260,   164,
     228,   228,   228,   250,   105,   162,   205,   228,   202,   228,
     228,   162,   121,    51,   228,   105,   205,   228,    33,   162,
     162,   162,   162,   162,    17,   214,   162,   105,   134,   127,
     256,   133,   159,   199,   254,   158,   136,   157,   159,   228,
     222,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   205,   228,   159,   254,
     127,   159,   228,   228,   228,   158,   219,   228,   257,   127,
     157,   256,   228,   157,   261,   257,   157,   157,   132,   202,
     202,    17,   205,   228,   162,    11,   202,   136,   121,   205,
     228,   162,   163,    36,   240,   110,   110,   157,   228,   117,
     202,   220,   159,   219,   228,   162,   159,   141,   162,   159,
     228,   162,   157,   237,   164,     9,    20,   185,   219,   162,
     202,   162,   204,   228,   228,   159,   228,   159,    16,   228,
     127,   159,   228,   157,   228,   157,   202,   228,   162,   164,
     254,   159,   141,   162,   266,   202,   202,   159,    16,   228,
      46,   157,   185,   254,   159,   228,   157,   159
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   168,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   170,   171,   171,   172,   173,   173,   173,
     174,   175,   176,   177,   177,   177,   178,   178,   179,   179,
     180,   181,   181,   182,   182,   183,   184,   184,   185,   185,
     185,   186,   187,   188,   189,   190,   190,   190,   190,   190,
     190,   191,   191,   192,   192,   192,   192,   192,   192,   192,
     192,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     197,   198,   198,   198,   199,   199,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   201,   202,   202,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   204,   204,   204,   205,   205,   206,
     206,   206,   207,   208,   208,   208,   208,   209,   210,   211,
     211,   211,   211,   211,   212,   212,   212,   212,   213,   214,
     214,   215,   215,   216,   217,   217,   217,   218,   218,   218,
     218,   218,   218,   219,   219,   220,   220,   221,   221,   222,
     222,   223,   223,   223,   223,   223,   223,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   225,   226,   227,   227,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   230,
     230,   231,   232,   232,   233,   233,   234,   234,   235,   236,
     236,   237,   237,   238,   238,   238,   238,   238,   239,   239,
     239,   240,   240,   240,   240,   240,   241,   241,   242,   242,
     243,   244,   244,   244,   245,   246,   247,   247,   248,   249,
     250,   250,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   252,   253,   253,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   255,   255,
     255,   255,   256,   256,   257,   257,   258,   258,   258,   258,
     259,   259,   260,   261,   261,   262,   262,   263,   263,   264,
     264,   265,   266,   266,   267
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     0,     2,
       4,     4,     6,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       5,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     3,     7,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     2,     1,
       3,     2,     2,     2,     4,     5,     2,     1,     1,     2,
       3,     4,     2,     3,     3,     4,     2,     3,     4,     0,
       2,     1,     1,     3,     5,     5,     5,     5,     8,    10,
       6,     9,    11,     1,     3,     1,     2,     1,     1,     1,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     6,     5,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     3,     3,     3,     3,     4,     3,     4,
       2,     4,     4,     6,     7,     3,     5,     4,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     0,     3,     0,
       1,     3,     0,     3,     1,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     3,     5,     3,     3,     1,     1,
       1,     4,     6,     5,     4,     3,     0,     2,     0,     1,
       5,     0,     3,     5,     4,     5,     0,     2,     2,     6,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       1,     1,     4,     3,     4,     2,     3,     2,     4,     3,
       2,     2,     3,     2,     2,     4,     6,     4,     1,     4,
       5,     1,     4,     5,     1,     4,     5,     4,     1,     1,
       1,     1,     3,     5,     1,     3,     5,     7,     5,     7,
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
          case 127: /* "name"  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2909 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2915 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2921 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2927 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2933 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2939 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2945 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2951 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2957 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2963 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2969 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2975 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2981 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2987 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2993 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2999 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3005 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3011 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3017 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3023 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3029 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3035 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3041 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3047 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3053 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3059 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3065 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3071 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3077 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3083 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3089 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3095 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3101 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3107 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3113 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3119 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3125 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3131 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3137 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3143 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3149 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3155 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3161 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3167 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3173 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3179 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3185 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3191 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3197 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3203 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3209 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3215 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3221 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3227 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3233 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3239 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3245 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3251 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3257 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3263 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3269 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3275 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3281 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3287 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3293 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3299 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3305 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3311 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3317 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3323 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3329 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3335 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3341 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3347 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3353 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3359 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3365 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 385 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 397 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 402 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 406 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 410 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 419 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3712 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 427 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3730 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 443 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 468 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 471 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 477 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 486 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-1].s), g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            if ( !info.importName.empty() ) {
                auto ita = das_module_alias.find(info.importName);
                if ( ita !=das_module_alias.end() ) {
                    if ( ita->second != info.moduleName ) {
                        das_yyerror("module alias already used " + info.importName + " as " + ita->second,tokAt((yylsp[-1])),
                            CompilationError::module_not_found);
                    }
                } else {
                    das_module_alias[info.importName] = info.moduleName;
                }
            }
        } else {
            das_yyerror("required module not found " + *(yyvsp[-1].s),tokAt((yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 3809 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 507 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto info = g_Access->getModuleInfo(*(yyvsp[-3].s), g_FileAccessStack.back()->name);
        if ( auto mod = g_Program->addModule(info.moduleName) ) {
            g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            auto ita = das_module_alias.find(*(yyvsp[-1].s));
            if ( ita !=das_module_alias.end() ) {
                if ( ita->second != info.moduleName ) {
                    das_yyerror("module alias already used " + *(yyvsp[-1].s) + " as " + ita->second,tokAt((yylsp[-3])),
                        CompilationError::module_not_found);
                }
            } else {
                das_module_alias[*(yyvsp[-1].s)] = info.moduleName;
            }
        } else {
            das_yyerror("required module not found " + *(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-3].s);
        delete (yyvsp[-1].s);
    }
#line 3834 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3840 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 544 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 547 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3862 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 553 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 559 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 562 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 568 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 569 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 596 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 615 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3959 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3977 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 619 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 620 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 624 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 629 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4039 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4051 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 650 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4095 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 679 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 680 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 684 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 691 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 703 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4151 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4157 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4163 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4169 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4190 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4208 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4214 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4226 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4238 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4244 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4256 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4262 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4274 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 849 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 852 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 862 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 872 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 883 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4651 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 910 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4675 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 916 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto ita = das_module_alias.find(*(yyvsp[-2].s));
            if ( ita == das_module_alias.end() ) {
                *(yyvsp[-2].s) += "::";
            } else {
                *(yyvsp[-2].s) = ita->second + "::";
            }
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 4697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 928 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 932 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4711 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 938 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 941 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 944 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4744 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 954 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 958 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4756 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 962 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 965 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 968 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 973 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4790 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 984 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4808 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 987 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 992 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4844 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1010 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1014 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4862 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4868 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1019 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
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
#line 4904 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1053 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1056 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4922 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1061 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1069 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1073 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1088 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4981 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1093 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1102 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1111 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1122 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1123 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1127 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
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
#line 5101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1184 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1185 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5143 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5149 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5155 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5161 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5167 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5173 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1199 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5179 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1201 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5191 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5197 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5209 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5239 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5297 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5303 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5309 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1248 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1249 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5333 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1252 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5339 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5351 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1260 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1261 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1262 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1264 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1265 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1266 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1267 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1268 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1271 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1272 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1273 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1276 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1280 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1281 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5516 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1285 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1289 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1292 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1296 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1297 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1309 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1341 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1380 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1385 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5737 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1393 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1403 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1404 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1408 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1409 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5778 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1413 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5786 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1419 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5798 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1424 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5804 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1425 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1429 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5820 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1434 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1437 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1441 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1447 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1456 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1458 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1462 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1465 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1470 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1475 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5929 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1495 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1507 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1537 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1540 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6012 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1549 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1561 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1573 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1587 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1588 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1592 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1631 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1697 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1704 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1723 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1724 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6272 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1726 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6284 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1727 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6290 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1728 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1729 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6302 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1730 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1731 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6314 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1732 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6320 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1733 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6326 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6332 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1735 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6338 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6344 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1740 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1751 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1755 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6396 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1768 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1788 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1792 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1797 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1802 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1814 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6487 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1819 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6505 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1827 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1832 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1837 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6537 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1844 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1849 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1869 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1878 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1882 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1896 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1900 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1905 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6678 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1923 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6684 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1924 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6690 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1925 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6696 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1926 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6702 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1930 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1937 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6725 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1946 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1951 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6744 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1958 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1976 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6790 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1987 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6798 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1990 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6814 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 2004 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6825 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 2013 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 2014 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2018 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2023 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2030 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2035 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2046 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6889 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2051 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2058 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2070 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6918 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2075 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6942 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6946 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2090 "ds_parser.ypp" /* yacc.c:1903  */


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


