/* A Bison parser, made by GNU Bison 3.2.2.  */

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
#define YYBISON_VERSION "3.2.2"

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
#line 43 "src/parser/ds_parser.ypp" /* yacc.c:338  */

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

#line 100 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_DAS_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_DAS_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
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
#line 2 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 182 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

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
#line 67 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 347 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
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

#endif /* !YY_DAS_YY_C_USERS_BORIS_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   4103

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  87
/* YYNRULES -- Number of rules.  */
#define YYNRULES  339
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  610

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
       0,   255,   255,   257,   258,   259,   260,   261,   262,   263,
     264,   268,   269,   273,   277,   280,   289,   297,   313,   334,
     338,   339,   343,   353,   357,   358,   362,   365,   371,   372,
     373,   380,   387,   399,   409,   418,   419,   420,   421,   422,
     423,   427,   432,   440,   441,   445,   452,   464,   468,   475,
     476,   480,   481,   482,   486,   490,   497,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   540,   609,   612,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   638,
     642,   648,   654,   668,   669,   675,   679,   685,   688,   691,
     695,   701,   705,   709,   712,   715,   723,   729,   730,   734,
     735,   739,   771,   774,   779,   787,   791,   796,   800,   808,
     811,   817,   822,   873,   874,   875,   876,   877,   878,   882,
     883,   884,   885,   886,   887,   888,   889,   890,   891,   892,
     893,   894,   895,   896,   897,   901,   912,   917,   927,   928,
     929,   930,   931,   932,   933,   934,   935,   936,   937,   938,
     939,   940,   941,   942,   943,   944,   945,   946,   947,   948,
     949,   950,   951,   952,   953,   954,   955,   956,   957,   958,
     959,   960,   961,   962,   963,   964,   965,   969,   973,   976,
     979,   980,   981,   982,   983,   984,   987,   988,   989,   990,
     991,   992,   993,  1003,  1013,  1047,  1048,  1052,  1053,  1057,
    1065,  1068,  1075,  1076,  1080,  1081,  1085,  1091,  1092,  1096,
    1097,  1101,  1105,  1108,  1112,  1117,  1125,  1128,  1132,  1136,
    1141,  1149,  1177,  1180,  1189,  1201,  1213,  1227,  1228,  1232,
    1271,  1337,  1344,  1353,  1354,  1355,  1356,  1357,  1358,  1359,
    1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,
    1370,  1371,  1372,  1373,  1374,  1375,  1376,  1380,  1390,  1394,
    1404,  1405,  1406,  1407,  1421,  1427,  1431,  1436,  1441,  1446,
    1451,  1458,  1462,  1467,  1473,  1478,  1482,  1487,  1496,  1500,
    1505,  1514,  1518,  1523,  1532,  1541,  1542,  1543,  1544,  1548,
    1555,  1564,  1569,  1576,  1581,  1590,  1593,  1607,  1616,  1617,
    1621,  1626,  1633,  1638,  1649,  1654,  1661,  1673,  1674,  1678
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
  "require_list", "require_module", "expect_declaration", "expect_list",
  "expect_error", "expression_else", "expression_if_then_else",
  "expression_for_loop", "expression_while_loop", "expression_with",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
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

#define YYPACT_NINF -403

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-403)))

#define YYTABLE_NINF -150

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -403,    19,  -403,  -403,   -79,   -70,   -62,    -7,    -1,  -403,
      -8,  -403,  -403,  -403,    70,  -403,    55,  -403,  -403,  -403,
    -403,   -24,  -403,    23,    26,    35,  -403,  -403,    96,  -403,
      81,   103,  -403,    86,   122,    98,  -403,   -31,  -403,   135,
      -3,  -403,   267,   -79,   144,   -70,   -62,  -403,  2663,   152,
    -403,   -79,    -8,  -403,   137,   119,  3471,   243,   244,  -403,
     127,   -93,  -403,  -403,  -403,  -403,  -403,   158,  -403,  -403,
    -403,  -403,  -403,   -85,   150,   153,   157,  -403,  -403,  -403,
     141,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,   162,  -403,  -403,  -403,   163,
     165,  -403,  -403,  -403,  -403,   167,  -403,  -403,  -403,  -403,
     206,  -403,   -29,  -403,   -83,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,   -12,   174,   -79,  -403,   261,   151,  -403,
     146,   -78,  -403,  2663,  2663,  2663,   188,  2593,  2593,  2593,
    2663,  -403,    -2,  -403,  -403,  1495,  -403,  -403,    43,   192,
    -403,  -403,  -403,    39,  -403,   -53,  2663,   159,    27,  -403,
     192,  -403,  -403,  -403,   200,  -403,   142,     1,   173,   160,
     174,   196,   174,   199,   174,   209,  -403,   -94,   206,   184,
    -403,   168,  -403,  -403,  2523,   169,  -403,   170,   187,   202,
     177,   203,   180,  2415,  2415,   619,  2663,  2415,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  2415,  2415,   -83,  2415,  2415,
    -403,  2415,  -403,   175,  -403,  -403,   -84,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  3493,   190,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,   -28,  -403,   219,  -403,  2663,  2415,
     206,  -403,  -403,  -403,   197,  -403,  2663,  -403,  -403,   211,
    -403,   213,  -403,   217,  -403,  -403,  2663,  -403,  -403,    67,
    -403,   233,  2415,  2663,  2663,  2415,  2663,  1624,   -35,   -35,
     192,   995,  1124,  3621,  -403,   -55,   138,   -35,   -35,  -403,
     -35,   -35,  3067,   700,   127,  -403,  1253,  2415,  2415,  -403,
    -403,  2415,  2415,  2415,  2415,   234,  2415,   235,  2415,  2415,
    2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,
    2415,  2415,  2415,   240,  2415,  -403,  1737,  -403,  -403,    74,
    -403,   259,  2821,  -403,   216,  -403,  -403,  -403,  -403,  1850,
    1366,  3133,   250,   262,  3199,   265,  -403,  3265,     5,   -76,
    1963,  3621,   238,   -50,   239,  -403,  -403,    72,   -39,  2415,
    2415,   210,  -403,  2415,   246,  -403,  -403,  2415,  2415,   192,
    2076,  -403,   159,  2415,  2415,  -403,  -403,   335,  -403,  -403,
    -403,  -403,  -403,  -403,   221,   222,   224,   228,  -403,   364,
    -403,   -71,  2892,   174,   269,  -403,   -16,  3789,   405,   405,
    3958,  3958,  3938,  3938,   249,   -35,  -403,   -56,   -56,   405,
     405,  3677,  3809,  3865,  3885,  3958,  3958,   342,   342,   459,
     459,   459,  -403,  3557,  -403,    61,  2415,    -6,  2415,  2415,
    2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,  2415,
    2415,  2415,  2415,  -403,  -403,    63,  2663,   272,  3331,  -403,
    2415,  2415,  -403,  2415,  -403,  2415,  2415,   269,   275,   247,
     269,  2415,   251,  2415,   253,  3789,  -403,  -403,  3733,  2948,
    2948,     6,  2415,  3789,   389,  3789,  2948,   254,  -403,  -403,
    -403,  -403,   367,   192,  -403,   159,   -37,    64,  2415,  -403,
    2189,  2415,  -403,  -403,  3789,  -403,  3789,  3789,  3789,  3789,
    3789,  3789,  3789,  3789,  3789,  3789,  3789,  3789,  3789,  3789,
    3789,  -403,    38,   279,  -403,  3958,  3958,  3958,   -73,  3789,
      94,   -37,  -403,   238,  3789,  -403,  -403,  -403,  -403,  -403,
     131,  2415,  3789,   159,  -403,  -403,  -403,  -403,   130,  -403,
     256,  3789,  -403,    82,  3789,  -403,  2302,  2415,   263,  2415,
     159,  2415,  -403,   -66,  -403,   853,  2663,  2415,  -403,  -403,
    2663,  3397,  3011,  -403,  3789,  -403,  2948,   159,  -403,    48,
    -403,  2669,    54,  -403,   368,   268,   131,  -403,  -403,  2415,
    -403,  -403,  2415,   270,  -403,  -403,  2745,  3789,  -403,  -403
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,   129,     0,     0,     0,     0,     0,   130,
       0,     9,     8,     7,     0,     6,     0,     5,    10,     4,
       3,     0,    41,    18,    26,    23,    24,    22,    19,    20,
       0,     0,    44,   113,     0,    45,    47,     0,    43,     0,
       0,   230,     0,     0,     0,     0,     0,   252,     0,     0,
     115,     0,     0,    50,   257,     0,     0,     0,     0,    56,
      51,   225,    39,    40,    37,    38,    36,     0,    35,    42,
      27,    25,    21,     0,     0,     0,     0,   263,   283,   264,
     288,   265,   269,   270,   271,   272,   276,   277,   278,   279,
     280,   281,   282,   284,   285,   305,   268,   275,   286,   308,
     311,   266,   273,   267,   274,     0,   287,   290,   292,   291,
     255,   114,     0,    48,     0,   259,   230,    59,    60,    62,
      61,    63,    64,    65,    66,    85,    86,    83,    84,    76,
      87,    88,    77,    74,    75,    89,    90,    91,    92,    79,
      80,    78,    72,    73,    68,    67,    69,    70,    71,    58,
      57,    81,    82,     0,    54,     0,   251,   227,     0,    11,
       0,     0,   256,     0,     0,     0,     0,    51,    51,    51,
       0,   296,     0,   301,   298,     0,    46,   258,   225,     0,
     261,    52,   234,     0,   232,   241,     0,     0,     0,   228,
       0,   231,    12,    13,     0,   253,     0,     0,     0,     0,
      54,     0,    54,     0,    54,     0,   237,     0,   236,   297,
     299,     0,   173,   174,     0,     0,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,   145,
     144,   146,   147,   148,    14,     0,     0,     0,     0,     0,
     294,     0,   109,    49,   171,   141,   169,   218,   217,   216,
     221,   170,   220,   219,     0,     0,   172,   315,   316,   317,
     318,   260,   233,    53,     0,   240,     0,   239,     0,     0,
      55,    93,   226,   229,     0,   302,     0,   304,   289,     0,
     306,     0,   309,     0,   312,   314,     0,   300,   295,   117,
     120,     0,     0,     0,     0,     0,     0,     0,   197,   198,
       0,     0,     0,     0,   334,     0,     0,   178,   177,   205,
     176,   175,     0,     0,    51,   224,     0,     0,     0,   199,
     200,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   293,     0,   235,   262,   242,
     245,     0,   244,   254,     0,   307,   310,   313,   238,     0,
       0,     0,     0,     0,     0,     0,   212,     0,     0,   113,
       0,   325,   321,     0,   328,   329,   330,     0,     0,     0,
       0,     0,    17,     0,    15,   201,   111,     0,     0,     0,
     123,   121,     0,     0,     0,   122,    96,    94,   107,   103,
     101,   102,   110,    97,     0,     0,     0,     0,   108,   127,
     100,     0,   149,    54,     0,   206,     0,   139,   179,   180,
     192,   193,   190,   191,     0,   214,   204,   222,   223,   181,
     182,     0,   195,   196,   194,   188,   189,   184,   183,   185,
     186,   187,   203,     0,   208,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   303,   118,     0,     0,     0,     0,   210,
       0,     0,   211,     0,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   335,   336,     0,     0,
       0,     0,     0,   124,     0,   116,     0,     0,    99,   104,
     105,   106,     0,     0,    98,     0,     0,     0,     0,   207,
       0,     0,   202,   209,   243,   112,   156,   157,   159,   158,
     160,   153,   154,   155,   161,   162,   151,   152,   163,   164,
     150,   119,     0,     0,   135,   132,   133,   134,     0,   319,
       0,     0,   323,   322,   326,   332,   331,   333,    16,    33,
      28,     0,   125,     0,    34,   109,   128,   131,     0,   142,
       0,   140,   166,     0,   215,   137,     0,     0,     0,     0,
       0,     0,    31,     0,   126,     0,     0,     0,   165,   167,
       0,     0,   337,   324,   320,    29,     0,     0,    95,     0,
     250,   149,     0,   136,     0,     0,    28,    32,   246,     0,
     249,   138,     0,     0,    30,   248,   149,   338,   339,   247
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -403,  -403,   105,  -403,  -403,  -403,  -403,  -403,  -403,   369,
    -403,  -403,   372,  -178,  -403,  -403,  -403,  -403,   370,   -17,
    -403,   371,  -403,   179,  -152,  -193,  -403,  -403,  -184,  -403,
    -135,  -264,   -10,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
     424,  -403,  -403,  -403,  -338,   -18,  -403,  -307,  -403,  -403,
     129,  -403,  -403,  -403,   317,   172,  -403,   148,  -403,  -183,
     -88,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -296,
     -47,  -403,  -403,   -34,   226,  -402,   -33,  -403,  -403,  -217,
     -40,   143,  -403,  -403,  -403,  -403,  -403
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   160,    68,   306,   244,    11,    12,    28,    29,
      13,    25,    26,   572,   398,   399,   400,   401,    22,    23,
      35,    36,    37,    14,   154,   187,    60,    15,   245,   402,
     313,   403,   246,   404,   247,   405,   406,   407,   408,   503,
     409,   410,   248,   249,   416,   250,   251,   351,   252,   253,
     417,   157,   190,   158,    61,   182,   183,   206,   207,   476,
     184,   557,    17,    73,    18,    19,   115,    55,    20,   185,
     255,   108,   109,   208,   256,   372,   373,   257,   374,   375,
     376,   377,   258,   305,   259,   595,   260
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,   107,   269,   271,   368,   350,   411,   279,   445,   281,
     304,   283,   507,   179,   110,   200,   202,   204,   265,     2,
      32,   465,   475,   551,     3,   209,    56,   447,   171,   179,
     161,   315,    33,   172,   112,   285,    21,   325,   106,    24,
     327,   265,    38,     4,   194,     5,   267,     6,   508,     7,
     319,   320,   155,    27,     8,   508,   286,   265,   325,   156,
       9,   327,   328,   316,   329,   171,    34,   162,   266,   267,
     172,   268,   195,    49,    39,   171,    40,   567,   543,   504,
     172,   171,    57,    58,   587,   267,   172,   180,   343,   344,
      52,   262,    43,   491,   171,   380,   479,   381,    42,   172,
     480,   171,   273,   180,   177,   508,   172,    33,    30,   343,
     344,   483,    59,   484,    31,    53,   107,   107,   107,   176,
     107,   107,   107,   107,   173,   210,   266,   266,   174,   196,
     197,   198,   509,   201,   203,   205,   181,   538,   188,   107,
     570,    34,   265,   211,    43,   242,   175,   243,    43,   571,
      44,   276,   270,   106,   106,   106,    45,   106,   106,   106,
     106,   173,   413,   486,    10,   174,   446,   107,   265,   171,
     267,   173,   563,   272,   172,   174,   106,   173,   107,   107,
     289,   174,   508,   175,   508,   478,   565,   263,   155,   264,
     173,   301,   302,   175,   174,   261,   267,   173,   598,   175,
     171,   174,   601,   508,   106,   172,    41,   558,   494,   513,
     560,   531,   175,   573,   359,   106,   106,    46,   482,   175,
     505,   107,   483,   171,   265,    48,   171,   309,   172,   107,
     579,   172,    47,   171,   349,    49,   171,    50,   172,   107,
     568,   172,   354,   171,   480,    51,   107,   107,   172,   107,
      54,   266,   267,    70,   576,   159,   382,   383,   106,   362,
     363,   114,   365,   192,   193,   173,   106,   111,   411,   174,
     116,   275,   151,   152,   153,   159,   106,   171,   163,    62,
      63,   164,   172,   106,   106,   165,   106,   175,   166,   171,
     167,   168,   171,   169,   172,   170,   173,   172,   186,   189,
     174,   191,   277,   199,   254,   549,   550,   180,   278,   274,
     242,   287,   554,   590,   288,   293,   291,   292,   175,   173,
      10,   559,   173,   174,   295,   280,   174,   297,   282,   173,
     294,   296,   173,   174,   348,   605,   174,   346,   284,   173,
     355,   175,   356,   174,   175,   463,   357,   353,   360,   424,
     426,   175,   298,   299,   175,   442,   303,   447,   569,   478,
     481,   175,   487,   192,   307,   308,   497,   310,   311,   574,
     312,   498,   499,   173,   500,   577,    64,   174,   501,   470,
      65,   502,    66,    67,   506,   173,   585,   533,   173,   174,
     541,   471,   174,   542,   473,   175,   510,   545,   352,   547,
     553,   556,   596,   597,   578,   555,   599,   175,   566,   583,
     175,   384,   602,    69,   603,    72,   608,    71,   604,   107,
     575,   361,   314,   113,   364,    16,   367,   319,   320,   515,
     371,   371,   532,   178,   358,   325,   347,   326,   327,   328,
     290,   329,   412,   546,   540,   378,   418,   419,     0,     0,
     420,   421,   422,   423,     0,   425,   106,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,     0,   443,   340,   341,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   344,     0,   468,
     319,   320,     0,     0,     0,     0,     0,     0,   325,   312,
     326,   327,   328,     0,   329,     0,     0,     0,   485,   303,
       0,     0,   488,     0,     0,     0,   489,   490,     0,   493,
       0,     0,   495,   496,     0,     0,     0,     0,     0,   107,
       0,     0,     0,   107,     0,   338,   339,   340,   341,   342,
       0,     0,   589,     0,   319,   320,   592,     0,     0,   343,
     344,     0,   325,     0,   326,   327,   328,     0,   329,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
     106,     0,     0,     0,     0,   514,     0,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,     0,     0,     0,     0,     0,     0,     0,   535,
     536,     0,   537,   343,   344,   539,     0,     0,     0,     0,
     544,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,   552,     0,     0,     0,     0,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,   561,    74,     0,
     564,     0,     0,     0,    75,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,     0,   581,   582,     0,   584,     0,
     586,   386,     0,     0,   412,     3,   591,   387,   388,     0,
     389,     0,   212,   213,   214,   215,     0,     0,     0,     0,
     390,   216,   391,   392,     0,     0,     0,     0,   606,     0,
       0,   607,   393,   217,    33,     0,   394,   218,     0,   219,
       0,     9,   220,   395,     0,   221,    77,    78,    79,     0,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,     0,    96,    97,    98,    34,   222,
     101,   102,   103,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   225,   226,   227,   228,
     229,   230,   231,   232,   233,    33,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     235,   236,     0,     0,     0,   237,     0,     0,   238,   239,
       0,     0,     0,     0,     0,     0,     0,   241,     0,    34,
     396,   242,   397,   243,   386,     0,     0,     0,     3,     0,
     387,   388,     0,   389,     0,   212,   213,   214,   215,     0,
       0,     0,     0,   390,   216,   391,   392,     0,     0,     0,
       0,     0,     0,     0,     0,   393,   217,     0,     0,   394,
     218,     0,   219,     0,     9,   220,   395,     0,   221,    77,
      78,    79,     0,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,     0,    96,    97,
      98,     0,   222,   101,   102,   103,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
     226,   227,   228,   229,   230,   231,   232,   233,    33,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   235,   236,     0,     0,     0,   237,     0,
       0,   238,   239,     0,     0,     0,     0,     0,     0,     0,
     241,     0,    34,   396,   242,   588,   243,   212,   213,   214,
     215,     0,     0,     0,     0,     0,   216,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,   172,   217,     0,
       0,     0,   218,     0,   219,     0,     0,   220,     0,     0,
     221,    77,    78,    79,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      96,    97,    98,     0,   222,   101,   102,   103,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     369,   234,     0,     0,     0,     0,     0,     0,   173,     0,
       0,     0,   174,     0,     0,   235,   236,     0,     0,     0,
     237,     0,     0,   238,   239,     0,   212,   213,   214,   215,
     175,     0,   370,     0,    34,   216,   242,     0,   243,     0,
       0,   171,     0,     0,     0,     0,   172,   217,     0,     0,
       0,   218,     0,   219,     0,     0,   220,     0,     0,   221,
      77,    78,    79,     0,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,    96,
      97,    98,     0,   222,   101,   102,   103,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,   227,   228,   229,   230,   231,   232,   233,    33,
     234,     0,     0,     0,     0,     0,     0,   173,     0,     0,
       0,   174,     0,     0,   235,   236,     0,     0,     0,   237,
       0,     0,   238,   239,     0,   212,   213,   214,   215,   175,
       0,   241,     0,    34,   216,   242,     0,   243,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,     0,
     218,     0,   219,     0,     0,   220,     0,     0,   221,    77,
      78,    79,     0,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,     0,    96,    97,
      98,     0,   222,   101,   102,   103,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
     226,   227,   228,   229,   230,   231,   232,   233,    33,   234,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
     214,   215,   466,   235,   236,     0,     0,   216,   237,     0,
       0,   238,   239,     0,     0,     0,     0,     0,   414,   217,
     241,   415,    34,   218,   242,   219,   243,     0,   220,     0,
       0,   221,    77,    78,    79,     0,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
       0,    96,    97,    98,     0,   222,   101,   102,   103,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   223,   224,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   226,   227,   228,   229,   230,   231,   232,
     233,    33,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   467,     0,   235,   236,     0,     0,
       0,   237,     0,     0,   238,   239,     0,   212,   213,   214,
     215,     0,     0,   241,     0,    34,   216,   242,     0,   243,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
       0,     0,   218,     0,   219,     0,     0,   220,     0,     0,
     221,    77,    78,    79,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      96,    97,    98,     0,   222,   101,   102,   103,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,   226,   227,   228,   229,   230,   231,   232,   233,
      33,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,     0,     0,     0,
     237,     0,     0,   238,   239,     0,   212,   213,   214,   215,
       0,   240,   241,     0,    34,   216,   242,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,   217,     0,     0,
       0,   218,     0,   219,     0,     0,   220,     0,     0,   221,
      77,    78,    79,     0,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,    96,
      97,    98,     0,   222,   101,   102,   103,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,   227,   228,   229,   230,   231,   232,   233,    33,
     234,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     213,   214,   215,     0,   235,   236,     0,     0,   216,   237,
       0,     0,   238,   239,     0,     0,     0,     0,     0,     0,
     217,   241,   366,    34,   218,   242,   219,   243,     0,   220,
       0,     0,   221,    77,    78,    79,     0,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,     0,    96,    97,    98,     0,   222,   101,   102,   103,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   223,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,   227,   228,   229,   230,   231,
     232,   233,    33,   234,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,   214,   215,     0,   235,   236,     0,
       0,   216,   237,     0,     0,   238,   239,     0,     0,     0,
       0,     0,     0,   217,   241,   444,    34,   218,   242,   219,
     243,     0,   220,     0,     0,   221,    77,    78,    79,     0,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,     0,    96,    97,    98,     0,   222,
     101,   102,   103,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   225,   226,   227,   228,
     229,   230,   231,   232,   233,    33,   234,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,   214,   215,     0,
     235,   236,     0,     0,   216,   237,     0,     0,   238,   239,
       0,     0,     0,     0,     0,     0,   217,   241,   464,    34,
     218,   242,   219,   243,     0,   220,     0,     0,   221,    77,
      78,    79,     0,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,     0,    96,    97,
      98,     0,   222,   101,   102,   103,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
     226,   227,   228,   229,   230,   231,   232,   233,    33,   234,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
     214,   215,     0,   235,   236,     0,     0,   216,   237,     0,
       0,   238,   239,     0,     0,     0,     0,     0,     0,   217,
     241,   477,    34,   218,   242,   219,   243,     0,   220,     0,
       0,   221,    77,    78,    79,     0,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
       0,    96,    97,    98,     0,   222,   101,   102,   103,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   223,   224,     0,     0,     0,     0,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   226,   227,   228,   229,   230,   231,   232,
     233,    33,   234,     0,     0,     0,     0,     0,     0,     0,
       0,   212,   213,   214,   215,     0,   235,   236,     0,     0,
     216,   237,     0,     0,   238,   239,     0,     0,     0,     0,
       0,     0,   217,   241,     0,    34,   218,   242,   219,   243,
       0,   220,     0,     0,   221,    77,    78,    79,     0,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,     0,    96,    97,    98,     0,   222,   101,
     102,   103,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   223,   224,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   225,   226,   227,   228,   229,
     230,   231,   232,   233,    33,   234,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,   214,   215,   580,   235,
     236,     0,     0,   216,   237,     0,     0,   238,   239,     0,
       0,     0,     0,     0,     0,   217,   241,   562,    34,   218,
     242,   219,   243,     0,   220,     0,     0,   221,    77,    78,
      79,     0,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,     0,    96,    97,    98,
       0,   222,   101,   102,   103,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,   224,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
     227,   228,   229,   230,   231,   232,   233,    33,   234,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   213,   214,
     215,     0,   235,   236,     0,     0,   216,   237,     0,     0,
     238,   239,     0,     0,     0,     0,     0,     0,   217,   241,
       0,    34,   218,   242,   219,   243,     0,   220,     0,     0,
     221,    77,    78,    79,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      96,    97,    98,     0,   222,   101,   102,   103,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,   226,   227,   228,   229,   230,   231,   232,   233,
      33,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,   235,   236,     0,    75,     0,
     237,     0,     0,   238,   239,     0,     0,     0,     0,     0,
       0,     0,   241,    76,    34,     0,   242,     0,   243,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
     226,   227,     0,    76,     0,     0,     0,     0,    33,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,     0,     0,     0,     0,    33,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,     0,
     153,     0,    34,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   317,   318,   319,   320,   321,   456,   457,   322,
     323,   324,   325,   458,   326,   327,   328,     0,   329,   459,
     330,   331,   460,   461,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   462,   332,     0,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,     0,     0,     0,     0,     0,
       0,     0,    34,   343,   344,     0,     0,     0,     0,   600,
     448,   449,   450,   451,   452,   453,   454,   455,   317,   318,
     319,   320,   321,   456,   457,   322,   323,   324,   325,   458,
     326,   327,   328,     0,   329,   459,   330,   331,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   462,   332,     0,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,     0,     0,     0,   609,   448,   449,   450,   451,
     452,   453,   454,   455,   317,   318,   319,   320,   321,   456,
     457,   322,   323,   324,   325,   458,   326,   327,   328,  -149,
     329,   459,   330,   331,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   462,   332,     0,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,   448,   449,   450,
     451,   452,   453,   454,   455,   317,   318,   319,   320,   321,
     456,   457,   322,   323,   324,   325,   458,   326,   327,   328,
       0,   329,   459,   330,   331,   460,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   462,   332,     0,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,     0,     0,
       0,   317,   318,   319,   320,   321,   343,   344,   322,   323,
     324,   325,     0,   326,   327,   328,     0,   329,     0,   330,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344,   317,   318,   319,   320,   321,   242,
       0,   322,   323,   324,   325,     0,   326,   327,   328,     0,
     329,     0,   330,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
     317,   318,   319,   320,   321,   343,   344,   322,   323,   324,
     325,   594,   326,   327,   328,     0,   329,     0,   330,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   344,     0,     0,   385,   317,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,     0,   326,   327,
     328,     0,   329,     0,   330,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   344,     0,
       0,   469,   317,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,     0,   326,   327,   328,     0,   329,     0,
     330,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   344,     0,     0,   472,   317,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,     0,
     326,   327,   328,     0,   329,     0,   330,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,     0,   474,   317,   318,   319,   320,   321,     0,
       0,   322,   323,   324,   325,     0,   326,   327,   328,     0,
     329,     0,   330,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   344,     0,     0,   534,
     317,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,     0,   326,   327,   328,     0,   329,     0,   330,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   344,     0,     0,   593,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,   135,   136,   137,   138,   317,   318,   319,   320,
     321,     0,     0,   322,   323,   324,   325,     0,   326,   327,
     328,     0,   329,     0,   330,   331,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,     0,   149,
     150,     0,     0,     0,     0,     0,   332,     0,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   344,   345,
     317,   318,   319,   320,   321,     0,     0,   322,   323,   324,
     325,     0,   326,   327,   328,     0,   329,     0,   330,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   344,   512,   317,   318,   319,   320,   321,     0,
       0,   322,   323,   324,   325,     0,   326,   327,   328,     0,
     329,     0,   330,   331,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
     317,   318,   319,   320,   321,   343,   344,   322,   323,   324,
     325,     0,   326,   327,   328,     0,   329,     0,   330,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   511,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,     0,     0,     0,   317,   318,   319,   320,
     321,   343,   344,   322,   323,   324,   325,     0,   326,   327,
     328,     0,   329,     0,   330,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   548,     0,     0,   332,     0,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,     0,
       0,     0,   317,   318,   319,   320,   321,   343,   344,   322,
     323,   324,   325,     0,   326,   327,   328,     0,   329,     0,
     330,   331,   317,   318,   319,   320,   321,     0,     0,   322,
     323,   324,   325,     0,   326,   327,   328,     0,   329,     0,
     330,   331,   332,     0,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   344,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,     0,     0,     0,   317,   318,
     319,   320,   321,   343,   344,   322,   323,   324,   325,     0,
     326,   327,   328,     0,   329,     0,   330,   331,   317,   318,
     319,   320,   321,     0,     0,   322,   323,   324,   325,     0,
     326,   327,   328,     0,   329,     0,   330,   331,     0,     0,
       0,     0,   335,   336,   337,   338,   339,   340,   341,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     344,     0,     0,   336,   337,   338,   339,   340,   341,   342,
       0,   317,   318,   319,   320,   321,     0,     0,   322,   343,
     344,   325,     0,   326,   327,   328,     0,   329,     0,   330,
     331,   317,   318,   319,   320,     0,     0,     0,     0,     0,
       0,   325,     0,   326,   327,   328,     0,   329,     0,   330,
     331,     0,     0,     0,     0,     0,   336,   337,   338,   339,
     340,   341,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344,     0,     0,     0,     0,   338,   339,
     340,   341,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,   344
};

static const yytype_int16 yycheck[] =
{
      10,    48,   185,   187,   300,   269,   313,   200,   346,   202,
     227,   204,   414,    41,    48,   167,   168,   169,    94,     0,
      28,   359,    17,    17,     5,    27,    29,    98,    27,    41,
     115,   115,   115,    32,    51,   129,   115,    93,    48,   109,
      96,    94,    52,    24,   122,    26,   122,    28,   121,    30,
      85,    86,   145,   115,    35,   121,   150,    94,    93,   152,
      41,    96,    97,   147,    99,    27,   149,   152,   121,   122,
      32,   124,   150,   149,     4,    27,     6,   150,   480,   150,
      32,    27,    85,    86,   150,   122,    32,   115,   144,   145,
     121,   179,   121,   389,    27,   150,   146,   152,   122,    32,
     150,    27,   190,   115,   114,   121,    32,   115,   115,   144,
     145,   150,   115,   152,   115,   146,   163,   164,   165,   148,
     167,   168,   169,   170,   123,   127,   121,   121,   127,   163,
     164,   165,   148,   167,   168,   169,   148,   475,   155,   186,
       9,   149,    94,   145,   121,   151,   145,   153,   121,    18,
     124,   150,   186,   163,   164,   165,   121,   167,   168,   169,
     170,   123,   314,   380,   145,   127,   349,   214,    94,    27,
     122,   123,   510,   146,    32,   127,   186,   123,   225,   226,
     214,   127,   121,   145,   121,   121,   148,   148,   145,   150,
     123,   225,   226,   145,   127,   152,   122,   123,   150,   145,
      27,   127,   148,   121,   214,    32,   151,   503,   392,   148,
     146,   148,   145,   551,   147,   225,   226,   121,   146,   145,
     413,   268,   150,    27,    94,   122,    27,   237,    32,   276,
     148,    32,   151,    27,   268,   149,    27,   115,    32,   286,
     146,    32,   276,    27,   150,   147,   293,   294,    32,   296,
     115,   121,   122,   109,   124,   117,   118,   119,   268,   293,
     294,   124,   296,   117,   118,   123,   276,   115,   575,   127,
     151,   129,    29,    29,   147,   117,   286,    27,   128,    12,
      13,   128,    32,   293,   294,   128,   296,   145,   147,    27,
     128,   128,    27,   128,    32,   128,   123,    32,   124,    38,
     127,   150,   129,   115,   175,   489,   490,   115,   148,   109,
     151,   127,   496,   577,   146,   128,   147,   147,   145,   123,
     145,   505,   123,   127,   147,   129,   127,   147,   129,   123,
     128,   128,   123,   127,   115,   599,   127,   147,   129,   123,
     129,   145,   129,   127,   145,   129,   129,   150,   115,   115,
     115,   145,   223,   224,   145,   115,   227,    98,   541,   121,
     121,   145,   152,   117,   235,   236,    31,   238,   239,   553,
     241,   150,   150,   123,   150,   558,   109,   127,   150,   129,
     113,    17,   115,   116,   115,   123,   570,   115,   123,   127,
     115,   129,   127,   146,   129,   145,   147,   146,   269,   146,
      11,    34,   586,   587,   148,   151,   589,   145,   129,   146,
     145,   306,    44,    43,   146,    46,   146,    45,   596,   466,
     555,   292,   243,    52,   295,     1,   297,    85,    86,   447,
     301,   302,   466,   116,   286,    93,   264,    95,    96,    97,
     214,    99,   313,   483,   477,   302,   317,   318,    -1,    -1,
     321,   322,   323,   324,    -1,   326,   466,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,    -1,   344,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,   360,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    93,   370,
      95,    96,    97,    -1,    99,    -1,    -1,    -1,   379,   380,
      -1,    -1,   383,    -1,    -1,    -1,   387,   388,    -1,   390,
      -1,    -1,   393,   394,    -1,    -1,    -1,    -1,    -1,   576,
      -1,    -1,    -1,   580,    -1,   130,   131,   132,   133,   134,
      -1,    -1,   576,    -1,    85,    86,   580,    -1,    -1,   144,
     145,    -1,    93,    -1,    95,    96,    97,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   576,    -1,    -1,    -1,
     580,    -1,    -1,    -1,    -1,   446,    -1,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   470,
     471,    -1,   473,   144,   145,   476,    -1,    -1,    -1,    -1,
     481,    -1,   483,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   492,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,    19,    -1,
     511,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,   566,   567,    -1,   569,    -1,
     571,     1,    -1,    -1,   575,     5,   577,     7,     8,    -1,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,   599,    -1,
      -1,   602,    32,    33,   115,    -1,    36,    37,    -1,    39,
      -1,    41,    42,    43,    -1,    45,    46,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,   149,    69,
      70,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,    -1,    -1,   135,    -1,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
     150,   151,   152,   153,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    36,
      37,    -1,    39,    -1,    41,    42,    43,    -1,    45,    46,
      47,    48,    -1,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,   135,    -1,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,    -1,   149,   150,   151,   152,   153,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,   127,    -1,    -1,   130,   131,    -1,    -1,    -1,
     135,    -1,    -1,   138,   139,    -1,    12,    13,    14,    15,
     145,    -1,   147,    -1,   149,    21,   151,    -1,   153,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      46,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,   127,    -1,    -1,   130,   131,    -1,    -1,    -1,   135,
      -1,    -1,   138,   139,    -1,    12,    13,    14,    15,   145,
      -1,   147,    -1,   149,    21,   151,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    46,
      47,    48,    -1,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,   130,   131,    -1,    -1,    21,   135,    -1,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,   145,    33,
     147,   148,   149,    37,   151,    39,   153,    -1,    42,    -1,
      -1,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,   135,    -1,    -1,   138,   139,    -1,    12,    13,    14,
      15,    -1,    -1,   147,    -1,   149,    21,   151,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
     135,    -1,    -1,   138,   139,    -1,    12,    13,    14,    15,
      -1,   146,   147,    -1,   149,    21,   151,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      46,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,   130,   131,    -1,    -1,    21,   135,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      33,   147,   148,   149,    37,   151,    39,   153,    -1,    42,
      -1,    -1,    45,    46,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    -1,   130,   131,    -1,
      -1,    21,   135,    -1,    -1,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    33,   147,   148,   149,    37,   151,    39,
     153,    -1,    42,    -1,    -1,    45,    46,    47,    48,    -1,
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
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,   130,   131,    -1,    -1,
      21,   135,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    33,   147,    -1,   149,    37,   151,    39,   153,
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
      -1,    -1,    -1,    -1,    -1,    33,   147,   148,   149,    37,
     151,    39,   153,    -1,    42,    -1,    -1,    45,    46,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    19,    -1,    -1,   130,   131,    -1,    25,    -1,
     135,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    40,   149,    -1,   151,    -1,   153,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,    -1,    40,    -1,    -1,    -1,    -1,   115,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   115,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
     147,    -1,   149,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   144,   145,    -1,    -1,    -1,    -1,   150,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,    -1,    -1,   150,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    83,    84,    85,    86,    87,   144,   145,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    83,    84,    85,    86,    87,   151,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,   144,   145,    90,    91,    92,
      93,   150,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,   148,    83,    84,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,   148,    83,    84,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,    -1,    -1,   148,    83,    84,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,   148,    83,    84,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,    -1,    -1,   148,
      83,    84,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,    -1,    -1,   148,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,    83,    84,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      83,    84,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,    83,    84,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,   144,   145,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    83,    84,    85,    86,
      87,   144,   145,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    83,    84,    85,    86,    87,   144,   145,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    83,    84,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    83,    84,
      85,    86,    87,   144,   145,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,   101,   102,    83,    84,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,    -1,   128,   129,   130,   131,   132,   133,   134,
      -1,    83,    84,    85,    86,    87,    -1,    -1,    90,   144,
     145,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   155,     0,     5,    24,    26,    28,    30,    35,    41,
     145,   160,   161,   164,   177,   181,   194,   216,   218,   219,
     222,   115,   172,   173,   109,   165,   166,   115,   162,   163,
     115,   115,    28,   115,   149,   174,   175,   176,   186,     4,
       6,   151,   122,   121,   124,   121,   121,   151,   122,   149,
     115,   147,   121,   146,   115,   221,    29,    85,    86,   115,
     180,   208,    12,    13,   109,   113,   115,   116,   157,   172,
     109,   166,   163,   217,    19,    25,    40,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,   186,   224,   225,   226,
     227,   115,   173,   175,   124,   220,   151,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,   101,   102,   103,   104,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   138,
     139,    29,    29,   147,   178,   145,   152,   205,   207,   117,
     156,   115,   152,   128,   128,   128,   147,   128,   128,   128,
     128,    27,    32,   123,   127,   145,   148,   186,   208,    41,
     115,   148,   209,   210,   214,   223,   124,   179,   173,    38,
     206,   150,   117,   118,   122,   150,   227,   227,   227,   115,
     178,   227,   178,   227,   178,   227,   211,   212,   227,    27,
     127,   145,    12,    13,    14,    15,    21,    33,    37,    39,
      42,    45,    69,    85,    86,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   116,   130,   131,   135,   138,   139,
     146,   147,   151,   153,   159,   182,   186,   188,   196,   197,
     199,   200,   202,   203,   204,   224,   228,   231,   236,   238,
     240,   152,   214,   148,   150,    94,   121,   122,   124,   213,
     227,   182,   146,   214,   109,   129,   150,   129,   148,   179,
     129,   179,   129,   179,   129,   129,   150,   127,   146,   227,
     228,   147,   147,   128,   128,   147,   128,   147,   204,   204,
      10,   227,   227,   204,   233,   237,   158,   204,   204,   186,
     204,   204,   204,   184,   177,   115,   147,    83,    84,    85,
      86,    87,    90,    91,    92,    93,    95,    96,    97,    99,
     101,   102,   123,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   144,   145,   146,   147,   209,   115,   227,
     185,   201,   204,   150,   227,   129,   129,   129,   211,   147,
     115,   204,   227,   227,   204,   227,   148,   204,   223,   115,
     147,   204,   229,   230,   232,   233,   234,   235,   235,   105,
     150,   152,   118,   119,   156,   148,     1,     7,     8,    10,
      20,    22,    23,    32,    36,    43,   150,   152,   168,   169,
     170,   171,   183,   185,   187,   189,   190,   191,   192,   194,
     195,   201,   204,   178,   145,   148,   198,   204,   204,   204,
     204,   204,   204,   204,   115,   204,   115,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   115,   204,   148,   198,   213,    98,    75,    76,
      77,    78,    79,    80,    81,    82,    88,    89,    94,   100,
     103,   104,   122,   129,   148,   198,    16,   128,   204,   148,
     129,   129,   148,   129,   148,    17,   213,   148,   121,   146,
     150,   121,   146,   150,   152,   204,   233,   152,   204,   204,
     204,   223,    94,   204,   182,   204,   204,    31,   150,   150,
     150,   150,    17,   193,   150,   179,   115,   229,   121,   148,
     147,   124,   146,   148,   204,   199,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   148,   227,   115,   148,   204,   204,   204,   198,   204,
     230,   115,   146,   229,   204,   146,   234,   146,   120,   182,
     182,    17,   204,    11,   182,   151,    34,   215,   223,   182,
     146,   204,   148,   198,   204,   148,   129,   150,   146,   213,
       9,    18,   167,   198,   182,   184,   124,   213,   148,   148,
      16,   204,   204,   146,   204,   182,   204,   150,   152,   227,
     185,   204,   227,   148,   150,   239,   182,   182,   150,   213,
     150,   148,    44,   146,   167,   185,   204,   204,   146,   150
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   154,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   158,   158,   158,   159,   160,   161,
     162,   162,   163,   164,   165,   165,   166,   166,   167,   167,
     167,   168,   169,   170,   171,   172,   172,   172,   172,   172,
     172,   173,   173,   174,   174,   175,   175,   176,   176,   177,
     177,   178,   178,   178,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   182,   182,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   184,
     184,   184,   185,   186,   186,   186,   187,   188,   188,   188,
     188,   189,   190,   191,   191,   191,   192,   193,   193,   194,
     194,   195,   196,   196,   196,   197,   197,   197,   197,   198,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   202,   203,   203,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   205,   205,   206,   206,   207,
     208,   208,   209,   209,   210,   210,   211,   212,   212,   213,
     213,   214,   214,   214,   214,   214,   215,   215,   215,   215,
     215,   216,   217,   217,   217,   218,   219,   220,   220,   221,
     222,   223,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   225,   226,   226,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   228,   228,   228,   228,   229,
     229,   230,   230,   231,   231,   232,   232,   233,   234,   234,
     235,   235,   236,   236,   237,   237,   238,   239,   239,   240
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     3,     0,     2,     4,     3,     2,     2,
       1,     3,     1,     2,     1,     3,     1,     3,     0,     2,
       4,     4,     6,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     3,     7,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     0,
       2,     2,     3,     1,     3,     2,     2,     2,     4,     5,
       2,     1,     1,     1,     2,     3,     4,     0,     2,     1,
       1,     3,     5,     5,     5,     5,     8,     6,     9,     1,
       3,     1,     5,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     5,     6,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     3,     3,     2,     3,     4,     3,     4,
       4,     4,     3,     4,     3,     5,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     3,     0,     1,     3,
       0,     3,     1,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     3,     5,     3,     3,     4,     6,     5,     4,
       3,     4,     0,     3,     5,     4,     5,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     1,     4,     3,     4,     2,     3,     2,     3,
       4,     2,     4,     6,     4,     1,     4,     5,     1,     4,
       5,     1,     4,     5,     4,     1,     1,     1,     1,     3,
       5,     1,     3,     5,     7,     1,     3,     3,     1,     1,
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
          case 115: /* NAME  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2447 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* character_sequence  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2453 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* string_constant  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2459 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* string_builder_body  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2465 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* string_builder  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2471 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expression_else  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2477 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expression_if_then_else  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2483 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_for_loop  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2489 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_while_loop  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2495 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_with  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2501 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* annotation_argument  */
#line 98 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2507 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* annotation_argument_list  */
#line 99 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2513 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* annotation_declaration_name  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2519 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* annotation_declaration  */
#line 100 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2525 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* annotation_list  */
#line 101 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2531 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* optional_annotation_list  */
#line 101 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2537 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* optional_function_argument_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2543 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* optional_function_type  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2549 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* function_name  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2555 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expression_block  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2561 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expression_any  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2567 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expressions  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2573 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expr_pipe  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2579 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* name_in_namespace  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2585 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_delete  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2591 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expr_new  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2597 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_break  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2603 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expression_continue  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2609 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_return  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2615 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_try_catch  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2621 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* expression_let  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2627 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expr_cast  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2633 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expr_type_info  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2639 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* expr_list  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2645 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expr_block  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2651 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* expr_numeric_const  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2657 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expr_assign  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2663 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expr_named_call  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2669 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expr_method_call  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2675 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expr  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2681 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* optional_field_annotation  */
#line 99 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2687 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* structure_variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2693 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* struct_variable_declaration_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2699 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* function_argument_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2705 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* function_argument_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2711 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* tuple_type  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2717 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* tuple_type_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2723 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2729 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* let_variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2735 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* enum_list  */
#line 103 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2741 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* optional_structure_parent  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2747 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* variable_name_list  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2753 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* structure_type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2759 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* auto_type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2765 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2771 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* make_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2777 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* make_struct_fields  */
#line 102 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2783 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* make_struct_dim  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2789 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* make_struct_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2795 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* make_tuple  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2801 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* make_map_tuple  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2807 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* make_any_tuple  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2813 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* make_dim  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2819 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* make_dim_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2825 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* make_table  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2831 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* make_table_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2837 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* array_comprehension_where  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2843 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* array_comprehension  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2849 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 268 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3137 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 269 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3143 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 273 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3149 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 277 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 3157 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 280 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3171 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 289 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3181 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 17:
#line 297 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3199 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 18:
#line 313 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3222 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 22:
#line 343 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3234 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 362 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3242 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 365 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3250 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 371 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3256 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 372 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3262 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 373 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3271 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 380 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3280 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 387 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3294 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 399 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3306 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 409 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3317 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 418 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3323 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 419 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3329 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 420 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3335 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 421 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3341 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 422 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3347 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 423 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3353 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 427 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3363 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 432 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3373 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 440 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3379 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 441 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("require"); }
#line 3385 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 445 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3397 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 452 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3411 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3420 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 468 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3429 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 475 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 3435 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 476 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3441 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3447 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 481 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3453 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 482 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3459 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 486 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3468 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 490 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3477 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 497 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3486 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 501 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 3492 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 502 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 3498 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 503 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 3504 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 504 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 3510 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 505 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 3516 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 506 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 3522 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 507 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 3528 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 508 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 3534 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 509 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 3540 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 510 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 3546 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 511 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 3552 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 512 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 3558 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 513 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 3564 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 514 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 3570 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 515 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 3576 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 516 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 3582 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 517 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 3588 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 518 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 3594 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 519 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 3600 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 520 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 3606 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 521 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 3612 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 522 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 3618 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 523 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 3624 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 524 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 3630 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 525 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 3636 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 526 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 3642 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 527 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 3648 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 528 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 3654 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 529 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<"); }
#line 3660 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 530 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>"); }
#line 3666 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 531 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<="); }
#line 3672 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 532 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>="); }
#line 3678 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 533 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<<"); }
#line 3684 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 534 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>>"); }
#line 3690 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 535 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<<="); }
#line 3696 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 536 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>>="); }
#line 3702 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 541 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3772 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 609 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3780 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 612 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3792 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 622 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3798 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 623 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3804 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 624 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3810 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 625 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3816 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 626 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3822 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 627 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3828 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 628 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3834 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 629 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3840 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 630 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3846 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 631 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3852 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 632 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3858 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 633 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3864 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 634 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3870 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 638 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = tokAt((yyloc));
    }
#line 3879 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 642 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3890 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 648 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3898 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 654 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3914 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 668 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3920 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 669 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3931 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 675 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3937 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 679 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3945 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 685 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3953 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 688 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3961 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 691 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3970 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 695 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3978 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 701 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3984 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 705 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 3990 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 709 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3998 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 712 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4006 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 715 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4016 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 723 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4024 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 729 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4030 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 730 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4036 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 734 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4042 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 735 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4048 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 739 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4082 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 4090 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 774 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 4100 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 779 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 4110 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 787 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 4119 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 791 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 4129 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 796 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 4138 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 800 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 4148 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 808 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4156 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 811 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4164 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 817 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 4174 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 823 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4226 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 873 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4232 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 874 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4238 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 875 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4244 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 876 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4250 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 877 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4256 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 878 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4262 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 882 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4268 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 883 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4274 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 884 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4280 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 885 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4286 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 886 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4292 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 887 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4298 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 888 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4304 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4310 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4316 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4322 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4328 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4334 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4340 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4346 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4352 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4358 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 901 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4370 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 912 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4380 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4392 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 927 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4398 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 928 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4404 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4410 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 930 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4416 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 931 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4422 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 932 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4428 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 933 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4434 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 934 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4440 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 935 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4446 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 936 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4452 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4458 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 938 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4464 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 939 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4470 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4476 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 941 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4482 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 942 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4488 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 943 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4494 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 944 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4500 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 945 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4506 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 946 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4512 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 947 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4518 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 948 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4524 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 949 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4530 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4536 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 951 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4542 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 952 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4548 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 953 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4554 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 954 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4560 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 955 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4566 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 956 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4572 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 957 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4578 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 958 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4584 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 959 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4590 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 960 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4596 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 961 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4602 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 962 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4608 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 963 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4614 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 964 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4620 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 965 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4629 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 969 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4638 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 973 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4646 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 976 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4654 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 979 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4660 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 980 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4666 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 981 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4672 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 982 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4678 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 983 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4684 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 984 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4692 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 987 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4698 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 988 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4704 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 989 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4710 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 990 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4716 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 991 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4722 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 992 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4728 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 993 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4743 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1003 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4758 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1013 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4794 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1047 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aaList) = nullptr; }
#line 4800 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1048 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4806 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1052 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4812 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1053 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4818 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1057 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4828 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1065 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4836 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1068 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4845 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1075 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4851 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1076 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4857 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1080 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4863 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1081 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4869 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1085 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4877 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1091 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4883 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1092 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4889 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1096 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4895 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1097 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4901 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1101 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4910 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1105 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4918 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1108 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4927 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 244:
#line 1112 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4937 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 245:
#line 1117 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4947 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 246:
#line 1125 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4955 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 247:
#line 1128 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4964 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 248:
#line 1132 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4973 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 249:
#line 1136 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4983 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 250:
#line 1141 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4993 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 251:
#line 1149 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5023 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 252:
#line 1177 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 5031 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 253:
#line 1180 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 5045 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 254:
#line 1189 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 5059 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 255:
#line 1201 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 5073 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 256:
#line 1213 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5089 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 257:
#line 1227 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = nullptr; }
#line 5095 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 258:
#line 1228 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5101 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 259:
#line 1232 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5142 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 260:
#line 1271 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5210 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 261:
#line 1337 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 5222 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 262:
#line 1344 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 5233 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 263:
#line 1353 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 5239 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 264:
#line 1354 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 5245 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 265:
#line 1355 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 5251 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 266:
#line 1356 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt8; }
#line 5257 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 267:
#line 1357 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt16; }
#line 5263 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 268:
#line 1358 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 5269 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 269:
#line 1359 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 5275 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 270:
#line 1360 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 5281 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 271:
#line 1361 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 5287 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 272:
#line 1362 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 5293 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 273:
#line 1363 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt8; }
#line 5299 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 274:
#line 1364 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt16; }
#line 5305 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 275:
#line 1365 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 5311 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 276:
#line 1366 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 5317 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 277:
#line 1367 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 5323 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 278:
#line 1368 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 5329 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 279:
#line 1369 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 5335 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 280:
#line 1370 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 5341 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 281:
#line 1371 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 5347 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 282:
#line 1372 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 5353 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 283:
#line 1373 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 5359 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 284:
#line 1374 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 5365 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 285:
#line 1375 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 5371 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 286:
#line 1376 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tDouble; }
#line 5377 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 287:
#line 1380 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 5389 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 288:
#line 1390 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5398 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 289:
#line 1394 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5410 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 290:
#line 1404 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5416 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 291:
#line 1405 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5422 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 292:
#line 1406 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5428 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 293:
#line 1407 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5447 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 294:
#line 1421 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5458 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 295:
#line 1427 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5467 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 296:
#line 1431 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5477 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 297:
#line 1436 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5487 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 298:
#line 1441 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5497 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 299:
#line 1446 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5507 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 300:
#line 1451 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5519 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 301:
#line 1458 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5528 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 302:
#line 1462 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5538 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 303:
#line 1467 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5549 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 304:
#line 1473 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5559 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 305:
#line 1478 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5568 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 306:
#line 1482 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5578 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 307:
#line 1487 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5592 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 308:
#line 1496 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5601 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 309:
#line 1500 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5611 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 310:
#line 1505 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5625 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 311:
#line 1514 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5634 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 312:
#line 1518 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5644 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 313:
#line 1523 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5658 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 314:
#line 1532 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 5669 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 315:
#line 1541 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5675 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 316:
#line 1542 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5681 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 317:
#line 1543 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5687 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 318:
#line 1544 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5693 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 319:
#line 1548 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5705 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 320:
#line 1555 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5716 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 321:
#line 1564 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5726 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 322:
#line 1569 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5735 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 323:
#line 1576 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5745 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 324:
#line 1581 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5756 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 325:
#line 1590 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5764 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 326:
#line 1593 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5780 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 327:
#line 1607 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 5791 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 328:
#line 1616 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5797 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 329:
#line 1617 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5803 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 330:
#line 1621 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5813 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 331:
#line 1626 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5822 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 332:
#line 1633 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5832 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 333:
#line 1638 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 5845 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 334:
#line 1649 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5855 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 335:
#line 1654 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5864 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 336:
#line 1661 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 5878 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 337:
#line 1673 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 5884 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 338:
#line 1674 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5890 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 339:
#line 1678 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 5908 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 5912 "C:/Users/Boris/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1645  */
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
#line 1693 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


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


