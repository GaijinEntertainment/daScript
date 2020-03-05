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
#define YYLAST   6270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  102
/* YYNRULES -- Number of rules.  */
#define YYNRULES  408
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  741

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
       0,   372,   372,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   387,   399,   400,   404,   408,   412,   421,
     429,   445,   466,   470,   473,   479,   488,   509,   532,   533,
     537,   541,   542,   546,   549,   555,   561,   564,   570,   571,
     572,   579,   586,   598,   608,   617,   618,   619,   620,   621,
     622,   626,   631,   639,   640,   641,   642,   643,   644,   645,
     646,   652,   665,   681,   682,   686,   693,   705,   709,   716,
     717,   721,   722,   723,   727,   731,   738,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,   760,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   781,   851,   854,   864,   865,   866,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   879,   880,   881,   885,   889,   895,   901,   912,   918,
     919,   930,   934,   940,   943,   946,   950,   956,   960,   964,
     967,   970,   975,   978,   986,   989,   994,   997,  1005,  1011,
    1012,  1016,  1017,  1021,  1055,  1058,  1063,  1071,  1075,  1080,
    1086,  1090,  1095,  1104,  1107,  1113,  1114,  1121,  1132,  1133,
    1137,  1142,  1194,  1195,  1196,  1197,  1198,  1199,  1203,  1204,
    1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,
    1215,  1216,  1217,  1218,  1222,  1223,  1227,  1238,  1243,  1253,
    1257,  1264,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,
    1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,
    1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1320,  1324,  1327,  1330,  1331,  1332,  1333,  1338,  1343,
    1344,  1347,  1350,  1351,  1352,  1353,  1354,  1355,  1356,  1366,
    1376,  1410,  1411,  1415,  1416,  1420,  1428,  1431,  1438,  1439,
    1443,  1444,  1448,  1454,  1455,  1459,  1460,  1464,  1469,  1472,
    1476,  1482,  1491,  1492,  1493,  1497,  1500,  1505,  1510,  1517,
    1527,  1530,  1537,  1538,  1542,  1572,  1575,  1584,  1596,  1608,
    1619,  1634,  1635,  1639,  1678,  1744,  1751,  1760,  1761,  1762,
    1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,
    1773,  1774,  1775,  1776,  1777,  1778,  1779,  1780,  1781,  1782,
    1783,  1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1798,
    1809,  1813,  1823,  1824,  1825,  1826,  1840,  1846,  1850,  1855,
    1860,  1865,  1872,  1877,  1881,  1885,  1890,  1895,  1902,  1907,
    1913,  1918,  1922,  1927,  1936,  1940,  1945,  1954,  1958,  1963,
    1972,  1981,  1982,  1983,  1984,  1988,  1995,  2004,  2009,  2016,
    2021,  2027,  2034,  2045,  2048,  2062,  2071,  2072,  2076,  2081,
    2088,  2093,  2104,  2109,  2116,  2128,  2129,  2133,  2147
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
  "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_expr", "expr", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "enum_list", "alias_declaration",
  "enum_declaration", "optional_structure_parent", "structure_name",
  "structure_declaration", "variable_name_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,    44,    61,    63,    58,   124,    94,    38,
      60,    62,    45,    43,    42,    47,    37,   388,   389,   126,
      33,   390,   391,   392,   393,    46,    91,    93,    40,    41,
     394,   395,    59,   123,   125,    36,    64,    35
};
# endif

#define YYPACT_NINF -429

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-429)))

#define YYTABLE_NINF -189

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -429,    34,  -429,  -429,   -92,   -39,   -40,    -4,    -1,  -429,
       3,    -7,  -429,  -429,  -429,  -429,  -429,   204,  -429,    -9,
    -429,  -429,  -429,  -429,    83,  -429,    35,    41,    38,  -429,
    -429,    25,  -429,    63,   120,  -429,  -429,    53,   128,   119,
    -429,    76,  -429,   160,   135,  -429,   141,    37,   -92,   175,
     -39,   187,  -429,   190,   195,  -429,   524,  -429,  5338,   197,
    -429,   -92,    -7,  -429,   194,   170,  6120,   305,   306,  -429,
     180,  -429,  -429,  -429,  -429,  -429,  -429,   210,   115,  -429,
    -429,  -429,  -429,   290,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,   184,   -99,   203,   208,   211,  -429,
    -429,  -429,   201,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,   220,  -429,  -429,
    -429,   222,   223,  -429,  -429,  -429,  -429,   224,  -429,  -429,
    -429,  -429,   617,  -429,    43,  -429,   -75,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,    -6,   229,   -95,  -429,     4,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,    80,  -429,  -429,
      39,  -429,  5338,  5338,  5338,   239,  5265,  5265,  5265,  5338,
    -429,    11,  -429,  -429,  -429,  -429,  2013,  -429,  -429,  -429,
     -32,   243,  -429,  -429,  -429,   -78,  -429,   111,  5338,   209,
    -429,  -429,    86,  -429,  -429,   115,  -429,   -94,  3887,  -429,
     122,   480,  1145,   214,   229,  1828,   229,  1877,   229,  2094,
    -429,   -73,   617,   232,  -429,   217,  -429,  -429,  -429,  5192,
     218,  -429,   219,   235,   241,   221,   245,   247,  3887,  3887,
      36,   755,  3887,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    3887,  3887,  3887,  3887,  3887,  -429,  3887,  -429,  -429,   225,
    -429,  -429,   -97,  -429,  -429,  -429,   233,  -429,  -429,  -429,
    -429,  -429,  5333,   230,  -429,  -429,  -429,  -429,  -429,   -92,
    -429,   338,   234,  -429,  -429,    -5,  -429,   265,  -429,  5338,
    2146,   617,  -429,  -429,  -429,  -429,  5338,  2146,  -429,  -429,
    4559,  -429,  5338,  -429,  -429,   256,  -429,   258,  -429,   259,
    -429,  -429,  5338,  -429,  -429,  2143,  -429,   274,  3887,  5338,
    5338,  3887,  5338,  5338,   570,   570,   243,  1197,   243,  1336,
    5626,  -429,   106,   172,   570,   570,  -429,   570,   570,  4691,
     876,   -85,  -429,  1608,   180,   388,  3887,  3887,  -429,  -429,
    3887,  3887,  3887,  3887,   280,  3887,   281,  3887,  3887,  3887,
    3887,  3887,  3887,  3887,  3887,  3887,  3887,  3887,  3887,  3887,
    3887,  3887,   282,  3887,  -429,  2279,    91,  -429,   243,  -429,
    -429,  -429,   361,   134,  -429,   301,  5478,   369,  -429,  4041,
    -429,  2360,  -429,  -429,  -429,  -429,  2412,  1747,  4757,  2626,
    2892,  4823,  2941,  3207,    19,   -81,  2545,  5626,   278,  -131,
     279,  -429,  -429,    61,    24,  2678,   117,   148,  3887,  3887,
     254,  -429,  3887,   292,  -429,  -429,  3887,  3887,   243,  1475,
    -429,   209,  3887,  3887,  -429,   262,   304,  2811,  1880,  -429,
     394,   266,   267,  -429,  -429,  -429,  -429,  -429,  -429,   272,
     273,   275,  -429,  -429,  -429,   423,  -429,   -90,  -429,  5570,
    5265,  -429,   314,  -429,    82,  5794,   229,  5338,   309,   309,
     212,   212,  6015,  6015,   284,   474,  -429,   -82,   -82,   309,
     309,  5682,   647,  5850,  5906,   212,   212,   238,   238,   474,
     474,   474,  -429,  5406,  -429,    92,  -429,  -429,  3887,  -429,
    -429,   134,  3887,  3887,  3887,  3887,  3887,  3887,  3887,  3887,
    3887,  3887,  3887,  3887,  3887,  3887,  3887,  -429,  2146,  -429,
    -429,  -429,   113,  5338,   316,  4889,  -429,  3887,  3887,  -429,
    3887,   291,  3887,  3887,   314,   329,   300,   314,  3887,   302,
    3887,  3887,   314,   303,   311,  5794,  -429,  -429,  5738,  3960,
    3960,    26,  2146,  -429,  -429,  4132,   447,  5794,  3960,  -429,
     325,   341,  5794,  2146,  -429,  4223,   307,  -429,  -429,  -429,
    -429,  -429,   427,   243,  -429,  2944,  3077,   229,  3291,   -76,
     131,  3887,  -429,   -96,   617,  3210,  3887,  -429,  -429,  5794,
    -429,  5794,  5794,  5794,  5794,  5794,  5794,  5794,  5794,  5794,
    5794,  5794,  5794,  5794,  5794,  5794,  -429,  4314,  -429,  3835,
      65,  -429,   212,   212,   212,  3343,   -77,  5794,   114,   -76,
    -429,   278,  5794,  -429,  -429,   -74,   151,  -429,  -429,  -429,
    -429,    70,  3887,  -429,  4405,  -429,   209,  -429,  -429,  -429,
    -429,  4496,  -429,  -429,  -429,  -429,  3887,  3887,   330,   -75,
     318,  5794,  3476,  -429,  -429,  -429,   133,  5794,  -429,  -429,
    3615,   346,  -429,  4955,  3887,   321,  3887,  3887,   322,   209,
    3887,  -429,    -2,  -429,  -429,  -429,  1042,  5962,  5962,   -75,
    -429,  -429,  3887,  5794,  -429,  5338,  5021,   339,  -429,  4628,
    -429,  5794,  4628,  -429,  -429,  3960,   209,  -429,  -429,  5794,
    3884,  -429,  3754,   436,   326,   323,    70,  -429,  -429,  5338,
    5087,  3887,   328,   331,  -429,  6038,  -429,  5794,  -429,  -429,
    -429
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    69,     1,   161,     0,     0,     0,     0,     0,   162,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   302,
       6,    11,     5,     4,    59,    61,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    64,   139,     0,    65,
      67,     0,    63,     0,     0,   303,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   305,     0,     0,
     141,     0,     0,    70,   311,     0,     0,     0,     0,    76,
      71,   300,    57,    58,    55,    56,    54,     0,     0,    53,
      62,    34,    32,    28,    25,    24,   341,   345,   344,   348,
     342,   346,   343,   347,     0,     0,     0,     0,     0,   317,
     337,   318,   350,   319,   323,   324,   325,   326,   330,   331,
     332,   333,   334,   335,   336,   338,   339,   371,   322,   329,
     340,   374,   377,   320,   327,   321,   328,     0,   349,   352,
     354,   353,   308,   140,     0,    68,     0,   313,   276,    79,
      80,    82,    81,    83,    84,    85,    86,   105,   106,   103,
     104,    96,   107,   108,    97,    94,    95,   109,   110,   111,
     112,    99,   100,    98,    92,    93,    88,    87,    89,    90,
      91,    78,    77,   101,   102,     0,    74,     0,    14,     0,
      49,    50,    47,    48,    46,    45,    51,     0,    27,   305,
       0,   309,     0,     0,     0,     0,    71,    71,    71,     0,
     358,     0,   364,   367,   366,   360,     0,   363,    66,   312,
     271,     0,   315,    72,   280,     0,   278,   287,     0,     0,
     304,   301,     0,    15,    16,     0,    60,     0,     0,   306,
       0,     0,     0,     0,    74,     0,    74,     0,    74,     0,
     283,     0,   282,   359,   362,     0,   365,   217,   218,     0,
       0,   212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   184,   183,   185,   186,   187,    17,
       0,     0,     0,     0,     0,   356,     0,   134,   178,   179,
     215,   180,   213,   264,   263,   262,    69,   267,   214,   266,
     265,   249,     0,     0,   216,   381,   382,   383,   384,     0,
     314,   273,     0,   279,    73,     0,   286,     0,   285,     0,
       0,    75,   113,   293,   294,   292,     0,     0,    52,   310,
       0,   368,     0,   370,   351,     0,   372,     0,   375,     0,
     378,   380,     0,   361,   357,   143,   146,     0,     0,     0,
       0,     0,     0,     0,   241,   242,     0,     0,     0,     0,
       0,   402,     0,     0,   222,   221,   254,   220,   219,     0,
       0,     0,   270,     0,    71,     0,     0,     0,   243,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   355,     0,     0,   274,     0,   277,
     281,   316,   288,     0,   291,     0,   290,     0,   299,   188,
     307,     0,   373,   376,   379,   284,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,     0,   393,   387,     0,
     396,   397,   398,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,    18,   245,   136,     0,     0,     0,     0,
     147,     0,     0,     0,   148,     0,     0,     0,     0,   116,
     114,     0,     0,   129,   124,   122,   123,   135,   117,     0,
       0,     0,   127,   128,   130,   159,   121,     0,   118,   188,
      71,   209,     0,   250,     0,   173,    74,     0,   223,   224,
     236,   237,   234,   235,     0,   259,   248,   268,   269,   225,
     226,     0,   239,   240,   238,   232,   233,   228,   227,   229,
     230,   231,   247,     0,   252,     0,   272,   275,     0,   179,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   298,
     369,   144,     0,     0,     0,     0,   255,     0,     0,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,   403,   404,     0,     0,
       0,     0,     0,   149,   152,   188,     0,   142,     0,   133,
       0,     0,    37,     0,   156,   188,     0,   131,   132,   120,
     125,   126,     0,     0,   119,     0,     0,    74,     0,     0,
       0,     0,   251,     0,   261,     0,     0,   246,   253,   289,
     137,   195,   196,   198,   197,   199,   192,   193,   194,   200,
     201,   190,   191,   202,   203,   189,   297,   188,   145,     0,
       0,   167,   164,   165,   166,     0,     0,   385,     0,     0,
     389,   388,   394,   400,   399,     0,     0,   391,   401,    19,
      43,    38,     0,   153,   188,   150,     0,    44,    35,    36,
     157,   188,   154,   134,   160,   163,     0,     0,     0,     0,
       0,   174,     0,   175,   181,   207,     0,   260,   296,   170,
       0,     0,   257,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,   151,   158,   155,     0,   205,   204,     0,
     210,   206,     0,   176,   208,     0,     0,     0,   258,   405,
     390,   386,   405,   392,    39,     0,     0,   115,   211,   177,
       0,   168,     0,     0,     0,     0,    38,    42,   171,     0,
       0,     0,     0,     0,    40,     0,   169,   406,   407,   408,
     172
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -429,  -429,  -429,   136,   443,  -429,  -429,  -429,  -429,  -429,
    -429,   410,  -429,  -429,   451,  -429,  -429,  -229,  -429,  -429,
    -429,  -429,   277,  -429,   457,   -46,  -429,   444,  -429,   227,
    -185,  -228,  -429,  -429,  -217,  -429,  -156,  -303,   -11,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,   509,  -429,  -429,
    -429,  -368,  -429,  -429,  -383,  -429,  -355,  -429,  -429,  -429,
    -429,   174,  -429,  -429,  -429,  -429,   206,  -429,   186,  -429,
    -213,  -193,   109,   -71,  -429,  -429,  -429,   335,  -429,  -429,
    -429,  -429,  -429,  -174,   -57,  -429,  -429,  -429,   -49,   270,
    -428,  -333,  -429,  -429,  -245,   -34,   171,  -429,  -429,  -429,
    -183,  -429
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   179,   185,   353,   280,    14,    15,    31,
      32,    55,    16,    28,    29,   461,   462,   691,   463,   464,
     465,   466,   186,   187,    25,    26,    39,    40,    41,    17,
     176,   219,    70,    18,   281,   467,   360,   468,   282,   469,
     283,   470,   471,   472,   473,   474,   593,   475,   476,   284,
     285,   484,   674,   286,   287,   288,   405,   478,   289,   290,
     291,   485,   301,   398,   302,   210,   214,   215,   240,   241,
     553,   216,   317,   221,   177,    46,    20,    95,    21,    22,
     137,    65,    23,   217,   293,    94,   130,   131,   242,   294,
     428,   429,   295,   430,   431,   432,   433,   296,   352,   297,
     724,   298
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   129,   312,   222,   310,   477,   325,   404,   327,   132,
     329,   234,   236,   238,   408,   134,   436,   351,   303,   521,
     520,   672,   374,    36,   306,   376,   556,   515,   190,   306,
     362,   557,   212,   190,     2,    24,   552,   211,   211,     3,
     243,   561,    37,   652,    51,    45,   346,   128,   542,    72,
      73,    42,    37,   308,   600,   480,   601,    96,   308,   601,
       4,   363,     5,    97,     6,   191,     7,   277,   331,   220,
     319,     8,   594,   392,   393,    52,    38,     9,    98,   689,
      59,   304,    27,    10,   305,   684,    38,    30,   687,   332,
     690,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      37,   212,   212,    33,   299,   209,    34,   180,   181,   641,
      35,   601,   300,   223,   224,   129,   129,   129,   610,   129,
     129,   129,   129,   230,   231,   232,   574,   235,   237,   239,
     244,   200,   307,   213,    38,   584,   201,   307,    74,   307,
     716,   129,    75,    37,    76,    77,    66,   245,    48,   311,
      53,    50,   424,   228,   434,   202,    48,    49,   246,   486,
      54,   128,   128,   128,   636,   128,   128,   128,   128,   518,
      11,   313,   129,   645,   566,    78,    12,    38,   314,    56,
     335,   229,   208,   129,   129,   517,   680,   128,    43,    62,
      44,   347,   349,   225,    59,   601,   306,    47,   559,   307,
     315,   638,   316,   560,    48,   601,    57,   681,   203,   646,
     365,    67,    68,    63,   576,   626,   182,   676,   128,   226,
     183,   602,   184,    77,   307,   308,   601,   309,   516,   128,
     128,   608,   129,   396,    58,    60,   365,   204,   603,   129,
     402,   205,    69,   321,   555,   129,   601,   407,   439,   653,
     440,   685,   628,   411,   571,   129,   557,    61,   206,   557,
     660,   563,   129,   129,   692,   129,   129,    64,   670,   207,
     419,   420,   704,   422,   423,   597,    81,   277,   128,   278,
     519,   178,   441,   442,    71,   128,   366,   367,   368,   369,
     560,   128,   564,   557,    83,   688,   374,    84,   375,   376,
     377,   128,    85,   378,   133,   379,   380,   365,   128,   128,
     136,   128,   128,   138,   368,   369,   173,   174,   175,   178,
      52,   477,   374,   192,   375,   376,   377,   189,   193,   378,
     481,   194,   650,   651,   387,   388,   389,   390,   391,   195,
     196,   657,   197,   198,   199,   218,   233,   392,   393,   668,
     212,   333,   277,   324,   334,   339,   337,   338,   397,   341,
     292,   340,   389,   390,   391,   342,   673,   343,   395,    11,
     200,   361,   401,   392,   393,   201,   399,   412,   200,   413,
     414,   417,   320,   201,   487,   368,   369,   494,   496,   512,
     521,   555,   558,   374,   202,   375,   376,   377,   567,   222,
     378,   223,   202,   129,   579,   580,   686,   586,   587,   588,
     129,   598,   344,   345,   589,   590,   350,   591,   604,   694,
     592,   599,   605,   630,   354,   355,   356,   357,   358,   635,
     359,   387,   388,   389,   390,   391,   639,   640,   656,   643,
     647,   658,   659,   664,   392,   393,   306,   203,   648,   128,
     663,   699,   714,   707,   313,   203,   128,   701,   710,   713,
     722,   314,   731,   732,   406,   738,   129,   733,   739,   443,
      79,   409,   365,   188,   629,   308,   204,   734,   726,   727,
     205,    82,   318,   315,   204,    80,   135,   696,   205,   200,
      19,   400,   418,   364,   201,   421,   538,   206,   415,   336,
     437,   427,   665,   427,   227,   206,   644,     0,   207,   725,
       0,   537,   128,   202,   479,     0,   207,     0,     0,     0,
     488,   489,     0,     0,   490,   491,   492,   493,     0,   495,
       0,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,     0,   513,     0,     0,
     368,   369,     0,     0,     0,     0,     0,     0,   374,     0,
     375,   376,   377,    86,     0,   378,   203,    87,   365,     0,
       0,   545,     0,     0,     0,     0,     0,     0,    88,    89,
     359,     0,     0,    90,    91,    92,    93,     0,     0,   359,
       0,     0,   565,   350,     0,   204,   568,     0,     0,   205,
     569,   570,     0,   575,     0,     0,   577,   578,     0,   392,
     393,   582,   585,     0,     0,     0,   206,     0,     0,     0,
       0,     0,   322,     0,     0,     0,   200,   207,   129,     0,
       0,   201,     0,     0,     0,     0,   720,     0,   700,     0,
       0,     0,     0,     0,     0,   365,   368,   369,     0,     0,
     202,     0,   129,     0,   374,     0,     0,   376,   377,     0,
     735,   378,     0,     0,     0,     0,     0,     0,   718,     0,
       0,     0,   609,     0,   128,     0,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,     0,   627,     0,     0,     0,     0,     0,   128,     0,
       0,   632,   633,   203,   634,   392,   393,   637,     0,     0,
       0,     0,   642,     0,   427,     0,     0,     0,     0,     0,
       0,   366,   367,   368,   369,   370,   654,     0,   371,   372,
     373,   374,   204,   375,   376,   377,   205,   661,   378,     0,
     379,   380,     0,     0,     0,   348,     0,     0,     0,   621,
     625,     0,     0,   206,     0,   671,    96,     0,     0,     0,
     677,     0,    97,     0,   207,   383,   384,   385,   386,   387,
     388,   389,   390,   391,     0,     0,     0,    98,     0,     0,
       0,     0,   392,   393,     0,     0,     0,     0,     0,   683,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     0,
     697,   698,     0,     0,     0,     0,   703,     0,     0,     0,
       0,     0,     0,     0,   706,     0,     0,     0,   709,     0,
     711,   712,     0,     0,   715,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,   719,   445,     0,     0,
       0,     3,    37,   446,   447,     0,   448,     0,   247,   248,
     249,   250,     0,     0,     0,     0,   730,     0,   449,   251,
     450,   451,     0,     0,     0,   737,     0,     0,     0,     0,
     452,   252,     0,     0,   453,   253,    38,   254,     0,     9,
     255,   454,     0,   455,   256,     0,     0,   456,   457,     0,
       0,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,     0,
     257,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,    37,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   271,
     272,     0,     0,     0,     0,   273,   274,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,    38,   459,   277,
     460,   278,   279,   445,     0,     0,     0,     3,     0,   446,
     447,     0,   448,     0,   247,   248,   249,   250,     0,     0,
       0,     0,     0,     0,   449,   251,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,   452,   252,     0,     0,
     453,   253,     0,   254,     0,     9,   255,   454,     0,   455,
     256,     0,     0,   456,   457,     0,     0,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,     0,   257,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    37,
     269,     0,     0,     0,   200,     0,     0,     0,     0,   201,
       0,     0,     0,     0,   270,   271,   272,     0,     0,     0,
       0,   273,   274,     0,     0,     0,     0,     0,   202,     0,
     276,     0,     0,    38,   459,   277,   717,   278,   279,   247,
     248,   249,   250,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,   200,     0,     0,     0,
       0,   201,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
     202,   203,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
     204,   257,     0,     0,   205,     0,   323,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,   206,     0,   203,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   425,   269,     0,     0,     0,     0,
       0,     0,   204,     0,     0,     0,   205,     0,     0,   270,
     271,   272,     0,     0,     0,     0,   273,   274,   247,   248,
     249,   250,     0,   206,     0,   426,     0,     0,    38,   251,
     277,     0,   278,   279,   207,   200,     0,     0,     0,     0,
     201,   252,     0,     0,     0,   253,     0,   254,     0,     0,
     255,     0,     0,     0,   256,     0,     0,     0,     0,   202,
       0,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,     0,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   425,   269,     0,     0,     0,     0,     0,
       0,   204,     0,     0,     0,   205,     0,     0,   270,   271,
     272,     0,     0,     0,     0,   273,   274,   247,   248,   249,
     250,     0,   206,     0,   435,     0,     0,    38,   251,   277,
       0,   278,   279,   207,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,     0,     0,
       0,   251,     0,   276,     0,     0,    38,   573,   277,     0,
     278,   279,     0,   252,     0,     0,     0,   253,     0,   254,
       0,     0,   255,     0,     0,     0,   256,     0,     0,     0,
       0,     0,     0,    99,   100,   101,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   118,   119,   120,     0,     0,   123,   124,   125,
     126,     0,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   258,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   267,   268,    37,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   271,   272,     0,     0,     0,     0,   273,   274,   247,
     248,   249,   250,   543,   482,     0,   276,   483,     0,    38,
     251,   277,     0,   278,   279,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,     0,   201,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,   544,     0,   270,
     271,   272,   247,   248,   249,   250,   273,   274,     0,     0,
       0,     0,     0,   251,     0,   276,   200,     0,    38,     0,
     277,   201,   278,   279,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
     202,     0,     0,     0,   203,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,   204,   257,     0,     0,   205,     0,   326,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,   203,   206,   583,     0,     0,     0,     0,
     403,     0,     0,     0,     0,   207,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,   204,     0,     0,     0,   205,     0,   328,     0,
       0,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,   206,     0,     0,   251,     0,   276,     0,
       0,    38,     0,   277,   207,   278,   279,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,     0,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,     0,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,     0,   201,     0,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   247,   248,
     249,   250,   273,   274,     0,     0,     0,     0,     0,   251,
     275,   276,   200,     0,    38,     0,   277,   201,   278,   279,
       0,   252,     0,     0,     0,   253,     0,   254,     0,     0,
     255,     0,     0,     0,   256,     0,   202,     0,     0,     0,
     203,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,   204,
     257,     0,     0,   205,     0,   330,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,   203,
     206,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,   207,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,    37,   269,     0,     0,     0,   204,     0,
       0,     0,   205,     0,     0,     0,     0,     0,   270,   271,
     272,   247,   248,   249,   250,   273,   274,     0,     0,   206,
       0,   416,   251,     0,   276,     0,     0,    38,     0,   277,
     207,   278,   279,     0,   252,     0,     0,     0,   253,     0,
     254,     0,     0,   255,     0,     0,     0,   256,     0,     0,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,     0,   257,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   200,
       0,     0,     0,     0,   201,     0,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,     0,     0,   202,     0,     0,     0,     0,     0,     0,
       0,   270,   271,   272,   247,   248,   249,   250,   273,   274,
       0,     0,     0,     0,     0,   251,     0,   276,   514,     0,
      38,     0,   277,     0,   278,   279,     0,   252,     0,     0,
       0,   253,     0,   254,     0,     0,   255,     0,     0,     0,
     256,     0,     0,     0,     0,     0,   203,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,   204,   257,     0,     0,   205,
       0,   540,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    37,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   271,   272,   247,   248,   249,
     250,   273,   274,     0,     0,     0,     0,     0,   251,     0,
     276,   541,     0,    38,     0,   277,     0,   278,   279,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   200,     0,     0,     0,     0,
     201,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,     0,     0,
       0,   251,     0,   276,   554,     0,    38,     0,   277,     0,
     278,   279,     0,   252,     0,     0,     0,   253,     0,   254,
       0,     0,   255,     0,     0,     0,   256,     0,     0,     0,
       0,     0,   203,    99,   100,   101,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   118,   119,   120,     0,     0,   123,   124,   125,
     126,   204,   257,     0,     0,   205,     0,   547,     0,     0,
       0,     0,     0,     0,   258,   259,     0,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   207,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   267,   268,    37,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   271,   272,   247,   248,   249,   250,   273,   274,     0,
       0,     0,     0,     0,   251,     0,   276,   562,     0,    38,
       0,   277,     0,   278,   279,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,   581,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,   201,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,   247,   248,   249,   250,
     273,   274,     0,     0,     0,     0,     0,   251,     0,   276,
     200,     0,    38,     0,   277,   201,   278,   279,     0,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,   202,     0,     0,     0,   203,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,     0,   123,   124,   125,   126,   204,   257,     0,
       0,   205,     0,   548,     0,     0,     0,     0,     0,     0,
     258,   259,     0,     0,     0,     0,     0,   203,   206,     0,
       0,     0,     0,     0,   666,     0,     0,     0,     0,   207,
       0,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,    37,   269,     0,     0,     0,   204,     0,     0,     0,
     205,     0,   550,     0,     0,     0,   270,   271,   272,   247,
     248,   249,   250,   273,   274,     0,     0,   206,     0,     0,
     251,     0,   276,     0,     0,    38,     0,   277,   207,   278,
     279,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   667,     0,     0,
       0,     0,     0,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     271,   272,   247,   248,   249,   250,   273,   274,     0,     0,
       0,     0,     0,   251,     0,   276,   200,     0,    38,     0,
     277,   201,   278,   279,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
     202,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,   203,     0,     0,     0,     0,     0,     0,
     200,     0,     0,     0,     0,   201,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,   204,     0,   202,     0,   205,     0,   551,     0,
       0,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,   206,     0,     0,   251,     0,   276,   675,
       0,    38,     0,   277,   207,   278,   279,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,   203,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,   204,   257,     0,     0,
     205,     0,   669,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   247,   248,
     249,   250,   273,   274,     0,     0,     0,     0,     0,   251,
       0,   276,   682,     0,    38,     0,   277,     0,   278,   279,
       0,   252,     0,     0,     0,   253,     0,   254,     0,     0,
     255,     0,     0,     0,   256,     0,     0,     0,     0,     0,
       0,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,     0,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,     0,
       0,   702,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,    37,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   271,
     272,     0,     0,     0,     0,   273,   274,   247,   248,   249,
     250,   705,     0,     0,   276,     0,     0,    38,   251,   277,
       0,   278,   279,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
       0,     0,     0,     0,   273,   274,   247,   248,   249,   250,
     729,     0,     0,   276,     0,     0,    38,   251,   277,     0,
     278,   279,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,     0,     0,     0,     0,     0,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,     0,   123,   124,   125,   126,     0,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   200,     0,     0,     0,     0,   201,
       0,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,    37,   269,     0,     0,     0,     0,     0,   202,     0,
       0,     0,     0,     0,     0,     0,   270,   271,   272,   247,
     248,   249,   250,   273,   274,     0,     0,     0,     0,     0,
     251,     0,   276,   200,     0,    38,     0,   277,   201,   278,
     279,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,   202,     0,     0,
       0,   203,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
     204,   257,     0,     0,   205,     0,     0,     0,   365,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
     203,   206,     0,     0,   679,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,   204,
       0,     0,     0,   205,     0,     0,     0,     0,     0,   270,
     271,   272,     0,     0,     0,     0,   273,   274,     0,     0,
     206,     0,     0,   728,     0,   276,     0,     0,    38,     0,
     277,   207,   278,   279,   366,   367,   368,   369,   370,   365,
       0,   371,   372,   373,   374,     0,   375,   376,   377,     0,
       0,   378,     0,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,     0,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,   522,   523,   524,
     525,   526,   527,   528,   529,   366,   367,   368,   369,   370,
     530,   531,   371,   372,   373,   374,   532,   375,   376,   377,
     365,     0,   378,   533,   379,   380,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,   381,     0,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   393,     0,     0,
       0,     0,     0,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   522,   523,
     524,   525,   526,   527,   528,   529,   366,   367,   368,   369,
     370,   530,   531,   371,   372,   373,   374,   532,   375,   376,
     377,   365,     0,   378,   533,   379,   380,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   536,   381,     0,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,     0,
       0,     0,     0,     0,   655,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   522,
     523,   524,   525,   526,   527,   528,   529,   366,   367,   368,
     369,   370,   530,   531,   371,   372,   373,   374,   532,   375,
     376,   377,   365,     0,   378,   533,   379,   380,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   381,     0,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
       0,     0,     0,     0,     0,     0,     0,     0,   392,   393,
       0,     0,     0,     0,     0,   662,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     522,   523,   524,   525,   526,   527,   528,   529,   366,   367,
     368,   369,   370,   530,   531,   371,   372,   373,   374,   532,
     375,   376,   377,   365,     0,   378,   533,   379,   380,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   536,   381,
       0,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,     0,     0,     0,     0,     0,   678,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   522,   523,   524,   525,   526,   527,   528,   529,   366,
     367,   368,   369,   370,   530,   531,   371,   372,   373,   374,
     532,   375,   376,   377,   365,     0,   378,   533,   379,   380,
     534,   535,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   536,
     381,     0,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,     0,     0,     0,     0,     0,     0,     0,     0,
     392,   393,     0,     0,     0,     0,     0,   693,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   522,   523,   524,   525,   526,   527,   528,   529,
     366,   367,   368,   369,   370,   530,   531,   371,   372,   373,
     374,   532,   375,   376,   377,     0,     0,   378,   533,   379,
     380,   534,   535,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   381,     0,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,     0,     0,     0,   365,     0,     0,     0,
       0,   392,   393,   366,   367,   368,   369,   370,   695,     0,
     371,   372,   373,   374,     0,   375,   376,   377,     0,     0,
     378,     0,   379,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,     0,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,     0,     0,     0,   365,
       0,     0,     0,     0,   392,   393,     0,     0,     0,     0,
       0,   410,   366,   367,   368,   369,   370,     0,     0,   371,
     372,   373,   374,     0,   375,   376,   377,     0,     0,   378,
       0,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   365,     0,     0,     0,     0,
       0,     0,     0,   392,   393,   366,   367,   368,   369,   370,
     723,     0,   371,   372,   373,   374,     0,   375,   376,   377,
       0,     0,   378,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,     0,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,     0,     0,
       0,   365,     0,     0,     0,     0,   392,   393,     0,     0,
     444,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   374,     0,   375,   376,   377,     0,     0,   378,     0,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,     0,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,     0,     0,     0,   365,     0,     0,
       0,     0,   392,   393,     0,     0,   546,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   374,     0,   375,
     376,   377,     0,     0,   378,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,     0,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
       0,     0,     0,   365,     0,     0,     0,     0,   392,   393,
       0,     0,   549,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   374,     0,   375,   376,   377,     0,     0,
     378,     0,   379,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,     0,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,     0,     0,     0,   365,
       0,     0,     0,     0,   392,   393,     0,     0,   631,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   374,
       0,   375,   376,   377,     0,     0,   378,     0,   379,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,     0,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,     0,     0,     0,   365,     0,     0,     0,     0,
     392,   393,     0,     0,   708,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   374,     0,   375,   376,   377,
       0,     0,   378,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,     0,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   393,     0,     0,
     721,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   374,     0,   375,   376,   377,     0,     0,   378,     0,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,    97,
       0,     0,   381,     0,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    98,     0,     0,     0,     0,     0,
       0,     0,   392,   393,     0,     0,   736,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
     260,   261,   262,     0,     0,     0,     0,     0,     0,    37,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     0,
       0,   365,     0,    38,     0,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,     0,   175,   365,     0,    38,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   374,     0,   375,
     376,   377,     0,     0,   378,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,   381,     0,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
       0,     0,     0,     0,     0,     0,     0,     0,   392,   393,
     394,     0,     0,     0,     0,     0,   365,     0,     0,    38,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
     374,     0,   375,   376,   377,     0,     0,   378,     0,   379,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,     0,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,     0,     0,     0,     0,     0,     0,     0,
       0,   392,   393,   607,   522,   523,   524,   525,   526,   527,
     528,   529,   366,   367,   368,   369,   370,   530,   531,   371,
     372,   373,   374,   532,   375,   376,   377,  -188,   365,   378,
     533,   379,   380,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   536,   381,     0,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   522,   523,   524,   525,
     526,   527,   528,   529,   366,   367,   368,   369,   370,   530,
     531,   371,   372,   373,   374,   595,   375,   376,   377,     0,
       0,   378,   533,   379,   380,   534,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,   596,   381,     0,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,     0,     0,     0,
     366,   367,   368,   369,   370,   392,   393,   371,   372,   373,
     374,     0,   375,   376,   377,     0,     0,   378,     0,   379,
     380,     0,     0,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,     0,     0,     0,
       0,   381,     0,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,     0,     0,     0,   366,   367,   368,   369,
     370,   392,   393,   371,   372,   373,   374,     0,   375,   376,
     377,     0,     0,   378,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,   381,   606,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,     0,
       0,     0,   366,   367,   368,   369,   370,   392,   393,   371,
     372,   373,   374,     0,   375,   376,   377,     0,     0,   378,
       0,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
     649,     0,     0,   381,     0,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,     0,     0,     0,   366,   367,
     368,   369,   370,   392,   393,   371,   372,   373,   374,     0,
     375,   376,   377,     0,     0,   378,     0,   379,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,   381,
       0,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,     0,     0,     0,   366,   367,   368,   369,   370,   392,
     393,   371,   372,   373,   374,     0,   375,   376,   377,     0,
       0,   378,     0,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,     0,     0,   384,
     385,   386,   387,   388,   389,   390,   391,     0,     0,     0,
     366,   367,   368,   369,   370,   392,   393,   371,   372,   373,
     374,     0,   375,   376,   377,     0,     0,   378,     0,   379,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   385,   386,   387,   388,
     389,   390,   391,     0,     0,     0,   366,   367,     0,     0,
     370,   392,   393,   371,   372,   373,   374,   200,   375,   376,
     377,     0,   201,   378,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,   381,     0,   382,
     383,   384,   385,   386,     0,     0,     0,   390,   391,   366,
     367,   368,   369,   370,     0,     0,   371,   392,   393,   374,
       0,   375,   376,   377,     0,     0,   378,     0,   379,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   385,   386,   387,   388,   389,
     390,   391,     0,     0,     0,     0,     0,     0,     0,     0,
     392,   393,     0,   204,     0,     0,     0,   205,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   206,     0,     0,   740,     0,     0,
       0,     0,     0,     0,     0,   207,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,   158,   159,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,   171,
     172
};

static const yytype_int16 yycheck[] =
{
      11,    58,   219,   177,   217,   360,   234,   310,   236,    58,
     238,   196,   197,   198,   317,    61,   349,   262,   211,   109,
     403,   117,   104,    30,   105,   107,   157,   395,   127,   105,
     127,   162,   127,   127,     0,   127,    17,    43,    43,     5,
      29,    17,   127,    17,    19,    54,    10,    58,   416,    12,
      13,    62,   127,   134,   482,   140,   133,    21,   134,   133,
      26,   158,    28,    27,    30,   164,    32,   163,   141,   164,
     164,    37,   162,   155,   156,    50,   161,    43,    42,     9,
     161,   159,   121,    49,   162,   162,   161,   127,   162,   162,
      20,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
     127,   127,   127,   127,   156,   136,   127,    12,    13,   557,
     127,   133,   164,   129,   130,   192,   193,   194,   521,   196,
     197,   198,   199,   192,   193,   194,   449,   196,   197,   198,
     139,    29,   133,   159,   161,   458,    34,   133,   121,   133,
     162,   218,   125,   127,   127,   128,    31,   156,   133,   218,
     145,   133,   346,   134,   348,    53,   133,   136,   167,   364,
     155,   192,   193,   194,   552,   196,   197,   198,   199,   402,
     156,   105,   249,   561,   439,   158,   162,   161,   112,   136,
     249,   162,   159,   260,   261,   398,   141,   218,     4,   133,
       6,   260,   261,   133,   161,   133,   105,   134,   157,   133,
     134,   554,   136,   162,   133,   133,   163,   162,   106,   562,
      18,    96,    97,   157,   451,   538,   121,   605,   249,   159,
     125,   159,   127,   128,   133,   134,   133,   136,   157,   260,
     261,   159,   309,   299,   134,   127,    18,   135,   486,   316,
     309,   139,   127,   141,   133,   322,   133,   316,   162,   572,
     164,   157,   159,   322,   448,   332,   162,   158,   156,   162,
     583,   164,   339,   340,   652,   342,   343,   127,   157,   167,
     339,   340,   159,   342,   343,   480,   121,   163,   309,   165,
     166,   129,   130,   131,   163,   316,    94,    95,    96,    97,
     162,   322,   164,   162,   127,   164,   104,   127,   106,   107,
     108,   332,   127,   111,   127,   113,   114,    18,   339,   340,
     136,   342,   343,   163,    96,    97,    31,    31,   158,   129,
      50,   696,   104,   140,   106,   107,   108,   163,   140,   111,
     361,   140,   569,   570,   142,   143,   144,   145,   146,   158,
     140,   578,   140,   140,   140,   136,   127,   155,   156,   597,
     127,   139,   163,   159,   157,   140,   158,   158,    40,   158,
     206,   140,   144,   145,   146,   140,   603,   140,   158,   156,
      29,   166,   127,   155,   156,    34,   162,   141,    29,   141,
     141,   127,   228,    34,    16,    96,    97,   127,   127,   127,
     109,   133,   133,   104,    53,   106,   107,   108,   164,   593,
     111,   129,    53,   480,   162,   121,   639,    33,   162,   162,
     487,   480,   258,   259,   162,   162,   262,   162,   487,   656,
      17,   127,   158,   127,   270,   271,   272,   273,   274,   158,
     276,   142,   143,   144,   145,   146,   127,   157,    11,   157,
     157,   136,   121,    36,   155,   156,   105,   106,   157,   480,
     163,   141,   689,   127,   105,   106,   487,   159,   157,   157,
     141,   112,    46,   157,   310,   157,   543,   164,   157,   353,
      47,   317,    18,    83,   543,   134,   135,   726,   715,   716,
     139,    50,   225,   134,   135,    48,    62,   663,   139,    29,
       1,   305,   338,   286,    34,   341,   407,   156,   332,   249,
     349,   347,   593,   349,   189,   156,   560,    -1,   167,   712,
      -1,   162,   543,    53,   360,    -1,   167,    -1,    -1,    -1,
     366,   367,    -1,    -1,   370,   371,   372,   373,    -1,   375,
      -1,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,   393,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    59,    -1,   111,   106,    63,    18,    -1,
      -1,   417,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
     426,    -1,    -1,    79,    80,    81,    82,    -1,    -1,   435,
      -1,    -1,   438,   439,    -1,   135,   442,    -1,    -1,   139,
     446,   447,    -1,   449,    -1,    -1,   452,   453,    -1,   155,
     156,   457,   458,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,   162,    -1,    -1,    -1,    29,   167,   705,    -1,
      -1,    34,    -1,    -1,    -1,    -1,   705,    -1,   669,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    96,    97,    -1,    -1,
      53,    -1,   729,    -1,   104,    -1,    -1,   107,   108,    -1,
     729,   111,    -1,    -1,    -1,    -1,    -1,    -1,   699,    -1,
      -1,    -1,   518,    -1,   705,    -1,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,    -1,   538,    -1,    -1,    -1,    -1,    -1,   729,    -1,
      -1,   547,   548,   106,   550,   155,   156,   553,    -1,    -1,
      -1,    -1,   558,    -1,   560,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,   572,    -1,   101,   102,
     103,   104,   135,   106,   107,   108,   139,   583,   111,    -1,
     113,   114,    -1,    -1,    -1,    10,    -1,    -1,    -1,   595,
     596,    -1,    -1,   156,    -1,   601,    21,    -1,    -1,    -1,
     606,    -1,    27,    -1,   167,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,   635,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
     666,   667,    -1,    -1,    -1,    -1,   672,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   680,    -1,    -1,    -1,   684,    -1,
     686,   687,    -1,    -1,   690,    -1,    -1,    -1,    -1,    -1,
     696,    -1,    -1,    -1,    -1,    -1,   702,     1,    -1,    -1,
      -1,     5,   127,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,   722,    -1,    22,    23,
      24,    25,    -1,    -1,    -1,   731,    -1,    -1,    -1,    -1,
      34,    35,    -1,    -1,    38,    39,   161,    41,    -1,    43,
      44,    45,    -1,    47,    48,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,   165,   166,     1,    -1,    -1,    -1,     5,    -1,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    45,    -1,    47,
      48,    -1,    -1,    51,    52,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    53,    -1,
     158,    -1,    -1,   161,   162,   163,   164,   165,   166,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    34,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      53,   106,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
     135,    84,    -1,    -1,   139,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,   139,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,   150,    12,    13,
      14,    15,    -1,   156,    -1,   158,    -1,    -1,   161,    23,
     163,    -1,   165,   166,   167,    29,    -1,    -1,    -1,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    53,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,   139,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,   150,    12,    13,    14,
      15,    -1,   156,    -1,   158,    -1,    -1,   161,    23,   163,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
      12,    13,    14,    15,   149,   150,    -1,    -1,    -1,    -1,
      -1,    23,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
     165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,   150,    12,
      13,    14,    15,    16,   156,    -1,   158,   159,    -1,   161,
      23,   163,    -1,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    34,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,   144,    12,    13,    14,    15,   149,   150,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   158,    29,    -1,   161,    -1,
     163,    34,   165,   166,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      53,    -1,    -1,    -1,   106,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,   135,    84,    -1,    -1,   139,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,   106,   156,   105,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,   167,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,   135,    -1,    -1,    -1,   139,    -1,   141,    -1,
      -1,    -1,   142,   143,   144,    12,    13,    14,    15,   149,
     150,    -1,    -1,   156,    -1,    -1,    23,    -1,   158,    -1,
      -1,   161,    -1,   163,   167,   165,   166,    -1,    35,    -1,
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
     157,   158,    29,    -1,   161,    -1,   163,    34,   165,   166,
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
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,    12,    13,    14,    15,   149,   150,    -1,    -1,   156,
      -1,   158,    23,    -1,   158,    -1,    -1,   161,    -1,   163,
     167,   165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,    12,    13,    14,    15,   149,   150,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   158,   159,    -1,
     161,    -1,   163,    -1,   165,   166,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,   106,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,   135,    84,    -1,    -1,   139,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,    14,
      15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     158,   159,    -1,   161,    -1,   163,    -1,   165,   166,    -1,
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
      -1,    23,    -1,   158,   159,    -1,   161,    -1,   163,    -1,
     165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,   106,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,   135,    84,    -1,    -1,   139,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,    12,    13,    14,    15,   149,   150,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   158,   159,    -1,   161,
      -1,   163,    -1,   165,   166,    -1,    35,    -1,    -1,    -1,
      39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,
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
      29,    -1,   161,    -1,   163,    34,   165,   166,    -1,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    53,    -1,    -1,    -1,   106,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,   135,    84,    -1,
      -1,   139,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,   106,   156,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,   167,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,   135,    -1,    -1,    -1,
     139,    -1,   141,    -1,    -1,    -1,   142,   143,   144,    12,
      13,    14,    15,   149,   150,    -1,    -1,   156,    -1,    -1,
      23,    -1,   158,    -1,    -1,   161,    -1,   163,   167,   165,
     166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,    12,    13,    14,    15,   149,   150,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   158,    29,    -1,   161,    -1,
     163,    34,   165,   166,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      53,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    34,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,   135,    -1,    53,    -1,   139,    -1,   141,    -1,
      -1,    -1,   142,   143,   144,    12,    13,    14,    15,   149,
     150,    -1,    -1,   156,    -1,    -1,    23,    -1,   158,   159,
      -1,   161,    -1,   163,   167,   165,   166,    -1,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,   106,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,   135,    84,    -1,    -1,
     139,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,
      14,    15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,
      -1,   158,   159,    -1,   161,    -1,   163,    -1,   165,   166,
      -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,   150,    12,    13,    14,
      15,    16,    -1,    -1,   158,    -1,    -1,   161,    23,   163,
      -1,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,   150,    12,    13,    14,    15,
      16,    -1,    -1,   158,    -1,    -1,   161,    23,   163,    -1,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,    12,
      13,    14,    15,   149,   150,    -1,    -1,    -1,    -1,    -1,
      23,    -1,   158,    29,    -1,   161,    -1,   163,    34,   165,
     166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    53,    -1,    -1,
      -1,   106,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
     135,    84,    -1,    -1,   139,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
     106,   156,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
     156,    -1,    -1,   159,    -1,   158,    -1,    -1,   161,    -1,
     163,   167,   165,   166,    94,    95,    96,    97,    98,    18,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
      18,    -1,   111,   112,   113,   114,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    18,    -1,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    18,    -1,   111,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    18,    -1,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    18,    -1,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,    -1,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   155,   156,    94,    95,    96,    97,    98,   162,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,
      -1,   162,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,    94,    95,    96,    97,    98,
     162,    -1,   101,   102,   103,   104,    -1,   106,   107,   108,
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
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
     159,    94,    95,    96,    97,    98,    -1,    -1,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,   159,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
     118,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    18,    -1,   161,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,   158,    18,    -1,   161,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   161,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,   157,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    18,   111,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    -1,
      -1,   111,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      94,    95,    96,    97,    98,   155,   156,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    94,    95,    96,    97,
      98,   155,   156,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    94,    95,    96,    97,    98,   155,   156,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
     132,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    94,    95,
      96,    97,    98,   155,   156,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    94,    95,    96,    97,    98,   155,
     156,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      94,    95,    96,    97,    98,   155,   156,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    94,    95,    -1,    -1,
      98,   155,   156,   101,   102,   103,   104,    29,   106,   107,
     108,    -1,    34,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,   145,   146,    94,
      95,    96,    97,    98,    -1,    -1,   101,   155,   156,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,   149,
     150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   169,     0,     5,    26,    28,    30,    32,    37,    43,
      49,   156,   162,   170,   175,   176,   180,   197,   201,   215,
     244,   246,   247,   250,   127,   192,   193,   121,   181,   182,
     127,   177,   178,   127,   127,   127,    30,   127,   161,   194,
     195,   196,   206,     4,     6,    54,   243,   134,   133,   136,
     133,    19,    50,   145,   155,   179,   136,   163,   134,   161,
     127,   158,   133,   157,   127,   249,    31,    96,    97,   127,
     200,   163,    12,    13,   121,   125,   127,   128,   158,   172,
     192,   121,   182,   127,   127,   127,    59,    63,    74,    75,
      79,    80,    81,    82,   253,   245,    21,    27,    42,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   206,   252,
     254,   255,   256,   127,   193,   195,   136,   248,   163,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   113,   114,   115,
     116,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   149,   150,    31,    31,   158,   198,   242,   129,   171,
      12,    13,   121,   125,   127,   172,   190,   191,   179,   163,
     127,   164,   140,   140,   140,   158,   140,   140,   140,   140,
      29,    34,    53,   106,   135,   139,   156,   167,   159,   206,
     233,    43,   127,   159,   234,   235,   239,   251,   136,   199,
     164,   241,   251,   129,   130,   133,   159,   245,   134,   162,
     256,   256,   256,   127,   198,   256,   198,   256,   198,   256,
     236,   237,   256,    29,   139,   156,   167,    12,    13,    14,
      15,    23,    35,    39,    41,    44,    48,    84,    96,    97,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   128,
     142,   143,   144,   149,   150,   157,   158,   163,   165,   166,
     174,   202,   206,   208,   217,   218,   221,   222,   223,   226,
     227,   228,   229,   252,   257,   260,   265,   267,   269,   156,
     164,   230,   232,   239,   159,   162,   105,   133,   134,   136,
     238,   256,   202,   105,   112,   134,   136,   240,   190,   164,
     229,   141,   162,   141,   159,   199,   141,   199,   141,   199,
     141,   141,   162,   139,   157,   256,   257,   158,   158,   140,
     140,   158,   140,   140,   229,   229,    10,   256,    10,   256,
     229,   262,   266,   173,   229,   229,   229,   229,   229,   229,
     204,   166,   127,   158,   197,    18,    94,    95,    96,    97,
      98,   101,   102,   103,   104,   106,   107,   108,   111,   113,
     114,   135,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   155,   156,   157,   158,   193,    40,   231,   162,
     234,   127,   256,   110,   205,   224,   229,   256,   205,   229,
     162,   256,   141,   141,   141,   236,   158,   127,   229,   256,
     256,   229,   256,   256,   251,   127,   158,   229,   258,   259,
     261,   262,   263,   264,   251,   158,   259,   264,   117,   162,
     164,   130,   131,   171,   159,     1,     7,     8,    10,    22,
      24,    25,    34,    38,    45,    47,    51,    52,    85,   162,
     164,   183,   184,   186,   187,   188,   189,   203,   205,   207,
     209,   210,   211,   212,   213,   215,   216,   224,   225,   229,
     140,   206,   156,   159,   219,   229,   198,    16,   229,   229,
     229,   229,   229,   229,   127,   229,   127,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   127,   229,   159,   219,   157,   239,   238,   166,
     222,   109,    86,    87,    88,    89,    90,    91,    92,    93,
      99,   100,   105,   112,   115,   116,   134,   162,   240,   162,
     141,   159,   219,    16,   140,   229,   159,   141,   141,   159,
     141,   141,    17,   238,   159,   133,   157,   162,   133,   157,
     162,    17,   159,   164,   164,   229,   262,   164,   229,   229,
     229,   251,   105,   162,   205,   229,   202,   229,   229,   162,
     121,    51,   229,   105,   205,   229,    33,   162,   162,   162,
     162,   162,    17,   214,   162,   105,   134,   198,   256,   127,
     258,   133,   159,   199,   256,   158,   136,   157,   159,   229,
     222,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   205,   229,   159,   256,
     127,   159,   229,   229,   229,   158,   219,   229,   259,   127,
     157,   258,   229,   157,   263,   219,   259,   157,   157,   132,
     202,   202,    17,   205,   229,   162,    11,   202,   136,   121,
     205,   229,   162,   163,    36,   241,   110,   110,   199,   141,
     157,   229,   117,   202,   220,   159,   219,   229,   162,   159,
     141,   162,   159,   229,   162,   157,   238,   162,   164,     9,
      20,   185,   219,   162,   202,   162,   204,   229,   229,   141,
     206,   159,   105,   229,   159,    16,   229,   127,   159,   229,
     157,   229,   229,   157,   202,   229,   162,   164,   206,   229,
     256,   159,   141,   162,   268,   268,   202,   202,   159,    16,
     229,    46,   157,   164,   185,   256,   159,   229,   157,   157,
     159
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
     218,   218,   218,   219,   219,   220,   220,   220,   221,   221,
     222,   222,   223,   223,   223,   223,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   225,   225,   226,   227,   227,   228,
     228,   228,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   230,   230,   231,   231,   232,   233,   233,   234,   234,
     235,   235,   236,   237,   237,   238,   238,   239,   239,   239,
     239,   239,   240,   240,   240,   241,   241,   241,   241,   241,
     242,   242,   243,   243,   244,   245,   245,   245,   246,   247,
     247,   248,   248,   249,   250,   251,   251,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   253,   253,   253,   253,   253,   253,   253,   253,   254,
     255,   255,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   257,   257,   257,   257,   258,   258,   259,   259,   260,
     260,   260,   260,   261,   261,   262,   263,   263,   264,   264,
     265,   265,   266,   266,   267,   268,   268,   269,   269
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
       6,     9,    11,     1,     3,     1,     2,     3,     1,     1,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     4,     6,     5,     6,     3,
       6,     7,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     3,     3,     1,
       3,     4,     3,     4,     2,     4,     4,     6,     7,     3,
       5,     4,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     3,     0,     1,     3,     0,     3,     1,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     3,     5,
       3,     3,     1,     1,     1,     4,     6,     5,     4,     3,
       0,     2,     0,     1,     5,     0,     3,     5,     4,     5,
       7,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     4,     3,     4,     2,     3,
       2,     4,     3,     2,     2,     3,     2,     2,     4,     6,
       4,     1,     4,     5,     1,     4,     5,     1,     4,     5,
       4,     1,     1,     1,     1,     3,     5,     1,     3,     5,
       7,     5,     7,     1,     3,     3,     1,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10,    10
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
#line 2981 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2987 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2993 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2999 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3005 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3011 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3017 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3023 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3029 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3035 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3041 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3047 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3053 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3059 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3065 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3071 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3077 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3083 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3089 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3095 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3101 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3107 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3113 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3119 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3125 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3131 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3137 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3143 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3149 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3155 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3161 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3167 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3173 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3179 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3185 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3191 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3197 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3203 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3209 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3215 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3221 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3227 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3233 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3239 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3245 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3251 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3257 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* func_addr_expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3263 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3269 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3275 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3281 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3287 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3293 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3299 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3305 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3311 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3317 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3323 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3329 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 245: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3335 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3341 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3347 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3353 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3359 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3365 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3371 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3377 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3383 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3389 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3395 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3401 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3407 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3413 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3419 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3425 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3431 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3437 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 269: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3443 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 387 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 400 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 404 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 408 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 412 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 421 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3790 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 429 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3808 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 445 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 470 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3839 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 473 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 488 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 509 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3912 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3918 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 546 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3940 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 564 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 572 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3985 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 586 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 598 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4020 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 608 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4043 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 619 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 620 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4061 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 622 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 626 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 631 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4093 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4155 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4173 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4179 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 682 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 686 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4197 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 693 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4211 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 709 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4229 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4241 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4247 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4253 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 723 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 727 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 738 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 854 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 872 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 881 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 885 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4710 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 912 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 918 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 919 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 930 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4781 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4789 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4797 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 943 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 946 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4814 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 960 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4834 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 964 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 967 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 975 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4868 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 978 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 986 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 989 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4896 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 994 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4904 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 997 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1005 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4922 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1011 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4934 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4940 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1021 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1055 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1058 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1063 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5010 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1080 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1090 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5070 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1107 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1114 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5104 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1137 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5139 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1143 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1199 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1210 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1264 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		auto expr = new ExprAddr(tokAt((yylsp[-5])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
		(yyval.pExpression) = expr;
    }
#line 5407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5413 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1280 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1281 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1282 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1283 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1285 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1286 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1287 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1288 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1289 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1290 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1291 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1292 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1293 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1294 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1296 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1297 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1299 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1300 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1301 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1302 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1304 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1309 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1310 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5669 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5675 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1338 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1343 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1350 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1351 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1352 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5747 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1353 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1354 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1355 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1366 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1410 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1411 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5843 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1415 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5849 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1416 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1428 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1431 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1443 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1444 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1448 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1454 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1455 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1459 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1460 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1464 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1469 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1476 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5987 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1491 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5999 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1493 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1497 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1500 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1505 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1510 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6057 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1527 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1537 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6080 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6140 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1584 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6154 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1596 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1608 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6184 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1619 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-5].s),tokAt((yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-6]));
        pEnum->name = *(yyvsp[-5].s); 
		pEnum->baseType = (yyvsp[-3].type);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
		delete (yyvsp[-5].s);
    }
#line 6201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1635 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1639 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1678 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1744 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1751 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6351 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6357 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6369 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6375 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6387 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6513 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6537 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1809 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1813 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1824 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1825 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1826 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1850 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6638 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6648 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1865 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1872 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6680 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1885 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6708 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1890 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6718 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1895 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6730 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1902 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1907 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1913 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1918 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6770 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1922 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1927 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1936 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6803 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6827 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1958 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1981 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1982 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1983 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6889 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1984 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 1988 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 1995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6918 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2004 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2009 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2016 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2021 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2027 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2034 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2045 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2048 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2062 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7018 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2071 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2076 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2081 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2088 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2093 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2104 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2109 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2116 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2128 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7111 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2129 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7117 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2133 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = make_shared<ExprFor>(tokAt((yylsp[-8])));
        pFor->iterators = *(yyvsp[-7].pNameList);
        delete (yyvsp[-7].pNameList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt((yylsp[-8])));
        pAC->generatorSyntax = true;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 7136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2147 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = make_shared<ExprFor>(tokAt((yylsp[-8])));
        pFor->iterators = *(yyvsp[-7].pNameList);
        delete (yyvsp[-7].pNameList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt((yylsp[-8])));
        pAC->generatorSyntax = false;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 7155 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7159 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2163 "ds_parser.ypp" /* yacc.c:1903  */


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


