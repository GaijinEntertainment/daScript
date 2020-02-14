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
#line 51 "ds_parser.ypp"

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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list );
    
    LineInfo tokAt ( const struct YYLTYPE & li );
    
    Annotation * findAnnotation ( const string & name, const LineInfo & at );

#line 192 "ds_parser.cpp"

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
#line 77 "ds_parser.ypp"

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

#line 368 "ds_parser.cpp"

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
#define YYLAST   5652

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  167
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  99
/* YYNRULES -- Number of rules.  */
#define YYNRULES  399
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  714

#define YYUNDEFTOK  2
#define YYMAXUTOK   394

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
       0,   369,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   383,   395,   396,   400,   404,   408,   417,
     425,   441,   462,   466,   469,   475,   484,   505,   528,   529,
     533,   537,   538,   542,   545,   551,   557,   560,   566,   567,
     568,   575,   582,   594,   604,   613,   614,   615,   616,   617,
     618,   622,   627,   635,   636,   637,   638,   639,   640,   641,
     642,   648,   661,   677,   678,   682,   689,   701,   705,   712,
     713,   717,   718,   719,   723,   727,   734,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,   760,
     761,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   777,   847,   850,   860,   861,   862,   863,
     864,   865,   866,   867,   868,   869,   870,   871,   872,   873,
     874,   875,   876,   880,   884,   890,   896,   910,   911,   922,
     926,   932,   935,   938,   942,   948,   952,   956,   959,   962,
     967,   970,   978,   981,   986,   989,   997,  1003,  1004,  1008,
    1009,  1013,  1047,  1050,  1055,  1063,  1067,  1072,  1078,  1082,
    1087,  1096,  1099,  1105,  1106,  1116,  1121,  1173,  1174,  1175,
    1176,  1177,  1178,  1182,  1183,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1201,
    1212,  1217,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,
    1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1269,  1273,  1276,  1279,  1280,  1281,  1282,  1283,  1284,
    1289,  1294,  1295,  1298,  1301,  1302,  1303,  1304,  1305,  1306,
    1307,  1317,  1327,  1361,  1362,  1366,  1367,  1371,  1379,  1382,
    1389,  1390,  1394,  1395,  1399,  1405,  1406,  1410,  1411,  1415,
    1420,  1423,  1427,  1433,  1442,  1443,  1444,  1448,  1451,  1456,
    1461,  1468,  1478,  1481,  1488,  1489,  1493,  1523,  1526,  1535,
    1547,  1559,  1570,  1585,  1586,  1590,  1629,  1695,  1702,  1711,
    1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,
    1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,  1731,
    1732,  1733,  1734,  1738,  1739,  1740,  1741,  1742,  1743,  1744,
    1745,  1749,  1760,  1764,  1774,  1775,  1776,  1777,  1791,  1797,
    1801,  1806,  1811,  1816,  1823,  1828,  1832,  1836,  1841,  1846,
    1853,  1858,  1864,  1869,  1873,  1878,  1887,  1891,  1896,  1905,
    1909,  1914,  1923,  1932,  1933,  1934,  1935,  1939,  1946,  1955,
    1960,  1967,  1972,  1978,  1985,  1996,  1999,  2013,  2022,  2023,
    2027,  2032,  2039,  2044,  2055,  2060,  2067,  2079,  2080,  2084
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
     385,   386,    44,    61,    63,    58,   124,    94,    38,    60,
      62,    45,    43,    42,    47,    37,    64,   387,   388,   126,
      33,   389,   390,   391,   392,    46,    91,    93,    40,    41,
     393,   394,    59,   123,   125,    36,    35
};
# endif

#define YYPACT_NINF -471

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-471)))

#define YYTABLE_NINF -184

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -471,    22,  -471,  -471,   -63,   -60,    -6,     2,     8,  -471,
      41,    -5,  -471,  -471,  -471,  -471,  -471,    40,  -471,    23,
    -471,  -471,  -471,  -471,    35,  -471,    67,    42,    68,  -471,
    -471,    11,  -471,   -82,    77,  -471,  -471,    50,    93,    76,
    -471,   122,  -471,   119,   100,  -471,    70,    45,   -63,   128,
     -60,   127,  -471,   131,   143,  -471,   564,  -471,  4559,   146,
    -471,   -63,    -5,  -471,   114,   115,  5502,   225,   253,  -471,
     147,  -471,  -471,  -471,  -471,  -471,  -471,   197,   209,  -471,
    -471,  -471,  -471,   284,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,   174,  -102,   212,   213,   215,  -471,
    -471,  -471,   201,  -471,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,   216,  -471,  -471,
    -471,   223,   224,  -471,  -471,  -471,  -471,   227,  -471,  -471,
    -471,  -471,  5421,  -471,     0,  -471,   -93,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,    -8,   232,   -92,  -471,   -49,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,    83,  -471,  -471,
      75,  -471,  4559,  4559,  4559,   243,  4487,  4487,  4487,  4559,
    -471,    -3,  -471,  -471,  -471,  -471,  1780,  -471,  -471,  -471,
     -42,   244,  -471,  -471,  -471,  -119,  -471,    25,  4559,   208,
    -471,  -471,   -66,  -471,  -471,   209,  -471,   -90,  3181,  -471,
     354,   346,   636,   214,   232,  5214,   232,  5270,   232,  5283,
    -471,   -98,  5421,   234,  -471,   219,  -471,  -471,  -471,  4415,
     220,  -471,   226,   238,   242,   229,   246,   231,   251,  3181,
    3181,    28,  4559,  3181,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  3181,  3181,  3181,   -93,  3181,  3181,  -471,  3181,  -471,
     235,  -471,  -471,   -85,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  4555,   239,  -471,  -471,  -471,  -471,  -471,   -63,  -471,
     356,   241,  -471,  -471,   -14,  -471,   274,  -471,  4559,  3181,
    5421,  -471,  -471,  -471,  -471,  4559,  3181,  -471,  -471,   624,
    -471,  4559,  -471,  -471,   261,  -471,   269,  -471,   272,  -471,
    -471,  4559,  -471,  -471,  5290,  -471,   287,  3181,  4559,  4559,
    3181,  4559,  1905,  4559,    19,    19,   244,  1154,  1293,  4854,
    -471,   -45,   194,    19,    19,  -471,  -471,    19,    19,  3848,
     835,   147,  -471,  1409,   399,  3181,  3181,  -471,  -471,  3181,
    3181,  3181,  3181,   290,  3181,   294,  3181,  3181,  3181,  3181,
    3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,
    3181,   296,  3181,  -471,  2021,   123,  -471,   244,  -471,  -471,
    -471,  4549,  -471,   314,  4700,   345,  -471,  3261,  -471,  5328,
    -471,  -471,  -471,  -471,  2137,  1525,  3914,  5337,  5366,  3980,
    5377,  -471,  4046,  5380,     1,    71,  2253,  4854,   292,   157,
     293,  -471,  -471,   169,  2369,   124,   130,  3181,  3181,   262,
    -471,  3181,   300,  -471,  -471,  3181,  3181,   244,  1664,  -471,
     208,  3181,  3181,  -471,   270,   317,  2485,  2601,  -471,   411,
     283,   285,  -471,  -471,  -471,  -471,  -471,  -471,   286,   295,
     297,  -471,  -471,  -471,   429,  -471,   -86,  4798,   232,   323,
    -471,   103,  5022,  4559,   475,   475,   298,   298,  5243,  5243,
     303,   121,  -471,    91,    91,   475,   475,  4910,  5078,  5134,
    5190,   298,   298,   457,   457,   121,   121,   121,  -471,  4628,
    -471,   107,  -471,  -471,  3181,   133,  3181,  3181,  3181,  3181,
    3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,  3181,
    3181,  -471,  3181,  -471,  -471,  -471,   109,  4559,   329,  4112,
    -471,  3181,  3181,  -471,  3181,  -471,   304,  3181,  3181,   323,
     332,   306,   323,  3181,   307,  3181,   323,   308,   311,  5022,
    -471,  -471,  4966,   205,   205,    15,  3181,  -471,  -471,  3352,
     458,  5022,   205,  -471,   335,   351,  5022,  3181,  -471,  3443,
     310,  -471,  -471,  -471,  -471,  -471,   440,   244,  -471,   -88,
      97,   132,  3181,  -471,  5421,  2717,  3181,  -471,  -471,  5022,
    -471,  5022,  5022,  5022,  5022,  5022,  5022,  5022,  5022,  5022,
    5022,  5022,  5022,  5022,  5022,  5022,  -471,  3534,  -471,  1232,
     -84,  -471,   298,   298,   298,  2833,    58,  5022,   196,    97,
    -471,   292,  5022,  -471,  -471,   176,  -471,  -471,  -471,  -471,
     104,  3181,  -471,  3625,  -471,   208,  -471,  -471,  -471,  -471,
    3716,  -471,  -471,  -471,  -471,  3181,  -471,  -471,   318,  5022,
    -471,   112,  5022,  -471,  -471,  2949,   359,  -471,  4178,  3181,
     325,  3181,   333,   208,  3181,  -471,    62,  -471,  -471,  -471,
    1000,  5022,  -471,  -471,  4559,  4244,   357,  -471,  3779,  -471,
    5022,  -471,  -471,   205,   208,  -471,  5391,  -471,  3065,   445,
     339,   104,  -471,  -471,  4559,  4310,  3181,   342,  -471,  5406,
    -471,  5022,  -471,  -471
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
       0,     0,    29,     0,     0,    26,     0,   297,     0,     0,
     139,     0,     0,    70,   303,     0,     0,     0,     0,    76,
      71,   292,    57,    58,    55,    56,    54,     0,     0,    53,
      62,    34,    32,    28,    25,    24,   333,   337,   336,   340,
     334,   338,   335,   339,     0,     0,     0,     0,     0,   309,
     329,   310,   342,   311,   315,   316,   317,   318,   322,   323,
     324,   325,   326,   327,   328,   330,   331,   363,   314,   321,
     332,   366,   369,   312,   319,   313,   320,     0,   341,   344,
     346,   345,   300,   138,     0,    68,     0,   305,   268,    79,
      80,    82,    81,    83,    84,    85,    86,   105,   106,   103,
     104,    96,   107,   108,    97,    94,    95,   109,   110,   111,
     112,    99,   100,    98,    92,    93,    88,    87,    89,    90,
      91,    78,    77,   101,   102,     0,    74,     0,    14,     0,
      49,    50,    47,    48,    46,    45,    51,     0,    27,   297,
       0,   301,     0,     0,     0,     0,    71,    71,    71,     0,
     350,     0,   356,   359,   358,   352,     0,   355,    66,   304,
     263,     0,   307,    72,   272,     0,   270,   279,     0,     0,
     296,   293,     0,    15,    16,     0,    60,     0,     0,   298,
       0,     0,     0,     0,    74,     0,    74,     0,    74,     0,
     275,     0,   274,   351,   354,     0,   357,   207,   208,     0,
       0,   202,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   177,   179,   178,   180,   181,   182,
      17,     0,     0,     0,     0,     0,     0,   348,     0,   133,
      69,   205,   175,   203,   256,   255,   254,   259,   204,   258,
     257,     0,     0,   206,   373,   374,   375,   376,     0,   306,
     265,     0,   271,    73,     0,   278,     0,   277,     0,     0,
      75,   113,   285,   286,   284,     0,     0,    52,   302,     0,
     360,     0,   362,   343,     0,   364,     0,   367,     0,   370,
     372,     0,   353,   349,   141,   144,     0,     0,     0,     0,
       0,     0,     0,     0,   231,   232,     0,     0,     0,     0,
     394,     0,     0,   212,   211,   244,   239,   210,   209,     0,
       0,    71,   262,     0,     0,     0,     0,   233,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,   266,     0,   269,   273,
     308,   280,   283,     0,   282,     0,   291,   183,   299,     0,
     365,   368,   371,   276,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,   137,     0,   385,   379,     0,
     388,   389,   390,     0,     0,     0,     0,     0,     0,     0,
      20,     0,    18,   235,   135,     0,     0,     0,     0,   145,
       0,     0,     0,   146,     0,     0,     0,     0,   116,   114,
       0,     0,   128,   123,   121,   122,   134,   117,     0,     0,
       0,   126,   127,   129,   157,   120,     0,   183,    74,     0,
     240,     0,   171,     0,   213,   214,   226,   227,   224,   225,
       0,   251,   238,   260,   261,   215,   216,     0,   229,   230,
     228,   222,   223,   218,   217,   219,   220,   221,   237,     0,
     242,     0,   264,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   287,     0,   290,   361,   142,     0,     0,     0,     0,
     245,     0,     0,   246,     0,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   387,
     395,   396,     0,     0,     0,     0,     0,   147,   150,   183,
       0,   140,     0,   132,     0,     0,    37,     0,   154,   183,
       0,   130,   131,   119,   124,   125,     0,     0,   118,     0,
       0,     0,     0,   241,   253,     0,     0,   236,   243,   281,
     136,   190,   191,   193,   192,   194,   187,   188,   189,   195,
     196,   185,   186,   197,   198,   184,   289,   183,   143,     0,
       0,   165,   162,   163,   164,     0,     0,   377,     0,     0,
     381,   380,   386,   392,   391,     0,   383,   393,    19,    43,
      38,     0,   151,   183,   148,     0,    44,    35,    36,   155,
     183,   152,   133,   158,   161,     0,   173,   176,     0,   172,
     200,     0,   252,   288,   168,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,    41,     0,   149,   156,   153,
       0,   174,   199,   201,     0,     0,     0,   250,   397,   382,
     378,   384,    39,     0,     0,   115,     0,   166,     0,     0,
       0,    38,    42,   169,     0,     0,     0,     0,    40,     0,
     167,   398,   399,   170
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -471,  -471,  -471,   151,   462,  -471,  -471,  -471,  -471,  -471,
    -471,   421,  -471,  -471,   463,  -471,  -471,  -187,  -471,  -471,
    -471,  -471,   291,  -471,   467,   -58,  -471,   455,  -471,   245,
    -182,  -217,  -471,  -471,  -207,  -471,  -133,  -296,   -11,  -471,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,   520,  -471,  -471,
    -471,  -383,  -471,     7,  -471,  -359,  -471,  -471,   158,  -471,
    -471,  -471,  -471,   247,  -471,   200,  -471,  -210,  -205,   144,
     -54,  -471,  -471,  -471,   363,  -471,  -471,  -471,  -471,  -471,
    -173,   -56,  -471,  -471,  -471,   -48,   309,  -470,  -340,  -471,
    -471,  -258,     4,   207,  -471,  -471,  -471,  -471,  -471
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   179,   185,   352,   281,    14,    15,    31,
      32,    55,    16,    28,    29,   460,   461,   675,   462,   463,
     464,   465,   186,   187,    25,    26,    39,    40,    41,    17,
     176,   219,    70,    18,   282,   466,   360,   467,   283,   468,
     284,   469,   470,   471,   472,   473,   587,   474,   475,   285,
     286,   481,   657,   287,   288,   403,   289,   290,   482,   300,
     397,   301,   210,   214,   215,   240,   241,   548,   216,   316,
     221,   177,    46,    20,    95,    21,    22,   137,    65,    23,
     217,   292,    94,   130,   131,   242,   293,   428,   429,   294,
     430,   431,   432,   433,   295,   351,   296,   700,   297
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   476,   129,   134,   222,   350,   302,   309,   435,   591,
     132,   511,   311,   402,   234,   236,   238,   324,   547,   326,
     406,   328,     2,   515,   190,    36,   243,     3,   655,   211,
      51,   536,   641,    37,   212,   211,   190,   364,   346,   312,
     303,   362,   330,   304,    43,   313,    44,   128,     4,    96,
       5,    42,     6,    56,     7,    97,   665,    72,    73,     8,
      27,    52,   191,    24,   331,     9,   306,   314,    38,   315,
      98,    10,   220,   363,   318,   279,   588,    45,   666,   223,
     224,    57,   631,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   212,   673,   298,   367,   368,   438,   212,   439,
      30,    37,   299,   373,   674,   209,   375,   376,    33,   377,
     305,    66,    48,   306,    34,   244,   129,   129,   129,   364,
     129,   129,   129,   129,   230,   231,   232,   306,   235,   237,
     239,   213,   568,   245,    37,    53,    38,   306,   307,   208,
     308,   578,   129,   246,   626,    74,    54,    35,    47,    75,
     310,    76,    77,   424,   391,   392,   305,    49,    11,   478,
     560,   128,   128,   128,    12,   128,   128,   128,   128,    38,
     592,   514,   513,   129,   592,   373,    67,    68,   375,    48,
      50,   334,   305,    78,   307,   129,   129,   128,   228,   628,
      58,    59,   661,   347,   348,   225,   635,   367,   368,    60,
     669,   180,   181,   364,   694,   373,    69,   374,   375,   376,
     307,   377,    59,    71,    61,   592,   616,   229,   128,   592,
     395,   592,   226,   570,   592,    64,   391,   392,    81,   136,
     128,   128,   129,    83,    62,    48,   173,    84,   676,   129,
     401,   589,   593,   356,   550,   129,   598,   405,   618,    85,
     642,   683,   133,   409,   565,   129,   391,   392,   138,    63,
     512,   649,   129,   129,   174,   129,   552,   129,   557,   658,
     417,   418,   555,   420,   558,   423,   279,   128,   280,   365,
     366,   367,   368,   369,   128,   175,   370,   371,   372,   373,
     128,   374,   375,   376,   551,   377,   364,   378,   379,   552,
     128,   476,   178,   440,   441,   178,   554,   128,   128,   182,
     128,   555,   128,   183,    52,   184,    77,   189,   552,   380,
     672,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   192,   193,   670,   194,   196,   639,   640,   552,   195,
     391,   392,   197,   198,   291,   646,   199,   218,   279,   233,
     212,   279,   332,   323,   200,   200,   333,   338,   336,   201,
     201,   339,   656,   200,   337,   341,   319,   340,   201,   342,
     343,    11,   365,   366,   367,   368,   396,   394,   202,   202,
     400,   410,   373,   398,   374,   375,   376,   202,   377,   411,
     378,   379,   412,   415,   222,   483,   490,   344,   345,   671,
     492,   349,   508,   515,   550,   553,   561,   129,   223,   353,
     354,   355,   573,   357,   358,   594,   359,   574,   678,   386,
     387,   388,   389,   390,   580,   581,   586,   582,   583,   590,
     312,   203,   203,   391,   392,   620,   313,   584,   629,   585,
     203,   595,   625,   630,   633,   636,   692,   404,   637,   645,
     647,   648,   128,   652,   407,   364,   653,   682,   314,   204,
     204,   129,   689,   205,   205,   686,   701,   702,   204,   619,
     691,   706,   205,   364,   320,   416,   707,   698,   419,   712,
     422,   206,   206,   442,   188,   427,   427,   531,   321,    79,
     206,   207,   207,    82,   708,    80,   317,   135,   477,   680,
     207,    19,   600,   484,   485,   361,   128,   486,   487,   488,
     489,   413,   491,   654,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   532,
     509,   399,   227,   367,   368,   436,     0,     0,   335,   634,
       0,   373,     0,   374,   375,   376,     0,   377,     0,     0,
       0,   367,   368,   539,     0,     0,     0,     0,     0,   373,
       0,   374,   375,   376,   359,   377,     0,     0,     0,     0,
       0,     0,   359,     0,     0,   559,   349,     0,     0,   562,
     388,   389,   390,   563,   564,     0,   569,     0,     0,   571,
     572,     0,   391,   392,   576,   579,   386,   387,   388,   389,
     390,     0,     0,    86,     0,     0,     0,    87,   129,     0,
     391,   392,     0,     0,     0,     0,   696,     0,    88,    89,
       0,     0,   364,    90,    91,    92,    93,     0,   129,     0,
       0,     0,     0,     0,     0,     0,   709,     0,     0,     0,
       0,     0,     0,     0,     0,   200,     0,     0,     0,     0,
     201,     0,   599,   128,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   202,
     617,     0,     0,   128,     0,     0,     0,     0,     0,   622,
     623,     0,   624,     0,     0,     0,   627,     0,     0,     0,
       0,   632,     0,   427,     0,     0,     0,     0,   365,   366,
     367,   368,   369,     0,   643,   370,   371,   372,   373,     0,
     374,   375,   376,     0,   377,   650,   378,   379,     0,     0,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
     659,     0,     0,     0,   662,     0,     0,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     204,     0,     0,     0,   205,     0,   322,     0,     0,   391,
     392,     0,     0,   668,     0,     0,   408,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   681,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   685,     0,     0,     0,   688,     0,   690,
       0,     0,   693,     0,     0,     0,   444,     0,   477,     0,
       3,     0,   445,   446,     0,   447,     0,   247,   248,   249,
     250,     0,     0,     0,     0,     0,   705,   448,   251,   449,
     450,     0,     0,     0,   711,     0,     0,     0,     0,   451,
     252,     0,     0,   452,   253,     0,   254,     0,     9,   255,
     453,     0,   454,   256,     0,     0,   455,   456,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,   257,   123,   124,   125,   126,     0,   258,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,   268,
     269,    37,   270,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   271,   272,   273,     0,
       0,   274,     0,     0,   275,   276,     0,     0,     0,     0,
       0,     0,     0,   278,     0,     0,    38,   458,   279,   459,
     280,   444,     0,     0,     0,     3,     0,   445,   446,     0,
     447,     0,   247,   248,   249,   250,     0,     0,     0,     0,
       0,     0,   448,   251,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   252,     0,     0,   452,   253,
       0,   254,     0,     9,   255,   453,     0,   454,   256,     0,
       0,   455,   456,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,   257,   123,
     124,   125,   126,     0,   258,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   262,   263,
     264,   265,   266,   267,   268,   269,    37,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,   272,   273,     0,     0,   274,     0,     0,   275,
     276,     0,     0,     0,     0,     0,     0,     0,   278,     0,
       0,    38,   458,   279,   695,   280,   247,   248,   249,   250,
       0,     0,     0,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,   200,     0,     0,     0,     0,   201,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,     0,     0,     0,   202,     0,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,   257,   123,   124,   125,   126,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   260,     0,     0,     0,     0,     0,     0,     0,     0,
     203,   200,     0,     0,     0,     0,   201,     0,     0,     0,
       0,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     425,   270,     0,     0,     0,   202,     0,     0,   204,     0,
       0,     0,   205,     0,     0,   271,   272,   273,     0,     0,
     274,     0,     0,   275,   276,   247,   248,   249,   250,     0,
     206,     0,   426,     0,     0,    38,   251,   279,     0,   280,
     207,     0,   200,     0,     0,     0,     0,   201,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,   203,     0,
       0,   256,     0,     0,     0,     0,   202,     0,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   204,   118,   119,   120,
     205,   257,   123,   124,   125,   126,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   206,   259,
     260,   664,     0,     0,     0,     0,     0,     0,   207,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   425,
     270,   247,   248,   249,   250,     0,     0,   204,     0,     0,
       0,   205,   251,     0,   271,   272,   273,     0,     0,   274,
       0,     0,   275,   276,   252,     0,     0,     0,   253,   206,
     254,   434,     0,   255,    38,     0,   279,   256,   280,   207,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,   257,   123,   124,
     125,   126,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    37,   270,   247,   248,   249,
     250,   537,     0,     0,     0,     0,     0,     0,   251,     0,
     271,   272,   273,     0,     0,   274,     0,     0,   275,   276,
     252,     0,     0,     0,   253,   479,   254,   278,   480,   255,
      38,     0,   279,   256,   280,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,   257,   123,   124,   125,   126,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,   268,
     269,    37,   270,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,     0,   271,   272,   273,     0,
       0,   274,     0,     0,   275,   276,   247,   248,   249,   250,
       0,     0,     0,   278,     0,     0,    38,   251,   279,     0,
     280,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,     0,     0,     0,     0,     0,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,   257,   123,   124,   125,   126,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   260,     0,     0,     0,     0,     0,     0,     0,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   262,   263,   264,   265,   266,   267,   268,   269,
      37,   270,   247,   248,   249,   250,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   271,   272,   273,     0,     0,
     274,     0,     0,   275,   276,   252,     0,     0,     0,   253,
       0,   254,   278,     0,   255,    38,   567,   279,   256,   280,
       0,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,   257,   123,
     124,   125,   126,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   262,   263,
     264,   265,   266,   267,   268,   269,    37,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   271,   272,   273,     0,     0,   274,     0,   251,   275,
     276,     0,     0,     0,     0,     0,     0,   277,   278,     0,
     252,    38,     0,   279,   253,   280,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,   257,   123,   124,   125,   126,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,   268,
     269,    37,   270,   247,   248,   249,   250,     0,     0,     0,
       0,     0,     0,     0,   251,     0,   271,   272,   273,     0,
       0,   274,     0,     0,   275,   276,   252,     0,     0,     0,
     253,     0,   254,   278,   421,   255,    38,     0,   279,   256,
     280,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,   257,
     123,   124,   125,   126,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,   262,
     263,   264,   265,   266,   267,   268,   269,    37,   270,   247,
     248,   249,   250,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   271,   272,   273,     0,     0,   274,     0,     0,
     275,   276,   252,     0,     0,     0,   253,     0,   254,   278,
     510,   255,    38,     0,   279,   256,   280,     0,     0,     0,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,   257,   123,   124,   125,   126,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   262,   263,   264,   265,   266,
     267,   268,   269,    37,   270,   247,   248,   249,   250,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   271,   272,
     273,     0,     0,   274,     0,     0,   275,   276,   252,     0,
       0,     0,   253,     0,   254,   278,   535,   255,    38,     0,
     279,   256,   280,     0,     0,     0,     0,     0,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,   257,   123,   124,   125,   126,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,   263,   264,   265,   266,   267,   268,   269,    37,
     270,   247,   248,   249,   250,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   271,   272,   273,     0,     0,   274,
       0,     0,   275,   276,   252,     0,     0,     0,   253,     0,
     254,   278,   549,   255,    38,     0,   279,   256,   280,     0,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,   257,   123,   124,
     125,   126,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    37,   270,   247,   248,   249,
     250,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     271,   272,   273,     0,     0,   274,     0,     0,   275,   276,
     252,     0,     0,     0,   253,     0,   254,   278,   556,   255,
      38,     0,   279,   256,   280,     0,   575,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,   257,   123,   124,   125,   126,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,   268,
     269,    37,   270,   247,   248,   249,   250,     0,     0,     0,
       0,     0,     0,     0,   251,     0,   271,   272,   273,     0,
       0,   274,     0,     0,   275,   276,   252,     0,     0,     0,
     253,     0,   254,   278,     0,   255,    38,     0,   279,   256,
     280,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,   257,
     123,   124,   125,   126,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,   262,
     263,   264,   265,   266,   267,   268,   269,    37,   270,   247,
     248,   249,   250,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   271,   272,   273,     0,     0,   274,     0,     0,
     275,   276,   252,     0,     0,     0,   253,     0,   254,   278,
       0,   255,    38,     0,   279,   256,   280,     0,     0,     0,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,   257,   123,   124,   125,   126,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   262,   263,   264,   265,   266,
     267,   268,   269,    37,   270,   247,   248,   249,   250,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   271,   272,
     273,     0,     0,   274,     0,     0,   275,   276,   252,     0,
       0,     0,   253,     0,   254,   278,   660,   255,    38,     0,
     279,   256,   280,     0,     0,     0,     0,     0,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,   257,   123,   124,   125,   126,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   262,   263,   264,   265,   266,   267,   268,   269,    37,
     270,   247,   248,   249,   250,   684,     0,     0,     0,     0,
       0,     0,   251,     0,   271,   272,   273,     0,     0,   274,
       0,     0,   275,   276,   252,     0,     0,     0,   253,     0,
     254,   278,   667,   255,    38,     0,   279,   256,   280,     0,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,   257,   123,   124,
     125,   126,     0,   258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    37,   270,   247,   248,   249,
     250,   704,     0,     0,     0,     0,     0,     0,   251,     0,
     271,   272,   273,     0,     0,   274,     0,     0,   275,   276,
     252,     0,     0,     0,   253,     0,   254,   278,     0,   255,
      38,     0,   279,   256,   280,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,   257,   123,   124,   125,   126,     0,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   260,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   262,   263,   264,   265,   266,   267,   268,
     269,    37,   270,   247,   248,   249,   250,     0,     0,     0,
       0,     0,     0,     0,   251,     0,   271,   272,   273,     0,
       0,   274,     0,     0,   275,   276,   252,     0,     0,     0,
     253,     0,   254,   278,     0,   255,    38,     0,   279,   256,
     280,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,   257,
     123,   124,   125,   126,     0,   258,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   261,   262,
     263,   264,   265,   266,   267,   268,   269,    37,   270,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   271,   272,   273,     0,     0,   274,     0,     0,
     275,   276,     0,     0,     0,     0,     0,     0,     0,   278,
       0,     0,    38,     0,   279,     0,   280,   516,   517,   518,
     519,   520,   521,   522,   523,   365,   366,   367,   368,   369,
     524,   525,   370,   371,   372,   373,   526,   374,   375,   376,
     364,   377,   527,   378,   379,   528,   529,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,   392,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,   523,   365,   366,   367,   368,
     369,   524,   525,   370,   371,   372,   373,   526,   374,   375,
     376,   364,   377,   527,   378,   379,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   530,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   391,   392,     0,
       0,     0,     0,     0,   644,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,   519,   520,   521,   522,   523,   365,   366,   367,
     368,   369,   524,   525,   370,   371,   372,   373,   526,   374,
     375,   376,   364,   377,   527,   378,   379,   528,   529,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   380,     0,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   391,   392,
       0,     0,     0,     0,     0,   651,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,   523,   365,   366,
     367,   368,   369,   524,   525,   370,   371,   372,   373,   526,
     374,   375,   376,   364,   377,   527,   378,   379,   528,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   530,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   391,
     392,     0,     0,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,   523,   365,
     366,   367,   368,   369,   524,   525,   370,   371,   372,   373,
     526,   374,   375,   376,   364,   377,   527,   378,   379,   528,
     529,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     391,   392,     0,     0,     0,     0,     0,   677,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   364,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,   523,
     365,   366,   367,   368,   369,   524,   525,   370,   371,   372,
     373,   526,   374,   375,   376,     0,   377,   527,   378,   379,
     528,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   530,
     380,     0,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,     0,     0,     0,     0,   364,     0,     0,     0,
       0,   391,   392,   365,   366,   367,   368,   369,   679,     0,
     370,   371,   372,   373,     0,   374,   375,   376,     0,   377,
       0,   378,   379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,     0,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,     0,     0,     0,     0,     0,
       0,     0,   364,     0,   391,   392,     0,     0,     0,     0,
       0,   699,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,   373,     0,   374,   375,   376,     0,   377,     0,
     378,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     0,     0,     0,     0,   364,     0,
       0,     0,     0,   391,   392,     0,     0,   443,   365,   366,
     367,   368,   369,     0,     0,   370,   371,   372,   373,     0,
     374,   375,   376,     0,   377,     0,   378,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
       0,     0,     0,     0,   364,     0,     0,     0,     0,   391,
     392,     0,     0,   540,   365,   366,   367,   368,   369,     0,
       0,   370,   371,   372,   373,     0,   374,   375,   376,     0,
     377,     0,   378,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,     0,     0,     0,     0,
     364,     0,     0,     0,     0,   391,   392,     0,     0,   543,
     365,   366,   367,   368,   369,     0,     0,   370,   371,   372,
     373,     0,   374,   375,   376,     0,   377,     0,   378,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,     0,     0,     0,     0,   364,     0,     0,     0,
       0,   391,   392,     0,     0,   545,   365,   366,   367,   368,
     369,     0,     0,   370,   371,   372,   373,     0,   374,   375,
     376,     0,   377,     0,   378,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,     0,     0,
       0,     0,   364,     0,     0,     0,     0,   391,   392,     0,
       0,   621,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,   373,     0,   374,   375,   376,     0,   377,     0,
     378,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     0,     0,     0,     0,   364,     0,
       0,     0,     0,   391,   392,     0,     0,   687,   365,   366,
     367,   368,   369,     0,     0,   370,   371,   372,   373,     0,
     374,   375,   376,     0,   377,     0,   378,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   391,
     392,     0,     0,   697,   365,   366,   367,   368,   369,     0,
       0,   370,   371,   372,   373,     0,   374,   375,   376,     0,
     377,     0,   378,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,    97,     0,   380,     0,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,     0,    98,     0,     0,
       0,     0,     0,     0,     0,   391,   392,     0,     0,   710,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,   261,   262,   263,     0,     0,     0,     0,     0,
       0,    37,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,     0,   364,     0,     0,    38,     0,   200,     0,
      96,     0,     0,   201,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,   202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,     0,     0,   175,   364,     0,    38,   365,
     366,   367,   368,   369,   305,   203,   370,   371,   372,   373,
       0,   374,   375,   376,     0,   377,     0,   378,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,   204,     0,    37,     0,   205,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,     0,     0,     0,   206,     0,     0,     0,     0,
     391,   392,   393,     0,     0,   207,     0,     0,   364,     0,
      38,     0,   365,   366,   367,   368,   369,     0,     0,   370,
     371,   372,   373,     0,   374,   375,   376,     0,   377,     0,
     378,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   391,   392,   597,   516,   517,   518,   519,
     520,   521,   522,   523,   365,   366,   367,   368,   369,   524,
     525,   370,   371,   372,   373,   526,   374,   375,   376,  -183,
     377,   527,   378,   379,   528,   529,   364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   530,   380,     0,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   391,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,   523,   365,   366,   367,   368,   369,   524,   525,   370,
     371,   372,   373,   526,   374,   375,   376,     0,   377,   527,
     378,   379,   528,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   364,     0,
       0,   530,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     0,     0,     0,     0,   365,   366,
     367,   368,   369,   391,   392,   370,   371,   372,   373,     0,
     374,   375,   376,     0,   377,     0,   378,   379,     0,     0,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   364,     0,     0,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
       0,     0,     0,     0,   365,   366,   367,   368,   369,   391,
     392,   370,   371,   372,   373,     0,   374,   375,   376,     0,
     377,     0,   378,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,     0,   380,   596,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,     0,     0,     0,     0,
     365,   366,   367,   368,   369,   391,   392,   370,   371,   372,
     373,     0,   374,   375,   376,     0,   377,     0,   378,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   364,   638,     0,     0,
     380,     0,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,     0,     0,     0,     0,   365,   366,   367,   368,
     369,   391,   392,   370,   371,   372,   373,     0,   374,   375,
     376,     0,   377,     0,   378,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,     0,     0,
       0,     0,   365,   366,   367,   368,   369,   391,   392,   370,
     371,   372,   373,     0,   374,   375,   376,     0,   377,     0,
     378,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   364,     0,
       0,     0,     0,     0,     0,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     0,     0,     0,     0,   365,   366,
     367,   368,   369,   391,   392,   370,   371,   372,   373,     0,
     374,   375,   376,   200,   377,     0,   378,   379,   201,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,   383,   384,   385,   386,   387,   388,   389,   390,
       0,     0,     0,     0,   365,   366,   367,   368,   369,   391,
     392,   370,   371,   372,   373,     0,   374,   375,   376,   200,
     377,     0,   378,   379,   201,     0,     0,     0,     0,     0,
       0,     0,   200,     0,     0,     0,     0,   201,     0,   200,
     203,     0,     0,   202,   201,     0,     0,     0,     0,   384,
     385,   386,   387,   388,   389,   390,   202,   365,   366,   367,
     368,   369,     0,   202,   370,   391,   392,   373,   204,   374,
     375,   376,   205,   377,   325,   378,   379,   200,     0,     0,
       0,     0,   201,     0,     0,     0,   200,     0,     0,     0,
     206,   201,     0,     0,     0,     0,   203,     0,     0,     0,
     207,   202,   384,   385,   386,   387,   388,   389,   390,   203,
     202,     0,     0,     0,     0,   200,   203,     0,   391,   392,
     201,     0,     0,     0,   204,     0,   200,     0,   205,   200,
     327,   201,     0,     0,   201,     0,     0,   204,     0,   202,
     200,   205,     0,   329,   204,   201,   206,     0,   205,     0,
     202,     0,     0,   202,   203,   200,   207,     0,     0,   206,
     201,     0,     0,   203,   202,     0,   206,     0,   414,   207,
     200,     0,     0,     0,     0,   201,   207,     0,     0,   202,
       0,     0,   204,     0,     0,     0,   205,     0,   534,     0,
       0,   204,   203,     0,   202,   205,     0,   541,     0,     0,
       0,     0,     0,   203,   206,     0,   203,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,   203,     0,     0,
     204,     0,     0,   207,   205,     0,   542,     0,     0,     0,
       0,   204,   203,     0,   204,   205,     0,   544,   205,     0,
     546,     0,   206,     0,     0,   204,     0,   203,     0,   205,
       0,     0,   207,   206,     0,     0,   206,     0,     0,     0,
     204,     0,     0,   207,   205,     0,   207,   206,     0,     0,
     703,     0,     0,     0,     0,   204,     0,   207,     0,   205,
       0,     0,   206,     0,     0,   713,     0,     0,     0,     0,
       0,     0,   207,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   207,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,     0,     0,     0,
       0,     0,     0,     0,   157,   158,   159,   160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,     0,     0,
       0,   171,   172
};

static const yytype_int16 yycheck[] =
{
      11,   360,    58,    61,   177,   263,   211,   217,   348,   479,
      58,   394,   219,   309,   196,   197,   198,   234,    17,   236,
     316,   238,     0,   109,   126,    30,    29,     5,   116,    43,
      19,   414,    17,   126,   126,    43,   126,    18,    10,   105,
     159,   126,   140,   162,     4,   111,     6,    58,    26,    21,
      28,    62,    30,   135,    32,    27,   140,    12,    13,    37,
     120,    50,   164,   126,   162,    43,   132,   133,   161,   135,
      42,    49,   164,   158,   164,   163,   162,    54,   162,   128,
     129,   163,   552,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,   126,     9,   156,    96,    97,   162,   126,   164,
     126,   126,   164,   104,    20,   136,   107,   108,   126,   110,
     105,    31,   132,   132,   126,   138,   192,   193,   194,    18,
     196,   197,   198,   199,   192,   193,   194,   132,   196,   197,
     198,   159,   448,   156,   126,   144,   161,   132,   133,   159,
     135,   457,   218,   166,   547,   120,   155,   126,   133,   124,
     218,   126,   127,   346,   155,   156,   105,   135,   156,   361,
     438,   192,   193,   194,   162,   196,   197,   198,   199,   161,
     132,   401,   397,   249,   132,   104,    96,    97,   107,   132,
     132,   249,   105,   158,   133,   261,   262,   218,   133,   549,
     133,   161,   595,   261,   262,   132,   556,    96,    97,   126,
     162,    12,    13,    18,   162,   104,   126,   106,   107,   108,
     133,   110,   161,   163,   158,   132,   532,   162,   249,   132,
     298,   132,   159,   450,   132,   126,   155,   156,   120,   135,
     261,   262,   308,   126,   132,   132,    31,   126,   641,   315,
     308,   478,   159,   274,   132,   321,   159,   315,   159,   126,
     566,   159,   126,   321,   447,   331,   155,   156,   163,   157,
     157,   577,   338,   339,    31,   341,   162,   343,   164,   157,
     338,   339,   162,   341,   164,   343,   163,   308,   165,    94,
      95,    96,    97,    98,   315,   158,   101,   102,   103,   104,
     321,   106,   107,   108,   157,   110,    18,   112,   113,   162,
     331,   680,   128,   129,   130,   128,   157,   338,   339,   120,
     341,   162,   343,   124,    50,   126,   127,   163,   162,   134,
     164,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   139,   139,   157,   139,   139,   563,   564,   162,   158,
     155,   156,   139,   139,   206,   572,   139,   135,   163,   126,
     126,   163,   138,   159,    29,    29,   157,   139,   158,    34,
      34,   139,   589,    29,   158,   139,   228,   158,    34,   158,
     139,   156,    94,    95,    96,    97,    40,   158,    53,    53,
     126,   140,   104,   162,   106,   107,   108,    53,   110,   140,
     112,   113,   140,   126,   587,    16,   126,   259,   260,   629,
     126,   263,   126,   109,   132,   132,   164,   483,   128,   271,
     272,   273,   162,   275,   276,   483,   278,   120,   645,   141,
     142,   143,   144,   145,    33,   162,    17,   162,   162,   126,
     105,   106,   106,   155,   156,   126,   111,   162,   126,   162,
     106,   158,   158,   157,   157,   157,   673,   309,   157,    11,
     135,   120,   483,   163,   316,    18,    36,   159,   133,   134,
     134,   537,   157,   138,   138,   126,   693,   694,   134,   537,
     157,    46,   138,    18,   140,   337,   157,   140,   340,   157,
     342,   156,   156,   352,    83,   347,   348,   162,   162,    47,
     156,   166,   166,    50,   701,    48,   225,    62,   360,   652,
     166,     1,   515,   365,   366,   280,   537,   369,   370,   371,
     372,   331,   374,   587,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   405,
     392,   304,   189,    96,    97,   348,    -1,    -1,   249,   555,
      -1,   104,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
      -1,    96,    97,   415,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,   426,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   434,    -1,    -1,   437,   438,    -1,    -1,   441,
     143,   144,   145,   445,   446,    -1,   448,    -1,    -1,   451,
     452,    -1,   155,   156,   456,   457,   141,   142,   143,   144,
     145,    -1,    -1,    59,    -1,    -1,    -1,    63,   684,    -1,
     155,   156,    -1,    -1,    -1,    -1,   684,    -1,    74,    75,
      -1,    -1,    18,    79,    80,    81,    82,    -1,   704,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   704,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,   514,   684,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,    53,
     532,    -1,    -1,   704,    -1,    -1,    -1,    -1,    -1,   541,
     542,    -1,   544,    -1,    -1,    -1,   548,    -1,    -1,    -1,
      -1,   553,    -1,   555,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    -1,   566,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   577,   112,   113,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     592,    -1,    -1,    -1,   596,    -1,    -1,    -1,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     134,    -1,    -1,    -1,   138,    -1,   140,    -1,    -1,   155,
     156,    -1,    -1,   625,    -1,    -1,   162,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   655,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   665,    -1,    -1,    -1,   669,    -1,   671,
      -1,    -1,   674,    -1,    -1,    -1,     1,    -1,   680,    -1,
       5,    -1,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,   698,    22,    23,    24,
      25,    -1,    -1,    -1,   706,    -1,    -1,    -1,    -1,    34,
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
      97,   159,    -1,    -1,    -1,    -1,    -1,    -1,   166,   106,
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
     144,   145,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   155,   156,    94,    95,    96,    97,    98,   162,    -1,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,   155,   156,    -1,    -1,    -1,    -1,
      -1,   162,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,
      96,    97,    98,    -1,    -1,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,   159,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,   159,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,   159,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,   159,    94,    95,
      96,    97,    98,    -1,    -1,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,   159,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,   134,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    18,    -1,    -1,   161,    -1,    29,    -1,
      21,    -1,    -1,    34,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,   158,    18,    -1,   161,    94,
      95,    96,    97,    98,   105,   106,   101,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   126,    -1,   138,    -1,   134,
      -1,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
     155,   156,   157,    -1,    -1,   166,    -1,    -1,    18,    -1,
     161,    -1,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,   157,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,   133,   134,    -1,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,   155,   156,   101,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,   134,    -1,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,   155,
     156,   101,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,   155,   156,   101,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,   131,    -1,    -1,
     134,    -1,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,   155,   156,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,   134,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,   155,   156,   101,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,   155,   156,   101,   102,   103,   104,    -1,
     106,   107,   108,    29,   110,    -1,   112,   113,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,   155,
     156,   101,   102,   103,   104,    -1,   106,   107,   108,    29,
     110,    -1,   112,   113,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    29,
     106,    -1,    -1,    53,    34,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,    53,    94,    95,    96,
      97,    98,    -1,    53,   101,   155,   156,   104,   134,   106,
     107,   108,   138,   110,   140,   112,   113,    29,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    29,    -1,    -1,    -1,
     156,    34,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     166,    53,   139,   140,   141,   142,   143,   144,   145,   106,
      53,    -1,    -1,    -1,    -1,    29,   106,    -1,   155,   156,
      34,    -1,    -1,    -1,   134,    -1,    29,    -1,   138,    29,
     140,    34,    -1,    -1,    34,    -1,    -1,   134,    -1,    53,
      29,   138,    -1,   140,   134,    34,   156,    -1,   138,    -1,
      53,    -1,    -1,    53,   106,    29,   166,    -1,    -1,   156,
      34,    -1,    -1,   106,    53,    -1,   156,    -1,   158,   166,
      29,    -1,    -1,    -1,    -1,    34,   166,    -1,    -1,    53,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
      -1,   134,   106,    -1,    53,   138,    -1,   140,    -1,    -1,
      -1,    -1,    -1,   106,   156,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   166,    -1,    -1,   106,    -1,    -1,
     134,    -1,    -1,   166,   138,    -1,   140,    -1,    -1,    -1,
      -1,   134,   106,    -1,   134,   138,    -1,   140,   138,    -1,
     140,    -1,   156,    -1,    -1,   134,    -1,   106,    -1,   138,
      -1,    -1,   166,   156,    -1,    -1,   156,    -1,    -1,    -1,
     134,    -1,    -1,   166,   138,    -1,   166,   156,    -1,    -1,
     159,    -1,    -1,    -1,    -1,   134,    -1,   166,    -1,   138,
      -1,    -1,   156,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,   149,   150
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
     132,    19,    50,   144,   155,   178,   135,   163,   133,   161,
     126,   158,   132,   157,   126,   245,    31,    96,    97,   126,
     199,   163,    12,    13,   120,   124,   126,   127,   158,   171,
     191,   120,   181,   126,   126,   126,    59,    63,    74,    75,
      79,    80,    81,    82,   249,   241,    21,    27,    42,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   205,   248,
     250,   251,   252,   126,   192,   194,   135,   244,   163,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   112,   113,   114,
     115,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   149,   150,    31,    31,   158,   197,   238,   128,   170,
      12,    13,   120,   124,   126,   171,   189,   190,   178,   163,
     126,   164,   139,   139,   139,   158,   139,   139,   139,   139,
      29,    34,    53,   106,   134,   138,   156,   166,   159,   205,
     229,    43,   126,   159,   230,   231,   235,   247,   135,   198,
     164,   237,   247,   128,   129,   132,   159,   241,   133,   162,
     252,   252,   252,   126,   197,   252,   197,   252,   197,   252,
     232,   233,   252,    29,   138,   156,   166,    12,    13,    14,
      15,    23,    35,    39,    41,    44,    48,    78,    84,    96,
      97,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     127,   141,   142,   143,   146,   149,   150,   157,   158,   163,
     165,   173,   201,   205,   207,   216,   217,   220,   221,   223,
     224,   225,   248,   253,   256,   261,   263,   265,   156,   164,
     226,   228,   235,   159,   162,   105,   132,   133,   135,   234,
     252,   201,   105,   111,   133,   135,   236,   189,   164,   225,
     140,   162,   140,   159,   198,   140,   198,   140,   198,   140,
     140,   162,   138,   157,   252,   253,   158,   158,   139,   139,
     158,   139,   158,   139,   225,   225,    10,   252,   252,   225,
     258,   262,   172,   225,   225,   225,   205,   225,   225,   225,
     203,   196,   126,   158,    18,    94,    95,    96,    97,    98,
     101,   102,   103,   104,   106,   107,   108,   110,   112,   113,
     134,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   155,   156,   157,   158,   192,    40,   227,   162,   230,
     126,   252,   204,   222,   225,   252,   204,   225,   162,   252,
     140,   140,   140,   232,   158,   126,   225,   252,   252,   225,
     252,   159,   225,   252,   247,   126,   158,   225,   254,   255,
     257,   258,   259,   260,   158,   255,   260,   116,   162,   164,
     129,   130,   170,   159,     1,     7,     8,    10,    22,    24,
      25,    34,    38,    45,    47,    51,    52,    85,   162,   164,
     182,   183,   185,   186,   187,   188,   202,   204,   206,   208,
     209,   210,   211,   212,   214,   215,   222,   225,   197,   156,
     159,   218,   225,    16,   225,   225,   225,   225,   225,   225,
     126,   225,   126,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   126,   225,
     159,   218,   157,   235,   234,   109,    86,    87,    88,    89,
      90,    91,    92,    93,    99,   100,   105,   111,   114,   115,
     133,   162,   236,   162,   140,   159,   218,    16,   139,   225,
     159,   140,   140,   159,   140,   159,   140,    17,   234,   159,
     132,   157,   162,   132,   157,   162,   159,   164,   164,   225,
     258,   164,   225,   225,   225,   247,   105,   162,   204,   225,
     201,   225,   225,   162,   120,    51,   225,   105,   204,   225,
      33,   162,   162,   162,   162,   162,    17,   213,   162,   198,
     126,   254,   132,   159,   252,   158,   135,   157,   159,   225,
     220,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   204,   225,   159,   252,
     126,   159,   225,   225,   225,   158,   218,   225,   255,   126,
     157,   254,   225,   157,   259,   255,   157,   157,   131,   201,
     201,    17,   204,   225,   162,    11,   201,   135,   120,   204,
     225,   162,   163,    36,   237,   116,   201,   219,   157,   225,
     159,   218,   225,   162,   159,   140,   162,   159,   225,   162,
     157,   234,   164,     9,    20,   184,   218,   162,   201,   162,
     203,   225,   159,   159,    16,   225,   126,   159,   225,   157,
     225,   157,   201,   225,   162,   164,   252,   159,   140,   162,
     264,   201,   201,   159,    16,   225,    46,   157,   184,   252,
     159,   225,   157,   159
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
     242,   243,   243,   244,   244,   245,   246,   247,   247,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   249,   249,   249,   249,
     249,   250,   251,   251,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   253,   253,   253,   253,   254,   254,   255,
     255,   256,   256,   256,   256,   257,   257,   258,   259,   259,
     260,   260,   261,   261,   262,   262,   263,   264,   264,   265
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
       4,     5,     7,     0,     2,     2,     6,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
    case 126: /* "name"  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2847 "ds_parser.cpp"
        break;

    case 170: /* character_sequence  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2853 "ds_parser.cpp"
        break;

    case 171: /* string_constant  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2859 "ds_parser.cpp"
        break;

    case 172: /* string_builder_body  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2865 "ds_parser.cpp"
        break;

    case 173: /* string_builder  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2871 "ds_parser.cpp"
        break;

    case 176: /* require_module_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2877 "ds_parser.cpp"
        break;

    case 182: /* expression_label  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2883 "ds_parser.cpp"
        break;

    case 183: /* expression_goto  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2889 "ds_parser.cpp"
        break;

    case 184: /* expression_else  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2895 "ds_parser.cpp"
        break;

    case 185: /* expression_if_then_else  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2901 "ds_parser.cpp"
        break;

    case 186: /* expression_for_loop  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2907 "ds_parser.cpp"
        break;

    case 187: /* expression_while_loop  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2913 "ds_parser.cpp"
        break;

    case 188: /* expression_with  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2919 "ds_parser.cpp"
        break;

    case 189: /* annotation_argument_value  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2925 "ds_parser.cpp"
        break;

    case 190: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2931 "ds_parser.cpp"
        break;

    case 191: /* annotation_argument  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 2937 "ds_parser.cpp"
        break;

    case 192: /* annotation_argument_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 2943 "ds_parser.cpp"
        break;

    case 193: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2949 "ds_parser.cpp"
        break;

    case 194: /* annotation_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 2955 "ds_parser.cpp"
        break;

    case 195: /* annotation_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2961 "ds_parser.cpp"
        break;

    case 196: /* optional_annotation_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 2967 "ds_parser.cpp"
        break;

    case 197: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2973 "ds_parser.cpp"
        break;

    case 198: /* optional_function_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 2979 "ds_parser.cpp"
        break;

    case 199: /* function_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 2985 "ds_parser.cpp"
        break;

    case 201: /* expression_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2991 "ds_parser.cpp"
        break;

    case 202: /* expression_any  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 2997 "ds_parser.cpp"
        break;

    case 203: /* expressions  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3003 "ds_parser.cpp"
        break;

    case 204: /* expr_pipe  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3009 "ds_parser.cpp"
        break;

    case 205: /* name_in_namespace  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3015 "ds_parser.cpp"
        break;

    case 206: /* expression_delete  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3021 "ds_parser.cpp"
        break;

    case 207: /* expr_new  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3027 "ds_parser.cpp"
        break;

    case 208: /* expression_break  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3033 "ds_parser.cpp"
        break;

    case 209: /* expression_continue  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3039 "ds_parser.cpp"
        break;

    case 210: /* expression_return  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3045 "ds_parser.cpp"
        break;

    case 211: /* expression_yield  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3051 "ds_parser.cpp"
        break;

    case 212: /* expression_try_catch  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3057 "ds_parser.cpp"
        break;

    case 215: /* expression_let  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3063 "ds_parser.cpp"
        break;

    case 216: /* expr_cast  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3069 "ds_parser.cpp"
        break;

    case 217: /* expr_type_info  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3075 "ds_parser.cpp"
        break;

    case 218: /* expr_list  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3081 "ds_parser.cpp"
        break;

    case 219: /* block_or_simple_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3087 "ds_parser.cpp"
        break;

    case 220: /* expr_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3093 "ds_parser.cpp"
        break;

    case 221: /* expr_numeric_const  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3099 "ds_parser.cpp"
        break;

    case 222: /* expr_assign  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3105 "ds_parser.cpp"
        break;

    case 223: /* expr_named_call  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3111 "ds_parser.cpp"
        break;

    case 224: /* expr_method_call  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3117 "ds_parser.cpp"
        break;

    case 225: /* expr  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3123 "ds_parser.cpp"
        break;

    case 226: /* optional_field_annotation  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3129 "ds_parser.cpp"
        break;

    case 228: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3135 "ds_parser.cpp"
        break;

    case 229: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3141 "ds_parser.cpp"
        break;

    case 230: /* function_argument_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3147 "ds_parser.cpp"
        break;

    case 231: /* function_argument_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3153 "ds_parser.cpp"
        break;

    case 232: /* tuple_type  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3159 "ds_parser.cpp"
        break;

    case 233: /* tuple_type_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3165 "ds_parser.cpp"
        break;

    case 235: /* variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3171 "ds_parser.cpp"
        break;

    case 237: /* let_variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3177 "ds_parser.cpp"
        break;

    case 238: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3183 "ds_parser.cpp"
        break;

    case 241: /* enum_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3189 "ds_parser.cpp"
        break;

    case 244: /* optional_structure_parent  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3195 "ds_parser.cpp"
        break;

    case 247: /* variable_name_list  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3201 "ds_parser.cpp"
        break;

    case 250: /* structure_type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3207 "ds_parser.cpp"
        break;

    case 251: /* auto_type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3213 "ds_parser.cpp"
        break;

    case 252: /* type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3219 "ds_parser.cpp"
        break;

    case 253: /* make_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3225 "ds_parser.cpp"
        break;

    case 254: /* make_struct_fields  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3231 "ds_parser.cpp"
        break;

    case 255: /* make_struct_dim  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3237 "ds_parser.cpp"
        break;

    case 256: /* make_struct_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3243 "ds_parser.cpp"
        break;

    case 257: /* make_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3249 "ds_parser.cpp"
        break;

    case 258: /* make_map_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3255 "ds_parser.cpp"
        break;

    case 259: /* make_any_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3261 "ds_parser.cpp"
        break;

    case 260: /* make_dim  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3267 "ds_parser.cpp"
        break;

    case 261: /* make_dim_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3273 "ds_parser.cpp"
        break;

    case 262: /* make_table  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3279 "ds_parser.cpp"
        break;

    case 263: /* make_table_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3285 "ds_parser.cpp"
        break;

    case 264: /* array_comprehension_where  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3291 "ds_parser.cpp"
        break;

    case 265: /* array_comprehension  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3297 "ds_parser.cpp"
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
#line 383 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3597 "ds_parser.cpp"
    break;

  case 14:
#line 395 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3603 "ds_parser.cpp"
    break;

  case 15:
#line 396 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3609 "ds_parser.cpp"
    break;

  case 16:
#line 400 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3615 "ds_parser.cpp"
    break;

  case 17:
#line 404 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3624 "ds_parser.cpp"
    break;

  case 18:
#line 408 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3638 "ds_parser.cpp"
    break;

  case 19:
#line 417 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3648 "ds_parser.cpp"
    break;

  case 20:
#line 425 "ds_parser.ypp"
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
#line 3666 "ds_parser.cpp"
    break;

  case 21:
#line 441 "ds_parser.ypp"
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
#line 3689 "ds_parser.cpp"
    break;

  case 23:
#line 466 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3697 "ds_parser.cpp"
    break;

  case 24:
#line 469 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3708 "ds_parser.cpp"
    break;

  case 25:
#line 475 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3719 "ds_parser.cpp"
    break;

  case 26:
#line 484 "ds_parser.ypp"
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
#line 3745 "ds_parser.cpp"
    break;

  case 27:
#line 505 "ds_parser.ypp"
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
#line 3770 "ds_parser.cpp"
    break;

  case 28:
#line 528 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3776 "ds_parser.cpp"
    break;

  case 29:
#line 529 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3782 "ds_parser.cpp"
    break;

  case 33:
#line 542 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3790 "ds_parser.cpp"
    break;

  case 34:
#line 545 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3798 "ds_parser.cpp"
    break;

  case 35:
#line 551 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3806 "ds_parser.cpp"
    break;

  case 36:
#line 557 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3814 "ds_parser.cpp"
    break;

  case 37:
#line 560 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3822 "ds_parser.cpp"
    break;

  case 38:
#line 566 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 3828 "ds_parser.cpp"
    break;

  case 39:
#line 567 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3834 "ds_parser.cpp"
    break;

  case 40:
#line 568 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3843 "ds_parser.cpp"
    break;

  case 41:
#line 575 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3852 "ds_parser.cpp"
    break;

  case 42:
#line 582 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3866 "ds_parser.cpp"
    break;

  case 43:
#line 594 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3878 "ds_parser.cpp"
    break;

  case 44:
#line 604 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3889 "ds_parser.cpp"
    break;

  case 45:
#line 613 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3895 "ds_parser.cpp"
    break;

  case 46:
#line 614 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 3901 "ds_parser.cpp"
    break;

  case 47:
#line 615 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 3907 "ds_parser.cpp"
    break;

  case 48:
#line 616 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 3913 "ds_parser.cpp"
    break;

  case 49:
#line 617 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 3919 "ds_parser.cpp"
    break;

  case 50:
#line 618 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 3925 "ds_parser.cpp"
    break;

  case 51:
#line 622 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 3935 "ds_parser.cpp"
    break;

  case 52:
#line 627 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3945 "ds_parser.cpp"
    break;

  case 53:
#line 635 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3951 "ds_parser.cpp"
    break;

  case 54:
#line 636 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3957 "ds_parser.cpp"
    break;

  case 55:
#line 637 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3963 "ds_parser.cpp"
    break;

  case 56:
#line 638 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3969 "ds_parser.cpp"
    break;

  case 57:
#line 639 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3975 "ds_parser.cpp"
    break;

  case 58:
#line 640 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3981 "ds_parser.cpp"
    break;

  case 59:
#line 641 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 3987 "ds_parser.cpp"
    break;

  case 60:
#line 642 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 3995 "ds_parser.cpp"
    break;

  case 61:
#line 648 "ds_parser.ypp"
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
#line 4013 "ds_parser.cpp"
    break;

  case 62:
#line 661 "ds_parser.ypp"
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
#line 4031 "ds_parser.cpp"
    break;

  case 63:
#line 677 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4037 "ds_parser.cpp"
    break;

  case 64:
#line 678 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4043 "ds_parser.cpp"
    break;

  case 65:
#line 682 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4055 "ds_parser.cpp"
    break;

  case 66:
#line 689 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4069 "ds_parser.cpp"
    break;

  case 67:
#line 701 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4078 "ds_parser.cpp"
    break;

  case 68:
#line 705 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4087 "ds_parser.cpp"
    break;

  case 69:
#line 712 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4093 "ds_parser.cpp"
    break;

  case 70:
#line 713 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4099 "ds_parser.cpp"
    break;

  case 71:
#line 717 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4105 "ds_parser.cpp"
    break;

  case 72:
#line 718 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4111 "ds_parser.cpp"
    break;

  case 73:
#line 719 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4117 "ds_parser.cpp"
    break;

  case 74:
#line 723 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4126 "ds_parser.cpp"
    break;

  case 75:
#line 727 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4135 "ds_parser.cpp"
    break;

  case 76:
#line 734 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4144 "ds_parser.cpp"
    break;

  case 77:
#line 738 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4150 "ds_parser.cpp"
    break;

  case 78:
#line 739 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4156 "ds_parser.cpp"
    break;

  case 79:
#line 740 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4162 "ds_parser.cpp"
    break;

  case 80:
#line 741 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4168 "ds_parser.cpp"
    break;

  case 81:
#line 742 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4174 "ds_parser.cpp"
    break;

  case 82:
#line 743 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4180 "ds_parser.cpp"
    break;

  case 83:
#line 744 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4186 "ds_parser.cpp"
    break;

  case 84:
#line 745 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4192 "ds_parser.cpp"
    break;

  case 85:
#line 746 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4198 "ds_parser.cpp"
    break;

  case 86:
#line 747 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4204 "ds_parser.cpp"
    break;

  case 87:
#line 748 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4210 "ds_parser.cpp"
    break;

  case 88:
#line 749 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4216 "ds_parser.cpp"
    break;

  case 89:
#line 750 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4222 "ds_parser.cpp"
    break;

  case 90:
#line 751 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4228 "ds_parser.cpp"
    break;

  case 91:
#line 752 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4234 "ds_parser.cpp"
    break;

  case 92:
#line 753 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4240 "ds_parser.cpp"
    break;

  case 93:
#line 754 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4246 "ds_parser.cpp"
    break;

  case 94:
#line 755 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4252 "ds_parser.cpp"
    break;

  case 95:
#line 756 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4258 "ds_parser.cpp"
    break;

  case 96:
#line 757 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4264 "ds_parser.cpp"
    break;

  case 97:
#line 758 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4270 "ds_parser.cpp"
    break;

  case 98:
#line 759 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4276 "ds_parser.cpp"
    break;

  case 99:
#line 760 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4282 "ds_parser.cpp"
    break;

  case 100:
#line 761 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4288 "ds_parser.cpp"
    break;

  case 101:
#line 762 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4294 "ds_parser.cpp"
    break;

  case 102:
#line 763 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4300 "ds_parser.cpp"
    break;

  case 103:
#line 764 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4306 "ds_parser.cpp"
    break;

  case 104:
#line 765 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4312 "ds_parser.cpp"
    break;

  case 105:
#line 766 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4318 "ds_parser.cpp"
    break;

  case 106:
#line 767 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4324 "ds_parser.cpp"
    break;

  case 107:
#line 768 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4330 "ds_parser.cpp"
    break;

  case 108:
#line 769 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4336 "ds_parser.cpp"
    break;

  case 109:
#line 770 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4342 "ds_parser.cpp"
    break;

  case 110:
#line 771 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4348 "ds_parser.cpp"
    break;

  case 111:
#line 772 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4354 "ds_parser.cpp"
    break;

  case 112:
#line 773 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4360 "ds_parser.cpp"
    break;

  case 113:
#line 778 "ds_parser.ypp"
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
#line 4431 "ds_parser.cpp"
    break;

  case 114:
#line 847 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4439 "ds_parser.cpp"
    break;

  case 115:
#line 850 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4451 "ds_parser.cpp"
    break;

  case 116:
#line 860 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4457 "ds_parser.cpp"
    break;

  case 117:
#line 861 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4463 "ds_parser.cpp"
    break;

  case 118:
#line 862 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4469 "ds_parser.cpp"
    break;

  case 119:
#line 863 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4475 "ds_parser.cpp"
    break;

  case 120:
#line 864 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4481 "ds_parser.cpp"
    break;

  case 121:
#line 865 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4487 "ds_parser.cpp"
    break;

  case 122:
#line 866 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4493 "ds_parser.cpp"
    break;

  case 123:
#line 867 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4499 "ds_parser.cpp"
    break;

  case 124:
#line 868 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4505 "ds_parser.cpp"
    break;

  case 125:
#line 869 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4511 "ds_parser.cpp"
    break;

  case 126:
#line 870 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4517 "ds_parser.cpp"
    break;

  case 127:
#line 871 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4523 "ds_parser.cpp"
    break;

  case 128:
#line 872 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4529 "ds_parser.cpp"
    break;

  case 129:
#line 873 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4535 "ds_parser.cpp"
    break;

  case 130:
#line 874 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4541 "ds_parser.cpp"
    break;

  case 131:
#line 875 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4547 "ds_parser.cpp"
    break;

  case 132:
#line 876 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4553 "ds_parser.cpp"
    break;

  case 133:
#line 880 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4562 "ds_parser.cpp"
    break;

  case 134:
#line 884 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4573 "ds_parser.cpp"
    break;

  case 135:
#line 890 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4581 "ds_parser.cpp"
    break;

  case 136:
#line 896 "ds_parser.ypp"
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
#line 4597 "ds_parser.cpp"
    break;

  case 137:
#line 910 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4603 "ds_parser.cpp"
    break;

  case 138:
#line 911 "ds_parser.ypp"
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
#line 4619 "ds_parser.cpp"
    break;

  case 139:
#line 922 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4625 "ds_parser.cpp"
    break;

  case 140:
#line 926 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4633 "ds_parser.cpp"
    break;

  case 141:
#line 932 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4641 "ds_parser.cpp"
    break;

  case 142:
#line 935 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4649 "ds_parser.cpp"
    break;

  case 143:
#line 938 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4658 "ds_parser.cpp"
    break;

  case 144:
#line 942 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4666 "ds_parser.cpp"
    break;

  case 145:
#line 948 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4672 "ds_parser.cpp"
    break;

  case 146:
#line 952 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4678 "ds_parser.cpp"
    break;

  case 147:
#line 956 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4686 "ds_parser.cpp"
    break;

  case 148:
#line 959 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4694 "ds_parser.cpp"
    break;

  case 149:
#line 962 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4704 "ds_parser.cpp"
    break;

  case 150:
#line 967 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4712 "ds_parser.cpp"
    break;

  case 151:
#line 970 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4722 "ds_parser.cpp"
    break;

  case 152:
#line 978 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4730 "ds_parser.cpp"
    break;

  case 153:
#line 981 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4740 "ds_parser.cpp"
    break;

  case 154:
#line 986 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4748 "ds_parser.cpp"
    break;

  case 155:
#line 989 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4758 "ds_parser.cpp"
    break;

  case 156:
#line 997 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4766 "ds_parser.cpp"
    break;

  case 157:
#line 1003 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4772 "ds_parser.cpp"
    break;

  case 158:
#line 1004 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4778 "ds_parser.cpp"
    break;

  case 159:
#line 1008 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4784 "ds_parser.cpp"
    break;

  case 160:
#line 1009 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4790 "ds_parser.cpp"
    break;

  case 161:
#line 1013 "ds_parser.ypp"
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
#line 4826 "ds_parser.cpp"
    break;

  case 162:
#line 1047 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4834 "ds_parser.cpp"
    break;

  case 163:
#line 1050 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4844 "ds_parser.cpp"
    break;

  case 164:
#line 1055 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4854 "ds_parser.cpp"
    break;

  case 165:
#line 1063 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4863 "ds_parser.cpp"
    break;

  case 166:
#line 1067 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4873 "ds_parser.cpp"
    break;

  case 167:
#line 1072 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4884 "ds_parser.cpp"
    break;

  case 168:
#line 1078 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4893 "ds_parser.cpp"
    break;

  case 169:
#line 1082 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4903 "ds_parser.cpp"
    break;

  case 170:
#line 1087 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4914 "ds_parser.cpp"
    break;

  case 171:
#line 1096 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4922 "ds_parser.cpp"
    break;

  case 172:
#line 1099 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4930 "ds_parser.cpp"
    break;

  case 173:
#line 1105 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4936 "ds_parser.cpp"
    break;

  case 174:
#line 1106 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 4948 "ds_parser.cpp"
    break;

  case 175:
#line 1116 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4958 "ds_parser.cpp"
    break;

  case 176:
#line 1122 "ds_parser.ypp"
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
#line 5011 "ds_parser.cpp"
    break;

  case 177:
#line 1173 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5017 "ds_parser.cpp"
    break;

  case 178:
#line 1174 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5023 "ds_parser.cpp"
    break;

  case 179:
#line 1175 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5029 "ds_parser.cpp"
    break;

  case 180:
#line 1176 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5035 "ds_parser.cpp"
    break;

  case 181:
#line 1177 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5041 "ds_parser.cpp"
    break;

  case 182:
#line 1178 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5047 "ds_parser.cpp"
    break;

  case 183:
#line 1182 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5053 "ds_parser.cpp"
    break;

  case 184:
#line 1183 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5059 "ds_parser.cpp"
    break;

  case 185:
#line 1184 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5065 "ds_parser.cpp"
    break;

  case 186:
#line 1185 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5071 "ds_parser.cpp"
    break;

  case 187:
#line 1186 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5077 "ds_parser.cpp"
    break;

  case 188:
#line 1187 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5083 "ds_parser.cpp"
    break;

  case 189:
#line 1188 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5089 "ds_parser.cpp"
    break;

  case 190:
#line 1189 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5095 "ds_parser.cpp"
    break;

  case 191:
#line 1190 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5101 "ds_parser.cpp"
    break;

  case 192:
#line 1191 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5107 "ds_parser.cpp"
    break;

  case 193:
#line 1192 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5113 "ds_parser.cpp"
    break;

  case 194:
#line 1193 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5119 "ds_parser.cpp"
    break;

  case 195:
#line 1194 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5125 "ds_parser.cpp"
    break;

  case 196:
#line 1195 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5131 "ds_parser.cpp"
    break;

  case 197:
#line 1196 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5137 "ds_parser.cpp"
    break;

  case 198:
#line 1197 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5143 "ds_parser.cpp"
    break;

  case 199:
#line 1201 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5155 "ds_parser.cpp"
    break;

  case 200:
#line 1212 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5165 "ds_parser.cpp"
    break;

  case 201:
#line 1217 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5177 "ds_parser.cpp"
    break;

  case 202:
#line 1227 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5183 "ds_parser.cpp"
    break;

  case 203:
#line 1228 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5189 "ds_parser.cpp"
    break;

  case 204:
#line 1229 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5195 "ds_parser.cpp"
    break;

  case 205:
#line 1230 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5201 "ds_parser.cpp"
    break;

  case 206:
#line 1231 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5207 "ds_parser.cpp"
    break;

  case 207:
#line 1232 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5213 "ds_parser.cpp"
    break;

  case 208:
#line 1233 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5219 "ds_parser.cpp"
    break;

  case 209:
#line 1234 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5225 "ds_parser.cpp"
    break;

  case 210:
#line 1235 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5231 "ds_parser.cpp"
    break;

  case 211:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5237 "ds_parser.cpp"
    break;

  case 212:
#line 1237 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5243 "ds_parser.cpp"
    break;

  case 213:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5249 "ds_parser.cpp"
    break;

  case 214:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5255 "ds_parser.cpp"
    break;

  case 215:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5261 "ds_parser.cpp"
    break;

  case 216:
#line 1241 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5267 "ds_parser.cpp"
    break;

  case 217:
#line 1242 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5273 "ds_parser.cpp"
    break;

  case 218:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5279 "ds_parser.cpp"
    break;

  case 219:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5285 "ds_parser.cpp"
    break;

  case 220:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5291 "ds_parser.cpp"
    break;

  case 221:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5297 "ds_parser.cpp"
    break;

  case 222:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5303 "ds_parser.cpp"
    break;

  case 223:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5309 "ds_parser.cpp"
    break;

  case 224:
#line 1249 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5315 "ds_parser.cpp"
    break;

  case 225:
#line 1250 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5321 "ds_parser.cpp"
    break;

  case 226:
#line 1251 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5327 "ds_parser.cpp"
    break;

  case 227:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5333 "ds_parser.cpp"
    break;

  case 228:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5339 "ds_parser.cpp"
    break;

  case 229:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5345 "ds_parser.cpp"
    break;

  case 230:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5351 "ds_parser.cpp"
    break;

  case 231:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5357 "ds_parser.cpp"
    break;

  case 232:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5363 "ds_parser.cpp"
    break;

  case 233:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5369 "ds_parser.cpp"
    break;

  case 234:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5375 "ds_parser.cpp"
    break;

  case 235:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5381 "ds_parser.cpp"
    break;

  case 236:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5387 "ds_parser.cpp"
    break;

  case 237:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5393 "ds_parser.cpp"
    break;

  case 238:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5399 "ds_parser.cpp"
    break;

  case 239:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5405 "ds_parser.cpp"
    break;

  case 240:
#line 1265 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5414 "ds_parser.cpp"
    break;

  case 241:
#line 1269 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5423 "ds_parser.cpp"
    break;

  case 242:
#line 1273 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5431 "ds_parser.cpp"
    break;

  case 243:
#line 1276 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5439 "ds_parser.cpp"
    break;

  case 244:
#line 1279 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5445 "ds_parser.cpp"
    break;

  case 245:
#line 1280 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5451 "ds_parser.cpp"
    break;

  case 246:
#line 1281 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5457 "ds_parser.cpp"
    break;

  case 247:
#line 1282 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 5463 "ds_parser.cpp"
    break;

  case 248:
#line 1283 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5469 "ds_parser.cpp"
    break;

  case 249:
#line 1284 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5479 "ds_parser.cpp"
    break;

  case 250:
#line 1289 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5489 "ds_parser.cpp"
    break;

  case 251:
#line 1294 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5495 "ds_parser.cpp"
    break;

  case 252:
#line 1295 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5503 "ds_parser.cpp"
    break;

  case 253:
#line 1298 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5511 "ds_parser.cpp"
    break;

  case 254:
#line 1301 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5517 "ds_parser.cpp"
    break;

  case 255:
#line 1302 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5523 "ds_parser.cpp"
    break;

  case 256:
#line 1303 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5529 "ds_parser.cpp"
    break;

  case 257:
#line 1304 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5535 "ds_parser.cpp"
    break;

  case 258:
#line 1305 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5541 "ds_parser.cpp"
    break;

  case 259:
#line 1306 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5547 "ds_parser.cpp"
    break;

  case 260:
#line 1307 "ds_parser.ypp"
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
#line 5562 "ds_parser.cpp"
    break;

  case 261:
#line 1317 "ds_parser.ypp"
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
#line 5577 "ds_parser.cpp"
    break;

  case 262:
#line 1327 "ds_parser.ypp"
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
#line 5613 "ds_parser.cpp"
    break;

  case 263:
#line 1361 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5619 "ds_parser.cpp"
    break;

  case 264:
#line 1362 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5625 "ds_parser.cpp"
    break;

  case 265:
#line 1366 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5631 "ds_parser.cpp"
    break;

  case 266:
#line 1367 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5637 "ds_parser.cpp"
    break;

  case 267:
#line 1371 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5647 "ds_parser.cpp"
    break;

  case 268:
#line 1379 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5655 "ds_parser.cpp"
    break;

  case 269:
#line 1382 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5664 "ds_parser.cpp"
    break;

  case 270:
#line 1389 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5670 "ds_parser.cpp"
    break;

  case 271:
#line 1390 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5676 "ds_parser.cpp"
    break;

  case 272:
#line 1394 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5682 "ds_parser.cpp"
    break;

  case 273:
#line 1395 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5688 "ds_parser.cpp"
    break;

  case 274:
#line 1399 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5696 "ds_parser.cpp"
    break;

  case 275:
#line 1405 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5702 "ds_parser.cpp"
    break;

  case 276:
#line 1406 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5708 "ds_parser.cpp"
    break;

  case 277:
#line 1410 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5714 "ds_parser.cpp"
    break;

  case 278:
#line 1411 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5720 "ds_parser.cpp"
    break;

  case 279:
#line 1415 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5730 "ds_parser.cpp"
    break;

  case 280:
#line 1420 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5738 "ds_parser.cpp"
    break;

  case 281:
#line 1423 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5747 "ds_parser.cpp"
    break;

  case 282:
#line 1427 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5758 "ds_parser.cpp"
    break;

  case 283:
#line 1433 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 5769 "ds_parser.cpp"
    break;

  case 284:
#line 1442 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 5775 "ds_parser.cpp"
    break;

  case 285:
#line 1443 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 5781 "ds_parser.cpp"
    break;

  case 286:
#line 1444 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 5787 "ds_parser.cpp"
    break;

  case 287:
#line 1448 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 5795 "ds_parser.cpp"
    break;

  case 288:
#line 1451 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5805 "ds_parser.cpp"
    break;

  case 289:
#line 1456 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5815 "ds_parser.cpp"
    break;

  case 290:
#line 1461 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 5827 "ds_parser.cpp"
    break;

  case 291:
#line 1468 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 5839 "ds_parser.cpp"
    break;

  case 292:
#line 1478 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5847 "ds_parser.cpp"
    break;

  case 293:
#line 1481 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 5856 "ds_parser.cpp"
    break;

  case 294:
#line 1488 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5862 "ds_parser.cpp"
    break;

  case 295:
#line 1489 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5868 "ds_parser.cpp"
    break;

  case 296:
#line 1493 "ds_parser.ypp"
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
#line 5900 "ds_parser.cpp"
    break;

  case 297:
#line 1523 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5908 "ds_parser.cpp"
    break;

  case 298:
#line 1526 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5922 "ds_parser.cpp"
    break;

  case 299:
#line 1535 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5936 "ds_parser.cpp"
    break;

  case 300:
#line 1547 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5950 "ds_parser.cpp"
    break;

  case 301:
#line 1559 "ds_parser.ypp"
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
#line 5966 "ds_parser.cpp"
    break;

  case 302:
#line 1570 "ds_parser.ypp"
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
#line 5983 "ds_parser.cpp"
    break;

  case 303:
#line 1585 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 5989 "ds_parser.cpp"
    break;

  case 304:
#line 1586 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5995 "ds_parser.cpp"
    break;

  case 305:
#line 1590 "ds_parser.ypp"
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
#line 6036 "ds_parser.cpp"
    break;

  case 306:
#line 1629 "ds_parser.ypp"
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
#line 6104 "ds_parser.cpp"
    break;

  case 307:
#line 1695 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6116 "ds_parser.cpp"
    break;

  case 308:
#line 1702 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6127 "ds_parser.cpp"
    break;

  case 309:
#line 1711 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6133 "ds_parser.cpp"
    break;

  case 310:
#line 1712 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6139 "ds_parser.cpp"
    break;

  case 311:
#line 1713 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6145 "ds_parser.cpp"
    break;

  case 312:
#line 1714 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6151 "ds_parser.cpp"
    break;

  case 313:
#line 1715 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6157 "ds_parser.cpp"
    break;

  case 314:
#line 1716 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6163 "ds_parser.cpp"
    break;

  case 315:
#line 1717 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6169 "ds_parser.cpp"
    break;

  case 316:
#line 1718 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6175 "ds_parser.cpp"
    break;

  case 317:
#line 1719 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6181 "ds_parser.cpp"
    break;

  case 318:
#line 1720 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6187 "ds_parser.cpp"
    break;

  case 319:
#line 1721 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6193 "ds_parser.cpp"
    break;

  case 320:
#line 1722 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6199 "ds_parser.cpp"
    break;

  case 321:
#line 1723 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6205 "ds_parser.cpp"
    break;

  case 322:
#line 1724 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6211 "ds_parser.cpp"
    break;

  case 323:
#line 1725 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6217 "ds_parser.cpp"
    break;

  case 324:
#line 1726 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6223 "ds_parser.cpp"
    break;

  case 325:
#line 1727 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6229 "ds_parser.cpp"
    break;

  case 326:
#line 1728 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6235 "ds_parser.cpp"
    break;

  case 327:
#line 1729 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6241 "ds_parser.cpp"
    break;

  case 328:
#line 1730 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6247 "ds_parser.cpp"
    break;

  case 329:
#line 1731 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6253 "ds_parser.cpp"
    break;

  case 330:
#line 1732 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6259 "ds_parser.cpp"
    break;

  case 331:
#line 1733 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6265 "ds_parser.cpp"
    break;

  case 332:
#line 1734 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6271 "ds_parser.cpp"
    break;

  case 333:
#line 1738 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6277 "ds_parser.cpp"
    break;

  case 334:
#line 1739 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6283 "ds_parser.cpp"
    break;

  case 335:
#line 1740 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6289 "ds_parser.cpp"
    break;

  case 336:
#line 1741 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6295 "ds_parser.cpp"
    break;

  case 337:
#line 1742 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6301 "ds_parser.cpp"
    break;

  case 338:
#line 1743 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6307 "ds_parser.cpp"
    break;

  case 339:
#line 1744 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6313 "ds_parser.cpp"
    break;

  case 340:
#line 1745 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6319 "ds_parser.cpp"
    break;

  case 341:
#line 1749 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6332 "ds_parser.cpp"
    break;

  case 342:
#line 1760 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6341 "ds_parser.cpp"
    break;

  case 343:
#line 1764 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6353 "ds_parser.cpp"
    break;

  case 344:
#line 1774 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6359 "ds_parser.cpp"
    break;

  case 345:
#line 1775 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6365 "ds_parser.cpp"
    break;

  case 346:
#line 1776 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6371 "ds_parser.cpp"
    break;

  case 347:
#line 1777 "ds_parser.ypp"
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
#line 6390 "ds_parser.cpp"
    break;

  case 348:
#line 1791 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6401 "ds_parser.cpp"
    break;

  case 349:
#line 1797 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6410 "ds_parser.cpp"
    break;

  case 350:
#line 1801 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6420 "ds_parser.cpp"
    break;

  case 351:
#line 1806 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6430 "ds_parser.cpp"
    break;

  case 352:
#line 1811 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6440 "ds_parser.cpp"
    break;

  case 353:
#line 1816 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6452 "ds_parser.cpp"
    break;

  case 354:
#line 1823 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6462 "ds_parser.cpp"
    break;

  case 355:
#line 1828 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6471 "ds_parser.cpp"
    break;

  case 356:
#line 1832 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6480 "ds_parser.cpp"
    break;

  case 357:
#line 1836 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6490 "ds_parser.cpp"
    break;

  case 358:
#line 1841 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6500 "ds_parser.cpp"
    break;

  case 359:
#line 1846 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6512 "ds_parser.cpp"
    break;

  case 360:
#line 1853 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6522 "ds_parser.cpp"
    break;

  case 361:
#line 1858 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6533 "ds_parser.cpp"
    break;

  case 362:
#line 1864 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6543 "ds_parser.cpp"
    break;

  case 363:
#line 1869 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6552 "ds_parser.cpp"
    break;

  case 364:
#line 1873 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6562 "ds_parser.cpp"
    break;

  case 365:
#line 1878 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6576 "ds_parser.cpp"
    break;

  case 366:
#line 1887 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6585 "ds_parser.cpp"
    break;

  case 367:
#line 1891 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6595 "ds_parser.cpp"
    break;

  case 368:
#line 1896 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6609 "ds_parser.cpp"
    break;

  case 369:
#line 1905 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6618 "ds_parser.cpp"
    break;

  case 370:
#line 1909 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6628 "ds_parser.cpp"
    break;

  case 371:
#line 1914 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6642 "ds_parser.cpp"
    break;

  case 372:
#line 1923 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6653 "ds_parser.cpp"
    break;

  case 373:
#line 1932 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6659 "ds_parser.cpp"
    break;

  case 374:
#line 1933 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6665 "ds_parser.cpp"
    break;

  case 375:
#line 1934 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6671 "ds_parser.cpp"
    break;

  case 376:
#line 1935 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6677 "ds_parser.cpp"
    break;

  case 377:
#line 1939 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6689 "ds_parser.cpp"
    break;

  case 378:
#line 1946 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6700 "ds_parser.cpp"
    break;

  case 379:
#line 1955 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6710 "ds_parser.cpp"
    break;

  case 380:
#line 1960 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6719 "ds_parser.cpp"
    break;

  case 381:
#line 1967 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6729 "ds_parser.cpp"
    break;

  case 382:
#line 1972 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6740 "ds_parser.cpp"
    break;

  case 383:
#line 1978 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6752 "ds_parser.cpp"
    break;

  case 384:
#line 1985 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 6765 "ds_parser.cpp"
    break;

  case 385:
#line 1996 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6773 "ds_parser.cpp"
    break;

  case 386:
#line 1999 "ds_parser.ypp"
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
#line 6789 "ds_parser.cpp"
    break;

  case 387:
#line 2013 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 6800 "ds_parser.cpp"
    break;

  case 388:
#line 2022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6806 "ds_parser.cpp"
    break;

  case 389:
#line 2023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6812 "ds_parser.cpp"
    break;

  case 390:
#line 2027 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6822 "ds_parser.cpp"
    break;

  case 391:
#line 2032 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6831 "ds_parser.cpp"
    break;

  case 392:
#line 2039 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6841 "ds_parser.cpp"
    break;

  case 393:
#line 2044 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 6854 "ds_parser.cpp"
    break;

  case 394:
#line 2055 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 6864 "ds_parser.cpp"
    break;

  case 395:
#line 2060 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6873 "ds_parser.cpp"
    break;

  case 396:
#line 2067 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 6887 "ds_parser.cpp"
    break;

  case 397:
#line 2079 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 6893 "ds_parser.cpp"
    break;

  case 398:
#line 2080 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6899 "ds_parser.cpp"
    break;

  case 399:
#line 2084 "ds_parser.ypp"
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
#line 6917 "ds_parser.cpp"
    break;


#line 6921 "ds_parser.cpp"

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
#line 2099 "ds_parser.ypp"


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


