/* A Bison parser, made by GNU Bison 3.2.4.  */

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
#define YYBISON_VERSION "3.2.4"

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
#line 42 "src/parser/ds_parser.ypp" /* yacc.c:338  */

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

#line 100 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:338  */
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
#ifndef YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
# define YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED
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

#line 181 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */

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
    DAS_TBOOL = 295,
    DAS_TVOID = 296,
    DAS_TSTRING = 297,
    DAS_TAUTO = 298,
    DAS_TINT = 299,
    DAS_TINT2 = 300,
    DAS_TINT3 = 301,
    DAS_TINT4 = 302,
    DAS_TUINT = 303,
    DAS_TUINT2 = 304,
    DAS_TUINT3 = 305,
    DAS_TUINT4 = 306,
    DAS_TFLOAT = 307,
    DAS_TFLOAT2 = 308,
    DAS_TFLOAT3 = 309,
    DAS_TFLOAT4 = 310,
    DAS_TRANGE = 311,
    DAS_TURANGE = 312,
    DAS_TBLOCK = 313,
    DAS_TINT64 = 314,
    DAS_TUINT64 = 315,
    DAS_TDOUBLE = 316,
    DAS_TFUNCTION = 317,
    DAS_TLAMBDA = 318,
    ADDEQU = 319,
    SUBEQU = 320,
    DIVEQU = 321,
    MULEQU = 322,
    MODEQU = 323,
    ANDEQU = 324,
    OREQU = 325,
    XOREQU = 326,
    SHL = 327,
    SHR = 328,
    ADDADD = 329,
    SUBSUB = 330,
    LEEQU = 331,
    SHLEQU = 332,
    SHREQU = 333,
    GREQU = 334,
    EQUEQU = 335,
    NOTEQU = 336,
    RARROW = 337,
    LARROW = 338,
    QQ = 339,
    QDOT = 340,
    LPIPE = 341,
    LBPIPE = 342,
    RPIPE = 343,
    INTEGER = 344,
    LONG_INTEGER = 345,
    UNSIGNED_INTEGER = 346,
    UNSIGNED_LONG_INTEGER = 347,
    FLOAT = 348,
    DOUBLE = 349,
    NAME = 350,
    BEGIN_STRING = 351,
    STRING_CHARACTER = 352,
    END_STRING = 353,
    BEGIN_STRING_EXPR = 354,
    END_STRING_EXPR = 355,
    UNARY_MINUS = 356,
    UNARY_PLUS = 357,
    PRE_INC = 358,
    PRE_DEC = 359,
    POST_INC = 360,
    POST_DEC = 361,
    COLCOL = 362
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 66 "src/parser/ds_parser.ypp" /* yacc.c:353  */

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

#line 326 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:353  */
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

#endif /* !YY_DAS_YY_D_COMMON_INFO_GAIJINGITHUB_DASCRIPT_GENERATED_DS_PARSER_HPP_INCLUDED  */



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
#define YYLAST   3344

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  298
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  529

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   362

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   119,     2,     2,   133,   114,   107,     2,
     127,   128,   112,   111,   101,   110,   124,   113,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   104,   132,
     108,   102,   109,   103,   115,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   125,     2,   126,   106,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   130,   105,   131,   118,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   116,   117,   120,   121,
     122,   123,   129
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   232,   232,   234,   235,   236,   237,   238,   239,   240,
     241,   245,   246,   250,   254,   257,   263,   271,   287,   308,
     312,   313,   317,   327,   331,   332,   336,   339,   345,   346,
     347,   354,   361,   373,   383,   392,   393,   394,   395,   396,
     397,   401,   406,   414,   415,   419,   426,   438,   442,   449,
     450,   454,   455,   456,   460,   464,   471,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   510,
     579,   582,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   607,   611,   617,   623,   637,   638,
     644,   648,   654,   657,   660,   664,   670,   674,   677,   680,
     688,   694,   695,   699,   725,   753,   756,   762,   766,   771,
     775,   783,   786,   792,   797,   848,   849,   850,   851,   852,
     853,   857,   858,   859,   860,   861,   862,   863,   864,   865,
     866,   867,   868,   873,   878,   888,   889,   890,   891,   892,
     893,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   912,
     913,   914,   915,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,   929,   933,   936,   939,   940,   941,   942,
     943,   946,   947,   948,   949,   950,   951,   961,   971,  1005,
    1006,  1010,  1011,  1015,  1023,  1026,  1033,  1034,  1038,  1039,
    1043,  1044,  1048,  1052,  1055,  1059,  1064,  1072,  1075,  1079,
    1083,  1088,  1096,  1119,  1122,  1131,  1143,  1155,  1169,  1170,
    1174,  1212,  1278,  1285,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,
    1310,  1311,  1312,  1313,  1317,  1327,  1331,  1341,  1342,  1343,
    1344,  1348,  1353,  1358,  1363,  1368,  1375,  1379,  1384,  1390,
    1394,  1399,  1408,  1412,  1417,  1426,  1430,  1435,  1447,  1448,
    1452,  1459,  1468,  1473,  1480,  1485,  1494,  1499,  1506
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
  "DAS_OVERRIDE", "DAS_UPCAST", "DAS_TBOOL", "DAS_TVOID", "DAS_TSTRING",
  "DAS_TAUTO", "DAS_TINT", "DAS_TINT2", "DAS_TINT3", "DAS_TINT4",
  "DAS_TUINT", "DAS_TUINT2", "DAS_TUINT3", "DAS_TUINT4", "DAS_TFLOAT",
  "DAS_TFLOAT2", "DAS_TFLOAT3", "DAS_TFLOAT4", "DAS_TRANGE", "DAS_TURANGE",
  "DAS_TBLOCK", "DAS_TINT64", "DAS_TUINT64", "DAS_TDOUBLE",
  "DAS_TFUNCTION", "DAS_TLAMBDA", "ADDEQU", "SUBEQU", "DIVEQU", "MULEQU",
  "MODEQU", "ANDEQU", "OREQU", "XOREQU", "SHL", "SHR", "ADDADD", "SUBSUB",
  "LEEQU", "SHLEQU", "SHREQU", "GREQU", "EQUEQU", "NOTEQU", "RARROW",
  "LARROW", "QQ", "QDOT", "LPIPE", "LBPIPE", "RPIPE", "INTEGER",
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
  "expression_delete", "expr_new", "expression_break", "expression_return",
  "expression_try_catch", "let_scope", "expression_let", "expr_cast",
  "expr_type_info", "expr_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_method_call", "expr", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "variable_declaration_list",
  "function_argument_list", "copy_or_move", "variable_declaration",
  "let_variable_declaration", "global_let", "enum_list",
  "alias_declaration", "enum_declaration", "optional_structure_parent",
  "structure_name", "structure_declaration", "variable_name_list",
  "basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "type_declaration", "make_decl",
  "make_struct_fields", "make_struct_dim", "make_struct_decl", "make_dim",
  "make_dim_decl", YY_NULLPTR
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
     355,    44,    61,    63,    58,   124,    94,    38,    60,    62,
      45,    43,    42,    47,    37,    64,   356,   357,   126,    33,
     358,   359,   360,   361,    46,    91,    93,    40,    41,   362,
     123,   125,    59,    36
};
# endif

#define YYPACT_NINF -312

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-312)))

#define YYTABLE_NINF -142

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -312,    13,  -312,   -74,    35,    45,    98,   130,   142,     4,
    -312,  -312,  -312,   111,  -312,  -312,  -312,  -312,  -312,  -312,
      93,  -312,    36,   102,   121,  -312,  -312,   139,  -312,   113,
     146,  -312,   120,   161,   133,  -312,   -68,  -312,   167,    44,
     -79,   269,    35,   179,    45,    98,  -312,  2186,   171,  -312,
      35,     4,  -312,   166,   141,  2758,   244,   247,  -312,   151,
      35,  -312,   245,   155,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,   -69,   180,   181,  -312,  -312,
    -312,   164,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,   196,  -312,  -312,  -312,
     197,   198,  -312,  -312,  -312,  -312,   227,  -312,    40,  -312,
     -31,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,   -24,   193,    82,  -312,   205,
    -312,    74,    24,  -312,  2186,  2186,   208,    34,    34,    34,
    -312,     7,  -312,  -312,   218,  -312,  -312,   -59,  -312,  -312,
      41,  -312,   101,  2186,   178,  -312,  -312,  -312,  -312,   220,
    -312,   127,   154,   182,   193,   160,   193,   177,   193,   189,
     204,  -312,   187,  -312,  -312,   205,  -312,   225,  -312,  2186,
    2035,   227,  -312,  -312,   183,  -312,  2186,  -312,   212,  -312,
     214,  -312,   219,  -312,  -312,  -312,  -312,  -312,   108,  -312,
    -312,  2130,   200,  -312,   210,   223,   233,   221,  2035,  2035,
    -312,  -312,  -312,  -312,  -312,  -312,    28,  -312,  2035,  2035,
     -31,  2035,  2035,   217,  2035,   222,  -312,  -312,  -312,   -66,
    -312,  -312,  -312,  -312,  -312,   259,  -312,  2356,   230,  -312,
    -312,  -312,   682,  -312,   192,  -312,  -312,  -312,  2035,    17,
    -312,   254,  2035,  2186,  2186,  1059,   281,   281,   115,   281,
     281,  -312,   281,   281,  2186,  2465,   151,  -312,  1181,    55,
    2035,  2035,  2035,  2035,  2035,  2035,  2035,  2035,  2035,  2035,
    -312,  -312,  2035,  2035,  2035,  2035,  2035,  2035,   255,  2035,
    2035,   258,  2035,  2035,  2035,  2035,  2035,  2035,  2035,  2035,
    2035,  2035,  2035,  2035,  2035,  2035,   264,  2035,  1303,  -312,
      -2,  2035,  2035,   205,  1425,  -312,   178,  2035,  2035,   323,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,   229,   236,   239,
    -312,  -312,   -67,  2418,  -312,  2955,  1547,   527,  2522,   215,
     226,  -312,  2579,  -312,  2035,   276,   937,  -312,   193,  -312,
      42,  2955,  -312,  2955,  2955,  2955,  2955,  2955,  2955,  2955,
    2955,  3219,  3219,  3175,  2955,  2955,  3175,  3158,  3158,   248,
    2955,   281,  -312,   -57,   -57,  2955,  2861,  3049,  3096,  3114,
    3175,  3175,   720,   720,   523,   523,   523,  -312,  2806,  -312,
      51,   340,   205,   205,   305,   305,     2,  2035,  2955,   365,
    2955,   305,   252,  -312,  -312,  -312,  -312,  -312,    61,  2186,
     297,  2636,  -312,  2035,  2035,  -312,  2908,   243,   -62,  1669,
    2955,   293,    97,   100,   178,  2035,  -312,  1791,  2035,  -312,
    -312,  -312,   118,  -312,  -312,   116,  -312,   156,  2035,  2955,
     178,  -312,  -312,  -312,     3,   287,  -312,  3175,  3175,  -312,
    2035,   302,   303,   273,   302,   274,  2035,  -312,  2955,  -312,
      66,  3002,   178,   205,  2186,  2035,   178,  2035,  -312,   -70,
    -312,   815,  -312,  1913,  2955,   -32,   109,   -32,  -312,   293,
    -312,  2955,  -312,  -312,  -312,    75,  -312,  2218,  -312,   305,
    -312,  -312,  2186,  2693,   275,  2035,  -312,  2035,  -312,   156,
      73,  -312,  -312,  2955,  -312,  2287,  -312,  -312,  -312
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    49,     1,     0,     0,     0,     0,     0,     0,     0,
       9,     8,     7,     0,     6,     5,    10,     4,     3,   214,
       0,    41,    18,    26,    23,    24,    22,    19,    20,     0,
       0,    44,   108,     0,    45,    47,     0,    43,     0,     0,
     209,     0,     0,     0,     0,     0,   233,     0,     0,   110,
       0,     0,    50,   238,     0,     0,     0,     0,    56,    51,
       0,   232,   211,     0,    39,    40,    37,    38,    36,    11,
      35,    42,    27,    25,    21,     0,     0,     0,   244,   260,
     245,   265,   246,   248,   249,   250,   251,   253,   254,   255,
     256,   257,   258,   259,   261,   262,   279,   247,   252,   263,
     282,   285,   264,   267,   269,   268,   236,   109,     0,    48,
       0,   240,   214,    59,    60,    62,    61,    63,    64,    65,
      66,    85,    86,    83,    84,    76,    87,    88,    77,    74,
      75,    79,    80,    78,    72,    73,    68,    67,    69,    70,
      71,    58,    57,    81,    82,     0,    54,     0,   212,     0,
     215,     0,     0,   237,     0,     0,     0,    51,    51,    51,
     271,     0,   276,   273,     0,    46,   239,   209,   242,    52,
       0,   218,   222,     0,     0,   210,   213,    12,    13,     0,
     234,     0,     0,     0,    54,     0,    54,     0,    54,     0,
     272,   274,     0,   241,    53,     0,   221,     0,   220,     0,
       0,    55,   104,    89,     0,   277,     0,   266,     0,   280,
       0,   283,     0,   286,   275,   270,   219,   243,   223,   160,
     161,     0,     0,   155,     0,     0,     0,     0,     0,     0,
     135,   137,   136,   138,   139,   140,   156,    14,     0,     0,
       0,     0,     0,     0,     0,    49,   158,   133,   226,     0,
     203,   202,   201,   205,   157,     0,   204,   225,     0,   159,
     288,   289,     0,   235,     0,   281,   284,   287,     0,   112,
     115,     0,     0,     0,     0,     0,   183,   184,    11,   165,
     164,   191,   163,   162,     0,     0,    51,   208,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
     121,     0,     0,     0,   117,   116,     0,     0,     0,    90,
      92,   102,    99,    97,    98,   105,    93,     0,     0,     0,
     103,    96,     0,   141,   278,   224,     0,     0,     0,     0,
       0,   197,     0,    17,     0,    15,     0,   187,    54,   192,
       0,   131,   107,   146,   147,   149,   148,   150,   143,   144,
     145,   167,   168,   178,   151,   152,   179,   176,   177,     0,
     166,   199,   190,   206,   207,   142,     0,   181,   182,   180,
     174,   175,   170,   169,   171,   172,   173,   189,     0,   194,
       0,     0,     0,     0,     0,     0,     0,     0,   118,     0,
     111,     0,     0,    95,   100,   101,    94,   113,     0,     0,
       0,     0,   196,     0,     0,   198,     0,   156,     0,     0,
     296,   292,     0,     0,     0,     0,   193,     0,     0,   188,
     195,   122,     0,   216,   124,     0,    33,    28,     0,   119,
       0,    34,   104,   114,     0,     0,   127,   125,   126,    16,
       0,     0,     0,     0,     0,     0,     0,   134,   132,   153,
       0,   200,     0,     0,     0,     0,     0,     0,    31,     0,
     120,     0,   129,     0,   290,     0,     0,     0,   294,   293,
     298,   297,   154,   123,   217,     0,   231,   141,    29,     0,
      32,    91,     0,     0,     0,     0,   227,     0,   230,    28,
       0,   128,   295,   291,   229,   141,    30,   130,   228
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -312,  -312,   124,  -312,  -312,  -312,  -312,  -312,  -312,   358,
    -312,  -312,   360,  -112,  -312,  -312,  -312,  -312,   367,   -10,
    -312,   370,  -312,   186,  -135,  -172,  -312,  -312,  -170,  -312,
     -40,  -192,    -9,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -311,   136,  -312,  -252,  -312,   195,  -312,  -312,
    -312,   314,  -312,  -312,  -161,  -140,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -297,   -46,  -312,  -312,  -152,   206,
     -42,   -43,  -312,  -312,  -312
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   151,    70,   278,   246,    10,    11,    27,    28,
      12,    24,    25,   488,   341,   342,   343,   344,    21,    22,
      34,    35,    36,    13,   146,   174,    59,    14,   247,   345,
     262,   346,   249,   347,   250,   348,   349,   350,   413,   351,
     251,   252,   370,   253,   254,   255,   256,   371,    62,   149,
      63,    40,   452,   170,   470,   171,   454,    15,    75,    16,
      17,   111,    54,    18,   172,   258,   104,   105,   106,   259,
     441,   442,   260,   443,   261
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,   103,   181,   182,   203,   185,   187,   189,   248,   176,
     352,   200,   208,     2,   210,   411,   212,   410,     3,   458,
     289,   201,   184,   186,   188,   308,   152,   192,   311,   287,
     160,   445,    31,    51,   190,   161,   416,     4,   102,     5,
     108,     6,    37,     7,   160,   428,    60,   218,     8,   161,
     147,   196,    61,    76,   264,   216,    19,   268,    52,    77,
     202,   288,   153,   284,    32,   426,    60,   326,   327,   269,
     198,   168,   193,    55,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    33,    32,
     160,   166,   160,   197,   169,   161,   162,   161,   103,   103,
     163,   103,   103,   103,   191,    38,   455,    39,    56,    57,
     162,   359,   360,  -108,   163,   412,   179,   103,   164,    32,
      20,   492,   366,    33,    23,   160,   480,    42,     9,    58,
     161,    42,   164,   445,   356,   102,   102,   489,   102,   102,
     102,   368,   445,   103,   160,  -108,   180,    48,   196,   161,
     103,   145,   445,    33,   102,   486,   419,   445,   165,   194,
     446,   177,   178,   195,   487,   103,   162,   198,   162,   450,
     163,   160,   163,    42,   196,   202,   161,   160,   245,   463,
     102,   196,   161,    26,   502,    41,   444,   102,   164,   196,
     164,   527,   197,   198,   160,   199,    43,   516,   175,   161,
     198,   162,   102,   363,   364,   163,   160,   197,   198,   160,
     484,   161,    44,   473,   161,    29,   475,   103,   103,   474,
     162,   281,   476,   164,   163,   514,   205,    30,   103,   352,
      45,   474,   160,    46,   456,   457,   482,   161,    47,    48,
     483,   461,   164,   160,   160,   206,    49,   162,   161,   161,
      50,   163,    53,   162,   102,   102,   107,   163,    72,   209,
     110,   112,   453,   143,   477,   102,   144,   464,   145,   164,
     162,    64,    65,   148,   163,   164,   211,   150,   154,   155,
     490,   156,   162,   506,   485,   162,   163,   173,   213,   163,
     168,   354,   164,   183,   157,   158,   159,   192,   202,   204,
     207,   214,   503,   215,   164,   263,   508,   164,   162,   510,
     217,   265,   163,   266,   433,   524,   196,   271,   267,   162,
     162,   273,   505,   163,   163,   434,   515,   272,  -108,   519,
     164,   274,   284,   504,   517,   198,   289,     9,   275,   357,
     389,   164,   164,   392,   422,   300,   301,   328,    66,   407,
     520,   423,    67,   308,    68,    69,   311,   312,   424,   313,
    -108,   425,    48,   177,   451,   447,   460,   298,   299,   300,
     301,   302,   462,   103,   305,   306,   307,   308,   309,   310,
     311,   312,   465,   313,   472,   257,   493,   495,   497,   498,
     500,   522,   365,    74,    73,   326,   327,   526,   315,    71,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     102,   109,   491,   276,   277,   372,   167,   270,   496,   326,
     327,   286,   499,   279,   280,   202,   282,   283,   103,   285,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,     0,     0,
       0,     0,     0,   355,     0,     0,   103,   358,     0,     0,
     362,     0,     0,     0,     0,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,     0,     0,   383,   384,   385,
     386,   387,   388,   102,   390,   391,     0,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,     0,   408,     0,     0,     0,   414,   415,     0,   418,
       0,     0,   420,   421,     0,     0,     0,     0,     0,   219,
     220,   221,   222,   429,     0,     0,     0,     0,   223,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   436,
     224,   440,     0,     0,   225,     0,   226,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,    99,     0,
     227,     0,     0,     0,     0,     0,     0,   300,   301,     0,
       0,   228,   229,     0,     0,   308,     0,   310,   311,   312,
       0,   313,   459,     0,     0,     0,   230,   231,   232,   233,
     234,   235,   236,   237,     0,     0,     0,     0,   467,   468,
       0,     0,     0,     0,   285,   430,     0,   238,   239,     0,
     478,     0,   240,   481,     0,   241,   242,   326,   327,     0,
       0,     0,   243,     0,   244,     0,    33,   202,     0,     0,
     245,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,   501,     0,     0,     0,     0,     0,     0,     0,     0,
     507,     0,   509,   329,     0,     0,   353,   330,   513,   331,
     332,     0,   333,     0,   219,   220,   221,   222,     0,     0,
       0,     0,   334,   223,   335,   336,     0,     0,     0,     0,
     523,     0,   525,     0,   337,   224,     0,     0,   338,   225,
       0,   226,    78,    79,    80,     0,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
       0,    97,    98,    99,     0,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,   229,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,   231,   232,   233,   234,   235,   236,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,   239,   300,   301,     0,   240,     0,     0,
     241,   242,   308,     0,   310,   311,   312,   243,   313,   244,
       0,    33,   202,   339,   340,   245,   329,     0,     0,     0,
     330,     0,   331,   332,     0,   333,     0,   219,   220,   221,
     222,     0,   323,   324,   325,   334,   223,   335,   336,     0,
       0,     0,     0,     0,   326,   327,     0,   337,   224,     0,
       0,   338,   225,     0,   226,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,    99,     0,   227,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
     240,     0,     0,   241,   242,     0,     0,     0,     0,     0,
     243,     0,   244,     0,    33,   202,   511,   340,   245,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,     0,   160,     0,     0,     0,     0,   161,
     224,     0,     0,     0,   225,     0,   226,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,    99,     0,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,   231,   232,   233,
     234,   235,   437,   237,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,   163,     0,     0,   238,   239,     0,
       0,     0,   240,     0,     0,   241,   242,     0,     0,     0,
       0,     0,   438,     0,   439,     0,    33,   202,     0,     0,
     245,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,   225,     0,   226,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
      99,     0,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
     239,     0,     0,     0,   240,     0,     0,   241,   242,     0,
       0,     0,     0,     0,   243,     0,   244,   361,    33,   202,
       0,     0,   245,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,   225,     0,
     226,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,    99,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,   231,   232,   233,   234,   235,   236,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,   239,     0,     0,     0,   240,     0,     0,   241,
     242,     0,     0,     0,     0,     0,   243,     0,   244,   369,
      33,   202,     0,     0,   245,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
     225,     0,   226,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,    99,     0,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,   232,   233,   234,   235,   236,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,   240,     0,
       0,   241,   242,     0,     0,     0,     0,     0,   243,     0,
     244,   409,    33,   202,     0,     0,   245,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,   225,     0,   226,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,    99,     0,   227,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,     0,     0,     0,     0,     0,     0,   417,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
     240,     0,     0,   241,   242,     0,     0,     0,     0,     0,
     243,     0,   244,     0,    33,   202,     0,     0,   245,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,   225,     0,   226,    78,    79,    80,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    97,    98,    99,     0,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,   231,   232,   233,
     234,   235,   236,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,   239,     0,
       0,     0,   240,     0,     0,   241,   242,     0,     0,     0,
       0,     0,   243,     0,   244,   427,    33,   202,     0,     0,
     245,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,   225,     0,   226,    78,
      79,    80,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    97,    98,
      99,     0,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
     232,   233,   234,   235,   236,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
     239,     0,     0,     0,   240,     0,     0,   241,   242,     0,
       0,     0,     0,     0,   243,     0,   244,   471,    33,   202,
       0,     0,   245,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,   225,     0,
     226,    78,    79,    80,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      97,    98,    99,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,   231,   232,   233,   234,   235,   236,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,   239,     0,     0,     0,   240,     0,     0,   241,
     242,     0,     0,     0,     0,     0,   243,     0,   244,   479,
      33,   202,     0,     0,   245,   219,   220,   221,   222,   512,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
     225,     0,   226,    78,    79,    80,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    97,    98,    99,     0,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   231,   232,   233,   234,   235,   236,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,   240,     0,
       0,   241,   242,     0,     0,     0,     0,     0,   243,     0,
     244,     0,    33,   202,     0,     0,   245,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,   225,     0,   226,    78,    79,    80,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    97,    98,    99,     0,   227,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   232,   233,   234,   235,
     236,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,    76,
     240,     0,     0,   241,   242,    77,     0,     0,     0,     0,
     243,     0,   244,     0,    33,   202,     0,     0,   245,     0,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
       0,     0,     0,     0,     0,   243,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
     314,   315,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,     0,     0,     0,     0,     0,     0,
     518,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,     0,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   314,
     315,     0,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,   327,     0,     0,     0,     0,     0,     0,   528,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,  -141,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   314,   315,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     314,   315,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   326,   327,   305,   306,   307,   308,   309,   310,
     311,   312,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
     327,     0,     0,   367,   298,   299,   300,   301,   302,     0,
       0,   305,   306,   307,   308,   309,   310,   311,   312,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   327,     0,     0,
     432,   298,   299,   300,   301,   302,     0,     0,   305,   306,
     307,   308,   309,   310,   311,   312,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327,     0,     0,   435,   298,   299,
     300,   301,   302,     0,     0,   305,   306,   307,   308,   309,
     310,   311,   312,     0,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,     0,     0,   466,   298,   299,   300,   301,   302,
       0,     0,   305,   306,   307,   308,   309,   310,   311,   312,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,   327,     0,
       0,   521,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,     0,     0,     0,   141,   142,   298,   299,
     300,   301,   302,     0,     0,   305,   306,   307,   308,   309,
     310,   311,   312,     0,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   449,   298,   299,   300,   301,   302,     0,     0,
     305,   306,   307,   308,   309,   310,   311,   312,     0,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,   448,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   326,   327,   305,   306,   307,
     308,   309,   310,   311,   312,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,   315,     0,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   326,   327,   305,   306,   307,   308,   309,   310,
     311,   312,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   326,
     327,   305,   306,   307,   308,     0,   310,   311,   312,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   326,   327,   305,   306,
     307,   308,     0,   310,   311,   312,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,   318,   319,   320,   321,
     322,   323,   324,   325,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   326,   327,   305,   306,   307,   308,     0,
     310,   311,   312,     0,   313,     0,   298,   299,   300,   301,
     302,     0,     0,   305,   306,   307,   308,     0,   310,   311,
     312,     0,   313,   318,   319,   320,   321,   322,   323,   324,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   327,   319,   320,   321,   322,   323,   324,   325,     0,
     298,   299,   300,   301,   302,     0,     0,   305,   326,   327,
     308,     0,   310,   311,   312,     0,   313,   298,   299,   300,
     301,     0,     0,     0,     0,     0,     0,   308,     0,   310,
     311,   312,     0,   313,     0,     0,   319,   320,   321,   322,
     323,   324,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,     0,   321,   322,   323,   324,   325,
       0,     0,     0,   300,   301,     0,     0,     0,     0,   326,
     327,   308,     0,   310,   311,   312,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   321,
     322,   323,   324,   325,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   326,   327
};

static const yytype_int16 yycheck[] =
{
       9,    47,   154,   155,   174,   157,   158,   159,   200,   149,
     262,   172,   184,     0,   186,    17,   188,   328,     5,    17,
      87,   173,   157,   158,   159,    82,    95,    89,    85,    95,
      27,   101,    28,   101,    27,    32,   333,    24,    47,    26,
      50,    28,    51,    30,    27,   356,   125,   199,    35,    32,
      60,    83,   131,    19,   206,   195,   130,   218,   126,    25,
     130,   127,   131,   125,    95,   132,   125,   124,   125,   221,
     102,    95,   131,    29,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,   129,    95,
      27,   110,    27,   101,   128,    32,   103,    32,   154,   155,
     107,   157,   158,   159,   107,     4,   413,     6,    74,    75,
     103,   273,   274,    95,   107,   127,   102,   173,   125,    95,
      95,   128,   284,   129,    89,    27,   447,   101,   125,    95,
      32,   101,   125,   101,   127,   154,   155,   458,   157,   158,
     159,   286,   101,   199,    27,   127,   132,   129,    83,    32,
     206,   127,   101,   129,   173,     9,   336,   101,   128,   128,
     128,    97,    98,   132,    18,   221,   103,   102,   103,   128,
     107,    27,   107,   101,    83,   130,    32,    27,   133,   128,
     199,    83,    32,    95,   128,   102,   368,   206,   125,    83,
     125,   128,   101,   102,    27,   104,   104,   132,   126,    32,
     102,   103,   221,    98,    99,   107,    27,   101,   102,    27,
     104,    32,   101,   126,    32,    95,   126,   273,   274,   132,
     103,   240,   132,   125,   107,   126,   109,    95,   284,   491,
     101,   132,    27,   130,   414,   415,   128,    32,   102,   129,
     132,   421,   125,    27,    27,   101,    95,   103,    32,    32,
     127,   107,    95,   103,   273,   274,    95,   107,    89,   109,
     104,   130,   412,    29,   444,   284,    29,   429,   127,   125,
     103,    12,    13,    38,   107,   125,   109,   132,   108,   108,
     460,   127,   103,   485,   455,   103,   107,   104,   109,   107,
      95,   109,   125,    95,   108,   108,   108,    89,   130,    89,
     128,   107,   482,   126,   125,   132,   486,   125,   103,   489,
      95,   109,   107,   109,   109,   517,    83,   127,   109,   103,
     103,   108,   484,   107,   107,   109,   497,   127,    95,   509,
     125,   108,   125,   483,   505,   102,    87,   125,   127,    95,
      95,   125,   125,    95,    31,    74,    75,   127,    89,    95,
     512,   132,    93,    82,    95,    96,    85,    86,   132,    88,
     127,   132,   129,    97,    34,   127,    11,    72,    73,    74,
      75,    76,   130,   429,    79,    80,    81,    82,    83,    84,
      85,    86,    95,    88,   101,   200,   109,    95,    95,   126,
     126,   126,   278,    45,    44,   124,   125,   519,   103,    42,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     429,    51,   462,   228,   229,   289,   112,   221,   471,   124,
     125,   245,   474,   238,   239,   130,   241,   242,   484,   244,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   262,    -1,    -1,
      -1,    -1,    -1,   268,    -1,    -1,   512,   272,    -1,    -1,
     275,    -1,    -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    -1,    -1,   302,   303,   304,
     305,   306,   307,   512,   309,   310,    -1,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,    -1,   327,    -1,    -1,    -1,   331,   332,    -1,   334,
      -1,    -1,   337,   338,    -1,    -1,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,   357,    -1,    -1,    -1,    -1,    -1,    -1,   364,
      33,   366,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,
      -1,    74,    75,    -1,    -1,    82,    -1,    84,    85,    86,
      -1,    88,   417,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   433,   434,
      -1,    -1,    -1,    -1,   439,   108,    -1,   110,   111,    -1,
     445,    -1,   115,   448,    -1,   118,   119,   124,   125,    -1,
      -1,    -1,   125,    -1,   127,    -1,   129,   130,    -1,    -1,
     133,    -1,    -1,    -1,    -1,   470,    -1,    -1,    -1,    -1,
      -1,   476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,   487,     1,    -1,    -1,   491,     5,   493,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
     515,    -1,   517,    -1,    32,    33,    -1,    -1,    36,    37,
      -1,    39,    40,    41,    42,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    74,    75,    -1,   115,    -1,    -1,
     118,   119,    82,    -1,    84,    85,    86,   125,    88,   127,
      -1,   129,   130,   131,   132,   133,     1,    -1,    -1,    -1,
       5,    -1,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    -1,   112,   113,   114,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    -1,   124,   125,    -1,    32,    33,    -1,
      -1,    36,    37,    -1,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
     115,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,    -1,   129,   130,   131,   132,   133,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
      -1,    -1,   115,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,    -1,   129,   130,    -1,    -1,
     133,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,   130,
      -1,    -1,   133,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,   115,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,   130,    -1,    -1,   133,    12,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,   115,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,   130,    -1,    -1,   133,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
     115,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
     125,    -1,   127,    -1,   129,   130,    -1,    -1,   133,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,   115,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,   125,    -1,   127,   128,   129,   130,    -1,    -1,
     133,    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    -1,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,   130,
      -1,    -1,   133,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,   115,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,   130,    -1,    -1,   133,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    39,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,   115,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,   130,    -1,    -1,   133,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    -1,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    19,
     115,    -1,    -1,   118,   119,    25,    -1,    -1,    -1,    -1,
     125,    -1,   127,    -1,   129,   130,    -1,    -1,   133,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,   124,   125,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,    -1,    -1,   128,    72,    73,    74,    75,    76,    -1,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,    -1,    -1,
     128,    72,    73,    74,    75,    76,    -1,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,    -1,    -1,   128,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,    -1,    -1,   128,    72,    73,    74,    75,    76,
      -1,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,    -1,
      -1,   128,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,   119,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,    72,    73,    74,    75,    76,    -1,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,   124,   125,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,   124,   125,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,   124,
     125,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,   124,   125,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,   124,   125,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    88,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   108,   109,   110,   111,   112,   113,   114,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    79,   124,   125,
      82,    -1,    84,    85,    86,    -1,    88,    72,    73,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    84,
      85,    86,    -1,    88,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,   124,
     125,    82,    -1,    84,    85,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   135,     0,     5,    24,    26,    28,    30,    35,   125,
     140,   141,   144,   157,   161,   191,   193,   194,   197,   130,
      95,   152,   153,    89,   145,   146,    95,   142,   143,    95,
      95,    28,    95,   129,   154,   155,   156,   166,     4,     6,
     185,   102,   101,   104,   101,   101,   130,   102,   129,    95,
     127,   101,   126,    95,   196,    29,    74,    75,    95,   160,
     125,   131,   182,   184,    12,    13,    89,    93,    95,    96,
     137,   152,    89,   146,   143,   192,    19,    25,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,   166,   199,   200,   201,   202,    95,   153,   155,
     104,   195,   130,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   118,   119,    29,    29,   127,   158,   153,    38,   183,
     132,   136,    95,   131,   108,   108,   127,   108,   108,   108,
      27,    32,   103,   107,   125,   128,   166,   185,    95,   128,
     187,   189,   198,   104,   159,   126,   189,    97,    98,   102,
     132,   202,   202,    95,   158,   202,   158,   202,   158,   202,
      27,   107,    89,   131,   128,   132,    83,   101,   102,   104,
     188,   202,   130,   162,    89,   109,   101,   128,   159,   109,
     159,   109,   159,   109,   107,   126,   189,    95,   202,    12,
      13,    14,    15,    21,    33,    37,    39,    63,    74,    75,
      89,    90,    91,    92,    93,    94,    95,    96,   110,   111,
     115,   118,   119,   125,   127,   133,   139,   162,   165,   166,
     168,   174,   175,   177,   178,   179,   180,   181,   199,   203,
     206,   208,   164,   132,   202,   109,   109,   109,   188,   202,
     203,   127,   127,   108,   108,   127,   181,   181,   138,   181,
     181,   166,   181,   181,   125,   181,   157,    95,   127,    87,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    88,   102,   103,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   124,   125,   127,     1,
       5,     7,     8,    10,    20,    22,    23,    32,    36,   131,
     132,   148,   149,   150,   151,   163,   165,   167,   169,   170,
     171,   173,   179,   181,   109,   181,   127,    95,   181,   202,
     202,   128,   181,    98,    99,   136,   202,   128,   158,   128,
     176,   181,   177,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,    95,
     181,   181,    95,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,    95,   181,   128,
     176,    17,   127,   172,   181,   181,   198,    83,   181,   162,
     181,   181,    31,   132,   132,   132,   132,   128,   176,    16,
     108,   181,   128,   109,   109,   128,   181,    95,   125,   127,
     181,   204,   205,   207,   159,   101,   128,   127,   104,   126,
     128,    34,   186,   189,   190,   198,   162,   162,    17,   181,
      11,   162,   130,   128,   202,    95,   128,   181,   181,   100,
     188,   128,   101,   126,   132,   126,   132,   162,   181,   128,
     176,   181,   128,   132,   104,   188,     9,    18,   147,   176,
     162,   164,   128,   109,   181,    95,   205,    95,   126,   204,
     126,   181,   128,   162,   189,   202,   165,   181,   162,   181,
     162,   131,    16,   181,   126,   188,   132,   188,   132,   162,
     202,   128,   126,   181,   165,   181,   147,   128,   132
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   134,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   136,   136,   137,   138,   138,   138,   139,   140,   141,
     142,   142,   143,   144,   145,   145,   146,   146,   147,   147,
     147,   148,   149,   150,   151,   152,   152,   152,   152,   152,
     152,   153,   153,   154,   154,   155,   155,   156,   156,   157,
     157,   158,   158,   158,   159,   159,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   161,
     162,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   164,   164,   164,   165,   166,   166,
     166,   167,   168,   168,   168,   168,   169,   170,   170,   170,
     171,   172,   172,   173,   173,   174,   174,   175,   175,   175,
     175,   176,   176,   177,   177,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   180,   180,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   182,
     182,   183,   183,   184,   185,   185,   186,   186,   187,   187,
     188,   188,   189,   189,   189,   189,   189,   190,   190,   190,
     190,   190,   191,   192,   192,   192,   193,   194,   195,   195,
     196,   197,   198,   198,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   200,   201,   201,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   203,   203,
     204,   204,   205,   205,   206,   206,   207,   207,   208
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     0,     2,     3,     0,     2,     4,     3,     2,     2,
       1,     3,     1,     2,     1,     3,     1,     3,     0,     2,
       4,     4,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     4,     1,     3,     0,
       3,     0,     2,     3,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       3,     7,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     0,     2,     2,     3,     1,     3,
       2,     2,     2,     4,     5,     2,     1,     1,     2,     3,
       4,     0,     2,     5,     3,     5,     5,     5,     8,     6,
       9,     1,     3,     1,     5,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     6,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     3,
       3,     2,     3,     4,     3,     4,     4,     3,     4,     3,
       5,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       3,     0,     1,     3,     0,     3,     1,     3,     1,     3,
       1,     1,     1,     3,     5,     3,     3,     4,     6,     5,
       4,     3,     4,     0,     3,     5,     4,     5,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     1,     1,
       4,     2,     3,     2,     3,     4,     2,     4,     6,     1,
       4,     5,     1,     4,     5,     1,     4,     5,     1,     1,
       3,     5,     1,     3,     6,     8,     1,     3,     6
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
          case 95: /* NAME  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2224 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 136: /* character_sequence  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2230 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 137: /* string_constant  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2236 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 138: /* string_builder_body  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2242 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 139: /* string_builder  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2248 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 147: /* expression_else  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2254 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 148: /* expression_if_then_else  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2260 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 149: /* expression_for_loop  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2266 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 150: /* expression_while_loop  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2272 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 151: /* expression_with  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2278 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 152: /* annotation_argument  */
#line 97 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 2284 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 153: /* annotation_argument_list  */
#line 98 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2290 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 154: /* annotation_declaration_name  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2296 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 155: /* annotation_declaration  */
#line 99 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 2302 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 156: /* annotation_list  */
#line 100 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2308 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 157: /* optional_annotation_list  */
#line 100 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 2314 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 158: /* optional_function_argument_list  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2320 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 159: /* optional_function_type  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2326 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 160: /* function_name  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2332 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 162: /* expression_block  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2338 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 163: /* expression_any  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2344 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 164: /* expressions  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2350 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 165: /* expr_pipe  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2356 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 166: /* name_in_namespace  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2362 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 167: /* expression_delete  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2368 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 168: /* expr_new  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2374 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 169: /* expression_break  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2380 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 170: /* expression_return  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2386 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 171: /* expression_try_catch  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2392 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* expression_let  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2398 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* expr_cast  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2404 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* expr_type_info  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2410 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* expr_list  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2416 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* expr_block  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2422 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 178: /* expr_numeric_const  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2428 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* expr_assign  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2434 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* expr_method_call  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2440 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* expr  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2446 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* optional_field_annotation  */
#line 98 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 2452 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* structure_variable_declaration  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2458 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* struct_variable_declaration_list  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2464 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* variable_declaration_list  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2470 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* function_argument_list  */
#line 94 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 2476 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* variable_declaration  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2482 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* let_variable_declaration  */
#line 93 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 2488 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* enum_list  */
#line 102 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 2494 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* optional_structure_parent  */
#line 91 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2500 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* variable_name_list  */
#line 92 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 2506 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* structure_type_declaration  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2512 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* auto_type_declaration  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2518 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* type_declaration  */
#line 95 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 2524 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* make_decl  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2530 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* make_struct_fields  */
#line 101 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 2536 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* make_struct_dim  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2542 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* make_struct_decl  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* make_dim  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2554 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* make_dim_decl  */
#line 96 "src/parser/ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 2560 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1254  */
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
#line 245 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(); }
#line 2848 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 12:
#line 246 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 2854 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 13:
#line 250 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2860 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 14:
#line 254 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
    }
#line 2868 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 15:
#line 257 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 2879 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 16:
#line 263 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 2889 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 17:
#line 271 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2907 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 18:
#line 287 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 2930 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 22:
#line 317 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        if ( !g_Program->addModule(*(yyvsp[0].s)) ) {
            das_yyerror("required module not found " + *(yyvsp[0].s),tokAt((yylsp[0])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[0].s);
    }
#line 2942 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 26:
#line 336 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 2950 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 27:
#line 339 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 2958 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 28:
#line 345 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 2964 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 29:
#line 346 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 2970 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 30:
#line 347 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2979 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 31:
#line 354 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 2988 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 32:
#line 361 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-4])));
        pFor->iterators = *(yyvsp[-3].pNameList);
        delete (yyvsp[-3].pNameList);
        pFor->sources = sequenceToList((yyvsp[-1].pExpression));
        pFor->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 3002 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 33:
#line 373 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 3014 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 34:
#line 383 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 3025 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 35:
#line 392 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3031 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 36:
#line 393 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 3037 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 37:
#line 394 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 3043 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 38:
#line 395 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 3049 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 39:
#line 396 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 3055 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 40:
#line 397 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 3061 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 41:
#line 401 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 3071 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 42:
#line 406 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->arguments.push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 3081 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 43:
#line 414 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3087 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 44:
#line 415 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("require"); }
#line 3093 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 45:
#line 419 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 3105 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 46:
#line 426 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 3119 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 47:
#line 438 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3128 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 48:
#line 442 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 3137 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 49:
#line 449 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = nullptr; }
#line 3143 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 50:
#line 450 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 3149 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 51:
#line 454 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3155 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 52:
#line 455 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = nullptr; }
#line 3161 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 53:
#line 456 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 3167 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 54:
#line 460 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3176 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 55:
#line 464 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 3185 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 56:
#line 471 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 3194 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 57:
#line 475 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!"); }
#line 3200 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 58:
#line 476 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("~"); }
#line 3206 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 59:
#line 477 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+="); }
#line 3212 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 60:
#line 478 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-="); }
#line 3218 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 61:
#line 479 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*="); }
#line 3224 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 62:
#line 480 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/="); }
#line 3230 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 63:
#line 481 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%="); }
#line 3236 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 64:
#line 482 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&="); }
#line 3242 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 65:
#line 483 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|="); }
#line 3248 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 66:
#line 484 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^="); }
#line 3254 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 67:
#line 485 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+"); }
#line 3260 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 68:
#line 486 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("-"); }
#line 3266 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 69:
#line 487 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("*"); }
#line 3272 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 70:
#line 488 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("/"); }
#line 3278 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 71:
#line 489 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("%"); }
#line 3284 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 72:
#line 490 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<"); }
#line 3290 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 73:
#line 491 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">"); }
#line 3296 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 74:
#line 492 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("=="); }
#line 3302 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 75:
#line 493 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("!="); }
#line 3308 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 76:
#line 494 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<="); }
#line 3314 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 77:
#line 495 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">="); }
#line 3320 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 78:
#line 496 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("&"); }
#line 3326 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 79:
#line 497 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("|"); }
#line 3332 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 80:
#line 498 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("^"); }
#line 3338 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 81:
#line 499 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("++"); }
#line 3344 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 82:
#line 500 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("--"); }
#line 3350 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 83:
#line 501 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("+++"); }
#line 3356 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 84:
#line 502 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("---"); }
#line 3362 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 85:
#line 503 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<"); }
#line 3368 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 86:
#line 504 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>"); }
#line 3374 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 87:
#line 505 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string("<<="); }
#line 3380 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 88:
#line 506 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = new string(">>="); }
#line 3386 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 89:
#line 511 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3456 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 90:
#line 579 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3464 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 91:
#line 582 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 3476 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 92:
#line 592 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = nullptr; }
#line 3482 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 93:
#line 593 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3488 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 94:
#line 594 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3494 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 95:
#line 595 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3500 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 96:
#line 596 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3506 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 97:
#line 597 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3512 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 98:
#line 598 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3518 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 99:
#line 599 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3524 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 100:
#line 600 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3530 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 101:
#line 601 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 3536 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 102:
#line 602 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3542 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 103:
#line 603 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 104:
#line 607 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = tokAt((yyloc));
    }
#line 3557 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 105:
#line 611 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 3568 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 106:
#line 617 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 3576 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 107:
#line 623 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3592 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 108:
#line 637 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3598 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 109:
#line 638 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            *(yyvsp[-2].s) += "::";
            *(yyvsp[-2].s) += *(yyvsp[0].s);
            delete (yyvsp[0].s);
            (yyval.s) = (yyvsp[-2].s);
        }
#line 3609 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 110:
#line 644 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 3615 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 111:
#line 648 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3623 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 112:
#line 654 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 3631 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 113:
#line 657 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 3639 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 114:
#line 660 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 3648 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 115:
#line 664 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3656 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 116:
#line 670 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 3662 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 117:
#line 674 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[0])),nullptr);
    }
#line 3670 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 118:
#line 677 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3678 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 119:
#line 680 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 3688 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 120:
#line 688 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3696 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 121:
#line 694 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 3702 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 122:
#line 695 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 3708 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 123:
#line 699 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pLet = new ExprLet();
        pLet->subexpr = ExpressionPtr((yyvsp[0].pExpression));
        for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name : *pDecl->pNameList ) {
                    if ( !pLet->find(name) ) {
                        VariablePtr pVar = make_shared<Variable>();
                        pVar->name = name;
                        pVar->at = pDecl->at;
                        pVar->type = make_shared<TypeDecl>(*pDecl->pTypeDecl);
                        if ( pDecl->pInit ) {
                            pVar->init = pDecl->pInit->clone();
                            pVar->move_to_init = pDecl->move_to_init;
                        }
                        pLet->variables.push_back(pVar);
                    } else {
                        das_yyerror("local variable is already declared " + name,tokAt((yylsp[-2])),
                            CompilationError::local_variable_already_declared);
                    }
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        (yyval.pExpression) = pLet;
    }
#line 3739 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 124:
#line 725 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pLet = new ExprLet();
        pLet->scoped = false;
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
#line 3769 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 125:
#line 753 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 3777 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 126:
#line 756 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)), true);
    }
#line 3785 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 127:
#line 762 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 3794 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 128:
#line 766 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 3804 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 129:
#line 771 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 3813 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 130:
#line 775 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 3823 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 131:
#line 783 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 3831 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 132:
#line 786 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3839 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 133:
#line 792 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 3849 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 134:
#line 798 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 3901 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 135:
#line 848 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 3907 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 136:
#line 849 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 3913 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 137:
#line 850 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 3919 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 138:
#line 851 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 3925 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 139:
#line 852 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 3931 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 140:
#line 853 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 3937 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 141:
#line 857 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 3943 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 142:
#line 858 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 3949 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 143:
#line 859 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3955 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 144:
#line 860 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3961 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 145:
#line 861 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3967 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 146:
#line 862 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3973 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 147:
#line 863 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3979 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 148:
#line 864 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3985 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 149:
#line 865 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3991 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 150:
#line 866 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 3997 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 151:
#line 867 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4003 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 152:
#line 868 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4009 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 153:
#line 873 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4019 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 154:
#line 878 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 4031 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 155:
#line 888 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 4037 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 156:
#line 889 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4043 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 157:
#line 890 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4049 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 158:
#line 891 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4055 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 159:
#line 892 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4061 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 160:
#line 893 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 4067 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 161:
#line 894 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 4073 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 162:
#line 895 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4079 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 163:
#line 896 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4085 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 164:
#line 897 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4091 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 165:
#line 898 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 4097 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 166:
#line 899 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4103 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 167:
#line 900 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4109 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 168:
#line 901 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4115 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 169:
#line 902 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4121 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 170:
#line 903 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4127 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 171:
#line 904 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4133 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 172:
#line 905 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4139 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 173:
#line 906 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4145 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 174:
#line 907 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4151 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 175:
#line 908 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4157 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 176:
#line 909 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4163 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 177:
#line 910 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4169 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 178:
#line 911 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4175 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 179:
#line 912 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4181 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 180:
#line 913 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4187 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 181:
#line 914 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4193 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 182:
#line 915 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 4199 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 183:
#line 916 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4205 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 184:
#line 917 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 4211 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 185:
#line 918 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4217 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 186:
#line 919 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4223 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 187:
#line 920 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4229 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 188:
#line 921 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4235 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 189:
#line 922 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4241 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 190:
#line 923 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4247 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 191:
#line 924 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4253 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 192:
#line 925 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 4262 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 193:
#line 929 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 4271 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 194:
#line 933 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 4279 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 195:
#line 936 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
        }
#line 4287 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 196:
#line 939 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4293 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 197:
#line 940 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-2]))); }
#line 4299 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 198:
#line 941 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprMakeLambda(tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 4305 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 199:
#line 942 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 4311 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 200:
#line 943 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 4319 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 201:
#line 946 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4325 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 202:
#line 947 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4331 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 203:
#line 948 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4337 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 204:
#line 949 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4343 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 205:
#line 950 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4349 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 206:
#line 951 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4364 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 207:
#line 961 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4379 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 208:
#line 971 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4415 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 209:
#line 1005 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aaList) = nullptr; }
#line 4421 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 210:
#line 1006 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 4427 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 211:
#line 1010 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4433 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 212:
#line 1011 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4439 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 213:
#line 1015 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 4449 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 214:
#line 1023 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 4457 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 215:
#line 1026 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 4466 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 216:
#line 1033 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4472 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 217:
#line 1034 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4478 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 218:
#line 1038 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4484 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 219:
#line 1039 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 4490 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 220:
#line 1043 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = false; }
#line 4496 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 221:
#line 1044 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.b) = true; }
#line 4502 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 222:
#line 1048 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 4511 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 223:
#line 1052 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 4519 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 224:
#line 1055 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4528 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 225:
#line 1059 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4538 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 226:
#line 1064 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4548 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 227:
#line 1072 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 4556 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 228:
#line 1075 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4565 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 229:
#line 1079 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4574 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 230:
#line 1083 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-2].b);
    }
#line 4584 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 231:
#line 1088 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->move_to_init = (yyvsp[-1].b);
    }
#line 4594 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 232:
#line 1096 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
                    if ( !g_Program->addVariable(pVar) )
                        das_yyerror("global variable is already declared " + name,pDecl->at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 4619 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 233:
#line 1119 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 4627 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 234:
#line 1122 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 4641 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 235:
#line 1131 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].i)) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 4655 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 236:
#line 1143 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 4669 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 237:
#line 1155 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4685 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 238:
#line 1169 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = nullptr; }
#line 4691 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 239:
#line 1170 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4697 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 240:
#line 1174 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4737 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 241:
#line 1212 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
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
#line 4805 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 242:
#line 1278 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 4817 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 243:
#line 1285 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 4828 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 244:
#line 1294 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tBool; }
#line 4834 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 245:
#line 1295 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tString; }
#line 4840 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 246:
#line 1296 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt; }
#line 4846 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 247:
#line 1297 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt64; }
#line 4852 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 248:
#line 1298 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt2; }
#line 4858 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 249:
#line 1299 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt3; }
#line 4864 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 250:
#line 1300 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tInt4; }
#line 4870 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 251:
#line 1301 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt; }
#line 4876 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 252:
#line 1302 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt64; }
#line 4882 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 253:
#line 1303 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt2; }
#line 4888 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 254:
#line 1304 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt3; }
#line 4894 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 255:
#line 1305 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tUInt4; }
#line 4900 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 256:
#line 1306 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat; }
#line 4906 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 257:
#line 1307 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat2; }
#line 4912 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 258:
#line 1308 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat3; }
#line 4918 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 259:
#line 1309 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tFloat4; }
#line 4924 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 260:
#line 1310 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tVoid; }
#line 4930 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 261:
#line 1311 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tRange; }
#line 4936 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 262:
#line 1312 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tURange; }
#line 4942 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 263:
#line 1313 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.type) = Type::tDouble; }
#line 4948 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 264:
#line 1317 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
        }
        delete (yyvsp[0].s);
    }
#line 4960 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 265:
#line 1327 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4969 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 266:
#line 1331 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 4981 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 267:
#line 1341 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 4987 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 268:
#line 1342 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4993 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 269:
#line 1343 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 4999 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 270:
#line 1344 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->dim.push_back((yyvsp[-1].i));
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5008 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 271:
#line 1348 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5018 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 272:
#line 1353 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5028 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 273:
#line 1358 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 5038 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 274:
#line 1363 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 5048 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 275:
#line 1368 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 5060 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 276:
#line 1375 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5069 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 277:
#line 1379 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5079 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 278:
#line 1384 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5090 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 279:
#line 1390 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5099 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 280:
#line 1394 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5109 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 281:
#line 1399 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5123 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 282:
#line 1408 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5132 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 283:
#line 1412 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5142 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 284:
#line 1417 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5156 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 285:
#line 1426 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 5165 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 286:
#line 1430 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 5175 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 287:
#line 1435 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 5189 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 288:
#line 1447 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5195 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 289:
#line 1448 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5201 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 290:
#line 1452 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 5213 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 291:
#line 1459 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 5224 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 292:
#line 1468 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 5234 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 293:
#line 1473 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5243 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 294:
#line 1480 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5253 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 295:
#line 1485 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5264 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 296:
#line 1494 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 5274 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 297:
#line 1499 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5283 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;

  case 298:
#line 1506 "src/parser/ds_parser.ypp" /* yacc.c:1645  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-5]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5293 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
    break;


#line 5297 "D:/common_info/GaijinGitHub/daScript/generated/ds_parser.cpp" /* yacc.c:1645  */
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
#line 1513 "src/parser/ds_parser.ypp" /* yacc.c:1903  */


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,at,cerr);
}

void das_yyerror(const string & error, CompilationError cerr) {
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


