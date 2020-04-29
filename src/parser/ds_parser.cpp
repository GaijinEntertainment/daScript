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
#line 54 "ds_parser.ypp"

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
    extern bool das_force_oxford_comma;

#line 195 "ds_parser.cpp"

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
#line 80 "ds_parser.ypp"

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

#line 377 "ds_parser.cpp"

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
#define YYLAST   6726

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  112
/* YYNRULES -- Number of rules.  */
#define YYNRULES  431
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  790

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
       0,   389,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   404,   416,   417,   421,   425,   430,
     439,   447,   463,   485,   489,   492,   498,   507,   528,   551,
     552,   556,   560,   561,   565,   568,   574,   580,   583,   589,
     590,   594,   595,   596,   605,   606,   610,   619,   631,   641,
     650,   651,   652,   653,   654,   655,   659,   664,   672,   673,
     674,   675,   676,   677,   678,   679,   685,   698,   714,   715,
     719,   726,   738,   742,   749,   750,   754,   755,   756,   760,
     765,   772,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   815,   888,
     891,   901,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   922,
     927,   933,   939,   950,   956,   957,   968,   972,   978,   981,
     984,   988,   994,   998,  1002,  1005,  1008,  1013,  1016,  1024,
    1027,  1032,  1035,  1043,  1049,  1050,  1054,  1055,  1059,  1093,
    1096,  1101,  1109,  1113,  1118,  1124,  1128,  1133,  1142,  1145,
    1151,  1152,  1159,  1170,  1171,  1175,  1180,  1235,  1236,  1237,
    1238,  1239,  1240,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1263,
    1264,  1268,  1279,  1284,  1294,  1298,  1305,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,
    1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1355,  1356,  1357,  1358,  1362,  1366,  1369,
    1372,  1373,  1374,  1375,  1380,  1385,  1386,  1389,  1392,  1396,
    1400,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1420,  1430,
    1464,  1465,  1469,  1470,  1474,  1482,  1485,  1492,  1497,  1505,
    1506,  1510,  1513,  1522,  1523,  1527,  1536,  1537,  1542,  1543,
    1547,  1552,  1555,  1559,  1565,  1574,  1575,  1576,  1580,  1581,
    1585,  1588,  1593,  1598,  1606,  1617,  1620,  1627,  1628,  1632,
    1659,  1659,  1667,  1670,  1679,  1692,  1704,  1705,  1709,  1713,
    1724,  1739,  1740,  1744,  1783,  1849,  1856,  1865,  1866,  1867,
    1868,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,
    1878,  1879,  1880,  1881,  1882,  1883,  1884,  1885,  1886,  1887,
    1888,  1892,  1893,  1894,  1895,  1896,  1897,  1901,  1912,  1916,
    1926,  1927,  1928,  1929,  1943,  1949,  1953,  1958,  1963,  1968,
    1975,  1980,  1984,  1988,  1993,  1998,  2004,  2011,  2016,  2022,
    2027,  2031,  2036,  2045,  2049,  2054,  2063,  2067,  2072,  2081,
    2087,  2096,  2102,  2117,  2118,  2119,  2120,  2124,  2131,  2140,
    2145,  2152,  2157,  2163,  2169,  2176,  2187,  2190,  2204,  2213,
    2214,  2218,  2223,  2230,  2235,  2246,  2251,  2258,  2270,  2271,
    2275,  2289
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
  "optional_shared", "global_let", "$@1", "enum_list", "single_alias",
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

#define YYPACT_NINF -492

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-492)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -492,    25,  -492,  -492,   -64,   -41,   -27,   -15,   -44,  -492,
      67,  -492,   -13,  -492,  -492,  -492,  -492,  -492,    56,  -492,
     156,  -492,  -492,  -492,  -492,    99,  -492,   109,  -492,   115,
     124,   131,  -492,  -492,    -9,  -492,   -75,   151,  -492,  -492,
     133,   170,   126,  -492,   -35,  -492,   177,    -7,  -492,   143,
     144,    26,   -64,   195,   -41,   196,  -492,   198,   199,  -492,
     209,  -492,   194,  -492,  -104,   204,  -492,   -64,   -13,  -492,
     200,   167,  6571,   306,   307,  -492,   178,  -492,   342,   212,
    -492,  -492,  -492,  -492,  -492,   213,   110,  -492,  -492,  -492,
    -492,   294,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,
     179,   -97,  5622,  -492,  -492,  -492,   -36,  -492,   -69,  -492,
    -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,  -492,  -492,  -492,   116,   207,   -96,
     181,   210,  -492,   185,  -492,    45,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,    39,  -492,  -492,   -92,  -492,   214,   215,
     216,   217,  -492,  -492,  -492,   193,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,
     218,  -492,  -492,  -492,   219,   220,  -492,  -492,  -492,  -492,
     221,   222,  -492,  -492,  -492,  -492,   658,  -492,  -492,   -58,
     -64,  -492,   -14,  -492,  -121,  5622,   188,  -492,  -492,  -492,
     162,  -492,  5622,   -91,  -492,  -492,   110,  -492,   -90,  4061,
    -492,  5622,  5622,  5622,  5622,   238,  5546,  5546,  5546,  5698,
     212,  -492,    30,  -492,  -492,  -492,  -492,  2078,  -492,  -492,
    -492,   329,   203,    35,   243,  -492,   -31,  -492,   223,  3400,
    -492,  -492,   246,  5622,  -492,   129,  3400,  -492,  -492,  -492,
    -492,  -492,  -492,  5470,   226,  -492,   227,   234,   236,   235,
     255,   271,  4061,  4061,   785,  4188,  4061,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,  4061,  4061,  4061,  4061,  4061,  4061,
    -492,   251,  -492,  -492,   -63,  -492,  -492,  -492,   257,  -492,
    -492,  -492,  -492,  -492,  4816,   256,  -492,  -492,  -492,  -492,
    -492,    61,  1624,   374,  1252,   263,   207,  1396,   207,  2203,
     207,  2289,    27,  -492,    83,  3400,    91,   239,  -492,   259,
    -492,  -492,  5693,  -492,   243,  -492,  -492,  -492,  -492,  -492,
    5622,  2206,  -492,   935,  -492,   571,  -492,  -492,  -492,  2206,
    2468,  -492,   300,  4061,  5622,  5622,  4061,  5622,  5622,   454,
     454,   243,  1253,   243,  1397,  6082,  -492,    22,   122,   454,
     454,   -78,   454,   454,  4953,   -80,  -492,  1678,   178,    28,
     301,  4061,  4061,  -492,  -492,  4061,  4061,  4061,  4061,   302,
    4061,   303,  4061,  4061,  4061,  4061,  4061,  2343,  4061,  4061,
    4061,  4061,  4061,  4061,  4061,  4061,  4061,  4061,   316,  4061,
    -492,  2471,  -492,  5622,  -492,  -492,  -492,   293,  -492,   308,
    -492,   309,  -492,  5622,  -492,  5698,  -492,   212,  -492,  -492,
    -492,  -492,   145,   153,  -492,   338,  5914,  -492,  4061,  -492,
    -492,   243,  1541,  -492,   188,  4061,  4061,  -492,   284,   337,
    2608,  1822,  -492,   433,   317,   318,  4061,  -492,  -492,  -492,
    -492,  -492,  -492,   319,   320,   321,  -492,  -492,  -492,   451,
    -492,   -82,  -492,  6017,  -492,  2206,  -492,  4276,  2736,  1950,
    5021,  2554,  2555,  5089,  2733,  2819,     4,   -85,   312,  2873,
    6082,   334,   141,   339,  -492,  -492,   142,    19,  3010,   117,
     155,  4061,  4061,   305,  -492,  4061,   351,  -492,  5546,  -492,
     359,  -492,    44,  6283,   207,  5622,  -492,  -492,   349,   349,
     512,   512,  6537,  6537,   328,    53,  -492,  5769,   -62,   -62,
     349,   349,   361,  6150,  6348,  6416,   296,   512,   512,   276,
     276,    53,    53,    53,  -492,  5838,  -492,    51,  2820,  -492,
    -492,  -492,  3400,  -492,  4061,  -492,  -492,   153,  4061,  4061,
    4061,  4061,  4061,  4061,  4061,  4061,  4061,  4061,  4061,  4061,
    4061,  4061,  4061,  4181,    40,  2206,  -492,  -492,  4371,   482,
    6283,  4181,  -492,   354,   375,  6283,  2206,  -492,  4466,   332,
    -492,  -492,  4181,  -492,  -492,  -492,   465,   243,  -492,  3147,
    3275,  -492,  4561,  -492,  -492,    76,  5622,   372,  5157,  -492,
    4061,  4061,  -492,  4061,   341,  4061,  4061,  -492,   359,   376,
     344,   359,  4061,   347,  4061,  4061,   359,   353,   356,  6283,
    -492,  -492,  6215,   207,  2957,   -30,    48,  4061,  -492,   -84,
    3400,  3403,  -492,  -492,  4061,  -492,  -492,  -492,  6283,  -492,
    6283,  6283,  6283,  6283,  6283,  6283,  6283,  6283,  6283,  6283,
    6283,  6283,  6283,  6283,  6283,  -492,  4061,  -492,  4656,  -492,
     188,  -492,  -492,  -492,  -492,  4751,  -492,  -492,   277,  -492,
    -492,  4061,  4061,  -492,  -492,  3094,   101,  -492,   512,   512,
     512,  3540,   -87,  6283,   149,   -30,  -492,   334,  6283,  -492,
    -492,   -49,   158,  -492,  -492,  -492,   366,   -69,   352,  6283,
    3677,  -492,  -492,  -492,    79,  6283,   -47,  -492,  -492,  -492,
    1106,   188,  -492,  -492,  4061,  -492,  6481,  6481,  -492,  3805,
     389,  -492,  5225,  4061,   360,  4061,  4061,   362,   -69,  -492,
    -492,  4061,  6283,  -492,   188,  -492,  -492,  4181,  5622,  5293,
     378,  -492,  4888,  -492,  6283,  4888,  -492,  -492,  6283,  -492,
     277,  3486,  -492,  3933,   478,   364,   358,  -492,  -492,  5622,
    5361,  4061,   367,   370,  3487,  -492,  6283,  -492,  -492,  -492
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    74,     1,   166,     0,     0,     0,     0,     0,   167,
       0,   401,     0,    13,     3,    10,     9,     8,     0,     7,
     317,     6,    11,     5,     4,     0,    12,    64,    66,    22,
      34,    31,    32,    24,    29,    23,     0,     0,    14,    69,
     144,     0,    70,    72,     0,    68,     0,     0,   318,   320,
       0,     0,     0,     0,     0,     0,    30,     0,     0,    27,
       0,   322,     0,   326,     0,     0,   146,     0,     0,    75,
     331,     0,     0,     0,     0,    81,    76,   315,     0,     0,
      62,    63,    60,    61,    59,     0,     0,    58,    67,    35,
      33,    29,    26,    25,   361,   364,   362,   365,   363,   366,
       0,     0,     0,   328,   327,   145,     0,    73,     0,   333,
     285,    84,    85,    87,    86,    88,    89,    90,    91,   110,
     111,   108,   109,   101,   112,   113,   102,    99,   100,   114,
     115,   116,   117,   104,   105,   103,    97,    98,    93,    92,
      94,    95,    96,    83,    82,   106,   107,   280,    79,     0,
       0,     0,   296,     0,    15,     0,    54,    55,    52,    53,
      51,    50,    56,     0,    28,   322,     0,   329,     0,     0,
       0,     0,   337,   357,   338,   368,   339,   343,   344,   345,
     346,   350,   351,   352,   353,   354,   355,   356,   358,   359,
     390,   342,   349,   360,   393,   396,   340,   347,   341,   348,
       0,     0,   367,   370,   372,   371,     0,    71,   332,   280,
       0,    77,     0,   289,     0,     0,     0,   335,   319,   316,
     308,   321,     0,     0,    16,    17,     0,    65,     0,     0,
     323,     0,     0,     0,     0,     0,    76,    76,    76,     0,
       0,   376,     0,   382,   386,   384,   378,     0,   325,   381,
     334,   282,     0,     0,     0,   287,   300,    78,   280,    80,
     139,   118,     0,     0,   309,     0,   295,   402,   297,    57,
     330,   222,   223,     0,     0,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   187,   189,   188,
     190,   191,   192,    18,     0,     0,     0,     0,     0,     0,
     183,   184,   220,   185,   218,   273,   272,   271,    74,   276,
     219,   275,   274,   255,     0,     0,   221,   403,   404,   405,
     406,     0,     0,     0,     0,     0,    79,     0,    79,     0,
      79,     0,   144,   293,     0,   291,     0,   377,   380,     0,
     383,   374,     0,   283,     0,   286,   281,   288,   299,   298,
       0,     0,   290,     0,   336,     0,   306,   307,   305,     0,
     148,   151,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,     0,     0,     0,     0,   425,     0,     0,   227,
     226,   260,   225,   224,     0,     0,   279,     0,    76,     0,
       0,     0,     0,   248,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,   387,     0,   389,   385,   369,     0,   391,     0,
     394,     0,   397,     0,   399,     0,   400,     0,   379,   375,
     373,   284,   301,     0,   304,     0,   303,   141,     0,    44,
      45,     0,     0,   152,     0,     0,     0,   153,     0,     0,
       0,     0,   121,   119,     0,     0,     0,   134,   129,   127,
     128,   140,   122,     0,     0,     0,   132,   133,   135,   164,
     126,     0,   123,   193,   310,     0,   314,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,     0,     0,
     416,   409,     0,   419,   420,   421,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,    19,   250,    76,   214,
       0,   256,     0,   178,    79,     0,   268,   269,   228,   229,
     241,   242,   239,   240,     0,   265,   254,     0,   277,   278,
     230,   231,     0,     0,   244,   245,   243,   237,   238,   233,
     232,   234,   235,   236,   253,     0,   258,     0,     0,   392,
     395,   398,   292,   294,     0,   184,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   154,   157,   193,     0,
     147,     0,   138,     0,     0,    38,     0,   161,   193,     0,
     136,   137,     0,   125,   130,   131,     0,     0,   124,     0,
       0,   312,   193,   313,   149,     0,     0,     0,     0,   261,
       0,     0,   262,     0,     0,     0,     0,   412,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   418,
     426,   427,     0,    79,     0,     0,     0,     0,   257,     0,
     267,     0,   252,   270,     0,   251,   259,   388,   302,   142,
     200,   201,   203,   202,   204,   197,   198,   199,   205,   206,
     195,   196,   207,   208,   194,    48,     0,   158,   193,   155,
       0,    49,    36,    37,   162,   193,   159,   139,    41,   165,
     168,     0,     0,   311,   150,     0,     0,   172,   169,   170,
     171,     0,     0,   407,     0,     0,   411,   410,   417,   423,
     422,     0,     0,   414,   424,    20,     0,     0,     0,   179,
       0,   180,   186,   212,     0,   266,     0,   156,   163,   160,
       0,     0,    39,    40,     0,    46,   210,   209,   175,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,   215,
     211,     0,   181,   213,     0,   120,    42,     0,     0,     0,
       0,   264,   428,   413,   408,   428,   415,   216,   182,    47,
      41,     0,   173,     0,     0,     0,     0,    43,   176,     0,
       0,     0,     0,     0,     0,   174,   429,   430,   431,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -492,  -492,  -492,   160,   483,  -492,  -492,  -492,  -492,  -492,
    -492,   448,  -492,  -492,   486,  -492,  -492,  -492,  -229,  -492,
    -492,  -492,  -492,  -492,   323,  -492,   490,   -40,  -492,   476,
    -492,   244,  -221,  -318,  -492,  -492,  -209,  -492,  -136,  -346,
     -12,  -492,  -492,  -492,  -492,  -492,  -492,  -492,  -492,   552,
    -492,  -492,  -492,  -417,  -492,  -492,  -420,  -492,  -350,  -492,
    -492,  -492,  -492,   184,   348,  -492,  -492,  -492,   310,  -492,
     125,  -492,  -205,   324,  -254,  -233,   206,  -492,   -48,  -492,
    -492,  -492,  -492,   398,   505,  -492,  -492,  -492,  -492,  -492,
    -492,  -143,  -101,  -492,  -492,  -492,   -88,  -492,  -492,   298,
    -491,  -354,  -492,  -492,  -277,   -61,   211,  -492,  -492,  -492,
    -193,  -492
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    14,   155,   161,   378,   302,    15,    16,    34,
      35,    59,    17,    31,    32,   464,   465,   734,   735,   466,
     467,   468,   469,   470,   162,   163,    28,    29,    42,    43,
      44,    18,   148,   216,    76,    19,   303,   471,   353,   472,
     304,   473,   305,   474,   475,   476,   477,   478,   607,   479,
     480,   306,   307,   522,   722,   308,   309,   310,   445,   482,
     311,   312,   313,   523,   212,   344,   252,   209,   213,   214,
     333,   334,   152,   153,   626,   255,   359,   265,   219,   149,
      49,    21,    78,   101,    63,    64,    22,    23,   109,    71,
      24,   256,   315,   100,   204,   205,   335,    25,    26,   316,
     501,   502,   317,   503,   504,   505,   506,   318,   377,   319,
     775,   320
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   203,   351,   481,   557,   444,   220,   261,   427,   376,
     429,    55,   431,   486,   206,   326,   328,   330,   268,    39,
     509,   347,   625,   566,   348,     2,    72,   106,    62,   646,
       3,   254,   567,   401,   402,   166,   217,   635,   720,    80,
      81,   151,   166,    56,   257,   525,   399,   229,   258,   401,
     402,   647,    40,     4,   349,     5,    45,     6,   676,     7,
      46,   337,    47,    40,     8,   518,    60,   103,    27,   386,
       9,   615,   389,   390,   167,   218,    10,   230,   348,   348,
     267,   270,   743,    65,   419,    30,   260,   608,    41,   647,
     202,   647,   241,    73,    74,    61,   208,   242,   418,    41,
     419,   387,    52,    68,   210,    33,   587,   262,   349,   349,
     350,   441,    11,   250,   203,   597,   243,    36,   217,    40,
     746,   203,   754,   156,   157,    75,    37,   259,    69,   207,
     203,   203,   203,   203,   266,   203,   203,   203,   203,   611,
     707,    57,   262,   321,   322,   323,   324,   659,   327,   329,
     331,    58,    82,   393,   394,    41,    83,   262,    84,    85,
     526,   399,   203,   400,   401,   402,   403,   524,   433,   404,
     253,   244,   203,    52,   338,   355,   241,   226,   262,   224,
     225,   242,   647,   203,   203,   360,   629,    12,   564,   647,
      86,   512,   339,   513,    13,    65,   372,   374,   346,    38,
     243,   245,   340,   202,   227,   246,   649,   422,   702,   648,
     202,   718,    48,   418,   647,   419,   656,   647,   711,   202,
     202,   202,   202,   247,   202,   202,   202,   202,   496,   434,
     507,    50,   268,   249,   724,   640,   158,   436,   356,   677,
     159,   694,   160,    85,   753,   589,   357,   739,    51,   203,
     684,   202,   435,    52,   348,   244,   154,   514,   515,   726,
     437,   202,   442,   203,   203,    53,   203,   203,   358,    54,
     740,    94,   202,   202,   704,    95,   491,   492,   210,   494,
     495,   211,   712,    62,   349,   245,   631,   731,   637,   246,
      67,    96,    97,    98,    99,   389,   390,   643,   732,   733,
     262,    65,    66,   263,   630,   633,   264,   247,   584,    70,
     631,   634,   744,    77,    79,   389,   390,   249,   631,   300,
     565,    89,   203,   260,   634,   716,   638,   631,    91,   747,
      92,    93,   203,   102,   203,   558,   105,   110,   202,   145,
     146,   108,   147,   150,   151,   562,    56,   154,   215,   165,
     221,   222,   202,   202,   223,   202,   202,   235,   260,   231,
     232,   233,   234,   236,   237,   238,   239,   240,   389,   390,
     325,   343,   345,   519,   675,   217,   393,   394,   354,   364,
     481,   365,   681,   438,   399,   210,   400,   401,   402,   403,
     362,   363,   404,   688,   391,   392,   393,   394,   395,   366,
     367,   396,   397,   398,   399,   241,   400,   401,   402,   403,
     242,   202,   404,   314,   405,   406,   368,   203,   385,    12,
     421,   202,   439,   202,   203,   415,   416,   417,   426,   243,
     644,   342,   489,   527,   534,   536,   418,   650,   419,   559,
     721,   411,   412,   413,   414,   415,   416,   417,   554,   393,
     394,   745,   567,   592,   560,   561,   418,   399,   419,   400,
     401,   402,   403,   593,   220,   404,   369,   370,   599,   606,
     375,   728,   629,   389,   390,   627,   641,   632,   379,   380,
     381,   382,   383,   384,   244,   224,   600,   601,   603,   604,
     605,   645,   651,   653,   680,   682,   413,   414,   415,   416,
     417,   683,   687,   689,   696,   701,   202,   706,   705,   418,
     709,   419,   748,   202,   245,   203,   713,   750,   246,   714,
     424,   760,   756,   763,   773,   766,   781,   782,   695,   783,
     787,   389,   390,   788,    87,   446,   247,   483,   516,   164,
      90,   777,    88,   487,   107,   769,   249,   490,   770,   269,
     493,   730,   388,    20,   393,   394,   500,   251,   500,   690,
     563,   485,   399,   228,   336,   401,   402,   403,   352,   104,
     404,   361,   776,   710,     0,   528,   529,     0,     0,   530,
     531,   532,   533,     0,   535,   510,   537,   538,   539,   540,
     541,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   241,   555,   202,     0,     0,   242,     0,     0,
     391,   392,   393,   394,   418,     0,   419,     0,     0,     0,
     399,     0,   400,   401,   402,   403,   243,     0,   404,     0,
     405,   406,   583,     0,     0,     0,   588,     0,     0,   590,
     591,     0,     0,     0,   595,   598,     0,     0,     0,     0,
     602,     0,     0,     0,     0,     0,     0,   203,     0,   413,
     414,   415,   416,   417,     0,     0,     0,     0,     0,   612,
     771,     0,   418,   618,   419,     0,     0,     0,   203,     0,
     356,   244,     0,   384,     0,     0,     0,     0,   357,   241,
       0,   784,   384,     0,   242,   639,   375,     0,     0,   642,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
     358,   245,     0,   243,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,   767,     0,     0,     0,
     484,     0,     0,   249,     0,     0,   202,     0,   658,     0,
       0,     0,   660,   661,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   202,   244,   678,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     685,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   670,   674,     0,   371,     0,   245,     0,
       0,     0,   246,     0,   698,   699,     0,   700,   168,     0,
     703,     0,     0,     0,   169,     0,   708,     0,   500,     0,
     247,     0,     0,     0,     0,     0,     0,   248,     0,   170,
     249,   719,     0,     0,     0,     0,     0,     0,   725,     0,
       0,     0,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,     0,     0,   736,   737,     0,     0,     0,
       0,     0,     0,     0,     0,   742,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   483,     0,     0,    40,   757,     0,
       0,     0,     0,   759,     0,     0,     0,   762,     0,   764,
     765,     0,     0,     0,     0,   768,   447,     0,     0,     0,
       3,     0,   448,   449,   450,     0,   451,     0,   271,   272,
     273,   274,     0,    41,     0,     0,     0,   780,     0,   452,
     275,   453,   454,     0,     0,   786,     0,     0,     0,     0,
       0,   455,   276,     0,     0,   456,   277,     0,   278,     0,
       9,   279,   457,     0,   458,   280,     0,     0,   459,   460,
       0,     0,     0,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,   191,   192,   193,     0,     0,   196,   197,   198,
     199,     0,     0,   281,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,    40,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,   295,   296,     0,     0,     0,     0,   297,
     298,     0,     0,     0,     0,     0,     0,     0,     0,   299,
       0,   300,   301,    41,   462,   260,   463,   447,     0,     0,
       0,     3,     0,   448,   449,   450,     0,   451,     0,   271,
     272,   273,   274,     0,     0,     0,     0,     0,     0,     0,
     452,   275,   453,   454,     0,     0,     0,     0,     0,     0,
       0,     0,   455,   276,     0,     0,   456,   277,     0,   278,
       0,     9,   279,   457,     0,   458,   280,     0,     0,   459,
     460,     0,     0,     0,   172,   173,   174,     0,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,   191,   192,   193,     0,     0,   196,   197,
     198,   199,     0,     0,   281,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    40,   293,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,   295,   296,     0,     0,     0,     0,
     297,   298,     0,     0,     0,     0,   271,   272,   273,   274,
     299,     0,   300,   301,    41,   462,   260,   755,   275,     0,
       0,     0,     0,   241,   241,     0,     0,     0,   242,   242,
     276,     0,     0,     0,   277,     0,   278,     0,     0,   279,
       0,     0,     0,   280,     0,     0,     0,   243,   243,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     191,   192,   193,     0,     0,   196,   197,   198,   199,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   282,   283,     0,     0,     0,     0,     0,
       0,     0,   244,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   497,   293,     0,     0,     0,
       0,     0,   245,   245,     0,     0,   246,   246,   425,     0,
     294,   295,   296,     0,     0,     0,     0,   297,   298,     0,
     271,   272,   273,   274,   247,   247,   498,   499,     0,   300,
     301,    41,   275,   260,   249,   249,     0,   241,   241,     0,
       0,     0,   242,   242,   276,     0,     0,     0,   277,     0,
     278,     0,     0,   279,     0,     0,     0,   280,     0,     0,
       0,   243,   243,     0,     0,   172,   173,   174,     0,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   191,   192,   193,     0,     0,   196,
     197,   198,   199,     0,     0,   281,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   283,     0,
       0,     0,     0,     0,     0,     0,   244,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   497,
     293,     0,     0,     0,     0,     0,   245,   245,     0,     0,
     246,   246,   428,     0,   294,   295,   296,     0,     0,     0,
       0,   297,   298,     0,   271,   272,   273,   274,   247,   247,
       0,   508,     0,   300,   301,    41,   275,   260,   249,   249,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,   277,     0,   278,     0,     0,   279,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,   191,   192,
     193,     0,     0,   196,   197,   198,   199,     0,     0,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   282,   283,     0,     0,     0,     0,     0,     0,     0,
     585,     0,     0,     0,     0,   241,   443,     0,     0,     0,
     242,     0,     0,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,    40,   293,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   271,   272,   273,   274,   297,   298,     0,     0,     0,
       0,     0,     0,   275,     0,   299,     0,   300,   301,    41,
     586,   260,     0,     0,     0,   276,     0,     0,     0,   277,
       0,   278,     0,     0,   279,     0,     0,     0,   280,     0,
       0,     0,     0,     0,   244,     0,   172,   173,   174,     0,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,   191,   192,   193,     0,     0,
     196,   197,   198,   199,   245,     0,   281,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   282,   283,
       0,     0,     0,     0,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   249,     0,     0,     0,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      40,   293,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   295,   296,     0,     0,
       0,     0,   297,   298,     0,   271,   272,   273,   274,     0,
     520,     0,   299,   521,   300,   301,    41,   275,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,   277,     0,   278,     0,     0,   279,     0,
       0,     0,   280,     0,     0,     0,     0,     0,     0,     0,
     172,   173,   174,     0,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,   191,
     192,   193,     0,     0,   196,   197,   198,   199,     0,     0,
     281,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   283,     0,     0,     0,     0,     0,     0,
       0,   596,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,    40,   293,     0,     0,     0,     0,
       0,     0,     0,   271,   272,   273,   274,   616,     0,   294,
     295,   296,     0,     0,     0,   275,   297,   298,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   276,   300,   301,
      41,   277,   260,   278,     0,     0,   279,     0,     0,     0,
     280,     0,     0,     0,     0,     0,     0,     0,   172,   173,
     174,     0,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   191,   192,   193,
       0,     0,   196,   197,   198,   199,     0,     0,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     282,   283,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,    40,   293,     0,     0,     0,     0,     0,     0,
       0,   271,   272,   273,   274,   617,     0,   294,   295,   296,
       0,     0,     0,   275,   297,   298,     0,     0,     0,     0,
       0,     0,     0,     0,   299,   276,   300,   301,    41,   277,
     260,   278,     0,     0,   279,     0,     0,     0,   280,     0,
       0,     0,     0,     0,     0,     0,   172,   173,   174,     0,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,   191,   192,   193,     0,     0,
     196,   197,   198,   199,     0,     0,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   282,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      40,   293,     0,     0,     0,     0,     0,     0,     0,   271,
     272,   273,   274,     0,     0,   294,   295,   296,     0,     0,
       0,   275,   297,   298,   241,     0,     0,     0,     0,   242,
       0,   341,   299,   276,   300,   301,    41,   277,   260,   278,
       0,     0,   279,     0,     0,     0,   280,     0,   243,     0,
       0,     0,     0,     0,   172,   173,   174,     0,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,   191,   192,   193,     0,     0,   196,   197,
     198,   199,     0,     0,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   282,   283,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
     241,   443,     0,     0,     0,   242,     0,     0,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    40,   293,
       0,     0,     0,   245,   243,     0,     0,   246,     0,   430,
       0,     0,     0,   294,   295,   296,   271,   272,   273,   274,
     297,   298,     0,   542,     0,   247,     0,     0,   275,     0,
     299,     0,   300,   301,    41,   249,   260,     0,     0,     0,
     276,     0,     0,     0,   277,     0,   278,     0,     0,   279,
       0,     0,     0,   280,     0,     0,     0,     0,     0,   244,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     191,   192,   193,     0,     0,   196,   197,   198,   199,   245,
       0,   281,     0,   246,     0,   432,     0,     0,     0,     0,
       0,     0,     0,   282,   283,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    40,   293,     0,     0,     0,
       0,     0,     0,     0,   271,   272,   273,   274,     0,     0,
     294,   295,   296,     0,     0,     0,   275,   297,   298,   241,
       0,     0,     0,     0,   242,     0,     0,   299,   276,   300,
     301,    41,   277,   260,   278,     0,     0,   279,     0,     0,
       0,   280,     0,   243,     0,     0,     0,     0,     0,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,   191,   192,
     193,     0,     0,   196,   197,   198,   199,     0,     0,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   282,   283,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,   241,   241,     0,     0,     0,
     242,   242,     0,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,    40,   293,     0,     0,     0,   245,   243,
     243,     0,   246,     0,     0,     0,     0,     0,   294,   295,
     296,   271,   272,   273,   274,   297,   298,     0,     0,     0,
     247,     0,   488,   275,     0,   299,   556,   300,   301,    41,
     249,   260,     0,     0,     0,   276,     0,     0,     0,   277,
       0,   278,     0,     0,   279,     0,     0,     0,   280,     0,
       0,   594,     0,     0,   244,   244,   172,   173,   174,     0,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,   191,   192,   193,     0,     0,
     196,   197,   198,   199,   245,   245,   281,     0,   246,   246,
     620,   621,     0,     0,     0,     0,     0,     0,   282,   283,
       0,     0,     0,     0,     0,     0,   247,   247,     0,     0,
       0,     0,     0,     0,     0,     0,   249,   249,     0,     0,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      40,   293,     0,     0,     0,     0,     0,     0,     0,   271,
     272,   273,   274,     0,     0,   294,   295,   296,     0,     0,
       0,   275,   297,   298,   241,     0,     0,     0,     0,   242,
       0,     0,   299,   276,   300,   301,    41,   277,   260,   278,
       0,     0,   279,     0,     0,     0,   280,     0,   243,     0,
       0,     0,     0,     0,   172,   173,   174,     0,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,   191,   192,   193,     0,     0,   196,   197,
     198,   199,     0,     0,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   282,   283,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
     241,   241,     0,     0,     0,   242,   242,     0,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    40,   293,
       0,     0,     0,   245,   243,   243,     0,   246,     0,   623,
       0,     0,     0,   294,   295,   296,   271,   272,   273,   274,
     297,   298,     0,     0,     0,   247,     0,     0,   275,     0,
     299,   614,   300,   301,    41,   249,   260,     0,     0,     0,
     276,     0,     0,     0,   277,     0,   278,     0,     0,   279,
       0,     0,     0,   280,     0,     0,     0,     0,     0,   244,
     244,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     191,   192,   193,     0,     0,   196,   197,   198,   199,   245,
     245,   281,     0,   246,   246,   624,   657,     0,     0,     0,
       0,     0,     0,   282,   283,     0,     0,     0,     0,     0,
       0,   247,   247,     0,     0,     0,     0,     0,   241,     0,
       0,   249,   249,   242,     0,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    40,   293,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,     0,     0,     0,
     294,   295,   296,   271,   272,   273,   274,   297,   298,     0,
       0,     0,     0,     0,     0,   275,     0,   299,   628,   300,
     301,    41,     0,   260,     0,     0,     0,   276,     0,     0,
       0,   277,     0,   278,     0,     0,   279,     0,     0,     0,
     280,     0,     0,     0,     0,     0,     0,   244,   172,   173,
     174,     0,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   191,   192,   193,
       0,     0,   196,   197,   198,   199,     0,   245,   281,     0,
       0,   246,     0,   717,     0,     0,     0,     0,     0,     0,
     282,   283,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,     0,     0,     0,   241,     0,     0,     0,   249,
     242,     0,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,    40,   293,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     271,   272,   273,   274,   297,   298,     0,     0,     0,     0,
       0,     0,   275,     0,   299,   636,   300,   301,    41,     0,
     260,     0,     0,     0,   276,     0,     0,     0,   277,     0,
     278,     0,     0,   279,     0,     0,     0,   280,     0,     0,
       0,     0,     0,     0,   244,   172,   173,   174,     0,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   191,   192,   193,     0,     0,   196,
     197,   198,   199,     0,   245,   281,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   283,     0,
       0,     0,     0,     0,     0,     0,   247,     0,     0,   738,
       0,     0,   691,     0,     0,     0,   249,     0,     0,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,    40,
     293,     0,     0,     0,     0,     0,     0,     0,   271,   272,
     273,   274,     0,     0,   294,   295,   296,     0,     0,     0,
     275,   297,   298,     0,     0,     0,     0,     0,     0,     0,
       0,   299,   276,   300,   301,    41,   277,   260,   278,     0,
       0,   279,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,   191,   192,   193,     0,     0,   196,   197,   198,
     199,     0,     0,   281,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     692,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,    40,   293,     0,
       0,     0,     0,     0,     0,     0,   271,   272,   273,   274,
       0,     0,   294,   295,   296,     0,     0,     0,   275,   297,
     298,   241,     0,     0,     0,     0,   242,     0,     0,   299,
     276,   300,   301,    41,   277,   260,   278,     0,     0,   279,
       0,     0,     0,   280,     0,   243,     0,     0,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     191,   192,   193,     0,     0,   196,   197,   198,   199,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   282,   283,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,   241,   241,     0,
       0,     0,   242,   242,     0,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    40,   293,     0,     0,     0,
     245,   243,   243,     0,   246,     0,     0,     0,     0,     0,
     294,   295,   296,   271,   272,   273,   274,   297,   298,     0,
       0,     0,   247,     0,     0,   275,     0,   299,   723,   300,
     301,    41,   249,   260,     0,     0,     0,   276,     0,     0,
       0,   277,     0,   278,     0,     0,   279,     0,     0,     0,
     280,     0,     0,     0,     0,     0,   244,   244,   172,   173,
     174,     0,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   191,   192,   193,
       0,     0,   196,   197,   198,   199,   245,   245,   281,     0,
     246,   246,     0,     0,     0,     0,     0,     0,     0,     0,
     282,   283,     0,     0,     0,     0,     0,     0,   247,   247,
       0,   778,   789,     0,     0,     0,     0,     0,   249,   249,
       0,     0,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,    40,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   296,
     271,   272,   273,   274,   297,   298,     0,     0,     0,     0,
       0,     0,   275,     0,   299,   741,   300,   301,    41,     0,
     260,     0,     0,     0,   276,     0,     0,     0,   277,     0,
     278,     0,     0,   279,     0,     0,     0,   280,     0,     0,
       0,     0,     0,     0,     0,   172,   173,   174,     0,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   191,   192,   193,     0,     0,   196,
     197,   198,   199,     0,     0,   281,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   283,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,    40,
     293,     0,     0,     0,     0,     0,     0,     0,   271,   272,
     273,   274,   758,     0,   294,   295,   296,     0,     0,     0,
     275,   297,   298,     0,     0,     0,     0,     0,     0,     0,
       0,   299,   276,   300,   301,    41,   277,   260,   278,     0,
       0,   279,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,   191,   192,   193,     0,     0,   196,   197,   198,
     199,     0,     0,   281,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,    40,   293,     0,
       0,     0,     0,     0,     0,     0,   271,   272,   273,   274,
     779,     0,   294,   295,   296,     0,     0,     0,   275,   297,
     298,     0,     0,     0,     0,     0,     0,     0,     0,   299,
     276,   300,   301,    41,   277,   260,   278,     0,     0,   279,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     191,   192,   193,     0,     0,   196,   197,   198,   199,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   282,   283,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    40,   293,     0,     0,     0,
       0,     0,     0,     0,   271,   272,   273,   274,     0,     0,
     294,   295,   296,     0,     0,     0,   275,   297,   298,     0,
       0,     0,     0,     0,     0,     0,     0,   299,   276,   300,
     301,    41,   277,   260,   278,     0,     0,   279,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,     0,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,   191,   192,
     193,     0,     0,   196,   197,   198,   199,     0,     0,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   282,   283,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,    40,   293,     0,     0,     0,     0,   373,
     389,   390,     0,     0,     0,     0,     0,     0,   294,   295,
     296,   168,     0,     0,     0,   297,   298,   169,     0,     0,
       0,     0,     0,     0,     0,   299,     0,   300,   301,    41,
       0,   260,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,     0,     0,     0,   391,
     392,   393,   394,   395,     0,     0,   396,   397,   398,   399,
       0,   400,   401,   402,   403,   389,   390,   404,     0,   405,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,     0,     0,     0,     0,
       0,   418,     0,   419,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   568,   569,   570,   571,
     572,   573,   574,   575,   391,   392,   393,   394,   395,   576,
     577,   396,   397,   398,   399,   578,   400,   401,   402,   403,
     389,   390,   404,   579,   405,   406,   580,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   582,   407,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,   419,     0,
       0,     0,     0,     0,     0,   613,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   568,   569,   570,   571,   572,   573,   574,   575,   391,
     392,   393,   394,   395,   576,   577,   396,   397,   398,   399,
     578,   400,   401,   402,   403,   389,   390,   404,   579,   405,
     406,   580,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     582,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,     0,     0,     0,     0,
       0,   418,     0,   419,     0,     0,     0,     0,     0,     0,
     679,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   568,   569,   570,   571,
     572,   573,   574,   575,   391,   392,   393,   394,   395,   576,
     577,   396,   397,   398,   399,   578,   400,   401,   402,   403,
     389,   390,   404,   579,   405,   406,   580,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   582,   407,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,   419,     0,
       0,     0,     0,     0,     0,   686,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   568,   569,   570,   571,   572,   573,   574,   575,   391,
     392,   393,   394,   395,   576,   577,   396,   397,   398,   399,
     578,   400,   401,   402,   403,   389,   390,   404,   579,   405,
     406,   580,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     582,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,     0,     0,     0,     0,
       0,   418,     0,   419,     0,     0,     0,     0,     0,     0,
     693,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   568,   569,   570,   571,
     572,   573,   574,   575,   391,   392,   393,   394,   395,   576,
     577,   396,   397,   398,   399,   578,   400,   401,   402,   403,
     389,   390,   404,   579,   405,   406,   580,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   582,   407,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,   419,     0,
       0,     0,     0,     0,     0,   727,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,   390,     0,     0,     0,
       0,   568,   569,   570,   571,   572,   573,   574,   575,   391,
     392,   393,   394,   395,   576,   577,   396,   397,   398,   399,
     578,   400,   401,   402,   403,     0,     0,   404,   579,   405,
     406,   580,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     582,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,     0,   389,   390,     0,
       0,   418,     0,   419,   391,   392,   393,   394,   395,     0,
     729,   396,   397,   398,   399,     0,   400,   401,   402,   403,
       0,     0,   404,     0,   405,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   407,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,     0,     0,
       0,     0,   389,   390,     0,     0,   418,     0,   419,     0,
       0,     0,     0,     0,     0,   420,   391,   392,   393,   394,
     395,     0,     0,   396,   397,   398,   399,     0,   400,   401,
     402,   403,     0,     0,   404,     0,   405,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     389,   390,     0,     0,     0,     0,     0,     0,   418,     0,
     419,   391,   392,   393,   394,   395,     0,   774,   396,   397,
     398,   399,     0,   400,   401,   402,   403,     0,     0,   404,
       0,   405,   406,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,     0,     0,   389,   390,
       0,     0,     0,   418,     0,   419,     0,     0,   517,   391,
     392,   393,   394,   395,     0,     0,   396,   397,   398,   399,
       0,   400,   401,   402,   403,     0,     0,   404,     0,   405,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,   389,   390,     0,     0,
       0,   418,     0,   419,     0,     0,   619,   391,   392,   393,
     394,   395,     0,     0,   396,   397,   398,   399,     0,   400,
     401,   402,   403,     0,     0,   404,     0,   405,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   407,
       0,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,     0,     0,     0,   389,   390,     0,     0,     0,   418,
       0,   419,     0,     0,   622,   391,   392,   393,   394,   395,
       0,     0,   396,   397,   398,   399,     0,   400,   401,   402,
     403,     0,     0,   404,     0,   405,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,     0,
       0,     0,   389,   390,     0,     0,     0,   418,     0,   419,
       0,     0,   697,   391,   392,   393,   394,   395,     0,     0,
     396,   397,   398,   399,     0,   400,   401,   402,   403,     0,
       0,   404,     0,   405,   406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   407,     0,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,     0,     0,     0,
     389,   390,     0,     0,     0,   418,     0,   419,     0,     0,
     761,   391,   392,   393,   394,   395,     0,     0,   396,   397,
     398,   399,     0,   400,   401,   402,   403,     0,     0,   404,
       0,   405,   406,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,     0,     0,     0,     0,
       0,     0,     0,   418,     0,   419,     0,     0,   772,   391,
     392,   393,   394,   395,     0,     0,   396,   397,   398,   399,
       0,   400,   401,   402,   403,     0,     0,   404,     0,   405,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,     0,     0,     0,   169,
       0,   407,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,   170,     0,     0,     0,     0,     0,
       0,   418,     0,   419,     0,     0,   785,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   168,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,     0,     0,   284,   285,   286,     0,     0,     0,     0,
       0,     0,    40,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     147,     0,   389,   390,    41,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,     0,     0,   389,   390,
      41,   391,   392,   393,   394,   395,     0,     0,   396,   397,
     398,   399,     0,   400,   401,   402,   403,     0,     0,   404,
       0,   405,   406,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,     0,   407,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,     0,     0,     0,     0,
       0,     0,     0,   418,     0,   419,   440,   389,   390,     0,
       0,     0,     0,     0,     0,     0,    41,   391,   392,   393,
     394,   395,     0,     0,   396,   397,   398,   399,     0,   400,
     401,   402,   403,     0,     0,   404,     0,   405,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   407,
       0,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,     0,     0,     0,     0,     0,     0,     0,     0,   418,
       0,   419,   652,   389,   390,     0,   391,   392,   393,   394,
     395,     0,     0,   396,   397,   398,   399,     0,   400,   401,
     402,   403,     0,     0,   404,     0,   405,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
       0,     0,     0,     0,     0,     0,     0,     0,   418,     0,
     419,   655,     0,     0,   568,   569,   570,   571,   572,   573,
     574,   575,   391,   392,   393,   394,   395,   576,   577,   396,
     397,   398,   399,   578,   400,   401,   402,   403,  -193,     0,
     404,   579,   405,   406,   580,   581,   389,   390,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   582,   407,     0,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,     0,     0,     0,     0,
       0,     0,     0,     0,   418,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,   390,     0,     0,     0,     0,   568,   569,   570,
     571,   572,   573,   574,   575,   391,   392,   393,   394,   395,
     576,   577,   396,   397,   398,   399,   609,   400,   401,   402,
     403,     0,     0,   404,   579,   405,   406,   580,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   610,   407,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   389,
     390,     0,     0,     0,     0,     0,     0,   418,     0,   419,
     391,   392,   393,   394,   395,     0,     0,   396,   397,   398,
     399,     0,   400,   401,   402,   403,     0,     0,   404,     0,
     405,   406,     0,     0,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   389,   390,     0,     0,     0,     0,
       0,     0,   418,     0,   419,     0,     0,     0,   391,   392,
     393,   394,   395,     0,     0,   396,   397,   398,   399,     0,
     400,   401,   402,   403,     0,     0,   404,     0,   405,   406,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,   654,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   389,   390,     0,     0,     0,     0,     0,     0,
     418,     0,   419,   391,   392,   393,   394,   395,     0,     0,
     396,   397,   398,   399,     0,   400,   401,   402,   403,     0,
       0,   404,     0,   405,   406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   715,     0,     0,   407,     0,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   389,   390,     0,
       0,     0,     0,     0,     0,   418,     0,   419,     0,     0,
       0,   391,   392,   393,   394,   395,     0,     0,   396,   397,
     398,   399,     0,   400,   401,   402,   403,     0,     0,   404,
       0,   405,   406,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   389,   390,     0,     0,     0,
       0,     0,     0,   418,     0,   419,   391,   392,   393,   394,
     395,     0,     0,   396,   397,   398,   399,     0,   400,   401,
     402,   403,     0,     0,   404,     0,   405,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     389,   390,     0,     0,     0,     0,     0,     0,   418,     0,
     419,     0,     0,     0,   391,   392,   393,   394,   395,     0,
       0,   396,   397,   398,   399,     0,   400,   401,   402,   403,
       0,     0,   404,     0,   405,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,   390,     0,     0,
     410,   411,   412,   413,   414,   415,   416,   417,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,   419,   391,
     392,     0,     0,   395,     0,     0,   396,   397,   398,   399,
       0,   400,   401,   402,   403,     0,     0,   404,     0,   405,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,   408,   409,   410,   411,   412,     0,     0,
       0,   416,   417,     0,     0,   391,   392,   393,   394,   395,
       0,   418,   396,   419,     0,   399,     0,   400,   401,   402,
     403,     0,     0,   404,     0,   405,   406,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,     0,
       0,     0,   411,   412,   413,   414,   415,   416,   417,   129,
     130,   131,   132,     0,     0,     0,     0,   418,     0,   419,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,     0,     0,   143,   144
};

static const yytype_int16 yycheck[] =
{
      12,   102,   256,   353,   421,   351,   149,   216,   326,   286,
     328,    20,   330,   359,   102,   236,   237,   238,   223,    32,
     374,   254,    18,   443,   109,     0,    33,    67,   132,   520,
       5,    45,   114,   111,   112,   132,   132,    18,   122,    13,
      14,   132,   132,    52,   165,    17,   108,   139,   169,   111,
     112,   138,   132,    28,   139,    30,    68,    32,    18,    34,
       4,    31,     6,   132,    39,   145,   141,   171,   132,   132,
      45,   488,    19,    20,   171,   171,    51,   169,   109,   109,
     171,   171,   169,   168,   162,   126,   170,   169,   168,   138,
     102,   138,    31,   100,   101,   170,   108,    36,   160,   168,
     162,   164,   138,   138,   162,   132,   452,   138,   139,   139,
     141,   344,    87,   171,   215,   461,    55,   132,   132,   132,
     169,   222,   169,    13,    14,   132,   170,   215,   163,   165,
     231,   232,   233,   234,   222,   236,   237,   238,   239,   485,
     631,   150,   138,   231,   232,   233,   234,   567,   236,   237,
     238,   160,   126,   100,   101,   168,   130,   138,   132,   133,
     132,   108,   263,   110,   111,   112,   113,   388,   141,   116,
     210,   110,   273,   138,   144,   263,    31,   138,   138,   134,
     135,    36,   138,   284,   285,   273,   138,   162,   442,   138,
     164,   169,   162,   171,   169,   168,   284,   285,   163,   132,
      55,   140,   172,   215,   165,   144,   524,   146,   625,   165,
     222,   163,    56,   160,   138,   162,   165,   138,   635,   231,
     232,   233,   234,   162,   236,   237,   238,   239,   371,   146,
     373,   132,   437,   172,   651,   512,   126,   146,   109,   585,
     130,   165,   132,   133,   165,   454,   117,   146,   139,   350,
     596,   263,   169,   138,   109,   110,   134,   135,   136,   676,
     169,   273,   350,   364,   365,   141,   367,   368,   139,   138,
     169,    62,   284,   285,   628,    66,   364,   365,   162,   367,
     368,   165,   636,   132,   139,   140,   169,    10,   171,   144,
     164,    82,    83,    84,    85,    19,    20,   518,    21,    22,
     138,   168,   132,   141,   163,   163,   144,   162,   451,   132,
     169,   169,   163,   170,   170,    19,    20,   172,   169,   166,
     167,   126,   423,   170,   169,   643,   171,   169,   132,   171,
     132,   132,   433,   139,   435,   423,   132,   170,   350,    33,
      33,   141,   164,     1,   132,   433,    52,   134,   141,   170,
     169,   141,   364,   365,   169,   367,   368,   164,   170,   145,
     145,   145,   145,   145,   145,   145,   145,   145,    19,    20,
     132,    42,   169,   385,   583,   132,   100,   101,   132,   145,
     730,   145,   591,   144,   108,   162,   110,   111,   112,   113,
     164,   164,   116,   602,    98,    99,   100,   101,   102,   164,
     145,   105,   106,   107,   108,    31,   110,   111,   112,   113,
      36,   423,   116,   229,   118,   119,   145,   518,   167,   162,
     164,   433,   163,   435,   525,   149,   150,   151,   165,    55,
     518,   247,   132,   132,   132,   132,   160,   525,   162,   146,
     649,   145,   146,   147,   148,   149,   150,   151,   132,   100,
     101,   705,   114,   169,   146,   146,   160,   108,   162,   110,
     111,   112,   113,   126,   607,   116,   282,   283,    35,    18,
     286,   680,   138,    19,    20,   163,   171,   138,   294,   295,
     296,   297,   298,   299,   110,   134,   169,   169,   169,   169,
     169,   132,   164,   132,    12,   141,   147,   148,   149,   150,
     151,   126,   170,    38,   132,   164,   518,   163,   132,   160,
     163,   162,   146,   525,   140,   616,   163,   165,   144,   163,
     146,   132,   731,   163,   146,   163,    48,   163,   616,   171,
     163,    19,    20,   163,    51,   351,   162,   353,   378,    91,
      54,   770,    52,   359,    68,   754,   172,   363,   757,   226,
     366,   687,   308,     1,   100,   101,   372,   209,   374,   607,
     435,   355,   108,   165,   240,   111,   112,   113,   258,    64,
     116,   273,   765,   634,    -1,   391,   392,    -1,    -1,   395,
     396,   397,   398,    -1,   400,   374,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,    31,   419,   616,    -1,    -1,    36,    -1,    -1,
      98,    99,   100,   101,   160,    -1,   162,    -1,    -1,    -1,
     108,    -1,   110,   111,   112,   113,    55,    -1,   116,    -1,
     118,   119,   448,    -1,    -1,    -1,   452,    -1,    -1,   455,
     456,    -1,    -1,    -1,   460,   461,    -1,    -1,    -1,    -1,
     466,    -1,    -1,    -1,    -1,    -1,    -1,   758,    -1,   147,
     148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,   485,
     758,    -1,   160,   489,   162,    -1,    -1,    -1,   779,    -1,
     109,   110,    -1,   499,    -1,    -1,    -1,    -1,   117,    31,
      -1,   779,   508,    -1,    36,   511,   512,    -1,    -1,   515,
      -1,    -1,    -1,    -1,    -1,   717,    -1,    -1,    -1,    -1,
     139,   140,    -1,    55,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,   748,    -1,    -1,    -1,
     169,    -1,    -1,   172,    -1,    -1,   758,    -1,   564,    -1,
      -1,    -1,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   779,   110,   585,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     596,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   609,   610,    -1,    11,    -1,   140,    -1,
      -1,    -1,   144,    -1,   620,   621,    -1,   623,    23,    -1,
     626,    -1,    -1,    -1,    29,    -1,   632,    -1,   634,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    44,
     172,   647,    -1,    -1,    -1,    -1,    -1,    -1,   654,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    -1,   691,   692,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   701,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   720,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   730,    -1,    -1,   132,   734,    -1,
      -1,    -1,    -1,   739,    -1,    -1,    -1,   743,    -1,   745,
     746,    -1,    -1,    -1,    -1,   751,     1,    -1,    -1,    -1,
       5,    -1,     7,     8,     9,    -1,    11,    -1,    13,    14,
      15,    16,    -1,   168,    -1,    -1,    -1,   773,    -1,    24,
      25,    26,    27,    -1,    -1,   781,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    47,    -1,    49,    50,    -1,    -1,    53,    54,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,    -1,    -1,    -1,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,   166,   167,   168,   169,   170,   171,     1,    -1,    -1,
      -1,     5,    -1,     7,     8,     9,    -1,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    47,    -1,    49,    50,    -1,    -1,    53,
      54,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    83,
      84,    85,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,    -1,    -1,    -1,    -1,
     154,   155,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
     164,    -1,   166,   167,   168,   169,   170,   171,    25,    -1,
      -1,    -1,    -1,    31,    31,    -1,    -1,    -1,    36,    36,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    55,    55,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,   140,   140,    -1,    -1,   144,   144,   146,    -1,
     147,   148,   149,    -1,    -1,    -1,    -1,   154,   155,    -1,
      13,    14,    15,    16,   162,   162,   163,   164,    -1,   166,
     167,   168,    25,   170,   172,   172,    -1,    31,    31,    -1,
      -1,    -1,    36,    36,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    55,    55,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,   140,   140,    -1,    -1,
     144,   144,   146,    -1,   147,   148,   149,    -1,    -1,    -1,
      -1,   154,   155,    -1,    13,    14,    15,    16,   162,   162,
      -1,   164,    -1,   166,   167,   168,    25,   170,   172,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    83,    84,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    31,   115,    -1,    -1,    -1,
      36,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,    13,    14,    15,    16,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,   164,    -1,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,   140,    -1,    88,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,   172,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,    -1,
      -1,    -1,   154,   155,    -1,    13,    14,    15,    16,    -1,
     162,    -1,   164,   165,   166,   167,   168,    25,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    -1,    77,
      78,    79,    -1,    -1,    82,    83,    84,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    17,    -1,   147,
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
      -1,    13,    14,    15,    16,   145,    -1,   147,   148,   149,
      -1,    -1,    -1,    25,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    37,   166,   167,   168,    41,
     170,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,   147,   148,   149,    -1,    -1,
      -1,    25,   154,   155,    31,    -1,    -1,    -1,    -1,    36,
      -1,   163,   164,    37,   166,   167,   168,    41,   170,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      31,   115,    -1,    -1,    -1,    36,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
      -1,    -1,    -1,   140,    55,    -1,    -1,   144,    -1,   146,
      -1,    -1,    -1,   147,   148,   149,    13,    14,    15,    16,
     154,   155,    -1,    20,    -1,   162,    -1,    -1,    25,    -1,
     164,    -1,   166,   167,   168,   172,   170,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,   140,
      -1,    88,    -1,   144,    -1,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
     147,   148,   149,    -1,    -1,    -1,    25,   154,   155,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   164,    37,   166,
     167,   168,    41,   170,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    55,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    83,    84,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    31,    -1,    -1,    -1,
      36,    36,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,    -1,   140,    55,
      55,    -1,   144,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,    13,    14,    15,    16,   154,   155,    -1,    -1,    -1,
     162,    -1,   164,    25,    -1,   164,   165,   166,   167,   168,
     172,   170,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,   110,   110,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,   140,   140,    88,    -1,   144,   144,
     146,   146,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,   172,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,   147,   148,   149,    -1,    -1,
      -1,    25,   154,   155,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,   164,    37,   166,   167,   168,    41,   170,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    31,    -1,    -1,    -1,    36,    36,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
      -1,    -1,    -1,   140,    55,    55,    -1,   144,    -1,   146,
      -1,    -1,    -1,   147,   148,   149,    13,    14,    15,    16,
     154,   155,    -1,    -1,    -1,   162,    -1,    -1,    25,    -1,
     164,   165,   166,   167,   168,   172,   170,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   110,
     110,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    -1,    -1,    82,    83,    84,    85,   140,
     140,    88,    -1,   144,   144,   146,   146,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   162,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   172,   172,    36,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    13,    14,    15,    16,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,   164,   165,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,   110,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    -1,    82,    83,    84,    85,    -1,   140,    88,    -1,
      -1,   144,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   172,
      36,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
      13,    14,    15,    16,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   164,   165,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,   140,    88,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,   165,
      -1,    -1,   115,    -1,    -1,    -1,   172,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,   147,   148,   149,    -1,    -1,    -1,
      25,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    37,   166,   167,   168,    41,   170,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    -1,    77,    78,    79,    -1,    -1,    82,    83,    84,
      85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
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
      -1,    -1,   162,    -1,    -1,    25,    -1,   164,   165,   166,
     167,   168,   172,   170,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   110,   110,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      -1,    -1,    82,    83,    84,    85,   140,   140,    88,    -1,
     144,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,   162,   162,
      -1,   165,   165,    -1,    -1,    -1,    -1,    -1,   172,   172,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
      13,    14,    15,    16,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   164,   165,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    -1,    -1,    82,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
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
      17,    -1,   147,   148,   149,    -1,    -1,    -1,    25,   154,
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
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
     147,   148,   149,    -1,    -1,    -1,    25,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    37,   166,
     167,   168,    41,   170,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    83,    84,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,    11,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,    23,    -1,    -1,    -1,   154,   155,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,   166,   167,   168,
      -1,   170,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,    -1,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,    19,    20,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      19,    20,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    19,    20,   116,   117,   118,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      19,    20,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    19,    20,   116,   117,   118,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      19,    20,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    19,    20,    -1,
      -1,   160,    -1,   162,    98,    99,   100,   101,   102,    -1,
     169,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    98,    99,   100,   101,   102,    -1,   169,   105,   106,
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
      19,    20,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,
     165,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    -1,    -1,   165,    98,
      99,   100,   101,   102,    -1,    -1,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,    -1,    -1,   165,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
     164,    -1,    19,    20,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    19,    20,
     168,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,   163,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    98,    99,   100,
     101,   102,    -1,    -1,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   162,   163,    19,    20,    -1,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,   163,    -1,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,   119,   120,   121,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,
      98,    99,   100,   101,   102,    -1,    -1,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   162,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,    -1,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   162,    98,    99,   100,   101,   102,    -1,    -1,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
      -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,    -1,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,    -1,    -1,   116,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,    98,    99,   100,   101,
     102,    -1,    -1,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,    -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,
      -1,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,    98,
      99,    -1,    -1,   102,    -1,    -1,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,    -1,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,   150,   151,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   160,   105,   162,    -1,   108,    -1,   110,   111,   112,
     113,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    -1,
      -1,    -1,   145,   146,   147,   148,   149,   150,   151,   118,
     119,   120,   121,    -1,    -1,    -1,    -1,   160,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,   154,   155
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   174,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    87,   162,   169,   175,   180,   181,   185,   204,   208,
     222,   254,   259,   260,   263,   270,   271,   132,   199,   200,
     126,   186,   187,   132,   182,   183,   132,   170,   132,    32,
     132,   168,   201,   202,   203,   213,     4,     6,    56,   253,
     132,   139,   138,   141,   138,    20,    52,   150,   160,   184,
     141,   170,   132,   257,   258,   168,   132,   164,   138,   163,
     132,   262,    33,   100,   101,   132,   207,   170,   255,   170,
      13,    14,   126,   130,   132,   133,   164,   177,   199,   126,
     187,   132,   132,   132,    62,    66,    82,    83,    84,    85,
     266,   256,   139,   171,   257,   132,   200,   202,   141,   261,
     170,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   118,
     119,   120,   121,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   154,   155,    33,    33,   164,   205,   252,
       1,   132,   245,   246,   134,   176,    13,    14,   126,   130,
     132,   177,   197,   198,   184,   170,   132,   171,    23,    29,
      44,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   213,   265,   267,   268,   269,   165,   213,   240,
     162,   165,   237,   241,   242,   141,   206,   132,   171,   251,
     264,   169,   141,   169,   134,   135,   138,   165,   256,   139,
     169,   145,   145,   145,   145,   164,   145,   145,   145,   145,
     145,    31,    36,    55,   110,   140,   144,   162,   169,   172,
     171,   237,   239,   200,    45,   248,   264,   165,   169,   269,
     170,   209,   138,   141,   144,   250,   269,   171,   245,   197,
     171,    13,    14,    15,    16,    25,    37,    41,    43,    46,
      50,    88,   100,   101,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   133,   147,   148,   149,   154,   155,   164,
     166,   167,   179,   209,   213,   215,   224,   225,   228,   229,
     230,   233,   234,   235,   236,   265,   272,   275,   280,   282,
     284,   269,   269,   269,   269,   132,   205,   269,   205,   269,
     205,   269,   132,   243,   244,   269,   246,    31,   144,   162,
     172,   163,   236,    42,   238,   169,   163,   248,   109,   139,
     141,   247,   241,   211,   132,   269,   109,   117,   139,   249,
     269,   272,   164,   164,   145,   145,   164,   145,   145,   236,
     236,    11,   269,    11,   269,   236,   277,   281,   178,   236,
     236,   236,   236,   236,   236,   167,   132,   164,   204,    19,
      20,    98,    99,   100,   101,   102,   105,   106,   107,   108,
     110,   111,   112,   113,   116,   118,   119,   140,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   160,   162,
     169,   164,   146,   169,   146,   146,   165,   206,   146,   206,
     146,   206,   146,   141,   146,   169,   146,   169,   144,   163,
     163,   248,   269,   115,   212,   231,   236,     1,     7,     8,
       9,    11,    24,    26,    27,    36,    40,    47,    49,    53,
      54,    89,   169,   171,   188,   189,   192,   193,   194,   195,
     196,   210,   212,   214,   216,   217,   218,   219,   220,   222,
     223,   231,   232,   236,   169,   249,   212,   236,   164,   132,
     236,   269,   269,   236,   269,   269,   264,   132,   163,   164,
     236,   273,   274,   276,   277,   278,   279,   264,   164,   274,
     279,   122,   169,   171,   135,   136,   176,   165,   145,   213,
     162,   165,   226,   236,   205,    17,   132,   132,   236,   236,
     236,   236,   236,   236,   132,   236,   132,   236,   236,   236,
     236,   236,    20,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   132,   236,   165,   226,   269,   146,
     146,   146,   269,   243,   247,   167,   229,   114,    90,    91,
      92,    93,    94,    95,    96,    97,   103,   104,   109,   117,
     120,   121,   139,   236,   264,   109,   169,   212,   236,   209,
     236,   236,   169,   126,    53,   236,   109,   212,   236,    35,
     169,   169,   236,   169,   169,   169,    18,   221,   169,   109,
     139,   212,   236,   169,   165,   226,    17,   145,   236,   165,
     146,   146,   165,   146,   146,    18,   247,   163,   165,   138,
     163,   169,   138,   163,   169,    18,   165,   171,   171,   236,
     277,   171,   236,   205,   269,   132,   273,   138,   165,   206,
     269,   164,   163,   132,   141,   163,   165,   146,   236,   229,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   209,    18,   212,   236,   169,
      12,   209,   141,   126,   212,   236,   169,   170,   209,    38,
     251,   115,   115,   169,   165,   269,   132,   165,   236,   236,
     236,   164,   226,   236,   274,   132,   163,   273,   236,   163,
     278,   226,   274,   163,   163,   137,   206,   146,   163,   236,
     122,   209,   227,   165,   226,   236,   226,   169,   209,   169,
     211,    10,    21,    22,   190,   191,   236,   236,   165,   146,
     169,   165,   236,   169,   163,   247,   169,   171,   146,   213,
     165,   109,   236,   165,   169,   171,   209,   236,    17,   236,
     132,   165,   236,   163,   236,   236,   163,   213,   236,   209,
     209,   269,   165,   146,   169,   283,   283,   191,   165,    17,
     236,    48,   163,   171,   269,   165,   236,   163,   163,   165
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
     255,   254,   256,   256,   256,   257,   258,   258,   259,   260,
     260,   261,   261,   262,   263,   264,   264,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   266,   266,   266,   266,   266,   266,   267,   268,   268,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   270,   271,   272,   272,   272,   272,   273,   273,   274,
     274,   275,   275,   275,   275,   275,   276,   276,   277,   278,
     278,   279,   279,   280,   280,   281,   281,   282,   283,   283,
     284,   284
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
       0,     5,     0,     3,     5,     4,     1,     2,     4,     5,
       7,     0,     2,     2,     6,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     3,     4,     2,     3,     2,     4,
       3,     2,     2,     3,     2,     4,     2,     4,     6,     4,
       1,     4,     5,     1,     4,     5,     1,     4,     5,     4,
       4,     1,     6,     1,     1,     1,     1,     3,     5,     1,
       3,     5,     4,     7,     5,     7,     1,     3,     3,     1,
       1,     1,     3,     5,     5,     1,     3,     4,     0,     3,
      10,    10
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
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3114 "ds_parser.cpp"
        break;

    case 176: /* character_sequence  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3120 "ds_parser.cpp"
        break;

    case 177: /* string_constant  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3126 "ds_parser.cpp"
        break;

    case 178: /* string_builder_body  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp"
        break;

    case 179: /* string_builder  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3138 "ds_parser.cpp"
        break;

    case 182: /* require_module_name  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3144 "ds_parser.cpp"
        break;

    case 188: /* expression_label  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3150 "ds_parser.cpp"
        break;

    case 189: /* expression_goto  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3156 "ds_parser.cpp"
        break;

    case 191: /* expression_else  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3162 "ds_parser.cpp"
        break;

    case 193: /* expression_if_then_else  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3168 "ds_parser.cpp"
        break;

    case 194: /* expression_for_loop  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3174 "ds_parser.cpp"
        break;

    case 195: /* expression_while_loop  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3180 "ds_parser.cpp"
        break;

    case 196: /* expression_with  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3186 "ds_parser.cpp"
        break;

    case 197: /* annotation_argument_value  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3192 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_value_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3198 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3204 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_list  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3210 "ds_parser.cpp"
        break;

    case 201: /* annotation_declaration_name  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3216 "ds_parser.cpp"
        break;

    case 202: /* annotation_declaration  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3222 "ds_parser.cpp"
        break;

    case 203: /* annotation_list  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3228 "ds_parser.cpp"
        break;

    case 204: /* optional_annotation_list  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3234 "ds_parser.cpp"
        break;

    case 205: /* optional_function_argument_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3240 "ds_parser.cpp"
        break;

    case 206: /* optional_function_type  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3246 "ds_parser.cpp"
        break;

    case 207: /* function_name  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3252 "ds_parser.cpp"
        break;

    case 209: /* expression_block  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp"
        break;

    case 210: /* expression_any  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp"
        break;

    case 211: /* expressions  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp"
        break;

    case 212: /* expr_pipe  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3276 "ds_parser.cpp"
        break;

    case 213: /* name_in_namespace  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3282 "ds_parser.cpp"
        break;

    case 214: /* expression_delete  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp"
        break;

    case 215: /* expr_new  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp"
        break;

    case 216: /* expression_break  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp"
        break;

    case 217: /* expression_continue  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp"
        break;

    case 218: /* expression_return  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp"
        break;

    case 219: /* expression_yield  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp"
        break;

    case 220: /* expression_try_catch  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp"
        break;

    case 223: /* expression_let  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp"
        break;

    case 224: /* expr_cast  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp"
        break;

    case 225: /* expr_type_info  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp"
        break;

    case 226: /* expr_list  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp"
        break;

    case 227: /* block_or_simple_block  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp"
        break;

    case 229: /* expr_block  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3360 "ds_parser.cpp"
        break;

    case 230: /* expr_numeric_const  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3366 "ds_parser.cpp"
        break;

    case 231: /* expr_assign  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3372 "ds_parser.cpp"
        break;

    case 232: /* expr_assign_pipe  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3378 "ds_parser.cpp"
        break;

    case 233: /* expr_named_call  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3384 "ds_parser.cpp"
        break;

    case 234: /* expr_method_call  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3390 "ds_parser.cpp"
        break;

    case 235: /* func_addr_expr  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3396 "ds_parser.cpp"
        break;

    case 236: /* expr  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3402 "ds_parser.cpp"
        break;

    case 237: /* optional_field_annotation  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3408 "ds_parser.cpp"
        break;

    case 239: /* structure_variable_declaration  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3414 "ds_parser.cpp"
        break;

    case 240: /* struct_variable_declaration_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3420 "ds_parser.cpp"
        break;

    case 241: /* function_argument_declaration  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3426 "ds_parser.cpp"
        break;

    case 242: /* function_argument_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3432 "ds_parser.cpp"
        break;

    case 243: /* tuple_type  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3438 "ds_parser.cpp"
        break;

    case 244: /* tuple_type_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3444 "ds_parser.cpp"
        break;

    case 245: /* variant_type  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3450 "ds_parser.cpp"
        break;

    case 246: /* variant_type_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3456 "ds_parser.cpp"
        break;

    case 248: /* variable_declaration  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3462 "ds_parser.cpp"
        break;

    case 251: /* let_variable_declaration  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3468 "ds_parser.cpp"
        break;

    case 252: /* global_variable_declaration_list  */
#line 108 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3474 "ds_parser.cpp"
        break;

    case 256: /* enum_list  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3480 "ds_parser.cpp"
        break;

    case 261: /* optional_structure_parent  */
#line 105 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3486 "ds_parser.cpp"
        break;

    case 264: /* variable_name_list  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3492 "ds_parser.cpp"
        break;

    case 267: /* structure_type_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3498 "ds_parser.cpp"
        break;

    case 268: /* auto_type_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3504 "ds_parser.cpp"
        break;

    case 269: /* type_declaration  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3510 "ds_parser.cpp"
        break;

    case 272: /* make_decl  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp"
        break;

    case 273: /* make_struct_fields  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3522 "ds_parser.cpp"
        break;

    case 274: /* make_struct_dim  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp"
        break;

    case 275: /* make_struct_decl  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp"
        break;

    case 276: /* make_tuple  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp"
        break;

    case 277: /* make_map_tuple  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3546 "ds_parser.cpp"
        break;

    case 278: /* make_any_tuple  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3552 "ds_parser.cpp"
        break;

    case 279: /* make_dim  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3558 "ds_parser.cpp"
        break;

    case 280: /* make_dim_decl  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3564 "ds_parser.cpp"
        break;

    case 281: /* make_table  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3570 "ds_parser.cpp"
        break;

    case 282: /* make_table_decl  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3576 "ds_parser.cpp"
        break;

    case 283: /* array_comprehension_where  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3582 "ds_parser.cpp"
        break;

    case 284: /* array_comprehension  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3588 "ds_parser.cpp"
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
#line 404 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3888 "ds_parser.cpp"
    break;

  case 15:
#line 416 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3894 "ds_parser.cpp"
    break;

  case 16:
#line 417 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3900 "ds_parser.cpp"
    break;

  case 17:
#line 421 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3906 "ds_parser.cpp"
    break;

  case 18:
#line 425 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 3916 "ds_parser.cpp"
    break;

  case 19:
#line 430 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3930 "ds_parser.cpp"
    break;

  case 20:
#line 439 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3940 "ds_parser.cpp"
    break;

  case 21:
#line 447 "ds_parser.ypp"
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
#line 3958 "ds_parser.cpp"
    break;

  case 22:
#line 463 "ds_parser.ypp"
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
#line 3982 "ds_parser.cpp"
    break;

  case 24:
#line 489 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3990 "ds_parser.cpp"
    break;

  case 25:
#line 492 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4001 "ds_parser.cpp"
    break;

  case 26:
#line 498 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4012 "ds_parser.cpp"
    break;

  case 27:
#line 507 "ds_parser.ypp"
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
#line 4038 "ds_parser.cpp"
    break;

  case 28:
#line 528 "ds_parser.ypp"
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
#line 4063 "ds_parser.cpp"
    break;

  case 29:
#line 551 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4069 "ds_parser.cpp"
    break;

  case 30:
#line 552 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4075 "ds_parser.cpp"
    break;

  case 34:
#line 565 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4083 "ds_parser.cpp"
    break;

  case 35:
#line 568 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4091 "ds_parser.cpp"
    break;

  case 36:
#line 574 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4099 "ds_parser.cpp"
    break;

  case 37:
#line 580 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4107 "ds_parser.cpp"
    break;

  case 38:
#line 583 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4115 "ds_parser.cpp"
    break;

  case 39:
#line 589 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4121 "ds_parser.cpp"
    break;

  case 40:
#line 590 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4127 "ds_parser.cpp"
    break;

  case 41:
#line 594 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4133 "ds_parser.cpp"
    break;

  case 42:
#line 595 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4139 "ds_parser.cpp"
    break;

  case 43:
#line 596 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4150 "ds_parser.cpp"
    break;

  case 44:
#line 605 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4156 "ds_parser.cpp"
    break;

  case 45:
#line 606 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4162 "ds_parser.cpp"
    break;

  case 46:
#line 610 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4173 "ds_parser.cpp"
    break;

  case 47:
#line 619 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4187 "ds_parser.cpp"
    break;

  case 48:
#line 631 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4199 "ds_parser.cpp"
    break;

  case 49:
#line 641 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4210 "ds_parser.cpp"
    break;

  case 50:
#line 650 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4216 "ds_parser.cpp"
    break;

  case 51:
#line 651 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4222 "ds_parser.cpp"
    break;

  case 52:
#line 652 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4228 "ds_parser.cpp"
    break;

  case 53:
#line 653 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4234 "ds_parser.cpp"
    break;

  case 54:
#line 654 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4240 "ds_parser.cpp"
    break;

  case 55:
#line 655 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4246 "ds_parser.cpp"
    break;

  case 56:
#line 659 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4256 "ds_parser.cpp"
    break;

  case 57:
#line 664 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4266 "ds_parser.cpp"
    break;

  case 58:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4272 "ds_parser.cpp"
    break;

  case 59:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4278 "ds_parser.cpp"
    break;

  case 60:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4284 "ds_parser.cpp"
    break;

  case 61:
#line 675 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4290 "ds_parser.cpp"
    break;

  case 62:
#line 676 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4296 "ds_parser.cpp"
    break;

  case 63:
#line 677 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4302 "ds_parser.cpp"
    break;

  case 64:
#line 678 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4308 "ds_parser.cpp"
    break;

  case 65:
#line 679 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4316 "ds_parser.cpp"
    break;

  case 66:
#line 685 "ds_parser.ypp"
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
#line 4334 "ds_parser.cpp"
    break;

  case 67:
#line 698 "ds_parser.ypp"
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
#line 4352 "ds_parser.cpp"
    break;

  case 68:
#line 714 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4358 "ds_parser.cpp"
    break;

  case 69:
#line 715 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4364 "ds_parser.cpp"
    break;

  case 70:
#line 719 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4376 "ds_parser.cpp"
    break;

  case 71:
#line 726 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4390 "ds_parser.cpp"
    break;

  case 72:
#line 738 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4399 "ds_parser.cpp"
    break;

  case 73:
#line 742 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4408 "ds_parser.cpp"
    break;

  case 74:
#line 749 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4414 "ds_parser.cpp"
    break;

  case 75:
#line 750 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4420 "ds_parser.cpp"
    break;

  case 76:
#line 754 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4426 "ds_parser.cpp"
    break;

  case 77:
#line 755 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4432 "ds_parser.cpp"
    break;

  case 78:
#line 756 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4438 "ds_parser.cpp"
    break;

  case 79:
#line 760 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4448 "ds_parser.cpp"
    break;

  case 80:
#line 765 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4457 "ds_parser.cpp"
    break;

  case 81:
#line 772 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4466 "ds_parser.cpp"
    break;

  case 82:
#line 776 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4472 "ds_parser.cpp"
    break;

  case 83:
#line 777 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4478 "ds_parser.cpp"
    break;

  case 84:
#line 778 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4484 "ds_parser.cpp"
    break;

  case 85:
#line 779 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4490 "ds_parser.cpp"
    break;

  case 86:
#line 780 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4496 "ds_parser.cpp"
    break;

  case 87:
#line 781 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4502 "ds_parser.cpp"
    break;

  case 88:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4508 "ds_parser.cpp"
    break;

  case 89:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4514 "ds_parser.cpp"
    break;

  case 90:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4520 "ds_parser.cpp"
    break;

  case 91:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4526 "ds_parser.cpp"
    break;

  case 92:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4532 "ds_parser.cpp"
    break;

  case 93:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4538 "ds_parser.cpp"
    break;

  case 94:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4544 "ds_parser.cpp"
    break;

  case 95:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4550 "ds_parser.cpp"
    break;

  case 96:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4556 "ds_parser.cpp"
    break;

  case 97:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4562 "ds_parser.cpp"
    break;

  case 98:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4568 "ds_parser.cpp"
    break;

  case 99:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4574 "ds_parser.cpp"
    break;

  case 100:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4580 "ds_parser.cpp"
    break;

  case 101:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4586 "ds_parser.cpp"
    break;

  case 102:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4592 "ds_parser.cpp"
    break;

  case 103:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4598 "ds_parser.cpp"
    break;

  case 104:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4604 "ds_parser.cpp"
    break;

  case 105:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4610 "ds_parser.cpp"
    break;

  case 106:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4616 "ds_parser.cpp"
    break;

  case 107:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4622 "ds_parser.cpp"
    break;

  case 108:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4628 "ds_parser.cpp"
    break;

  case 109:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4634 "ds_parser.cpp"
    break;

  case 110:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4640 "ds_parser.cpp"
    break;

  case 111:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4646 "ds_parser.cpp"
    break;

  case 112:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4652 "ds_parser.cpp"
    break;

  case 113:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4658 "ds_parser.cpp"
    break;

  case 114:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4664 "ds_parser.cpp"
    break;

  case 115:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4670 "ds_parser.cpp"
    break;

  case 116:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4676 "ds_parser.cpp"
    break;

  case 117:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4682 "ds_parser.cpp"
    break;

  case 118:
#line 816 "ds_parser.ypp"
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
#line 4756 "ds_parser.cpp"
    break;

  case 119:
#line 888 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4764 "ds_parser.cpp"
    break;

  case 120:
#line 891 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4776 "ds_parser.cpp"
    break;

  case 121:
#line 901 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4782 "ds_parser.cpp"
    break;

  case 122:
#line 902 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4788 "ds_parser.cpp"
    break;

  case 123:
#line 903 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4794 "ds_parser.cpp"
    break;

  case 124:
#line 904 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4800 "ds_parser.cpp"
    break;

  case 125:
#line 905 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4806 "ds_parser.cpp"
    break;

  case 126:
#line 906 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4812 "ds_parser.cpp"
    break;

  case 127:
#line 907 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4818 "ds_parser.cpp"
    break;

  case 128:
#line 908 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4824 "ds_parser.cpp"
    break;

  case 129:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4830 "ds_parser.cpp"
    break;

  case 130:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4836 "ds_parser.cpp"
    break;

  case 131:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4842 "ds_parser.cpp"
    break;

  case 132:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4848 "ds_parser.cpp"
    break;

  case 133:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4854 "ds_parser.cpp"
    break;

  case 134:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4860 "ds_parser.cpp"
    break;

  case 135:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4866 "ds_parser.cpp"
    break;

  case 136:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4872 "ds_parser.cpp"
    break;

  case 137:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4878 "ds_parser.cpp"
    break;

  case 138:
#line 918 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4884 "ds_parser.cpp"
    break;

  case 139:
#line 922 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4894 "ds_parser.cpp"
    break;

  case 140:
#line 927 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4905 "ds_parser.cpp"
    break;

  case 141:
#line 933 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4913 "ds_parser.cpp"
    break;

  case 142:
#line 939 "ds_parser.ypp"
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
#line 4929 "ds_parser.cpp"
    break;

  case 143:
#line 950 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4937 "ds_parser.cpp"
    break;

  case 144:
#line 956 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4943 "ds_parser.cpp"
    break;

  case 145:
#line 957 "ds_parser.ypp"
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
#line 4959 "ds_parser.cpp"
    break;

  case 146:
#line 968 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4965 "ds_parser.cpp"
    break;

  case 147:
#line 972 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4973 "ds_parser.cpp"
    break;

  case 148:
#line 978 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4981 "ds_parser.cpp"
    break;

  case 149:
#line 981 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4989 "ds_parser.cpp"
    break;

  case 150:
#line 984 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4998 "ds_parser.cpp"
    break;

  case 151:
#line 988 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5006 "ds_parser.cpp"
    break;

  case 152:
#line 994 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5012 "ds_parser.cpp"
    break;

  case 153:
#line 998 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5018 "ds_parser.cpp"
    break;

  case 154:
#line 1002 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5026 "ds_parser.cpp"
    break;

  case 155:
#line 1005 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5034 "ds_parser.cpp"
    break;

  case 156:
#line 1008 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5044 "ds_parser.cpp"
    break;

  case 157:
#line 1013 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5052 "ds_parser.cpp"
    break;

  case 158:
#line 1016 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5062 "ds_parser.cpp"
    break;

  case 159:
#line 1024 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5070 "ds_parser.cpp"
    break;

  case 160:
#line 1027 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5080 "ds_parser.cpp"
    break;

  case 161:
#line 1032 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5088 "ds_parser.cpp"
    break;

  case 162:
#line 1035 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5098 "ds_parser.cpp"
    break;

  case 163:
#line 1043 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5106 "ds_parser.cpp"
    break;

  case 164:
#line 1049 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5112 "ds_parser.cpp"
    break;

  case 165:
#line 1050 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5118 "ds_parser.cpp"
    break;

  case 166:
#line 1054 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5124 "ds_parser.cpp"
    break;

  case 167:
#line 1055 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5130 "ds_parser.cpp"
    break;

  case 168:
#line 1059 "ds_parser.ypp"
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
#line 5166 "ds_parser.cpp"
    break;

  case 169:
#line 1093 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5174 "ds_parser.cpp"
    break;

  case 170:
#line 1096 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5184 "ds_parser.cpp"
    break;

  case 171:
#line 1101 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5194 "ds_parser.cpp"
    break;

  case 172:
#line 1109 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5203 "ds_parser.cpp"
    break;

  case 173:
#line 1113 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5213 "ds_parser.cpp"
    break;

  case 174:
#line 1118 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5224 "ds_parser.cpp"
    break;

  case 175:
#line 1124 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5233 "ds_parser.cpp"
    break;

  case 176:
#line 1128 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5243 "ds_parser.cpp"
    break;

  case 177:
#line 1133 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5254 "ds_parser.cpp"
    break;

  case 178:
#line 1142 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5262 "ds_parser.cpp"
    break;

  case 179:
#line 1145 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5270 "ds_parser.cpp"
    break;

  case 180:
#line 1151 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5276 "ds_parser.cpp"
    break;

  case 181:
#line 1152 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5288 "ds_parser.cpp"
    break;

  case 182:
#line 1159 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5301 "ds_parser.cpp"
    break;

  case 183:
#line 1170 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5307 "ds_parser.cpp"
    break;

  case 184:
#line 1171 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5313 "ds_parser.cpp"
    break;

  case 185:
#line 1175 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5323 "ds_parser.cpp"
    break;

  case 186:
#line 1181 "ds_parser.ypp"
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
#line 5379 "ds_parser.cpp"
    break;

  case 187:
#line 1235 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5385 "ds_parser.cpp"
    break;

  case 188:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5391 "ds_parser.cpp"
    break;

  case 189:
#line 1237 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5397 "ds_parser.cpp"
    break;

  case 190:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5403 "ds_parser.cpp"
    break;

  case 191:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5409 "ds_parser.cpp"
    break;

  case 192:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5415 "ds_parser.cpp"
    break;

  case 193:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5421 "ds_parser.cpp"
    break;

  case 194:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5427 "ds_parser.cpp"
    break;

  case 195:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5433 "ds_parser.cpp"
    break;

  case 196:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5439 "ds_parser.cpp"
    break;

  case 197:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5445 "ds_parser.cpp"
    break;

  case 198:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5451 "ds_parser.cpp"
    break;

  case 199:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5457 "ds_parser.cpp"
    break;

  case 200:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5463 "ds_parser.cpp"
    break;

  case 201:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5469 "ds_parser.cpp"
    break;

  case 202:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5475 "ds_parser.cpp"
    break;

  case 203:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5481 "ds_parser.cpp"
    break;

  case 204:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5487 "ds_parser.cpp"
    break;

  case 205:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5493 "ds_parser.cpp"
    break;

  case 206:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5499 "ds_parser.cpp"
    break;

  case 207:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5505 "ds_parser.cpp"
    break;

  case 208:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5511 "ds_parser.cpp"
    break;

  case 209:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5517 "ds_parser.cpp"
    break;

  case 210:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5523 "ds_parser.cpp"
    break;

  case 211:
#line 1268 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5535 "ds_parser.cpp"
    break;

  case 212:
#line 1279 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5545 "ds_parser.cpp"
    break;

  case 213:
#line 1284 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5557 "ds_parser.cpp"
    break;

  case 214:
#line 1294 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5566 "ds_parser.cpp"
    break;

  case 215:
#line 1298 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5578 "ds_parser.cpp"
    break;

  case 216:
#line 1305 "ds_parser.ypp"
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
#line 5594 "ds_parser.cpp"
    break;

  case 217:
#line 1319 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5600 "ds_parser.cpp"
    break;

  case 218:
#line 1320 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5606 "ds_parser.cpp"
    break;

  case 219:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5612 "ds_parser.cpp"
    break;

  case 220:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5618 "ds_parser.cpp"
    break;

  case 221:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5624 "ds_parser.cpp"
    break;

  case 222:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5630 "ds_parser.cpp"
    break;

  case 223:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5636 "ds_parser.cpp"
    break;

  case 224:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5642 "ds_parser.cpp"
    break;

  case 225:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5648 "ds_parser.cpp"
    break;

  case 226:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5654 "ds_parser.cpp"
    break;

  case 227:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5660 "ds_parser.cpp"
    break;

  case 228:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5666 "ds_parser.cpp"
    break;

  case 229:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5672 "ds_parser.cpp"
    break;

  case 230:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5678 "ds_parser.cpp"
    break;

  case 231:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5684 "ds_parser.cpp"
    break;

  case 232:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5690 "ds_parser.cpp"
    break;

  case 233:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5696 "ds_parser.cpp"
    break;

  case 234:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5702 "ds_parser.cpp"
    break;

  case 235:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5708 "ds_parser.cpp"
    break;

  case 236:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5714 "ds_parser.cpp"
    break;

  case 237:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5720 "ds_parser.cpp"
    break;

  case 238:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5726 "ds_parser.cpp"
    break;

  case 239:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5732 "ds_parser.cpp"
    break;

  case 240:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5738 "ds_parser.cpp"
    break;

  case 241:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5744 "ds_parser.cpp"
    break;

  case 242:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5750 "ds_parser.cpp"
    break;

  case 243:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5756 "ds_parser.cpp"
    break;

  case 244:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5762 "ds_parser.cpp"
    break;

  case 245:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5768 "ds_parser.cpp"
    break;

  case 246:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5774 "ds_parser.cpp"
    break;

  case 247:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5780 "ds_parser.cpp"
    break;

  case 248:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5786 "ds_parser.cpp"
    break;

  case 249:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5792 "ds_parser.cpp"
    break;

  case 250:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5798 "ds_parser.cpp"
    break;

  case 251:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5804 "ds_parser.cpp"
    break;

  case 252:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5810 "ds_parser.cpp"
    break;

  case 253:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5816 "ds_parser.cpp"
    break;

  case 254:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5822 "ds_parser.cpp"
    break;

  case 255:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5828 "ds_parser.cpp"
    break;

  case 256:
#line 1358 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5837 "ds_parser.cpp"
    break;

  case 257:
#line 1362 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5846 "ds_parser.cpp"
    break;

  case 258:
#line 1366 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5854 "ds_parser.cpp"
    break;

  case 259:
#line 1369 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5862 "ds_parser.cpp"
    break;

  case 260:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5868 "ds_parser.cpp"
    break;

  case 261:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5874 "ds_parser.cpp"
    break;

  case 262:
#line 1374 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5880 "ds_parser.cpp"
    break;

  case 263:
#line 1375 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5890 "ds_parser.cpp"
    break;

  case 264:
#line 1380 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5900 "ds_parser.cpp"
    break;

  case 265:
#line 1385 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5906 "ds_parser.cpp"
    break;

  case 266:
#line 1386 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5914 "ds_parser.cpp"
    break;

  case 267:
#line 1389 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5922 "ds_parser.cpp"
    break;

  case 268:
#line 1392 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5931 "ds_parser.cpp"
    break;

  case 269:
#line 1396 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5940 "ds_parser.cpp"
    break;

  case 270:
#line 1400 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5949 "ds_parser.cpp"
    break;

  case 271:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5955 "ds_parser.cpp"
    break;

  case 272:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5961 "ds_parser.cpp"
    break;

  case 273:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5967 "ds_parser.cpp"
    break;

  case 274:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5973 "ds_parser.cpp"
    break;

  case 275:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5979 "ds_parser.cpp"
    break;

  case 276:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5985 "ds_parser.cpp"
    break;

  case 277:
#line 1410 "ds_parser.ypp"
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
#line 6000 "ds_parser.cpp"
    break;

  case 278:
#line 1420 "ds_parser.ypp"
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
#line 6015 "ds_parser.cpp"
    break;

  case 279:
#line 1430 "ds_parser.ypp"
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
#line 6051 "ds_parser.cpp"
    break;

  case 280:
#line 1464 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6057 "ds_parser.cpp"
    break;

  case 281:
#line 1465 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6063 "ds_parser.cpp"
    break;

  case 282:
#line 1469 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6069 "ds_parser.cpp"
    break;

  case 283:
#line 1470 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6075 "ds_parser.cpp"
    break;

  case 284:
#line 1474 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6085 "ds_parser.cpp"
    break;

  case 285:
#line 1482 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6093 "ds_parser.cpp"
    break;

  case 286:
#line 1485 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6102 "ds_parser.cpp"
    break;

  case 287:
#line 1492 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6112 "ds_parser.cpp"
    break;

  case 288:
#line 1497 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6122 "ds_parser.cpp"
    break;

  case 289:
#line 1505 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6128 "ds_parser.cpp"
    break;

  case 290:
#line 1506 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6134 "ds_parser.cpp"
    break;

  case 291:
#line 1510 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6142 "ds_parser.cpp"
    break;

  case 292:
#line 1513 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6153 "ds_parser.cpp"
    break;

  case 293:
#line 1522 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6159 "ds_parser.cpp"
    break;

  case 294:
#line 1523 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6165 "ds_parser.cpp"
    break;

  case 295:
#line 1527 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6176 "ds_parser.cpp"
    break;

  case 296:
#line 1536 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6182 "ds_parser.cpp"
    break;

  case 297:
#line 1537 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6188 "ds_parser.cpp"
    break;

  case 298:
#line 1542 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6194 "ds_parser.cpp"
    break;

  case 299:
#line 1543 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6200 "ds_parser.cpp"
    break;

  case 300:
#line 1547 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6210 "ds_parser.cpp"
    break;

  case 301:
#line 1552 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6218 "ds_parser.cpp"
    break;

  case 302:
#line 1555 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6227 "ds_parser.cpp"
    break;

  case 303:
#line 1559 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6238 "ds_parser.cpp"
    break;

  case 304:
#line 1565 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6249 "ds_parser.cpp"
    break;

  case 305:
#line 1574 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6255 "ds_parser.cpp"
    break;

  case 306:
#line 1575 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6261 "ds_parser.cpp"
    break;

  case 307:
#line 1576 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6267 "ds_parser.cpp"
    break;

  case 308:
#line 1580 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6273 "ds_parser.cpp"
    break;

  case 309:
#line 1581 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6279 "ds_parser.cpp"
    break;

  case 310:
#line 1585 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6287 "ds_parser.cpp"
    break;

  case 311:
#line 1588 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6297 "ds_parser.cpp"
    break;

  case 312:
#line 1593 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6307 "ds_parser.cpp"
    break;

  case 313:
#line 1598 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6320 "ds_parser.cpp"
    break;

  case 314:
#line 1606 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6333 "ds_parser.cpp"
    break;

  case 315:
#line 1617 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6341 "ds_parser.cpp"
    break;

  case 316:
#line 1620 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6350 "ds_parser.cpp"
    break;

  case 317:
#line 1627 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6356 "ds_parser.cpp"
    break;

  case 318:
#line 1628 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6362 "ds_parser.cpp"
    break;

  case 319:
#line 1632 "ds_parser.ypp"
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
#line 6394 "ds_parser.cpp"
    break;

  case 320:
#line 1659 "ds_parser.ypp"
    { das_force_oxford_comma = true; }
#line 6400 "ds_parser.cpp"
    break;

  case 321:
#line 1659 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        yyerrok;
    }
#line 6410 "ds_parser.cpp"
    break;

  case 322:
#line 1667 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6418 "ds_parser.cpp"
    break;

  case 323:
#line 1670 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6432 "ds_parser.cpp"
    break;

  case 324:
#line 1679 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6446 "ds_parser.cpp"
    break;

  case 325:
#line 1692 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6460 "ds_parser.cpp"
    break;

  case 329:
#line 1713 "ds_parser.ypp"
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
#line 6476 "ds_parser.cpp"
    break;

  case 330:
#line 1724 "ds_parser.ypp"
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
#line 6493 "ds_parser.cpp"
    break;

  case 331:
#line 1739 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6499 "ds_parser.cpp"
    break;

  case 332:
#line 1740 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6505 "ds_parser.cpp"
    break;

  case 333:
#line 1744 "ds_parser.ypp"
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
#line 6546 "ds_parser.cpp"
    break;

  case 334:
#line 1783 "ds_parser.ypp"
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
#line 6614 "ds_parser.cpp"
    break;

  case 335:
#line 1849 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6626 "ds_parser.cpp"
    break;

  case 336:
#line 1856 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6637 "ds_parser.cpp"
    break;

  case 337:
#line 1865 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6643 "ds_parser.cpp"
    break;

  case 338:
#line 1866 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6649 "ds_parser.cpp"
    break;

  case 339:
#line 1867 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6655 "ds_parser.cpp"
    break;

  case 340:
#line 1868 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6661 "ds_parser.cpp"
    break;

  case 341:
#line 1869 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6667 "ds_parser.cpp"
    break;

  case 342:
#line 1870 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6673 "ds_parser.cpp"
    break;

  case 343:
#line 1871 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6679 "ds_parser.cpp"
    break;

  case 344:
#line 1872 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6685 "ds_parser.cpp"
    break;

  case 345:
#line 1873 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6691 "ds_parser.cpp"
    break;

  case 346:
#line 1874 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6697 "ds_parser.cpp"
    break;

  case 347:
#line 1875 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6703 "ds_parser.cpp"
    break;

  case 348:
#line 1876 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6709 "ds_parser.cpp"
    break;

  case 349:
#line 1877 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6715 "ds_parser.cpp"
    break;

  case 350:
#line 1878 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6721 "ds_parser.cpp"
    break;

  case 351:
#line 1879 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6727 "ds_parser.cpp"
    break;

  case 352:
#line 1880 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6733 "ds_parser.cpp"
    break;

  case 353:
#line 1881 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6739 "ds_parser.cpp"
    break;

  case 354:
#line 1882 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6745 "ds_parser.cpp"
    break;

  case 355:
#line 1883 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6751 "ds_parser.cpp"
    break;

  case 356:
#line 1884 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6757 "ds_parser.cpp"
    break;

  case 357:
#line 1885 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6763 "ds_parser.cpp"
    break;

  case 358:
#line 1886 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6769 "ds_parser.cpp"
    break;

  case 359:
#line 1887 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6775 "ds_parser.cpp"
    break;

  case 360:
#line 1888 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6781 "ds_parser.cpp"
    break;

  case 361:
#line 1892 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6787 "ds_parser.cpp"
    break;

  case 362:
#line 1893 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6793 "ds_parser.cpp"
    break;

  case 363:
#line 1894 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6799 "ds_parser.cpp"
    break;

  case 364:
#line 1895 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6805 "ds_parser.cpp"
    break;

  case 365:
#line 1896 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6811 "ds_parser.cpp"
    break;

  case 366:
#line 1897 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6817 "ds_parser.cpp"
    break;

  case 367:
#line 1901 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6830 "ds_parser.cpp"
    break;

  case 368:
#line 1912 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6839 "ds_parser.cpp"
    break;

  case 369:
#line 1916 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6851 "ds_parser.cpp"
    break;

  case 370:
#line 1926 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6857 "ds_parser.cpp"
    break;

  case 371:
#line 1927 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6863 "ds_parser.cpp"
    break;

  case 372:
#line 1928 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6869 "ds_parser.cpp"
    break;

  case 373:
#line 1929 "ds_parser.ypp"
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
#line 6888 "ds_parser.cpp"
    break;

  case 374:
#line 1943 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6899 "ds_parser.cpp"
    break;

  case 375:
#line 1949 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6908 "ds_parser.cpp"
    break;

  case 376:
#line 1953 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6918 "ds_parser.cpp"
    break;

  case 377:
#line 1958 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6928 "ds_parser.cpp"
    break;

  case 378:
#line 1963 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6938 "ds_parser.cpp"
    break;

  case 379:
#line 1968 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6950 "ds_parser.cpp"
    break;

  case 380:
#line 1975 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6960 "ds_parser.cpp"
    break;

  case 381:
#line 1980 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6969 "ds_parser.cpp"
    break;

  case 382:
#line 1984 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6978 "ds_parser.cpp"
    break;

  case 383:
#line 1988 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6988 "ds_parser.cpp"
    break;

  case 384:
#line 1993 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6998 "ds_parser.cpp"
    break;

  case 385:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7009 "ds_parser.cpp"
    break;

  case 386:
#line 2004 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7021 "ds_parser.cpp"
    break;

  case 387:
#line 2011 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7031 "ds_parser.cpp"
    break;

  case 388:
#line 2016 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7042 "ds_parser.cpp"
    break;

  case 389:
#line 2022 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7052 "ds_parser.cpp"
    break;

  case 390:
#line 2027 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7061 "ds_parser.cpp"
    break;

  case 391:
#line 2031 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7071 "ds_parser.cpp"
    break;

  case 392:
#line 2036 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7085 "ds_parser.cpp"
    break;

  case 393:
#line 2045 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7094 "ds_parser.cpp"
    break;

  case 394:
#line 2049 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7104 "ds_parser.cpp"
    break;

  case 395:
#line 2054 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7118 "ds_parser.cpp"
    break;

  case 396:
#line 2063 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7127 "ds_parser.cpp"
    break;

  case 397:
#line 2067 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7137 "ds_parser.cpp"
    break;

  case 398:
#line 2072 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7151 "ds_parser.cpp"
    break;

  case 399:
#line 2081 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7162 "ds_parser.cpp"
    break;

  case 400:
#line 2087 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7173 "ds_parser.cpp"
    break;

  case 401:
#line 2096 "ds_parser.ypp"
    {
        das_need_oxford_comma=false;
    }
#line 7181 "ds_parser.cpp"
    break;

  case 402:
#line 2102 "ds_parser.ypp"
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
#line 7198 "ds_parser.cpp"
    break;

  case 403:
#line 2117 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7204 "ds_parser.cpp"
    break;

  case 404:
#line 2118 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7210 "ds_parser.cpp"
    break;

  case 405:
#line 2119 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7216 "ds_parser.cpp"
    break;

  case 406:
#line 2120 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7222 "ds_parser.cpp"
    break;

  case 407:
#line 2124 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7234 "ds_parser.cpp"
    break;

  case 408:
#line 2131 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7245 "ds_parser.cpp"
    break;

  case 409:
#line 2140 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7255 "ds_parser.cpp"
    break;

  case 410:
#line 2145 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7264 "ds_parser.cpp"
    break;

  case 411:
#line 2152 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7274 "ds_parser.cpp"
    break;

  case 412:
#line 2157 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7285 "ds_parser.cpp"
    break;

  case 413:
#line 2163 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7296 "ds_parser.cpp"
    break;

  case 414:
#line 2169 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7308 "ds_parser.cpp"
    break;

  case 415:
#line 2176 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7321 "ds_parser.cpp"
    break;

  case 416:
#line 2187 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7329 "ds_parser.cpp"
    break;

  case 417:
#line 2190 "ds_parser.ypp"
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
#line 7345 "ds_parser.cpp"
    break;

  case 418:
#line 2204 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7356 "ds_parser.cpp"
    break;

  case 419:
#line 2213 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7362 "ds_parser.cpp"
    break;

  case 420:
#line 2214 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7368 "ds_parser.cpp"
    break;

  case 421:
#line 2218 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7378 "ds_parser.cpp"
    break;

  case 422:
#line 2223 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7387 "ds_parser.cpp"
    break;

  case 423:
#line 2230 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7397 "ds_parser.cpp"
    break;

  case 424:
#line 2235 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7410 "ds_parser.cpp"
    break;

  case 425:
#line 2246 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7420 "ds_parser.cpp"
    break;

  case 426:
#line 2251 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7429 "ds_parser.cpp"
    break;

  case 427:
#line 2258 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7443 "ds_parser.cpp"
    break;

  case 428:
#line 2270 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7449 "ds_parser.cpp"
    break;

  case 429:
#line 2271 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7455 "ds_parser.cpp"
    break;

  case 430:
#line 2275 "ds_parser.ypp"
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
#line 7474 "ds_parser.cpp"
    break;

  case 431:
#line 2289 "ds_parser.ypp"
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
#line 7493 "ds_parser.cpp"
    break;


#line 7497 "ds_parser.cpp"

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
#line 2305 "ds_parser.ypp"


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


