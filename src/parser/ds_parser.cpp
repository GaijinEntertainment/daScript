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
#line 53 "ds_parser.ypp"

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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );

    LineInfo tokAt ( const struct YYLTYPE & li );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;

#line 194 "ds_parser.cpp"

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
    DAS_STATIC_IF = 264,
    DAS_ELSE = 265,
    DAS_FOR = 266,
    DAS_CATCH = 267,
    DAS_TRUE = 268,
    DAS_FALSE = 269,
    DAS_NEWT = 270,
    DAS_TYPEINFO = 271,
    DAS_TYPE = 272,
    DAS_IN = 273,
    DAS_IS = 274,
    DAS_AS = 275,
    DAS_ELIF = 276,
    DAS_STATIC_ELIF = 277,
    DAS_ARRAY = 278,
    DAS_RETURN = 279,
    DAS_NULL = 280,
    DAS_BREAK = 281,
    DAS_TRY = 282,
    DAS_OPTIONS = 283,
    DAS_TABLE = 284,
    DAS_EXPECT = 285,
    DAS_CONST = 286,
    DAS_REQUIRE = 287,
    DAS_OPERATOR = 288,
    DAS_ENUM = 289,
    DAS_FINALLY = 290,
    DAS_DELETE = 291,
    DAS_DEREF = 292,
    DAS_SCOPE = 293,
    DAS_TYPEDEF = 294,
    DAS_WITH = 295,
    DAS_CAST = 296,
    DAS_OVERRIDE = 297,
    DAS_UPCAST = 298,
    DAS_ITERATOR = 299,
    DAS_VAR = 300,
    DAS_ADDR = 301,
    DAS_CONTINUE = 302,
    DAS_WHERE = 303,
    DAS_PASS = 304,
    DAS_REINTERPRET = 305,
    DAS_MODULE = 306,
    DAS_PUBLIC = 307,
    DAS_LABEL = 308,
    DAS_GOTO = 309,
    DAS_IMPLICIT = 310,
    DAS_SHARED = 311,
    DAS_SMART_PTR = 312,
    DAS_TBOOL = 313,
    DAS_TVOID = 314,
    DAS_TSTRING = 315,
    DAS_TAUTO = 316,
    DAS_TINT = 317,
    DAS_TINT2 = 318,
    DAS_TINT3 = 319,
    DAS_TINT4 = 320,
    DAS_TUINT = 321,
    DAS_TUINT2 = 322,
    DAS_TUINT3 = 323,
    DAS_TUINT4 = 324,
    DAS_TFLOAT = 325,
    DAS_TFLOAT2 = 326,
    DAS_TFLOAT3 = 327,
    DAS_TFLOAT4 = 328,
    DAS_TRANGE = 329,
    DAS_TURANGE = 330,
    DAS_TBLOCK = 331,
    DAS_TINT64 = 332,
    DAS_TUINT64 = 333,
    DAS_TDOUBLE = 334,
    DAS_TFUNCTION = 335,
    DAS_TLAMBDA = 336,
    DAS_TINT8 = 337,
    DAS_TUINT8 = 338,
    DAS_TINT16 = 339,
    DAS_TUINT16 = 340,
    DAS_TTUPLE = 341,
    DAS_TVARIANT = 342,
    DAS_GENERATOR = 343,
    DAS_YIELD = 344,
    ADDEQU = 345,
    SUBEQU = 346,
    DIVEQU = 347,
    MULEQU = 348,
    MODEQU = 349,
    ANDEQU = 350,
    OREQU = 351,
    XOREQU = 352,
    SHL = 353,
    SHR = 354,
    ADDADD = 355,
    SUBSUB = 356,
    LEEQU = 357,
    SHLEQU = 358,
    SHREQU = 359,
    GREQU = 360,
    EQUEQU = 361,
    NOTEQU = 362,
    RARROW = 363,
    LARROW = 364,
    QQ = 365,
    QDOT = 366,
    QBRA = 367,
    LPIPE = 368,
    LBPIPE = 369,
    LAPIPE = 370,
    RPIPE = 371,
    CLONEEQU = 372,
    ROTL = 373,
    ROTR = 374,
    ROTLEQU = 375,
    ROTREQU = 376,
    MAPTO = 377,
    BRABRAB = 378,
    BRACBRB = 379,
    CBRCBRB = 380,
    INTEGER = 381,
    LONG_INTEGER = 382,
    UNSIGNED_INTEGER = 383,
    UNSIGNED_LONG_INTEGER = 384,
    FLOAT = 385,
    DOUBLE = 386,
    NAME = 387,
    BEGIN_STRING = 388,
    STRING_CHARACTER = 389,
    END_STRING = 390,
    BEGIN_STRING_EXPR = 391,
    END_STRING_EXPR = 392,
    UNARY_MINUS = 393,
    UNARY_PLUS = 394,
    PRE_INC = 395,
    PRE_DEC = 396,
    POST_INC = 397,
    POST_DEC = 398,
    DEREF = 399,
    COLCOL = 400
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 79 "ds_parser.ypp"

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

#line 376 "ds_parser.cpp"

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
#define YYLAST   6718

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  429
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  787

#define YYUNDEFTOK  2
#define YYMAXUTOK   400

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
       2,     2,     2,   155,     2,   172,   166,   151,   144,     2,
     164,   165,   149,   148,   138,   147,   160,   150,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   141,   169,
     145,   139,   146,   140,   167,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   162,     2,   163,   143,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   170,   142,   171,   154,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   152,   153,   156,   157,   158,   159,   161,
     168
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   388,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   403,   415,   416,   420,   424,   429,
     438,   446,   462,   484,   488,   491,   497,   506,   527,   550,
     551,   555,   559,   560,   564,   567,   573,   579,   582,   588,
     589,   593,   594,   595,   604,   605,   609,   618,   630,   640,
     649,   650,   651,   652,   653,   654,   658,   663,   671,   672,
     673,   674,   675,   676,   677,   678,   684,   697,   713,   714,
     718,   725,   737,   741,   748,   749,   753,   754,   755,   759,
     764,   771,   775,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   814,   887,
     890,   900,   901,   902,   903,   904,   905,   906,   907,   908,
     909,   910,   911,   912,   913,   914,   915,   916,   917,   921,
     926,   932,   938,   949,   955,   956,   967,   971,   977,   980,
     983,   987,   993,   997,  1001,  1004,  1007,  1012,  1015,  1023,
    1026,  1031,  1034,  1042,  1048,  1049,  1053,  1054,  1058,  1092,
    1095,  1100,  1108,  1112,  1117,  1123,  1127,  1132,  1141,  1144,
    1150,  1151,  1158,  1169,  1170,  1174,  1179,  1234,  1235,  1236,
    1237,  1238,  1239,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1262,
    1263,  1267,  1278,  1283,  1293,  1297,  1304,  1318,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,
    1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1361,  1365,  1368,
    1371,  1372,  1373,  1374,  1379,  1384,  1385,  1388,  1391,  1395,
    1399,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1419,  1429,
    1463,  1464,  1468,  1469,  1473,  1481,  1484,  1491,  1496,  1504,
    1505,  1509,  1512,  1521,  1522,  1526,  1535,  1536,  1541,  1542,
    1546,  1551,  1554,  1558,  1564,  1573,  1574,  1575,  1579,  1580,
    1584,  1587,  1592,  1597,  1605,  1616,  1619,  1626,  1627,  1631,
    1661,  1664,  1673,  1686,  1698,  1699,  1703,  1707,  1718,  1733,
    1734,  1738,  1777,  1843,  1850,  1859,  1860,  1861,  1862,  1863,
    1864,  1865,  1866,  1867,  1868,  1869,  1870,  1871,  1872,  1873,
    1874,  1875,  1876,  1877,  1878,  1879,  1880,  1881,  1882,  1886,
    1887,  1888,  1889,  1890,  1891,  1895,  1906,  1910,  1920,  1921,
    1922,  1923,  1937,  1943,  1947,  1952,  1957,  1962,  1969,  1974,
    1978,  1982,  1987,  1992,  1998,  2005,  2010,  2016,  2021,  2025,
    2030,  2039,  2043,  2048,  2057,  2061,  2066,  2075,  2081,  2090,
    2096,  2111,  2112,  2113,  2114,  2118,  2125,  2134,  2139,  2146,
    2151,  2157,  2163,  2170,  2181,  2184,  2198,  2207,  2208,  2212,
    2217,  2224,  2229,  2240,  2245,  2252,  2264,  2265,  2269,  2283
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"let\"", "\"def\"", "\"while\"", "\"if\"", "\"static_if\"", "\"else\"",
  "\"for\"", "\"recover\"", "\"true\"", "\"false\"", "\"new\"",
  "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"",
  "\"static_elif\"", "\"array\"", "\"return\"", "\"null\"", "\"break\"",
  "\"try\"", "\"options\"", "\"table\"", "\"expect\"", "\"const\"",
  "\"require\"", "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"",
  "\"deref\"", "\"scope\"", "\"typedef\"", "\"with\"", "\"cast\"",
  "\"override\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"shared\"",
  "\"smart_ptr\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"block\"", "\"int64\"",
  "\"uint64\"", "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"",
  "\"uint8\"", "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"",
  "\"generator\"", "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS",
  "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "'.'",
  "DEREF", "'['", "']'", "'('", "')'", "'$'", "'@'", "COLCOL", "';'",
  "'{'", "'}'", "'#'", "$accept", "program", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_if_then_else", "expression_for_loop",
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
  "block_or_lambda", "expr_block", "expr_numeric_const", "expr_assign",
  "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_expr", "expr", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "variant_keyword",
  "variant_alias_declaration", "make_decl", "make_struct_fields",
  "make_struct_dim", "make_struct_decl", "make_tuple", "make_map_tuple",
  "make_any_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
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
     385,   386,   387,   388,   389,   390,   391,   392,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,   393,   394,   126,    33,   395,   396,   397,   398,
      46,   399,    91,    93,    40,    41,    36,    64,   400,    59,
     123,   125,    35
};
# endif

#define YYPACT_NINF -490

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-490)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -490,    30,  -490,  -490,   -82,   -22,   -77,    33,     6,  -490,
      57,  -490,   -11,  -490,  -490,  -490,  -490,  -490,   192,  -490,
     164,  -490,  -490,  -490,  -490,   103,  -490,   110,  -490,   119,
     122,   130,  -490,  -490,     9,  -490,    45,   144,  -490,  -490,
     113,   155,   139,  -490,   -48,  -490,   162,   -21,  -490,   138,
     143,    23,   -82,   188,   -22,   183,  -490,   184,   185,  -490,
     227,  -490,   180,  -490,   -94,   186,  -490,   -82,   -11,  -490,
     181,   151,  6563,   290,   292,  -490,   167,  -490,   196,  -490,
    -490,  -490,  -490,  -490,   195,   158,  -490,  -490,  -490,  -490,
     288,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,   171,
     -93,  5614,  -490,  -490,  -490,   -53,  -490,   -69,  -490,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,    35,   203,   -84,   210,
    -490,   187,  -490,   172,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,    12,  -490,  -490,   -55,  -490,   209,   216,   217,   219,
    -490,  -490,  -490,   204,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,   222,  -490,
    -490,  -490,   224,   233,  -490,  -490,  -490,  -490,   237,   238,
    -490,  -490,  -490,  -490,    70,  -490,  -490,   -44,   -82,  -490,
     -23,  -490,   -62,  5614,   201,  -490,  -490,  -490,   154,  5614,
     -78,  -490,  -490,   158,  -490,   -75,  4053,  -490,  5614,  5614,
    5614,  5614,   252,  5538,  5538,  5538,  5690,   196,  -490,    -4,
    -490,  -490,  -490,  -490,  2070,  -490,  -490,  -490,   337,   218,
      74,   253,  -490,    65,  -490,   228,  3392,  -490,  -490,   257,
    5614,  -490,    -1,  3392,  -490,  -490,  -490,  -490,  -490,  -490,
    5462,   229,  -490,   235,   255,   256,   239,   259,   265,  4053,
    4053,   777,  4180,  4053,  -490,  -490,  -490,  -490,  -490,  -490,
    -490,  4053,  4053,  4053,  4053,  4053,  4053,  -490,   225,  -490,
    -490,   -91,  -490,  -490,  -490,   240,  -490,  -490,  -490,  -490,
    -490,  4808,   250,  -490,  -490,  -490,  -490,  -490,   390,  1616,
     631,  1244,   251,   203,  1388,   203,  2195,   203,  2281,    22,
    -490,  -101,  3392,   -99,   273,  -490,   261,  -490,  -490,  5685,
    -490,   253,  -490,  -490,  -490,  -490,  -490,  5614,  2198,  -490,
     927,  -490,   375,  -490,  -490,  -490,  2198,  2460,  -490,   287,
    4053,  5614,  5614,  4053,  5614,  5614,   393,   393,   253,  1245,
     253,  1389,  6074,  -490,   -49,   165,   393,   393,   -79,   393,
     393,  4945,   -80,  -490,  1670,   167,    -9,   293,  4053,  4053,
    -490,  -490,  4053,  4053,  4053,  4053,   295,  4053,   297,  4053,
    4053,  4053,  4053,  4053,  2335,  4053,  4053,  4053,  4053,  4053,
    4053,  4053,  4053,  4053,  4053,   299,  4053,  -490,  2463,  -490,
    5614,  -490,  -490,  -490,   286,  -490,   291,  -490,   294,  -490,
    5614,  -490,  5690,  -490,   196,  -490,  -490,  -490,  -490,    69,
     116,  -490,   319,  5906,  -490,  4053,  -490,  -490,   253,  1533,
    -490,   201,  4053,  4053,  -490,   267,   308,  2600,  1814,  -490,
     404,   272,   274,  4053,  -490,  -490,  -490,  -490,  -490,  -490,
     275,   277,   278,  -490,  -490,  -490,   424,  -490,   -83,  -490,
    6009,  -490,  2198,  -490,  4268,  2728,  1942,  5013,  2546,  2547,
    5081,  2725,  2811,     7,   -86,   285,  2865,  6074,   311,    67,
     312,  -490,  -490,    71,    16,  3002,    81,   102,  4053,  4053,
     281,  -490,  4053,   320,  -490,  5538,  -490,   321,  -490,    26,
    6275,   203,  5614,  -490,  -490,   226,   226,   247,   247,  6529,
    6529,   296,   600,  -490,  5761,   -68,   -68,   226,   226,   323,
    6142,  6340,  6408,   498,   247,   247,   619,   619,   600,   600,
     600,  -490,  5830,  -490,    28,  2812,  -490,  -490,  -490,  3392,
    -490,  4053,  -490,  -490,   116,  4053,  4053,  4053,  4053,  4053,
    4053,  4053,  4053,  4053,  4053,  4053,  4053,  4053,  4053,  4053,
    4173,    24,  2198,  -490,  -490,  4363,   444,  6275,  4173,  -490,
     316,   338,  6275,  2198,  -490,  4458,   298,  -490,  -490,  4173,
    -490,  -490,  -490,   425,   253,  -490,  3139,  3267,  -490,  4553,
    -490,  -490,    37,  5614,   333,  5149,  -490,  4053,  4053,  -490,
    4053,   302,  4053,  4053,  -490,   321,   335,   313,   321,  4053,
     314,  4053,  4053,   321,   315,   317,  6275,  -490,  -490,  6207,
     203,  2949,   -63,    88,  4053,  -490,   -96,  3392,  3395,  -490,
    -490,  4053,  -490,  -490,  -490,  6275,  -490,  6275,  6275,  6275,
    6275,  6275,  6275,  6275,  6275,  6275,  6275,  6275,  6275,  6275,
    6275,  6275,  -490,  4053,  -490,  4648,  -490,   201,  -490,  -490,
    -490,  -490,  4743,  -490,  -490,   234,  -490,  -490,  4053,  4053,
    -490,  -490,  3086,   -97,  -490,   247,   247,   247,  3532,   -72,
    6275,    90,   -63,  -490,   311,  6275,  -490,  -490,   -71,   133,
    -490,  -490,  -490,   336,   -69,   318,  6275,  3669,  -490,  -490,
    -490,    46,  6275,   -67,  -490,  -490,  -490,  1098,   201,  -490,
    -490,  4053,  -490,  6473,  6473,  -490,  3797,   347,  -490,  5217,
    4053,   324,  4053,  4053,   325,   -69,  -490,  -490,  4053,  6275,
    -490,   201,  -490,  -490,  4173,  5614,  5285,   340,  -490,  4880,
    -490,  6275,  4880,  -490,  -490,  6275,  -490,   234,  3478,  -490,
    3925,   433,   326,   327,  -490,  -490,  5614,  5353,  4053,   328,
     332,  3479,  -490,  6275,  -490,  -490,  -490
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    74,     1,   166,     0,     0,     0,     0,     0,   167,
       0,   399,     0,    13,     3,    10,     9,     8,     0,     7,
     317,     6,    11,     5,     4,     0,    12,    64,    66,    22,
      34,    31,    32,    24,    29,    23,     0,     0,    14,    69,
     144,     0,    70,    72,     0,    68,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,    30,     0,     0,    27,
       0,   320,     0,   324,     0,     0,   146,     0,     0,    75,
     329,     0,     0,     0,     0,    81,    76,   315,     0,    62,
      63,    60,    61,    59,     0,     0,    58,    67,    35,    33,
      29,    26,    25,   359,   362,   360,   363,   361,   364,     0,
       0,     0,   326,   325,   145,     0,    73,     0,   331,   285,
      84,    85,    87,    86,    88,    89,    90,    91,   110,   111,
     108,   109,   101,   112,   113,   102,    99,   100,   114,   115,
     116,   117,   104,   105,   103,    97,    98,    93,    92,    94,
      95,    96,    83,    82,   106,   107,   280,    79,     0,     0,
     296,     0,    15,     0,    54,    55,    52,    53,    51,    50,
      56,     0,    28,   320,     0,   327,     0,     0,     0,     0,
     335,   355,   336,   366,   337,   341,   342,   343,   344,   348,
     349,   350,   351,   352,   353,   354,   356,   357,   388,   340,
     347,   358,   391,   394,   338,   345,   339,   346,     0,     0,
     365,   368,   370,   369,     0,    71,   330,   280,     0,    77,
       0,   289,     0,     0,     0,   333,   319,   316,   308,     0,
       0,    16,    17,     0,    65,     0,     0,   321,     0,     0,
       0,     0,     0,    76,    76,    76,     0,     0,   374,     0,
     380,   384,   382,   376,     0,   323,   379,   332,   282,     0,
       0,     0,   287,   300,    78,   280,    80,   139,   118,     0,
       0,   309,     0,   295,   400,   297,    57,   328,   222,   223,
       0,     0,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   189,   188,   190,   191,   192,
      18,     0,     0,     0,     0,     0,     0,   183,   184,   220,
     185,   218,   273,   272,   271,    74,   276,   219,   275,   274,
     255,     0,     0,   221,   401,   402,   403,   404,     0,     0,
       0,     0,     0,    79,     0,    79,     0,    79,     0,   144,
     293,     0,   291,     0,   375,   378,     0,   381,   372,     0,
     283,     0,   286,   281,   288,   299,   298,     0,     0,   290,
       0,   334,     0,   306,   307,   305,     0,   148,   151,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,     0,   423,     0,     0,   227,   226,   260,   225,
     224,     0,     0,   279,     0,    76,     0,     0,     0,     0,
     248,   249,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,   385,
       0,   387,   383,   367,     0,   389,     0,   392,     0,   395,
       0,   397,     0,   398,     0,   377,   373,   371,   284,   301,
       0,   304,     0,   303,   141,     0,    44,    45,     0,     0,
     152,     0,     0,     0,   153,     0,     0,     0,     0,   121,
     119,     0,     0,     0,   134,   129,   127,   128,   140,   122,
       0,     0,     0,   132,   133,   135,   164,   126,     0,   123,
     193,   310,     0,   314,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,     0,     0,   414,   407,     0,
     417,   418,   419,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,    19,   250,    76,   214,     0,   256,     0,
     178,    79,     0,   268,   269,   228,   229,   241,   242,   239,
     240,     0,   265,   254,     0,   277,   278,   230,   231,     0,
       0,   244,   245,   243,   237,   238,   233,   232,   234,   235,
     236,   253,     0,   258,     0,     0,   390,   393,   396,   292,
     294,     0,   184,   143,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,   157,   193,     0,   147,     0,   138,
       0,     0,    38,     0,   161,   193,     0,   136,   137,     0,
     125,   130,   131,     0,     0,   124,     0,     0,   312,   193,
     313,   149,     0,     0,     0,     0,   261,     0,     0,   262,
       0,     0,     0,     0,   410,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   416,   424,   425,     0,
      79,     0,     0,     0,     0,   257,     0,   267,     0,   252,
     270,     0,   251,   259,   386,   302,   142,   200,   201,   203,
     202,   204,   197,   198,   199,   205,   206,   195,   196,   207,
     208,   194,    48,     0,   158,   193,   155,     0,    49,    36,
      37,   162,   193,   159,   139,    41,   165,   168,     0,     0,
     311,   150,     0,     0,   172,   169,   170,   171,     0,     0,
     405,     0,     0,   409,   408,   415,   421,   420,     0,     0,
     412,   422,    20,     0,     0,     0,   179,     0,   180,   186,
     212,     0,   266,     0,   156,   163,   160,     0,     0,    39,
      40,     0,    46,   210,   209,   175,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,   215,   211,     0,   181,
     213,     0,   120,    42,     0,     0,     0,     0,   264,   426,
     411,   406,   426,   413,   216,   182,    47,    41,     0,   173,
       0,     0,     0,     0,    43,   176,     0,     0,     0,     0,
       0,     0,   174,   427,   428,   429,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -490,  -490,  -490,   115,   445,  -490,  -490,  -490,  -490,  -490,
    -490,   407,  -490,  -490,   448,  -490,  -490,  -490,  -268,  -490,
    -490,  -490,  -490,  -490,   284,  -490,   456,   -57,  -490,   443,
    -490,   207,  -218,  -316,  -490,  -490,  -208,  -490,  -168,  -297,
     -12,  -490,  -490,  -490,  -490,  -490,  -490,  -490,  -490,   520,
    -490,  -490,  -490,  -394,  -490,  -490,  -427,  -490,  -346,  -490,
    -490,  -490,  -490,   179,   331,  -490,  -490,  -490,   268,  -490,
      92,  -490,  -201,   303,  -251,  -231,   170,  -490,   -76,  -490,
    -490,  -490,   362,   467,  -490,  -490,  -490,  -490,  -490,  -490,
    -143,  -100,  -490,  -490,  -490,   -87,  -490,  -490,   262,  -489,
    -353,  -490,  -490,  -280,   -98,   174,  -490,  -490,  -490,  -221,
    -490
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    14,   153,   159,   375,   299,    15,    16,    34,
      35,    59,    17,    31,    32,   461,   462,   731,   732,   463,
     464,   465,   466,   467,   160,   161,    28,    29,    42,    43,
      44,    18,   147,   214,    76,    19,   300,   468,   350,   469,
     301,   470,   302,   471,   472,   473,   474,   475,   604,   476,
     477,   303,   304,   519,   719,   305,   306,   307,   442,   479,
     308,   309,   310,   520,   210,   341,   249,   207,   211,   212,
     330,   331,   150,   151,   623,   252,   356,   262,   217,   148,
      49,    21,   100,    63,    64,    22,    23,   108,    71,    24,
     253,   312,    99,   202,   203,   332,    25,    26,   313,   498,
     499,   314,   500,   501,   502,   503,   315,   374,   316,   772,
     317
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   201,   348,   373,   478,   218,   258,   424,   522,   426,
     105,   428,    72,   563,   204,   323,   325,   327,   506,   265,
     344,    39,   251,   345,   554,   622,   717,   334,   643,    55,
       2,   564,   398,   399,   632,     3,    79,    80,    62,   164,
     396,   383,   673,   398,   399,   431,   345,   433,   215,   736,
      27,   441,    40,   346,   149,    33,    45,   164,     4,   483,
       5,    56,     6,    40,     7,   515,   644,   644,   432,     8,
     434,   644,   737,   384,   257,     9,   346,   102,   165,    73,
      74,    10,    65,   416,   226,    52,   605,   216,    41,   200,
      68,   612,   415,   264,   416,   206,   267,   740,   743,    41,
     238,   238,   751,   254,    30,   239,   239,   255,   353,   215,
     438,    75,   205,   201,   227,    69,   354,    11,   208,   201,
     509,    40,   510,   523,   240,   240,   256,   247,   201,   201,
     201,   201,   263,   201,   201,   201,   201,   656,   355,   704,
     335,   318,   319,   320,   321,   259,   324,   326,   328,    81,
     223,   250,   584,    82,   259,    83,    84,    41,   336,    57,
     201,   594,   259,   430,   644,    36,   644,   521,   337,    58,
     201,   154,   155,   352,   345,   644,    37,   224,   345,   241,
     241,   201,   201,   357,   644,   608,    60,    85,   561,    38,
      65,   645,    12,   653,   369,   371,    46,   208,    47,    13,
     209,   200,   691,   259,   346,   646,   347,   200,   346,   242,
     242,   750,    52,   243,   243,    61,   200,   200,   200,   200,
      48,   200,   200,   200,   200,   493,   626,   504,   699,   637,
     627,   244,   244,   265,   630,    50,   628,   343,   708,   245,
     631,   246,   246,   586,   728,   386,   387,   201,   200,    51,
     628,   715,   634,   741,   721,   729,   730,    52,   200,   628,
     439,   201,   201,    53,   201,   201,   386,   387,    54,   200,
     200,   631,   701,   635,   488,   489,    62,   491,   492,   723,
     709,    65,   297,   562,   156,   674,   257,    66,   157,    93,
     158,    84,   259,    94,    70,   260,   681,   640,   261,   152,
     511,   512,   628,    67,   744,   581,   221,   222,    77,    95,
      96,    97,    98,    78,    88,    90,    91,    92,   104,   101,
     201,   109,   107,   144,   713,   145,   390,   391,   149,   152,
     201,   146,   201,   555,   396,   200,   397,   398,   399,   400,
      56,   163,   401,   559,   213,   388,   389,   390,   391,   200,
     200,   219,   200,   200,   228,   396,   220,   397,   398,   399,
     400,   229,   230,   401,   231,   402,   403,   233,   232,   234,
     516,   257,   672,   410,   411,   412,   413,   414,   235,   340,
     678,   478,   236,   237,   322,   215,   415,   342,   416,   351,
     208,   685,   382,   359,   410,   411,   412,   413,   414,   360,
     361,   362,    12,   363,   364,   311,   238,   415,   200,   416,
     365,   239,   386,   387,   418,   201,   423,   435,   200,   486,
     200,   238,   201,   339,   436,   524,   239,   531,   641,   533,
     240,   551,   556,   564,   590,   647,   589,   557,   718,   596,
     558,   597,   603,   598,   600,   240,   601,   602,   624,   626,
     629,   742,   638,   642,   221,   650,   677,   679,   366,   367,
     648,   218,   372,   686,   680,   693,   698,   702,   684,   725,
     376,   377,   378,   379,   380,   381,   703,   706,   710,   757,
     711,   778,   745,   747,   353,   241,   770,   760,   763,   779,
     513,   784,   354,   390,   391,   785,    86,   162,   780,   774,
     241,   396,    89,   200,   398,   399,   400,   266,    87,   401,
     200,   106,   385,   201,   355,   242,   727,   386,   387,   243,
     753,    20,   482,   349,   560,   225,   692,   443,   687,   480,
     242,   103,   358,   707,   243,   484,   419,   244,   248,   487,
     333,   773,   490,   766,   481,   507,   767,   246,   497,     0,
     497,     0,   244,   415,     0,   416,     0,     0,     0,     0,
       0,     0,   246,     0,     0,     0,     0,   525,   526,     0,
       0,   527,   528,   529,   530,     0,   532,     0,   534,   535,
     536,   537,   538,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   549,   550,     0,   552,   388,   389,   390,   391,
     392,   200,     0,   393,   394,   395,   396,     0,   397,   398,
     399,   400,     0,     0,   401,     0,   402,   403,     0,   386,
     387,     0,     0,     0,   580,     0,     0,     0,   585,     0,
       0,   587,   588,     0,     0,     0,   592,   595,   386,   387,
       0,     0,   599,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,     0,   201,     0,     0,   415,     0,
     416,   609,   238,     0,     0,   615,     0,   239,   768,     0,
       0,     0,     0,     0,     0,   381,   201,     0,     0,     0,
       0,     0,     0,     0,   381,     0,   240,   636,   372,   781,
       0,   639,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,   746,     0,     0,     0,     0,     0,   396,     0,
     397,   398,   399,   400,     0,     0,   401,     0,     0,   390,
     391,     0,     0,     0,     0,     0,     0,   396,     0,   397,
     398,   399,   400,   764,     0,   401,     0,     0,     0,     0,
     655,   241,     0,   200,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,     0,
     415,   675,   416,     0,   200,     0,     0,     0,   412,   413,
     414,   242,   682,     0,     0,   243,     0,   421,     0,   415,
       0,   416,     0,     0,     0,   667,   671,     0,   368,     0,
       0,     0,     0,   244,     0,     0,   695,   696,     0,   697,
     166,     0,   700,   246,     0,     0,   167,     0,   705,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,   716,     0,     0,     0,     0,     0,     0,
     722,     0,     0,     0,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,     0,     0,   733,   734,     0,
       0,     0,     0,     0,     0,     0,     0,   739,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   480,     0,     0,    40,
     754,     0,     0,     0,     0,   756,     0,     0,     0,   759,
       0,   761,   762,     0,     0,     0,     0,   765,   444,     0,
       0,     0,     3,     0,   445,   446,   447,     0,   448,     0,
     268,   269,   270,   271,     0,    41,     0,     0,     0,   777,
       0,   449,   272,   450,   451,     0,     0,   783,     0,     0,
       0,     0,     0,   452,   273,     0,     0,   453,   274,     0,
     275,     0,     9,   276,   454,     0,   455,   277,     0,     0,
     456,   457,     0,     0,     0,   170,   171,   172,     0,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,     0,   189,   190,   191,     0,     0,   194,
     195,   196,   197,     0,     0,   278,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   279,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,    40,
     290,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,   292,   293,     0,     0,     0,
       0,   294,   295,     0,     0,     0,     0,     0,     0,     0,
       0,   296,     0,   297,   298,    41,   459,   257,   460,   444,
       0,     0,     0,     3,     0,   445,   446,   447,     0,   448,
       0,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,     0,   449,   272,   450,   451,     0,     0,     0,     0,
       0,     0,     0,     0,   452,   273,     0,     0,   453,   274,
       0,   275,     0,     9,   276,   454,     0,   455,   277,     0,
       0,   456,   457,     0,     0,     0,   170,   171,   172,     0,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,     0,   189,   190,   191,     0,     0,
     194,   195,   196,   197,     0,     0,   278,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
      40,   290,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   292,   293,     0,     0,
       0,     0,   294,   295,     0,     0,     0,     0,   268,   269,
     270,   271,   296,     0,   297,   298,    41,   459,   257,   752,
     272,     0,     0,     0,     0,   238,   238,     0,     0,     0,
     239,   239,   273,     0,     0,     0,   274,     0,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,     0,   240,
     240,     0,     0,   170,   171,   172,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,   189,   190,   191,     0,     0,   194,   195,   196,
     197,     0,     0,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,     0,     0,   241,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   494,   290,     0,
       0,     0,     0,     0,   242,   242,     0,     0,   243,   243,
     422,     0,   291,   292,   293,     0,     0,     0,     0,   294,
     295,     0,   268,   269,   270,   271,   244,   244,   495,   496,
       0,   297,   298,    41,   272,   257,   246,   246,     0,   238,
     238,     0,     0,     0,   239,   239,   273,     0,     0,     0,
     274,     0,   275,     0,     0,   276,     0,     0,     0,   277,
       0,     0,     0,   240,   240,     0,     0,   170,   171,   172,
       0,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,     0,   189,   190,   191,     0,
       0,   194,   195,   196,   197,     0,     0,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   279,
     280,     0,     0,     0,     0,     0,     0,     0,   241,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   494,   290,     0,     0,     0,     0,     0,   242,   242,
       0,     0,   243,   243,   425,     0,   291,   292,   293,     0,
       0,     0,     0,   294,   295,     0,   268,   269,   270,   271,
     244,   244,     0,   505,     0,   297,   298,    41,   272,   257,
     246,   246,     0,     0,     0,     0,     0,     0,     0,     0,
     273,     0,     0,     0,   274,     0,   275,     0,     0,   276,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
       0,   170,   171,   172,     0,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,     0,
     189,   190,   191,     0,     0,   194,   195,   196,   197,     0,
       0,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,   280,     0,     0,     0,     0,     0,
       0,     0,   582,     0,     0,     0,     0,   238,   440,     0,
       0,     0,   239,     0,     0,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    40,   290,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     291,   292,   293,   268,   269,   270,   271,   294,   295,     0,
       0,     0,     0,     0,     0,   272,     0,   296,     0,   297,
     298,    41,   583,   257,     0,     0,     0,   273,     0,     0,
       0,   274,     0,   275,     0,     0,   276,     0,     0,     0,
     277,     0,     0,     0,     0,     0,   241,     0,   170,   171,
     172,     0,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,   189,   190,   191,
       0,     0,   194,   195,   196,   197,   242,     0,   278,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   280,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,   420,     0,     0,   246,     0,
       0,     0,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,    40,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   291,   292,   293,
       0,     0,     0,     0,   294,   295,     0,   268,   269,   270,
     271,     0,   517,     0,   296,   518,   297,   298,    41,   272,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,     0,   274,     0,   275,     0,     0,
     276,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   172,     0,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
       0,   189,   190,   191,     0,     0,   194,   195,   196,   197,
       0,     0,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   279,   280,     0,     0,     0,     0,
       0,     0,     0,   593,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,    40,   290,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,   271,   613,
       0,   291,   292,   293,     0,     0,     0,   272,   294,   295,
       0,     0,     0,     0,     0,     0,     0,     0,   296,   273,
     297,   298,    41,   274,   257,   275,     0,     0,   276,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   172,     0,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,     0,   189,
     190,   191,     0,     0,   194,   195,   196,   197,     0,     0,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,    40,   290,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,   271,   614,     0,   291,
     292,   293,     0,     0,     0,   272,   294,   295,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   273,   297,   298,
      41,   274,   257,   275,     0,     0,   276,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     172,     0,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,   189,   190,   191,
       0,     0,   194,   195,   196,   197,     0,     0,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,    40,   290,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,   271,     0,     0,   291,   292,   293,
       0,     0,     0,   272,   294,   295,   238,     0,     0,     0,
       0,   239,     0,   338,   296,   273,   297,   298,    41,   274,
     257,   275,     0,     0,   276,     0,     0,     0,   277,     0,
     240,     0,     0,     0,     0,     0,   170,   171,   172,     0,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,     0,   189,   190,   191,     0,     0,
     194,   195,   196,   197,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   238,   440,     0,     0,     0,   239,     0,     0,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
      40,   290,     0,     0,     0,   242,   240,     0,     0,   243,
       0,   427,     0,     0,     0,   291,   292,   293,   268,   269,
     270,   271,   294,   295,     0,   539,     0,   244,     0,     0,
     272,     0,   296,     0,   297,   298,    41,   246,   257,     0,
       0,     0,   273,     0,     0,     0,   274,     0,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,     0,     0,
       0,   241,     0,   170,   171,   172,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,   189,   190,   191,     0,     0,   194,   195,   196,
     197,   242,     0,   278,     0,   243,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,     0,     0,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,    40,   290,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,   271,
       0,     0,   291,   292,   293,     0,     0,     0,   272,   294,
     295,   238,     0,     0,     0,     0,   239,     0,     0,   296,
     273,   297,   298,    41,   274,   257,   275,     0,     0,   276,
       0,     0,     0,   277,     0,   240,     0,     0,     0,     0,
       0,   170,   171,   172,     0,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,     0,
     189,   190,   191,     0,     0,   194,   195,   196,   197,     0,
       0,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,   280,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   238,   238,     0,
       0,     0,   239,   239,     0,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    40,   290,     0,     0,     0,
     242,   240,   240,     0,   243,     0,     0,     0,     0,     0,
     291,   292,   293,   268,   269,   270,   271,   294,   295,     0,
       0,     0,   244,     0,   485,   272,     0,   296,   553,   297,
     298,    41,   246,   257,     0,     0,     0,   273,     0,     0,
       0,   274,     0,   275,     0,     0,   276,     0,     0,     0,
     277,     0,     0,   591,     0,     0,   241,   241,   170,   171,
     172,     0,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,   189,   190,   191,
       0,     0,   194,   195,   196,   197,   242,   242,   278,     0,
     243,   243,   617,   618,     0,     0,     0,     0,     0,     0,
     279,   280,     0,     0,     0,     0,     0,     0,   244,   244,
       0,     0,     0,     0,     0,     0,     0,     0,   246,   246,
       0,     0,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,    40,   290,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,   271,     0,     0,   291,   292,   293,
       0,     0,     0,   272,   294,   295,   238,     0,     0,     0,
       0,   239,     0,     0,   296,   273,   297,   298,    41,   274,
     257,   275,     0,     0,   276,     0,     0,     0,   277,     0,
     240,     0,     0,     0,     0,     0,   170,   171,   172,     0,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,     0,   189,   190,   191,     0,     0,
     194,   195,   196,   197,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   238,   238,     0,     0,     0,   239,   239,     0,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
      40,   290,     0,     0,     0,   242,   240,   240,     0,   243,
       0,   620,     0,     0,     0,   291,   292,   293,   268,   269,
     270,   271,   294,   295,     0,     0,     0,   244,     0,     0,
     272,     0,   296,   611,   297,   298,    41,   246,   257,     0,
       0,     0,   273,     0,     0,     0,   274,     0,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,     0,     0,
       0,   241,   241,   170,   171,   172,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,   189,   190,   191,     0,     0,   194,   195,   196,
     197,   242,   242,   278,     0,   243,   243,   621,   654,     0,
       0,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,     0,   244,   244,     0,     0,     0,     0,     0,
     238,     0,     0,   246,   246,   239,     0,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,    40,   290,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,     0,
       0,     0,   291,   292,   293,   268,   269,   270,   271,   294,
     295,     0,     0,     0,     0,     0,     0,   272,     0,   296,
     625,   297,   298,    41,     0,   257,     0,     0,     0,   273,
       0,     0,     0,   274,     0,   275,     0,     0,   276,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,   241,
     170,   171,   172,     0,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,     0,   189,
     190,   191,     0,     0,   194,   195,   196,   197,     0,   242,
     278,     0,     0,   243,     0,   714,     0,     0,     0,     0,
       0,     0,   279,   280,     0,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,   238,     0,     0,
       0,   246,   239,     0,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,    40,   290,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,     0,   291,
     292,   293,   268,   269,   270,   271,   294,   295,     0,     0,
       0,     0,     0,     0,   272,     0,   296,   633,   297,   298,
      41,     0,   257,     0,     0,     0,   273,     0,     0,     0,
     274,     0,   275,     0,     0,   276,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   241,   170,   171,   172,
       0,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,     0,   189,   190,   191,     0,
       0,   194,   195,   196,   197,     0,   242,   278,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,   279,
     280,     0,     0,     0,     0,     0,     0,     0,   244,     0,
       0,   735,     0,     0,   688,     0,     0,     0,   246,     0,
       0,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,    40,   290,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,   271,     0,     0,   291,   292,   293,     0,
       0,     0,   272,   294,   295,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   273,   297,   298,    41,   274,   257,
     275,     0,     0,   276,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,   170,   171,   172,     0,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,     0,   189,   190,   191,     0,     0,   194,
     195,   196,   197,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   279,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,    40,
     290,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,   271,     0,     0,   291,   292,   293,     0,     0,     0,
     272,   294,   295,   238,     0,     0,     0,     0,   239,     0,
       0,   296,   273,   297,   298,    41,   274,   257,   275,     0,
       0,   276,     0,     0,     0,   277,     0,   240,     0,     0,
       0,     0,     0,   170,   171,   172,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,   189,   190,   191,     0,     0,   194,   195,   196,
     197,     0,     0,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   238,
     238,     0,     0,     0,   239,   239,     0,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,    40,   290,     0,
       0,     0,   242,   240,   240,     0,   243,     0,     0,     0,
       0,     0,   291,   292,   293,   268,   269,   270,   271,   294,
     295,     0,     0,     0,   244,     0,     0,   272,     0,   296,
     720,   297,   298,    41,   246,   257,     0,     0,     0,   273,
       0,     0,     0,   274,     0,   275,     0,     0,   276,     0,
       0,     0,   277,     0,     0,     0,     0,     0,   241,   241,
     170,   171,   172,     0,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,     0,   189,
     190,   191,     0,     0,   194,   195,   196,   197,   242,   242,
     278,     0,   243,   243,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   280,     0,     0,     0,     0,     0,     0,
     244,   244,     0,   775,   786,     0,     0,     0,     0,     0,
     246,   246,     0,     0,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,    40,   290,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   291,
     292,   293,   268,   269,   270,   271,   294,   295,     0,     0,
       0,     0,     0,     0,   272,     0,   296,   738,   297,   298,
      41,     0,   257,     0,     0,     0,   273,     0,     0,     0,
     274,     0,   275,     0,     0,   276,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
       0,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,     0,   189,   190,   191,     0,
       0,   194,   195,   196,   197,     0,     0,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   279,
     280,     0,     0,     0,     0,     0,     0,     0,   748,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,    40,   290,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,   271,   755,     0,   291,   292,   293,     0,
       0,     0,   272,   294,   295,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   273,   297,   298,    41,   274,   257,
     275,     0,     0,   276,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,   170,   171,   172,     0,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,     0,   189,   190,   191,     0,     0,   194,
     195,   196,   197,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   279,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,    40,
     290,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,   271,   776,     0,   291,   292,   293,     0,     0,     0,
     272,   294,   295,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   273,   297,   298,    41,   274,   257,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,     0,   170,   171,   172,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,   189,   190,   191,     0,     0,   194,   195,   196,
     197,     0,     0,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,    40,   290,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,   271,
       0,     0,   291,   292,   293,     0,     0,     0,   272,   294,
     295,     0,     0,     0,     0,     0,     0,     0,     0,   296,
     273,   297,   298,    41,   274,   257,   275,     0,     0,   276,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
       0,   170,   171,   172,     0,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,     0,
     189,   190,   191,     0,     0,   194,   195,   196,   197,     0,
       0,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    40,   290,     0,     0,     0,
       0,   370,   386,   387,     0,     0,     0,     0,     0,     0,
     291,   292,   293,   166,     0,     0,     0,   294,   295,   167,
       0,     0,     0,     0,     0,     0,     0,   296,     0,   297,
     298,    41,     0,   257,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,     0,     0,
       0,   388,   389,   390,   391,   392,     0,     0,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   386,   387,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,     0,   416,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,   566,
     567,   568,   569,   570,   571,   572,   388,   389,   390,   391,
     392,   573,   574,   393,   394,   395,   396,   575,   397,   398,
     399,   400,   386,   387,   401,   576,   402,   403,   577,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,   404,     0,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     416,     0,     0,     0,     0,     0,     0,   610,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,   566,   567,   568,   569,   570,   571,
     572,   388,   389,   390,   391,   392,   573,   574,   393,   394,
     395,   396,   575,   397,   398,   399,   400,   386,   387,   401,
     576,   402,   403,   577,   578,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   579,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,     0,   416,     0,     0,     0,     0,
       0,     0,   676,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,   566,
     567,   568,   569,   570,   571,   572,   388,   389,   390,   391,
     392,   573,   574,   393,   394,   395,   396,   575,   397,   398,
     399,   400,   386,   387,   401,   576,   402,   403,   577,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,   404,     0,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     416,     0,     0,     0,     0,     0,     0,   683,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,   566,   567,   568,   569,   570,   571,
     572,   388,   389,   390,   391,   392,   573,   574,   393,   394,
     395,   396,   575,   397,   398,   399,   400,   386,   387,   401,
     576,   402,   403,   577,   578,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   579,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,     0,   416,     0,     0,     0,     0,
       0,     0,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,   566,
     567,   568,   569,   570,   571,   572,   388,   389,   390,   391,
     392,   573,   574,   393,   394,   395,   396,   575,   397,   398,
     399,   400,   386,   387,   401,   576,   402,   403,   577,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,   404,     0,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     416,     0,     0,     0,     0,     0,     0,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   386,   387,     0,
       0,     0,     0,   565,   566,   567,   568,   569,   570,   571,
     572,   388,   389,   390,   391,   392,   573,   574,   393,   394,
     395,   396,   575,   397,   398,   399,   400,     0,     0,   401,
     576,   402,   403,   577,   578,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   579,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,     0,     0,     0,   386,
     387,     0,     0,   415,     0,   416,   388,   389,   390,   391,
     392,     0,   726,   393,   394,   395,   396,     0,   397,   398,
     399,   400,     0,     0,   401,     0,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,   386,   387,     0,     0,   415,     0,
     416,     0,     0,     0,     0,     0,     0,   417,   388,   389,
     390,   391,   392,     0,     0,   393,   394,   395,   396,     0,
     397,   398,   399,   400,     0,     0,   401,     0,   402,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   386,   387,     0,     0,     0,     0,     0,     0,
     415,     0,   416,   388,   389,   390,   391,   392,     0,   771,
     393,   394,   395,   396,     0,   397,   398,   399,   400,     0,
       0,   401,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,     0,     0,     0,
     386,   387,     0,     0,     0,   415,     0,   416,     0,     0,
     514,   388,   389,   390,   391,   392,     0,     0,   393,   394,
     395,   396,     0,   397,   398,   399,   400,     0,     0,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,     0,     0,   386,   387,
       0,     0,     0,   415,     0,   416,     0,     0,   616,   388,
     389,   390,   391,   392,     0,     0,   393,   394,   395,   396,
       0,   397,   398,   399,   400,     0,     0,   401,     0,   402,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,     0,     0,     0,   386,   387,     0,     0,
       0,   415,     0,   416,     0,     0,   619,   388,   389,   390,
     391,   392,     0,     0,   393,   394,   395,   396,     0,   397,
     398,   399,   400,     0,     0,   401,     0,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
       0,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,     0,     0,     0,   386,   387,     0,     0,     0,   415,
       0,   416,     0,     0,   694,   388,   389,   390,   391,   392,
       0,     0,   393,   394,   395,   396,     0,   397,   398,   399,
     400,     0,     0,   401,     0,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,     0,
       0,     0,   386,   387,     0,     0,     0,   415,     0,   416,
       0,     0,   758,   388,   389,   390,   391,   392,     0,     0,
     393,   394,   395,   396,     0,   397,   398,   399,   400,     0,
       0,   401,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,     0,   416,     0,     0,
     769,   388,   389,   390,   391,   392,     0,     0,   393,   394,
     395,   396,     0,   397,   398,   399,   400,     0,     0,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,   167,     0,   404,     0,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,     0,   168,     0,     0,     0,
       0,     0,     0,   415,     0,   416,     0,     0,   782,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,     0,     0,     0,     0,   167,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   168,     0,     0,   281,   282,   283,     0,     0,
       0,     0,     0,     0,    40,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   146,     0,   386,   387,    41,     0,     0,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,   167,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,     0,     0,
     386,   387,    41,   388,   389,   390,   391,   392,     0,     0,
     393,   394,   395,   396,     0,   397,   398,   399,   400,     0,
       0,   401,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,     0,     0,   404,     0,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,     0,   416,   437,   386,
     387,     0,     0,     0,     0,     0,     0,     0,    41,   388,
     389,   390,   391,   392,     0,     0,   393,   394,   395,   396,
       0,   397,   398,   399,   400,     0,     0,   401,     0,   402,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,     0,     0,     0,     0,     0,     0,     0,
       0,   415,     0,   416,   649,   386,   387,     0,   388,   389,
     390,   391,   392,     0,     0,   393,   394,   395,   396,     0,
     397,   398,   399,   400,     0,     0,   401,     0,   402,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,     0,   416,   652,     0,     0,   565,   566,   567,   568,
     569,   570,   571,   572,   388,   389,   390,   391,   392,   573,
     574,   393,   394,   395,   396,   575,   397,   398,   399,   400,
    -193,     0,   401,   576,   402,   403,   577,   578,   386,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   579,   404,     0,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,     0,     0,
       0,     0,     0,     0,     0,     0,   415,     0,   416,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   386,   387,     0,     0,     0,     0,   565,
     566,   567,   568,   569,   570,   571,   572,   388,   389,   390,
     391,   392,   573,   574,   393,   394,   395,   396,   606,   397,
     398,   399,   400,     0,     0,   401,   576,   402,   403,   577,
     578,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   607,   404,
       0,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   386,   387,     0,     0,     0,     0,     0,     0,   415,
       0,   416,   388,   389,   390,   391,   392,     0,     0,   393,
     394,   395,   396,     0,   397,   398,   399,   400,     0,     0,
     401,     0,   402,   403,     0,     0,   508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,     0,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   386,   387,     0,     0,
       0,     0,     0,     0,   415,     0,   416,     0,     0,     0,
     388,   389,   390,   391,   392,     0,     0,   393,   394,   395,
     396,     0,   397,   398,   399,   400,     0,     0,   401,     0,
     402,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   651,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   386,   387,     0,     0,     0,     0,
       0,     0,   415,     0,   416,   388,   389,   390,   391,   392,
       0,     0,   393,   394,   395,   396,     0,   397,   398,   399,
     400,     0,     0,   401,     0,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   712,     0,     0,   404,     0,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   386,
     387,     0,     0,     0,     0,     0,     0,   415,     0,   416,
       0,     0,     0,   388,   389,   390,   391,   392,     0,     0,
     393,   394,   395,   396,     0,   397,   398,   399,   400,     0,
       0,   401,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   386,   387,     0,
       0,     0,     0,     0,     0,   415,     0,   416,   388,   389,
     390,   391,   392,     0,     0,   393,   394,   395,   396,     0,
     397,   398,   399,   400,     0,     0,   401,     0,   402,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   386,   387,     0,     0,     0,     0,     0,     0,
     415,     0,   416,     0,     0,     0,   388,   389,   390,   391,
     392,     0,     0,   393,   394,   395,   396,     0,   397,   398,
     399,   400,     0,     0,   401,     0,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   386,   387,
       0,     0,   407,   408,   409,   410,   411,   412,   413,   414,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     416,   388,   389,     0,     0,   392,     0,     0,   393,   394,
     395,   396,     0,   397,   398,   399,   400,     0,     0,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,   405,   406,   407,   408,   409,
       0,     0,     0,   413,   414,     0,     0,   388,   389,   390,
     391,   392,     0,   415,   393,   416,     0,   396,     0,   397,
     398,   399,   400,     0,     0,   401,     0,   402,   403,     0,
       0,     0,     0,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,     0,     0,   408,   409,   410,   411,   412,   413,
     414,   128,   129,   130,   131,     0,     0,     0,     0,   415,
       0,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,     0,     0,   142,   143
};

static const yytype_int16 yycheck[] =
{
      12,   101,   253,   283,   350,   148,   214,   323,    17,   325,
      67,   327,    33,   440,   101,   233,   234,   235,   371,   220,
     251,    32,    45,   109,   418,    18,   122,    31,   517,    20,
       0,   114,   111,   112,    18,     5,    13,    14,   132,   132,
     108,   132,    18,   111,   112,   146,   109,   146,   132,   146,
     132,   348,   132,   139,   132,   132,    68,   132,    28,   356,
      30,    52,    32,   132,    34,   145,   138,   138,   169,    39,
     169,   138,   169,   164,   170,    45,   139,   171,   171,   100,
     101,    51,   168,   162,   139,   138,   169,   171,   168,   101,
     138,   485,   160,   171,   162,   107,   171,   169,   169,   168,
      31,    31,   169,   165,   126,    36,    36,   169,   109,   132,
     341,   132,   165,   213,   169,   163,   117,    87,   162,   219,
     169,   132,   171,   132,    55,    55,   213,   171,   228,   229,
     230,   231,   219,   233,   234,   235,   236,   564,   139,   628,
     144,   228,   229,   230,   231,   138,   233,   234,   235,   126,
     138,   208,   449,   130,   138,   132,   133,   168,   162,   150,
     260,   458,   138,   141,   138,   132,   138,   385,   172,   160,
     270,    13,    14,   260,   109,   138,   170,   165,   109,   110,
     110,   281,   282,   270,   138,   482,   141,   164,   439,   132,
     168,   165,   162,   165,   281,   282,     4,   162,     6,   169,
     165,   213,   165,   138,   139,   521,   141,   219,   139,   140,
     140,   165,   138,   144,   144,   170,   228,   229,   230,   231,
      56,   233,   234,   235,   236,   368,   138,   370,   622,   509,
     163,   162,   162,   434,   163,   132,   169,   163,   632,   169,
     169,   172,   172,   451,    10,    19,    20,   347,   260,   139,
     169,   163,   171,   163,   648,    21,    22,   138,   270,   169,
     347,   361,   362,   141,   364,   365,    19,    20,   138,   281,
     282,   169,   625,   171,   361,   362,   132,   364,   365,   673,
     633,   168,   166,   167,   126,   582,   170,   132,   130,    62,
     132,   133,   138,    66,   132,   141,   593,   515,   144,   134,
     135,   136,   169,   164,   171,   448,   134,   135,   170,    82,
      83,    84,    85,   170,   126,   132,   132,   132,   132,   139,
     420,   170,   141,    33,   640,    33,   100,   101,   132,   134,
     430,   164,   432,   420,   108,   347,   110,   111,   112,   113,
      52,   170,   116,   430,   141,    98,    99,   100,   101,   361,
     362,   141,   364,   365,   145,   108,   169,   110,   111,   112,
     113,   145,   145,   116,   145,   118,   119,   145,   164,   145,
     382,   170,   580,   147,   148,   149,   150,   151,   145,    42,
     588,   727,   145,   145,   132,   132,   160,   169,   162,   132,
     162,   599,   167,   164,   147,   148,   149,   150,   151,   164,
     145,   145,   162,   164,   145,   226,    31,   160,   420,   162,
     145,    36,    19,    20,   164,   515,   165,   144,   430,   132,
     432,    31,   522,   244,   163,   132,    36,   132,   515,   132,
      55,   132,   146,   114,   126,   522,   169,   146,   646,    35,
     146,   169,    18,   169,   169,    55,   169,   169,   163,   138,
     138,   702,   171,   132,   134,   132,    12,   141,   279,   280,
     164,   604,   283,    38,   126,   132,   164,   132,   170,   677,
     291,   292,   293,   294,   295,   296,   163,   163,   163,   132,
     163,    48,   146,   165,   109,   110,   146,   163,   163,   163,
     375,   163,   117,   100,   101,   163,    51,    90,   171,   767,
     110,   108,    54,   515,   111,   112,   113,   223,    52,   116,
     522,    68,   305,   613,   139,   140,   684,    19,    20,   144,
     728,     1,   352,   255,   432,   163,   613,   348,   604,   350,
     140,    64,   270,   631,   144,   356,   146,   162,   207,   360,
     237,   762,   363,   751,   169,   371,   754,   172,   369,    -1,
     371,    -1,   162,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,   388,   389,    -1,
      -1,   392,   393,   394,   395,    -1,   397,    -1,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,    -1,   416,    98,    99,   100,   101,
     102,   613,    -1,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    19,
      20,    -1,    -1,    -1,   445,    -1,    -1,    -1,   449,    -1,
      -1,   452,   453,    -1,    -1,    -1,   457,   458,    19,    20,
      -1,    -1,   463,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,   755,    -1,    -1,   160,    -1,
     162,   482,    31,    -1,    -1,   486,    -1,    36,   755,    -1,
      -1,    -1,    -1,    -1,    -1,   496,   776,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   505,    -1,    55,   508,   509,   776,
      -1,   512,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   714,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     110,   111,   112,   113,    -1,    -1,   116,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,
     111,   112,   113,   745,    -1,   116,    -1,    -1,    -1,    -1,
     561,   110,    -1,   755,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,    -1,
     160,   582,   162,    -1,   776,    -1,    -1,    -1,   149,   150,
     151,   140,   593,    -1,    -1,   144,    -1,   146,    -1,   160,
      -1,   162,    -1,    -1,    -1,   606,   607,    -1,    11,    -1,
      -1,    -1,    -1,   162,    -1,    -1,   617,   618,    -1,   620,
      23,    -1,   623,   172,    -1,    -1,    29,    -1,   629,    -1,
     631,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,   644,    -1,    -1,    -1,    -1,    -1,    -1,
     651,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,   688,   689,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   698,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   717,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   727,    -1,    -1,   132,
     731,    -1,    -1,    -1,    -1,   736,    -1,    -1,    -1,   740,
      -1,   742,   743,    -1,    -1,    -1,    -1,   748,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,     9,    -1,    11,    -1,
      13,    14,    15,    16,    -1,   168,    -1,    -1,    -1,   770,
      -1,    24,    25,    26,    27,    -1,    -1,   778,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    47,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,   148,   149,    -1,    -1,    -1,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,   167,   168,   169,   170,   171,     1,
      -1,    -1,    -1,     5,    -1,     7,     8,     9,    -1,    11,
      -1,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    47,    -1,    49,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,    -1,
      -1,    -1,   154,   155,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,   164,    -1,   166,   167,   168,   169,   170,   171,
      25,    -1,    -1,    -1,    -1,    31,    31,    -1,    -1,    -1,
      36,    36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    55,
      55,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,   140,   140,    -1,    -1,   144,   144,
     146,    -1,   147,   148,   149,    -1,    -1,    -1,    -1,   154,
     155,    -1,    13,    14,    15,    16,   162,   162,   163,   164,
      -1,   166,   167,   168,    25,   170,   172,   172,    -1,    31,
      31,    -1,    -1,    -1,    36,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    55,    55,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    77,    78,    79,    -1,
      -1,    82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,   140,   140,
      -1,    -1,   144,   144,   146,    -1,   147,   148,   149,    -1,
      -1,    -1,    -1,   154,   155,    -1,    13,    14,    15,    16,
     162,   162,    -1,   164,    -1,   166,   167,   168,    25,   170,
     172,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    31,   115,    -1,
      -1,    -1,    36,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    13,    14,    15,    16,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,   164,    -1,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   110,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    -1,    82,    83,    84,    85,   140,    -1,    88,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,   172,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
      -1,    -1,    -1,    -1,   154,   155,    -1,    13,    14,    15,
      16,    -1,   162,    -1,   164,   165,   166,   167,   168,    25,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      -1,    77,    78,    79,    -1,    -1,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,    17,
      -1,   147,   148,   149,    -1,    -1,    -1,    25,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    37,
     166,   167,   168,    41,   170,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    -1,    82,    83,    84,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,   145,    -1,   147,
     148,   149,    -1,    -1,    -1,    25,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    37,   166,   167,
     168,    41,   170,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    -1,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,   147,   148,   149,
      -1,    -1,    -1,    25,   154,   155,    31,    -1,    -1,    -1,
      -1,    36,    -1,   163,   164,    37,   166,   167,   168,    41,
     170,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    31,   115,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,   140,    55,    -1,    -1,   144,
      -1,   146,    -1,    -1,    -1,   147,   148,   149,    13,    14,
      15,    16,   154,   155,    -1,    20,    -1,   162,    -1,    -1,
      25,    -1,   164,    -1,   166,   167,   168,   172,   170,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,   140,    -1,    88,    -1,   144,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,   147,   148,   149,    -1,    -1,    -1,    25,   154,
     155,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   164,
      37,   166,   167,   168,    41,   170,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    31,    31,    -1,
      -1,    -1,    36,    36,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
     140,    55,    55,    -1,   144,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    13,    14,    15,    16,   154,   155,    -1,
      -1,    -1,   162,    -1,   164,    25,    -1,   164,   165,   166,
     167,   168,   172,   170,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,   110,   110,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    -1,    82,    83,    84,    85,   140,   140,    88,    -1,
     144,   144,   146,   146,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,   162,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,   172,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,   147,   148,   149,
      -1,    -1,    -1,    25,   154,   155,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,   164,    37,   166,   167,   168,    41,
     170,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    31,    -1,    -1,    -1,    36,    36,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,   140,    55,    55,    -1,   144,
      -1,   146,    -1,    -1,    -1,   147,   148,   149,    13,    14,
      15,    16,   154,   155,    -1,    -1,    -1,   162,    -1,    -1,
      25,    -1,   164,   165,   166,   167,   168,   172,   170,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   110,   110,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,   140,   140,    88,    -1,   144,   144,   146,   146,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   162,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   172,   172,    36,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,    13,    14,    15,    16,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,   164,
     165,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    -1,    82,    83,    84,    85,    -1,   140,
      88,    -1,    -1,   144,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   172,    36,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,    13,    14,    15,    16,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   164,   165,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    77,    78,    79,    -1,
      -1,    82,    83,    84,    85,    -1,   140,    88,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,   165,    -1,    -1,   115,    -1,    -1,    -1,   172,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    15,    16,    -1,    -1,   147,   148,   149,    -1,
      -1,    -1,    25,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    37,   166,   167,   168,    41,   170,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,   147,   148,   149,    -1,    -1,    -1,
      25,   154,   155,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,   164,    37,   166,   167,   168,    41,   170,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      31,    -1,    -1,    -1,    36,    36,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,   140,    55,    55,    -1,   144,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,    13,    14,    15,    16,   154,
     155,    -1,    -1,    -1,   162,    -1,    -1,    25,    -1,   164,
     165,   166,   167,   168,   172,   170,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   110,   110,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    -1,    82,    83,    84,    85,   140,   140,
      88,    -1,   144,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   162,    -1,   165,   165,    -1,    -1,    -1,    -1,    -1,
     172,   172,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,    13,    14,    15,    16,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   164,   165,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    -1,    77,    78,    79,    -1,
      -1,    82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    15,    16,    17,    -1,   147,   148,   149,    -1,
      -1,    -1,    25,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    37,   166,   167,   168,    41,   170,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    17,    -1,   147,   148,   149,    -1,    -1,    -1,
      25,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    37,   166,   167,   168,    41,   170,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,   147,   148,   149,    -1,    -1,    -1,    25,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      37,   166,   167,   168,    41,   170,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    11,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    23,    -1,    -1,    -1,   154,   155,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   166,
     167,   168,    -1,   170,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    19,    20,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    19,    20,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    19,    20,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    19,    20,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    19,    20,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    19,    20,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,    -1,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,   160,    -1,   162,    98,    99,   100,   101,
     102,    -1,   169,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   162,    98,    99,   100,   101,   102,    -1,   169,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,    -1,
      19,    20,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,
     165,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,   165,    98,
      99,   100,   101,   102,    -1,    -1,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,   160,    -1,   162,    -1,    -1,   165,    98,    99,   100,
     101,   102,    -1,    -1,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,   160,
      -1,   162,    -1,    -1,   165,    98,    99,   100,   101,   102,
      -1,    -1,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,   160,    -1,   162,
      -1,    -1,   165,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,
     165,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,   165,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   164,    -1,    19,    20,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      19,    20,   168,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    98,
      99,   100,   101,   102,    -1,    -1,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,   163,    19,    20,    -1,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   162,   163,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   162,    98,    99,   100,   101,   102,    -1,    -1,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,    -1,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   162,    98,    99,   100,   101,   102,
      -1,    -1,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,   140,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,
      -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   162,    -1,    -1,    -1,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    98,    99,    -1,    -1,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,   150,   151,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   160,   105,   162,    -1,   108,    -1,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    -1,    -1,    -1,   145,   146,   147,   148,   149,   150,
     151,   118,   119,   120,   121,    -1,    -1,    -1,    -1,   160,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,   154,   155
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   174,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    87,   162,   169,   175,   180,   181,   185,   204,   208,
     222,   254,   258,   259,   262,   269,   270,   132,   199,   200,
     126,   186,   187,   132,   182,   183,   132,   170,   132,    32,
     132,   168,   201,   202,   203,   213,     4,     6,    56,   253,
     132,   139,   138,   141,   138,    20,    52,   150,   160,   184,
     141,   170,   132,   256,   257,   168,   132,   164,   138,   163,
     132,   261,    33,   100,   101,   132,   207,   170,   170,    13,
      14,   126,   130,   132,   133,   164,   177,   199,   126,   187,
     132,   132,   132,    62,    66,    82,    83,    84,    85,   265,
     255,   139,   171,   256,   132,   200,   202,   141,   260,   170,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   118,   119,
     120,   121,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   154,   155,    33,    33,   164,   205,   252,   132,
     245,   246,   134,   176,    13,    14,   126,   130,   132,   177,
     197,   198,   184,   170,   132,   171,    23,    29,    44,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
     213,   264,   266,   267,   268,   165,   213,   240,   162,   165,
     237,   241,   242,   141,   206,   132,   171,   251,   263,   141,
     169,   134,   135,   138,   165,   255,   139,   169,   145,   145,
     145,   145,   164,   145,   145,   145,   145,   145,    31,    36,
      55,   110,   140,   144,   162,   169,   172,   171,   237,   239,
     200,    45,   248,   263,   165,   169,   268,   170,   209,   138,
     141,   144,   250,   268,   171,   245,   197,   171,    13,    14,
      15,    16,    25,    37,    41,    43,    46,    50,    88,   100,
     101,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     133,   147,   148,   149,   154,   155,   164,   166,   167,   179,
     209,   213,   215,   224,   225,   228,   229,   230,   233,   234,
     235,   236,   264,   271,   274,   279,   281,   283,   268,   268,
     268,   268,   132,   205,   268,   205,   268,   205,   268,   132,
     243,   244,   268,   246,    31,   144,   162,   172,   163,   236,
      42,   238,   169,   163,   248,   109,   139,   141,   247,   241,
     211,   132,   268,   109,   117,   139,   249,   268,   271,   164,
     164,   145,   145,   164,   145,   145,   236,   236,    11,   268,
      11,   268,   236,   276,   280,   178,   236,   236,   236,   236,
     236,   236,   167,   132,   164,   204,    19,    20,    98,    99,
     100,   101,   102,   105,   106,   107,   108,   110,   111,   112,
     113,   116,   118,   119,   140,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   160,   162,   169,   164,   146,
     169,   146,   146,   165,   206,   146,   206,   146,   206,   146,
     141,   146,   169,   146,   169,   144,   163,   163,   248,   268,
     115,   212,   231,   236,     1,     7,     8,     9,    11,    24,
      26,    27,    36,    40,    47,    49,    53,    54,    89,   169,
     171,   188,   189,   192,   193,   194,   195,   196,   210,   212,
     214,   216,   217,   218,   219,   220,   222,   223,   231,   232,
     236,   169,   249,   212,   236,   164,   132,   236,   268,   268,
     236,   268,   268,   263,   132,   163,   164,   236,   272,   273,
     275,   276,   277,   278,   263,   164,   273,   278,   122,   169,
     171,   135,   136,   176,   165,   145,   213,   162,   165,   226,
     236,   205,    17,   132,   132,   236,   236,   236,   236,   236,
     236,   132,   236,   132,   236,   236,   236,   236,   236,    20,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   132,   236,   165,   226,   268,   146,   146,   146,   268,
     243,   247,   167,   229,   114,    90,    91,    92,    93,    94,
      95,    96,    97,   103,   104,   109,   117,   120,   121,   139,
     236,   263,   109,   169,   212,   236,   209,   236,   236,   169,
     126,    53,   236,   109,   212,   236,    35,   169,   169,   236,
     169,   169,   169,    18,   221,   169,   109,   139,   212,   236,
     169,   165,   226,    17,   145,   236,   165,   146,   146,   165,
     146,   146,    18,   247,   163,   165,   138,   163,   169,   138,
     163,   169,    18,   165,   171,   171,   236,   276,   171,   236,
     205,   268,   132,   272,   138,   165,   206,   268,   164,   163,
     132,   141,   163,   165,   146,   236,   229,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   209,    18,   212,   236,   169,    12,   209,   141,
     126,   212,   236,   169,   170,   209,    38,   251,   115,   115,
     169,   165,   268,   132,   165,   236,   236,   236,   164,   226,
     236,   273,   132,   163,   272,   236,   163,   277,   226,   273,
     163,   163,   137,   206,   146,   163,   236,   122,   209,   227,
     165,   226,   236,   226,   169,   209,   169,   211,    10,    21,
      22,   190,   191,   236,   236,   165,   146,   169,   165,   236,
     169,   163,   247,   169,   171,   146,   213,   165,   109,   236,
     165,   169,   171,   209,   236,    17,   236,   132,   165,   236,
     163,   236,   236,   163,   213,   236,   209,   209,   268,   165,
     146,   169,   282,   282,   191,   165,    17,   236,    48,   163,
     171,   268,   165,   236,   163,   163,   165
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   173,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   175,   176,   176,   177,   178,   178,
     178,   179,   180,   181,   182,   182,   182,   183,   183,   184,
     184,   185,   186,   186,   187,   187,   188,   189,   189,   190,
     190,   191,   191,   191,   192,   192,   193,   194,   195,   196,
     197,   197,   197,   197,   197,   197,   198,   198,   199,   199,
     199,   199,   199,   199,   199,   199,   200,   200,   201,   201,
     202,   202,   203,   203,   204,   204,   205,   205,   205,   206,
     206,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   208,   209,
     209,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   211,
     211,   211,   212,   212,   213,   213,   213,   214,   215,   215,
     215,   215,   216,   217,   218,   218,   218,   218,   218,   219,
     219,   219,   219,   220,   221,   221,   222,   222,   223,   224,
     224,   224,   225,   225,   225,   225,   225,   225,   226,   226,
     227,   227,   227,   228,   228,   229,   229,   230,   230,   230,
     230,   230,   230,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   232,
     232,   233,   234,   234,   235,   235,   235,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     237,   237,   238,   238,   239,   240,   240,   241,   241,   242,
     242,   243,   243,   244,   244,   245,   246,   246,   247,   247,
     248,   248,   248,   248,   248,   249,   249,   249,   250,   250,
     251,   251,   251,   251,   251,   252,   252,   253,   253,   254,
     255,   255,   255,   256,   257,   257,   258,   259,   259,   260,
     260,   261,   262,   263,   263,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   265,
     265,   265,   265,   265,   265,   266,   267,   267,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   269,
     270,   271,   271,   271,   271,   272,   272,   273,   273,   274,
     274,   274,   274,   274,   275,   275,   276,   277,   277,   278,
     278,   279,   279,   280,   280,   281,   282,   282,   283,   283
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     3,     0,     2,
       4,     3,     2,     2,     1,     3,     3,     2,     4,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     2,     1,
       1,     0,     2,     4,     1,     1,     4,     6,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     5,     1,     3,     1,     1,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     3,
       7,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     2,     1,     3,     2,     2,     2,     4,
       5,     2,     1,     1,     2,     3,     4,     2,     3,     3,
       4,     2,     3,     4,     0,     2,     1,     1,     3,     5,
       5,     5,     5,     8,    10,     6,     9,    11,     1,     3,
       1,     2,     3,     1,     1,     1,     5,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     6,     5,     6,     3,     6,     7,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     4,     3,     3,     1,     3,     4,     3,     4,
       2,     4,     4,     6,     7,     3,     5,     4,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     0,     1,     3,     0,     3,     2,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       4,     6,     5,     5,     4,     0,     2,     0,     1,     5,
       0,     3,     5,     4,     1,     2,     4,     5,     7,     0,
       2,     2,     6,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     4,     3,     4,     2,     3,     2,     4,     3,     2,
       2,     3,     2,     4,     2,     4,     6,     4,     1,     4,
       5,     1,     4,     5,     1,     4,     5,     4,     4,     1,
       6,     1,     1,     1,     1,     3,     5,     1,     3,     5,
       4,     7,     5,     7,     1,     3,     3,     1,     1,     1,
       3,     5,     5,     1,     3,     4,     0,     3,    10,    10
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
    case 132: /* "name"  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3108 "ds_parser.cpp"
        break;

    case 176: /* character_sequence  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3114 "ds_parser.cpp"
        break;

    case 177: /* string_constant  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3120 "ds_parser.cpp"
        break;

    case 178: /* string_builder_body  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3126 "ds_parser.cpp"
        break;

    case 179: /* string_builder  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp"
        break;

    case 182: /* require_module_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3138 "ds_parser.cpp"
        break;

    case 188: /* expression_label  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3144 "ds_parser.cpp"
        break;

    case 189: /* expression_goto  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3150 "ds_parser.cpp"
        break;

    case 191: /* expression_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3156 "ds_parser.cpp"
        break;

    case 193: /* expression_if_then_else  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3162 "ds_parser.cpp"
        break;

    case 194: /* expression_for_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3168 "ds_parser.cpp"
        break;

    case 195: /* expression_while_loop  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3174 "ds_parser.cpp"
        break;

    case 196: /* expression_with  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3180 "ds_parser.cpp"
        break;

    case 197: /* annotation_argument_value  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3186 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3192 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3198 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3204 "ds_parser.cpp"
        break;

    case 201: /* annotation_declaration_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3210 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3216 "ds_parser.cpp"
        break;

    case 203: /* annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3222 "ds_parser.cpp"
        break;

    case 204: /* optional_annotation_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3228 "ds_parser.cpp"
        break;

    case 205: /* optional_function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3234 "ds_parser.cpp"
        break;

    case 206: /* optional_function_type  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3240 "ds_parser.cpp"
        break;

    case 207: /* function_name  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3246 "ds_parser.cpp"
        break;

    case 209: /* expression_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3252 "ds_parser.cpp"
        break;

    case 210: /* expression_any  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp"
        break;

    case 211: /* expressions  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp"
        break;

    case 212: /* expr_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp"
        break;

    case 213: /* name_in_namespace  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3276 "ds_parser.cpp"
        break;

    case 214: /* expression_delete  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3282 "ds_parser.cpp"
        break;

    case 215: /* expr_new  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp"
        break;

    case 216: /* expression_break  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp"
        break;

    case 217: /* expression_continue  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp"
        break;

    case 218: /* expression_return  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp"
        break;

    case 219: /* expression_yield  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp"
        break;

    case 220: /* expression_try_catch  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp"
        break;

    case 223: /* expression_let  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp"
        break;

    case 224: /* expr_cast  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp"
        break;

    case 225: /* expr_type_info  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 226: /* expr_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 227: /* block_or_simple_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 229: /* expr_block  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp"
        break;

    case 230: /* expr_numeric_const  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 231: /* expr_assign  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 232: /* expr_assign_pipe  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 233: /* expr_named_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 234: /* expr_method_call  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3384 "ds_parser.cpp"
        break;

    case 235: /* func_addr_expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3390 "ds_parser.cpp"
        break;

    case 236: /* expr  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3396 "ds_parser.cpp"
        break;

    case 237: /* optional_field_annotation  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3402 "ds_parser.cpp"
        break;

    case 239: /* structure_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3408 "ds_parser.cpp"
        break;

    case 240: /* struct_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3414 "ds_parser.cpp"
        break;

    case 241: /* function_argument_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3420 "ds_parser.cpp"
        break;

    case 242: /* function_argument_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3426 "ds_parser.cpp"
        break;

    case 243: /* tuple_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3432 "ds_parser.cpp"
        break;

    case 244: /* tuple_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3438 "ds_parser.cpp"
        break;

    case 245: /* variant_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3444 "ds_parser.cpp"
        break;

    case 246: /* variant_type_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3450 "ds_parser.cpp"
        break;

    case 248: /* variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3456 "ds_parser.cpp"
        break;

    case 251: /* let_variable_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3462 "ds_parser.cpp"
        break;

    case 252: /* global_variable_declaration_list  */
#line 107 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3468 "ds_parser.cpp"
        break;

    case 255: /* enum_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3474 "ds_parser.cpp"
        break;

    case 260: /* optional_structure_parent  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3480 "ds_parser.cpp"
        break;

    case 263: /* variable_name_list  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3486 "ds_parser.cpp"
        break;

    case 266: /* structure_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3492 "ds_parser.cpp"
        break;

    case 267: /* auto_type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3498 "ds_parser.cpp"
        break;

    case 268: /* type_declaration  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3504 "ds_parser.cpp"
        break;

    case 271: /* make_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp"
        break;

    case 272: /* make_struct_fields  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3516 "ds_parser.cpp"
        break;

    case 273: /* make_struct_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp"
        break;

    case 274: /* make_struct_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 275: /* make_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 276: /* make_map_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 277: /* make_any_tuple  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 278: /* make_dim  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 279: /* make_dim_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 280: /* make_table  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 281: /* make_table_decl  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 282: /* array_comprehension_where  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 283: /* array_comprehension  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
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
  case 14:
#line 403 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3882 "ds_parser.cpp"
    break;

  case 15:
#line 415 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3888 "ds_parser.cpp"
    break;

  case 16:
#line 416 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3894 "ds_parser.cpp"
    break;

  case 17:
#line 420 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3900 "ds_parser.cpp"
    break;

  case 18:
#line 424 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 3910 "ds_parser.cpp"
    break;

  case 19:
#line 429 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3924 "ds_parser.cpp"
    break;

  case 20:
#line 438 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3934 "ds_parser.cpp"
    break;

  case 21:
#line 446 "ds_parser.ypp"
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
#line 3952 "ds_parser.cpp"
    break;

  case 22:
#line 462 "ds_parser.ypp"
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
            g_FileAccessStack.back()->tabSize = das_tab_size;
		}
		delete (yyvsp[0].aaList);
	}
#line 3976 "ds_parser.cpp"
    break;

  case 24:
#line 488 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3984 "ds_parser.cpp"
    break;

  case 25:
#line 491 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3995 "ds_parser.cpp"
    break;

  case 26:
#line 497 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4006 "ds_parser.cpp"
    break;

  case 27:
#line 506 "ds_parser.ypp"
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
#line 4032 "ds_parser.cpp"
    break;

  case 28:
#line 527 "ds_parser.ypp"
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
#line 4057 "ds_parser.cpp"
    break;

  case 29:
#line 550 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4063 "ds_parser.cpp"
    break;

  case 30:
#line 551 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4069 "ds_parser.cpp"
    break;

  case 34:
#line 564 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4077 "ds_parser.cpp"
    break;

  case 35:
#line 567 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4085 "ds_parser.cpp"
    break;

  case 36:
#line 573 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4093 "ds_parser.cpp"
    break;

  case 37:
#line 579 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4101 "ds_parser.cpp"
    break;

  case 38:
#line 582 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4109 "ds_parser.cpp"
    break;

  case 39:
#line 588 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4115 "ds_parser.cpp"
    break;

  case 40:
#line 589 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4121 "ds_parser.cpp"
    break;

  case 41:
#line 593 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4127 "ds_parser.cpp"
    break;

  case 42:
#line 594 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4133 "ds_parser.cpp"
    break;

  case 43:
#line 595 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4144 "ds_parser.cpp"
    break;

  case 44:
#line 604 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4150 "ds_parser.cpp"
    break;

  case 45:
#line 605 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4156 "ds_parser.cpp"
    break;

  case 46:
#line 609 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4167 "ds_parser.cpp"
    break;

  case 47:
#line 618 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4181 "ds_parser.cpp"
    break;

  case 48:
#line 630 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4193 "ds_parser.cpp"
    break;

  case 49:
#line 640 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4204 "ds_parser.cpp"
    break;

  case 50:
#line 649 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4210 "ds_parser.cpp"
    break;

  case 51:
#line 650 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4216 "ds_parser.cpp"
    break;

  case 52:
#line 651 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4222 "ds_parser.cpp"
    break;

  case 53:
#line 652 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4228 "ds_parser.cpp"
    break;

  case 54:
#line 653 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4234 "ds_parser.cpp"
    break;

  case 55:
#line 654 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4240 "ds_parser.cpp"
    break;

  case 56:
#line 658 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4250 "ds_parser.cpp"
    break;

  case 57:
#line 663 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4260 "ds_parser.cpp"
    break;

  case 58:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4266 "ds_parser.cpp"
    break;

  case 59:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4272 "ds_parser.cpp"
    break;

  case 60:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4278 "ds_parser.cpp"
    break;

  case 61:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4284 "ds_parser.cpp"
    break;

  case 62:
#line 675 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 63:
#line 676 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4296 "ds_parser.cpp"
    break;

  case 64:
#line 677 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4302 "ds_parser.cpp"
    break;

  case 65:
#line 678 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4310 "ds_parser.cpp"
    break;

  case 66:
#line 684 "ds_parser.ypp"
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
#line 4328 "ds_parser.cpp"
    break;

  case 67:
#line 697 "ds_parser.ypp"
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
#line 4346 "ds_parser.cpp"
    break;

  case 68:
#line 713 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4352 "ds_parser.cpp"
    break;

  case 69:
#line 714 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4358 "ds_parser.cpp"
    break;

  case 70:
#line 718 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4370 "ds_parser.cpp"
    break;

  case 71:
#line 725 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4384 "ds_parser.cpp"
    break;

  case 72:
#line 737 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4393 "ds_parser.cpp"
    break;

  case 73:
#line 741 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4402 "ds_parser.cpp"
    break;

  case 74:
#line 748 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4408 "ds_parser.cpp"
    break;

  case 75:
#line 749 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4414 "ds_parser.cpp"
    break;

  case 76:
#line 753 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4420 "ds_parser.cpp"
    break;

  case 77:
#line 754 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4426 "ds_parser.cpp"
    break;

  case 78:
#line 755 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4432 "ds_parser.cpp"
    break;

  case 79:
#line 759 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4442 "ds_parser.cpp"
    break;

  case 80:
#line 764 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4451 "ds_parser.cpp"
    break;

  case 81:
#line 771 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4460 "ds_parser.cpp"
    break;

  case 82:
#line 775 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4466 "ds_parser.cpp"
    break;

  case 83:
#line 776 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4472 "ds_parser.cpp"
    break;

  case 84:
#line 777 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4478 "ds_parser.cpp"
    break;

  case 85:
#line 778 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4484 "ds_parser.cpp"
    break;

  case 86:
#line 779 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4490 "ds_parser.cpp"
    break;

  case 87:
#line 780 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4496 "ds_parser.cpp"
    break;

  case 88:
#line 781 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4502 "ds_parser.cpp"
    break;

  case 89:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4508 "ds_parser.cpp"
    break;

  case 90:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4514 "ds_parser.cpp"
    break;

  case 91:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4520 "ds_parser.cpp"
    break;

  case 92:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4526 "ds_parser.cpp"
    break;

  case 93:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4532 "ds_parser.cpp"
    break;

  case 94:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4538 "ds_parser.cpp"
    break;

  case 95:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4544 "ds_parser.cpp"
    break;

  case 96:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4550 "ds_parser.cpp"
    break;

  case 97:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4556 "ds_parser.cpp"
    break;

  case 98:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4562 "ds_parser.cpp"
    break;

  case 99:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4568 "ds_parser.cpp"
    break;

  case 100:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4574 "ds_parser.cpp"
    break;

  case 101:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4580 "ds_parser.cpp"
    break;

  case 102:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4586 "ds_parser.cpp"
    break;

  case 103:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4592 "ds_parser.cpp"
    break;

  case 104:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4598 "ds_parser.cpp"
    break;

  case 105:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4604 "ds_parser.cpp"
    break;

  case 106:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4610 "ds_parser.cpp"
    break;

  case 107:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4616 "ds_parser.cpp"
    break;

  case 108:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4622 "ds_parser.cpp"
    break;

  case 109:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4628 "ds_parser.cpp"
    break;

  case 110:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4634 "ds_parser.cpp"
    break;

  case 111:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4640 "ds_parser.cpp"
    break;

  case 112:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4646 "ds_parser.cpp"
    break;

  case 113:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4652 "ds_parser.cpp"
    break;

  case 114:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4658 "ds_parser.cpp"
    break;

  case 115:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4664 "ds_parser.cpp"
    break;

  case 116:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4670 "ds_parser.cpp"
    break;

  case 117:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4676 "ds_parser.cpp"
    break;

  case 118:
#line 815 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-4]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
							if ( pDecl->annotation ) {
								pVar->annotation = *pDecl->annotation;
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
#line 4750 "ds_parser.cpp"
    break;

  case 119:
#line 887 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4758 "ds_parser.cpp"
    break;

  case 120:
#line 890 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4770 "ds_parser.cpp"
    break;

  case 121:
#line 900 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4776 "ds_parser.cpp"
    break;

  case 122:
#line 901 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4782 "ds_parser.cpp"
    break;

  case 123:
#line 902 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4788 "ds_parser.cpp"
    break;

  case 124:
#line 903 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4794 "ds_parser.cpp"
    break;

  case 125:
#line 904 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4800 "ds_parser.cpp"
    break;

  case 126:
#line 905 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4806 "ds_parser.cpp"
    break;

  case 127:
#line 906 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4812 "ds_parser.cpp"
    break;

  case 128:
#line 907 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4818 "ds_parser.cpp"
    break;

  case 129:
#line 908 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4824 "ds_parser.cpp"
    break;

  case 130:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4830 "ds_parser.cpp"
    break;

  case 131:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4836 "ds_parser.cpp"
    break;

  case 132:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4842 "ds_parser.cpp"
    break;

  case 133:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4848 "ds_parser.cpp"
    break;

  case 134:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4854 "ds_parser.cpp"
    break;

  case 135:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4860 "ds_parser.cpp"
    break;

  case 136:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4866 "ds_parser.cpp"
    break;

  case 137:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4872 "ds_parser.cpp"
    break;

  case 138:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4878 "ds_parser.cpp"
    break;

  case 139:
#line 921 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4888 "ds_parser.cpp"
    break;

  case 140:
#line 926 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4899 "ds_parser.cpp"
    break;

  case 141:
#line 932 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4907 "ds_parser.cpp"
    break;

  case 142:
#line 938 "ds_parser.ypp"
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
#line 4923 "ds_parser.cpp"
    break;

  case 143:
#line 949 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4931 "ds_parser.cpp"
    break;

  case 144:
#line 955 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4937 "ds_parser.cpp"
    break;

  case 145:
#line 956 "ds_parser.ypp"
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
#line 4953 "ds_parser.cpp"
    break;

  case 146:
#line 967 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4959 "ds_parser.cpp"
    break;

  case 147:
#line 971 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4967 "ds_parser.cpp"
    break;

  case 148:
#line 977 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4975 "ds_parser.cpp"
    break;

  case 149:
#line 980 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4983 "ds_parser.cpp"
    break;

  case 150:
#line 983 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4992 "ds_parser.cpp"
    break;

  case 151:
#line 987 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5000 "ds_parser.cpp"
    break;

  case 152:
#line 993 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5006 "ds_parser.cpp"
    break;

  case 153:
#line 997 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5012 "ds_parser.cpp"
    break;

  case 154:
#line 1001 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5020 "ds_parser.cpp"
    break;

  case 155:
#line 1004 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5028 "ds_parser.cpp"
    break;

  case 156:
#line 1007 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5038 "ds_parser.cpp"
    break;

  case 157:
#line 1012 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5046 "ds_parser.cpp"
    break;

  case 158:
#line 1015 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5056 "ds_parser.cpp"
    break;

  case 159:
#line 1023 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5064 "ds_parser.cpp"
    break;

  case 160:
#line 1026 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5074 "ds_parser.cpp"
    break;

  case 161:
#line 1031 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5082 "ds_parser.cpp"
    break;

  case 162:
#line 1034 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5092 "ds_parser.cpp"
    break;

  case 163:
#line 1042 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5100 "ds_parser.cpp"
    break;

  case 164:
#line 1048 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5106 "ds_parser.cpp"
    break;

  case 165:
#line 1049 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5112 "ds_parser.cpp"
    break;

  case 166:
#line 1053 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5118 "ds_parser.cpp"
    break;

  case 167:
#line 1054 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5124 "ds_parser.cpp"
    break;

  case 168:
#line 1058 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->inScope = (yyvsp[-1].b);
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name;
                    pVar->at = (yyvsp[0].pVarDecl)->at;
                    pVar->type = make_smart<TypeDecl>(*(yyvsp[0].pVarDecl)->pTypeDecl);
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
#line 5160 "ds_parser.cpp"
    break;

  case 169:
#line 1092 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5168 "ds_parser.cpp"
    break;

  case 170:
#line 1095 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5178 "ds_parser.cpp"
    break;

  case 171:
#line 1100 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5188 "ds_parser.cpp"
    break;

  case 172:
#line 1108 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5197 "ds_parser.cpp"
    break;

  case 173:
#line 1112 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5207 "ds_parser.cpp"
    break;

  case 174:
#line 1117 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5218 "ds_parser.cpp"
    break;

  case 175:
#line 1123 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5227 "ds_parser.cpp"
    break;

  case 176:
#line 1127 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5237 "ds_parser.cpp"
    break;

  case 177:
#line 1132 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5248 "ds_parser.cpp"
    break;

  case 178:
#line 1141 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5256 "ds_parser.cpp"
    break;

  case 179:
#line 1144 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5264 "ds_parser.cpp"
    break;

  case 180:
#line 1150 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5270 "ds_parser.cpp"
    break;

  case 181:
#line 1151 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5282 "ds_parser.cpp"
    break;

  case 182:
#line 1158 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5295 "ds_parser.cpp"
    break;

  case 183:
#line 1169 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5301 "ds_parser.cpp"
    break;

  case 184:
#line 1170 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5307 "ds_parser.cpp"
    break;

  case 185:
#line 1174 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5317 "ds_parser.cpp"
    break;

  case 186:
#line 1180 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name;
                            pVar->at = pDecl->at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
							if ( pDecl->annotation ) {
								pVar->annotation = *pDecl->annotation;
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
#line 5373 "ds_parser.cpp"
    break;

  case 187:
#line 1234 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5379 "ds_parser.cpp"
    break;

  case 188:
#line 1235 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5385 "ds_parser.cpp"
    break;

  case 189:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5391 "ds_parser.cpp"
    break;

  case 190:
#line 1237 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5397 "ds_parser.cpp"
    break;

  case 191:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5403 "ds_parser.cpp"
    break;

  case 192:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5409 "ds_parser.cpp"
    break;

  case 193:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5415 "ds_parser.cpp"
    break;

  case 194:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5421 "ds_parser.cpp"
    break;

  case 195:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5427 "ds_parser.cpp"
    break;

  case 196:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5433 "ds_parser.cpp"
    break;

  case 197:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5439 "ds_parser.cpp"
    break;

  case 198:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5445 "ds_parser.cpp"
    break;

  case 199:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5451 "ds_parser.cpp"
    break;

  case 200:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5457 "ds_parser.cpp"
    break;

  case 201:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5463 "ds_parser.cpp"
    break;

  case 202:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5469 "ds_parser.cpp"
    break;

  case 203:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5475 "ds_parser.cpp"
    break;

  case 204:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5481 "ds_parser.cpp"
    break;

  case 205:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5487 "ds_parser.cpp"
    break;

  case 206:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5493 "ds_parser.cpp"
    break;

  case 207:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5499 "ds_parser.cpp"
    break;

  case 208:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5505 "ds_parser.cpp"
    break;

  case 209:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5511 "ds_parser.cpp"
    break;

  case 210:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5517 "ds_parser.cpp"
    break;

  case 211:
#line 1267 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5529 "ds_parser.cpp"
    break;

  case 212:
#line 1278 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5539 "ds_parser.cpp"
    break;

  case 213:
#line 1283 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5551 "ds_parser.cpp"
    break;

  case 214:
#line 1293 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5560 "ds_parser.cpp"
    break;

  case 215:
#line 1297 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5572 "ds_parser.cpp"
    break;

  case 216:
#line 1304 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-5])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
		(yyval.pExpression) = expr;
    }
#line 5588 "ds_parser.cpp"
    break;

  case 217:
#line 1318 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5594 "ds_parser.cpp"
    break;

  case 218:
#line 1319 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5600 "ds_parser.cpp"
    break;

  case 219:
#line 1320 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5606 "ds_parser.cpp"
    break;

  case 220:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5612 "ds_parser.cpp"
    break;

  case 221:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5618 "ds_parser.cpp"
    break;

  case 222:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5624 "ds_parser.cpp"
    break;

  case 223:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5630 "ds_parser.cpp"
    break;

  case 224:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5636 "ds_parser.cpp"
    break;

  case 225:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5642 "ds_parser.cpp"
    break;

  case 226:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5648 "ds_parser.cpp"
    break;

  case 227:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5654 "ds_parser.cpp"
    break;

  case 228:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5660 "ds_parser.cpp"
    break;

  case 229:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5666 "ds_parser.cpp"
    break;

  case 230:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5672 "ds_parser.cpp"
    break;

  case 231:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5678 "ds_parser.cpp"
    break;

  case 232:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5684 "ds_parser.cpp"
    break;

  case 233:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5690 "ds_parser.cpp"
    break;

  case 234:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5696 "ds_parser.cpp"
    break;

  case 235:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5702 "ds_parser.cpp"
    break;

  case 236:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5708 "ds_parser.cpp"
    break;

  case 237:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5714 "ds_parser.cpp"
    break;

  case 238:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5720 "ds_parser.cpp"
    break;

  case 239:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5726 "ds_parser.cpp"
    break;

  case 240:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5732 "ds_parser.cpp"
    break;

  case 241:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5738 "ds_parser.cpp"
    break;

  case 242:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5744 "ds_parser.cpp"
    break;

  case 243:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5750 "ds_parser.cpp"
    break;

  case 244:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5756 "ds_parser.cpp"
    break;

  case 245:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5762 "ds_parser.cpp"
    break;

  case 246:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5768 "ds_parser.cpp"
    break;

  case 247:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5774 "ds_parser.cpp"
    break;

  case 248:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5780 "ds_parser.cpp"
    break;

  case 249:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5786 "ds_parser.cpp"
    break;

  case 250:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5792 "ds_parser.cpp"
    break;

  case 251:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5798 "ds_parser.cpp"
    break;

  case 252:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5804 "ds_parser.cpp"
    break;

  case 253:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5810 "ds_parser.cpp"
    break;

  case 254:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5816 "ds_parser.cpp"
    break;

  case 255:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5822 "ds_parser.cpp"
    break;

  case 256:
#line 1357 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5831 "ds_parser.cpp"
    break;

  case 257:
#line 1361 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5840 "ds_parser.cpp"
    break;

  case 258:
#line 1365 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5848 "ds_parser.cpp"
    break;

  case 259:
#line 1368 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5856 "ds_parser.cpp"
    break;

  case 260:
#line 1371 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5862 "ds_parser.cpp"
    break;

  case 261:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5868 "ds_parser.cpp"
    break;

  case 262:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5874 "ds_parser.cpp"
    break;

  case 263:
#line 1374 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5884 "ds_parser.cpp"
    break;

  case 264:
#line 1379 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5894 "ds_parser.cpp"
    break;

  case 265:
#line 1384 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5900 "ds_parser.cpp"
    break;

  case 266:
#line 1385 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5908 "ds_parser.cpp"
    break;

  case 267:
#line 1388 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5916 "ds_parser.cpp"
    break;

  case 268:
#line 1391 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5925 "ds_parser.cpp"
    break;

  case 269:
#line 1395 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5934 "ds_parser.cpp"
    break;

  case 270:
#line 1399 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5943 "ds_parser.cpp"
    break;

  case 271:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5949 "ds_parser.cpp"
    break;

  case 272:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5955 "ds_parser.cpp"
    break;

  case 273:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5961 "ds_parser.cpp"
    break;

  case 274:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5967 "ds_parser.cpp"
    break;

  case 275:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5973 "ds_parser.cpp"
    break;

  case 276:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5979 "ds_parser.cpp"
    break;

  case 277:
#line 1409 "ds_parser.ypp"
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
#line 5994 "ds_parser.cpp"
    break;

  case 278:
#line 1419 "ds_parser.ypp"
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
#line 6009 "ds_parser.cpp"
    break;

  case 279:
#line 1429 "ds_parser.ypp"
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
				auto td = make_smart<TypeDecl>(pEnum);
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
#line 6045 "ds_parser.cpp"
    break;

  case 280:
#line 1463 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6051 "ds_parser.cpp"
    break;

  case 281:
#line 1464 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6057 "ds_parser.cpp"
    break;

  case 282:
#line 1468 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6063 "ds_parser.cpp"
    break;

  case 283:
#line 1469 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6069 "ds_parser.cpp"
    break;

  case 284:
#line 1473 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6079 "ds_parser.cpp"
    break;

  case 285:
#line 1481 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6087 "ds_parser.cpp"
    break;

  case 286:
#line 1484 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6096 "ds_parser.cpp"
    break;

  case 287:
#line 1491 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6106 "ds_parser.cpp"
    break;

  case 288:
#line 1496 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6116 "ds_parser.cpp"
    break;

  case 289:
#line 1504 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6122 "ds_parser.cpp"
    break;

  case 290:
#line 1505 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6128 "ds_parser.cpp"
    break;

  case 291:
#line 1509 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6136 "ds_parser.cpp"
    break;

  case 292:
#line 1512 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6147 "ds_parser.cpp"
    break;

  case 293:
#line 1521 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6153 "ds_parser.cpp"
    break;

  case 294:
#line 1522 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6159 "ds_parser.cpp"
    break;

  case 295:
#line 1526 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6170 "ds_parser.cpp"
    break;

  case 296:
#line 1535 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6176 "ds_parser.cpp"
    break;

  case 297:
#line 1536 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6182 "ds_parser.cpp"
    break;

  case 298:
#line 1541 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6188 "ds_parser.cpp"
    break;

  case 299:
#line 1542 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6194 "ds_parser.cpp"
    break;

  case 300:
#line 1546 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6204 "ds_parser.cpp"
    break;

  case 301:
#line 1551 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6212 "ds_parser.cpp"
    break;

  case 302:
#line 1554 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6221 "ds_parser.cpp"
    break;

  case 303:
#line 1558 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6232 "ds_parser.cpp"
    break;

  case 304:
#line 1564 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6243 "ds_parser.cpp"
    break;

  case 305:
#line 1573 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6249 "ds_parser.cpp"
    break;

  case 306:
#line 1574 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6255 "ds_parser.cpp"
    break;

  case 307:
#line 1575 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6261 "ds_parser.cpp"
    break;

  case 308:
#line 1579 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6267 "ds_parser.cpp"
    break;

  case 309:
#line 1580 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6273 "ds_parser.cpp"
    break;

  case 310:
#line 1584 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6281 "ds_parser.cpp"
    break;

  case 311:
#line 1587 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6291 "ds_parser.cpp"
    break;

  case 312:
#line 1592 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6301 "ds_parser.cpp"
    break;

  case 313:
#line 1597 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6314 "ds_parser.cpp"
    break;

  case 314:
#line 1605 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6327 "ds_parser.cpp"
    break;

  case 315:
#line 1616 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6335 "ds_parser.cpp"
    break;

  case 316:
#line 1619 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6344 "ds_parser.cpp"
    break;

  case 317:
#line 1626 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6350 "ds_parser.cpp"
    break;

  case 318:
#line 1627 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6356 "ds_parser.cpp"
    break;

  case 319:
#line 1631 "ds_parser.ypp"
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name;
                    pVar->at = pDecl->at;
                    pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
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
#line 6388 "ds_parser.cpp"
    break;

  case 320:
#line 1661 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6396 "ds_parser.cpp"
    break;

  case 321:
#line 1664 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6410 "ds_parser.cpp"
    break;

  case 322:
#line 1673 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6424 "ds_parser.cpp"
    break;

  case 323:
#line 1686 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6438 "ds_parser.cpp"
    break;

  case 327:
#line 1707 "ds_parser.ypp"
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
#line 6454 "ds_parser.cpp"
    break;

  case 328:
#line 1718 "ds_parser.ypp"
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
#line 6471 "ds_parser.cpp"
    break;

  case 329:
#line 1733 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6477 "ds_parser.cpp"
    break;

  case 330:
#line 1734 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6483 "ds_parser.cpp"
    break;

  case 331:
#line 1738 "ds_parser.ypp"
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
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
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
#line 6524 "ds_parser.cpp"
    break;

  case 332:
#line 1777 "ds_parser.ypp"
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure);
            pStruct->at = tokAt((yylsp[-4]));
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything, "+name,pDecl->at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
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
#line 6592 "ds_parser.cpp"
    break;

  case 333:
#line 1843 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6604 "ds_parser.cpp"
    break;

  case 334:
#line 1850 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6615 "ds_parser.cpp"
    break;

  case 335:
#line 1859 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6621 "ds_parser.cpp"
    break;

  case 336:
#line 1860 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6627 "ds_parser.cpp"
    break;

  case 337:
#line 1861 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6633 "ds_parser.cpp"
    break;

  case 338:
#line 1862 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6639 "ds_parser.cpp"
    break;

  case 339:
#line 1863 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6645 "ds_parser.cpp"
    break;

  case 340:
#line 1864 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6651 "ds_parser.cpp"
    break;

  case 341:
#line 1865 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6657 "ds_parser.cpp"
    break;

  case 342:
#line 1866 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6663 "ds_parser.cpp"
    break;

  case 343:
#line 1867 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6669 "ds_parser.cpp"
    break;

  case 344:
#line 1868 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6675 "ds_parser.cpp"
    break;

  case 345:
#line 1869 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6681 "ds_parser.cpp"
    break;

  case 346:
#line 1870 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6687 "ds_parser.cpp"
    break;

  case 347:
#line 1871 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6693 "ds_parser.cpp"
    break;

  case 348:
#line 1872 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6699 "ds_parser.cpp"
    break;

  case 349:
#line 1873 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6705 "ds_parser.cpp"
    break;

  case 350:
#line 1874 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6711 "ds_parser.cpp"
    break;

  case 351:
#line 1875 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6717 "ds_parser.cpp"
    break;

  case 352:
#line 1876 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6723 "ds_parser.cpp"
    break;

  case 353:
#line 1877 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6729 "ds_parser.cpp"
    break;

  case 354:
#line 1878 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6735 "ds_parser.cpp"
    break;

  case 355:
#line 1879 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6741 "ds_parser.cpp"
    break;

  case 356:
#line 1880 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6747 "ds_parser.cpp"
    break;

  case 357:
#line 1881 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6753 "ds_parser.cpp"
    break;

  case 358:
#line 1882 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6759 "ds_parser.cpp"
    break;

  case 359:
#line 1886 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6765 "ds_parser.cpp"
    break;

  case 360:
#line 1887 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6771 "ds_parser.cpp"
    break;

  case 361:
#line 1888 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6777 "ds_parser.cpp"
    break;

  case 362:
#line 1889 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6783 "ds_parser.cpp"
    break;

  case 363:
#line 1890 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6789 "ds_parser.cpp"
    break;

  case 364:
#line 1891 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6795 "ds_parser.cpp"
    break;

  case 365:
#line 1895 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6808 "ds_parser.cpp"
    break;

  case 366:
#line 1906 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6817 "ds_parser.cpp"
    break;

  case 367:
#line 1910 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6829 "ds_parser.cpp"
    break;

  case 368:
#line 1920 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6835 "ds_parser.cpp"
    break;

  case 369:
#line 1921 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6841 "ds_parser.cpp"
    break;

  case 370:
#line 1922 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6847 "ds_parser.cpp"
    break;

  case 371:
#line 1923 "ds_parser.ypp"
    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don`t get extra infer pass on every array
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
#line 6866 "ds_parser.cpp"
    break;

  case 372:
#line 1937 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6877 "ds_parser.cpp"
    break;

  case 373:
#line 1943 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6886 "ds_parser.cpp"
    break;

  case 374:
#line 1947 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6896 "ds_parser.cpp"
    break;

  case 375:
#line 1952 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6906 "ds_parser.cpp"
    break;

  case 376:
#line 1957 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6916 "ds_parser.cpp"
    break;

  case 377:
#line 1962 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6928 "ds_parser.cpp"
    break;

  case 378:
#line 1969 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6938 "ds_parser.cpp"
    break;

  case 379:
#line 1974 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6947 "ds_parser.cpp"
    break;

  case 380:
#line 1978 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6956 "ds_parser.cpp"
    break;

  case 381:
#line 1982 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6966 "ds_parser.cpp"
    break;

  case 382:
#line 1987 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6976 "ds_parser.cpp"
    break;

  case 383:
#line 1992 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6987 "ds_parser.cpp"
    break;

  case 384:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6999 "ds_parser.cpp"
    break;

  case 385:
#line 2005 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7009 "ds_parser.cpp"
    break;

  case 386:
#line 2010 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7020 "ds_parser.cpp"
    break;

  case 387:
#line 2016 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7030 "ds_parser.cpp"
    break;

  case 388:
#line 2021 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7039 "ds_parser.cpp"
    break;

  case 389:
#line 2025 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7049 "ds_parser.cpp"
    break;

  case 390:
#line 2030 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7063 "ds_parser.cpp"
    break;

  case 391:
#line 2039 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7072 "ds_parser.cpp"
    break;

  case 392:
#line 2043 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7082 "ds_parser.cpp"
    break;

  case 393:
#line 2048 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7096 "ds_parser.cpp"
    break;

  case 394:
#line 2057 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7105 "ds_parser.cpp"
    break;

  case 395:
#line 2061 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7115 "ds_parser.cpp"
    break;

  case 396:
#line 2066 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7129 "ds_parser.cpp"
    break;

  case 397:
#line 2075 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7140 "ds_parser.cpp"
    break;

  case 398:
#line 2081 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7151 "ds_parser.cpp"
    break;

  case 399:
#line 2090 "ds_parser.ypp"
    {
        das_need_oxford_comma=false;
    }
#line 7159 "ds_parser.cpp"
    break;

  case 400:
#line 2096 "ds_parser.ypp"
    {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-4].s);
        vtype->at = tokAt((yylsp[-4]));
        varDeclToTypeDecl(vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !g_Program->addAlias(vtype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt((yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
    }
#line 7176 "ds_parser.cpp"
    break;

  case 401:
#line 2111 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7182 "ds_parser.cpp"
    break;

  case 402:
#line 2112 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7188 "ds_parser.cpp"
    break;

  case 403:
#line 2113 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7194 "ds_parser.cpp"
    break;

  case 404:
#line 2114 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7200 "ds_parser.cpp"
    break;

  case 405:
#line 2118 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7212 "ds_parser.cpp"
    break;

  case 406:
#line 2125 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7223 "ds_parser.cpp"
    break;

  case 407:
#line 2134 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7233 "ds_parser.cpp"
    break;

  case 408:
#line 2139 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7242 "ds_parser.cpp"
    break;

  case 409:
#line 2146 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7252 "ds_parser.cpp"
    break;

  case 410:
#line 2151 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7263 "ds_parser.cpp"
    break;

  case 411:
#line 2157 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7274 "ds_parser.cpp"
    break;

  case 412:
#line 2163 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7286 "ds_parser.cpp"
    break;

  case 413:
#line 2170 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7299 "ds_parser.cpp"
    break;

  case 414:
#line 2181 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7307 "ds_parser.cpp"
    break;

  case 415:
#line 2184 "ds_parser.ypp"
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
#line 7323 "ds_parser.cpp"
    break;

  case 416:
#line 2198 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7334 "ds_parser.cpp"
    break;

  case 417:
#line 2207 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7340 "ds_parser.cpp"
    break;

  case 418:
#line 2208 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7346 "ds_parser.cpp"
    break;

  case 419:
#line 2212 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7356 "ds_parser.cpp"
    break;

  case 420:
#line 2217 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7365 "ds_parser.cpp"
    break;

  case 421:
#line 2224 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7375 "ds_parser.cpp"
    break;

  case 422:
#line 2229 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7388 "ds_parser.cpp"
    break;

  case 423:
#line 2240 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7398 "ds_parser.cpp"
    break;

  case 424:
#line 2245 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7407 "ds_parser.cpp"
    break;

  case 425:
#line 2252 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7421 "ds_parser.cpp"
    break;

  case 426:
#line 2264 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7427 "ds_parser.cpp"
    break;

  case 427:
#line 2265 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7433 "ds_parser.cpp"
    break;

  case 428:
#line 2269 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 7452 "ds_parser.cpp"
    break;

  case 429:
#line 2283 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
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
#line 7471 "ds_parser.cpp"
    break;


#line 7475 "ds_parser.cpp"

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
#line 2299 "ds_parser.ypp"


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,"","",at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
    g_Program->error(error,"","",LineInfo(g_FileAccessStack.back(),
        yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line),cerr);
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
        argList.push_back(arg);
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
    return LineInfo(g_FileAccessStack.back(),
        li.first_column,li.first_line,yylloc.last_column,yylloc.last_line);
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

void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames ) {
    bool anyNames = false;
    for ( auto pDecl : *list ) {
        if ( pDecl->pTypeDecl ) {
            int count = pDecl->pNameList ? int(pDecl->pNameList->size()) : 1;
            for ( int ai=0; ai!=count; ++ai ) {
                auto pVarType = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    das_yyerror("can't have default values in type declaration",
                    pDecl->at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
                if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].empty() ) {
                    anyNames = true;
                }
            }
        }
    }
    if ( anyNames ) {
        for ( auto pDecl : *list ) {
            if ( pDecl->pTypeDecl ) {
                if ( pDecl->pNameList ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        pType->argNames.push_back(name);
                    }
                } else {
                    pType->argNames.push_back(string());
                }
            }
        }
    }
}


