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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );
    
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
    DAS_TBOOL = 312,
    DAS_TVOID = 313,
    DAS_TSTRING = 314,
    DAS_TAUTO = 315,
    DAS_TINT = 316,
    DAS_TINT2 = 317,
    DAS_TINT3 = 318,
    DAS_TINT4 = 319,
    DAS_TUINT = 320,
    DAS_TUINT2 = 321,
    DAS_TUINT3 = 322,
    DAS_TUINT4 = 323,
    DAS_TFLOAT = 324,
    DAS_TFLOAT2 = 325,
    DAS_TFLOAT3 = 326,
    DAS_TFLOAT4 = 327,
    DAS_TRANGE = 328,
    DAS_TURANGE = 329,
    DAS_TBLOCK = 330,
    DAS_TINT64 = 331,
    DAS_TUINT64 = 332,
    DAS_TDOUBLE = 333,
    DAS_TFUNCTION = 334,
    DAS_TLAMBDA = 335,
    DAS_TINT8 = 336,
    DAS_TUINT8 = 337,
    DAS_TINT16 = 338,
    DAS_TUINT16 = 339,
    DAS_TTUPLE = 340,
    DAS_TVARIANT = 341,
    DAS_GENERATOR = 342,
    DAS_YIELD = 343,
    ADDEQU = 344,
    SUBEQU = 345,
    DIVEQU = 346,
    MULEQU = 347,
    MODEQU = 348,
    ANDEQU = 349,
    OREQU = 350,
    XOREQU = 351,
    SHL = 352,
    SHR = 353,
    ADDADD = 354,
    SUBSUB = 355,
    LEEQU = 356,
    SHLEQU = 357,
    SHREQU = 358,
    GREQU = 359,
    EQUEQU = 360,
    NOTEQU = 361,
    RARROW = 362,
    LARROW = 363,
    QQ = 364,
    QDOT = 365,
    LPIPE = 366,
    LBPIPE = 367,
    LAPIPE = 368,
    RPIPE = 369,
    CLONEEQU = 370,
    ROTL = 371,
    ROTR = 372,
    ROTLEQU = 373,
    ROTREQU = 374,
    MAPTO = 375,
    BRABRAB = 376,
    BRACBRB = 377,
    CBRCBRB = 378,
    INTEGER = 379,
    LONG_INTEGER = 380,
    UNSIGNED_INTEGER = 381,
    UNSIGNED_LONG_INTEGER = 382,
    FLOAT = 383,
    DOUBLE = 384,
    NAME = 385,
    BEGIN_STRING = 386,
    STRING_CHARACTER = 387,
    END_STRING = 388,
    BEGIN_STRING_EXPR = 389,
    END_STRING_EXPR = 390,
    UNARY_MINUS = 391,
    UNARY_PLUS = 392,
    PRE_INC = 393,
    PRE_DEC = 394,
    POST_INC = 395,
    POST_DEC = 396,
    DEREF = 397,
    COLCOL = 398
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

#line 372 "ds_parser.cpp"

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
#define YYLAST   6346

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  107
/* YYNRULES -- Number of rules.  */
#define YYNRULES  419
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  761

#define YYUNDEFTOK  2
#define YYMAXUTOK   398

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
       2,     2,     2,   153,     2,   170,   168,   149,   142,     2,
     161,   162,   147,   146,   136,   145,   158,   148,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   139,   165,
     143,   137,   144,   138,   169,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   159,     2,   160,   141,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   166,   140,   167,   152,     2,     2,     2,
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
     135,   150,   151,   154,   155,   156,   157,   163,   164
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   382,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   396,   408,   409,   413,   417,   421,   430,
     438,   454,   475,   479,   482,   488,   497,   518,   541,   542,
     546,   550,   551,   555,   558,   564,   570,   573,   579,   580,
     584,   585,   586,   595,   596,   600,   609,   621,   631,   640,
     641,   642,   643,   644,   645,   649,   654,   662,   663,   664,
     665,   666,   667,   668,   669,   675,   688,   704,   705,   709,
     716,   728,   732,   739,   740,   744,   745,   746,   750,   754,
     761,   765,   766,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   804,   874,   877,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   908,   912,
     918,   924,   935,   941,   942,   953,   957,   963,   966,   969,
     973,   979,   983,   987,   990,   993,   998,  1001,  1009,  1012,
    1017,  1020,  1028,  1034,  1035,  1039,  1040,  1044,  1078,  1081,
    1086,  1094,  1098,  1103,  1109,  1113,  1118,  1127,  1130,  1136,
    1137,  1144,  1155,  1156,  1160,  1165,  1217,  1218,  1219,  1220,
    1221,  1222,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1245,  1246,
    1250,  1261,  1266,  1276,  1280,  1287,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,
    1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,
    1335,  1336,  1337,  1338,  1339,  1343,  1347,  1350,  1353,  1354,
    1355,  1356,  1361,  1366,  1367,  1370,  1373,  1377,  1378,  1379,
    1380,  1381,  1382,  1383,  1393,  1403,  1437,  1438,  1442,  1443,
    1447,  1455,  1458,  1465,  1466,  1470,  1471,  1475,  1478,  1487,
    1488,  1492,  1501,  1502,  1507,  1508,  1512,  1517,  1520,  1524,
    1530,  1539,  1540,  1541,  1545,  1546,  1550,  1553,  1558,  1563,
    1571,  1582,  1585,  1592,  1593,  1597,  1627,  1630,  1639,  1651,
    1663,  1674,  1689,  1690,  1694,  1733,  1799,  1806,  1815,  1816,
    1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,  1825,  1826,
    1827,  1828,  1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,
    1837,  1838,  1842,  1843,  1844,  1845,  1846,  1847,  1851,  1862,
    1866,  1876,  1877,  1878,  1879,  1893,  1899,  1903,  1908,  1913,
    1918,  1925,  1930,  1934,  1938,  1943,  1948,  1955,  1960,  1966,
    1971,  1975,  1980,  1989,  1993,  1998,  2007,  2011,  2016,  2025,
    2031,  2040,  2041,  2042,  2043,  2047,  2054,  2063,  2068,  2075,
    2080,  2086,  2092,  2099,  2110,  2113,  2127,  2136,  2137,  2141,
    2146,  2153,  2158,  2169,  2174,  2181,  2193,  2194,  2198,  2212
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"let\"", "\"def\"", "\"while\"", "\"if\"", "\"static_if\"", "\"else\"",
  "\"for\"", "\"catch\"", "\"true\"", "\"false\"", "\"new\"",
  "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"",
  "\"static_elif\"", "\"array\"", "\"return\"", "\"null\"", "\"break\"",
  "\"try\"", "\"options\"", "\"table\"", "\"expect\"", "\"const\"",
  "\"require\"", "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"",
  "\"deref\"", "\"scope\"", "\"typedef\"", "\"with\"", "\"cast\"",
  "\"override\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"shared\"",
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"uint2\"", "\"uint3\"",
  "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"",
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
     385,   386,   387,   388,   389,   390,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     391,   392,   126,    33,   393,   394,   395,   396,    46,    91,
      93,    40,    41,   397,   398,    59,   123,   125,    36,    64,
      35
};
# endif

#define YYPACT_NINF -470

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-470)))

#define YYTABLE_NINF -193

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -470,    35,  -470,  -470,   -92,   -53,   -51,   -36,   -19,  -470,
      59,    -9,  -470,  -470,  -470,  -470,  -470,   214,  -470,   115,
    -470,  -470,  -470,  -470,    85,  -470,   111,   101,   114,  -470,
    -470,    -2,  -470,   -85,   123,  -470,  -470,   100,   151,   121,
    -470,   -46,  -470,   169,    -4,  -470,   120,    30,   -92,   180,
     -53,   181,  -470,   190,   197,  -470,   226,  -470,  5438,   201,
    -470,   -92,    -9,  -470,   193,   170,  6193,   306,   311,  -470,
     184,  -470,  -470,  -470,  -470,  -470,  -470,   217,   104,  -470,
    -470,  -470,  -470,   294,  -470,  -470,  -470,  -470,  -470,  -470,
    -470,  -470,   185,   -94,   212,   218,   221,  -470,  -470,  -470,
     204,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,   229,  -470,  -470,  -470,   230,
     231,  -470,  -470,  -470,  -470,   236,   237,  -470,  -470,  -470,
    -470,  3791,  -470,    33,  -470,  -100,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,   -24,   223,   -91,  -470,   163,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,    63,  -470,  -470,  -104,
    -470,  5438,  5438,  5438,   251,  5364,  5364,  5364,  5512,   253,
    -470,    31,  -470,  -470,  -470,  -470,  2018,  -470,  -470,  -470,
     -34,   255,  -470,  -470,  -470,  -128,  -470,   107,  5438,   220,
    -470,  -470,   136,  -470,  -470,   104,  -470,   -82,  3929,  -470,
     340,   558,   706,   225,   223,   715,   223,  1126,   223,  1403,
      39,  -470,   -88,  3791,   254,  -470,   -87,   256,  -470,   239,
    -470,  -470,  -470,  5290,   235,  -470,   244,   257,   263,   247,
     266,   267,  3929,  3929,   579,  4050,  3929,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,  3929,  3929,  3929,  3929,  3929,  -470,
    3929,  -470,  -470,   228,  -470,  -470,   -78,  -470,  -470,  -470,
     252,  -470,  -470,  -470,  -470,  -470,  5507,   259,  -470,  -470,
    -470,  -470,  -470,   -92,  -470,   370,   248,  -470,  -470,   -20,
    -470,   284,  -470,  5438,  2160,  3791,  -470,  5438,  -470,   -49,
    -470,  -470,  4654,  -470,  5438,  -470,  -470,   278,  -470,   279,
    -470,   280,  -470,  5438,  -470,  5512,  5438,  -470,   253,  -470,
    -470,  1745,  -470,   296,  3929,  5438,  5438,  3929,  5438,  5438,
       9,     9,   255,  1179,   255,  1321,  5802,  -470,    74,    -3,
       9,     9,  -470,     9,     9,  4786,   853,   -75,  -470,  1591,
     184,     2,  3929,  3929,  -470,  -470,  3929,  3929,  3929,  3929,
     297,  3929,   298,  3929,  3929,  3929,  3929,  3929,  3929,  3929,
    3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,   299,  3929,
    -470,  2295,   -45,  -470,   255,  -470,  -470,  -470,   339,   105,
    -470,   318,  5652,   327,  -470,  -470,  -470,  2160,  -470,  1830,
    -470,  -470,  -470,  3791,  -470,  3791,  -470,  2430,  1748,  4854,
    2157,  2377,  4920,  2512,  2647,    14,   -77,   271,  2565,  5802,
     301,   -38,   303,  -470,  -470,    71,    23,  2700,   149,   150,
    3929,  3929,   265,  -470,  3929,   302,  -470,  -470,  3929,  -470,
    -470,   255,  1456,  -470,   220,  3929,  3929,  -470,   275,   319,
    2835,  1883,  -470,   415,   286,   287,  3929,  -470,  -470,  -470,
    -470,  -470,  -470,   291,   292,   293,  -470,  -470,  -470,   442,
    -470,   -90,  -470,  5746,  5364,  -470,   331,  -470,    83,  5970,
     223,  5438,  -470,    65,    65,   243,   243,  6159,  6159,   305,
      -7,  -470,   -61,   -61,    65,    65,  5858,  6026,  6047,  6103,
     243,   243,   219,   219,    -7,    -7,    -7,  -470,  5580,  -470,
      90,  -470,  -470,  3929,  -470,  -470,   105,  3929,  3929,  3929,
    3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,
    3929,  3929,  -470,  2160,  -470,  4126,  -470,  -470,    94,  5438,
     332,  4986,  -470,  3929,  3929,  -470,  3929,   307,  3929,  3929,
    -470,   331,   333,   310,   331,  3929,   312,  3929,  3929,   331,
     313,   314,  5970,  -470,  -470,  5914,  4043,    27,  2160,  -470,
    -470,  4219,   455,  5970,  4043,  -470,   336,   347,  5970,  2160,
    -470,  4312,   321,  -470,  -470,  4043,  -470,  -470,  -470,   441,
     255,  -470,  2970,  3105,   223,  2782,   -55,    91,  3929,  -470,
     -96,  3791,  3240,  3929,  -470,  -470,  5970,  -470,  5970,  5970,
    5970,  5970,  5970,  5970,  5970,  5970,  5970,  5970,  5970,  5970,
    5970,  5970,  5970,  -470,  4405,  -470,  -470,  2917,   -31,  -470,
     243,   243,   243,  3375,   -64,  5970,    98,   -55,  -470,   301,
    5970,  -470,  -470,   -37,   156,  -470,  -470,  -470,  -470,  3929,
    -470,  4498,  -470,   220,  -470,  -470,  -470,  -470,  4591,  -470,
    -470,   245,  -470,  -470,  3929,  3929,   345,  -100,   329,  5970,
    3510,  -470,  -470,  -470,    97,  5970,  -470,  -470,  3652,   350,
    -470,  5052,  3929,   335,  3929,  3929,   341,    41,  -470,  -470,
    -470,  1022,   220,  -470,  -470,  3929,  -470,   679,   679,  -100,
    -470,  -470,  3929,  5970,  -470,  5438,  5118,   349,  -470,  4723,
    -470,  5970,  4723,  -470,   220,  -470,  -470,  4043,  -470,  5970,
    3322,  -470,  3794,   452,   342,   337,  -470,   245,  -470,  5438,
    5184,  3929,   346,   348,  -470,  3457,  -470,  5970,  -470,  -470,
    -470
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    73,     1,   165,     0,     0,     0,     0,     0,   166,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   313,
       6,    11,     5,     4,    63,    65,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    68,   143,     0,    69,
      71,     0,    67,     0,     0,   314,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   316,     0,     0,
     145,     0,     0,    74,   322,     0,     0,     0,     0,    80,
      75,   311,    61,    62,    59,    60,    58,     0,     0,    57,
      66,    34,    32,    28,    25,    24,   352,   355,   353,   356,
     354,   357,     0,     0,     0,     0,     0,   328,   348,   329,
     359,   330,   334,   335,   336,   337,   341,   342,   343,   344,
     345,   346,   347,   349,   350,   380,   333,   340,   351,   383,
     386,   331,   338,   332,   339,     0,     0,   358,   361,   363,
     362,   319,   144,     0,    72,     0,   324,   281,    83,    84,
      86,    85,    87,    88,    89,    90,   109,   110,   107,   108,
     100,   111,   112,   101,    98,    99,   113,   114,   115,   116,
     103,   104,   102,    96,    97,    92,    91,    93,    94,    95,
      82,    81,   105,   106,     0,    78,     0,    14,     0,    53,
      54,    51,    52,    50,    49,    55,     0,    27,   316,     0,
     320,     0,     0,     0,     0,    75,    75,    75,     0,     0,
     367,     0,   373,   376,   375,   369,     0,   372,    70,   323,
     276,     0,   326,    76,   285,     0,   283,   296,     0,     0,
     315,   312,   304,    15,    16,     0,    64,     0,     0,   317,
       0,     0,     0,     0,    78,     0,    78,     0,    78,     0,
     143,   289,     0,   287,     0,   292,     0,   368,   371,     0,
     374,   221,   222,     0,     0,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   188,   187,
     189,   190,   191,    17,     0,     0,     0,     0,     0,   365,
       0,   138,   182,   183,   219,   184,   217,   269,   268,   267,
      73,   272,   218,   271,   270,   253,     0,     0,   220,   391,
     392,   393,   394,     0,   325,   278,     0,   284,    77,     0,
     295,     0,   294,     0,     0,    79,   117,     0,   305,     0,
      56,   321,     0,   377,     0,   379,   360,     0,   381,     0,
     384,     0,   387,     0,   389,     0,     0,   390,     0,   370,
     366,   147,   150,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,     0,     0,     0,     0,   413,     0,     0,
     226,   225,   258,   224,   223,     0,     0,     0,   275,     0,
      75,     0,     0,     0,   247,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,   279,     0,   282,   286,   327,   297,     0,
     300,     0,   299,     0,   302,   303,   301,     0,   318,     0,
     382,   385,   388,   288,   290,   291,   293,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,   404,
     397,     0,   407,   408,   409,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    18,   249,   140,     0,    43,
      44,     0,     0,   151,     0,     0,     0,   152,     0,     0,
       0,     0,   120,   118,     0,     0,     0,   133,   128,   126,
     127,   139,   121,     0,     0,     0,   131,   132,   134,   163,
     125,     0,   122,   192,    75,   213,     0,   254,     0,   177,
      78,     0,   266,   227,   228,   240,   241,   238,   239,     0,
     263,   252,   273,   274,   229,   230,     0,   243,   244,   242,
     236,   237,   232,   231,   233,   234,   235,   251,     0,   256,
       0,   277,   280,     0,   183,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   306,     0,   310,   192,   378,   148,     0,     0,
       0,     0,   259,     0,     0,   260,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   414,   415,     0,     0,     0,     0,   153,
     156,   192,     0,   146,     0,   137,     0,     0,    37,     0,
     160,   192,     0,   135,   136,     0,   124,   129,   130,     0,
       0,   123,     0,     0,    78,     0,     0,     0,     0,   255,
       0,   265,     0,     0,   250,   257,   298,   141,   199,   200,
     202,   201,   203,   196,   197,   198,   204,   205,   194,   195,
     206,   207,   193,   308,   192,   309,   149,     0,     0,   171,
     168,   169,   170,     0,     0,   395,     0,     0,   399,   398,
     405,   411,   410,     0,     0,   402,   412,    19,    47,     0,
     157,   192,   154,     0,    48,    35,    36,   161,   192,   158,
     138,    40,   164,   167,     0,     0,     0,     0,     0,   178,
       0,   179,   185,   211,     0,   264,   307,   174,     0,     0,
     261,     0,     0,     0,     0,     0,     0,     0,   155,   162,
     159,     0,     0,    38,    39,     0,    45,   209,   208,     0,
     214,   210,     0,   180,   212,     0,     0,     0,   262,   416,
     401,   396,   416,   403,     0,   119,    41,     0,   215,   181,
       0,   172,     0,     0,     0,     0,    46,    40,   175,     0,
       0,     0,     0,     0,    42,     0,   173,   417,   418,   419,
     176
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -470,  -470,  -470,   146,   468,  -470,  -470,  -470,  -470,  -470,
    -470,   433,  -470,  -470,   471,  -470,  -470,  -470,  -224,  -470,
    -470,  -470,  -470,  -470,   316,  -470,   491,   -54,  -470,   480,
    -470,   260,  -182,  -230,  -470,  -470,  -161,  -470,  -137,  -305,
     -11,  -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,   543,
    -470,  -470,  -470,  -385,  -470,  -470,  -383,  -470,  -363,  -470,
    -470,  -470,  -470,   141,  -470,  -470,  -470,  -470,   238,  -470,
     210,  -470,   208,  -470,  -216,  -200,   139,  -470,   -57,  -470,
    -470,  -470,   361,  -470,  -470,  -470,  -470,  -470,  -156,   -56,
    -470,  -470,  -470,   -48,   304,  -469,  -350,  -470,  -470,  -249,
     -23,   200,  -470,  -470,  -470,  -176,  -470
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   178,   184,   359,   284,    14,    15,    31,
      32,    55,    16,    28,    29,   474,   475,   715,   716,   476,
     477,   478,   479,   480,   185,   186,    25,    26,    39,    40,
      41,    17,   175,   219,    70,    18,   285,   481,   366,   482,
     286,   483,   287,   484,   485,   486,   487,   488,   610,   489,
     490,   288,   289,   498,   692,   290,   291,   292,   411,   492,
     293,   294,   295,   499,   305,   404,   306,   210,   214,   215,
     241,   242,   245,   246,   569,   216,   417,   319,   221,   176,
      46,    20,    93,    21,    22,   136,    65,    23,   217,   297,
      92,   129,   130,   243,   298,   440,   441,   299,   442,   443,
     444,   445,   300,   358,   301,   744,   302
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   314,   128,   491,   327,   448,   329,   133,   331,   410,
     131,   307,   371,   234,   236,   238,   530,   357,    51,   501,
     222,   211,   536,    36,   690,   211,   535,   617,   371,    66,
      37,   310,   568,   228,   308,     2,   189,   309,    24,   212,
       3,   578,   558,    72,    73,   669,   380,   127,   189,   382,
      52,    42,   368,   310,    56,    37,   334,   337,   316,   414,
     312,   229,   247,     4,    38,     5,   415,     6,   494,     7,
     281,    27,   618,   190,     8,   611,   220,   335,   338,    30,
       9,    57,   312,   369,   371,   321,    10,    59,   416,    38,
      62,    48,   374,   375,    33,    67,    68,   398,   399,   618,
     380,   702,   381,   382,   383,   659,   212,   384,   374,   375,
     212,    34,   554,   698,    63,   531,   380,   179,   180,   382,
     383,    37,   573,   384,   209,   303,    69,   574,   705,   177,
     453,   454,   502,   304,   699,   128,   128,   128,   213,   128,
     128,   128,   128,   230,   231,   232,    53,   235,   237,   239,
     311,   398,   399,   627,    74,    38,    54,   590,    75,   311,
      76,    77,   128,   311,   374,   375,   600,   398,   399,    48,
     315,    45,   380,   248,   381,   382,   383,   618,   333,   384,
     127,   127,   127,   654,   127,   127,   127,   127,   500,    35,
     249,    78,   533,   663,    11,   208,   435,   128,   446,   225,
      12,   250,   583,    59,   532,   341,   734,   127,   128,   128,
     393,   394,   395,   396,   397,   310,   353,   355,    43,   618,
      44,   656,    47,   398,   399,   226,   618,   572,   181,   664,
     618,   576,   182,   618,   183,    77,   577,   694,   371,   451,
      49,   452,   127,   311,   312,   619,   313,    48,   643,   402,
      50,   688,   625,   127,   127,   712,   646,   128,   703,   724,
      58,   128,   371,   574,    59,   408,   713,   714,   128,   413,
     620,   281,   311,   282,   534,   317,   419,   128,   318,   128,
     128,    60,    61,   670,   707,   423,    71,    86,   425,   128,
     128,    87,   128,   128,   677,   223,   224,   430,   431,    64,
     433,   434,   127,   592,    81,   587,   127,    88,    89,    90,
      91,    83,   614,   127,   574,   577,   580,   581,   374,   375,
      84,   574,   127,   706,   127,   127,   380,    85,   381,   382,
     383,   132,   135,   384,   127,   127,   137,   127,   127,   172,
     372,   373,   374,   375,   173,   174,    52,   296,   491,   177,
     380,   188,   381,   382,   383,   191,   495,   384,   200,   385,
     386,   192,   218,   201,   193,   194,   395,   396,   397,   322,
     200,   200,   195,   196,   197,   201,   201,   398,   399,   198,
     199,   233,   202,   244,   686,   212,   281,   326,   393,   394,
     395,   396,   397,   336,   202,   202,   343,   367,   339,   340,
     345,   398,   399,   350,   351,   344,   346,   356,   347,   348,
     349,    11,   403,   405,   407,   360,   361,   362,   363,   364,
     401,   365,   420,   421,   422,   668,   428,   509,   511,   527,
     536,   570,   584,   674,   223,   414,   203,   572,   128,   575,
     595,   704,   415,   596,   681,   128,   615,   310,   203,   203,
     602,   603,   604,   621,   222,   412,   606,   607,   608,   691,
     609,   616,   648,   657,   416,   204,   622,   673,   653,   205,
     658,   676,   661,   665,   666,   675,   312,   204,   204,   682,
     727,   205,   205,   127,   323,   429,   206,   680,   432,   719,
     127,   721,   552,   742,   439,   730,   439,   207,   206,   206,
     751,   733,   752,   128,   753,   455,   758,   493,   759,   207,
     207,   647,   709,   503,   504,    79,   187,   505,   506,   507,
     508,    82,   510,   754,   512,   513,   514,   515,   516,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,    80,
     528,   320,   134,   711,    19,   424,   426,   406,   127,   227,
     370,   736,   553,   683,   662,   449,   745,   342,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
       0,     0,     0,   746,     0,     0,   747,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,   365,   200,
     352,   582,   356,     0,   201,   585,     0,     0,     0,   586,
       0,     0,    94,   591,     0,     0,   593,   594,    95,     0,
       0,   598,   601,   202,     0,     0,     0,   605,     0,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,     0,   203,     0,   128,
       0,     0,     0,     0,   626,     0,   720,   740,   628,   629,
     630,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   128,   644,     0,   204,     0,   371,     0,
     205,   755,     0,     0,   650,   651,     0,   652,   738,    37,
     655,     0,     0,     0,   127,     0,   660,   206,   439,     0,
       0,     0,     0,   324,     0,     0,     0,     0,   207,   671,
       0,     0,     0,     0,     0,     0,     0,   200,   127,     0,
     678,     0,   201,    38,     0,     0,   200,     0,     0,     0,
       0,   201,     0,   638,   642,     0,     0,     0,     0,   689,
       0,   202,     0,     0,   695,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,   372,   373,     0,     0,
     376,     0,     0,   377,   378,   379,   380,     0,   381,   382,
     383,     0,     0,   384,   701,   385,   386,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,   387,     0,   388,
     389,   390,   391,   392,   203,   717,   718,   396,   397,     0,
       0,   723,     0,     0,     0,     0,     0,   398,   399,   726,
       0,     0,     0,   729,   204,   731,   732,     0,   205,     0,
     325,     0,   493,   204,   457,     0,   737,   205,     3,   328,
     458,   459,   460,   739,   461,   206,   251,   252,   253,   254,
       0,     0,     0,     0,   206,     0,   207,   462,   255,   463,
     464,     0,     0,   750,     0,   207,     0,     0,     0,   465,
     256,     0,   757,   466,   257,     0,   258,     0,     9,   259,
     467,     0,   468,   260,     0,     0,   469,   470,     0,     0,
      97,    98,    99,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   116,
     117,   118,     0,     0,   121,   122,   123,   124,     0,     0,
     261,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,     0,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    37,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,   275,
     276,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,     0,     0,   280,     0,     0,    38,   472,   281,
     473,   282,   283,   457,     0,     0,     0,     3,     0,   458,
     459,   460,     0,   461,     0,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,   462,   255,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,   465,   256,
       0,     0,   466,   257,     0,   258,     0,     9,   259,   467,
       0,   468,   260,     0,     0,   469,   470,     0,     0,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,     0,     0,   261,
     471,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,     0,     0,
       0,     0,     0,   264,   265,   266,   267,   268,   269,   270,
     271,   272,    37,   273,     0,     0,     0,   200,     0,     0,
       0,     0,   201,     0,     0,     0,     0,   274,   275,   276,
       0,     0,     0,     0,   277,   278,     0,     0,     0,     0,
       0,   202,     0,   280,     0,     0,    38,   472,   281,   735,
     282,   283,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,     0,     0,
     200,     0,     0,     0,     0,   201,   256,     0,     0,     0,
     257,     0,   258,     0,     0,   259,     0,     0,     0,   260,
       0,     0,     0,     0,   202,   203,    97,    98,    99,     0,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,   116,   117,   118,     0,     0,
     121,   122,   123,   124,   204,     0,   261,     0,   205,     0,
     330,     0,     0,     0,     0,     0,     0,     0,   262,   263,
       0,     0,     0,     0,     0,   206,     0,     0,   203,     0,
       0,     0,     0,     0,     0,     0,   207,     0,     0,     0,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   436,
     273,     0,     0,     0,     0,     0,     0,   204,     0,     0,
       0,   205,     0,     0,   274,   275,   276,     0,     0,     0,
       0,   277,   278,     0,   251,   252,   253,   254,   206,   437,
     438,     0,     0,    38,     0,   281,   255,   282,   283,   207,
       0,     0,   200,     0,     0,     0,     0,   201,   256,     0,
       0,     0,   257,     0,   258,     0,     0,   259,     0,     0,
       0,   260,     0,     0,     0,     0,   202,     0,    97,    98,
      99,     0,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,     0,   116,   117,   118,
       0,     0,   121,   122,   123,   124,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,     0,     0,     0,     0,     0,     0,     0,     0,
     203,     0,     0,     0,   200,     0,     0,     0,     0,   201,
       0,     0,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   436,   273,     0,     0,     0,     0,     0,   202,   204,
       0,     0,     0,   205,     0,     0,   274,   275,   276,   251,
     252,   253,   254,   277,   278,     0,     0,     0,     0,     0,
     206,   255,   447,     0,     0,    38,     0,   281,     0,   282,
     283,   207,     0,   256,     0,     0,     0,   257,     0,   258,
       0,     0,   259,     0,     0,     0,   260,     0,     0,     0,
       0,     0,   203,    97,    98,    99,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,   116,   117,   118,     0,     0,   121,   122,   123,
     124,   204,     0,   261,     0,   205,     0,   332,     0,     0,
       0,     0,     0,     0,     0,   262,   263,     0,     0,     0,
       0,     0,   206,     0,   588,     0,     0,     0,     0,   409,
       0,     0,     0,   207,     0,     0,     0,   264,   265,   266,
     267,   268,   269,   270,   271,   272,    37,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   251,   252,   253,   254,   277,   278,
       0,     0,     0,     0,     0,     0,   255,   280,     0,     0,
      38,   589,   281,     0,   282,   283,     0,     0,   256,     0,
       0,     0,   257,     0,   258,     0,     0,   259,     0,     0,
       0,   260,     0,     0,     0,     0,     0,     0,    97,    98,
      99,     0,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,     0,   116,   117,   118,
       0,     0,   121,   122,   123,   124,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,   265,   266,   267,   268,   269,   270,   271,
     272,    37,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   275,   276,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
     496,     0,   280,   497,     0,    38,     0,   281,     0,   282,
     283,   251,   252,   253,   254,   559,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,   200,     0,     0,     0,
       0,   201,     0,     0,     0,   256,     0,     0,     0,   257,
       0,   258,     0,     0,   259,     0,     0,     0,   260,     0,
     202,     0,     0,     0,     0,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,     0,
       0,     0,     0,     0,   203,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,   201,     0,     0,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    37,   273,
       0,     0,     0,   204,     0,   202,     0,   205,     0,     0,
       0,   560,     0,   274,   275,   276,   251,   252,   253,   254,
     277,   278,     0,     0,   206,     0,   427,     0,   255,   280,
       0,     0,    38,     0,   281,   207,   282,   283,     0,     0,
     256,     0,     0,     0,   257,     0,   258,     0,     0,   259,
       0,     0,     0,   260,     0,     0,     0,     0,     0,   203,
      97,    98,    99,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   116,
     117,   118,     0,     0,   121,   122,   123,   124,   204,     0,
     261,     0,   205,     0,   556,     0,     0,     0,     0,     0,
       0,     0,   262,   263,     0,     0,     0,     0,     0,   206,
       0,   599,     0,     0,     0,     0,   409,     0,     0,     0,
     207,     0,     0,     0,   264,   265,   266,   267,   268,   269,
     270,   271,   272,    37,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   251,   252,   253,   254,   277,   278,     0,     0,     0,
       0,     0,     0,   255,   280,     0,     0,    38,     0,   281,
       0,   282,   283,     0,     0,   256,     0,     0,     0,   257,
       0,   258,     0,     0,   259,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,    97,    98,    99,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,   116,   117,   118,     0,     0,   121,
     122,   123,   124,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    37,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   275,   276,     0,     0,     0,     0,
     277,   278,     0,   251,   252,   253,   254,     0,   279,   280,
       0,     0,    38,     0,   281,   255,   282,   283,   200,     0,
       0,     0,     0,   201,     0,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,   202,     0,     0,     0,     0,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,   203,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,   204,     0,     0,     0,   205,
       0,   563,     0,     0,     0,   274,   275,   276,   251,   252,
     253,   254,   277,   278,     0,     0,   206,     0,     0,     0,
     255,   280,     0,     0,    38,     0,   281,   207,   282,   283,
       0,     0,   256,     0,     0,     0,   257,     0,   258,     0,
       0,   259,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,    97,    98,    99,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   116,   117,   118,     0,     0,   121,   122,   123,   124,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   200,     0,
       0,     0,     0,   201,     0,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    37,   273,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   251,   252,   253,   254,   277,   278,     0,
       0,     0,     0,     0,     0,   255,   280,   529,     0,    38,
       0,   281,     0,   282,   283,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   203,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   204,     0,   261,     0,   205,
       0,   564,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,   207,   201,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   251,   252,
     253,   254,   277,   278,     0,     0,     0,     0,     0,     0,
     255,   280,   557,     0,    38,     0,   281,     0,   282,   283,
       0,     0,   256,     0,     0,     0,   257,     0,   258,     0,
       0,   259,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   203,    97,    98,    99,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   116,   117,   118,     0,     0,   121,   122,   123,   124,
     204,     0,   261,     0,   205,     0,   566,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   200,     0,
       0,     0,   207,   201,     0,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    37,   273,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   251,   252,   253,   254,   277,   278,     0,
       0,     0,     0,     0,     0,   255,   280,   571,     0,    38,
       0,   281,     0,   282,   283,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   203,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   204,     0,   261,     0,   205,
       0,   567,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,   206,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,   207,   201,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   251,   252,
     253,   254,   277,   278,     0,     0,     0,     0,     0,     0,
     255,   280,   579,     0,    38,     0,   281,     0,   282,   283,
       0,     0,   256,     0,     0,     0,   257,     0,   258,     0,
       0,   259,     0,     0,     0,   260,     0,     0,   597,     0,
       0,   203,    97,    98,    99,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   116,   117,   118,     0,     0,   121,   122,   123,   124,
     204,     0,   261,     0,   205,     0,   687,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   200,     0,
       0,     0,   207,   201,     0,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    37,   273,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   251,   252,   253,   254,   277,   278,     0,
       0,     0,     0,     0,     0,   255,   280,     0,     0,    38,
       0,   281,     0,   282,   283,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   203,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   204,     0,   261,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,   206,     0,     0,   697,
       0,     0,     0,   684,     0,     0,     0,   207,     0,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   251,   252,
     253,   254,   277,   278,     0,     0,     0,     0,     0,     0,
     255,   280,     0,     0,    38,     0,   281,     0,   282,   283,
       0,     0,   256,     0,     0,     0,   257,     0,   258,     0,
       0,   259,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,    97,    98,    99,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   116,   117,   118,     0,     0,   121,   122,   123,   124,
       0,     0,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   685,     0,
       0,     0,     0,     0,     0,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    37,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   251,   252,   253,   254,   277,   278,     0,
       0,     0,     0,     0,     0,   255,   280,     0,     0,    38,
       0,   281,     0,   282,   283,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   200,     0,     0,     0,     0,   201,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   251,   252,
     253,   254,   277,   278,     0,     0,     0,     0,     0,     0,
     255,   280,   693,     0,    38,     0,   281,     0,   282,   283,
       0,     0,   256,     0,     0,     0,   257,     0,   258,     0,
       0,   259,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   203,    97,    98,    99,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   116,   117,   118,     0,     0,   121,   122,   123,   124,
     204,     0,   261,     0,   205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,   206,     0,     0,   748,     0,     0,     0,   200,     0,
       0,     0,   207,   201,     0,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,    37,   273,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   251,   252,   253,   254,   277,   278,     0,
       0,     0,     0,     0,     0,   255,   280,   700,     0,    38,
       0,   281,     0,   282,   283,     0,     0,   256,     0,     0,
       0,   257,     0,   258,     0,     0,   259,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   203,    97,    98,    99,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,   116,   117,   118,     0,
       0,   121,   122,   123,   124,   204,     0,   261,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
     263,     0,     0,     0,     0,     0,   206,     0,   722,   760,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
       0,   264,   265,   266,   267,   268,   269,   270,   271,   272,
      37,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,   275,   276,     0,     0,
       0,     0,   277,   278,     0,   251,   252,   253,   254,   725,
       0,   280,     0,     0,    38,     0,   281,   255,   282,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,     0,     0,   257,     0,   258,     0,     0,   259,     0,
       0,     0,   260,     0,     0,     0,     0,     0,     0,    97,
      98,    99,     0,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,   116,   117,
     118,     0,     0,   121,   122,   123,   124,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,   265,   266,   267,   268,   269,   270,
     271,   272,    37,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,   275,   276,
       0,     0,     0,     0,   277,   278,     0,   251,   252,   253,
     254,   749,     0,   280,     0,     0,    38,     0,   281,   255,
     282,   283,   200,     0,     0,     0,     0,   201,     0,     0,
       0,   256,     0,     0,     0,   257,     0,   258,     0,     0,
     259,     0,     0,     0,   260,     0,   202,     0,     0,     0,
       0,    97,    98,    99,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     116,   117,   118,     0,     0,   121,   122,   123,   124,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   263,     0,     0,     0,     0,     0,
     203,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,   265,   266,   267,   268,
     269,   270,   271,   272,    37,   273,     0,     0,     0,   204,
       0,     0,     0,   205,     0,     0,     0,     0,     0,   274,
     275,   276,   251,   252,   253,   254,   277,   278,     0,     0,
     206,     0,     0,     0,   255,   280,     0,     0,    38,     0,
     281,   207,   282,   283,     0,     0,   256,     0,     0,     0,
     257,     0,   258,     0,     0,   259,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,    97,    98,    99,     0,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,   116,   117,   118,     0,     0,
     121,   122,   123,   124,     0,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,   265,   266,   267,   268,   269,   270,   271,   272,    37,
     273,   354,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,   274,   275,   276,     0,     0,    95,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,    38,    96,   281,     0,   282,   283,     0,
       0,     0,     0,     0,     0,     0,     0,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,     0,     0,     0,
     372,   373,   374,   375,   376,   371,     0,   377,   378,   379,
     380,     0,   381,   382,   383,     0,     0,   384,     0,   385,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,   387,     0,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,     0,     0,     0,     0,     0,     0,     0,
       0,   398,   399,     0,     0,     0,     0,     0,     0,   281,
       0,     0,     0,     0,    38,   537,   538,   539,   540,   541,
     542,   543,   544,   372,   373,   374,   375,   376,   545,   546,
     377,   378,   379,   380,   547,   381,   382,   383,   371,     0,
     384,   548,   385,   386,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   387,     0,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,     0,     0,     0,     0,
       0,     0,     0,     0,   398,   399,     0,     0,     0,     0,
       0,   645,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   537,   538,
     539,   540,   541,   542,   543,   544,   372,   373,   374,   375,
     376,   545,   546,   377,   378,   379,   380,   547,   381,   382,
     383,   371,     0,   384,   548,   385,   386,   549,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   551,   387,     0,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,     0,
       0,     0,     0,     0,     0,     0,     0,   398,   399,     0,
       0,     0,     0,     0,   672,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   538,   539,   540,   541,   542,   543,   544,   372,
     373,   374,   375,   376,   545,   546,   377,   378,   379,   380,
     547,   381,   382,   383,   371,     0,   384,   548,   385,   386,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     387,     0,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,     0,     0,     0,     0,     0,     0,     0,     0,
     398,   399,     0,     0,     0,     0,     0,   679,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   537,   538,   539,   540,   541,   542,
     543,   544,   372,   373,   374,   375,   376,   545,   546,   377,
     378,   379,   380,   547,   381,   382,   383,   371,     0,   384,
     548,   385,   386,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   551,   387,     0,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,     0,     0,     0,     0,     0,
       0,     0,     0,   398,   399,     0,     0,     0,     0,     0,
     696,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,   539,
     540,   541,   542,   543,   544,   372,   373,   374,   375,   376,
     545,   546,   377,   378,   379,   380,   547,   381,   382,   383,
     371,     0,   384,   548,   385,   386,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   387,     0,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,     0,     0,
       0,     0,     0,     0,     0,     0,   398,   399,     0,     0,
       0,     0,     0,   708,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,     0,     0,     0,     0,     0,     0,
     537,   538,   539,   540,   541,   542,   543,   544,   372,   373,
     374,   375,   376,   545,   546,   377,   378,   379,   380,   547,
     381,   382,   383,     0,     0,   384,   548,   385,   386,   549,
     550,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   551,   387,
       0,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,     0,   371,     0,     0,     0,     0,     0,     0,   398,
     399,   372,   373,   374,   375,   376,   710,     0,   377,   378,
     379,   380,     0,   381,   382,   383,     0,     0,   384,     0,
     385,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   387,     0,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,     0,   371,     0,     0,     0,     0,
       0,     0,   398,   399,     0,     0,     0,     0,     0,   418,
     372,   373,   374,   375,   376,     0,     0,   377,   378,   379,
     380,     0,   381,   382,   383,     0,     0,   384,     0,   385,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   371,     0,     0,     0,     0,     0,     0,
       0,   398,   399,   372,   373,   374,   375,   376,   743,     0,
     377,   378,   379,   380,     0,   381,   382,   383,     0,     0,
     384,     0,   385,   386,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   387,     0,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,     0,     0,     0,   371,
       0,     0,     0,     0,   398,   399,     0,     0,   456,     0,
       0,   372,   373,   374,   375,   376,     0,     0,   377,   378,
     379,   380,     0,   381,   382,   383,     0,     0,   384,     0,
     385,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   387,     0,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,     0,   371,     0,     0,     0,     0,
       0,     0,   398,   399,     0,     0,   562,   372,   373,   374,
     375,   376,     0,     0,   377,   378,   379,   380,     0,   381,
     382,   383,     0,     0,   384,     0,   385,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   387,     0,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
       0,   371,     0,     0,     0,     0,     0,     0,   398,   399,
       0,     0,   565,   372,   373,   374,   375,   376,     0,     0,
     377,   378,   379,   380,     0,   381,   382,   383,     0,     0,
     384,     0,   385,   386,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   387,     0,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,     0,   371,     0,     0,
       0,     0,     0,     0,   398,   399,     0,     0,   649,   372,
     373,   374,   375,   376,     0,     0,   377,   378,   379,   380,
       0,   381,   382,   383,     0,     0,   384,     0,   385,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     387,     0,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,     0,   371,     0,     0,     0,     0,     0,     0,
     398,   399,     0,     0,   728,   372,   373,   374,   375,   376,
       0,     0,   377,   378,   379,   380,     0,   381,   382,   383,
       0,     0,   384,     0,   385,   386,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   387,     0,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,     0,     0,
       0,     0,     0,     0,     0,     0,   398,   399,     0,     0,
     741,   372,   373,   374,   375,   376,     0,     0,   377,   378,
     379,   380,     0,   381,   382,   383,     0,     0,   384,     0,
     385,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,    95,
       0,     0,   387,     0,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,    96,     0,     0,     0,     0,     0,
       0,     0,   398,   399,     0,     0,   756,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,   264,   265,   266,     0,     0,     0,     0,     0,     0,
      37,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   174,   371,     0,    38,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   371,
       0,     0,    38,     0,   372,   373,   374,   375,   376,     0,
       0,   377,   378,   379,   380,     0,   381,   382,   383,     0,
       0,   384,     0,   385,   386,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   240,     0,     0,   387,     0,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,   398,   399,   400,     0,     0,
       0,   371,     0,     0,     0,     0,    38,   372,   373,   374,
     375,   376,     0,     0,   377,   378,   379,   380,     0,   381,
     382,   383,     0,     0,   384,     0,   385,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   387,     0,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
       0,     0,     0,     0,     0,     0,     0,     0,   398,   399,
     624,   537,   538,   539,   540,   541,   542,   543,   544,   372,
     373,   374,   375,   376,   545,   546,   377,   378,   379,   380,
     547,   381,   382,   383,  -192,   371,   384,   548,   385,   386,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     387,     0,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,     0,     0,     0,     0,     0,     0,     0,     0,
     398,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   537,   538,   539,   540,   541,
     542,   543,   544,   372,   373,   374,   375,   376,   545,   546,
     377,   378,   379,   380,   612,   381,   382,   383,     0,     0,
     384,   548,   385,   386,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,     0,   613,   387,     0,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,     0,     0,     0,   372,
     373,   374,   375,   376,   398,   399,   377,   378,   379,   380,
       0,   381,   382,   383,     0,     0,   384,     0,   385,   386,
       0,     0,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,     0,     0,     0,     0,     0,     0,
     387,     0,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,     0,     0,     0,   372,   373,   374,   375,   376,
     398,   399,   377,   378,   379,   380,     0,   381,   382,   383,
       0,     0,   384,     0,   385,   386,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
       0,     0,     0,     0,     0,     0,   387,   623,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,     0,     0,
       0,   372,   373,   374,   375,   376,   398,   399,   377,   378,
     379,   380,     0,   381,   382,   383,     0,     0,   384,     0,
     385,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,     0,     0,     0,   667,
       0,     0,   387,     0,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   371,   372,   373,   374,
     375,   376,   398,   399,   377,   378,   379,   380,     0,   381,
     382,   383,     0,     0,   384,     0,   385,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   387,     0,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
       0,     0,   371,   372,   373,   374,   375,   376,   398,   399,
     377,   378,   379,   380,     0,   381,   382,   383,     0,     0,
     384,     0,   385,   386,   372,   373,   374,   375,   376,     0,
       0,   377,   378,   379,   380,     0,   381,   382,   383,     0,
       0,   384,     0,   385,   386,     0,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   371,     0,
       0,     0,     0,     0,   398,   399,     0,     0,     0,   390,
     391,   392,   393,   394,   395,   396,   397,     0,     0,     0,
     372,   373,   374,   375,   376,   398,   399,   377,   378,   379,
     380,     0,   381,   382,   383,     0,     0,   384,     0,   385,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,   392,   393,   394,
     395,   396,   397,     0,     0,     0,   372,   373,   374,   375,
     376,   398,   399,   377,     0,     0,   380,     0,   381,   382,
     383,     0,     0,   384,     0,   385,   386,     0,     0,     0,
       0,     0,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,   391,   392,   393,   394,   395,   396,   397,   156,
     157,   158,   159,     0,     0,     0,     0,   398,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,     0,     0,   170,   171
};

static const yytype_int16 yycheck[] =
{
      11,   217,    58,   366,   234,   355,   236,    61,   238,   314,
      58,   211,    19,   195,   196,   197,   401,   266,    20,    17,
     176,    45,   112,    32,   120,    45,   409,   496,    19,    33,
     130,   108,    18,   137,   162,     0,   130,   165,   130,   130,
       5,    18,   427,    13,    14,    18,   107,    58,   130,   110,
      52,    62,   130,   108,   139,   130,   144,   144,   219,   108,
     137,   165,    31,    28,   164,    30,   115,    32,   143,    34,
     166,   124,   136,   167,    39,   165,   167,   165,   165,   130,
      45,   166,   137,   161,    19,   167,    51,   164,   137,   164,
     136,   136,    99,   100,   130,    99,   100,   158,   159,   136,
     107,   165,   109,   110,   111,   574,   130,   114,    99,   100,
     130,   130,   417,   144,   160,   160,   107,    13,    14,   110,
     111,   130,   160,   114,   135,   159,   130,   165,   165,   132,
     133,   134,   130,   167,   165,   191,   192,   193,   162,   195,
     196,   197,   198,   191,   192,   193,   148,   195,   196,   197,
     136,   158,   159,   536,   124,   164,   158,   462,   128,   136,
     130,   131,   218,   136,    99,   100,   471,   158,   159,   136,
     218,    56,   107,   142,   109,   110,   111,   136,   139,   114,
     191,   192,   193,   568,   195,   196,   197,   198,   370,   130,
     159,   161,   408,   578,   159,   162,   352,   253,   354,   136,
     165,   170,   451,   164,   404,   253,   165,   218,   264,   265,
     145,   146,   147,   148,   149,   108,   264,   265,     4,   136,
       6,   571,   137,   158,   159,   162,   136,   136,   124,   579,
     136,   160,   128,   136,   130,   131,   165,   622,    19,   165,
     139,   167,   253,   136,   137,   162,   139,   136,   553,   303,
     136,   160,   162,   264,   265,    10,   162,   313,   160,   162,
     137,   317,    19,   165,   164,   313,    21,    22,   324,   317,
     500,   166,   136,   168,   169,   139,   324,   333,   142,   335,
     336,   130,   161,   588,   669,   333,   166,    61,   336,   345,
     346,    65,   348,   349,   599,   132,   133,   345,   346,   130,
     348,   349,   313,   464,   124,   461,   317,    81,    82,    83,
      84,   130,   494,   324,   165,   165,   167,   167,    99,   100,
     130,   165,   333,   167,   335,   336,   107,   130,   109,   110,
     111,   130,   139,   114,   345,   346,   166,   348,   349,    33,
      97,    98,    99,   100,    33,   161,    52,   206,   711,   132,
     107,   166,   109,   110,   111,   143,   367,   114,    31,   116,
     117,   143,   139,    36,   143,   161,   147,   148,   149,   228,
      31,    31,   143,   143,   143,    36,    36,   158,   159,   143,
     143,   130,    55,   130,   614,   130,   166,   162,   145,   146,
     147,   148,   149,   139,    55,    55,   161,   169,   142,   160,
     143,   158,   159,   262,   263,   161,   143,   266,   161,   143,
     143,   159,    42,   165,   130,   274,   275,   276,   277,   278,
     161,   280,   144,   144,   144,   586,   130,   130,   130,   130,
     112,   160,   167,   594,   132,   108,   109,   136,   494,   136,
     165,   657,   115,   124,   605,   501,   494,   108,   109,   109,
      35,   165,   165,   501,   610,   314,   165,   165,   165,   620,
      18,   130,   130,   130,   137,   138,   161,    12,   161,   142,
     160,   124,   160,   160,   160,   139,   137,   138,   138,    38,
     130,   142,   142,   494,   144,   344,   159,   166,   347,   144,
     501,   162,   165,   144,   353,   160,   355,   170,   159,   159,
      48,   160,   160,   559,   167,   359,   160,   366,   160,   170,
     170,   559,   673,   372,   373,    47,    83,   376,   377,   378,
     379,    50,   381,   747,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,    48,
     399,   225,    62,   680,     1,   335,   338,   309,   559,   188,
     290,   712,   413,   610,   577,   355,   732,   253,   417,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   428,
      -1,    -1,    -1,   734,    -1,    -1,   737,    -1,    -1,   438,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   447,    31,
      11,   450,   451,    -1,    36,   454,    -1,    -1,    -1,   458,
      -1,    -1,    23,   462,    -1,    -1,   465,   466,    29,    -1,
      -1,   470,   471,    55,    -1,    -1,    -1,   476,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,   109,    -1,   725,
      -1,    -1,    -1,    -1,   533,    -1,   687,   725,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   749,   553,    -1,   138,    -1,    19,    -1,
     142,   749,    -1,    -1,   563,   564,    -1,   566,   719,   130,
     569,    -1,    -1,    -1,   725,    -1,   575,   159,   577,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,   170,   588,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   749,    -1,
     599,    -1,    36,   164,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,   612,   613,    -1,    -1,    -1,    -1,   618,
      -1,    55,    -1,    -1,   623,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,    -1,   114,   653,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,   138,    -1,   140,
     141,   142,   143,   144,   109,   684,   685,   148,   149,    -1,
      -1,   690,    -1,    -1,    -1,    -1,    -1,   158,   159,   698,
      -1,    -1,    -1,   702,   138,   704,   705,    -1,   142,    -1,
     144,    -1,   711,   138,     1,    -1,   715,   142,     5,   144,
       7,     8,     9,   722,    11,   159,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,   159,    -1,   170,    24,    25,    26,
      27,    -1,    -1,   742,    -1,   170,    -1,    -1,    -1,    36,
      37,    -1,   751,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
     167,   168,   169,     1,    -1,    -1,    -1,     5,    -1,     7,
       8,     9,    -1,    11,    -1,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    47,
      -1,    49,    50,    -1,    -1,    53,    54,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
      -1,    55,    -1,   161,    -1,    -1,   164,   165,   166,   167,
     168,   169,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,   109,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,   138,    -1,    87,    -1,   142,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,   153,    -1,    13,    14,    15,    16,   159,   160,
     161,    -1,    -1,   164,    -1,   166,    25,   168,   169,   170,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    55,   138,
      -1,    -1,    -1,   142,    -1,    -1,   145,   146,   147,    13,
      14,    15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,
     159,    25,   161,    -1,    -1,   164,    -1,   166,    -1,   168,
     169,   170,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,   109,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,   138,    -1,    87,    -1,   142,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,   170,    -1,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,    13,    14,    15,    16,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   161,    -1,    -1,
     164,   165,   166,    -1,   168,   169,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    -1,   164,    -1,   166,    -1,   168,
     169,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,   138,    -1,    55,    -1,   142,    -1,    -1,
      -1,   143,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,   159,    -1,   161,    -1,    25,   161,
      -1,    -1,   164,    -1,   166,   170,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
     170,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   161,    -1,    -1,   164,    -1,   166,
      -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,   153,    -1,    13,    14,    15,    16,    -1,   160,   161,
      -1,    -1,   164,    -1,   166,    25,   168,   169,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    55,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,   144,    -1,    -1,    -1,   145,   146,   147,    13,    14,
      15,    16,   152,   153,    -1,    -1,   159,    -1,    -1,    -1,
      25,   161,    -1,    -1,   164,    -1,   166,   170,   168,   169,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   161,   162,    -1,   164,
      -1,   166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   109,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   138,    -1,    87,    -1,   142,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   170,    36,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    13,    14,
      15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   161,   162,    -1,   164,    -1,   166,    -1,   168,   169,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   109,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
     138,    -1,    87,    -1,   142,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   170,    36,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   161,   162,    -1,   164,
      -1,   166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   109,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   138,    -1,    87,    -1,   142,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   170,    36,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    13,    14,
      15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   161,   162,    -1,   164,    -1,   166,    -1,   168,   169,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,   109,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
     138,    -1,    87,    -1,   142,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   170,    36,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   109,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   138,    -1,    87,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    13,    14,
      15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   161,    -1,    -1,   164,    -1,   166,    -1,   168,   169,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   161,    -1,    -1,   164,
      -1,   166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    13,    14,
      15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   161,   162,    -1,   164,    -1,   166,    -1,   168,   169,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   109,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
     138,    -1,    87,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   170,    36,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   161,   162,    -1,   164,
      -1,   166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   109,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,   138,    -1,    87,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,   159,    -1,   108,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    13,    14,    15,    16,    17,
      -1,   161,    -1,    -1,   164,    -1,   166,    25,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,   153,    -1,    13,    14,    15,
      16,    17,    -1,   161,    -1,    -1,   164,    -1,   166,    25,
     168,   169,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,   138,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,    13,    14,    15,    16,   152,   153,    -1,    -1,
     159,    -1,    -1,    -1,    25,   161,    -1,    -1,   164,    -1,
     166,   170,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    11,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,   145,   146,   147,    -1,    -1,    29,
      -1,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,    44,   166,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    19,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,   164,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    19,    -1,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    19,    -1,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    19,    -1,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    19,    -1,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      19,    -1,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,    97,    98,    99,   100,   101,   165,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   165,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    97,    98,    99,   100,   101,   165,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,   162,    -1,
      -1,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,   162,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
      -1,    -1,   162,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,   162,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,   162,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,
     162,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,   162,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,   161,    19,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    19,
      -1,    -1,   164,    -1,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   164,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    19,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    -1,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   158,   159,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     158,   159,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   158,   159,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,   135,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,    19,    97,    98,    99,
     100,   101,   158,   159,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    19,    97,    98,    99,   100,   101,   158,   159,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   158,   159,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   158,   159,   104,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   143,   144,   145,   146,   147,   148,   149,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,   152,   153
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   172,     0,     5,    28,    30,    32,    34,    39,    45,
      51,   159,   165,   173,   178,   179,   183,   202,   206,   220,
     252,   254,   255,   258,   130,   197,   198,   124,   184,   185,
     130,   180,   181,   130,   130,   130,    32,   130,   164,   199,
     200,   201,   211,     4,     6,    56,   251,   137,   136,   139,
     136,    20,    52,   148,   158,   182,   139,   166,   137,   164,
     130,   161,   136,   160,   130,   257,    33,    99,   100,   130,
     205,   166,    13,    14,   124,   128,   130,   131,   161,   175,
     197,   124,   185,   130,   130,   130,    61,    65,    81,    82,
      83,    84,   261,   253,    23,    29,    44,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,   211,   260,   262,
     263,   264,   130,   198,   200,   139,   256,   166,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   116,   117,   118,   119,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     152,   153,    33,    33,   161,   203,   250,   132,   174,    13,
      14,   124,   128,   130,   175,   195,   196,   182,   166,   130,
     167,   143,   143,   143,   161,   143,   143,   143,   143,   143,
      31,    36,    55,   109,   138,   142,   159,   170,   162,   211,
     238,    45,   130,   162,   239,   240,   246,   259,   139,   204,
     167,   249,   259,   132,   133,   136,   162,   253,   137,   165,
     264,   264,   264,   130,   203,   264,   203,   264,   203,   264,
     130,   241,   242,   264,   130,   243,   244,    31,   142,   159,
     170,    13,    14,    15,    16,    25,    37,    41,    43,    46,
      50,    87,    99,   100,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   131,   145,   146,   147,   152,   153,   160,
     161,   166,   168,   169,   177,   207,   211,   213,   222,   223,
     226,   227,   228,   231,   232,   233,   234,   260,   265,   268,
     273,   275,   277,   159,   167,   235,   237,   246,   162,   165,
     108,   136,   137,   139,   245,   264,   207,   139,   142,   248,
     195,   167,   234,   144,   165,   144,   162,   204,   144,   204,
     144,   204,   144,   139,   144,   165,   139,   144,   165,   142,
     160,   264,   265,   161,   161,   143,   143,   161,   143,   143,
     234,   234,    11,   264,    11,   264,   234,   270,   274,   176,
     234,   234,   234,   234,   234,   234,   209,   169,   130,   161,
     202,    19,    97,    98,    99,   100,   101,   104,   105,   106,
     107,   109,   110,   111,   114,   116,   117,   138,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   158,   159,
     160,   161,   198,    42,   236,   165,   239,   130,   264,   113,
     210,   229,   234,   264,   108,   115,   137,   247,   165,   264,
     144,   144,   144,   264,   241,   264,   243,   161,   130,   234,
     264,   264,   234,   264,   264,   259,   130,   160,   161,   234,
     266,   267,   269,   270,   271,   272,   259,   161,   267,   272,
     120,   165,   167,   133,   134,   174,   162,     1,     7,     8,
       9,    11,    24,    26,    27,    36,    40,    47,    49,    53,
      54,    88,   165,   167,   186,   187,   190,   191,   192,   193,
     194,   208,   210,   212,   214,   215,   216,   217,   218,   220,
     221,   229,   230,   234,   143,   211,   159,   162,   224,   234,
     203,    17,   130,   234,   234,   234,   234,   234,   234,   130,
     234,   130,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   130,   234,   162,
     224,   160,   246,   245,   169,   227,   112,    89,    90,    91,
      92,    93,    94,    95,    96,   102,   103,   108,   115,   118,
     119,   137,   165,   247,   210,   234,   144,   162,   224,    17,
     143,   234,   162,   144,   144,   162,   144,   144,    18,   245,
     160,   162,   136,   160,   165,   136,   160,   165,    18,   162,
     167,   167,   234,   270,   167,   234,   234,   259,   108,   165,
     210,   234,   207,   234,   234,   165,   124,    53,   234,   108,
     210,   234,    35,   165,   165,   234,   165,   165,   165,    18,
     219,   165,   108,   137,   203,   264,   130,   266,   136,   162,
     204,   264,   161,   139,   160,   162,   234,   227,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   210,   234,   165,   162,   264,   130,   162,
     234,   234,   234,   161,   224,   234,   267,   130,   160,   266,
     234,   160,   271,   224,   267,   160,   160,   135,   207,    18,
     210,   234,   165,    12,   207,   139,   124,   210,   234,   165,
     166,   207,    38,   249,   113,   113,   204,   144,   160,   234,
     120,   207,   225,   162,   224,   234,   165,   162,   144,   165,
     162,   234,   165,   160,   245,   165,   167,   224,   165,   207,
     165,   209,    10,    21,    22,   188,   189,   234,   234,   144,
     211,   162,   108,   234,   162,    17,   234,   130,   162,   234,
     160,   234,   234,   160,   165,   167,   207,   234,   211,   234,
     264,   162,   144,   165,   276,   276,   207,   207,   162,    17,
     234,    48,   160,   167,   189,   264,   162,   234,   160,   160,
     162
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   173,   174,   174,   175,   176,   176,   176,
     177,   178,   179,   180,   180,   180,   181,   181,   182,   182,
     183,   184,   184,   185,   185,   186,   187,   187,   188,   188,
     189,   189,   189,   190,   190,   191,   192,   193,   194,   195,
     195,   195,   195,   195,   195,   196,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   198,   198,   199,   199,   200,
     200,   201,   201,   202,   202,   203,   203,   203,   204,   204,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   206,   207,   207,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   209,   209,
     209,   210,   210,   211,   211,   211,   212,   213,   213,   213,
     213,   214,   215,   216,   216,   216,   216,   216,   217,   217,
     217,   217,   218,   219,   219,   220,   220,   221,   222,   222,
     222,   223,   223,   223,   223,   223,   223,   224,   224,   225,
     225,   225,   226,   226,   227,   227,   228,   228,   228,   228,
     228,   228,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   230,   230,
     231,   232,   232,   233,   233,   233,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   235,   235,   236,   236,
     237,   238,   238,   239,   239,   240,   240,   241,   241,   242,
     242,   243,   244,   244,   245,   245,   246,   246,   246,   246,
     246,   247,   247,   247,   248,   248,   249,   249,   249,   249,
     249,   250,   250,   251,   251,   252,   253,   253,   253,   254,
     255,   255,   256,   256,   257,   258,   259,   259,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   261,   261,   261,   261,   262,   263,
     263,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   265,   265,   265,   265,   266,   266,   267,   267,   268,
     268,   268,   268,   268,   269,   269,   270,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   276,   276,   277,   277
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     3,     0,     2,     4,
       3,     2,     2,     1,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     1,     1,
       0,     2,     4,     1,     1,     4,     6,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     1,     1,
       4,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     6,     3,     7,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     2,     1,     3,     2,     2,     2,     4,     5,
       2,     1,     1,     2,     3,     4,     2,     3,     3,     4,
       2,     3,     4,     0,     2,     1,     1,     3,     5,     5,
       5,     5,     8,    10,     6,     9,    11,     1,     3,     1,
       2,     3,     1,     1,     1,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       6,     5,     6,     3,     6,     7,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     3,     3,     1,     3,     4,     3,     4,     2,     4,
       4,     6,     7,     3,     5,     4,     3,     1,     1,     1,
       1,     1,     1,     3,     3,     2,     0,     3,     0,     1,
       3,     0,     3,     1,     2,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     4,     6,     5,     5,
       4,     0,     2,     0,     1,     5,     0,     3,     5,     4,
       5,     7,     0,     2,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     4,     3,     4,     2,     3,     2,
       4,     3,     2,     2,     3,     2,     2,     4,     6,     4,
       1,     4,     5,     1,     4,     5,     1,     4,     5,     4,
       4,     1,     1,     1,     1,     3,     5,     1,     3,     5,
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
    case 130: /* "name"  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3016 "ds_parser.cpp"
        break;

    case 174: /* character_sequence  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3022 "ds_parser.cpp"
        break;

    case 175: /* string_constant  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3028 "ds_parser.cpp"
        break;

    case 176: /* string_builder_body  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3034 "ds_parser.cpp"
        break;

    case 177: /* string_builder  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3040 "ds_parser.cpp"
        break;

    case 180: /* require_module_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3046 "ds_parser.cpp"
        break;

    case 186: /* expression_label  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3052 "ds_parser.cpp"
        break;

    case 187: /* expression_goto  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3058 "ds_parser.cpp"
        break;

    case 189: /* expression_else  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3064 "ds_parser.cpp"
        break;

    case 191: /* expression_if_then_else  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3070 "ds_parser.cpp"
        break;

    case 192: /* expression_for_loop  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3076 "ds_parser.cpp"
        break;

    case 193: /* expression_while_loop  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3082 "ds_parser.cpp"
        break;

    case 194: /* expression_with  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3088 "ds_parser.cpp"
        break;

    case 195: /* annotation_argument_value  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3094 "ds_parser.cpp"
        break;

    case 196: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3100 "ds_parser.cpp"
        break;

    case 197: /* annotation_argument  */
#line 108 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3106 "ds_parser.cpp"
        break;

    case 198: /* annotation_argument_list  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3112 "ds_parser.cpp"
        break;

    case 199: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3118 "ds_parser.cpp"
        break;

    case 200: /* annotation_declaration  */
#line 110 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3124 "ds_parser.cpp"
        break;

    case 201: /* annotation_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3130 "ds_parser.cpp"
        break;

    case 202: /* optional_annotation_list  */
#line 111 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3136 "ds_parser.cpp"
        break;

    case 203: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3142 "ds_parser.cpp"
        break;

    case 204: /* optional_function_type  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3148 "ds_parser.cpp"
        break;

    case 205: /* function_name  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3154 "ds_parser.cpp"
        break;

    case 207: /* expression_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3160 "ds_parser.cpp"
        break;

    case 208: /* expression_any  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3166 "ds_parser.cpp"
        break;

    case 209: /* expressions  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3172 "ds_parser.cpp"
        break;

    case 210: /* expr_pipe  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3178 "ds_parser.cpp"
        break;

    case 211: /* name_in_namespace  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3184 "ds_parser.cpp"
        break;

    case 212: /* expression_delete  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3190 "ds_parser.cpp"
        break;

    case 213: /* expr_new  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3196 "ds_parser.cpp"
        break;

    case 214: /* expression_break  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3202 "ds_parser.cpp"
        break;

    case 215: /* expression_continue  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3208 "ds_parser.cpp"
        break;

    case 216: /* expression_return  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3214 "ds_parser.cpp"
        break;

    case 217: /* expression_yield  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3220 "ds_parser.cpp"
        break;

    case 218: /* expression_try_catch  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3226 "ds_parser.cpp"
        break;

    case 221: /* expression_let  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3232 "ds_parser.cpp"
        break;

    case 222: /* expr_cast  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3238 "ds_parser.cpp"
        break;

    case 223: /* expr_type_info  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3244 "ds_parser.cpp"
        break;

    case 224: /* expr_list  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3250 "ds_parser.cpp"
        break;

    case 225: /* block_or_simple_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3256 "ds_parser.cpp"
        break;

    case 227: /* expr_block  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3262 "ds_parser.cpp"
        break;

    case 228: /* expr_numeric_const  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3268 "ds_parser.cpp"
        break;

    case 229: /* expr_assign  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3274 "ds_parser.cpp"
        break;

    case 230: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3280 "ds_parser.cpp"
        break;

    case 231: /* expr_named_call  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3286 "ds_parser.cpp"
        break;

    case 232: /* expr_method_call  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3292 "ds_parser.cpp"
        break;

    case 233: /* func_addr_expr  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3298 "ds_parser.cpp"
        break;

    case 234: /* expr  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3304 "ds_parser.cpp"
        break;

    case 235: /* optional_field_annotation  */
#line 109 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3310 "ds_parser.cpp"
        break;

    case 237: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3316 "ds_parser.cpp"
        break;

    case 238: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3322 "ds_parser.cpp"
        break;

    case 239: /* function_argument_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3328 "ds_parser.cpp"
        break;

    case 240: /* function_argument_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3334 "ds_parser.cpp"
        break;

    case 241: /* tuple_type  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3340 "ds_parser.cpp"
        break;

    case 242: /* tuple_type_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3346 "ds_parser.cpp"
        break;

    case 243: /* variant_type  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3352 "ds_parser.cpp"
        break;

    case 244: /* variant_type_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3358 "ds_parser.cpp"
        break;

    case 246: /* variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3364 "ds_parser.cpp"
        break;

    case 249: /* let_variable_declaration  */
#line 104 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3370 "ds_parser.cpp"
        break;

    case 250: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3376 "ds_parser.cpp"
        break;

    case 253: /* enum_list  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3382 "ds_parser.cpp"
        break;

    case 256: /* optional_structure_parent  */
#line 102 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3388 "ds_parser.cpp"
        break;

    case 259: /* variable_name_list  */
#line 103 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3394 "ds_parser.cpp"
        break;

    case 262: /* structure_type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3400 "ds_parser.cpp"
        break;

    case 263: /* auto_type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3406 "ds_parser.cpp"
        break;

    case 264: /* type_declaration  */
#line 106 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3412 "ds_parser.cpp"
        break;

    case 265: /* make_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3418 "ds_parser.cpp"
        break;

    case 266: /* make_struct_fields  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3424 "ds_parser.cpp"
        break;

    case 267: /* make_struct_dim  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3430 "ds_parser.cpp"
        break;

    case 268: /* make_struct_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3436 "ds_parser.cpp"
        break;

    case 269: /* make_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3442 "ds_parser.cpp"
        break;

    case 270: /* make_map_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3448 "ds_parser.cpp"
        break;

    case 271: /* make_any_tuple  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3454 "ds_parser.cpp"
        break;

    case 272: /* make_dim  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3460 "ds_parser.cpp"
        break;

    case 273: /* make_dim_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3466 "ds_parser.cpp"
        break;

    case 274: /* make_table  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3472 "ds_parser.cpp"
        break;

    case 275: /* make_table_decl  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3478 "ds_parser.cpp"
        break;

    case 276: /* array_comprehension_where  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3484 "ds_parser.cpp"
        break;

    case 277: /* array_comprehension  */
#line 107 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3490 "ds_parser.cpp"
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
#line 396 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3790 "ds_parser.cpp"
    break;

  case 14:
#line 408 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3796 "ds_parser.cpp"
    break;

  case 15:
#line 409 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3802 "ds_parser.cpp"
    break;

  case 16:
#line 413 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 3808 "ds_parser.cpp"
    break;

  case 17:
#line 417 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3817 "ds_parser.cpp"
    break;

  case 18:
#line 421 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3831 "ds_parser.cpp"
    break;

  case 19:
#line 430 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3841 "ds_parser.cpp"
    break;

  case 20:
#line 438 "ds_parser.ypp"
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
#line 3859 "ds_parser.cpp"
    break;

  case 21:
#line 454 "ds_parser.ypp"
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
#line 3882 "ds_parser.cpp"
    break;

  case 23:
#line 479 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3890 "ds_parser.cpp"
    break;

  case 24:
#line 482 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3901 "ds_parser.cpp"
    break;

  case 25:
#line 488 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3912 "ds_parser.cpp"
    break;

  case 26:
#line 497 "ds_parser.ypp"
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
#line 3938 "ds_parser.cpp"
    break;

  case 27:
#line 518 "ds_parser.ypp"
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
#line 3963 "ds_parser.cpp"
    break;

  case 28:
#line 541 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 3969 "ds_parser.cpp"
    break;

  case 29:
#line 542 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 3975 "ds_parser.cpp"
    break;

  case 33:
#line 555 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3983 "ds_parser.cpp"
    break;

  case 34:
#line 558 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3991 "ds_parser.cpp"
    break;

  case 35:
#line 564 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3999 "ds_parser.cpp"
    break;

  case 36:
#line 570 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4007 "ds_parser.cpp"
    break;

  case 37:
#line 573 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4015 "ds_parser.cpp"
    break;

  case 38:
#line 579 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4021 "ds_parser.cpp"
    break;

  case 39:
#line 580 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4027 "ds_parser.cpp"
    break;

  case 40:
#line 584 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4033 "ds_parser.cpp"
    break;

  case 41:
#line 585 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4039 "ds_parser.cpp"
    break;

  case 42:
#line 586 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4050 "ds_parser.cpp"
    break;

  case 43:
#line 595 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4056 "ds_parser.cpp"
    break;

  case 44:
#line 596 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4062 "ds_parser.cpp"
    break;

  case 45:
#line 600 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4073 "ds_parser.cpp"
    break;

  case 46:
#line 609 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4087 "ds_parser.cpp"
    break;

  case 47:
#line 621 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4099 "ds_parser.cpp"
    break;

  case 48:
#line 631 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4110 "ds_parser.cpp"
    break;

  case 49:
#line 640 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4116 "ds_parser.cpp"
    break;

  case 50:
#line 641 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4122 "ds_parser.cpp"
    break;

  case 51:
#line 642 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4128 "ds_parser.cpp"
    break;

  case 52:
#line 643 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4134 "ds_parser.cpp"
    break;

  case 53:
#line 644 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4140 "ds_parser.cpp"
    break;

  case 54:
#line 645 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4146 "ds_parser.cpp"
    break;

  case 55:
#line 649 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4156 "ds_parser.cpp"
    break;

  case 56:
#line 654 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4166 "ds_parser.cpp"
    break;

  case 57:
#line 662 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4172 "ds_parser.cpp"
    break;

  case 58:
#line 663 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4178 "ds_parser.cpp"
    break;

  case 59:
#line 664 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4184 "ds_parser.cpp"
    break;

  case 60:
#line 665 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4190 "ds_parser.cpp"
    break;

  case 61:
#line 666 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4196 "ds_parser.cpp"
    break;

  case 62:
#line 667 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4202 "ds_parser.cpp"
    break;

  case 63:
#line 668 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4208 "ds_parser.cpp"
    break;

  case 64:
#line 669 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4216 "ds_parser.cpp"
    break;

  case 65:
#line 675 "ds_parser.ypp"
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
#line 4234 "ds_parser.cpp"
    break;

  case 66:
#line 688 "ds_parser.ypp"
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
#line 4252 "ds_parser.cpp"
    break;

  case 67:
#line 704 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4258 "ds_parser.cpp"
    break;

  case 68:
#line 705 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4264 "ds_parser.cpp"
    break;

  case 69:
#line 709 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4276 "ds_parser.cpp"
    break;

  case 70:
#line 716 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4290 "ds_parser.cpp"
    break;

  case 71:
#line 728 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4299 "ds_parser.cpp"
    break;

  case 72:
#line 732 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4308 "ds_parser.cpp"
    break;

  case 73:
#line 739 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4314 "ds_parser.cpp"
    break;

  case 74:
#line 740 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4320 "ds_parser.cpp"
    break;

  case 75:
#line 744 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4326 "ds_parser.cpp"
    break;

  case 76:
#line 745 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4332 "ds_parser.cpp"
    break;

  case 77:
#line 746 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4338 "ds_parser.cpp"
    break;

  case 78:
#line 750 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4347 "ds_parser.cpp"
    break;

  case 79:
#line 754 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4356 "ds_parser.cpp"
    break;

  case 80:
#line 761 "ds_parser.ypp"
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4365 "ds_parser.cpp"
    break;

  case 81:
#line 765 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4371 "ds_parser.cpp"
    break;

  case 82:
#line 766 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4377 "ds_parser.cpp"
    break;

  case 83:
#line 767 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4383 "ds_parser.cpp"
    break;

  case 84:
#line 768 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4389 "ds_parser.cpp"
    break;

  case 85:
#line 769 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4395 "ds_parser.cpp"
    break;

  case 86:
#line 770 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4401 "ds_parser.cpp"
    break;

  case 87:
#line 771 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4407 "ds_parser.cpp"
    break;

  case 88:
#line 772 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4413 "ds_parser.cpp"
    break;

  case 89:
#line 773 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4419 "ds_parser.cpp"
    break;

  case 90:
#line 774 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4425 "ds_parser.cpp"
    break;

  case 91:
#line 775 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4431 "ds_parser.cpp"
    break;

  case 92:
#line 776 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4437 "ds_parser.cpp"
    break;

  case 93:
#line 777 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4443 "ds_parser.cpp"
    break;

  case 94:
#line 778 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4449 "ds_parser.cpp"
    break;

  case 95:
#line 779 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4455 "ds_parser.cpp"
    break;

  case 96:
#line 780 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4461 "ds_parser.cpp"
    break;

  case 97:
#line 781 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4467 "ds_parser.cpp"
    break;

  case 98:
#line 782 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4473 "ds_parser.cpp"
    break;

  case 99:
#line 783 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4479 "ds_parser.cpp"
    break;

  case 100:
#line 784 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4485 "ds_parser.cpp"
    break;

  case 101:
#line 785 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4491 "ds_parser.cpp"
    break;

  case 102:
#line 786 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4497 "ds_parser.cpp"
    break;

  case 103:
#line 787 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4503 "ds_parser.cpp"
    break;

  case 104:
#line 788 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4509 "ds_parser.cpp"
    break;

  case 105:
#line 789 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4515 "ds_parser.cpp"
    break;

  case 106:
#line 790 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4521 "ds_parser.cpp"
    break;

  case 107:
#line 791 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4527 "ds_parser.cpp"
    break;

  case 108:
#line 792 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4533 "ds_parser.cpp"
    break;

  case 109:
#line 793 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4539 "ds_parser.cpp"
    break;

  case 110:
#line 794 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4545 "ds_parser.cpp"
    break;

  case 111:
#line 795 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4551 "ds_parser.cpp"
    break;

  case 112:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4557 "ds_parser.cpp"
    break;

  case 113:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4563 "ds_parser.cpp"
    break;

  case 114:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4569 "ds_parser.cpp"
    break;

  case 115:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4575 "ds_parser.cpp"
    break;

  case 116:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4581 "ds_parser.cpp"
    break;

  case 117:
#line 805 "ds_parser.ypp"
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
#line 4652 "ds_parser.cpp"
    break;

  case 118:
#line 874 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4660 "ds_parser.cpp"
    break;

  case 119:
#line 877 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4672 "ds_parser.cpp"
    break;

  case 120:
#line 887 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4678 "ds_parser.cpp"
    break;

  case 121:
#line 888 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4684 "ds_parser.cpp"
    break;

  case 122:
#line 889 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4690 "ds_parser.cpp"
    break;

  case 123:
#line 890 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4696 "ds_parser.cpp"
    break;

  case 124:
#line 891 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4702 "ds_parser.cpp"
    break;

  case 125:
#line 892 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4708 "ds_parser.cpp"
    break;

  case 126:
#line 893 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4714 "ds_parser.cpp"
    break;

  case 127:
#line 894 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4720 "ds_parser.cpp"
    break;

  case 128:
#line 895 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4726 "ds_parser.cpp"
    break;

  case 129:
#line 896 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4732 "ds_parser.cpp"
    break;

  case 130:
#line 897 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4738 "ds_parser.cpp"
    break;

  case 131:
#line 898 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4744 "ds_parser.cpp"
    break;

  case 132:
#line 899 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4750 "ds_parser.cpp"
    break;

  case 133:
#line 900 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4756 "ds_parser.cpp"
    break;

  case 134:
#line 901 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4762 "ds_parser.cpp"
    break;

  case 135:
#line 902 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4768 "ds_parser.cpp"
    break;

  case 136:
#line 903 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4774 "ds_parser.cpp"
    break;

  case 137:
#line 904 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4780 "ds_parser.cpp"
    break;

  case 138:
#line 908 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4789 "ds_parser.cpp"
    break;

  case 139:
#line 912 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4800 "ds_parser.cpp"
    break;

  case 140:
#line 918 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4808 "ds_parser.cpp"
    break;

  case 141:
#line 924 "ds_parser.ypp"
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
#line 4824 "ds_parser.cpp"
    break;

  case 142:
#line 935 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4832 "ds_parser.cpp"
    break;

  case 143:
#line 941 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4838 "ds_parser.cpp"
    break;

  case 144:
#line 942 "ds_parser.ypp"
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
#line 4854 "ds_parser.cpp"
    break;

  case 145:
#line 953 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4860 "ds_parser.cpp"
    break;

  case 146:
#line 957 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4868 "ds_parser.cpp"
    break;

  case 147:
#line 963 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4876 "ds_parser.cpp"
    break;

  case 148:
#line 966 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4884 "ds_parser.cpp"
    break;

  case 149:
#line 969 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4893 "ds_parser.cpp"
    break;

  case 150:
#line 973 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4901 "ds_parser.cpp"
    break;

  case 151:
#line 979 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4907 "ds_parser.cpp"
    break;

  case 152:
#line 983 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4913 "ds_parser.cpp"
    break;

  case 153:
#line 987 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4921 "ds_parser.cpp"
    break;

  case 154:
#line 990 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4929 "ds_parser.cpp"
    break;

  case 155:
#line 993 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4939 "ds_parser.cpp"
    break;

  case 156:
#line 998 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4947 "ds_parser.cpp"
    break;

  case 157:
#line 1001 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4957 "ds_parser.cpp"
    break;

  case 158:
#line 1009 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4965 "ds_parser.cpp"
    break;

  case 159:
#line 1012 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4975 "ds_parser.cpp"
    break;

  case 160:
#line 1017 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4983 "ds_parser.cpp"
    break;

  case 161:
#line 1020 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4993 "ds_parser.cpp"
    break;

  case 162:
#line 1028 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5001 "ds_parser.cpp"
    break;

  case 163:
#line 1034 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5007 "ds_parser.cpp"
    break;

  case 164:
#line 1035 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5013 "ds_parser.cpp"
    break;

  case 165:
#line 1039 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5019 "ds_parser.cpp"
    break;

  case 166:
#line 1040 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5025 "ds_parser.cpp"
    break;

  case 167:
#line 1044 "ds_parser.ypp"
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
#line 5061 "ds_parser.cpp"
    break;

  case 168:
#line 1078 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5069 "ds_parser.cpp"
    break;

  case 169:
#line 1081 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5079 "ds_parser.cpp"
    break;

  case 170:
#line 1086 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5089 "ds_parser.cpp"
    break;

  case 171:
#line 1094 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5098 "ds_parser.cpp"
    break;

  case 172:
#line 1098 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5108 "ds_parser.cpp"
    break;

  case 173:
#line 1103 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5119 "ds_parser.cpp"
    break;

  case 174:
#line 1109 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5128 "ds_parser.cpp"
    break;

  case 175:
#line 1113 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5138 "ds_parser.cpp"
    break;

  case 176:
#line 1118 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5149 "ds_parser.cpp"
    break;

  case 177:
#line 1127 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5157 "ds_parser.cpp"
    break;

  case 178:
#line 1130 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5165 "ds_parser.cpp"
    break;

  case 179:
#line 1136 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5171 "ds_parser.cpp"
    break;

  case 180:
#line 1137 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5183 "ds_parser.cpp"
    break;

  case 181:
#line 1144 "ds_parser.ypp"
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5196 "ds_parser.cpp"
    break;

  case 182:
#line 1155 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5202 "ds_parser.cpp"
    break;

  case 183:
#line 1156 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5208 "ds_parser.cpp"
    break;

  case 184:
#line 1160 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5218 "ds_parser.cpp"
    break;

  case 185:
#line 1166 "ds_parser.ypp"
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
#line 5271 "ds_parser.cpp"
    break;

  case 186:
#line 1217 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5277 "ds_parser.cpp"
    break;

  case 187:
#line 1218 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5283 "ds_parser.cpp"
    break;

  case 188:
#line 1219 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5289 "ds_parser.cpp"
    break;

  case 189:
#line 1220 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5295 "ds_parser.cpp"
    break;

  case 190:
#line 1221 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5301 "ds_parser.cpp"
    break;

  case 191:
#line 1222 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5307 "ds_parser.cpp"
    break;

  case 192:
#line 1226 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5313 "ds_parser.cpp"
    break;

  case 193:
#line 1227 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5319 "ds_parser.cpp"
    break;

  case 194:
#line 1228 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5325 "ds_parser.cpp"
    break;

  case 195:
#line 1229 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5331 "ds_parser.cpp"
    break;

  case 196:
#line 1230 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5337 "ds_parser.cpp"
    break;

  case 197:
#line 1231 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5343 "ds_parser.cpp"
    break;

  case 198:
#line 1232 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5349 "ds_parser.cpp"
    break;

  case 199:
#line 1233 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5355 "ds_parser.cpp"
    break;

  case 200:
#line 1234 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5361 "ds_parser.cpp"
    break;

  case 201:
#line 1235 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5367 "ds_parser.cpp"
    break;

  case 202:
#line 1236 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5373 "ds_parser.cpp"
    break;

  case 203:
#line 1237 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5379 "ds_parser.cpp"
    break;

  case 204:
#line 1238 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5385 "ds_parser.cpp"
    break;

  case 205:
#line 1239 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5391 "ds_parser.cpp"
    break;

  case 206:
#line 1240 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5397 "ds_parser.cpp"
    break;

  case 207:
#line 1241 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5403 "ds_parser.cpp"
    break;

  case 208:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5409 "ds_parser.cpp"
    break;

  case 209:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5415 "ds_parser.cpp"
    break;

  case 210:
#line 1250 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5427 "ds_parser.cpp"
    break;

  case 211:
#line 1261 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5437 "ds_parser.cpp"
    break;

  case 212:
#line 1266 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5449 "ds_parser.cpp"
    break;

  case 213:
#line 1276 "ds_parser.ypp"
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5458 "ds_parser.cpp"
    break;

  case 214:
#line 1280 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5470 "ds_parser.cpp"
    break;

  case 215:
#line 1287 "ds_parser.ypp"
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
#line 5486 "ds_parser.cpp"
    break;

  case 216:
#line 1301 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5492 "ds_parser.cpp"
    break;

  case 217:
#line 1302 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5498 "ds_parser.cpp"
    break;

  case 218:
#line 1303 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5504 "ds_parser.cpp"
    break;

  case 219:
#line 1304 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5510 "ds_parser.cpp"
    break;

  case 220:
#line 1305 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5516 "ds_parser.cpp"
    break;

  case 221:
#line 1306 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5522 "ds_parser.cpp"
    break;

  case 222:
#line 1307 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5528 "ds_parser.cpp"
    break;

  case 223:
#line 1308 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5534 "ds_parser.cpp"
    break;

  case 224:
#line 1309 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5540 "ds_parser.cpp"
    break;

  case 225:
#line 1310 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5546 "ds_parser.cpp"
    break;

  case 226:
#line 1311 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5552 "ds_parser.cpp"
    break;

  case 227:
#line 1312 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5558 "ds_parser.cpp"
    break;

  case 228:
#line 1313 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5564 "ds_parser.cpp"
    break;

  case 229:
#line 1314 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5570 "ds_parser.cpp"
    break;

  case 230:
#line 1315 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5576 "ds_parser.cpp"
    break;

  case 231:
#line 1316 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5582 "ds_parser.cpp"
    break;

  case 232:
#line 1317 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5588 "ds_parser.cpp"
    break;

  case 233:
#line 1318 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5594 "ds_parser.cpp"
    break;

  case 234:
#line 1319 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5600 "ds_parser.cpp"
    break;

  case 235:
#line 1320 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5606 "ds_parser.cpp"
    break;

  case 236:
#line 1321 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5612 "ds_parser.cpp"
    break;

  case 237:
#line 1322 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5618 "ds_parser.cpp"
    break;

  case 238:
#line 1323 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5624 "ds_parser.cpp"
    break;

  case 239:
#line 1324 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5630 "ds_parser.cpp"
    break;

  case 240:
#line 1325 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5636 "ds_parser.cpp"
    break;

  case 241:
#line 1326 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5642 "ds_parser.cpp"
    break;

  case 242:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5648 "ds_parser.cpp"
    break;

  case 243:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5654 "ds_parser.cpp"
    break;

  case 244:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5660 "ds_parser.cpp"
    break;

  case 245:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5666 "ds_parser.cpp"
    break;

  case 246:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5672 "ds_parser.cpp"
    break;

  case 247:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5678 "ds_parser.cpp"
    break;

  case 248:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5684 "ds_parser.cpp"
    break;

  case 249:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5690 "ds_parser.cpp"
    break;

  case 250:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5696 "ds_parser.cpp"
    break;

  case 251:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5702 "ds_parser.cpp"
    break;

  case 252:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5708 "ds_parser.cpp"
    break;

  case 253:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5714 "ds_parser.cpp"
    break;

  case 254:
#line 1339 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5723 "ds_parser.cpp"
    break;

  case 255:
#line 1343 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5732 "ds_parser.cpp"
    break;

  case 256:
#line 1347 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5740 "ds_parser.cpp"
    break;

  case 257:
#line 1350 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5748 "ds_parser.cpp"
    break;

  case 258:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5754 "ds_parser.cpp"
    break;

  case 259:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5760 "ds_parser.cpp"
    break;

  case 260:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5766 "ds_parser.cpp"
    break;

  case 261:
#line 1356 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5776 "ds_parser.cpp"
    break;

  case 262:
#line 1361 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5786 "ds_parser.cpp"
    break;

  case 263:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5792 "ds_parser.cpp"
    break;

  case 264:
#line 1367 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5800 "ds_parser.cpp"
    break;

  case 265:
#line 1370 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5808 "ds_parser.cpp"
    break;

  case 266:
#line 1373 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5817 "ds_parser.cpp"
    break;

  case 267:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5823 "ds_parser.cpp"
    break;

  case 268:
#line 1378 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5829 "ds_parser.cpp"
    break;

  case 269:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5835 "ds_parser.cpp"
    break;

  case 270:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5841 "ds_parser.cpp"
    break;

  case 271:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5847 "ds_parser.cpp"
    break;

  case 272:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5853 "ds_parser.cpp"
    break;

  case 273:
#line 1383 "ds_parser.ypp"
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
#line 5868 "ds_parser.cpp"
    break;

  case 274:
#line 1393 "ds_parser.ypp"
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
#line 5883 "ds_parser.cpp"
    break;

  case 275:
#line 1403 "ds_parser.ypp"
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
#line 5919 "ds_parser.cpp"
    break;

  case 276:
#line 1437 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 5925 "ds_parser.cpp"
    break;

  case 277:
#line 1438 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5931 "ds_parser.cpp"
    break;

  case 278:
#line 1442 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5937 "ds_parser.cpp"
    break;

  case 279:
#line 1443 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5943 "ds_parser.cpp"
    break;

  case 280:
#line 1447 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5953 "ds_parser.cpp"
    break;

  case 281:
#line 1455 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5961 "ds_parser.cpp"
    break;

  case 282:
#line 1458 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5970 "ds_parser.cpp"
    break;

  case 283:
#line 1465 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5976 "ds_parser.cpp"
    break;

  case 284:
#line 1466 "ds_parser.ypp"
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5982 "ds_parser.cpp"
    break;

  case 285:
#line 1470 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5988 "ds_parser.cpp"
    break;

  case 286:
#line 1471 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5994 "ds_parser.cpp"
    break;

  case 287:
#line 1475 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6002 "ds_parser.cpp"
    break;

  case 288:
#line 1478 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6013 "ds_parser.cpp"
    break;

  case 289:
#line 1487 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6019 "ds_parser.cpp"
    break;

  case 290:
#line 1488 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6025 "ds_parser.cpp"
    break;

  case 291:
#line 1492 "ds_parser.ypp"
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6036 "ds_parser.cpp"
    break;

  case 292:
#line 1501 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6042 "ds_parser.cpp"
    break;

  case 293:
#line 1502 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6048 "ds_parser.cpp"
    break;

  case 294:
#line 1507 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6054 "ds_parser.cpp"
    break;

  case 295:
#line 1508 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6060 "ds_parser.cpp"
    break;

  case 296:
#line 1512 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6070 "ds_parser.cpp"
    break;

  case 297:
#line 1517 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6078 "ds_parser.cpp"
    break;

  case 298:
#line 1520 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6087 "ds_parser.cpp"
    break;

  case 299:
#line 1524 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6098 "ds_parser.cpp"
    break;

  case 300:
#line 1530 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6109 "ds_parser.cpp"
    break;

  case 301:
#line 1539 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6115 "ds_parser.cpp"
    break;

  case 302:
#line 1540 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6121 "ds_parser.cpp"
    break;

  case 303:
#line 1541 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6127 "ds_parser.cpp"
    break;

  case 304:
#line 1545 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6133 "ds_parser.cpp"
    break;

  case 305:
#line 1546 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6139 "ds_parser.cpp"
    break;

  case 306:
#line 1550 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6147 "ds_parser.cpp"
    break;

  case 307:
#line 1553 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6157 "ds_parser.cpp"
    break;

  case 308:
#line 1558 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6167 "ds_parser.cpp"
    break;

  case 309:
#line 1563 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6180 "ds_parser.cpp"
    break;

  case 310:
#line 1571 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6193 "ds_parser.cpp"
    break;

  case 311:
#line 1582 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6201 "ds_parser.cpp"
    break;

  case 312:
#line 1585 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6210 "ds_parser.cpp"
    break;

  case 313:
#line 1592 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6216 "ds_parser.cpp"
    break;

  case 314:
#line 1593 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6222 "ds_parser.cpp"
    break;

  case 315:
#line 1597 "ds_parser.ypp"
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
#line 6254 "ds_parser.cpp"
    break;

  case 316:
#line 1627 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6262 "ds_parser.cpp"
    break;

  case 317:
#line 1630 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6276 "ds_parser.cpp"
    break;

  case 318:
#line 1639 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6290 "ds_parser.cpp"
    break;

  case 319:
#line 1651 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6304 "ds_parser.cpp"
    break;

  case 320:
#line 1663 "ds_parser.ypp"
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
#line 6320 "ds_parser.cpp"
    break;

  case 321:
#line 1674 "ds_parser.ypp"
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
#line 6337 "ds_parser.cpp"
    break;

  case 322:
#line 1689 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6343 "ds_parser.cpp"
    break;

  case 323:
#line 1690 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6349 "ds_parser.cpp"
    break;

  case 324:
#line 1694 "ds_parser.ypp"
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
#line 6390 "ds_parser.cpp"
    break;

  case 325:
#line 1733 "ds_parser.ypp"
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
#line 6458 "ds_parser.cpp"
    break;

  case 326:
#line 1799 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6470 "ds_parser.cpp"
    break;

  case 327:
#line 1806 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6481 "ds_parser.cpp"
    break;

  case 328:
#line 1815 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6487 "ds_parser.cpp"
    break;

  case 329:
#line 1816 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6493 "ds_parser.cpp"
    break;

  case 330:
#line 1817 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6499 "ds_parser.cpp"
    break;

  case 331:
#line 1818 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6505 "ds_parser.cpp"
    break;

  case 332:
#line 1819 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6511 "ds_parser.cpp"
    break;

  case 333:
#line 1820 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6517 "ds_parser.cpp"
    break;

  case 334:
#line 1821 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6523 "ds_parser.cpp"
    break;

  case 335:
#line 1822 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 6529 "ds_parser.cpp"
    break;

  case 336:
#line 1823 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 6535 "ds_parser.cpp"
    break;

  case 337:
#line 1824 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6541 "ds_parser.cpp"
    break;

  case 338:
#line 1825 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6547 "ds_parser.cpp"
    break;

  case 339:
#line 1826 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6553 "ds_parser.cpp"
    break;

  case 340:
#line 1827 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 6559 "ds_parser.cpp"
    break;

  case 341:
#line 1828 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 6565 "ds_parser.cpp"
    break;

  case 342:
#line 1829 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 6571 "ds_parser.cpp"
    break;

  case 343:
#line 1830 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 6577 "ds_parser.cpp"
    break;

  case 344:
#line 1831 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 6583 "ds_parser.cpp"
    break;

  case 345:
#line 1832 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 6589 "ds_parser.cpp"
    break;

  case 346:
#line 1833 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 6595 "ds_parser.cpp"
    break;

  case 347:
#line 1834 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 6601 "ds_parser.cpp"
    break;

  case 348:
#line 1835 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 6607 "ds_parser.cpp"
    break;

  case 349:
#line 1836 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 6613 "ds_parser.cpp"
    break;

  case 350:
#line 1837 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 6619 "ds_parser.cpp"
    break;

  case 351:
#line 1838 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 6625 "ds_parser.cpp"
    break;

  case 352:
#line 1842 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6631 "ds_parser.cpp"
    break;

  case 353:
#line 1843 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6637 "ds_parser.cpp"
    break;

  case 354:
#line 1844 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6643 "ds_parser.cpp"
    break;

  case 355:
#line 1845 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 6649 "ds_parser.cpp"
    break;

  case 356:
#line 1846 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 6655 "ds_parser.cpp"
    break;

  case 357:
#line 1847 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 6661 "ds_parser.cpp"
    break;

  case 358:
#line 1851 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6674 "ds_parser.cpp"
    break;

  case 359:
#line 1862 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6683 "ds_parser.cpp"
    break;

  case 360:
#line 1866 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6695 "ds_parser.cpp"
    break;

  case 361:
#line 1876 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6701 "ds_parser.cpp"
    break;

  case 362:
#line 1877 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6707 "ds_parser.cpp"
    break;

  case 363:
#line 1878 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6713 "ds_parser.cpp"
    break;

  case 364:
#line 1879 "ds_parser.ypp"
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
#line 6732 "ds_parser.cpp"
    break;

  case 365:
#line 1893 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6743 "ds_parser.cpp"
    break;

  case 366:
#line 1899 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6752 "ds_parser.cpp"
    break;

  case 367:
#line 1903 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6762 "ds_parser.cpp"
    break;

  case 368:
#line 1908 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6772 "ds_parser.cpp"
    break;

  case 369:
#line 1913 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6782 "ds_parser.cpp"
    break;

  case 370:
#line 1918 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6794 "ds_parser.cpp"
    break;

  case 371:
#line 1925 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6804 "ds_parser.cpp"
    break;

  case 372:
#line 1930 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6813 "ds_parser.cpp"
    break;

  case 373:
#line 1934 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6822 "ds_parser.cpp"
    break;

  case 374:
#line 1938 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6832 "ds_parser.cpp"
    break;

  case 375:
#line 1943 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6842 "ds_parser.cpp"
    break;

  case 376:
#line 1948 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6854 "ds_parser.cpp"
    break;

  case 377:
#line 1955 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6864 "ds_parser.cpp"
    break;

  case 378:
#line 1960 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6875 "ds_parser.cpp"
    break;

  case 379:
#line 1966 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6885 "ds_parser.cpp"
    break;

  case 380:
#line 1971 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6894 "ds_parser.cpp"
    break;

  case 381:
#line 1975 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6904 "ds_parser.cpp"
    break;

  case 382:
#line 1980 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6918 "ds_parser.cpp"
    break;

  case 383:
#line 1989 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6927 "ds_parser.cpp"
    break;

  case 384:
#line 1993 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6937 "ds_parser.cpp"
    break;

  case 385:
#line 1998 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6951 "ds_parser.cpp"
    break;

  case 386:
#line 2007 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6960 "ds_parser.cpp"
    break;

  case 387:
#line 2011 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6970 "ds_parser.cpp"
    break;

  case 388:
#line 2016 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6984 "ds_parser.cpp"
    break;

  case 389:
#line 2025 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6995 "ds_parser.cpp"
    break;

  case 390:
#line 2031 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7006 "ds_parser.cpp"
    break;

  case 391:
#line 2040 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7012 "ds_parser.cpp"
    break;

  case 392:
#line 2041 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7018 "ds_parser.cpp"
    break;

  case 393:
#line 2042 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7024 "ds_parser.cpp"
    break;

  case 394:
#line 2043 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7030 "ds_parser.cpp"
    break;

  case 395:
#line 2047 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7042 "ds_parser.cpp"
    break;

  case 396:
#line 2054 "ds_parser.ypp"
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7053 "ds_parser.cpp"
    break;

  case 397:
#line 2063 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7063 "ds_parser.cpp"
    break;

  case 398:
#line 2068 "ds_parser.ypp"
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7072 "ds_parser.cpp"
    break;

  case 399:
#line 2075 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7082 "ds_parser.cpp"
    break;

  case 400:
#line 2080 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructure();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7093 "ds_parser.cpp"
    break;

  case 401:
#line 2086 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7104 "ds_parser.cpp"
    break;

  case 402:
#line 2092 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7116 "ds_parser.cpp"
    break;

  case 403:
#line 2099 "ds_parser.ypp"
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7129 "ds_parser.cpp"
    break;

  case 404:
#line 2110 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7137 "ds_parser.cpp"
    break;

  case 405:
#line 2113 "ds_parser.ypp"
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
#line 7153 "ds_parser.cpp"
    break;

  case 406:
#line 2127 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7164 "ds_parser.cpp"
    break;

  case 407:
#line 2136 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7170 "ds_parser.cpp"
    break;

  case 408:
#line 2137 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7176 "ds_parser.cpp"
    break;

  case 409:
#line 2141 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7186 "ds_parser.cpp"
    break;

  case 410:
#line 2146 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7195 "ds_parser.cpp"
    break;

  case 411:
#line 2153 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7205 "ds_parser.cpp"
    break;

  case 412:
#line 2158 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7218 "ds_parser.cpp"
    break;

  case 413:
#line 2169 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7228 "ds_parser.cpp"
    break;

  case 414:
#line 2174 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7237 "ds_parser.cpp"
    break;

  case 415:
#line 2181 "ds_parser.ypp"
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7251 "ds_parser.cpp"
    break;

  case 416:
#line 2193 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7257 "ds_parser.cpp"
    break;

  case 417:
#line 2194 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7263 "ds_parser.cpp"
    break;

  case 418:
#line 2198 "ds_parser.ypp"
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
#line 7282 "ds_parser.cpp"
    break;

  case 419:
#line 2212 "ds_parser.ypp"
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
#line 7301 "ds_parser.cpp"
    break;


#line 7305 "ds_parser.cpp"

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
#line 2228 "ds_parser.ypp"


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

void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames ) {
    bool anyNames = false;
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


