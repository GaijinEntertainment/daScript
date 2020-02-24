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
#define YYLAST   6132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  101
/* YYNRULES -- Number of rules.  */
#define YYNRULES  404
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  723

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
       0,   371,   371,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   386,   398,   399,   403,   407,   411,   420,
     428,   444,   465,   469,   472,   478,   487,   508,   531,   532,
     536,   540,   541,   545,   548,   554,   560,   563,   569,   570,
     571,   578,   585,   597,   607,   616,   617,   618,   619,   620,
     621,   625,   630,   638,   639,   640,   641,   642,   643,   644,
     645,   651,   664,   680,   681,   685,   692,   704,   708,   715,
     716,   720,   721,   722,   726,   730,   737,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   780,   850,   853,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   884,   888,   894,   900,   911,   917,
     918,   929,   933,   939,   942,   945,   949,   955,   959,   963,
     966,   969,   974,   977,   985,   988,   993,   996,  1004,  1010,
    1011,  1015,  1016,  1020,  1054,  1057,  1062,  1070,  1074,  1079,
    1085,  1089,  1094,  1103,  1106,  1112,  1113,  1120,  1131,  1132,
    1136,  1141,  1193,  1194,  1195,  1196,  1197,  1198,  1202,  1203,
    1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,
    1214,  1215,  1216,  1217,  1221,  1222,  1226,  1237,  1242,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,
    1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1294,  1298,
    1301,  1304,  1305,  1306,  1307,  1312,  1317,  1318,  1321,  1324,
    1325,  1326,  1327,  1328,  1329,  1330,  1340,  1350,  1384,  1385,
    1389,  1390,  1394,  1402,  1405,  1412,  1413,  1417,  1418,  1422,
    1428,  1429,  1433,  1434,  1438,  1443,  1446,  1450,  1456,  1465,
    1466,  1467,  1471,  1474,  1479,  1484,  1491,  1501,  1504,  1511,
    1512,  1516,  1546,  1549,  1558,  1570,  1582,  1593,  1608,  1609,
    1613,  1652,  1718,  1725,  1734,  1735,  1736,  1737,  1738,  1739,
    1740,  1741,  1742,  1743,  1744,  1745,  1746,  1747,  1748,  1749,
    1750,  1751,  1752,  1753,  1754,  1755,  1756,  1757,  1761,  1762,
    1763,  1764,  1765,  1766,  1767,  1768,  1772,  1783,  1787,  1797,
    1798,  1799,  1800,  1814,  1820,  1824,  1829,  1834,  1839,  1846,
    1851,  1855,  1859,  1864,  1869,  1876,  1881,  1887,  1892,  1896,
    1901,  1910,  1914,  1919,  1928,  1932,  1937,  1946,  1955,  1956,
    1957,  1958,  1962,  1969,  1978,  1983,  1990,  1995,  2001,  2008,
    2019,  2022,  2036,  2045,  2046,  2050,  2055,  2062,  2067,  2078,
    2083,  2090,  2102,  2103,  2107
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

#define YYPACT_NINF -466

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-466)))

#define YYTABLE_NINF -189

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -466,    18,  -466,  -466,   -48,   -82,    29,    48,    87,  -466,
     103,     3,  -466,  -466,  -466,  -466,  -466,    53,  -466,    72,
    -466,  -466,  -466,  -466,   102,  -466,    88,   118,   110,  -466,
    -466,    22,  -466,    -3,   128,  -466,  -466,   108,   153,   130,
    -466,   -79,  -466,   164,   101,  -466,   140,    51,   -48,   188,
     -82,   186,  -466,   187,   189,  -466,   220,  -466,  5177,   192,
    -466,   -48,     3,  -466,   179,   158,  5982,   291,   292,  -466,
     171,  -466,  -466,  -466,  -466,  -466,  -466,   205,    74,  -466,
    -466,  -466,  -466,   288,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,   178,   -95,   202,   203,   204,  -466,
    -466,  -466,   194,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,   219,  -466,  -466,
    -466,   224,   235,  -466,  -466,  -466,  -466,   236,  -466,  -466,
    -466,  -466,  4875,  -466,    25,  -466,   -78,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,    -8,   209,   -89,  -466,   182,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,    89,  -466,  -466,
      82,  -466,  5177,  5177,  5177,   250,  5104,  5104,  5104,  5177,
    -466,    -1,  -466,  -466,  -466,  -466,  1988,  -466,  -466,  -466,
     107,   251,  -466,  -466,  -466,   -86,  -466,   -68,  5177,   217,
    -466,  -466,   -52,  -466,  -466,    74,  -466,   -75,  3862,  -466,
    1120,   648,  1803,   223,   209,  1852,   209,  2069,   209,  2118,
    -466,    83,  4875,   244,  -466,   230,  -466,  -466,  -466,  5031,
     228,  -466,   231,   248,   252,   232,   255,   266,  3862,  3862,
      35,  5177,  3862,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    3862,  3862,  3862,  3862,  3862,  -466,  3862,  -466,  -466,   225,
    -466,  -466,     1,  -466,  -466,  -466,   253,  -466,  -466,  -466,
    -466,  5172,   249,  -466,  -466,  -466,  -466,  -466,   -48,  -466,
     368,   254,  -466,  -466,    -7,  -466,   283,  -466,  5177,  2121,
    4875,  -466,  -466,  -466,  -466,  5177,  2121,  -466,  -466,   633,
    -466,  5177,  -466,  -466,   270,  -466,   271,  -466,   278,  -466,
    -466,  5177,  -466,  -466,  2335,  -466,   293,  3862,  5177,  5177,
    3862,  5177,  5177,   321,   321,   251,  1172,  1311,  5465,  -466,
     -40,   176,   321,   321,  -466,   321,   321,  4530,   851,   -78,
    -466,  1583,   171,   414,  3862,  3862,  -466,  -466,  3862,  3862,
    3862,  3862,   313,  3862,   315,  3862,  3862,  3862,  3862,  3862,
    3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,
     316,  3862,  -466,  2254,    75,  -466,   251,  -466,  -466,  -466,
     100,  -123,  -466,   335,  5317,   212,  -466,  3943,  -466,  2601,
    -466,  -466,  -466,  -466,  2387,  1722,  4596,  2867,  2916,  4662,
    3182,  3266,    -6,   -76,  2520,  5465,   317,    61,   318,  -466,
    -466,    63,  2653,     9,   114,  3862,  3862,   296,  -466,  3862,
     327,  -466,  -466,  3862,  3862,   251,  1450,  -466,   217,  3862,
    3862,  -466,   299,   345,  2786,  1855,  -466,   434,   306,   307,
    -466,  -466,  -466,  -466,  -466,  -466,   308,   309,   311,  -466,
    -466,  -466,   458,  -466,   -88,  -466,  5409,  -466,   352,  -466,
      93,  5633,   209,  5177,   561,   561,  5875,  5875,  5854,  5854,
     320,   229,  -466,   -85,   -85,   561,   561,  5521,  5689,  5745,
     351,  5875,  5875,   413,   413,   229,   229,   229,  -466,  5245,
    -466,    94,  -466,  -466,  3862,  -466,  -466,  -123,  3862,  3862,
    3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,  3862,
    3862,  3862,  3862,  -466,  2121,  -466,  -466,  -466,    96,  5177,
     353,  4728,  -466,  3862,  3862,  -466,  3862,   325,  3862,  3862,
     352,   357,   328,   352,  3862,   331,  3862,   352,   332,   341,
    5633,  -466,  -466,  5577,   259,   259,    -2,  2121,  -466,  -466,
    4034,   488,  5633,   259,  -466,   364,   380,  5633,  2121,  -466,
    4125,   340,  -466,  -466,  -466,  -466,  -466,   468,   251,  -466,
    2919,  3052,    16,    80,  3862,  -466,   -83,  4875,  3185,  3862,
    -466,  -466,  5633,  -466,  5633,  5633,  5633,  5633,  5633,  5633,
    5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  -466,
    4216,  -466,  3810,   146,  -466,  5875,  5875,  5875,  3318,   -10,
    5633,   111,    16,  -466,   317,  5633,  -466,  -466,   134,  -466,
    -466,  -466,  -466,   148,  3862,  -466,  4307,  -466,   217,  -466,
    -466,  -466,  -466,  4398,  -466,  -466,  -466,  -466,  3862,  3862,
     349,  5633,  3451,  -466,  -466,  -466,    98,  5633,  -466,  -466,
    3590,   386,  -466,  4794,  3862,   358,  3862,   359,   217,  3862,
    -466,    57,  -466,  -466,  -466,  1017,  5801,  5801,  -466,  3862,
    5633,  -466,  5177,  4860,   373,  -466,  4461,  -466,  5633,  -466,
    -466,   259,   217,  -466,  5633,  3859,  -466,  3729,   472,   365,
     148,  -466,  -466,  5177,  4926,  3862,   369,  -466,  5900,  -466,
    5633,  -466,  -466
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    69,     1,   161,     0,     0,     0,     0,     0,   162,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   299,
       6,    11,     5,     4,    59,    61,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    64,   139,     0,    65,
      67,     0,    63,     0,     0,   300,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   302,     0,     0,
     141,     0,     0,    70,   308,     0,     0,     0,     0,    76,
      71,   297,    57,    58,    55,    56,    54,     0,     0,    53,
      62,    34,    32,    28,    25,    24,   338,   342,   341,   345,
     339,   343,   340,   344,     0,     0,     0,     0,     0,   314,
     334,   315,   347,   316,   320,   321,   322,   323,   327,   328,
     329,   330,   331,   332,   333,   335,   336,   368,   319,   326,
     337,   371,   374,   317,   324,   318,   325,     0,   346,   349,
     351,   350,   305,   140,     0,    68,     0,   310,   273,    79,
      80,    82,    81,    83,    84,    85,    86,   105,   106,   103,
     104,    96,   107,   108,    97,    94,    95,   109,   110,   111,
     112,    99,   100,    98,    92,    93,    88,    87,    89,    90,
      91,    78,    77,   101,   102,     0,    74,     0,    14,     0,
      49,    50,    47,    48,    46,    45,    51,     0,    27,   302,
       0,   306,     0,     0,     0,     0,    71,    71,    71,     0,
     355,     0,   361,   364,   363,   357,     0,   360,    66,   309,
     268,     0,   312,    72,   277,     0,   275,   284,     0,     0,
     301,   298,     0,    15,    16,     0,    60,     0,     0,   303,
       0,     0,     0,     0,    74,     0,    74,     0,    74,     0,
     280,     0,   279,   356,   359,     0,   362,   214,   215,     0,
       0,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,   184,   183,   185,   186,   187,    17,
       0,     0,     0,     0,     0,   353,     0,   134,   178,   179,
     212,   180,   210,   261,   260,   259,    69,   264,   211,   263,
     262,     0,     0,   213,   378,   379,   380,   381,     0,   311,
     270,     0,   276,    73,     0,   283,     0,   282,     0,     0,
      75,   113,   290,   291,   289,     0,     0,    52,   307,     0,
     365,     0,   367,   348,     0,   369,     0,   372,     0,   375,
     377,     0,   358,   354,   143,   146,     0,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,     0,   399,
       0,     0,   219,   218,   251,   217,   216,     0,     0,     0,
     267,     0,    71,     0,     0,     0,   240,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,     0,     0,   271,     0,   274,   278,   313,
     285,     0,   288,     0,   287,     0,   296,   188,   304,     0,
     370,   373,   376,   281,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,   390,   384,     0,   393,   394,
     395,     0,     0,     0,     0,     0,     0,     0,    20,     0,
      18,   242,   136,     0,     0,     0,     0,   147,     0,     0,
       0,   148,     0,     0,     0,     0,   116,   114,     0,     0,
     129,   124,   122,   123,   135,   117,     0,     0,     0,   127,
     128,   130,   159,   121,     0,   118,   188,   246,     0,   247,
       0,   173,    74,     0,   220,   221,   233,   234,   231,   232,
       0,   256,   245,   265,   266,   222,   223,     0,   236,   237,
     235,   229,   230,   225,   224,   226,   227,   228,   244,     0,
     249,     0,   269,   272,     0,   179,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,     0,   295,   366,   144,     0,     0,
       0,     0,   252,     0,     0,   253,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     392,   400,   401,     0,     0,     0,     0,     0,   149,   152,
     188,     0,   142,     0,   133,     0,     0,    37,     0,   156,
     188,     0,   131,   132,   120,   125,   126,     0,     0,   119,
       0,     0,     0,     0,     0,   248,     0,   258,     0,     0,
     243,   250,   286,   137,   195,   196,   198,   197,   199,   192,
     193,   194,   200,   201,   190,   191,   202,   203,   189,   294,
     188,   145,     0,     0,   167,   164,   165,   166,     0,     0,
     382,     0,     0,   386,   385,   391,   397,   396,     0,   388,
     398,    19,    43,    38,     0,   153,   188,   150,     0,    44,
      35,    36,   157,   188,   154,   134,   160,   163,     0,     0,
       0,   174,     0,   175,   181,   207,     0,   257,   293,   170,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
      41,     0,   151,   158,   155,     0,   205,   204,   206,     0,
     176,   208,     0,     0,     0,   255,   402,   387,   383,   389,
      39,     0,     0,   115,   177,     0,   168,     0,     0,     0,
      38,    42,   171,     0,     0,     0,     0,    40,     0,   169,
     403,   404,   172
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -466,  -466,  -466,   174,   480,  -466,  -466,  -466,  -466,  -466,
    -466,   448,  -466,  -466,   482,  -466,  -466,  -173,  -466,  -466,
    -466,  -466,   314,  -466,   507,   -58,  -466,   498,  -466,   275,
    -171,  -222,  -466,  -466,  -215,  -466,   -93,  -292,   -11,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,   562,  -466,  -466,
    -466,  -383,  -466,  -466,  -371,  -466,  -353,  -466,  -466,  -466,
     165,  -466,  -466,  -466,  -466,   260,  -466,   234,  -466,  -211,
    -203,   161,   -21,  -466,  -466,  -466,   381,  -466,  -466,  -466,
    -466,  -466,  -175,   -57,  -466,  -466,  -466,   -49,   322,  -465,
    -340,  -466,  -466,  -242,    17,   227,  -466,  -466,  -466,  -466,
    -466
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   179,   185,   351,   280,    14,    15,    31,
      32,    55,    16,    28,    29,   458,   459,   680,   460,   461,
     462,   463,   186,   187,    25,    26,    39,    40,    41,    17,
     176,   219,    70,    18,   281,   464,   358,   465,   282,   466,
     283,   467,   468,   469,   470,   471,   588,   472,   473,   284,
     285,   480,   664,   286,   287,   288,   403,   475,   289,   290,
     481,   300,   396,   301,   210,   214,   215,   240,   241,   549,
     216,   316,   221,   177,    46,    20,    95,    21,    22,   137,
      65,    23,   217,   292,    94,   130,   131,   242,   293,   426,
     427,   294,   428,   429,   430,   431,   295,   350,   296,   709,
     297
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   129,   222,   134,   311,   474,   309,   433,   302,   132,
     511,   548,   324,   593,   326,   644,   328,   402,     2,   372,
     349,   517,   374,     3,   406,   234,   236,   238,   243,   305,
     516,   538,   190,    36,   662,   211,   211,   305,   212,    27,
     277,    51,   278,   515,     4,   345,     5,   128,     6,    37,
       7,    42,   190,   312,    62,     8,    96,    43,   307,    44,
     313,     9,    97,    72,    73,   306,   307,    10,   308,   191,
     390,   391,    52,   303,   589,   220,   304,    98,    63,    24,
     277,   306,   314,    38,   315,    59,   180,   181,   634,   318,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   212,
     212,   305,   436,   594,   437,   209,    45,   306,   360,   200,
      37,   306,    66,    56,   201,   129,   129,   129,   244,   129,
     129,   129,   129,   230,   231,   232,   603,   235,   237,   239,
     307,   213,   674,   202,   569,   245,    30,   678,    48,   361,
      57,   129,    37,   579,    38,   629,   246,    53,   679,   310,
     422,   553,    74,   558,    11,    33,    75,    54,    76,    77,
      12,   128,   128,   128,   208,   128,   128,   128,   128,   514,
     594,   482,   129,   513,   561,   182,    38,    67,    68,   183,
     334,   184,    77,   129,   129,   305,   203,   128,    48,    78,
     631,   346,   347,   551,    34,   666,   228,   638,   552,   702,
     555,    48,   225,   553,   330,   556,   594,   594,    69,   594,
      35,   594,   512,   571,   307,   204,    47,   660,   128,   205,
     394,   200,   619,    50,   229,   331,   201,   363,   226,   128,
     128,   129,   595,   601,    49,   621,   206,   691,   129,   400,
     596,   681,    58,   298,   129,   202,   405,   207,   675,    59,
     566,   299,   409,   553,   129,   645,   556,   363,   559,    86,
      60,   129,   129,    87,   129,   129,   652,   670,    61,   417,
     418,    64,   420,   421,    88,    89,   553,   128,   677,    90,
      91,    92,    93,    71,   128,   178,   438,   439,   671,    81,
     128,   223,   224,    83,    84,   136,    85,   312,   203,   133,
     128,   138,   173,   174,   313,   366,   367,   128,   128,   175,
     128,   128,   474,   372,   178,   373,   374,   375,    52,   363,
     376,   189,   192,   193,   194,   218,   314,   204,   477,   642,
     643,   205,   195,   364,   365,   366,   367,   368,   649,   196,
     369,   370,   371,   372,   197,   373,   374,   375,   206,   363,
     376,   291,   377,   378,   533,   198,   199,   233,   212,   207,
     277,   663,   323,   332,   390,   391,   336,   333,   338,   337,
     340,   359,   339,   319,   379,   341,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   342,   393,   395,    11,
     399,   410,   411,   222,   390,   391,   397,   366,   367,   412,
     415,   676,   277,   343,   344,   372,   129,   348,   374,   375,
     483,   363,   376,   683,   597,   352,   353,   354,   355,   356,
     490,   357,   492,   508,   517,   364,   365,   366,   367,   368,
     551,   554,   369,   370,   371,   372,   223,   373,   374,   375,
     562,   574,   376,   700,   377,   378,   575,   581,   582,   583,
     584,   585,   128,   586,   404,   587,   390,   391,   598,   592,
     623,   407,   129,   628,   632,   633,   710,   711,   636,   639,
     622,   383,   384,   385,   386,   387,   388,   389,   640,   648,
     650,   651,   416,   655,   656,   419,   390,   391,   688,   366,
     367,   425,   425,   694,   707,   697,   699,   372,   715,   373,
     374,   375,   716,   476,   376,   440,   721,    79,   128,   484,
     485,   188,    82,   486,   487,   488,   489,   717,   491,   317,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,    80,   509,   387,   388,   389,
     135,   362,   685,    19,   398,   413,   534,   657,   390,   391,
     227,   335,     0,   637,   434,     0,     0,     0,     0,   363,
     541,     0,     0,     0,     0,     0,     0,     0,     0,   357,
       0,     0,     0,     0,     0,     0,     0,   357,     0,     0,
     560,   348,     0,     0,   563,     0,     0,     0,   564,   565,
       0,   570,     0,     0,   572,   573,     0,     0,     0,   577,
     580,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,   705,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,   129,   366,   367,     0,
       0,     0,     0,     0,   718,   372,     0,   373,   374,   375,
       0,     0,   376,     0,     0,     0,     0,   200,     0,   602,
       0,   128,   201,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,   614,   615,   616,   617,   618,     0,   620,
       0,   202,   128,   385,   386,   387,   388,   389,   625,   626,
       0,   627,     0,     0,   630,     0,   390,   391,     0,   635,
       0,   425,     0,     0,     0,     0,     0,   364,   365,   366,
     367,   368,   646,     0,   369,   370,   371,   372,     0,   373,
     374,   375,     0,   653,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,   203,   614,   618,     0,     0,   661,
       0,     0,     0,     0,   667,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,   204,     0,     0,     0,   205,   390,   391,
       0,     0,     0,   673,     0,   408,     0,     0,     0,     0,
       0,     0,     0,     0,   206,     0,     0,     0,     0,     0,
     321,     0,     0,     0,     0,   207,     0,     0,     0,     0,
       0,     0,     0,   686,   687,     0,     0,   690,     0,     0,
       0,     0,     0,     0,     0,   693,     0,     0,     0,   696,
       0,   698,     0,     0,   701,     0,     0,     0,     0,     0,
     476,     0,   442,     0,   704,     0,     3,     0,   443,   444,
       0,   445,     0,   247,   248,   249,   250,     0,     0,     0,
       0,     0,   714,   446,   251,   447,   448,     0,     0,     0,
     720,     0,     0,     0,     0,   449,   252,     0,     0,   450,
     253,     0,   254,     0,     9,   255,   451,     0,   452,   256,
       0,     0,   453,   454,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,     0,     0,     0,
     273,   274,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,    38,   456,   277,   457,   278,   279,   442,     0,
       0,     0,     3,     0,   443,   444,     0,   445,     0,   247,
     248,   249,   250,     0,     0,     0,     0,     0,     0,   446,
     251,   447,   448,     0,     0,     0,     0,     0,     0,     0,
       0,   449,   252,     0,     0,   450,   253,     0,   254,     0,
       9,   255,   451,     0,   452,   256,     0,     0,   453,   454,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
       0,   257,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,   200,
       0,     0,     0,     0,   201,     0,     0,     0,     0,   270,
     271,   272,     0,     0,     0,     0,   273,   274,     0,     0,
       0,     0,     0,   202,     0,   276,     0,     0,    38,   456,
     277,   703,   278,   279,   247,   248,   249,   250,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,   201,   252,     0,     0,
       0,   253,     0,   254,     0,     0,   255,     0,     0,     0,
     256,     0,     0,     0,     0,   202,   203,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,   204,   257,     0,     0,   205,
       0,   320,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,     0,     0,     0,   206,     0,   203,     0,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   423,
     269,     0,     0,     0,     0,     0,     0,   204,     0,     0,
       0,   205,     0,     0,   270,   271,   272,     0,     0,     0,
       0,   273,   274,   247,   248,   249,   250,     0,   206,     0,
     424,     0,     0,    38,   251,   277,     0,   278,   279,   207,
     200,     0,     0,     0,     0,   201,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,     0,     0,   202,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   423,   269,
       0,     0,     0,     0,     0,     0,   204,     0,     0,     0,
     205,     0,     0,   270,   271,   272,     0,     0,     0,     0,
     273,   274,   247,   248,   249,   250,     0,   206,     0,   432,
       0,     0,    38,   251,   277,     0,   278,   279,   207,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
       0,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,     0,     0,   567,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,     0,     0,     0,   251,     0,   276,     0,
       0,    38,   568,   277,     0,   278,   279,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,     0,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,     0,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     0,     0,
       0,     0,   273,   274,   247,   248,   249,   250,   539,   478,
       0,   276,   479,     0,    38,   251,   277,     0,   278,   279,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   253,     0,   254,     0,     0,   255,     0,     0,     0,
     256,     0,     0,     0,     0,     0,     0,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,     0,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   200,     0,     0,     0,     0,   201,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    37,
     269,     0,     0,     0,     0,     0,   202,     0,     0,     0,
       0,     0,   540,     0,   270,   271,   272,   247,   248,   249,
     250,   273,   274,     0,     0,     0,     0,     0,   251,     0,
     276,   200,     0,    38,     0,   277,   201,   278,   279,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,   202,     0,     0,     0,   203,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,   204,   257,
       0,     0,   205,     0,   322,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,   203,   206,
     578,     0,     0,     0,     0,   401,     0,     0,     0,     0,
     207,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,   204,     0,     0,
       0,   205,     0,   325,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,   206,     0,
       0,   251,     0,   276,     0,     0,    38,     0,   277,   207,
     278,   279,     0,   252,     0,     0,     0,   253,     0,   254,
       0,     0,   255,     0,     0,     0,   256,     0,     0,     0,
       0,     0,     0,    99,   100,   101,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   118,   119,   120,     0,     0,   123,   124,   125,
     126,     0,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   258,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   200,     0,
       0,     0,     0,   201,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   267,   268,    37,   269,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
     270,   271,   272,   247,   248,   249,   250,   273,   274,     0,
       0,     0,     0,     0,   251,   275,   276,   200,     0,    38,
       0,   277,   201,   278,   279,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,   202,     0,     0,     0,   203,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,   204,   257,     0,     0,   205,     0,
     327,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,   203,   206,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,   207,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,   204,     0,     0,     0,   205,     0,   329,
       0,     0,     0,   270,   271,   272,   247,   248,   249,   250,
     273,   274,     0,     0,   206,     0,     0,   251,     0,   276,
       0,     0,    38,     0,   277,   207,   278,   279,     0,   252,
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
     251,     0,   276,   510,     0,    38,     0,   277,     0,   278,
     279,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,   203,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
     204,   257,     0,     0,   205,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,   206,     0,   414,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     271,   272,   247,   248,   249,   250,   273,   274,     0,     0,
       0,     0,     0,   251,     0,   276,   537,     0,    38,     0,
     277,     0,   278,   279,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
       0,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     200,     0,     0,     0,     0,   201,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,     0,     0,   202,     0,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,     0,     0,     0,   251,     0,   276,   550,
       0,    38,     0,   277,     0,   278,   279,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,   203,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,   204,   257,     0,     0,
     205,     0,   536,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   247,   248,
     249,   250,   273,   274,     0,     0,     0,     0,     0,   251,
       0,   276,   557,     0,    38,     0,   277,     0,   278,   279,
       0,   252,     0,     0,     0,   253,     0,   254,     0,     0,
     255,     0,     0,     0,   256,     0,     0,   576,     0,     0,
       0,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,     0,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   200,     0,     0,     0,
       0,   201,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,    37,   269,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,     0,     0,   270,   271,
     272,   247,   248,   249,   250,   273,   274,     0,     0,     0,
       0,     0,   251,     0,   276,   200,     0,    38,     0,   277,
     201,   278,   279,     0,   252,     0,     0,     0,   253,     0,
     254,     0,     0,   255,     0,     0,     0,   256,     0,   202,
       0,     0,     0,   203,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,   204,   257,     0,     0,   205,     0,   543,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,   203,   206,     0,     0,     0,     0,     0,   658,
       0,     0,     0,     0,   207,     0,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,   204,     0,     0,     0,   205,     0,   544,     0,     0,
       0,   270,   271,   272,   247,   248,   249,   250,   273,   274,
       0,     0,   206,     0,     0,   251,     0,   276,     0,     0,
      38,     0,   277,   207,   278,   279,     0,   252,     0,     0,
       0,   253,     0,   254,     0,     0,   255,     0,     0,     0,
     256,     0,     0,     0,     0,     0,     0,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,     0,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   659,     0,     0,     0,     0,     0,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    37,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   271,   272,   247,   248,   249,
     250,   273,   274,     0,     0,     0,     0,     0,   251,     0,
     276,   200,     0,    38,     0,   277,   201,   278,   279,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,   202,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,   203,     0,
       0,     0,     0,     0,     0,   200,     0,     0,     0,     0,
     201,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,   204,     0,   202,
       0,   205,     0,   546,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,   206,     0,
       0,   251,     0,   276,   665,     0,    38,     0,   277,   207,
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
       0,     0,     0,     0,   251,     0,   276,   672,     0,    38,
       0,   277,     0,   278,   279,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,   689,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,     0,     0,     0,
     273,   274,   247,   248,   249,   250,   692,     0,     0,   276,
       0,     0,    38,   251,   277,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
       0,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,   271,   272,     0,     0,     0,     0,   273,
     274,   247,   248,   249,   250,   713,     0,     0,   276,     0,
       0,    38,   251,   277,     0,   278,   279,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   253,     0,
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
       0,     0,     0,     0,     0,   251,     0,   276,   200,     0,
      38,     0,   277,   201,   278,   279,     0,   252,     0,     0,
       0,   253,     0,   254,     0,     0,   255,     0,     0,     0,
     256,     0,   202,     0,     0,     0,   203,    99,   100,   101,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   118,   119,   120,     0,
       0,   123,   124,   125,   126,   204,   257,     0,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,   258,   259,
       0,   363,     0,     0,     0,   203,   206,     0,     0,   669,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    37,
     269,     0,     0,     0,   204,     0,     0,     0,   205,     0,
       0,     0,     0,     0,   270,   271,   272,     0,     0,     0,
       0,   273,   274,     0,     0,   206,     0,     0,   712,     0,
     276,     0,     0,    38,     0,   277,   207,   278,   279,   518,
     519,   520,   521,   522,   523,   524,   525,   364,   365,   366,
     367,   368,   526,   527,   369,   370,   371,   372,   528,   373,
     374,   375,   363,     0,   376,   529,   377,   378,   530,   531,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   532,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,     0,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,   521,   522,   523,   524,   525,   364,   365,
     366,   367,   368,   526,   527,   369,   370,   371,   372,   528,
     373,   374,   375,   363,     0,   376,   529,   377,   378,   530,
     531,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   532,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     391,     0,     0,     0,     0,     0,   647,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,   525,   364,
     365,   366,   367,   368,   526,   527,   369,   370,   371,   372,
     528,   373,   374,   375,   363,     0,   376,   529,   377,   378,
     530,   531,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   532,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,     0,     0,     0,     0,     0,   654,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,   521,   522,   523,   524,   525,
     364,   365,   366,   367,   368,   526,   527,   369,   370,   371,
     372,   528,   373,   374,   375,   363,     0,   376,   529,   377,
     378,   530,   531,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     532,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,     0,     0,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,     0,     0,     0,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
     525,   364,   365,   366,   367,   368,   526,   527,   369,   370,
     371,   372,   528,   373,   374,   375,   363,     0,   376,   529,
     377,   378,   530,   531,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   532,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   391,     0,     0,     0,     0,     0,   682,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,   518,   519,   520,   521,   522,   523,
     524,   525,   364,   365,   366,   367,   368,   526,   527,   369,
     370,   371,   372,   528,   373,   374,   375,     0,     0,   376,
     529,   377,   378,   530,   531,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   532,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,   363,     0,
       0,     0,     0,   390,   391,   364,   365,   366,   367,   368,
     684,     0,   369,   370,   371,   372,     0,   373,   374,   375,
       0,     0,   376,     0,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,     0,     0,     0,   363,     0,   390,   391,     0,     0,
       0,     0,     0,   708,   364,   365,   366,   367,   368,     0,
       0,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
     363,     0,     0,     0,     0,   390,   391,     0,     0,   441,
     364,   365,   366,   367,   368,     0,     0,   369,   370,   371,
     372,     0,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,     0,     0,     0,   363,     0,     0,     0,
       0,   390,   391,     0,     0,   542,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,     0,   363,     0,     0,     0,     0,   390,   391,     0,
       0,   545,   364,   365,   366,   367,   368,     0,     0,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,   363,     0,
       0,     0,     0,   390,   391,     0,     0,   624,   364,   365,
     366,   367,   368,     0,     0,   369,   370,   371,   372,     0,
     373,   374,   375,     0,   200,   376,     0,   377,   378,   201,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   202,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,   363,     0,     0,     0,     0,   390,
     391,     0,     0,   695,   364,   365,   366,   367,   368,     0,
       0,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
     204,     0,     0,     0,   205,   390,   391,     0,     0,   706,
     364,   365,   366,   367,   368,     0,     0,   369,   370,   371,
     372,   206,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,    97,     0,
       0,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,    98,     0,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,   719,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,   260,
     261,   262,     0,     0,     0,     0,     0,     0,    37,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,     0,     0,
     363,     0,    38,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,   175,   363,     0,    38,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   390,   391,   392,
       0,     0,     0,     0,     0,   363,     0,     0,    38,   364,
     365,   366,   367,   368,     0,     0,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,   600,   518,   519,   520,   521,   522,   523,   524,
     525,   364,   365,   366,   367,   368,   526,   527,   369,   370,
     371,   372,   528,   373,   374,   375,  -188,   363,   376,   529,
     377,   378,   530,   531,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   532,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,   525,   364,   365,   366,   367,   368,   526,   527,
     369,   370,   371,   372,   590,   373,   374,   375,     0,     0,
     376,   529,   377,   378,   530,   531,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,   591,   379,     0,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,   364,
     365,   366,   367,   368,   390,   391,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,   364,   365,   366,   367,   368,
     390,   391,   369,   370,   371,   372,     0,   373,   374,   375,
       0,     0,   376,     0,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,   379,   599,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,   364,   365,   366,   367,   368,   390,   391,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,   641,
       0,     0,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,   364,   365,   366,
     367,   368,   390,   391,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,   364,   365,   366,   367,   368,   390,   391,
     369,   370,   371,   372,     0,   373,   374,   375,     0,     0,
     376,     0,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,   364,
     365,   366,   367,   368,   390,   391,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   382,   383,   384,   385,   386,   387,
     388,   389,     0,   363,     0,   364,   365,     0,     0,   368,
     390,   391,   369,   370,   371,   372,     0,   373,   374,   375,
       0,     0,   376,     0,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   200,
       0,     0,     0,     0,   201,     0,   379,     0,   380,   381,
     382,   383,   384,     0,     0,     0,   388,   389,   364,   365,
     366,   367,   368,   202,     0,   369,   390,   391,   372,     0,
     373,   374,   375,     0,     0,   376,     0,   377,   378,   364,
     365,   366,   367,     0,     0,     0,     0,     0,     0,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,     0,     0,   203,     0,     0,   390,
     391,     0,     0,     0,     0,     0,     0,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,     0,     0,     0,   204,     0,     0,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   206,     0,     0,   722,
       0,     0,     0,     0,     0,     0,     0,   207,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,   158,   159,   160,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,   171,   172
};

static const yytype_int16 yycheck[] =
{
      11,    58,   177,    61,   219,   358,   217,   347,   211,    58,
     393,    17,   234,   478,   236,    17,   238,   309,     0,   104,
     262,   109,   107,     5,   316,   196,   197,   198,    29,   105,
     401,   414,   127,    30,   117,    43,    43,   105,   127,   121,
     163,    19,   165,   166,    26,    10,    28,    58,    30,   127,
      32,    62,   127,   105,   133,    37,    21,     4,   134,     6,
     112,    43,    27,    12,    13,   133,   134,    49,   136,   164,
     155,   156,    50,   159,   162,   164,   162,    42,   157,   127,
     163,   133,   134,   161,   136,   161,    12,    13,   553,   164,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,   127,
     127,   105,   162,   133,   164,   136,    54,   133,   127,    29,
     127,   133,    31,   136,    34,   192,   193,   194,   139,   196,
     197,   198,   199,   192,   193,   194,   517,   196,   197,   198,
     134,   159,   162,    53,   446,   156,   127,     9,   133,   158,
     163,   218,   127,   455,   161,   548,   167,   145,    20,   218,
     345,   162,   121,   164,   156,   127,   125,   155,   127,   128,
     162,   192,   193,   194,   159,   196,   197,   198,   199,   400,
     133,   362,   249,   396,   436,   121,   161,    96,    97,   125,
     249,   127,   128,   260,   261,   105,   106,   218,   133,   158,
     550,   260,   261,   133,   127,   598,   134,   557,   157,   162,
     157,   133,   133,   162,   141,   162,   133,   133,   127,   133,
     127,   133,   157,   448,   134,   135,   134,   157,   249,   139,
     298,    29,   534,   133,   162,   162,    34,    18,   159,   260,
     261,   308,   159,   159,   136,   159,   156,   159,   315,   308,
     482,   644,   134,   156,   321,    53,   315,   167,   157,   161,
     445,   164,   321,   162,   331,   567,   162,    18,   164,    59,
     127,   338,   339,    63,   341,   342,   578,   141,   158,   338,
     339,   127,   341,   342,    74,    75,   162,   308,   164,    79,
      80,    81,    82,   163,   315,   129,   130,   131,   162,   121,
     321,   129,   130,   127,   127,   136,   127,   105,   106,   127,
     331,   163,    31,    31,   112,    96,    97,   338,   339,   158,
     341,   342,   685,   104,   129,   106,   107,   108,    50,    18,
     111,   163,   140,   140,   140,   136,   134,   135,   359,   564,
     565,   139,   158,    94,    95,    96,    97,    98,   573,   140,
     101,   102,   103,   104,   140,   106,   107,   108,   156,    18,
     111,   206,   113,   114,   162,   140,   140,   127,   127,   167,
     163,   596,   159,   139,   155,   156,   158,   157,   140,   158,
     158,   166,   140,   228,   135,   140,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   140,   158,    40,   156,
     127,   141,   141,   588,   155,   156,   162,    96,    97,   141,
     127,   632,   163,   258,   259,   104,   483,   262,   107,   108,
      16,    18,   111,   648,   483,   270,   271,   272,   273,   274,
     127,   276,   127,   127,   109,    94,    95,    96,    97,    98,
     133,   133,   101,   102,   103,   104,   129,   106,   107,   108,
     164,   162,   111,   678,   113,   114,   121,    33,   162,   162,
     162,   162,   483,   162,   309,    17,   155,   156,   158,   127,
     127,   316,   539,   158,   127,   157,   701,   702,   157,   157,
     539,   140,   141,   142,   143,   144,   145,   146,   157,    11,
     136,   121,   337,   163,    36,   340,   155,   156,   159,    96,
      97,   346,   347,   127,   141,   157,   157,   104,    46,   106,
     107,   108,   157,   358,   111,   351,   157,    47,   539,   364,
     365,    83,    50,   368,   369,   370,   371,   710,   373,   225,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,    48,   391,   144,   145,   146,
      62,   286,   655,     1,   304,   331,   405,   588,   155,   156,
     189,   249,    -1,   556,   347,    -1,    -1,    -1,    -1,    18,
     415,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   424,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   432,    -1,    -1,
     435,   436,    -1,    -1,   439,    -1,    -1,    -1,   443,   444,
      -1,   446,    -1,    -1,   449,   450,    -1,    -1,    -1,   454,
     455,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   692,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   692,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   713,    96,    97,    -1,
      -1,    -1,    -1,    -1,   713,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    29,    -1,   514,
      -1,   692,    34,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,    -1,   534,
      -1,    53,   713,   142,   143,   144,   145,   146,   543,   544,
      -1,   546,    -1,    -1,   549,    -1,   155,   156,    -1,   554,
      -1,   556,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,   567,    -1,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,   578,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   590,   591,    -1,    -1,   594,
      -1,    -1,    -1,    -1,   599,    -1,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,   135,    -1,    -1,    -1,   139,   155,   156,
      -1,    -1,    -1,   628,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   658,   659,    -1,    -1,   662,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   670,    -1,    -1,    -1,   674,
      -1,   676,    -1,    -1,   679,    -1,    -1,    -1,    -1,    -1,
     685,    -1,     1,    -1,   689,    -1,     5,    -1,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,   707,    22,    23,    24,    25,    -1,    -1,    -1,
     715,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    45,    -1,    47,    48,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,   165,   166,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    -1,    38,    39,    -1,    41,    -1,
      43,    44,    45,    -1,    47,    48,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,    -1,    -1,    53,    -1,   158,    -1,    -1,   161,   162,
     163,   164,   165,   166,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    53,   106,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,   135,    84,    -1,    -1,   139,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,   139,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,   150,    12,    13,    14,    15,    -1,   156,    -1,
     158,    -1,    -1,   161,    23,   163,    -1,   165,   166,   167,
      29,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,
      39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
     139,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,   150,    12,    13,    14,    15,    -1,   156,    -1,   158,
      -1,    -1,   161,    23,   163,    -1,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,    12,    13,    14,    15,   149,
     150,    -1,    -1,    -1,    -1,    -1,    23,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,   165,   166,    -1,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,   150,    12,    13,    14,    15,    16,   156,
      -1,   158,   159,    -1,   161,    23,   163,    -1,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,   143,   144,    12,    13,    14,
      15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     158,    29,    -1,   161,    -1,   163,    34,   165,   166,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    53,    -1,    -1,    -1,   106,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,   135,    84,
      -1,    -1,   139,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   106,   156,
     105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
     167,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,   135,    -1,    -1,
      -1,   139,    -1,   141,    -1,    -1,    -1,   142,   143,   144,
      12,    13,    14,    15,   149,   150,    -1,    -1,   156,    -1,
      -1,    23,    -1,   158,    -1,    -1,   161,    -1,   163,   167,
     165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    80,    81,
      82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,    12,    13,    14,    15,   149,   150,    -1,
      -1,    -1,    -1,    -1,    23,   157,   158,    29,    -1,   161,
      -1,   163,    34,   165,   166,    -1,    35,    -1,    -1,    -1,
      39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    53,    -1,    -1,    -1,   106,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,   135,    84,    -1,    -1,   139,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,   106,   156,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,   167,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,   141,
      -1,    -1,    -1,   142,   143,   144,    12,    13,    14,    15,
     149,   150,    -1,    -1,   156,    -1,    -1,    23,    -1,   158,
      -1,    -1,   161,    -1,   163,   167,   165,   166,    -1,    35,
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
      23,    -1,   158,   159,    -1,   161,    -1,   163,    -1,   165,
     166,    -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,   106,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    80,    81,    82,
     135,    84,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,    12,    13,    14,    15,   149,   150,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   158,   159,    -1,   161,    -1,
     163,    -1,   165,   166,    -1,    35,    -1,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    34,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,    12,    13,    14,    15,   149,
     150,    -1,    -1,    -1,    -1,    -1,    23,    -1,   158,   159,
      -1,   161,    -1,   163,    -1,   165,   166,    -1,    35,    -1,
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
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    34,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,    12,    13,    14,    15,   149,   150,    -1,    -1,    -1,
      -1,    -1,    23,    -1,   158,    29,    -1,   161,    -1,   163,
      34,   165,   166,    -1,    35,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    53,
      -1,    -1,    -1,   106,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,   135,    84,    -1,    -1,   139,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,   106,   156,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,   167,    -1,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,   135,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,
      -1,   142,   143,   144,    12,    13,    14,    15,   149,   150,
      -1,    -1,   156,    -1,    -1,    23,    -1,   158,    -1,    -1,
     161,    -1,   163,   167,   165,   166,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,    12,    13,    14,
      15,   149,   150,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     158,    29,    -1,   161,    -1,   163,    34,   165,   166,    -1,
      35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    53,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,   135,    -1,    53,
      -1,   139,    -1,   141,    -1,    -1,    -1,   142,   143,   144,
      12,    13,    14,    15,   149,   150,    -1,    -1,   156,    -1,
      -1,    23,    -1,   158,   159,    -1,   161,    -1,   163,   167,
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
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,   150,    12,    13,    14,    15,    16,    -1,    -1,   158,
      -1,    -1,   161,    23,   163,    -1,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,
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
     150,    12,    13,    14,    15,    16,    -1,    -1,   158,    -1,
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
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,    12,    13,    14,    15,   149,   150,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   158,    29,    -1,
     161,    -1,   163,    34,   165,   166,    -1,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    53,    -1,    -1,    -1,   106,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,   135,    84,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    18,    -1,    -1,    -1,   106,   156,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,   156,    -1,    -1,   159,    -1,
     158,    -1,    -1,   161,    -1,   163,   167,   165,   166,    86,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    18,    -1,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    18,    -1,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    -1,    -1,   111,
     112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   155,   156,    94,    95,    96,    97,    98,
     162,    -1,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,   162,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,   159,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,   159,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,
      96,    97,    98,    -1,    -1,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,    29,   111,    -1,   113,   114,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,   159,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
     135,    -1,    -1,    -1,   139,   155,   156,    -1,    -1,   159,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,   156,   106,   107,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,   159,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,   118,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,   127,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      18,    -1,   161,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,   158,    18,    -1,   161,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   161,    94,
      95,    96,    97,    98,    -1,    -1,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,   157,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    18,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    -1,
     111,   112,   113,   114,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    94,
      95,    96,    97,    98,   155,   156,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    94,    95,    96,    97,    98,
     155,   156,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    94,    95,    96,    97,    98,   155,   156,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,    -1,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,   132,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    94,    95,    96,
      97,    98,   155,   156,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    94,    95,    96,    97,    98,   155,   156,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,    -1,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    94,
      95,    96,    97,    98,   155,   156,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,    -1,    18,    -1,    94,    95,    -1,    -1,    98,
     155,   156,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,    -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    34,    -1,   135,    -1,   137,   138,
     139,   140,   141,    -1,    -1,    -1,   145,   146,    94,    95,
      96,    97,    98,    53,    -1,   101,   155,   156,   104,    -1,
     106,   107,   108,    -1,    -1,   111,    -1,   113,   114,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,   149,   150
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
     133,    19,    50,   145,   155,   179,   136,   163,   134,   161,
     127,   158,   133,   157,   127,   248,    31,    96,    97,   127,
     200,   163,    12,    13,   121,   125,   127,   128,   158,   172,
     192,   121,   182,   127,   127,   127,    59,    63,    74,    75,
      79,    80,    81,    82,   252,   244,    21,    27,    42,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   206,   251,
     253,   254,   255,   127,   193,   195,   136,   247,   163,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   113,   114,   115,
     116,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   149,   150,    31,    31,   158,   198,   241,   129,   171,
      12,    13,   121,   125,   127,   172,   190,   191,   179,   163,
     127,   164,   140,   140,   140,   158,   140,   140,   140,   140,
      29,    34,    53,   106,   135,   139,   156,   167,   159,   206,
     232,    43,   127,   159,   233,   234,   238,   250,   136,   199,
     164,   240,   250,   129,   130,   133,   159,   244,   134,   162,
     255,   255,   255,   127,   198,   255,   198,   255,   198,   255,
     235,   236,   255,    29,   139,   156,   167,    12,    13,    14,
      15,    23,    35,    39,    41,    44,    48,    84,    96,    97,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   128,
     142,   143,   144,   149,   150,   157,   158,   163,   165,   166,
     174,   202,   206,   208,   217,   218,   221,   222,   223,   226,
     227,   228,   251,   256,   259,   264,   266,   268,   156,   164,
     229,   231,   238,   159,   162,   105,   133,   134,   136,   237,
     255,   202,   105,   112,   134,   136,   239,   190,   164,   228,
     141,   162,   141,   159,   199,   141,   199,   141,   199,   141,
     141,   162,   139,   157,   255,   256,   158,   158,   140,   140,
     158,   140,   140,   228,   228,    10,   255,   255,   228,   261,
     265,   173,   228,   228,   228,   228,   228,   228,   204,   166,
     127,   158,   197,    18,    94,    95,    96,    97,    98,   101,
     102,   103,   104,   106,   107,   108,   111,   113,   114,   135,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     155,   156,   157,   158,   193,    40,   230,   162,   233,   127,
     255,   110,   205,   224,   228,   255,   205,   228,   162,   255,
     141,   141,   141,   235,   158,   127,   228,   255,   255,   228,
     255,   255,   250,   127,   158,   228,   257,   258,   260,   261,
     262,   263,   158,   258,   263,   117,   162,   164,   130,   131,
     171,   159,     1,     7,     8,    10,    22,    24,    25,    34,
      38,    45,    47,    51,    52,    85,   162,   164,   183,   184,
     186,   187,   188,   189,   203,   205,   207,   209,   210,   211,
     212,   213,   215,   216,   224,   225,   228,   206,   156,   159,
     219,   228,   198,    16,   228,   228,   228,   228,   228,   228,
     127,   228,   127,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   127,   228,
     159,   219,   157,   238,   237,   166,   222,   109,    86,    87,
      88,    89,    90,    91,    92,    93,    99,   100,   105,   112,
     115,   116,   134,   162,   239,   162,   141,   159,   219,    16,
     140,   228,   159,   141,   141,   159,   141,   141,    17,   237,
     159,   133,   157,   162,   133,   157,   162,   159,   164,   164,
     228,   261,   164,   228,   228,   228,   250,   105,   162,   205,
     228,   202,   228,   228,   162,   121,    51,   228,   105,   205,
     228,    33,   162,   162,   162,   162,   162,    17,   214,   162,
     105,   134,   127,   257,   133,   159,   199,   255,   158,   136,
     157,   159,   228,   222,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   205,
     228,   159,   255,   127,   159,   228,   228,   228,   158,   219,
     228,   258,   127,   157,   257,   228,   157,   262,   258,   157,
     157,   132,   202,   202,    17,   205,   228,   162,    11,   202,
     136,   121,   205,   228,   162,   163,    36,   240,   110,   110,
     157,   228,   117,   202,   220,   159,   219,   228,   162,   159,
     141,   162,   159,   228,   162,   157,   237,   164,     9,    20,
     185,   219,   162,   202,   162,   204,   228,   228,   159,   105,
     228,   159,    16,   228,   127,   159,   228,   157,   228,   157,
     202,   228,   162,   164,   228,   255,   159,   141,   162,   267,
     202,   202,   159,    16,   228,    46,   157,   185,   255,   159,
     228,   157,   159
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
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     230,   230,   231,   232,   232,   233,   233,   234,   234,   235,
     236,   236,   237,   237,   238,   238,   238,   238,   238,   239,
     239,   239,   240,   240,   240,   240,   240,   241,   241,   242,
     242,   243,   244,   244,   244,   245,   246,   246,   247,   247,
     248,   249,   250,   250,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   252,   252,
     252,   252,   252,   252,   252,   252,   253,   254,   254,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   256,   256,
     256,   256,   257,   257,   258,   258,   259,   259,   259,   259,
     260,   260,   261,   262,   262,   263,   263,   264,   264,   265,
     265,   266,   267,   267,   268
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
       3,     3,     3,     3,     4,     4,     6,     5,     6,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     3,     3,     3,     3,     4,     3,
       4,     2,     4,     4,     6,     7,     3,     5,     4,     1,
       1,     1,     1,     1,     1,     3,     3,     2,     0,     3,
       0,     1,     3,     0,     3,     1,     2,     1,     3,     1,
       1,     3,     1,     1,     1,     3,     5,     3,     3,     1,
       1,     1,     4,     6,     5,     4,     3,     0,     2,     0,
       1,     5,     0,     3,     5,     4,     5,     7,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 2947 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2953 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2959 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2965 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2971 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2977 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2983 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2989 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2995 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3001 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3007 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3013 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3019 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3025 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3031 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3037 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3043 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3049 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3055 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3061 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3067 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3073 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3079 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3085 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3091 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3097 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3103 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3109 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3115 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3121 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3127 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3133 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3139 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3145 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3151 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3157 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3163 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3169 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3175 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3181 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3187 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3193 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3199 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3205 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3211 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3217 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3223 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3229 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3235 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3241 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3247 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3253 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3259 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3265 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3271 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3277 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3283 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3289 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3295 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3301 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3307 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3313 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3319 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3325 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3331 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3337 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3343 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3349 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3355 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3361 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3367 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3373 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3379 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3385 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3391 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3397 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3403 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 386 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3699 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3705 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3711 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 403 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 407 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 411 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 420 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 428 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 444 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 469 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3799 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 478 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3821 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 487 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 508 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 531 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3884 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 545 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 548 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 554 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 560 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 563 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 569 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 571 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 578 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 585 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 607 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3991 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 619 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 620 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4027 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 625 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 630 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4065 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4071 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4115 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 680 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4139 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4157 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 692 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 726 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 730 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4237 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4396 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4420 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 850 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 853 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 872 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 884 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 888 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4705 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 917 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 918 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4735 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 929 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4741 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 933 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4749 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 942 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 949 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 955 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 959 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4802 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4820 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 974 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 977 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4846 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 988 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 993 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4874 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1010 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1011 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4894 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1020 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1057 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1062 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1070 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4979 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1074 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1079 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5000 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1085 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1089 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1094 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1103 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5046 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1113 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5064 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5083 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1132 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5099 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1142 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5152 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5158 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5164 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5170 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5176 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1198 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5188 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1207 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1209 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1210 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1213 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5266 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5272 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5284 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1221 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5290 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1222 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5296 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1252 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1260 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1261 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1262 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5396 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1264 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5408 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1265 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1266 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5420 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1267 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1268 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5432 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5438 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5444 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1271 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1272 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1273 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1274 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1276 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5480 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5486 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5492 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1280 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5504 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1281 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5510 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1282 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5516 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1283 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5528 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1285 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5534 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1286 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5540 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1287 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5546 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1288 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5552 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1289 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5558 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1290 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1294 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5584 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1301 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1304 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5630 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5636 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5644 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5718 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1350 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1384 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1385 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1389 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1390 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5778 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1402 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5796 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1405 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1412 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1413 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1417 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1418 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1422 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1428 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5843 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1429 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5849 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1433 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1434 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1438 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5871 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1443 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1446 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1450 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1456 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5910 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1465 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1466 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5922 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1467 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1471 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1474 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1484 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1491 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1501 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1504 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1511 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1516 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1546 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1549 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6063 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1558 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1570 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1582 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1593 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6124 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1608 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6130 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1609 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1613 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1718 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1734 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6274 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1735 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1738 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1739 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6316 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1772 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1783 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6494 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6500 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6506 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6512 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1800 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1814 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1820 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1824 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1829 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1834 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1839 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6612 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1859 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1869 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1876 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6663 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6674 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6684 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1892 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6693 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1896 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1901 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1910 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1919 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1928 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1932 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1937 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1946 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1955 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1956 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1957 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1958 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1962 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1978 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6851 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 1983 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 1990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 1995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2001 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2008 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6906 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2019 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2022 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2036 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2045 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2046 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2050 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2055 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2062 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2067 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6995 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2078 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2083 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2090 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7028 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2102 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2103 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2107 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7058 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7062 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2122 "ds_parser.ypp" /* yacc.c:1903  */


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


