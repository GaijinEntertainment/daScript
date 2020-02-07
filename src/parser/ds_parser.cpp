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
    RPIPE = 365,
    CLONEEQU = 366,
    ROTL = 367,
    ROTR = 368,
    ROTLEQU = 369,
    ROTREQU = 370,
    MAPTO = 371,
    BRABRAB = 372,
    BRACBRB = 373,
    CBRCBRB = 374,
    INTEGER = 375,
    LONG_INTEGER = 376,
    UNSIGNED_INTEGER = 377,
    UNSIGNED_LONG_INTEGER = 378,
    FLOAT = 379,
    DOUBLE = 380,
    NAME = 381,
    BEGIN_STRING = 382,
    STRING_CHARACTER = 383,
    END_STRING = 384,
    BEGIN_STRING_EXPR = 385,
    END_STRING_EXPR = 386,
    UNARY_MINUS = 387,
    UNARY_PLUS = 388,
    PRE_INC = 389,
    PRE_DEC = 390,
    POST_INC = 391,
    POST_DEC = 392,
    DEREF = 393,
    COLCOL = 394
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

#line 367 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   5628

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  167
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  98
/* YYNRULES -- Number of rules.  */
#define YYNRULES  390
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  701

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   394

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   150,     2,   166,   165,   145,   138,     2,
     158,   159,   143,   142,   132,   141,   155,   144,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   135,   162,
     139,   133,   140,   134,   146,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   156,     2,   157,   137,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   163,   136,   164,   149,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   147,   148,   151,
     152,   153,   154,   160,   161
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   367,   367,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   382,   394,   395,   399,   403,   407,   416,
     424,   440,   461,   465,   468,   474,   483,   504,   527,   528,
     532,   536,   537,   541,   544,   550,   556,   559,   565,   566,
     567,   574,   581,   593,   603,   612,   613,   614,   615,   616,
     617,   621,   626,   634,   635,   636,   637,   638,   639,   640,
     641,   647,   660,   676,   677,   681,   688,   700,   704,   711,
     712,   716,   717,   718,   722,   726,   733,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   776,   846,   849,   859,   860,   861,   862,
     863,   864,   865,   866,   867,   868,   869,   870,   871,   872,
     873,   874,   875,   879,   883,   889,   895,   909,   910,   921,
     925,   931,   934,   937,   941,   947,   951,   955,   958,   961,
     966,   969,   977,   980,   985,   988,   996,  1002,  1003,  1007,
    1008,  1012,  1046,  1049,  1054,  1062,  1066,  1071,  1077,  1081,
    1086,  1095,  1098,  1104,  1105,  1115,  1120,  1172,  1173,  1174,
    1175,  1176,  1177,  1181,  1182,  1183,  1184,  1185,  1186,  1187,
    1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1200,
    1211,  1216,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1268,  1272,  1275,  1278,  1279,  1280,  1281,  1282,  1283,
    1288,  1293,  1294,  1297,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1316,  1326,  1360,  1361,  1365,  1366,  1370,  1378,  1381,
    1388,  1389,  1393,  1394,  1398,  1404,  1405,  1409,  1410,  1414,
    1419,  1422,  1426,  1432,  1441,  1442,  1443,  1447,  1450,  1455,
    1460,  1467,  1477,  1480,  1487,  1488,  1492,  1522,  1525,  1534,
    1546,  1558,  1572,  1573,  1577,  1616,  1682,  1689,  1698,  1699,
    1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,
    1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,
    1720,  1721,  1725,  1736,  1740,  1750,  1751,  1752,  1753,  1767,
    1773,  1777,  1782,  1787,  1792,  1799,  1804,  1808,  1812,  1817,
    1822,  1829,  1834,  1840,  1845,  1849,  1854,  1863,  1867,  1872,
    1881,  1885,  1890,  1899,  1908,  1909,  1910,  1911,  1915,  1922,
    1931,  1936,  1943,  1948,  1954,  1961,  1972,  1975,  1989,  1998,
    1999,  2003,  2008,  2015,  2020,  2031,  2036,  2043,  2055,  2056,
    2060
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
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
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
     385,   386,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,    64,   387,   388,   126,
      33,   389,   390,   391,   392,    46,    91,    93,    40,    41,
     393,   394,    59,   123,   125,    36,    35
};
# endif

#define YYPACT_NINF -423

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-423)))

#define YYTABLE_NINF -184

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -423,    17,  -423,  -423,   -55,   -78,   -45,    -6,    39,  -423,
      83,    -7,  -423,  -423,  -423,  -423,  -423,    71,  -423,    98,
    -423,  -423,  -423,  -423,   108,  -423,   113,   111,   118,  -423,
    -423,     6,  -423,   -85,   143,  -423,  -423,   123,   165,   145,
    -423,   -73,  -423,   185,    27,  -423,   149,   109,   -55,   186,
     -78,   187,  -423,   190,   194,  -423,  -423,  4500,   195,  -423,
     -55,    -7,  -423,   183,   159,  5478,   292,   294,  -423,   169,
    -423,  -423,  -423,  -423,  -423,  -423,   210,    16,  -423,  -423,
    -423,  -423,   289,  -423,  -423,   -91,   201,   202,   203,  -423,
    -423,  -423,   188,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,   204,  -423,  -423,
    -423,   211,   212,  -423,  -423,  -423,  -423,   213,  -423,  -423,
    -423,  -423,  5397,  -423,   -77,  -423,   -92,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,   -12,   214,   -90,  -423,   -67,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,    65,  -423,   -94,
    -423,  4500,  4500,  4500,   228,  4428,  4428,  4428,  4500,  -423,
      24,  -423,  -423,  -423,  -423,  1790,  -423,  -423,  -423,    48,
     230,  -423,  -423,  -423,    66,  -423,   -68,  4500,   218,  -423,
    -423,    87,  -423,  -423,    16,  -423,  3191,  -423,   674,  1242,
    5155,   198,   214,  5213,   214,  5224,   214,  5278,  -423,   -99,
    5397,   220,  -423,   207,  -423,  -423,  -423,  4356,   224,  -423,
     225,   226,   245,   227,   247,   234,   249,  3191,  3191,    30,
    4500,  3191,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  3191,
    3191,  3191,   -92,  3191,  3191,  -423,  3191,  -423,   239,  -423,
    -423,   -88,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  4496,
     238,  -423,  -423,  -423,  -423,  -423,   -55,  -423,   358,   250,
    -423,  -423,   -10,  -423,   274,  -423,  4500,  3191,  5397,  -423,
    -423,  -423,  -423,  4500,  3191,  -423,   265,  -423,  4500,  -423,
    -423,   271,  -423,   275,  -423,   276,  -423,  -423,  4500,  -423,
    -423,   115,  -423,   291,  3191,  4500,  4500,  3191,  4500,  1915,
    4500,   549,   549,   230,  1164,  1303,  4795,  -423,    41,   180,
     549,   549,  -423,  -423,   549,   549,  3789,   845,   169,  -423,
    1419,   402,  3191,  3191,  -423,  -423,  3191,  3191,  3191,  3191,
     297,  3191,   298,  3191,  3191,  3191,  3191,  3191,  3191,  3191,
    3191,  3191,  3191,  3191,  3191,  3191,  3191,  3191,   299,  3191,
    -423,  2031,    26,  -423,   230,  -423,  -423,  -423,   545,  -423,
     321,  4641,  5211,  -423,  3271,  -423,  5279,  -423,  -423,  -423,
    -423,  2147,  1535,  3855,  5308,  5321,  3921,  5341,  -423,  3987,
    5352,    -4,   -81,  2263,  4795,   301,     7,   302,  -423,  -423,
      61,  2379,    94,    97,  3191,  3191,   267,  -423,  3191,   307,
    -423,  -423,  3191,  3191,   230,  1674,  -423,   218,  3191,  3191,
    -423,   281,   316,  2495,  2611,  -423,   404,   284,   286,  -423,
    -423,  -423,  -423,  -423,  -423,   287,   288,   293,  -423,  -423,
    -423,   434,  -423,   -79,  4739,   214,   330,  -423,    68,  4963,
    4500,   356,   356,   376,   376,  5184,  5184,   300,   283,  -423,
      59,    59,   356,   356,  4851,  5019,  5075,  5131,   376,   376,
     485,   485,   283,   283,   283,  -423,  4569,  -423,    75,  -423,
    -423,  3191,   100,  3191,  3191,  3191,  3191,  3191,  3191,  3191,
    3191,  3191,  3191,  3191,  3191,  3191,  3191,  3191,  -423,  3191,
    -423,  -423,  -423,    84,  4500,   331,  4053,  -423,  3191,  3191,
    -423,  3191,  -423,   303,  3191,  3191,   330,   339,   310,   330,
    3191,   312,  3191,   330,   320,   322,  4963,  -423,  -423,  4907,
     192,   192,     9,  3191,  -423,  -423,  3362,   470,  4963,   192,
    -423,   352,   370,  4963,  3191,  -423,  3453,   328,  -423,  -423,
    -423,  -423,  -423,   456,   230,  -423,   -84,   -57,    54,  3191,
    -423,  5397,  2727,  3191,  -423,  -423,  4963,  -423,  4963,  4963,
    4963,  4963,  4963,  4963,  4963,  4963,  4963,  4963,  4963,  4963,
    4963,  4963,  4963,  -423,  3544,  -423,  4490,     5,  -423,   376,
     376,   376,  2843,   -14,  4963,    80,   -57,  -423,   301,  4963,
    -423,  -423,   104,  -423,  -423,  -423,  -423,   179,  3191,  -423,
    3635,  -423,   218,  -423,  -423,  -423,  -423,  3726,  -423,  -423,
    -423,  -423,  3191,  -423,  -423,   334,  4963,  -423,    85,  4963,
    -423,  -423,  2959,   368,  -423,  4119,  3191,   338,  3191,   345,
     218,  3191,  -423,    25,  -423,  -423,  -423,  1010,  4963,  -423,
    -423,  4500,  4185,   364,  -423,   634,  -423,  4963,  -423,  -423,
     192,   218,  -423,  5367,  -423,  3075,   450,   349,   179,  -423,
    -423,  4500,  4251,  3191,   350,  -423,  5396,  -423,  4963,  -423,
    -423
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    69,     1,   159,     0,     0,     0,     0,     0,   160,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   294,
       6,    11,     5,     4,    59,    61,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    64,   137,     0,    65,
      67,     0,    63,     0,     0,   295,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,   297,     0,     0,   139,
       0,     0,    70,   302,     0,     0,     0,     0,    76,    71,
     292,    57,    58,    55,    56,    54,     0,     0,    53,    62,
      34,    32,    28,    25,    24,     0,     0,     0,     0,   308,
     328,   309,   333,   310,   314,   315,   316,   317,   321,   322,
     323,   324,   325,   326,   327,   329,   330,   354,   313,   320,
     331,   357,   360,   311,   318,   312,   319,     0,   332,   335,
     337,   336,   300,   138,     0,    68,     0,   304,   268,    79,
      80,    82,    81,    83,    84,    85,    86,   105,   106,   103,
     104,    96,   107,   108,    97,    94,    95,   109,   110,   111,
     112,    99,   100,    98,    92,    93,    88,    87,    89,    90,
      91,    78,    77,   101,   102,     0,    74,     0,    14,     0,
      49,    50,    47,    48,    46,    45,    51,     0,    27,     0,
     301,     0,     0,     0,     0,    71,    71,    71,     0,   341,
       0,   347,   350,   349,   343,     0,   346,    66,   303,   263,
       0,   306,    72,   272,     0,   270,   279,     0,     0,   296,
     293,     0,    15,    16,     0,    60,     0,   298,     0,     0,
       0,     0,    74,     0,    74,     0,    74,     0,   275,     0,
     274,   342,   345,     0,   348,   207,   208,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,   179,   178,   180,   181,   182,    17,     0,
       0,     0,     0,     0,     0,   339,     0,   133,    69,   205,
     175,   203,   256,   255,   254,   259,   204,   258,   257,     0,
       0,   206,   364,   365,   366,   367,     0,   305,   265,     0,
     271,    73,     0,   278,     0,   277,     0,     0,    75,   113,
     285,   286,   284,     0,     0,    52,     0,   351,     0,   353,
     334,     0,   355,     0,   358,     0,   361,   363,     0,   344,
     340,   141,   144,     0,     0,     0,     0,     0,     0,     0,
       0,   231,   232,     0,     0,     0,     0,   385,     0,     0,
     212,   211,   244,   239,   210,   209,     0,     0,    71,   262,
       0,     0,     0,     0,   233,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,     0,     0,   266,     0,   269,   273,   307,   280,   283,
       0,   282,     0,   291,   183,   299,     0,   356,   359,   362,
     276,     0,     0,     0,     0,     0,     0,     0,   247,     0,
       0,     0,   137,     0,   376,   370,     0,   379,   380,   381,
       0,     0,     0,     0,     0,     0,     0,    20,     0,    18,
     235,   135,     0,     0,     0,     0,   145,     0,     0,     0,
     146,     0,     0,     0,     0,   116,   114,     0,     0,   128,
     123,   121,   122,   134,   117,     0,     0,     0,   126,   127,
     129,   157,   120,     0,   183,    74,     0,   240,     0,   171,
       0,   213,   214,   226,   227,   224,   225,     0,   251,   238,
     260,   261,   215,   216,     0,   229,   230,   228,   222,   223,
     218,   217,   219,   220,   221,   237,     0,   242,     0,   264,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   287,     0,
     290,   352,   142,     0,     0,     0,     0,   245,     0,     0,
     246,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   386,   387,     0,
       0,     0,     0,     0,   147,   150,   183,     0,   140,     0,
     132,     0,     0,    37,     0,   154,   183,     0,   130,   131,
     119,   124,   125,     0,     0,   118,     0,     0,     0,     0,
     241,   253,     0,     0,   236,   243,   281,   136,   190,   191,
     193,   192,   194,   187,   188,   189,   195,   196,   185,   186,
     197,   198,   184,   289,   183,   143,     0,     0,   165,   162,
     163,   164,     0,     0,   368,     0,     0,   372,   371,   377,
     383,   382,     0,   374,   384,    19,    43,    38,     0,   151,
     183,   148,     0,    44,    35,    36,   155,   183,   152,   133,
     158,   161,     0,   173,   176,     0,   172,   200,     0,   252,
     288,   168,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,    41,     0,   149,   156,   153,     0,   174,   199,
     201,     0,     0,     0,   250,   388,   373,   369,   375,    39,
       0,     0,   115,     0,   166,     0,     0,     0,    38,    42,
     169,     0,     0,     0,     0,    40,     0,   167,   389,   390,
     170
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -423,  -423,  -423,   170,   467,  -423,  -423,  -423,  -423,  -423,
    -423,   440,  -423,  -423,   473,  -423,  -423,  -164,  -423,  -423,
    -423,  -423,   311,  -423,   478,   -54,  -423,   466,  -423,   261,
    -166,  -208,  -423,  -423,  -206,  -423,  -109,  -289,   -11,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,   534,  -423,  -423,
    -423,  -374,  -423,    34,  -423,  -343,  -423,  -423,   181,  -423,
    -423,  -423,  -423,   251,  -423,   223,  -423,  -203,  -189,   167,
     -13,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
    -155,   -56,  -423,  -423,   -48,   325,  -422,  -330,  -423,  -423,
    -241,    21,   229,  -423,  -423,  -423,  -423,  -423
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   169,   175,   339,   269,    14,    15,    31,
      32,    55,    16,    28,    29,   447,   448,   662,   449,   450,
     451,   452,   176,   177,    25,    26,    39,    40,    41,    17,
     166,   208,    69,    18,   270,   453,   347,   454,   271,   455,
     272,   456,   457,   458,   459,   460,   574,   461,   462,   273,
     274,   468,   644,   275,   276,   390,   277,   278,   469,   288,
     384,   289,   199,   203,   204,   228,   229,   535,   205,   304,
     210,   167,    46,    20,    85,    21,    22,   127,    64,    23,
     206,   280,   120,   121,   230,   281,   415,   416,   282,   417,
     418,   419,   420,   283,   338,   284,   687,   285
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   119,   299,   297,   463,   422,   124,   498,   389,   122,
     337,   290,   211,   534,   311,   393,   313,     2,   315,   222,
     224,   226,     3,    36,   293,    51,   628,   523,   170,   171,
     502,   200,   642,   200,    37,   179,   201,   293,   349,   216,
     333,   317,    27,     4,   578,     5,   118,     6,   293,     7,
      42,    86,   295,   231,     8,    48,    52,    87,    65,    61,
       9,   212,   213,   318,   294,   295,    10,   296,   217,    38,
     350,    24,    88,   180,   209,    43,   295,    44,    56,   267,
      58,    30,   197,   575,    62,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   201,   198,   201,   618,   579,    37,
      33,    71,    72,    66,    67,   119,   119,   119,   294,   119,
     119,   119,   119,   218,   219,   220,   172,   223,   225,   227,
     173,   294,   174,    76,   189,   652,   555,   202,   656,   190,
      53,   119,    45,    68,    38,   565,    37,   579,    48,   298,
     613,    54,   232,   360,   538,    34,   362,   653,   191,   539,
     118,   118,   118,    11,   118,   118,   118,   118,   411,    12,
     233,   119,   465,   499,   547,   501,   537,   681,   660,   321,
     234,    38,   300,   119,   119,   500,   118,   214,   301,   661,
     579,   334,   335,   425,   286,   426,   615,   579,   648,    35,
     351,   645,   287,   622,   378,   379,   579,   579,   541,   294,
     302,   192,   303,   542,   215,   291,   118,   580,   292,    73,
     603,   557,   382,    74,   585,    75,    76,   657,   118,   118,
     119,    47,   539,   605,   670,    48,    49,   119,   388,   193,
      50,   343,   119,   194,   663,   392,   539,   576,   544,   542,
     396,   545,   119,   267,   629,   268,   539,    77,   659,   119,
     119,   195,   119,   401,   119,   636,    57,   404,   405,   552,
     407,   196,   410,   351,    58,   118,   352,   353,   354,   355,
     356,    59,   118,   357,   358,   359,   360,   118,   361,   362,
     363,   351,   364,    60,   365,   366,    80,   118,   168,   427,
     428,    63,    70,    82,   118,   118,    83,   118,   126,   118,
      84,   123,   128,   163,   463,   164,   367,   165,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   168,    52,
     181,   182,   183,   185,   626,   627,   184,   378,   379,   207,
     186,   187,   188,   633,   221,   267,   201,   310,   319,   352,
     353,   354,   355,   356,   320,   325,   357,   358,   359,   360,
     643,   361,   362,   363,   351,   364,   279,   365,   366,   354,
     355,   267,   323,   324,   326,   327,   328,   360,   330,   361,
     362,   363,   329,   364,   351,    11,   381,   306,   383,   367,
     387,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   397,   385,   658,   119,   398,   399,   402,   470,   211,
     378,   379,   581,   477,   479,   495,   665,   395,   331,   332,
     502,   548,   336,   537,   540,   212,   561,   567,   378,   379,
     340,   341,   342,   560,   344,   345,   568,   346,   569,   570,
     571,   573,   354,   355,   679,   572,   577,   607,   582,   118,
     360,   612,   361,   362,   363,   616,   364,   617,   119,   620,
     352,   353,   354,   355,   688,   689,   606,   623,   391,   624,
     360,   632,   361,   362,   363,   394,   364,   634,   365,   366,
     635,   639,   640,   669,   673,   676,   693,   373,   374,   375,
     376,   377,   678,   351,   685,   403,   694,   699,   406,   429,
     409,   378,   379,   118,    78,   414,   414,   373,   374,   375,
     376,   377,   178,    81,   695,   305,    79,   125,   464,   348,
     667,   378,   379,   471,   472,    19,   587,   473,   474,   475,
     476,   400,   478,   386,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   519,
     496,   641,   322,   621,   423,     0,     0,   351,     0,     0,
       0,     0,     0,     0,   189,     0,     0,     0,     0,   190,
       0,   354,   355,   526,     0,     0,     0,     0,     0,   360,
       0,   361,   362,   363,   346,   364,     0,     0,   191,     0,
       0,     0,   346,     0,     0,   546,   336,     0,     0,   549,
       0,     0,     0,   550,   551,   119,   556,     0,     0,   558,
     559,     0,     0,   683,   563,   566,     0,     0,   375,   376,
     377,     0,     0,     0,     0,   119,     0,     0,     0,     0,
     378,   379,     0,   696,     0,   354,   355,     0,     0,     0,
     293,   192,   351,   360,     0,     0,   362,   363,     0,   364,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   193,
     118,     0,   586,   194,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,     0,
     604,   195,     0,   189,   378,   379,     0,     0,   190,   609,
     610,   196,   611,     0,     0,     0,   614,     0,     0,     0,
       0,   619,     0,   414,     0,     0,     0,   191,   352,   353,
     354,   355,   356,     0,   630,   357,   358,   359,   360,     0,
     361,   362,   363,     0,   364,   637,   365,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     646,     0,     0,     0,   649,     0,     0,     0,   367,     0,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     192,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,     0,   655,     0,     0,   686,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   193,     0,
       0,     0,   194,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,   668,     0,     0,     0,     0,     0,     0,
     195,     0,     0,   672,     0,     0,     0,   675,     0,   677,
     196,     0,   680,     0,     0,     0,   431,     0,   464,     0,
       3,     0,   432,   433,     0,   434,     0,   235,   236,   237,
     238,     0,     0,     0,     0,     0,   692,   435,   239,   436,
     437,     0,     0,     0,   698,     0,     0,     0,     0,   438,
     240,     0,     0,   439,   241,     0,   242,     0,     9,   243,
     440,     0,   441,   244,     0,     0,   442,   443,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,   245,   113,   114,   115,   116,     0,   246,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,    37,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   260,   261,     0,
       0,   262,     0,     0,   263,   264,     0,     0,     0,     0,
       0,     0,     0,   266,     0,     0,    38,   445,   267,   446,
     268,   431,     0,     0,     0,     3,     0,   432,   433,     0,
     434,     0,   235,   236,   237,   238,     0,     0,     0,     0,
       0,     0,   435,   239,   436,   437,     0,     0,     0,     0,
       0,     0,     0,     0,   438,   240,     0,     0,   439,   241,
       0,   242,     0,     9,   243,   440,     0,   441,   244,     0,
       0,   442,   443,     0,     0,    89,    90,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   108,   109,   110,     0,   245,   113,
     114,   115,   116,     0,   246,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,    37,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,   261,     0,     0,   262,     0,     0,   263,
     264,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,    38,   445,   267,   682,   268,   235,   236,   237,   238,
       0,     0,     0,     0,     0,     0,     0,   239,     0,     0,
       0,     0,     0,   189,     0,     0,     0,     0,   190,   240,
       0,     0,     0,   241,     0,   242,     0,     0,   243,     0,
       0,     0,   244,     0,     0,     0,     0,   191,     0,    89,
      90,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   108,   109,
     110,     0,   245,   113,   114,   115,   116,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,   248,     0,     0,     0,     0,     0,     0,     0,     0,
     192,   189,     0,     0,     0,     0,   190,     0,     0,     0,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     412,   258,     0,     0,     0,   191,     0,     0,   193,     0,
       0,     0,   194,     0,     0,   259,   260,   261,     0,     0,
     262,     0,     0,   263,   264,   235,   236,   237,   238,     0,
     195,     0,   413,     0,     0,    38,   239,   267,     0,   268,
     196,     0,   189,     0,     0,     0,     0,   190,   240,     0,
       0,     0,   241,     0,   242,     0,     0,   243,   192,     0,
       0,   244,     0,     0,     0,     0,   191,     0,    89,    90,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   193,   108,   109,   110,
     194,   245,   113,   114,   115,   116,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   195,   247,
     248,     0,     0,     0,   308,     0,     0,     0,   196,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   412,
     258,   235,   236,   237,   238,     0,     0,   193,     0,     0,
       0,   194,   239,     0,   259,   260,   261,     0,     0,   262,
       0,     0,   263,   264,   240,     0,     0,     0,   241,   195,
     242,   421,     0,   243,    38,     0,   267,   244,   268,   196,
       0,     0,     0,     0,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,   245,   113,   114,
     115,   116,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,    37,   258,   235,   236,   237,
     238,   524,     0,     0,     0,     0,     0,     0,   239,     0,
     259,   260,   261,     0,     0,   262,     0,     0,   263,   264,
     240,     0,     0,     0,   241,   466,   242,   266,   467,   243,
      38,     0,   267,   244,   268,     0,     0,     0,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,   245,   113,   114,   115,   116,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,    37,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,     0,   259,   260,   261,     0,
       0,   262,     0,     0,   263,   264,   235,   236,   237,   238,
       0,     0,     0,   266,     0,     0,    38,   239,   267,     0,
     268,     0,     0,     0,     0,     0,     0,     0,     0,   240,
       0,     0,     0,   241,     0,   242,     0,     0,   243,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,    89,
      90,    91,     0,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   108,   109,
     110,     0,   245,   113,   114,   115,   116,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,   248,     0,     0,     0,     0,     0,     0,     0,   553,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
      37,   258,   235,   236,   237,   238,     0,     0,     0,     0,
       0,     0,     0,   239,     0,   259,   260,   261,     0,     0,
     262,     0,     0,   263,   264,   240,     0,     0,     0,   241,
       0,   242,   266,     0,   243,    38,   554,   267,   244,   268,
       0,     0,     0,     0,     0,    89,    90,    91,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,     0,   108,   109,   110,     0,   245,   113,
     114,   115,   116,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,    37,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,   237,
     238,   259,   260,   261,     0,     0,   262,     0,   239,   263,
     264,     0,     0,     0,     0,     0,     0,   265,   266,     0,
     240,    38,     0,   267,   241,   268,   242,     0,     0,   243,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,   245,   113,   114,   115,   116,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,    37,   258,   235,   236,   237,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   259,   260,   261,     0,
       0,   262,     0,     0,   263,   264,   240,     0,     0,     0,
     241,     0,   242,   266,   408,   243,    38,     0,   267,   244,
     268,     0,     0,     0,     0,     0,    89,    90,    91,     0,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   108,   109,   110,     0,   245,
     113,   114,   115,   116,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,    37,   258,   235,
     236,   237,   238,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   259,   260,   261,     0,     0,   262,     0,     0,
     263,   264,   240,     0,     0,     0,   241,     0,   242,   266,
     497,   243,    38,     0,   267,   244,   268,     0,     0,     0,
       0,     0,    89,    90,    91,     0,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       0,   108,   109,   110,     0,   245,   113,   114,   115,   116,
       0,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,    37,   258,   235,   236,   237,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   259,   260,
     261,     0,     0,   262,     0,     0,   263,   264,   240,     0,
       0,     0,   241,     0,   242,   266,   522,   243,    38,     0,
     267,   244,   268,     0,     0,     0,     0,     0,    89,    90,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   108,   109,   110,
       0,   245,   113,   114,   115,   116,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
     248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,    37,
     258,   235,   236,   237,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   259,   260,   261,     0,     0,   262,
       0,     0,   263,   264,   240,     0,     0,     0,   241,     0,
     242,   266,   536,   243,    38,     0,   267,   244,   268,     0,
       0,     0,     0,     0,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,   245,   113,   114,
     115,   116,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,    37,   258,   235,   236,   237,
     238,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     259,   260,   261,     0,     0,   262,     0,     0,   263,   264,
     240,     0,     0,     0,   241,     0,   242,   266,   543,   243,
      38,     0,   267,   244,   268,     0,   562,     0,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,   245,   113,   114,   115,   116,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,    37,   258,   235,   236,   237,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   259,   260,   261,     0,
       0,   262,     0,     0,   263,   264,   240,     0,     0,     0,
     241,     0,   242,   266,     0,   243,    38,     0,   267,   244,
     268,     0,     0,     0,     0,     0,    89,    90,    91,     0,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   108,   109,   110,     0,   245,
     113,   114,   115,   116,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,    37,   258,   235,
     236,   237,   238,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   259,   260,   261,     0,     0,   262,     0,     0,
     263,   264,   240,     0,     0,     0,   241,     0,   242,   266,
       0,   243,    38,     0,   267,   244,   268,     0,     0,     0,
       0,     0,    89,    90,    91,     0,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       0,   108,   109,   110,     0,   245,   113,   114,   115,   116,
       0,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,    37,   258,   235,   236,   237,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   259,   260,
     261,     0,     0,   262,     0,     0,   263,   264,   240,     0,
       0,     0,   241,     0,   242,   266,   647,   243,    38,     0,
     267,   244,   268,     0,     0,     0,     0,     0,    89,    90,
      91,     0,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   108,   109,   110,
       0,   245,   113,   114,   115,   116,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
     248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,    37,
     258,   235,   236,   237,   238,   671,     0,     0,     0,     0,
       0,     0,   239,     0,   259,   260,   261,     0,     0,   262,
       0,     0,   263,   264,   240,     0,     0,     0,   241,     0,
     242,   266,   654,   243,    38,     0,   267,   244,   268,     0,
       0,     0,     0,     0,    89,    90,    91,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   108,   109,   110,     0,   245,   113,   114,
     115,   116,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,    37,   258,   235,   236,   237,
     238,   691,     0,     0,     0,     0,     0,     0,   239,     0,
     259,   260,   261,     0,     0,   262,     0,     0,   263,   264,
     240,     0,     0,     0,   241,     0,   242,   266,     0,   243,
      38,     0,   267,   244,   268,     0,     0,     0,     0,     0,
      89,    90,    91,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   108,
     109,   110,     0,   245,   113,   114,   115,   116,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,    37,   258,   235,   236,   237,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   259,   260,   261,     0,
       0,   262,     0,     0,   263,   264,   240,     0,     0,     0,
     241,     0,   242,   266,     0,   243,    38,     0,   267,   244,
     268,     0,     0,     0,     0,     0,    89,    90,    91,     0,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     0,   108,   109,   110,     0,   245,
     113,   114,   115,   116,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   351,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,    37,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   261,     0,     0,   262,     0,     0,
     263,   264,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,    38,     0,   267,     0,   268,   503,   504,   505,
     506,   507,   508,   509,   510,   352,   353,   354,   355,   356,
     511,   512,   357,   358,   359,   360,   513,   361,   362,   363,
     351,   364,   514,   365,   366,   515,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   517,   367,     0,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   379,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   508,   509,   510,   352,   353,   354,   355,
     356,   511,   512,   357,   358,   359,   360,   513,   361,   362,
     363,   351,   364,   514,   365,   366,   515,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   517,   367,     0,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
       0,     0,     0,     0,   631,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   508,   509,   510,   352,   353,   354,
     355,   356,   511,   512,   357,   358,   359,   360,   513,   361,
     362,   363,   351,   364,   514,   365,   366,   515,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   517,   367,     0,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   379,
       0,     0,     0,     0,     0,   638,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   508,   509,   510,   352,   353,
     354,   355,   356,   511,   512,   357,   358,   359,   360,   513,
     361,   362,   363,   351,   364,   514,   365,   366,   515,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   517,   367,     0,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,     0,     0,     0,     0,   650,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   508,   509,   510,   352,
     353,   354,   355,   356,   511,   512,   357,   358,   359,   360,
     513,   361,   362,   363,   351,   364,   514,   365,   366,   515,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   367,
       0,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,     0,     0,     0,     0,   664,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   508,   509,   510,
     352,   353,   354,   355,   356,   511,   512,   357,   358,   359,
     360,   513,   361,   362,   363,     0,   364,   514,   365,   366,
     515,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   517,
     367,     0,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,     0,   351,     0,     0,     0,     0,     0,     0,
       0,   378,   379,   352,   353,   354,   355,   356,   666,     0,
     357,   358,   359,   360,     0,   361,   362,   363,     0,   364,
       0,   365,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   378,   379,     0,     0,   430,   352,
     353,   354,   355,   356,     0,     0,   357,   358,   359,   360,
       0,   361,   362,   363,     0,   364,     0,   365,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     378,   379,     0,     0,   527,   352,   353,   354,   355,   356,
       0,     0,   357,   358,   359,   360,     0,   361,   362,   363,
       0,   364,     0,   365,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,     0,     0,     0,
       0,   351,     0,     0,     0,     0,   378,   379,     0,     0,
     530,   352,   353,   354,   355,   356,     0,     0,   357,   358,
     359,   360,     0,   361,   362,   363,     0,   364,     0,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,     0,     0,     0,     0,   351,     0,     0,
       0,     0,   378,   379,     0,     0,   532,   352,   353,   354,
     355,   356,     0,     0,   357,   358,   359,   360,     0,   361,
     362,   363,     0,   364,     0,   365,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,     0,
       0,     0,     0,   351,     0,     0,     0,     0,   378,   379,
       0,     0,   608,   352,   353,   354,   355,   356,     0,     0,
     357,   358,   359,   360,     0,   361,   362,   363,     0,   364,
       0,   365,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   378,   379,     0,     0,   674,   352,
     353,   354,   355,   356,     0,     0,   357,   358,   359,   360,
       0,   361,   362,   363,     0,   364,     0,   365,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,     0,   684,   352,   353,   354,   355,   356,
       0,     0,   357,   358,   359,   360,     0,   361,   362,   363,
       0,   364,     0,   365,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,    87,     0,   367,     0,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,     0,    88,     0,
       0,     0,     0,     0,     0,     0,   378,   379,     0,     0,
     697,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,   249,   250,   251,     0,     0,     0,     0,
       0,     0,    37,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     0,     0,   351,     0,     0,    38,     0,   189,
       0,    86,     0,     0,   190,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,   191,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,     0,   165,   351,     0,    38,
     352,   353,   354,   355,   356,     0,   192,   357,   358,   359,
     360,     0,   361,   362,   363,     0,   364,     0,   365,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   193,     0,    37,     0,   194,     0,
     367,     0,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,     0,     0,     0,     0,   195,     0,     0,   651,
       0,   378,   379,   380,     0,     0,   196,     0,     0,   351,
       0,    38,     0,   352,   353,   354,   355,   356,     0,     0,
     357,   358,   359,   360,     0,   361,   362,   363,     0,   364,
       0,   365,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,   584,   503,   504,   505,
     506,   507,   508,   509,   510,   352,   353,   354,   355,   356,
     511,   512,   357,   358,   359,   360,   513,   361,   362,   363,
    -183,   364,   514,   365,   366,   515,   516,   351,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   517,   367,     0,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     508,   509,   510,   352,   353,   354,   355,   356,   511,   512,
     357,   358,   359,   360,   513,   361,   362,   363,     0,   364,
     514,   365,   366,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,   517,   367,     0,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,     0,     0,     0,     0,   352,
     353,   354,   355,   356,   378,   379,   357,   358,   359,   360,
       0,   361,   362,   363,     0,   364,     0,   365,   366,     0,
       0,   424,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,   367,
       0,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,     0,     0,     0,     0,   352,   353,   354,   355,   356,
     378,   379,   357,   358,   359,   360,     0,   361,   362,   363,
       0,   364,     0,   365,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,     0,     0,     0,   367,   583,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,     0,     0,     0,
       0,   352,   353,   354,   355,   356,   378,   379,   357,   358,
     359,   360,     0,   361,   362,   363,     0,   364,     0,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   625,     0,
       0,   367,     0,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,     0,     0,     0,     0,   352,   353,   354,
     355,   356,   378,   379,   357,   358,   359,   360,     0,   361,
     362,   363,     0,   364,     0,   365,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,   367,     0,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,     0,
       0,     0,     0,   352,   353,   354,   355,   356,   378,   379,
     357,   358,   359,   360,     0,   361,   362,   363,     0,   364,
       0,   365,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,     0,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,     0,     0,     0,     0,   352,
     353,   354,   355,   356,   378,   379,   357,   358,   359,   360,
       0,   361,   362,   363,   189,   364,     0,   365,   366,   190,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,     0,   191,     0,
       0,     0,     0,   370,   371,   372,   373,   374,   375,   376,
     377,     0,     0,     0,     0,   352,   353,   354,   355,   356,
     378,   379,   357,   358,   359,   360,     0,   361,   362,   363,
     189,   364,   189,   365,   366,   190,     0,   190,     0,     0,
       0,     0,     0,   189,     0,     0,     0,     0,   190,     0,
       0,   192,     0,     0,   191,     0,   191,     0,     0,     0,
     371,   372,   373,   374,   375,   376,   377,   191,   352,   353,
     354,   355,   356,     0,     0,   357,   378,   379,   360,   193,
     361,   362,   363,   194,   364,   309,   365,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   189,     0,
       0,   195,   190,   190,     0,     0,   300,   192,     0,   192,
       0,   196,   301,   371,   372,   373,   374,   375,   376,   377,
     192,   191,   191,     0,     0,     0,     0,   189,     0,   378,
     379,     0,   190,     0,   302,   193,     0,   193,     0,   194,
     189,   194,     0,   312,     0,   190,     0,     0,   193,     0,
       0,   191,   194,     0,   314,     0,     0,   195,     0,   195,
     189,     0,     0,   518,   191,   190,     0,   196,     0,   196,
     195,   189,     0,     0,   192,   192,   190,     0,     0,     0,
     196,     0,     0,     0,   191,     0,   189,     0,     0,     0,
       0,   190,     0,     0,     0,   191,     0,     0,     0,     0,
       0,     0,   193,   193,   192,     0,   194,   194,   316,   521,
     191,     0,     0,     0,     0,   189,   189,   192,     0,     0,
     190,   190,     0,     0,   195,   195,     0,     0,     0,     0,
       0,     0,   193,     0,   196,   196,   194,   192,   528,   191,
     191,     0,     0,     0,     0,   193,     0,     0,   192,   194,
       0,   529,     0,     0,   195,     0,     0,     0,     0,     0,
       0,     0,     0,   192,   196,   193,     0,   195,     0,   194,
       0,   531,     0,     0,     0,     0,   193,   196,     0,     0,
     194,     0,   533,     0,     0,     0,     0,   195,     0,     0,
       0,   193,   192,   192,     0,   194,     0,   196,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,     0,     0,   195,     0,     0,   690,     0,     0,     0,
     193,   193,     0,   196,   194,   194,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   195,   195,     0,   700,     0,     0,     0,     0,
       0,     0,   196,   196,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     147,   148,   149,   150,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,     0,     0,     0,   161,   162
};

static const yytype_int16 yycheck[] =
{
      11,    57,   208,   206,   347,   335,    60,   381,   297,    57,
     251,   200,   167,    17,   222,   304,   224,     0,   226,   185,
     186,   187,     5,    30,   105,    19,    17,   401,    12,    13,
     109,    43,   116,    43,   126,   126,   126,   105,   126,   133,
      10,   140,   120,    26,   466,    28,    57,    30,   105,    32,
      61,    21,   133,    29,    37,   132,    50,    27,    31,   132,
      43,   128,   129,   162,   132,   133,    49,   135,   162,   161,
     158,   126,    42,   164,   164,     4,   133,     6,   163,   163,
     161,   126,   159,   162,   157,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,   126,   126,   126,   539,   132,   126,
     126,    12,    13,    96,    97,   181,   182,   183,   132,   185,
     186,   187,   188,   181,   182,   183,   120,   185,   186,   187,
     124,   132,   126,   127,    29,   140,   435,   159,   162,    34,
     144,   207,    54,   126,   161,   444,   126,   132,   132,   207,
     534,   155,   138,   104,   157,   126,   107,   162,    53,   162,
     181,   182,   183,   156,   185,   186,   187,   188,   333,   162,
     156,   237,   348,   157,   425,   388,   132,   162,     9,   237,
     166,   161,   105,   249,   250,   384,   207,   132,   111,    20,
     132,   249,   250,   162,   156,   164,   536,   132,   582,   126,
      18,   157,   164,   543,   155,   156,   132,   132,   157,   132,
     133,   106,   135,   162,   159,   159,   237,   159,   162,   120,
     519,   437,   286,   124,   159,   126,   127,   157,   249,   250,
     296,   133,   162,   159,   159,   132,   135,   303,   296,   134,
     132,   262,   308,   138,   628,   303,   162,   465,   164,   162,
     308,   164,   318,   163,   553,   165,   162,   158,   164,   325,
     326,   156,   328,   158,   330,   564,   133,   325,   326,   434,
     328,   166,   330,    18,   161,   296,    94,    95,    96,    97,
      98,   126,   303,   101,   102,   103,   104,   308,   106,   107,
     108,    18,   110,   158,   112,   113,   120,   318,   128,   129,
     130,   126,   163,   126,   325,   326,   126,   328,   135,   330,
     126,   126,   163,    31,   667,    31,   134,   158,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   128,    50,
     139,   139,   139,   139,   550,   551,   158,   155,   156,   135,
     139,   139,   139,   559,   126,   163,   126,   159,   138,    94,
      95,    96,    97,    98,   157,   139,   101,   102,   103,   104,
     576,   106,   107,   108,    18,   110,   195,   112,   113,    96,
      97,   163,   158,   158,   139,   158,   139,   104,   139,   106,
     107,   108,   158,   110,    18,   156,   158,   216,    40,   134,
     126,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   140,   162,   616,   470,   140,   140,   126,    16,   574,
     155,   156,   470,   126,   126,   126,   632,   162,   247,   248,
     109,   164,   251,   132,   132,   128,   120,    33,   155,   156,
     259,   260,   261,   162,   263,   264,   162,   266,   162,   162,
     162,    17,    96,    97,   660,   162,   126,   126,   158,   470,
     104,   158,   106,   107,   108,   126,   110,   157,   524,   157,
      94,    95,    96,    97,   680,   681,   524,   157,   297,   157,
     104,    11,   106,   107,   108,   304,   110,   135,   112,   113,
     120,   163,    36,   159,   126,   157,    46,   141,   142,   143,
     144,   145,   157,    18,   140,   324,   157,   157,   327,   339,
     329,   155,   156,   524,    47,   334,   335,   141,   142,   143,
     144,   145,    82,    50,   688,   214,    48,    61,   347,   268,
     639,   155,   156,   352,   353,     1,   502,   356,   357,   358,
     359,   318,   361,   292,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   392,
     379,   574,   237,   542,   335,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,
      -1,    96,    97,   402,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,   413,   110,    -1,    -1,    53,    -1,
      -1,    -1,   421,    -1,    -1,   424,   425,    -1,    -1,   428,
      -1,    -1,    -1,   432,   433,   671,   435,    -1,    -1,   438,
     439,    -1,    -1,   671,   443,   444,    -1,    -1,   143,   144,
     145,    -1,    -1,    -1,    -1,   691,    -1,    -1,    -1,    -1,
     155,   156,    -1,   691,    -1,    96,    97,    -1,    -1,    -1,
     105,   106,    18,   104,    -1,    -1,   107,   108,    -1,   110,
     671,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     691,    -1,   501,   138,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,    -1,
     519,   156,    -1,    29,   155,   156,    -1,    -1,    34,   528,
     529,   166,   531,    -1,    -1,    -1,   535,    -1,    -1,    -1,
      -1,   540,    -1,   542,    -1,    -1,    -1,    53,    94,    95,
      96,    97,    98,    -1,   553,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   564,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     579,    -1,    -1,    -1,   583,    -1,    -1,    -1,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,   612,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   642,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   652,    -1,    -1,    -1,   656,    -1,   658,
     166,    -1,   661,    -1,    -1,    -1,     1,    -1,   667,    -1,
       5,    -1,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,   685,    22,    23,    24,
      25,    -1,    -1,    -1,   693,    -1,    -1,    -1,    -1,    34,
      35,    -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,
      45,    -1,    47,    48,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
     165,     1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    45,    -1,    47,    48,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,   146,    -1,    -1,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,   165,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    53,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    53,    -1,    -1,   134,    -1,
      -1,    -1,   138,    -1,    -1,   141,   142,   143,    -1,    -1,
     146,    -1,    -1,   149,   150,    12,    13,    14,    15,    -1,
     156,    -1,   158,    -1,    -1,   161,    23,   163,    -1,   165,
     166,    -1,    29,    -1,    -1,    -1,    -1,    34,    35,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    44,   106,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,   134,    74,    75,    76,
     138,    78,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    96,
      97,    -1,    -1,    -1,   162,    -1,    -1,    -1,   166,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    12,    13,    14,    15,    -1,    -1,   134,    -1,    -1,
      -1,   138,    23,    -1,   141,   142,   143,    -1,    -1,   146,
      -1,    -1,   149,   150,    35,    -1,    -1,    -1,    39,   156,
      41,   158,    -1,    44,   161,    -1,   163,    48,   165,   166,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     141,   142,   143,    -1,    -1,   146,    -1,    -1,   149,   150,
      35,    -1,    -1,    -1,    39,   156,    41,   158,   159,    44,
     161,    -1,   163,    48,   165,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,   150,    12,    13,    14,    15,
      -1,    -1,    -1,   158,    -1,    -1,   161,    23,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,   141,   142,   143,    -1,    -1,
     146,    -1,    -1,   149,   150,    35,    -1,    -1,    -1,    39,
      -1,    41,   158,    -1,    44,   161,   162,   163,    48,   165,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,
      15,   141,   142,   143,    -1,    -1,   146,    -1,    23,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,
      35,   161,    -1,   163,    39,   165,    41,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,   150,    35,    -1,    -1,    -1,
      39,    -1,    41,   158,   159,    44,   161,    -1,   163,    48,
     165,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,   141,   142,   143,    -1,    -1,   146,    -1,    -1,
     149,   150,    35,    -1,    -1,    -1,    39,    -1,    41,   158,
     159,    44,   161,    -1,   163,    48,   165,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,   141,   142,
     143,    -1,    -1,   146,    -1,    -1,   149,   150,    35,    -1,
      -1,    -1,    39,    -1,    41,   158,   159,    44,   161,    -1,
     163,    48,   165,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,   141,   142,   143,    -1,    -1,   146,
      -1,    -1,   149,   150,    35,    -1,    -1,    -1,    39,    -1,
      41,   158,   159,    44,   161,    -1,   163,    48,   165,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     141,   142,   143,    -1,    -1,   146,    -1,    -1,   149,   150,
      35,    -1,    -1,    -1,    39,    -1,    41,   158,   159,    44,
     161,    -1,   163,    48,   165,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,   150,    35,    -1,    -1,    -1,
      39,    -1,    41,   158,    -1,    44,   161,    -1,   163,    48,
     165,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,   141,   142,   143,    -1,    -1,   146,    -1,    -1,
     149,   150,    35,    -1,    -1,    -1,    39,    -1,    41,   158,
      -1,    44,   161,    -1,   163,    48,   165,    -1,    -1,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,   141,   142,
     143,    -1,    -1,   146,    -1,    -1,   149,   150,    35,    -1,
      -1,    -1,    39,    -1,    41,   158,   159,    44,   161,    -1,
     163,    48,   165,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,   141,   142,   143,    -1,    -1,   146,
      -1,    -1,   149,   150,    35,    -1,    -1,    -1,    39,    -1,
      41,   158,   159,    44,   161,    -1,   163,    48,   165,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
     141,   142,   143,    -1,    -1,   146,    -1,    -1,   149,   150,
      35,    -1,    -1,    -1,    39,    -1,    41,   158,    -1,    44,
     161,    -1,   163,    48,   165,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,   150,    35,    -1,    -1,    -1,
      39,    -1,    41,   158,    -1,    44,   161,    -1,   163,    48,
     165,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,   146,    -1,    -1,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,    -1,   163,    -1,   165,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
      18,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    18,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    18,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    18,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    18,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,    94,    95,    96,    97,    98,   162,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,
      95,    96,    97,    98,    -1,    -1,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,   159,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
     159,    94,    95,    96,    97,    98,    -1,    -1,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   155,   156,
      -1,    -1,   159,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,
      95,    96,    97,    98,    -1,    -1,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,   156,    -1,    -1,   159,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
     159,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    18,    -1,    -1,   161,    -1,    29,
      -1,    21,    -1,    -1,    34,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,   158,    18,    -1,   161,
      94,    95,    96,    97,    98,    -1,   106,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,    -1,   126,    -1,   138,    -1,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
      -1,   155,   156,   157,    -1,    -1,   166,    -1,    -1,    18,
      -1,   161,    -1,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   156,   157,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,   133,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,   155,   156,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,   134,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
     155,   156,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,   155,   156,   101,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,   131,    -1,
      -1,   134,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,   155,   156,   101,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,   134,    -1,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,   155,   156,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,   155,   156,   101,   102,   103,   104,
      -1,   106,   107,   108,    29,   110,    -1,   112,   113,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
     155,   156,   101,   102,   103,   104,    -1,   106,   107,   108,
      29,   110,    29,   112,   113,    34,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   106,    -1,    -1,    53,    -1,    53,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,    53,    94,    95,
      96,    97,    98,    -1,    -1,   101,   155,   156,   104,   134,
     106,   107,   108,   138,   110,   140,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    29,    -1,
      -1,   156,    34,    34,    -1,    -1,   105,   106,    -1,   106,
      -1,   166,   111,   139,   140,   141,   142,   143,   144,   145,
     106,    53,    53,    -1,    -1,    -1,    -1,    29,    -1,   155,
     156,    -1,    34,    -1,   133,   134,    -1,   134,    -1,   138,
      29,   138,    -1,   140,    -1,    34,    -1,    -1,   134,    -1,
      -1,    53,   138,    -1,   140,    -1,    -1,   156,    -1,   156,
      29,    -1,    -1,   162,    53,    34,    -1,   166,    -1,   166,
     156,    29,    -1,    -1,   106,   106,    34,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    53,    -1,    29,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   134,   106,    -1,   138,   138,   140,   140,
      53,    -1,    -1,    -1,    -1,    29,    29,   106,    -1,    -1,
      34,    34,    -1,    -1,   156,   156,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   166,   166,   138,   106,   140,    53,
      53,    -1,    -1,    -1,    -1,   134,    -1,    -1,   106,   138,
      -1,   140,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   166,   134,    -1,   156,    -1,   138,
      -1,   140,    -1,    -1,    -1,    -1,   134,   166,    -1,    -1,
     138,    -1,   140,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,   134,   106,   106,    -1,   138,    -1,   166,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,   156,    -1,    -1,   159,    -1,    -1,    -1,
     134,   134,    -1,   166,   138,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   156,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   166,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,   149,   150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   168,     0,     5,    26,    28,    30,    32,    37,    43,
      49,   156,   162,   169,   174,   175,   179,   196,   200,   214,
     240,   242,   243,   246,   126,   191,   192,   120,   180,   181,
     126,   176,   177,   126,   126,   126,    30,   126,   161,   193,
     194,   195,   205,     4,     6,    54,   239,   133,   132,   135,
     132,    19,    50,   144,   155,   178,   163,   133,   161,   126,
     158,   132,   157,   126,   245,    31,    96,    97,   126,   199,
     163,    12,    13,   120,   124,   126,   127,   158,   171,   191,
     120,   181,   126,   126,   126,   241,    21,    27,    42,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   205,   248,
     249,   250,   251,   126,   192,   194,   135,   244,   163,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   112,   113,   114,
     115,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   149,   150,    31,    31,   158,   197,   238,   128,   170,
      12,    13,   120,   124,   126,   171,   189,   190,   178,   126,
     164,   139,   139,   139,   158,   139,   139,   139,   139,    29,
      34,    53,   106,   134,   138,   156,   166,   159,   205,   229,
      43,   126,   159,   230,   231,   235,   247,   135,   198,   164,
     237,   247,   128,   129,   132,   159,   133,   162,   251,   251,
     251,   126,   197,   251,   197,   251,   197,   251,   232,   233,
     251,    29,   138,   156,   166,    12,    13,    14,    15,    23,
      35,    39,    41,    44,    48,    78,    84,    96,    97,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   127,   141,
     142,   143,   146,   149,   150,   157,   158,   163,   165,   173,
     201,   205,   207,   216,   217,   220,   221,   223,   224,   225,
     248,   252,   255,   260,   262,   264,   156,   164,   226,   228,
     235,   159,   162,   105,   132,   133,   135,   234,   251,   201,
     105,   111,   133,   135,   236,   189,   225,   140,   162,   140,
     159,   198,   140,   198,   140,   198,   140,   140,   162,   138,
     157,   251,   252,   158,   158,   139,   139,   158,   139,   158,
     139,   225,   225,    10,   251,   251,   225,   257,   261,   172,
     225,   225,   225,   205,   225,   225,   225,   203,   196,   126,
     158,    18,    94,    95,    96,    97,    98,   101,   102,   103,
     104,   106,   107,   108,   110,   112,   113,   134,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   155,   156,
     157,   158,   192,    40,   227,   162,   230,   126,   251,   204,
     222,   225,   251,   204,   225,   162,   251,   140,   140,   140,
     232,   158,   126,   225,   251,   251,   225,   251,   159,   225,
     251,   247,   126,   158,   225,   253,   254,   256,   257,   258,
     259,   158,   254,   259,   116,   162,   164,   129,   130,   170,
     159,     1,     7,     8,    10,    22,    24,    25,    34,    38,
      45,    47,    51,    52,    85,   162,   164,   182,   183,   185,
     186,   187,   188,   202,   204,   206,   208,   209,   210,   211,
     212,   214,   215,   222,   225,   197,   156,   159,   218,   225,
      16,   225,   225,   225,   225,   225,   225,   126,   225,   126,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   126,   225,   159,   218,   157,
     235,   234,   109,    86,    87,    88,    89,    90,    91,    92,
      93,    99,   100,   105,   111,   114,   115,   133,   162,   236,
     162,   140,   159,   218,    16,   139,   225,   159,   140,   140,
     159,   140,   159,   140,    17,   234,   159,   132,   157,   162,
     132,   157,   162,   159,   164,   164,   225,   257,   164,   225,
     225,   225,   247,   105,   162,   204,   225,   201,   225,   225,
     162,   120,    51,   225,   105,   204,   225,    33,   162,   162,
     162,   162,   162,    17,   213,   162,   198,   126,   253,   132,
     159,   251,   158,   135,   157,   159,   225,   220,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   204,   225,   159,   251,   126,   159,   225,
     225,   225,   158,   218,   225,   254,   126,   157,   253,   225,
     157,   258,   254,   157,   157,   131,   201,   201,    17,   204,
     225,   162,    11,   201,   135,   120,   204,   225,   162,   163,
      36,   237,   116,   201,   219,   157,   225,   159,   218,   225,
     162,   159,   140,   162,   159,   225,   162,   157,   234,   164,
       9,    20,   184,   218,   162,   201,   162,   203,   225,   159,
     159,    16,   225,   126,   159,   225,   157,   225,   157,   201,
     225,   162,   164,   251,   159,   140,   162,   263,   201,   201,
     159,    16,   225,    46,   157,   184,   251,   159,   225,   157,
     159
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   167,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   169,   170,   170,   171,   172,   172,   172,
     173,   174,   175,   176,   176,   176,   177,   177,   178,   178,
     179,   180,   180,   181,   181,   182,   183,   183,   184,   184,
     184,   185,   186,   187,   188,   189,   189,   189,   189,   189,
     189,   190,   190,   191,   191,   191,   191,   191,   191,   191,
     191,   192,   192,   193,   193,   194,   194,   195,   195,   196,
     196,   197,   197,   197,   198,   198,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   200,   201,   201,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   203,   203,   203,   204,   205,   205,   205,
     206,   207,   207,   207,   207,   208,   209,   210,   210,   210,
     210,   210,   211,   211,   211,   211,   212,   213,   213,   214,
     214,   215,   216,   216,   216,   217,   217,   217,   217,   217,
     217,   218,   218,   219,   219,   220,   220,   221,   221,   221,
     221,   221,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     224,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   226,   226,   227,   227,   228,   229,   229,
     230,   230,   231,   231,   232,   233,   233,   234,   234,   235,
     235,   235,   235,   235,   236,   236,   236,   237,   237,   237,
     237,   237,   238,   238,   239,   239,   240,   241,   241,   241,
     242,   243,   244,   244,   245,   246,   247,   247,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   249,   250,   250,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   252,   252,   252,   252,   253,   253,
     254,   254,   255,   255,   255,   255,   256,   256,   257,   258,
     258,   259,   259,   260,   260,   261,   261,   262,   263,   263,
     264
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
       2,     2,     2,     6,     3,     7,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     3,     1,     3,     2,
       2,     2,     4,     5,     2,     1,     1,     2,     3,     4,
       2,     3,     3,     4,     2,     3,     4,     0,     2,     1,
       1,     3,     5,     5,     5,     5,     8,    10,     6,     9,
      11,     1,     3,     1,     2,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       5,     6,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     3,     3,     2,
       3,     4,     3,     4,     2,     4,     4,     3,     4,     6,
       7,     3,     5,     4,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     0,     3,     0,     1,     3,     0,     3,
       1,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       3,     5,     3,     3,     1,     1,     1,     4,     6,     5,
       4,     3,     0,     2,     0,     1,     5,     0,     3,     5,
       4,     5,     0,     2,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     4,     3,
       4,     2,     3,     2,     4,     3,     2,     2,     3,     2,
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
          case 126: /* "name"  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2830 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2836 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2842 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2848 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2854 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2860 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2866 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2872 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2878 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2884 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2890 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2896 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2902 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2908 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2914 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2920 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2926 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2932 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2938 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2944 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2950 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2956 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2962 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2968 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2974 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2980 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2986 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2992 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2998 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3004 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3010 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3016 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3022 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3028 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3034 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3040 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3046 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3052 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3058 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3064 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 219: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3070 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3076 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3082 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3088 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3094 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3100 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3106 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3112 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3118 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3124 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3130 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3136 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3142 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3148 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3154 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3160 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3166 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3172 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3178 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3184 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3190 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3196 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3202 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3208 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3214 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3220 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3226 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3232 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3238 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3244 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3250 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3256 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3262 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3268 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3274 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3280 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 382 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3588 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3594 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 403 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 407 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 416 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 424 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 440 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 465 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 468 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 474 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3698 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 483 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3724 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 504 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3749 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 528 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 544 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 550 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3785 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 556 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 559 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 565 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 566 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 567 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 574 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3857 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 603 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3868 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 612 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3874 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 613 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3880 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 614 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 615 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 616 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3904 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3914 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 626 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 635 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 636 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 647 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4010 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 676 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4016 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 677 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4022 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4034 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 688 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 700 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4057 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 722 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4105 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 726 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 733 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 738 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4183 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4201 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4231 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 755 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4237 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 756 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4243 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4249 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 759 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 760 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4297 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4303 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4309 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4327 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4333 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4339 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 846 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 849 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 859 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 860 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 862 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 872 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 883 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4552 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4560 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4576 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4582 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 910 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 925 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4612 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 931 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 937 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 941 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 947 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4651 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 951 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4657 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 955 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 958 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 961 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 977 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 980 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 988 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4737 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1002 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1046 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1049 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4833 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1062 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1066 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1071 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4863 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1077 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1086 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4901 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1098 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1105 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4937 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1172 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1173 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1174 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5008 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1175 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1176 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5020 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1177 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1181 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1182 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1183 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1184 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1185 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1186 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5080 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1190 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1191 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5098 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5104 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5116 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5122 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5134 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1211 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5186 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1239 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1248 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1249 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1252 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1253 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1257 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1260 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1261 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1262 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1264 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1268 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1272 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1279 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1280 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1281 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1282 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1283 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5458 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1288 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1293 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1294 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1297 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1300 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1301 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1302 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1304 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5541 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1360 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1361 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1365 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1366 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5626 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1378 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1388 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1389 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1393 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1398 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5675 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1404 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5681 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1405 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1409 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5693 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1410 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5699 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1414 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1419 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1422 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1426 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5737 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1432 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5748 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1441 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 5754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1442 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 5760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1443 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 5766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1447 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1450 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5784 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1455 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1460 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1467 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1477 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1480 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1487 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1488 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1525 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5901 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1546 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5929 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1558 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 5951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1573 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5957 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1577 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1616 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1682 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1689 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6089 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1698 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6095 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1699 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1700 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1701 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1702 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1703 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1706 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6143 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1707 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6149 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1708 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6155 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1709 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6161 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1710 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6167 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1711 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6173 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1712 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6179 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1713 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1714 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6191 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1715 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6197 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1716 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1717 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6209 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1718 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1719 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1720 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1736 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1740 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1753 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1767 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6315 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1773 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1777 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6344 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1792 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1804 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6385 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1808 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1812 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1817 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6414 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1822 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1829 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1834 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6457 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1845 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1849 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6476 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1854 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1863 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6499 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1867 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1872 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6523 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1885 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6542 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1890 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1899 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1908 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1910 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1911 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1915 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1922 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6614 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1931 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6624 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1936 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6633 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1943 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6654 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6666 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1961 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1975 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1989 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1998 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6720 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1999 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 2003 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 2008 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 2015 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 2020 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2031 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6778 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2036 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2043 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2055 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 6807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2056 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6813 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2060 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6831 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 6835 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2075 "ds_parser.ypp" /* yacc.c:1903  */


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


