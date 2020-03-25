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
    DAS_GENERATOR = 341,
    DAS_YIELD = 342,
    ADDEQU = 343,
    SUBEQU = 344,
    DIVEQU = 345,
    MULEQU = 346,
    MODEQU = 347,
    ANDEQU = 348,
    OREQU = 349,
    XOREQU = 350,
    SHL = 351,
    SHR = 352,
    ADDADD = 353,
    SUBSUB = 354,
    LEEQU = 355,
    SHLEQU = 356,
    SHREQU = 357,
    GREQU = 358,
    EQUEQU = 359,
    NOTEQU = 360,
    RARROW = 361,
    LARROW = 362,
    QQ = 363,
    QDOT = 364,
    LPIPE = 365,
    LBPIPE = 366,
    LAPIPE = 367,
    RPIPE = 368,
    CLONEEQU = 369,
    ROTL = 370,
    ROTR = 371,
    ROTLEQU = 372,
    ROTREQU = 373,
    MAPTO = 374,
    BRABRAB = 375,
    BRACBRB = 376,
    CBRCBRB = 377,
    INTEGER = 378,
    LONG_INTEGER = 379,
    UNSIGNED_INTEGER = 380,
    UNSIGNED_LONG_INTEGER = 381,
    FLOAT = 382,
    DOUBLE = 383,
    NAME = 384,
    BEGIN_STRING = 385,
    STRING_CHARACTER = 386,
    END_STRING = 387,
    BEGIN_STRING_EXPR = 388,
    END_STRING_EXPR = 389,
    UNARY_MINUS = 390,
    UNARY_PLUS = 391,
    PRE_INC = 392,
    PRE_DEC = 393,
    POST_INC = 394,
    POST_DEC = 395,
    DEREF = 396,
    COLCOL = 397
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

#line 370 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   6312

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  170
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  415
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  749

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   397

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   152,     2,   169,   167,   148,   141,     2,
     160,   161,   146,   145,   135,   144,   157,   147,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   138,   164,
     142,   136,   143,   137,   168,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   158,     2,   159,   140,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   165,   139,   166,   151,     2,     2,     2,
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
     149,   150,   153,   154,   155,   156,   162,   163
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   378,   378,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   393,   405,   406,   410,   414,   418,   427,
     435,   451,   472,   476,   479,   485,   494,   515,   538,   539,
     543,   547,   548,   552,   555,   561,   567,   570,   576,   577,
     581,   582,   583,   592,   593,   597,   606,   618,   628,   637,
     638,   639,   640,   641,   642,   646,   651,   659,   660,   661,
     662,   663,   664,   665,   666,   672,   685,   701,   702,   706,
     713,   725,   729,   736,   737,   741,   742,   743,   747,   751,
     758,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,   797,   801,   871,   874,
     884,   885,   886,   887,   888,   889,   890,   891,   892,   893,
     894,   895,   896,   897,   898,   899,   900,   901,   905,   909,
     915,   921,   932,   938,   939,   950,   954,   960,   963,   966,
     970,   976,   980,   984,   987,   990,   995,   998,  1006,  1009,
    1014,  1017,  1025,  1031,  1032,  1036,  1037,  1041,  1075,  1078,
    1083,  1091,  1095,  1100,  1106,  1110,  1115,  1124,  1127,  1133,
    1134,  1141,  1152,  1153,  1157,  1162,  1214,  1215,  1216,  1217,
    1218,  1219,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1242,  1243,
    1247,  1258,  1263,  1273,  1277,  1284,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,
    1332,  1333,  1334,  1335,  1336,  1340,  1344,  1347,  1350,  1351,
    1352,  1353,  1358,  1363,  1364,  1367,  1370,  1371,  1372,  1373,
    1374,  1375,  1376,  1386,  1396,  1430,  1431,  1435,  1436,  1440,
    1448,  1451,  1458,  1459,  1463,  1464,  1468,  1474,  1475,  1479,
    1480,  1484,  1489,  1492,  1496,  1502,  1511,  1512,  1513,  1517,
    1518,  1522,  1525,  1530,  1535,  1543,  1554,  1557,  1564,  1565,
    1569,  1599,  1602,  1611,  1623,  1635,  1646,  1661,  1662,  1666,
    1705,  1771,  1778,  1787,  1788,  1789,  1790,  1791,  1792,  1793,
    1794,  1795,  1796,  1797,  1798,  1799,  1800,  1801,  1802,  1803,
    1804,  1805,  1806,  1807,  1808,  1809,  1810,  1814,  1815,  1816,
    1817,  1818,  1819,  1820,  1821,  1825,  1836,  1840,  1850,  1851,
    1852,  1853,  1867,  1873,  1877,  1882,  1887,  1892,  1899,  1904,
    1908,  1912,  1917,  1922,  1929,  1934,  1940,  1945,  1949,  1954,
    1963,  1967,  1972,  1981,  1985,  1990,  1999,  2008,  2009,  2010,
    2011,  2015,  2022,  2031,  2036,  2043,  2048,  2054,  2060,  2067,
    2078,  2081,  2095,  2104,  2105,  2109,  2114,  2121,  2126,  2137,
    2142,  2149,  2161,  2162,  2166,  2180
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
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"generator\"", "\"yield\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
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
     385,   386,   387,   388,   389,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   390,
     391,   126,    33,   392,   393,   394,   395,    46,    91,    93,
      40,    41,   396,   397,    59,   123,   125,    36,    64,    35
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
    -441,    33,  -441,  -441,   -59,    44,    50,    85,   112,  -441,
     122,    -8,  -441,  -441,  -441,  -441,  -441,    60,  -441,   220,
    -441,  -441,  -441,  -441,   149,  -441,   170,   172,   180,  -441,
    -441,     6,  -441,   -92,   184,  -441,  -441,   195,   230,   190,
    -441,    83,  -441,   232,   101,  -441,   201,    35,   -59,   249,
      44,   244,  -441,   245,   246,  -441,   286,  -441,  5391,   248,
    -441,   -59,    -8,  -441,   240,   214,  6160,   347,   349,  -441,
     226,  -441,  -441,  -441,  -441,  -441,  -441,   252,   211,  -441,
    -441,  -441,  -441,   336,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,   224,  -110,   251,   254,   255,  -441,
    -441,  -441,   234,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,   256,  -441,  -441,
    -441,   257,   258,  -441,  -441,  -441,  -441,   259,  -441,  -441,
    -441,  -441,  2929,  -441,    37,  -441,   -77,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,   -10,   264,  -106,  -441,   169,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,    71,  -441,  -441,
    -107,  -441,  5391,  5391,  5391,   274,  5318,  5318,  5318,  5391,
    -441,    11,  -441,  -441,  -441,  -441,  2036,  -441,  -441,  -441,
      63,   275,  -441,  -441,  -441,    73,  -441,   -53,  5391,   241,
    -441,  -441,   187,  -441,  -441,   211,  -441,  -104,  3933,  -441,
     102,   223,   384,   247,   264,   615,   264,   733,   264,  1151,
    -441,   123,  2929,   268,  -441,   253,  -441,  -441,  -441,  5245,
     250,  -441,   261,   263,   269,   262,   271,   276,  3933,  3933,
      30,   734,  3933,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    3933,  3933,  3933,  3933,  3933,  -441,  3933,  -441,  -441,   260,
    -441,  -441,   -85,  -441,  -441,  -441,   265,  -441,  -441,  -441,
    -441,  -441,  5386,   267,  -441,  -441,  -441,  -441,  -441,   -59,
    -441,   374,   266,  -441,  -441,    -9,  -441,   288,  -441,  5391,
    2177,  2929,  -441,  5391,  -441,   108,  -441,  -441,  4611,  -441,
    5391,  -441,  -441,   281,  -441,   289,  -441,   293,  -441,  -441,
    5391,  -441,  -441,  1560,  -441,   290,  3933,  5391,  5391,  3933,
    5391,  5391,   110,   110,   275,  1203,   275,  1344,  5681,  -441,
     -34,   221,   110,   110,  -441,   110,   110,  4743,   879,    48,
    -441,  1612,   226,   412,  3933,  3933,  -441,  -441,  3933,  3933,
    3933,  3933,   311,  3933,   314,  3933,  3933,  3933,  3933,  3933,
    3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,
     316,  3933,  -441,  2311,   111,  -441,   275,  -441,  -441,  -441,
     359,   168,  -441,   320,  5532,  1765,  -441,  -441,  -441,  2177,
    -441,  1716,  -441,  -441,  -441,  -441,  2445,  1768,  4810,  1850,
    1984,  4876,  2174,  2259,    12,   -86,   287,  2579,  5681,   312,
    -132,   313,  -441,  -441,   -83,    21,  2713,     2,   152,  3933,
    3933,   291,  -441,  3933,   323,  -441,  -441,  3933,  -441,  -441,
     275,  1478,  -441,   241,  3933,  3933,  -441,   292,   335,  2847,
    1902,  -441,   424,   296,   297,  3933,  -441,  -441,  -441,  -441,
    -441,  -441,   298,   301,   304,  -441,  -441,  -441,   451,  -441,
     -93,  -441,  5625,  5318,  -441,   341,  -441,    96,  5849,   264,
    5391,   365,   365,   198,   198,  6126,  6126,   317,    18,  -441,
     -78,   -78,   365,   365,  5737,  5905,  5961,  6017,   198,   198,
     495,   495,    18,    18,    18,  -441,  5460,  -441,   113,  -441,
    -441,  3933,  -441,  -441,   168,  3933,  3933,  3933,  3933,  3933,
    3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,  3933,
    -441,  2177,  -441,  4088,  -441,  -441,   118,  5391,   351,  4942,
    -441,  3933,  3933,  -441,  3933,   321,  3933,  3933,  -441,   341,
     353,   325,   341,  3933,   326,  3933,  3933,   341,   327,   328,
    5849,  -441,  -441,  5793,  4006,    25,  2177,  -441,  -441,  4180,
     464,  5849,  4006,  -441,   350,   368,  5849,  2177,  -441,  4272,
     329,  -441,  -441,  4006,  -441,  -441,  -441,   455,   275,  -441,
    2981,  3115,   264,  2393,   -67,   129,  3933,  -441,   -97,  2929,
    3249,  3933,  -441,  -441,  5849,  -441,  5849,  5849,  5849,  5849,
    5849,  5849,  5849,  5849,  5849,  5849,  5849,  5849,  5849,  5849,
    5849,  -441,  4364,  -441,  -441,  2527,   139,  -441,   198,   198,
     198,  3383,    91,  5849,   -41,   -67,  -441,   312,  5849,  -441,
    -441,    98,   173,  -441,  -441,  -441,  -441,  3933,  -441,  4456,
    -441,   241,  -441,  -441,  -441,  -441,  4548,  -441,  -441,   302,
    -441,  -441,  3933,  3933,   354,   -77,   337,  5849,  3517,  -441,
    -441,  -441,   156,  5849,  -441,  -441,  3658,   372,  -441,  5008,
    3933,   343,  3933,  3933,   344,   105,  -441,  -441,  -441,  1047,
     241,  -441,  -441,  3933,  -441,  6073,  6073,   -77,  -441,  -441,
    3933,  5849,  -441,  5391,  5074,   361,  -441,  4680,  -441,  5849,
    4680,  -441,   241,  -441,  -441,  4006,  -441,  5849,  2661,  -441,
    3799,   458,   348,   363,  -441,   302,  -441,  5391,  5140,  3933,
     357,   360,  -441,  2795,  -441,  5849,  -441,  -441,  -441
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    73,     1,   165,     0,     0,     0,     0,     0,   166,
       0,     0,    12,     3,    10,     9,     8,     0,     7,   308,
       6,    11,     5,     4,    63,    65,    21,    33,    30,    31,
      23,    28,    22,     0,     0,    13,    68,   143,     0,    69,
      71,     0,    67,     0,     0,   309,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    26,     0,   311,     0,     0,
     145,     0,     0,    74,   317,     0,     0,     0,     0,    80,
      75,   306,    61,    62,    59,    60,    58,     0,     0,    57,
      66,    34,    32,    28,    25,    24,   347,   351,   350,   354,
     348,   352,   349,   353,     0,     0,     0,     0,     0,   323,
     343,   324,   356,   325,   329,   330,   331,   332,   336,   337,
     338,   339,   340,   341,   342,   344,   345,   377,   328,   335,
     346,   380,   383,   326,   333,   327,   334,     0,   355,   358,
     360,   359,   314,   144,     0,    72,     0,   319,   280,    83,
      84,    86,    85,    87,    88,    89,    90,   109,   110,   107,
     108,   100,   111,   112,   101,    98,    99,   113,   114,   115,
     116,   103,   104,   102,    96,    97,    92,    91,    93,    94,
      95,    82,    81,   105,   106,     0,    78,     0,    14,     0,
      53,    54,    51,    52,    50,    49,    55,     0,    27,   311,
       0,   315,     0,     0,     0,     0,    75,    75,    75,     0,
     364,     0,   370,   373,   372,   366,     0,   369,    70,   318,
     275,     0,   321,    76,   284,     0,   282,   291,     0,     0,
     310,   307,   299,    15,    16,     0,    64,     0,     0,   312,
       0,     0,     0,     0,    78,     0,    78,     0,    78,     0,
     287,     0,   286,   365,   368,     0,   371,   221,   222,     0,
       0,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   188,   187,   189,   190,   191,    17,
       0,     0,     0,     0,     0,   362,     0,   138,   182,   183,
     219,   184,   217,   268,   267,   266,    73,   271,   218,   270,
     269,   253,     0,     0,   220,   387,   388,   389,   390,     0,
     320,   277,     0,   283,    77,     0,   290,     0,   289,     0,
       0,    79,   117,     0,   300,     0,    56,   316,     0,   374,
       0,   376,   357,     0,   378,     0,   381,     0,   384,   386,
       0,   367,   363,   147,   150,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,     0,     0,     0,     0,   409,
       0,     0,   226,   225,   258,   224,   223,     0,     0,     0,
     274,     0,    75,     0,     0,     0,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   361,     0,     0,   278,     0,   281,   285,   322,
     292,     0,   295,     0,   294,     0,   297,   298,   296,     0,
     313,     0,   379,   382,   385,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,   400,   393,
       0,   403,   404,   405,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,    18,   249,   140,     0,    43,    44,
       0,     0,   151,     0,     0,     0,   152,     0,     0,     0,
       0,   120,   118,     0,     0,     0,   133,   128,   126,   127,
     139,   121,     0,     0,     0,   131,   132,   134,   163,   125,
       0,   122,   192,    75,   213,     0,   254,     0,   177,    78,
       0,   227,   228,   240,   241,   238,   239,     0,   263,   252,
     272,   273,   229,   230,     0,   243,   244,   242,   236,   237,
     232,   231,   233,   234,   235,   251,     0,   256,     0,   276,
     279,     0,   183,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,   305,   192,   375,   148,     0,     0,     0,     0,
     259,     0,     0,   260,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   410,   411,     0,     0,     0,     0,   153,   156,   192,
       0,   146,     0,   137,     0,     0,    37,     0,   160,   192,
       0,   135,   136,     0,   124,   129,   130,     0,     0,   123,
       0,     0,    78,     0,     0,     0,     0,   255,     0,   265,
       0,     0,   250,   257,   293,   141,   199,   200,   202,   201,
     203,   196,   197,   198,   204,   205,   194,   195,   206,   207,
     193,   303,   192,   304,   149,     0,     0,   171,   168,   169,
     170,     0,     0,   391,     0,     0,   395,   394,   401,   407,
     406,     0,     0,   398,   408,    19,    47,     0,   157,   192,
     154,     0,    48,    35,    36,   161,   192,   158,   138,    40,
     164,   167,     0,     0,     0,     0,     0,   178,     0,   179,
     185,   211,     0,   264,   302,   174,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,   155,   162,   159,     0,
       0,    38,    39,     0,    45,   209,   208,     0,   214,   210,
       0,   180,   212,     0,     0,     0,   262,   412,   397,   392,
     412,   399,     0,   119,    41,     0,   215,   181,     0,   172,
       0,     0,     0,     0,    46,    40,   175,     0,     0,     0,
       0,     0,    42,     0,   173,   413,   414,   415,   176
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -441,  -441,  -441,   181,   473,  -441,  -441,  -441,  -441,  -441,
    -441,   447,  -441,  -441,   481,  -441,  -441,  -441,  -202,  -441,
    -441,  -441,  -441,  -441,   309,  -441,   487,   -52,  -441,   476,
    -441,   283,  -184,  -231,  -441,  -441,  -217,  -441,  -129,  -255,
     -11,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,   539,
    -441,  -441,  -441,  -382,  -441,  -441,  -381,  -441,  -350,  -441,
    -441,  -441,  -441,   179,  -441,  -441,  -441,  -441,   236,  -441,
     215,  -441,  -211,  -194,   141,  -441,   -47,  -441,  -441,  -441,
     382,  -441,  -441,  -441,  -441,  -441,  -173,   -57,  -441,  -441,
    -441,   -48,   324,  -440,  -332,  -441,  -441,  -246,     7,   227,
    -441,  -441,  -441,  -145,  -441
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,   179,   185,   351,   280,    14,    15,    31,
      32,    55,    16,    28,    29,   463,   464,   703,   704,   465,
     466,   467,   468,   469,   186,   187,    25,    26,    39,    40,
      41,    17,   176,   219,    70,    18,   281,   470,   358,   471,
     282,   472,   283,   473,   474,   475,   476,   477,   598,   478,
     479,   284,   285,   487,   680,   286,   287,   288,   403,   481,
     289,   290,   291,   488,   301,   396,   302,   210,   214,   215,
     240,   241,   557,   216,   409,   315,   221,   177,    46,    20,
      95,    21,    22,   137,    65,    23,   217,   293,    94,   130,
     131,   242,   294,   429,   430,   295,   431,   432,   433,   434,
     296,   350,   297,   732,   298
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   129,   312,   323,   222,   325,   310,   327,   480,   134,
     132,   518,   234,   236,   238,   437,   349,   303,   524,   190,
     523,   306,   678,   212,    36,   190,    51,   561,   372,   228,
     556,   374,   562,     2,   546,   211,   211,   363,     3,   566,
     306,   344,   243,   657,   360,   605,    56,   128,    72,    73,
     308,    42,    37,    96,   306,   402,   191,   229,    52,    97,
     220,     4,   317,     5,    43,     6,    44,     7,   277,   308,
      24,   599,     8,    57,    98,   361,   564,    59,     9,   390,
     391,   565,   307,   308,    10,   309,    38,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   366,   367,   691,   212,
     212,    37,   647,   562,   372,   209,   373,   374,   375,   363,
     440,   376,   441,   200,    66,   129,   129,   129,   201,   129,
     129,   129,   129,   615,   230,   231,   232,   307,   235,   237,
     239,   213,   244,    53,   542,    38,   307,   202,    74,    37,
     307,   129,    75,    54,    76,    77,   562,    27,   568,   245,
     311,   424,    48,   435,   642,   390,   391,    37,   489,    30,
     246,   128,   128,   128,   651,   128,   128,   128,   128,   521,
     483,    11,   129,    38,   571,    78,   578,    12,   208,    67,
      68,   333,   520,   129,   129,   588,   225,   128,   366,   367,
     203,    38,   345,   347,    33,   406,   372,   363,    62,   374,
     375,   299,   407,   376,   180,   181,   606,   644,   682,   300,
      69,   606,   226,   606,   304,   652,   580,   305,   128,   204,
     606,    34,    63,   205,   408,   319,    48,   394,   606,   128,
     128,    35,   129,   606,   200,   690,   129,   607,   608,   201,
     206,   400,   693,   129,   560,   405,   329,   390,   391,   722,
     519,   207,   411,   129,   613,   695,    45,   575,   202,   634,
     129,   129,   686,   129,   129,    47,   631,   330,   676,   419,
     420,   606,   422,   423,   364,   365,   366,   367,   128,   602,
     223,   224,   128,   687,   372,    48,   373,   374,   375,   128,
      49,   376,   700,   377,   378,    50,   565,   712,   569,   128,
      58,   658,   307,   701,   702,   313,   128,   128,   314,   128,
     128,   203,   665,   277,   182,   278,   522,   562,   183,   694,
     184,    77,   385,   386,   387,   388,   389,    86,   484,   480,
      61,    87,   178,   442,   443,   390,   391,   656,    59,    60,
     204,    64,    88,    89,   205,   662,    71,    90,    91,    92,
      93,   674,    81,    83,    84,    85,   669,   133,   136,   138,
     173,   206,   174,   178,   363,   292,   175,   320,    52,   189,
     200,   679,   207,   192,   195,   201,   193,   194,   196,   197,
     198,   199,   218,   233,   212,   337,   277,   318,   322,   331,
     335,   338,   332,   340,   202,   200,   395,   399,   341,   417,
     201,   336,   339,    11,   412,   222,   129,   393,   359,   490,
     397,   524,   413,   129,   692,   603,   414,   342,   343,   202,
     497,   348,   609,   499,   697,   515,   558,   560,   563,   352,
     353,   354,   355,   356,   223,   357,   583,   572,   584,   590,
     591,   592,   594,   366,   367,   595,   306,   203,   596,   597,
     604,   372,   128,   373,   374,   375,   661,   610,   376,   128,
     636,   641,   645,   724,   646,   649,   653,   654,   663,   404,
     129,   664,   203,   670,   668,   308,   204,   707,   709,   635,
     205,   715,   718,   721,   730,   734,   739,   740,   735,   385,
     386,   387,   388,   389,   363,   418,   746,   206,   421,   747,
      79,   204,   390,   391,   428,   205,   428,   321,   207,   741,
     188,    82,   444,   742,   316,    80,   128,   482,   135,   699,
      19,   398,   206,   491,   492,   415,   541,   493,   494,   495,
     496,   671,   498,   207,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   362,
     516,   227,   650,   334,   438,   733,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   543,     0,
       0,     0,     0,   366,   367,     0,   549,     0,     0,     0,
       0,   372,     0,   373,   374,   375,   357,     0,   376,     0,
       0,     0,     0,     0,     0,   357,     0,     0,   570,   348,
       0,     0,   573,     0,     0,     0,   574,     0,     0,     0,
     579,     0,     0,   581,   582,     0,     0,     0,   586,   589,
       0,   387,   388,   389,   593,     0,   200,     0,     0,     0,
       0,   201,   390,   391,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,   708,   728,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,   743,
       0,     0,     0,     0,     0,     0,   726,     0,     0,     0,
     614,     0,   128,     0,   616,   617,   618,   619,   620,   621,
     622,   623,   624,   625,   626,   627,   628,   629,   630,     0,
     632,     0,     0,   203,     0,     0,   128,     0,     0,     0,
     638,   639,     0,   640,     0,     0,   643,     0,     0,     0,
       0,     0,   648,     0,   428,   346,     0,     0,     0,     0,
       0,     0,   204,     0,     0,   659,   205,    96,   324,     0,
       0,     0,     0,    97,   200,     0,   666,     0,     0,   201,
       0,     0,     0,   206,     0,     0,     0,     0,    98,   626,
     630,     0,     0,     0,   207,   677,     0,     0,   202,     0,
     683,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     689,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   705,   706,     0,     0,     0,     0,   711,     0,     0,
       0,     0,     0,    37,     0,   714,     0,     0,     0,   717,
     204,   719,   720,     0,   205,     0,   326,     0,   482,     0,
     446,     0,   725,     0,     3,     0,   447,   448,   449,   727,
     450,   206,   247,   248,   249,   250,     0,    38,     0,     0,
       0,     0,   207,   451,   251,   452,   453,     0,     0,   738,
       0,     0,     0,     0,     0,   454,   252,     0,   745,   455,
     253,     0,   254,     0,     9,   255,   456,     0,   457,   256,
       0,     0,   458,   459,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,   460,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,     0,     0,     0,     0,
     273,   274,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,    38,   461,   277,   462,   278,   279,   446,     0,
       0,     0,     3,     0,   447,   448,   449,     0,   450,     0,
     247,   248,   249,   250,     0,     0,     0,     0,     0,     0,
       0,   451,   251,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,   454,   252,     0,     0,   455,   253,     0,
     254,     0,     9,   255,   456,     0,   457,   256,     0,     0,
     458,   459,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,     0,   257,   460,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,     0,   200,     0,     0,     0,     0,   201,     0,     0,
       0,   270,   271,   272,     0,     0,     0,     0,   273,   274,
       0,     0,     0,     0,     0,     0,   202,   276,     0,     0,
      38,   461,   277,   723,   278,   279,   247,   248,   249,   250,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,   200,     0,     0,     0,     0,   201,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,   202,   203,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,   204,   257,
       0,     0,   205,     0,   328,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,   206,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
     207,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   425,   269,     0,     0,     0,     0,     0,     0,
     204,     0,     0,     0,   205,     0,     0,   270,   271,   272,
       0,     0,     0,     0,   273,   274,     0,   247,   248,   249,
     250,   206,   426,   427,     0,     0,    38,     0,   277,   251,
     278,   279,   207,     0,     0,   200,     0,     0,     0,     0,
     201,   252,     0,     0,     0,   253,     0,   254,     0,     0,
     255,     0,     0,     0,   256,     0,     0,     0,     0,   202,
       0,    99,   100,   101,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     118,   119,   120,     0,     0,   123,   124,   125,   126,     0,
     257,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   425,   269,     0,     0,     0,     0,     0,
       0,   204,     0,     0,     0,   205,     0,     0,   270,   271,
     272,   247,   248,   249,   250,   273,   274,     0,     0,     0,
       0,     0,   206,   251,   436,     0,     0,    38,     0,   277,
       0,   278,   279,   207,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
       0,     0,     0,     0,     0,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
     401,   200,     0,     0,     0,     0,   201,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,     0,     0,     0,     0,   251,   276,     0,
       0,    38,   577,   277,     0,   278,   279,     0,     0,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,     0,     0,     0,     0,   203,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,     0,   123,   124,   125,   126,   204,   257,     0,
       0,   205,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   259,     0,     0,     0,     0,     0,     0,   206,     0,
     416,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,    37,   269,     0,     0,     0,     0,   200,     0,     0,
       0,     0,   201,     0,     0,     0,   270,   271,   272,     0,
       0,     0,     0,   273,   274,     0,     0,     0,     0,     0,
     485,   202,   276,   486,     0,    38,     0,   277,     0,   278,
     279,   247,   248,   249,   250,   547,     0,     0,     0,     0,
       0,     0,     0,   251,     0,     0,   200,     0,     0,     0,
       0,   201,     0,     0,     0,   252,     0,     0,     0,   253,
       0,   254,     0,     0,   255,     0,     0,     0,   256,     0,
     202,     0,     0,     0,   203,    99,   100,   101,     0,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,   120,     0,     0,   123,
     124,   125,   126,   204,   257,     0,     0,   205,     0,   544,
       0,     0,     0,     0,     0,     0,   258,   259,     0,     0,
       0,     0,   406,   203,   206,     0,     0,     0,     0,   407,
       0,   200,     0,     0,     0,   207,   201,     0,   260,   261,
     262,   263,   264,   265,   266,   267,   268,    37,   269,     0,
       0,   408,   204,     0,     0,   202,   205,     0,     0,     0,
     548,     0,   270,   271,   272,   247,   248,   249,   250,   273,
     274,     0,     0,   206,     0,     0,     0,   251,   276,   540,
       0,    38,     0,   277,   207,   278,   279,     0,     0,   252,
       0,     0,     0,   253,     0,   254,     0,     0,   255,     0,
       0,     0,   256,     0,     0,     0,     0,     0,   203,    99,
     100,   101,     0,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,   118,   119,
     120,     0,     0,   123,   124,   125,   126,   204,   257,     0,
       0,   205,     0,   551,     0,     0,     0,     0,     0,     0,
     258,   259,     0,     0,     0,     0,     0,     0,   206,   587,
       0,     0,     0,     0,   401,   200,     0,     0,     0,   207,
     201,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,    37,   269,     0,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,   270,   271,   272,   247,
     248,   249,   250,   273,   274,     0,     0,     0,     0,     0,
       0,   251,   276,     0,     0,    38,     0,   277,     0,   278,
     279,     0,     0,   252,     0,     0,     0,   253,     0,   254,
       0,     0,   255,     0,     0,     0,   256,     0,     0,     0,
       0,     0,   203,    99,   100,   101,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   118,   119,   120,     0,     0,   123,   124,   125,
     126,   204,   257,     0,     0,   205,     0,   552,     0,     0,
       0,     0,     0,     0,   258,   259,     0,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   207,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   267,   268,    37,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   271,   272,     0,     0,     0,     0,   273,   274,     0,
     247,   248,   249,   250,     0,   275,   276,     0,     0,    38,
       0,   277,   251,   278,   279,   200,     0,     0,     0,     0,
     201,     0,     0,     0,   252,     0,     0,     0,   253,     0,
     254,     0,     0,   255,     0,     0,     0,   256,     0,   202,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,     0,   257,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,     0,     0,   401,
     200,     0,     0,     0,     0,   201,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,   204,     0,     0,   202,   205,     0,   554,     0,     0,
       0,   270,   271,   272,   247,   248,   249,   250,   273,   274,
       0,     0,   206,     0,     0,     0,   251,   276,     0,     0,
      38,     0,   277,   207,   278,   279,     0,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,   203,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,   204,   257,     0,     0,
     205,     0,   555,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,   200,     0,     0,     0,   207,   201,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,     0,   202,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   247,   248,
     249,   250,   273,   274,     0,     0,     0,     0,     0,     0,
     251,   276,   517,     0,    38,     0,   277,     0,   278,   279,
       0,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,   203,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
     204,   257,     0,     0,   205,     0,   675,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   200,     0,
       0,     0,   207,   201,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,     0,
       0,     0,   202,     0,     0,     0,     0,     0,     0,   270,
     271,   272,   247,   248,   249,   250,   273,   274,     0,     0,
       0,     0,     0,     0,   251,   276,   545,     0,    38,     0,
     277,     0,   278,   279,     0,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,     0,     0,     0,   203,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,   204,   257,     0,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,   206,     0,     0,   685,     0,
       0,     0,   200,     0,     0,     0,   207,   201,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,   202,     0,     0,     0,
       0,     0,     0,   270,   271,   272,   247,   248,   249,   250,
     273,   274,     0,     0,     0,     0,     0,     0,   251,   276,
     559,     0,    38,     0,   277,     0,   278,   279,     0,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,   203,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,   204,   257,
       0,     0,   205,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,   206,
       0,     0,   736,     0,     0,     0,   200,     0,     0,     0,
     207,   201,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,     0,     0,
       0,     0,   251,   276,   567,     0,    38,     0,   277,     0,
     278,   279,     0,     0,   252,     0,     0,     0,   253,     0,
     254,     0,     0,   255,     0,     0,     0,   256,     0,     0,
     585,     0,     0,   203,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,   204,   257,     0,     0,   205,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,     0,   206,     0,     0,   748,     0,     0,     0,
     200,     0,     0,     0,   207,   201,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,     0,     0,     0,   202,     0,     0,     0,     0,     0,
       0,   270,   271,   272,   247,   248,   249,   250,   273,   274,
       0,     0,     0,     0,     0,     0,   251,   276,     0,     0,
      38,     0,   277,     0,   278,   279,     0,     0,   252,     0,
       0,     0,   253,     0,   254,     0,     0,   255,     0,     0,
       0,   256,     0,     0,     0,     0,     0,   203,    99,   100,
     101,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   118,   119,   120,
       0,     0,   123,   124,   125,   126,   204,   257,     0,     0,
     205,     0,     0,     0,     0,     0,     0,     0,     0,   258,
     259,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,   672,     0,     0,     0,     0,   207,     0,
       0,   260,   261,   262,   263,   264,   265,   266,   267,   268,
      37,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   247,   248,
     249,   250,   273,   274,     0,     0,     0,     0,     0,     0,
     251,   276,     0,     0,    38,     0,   277,     0,   278,   279,
       0,     0,   252,     0,     0,     0,   253,     0,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,     0,    99,   100,   101,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   118,   119,   120,     0,     0,   123,   124,   125,   126,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   673,     0,     0,
       0,     0,     0,     0,     0,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    37,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     271,   272,   247,   248,   249,   250,   273,   274,     0,     0,
       0,     0,     0,     0,   251,   276,     0,     0,    38,     0,
     277,     0,   278,   279,     0,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,   247,   248,   249,   250,
     273,   274,     0,     0,     0,     0,     0,     0,   251,   276,
     681,     0,    38,     0,   277,     0,   278,   279,     0,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     247,   248,   249,   250,   273,   274,     0,     0,     0,     0,
       0,     0,   251,   276,   688,     0,    38,     0,   277,     0,
     278,   279,     0,     0,   252,     0,     0,     0,   253,     0,
     254,     0,     0,   255,     0,     0,     0,   256,     0,     0,
       0,     0,     0,     0,    99,   100,   101,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   118,   119,   120,     0,     0,   123,   124,
     125,   126,     0,   257,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,     0,     0,
       0,     0,     0,     0,   710,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,    37,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,   271,   272,     0,     0,     0,     0,   273,   274,
       0,   247,   248,   249,   250,   713,     0,   276,     0,     0,
      38,     0,   277,   251,   278,   279,     0,     0,     0,     0,
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
     274,     0,   247,   248,   249,   250,   737,     0,   276,     0,
       0,    38,     0,   277,   251,   278,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     253,     0,   254,     0,     0,   255,     0,     0,     0,   256,
       0,     0,     0,     0,     0,     0,    99,   100,   101,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   118,   119,   120,     0,     0,
     123,   124,   125,   126,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,   265,   266,   267,   268,    37,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   271,   272,   247,   248,   249,   250,
     273,   274,     0,     0,     0,     0,     0,     0,   251,   276,
       0,     0,    38,     0,   277,     0,   278,   279,     0,     0,
     252,     0,     0,     0,   253,     0,   254,     0,     0,   255,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
      99,   100,   101,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   118,
     119,   120,     0,     0,   123,   124,   125,   126,     0,   257,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,   266,
     267,   268,    37,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   271,   272,
       0,     0,     0,     0,   273,   274,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,    38,     0,   277,     0,
     278,   279,   364,   365,   366,   367,   368,   363,     0,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,     0,     0,
       0,     0,     0,   390,   391,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,   531,   532,   364,   365,   366,   367,   368,   533,
     534,   369,   370,   371,   372,   535,   373,   374,   375,   363,
       0,   376,   536,   377,   378,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,     0,
       0,     0,     0,     0,     0,   390,   391,     0,     0,     0,
       0,     0,   633,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,   526,
     527,   528,   529,   530,   531,   532,   364,   365,   366,   367,
     368,   533,   534,   369,   370,   371,   372,   535,   373,   374,
     375,   363,     0,   376,   536,   377,   378,   537,   538,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   390,   391,     0,
       0,     0,     0,     0,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,   532,   364,   365,
     366,   367,   368,   533,   534,   369,   370,   371,   372,   535,
     373,   374,   375,   363,     0,   376,   536,   377,   378,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   539,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     391,     0,     0,     0,     0,     0,   667,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,   528,   529,   530,   531,   532,
     364,   365,   366,   367,   368,   533,   534,   369,   370,   371,
     372,   535,   373,   374,   375,   363,     0,   376,   536,   377,
     378,   537,   538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     539,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,     0,     0,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,     0,     0,     0,   684,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,   532,   364,   365,   366,   367,   368,   533,   534,   369,
     370,   371,   372,   535,   373,   374,   375,   363,     0,   376,
     536,   377,   378,   537,   538,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,     0,     0,
       0,     0,     0,   390,   391,     0,     0,     0,     0,     0,
     696,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,   531,   532,   364,   365,   366,   367,   368,   533,
     534,   369,   370,   371,   372,   535,   373,   374,   375,     0,
       0,   376,   536,   377,   378,   537,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   363,
       0,     0,     0,     0,     0,   390,   391,   364,   365,   366,
     367,   368,   698,     0,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,   363,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,     0,   410,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   363,
       0,     0,     0,     0,     0,     0,     0,   390,   391,   364,
     365,   366,   367,   368,   731,     0,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,   363,     0,     0,     0,     0,
     390,   391,     0,     0,   445,     0,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,   363,     0,     0,     0,     0,     0,   390,   391,     0,
       0,   550,   364,   365,   366,   367,   368,     0,     0,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,   363,     0,     0,
       0,     0,     0,   390,   391,     0,     0,   553,   364,   365,
     366,   367,   368,     0,     0,   369,   370,   371,   372,     0,
     373,   374,   375,     0,     0,   376,     0,   377,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,   363,     0,     0,     0,     0,     0,   390,
     391,     0,     0,   637,   364,   365,   366,   367,   368,     0,
       0,   369,   370,   371,   372,     0,   373,   374,   375,     0,
       0,   376,     0,   377,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   363,
       0,     0,     0,     0,     0,   390,   391,     0,     0,   716,
     364,   365,   366,   367,   368,     0,     0,   369,   370,   371,
     372,     0,   373,   374,   375,     0,     0,   376,     0,   377,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,     0,     0,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,   729,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,    97,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,    98,
       0,     0,     0,     0,     0,     0,     0,   390,   391,     0,
       0,   744,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,   260,   261,   262,     0,     0,
       0,     0,     0,     0,    37,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,     0,   363,     0,     0,    38,     0,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,     0,   175,   363,
       0,    38,   364,   365,   366,   367,   368,     0,     0,   369,
     370,   371,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,     0,     0,
       0,     0,     0,   390,   391,   392,     0,     0,     0,     0,
       0,   363,     0,     0,    38,     0,   364,   365,   366,   367,
     368,     0,     0,   369,   370,   371,   372,     0,   373,   374,
     375,     0,     0,   376,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   390,   391,   612,
     525,   526,   527,   528,   529,   530,   531,   532,   364,   365,
     366,   367,   368,   533,   534,   369,   370,   371,   372,   535,
     373,   374,   375,  -192,   363,   376,   536,   377,   378,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   539,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     391,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
     532,   364,   365,   366,   367,   368,   533,   534,   369,   370,
     371,   372,   600,   373,   374,   375,     0,     0,   376,   536,
     377,   378,   537,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,   601,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,   364,   365,   366,
     367,   368,   390,   391,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,   379,     0,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,   364,   365,   366,   367,   368,   390,   391,
     369,   370,   371,   372,     0,   373,   374,   375,     0,     0,
     376,     0,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,     0,
       0,     0,     0,     0,   379,   611,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,   364,
     365,   366,   367,   368,   390,   391,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,     0,   655,     0,     0,
     379,     0,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,   364,   365,   366,   367,   368,
     390,   391,   369,   370,   371,   372,     0,   373,   374,   375,
       0,     0,   376,     0,   377,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,   364,   365,   366,   367,   368,   390,   391,   369,   370,
     371,   372,     0,   373,   374,   375,     0,     0,   376,     0,
     377,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,   383,   384,   385,
     386,   387,   388,   389,     0,     0,     0,   364,   365,   366,
     367,   368,   390,   391,   369,   370,   371,   372,     0,   373,
     374,   375,     0,     0,   376,     0,   377,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   382,   383,   384,   385,   386,   387,   388,   389,
       0,     0,     0,   364,   365,   366,   367,   368,   390,   391,
     369,   370,   371,   372,     0,   373,   374,   375,     0,     0,
     376,     0,   377,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,   388,   389,     0,     0,     0,   364,
     365,     0,     0,   368,   390,   391,   369,   370,   371,   372,
       0,   373,   374,   375,     0,     0,   376,     0,   377,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,   380,   381,   382,   383,   384,     0,     0,     0,
     388,   389,   364,   365,   366,   367,   368,     0,     0,   369,
     390,   391,   372,     0,   373,   374,   375,     0,     0,   376,
       0,   377,   378,     0,     0,     0,     0,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,     0,   383,   384,
     385,   386,   387,   388,   389,   157,   158,   159,   160,     0,
       0,     0,     0,   390,   391,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,   171,   172
};

static const yytype_int16 yycheck[] =
{
      11,    58,   219,   234,   177,   236,   217,   238,   358,    61,
      58,   393,   196,   197,   198,   347,   262,   211,   111,   129,
     401,   107,   119,   129,    32,   129,    20,   159,   106,   136,
      18,   109,   164,     0,   416,    45,    45,    19,     5,    18,
     107,    11,    31,    18,   129,   485,   138,    58,    13,    14,
     136,    62,   129,    23,   107,   310,   166,   164,    52,    29,
     166,    28,   166,    30,     4,    32,     6,    34,   165,   136,
     129,   164,    39,   165,    44,   160,   159,   163,    45,   157,
     158,   164,   135,   136,    51,   138,   163,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    98,    99,   159,   129,
     129,   129,   562,   164,   106,   136,   108,   109,   110,    19,
     164,   113,   166,    31,    33,   192,   193,   194,    36,   196,
     197,   198,   199,   524,   192,   193,   194,   135,   196,   197,
     198,   161,   141,   147,   409,   163,   135,    55,   123,   129,
     135,   218,   127,   157,   129,   130,   164,   123,   166,   158,
     218,   344,   135,   346,   556,   157,   158,   129,   362,   129,
     169,   192,   193,   194,   566,   196,   197,   198,   199,   400,
     142,   158,   249,   163,   440,   160,   451,   164,   161,    98,
      99,   249,   396,   260,   261,   460,   135,   218,    98,    99,
     108,   163,   260,   261,   129,   107,   106,    19,   135,   109,
     110,   158,   114,   113,    13,    14,   135,   559,   610,   166,
     129,   135,   161,   135,   161,   567,   453,   164,   249,   137,
     135,   129,   159,   141,   136,   143,   135,   299,   135,   260,
     261,   129,   309,   135,    31,   164,   313,   161,   489,    36,
     158,   309,   164,   320,   135,   313,   143,   157,   158,   164,
     159,   169,   320,   330,   161,   657,    56,   450,    55,   161,
     337,   338,   143,   340,   341,   136,   541,   164,   159,   337,
     338,   135,   340,   341,    96,    97,    98,    99,   309,   483,
     131,   132,   313,   164,   106,   135,   108,   109,   110,   320,
     138,   113,    10,   115,   116,   135,   164,   161,   166,   330,
     136,   576,   135,    21,    22,   138,   337,   338,   141,   340,
     341,   108,   587,   165,   123,   167,   168,   164,   127,   166,
     129,   130,   144,   145,   146,   147,   148,    61,   359,   699,
     160,    65,   131,   132,   133,   157,   158,   574,   163,   129,
     137,   129,    76,    77,   141,   582,   165,    81,    82,    83,
      84,   602,   123,   129,   129,   129,   593,   129,   138,   165,
      33,   158,    33,   131,    19,   206,   160,   164,    52,   165,
      31,   608,   169,   142,   160,    36,   142,   142,   142,   142,
     142,   142,   138,   129,   129,   142,   165,   228,   161,   141,
     160,   142,   159,   142,    55,    31,    42,   129,   142,   129,
      36,   160,   160,   158,   143,   598,   483,   160,   168,    17,
     164,   111,   143,   490,   645,   483,   143,   258,   259,    55,
     129,   262,   490,   129,   661,   129,   159,   135,   135,   270,
     271,   272,   273,   274,   131,   276,   164,   166,   123,    35,
     164,   164,   164,    98,    99,   164,   107,   108,   164,    18,
     129,   106,   483,   108,   109,   110,    12,   160,   113,   490,
     129,   160,   129,   700,   159,   159,   159,   159,   138,   310,
     547,   123,   108,    38,   165,   136,   137,   143,   161,   547,
     141,   129,   159,   159,   143,   722,    48,   159,   725,   144,
     145,   146,   147,   148,    19,   336,   159,   158,   339,   159,
      47,   137,   157,   158,   345,   141,   347,   143,   169,   166,
      83,    50,   351,   735,   225,    48,   547,   358,    62,   668,
       1,   305,   158,   364,   365,   330,   405,   368,   369,   370,
     371,   598,   373,   169,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   286,
     391,   189,   565,   249,   347,   720,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   409,    -1,
      -1,    -1,    -1,    98,    99,    -1,   417,    -1,    -1,    -1,
      -1,   106,    -1,   108,   109,   110,   427,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,   436,    -1,    -1,   439,   440,
      -1,    -1,   443,    -1,    -1,    -1,   447,    -1,    -1,    -1,
     451,    -1,    -1,   454,   455,    -1,    -1,    -1,   459,   460,
      -1,   146,   147,   148,   465,    -1,    31,    -1,    -1,    -1,
      -1,    36,   157,   158,    -1,    -1,   713,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   675,   713,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     737,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   737,
      -1,    -1,    -1,    -1,    -1,    -1,   707,    -1,    -1,    -1,
     521,    -1,   713,    -1,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,    -1,
     541,    -1,    -1,   108,    -1,    -1,   737,    -1,    -1,    -1,
     551,   552,    -1,   554,    -1,    -1,   557,    -1,    -1,    -1,
      -1,    -1,   563,    -1,   565,    11,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,   576,   141,    23,   143,    -1,
      -1,    -1,    -1,    29,    31,    -1,   587,    -1,    -1,    36,
      -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    44,   600,
     601,    -1,    -1,    -1,   169,   606,    -1,    -1,    55,    -1,
     611,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
     641,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   672,   673,    -1,    -1,    -1,    -1,   678,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   686,    -1,    -1,    -1,   690,
     137,   692,   693,    -1,   141,    -1,   143,    -1,   699,    -1,
       1,    -1,   703,    -1,     5,    -1,     7,     8,     9,   710,
      11,   158,    13,    14,    15,    16,    -1,   163,    -1,    -1,
      -1,    -1,   169,    24,    25,    26,    27,    -1,    -1,   730,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,   739,    40,
      41,    -1,    43,    -1,    45,    46,    47,    -1,    49,    50,
      -1,    -1,    53,    54,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,   167,   168,     1,    -1,
      -1,    -1,     5,    -1,     7,     8,     9,    -1,    11,    -1,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    47,    -1,    49,    50,    -1,    -1,
      53,    54,    -1,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    55,   160,    -1,    -1,
     163,   164,   165,   166,   167,   168,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,   108,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   137,    86,
      -1,    -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,   152,    -1,    13,    14,    15,
      16,   158,   159,   160,    -1,    -1,   163,    -1,   165,    25,
     167,   168,   169,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,    -1,   144,   145,
     146,    13,    14,    15,    16,   151,   152,    -1,    -1,    -1,
      -1,    -1,   158,    25,   160,    -1,    -1,   163,    -1,   165,
      -1,   167,   168,   169,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
     112,    31,    -1,    -1,    -1,    -1,    36,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    13,    14,    15,    16,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,    -1,
      -1,   163,   164,   165,    -1,   167,   168,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   108,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,   137,    86,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
     158,    55,   160,   161,    -1,   163,    -1,   165,    -1,   167,
     168,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      55,    -1,    -1,    -1,   108,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   137,    86,    -1,    -1,   141,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,   107,   108,   158,    -1,    -1,    -1,    -1,   114,
      -1,    31,    -1,    -1,    -1,   169,    36,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   136,   137,    -1,    -1,    55,   141,    -1,    -1,    -1,
     142,    -1,   144,   145,   146,    13,    14,    15,    16,   151,
     152,    -1,    -1,   158,    -1,    -1,    -1,    25,   160,   164,
      -1,   163,    -1,   165,   169,   167,   168,    -1,    -1,    37,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   108,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    -1,    -1,    81,    82,    83,    84,   137,    86,    -1,
      -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,   158,   107,
      -1,    -1,    -1,    -1,   112,    31,    -1,    -1,    -1,   169,
      36,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,    13,
      14,    15,    16,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   160,    -1,    -1,   163,    -1,   165,    -1,   167,
     168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,   108,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,   137,    86,    -1,    -1,   141,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,    -1,
      13,    14,    15,    16,    -1,   159,   160,    -1,    -1,   163,
      -1,   165,    25,   167,   168,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    55,
      -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      31,    -1,    -1,    -1,    -1,    36,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,   137,    -1,    -1,    55,   141,    -1,   143,    -1,    -1,
      -1,   144,   145,   146,    13,    14,    15,    16,   151,   152,
      -1,    -1,   158,    -1,    -1,    -1,    25,   160,    -1,    -1,
     163,    -1,   165,   169,   167,   168,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,   108,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   137,    86,    -1,    -1,
     141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   169,    36,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,    13,    14,
      15,    16,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   160,   161,    -1,   163,    -1,   165,    -1,   167,   168,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   108,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
     137,    86,    -1,    -1,   141,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   169,    36,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,    13,    14,    15,    16,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   160,   161,    -1,   163,    -1,
     165,    -1,   167,   168,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,   108,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,   137,    86,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   169,    36,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,    13,    14,    15,    16,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,
     161,    -1,   163,    -1,   165,    -1,   167,   168,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   108,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   137,    86,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     169,    36,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      13,    14,    15,    16,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   160,   161,    -1,   163,    -1,   165,    -1,
     167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,   108,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,   137,    86,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   169,    36,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,    13,    14,    15,    16,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   160,    -1,    -1,
     163,    -1,   165,    -1,   167,   168,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,   108,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   137,    86,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,   169,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,    13,    14,
      15,    16,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      25,   160,    -1,    -1,   163,    -1,   165,    -1,   167,   168,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,    13,    14,    15,    16,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   160,    -1,    -1,   163,    -1,
     165,    -1,   167,   168,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,    13,    14,    15,    16,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,
     161,    -1,   163,    -1,   165,    -1,   167,   168,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      13,    14,    15,    16,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   160,   161,    -1,   163,    -1,   165,    -1,
     167,   168,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,
      -1,    13,    14,    15,    16,    17,    -1,   160,    -1,    -1,
     163,    -1,   165,    25,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
     152,    -1,    13,    14,    15,    16,    17,    -1,   160,    -1,
      -1,   163,    -1,   165,    25,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,    13,    14,    15,    16,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    25,   160,
      -1,    -1,   163,    -1,   165,    -1,   167,   168,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,    -1,   165,    -1,
     167,   168,    96,    97,    98,    99,   100,    19,    -1,   103,
     104,   105,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    19,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    19,    -1,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    19,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    19,    -1,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,   137,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,    19,    -1,   113,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   157,   158,    96,    97,    98,
      99,   100,   164,    -1,   103,   104,   105,   106,    -1,   108,
     109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,    -1,    -1,    -1,    -1,   164,    96,    97,    98,    99,
     100,    -1,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    96,
      97,    98,    99,   100,   164,    -1,   103,   104,   105,   106,
      -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
     157,   158,    -1,    -1,   161,    -1,    96,    97,    98,    99,
     100,    -1,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,
      -1,   161,    96,    97,    98,    99,   100,    -1,    -1,   103,
     104,   105,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,   161,    96,    97,
      98,    99,   100,    -1,    -1,   103,   104,   105,   106,    -1,
     108,   109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   157,
     158,    -1,    -1,   161,    96,    97,    98,    99,   100,    -1,
      -1,   103,   104,   105,   106,    -1,   108,   109,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   157,   158,    -1,    -1,   161,
      96,    97,    98,    99,   100,    -1,    -1,   103,   104,   105,
     106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,    -1,   161,    96,    97,    98,    99,
     100,    -1,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,
      -1,   161,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,   120,   121,   122,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    19,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,   160,    19,
      -1,   163,    96,    97,    98,    99,   100,    -1,    -1,   103,
     104,   105,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,   137,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   163,    -1,    96,    97,    98,    99,
     100,    -1,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    19,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,   137,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   157,   158,   103,   104,   105,   106,    -1,   108,
     109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   157,   158,
     103,   104,   105,   106,    -1,   108,   109,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   157,   158,   103,   104,   105,   106,
      -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   134,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     157,   158,   103,   104,   105,   106,    -1,   108,   109,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   137,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   157,   158,   103,   104,
     105,   106,    -1,   108,   109,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   157,   158,   103,   104,   105,   106,    -1,   108,
     109,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   157,   158,
     103,   104,   105,   106,    -1,   108,   109,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    96,
      97,    -1,    -1,   100,   157,   158,   103,   104,   105,   106,
      -1,   108,   109,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
     147,   148,    96,    97,    98,    99,   100,    -1,    -1,   103,
     157,   158,   106,    -1,   108,   109,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,   142,   143,
     144,   145,   146,   147,   148,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,   151,   152
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   171,     0,     5,    28,    30,    32,    34,    39,    45,
      51,   158,   164,   172,   177,   178,   182,   201,   205,   219,
     249,   251,   252,   255,   129,   196,   197,   123,   183,   184,
     129,   179,   180,   129,   129,   129,    32,   129,   163,   198,
     199,   200,   210,     4,     6,    56,   248,   136,   135,   138,
     135,    20,    52,   147,   157,   181,   138,   165,   136,   163,
     129,   160,   135,   159,   129,   254,    33,    98,    99,   129,
     204,   165,    13,    14,   123,   127,   129,   130,   160,   174,
     196,   123,   184,   129,   129,   129,    61,    65,    76,    77,
      81,    82,    83,    84,   258,   250,    23,    29,    44,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,   210,   257,
     259,   260,   261,   129,   197,   199,   138,   253,   165,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   115,   116,   117,
     118,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   151,   152,    33,    33,   160,   202,   247,   131,   173,
      13,    14,   123,   127,   129,   174,   194,   195,   181,   165,
     129,   166,   142,   142,   142,   160,   142,   142,   142,   142,
      31,    36,    55,   108,   137,   141,   158,   169,   161,   210,
     237,    45,   129,   161,   238,   239,   243,   256,   138,   203,
     166,   246,   256,   131,   132,   135,   161,   250,   136,   164,
     261,   261,   261,   129,   202,   261,   202,   261,   202,   261,
     240,   241,   261,    31,   141,   158,   169,    13,    14,    15,
      16,    25,    37,    41,    43,    46,    50,    86,    98,    99,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   130,
     144,   145,   146,   151,   152,   159,   160,   165,   167,   168,
     176,   206,   210,   212,   221,   222,   225,   226,   227,   230,
     231,   232,   233,   257,   262,   265,   270,   272,   274,   158,
     166,   234,   236,   243,   161,   164,   107,   135,   136,   138,
     242,   261,   206,   138,   141,   245,   194,   166,   233,   143,
     164,   143,   161,   203,   143,   203,   143,   203,   143,   143,
     164,   141,   159,   261,   262,   160,   160,   142,   142,   160,
     142,   142,   233,   233,    11,   261,    11,   261,   233,   267,
     271,   175,   233,   233,   233,   233,   233,   233,   208,   168,
     129,   160,   201,    19,    96,    97,    98,    99,   100,   103,
     104,   105,   106,   108,   109,   110,   113,   115,   116,   137,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     157,   158,   159,   160,   197,    42,   235,   164,   238,   129,
     261,   112,   209,   228,   233,   261,   107,   114,   136,   244,
     164,   261,   143,   143,   143,   240,   160,   129,   233,   261,
     261,   233,   261,   261,   256,   129,   159,   160,   233,   263,
     264,   266,   267,   268,   269,   256,   160,   264,   269,   119,
     164,   166,   132,   133,   173,   161,     1,     7,     8,     9,
      11,    24,    26,    27,    36,    40,    47,    49,    53,    54,
      87,   164,   166,   185,   186,   189,   190,   191,   192,   193,
     207,   209,   211,   213,   214,   215,   216,   217,   219,   220,
     228,   229,   233,   142,   210,   158,   161,   223,   233,   202,
      17,   233,   233,   233,   233,   233,   233,   129,   233,   129,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   129,   233,   161,   223,   159,
     243,   242,   168,   226,   111,    88,    89,    90,    91,    92,
      93,    94,    95,   101,   102,   107,   114,   117,   118,   136,
     164,   244,   209,   233,   143,   161,   223,    17,   142,   233,
     161,   143,   143,   161,   143,   143,    18,   242,   159,   161,
     135,   159,   164,   135,   159,   164,    18,   161,   166,   166,
     233,   267,   166,   233,   233,   256,   107,   164,   209,   233,
     206,   233,   233,   164,   123,    53,   233,   107,   209,   233,
      35,   164,   164,   233,   164,   164,   164,    18,   218,   164,
     107,   136,   202,   261,   129,   263,   135,   161,   203,   261,
     160,   138,   159,   161,   233,   226,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   209,   233,   164,   161,   261,   129,   161,   233,   233,
     233,   160,   223,   233,   264,   129,   159,   263,   233,   159,
     268,   223,   264,   159,   159,   134,   206,    18,   209,   233,
     164,    12,   206,   138,   123,   209,   233,   164,   165,   206,
      38,   246,   112,   112,   203,   143,   159,   233,   119,   206,
     224,   161,   223,   233,   164,   161,   143,   164,   161,   233,
     164,   159,   242,   164,   166,   223,   164,   206,   164,   208,
      10,    21,    22,   187,   188,   233,   233,   143,   210,   161,
     107,   233,   161,    17,   233,   129,   161,   233,   159,   233,
     233,   159,   164,   166,   206,   233,   210,   233,   261,   161,
     143,   164,   273,   273,   206,   206,   161,    17,   233,    48,
     159,   166,   188,   261,   161,   233,   159,   159,   161
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   170,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   173,   173,   174,   175,   175,   175,
     176,   177,   178,   179,   179,   179,   180,   180,   181,   181,
     182,   183,   183,   184,   184,   185,   186,   186,   187,   187,
     188,   188,   188,   189,   189,   190,   191,   192,   193,   194,
     194,   194,   194,   194,   194,   195,   195,   196,   196,   196,
     196,   196,   196,   196,   196,   197,   197,   198,   198,   199,
     199,   200,   200,   201,   201,   202,   202,   202,   203,   203,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   205,   206,   206,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   208,   208,
     208,   209,   209,   210,   210,   210,   211,   212,   212,   212,
     212,   213,   214,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   217,   218,   218,   219,   219,   220,   221,   221,
     221,   222,   222,   222,   222,   222,   222,   223,   223,   224,
     224,   224,   225,   225,   226,   226,   227,   227,   227,   227,
     227,   227,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     230,   231,   231,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   235,   236,
     237,   237,   238,   238,   239,   239,   240,   241,   241,   242,
     242,   243,   243,   243,   243,   243,   244,   244,   244,   245,
     245,   246,   246,   246,   246,   246,   247,   247,   248,   248,
     249,   250,   250,   250,   251,   252,   252,   253,   253,   254,
     255,   256,   256,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   258,   258,   258,
     258,   258,   258,   258,   258,   259,   260,   260,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   263,   263,   264,   264,   265,   265,   265,   265,   265,
     266,   266,   267,   268,   268,   269,   269,   270,   270,   271,
     271,   272,   273,   273,   274,   274
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
       4,     6,     7,     3,     5,     4,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     3,     0,     1,     3,
       0,     3,     1,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     2,     0,     1,
       5,     0,     3,     5,     4,     5,     7,     0,     2,     2,
       6,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     4,     3,     4,     2,     3,     2,     4,     3,     2,
       2,     3,     2,     2,     4,     6,     4,     1,     4,     5,
       1,     4,     5,     1,     4,     5,     4,     1,     1,     1,
       1,     3,     5,     1,     3,     5,     4,     7,     5,     7,
       1,     3,     3,     1,     1,     1,     3,     5,     5,     1,
       3,     4,     0,     3,    10,    10
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
          case 129: /* "name"  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 2995 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 173: /* character_sequence  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3001 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* string_constant  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3007 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* string_builder_body  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3013 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* string_builder  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3019 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 179: /* require_module_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3025 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 185: /* expression_label  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3031 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_goto  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3037 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 188: /* expression_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3043 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* expression_if_then_else  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3049 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_for_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3055 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_while_loop  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3061 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_with  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3067 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* annotation_argument_value  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3073 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_argument_value_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3079 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_argument  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3085 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* annotation_argument_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3091 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* annotation_declaration_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3097 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* annotation_declaration  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3103 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3109 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* optional_annotation_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3115 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* optional_function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3121 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* optional_function_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3127 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* function_name  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3133 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* expression_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3139 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_any  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3145 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expressions  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3151 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expr_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3157 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* name_in_namespace  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3163 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* expression_delete  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3169 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expr_new  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3175 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expression_break  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3181 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expression_continue  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3187 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expression_return  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3193 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_yield  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3199 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expression_try_catch  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3205 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expression_let  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3211 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expr_cast  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3217 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_type_info  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3223 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3229 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* block_or_simple_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3235 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_block  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3241 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_numeric_const  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3247 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr_assign  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3253 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expr_assign_pipe  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3259 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* expr_named_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3265 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* expr_method_call  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3271 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* func_addr_expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3277 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* expr  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3283 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* optional_field_annotation  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3289 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 236: /* structure_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3295 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* struct_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3301 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* function_argument_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3307 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* function_argument_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3313 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* tuple_type  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3319 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* tuple_type_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3325 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3331 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* let_variable_declaration  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3337 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 247: /* global_variable_declaration_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3343 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* enum_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3349 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* optional_structure_parent  */
#line 102 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3355 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* variable_name_list  */
#line 103 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3361 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* structure_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3367 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* auto_type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3373 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* type_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3379 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 262: /* make_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3385 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 263: /* make_struct_fields  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3391 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* make_struct_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3397 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* make_struct_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3403 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* make_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3409 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 267: /* make_map_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3415 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* make_any_tuple  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3421 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 269: /* make_dim  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3427 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 270: /* make_dim_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3433 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 271: /* make_table  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3439 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 272: /* make_table_decl  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3445 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 273: /* array_comprehension_where  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3451 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 274: /* array_comprehension  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3457 "ds_parser.cpp" /* yacc.c:1254  */
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
#line 393 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3753 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 14:
#line 405 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3759 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 406 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3765 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 410 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 414 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3780 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 418 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 427 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3804 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 435 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 451 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 476 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3853 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 479 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 485 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 494 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3901 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 515 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3926 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 538 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 539 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 552 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 3946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 3954 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 3962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 567 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 3970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 570 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 3978 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 576 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 3984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 577 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 3990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 581 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 3996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4002 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 592 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 597 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4036 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 606 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 618 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 628 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4073 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 639 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4103 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 642 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 659 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4141 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4153 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 663 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 664 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4179 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 672 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4197 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 685 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 701 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 702 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 706 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4239 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 713 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4253 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 725 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4262 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4271 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 736 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 737 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4283 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 741 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4295 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4319 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 758 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4328 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4334 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 763 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 765 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4352 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 766 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4358 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 767 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4364 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4370 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4376 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4388 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 784 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 802 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 871 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 884 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 885 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 886 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 888 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 892 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4689 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4719 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4725 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4731 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4737 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4743 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 915 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 921 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 932 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 938 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 939 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 950 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4831 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 960 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4839 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4856 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 970 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 976 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 980 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 984 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4884 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 987 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4902 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4910 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1006 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1014 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1017 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1025 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4964 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1031 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1032 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4976 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1036 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1037 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1041 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1075 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1078 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1083 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5052 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1091 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5061 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1095 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5071 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1110 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5101 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5112 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1127 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5128 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5134 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1134 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5146 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1141 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5159 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1152 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1157 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1163 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1214 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1215 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1216 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5252 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1218 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5264 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1219 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5270 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5276 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1224 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5282 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1225 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5306 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5312 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5318 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1233 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1234 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5342 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5348 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1263 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1273 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5433 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1284 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1298 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1299 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1300 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1301 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1302 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1303 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1304 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1305 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1307 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1308 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1309 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1310 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1322 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5611 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5617 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5623 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5629 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5635 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5641 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5647 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5653 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5659 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1334 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5671 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1335 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5677 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5686 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5711 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1350 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5717 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1351 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5723 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1352 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5729 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1353 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1358 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5749 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1363 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1364 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1367 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5771 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5777 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1371 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5783 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5789 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5795 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5801 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5822 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1386 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5837 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1430 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5879 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1431 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 5885 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1435 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1436 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5897 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1440 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 5907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1448 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 5915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1451 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 5924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1458 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->constant = true; }
#line 5930 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1459 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDecl) = (yyvsp[0].pVarDecl); (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true; }
#line 5936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1463 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1464 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5948 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1468 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1474 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1475 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 5968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1479 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1484 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 5990 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1489 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 5998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1496 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6018 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1511 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 6035 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 6041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1513 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6047 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1518 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6059 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1525 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6077 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1530 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1535 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6100 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1543 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6113 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1554 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6121 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1557 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6130 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1564 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6136 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1565 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6142 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1569 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1599 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1602 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6196 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1611 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1623 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-2].s),tokAt((yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-2].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 6224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1635 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1646 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6263 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1662 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6269 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1666 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1705 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1771 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6390 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1778 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6401 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6413 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1800 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1801 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1802 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1803 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6503 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1804 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6509 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1805 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6515 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1806 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6521 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1807 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6527 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1808 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6533 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1809 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1810 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1814 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1815 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6557 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1816 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6563 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1817 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6569 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1818 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6575 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1819 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6581 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1820 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6587 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1821 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6593 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1825 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6606 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1836 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1840 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1850 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6633 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6639 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1852 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1853 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1867 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6675 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1873 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6684 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6694 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1882 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6704 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1887 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6714 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1892 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1899 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1904 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1908 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1912 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6764 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1917 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6774 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1922 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6786 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1929 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6796 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6807 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1940 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6826 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1949 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6859 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1967 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6869 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6883 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1981 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1985 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6902 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 1990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6916 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 1999 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList));
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6927 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2009 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6939 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2010 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6945 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2011 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6951 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2015 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 6963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2022 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 6974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2031 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 6984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2036 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 6993 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2043 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2048 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructure();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2054 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2060 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2067 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructure *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2078 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7058 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2081 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2095 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2104 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2105 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2109 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7107 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2114 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7116 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2121 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2126 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7139 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 409:
#line 2137 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7149 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 410:
#line 2142 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7158 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 411:
#line 2149 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7172 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 412:
#line 2161 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7178 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 413:
#line 2162 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7184 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 414:
#line 2166 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 415:
#line 2180 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7222 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7226 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2196 "ds_parser.ypp" /* yacc.c:1903  */


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


