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

#line 100 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_DAS_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_DAS_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED
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
            delete pNameList;
            delete pTypeDecl;
            delete pInit;
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

#line 182 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */

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
    DAS_TBOOL = 299,
    DAS_TVOID = 300,
    DAS_TSTRING = 301,
    DAS_TAUTO = 302,
    DAS_TINT = 303,
    DAS_TINT2 = 304,
    DAS_TINT3 = 305,
    DAS_TINT4 = 306,
    DAS_TUINT = 307,
    DAS_TUINT2 = 308,
    DAS_TUINT3 = 309,
    DAS_TUINT4 = 310,
    DAS_TFLOAT = 311,
    DAS_TFLOAT2 = 312,
    DAS_TFLOAT3 = 313,
    DAS_TFLOAT4 = 314,
    DAS_TRANGE = 315,
    DAS_TURANGE = 316,
    DAS_TBLOCK = 317,
    DAS_TINT64 = 318,
    DAS_TUINT64 = 319,
    DAS_TDOUBLE = 320,
    DAS_TFUNCTION = 321,
    DAS_TLAMBDA = 322,
    ADDEQU = 323,
    SUBEQU = 324,
    DIVEQU = 325,
    MULEQU = 326,
    MODEQU = 327,
    ANDEQU = 328,
    OREQU = 329,
    XOREQU = 330,
    SHL = 331,
    SHR = 332,
    ADDADD = 333,
    SUBSUB = 334,
    LEEQU = 335,
    SHLEQU = 336,
    SHREQU = 337,
    GREQU = 338,
    EQUEQU = 339,
    NOTEQU = 340,
    RARROW = 341,
    LARROW = 342,
    QQ = 343,
    QDOT = 344,
    LPIPE = 345,
    LBPIPE = 346,
    RPIPE = 347,
    CLONEEQU = 348,
    INTEGER = 349,
    LONG_INTEGER = 350,
    UNSIGNED_INTEGER = 351,
    UNSIGNED_LONG_INTEGER = 352,
    FLOAT = 353,
    DOUBLE = 354,
    NAME = 355,
    BEGIN_STRING = 356,
    STRING_CHARACTER = 357,
    END_STRING = 358,
    BEGIN_STRING_EXPR = 359,
    END_STRING_EXPR = 360,
    UNARY_MINUS = 361,
    UNARY_PLUS = 362,
    PRE_INC = 363,
    PRE_DEC = 364,
    POST_INC = 365,
    POST_DEC = 366,
    COLCOL = 367
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

#line 332 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:353  */
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

#endif /* !YY_DAS_YY_USERS_BORISBATKIN_WORK_YZG_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   3794

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  307
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  547

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   367

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   124,     2,     2,   138,   119,   112,     2,
     132,   133,   117,   116,   106,   115,   129,   118,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   109,   137,
     113,   107,   114,   108,   120,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   130,     2,   131,   111,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   135,   110,   136,   123,     2,     2,     2,
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
     105,   121,   122,   125,   126,   127,   128,   134
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   238,   238,   240,   241,   242,   243,   244,   245,   246,
     247,   251,   252,   256,   260,   263,   272,   280,   296,   317,
     321,   322,   326,   336,   340,   341,   345,   348,   354,   355,
     356,   363,   370,   382,   392,   401,   402,   403,   404,   405,
     406,   410,   415,   423,   424,   428,   435,   447,   451,   458,
     459,   463,   464,   465,   469,   473,   480,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   519,
     588,   591,   601,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   613,   617,   621,   627,   633,   647,
     648,   654,   658,   664,   667,   670,   674,   680,   684,   688,
     691,   694,   702,   708,   709,   713,   714,   718,   750,   753,
     759,   763,   768,   772,   780,   783,   789,   794,   845,   846,
     847,   848,   849,   850,   854,   855,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,   866,   867,   871,   882,
     887,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,   908,   909,   910,   911,   912,   913,   914,   915,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
     926,   927,   928,   929,   930,   931,   932,   933,   937,   941,
     944,   947,   948,   949,   950,   951,   952,   955,   956,   957,
     958,   959,   960,   961,   971,   981,  1015,  1016,  1020,  1021,
    1025,  1033,  1036,  1043,  1044,  1048,  1049,  1053,  1054,  1058,
    1062,  1065,  1069,  1074,  1082,  1085,  1089,  1093,  1098,  1106,
    1134,  1137,  1146,  1158,  1170,  1184,  1185,  1189,  1227,  1293,
    1300,  1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,
    1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,  1327,
    1328,  1332,  1342,  1346,  1356,  1357,  1358,  1359,  1372,  1377,
    1382,  1387,  1392,  1397,  1404,  1408,  1413,  1419,  1424,  1428,
    1433,  1442,  1446,  1451,  1460,  1464,  1469,  1481,  1482,  1486,
    1493,  1502,  1507,  1514,  1519,  1528,  1533,  1540
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
  "DAS_CONTINUE", "DAS_TBOOL", "DAS_TVOID", "DAS_TSTRING", "DAS_TAUTO",
  "DAS_TINT", "DAS_TINT2", "DAS_TINT3", "DAS_TINT4", "DAS_TUINT",
  "DAS_TUINT2", "DAS_TUINT3", "DAS_TUINT4", "DAS_TFLOAT", "DAS_TFLOAT2",
  "DAS_TFLOAT3", "DAS_TFLOAT4", "DAS_TRANGE", "DAS_TURANGE", "DAS_TBLOCK",
  "DAS_TINT64", "DAS_TUINT64", "DAS_TDOUBLE", "DAS_TFUNCTION",
  "DAS_TLAMBDA", "ADDEQU", "SUBEQU", "DIVEQU", "MULEQU", "MODEQU",
  "ANDEQU", "OREQU", "XOREQU", "SHL", "SHR", "ADDADD", "SUBSUB", "LEEQU",
  "SHLEQU", "SHREQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW", "LARROW",
  "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE", "CLONEEQU", "INTEGER",
  "LONG_INTEGER", "UNSIGNED_INTEGER", "UNSIGNED_LONG_INTEGER", "FLOAT",
  "DOUBLE", "NAME", "BEGIN_STRING", "STRING_CHARACTER", "END_STRING",
  "BEGIN_STRING_EXPR", "END_STRING_EXPR", "','", "'='", "'?'", "':'",
  "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "'@'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "'.'", "'['", "']'", "'('", "')'", "COLCOL",
  "'{'", "'}'", "';'", "'$'", "$accept", "program", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "options_declaration", "require_declaration", "require_list",
  "require_module", "expect_declaration", "expect_list", "expect_error",
  "expression_else", "expression_if_then_else", "expression_for_loop",
  "expression_while_loop", "expression_with", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
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
  "function_argument_declaration", "function_argument_list",
  "copy_or_move", "variable_declaration", "let_variable_declaration",
  "global_let", "enum_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "structure_declaration",
  "variable_name_list", "basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration",
  "type_declaration", "make_decl", "make_struct_fields", "make_struct_dim",
  "make_struct_decl", "make_dim", "make_dim_decl", YY_NULLPTR
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
     355,   356,   357,   358,   359,   360,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
      64,   361,   362,   126,    33,   363,   364,   365,   366,    46,
      91,    93,    40,    41,   367,   123,   125,    59,    36
};
# endif

#define YYPACT_NINF -362

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-362)))

#define YYTABLE_NINF -145

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -362,    13,  -362,  -362,   -70,   -43,   -39,   -32,    -4,  -362,
      -6,  -362,  -362,  -362,   165,  -362,     6,  -362,  -362,  -362,
    -362,    24,  -362,   110,   149,   153,  -362,  -362,   157,  -362,
     134,   138,  -362,   144,   179,   151,  -362,   -73,  -362,   184,
      73,  -362,   176,   -70,   191,   -43,   -39,  -362,  2515,   190,
    -362,   -70,    -6,  -362,   182,   159,  3203,   269,   270,  -362,
     168,   -84,  -362,  -362,  -362,  -362,  -362,   199,  -362,  -362,
    -362,  -362,  -362,   -77,   192,   197,   200,  -362,  -362,  -362,
     180,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,   201,  -362,  -362,  -362,   202,
     203,  -362,  -362,  -362,  -362,   438,  -362,   -66,  -362,   -72,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,     4,   209,   -70,  -362,   281,   185,
    -362,   139,   -88,  -362,  2515,  2515,  2515,   223,    25,    25,
      25,  -362,   -15,  -362,  -362,  1088,  -362,  -362,    99,   226,
    -362,  -362,  -362,    70,  -362,   108,  2515,   193,    48,  -362,
     226,  -362,  -362,  -362,   233,  -362,    68,   145,   174,   196,
     209,   181,   209,   194,   209,   195,   218,  -362,  -362,  -362,
     366,   206,  -362,   214,   221,   222,   230,   232,  2358,  2358,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  2358,  2358,   -72,
    2358,  2358,   235,  -362,  2358,  -362,   236,  -362,  -362,   -69,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  3220,   237,  -362,
    -362,  -362,  -362,  -362,  -362,    92,  -362,   231,  -362,  2515,
    2358,   438,  -362,  -362,  -362,   234,  -362,  2515,  -362,  -362,
     254,  -362,   258,  -362,   259,  -362,  -362,     2,  -362,   274,
    2358,  2515,  2515,  2358,  1215,   470,   470,    82,   470,   470,
    -362,   470,   470,  2515,  2847,   569,   168,  -362,  1342,  2358,
    2358,  -362,  -362,  2358,  2358,  2358,  2358,   277,  2358,   278,
    2358,  2358,  2358,  2358,  2358,  2358,  2358,  2358,  2358,  2358,
    2358,  2358,  2358,   280,  2358,  -362,  1469,  -362,  -362,   136,
    -362,   290,  2688,  -362,   376,  -362,  -362,  -362,  1596,   961,
    2905,   408,   409,  2963,  -362,  3021,  -362,  2358,   285,   834,
    -362,  -362,  2358,  2358,   226,  1723,  -362,   193,  2358,  2358,
    -362,   351,  -362,  -362,  -362,  -362,  -362,  -362,  -362,   246,
     251,   256,   257,  -362,   380,  -362,   -34,  2751,   209,   -64,
    -362,    54,  3428,  3649,  3649,  3604,  3604,  3587,  3587,   266,
     470,  -362,    75,    75,  3332,  3476,  3524,  3542,  3604,  3604,
    3664,  3664,   415,   415,   415,  -362,  3276,  -362,    64,  2358,
      93,  2358,  2358,  2358,  2358,  2358,  2358,  2358,  2358,  2358,
    2358,  2358,  2358,  2358,  -362,  -362,    85,  2515,   299,  3079,
    -362,  2358,  2358,  -362,  -362,  3380,   -81,  1850,  1977,  3428,
     294,   101,   109,  2799,  2799,     0,  2358,  3428,   390,  3428,
    2799,   267,  -362,  -362,  -362,  -362,   370,   226,  -362,   193,
      32,    69,  2358,  -362,  2104,  2358,  -362,  -362,  3428,  -362,
    3428,  3428,  3428,  3428,  3428,  3428,  3428,  3428,  3428,  3428,
    3428,  3428,  3428,  -362,    -7,   291,  -362,  3604,  3604,  -362,
    2358,  2458,   309,   337,   307,   309,   308,  2358,  -362,    38,
    2358,  3428,   193,  -362,  -362,  -362,  -362,   143,  -362,   310,
    3428,  -362,    87,  3428,  -362,  2231,  3428,   123,    32,  -362,
     294,  -362,  3428,   193,  2358,  -362,   -71,  -362,   707,  2515,
    2358,  -362,  -362,  2515,  3137,   311,  2358,  -362,  2799,  -362,
    -362,    28,  -362,  2548,    66,  -362,  -362,  3428,    38,  -362,
    2358,  -362,  -362,  -362,  -362,  2618,  -362
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,   125,     0,     0,     0,     0,     0,   126,
       0,     9,     8,     7,     0,     6,     0,     5,    10,     4,
       3,     0,    41,    18,    26,    23,    24,    22,    19,    20,
       0,     0,    44,   109,     0,    45,    47,     0,    43,     0,
       0,   221,     0,     0,     0,     0,     0,   240,     0,     0,
     111,     0,     0,    50,   245,     0,     0,     0,     0,    56,
      51,   216,    39,    40,    37,    38,    36,     0,    35,    42,
      27,    25,    21,     0,     0,     0,     0,   251,   267,   252,
     272,   253,   255,   256,   257,   258,   260,   261,   262,   263,
     264,   265,   266,   268,   269,   288,   254,   259,   270,   291,
     294,   271,   274,   276,   275,   243,   110,     0,    48,     0,
     247,   221,    59,    60,    62,    61,    63,    64,    65,    66,
      85,    86,    83,    84,    76,    87,    88,    77,    74,    75,
      79,    80,    78,    72,    73,    68,    67,    69,    70,    71,
      58,    57,    81,    82,     0,    54,     0,   239,   218,     0,
      11,     0,     0,   244,     0,     0,     0,     0,    51,    51,
      51,   279,     0,   284,   281,     0,    46,   246,   216,     0,
     249,    52,   225,     0,   223,   229,     0,     0,     0,   219,
       0,   222,    12,    13,     0,   241,     0,     0,     0,     0,
      54,     0,    54,     0,    54,     0,   280,   282,   166,   167,
       0,     0,   161,     0,     0,     0,     0,     0,     0,     0,
     138,   140,   139,   141,   142,   143,    14,     0,     0,     0,
       0,     0,     0,   278,     0,   105,    49,   164,   136,   162,
     209,   208,   207,   212,   163,   211,   210,     0,     0,   165,
     297,   298,   248,   224,    53,     0,   228,     0,   227,     0,
       0,    55,    89,   217,   220,     0,   285,     0,   287,   273,
       0,   289,     0,   292,     0,   295,   283,   113,   116,     0,
       0,     0,     0,     0,     0,   188,   189,     0,   171,   170,
     196,   169,   168,     0,     0,     0,    51,   215,     0,     0,
       0,   190,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,   226,   250,   230,
     233,     0,   232,   242,     0,   290,   293,   296,     0,     0,
       0,     0,     0,     0,   203,     0,    17,     0,    15,     0,
     192,   107,     0,     0,     0,   119,   117,     0,     0,     0,
     118,    90,    92,   103,    99,    97,    98,   106,    93,     0,
       0,     0,     0,   104,   123,    96,     0,   144,    54,     0,
     197,     0,   134,   172,   173,   183,   184,   181,   182,     0,
     205,   195,   213,   214,     0,   186,   187,   185,   179,   180,
     175,   174,   176,   177,   178,   194,     0,   199,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   114,     0,     0,     0,     0,
     201,     0,     0,   202,   204,     0,   109,     0,     0,   305,
     301,     0,     0,     0,     0,     0,     0,   120,     0,   112,
       0,     0,    95,   100,   101,   102,     0,     0,    94,     0,
       0,     0,     0,   198,     0,     0,   193,   200,   231,   108,
     151,   152,   154,   153,   155,   148,   149,   150,   156,   157,
     146,   147,   145,   115,     0,     0,   130,   128,   129,    16,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    28,
       0,   121,     0,    34,   105,   124,   127,     0,   137,     0,
     135,   159,     0,   206,   132,     0,   299,     0,     0,   303,
     302,   307,   306,     0,     0,    31,     0,   122,     0,     0,
       0,   158,   160,     0,     0,     0,     0,    29,     0,    32,
      91,     0,   238,   144,     0,   131,   304,   300,    28,   234,
       0,   237,   133,    30,   236,   144,   235
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -362,  -362,   167,  -362,  -362,  -362,  -362,  -362,  -362,   399,
    -362,  -362,   402,   -76,  -362,  -362,  -362,  -362,   418,   -19,
    -362,   411,  -362,   238,  -144,  -187,  -362,  -362,  -153,  -362,
     -27,  -248,   -10,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
     472,  -362,  -362,  -362,  -307,    77,  -362,  -281,  -362,  -362,
      47,  -362,  -362,  -362,   367,   241,  -362,  -164,   -13,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  -317,   -47,  -362,
    -362,   -38,   279,  -361,    -2,  -362,  -362,  -362
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   151,    68,   277,   227,    11,    12,    28,    29,
      13,    25,    26,   515,   353,   354,   355,   356,    22,    23,
      35,    36,    37,    14,   145,   177,    60,    15,   228,   357,
     285,   358,   229,   359,   230,   360,   361,   362,   363,   447,
     364,   365,   231,   232,   371,   233,   234,   321,   235,   236,
     372,   148,   180,   149,    61,   172,   173,   480,   174,   496,
      17,    73,    18,    19,   110,    55,    20,   175,   238,   103,
     104,   339,   239,   430,   431,   240,   432,   241
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,   102,   320,   260,   366,   262,   246,   264,   451,   398,
     105,   250,   196,     2,   190,   192,   194,   490,     3,   184,
     161,   416,    32,   152,   252,   162,   248,   435,    33,   161,
      21,   287,   107,    52,   162,   452,   450,     4,   101,     5,
      43,     6,    38,     7,    74,   169,   146,   513,     8,   185,
      75,    24,   147,    49,     9,   161,   514,   400,    53,   153,
     162,    27,    34,   288,   225,    76,   283,   166,    30,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   161,    33,   161,    31,   197,   162,   167,
     162,   163,    56,   448,   170,   164,   247,   102,   102,   102,
     163,   102,   102,   102,   164,   246,   186,   187,   188,   246,
     191,   193,   195,   165,   510,    33,   504,   178,    34,   102,
     497,    42,   165,   169,   328,   248,   163,   171,   251,   248,
     164,    41,   368,    10,   101,   101,   101,   502,   101,   101,
     101,    57,    58,   102,    43,   399,   243,   144,   165,    34,
     452,   297,   267,   161,   299,   539,   101,   254,   162,    39,
     452,    40,   161,    59,   163,   483,   163,   162,   164,   253,
     164,   449,   256,   516,   150,   336,   337,   453,    62,    63,
     101,   452,   170,   452,   438,   246,   165,   457,   165,   542,
     499,   161,   102,   244,   313,   314,   162,   245,   161,   280,
     102,   319,   237,   162,   247,   248,    43,   249,   473,   324,
     522,   161,   161,   246,   102,   102,   162,   162,   225,   146,
     246,   226,   484,   331,   332,   242,   102,   366,   485,   101,
     486,   182,   183,   248,   163,    48,   487,   101,   164,   247,
     248,   257,   519,   163,   525,   275,   276,   164,    44,    45,
     485,   101,   101,    46,   278,   279,   165,   281,   282,    47,
      64,   284,   532,   101,    65,   165,    66,    67,    49,    50,
     488,   489,   163,    51,    54,    70,   164,   493,   258,   163,
     106,   109,   544,   164,   111,   261,   498,   322,   142,   143,
     144,   150,   163,   163,   165,   154,   164,   164,   263,   265,
     155,   165,   157,   156,   158,   159,   160,   330,   176,   179,
     333,   335,   181,   189,   165,   165,   170,   255,   225,   259,
     266,   318,   367,   520,   271,   272,   373,   374,   269,   517,
     375,   376,   377,   378,   526,   380,   270,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     527,   396,   273,   529,   274,   283,    10,   540,   325,   316,
     102,   323,   326,   327,   329,   538,   419,   379,   381,   474,
     395,   400,   441,   442,   425,    74,   429,   182,   443,   433,
     434,    75,   437,   444,   445,   439,   440,   446,   454,   475,
     483,   492,   494,   161,   495,   505,    76,   101,   162,   450,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   102,   161,   161,   508,   509,   511,
     162,   162,   536,   521,   338,    72,   458,    71,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,    69,   543,   108,   286,   161,    33,   518,   477,   478,
     162,   101,   102,    16,   237,   284,   102,   459,   168,   268,
     507,   531,     0,   491,   163,   534,   317,     0,   164,     0,
     414,     0,     0,   291,   292,     0,   222,     0,     0,   500,
      34,   297,   503,   298,   299,   300,   165,   301,     0,   101,
       0,     0,     0,   101,     0,     0,   163,   163,     0,     0,
     164,   164,   421,   422,     0,     0,     0,   506,     0,     0,
       0,     0,     0,     0,   512,     0,     0,     0,   165,   165,
       0,     0,     0,     0,   313,   314,   163,     0,   291,   292,
     164,     0,   524,     0,     0,     0,   297,     0,     0,   299,
     300,   528,   301,     0,     0,   367,     0,   533,   165,     0,
     341,     0,     0,   537,     3,     0,   342,   343,     0,   344,
       0,   198,   199,   200,   201,     0,     0,   545,     0,   345,
     202,   346,   347,     0,     0,     0,     0,     0,     0,   313,
     314,   348,   203,     0,     0,   349,   204,     0,   205,     0,
       9,   206,   350,    77,    78,    79,     0,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,     0,    96,    97,    98,     0,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   210,   211,   212,   213,   214,   215,    33,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   218,     0,     0,     0,   219,
       0,     0,   220,   221,     0,     0,     0,     0,     0,   222,
       0,   224,     0,    34,   225,   351,   352,   226,   341,     0,
       0,     0,     3,     0,   342,   343,     0,   344,     0,   198,
     199,   200,   201,     0,     0,     0,     0,   345,   202,   346,
     347,     0,     0,     0,     0,     0,     0,     0,     0,   348,
     203,     0,     0,   349,   204,     0,   205,     0,     9,   206,
     350,    77,    78,    79,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      96,    97,    98,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   209,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   211,   212,   213,   214,   215,    33,   216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   218,     0,     0,     0,   219,     0,     0,
     220,   221,     0,     0,     0,     0,     0,   222,     0,   224,
       0,    34,   225,   530,   352,   226,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,   161,     0,     0,     0,     0,   162,   203,     0,     0,
       0,   204,     0,   205,     0,     0,   206,     0,    77,    78,
      79,     0,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,     0,    96,    97,    98,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   209,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   210,   211,
     212,   213,   214,   215,   426,   216,     0,     0,     0,     0,
       0,     0,   163,     0,     0,     0,   164,     0,     0,   217,
     218,     0,     0,     0,   219,     0,     0,   220,   221,     0,
       0,     0,     0,     0,   427,     0,   428,     0,    34,   225,
       0,     0,   226,   198,   199,   200,   201,   417,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,   204,     0,
     205,     0,     0,   206,     0,    77,    78,    79,     0,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,     0,    96,    97,    98,     0,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,   211,   212,   213,   214,
     215,    33,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   418,     0,   217,   218,     0,     0,
       0,   219,     0,     0,   220,   221,     0,     0,     0,     0,
       0,   222,     0,   224,     0,    34,   225,     0,     0,   226,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,   204,     0,   205,     0,     0,
     206,     0,    77,    78,    79,     0,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
       0,    96,    97,    98,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   209,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   211,   212,   213,   214,   215,    33,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,   218,     0,     0,     0,   219,     0,
       0,   220,   221,     0,     0,     0,     0,     0,   222,   223,
     224,     0,    34,   225,     0,     0,   226,   198,   199,   200,
     201,     0,     0,     0,     0,     0,   202,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   203,     0,
       0,     0,   204,     0,   205,     0,     0,   206,     0,    77,
      78,    79,     0,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,     0,    96,    97,
      98,     0,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   209,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   210,
     211,   212,   213,   214,   215,    33,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,   218,     0,     0,     0,   219,     0,     0,   220,   221,
       0,     0,     0,     0,     0,   222,     0,   224,   334,    34,
     225,     0,     0,   226,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,     0,     0,   204,
       0,   205,     0,     0,   206,     0,    77,    78,    79,     0,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,     0,    96,    97,    98,     0,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   210,   211,   212,   213,
     214,   215,    33,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   217,   218,     0,
       0,     0,   219,     0,     0,   220,   221,     0,     0,     0,
       0,     0,   369,     0,   224,   370,    34,   225,     0,     0,
     226,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,     0,   204,     0,   205,     0,
       0,   206,     0,    77,    78,    79,     0,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,     0,    96,    97,    98,     0,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   210,   211,   212,   213,   214,   215,    33,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   218,     0,     0,     0,   219,
       0,     0,   220,   221,     0,     0,     0,     0,     0,   222,
       0,   224,   397,    34,   225,     0,     0,   226,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   203,
       0,     0,     0,   204,     0,   205,     0,     0,   206,     0,
      77,    78,    79,     0,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,    96,
      97,    98,     0,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   209,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     210,   211,   212,   213,   214,   215,    33,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,   218,     0,     0,     0,   219,     0,     0,   220,
     221,     0,     0,     0,     0,     0,   222,     0,   224,   415,
      34,   225,     0,     0,   226,   198,   199,   200,   201,     0,
       0,     0,     0,     0,   202,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   203,     0,     0,     0,
     204,     0,   205,     0,     0,   206,     0,    77,    78,    79,
       0,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,     0,    96,    97,    98,     0,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,   209,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,   210,   211,   212,
     213,   214,   215,    33,   216,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   218,
       0,     0,     0,   219,     0,     0,   220,   221,     0,     0,
       0,     0,     0,   222,     0,   224,     0,    34,   225,     0,
       0,   226,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   203,     0,     0,     0,   204,     0,   205,
       0,     0,   206,     0,    77,    78,    79,     0,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,     0,    96,    97,    98,     0,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,   209,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   210,   211,   212,   213,   214,   215,
      33,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   217,   218,     0,     0,     0,
     219,     0,     0,   220,   221,     0,     0,     0,     0,     0,
     481,   223,   224,     0,    34,   225,     0,     0,   226,   198,
     199,   200,   201,     0,     0,     0,     0,     0,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     203,     0,     0,     0,   204,     0,   205,     0,     0,   206,
       0,    77,    78,    79,     0,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      96,    97,    98,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   209,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   211,   212,   213,   214,   215,    33,   216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   218,     0,     0,     0,   219,     0,     0,
     220,   221,     0,     0,     0,     0,     0,   222,     0,   224,
     482,    34,   225,     0,     0,   226,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   203,     0,     0,
       0,   204,     0,   205,     0,     0,   206,     0,    77,    78,
      79,     0,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,     0,    96,    97,    98,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   209,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   210,   211,
     212,   213,   214,   215,    33,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   217,
     218,     0,     0,     0,   219,     0,     0,   220,   221,     0,
       0,     0,     0,     0,   222,     0,   224,   501,    34,   225,
       0,     0,   226,   198,   199,   200,   201,   523,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,   204,     0,
     205,     0,     0,   206,     0,    77,    78,    79,     0,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,     0,    96,    97,    98,     0,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,   211,   212,   213,   214,
     215,    33,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   218,     0,     0,
       0,   219,     0,     0,   220,   221,     0,     0,     0,     0,
       0,   222,     0,   224,     0,    34,   225,     0,     0,   226,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,   204,     0,   205,     0,     0,
     206,     0,    77,    78,    79,     0,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
       0,    96,    97,    98,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   209,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   211,   212,   213,   214,   215,    33,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,   218,     0,     0,    74,   219,     0,
       0,   220,   221,    75,     0,     0,     0,     0,   222,     0,
     224,     0,    34,   225,     0,     0,   226,     0,    76,     0,
       0,     0,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     0,    33,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,     0,     0,     0,     0,     0,   283,     0,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,   401,   402,   403,   404,
     405,   406,   407,   408,   289,   290,   291,   292,   293,   409,
     410,   294,   295,   296,   297,   411,   298,   299,   300,     0,
     301,   412,     0,     0,     0,     0,     0,     0,     0,    34,
       0,     0,     0,     0,     0,   413,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   314,     0,
       0,     0,     0,     0,     0,   541,   401,   402,   403,   404,
     405,   406,   407,   408,   289,   290,   291,   292,   293,   409,
     410,   294,   295,   296,   297,   411,   298,   299,   300,     0,
     301,   412,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   314,     0,
       0,     0,     0,     0,     0,   546,   401,   402,   403,   404,
     405,   406,   407,   408,   289,   290,   291,   292,   293,   409,
     410,   294,   295,   296,   297,   411,   298,   299,   300,  -144,
     301,   412,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   314,   401,
     402,   403,   404,   405,   406,   407,   408,   289,   290,   291,
     292,   293,   409,   410,   294,   295,   296,   297,   411,   298,
     299,   300,     0,   301,   412,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,     0,     0,     0,   289,   290,   291,   292,   293,
     313,   314,   294,   295,   296,   297,     0,   298,   299,   300,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,     0,
       0,     0,     0,   289,   290,   291,   292,   293,   313,   314,
     294,   295,   296,   297,   225,   298,   299,   300,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,   314,     0,     0,
     340,   289,   290,   291,   292,   293,     0,     0,   294,   295,
     296,   297,     0,   298,   299,   300,     0,   301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,   314,     0,     0,   420,   289,
     290,   291,   292,   293,     0,     0,   294,   295,   296,   297,
       0,   298,   299,   300,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,   314,     0,     0,   423,   289,   290,   291,
     292,   293,     0,     0,   294,   295,   296,   297,     0,   298,
     299,   300,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,   314,     0,     0,   424,   289,   290,   291,   292,   293,
       0,     0,   294,   295,   296,   297,     0,   298,   299,   300,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   314,
       0,     0,   476,   289,   290,   291,   292,   293,     0,     0,
     294,   295,   296,   297,     0,   298,   299,   300,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,   314,     0,     0,
     535,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,   292,
     293,     0,     0,   294,   295,   296,   297,     0,   298,   299,
     300,     0,   301,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,     0,     0,     0,   140,   141,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
     314,   315,   289,   290,   291,   292,   293,     0,     0,   294,
     295,   296,   297,     0,   298,   299,   300,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,   314,   456,   289,   290,
     291,   292,   293,     0,     0,   294,   295,   296,   297,     0,
     298,   299,   300,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,   455,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,     0,     0,     0,     0,   289,   290,   291,   292,
     293,   313,   314,   294,   295,   296,   297,     0,   298,   299,
     300,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   479,     0,     0,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
       0,     0,     0,     0,   289,   290,   291,   292,   293,   313,
     314,   294,   295,   296,   297,     0,   298,   299,   300,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,     0,     0,
       0,     0,   289,   290,   291,   292,   293,   313,   314,   294,
     295,   296,   297,     0,   298,   299,   300,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,     0,     0,     0,
     289,   290,   291,   292,   293,   313,   314,   294,   295,   296,
     297,     0,   298,   299,   300,     0,   301,     0,   289,   290,
     291,   292,   293,     0,     0,   294,   295,   296,   297,     0,
     298,   299,   300,     0,   301,     0,   305,   306,   307,   308,
     309,   310,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314,   306,   307,   308,   309,   310,
     311,   312,     0,   289,   290,   291,   292,   293,     0,     0,
     294,   313,   314,   297,     0,   298,   299,   300,     0,   301,
     289,   290,   291,   292,     0,     0,     0,     0,     0,     0,
     297,     0,   298,   299,   300,     0,   301,     0,     0,     0,
     306,   307,   308,   309,   310,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,   314,     0,   308,
     309,   310,   311,   312,     0,     0,     0,   291,   292,     0,
       0,     0,     0,   313,   314,   297,     0,   298,   299,   300,
       0,   301,   291,   292,     0,     0,     0,     0,     0,     0,
     297,     0,   298,   299,   300,     0,   301,     0,     0,     0,
       0,     0,     0,     0,   308,   309,   310,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   314,
       0,   310,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314
};

static const yytype_int16 yycheck[] =
{
      10,    48,   250,   190,   285,   192,    87,   194,   369,   316,
      48,   175,    27,     0,   158,   159,   160,    17,     5,   107,
      27,   328,    28,   100,   177,    32,   107,   344,   100,    27,
     100,   100,    51,   106,    32,   106,   100,    24,    48,    26,
     106,    28,    52,    30,    19,    41,   130,     9,    35,   137,
      25,    94,   136,   134,    41,    27,    18,    91,   131,   136,
      32,   100,   134,   132,   135,    40,   130,   133,   100,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    27,   100,    27,   100,   112,    32,   109,
      32,   108,    29,   137,   100,   112,   106,   154,   155,   156,
     108,   158,   159,   160,   112,    87,   154,   155,   156,    87,
     158,   159,   160,   130,   485,   100,   133,   146,   134,   176,
     447,   107,   130,    41,   132,   107,   108,   133,   176,   107,
     112,   135,   286,   130,   154,   155,   156,   454,   158,   159,
     160,    78,    79,   200,   106,   319,   169,   132,   130,   134,
     106,    86,   200,    27,    89,   137,   176,   180,    32,     4,
     106,     6,    27,   100,   108,   106,   108,    32,   112,   131,
     112,   368,   114,   490,   102,   103,   104,   133,    12,    13,
     200,   106,   100,   106,   347,    87,   130,   133,   130,   133,
     131,    27,   249,   133,   129,   130,    32,   137,    27,   219,
     257,   249,   165,    32,   106,   107,   106,   109,   133,   257,
     133,    27,    27,    87,   271,   272,    32,    32,   135,   130,
      87,   138,   131,   271,   272,   136,   283,   518,   137,   249,
     131,   102,   103,   107,   108,   107,   137,   257,   112,   106,
     107,   106,   109,   108,   131,   208,   209,   112,   109,   106,
     137,   271,   272,   106,   217,   218,   130,   220,   221,   135,
      94,   224,   520,   283,    98,   130,   100,   101,   134,   100,
     433,   434,   108,   132,   100,    94,   112,   440,   114,   108,
     100,   109,   540,   112,   135,   114,   449,   250,    29,    29,
     132,   102,   108,   108,   130,   113,   112,   112,   114,   114,
     113,   130,   132,   113,   113,   113,   113,   270,   109,    38,
     273,   274,   137,   100,   130,   130,   100,    94,   135,   133,
     112,   100,   285,   497,   113,   113,   289,   290,   132,   492,
     293,   294,   295,   296,   508,   298,   132,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     513,   314,   132,   516,   132,   130,   130,   531,   114,   132,
     417,   137,   114,   114,   100,   528,   329,   100,   100,   417,
     100,    91,    31,   137,   337,    19,   339,   102,   137,   342,
     343,    25,   345,   137,   137,   348,   349,    17,   132,   100,
     106,    11,   135,    27,    34,   114,    40,   417,    32,   100,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,   481,    27,    27,   100,   131,   131,
      32,    32,   131,   133,   277,    46,   399,    45,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,    43,   538,    52,   226,    27,   100,   494,   421,   422,
      32,   481,   519,     1,   427,   428,   523,   400,   111,   200,
     482,   519,    -1,   436,   108,   523,   245,    -1,   112,    -1,
     114,    -1,    -1,    78,    79,    -1,   130,    -1,    -1,   452,
     134,    86,   455,    88,    89,    90,   130,    92,    -1,   519,
      -1,    -1,    -1,   523,    -1,    -1,   108,   108,    -1,    -1,
     112,   112,   114,   114,    -1,    -1,    -1,   480,    -1,    -1,
      -1,    -1,    -1,    -1,   487,    -1,    -1,    -1,   130,   130,
      -1,    -1,    -1,    -1,   129,   130,   108,    -1,    78,    79,
     112,    -1,   505,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,   514,    92,    -1,    -1,   518,    -1,   520,   130,    -1,
       1,    -1,    -1,   526,     5,    -1,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    -1,   540,    -1,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    32,    33,    -1,    -1,    36,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,   135,   136,   137,   138,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
      -1,   134,   135,   136,   137,   138,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    44,    45,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,
     116,    -1,    -1,    -1,   120,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,
      -1,    -1,   138,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    44,    45,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,   120,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,    -1,   134,   135,    -1,    -1,   138,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,
      42,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,    -1,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,   134,   135,    -1,    -1,   138,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,    44,
      45,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,   120,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,
     135,    -1,    -1,   138,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    39,    -1,    -1,    42,    -1,    44,    45,    46,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,   120,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,   134,   135,    -1,    -1,
     138,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,
      -1,    42,    -1,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   130,
      -1,   132,   133,   134,   135,    -1,    -1,   138,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    46,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
     134,   135,    -1,    -1,   138,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    -1,    -1,    42,    -1,    44,    45,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,   120,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,    -1,
      -1,   138,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    -1,    42,    -1,    44,    45,    46,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
     120,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,   134,   135,    -1,    -1,   138,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    42,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,   134,   135,    -1,    -1,   138,    12,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    39,    -1,    -1,    42,    -1,    44,    45,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,   120,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,   134,   135,
      -1,    -1,   138,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    44,    45,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,   120,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,   130,    -1,   132,    -1,   134,   135,    -1,    -1,   138,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,
      42,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    19,   120,    -1,
      -1,   123,   124,    25,    -1,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,   135,    -1,    -1,   138,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,   100,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,
     129,   130,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    76,    77,    78,    79,    80,   129,   130,
      83,    84,    85,    86,   135,    88,    89,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,    -1,
     133,    76,    77,    78,    79,    80,    -1,    -1,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,   133,    76,
      77,    78,    79,    80,    -1,    -1,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,    -1,   133,    76,    77,    78,
      79,    80,    -1,    -1,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,    -1,   133,    76,    77,    78,    79,    80,
      -1,    -1,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,    -1,   133,    76,    77,    78,    79,    80,    -1,    -1,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,    -1,
     133,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79,
      80,    -1,    -1,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,   123,   124,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,    76,    77,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,    -1,    76,    77,    78,    79,
      80,   129,   130,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,   129,
     130,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    76,    77,    78,    79,    80,   129,   130,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      76,    77,    78,    79,    80,   129,   130,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    92,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   113,   114,   115,   116,   117,
     118,   119,    -1,    76,    77,    78,    79,    80,    -1,    -1,
      83,   129,   130,    86,    -1,    88,    89,    90,    -1,    92,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    90,    -1,    92,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    78,    79,    -1,
      -1,    -1,    -1,   129,   130,    86,    -1,    88,    89,    90,
      -1,    92,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   140,     0,     5,    24,    26,    28,    30,    35,    41,
     130,   145,   146,   149,   162,   166,   179,   199,   201,   202,
     205,   100,   157,   158,    94,   150,   151,   100,   147,   148,
     100,   100,    28,   100,   134,   159,   160,   161,   171,     4,
       6,   135,   107,   106,   109,   106,   106,   135,   107,   134,
     100,   132,   106,   131,   100,   204,    29,    78,    79,   100,
     165,   193,    12,    13,    94,    98,   100,   101,   142,   157,
      94,   151,   148,   200,    19,    25,    40,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,   171,   207,   208,   209,   210,   100,   158,   160,   109,
     203,   135,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     123,   124,    29,    29,   132,   163,   130,   136,   190,   192,
     102,   141,   100,   136,   113,   113,   113,   132,   113,   113,
     113,    27,    32,   108,   112,   130,   133,   171,   193,    41,
     100,   133,   194,   195,   197,   206,   109,   164,   158,    38,
     191,   137,   102,   103,   107,   137,   210,   210,   210,   100,
     163,   210,   163,   210,   163,   210,    27,   112,    12,    13,
      14,    15,    21,    33,    37,    39,    42,    67,    78,    79,
      94,    95,    96,    97,    98,    99,   101,   115,   116,   120,
     123,   124,   130,   131,   132,   135,   138,   144,   167,   171,
     173,   181,   182,   184,   185,   187,   188,   189,   207,   211,
     214,   216,   136,   197,   133,   137,    87,   106,   107,   109,
     196,   210,   167,   131,   197,    94,   114,   106,   114,   133,
     164,   114,   164,   114,   164,   114,   112,   210,   211,   132,
     132,   113,   113,   132,   132,   189,   189,   143,   189,   189,
     171,   189,   189,   130,   189,   169,   162,   100,   132,    76,
      77,    78,    79,    80,    83,    84,    85,    86,    88,    89,
      90,    92,   108,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   129,   130,   131,   132,   194,   100,   210,
     170,   186,   189,   137,   210,   114,   114,   114,   132,   100,
     189,   210,   210,   189,   133,   189,   103,   104,   141,   210,
     133,     1,     7,     8,    10,    20,    22,    23,    32,    36,
      43,   136,   137,   153,   154,   155,   156,   168,   170,   172,
     174,   175,   176,   177,   179,   180,   186,   189,   163,   130,
     133,   183,   189,   189,   189,   189,   189,   189,   189,   100,
     189,   100,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   100,   189,   133,   183,   196,
      91,    68,    69,    70,    71,    72,    73,    74,    75,    81,
      82,    87,    93,   107,   114,   133,   183,    16,   113,   189,
     133,   114,   114,   133,   133,   189,   100,   130,   132,   189,
     212,   213,   215,   189,   189,   206,    87,   189,   167,   189,
     189,    31,   137,   137,   137,   137,    17,   178,   137,   164,
     100,   212,   106,   133,   132,   109,   131,   133,   189,   184,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   133,   210,   100,   133,   189,   189,   105,
     196,   130,   133,   106,   131,   137,   131,   137,   167,   167,
      17,   189,    11,   167,   135,    34,   198,   206,   167,   131,
     189,   133,   183,   189,   133,   114,   189,   213,   100,   131,
     212,   131,   189,     9,    18,   152,   183,   167,   169,   109,
     196,   133,   133,    16,   189,   131,   196,   167,   189,   167,
     136,   210,   170,   189,   210,   133,   131,   189,   167,   137,
     196,   137,   133,   152,   170,   189,   137
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   139,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   141,   141,   142,   143,   143,   143,   144,   145,   146,
     147,   147,   148,   149,   150,   150,   151,   151,   152,   152,
     152,   153,   154,   155,   156,   157,   157,   157,   157,   157,
     157,   158,   158,   159,   159,   160,   160,   161,   161,   162,
     162,   163,   163,   163,   164,   164,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   166,
     167,   167,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   169,   169,   170,   171,
     171,   171,   172,   173,   173,   173,   173,   174,   175,   176,
     176,   176,   177,   178,   178,   179,   179,   180,   181,   181,
     182,   182,   182,   182,   183,   183,   184,   184,   185,   185,
     185,   185,   185,   185,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   188,
     188,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   191,   191,
     192,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     197,   197,   197,   197,   198,   198,   198,   198,   198,   199,
     200,   200,   200,   201,   202,   203,   203,   204,   205,   206,
     206,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   208,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   211,   211,   212,
     212,   213,   213,   214,   214,   215,   215,   216
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     3,     0,     2,     4,     3,     2,     2,
       1,     3,     1,     2,     1,     3,     1,     3,     0,     2,
       4,     4,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     7,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     0,     2,     2,     3,     1,
       3,     2,     2,     2,     4,     5,     2,     1,     1,     1,
       2,     3,     4,     0,     2,     1,     1,     3,     5,     5,
       5,     8,     6,     9,     1,     3,     1,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     6,     5,
       6,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     3,     3,     2,     3,     4,     3,
       4,     4,     4,     3,     4,     3,     5,     1,     1,     1,
       1,     1,     1,     3,     3,     2,     0,     3,     0,     1,
       3,     0,     3,     1,     2,     1,     3,     1,     1,     1,
       3,     5,     3,     3,     4,     6,     5,     4,     3,     4,
       0,     3,     5,     4,     5,     0,     2,     2,     6,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     4,     3,     2,
       3,     2,     3,     4,     2,     4,     6,     4,     1,     4,
       5,     1,     4,     5,     1,     4,     5,     1,     1,     3,
       5,     1,     3,     6,     8,     1,     3,     6
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
          case 100: /* NAME  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2331 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 141: /* character_sequence  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2337 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 142: /* string_constant  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2343 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 143: /* string_builder_body  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2349 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 144: /* string_builder  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2355 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* expression_else  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2361 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* expression_if_then_else  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2367 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* expression_for_loop  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2373 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* expression_while_loop  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2379 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* expression_with  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2385 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* annotation_argument  */
#line 98 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2391 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* annotation_argument_list  */
#line 99 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2397 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* annotation_declaration_name  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2403 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* annotation_declaration  */
#line 100 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2409 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 161: /* annotation_list  */
#line 101 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2415 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* optional_annotation_list  */
#line 101 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2421 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* optional_function_argument_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2427 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* optional_function_type  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2433 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* function_name  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2439 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expression_block  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2445 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expression_any  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2451 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expressions  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2457 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expr_pipe  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2463 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* name_in_namespace  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2469 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 172: /* expression_delete  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2475 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expr_new  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2481 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expression_break  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2487 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expression_continue  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2493 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expression_return  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2499 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expression_try_catch  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2505 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expression_let  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2511 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expr_cast  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2517 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* expr_type_info  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2523 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* expr_list  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2529 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* expr_block  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2535 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expr_numeric_const  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2541 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expr_assign  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2547 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expr_named_call  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2553 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expr_method_call  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2559 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expr  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2565 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* optional_field_annotation  */
#line 99 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2571 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* structure_variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2577 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* struct_variable_declaration_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2583 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* function_argument_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2589 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* function_argument_list  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2595 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2601 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* let_variable_declaration  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2607 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* enum_list  */
#line 103 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2613 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* optional_structure_parent  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2619 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* variable_name_list  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2625 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* structure_type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2631 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* auto_type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2637 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* type_declaration  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2643 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* make_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2649 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* make_struct_fields  */
#line 102 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2655 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* make_struct_dim  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2661 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* make_struct_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2667 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* make_dim  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2673 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* make_dim_decl  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2679 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 251 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 2967 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 12:
#line 252 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2973 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 13:
#line 256 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2979 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 260 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2987 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 263 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3001 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 272 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3011 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 280 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3029 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 296 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		if ( g_Program->options.arguments.size() ) {
			g_Program->options.arguments.insert ( g_Program->options.arguments.begin(),
                (yyvsp[0].aaList)->arguments.begin(), (yyvsp[0].aaList)->arguments.end() );
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
#line 3052 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 326 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 3064 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 345 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3072 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 348 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3080 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 354 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3086 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 355 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3092 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 356 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3101 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 363 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3110 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 370 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3124 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 382 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3136 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 392 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3147 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 401 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3153 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 402 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3159 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 403 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3165 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 404 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3171 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 405 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3177 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 406 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3183 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 410 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3193 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 415 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3203 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 423 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3209 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 424 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 3215 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 428 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3227 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 435 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3241 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 447 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3250 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 451 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3259 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 458 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 3265 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 459 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3271 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 463 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3277 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3283 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 465 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3289 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 469 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3298 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 473 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3307 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3316 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 484 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 3322 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 485 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 3328 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 486 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 3334 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 487 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 3340 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 488 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 3346 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 489 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 3352 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 490 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 3358 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 491 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 3364 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 492 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 3370 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 493 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 3376 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 494 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 3382 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 495 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 3388 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 496 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 3394 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 497 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 3400 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 498 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 3406 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 499 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 3412 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 500 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 3418 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 501 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 3424 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 502 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 3430 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 503 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 3436 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 504 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 3442 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 505 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 3448 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 506 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 3454 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 507 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 3460 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 508 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 3466 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 509 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 3472 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 510 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 3478 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 511 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 3484 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 512 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 3490 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 513 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 3496 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 514 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 3502 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 515 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 3508 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 520 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3578 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 588 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3586 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 591 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3598 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 601 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3604 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 602 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3610 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 603 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3616 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 604 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3622 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 605 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3628 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 606 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3634 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 607 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3640 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 608 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3646 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 609 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3652 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 610 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3658 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 611 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3664 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 612 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3670 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 613 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3676 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 617 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = tokAt((yyloc));
    }
#line 3685 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 621 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3696 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 627 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3704 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 633 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3720 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 647 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3726 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 648 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3737 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 654 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3743 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 658 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3751 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 664 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3759 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 667 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3767 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 670 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3776 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 674 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3784 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 680 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3790 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 684 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 3796 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 688 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3804 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 691 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3812 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 694 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3822 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 702 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3830 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 708 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3836 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 709 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3842 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 713 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3848 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 714 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3854 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 718 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 3888 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 750 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 3896 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 753 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)), true);
    }
#line 3904 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 759 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 3913 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 763 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 3923 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 768 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 3932 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 772 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 3942 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 780 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3950 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3958 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 789 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3968 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 795 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4020 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 845 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 4026 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 846 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 4032 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 847 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 4038 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 848 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 4044 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 849 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 4050 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 850 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 4056 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 854 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4062 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 855 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4068 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 856 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4074 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 857 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4080 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 858 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4086 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 859 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4092 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 860 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4098 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 861 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4104 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 862 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4110 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 863 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4116 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 864 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4122 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 865 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4128 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 866 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4134 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 867 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4140 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 871 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 4152 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 882 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4162 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 887 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4174 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4180 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4186 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4192 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4198 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 901 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4204 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 902 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4210 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 903 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4216 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4222 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 905 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4228 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 906 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4234 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 907 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4240 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4246 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 909 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4252 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 910 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4258 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4264 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 912 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4270 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4276 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 914 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4282 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 915 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4288 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 916 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4294 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4300 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 918 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4306 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 919 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4312 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 920 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4318 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 921 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4324 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 922 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4330 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 923 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4336 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4342 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 925 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4348 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 926 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4354 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 927 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4360 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 928 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4366 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4372 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 930 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4378 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 931 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4384 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 932 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4390 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 933 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4399 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 937 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4408 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 941 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4416 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 944 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4424 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 947 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4430 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 948 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4436 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 949 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4442 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4448 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 951 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4454 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 952 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4462 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 955 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4468 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 956 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4474 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 957 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4480 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 958 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4486 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 959 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4492 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 960 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4498 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 961 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4513 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 971 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4528 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 981 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4564 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1015 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 4570 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1016 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4576 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1020 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4582 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1021 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4588 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1025 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4598 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1033 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4606 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1036 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4615 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1043 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 4621 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1044 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 4627 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1048 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4633 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1049 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4639 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1053 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4645 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1054 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4651 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1058 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4660 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1062 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4668 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1065 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4677 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1069 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4687 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1074 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4697 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1082 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4705 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1085 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4714 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1089 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4723 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1093 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4733 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1098 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4743 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1106 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4773 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1134 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4781 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1137 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4795 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1146 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4809 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1158 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 4823 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1170 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4839 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1184 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 4845 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1185 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4851 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1189 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
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
#line 4891 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1227 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
#line 4959 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1293 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4971 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1300 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4982 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1309 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 4988 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1310 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 4994 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1311 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 5000 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1312 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 5006 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1313 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 5012 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1314 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 5018 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1315 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 5024 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1316 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 5030 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1317 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 5036 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1318 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 5042 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1319 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 5048 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1320 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 5054 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1321 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 5060 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1322 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 5066 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1323 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 5072 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1324 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 5078 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1325 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 5084 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1326 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 5090 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1327 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 5096 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1328 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 5102 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1332 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 5114 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1342 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5123 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1346 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 5135 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1356 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 5141 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1357 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5147 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1358 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 5153 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1359 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
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
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5171 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1372 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5181 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1377 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5191 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1382 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5201 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1387 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5211 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1392 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5221 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1397 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5233 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1404 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5242 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1408 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5252 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1413 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5263 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1419 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5273 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1424 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5282 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1428 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5292 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1433 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5306 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1442 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5315 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1446 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5325 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1451 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5339 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1460 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5348 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1464 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5358 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1469 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5372 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1481 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5378 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1482 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5384 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1486 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5396 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1493 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5407 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1502 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5417 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1507 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5426 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1514 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5436 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1519 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5447 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1528 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5457 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1533 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5466 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1540 "src/parser/ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5476 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 5480 "/Users/borisbatkin/Work/yzg/generated/ds_parser.cpp" /* yacc.c:1660  */
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
#line 1547 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


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
            for ( size_t ai=0; ai!= pDecl->pNameList->size(); ++ai ) {
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


