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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = false );
    
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

#line 371 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   6532

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  109
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  772

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   398

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
       0,   381,   381,   383,   384,   385,   386,   387,   388,   389,
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
     794,   795,   796,   797,   798,   799,   800,   804,   877,   880,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,   911,   915,
     921,   927,   938,   944,   945,   956,   960,   966,   969,   972,
     976,   982,   986,   990,   993,   996,  1001,  1004,  1012,  1015,
    1020,  1023,  1031,  1037,  1038,  1042,  1043,  1047,  1081,  1084,
    1089,  1097,  1101,  1106,  1112,  1116,  1121,  1130,  1133,  1139,
    1140,  1147,  1158,  1159,  1163,  1168,  1223,  1224,  1225,  1226,
    1227,  1228,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1251,  1252,
    1256,  1267,  1272,  1282,  1286,  1293,  1307,  1308,  1309,  1310,
    1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,
    1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1349,  1353,  1356,  1359,  1360,
    1361,  1362,  1367,  1372,  1373,  1376,  1379,  1383,  1387,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1407,  1417,  1451,  1452,
    1456,  1457,  1461,  1469,  1472,  1479,  1484,  1492,  1493,  1497,
    1500,  1509,  1510,  1514,  1523,  1524,  1529,  1530,  1534,  1539,
    1542,  1546,  1552,  1561,  1562,  1563,  1567,  1568,  1572,  1575,
    1580,  1585,  1593,  1604,  1607,  1614,  1615,  1619,  1649,  1652,
    1661,  1674,  1686,  1687,  1691,  1695,  1706,  1721,  1722,  1726,
    1765,  1831,  1838,  1847,  1848,  1849,  1850,  1851,  1852,  1853,
    1854,  1855,  1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,
    1864,  1865,  1866,  1867,  1868,  1869,  1870,  1874,  1875,  1876,
    1877,  1878,  1879,  1883,  1894,  1898,  1908,  1909,  1910,  1911,
    1925,  1931,  1935,  1940,  1945,  1950,  1957,  1962,  1966,  1970,
    1975,  1980,  1987,  1992,  1998,  2003,  2007,  2012,  2021,  2025,
    2030,  2039,  2043,  2048,  2057,  2063,  2072,  2073,  2074,  2075,
    2079,  2086,  2095,  2100,  2107,  2112,  2118,  2124,  2131,  2142,
    2145,  2159,  2168,  2169,  2173,  2178,  2185,  2190,  2201,  2206,
    2213,  2225,  2226,  2230,  2244
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
  "optional_shared", "global_let", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
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
     385,   386,   387,   388,   389,   390,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     391,   392,   126,    33,   393,   394,   395,   396,    46,    91,
      93,    40,    41,   397,   398,    59,   123,   125,    36,    64,
      35
};
# endif

#define YYPACT_NINF -441

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-441)))

#define YYTABLE_NINF -193

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -441,    13,  -441,  -441,   -58,   -36,   -13,    34,  -122,  -441,
      75,    27,  -441,  -441,  -441,  -441,  -441,   120,  -441,   107,
    -441,  -441,  -441,  -441,    97,  -441,    81,   128,   139,  -441,
    -441,     4,  -441,   -73,   161,  -441,  -441,   117,   169,   135,
    -441,    44,  -441,   175,   164,  -441,   155,    18,   -58,   201,
     -36,   204,  -441,   212,   224,  -441,   306,  -441,   219,  -441,
     -94,   229,  -441,   -58,    27,  -441,   221,   202,  6379,   337,
     347,  -441,   220,  -441,  -441,  -441,  -441,  -441,  -441,   251,
      64,  -441,  -441,  -441,  -441,   332,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,   225,   -93,  5526,  -441,  -441,  -441,
     -37,  -441,   -80,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,    24,   247,   -91,  -441,   -21,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,    53,  -441,  -441,   -50,  -441,   249,   250,
     253,  -441,  -441,  -441,   237,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,   256,
    -441,  -441,  -441,   257,   258,  -441,  -441,  -441,  -441,   259,
     264,  -441,  -441,  -441,  -441,   342,  -441,  -441,    80,   -58,
    -441,   -28,  -441,   130,  5526,   242,  -441,  -441,  -441,   110,
    -441,  -441,    64,  -441,   -81,  4109,  -441,  5526,  5526,  5526,
     281,  5452,  5452,  5452,  5600,   283,  -441,    11,  -441,  -441,
    -441,  -441,  2063,  -441,  -441,  -441,   372,   254,    60,   285,
    -441,   -39,  -441,   261,  3971,  -441,  -441,   286,  5526,  -441,
     -48,  -441,  -441,  -441,  -441,  5378,   260,  -441,   266,   279,
     287,   268,   288,   290,  4109,  4109,  4230,  4306,  4109,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  4109,  4109,  4109,  4109,
    4109,  4109,  -441,   269,  -441,  -441,   -60,  -441,  -441,  -441,
     267,  -441,  -441,  -441,  -441,  -441,  4742,   273,  -441,  -441,
    -441,  -441,  -441,   558,   588,  1186,   275,   247,  1463,   247,
    1733,   247,  1925,   -84,  -441,    56,  3971,   300,  -441,    88,
     298,  -441,   289,  -441,  -441,  5595,  -441,   285,  -441,  -441,
    -441,  -441,  -441,  5526,  2205,  -441,   913,  -441,   327,  -441,
    -441,  -441,  2205,  2010,  -441,   311,  4109,  5526,  5526,  4109,
    5526,  5526,   265,   265,   285,  1239,   285,  1381,  5897,  -441,
      83,   196,   265,   265,  -441,   265,   265,  4874,  -103,  -441,
    1651,   220,    31,   313,  4109,  4109,  -441,  -441,  4109,  4109,
    4109,  4109,   316,  4109,   317,  4109,  4109,  4109,  4109,  2340,
    4109,  4109,  4109,  4109,  4109,  4109,  4109,  4109,  4109,  4109,
     322,  4109,  -441,  2475,  -441,  5526,  -441,  -441,   310,  -441,
     319,  -441,   323,  -441,  5526,  -441,  5600,  5526,  -441,   283,
    -441,  -441,  -441,  -441,   373,   105,  -441,   343,  5740,  -441,
    4109,  -441,  -441,   285,  1516,  -441,   242,  4109,  4109,  -441,
     301,   344,  2610,  1786,  -441,   435,   307,   308,  4109,  -441,
    -441,  -441,  -441,  -441,  -441,   309,   312,   314,  -441,  -441,
    -441,   453,  -441,   -86,  -441,  5841,  -441,  2205,  -441,   732,
    2745,  1928,  4942,  2202,  2422,  5008,  2557,  2692,    -6,   -74,
     315,  2880,  5897,   340,    22,   349,  -441,  -441,   122,    33,
    3015,   151,   166,  4109,  4109,   320,  -441,  4109,   346,  -441,
    5452,  -441,   353,  -441,    62,  6065,   247,  5526,  -441,  -441,
     131,   131,  6308,  6308,  6286,  6286,   329,     9,  -441,   -77,
     -77,   131,   131,   358,  5953,  6121,  6177,   486,  6308,  6308,
      -4,    -4,     9,     9,     9,  -441,  5668,  -441,    63,  2827,
    -441,  -441,  -441,  3971,  -441,  3971,  -441,  4109,  -441,  -441,
     105,  4109,  4109,  4109,  4109,  4109,  4109,  4109,  4109,  4109,
    4109,  4109,  4109,  4109,  4109,  4109,   203,    39,  2205,  -441,
    -441,  4307,   479,  6065,   203,  -441,   354,   370,  6065,  2205,
    -441,  4400,   333,  -441,  -441,   203,  -441,  -441,  -441,   457,
     285,  -441,  3150,  3285,  -441,  4493,  -441,  -441,    65,  5526,
     368,  5074,  -441,  4109,  4109,  -441,  4109,   339,  4109,  4109,
    -441,   353,   374,   348,   353,  4109,   359,  4109,  4109,   353,
     360,   361,  6065,  -441,  -441,  6009,   247,  2962,   -54,    66,
    4109,  -441,   -98,  3971,  3420,  -441,  4109,  -441,  -441,  -441,
    6065,  -441,  6065,  6065,  6065,  6065,  6065,  6065,  6065,  6065,
    6065,  6065,  6065,  6065,  6065,  6065,  6065,  -441,  4109,  -441,
    4586,  -441,   242,  -441,  -441,  -441,  -441,  4679,  -441,  -441,
     276,  -441,  -441,  4109,  4109,  -441,  -441,  3097,    89,  -441,
    6308,  6308,  6308,  3555,   -71,  6065,   146,   -54,  -441,   340,
    6065,  -441,  -441,   -61,   188,  -441,  -441,  -441,   379,   -80,
     341,  6065,  3690,  -441,  -441,  -441,    67,  6065,   -44,  -441,
    -441,  -441,  1082,   242,  -441,  -441,  4109,  -441,  6233,  6233,
    -441,  3832,   388,  -441,  5140,  4109,   364,  4109,  4109,   365,
     -80,  -441,  -441,  4109,  6065,  -441,   242,  -441,  -441,   203,
    5526,  5206,   383,  -441,  4811,  -441,  6065,  4811,  -441,  -441,
    6065,  -441,   276,  3502,  -441,  3974,   454,   371,   363,  -441,
    -441,  5526,  5272,  4109,   378,   380,  3637,  -441,  6065,  -441,
    -441,  -441
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    73,     1,   165,     0,     0,     0,     0,     0,   166,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   315,
       6,    11,     5,     4,    63,    65,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    68,   143,     0,    69,
      71,     0,    67,     0,     0,   316,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   318,     0,   322,
       0,     0,   145,     0,     0,    74,   327,     0,     0,     0,
       0,    80,    75,   313,    61,    62,    59,    60,    58,     0,
       0,    57,    66,    34,    32,    28,    25,    24,   357,   360,
     358,   361,   359,   362,     0,     0,     0,   324,   323,   144,
       0,    72,     0,   329,   283,    83,    84,    86,    85,    87,
      88,    89,    90,   109,   110,   107,   108,   100,   111,   112,
     101,    98,    99,   113,   114,   115,   116,   103,   104,   102,
      96,    97,    92,    91,    93,    94,    95,    82,    81,   105,
     106,   278,    78,     0,    14,     0,    53,    54,    51,    52,
      50,    49,    55,     0,    27,   318,     0,   325,     0,     0,
       0,   333,   353,   334,   364,   335,   339,   340,   341,   342,
     346,   347,   348,   349,   350,   351,   352,   354,   355,   385,
     338,   345,   356,   388,   391,   336,   343,   337,   344,     0,
       0,   363,   366,   368,   367,     0,    70,   328,   278,     0,
      76,     0,   287,     0,     0,     0,   331,   317,   314,   306,
      15,    16,     0,    64,     0,     0,   319,     0,     0,     0,
       0,    75,    75,    75,     0,     0,   372,     0,   378,   381,
     380,   374,     0,   321,   377,   330,   280,     0,     0,     0,
     285,   298,    77,   278,    79,   138,   117,     0,     0,   307,
       0,    56,   326,   221,   222,     0,     0,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
     188,   187,   189,   190,   191,    17,     0,     0,     0,     0,
       0,     0,   182,   183,   219,   184,   217,   271,   270,   269,
      73,   274,   218,   273,   272,   253,     0,     0,   220,   396,
     397,   398,   399,     0,     0,     0,     0,    78,     0,    78,
       0,    78,     0,   143,   291,     0,   289,     0,   294,     0,
     373,   376,     0,   379,   370,     0,   281,     0,   284,   279,
     286,   297,   296,     0,     0,   288,     0,   332,     0,   304,
     305,   303,     0,   147,   150,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,   418,
       0,     0,   226,   225,   258,   224,   223,     0,     0,   277,
       0,    75,     0,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,     0,   382,     0,   384,   365,     0,   386,
       0,   389,     0,   392,     0,   394,     0,     0,   395,     0,
     375,   371,   369,   282,   299,     0,   302,     0,   301,   140,
       0,    43,    44,     0,     0,   151,     0,     0,     0,   152,
       0,     0,     0,     0,   120,   118,     0,     0,     0,   133,
     128,   126,   127,   139,   121,     0,     0,     0,   131,   132,
     134,   163,   125,     0,   122,   192,   308,     0,   312,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,   409,   402,     0,   412,   413,   414,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,    18,   249,
      75,   213,     0,   254,     0,   177,    78,     0,   266,   267,
     227,   228,   240,   241,   238,   239,     0,   263,   252,   275,
     276,   229,   230,     0,     0,   243,   244,   242,   236,   237,
     232,   231,   233,   234,   235,   251,     0,   256,     0,     0,
     387,   390,   393,   290,   292,   293,   295,     0,   183,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     156,   192,     0,   146,     0,   137,     0,     0,    37,     0,
     160,   192,     0,   135,   136,     0,   124,   129,   130,     0,
       0,   123,     0,     0,   310,   192,   311,   148,     0,     0,
       0,     0,   259,     0,     0,   260,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   411,   419,   420,     0,    78,     0,     0,     0,
       0,   255,     0,   265,     0,   268,     0,   250,   257,   383,
     300,   141,   199,   200,   202,   201,   203,   196,   197,   198,
     204,   205,   194,   195,   206,   207,   193,    47,     0,   157,
     192,   154,     0,    48,    35,    36,   161,   192,   158,   138,
      40,   164,   167,     0,     0,   309,   149,     0,     0,   171,
     168,   169,   170,     0,     0,   400,     0,     0,   404,   403,
     410,   416,   415,     0,     0,   407,   417,    19,     0,     0,
       0,   178,     0,   179,   185,   211,     0,   264,     0,   155,
     162,   159,     0,     0,    38,    39,     0,    45,   209,   208,
     174,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,   214,   210,     0,   180,   212,     0,   119,    41,     0,
       0,     0,     0,   262,   421,   406,   401,   421,   408,   215,
     181,    46,    40,     0,   172,     0,     0,     0,     0,    42,
     175,     0,     0,     0,     0,     0,     0,   173,   422,   423,
     424,   176
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -441,  -441,  -441,   167,   487,  -441,  -441,  -441,  -441,  -441,
    -441,   448,  -441,  -441,   491,  -441,  -441,  -441,  -210,  -441,
    -441,  -441,  -441,  -441,   334,  -441,   496,   -43,  -441,   481,
    -441,   262,  -212,  -288,  -441,  -441,  -200,  -441,  -121,  -296,
     -11,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,   546,
    -441,  -441,  -441,  -399,  -441,  -441,  -390,  -441,  -335,  -441,
    -441,  -441,  -441,   180,   351,  -441,  -441,  -441,   321,  -441,
     134,  -441,   132,  -441,  -234,  -214,   215,  -441,   -34,  -441,
    -441,  -441,   402,   502,  -441,  -441,  -441,  -441,  -441,  -441,
    -140,   -90,  -441,  -441,  -441,   -82,   325,  -440,  -349,  -441,
    -441,  -266,   -35,   241,  -441,  -441,  -441,  -148,  -441
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   145,   151,   361,   284,    14,    15,    31,
      32,    55,    16,    28,    29,   446,   447,   716,   717,   448,
     449,   450,   451,   452,   152,   153,    25,    26,    39,    40,
      41,    17,   142,   205,    72,    18,   285,   453,   336,   454,
     286,   455,   287,   456,   457,   458,   459,   460,   590,   461,
     462,   288,   289,   504,   704,   290,   291,   292,   427,   464,
     293,   294,   295,   505,   201,   327,   237,   198,   202,   203,
     314,   315,   318,   319,   609,   240,   342,   250,   208,   143,
      46,    20,    95,    59,    60,    21,    22,   103,    67,    23,
     241,   297,    94,   193,   194,   316,   298,   483,   484,   299,
     485,   486,   487,   488,   300,   360,   301,   757,   302
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   463,   359,   209,   538,   246,   192,   334,   491,   307,
     309,   311,   608,     2,   195,   372,   373,   239,     3,   408,
     100,   410,   702,   412,    51,   330,   550,    37,   372,   373,
     382,    74,    75,   384,   331,   549,    58,   156,   426,   206,
     500,     4,   320,     5,    34,     6,   468,     7,   507,   156,
      37,   618,     8,    42,   331,   414,    52,   658,     9,    36,
     339,    38,   629,   332,    10,   630,    56,   340,   245,   331,
     369,   598,    24,    97,   157,   630,   207,   146,   147,   591,
      61,   400,   401,   332,    38,   191,   252,   215,    27,   341,
      61,   197,   630,    57,   725,   376,   377,   247,   332,    48,
     333,   370,   206,   382,   728,   383,   384,   385,   376,   377,
     386,   210,   211,   423,   192,   216,   382,    30,   383,   384,
     385,   736,   244,   386,    43,   196,    44,   192,   192,   192,
     247,   192,   192,   192,   192,   303,   304,   305,   570,   308,
     310,   312,    76,   397,   398,   399,    77,   580,    78,    79,
     372,   373,    53,   321,   400,   401,   238,    37,   192,   506,
     641,   508,    54,    45,    33,   192,   338,   400,   401,   247,
     322,   594,    11,   343,   689,   247,   192,   192,    12,    80,
      64,   323,   613,   199,   355,   357,   200,   614,   148,   212,
     547,    38,   149,   191,   150,    79,    48,    68,   630,   630,
     415,   630,   612,   630,    65,    35,   191,   191,   191,   684,
     191,   191,   191,   191,   478,   213,   489,    48,   632,   693,
     329,   416,   372,   373,   631,   638,   700,   676,   623,   735,
     376,   377,   418,   721,    47,   706,   572,   191,   382,   199,
     383,   384,   385,   192,   191,   386,   247,   235,   494,   248,
     495,   424,   249,   419,   722,   191,   191,   192,   192,   708,
     192,   192,   686,    69,    70,   473,   474,    49,   476,   477,
     694,   245,   659,   282,   548,    50,   395,   396,   397,   398,
     399,    61,   616,   666,   372,   373,   713,   617,   626,   400,
     401,    58,   242,   567,    71,   243,    63,   714,   715,    62,
     374,   375,   376,   377,   378,    66,   726,   379,   380,   381,
     382,   614,   383,   384,   385,   192,   614,   386,   620,   387,
     388,    73,   191,   539,   192,    83,   192,   192,   144,   496,
     497,   617,   543,   621,    85,   545,   191,   191,   698,   191,
     191,   389,    86,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   614,    87,   729,    96,   501,   226,    99,
     102,   400,   401,   227,   376,   377,   657,    88,   104,   245,
     139,    89,   382,   226,   663,   384,   385,   463,   227,   386,
     140,   141,   228,   144,    52,   670,   204,    90,    91,    92,
      93,   155,   217,   218,   191,   296,   219,   228,   220,   221,
     222,   223,   224,   191,   226,   191,   191,   225,   245,   227,
     192,   306,   325,   317,   326,   206,   337,   192,   627,   328,
     199,   345,   347,   400,   401,   633,    11,   346,   228,   349,
     348,   350,   703,   351,   403,   339,   229,   407,   368,   417,
     420,   471,   340,   509,   352,   353,   516,   518,   358,   421,
     209,   229,   535,   727,   540,   550,   362,   363,   364,   365,
     366,   367,   710,   541,   341,   230,   575,   542,   576,   231,
     582,   589,   583,   584,   586,   610,   612,   587,   210,   588,
     230,   331,   229,   628,   231,   615,   232,   624,   635,   191,
     634,   662,   466,   664,   665,   671,   191,   234,   678,   669,
     683,   232,   763,   732,   687,   372,   373,   233,   688,   192,
     332,   230,   234,   738,   428,   231,   465,   677,   742,   691,
     695,   696,   469,   730,   745,   748,   472,   755,   498,   475,
     765,   764,   232,   154,    81,   482,   751,   482,   769,   752,
     770,    84,   759,   234,    82,   101,   251,    19,   712,   236,
     544,   546,   371,   467,   510,   511,   672,   214,   512,   513,
     514,   515,    98,   517,   335,   519,   520,   521,   522,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     344,   536,   692,   374,   375,   376,   377,   378,   191,   226,
     379,   380,   381,   382,   227,   383,   384,   385,   492,   758,
     386,     0,   387,   388,     0,     0,     0,     0,     0,     0,
     566,     0,     0,   228,   571,     0,     0,   573,   574,   226,
       0,     0,   578,   581,   227,     0,     0,     0,   585,   393,
     394,   395,   396,   397,   398,   399,     0,     0,     0,     0,
       0,     0,     0,   228,   400,   401,     0,   595,     0,     0,
     192,   601,     0,     0,     0,     0,     0,     0,   753,     0,
       0,   367,     0,     0,     0,     0,     0,   229,     0,     0,
     367,   192,     0,   622,   358,     0,     0,   625,     0,   766,
       0,     0,     0,     0,     0,     0,     0,     0,   731,     0,
       0,     0,     0,     0,     0,     0,   230,   229,     0,     0,
     231,     0,   404,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,   749,
       0,     0,     0,     0,     0,     0,   230,   640,   234,   191,
     231,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,     0,   232,   660,     0,
     191,   372,   373,   405,     0,     0,     0,     0,   234,   667,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   652,   656,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,   681,     0,   682,     0,     0,   685,
       0,     0,     0,     0,     0,   690,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     701,     0,     0,     0,     0,     0,   707,     0,     0,     0,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   374,
     375,   376,   377,   378,   559,   560,   379,   380,   381,   382,
     561,   383,   384,   385,     0,     0,   386,   562,   387,   388,
     563,   564,     0,   718,   719,     0,     0,     0,     0,     0,
       0,     0,     0,   724,     0,     0,     0,     0,     0,   565,
     389,     0,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   734,     0,     0,     0,     0,     0,     0,     0,
     400,   401,   465,     0,     0,     0,   739,   596,     0,     0,
       0,   741,     0,     0,     0,   744,     0,   746,   747,     0,
       0,     0,     0,   750,   429,     0,     0,     0,     3,     0,
     430,   431,   432,     0,   433,     0,   253,   254,   255,   256,
       0,     0,     0,     0,     0,   762,     0,   434,   257,   435,
     436,     0,     0,   768,     0,     0,     0,     0,     0,   437,
     258,     0,     0,   438,   259,     0,   260,     0,     9,   261,
     439,     0,   440,   262,     0,     0,   441,   442,     0,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,   180,
     181,   182,     0,     0,   185,   186,   187,   188,     0,     0,
     263,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,    37,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,   277,
     278,     0,     0,     0,     0,   279,   280,     0,     0,     0,
       0,     0,     0,     0,   281,     0,     0,    38,   444,   245,
     445,   282,   283,   429,     0,     0,     0,     3,     0,   430,
     431,   432,     0,   433,     0,   253,   254,   255,   256,     0,
       0,     0,     0,     0,     0,     0,   434,   257,   435,   436,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   258,
       0,     0,   438,   259,     0,   260,     0,     9,   261,   439,
       0,   440,   262,     0,     0,   441,   442,     0,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,     0,   180,   181,
     182,     0,     0,   185,   186,   187,   188,     0,     0,   263,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,    37,   275,     0,     0,     0,   226,     0,     0,
       0,     0,   227,     0,     0,     0,     0,   276,   277,   278,
       0,     0,     0,     0,   279,   280,     0,     0,     0,     0,
       0,   228,     0,   281,     0,     0,    38,   444,   245,   737,
     282,   283,   253,   254,   255,   256,     0,     0,     0,     0,
       0,     0,     0,     0,   257,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   227,   258,     0,     0,     0,
     259,     0,   260,     0,     0,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   228,   229,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,     0,   180,   181,   182,     0,     0,
     185,   186,   187,   188,   230,     0,   263,     0,   231,     0,
     406,     0,     0,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,     0,     0,   232,     0,     0,   229,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   479,
     275,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,   231,     0,     0,   276,   277,   278,     0,     0,     0,
       0,   279,   280,     0,   253,   254,   255,   256,   232,   480,
     481,     0,     0,    38,     0,   245,   257,   282,   283,   234,
       0,     0,   226,     0,     0,     0,     0,   227,   258,     0,
       0,     0,   259,     0,   260,     0,     0,   261,     0,     0,
       0,   262,     0,     0,     0,     0,   228,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,   180,   181,   182,
       0,     0,   185,   186,   187,   188,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,   265,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,     0,   226,     0,     0,     0,     0,   227,
       0,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   479,   275,     0,     0,     0,     0,     0,   228,   230,
       0,     0,     0,   231,     0,     0,   276,   277,   278,   253,
     254,   255,   256,   279,   280,     0,     0,     0,     0,     0,
     232,   257,   490,     0,     0,    38,     0,   245,     0,   282,
     283,   234,     0,   258,     0,     0,     0,   259,     0,   260,
       0,     0,   261,     0,     0,     0,   262,     0,     0,     0,
       0,     0,   229,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,     0,   180,   181,   182,     0,     0,   185,   186,   187,
     188,   230,     0,   263,     0,   231,     0,   409,     0,     0,
       0,     0,     0,     0,     0,   264,   265,     0,     0,     0,
       0,     0,   232,     0,   568,     0,     0,     0,     0,   425,
       0,     0,     0,   234,     0,     0,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,    37,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,   277,   278,   253,   254,   255,   256,   279,   280,
       0,     0,     0,     0,     0,     0,   257,   281,     0,     0,
      38,   569,   245,     0,   282,   283,     0,     0,   258,     0,
       0,     0,   259,     0,   260,     0,     0,   261,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,   180,   181,   182,
       0,     0,   185,   186,   187,   188,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   227,
       0,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,    37,   275,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,   276,   277,   278,   253,
     254,   255,   256,   279,   280,     0,     0,     0,     0,     0,
     502,   257,   281,   503,     0,    38,     0,   245,     0,   282,
     283,     0,     0,   258,     0,     0,     0,   259,     0,   260,
       0,     0,   261,     0,     0,     0,   262,     0,     0,     0,
       0,     0,   229,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,     0,   180,   181,   182,     0,     0,   185,   186,   187,
     188,   230,     0,   263,     0,   231,     0,   411,     0,     0,
       0,     0,     0,     0,     0,   264,   265,     0,     0,     0,
       0,     0,   232,     0,   579,     0,     0,     0,     0,   425,
       0,     0,     0,   234,     0,     0,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,    37,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,   277,   278,     0,     0,     0,     0,   279,   280,
       0,   253,   254,   255,   256,   599,     0,   281,     0,     0,
      38,     0,   245,   257,   282,   283,   226,     0,     0,     0,
       0,   227,     0,     0,     0,   258,     0,     0,     0,   259,
       0,   260,     0,     0,   261,     0,     0,     0,   262,     0,
     228,     0,     0,     0,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,     0,   180,   181,   182,     0,     0,   185,
     186,   187,   188,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,   265,     0,
       0,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,   227,     0,     0,   266,
     267,   268,   269,   270,   271,   272,   273,   274,    37,   275,
       0,     0,     0,   230,     0,   228,     0,   231,     0,   413,
       0,   600,     0,   276,   277,   278,   253,   254,   255,   256,
     279,   280,     0,     0,   232,     0,     0,     0,   257,   281,
       0,     0,    38,     0,   245,   234,   282,   283,     0,     0,
     258,     0,     0,     0,   259,     0,   260,     0,     0,   261,
       0,     0,     0,   262,     0,     0,     0,     0,     0,   229,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,   180,
     181,   182,     0,     0,   185,   186,   187,   188,   230,     0,
     263,     0,   231,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,   265,     0,     0,     0,     0,     0,   232,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,    37,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,   277,
     278,     0,     0,     0,     0,   279,   280,     0,   253,   254,
     255,   256,     0,   324,   281,     0,     0,    38,     0,   245,
     257,   282,   283,   226,     0,     0,     0,     0,   227,     0,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,   228,     0,     0,
       0,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
     230,     0,     0,     0,   231,     0,   603,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
     523,   232,     0,     0,     0,   257,   281,     0,     0,    38,
       0,   245,   234,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,   227,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,   253,   254,
     255,   256,   279,   280,     0,     0,     0,     0,     0,     0,
     257,   281,     0,     0,    38,     0,   245,     0,   282,   283,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   229,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
     230,     0,   263,     0,   231,     0,   604,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,   234,   227,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
       0,     0,     0,     0,     0,   257,   281,   537,     0,    38,
       0,   245,     0,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,   577,     0,     0,   229,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,   230,     0,   263,     0,   231,
       0,   606,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,   234,   227,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,   253,   254,
     255,   256,   279,   280,     0,     0,     0,     0,     0,     0,
     257,   281,     0,     0,    38,     0,   245,     0,   282,   283,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   229,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
     230,     0,   263,     0,   231,     0,   607,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,   234,   227,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
       0,     0,     0,     0,     0,   257,   281,   597,     0,    38,
       0,   245,     0,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,     0,   229,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,   230,     0,   263,     0,   231,
       0,   639,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,   234,   227,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,   253,   254,
     255,   256,   279,   280,     0,     0,     0,     0,     0,     0,
     257,   281,   611,     0,    38,     0,   245,     0,   282,   283,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   229,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
     230,     0,   263,     0,   231,     0,   699,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,   234,   227,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
       0,     0,     0,     0,     0,   257,   281,   619,     0,    38,
       0,   245,     0,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,     0,   229,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,   230,     0,   263,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,   232,     0,     0,   720,
       0,     0,     0,   673,     0,     0,     0,   234,     0,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,   253,   254,
     255,   256,   279,   280,     0,     0,     0,     0,     0,     0,
     257,   281,     0,     0,    38,     0,   245,     0,   282,   283,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   674,     0,
       0,     0,     0,     0,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
       0,     0,     0,     0,     0,   257,   281,     0,     0,    38,
       0,   245,     0,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,   227,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,   253,   254,
     255,   256,   279,   280,     0,     0,     0,     0,     0,     0,
     257,   281,   705,     0,    38,     0,   245,     0,   282,   283,
       0,     0,   258,     0,     0,     0,   259,     0,   260,     0,
       0,   261,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   229,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,   180,   181,   182,     0,     0,   185,   186,   187,   188,
     230,     0,   263,     0,   231,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,     0,
       0,   232,     0,     0,   760,     0,     0,     0,   226,     0,
       0,     0,   234,   227,     0,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    37,   275,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
     276,   277,   278,   253,   254,   255,   256,   279,   280,     0,
       0,     0,     0,     0,     0,   257,   281,   723,     0,    38,
       0,   245,     0,   282,   283,     0,     0,   258,     0,     0,
       0,   259,     0,   260,     0,     0,   261,     0,     0,     0,
     262,     0,     0,     0,     0,     0,   229,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   182,     0,
       0,   185,   186,   187,   188,   230,     0,   263,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
     265,     0,     0,     0,     0,     0,   232,     0,   733,   771,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      37,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   277,   278,     0,     0,
       0,     0,   279,   280,     0,   253,   254,   255,   256,   740,
       0,   281,     0,     0,    38,     0,   245,   257,   282,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   258,
       0,     0,     0,   259,     0,   260,     0,     0,   261,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,     0,   180,   181,
     182,     0,     0,   185,   186,   187,   188,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,   267,   268,   269,   270,   271,   272,
     273,   274,    37,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,   277,   278,
       0,     0,     0,     0,   279,   280,     0,   253,   254,   255,
     256,   761,     0,   281,     0,     0,    38,     0,   245,   257,
     282,   283,   226,     0,     0,     0,     0,   227,     0,     0,
       0,   258,     0,     0,     0,   259,     0,   260,     0,     0,
     261,     0,     0,     0,   262,     0,   228,     0,     0,     0,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,     0,
     180,   181,   182,     0,     0,   185,   186,   187,   188,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,   265,     0,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,   267,   268,   269,   270,
     271,   272,   273,   274,    37,   275,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   276,
     277,   278,   253,   254,   255,   256,   279,   280,     0,     0,
     232,     0,     0,     0,   257,   281,     0,     0,    38,     0,
     245,   234,   282,   283,     0,     0,   258,     0,     0,     0,
     259,     0,   260,     0,     0,   261,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,     0,   180,   181,   182,     0,     0,
     185,   186,   187,   188,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,   267,   268,   269,   270,   271,   272,   273,   274,    37,
     275,   354,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,   276,   277,   278,     0,     0,   159,
       0,   279,   280,     0,     0,     0,     0,     0,     0,     0,
     281,     0,     0,    38,   160,   245,     0,   282,   283,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   356,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   373,     0,   158,
       0,     0,     0,     0,     0,   159,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,    38,     0,   551,   552,   553,   554,
     555,   556,   557,   558,   374,   375,   376,   377,   378,   559,
     560,   379,   380,   381,   382,   561,   383,   384,   385,   372,
     373,   386,   562,   387,   388,   563,   564,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,   565,   389,     0,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   401,     0,     0,     0,
      38,     0,   661,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   374,   375,   376,
     377,   378,   559,   560,   379,   380,   381,   382,   561,   383,
     384,   385,   372,   373,   386,   562,   387,   388,   563,   564,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,   389,     0,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   401,
       0,     0,     0,     0,     0,   668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   551,   552,   553,   554,   555,   556,   557,   558,
     374,   375,   376,   377,   378,   559,   560,   379,   380,   381,
     382,   561,   383,   384,   385,   372,   373,   386,   562,   387,
     388,   563,   564,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   389,     0,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   401,     0,     0,     0,     0,     0,   675,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   374,   375,   376,   377,   378,   559,   560,
     379,   380,   381,   382,   561,   383,   384,   385,   372,   373,
     386,   562,   387,   388,   563,   564,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,   389,     0,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,     0,     0,     0,
       0,   709,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,   373,     0,     0,     0,     0,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   374,   375,   376,   377,
     378,   559,   560,   379,   380,   381,   382,   561,   383,   384,
     385,     0,     0,   386,   562,   387,   388,   563,   564,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   565,   389,     0,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,     0,
     372,   373,     0,     0,     0,     0,     0,   400,   401,   374,
     375,   376,   377,   378,   711,     0,   379,   380,   381,   382,
       0,   383,   384,   385,     0,     0,   386,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     389,     0,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,     0,   372,   373,     0,     0,     0,     0,     0,
     400,   401,     0,     0,     0,     0,     0,   402,   374,   375,
     376,   377,   378,     0,     0,   379,   380,   381,   382,     0,
     383,   384,   385,     0,     0,   386,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   389,
       0,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   372,   373,     0,     0,     0,     0,     0,     0,   400,
     401,   374,   375,   376,   377,   378,   756,     0,   379,   380,
     381,   382,     0,   383,   384,   385,     0,     0,   386,     0,
     387,   388,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,     0,     0,     0,   372,   373,     0,
       0,     0,   400,   401,     0,     0,   499,     0,     0,   374,
     375,   376,   377,   378,     0,     0,   379,   380,   381,   382,
       0,   383,   384,   385,     0,     0,   386,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     389,     0,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,     0,   372,   373,     0,     0,     0,     0,     0,
     400,   401,     0,     0,   602,   374,   375,   376,   377,   378,
       0,     0,   379,   380,   381,   382,     0,   383,   384,   385,
       0,     0,   386,     0,   387,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,     0,   372,
     373,     0,     0,     0,     0,     0,   400,   401,     0,     0,
     605,   374,   375,   376,   377,   378,     0,     0,   379,   380,
     381,   382,     0,   383,   384,   385,     0,     0,   386,     0,
     387,   388,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,     0,   372,   373,     0,     0,     0,
       0,     0,   400,   401,     0,     0,   679,   374,   375,   376,
     377,   378,     0,     0,   379,   380,   381,   382,     0,   383,
     384,   385,     0,     0,   386,     0,   387,   388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,     0,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
       0,   372,   373,     0,     0,     0,     0,     0,   400,   401,
       0,     0,   743,   374,   375,   376,   377,   378,     0,     0,
     379,   380,   381,   382,     0,   383,   384,   385,     0,     0,
     386,     0,   387,   388,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   389,     0,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,     0,   754,   374,
     375,   376,   377,   378,     0,     0,   379,   380,   381,   382,
       0,   383,   384,   385,     0,     0,   386,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,   159,     0,     0,
     389,     0,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   160,     0,     0,     0,     0,     0,     0,     0,
     400,   401,     0,     0,   767,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,     0,     0,     0,     0,
       0,   159,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,     0,   266,
     267,   268,     0,     0,     0,     0,     0,     0,    37,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,   159,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   141,   372,   373,    38,     0,     0,     0,
       0,     0,     0,   158,     0,     0,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   372,   373,     0,
      38,     0,   374,   375,   376,   377,   378,     0,     0,   379,
     380,   381,   382,     0,   383,   384,   385,     0,     0,   386,
       0,   387,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,   389,     0,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   401,   422,     0,     0,     0,   372,
     373,     0,     0,     0,    38,   374,   375,   376,   377,   378,
       0,     0,   379,   380,   381,   382,     0,   383,   384,   385,
       0,     0,   386,     0,   387,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,   637,   551,
     552,   553,   554,   555,   556,   557,   558,   374,   375,   376,
     377,   378,   559,   560,   379,   380,   381,   382,   561,   383,
     384,   385,  -192,     0,   386,   562,   387,   388,   563,   564,
     372,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,   389,     0,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,   556,   557,   558,   374,   375,
     376,   377,   378,   559,   560,   379,   380,   381,   382,   592,
     383,   384,   385,     0,     0,   386,   562,   387,   388,   563,
     564,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,   373,     0,     0,     0,     0,   593,   389,
       0,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,     0,     0,     0,   374,   375,   376,   377,   378,   400,
     401,   379,   380,   381,   382,     0,   383,   384,   385,     0,
       0,   386,     0,   387,   388,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   373,
       0,     0,     0,     0,     0,   389,     0,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,     0,     0,     0,
     374,   375,   376,   377,   378,   400,   401,   379,   380,   381,
     382,     0,   383,   384,   385,     0,     0,   386,     0,   387,
     388,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   372,   373,     0,     0,     0,     0,
       0,   389,   636,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,     0,     0,     0,   374,   375,   376,   377,
     378,   400,   401,   379,   380,   381,   382,     0,   383,   384,
     385,     0,     0,   386,     0,   387,   388,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,   373,     0,     0,   697,     0,     0,   389,     0,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,     0,
       0,     0,   374,   375,   376,   377,   378,   400,   401,   379,
     380,   381,   382,     0,   383,   384,   385,     0,     0,   386,
       0,   387,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   373,     0,     0,
       0,     0,     0,   389,     0,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,     0,     0,     0,   374,   375,
     376,   377,   378,   400,   401,   379,   380,   381,   382,     0,
     383,   384,   385,     0,     0,   386,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,   373,     0,     0,     0,     0,     0,     0,
       0,     0,   391,   392,   393,   394,   395,   396,   397,   398,
     399,     0,     0,     0,   374,   375,   376,   377,   378,   400,
     401,   379,   380,   381,   382,     0,   383,   384,   385,     0,
       0,   386,     0,   387,   388,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,   394,   395,   396,   397,   398,   399,   372,   373,     0,
     374,   375,     0,     0,   378,   400,   401,   379,   380,   381,
     382,     0,   383,   384,   385,     0,     0,   386,     0,   387,
     388,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,     0,   390,   391,   392,   393,   394,     0,     0,
       0,   398,   399,   374,   375,   376,   377,   378,     0,     0,
     379,   400,   401,   382,     0,   383,   384,   385,     0,     0,
     386,     0,   387,   388,     0,   374,   375,   376,   377,     0,
       0,     0,     0,     0,     0,   382,     0,   383,   384,   385,
       0,     0,   386,     0,   387,   388,     0,     0,     0,   393,
     394,   395,   396,   397,   398,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,     0,     0,     0,
       0,     0,     0,   395,   396,   397,   398,   399,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   123,   124,   125,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,     0,
       0,   137,   138
};

static const yytype_int16 yycheck[] =
{
      11,   336,   268,   143,   403,   205,    96,   241,   357,   221,
     222,   223,    18,     0,    96,    19,    20,    45,     5,   307,
      63,   309,   120,   311,    20,   239,   112,   130,    19,    20,
     107,    13,    14,   110,   108,   425,   130,   130,   334,   130,
     143,    28,    31,    30,   166,    32,   342,    34,    17,   130,
     130,    18,    39,    64,   108,   139,    52,    18,    45,    32,
     108,   164,   502,   137,    51,   136,   139,   115,   166,   108,
     130,   470,   130,   167,   167,   136,   167,    13,    14,   165,
     164,   158,   159,   137,   164,    96,   167,   137,   124,   137,
     164,   102,   136,   166,   165,    99,   100,   136,   137,   136,
     139,   161,   130,   107,   165,   109,   110,   111,    99,   100,
     114,   132,   133,   327,   204,   165,   107,   130,   109,   110,
     111,   165,   204,   114,     4,   162,     6,   217,   218,   219,
     136,   221,   222,   223,   224,   217,   218,   219,   434,   221,
     222,   223,   124,   147,   148,   149,   128,   443,   130,   131,
      19,    20,   148,   142,   158,   159,   199,   130,   248,   371,
     550,   130,   158,    56,   130,   255,   248,   158,   159,   136,
     159,   467,   159,   255,   614,   136,   266,   267,   165,   161,
     136,   170,   160,   159,   266,   267,   162,   165,   124,   136,
     424,   164,   128,   204,   130,   131,   136,    33,   136,   136,
     144,   136,   136,   136,   160,   130,   217,   218,   219,   608,
     221,   222,   223,   224,   354,   162,   356,   136,   506,   618,
     160,   165,    19,    20,   162,   162,   160,   162,   494,   162,
      99,   100,   144,   144,   137,   634,   436,   248,   107,   159,
     109,   110,   111,   333,   255,   114,   136,   167,   165,   139,
     167,   333,   142,   165,   165,   266,   267,   347,   348,   658,
     350,   351,   611,    99,   100,   347,   348,   139,   350,   351,
     619,   166,   568,   168,   169,   136,   145,   146,   147,   148,
     149,   164,   160,   579,    19,    20,    10,   165,   500,   158,
     159,   130,   162,   433,   130,   165,   161,    21,    22,   130,
      97,    98,    99,   100,   101,   130,   160,   104,   105,   106,
     107,   165,   109,   110,   111,   405,   165,   114,   167,   116,
     117,   166,   333,   405,   414,   124,   416,   417,   132,   133,
     134,   165,   414,   167,   130,   417,   347,   348,   626,   350,
     351,   138,   130,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   165,   130,   167,   137,   368,    31,   130,
     139,   158,   159,    36,    99,   100,   566,    61,   166,   166,
      33,    65,   107,    31,   574,   110,   111,   712,    36,   114,
      33,   161,    55,   132,    52,   585,   139,    81,    82,    83,
      84,   166,   143,   143,   405,   215,   143,    55,   161,   143,
     143,   143,   143,   414,    31,   416,   417,   143,   166,    36,
     500,   130,   232,   130,    42,   130,   130,   507,   500,   165,
     159,   161,   143,   158,   159,   507,   159,   161,    55,   161,
     143,   143,   632,   143,   161,   108,   109,   162,   169,   139,
     142,   130,   115,   130,   264,   265,   130,   130,   268,   160,
     590,   109,   130,   687,   144,   112,   276,   277,   278,   279,
     280,   281,   662,   144,   137,   138,   165,   144,   124,   142,
      35,    18,   165,   165,   165,   160,   136,   165,   132,   165,
     138,   108,   109,   130,   142,   136,   159,   167,   130,   500,
     161,    12,   165,   139,   124,    38,   507,   170,   130,   166,
     161,   159,    48,   162,   130,    19,    20,   165,   160,   599,
     137,   138,   170,   713,   334,   142,   336,   599,   130,   160,
     160,   160,   342,   144,   160,   160,   346,   144,   361,   349,
     167,   160,   159,    85,    47,   355,   736,   357,   160,   739,
     160,    50,   752,   170,    48,    64,   212,     1,   669,   198,
     416,   419,   290,   338,   374,   375,   590,   155,   378,   379,
     380,   381,    60,   383,   243,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     255,   401,   617,    97,    98,    99,   100,   101,   599,    31,
     104,   105,   106,   107,    36,   109,   110,   111,   357,   747,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
     430,    -1,    -1,    55,   434,    -1,    -1,   437,   438,    31,
      -1,    -1,   442,   443,    36,    -1,    -1,    -1,   448,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,   158,   159,    -1,   467,    -1,    -1,
     740,   471,    -1,    -1,    -1,    -1,    -1,    -1,   740,    -1,
      -1,   481,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     490,   761,    -1,   493,   494,    -1,    -1,   497,    -1,   761,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   699,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   109,    -1,    -1,
     142,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   730,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   547,   170,   740,
     142,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,    -1,   159,   568,    -1,
     761,    19,    20,   165,    -1,    -1,    -1,    -1,   170,   579,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   592,   593,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   603,   604,    -1,   606,    -1,    -1,   609,
      -1,    -1,    -1,    -1,    -1,   615,    -1,   617,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     630,    -1,    -1,    -1,    -1,    -1,   636,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,   114,   115,   116,   117,
     118,   119,    -1,   673,   674,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   683,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   702,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   712,    -1,    -1,    -1,   716,   165,    -1,    -1,
      -1,   721,    -1,    -1,    -1,   725,    -1,   727,   728,    -1,
      -1,    -1,    -1,   733,     1,    -1,    -1,    -1,     5,    -1,
       7,     8,     9,    -1,    11,    -1,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,   755,    -1,    24,    25,    26,
      27,    -1,    -1,   763,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
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
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    13,
      14,    15,    16,   152,   153,    -1,    -1,    -1,    -1,    -1,
     159,    25,   161,   162,    -1,   164,    -1,   166,    -1,   168,
     169,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
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
      -1,   145,   146,   147,    -1,    -1,    -1,    -1,   152,   153,
      -1,    13,    14,    15,    16,    17,    -1,   161,    -1,    -1,
     164,    -1,   166,    25,   168,   169,    31,    -1,    -1,    -1,
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
      -1,    -1,    -1,   138,    -1,    55,    -1,   142,    -1,   144,
      -1,   143,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,   159,    -1,    -1,    -1,    25,   161,
      -1,    -1,   164,    -1,   166,   170,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,   153,    -1,    13,    14,
      15,    16,    -1,   160,   161,    -1,    -1,   164,    -1,   166,
      25,   168,   169,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
     138,    -1,    -1,    -1,   142,    -1,   144,    -1,    -1,    -1,
     145,   146,   147,    13,    14,    15,    16,   152,   153,    -1,
      20,   159,    -1,    -1,    -1,    25,   161,    -1,    -1,   164,
      -1,   166,   170,   168,   169,    -1,    -1,    37,    -1,    -1,
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
      25,   161,    -1,    -1,   164,    -1,   166,    -1,   168,   169,
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
      50,    -1,    -1,    53,    -1,    -1,   109,    57,    58,    59,
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
      25,   161,    -1,    -1,   164,    -1,   166,    -1,   168,   169,
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
     131,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,   145,   146,   147,    -1,    -1,    29,
      -1,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,    44,   166,    -1,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,   164,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    19,
      20,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,
     164,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    19,    20,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    19,    20,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    19,    20,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      19,    20,    -1,    -1,    -1,    -1,    -1,   158,   159,    97,
      98,    99,   100,   101,   165,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,    -1,    -1,    -1,   165,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,    97,    98,    99,   100,   101,   165,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,    -1,    19,    20,    -1,
      -1,    -1,   158,   159,    -1,    -1,   162,    -1,    -1,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,   162,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    19,
      20,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,
     162,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,   162,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    19,    20,    -1,    -1,    -1,    -1,    -1,   158,   159,
      -1,    -1,   162,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,   162,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,    -1,   162,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,   121,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   161,    19,    20,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    19,    20,    -1,
     164,    -1,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,    -1,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,   164,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    -1,    -1,    97,    98,    99,   100,   101,   158,
     159,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   158,   159,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   158,   159,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    -1,    -1,   135,    -1,    -1,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   158,   159,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   158,   159,   104,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    -1,    -1,    97,    98,    99,   100,   101,   158,
     159,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,    19,    20,    -1,
      97,    98,    -1,    -1,   101,   158,   159,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,   148,   149,    97,    98,    99,   100,   101,    -1,    -1,
     104,   158,   159,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    97,    98,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      -1,   152,   153
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   172,     0,     5,    28,    30,    32,    34,    39,    45,
      51,   159,   165,   173,   178,   179,   183,   202,   206,   220,
     252,   256,   257,   260,   130,   197,   198,   124,   184,   185,
     130,   180,   181,   130,   166,   130,    32,   130,   164,   199,
     200,   201,   211,     4,     6,    56,   251,   137,   136,   139,
     136,    20,    52,   148,   158,   182,   139,   166,   130,   254,
     255,   164,   130,   161,   136,   160,   130,   259,    33,    99,
     100,   130,   205,   166,    13,    14,   124,   128,   130,   131,
     161,   175,   197,   124,   185,   130,   130,   130,    61,    65,
      81,    82,    83,    84,   263,   253,   137,   167,   254,   130,
     198,   200,   139,   258,   166,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   116,   117,   118,   119,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   152,   153,    33,
      33,   161,   203,   250,   132,   174,    13,    14,   124,   128,
     130,   175,   195,   196,   182,   166,   130,   167,    23,    29,
      44,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,   211,   262,   264,   265,   266,   162,   211,   238,   159,
     162,   235,   239,   240,   139,   204,   130,   167,   249,   261,
     132,   133,   136,   162,   253,   137,   165,   143,   143,   143,
     161,   143,   143,   143,   143,   143,    31,    36,    55,   109,
     138,   142,   159,   165,   170,   167,   235,   237,   198,    45,
     246,   261,   162,   165,   266,   166,   207,   136,   139,   142,
     248,   195,   167,    13,    14,    15,    16,    25,    37,    41,
      43,    46,    50,    87,    99,   100,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   131,   145,   146,   147,   152,
     153,   161,   168,   169,   177,   207,   211,   213,   222,   223,
     226,   227,   228,   231,   232,   233,   234,   262,   267,   270,
     275,   277,   279,   266,   266,   266,   130,   203,   266,   203,
     266,   203,   266,   130,   241,   242,   266,   130,   243,   244,
      31,   142,   159,   170,   160,   234,    42,   236,   165,   160,
     246,   108,   137,   139,   245,   239,   209,   130,   266,   108,
     115,   137,   247,   266,   267,   161,   161,   143,   143,   161,
     143,   143,   234,   234,    11,   266,    11,   266,   234,   272,
     276,   176,   234,   234,   234,   234,   234,   234,   169,   130,
     161,   202,    19,    20,    97,    98,    99,   100,   101,   104,
     105,   106,   107,   109,   110,   111,   114,   116,   117,   138,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     158,   159,   165,   161,   144,   165,   144,   162,   204,   144,
     204,   144,   204,   144,   139,   144,   165,   139,   144,   165,
     142,   160,   160,   246,   266,   113,   210,   229,   234,     1,
       7,     8,     9,    11,    24,    26,    27,    36,    40,    47,
      49,    53,    54,    88,   165,   167,   186,   187,   190,   191,
     192,   193,   194,   208,   210,   212,   214,   215,   216,   217,
     218,   220,   221,   229,   230,   234,   165,   247,   210,   234,
     161,   130,   234,   266,   266,   234,   266,   266,   261,   130,
     160,   161,   234,   268,   269,   271,   272,   273,   274,   261,
     161,   269,   274,   120,   165,   167,   133,   134,   174,   162,
     143,   211,   159,   162,   224,   234,   203,    17,   130,   130,
     234,   234,   234,   234,   234,   234,   130,   234,   130,   234,
     234,   234,   234,    20,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   130,   234,   162,   224,   266,
     144,   144,   144,   266,   241,   266,   243,   245,   169,   227,
     112,    89,    90,    91,    92,    93,    94,    95,    96,   102,
     103,   108,   115,   118,   119,   137,   234,   261,   108,   165,
     210,   234,   207,   234,   234,   165,   124,    53,   234,   108,
     210,   234,    35,   165,   165,   234,   165,   165,   165,    18,
     219,   165,   108,   137,   210,   234,   165,   162,   224,    17,
     143,   234,   162,   144,   144,   162,   144,   144,    18,   245,
     160,   162,   136,   160,   165,   136,   160,   165,    18,   162,
     167,   167,   234,   272,   167,   234,   203,   266,   130,   268,
     136,   162,   204,   266,   161,   130,   139,   160,   162,   144,
     234,   227,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   207,    18,   210,
     234,   165,    12,   207,   139,   124,   210,   234,   165,   166,
     207,    38,   249,   113,   113,   165,   162,   266,   130,   162,
     234,   234,   234,   161,   224,   234,   269,   130,   160,   268,
     234,   160,   273,   224,   269,   160,   160,   135,   204,   144,
     160,   234,   120,   207,   225,   162,   224,   234,   224,   165,
     207,   165,   209,    10,    21,    22,   188,   189,   234,   234,
     162,   144,   165,   162,   234,   165,   160,   245,   165,   167,
     144,   211,   162,   108,   234,   162,   165,   167,   207,   234,
      17,   234,   130,   162,   234,   160,   234,   234,   160,   211,
     234,   207,   207,   266,   162,   144,   165,   278,   278,   189,
     162,    17,   234,    48,   160,   167,   266,   162,   234,   160,
     160,   162
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
     234,   234,   234,   234,   234,   234,   234,   234,   235,   235,
     236,   236,   237,   238,   238,   239,   239,   240,   240,   241,
     241,   242,   242,   243,   244,   244,   245,   245,   246,   246,
     246,   246,   246,   247,   247,   247,   248,   248,   249,   249,
     249,   249,   249,   250,   250,   251,   251,   252,   253,   253,
     253,   254,   255,   255,   256,   257,   257,   258,   258,   259,
     260,   261,   261,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   263,   263,   263,
     263,   263,   263,   264,   265,   265,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   267,   267,   267,   267,
     268,   268,   269,   269,   270,   270,   270,   270,   270,   271,
     271,   272,   273,   273,   274,   274,   275,   275,   276,   276,
     277,   278,   278,   279,   279
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
       4,     6,     7,     3,     5,     4,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     3,     3,     2,     0,     3,
       0,     1,     3,     0,     3,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     3,
       5,     3,     3,     1,     1,     1,     0,     1,     4,     6,
       5,     5,     4,     0,     2,     0,     1,     5,     0,     3,
       5,     4,     1,     2,     4,     5,     7,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     1,     4,
       3,     4,     2,     3,     2,     4,     3,     2,     2,     3,
       2,     2,     4,     6,     4,     1,     4,     5,     1,     4,
       5,     1,     4,     5,     4,     4,     1,     1,     1,     1,
       3,     5,     1,     3,     5,     4,     7,     5,     7,     1,
       3,     3,     1,     1,     1,     3,     5,     5,     1,     3,
       4,     0,     3,    10,    10
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
          case 130: /* "name"  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3054 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3060 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3066 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3072 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3078 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3084 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3090 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3096 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3102 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3108 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3114 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3120 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3126 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3132 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3138 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3144 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3150 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3156 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3162 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3168 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3174 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3180 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3186 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3192 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3198 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3204 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3210 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3216 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3222 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3228 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3234 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3240 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3246 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3252 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3276 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3282 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* func_addr_expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3348 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3354 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3360 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3366 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3372 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3378 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3384 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* variant_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3390 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* variant_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3396 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3402 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3408 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3414 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3420 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3426 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3432 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3438 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3444 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3450 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3456 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3462 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 269: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3468 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 270: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3474 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 271: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3480 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 272: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3486 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 273: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3492 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 274: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3498 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 275: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3504 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 276: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 277: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 278: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 279: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 396 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3824 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 408 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 409 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 413 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 417 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3851 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 421 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3865 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 430 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 438 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 454 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3935 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 488 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 497 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 518 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 541 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 542 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4017 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 558 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 564 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4033 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 573 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4049 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 579 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4055 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 580 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4061 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 585 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4073 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 586 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 595 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 596 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 600 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 609 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4121 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 621 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 631 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4150 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 649 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4190 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4200 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 668 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 669 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4250 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 675 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 688 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4286 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 704 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4292 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 709 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 716 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 728 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4333 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 732 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 740 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 744 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 745 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 746 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4381 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 754 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 761 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4405 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4513 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 784 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4537 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4543 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4549 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4597 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 800 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 805 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 892 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4781 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 902 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 904 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4799 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 906 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 911 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 915 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 927 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4861 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 938 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4869 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 944 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 945 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 956 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 960 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 982 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4944 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 986 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 993 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4966 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 1001 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4994 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5012 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1020 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5020 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1023 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5030 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1037 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5044 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1038 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1042 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1043 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1047 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5098 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1081 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5106 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5116 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1089 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1097 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1101 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1116 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1121 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5186 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1130 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5194 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5202 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1139 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5208 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1140 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1147 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1158 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5239 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1159 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1163 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5255 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1169 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5311 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5317 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1224 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5323 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5335 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5341 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5347 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5353 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5359 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5365 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5371 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5377 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5383 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1239 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5395 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5401 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5413 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1252 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1256 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1267 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1272 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1282 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5498 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1286 "ds_parser.ypp" /* yacc.c:1660  */
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5510 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1293 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1309 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1310 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1322 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5622 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5640 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1334 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5694 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1335 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5700 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5706 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1337 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5712 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1338 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5718 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1339 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5724 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5730 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1341 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1342 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5742 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1343 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5748 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1345 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1349 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1353 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1359 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1360 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1361 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1362 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5816 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1367 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5832 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5840 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5857 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1387 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1391 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1393 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5911 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1397 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1407 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1417 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5977 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1451 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1452 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1456 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5995 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6001 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1461 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6011 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1469 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6028 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1479 "ds_parser.ypp" /* yacc.c:1660  */
    { 
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true; 
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1484 "ds_parser.ypp" /* yacc.c:1660  */
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl); 
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1493 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1497 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1500 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1509 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1510 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1514 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1523 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1524 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1529 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1534 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1539 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6144 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1546 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6164 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1552 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1561 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 6181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1562 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 6187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1563 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6193 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1567 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1568 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6205 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1572 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6213 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6223 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1580 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1585 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1593 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1604 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1607 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1614 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1615 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1619 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6320 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1649 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6356 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1674 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1695 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6386 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1706 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1721 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1722 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1726 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1765 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6524 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1831 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1838 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6547 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1847 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6553 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1848 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1849 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1850 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1852 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1853 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1856 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1857 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1858 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1859 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1862 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1883 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6740 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1894 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6749 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1898 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1908 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6767 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6773 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1910 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1911 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6798 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1925 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6809 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1931 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1935 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1950 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1957 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1962 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6888 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1970 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1975 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6908 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1980 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1987 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1992 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1998 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2003 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6960 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2007 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2012 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2021 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2025 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2030 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7017 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2039 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7026 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2043 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7036 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2048 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2057 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7061 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2063 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2072 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7078 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2073 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2074 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7090 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2075 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2079 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2086 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2095 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2100 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2107 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7148 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2112 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2118 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7170 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2131 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7195 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 409:
#line 2142 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 410:
#line 2145 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 411:
#line 2159 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 412:
#line 2168 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 413:
#line 2169 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 414:
#line 2173 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 415:
#line 2178 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7261 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 416:
#line 2185 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7271 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 417:
#line 2190 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7284 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 418:
#line 2201 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 419:
#line 2206 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7303 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 420:
#line 2213 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7317 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 421:
#line 2225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7323 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 422:
#line 2226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 423:
#line 2230 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 424:
#line 2244 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7367 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7371 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2260 "ds_parser.ypp" /* yacc.c:1903  */


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


