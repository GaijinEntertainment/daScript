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
#line 53 "ds_parser.ypp" /* yacc.c:338  */

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

    extern bool das_need_oxford_comma;

#line 192 "ds_parser.cpp" /* yacc.c:353  */

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
#line 79 "ds_parser.ypp" /* yacc.c:353  */

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

#line 373 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   6574

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  171
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  427
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  780

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
       0,   383,   383,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   399,   411,   412,   416,   420,   424,
     433,   441,   457,   478,   482,   485,   491,   500,   521,   544,
     545,   549,   553,   554,   558,   561,   567,   573,   576,   582,
     583,   587,   588,   589,   598,   599,   603,   612,   624,   634,
     643,   644,   645,   646,   647,   648,   652,   657,   665,   666,
     667,   668,   669,   670,   671,   672,   678,   691,   707,   708,
     712,   719,   731,   735,   742,   743,   747,   748,   749,   753,
     757,   764,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   807,   880,
     883,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   914,
     918,   924,   930,   941,   947,   948,   959,   963,   969,   972,
     975,   979,   985,   989,   993,   996,   999,  1004,  1007,  1015,
    1018,  1023,  1026,  1034,  1040,  1041,  1045,  1046,  1050,  1084,
    1087,  1092,  1100,  1104,  1109,  1115,  1119,  1124,  1133,  1136,
    1142,  1143,  1150,  1161,  1162,  1166,  1171,  1226,  1227,  1228,
    1229,  1230,  1231,  1235,  1236,  1237,  1238,  1239,  1240,  1241,
    1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1254,
    1255,  1259,  1270,  1275,  1285,  1289,  1296,  1310,  1311,  1312,
    1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,
    1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,
    1343,  1344,  1345,  1346,  1347,  1348,  1352,  1356,  1359,  1362,
    1363,  1364,  1365,  1370,  1375,  1376,  1379,  1382,  1386,  1390,
    1394,  1395,  1396,  1397,  1398,  1399,  1400,  1410,  1420,  1454,
    1455,  1459,  1460,  1464,  1472,  1475,  1482,  1487,  1495,  1496,
    1500,  1503,  1512,  1513,  1517,  1526,  1527,  1532,  1533,  1537,
    1542,  1545,  1549,  1555,  1564,  1565,  1566,  1570,  1571,  1575,
    1578,  1583,  1588,  1596,  1607,  1610,  1617,  1618,  1622,  1652,
    1655,  1664,  1677,  1689,  1690,  1694,  1698,  1709,  1724,  1725,
    1729,  1768,  1834,  1841,  1850,  1851,  1852,  1853,  1854,  1855,
    1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,  1864,  1865,
    1866,  1867,  1868,  1869,  1870,  1871,  1872,  1873,  1877,  1878,
    1879,  1880,  1881,  1882,  1886,  1897,  1901,  1911,  1912,  1913,
    1914,  1928,  1934,  1938,  1943,  1948,  1953,  1960,  1965,  1969,
    1973,  1978,  1983,  1990,  1995,  2001,  2006,  2010,  2015,  2024,
    2028,  2033,  2042,  2046,  2051,  2060,  2066,  2075,  2081,  2096,
    2097,  2098,  2099,  2103,  2110,  2119,  2124,  2131,  2136,  2142,
    2148,  2155,  2166,  2169,  2183,  2192,  2193,  2197,  2202,  2209,
    2214,  2225,  2230,  2237,  2249,  2250,  2254,  2268
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
     385,   386,   387,   388,   389,   390,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     391,   392,   126,    33,   393,   394,   395,   396,    46,    91,
      93,    40,    41,   397,   398,    59,   123,   125,    36,    64,
      35
};
# endif

#define YYPACT_NINF -485

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-485)))

#define YYTABLE_NINF -194

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -485,    10,  -485,  -485,   -23,    30,    -5,    36,    33,  -485,
      83,  -485,   -15,  -485,  -485,  -485,  -485,  -485,   296,  -485,
     126,  -485,  -485,  -485,  -485,    99,  -485,    81,  -485,    92,
     102,   111,  -485,  -485,    16,  -485,   -55,   118,  -485,  -485,
      87,   122,    96,  -485,    17,  -485,   141,    14,  -485,   112,
     117,    11,   -23,   168,    30,   169,  -485,   173,   176,  -485,
     273,  -485,   186,  -485,   -91,   183,  -485,   -23,   -15,  -485,
     185,   164,  6421,   302,   306,  -485,   179,  -485,   212,  -485,
    -485,  -485,  -485,  -485,   211,   109,  -485,  -485,  -485,  -485,
     293,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,   182,
     -89,  5627,  -485,  -485,  -485,   -38,  -485,   -71,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,   -92,   213,   -87,   220,
    -485,   197,  -485,    77,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,    35,  -485,  -485,   -68,  -485,   208,   222,   225,  -485,
    -485,  -485,   202,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,   227,  -485,  -485,
    -485,   228,   229,  -485,  -485,  -485,  -485,   230,   232,  -485,
    -485,  -485,  -485,   466,  -485,  -485,  -105,   -23,  -485,   -14,
    -485,   -56,  5627,   214,  -485,  -485,  -485,   146,  5627,   -85,
    -485,  -485,   109,  -485,   -77,  4117,  -485,  5627,  5627,  5627,
     247,  5553,  5553,  5553,  5701,   212,  -485,     3,  -485,  -485,
    -485,  -485,  2071,  -485,  -485,  -485,   336,   216,    45,   249,
    -485,   -62,  -485,   234,  3645,  -485,  -485,   254,  5627,  -485,
     -58,  3645,  -485,  -485,  -485,  -485,  -485,  -485,  5479,   226,
    -485,   236,   245,   255,   239,   260,   263,  4117,  4117,  4238,
    4314,  4117,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  4117,
    4117,  4117,  4117,  4117,  4117,  -485,   238,  -485,  -485,   -67,
    -485,  -485,  -485,   250,  -485,  -485,  -485,  -485,  -485,  4843,
     251,  -485,  -485,  -485,  -485,  -485,   563,   657,  1194,   246,
     213,  1471,   213,  1741,   213,  1933,   -34,  -485,   -84,  3645,
      88,   269,  -485,   256,  -485,  -485,  5696,  -485,   249,  -485,
    -485,  -485,  -485,  -485,  5627,  2213,  -485,   921,  -485,   199,
    -485,  -485,  -485,  2213,   492,  -485,   284,  4117,  5627,  5627,
    4117,  5627,  5627,   444,   444,   249,  1247,   249,  1389,  5998,
    -485,   144,   163,   444,   444,  -485,   444,   444,  4975,   -78,
    -485,  1659,   179,     6,   288,  4117,  4117,  -485,  -485,  4117,
    4117,  4117,  4117,   289,  4117,   290,  4117,  4117,  4117,  4117,
    2348,  4117,  4117,  4117,  4117,  4117,  4117,  4117,  4117,  4117,
    4117,   291,  4117,  -485,  2483,  -485,  5627,  -485,  -485,   280,
    -485,   286,  -485,   287,  -485,  5627,  -485,  5701,  -485,   212,
    -485,  -485,  -485,  -485,   156,   107,  -485,   323,  5841,  -485,
    4117,  -485,  -485,   249,  1524,  -485,   214,  4117,  4117,  -485,
     271,   313,  2618,  1794,  -485,   404,   277,   282,  4117,  -485,
    -485,  -485,  -485,  -485,  -485,   294,   295,   300,  -485,  -485,
    -485,   431,  -485,   -80,  -485,  5942,  -485,  2213,  -485,  4315,
    2753,  1936,  5043,  2018,  2210,  5109,  2430,  2565,     1,   -73,
     307,  2888,  5998,   317,   -94,   318,  -485,  -485,   -13,    12,
    3023,   145,   155,  4117,  4117,   299,  -485,  4117,   343,  -485,
    5553,  -485,   338,  -485,    40,  6166,   213,  5627,  -485,  -485,
     341,   341,   662,   662,  6387,  6387,   315,    84,  -485,   -59,
     -59,   341,   341,   347,  6054,  6222,  6278,  6334,   662,   662,
     170,   170,    84,    84,    84,  -485,  5769,  -485,    42,  2700,
    -485,  -485,  -485,  3645,  -485,  4117,  -485,  -485,   107,  4117,
    4117,  4117,  4117,  4117,  4117,  4117,  4117,  4117,  4117,  4117,
    4117,  4117,  4117,  4117,   735,    15,  2213,  -485,  -485,  4408,
     467,  6166,   735,  -485,   339,   356,  6166,  2213,  -485,  4501,
     316,  -485,  -485,   735,  -485,  -485,  -485,   443,   249,  -485,
    3158,  3293,  -485,  4594,  -485,  -485,    52,  5627,   353,  5175,
    -485,  4117,  4117,  -485,  4117,   324,  4117,  4117,  -485,   338,
     354,   331,   338,  4117,   332,  4117,  4117,   338,   333,   334,
    6166,  -485,  -485,  6110,   213,  2835,   -50,    76,  4117,  -485,
     -83,  3645,  3428,  -485,  4117,  -485,  -485,  -485,  6166,  -485,
    6166,  6166,  6166,  6166,  6166,  6166,  6166,  6166,  6166,  6166,
    6166,  6166,  6166,  6166,  6166,  -485,  4117,  -485,  4687,  -485,
     214,  -485,  -485,  -485,  -485,  4780,  -485,  -485,    98,  -485,
    -485,  4117,  4117,  -485,  -485,  2970,   101,  -485,   662,   662,
     662,  3563,   -64,  6166,     5,   -50,  -485,   317,  6166,  -485,
    -485,   -63,   166,  -485,  -485,  -485,   351,   -71,   342,  6166,
    3698,  -485,  -485,  -485,    65,  6166,   -44,  -485,  -485,  -485,
    1090,   214,  -485,  -485,  4117,  -485,   285,   285,  -485,  3840,
     366,  -485,  5241,  4117,   346,  4117,  4117,   348,   -71,  -485,
    -485,  4117,  6166,  -485,   214,  -485,  -485,   735,  5627,  5307,
     357,  -485,  4912,  -485,  6166,  4912,  -485,  -485,  6166,  -485,
      98,  3105,  -485,  3982,   455,   349,   344,  -485,  -485,  5627,
    5373,  4117,   350,   352,  3510,  -485,  6166,  -485,  -485,  -485
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    74,     1,   166,     0,     0,     0,     0,     0,   167,
       0,   397,     0,    13,     3,    10,     9,     8,     0,     7,
     316,     6,    11,     5,     4,     0,    12,    64,    66,    22,
      34,    31,    32,    24,    29,    23,     0,     0,    14,    69,
     144,     0,    70,    72,     0,    68,     0,     0,   317,     0,
       0,     0,     0,     0,     0,     0,    30,     0,     0,    27,
       0,   319,     0,   323,     0,     0,   146,     0,     0,    75,
     328,     0,     0,     0,     0,    81,    76,   314,     0,    62,
      63,    60,    61,    59,     0,     0,    58,    67,    35,    33,
      29,    26,    25,   358,   361,   359,   362,   360,   363,     0,
       0,     0,   325,   324,   145,     0,    73,     0,   330,   284,
      84,    85,    87,    86,    88,    89,    90,    91,   110,   111,
     108,   109,   101,   112,   113,   102,    99,   100,   114,   115,
     116,   117,   104,   105,   103,    97,    98,    93,    92,    94,
      95,    96,    83,    82,   106,   107,   279,    79,     0,     0,
     295,     0,    15,     0,    54,    55,    52,    53,    51,    50,
      56,     0,    28,   319,     0,   326,     0,     0,     0,   334,
     354,   335,   365,   336,   340,   341,   342,   343,   347,   348,
     349,   350,   351,   352,   353,   355,   356,   386,   339,   346,
     357,   389,   392,   337,   344,   338,   345,     0,     0,   364,
     367,   369,   368,     0,    71,   329,   279,     0,    77,     0,
     288,     0,     0,     0,   332,   318,   315,   307,     0,     0,
      16,    17,     0,    65,     0,     0,   320,     0,     0,     0,
       0,    76,    76,    76,     0,     0,   373,     0,   379,   382,
     381,   375,     0,   322,   378,   331,   281,     0,     0,     0,
     286,   299,    78,   279,    80,   139,   118,     0,     0,   308,
       0,   294,   398,   296,    57,   327,   222,   223,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   189,   188,   190,   191,   192,    18,     0,
       0,     0,     0,     0,     0,   183,   184,   220,   185,   218,
     272,   271,   270,    74,   275,   219,   274,   273,   254,     0,
       0,   221,   399,   400,   401,   402,     0,     0,     0,     0,
      79,     0,    79,     0,    79,     0,   144,   292,     0,   290,
       0,   374,   377,     0,   380,   371,     0,   282,     0,   285,
     280,   287,   298,   297,     0,     0,   289,     0,   333,     0,
     305,   306,   304,     0,   148,   151,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,     0,     0,
     421,     0,     0,   227,   226,   259,   225,   224,     0,     0,
     278,     0,    76,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,     0,   383,     0,   385,   366,     0,
     387,     0,   390,     0,   393,     0,   395,     0,   396,     0,
     376,   372,   370,   283,   300,     0,   303,     0,   302,   141,
       0,    44,    45,     0,     0,   152,     0,     0,     0,   153,
       0,     0,     0,     0,   121,   119,     0,     0,     0,   134,
     129,   127,   128,   140,   122,     0,     0,     0,   132,   133,
     135,   164,   126,     0,   123,   193,   309,     0,   313,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,     0,   412,   405,     0,   415,   416,   417,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,    19,   250,
      76,   214,     0,   255,     0,   178,    79,     0,   267,   268,
     228,   229,   241,   242,   239,   240,     0,   264,   253,   276,
     277,   230,   231,     0,     0,   244,   245,   243,   237,   238,
     233,   232,   234,   235,   236,   252,     0,   257,     0,     0,
     388,   391,   394,   291,   293,     0,   184,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   157,   193,
       0,   147,     0,   138,     0,     0,    38,     0,   161,   193,
       0,   136,   137,     0,   125,   130,   131,     0,     0,   124,
       0,     0,   311,   193,   312,   149,     0,     0,     0,     0,
     260,     0,     0,   261,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,   422,   423,     0,    79,     0,     0,     0,     0,   256,
       0,   266,     0,   269,     0,   251,   258,   384,   301,   142,
     200,   201,   203,   202,   204,   197,   198,   199,   205,   206,
     195,   196,   207,   208,   194,    48,     0,   158,   193,   155,
       0,    49,    36,    37,   162,   193,   159,   139,    41,   165,
     168,     0,     0,   310,   150,     0,     0,   172,   169,   170,
     171,     0,     0,   403,     0,     0,   407,   406,   413,   419,
     418,     0,     0,   410,   420,    20,     0,     0,     0,   179,
       0,   180,   186,   212,     0,   265,     0,   156,   163,   160,
       0,     0,    39,    40,     0,    46,   210,   209,   175,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,   215,
     211,     0,   181,   213,     0,   120,    42,     0,     0,     0,
       0,   263,   424,   409,   404,   424,   411,   216,   182,    47,
      41,     0,   173,     0,     0,     0,     0,    43,   176,     0,
       0,     0,     0,     0,     0,   174,   425,   426,   427,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -485,  -485,  -485,   142,   464,  -485,  -485,  -485,  -485,  -485,
    -485,   426,  -485,  -485,   463,  -485,  -485,  -485,  -242,  -485,
    -485,  -485,  -485,  -485,   297,  -485,   468,   -40,  -485,   454,
    -485,   221,  -219,  -313,  -485,  -485,  -208,  -485,  -151,  -327,
     -12,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,   528,
    -485,  -485,  -485,  -392,  -485,  -485,  -415,  -485,  -344,  -485,
    -485,  -485,  -485,   180,   325,  -485,  -485,  -485,   279,  -485,
     103,  -485,  -203,   303,  -249,  -228,   192,  -485,   -53,  -485,
    -485,  -485,   371,   478,  -485,  -485,  -485,  -485,  -485,  -485,
    -142,  -100,  -485,  -485,  -485,   -72,  -485,  -485,   281,  -484,
    -364,  -485,  -485,  -273,   -75,   184,  -485,  -485,  -485,  -202,
    -485
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    14,   153,   159,   372,   297,    15,    16,    34,
      35,    59,    17,    31,    32,   456,   457,   724,   725,   458,
     459,   460,   461,   462,   160,   161,    28,    29,    42,    43,
      44,    18,   147,   213,    76,    19,   298,   463,   347,   464,
     299,   465,   300,   466,   467,   468,   469,   470,   598,   471,
     472,   301,   302,   514,   712,   303,   304,   305,   437,   474,
     306,   307,   308,   515,   209,   338,   247,   206,   210,   211,
     327,   328,   150,   151,   617,   250,   353,   260,   216,   148,
      49,    21,   100,    63,    64,    22,    23,   108,    71,    24,
     251,   310,    99,   201,   202,   329,    25,    26,   311,   493,
     494,   312,   495,   496,   497,   498,   313,   371,   314,   765,
     315
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   200,   345,   473,   501,   256,   217,   419,   370,   421,
       2,   423,   320,   322,   324,     3,   263,    39,   436,   616,
     557,   341,   548,   517,    79,    80,   478,   105,   637,   203,
     626,   249,   558,   666,   331,   342,    55,   710,     4,    62,
       5,   164,     6,   214,     7,   149,   342,    72,   393,     8,
     350,   395,    40,   164,   207,     9,    45,   351,   342,    40,
     426,    10,   245,   380,   343,   510,   621,   207,    56,   225,
     208,   622,   638,   638,   257,   343,   102,   344,   165,   352,
     215,   427,   262,   255,    60,   599,    41,   343,   606,   199,
     265,    65,   638,    41,   381,   205,    11,   226,    52,   411,
     412,   733,   736,   383,   384,   425,   252,    27,   721,   253,
     433,    61,   200,    73,    74,    40,   214,   578,   200,   722,
     723,   744,   154,   155,   204,    33,   588,   200,   200,   200,
      65,   200,   200,   200,   200,    81,   518,   257,   697,    82,
     254,    83,    84,   649,    75,   332,   261,   624,   257,    41,
     602,   257,   625,    68,    30,   316,   317,   318,   200,   321,
     323,   325,   333,   516,    57,   734,    36,   248,   200,    12,
     622,   222,    85,   334,    58,    13,   638,    69,   638,   200,
     200,    52,    48,   387,   388,   555,   349,   236,   638,   383,
     384,   393,   237,   394,   395,   396,   354,   223,   397,    37,
     199,   638,   639,   640,   646,   340,   199,   366,   368,   220,
     221,   238,   620,    38,   684,   199,   199,   199,    51,   199,
     199,   199,   199,   488,   692,   499,   263,   743,    52,    50,
     236,   631,   428,   156,   701,   237,   708,   157,   580,   158,
      84,    53,   411,   412,   200,   729,   199,    54,    62,   667,
     714,    65,    66,   429,   238,   694,   199,    67,   200,   200,
     674,   200,   200,   702,   342,   239,   730,   199,   199,   387,
     388,    70,   434,   255,   716,   295,   556,   393,    77,   394,
     395,   396,   257,    78,   397,   258,   483,   484,   259,   486,
     487,   634,    88,   343,   240,   152,   506,   507,   241,    90,
      46,   575,    47,    91,   383,   384,    92,   350,   239,   504,
     622,   505,   628,   104,   351,   242,   200,   408,   409,   410,
     625,   706,   629,   101,   107,   200,   244,   200,   411,   412,
     109,   622,   199,   737,    93,   144,   352,   240,    94,   145,
     146,   241,   149,   152,   549,    56,   199,   199,   163,   199,
     199,   227,   212,   553,    95,    96,    97,    98,   242,   218,
     383,   384,   219,   230,   476,   228,   665,   511,   229,   244,
     231,   232,   233,   234,   671,   235,   473,   319,   337,   214,
     255,   339,   385,   386,   348,   678,   389,   356,   358,   390,
     391,   392,   393,   207,   394,   395,   396,   357,   359,   397,
     360,   398,   399,   361,   199,   309,   362,   379,   418,    12,
     200,   430,   414,   199,   481,   199,   431,   200,   519,   526,
     528,   545,   336,   400,   550,   401,   402,   403,   404,   405,
     551,   552,   711,   409,   410,   558,   583,   584,   635,   590,
     387,   388,   591,   411,   412,   641,   735,   592,   393,   597,
     394,   395,   396,   620,   623,   397,   217,   363,   364,   594,
     595,   369,   718,   383,   384,   596,   632,   618,   636,   373,
     374,   375,   376,   377,   378,   220,   642,   643,   672,   670,
     673,   679,   677,   686,   695,   691,   406,   407,   408,   409,
     410,   696,   699,   703,   704,   738,   750,   236,   199,   411,
     412,   763,   237,   771,   740,   199,   753,   200,   756,   772,
     777,   773,   778,   746,   508,    86,   162,    89,   767,   264,
      87,   238,   106,   236,   382,   438,   720,   475,   237,    20,
     554,   246,   346,   479,   224,   685,   759,   482,   330,   760,
     485,   477,   103,   387,   388,   680,   492,   238,   492,   355,
     700,   393,   502,   766,   395,   396,     0,     0,   397,     0,
       0,     0,     0,     0,     0,   520,   521,     0,     0,   522,
     523,   524,   525,     0,   527,   239,   529,   530,   531,   532,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,     0,   546,     0,   236,   199,     0,     0,     0,   237,
       0,   239,   411,   412,   240,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     574,     0,     0,     0,   579,   242,     0,   581,   582,     0,
     240,   243,   586,   589,   241,     0,   244,     0,   593,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   200,     0,
       0,   242,     0,   480,     0,     0,     0,   603,     0,     0,
       0,   609,   244,     0,     0,     0,     0,     0,     0,   200,
       0,   378,   239,     0,     0,     0,   761,     0,     0,     0,
     378,   383,   384,   630,   369,     0,     0,   633,   236,     0,
       0,     0,     0,   237,     0,   739,     0,   774,     0,     0,
       0,   240,     0,     0,     0,   241,     0,   415,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,     0,   757,     0,     0,     0,
       0,     0,     0,   244,     0,   648,   199,     0,     0,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   664,   383,   384,   668,   199,     0,   385,
     386,   387,   388,     0,     0,     0,   239,   675,     0,   393,
       0,   394,   395,   396,     0,     0,   397,     0,   398,   399,
     660,   664,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   688,   689,     0,   690,   240,     0,   693,     0,   241,
       0,     0,     0,   698,     0,   492,     0,   406,   407,   408,
     409,   410,     0,     0,     0,     0,   242,     0,   709,     0,
     411,   412,   416,     0,   715,     0,     0,   244,     0,     0,
       0,     0,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,     0,     0,   397,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   726,   727,     0,     0,     0,     0,     0,     0,     0,
       0,   732,     0,   400,     0,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,     0,     0,     0,     0,     0,
     742,     0,     0,   411,   412,     0,     0,     0,     0,     0,
     475,   255,     0,     0,   747,     0,     0,     0,     0,   749,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,   439,     0,     0,     0,     3,     0,   440,   441,
     442,     0,   443,     0,   266,   267,   268,   269,     0,     0,
       0,     0,     0,   770,     0,   444,   270,   445,   446,     0,
       0,   776,     0,     0,     0,     0,     0,   447,   271,     0,
       0,   448,   272,     0,   273,     0,     9,   274,   449,     0,
     450,   275,     0,     0,   451,   452,     0,     0,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,     0,     0,   276,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,     0,
       0,     0,     0,   292,   293,     0,     0,     0,     0,     0,
       0,     0,   294,     0,     0,    41,   454,   255,   455,   295,
     296,   439,     0,     0,     0,     3,     0,   440,   441,   442,
       0,   443,     0,   266,   267,   268,   269,     0,     0,     0,
       0,     0,     0,     0,   444,   270,   445,   446,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   271,     0,     0,
     448,   272,     0,   273,     0,     9,   274,   449,     0,   450,
     275,     0,     0,   451,   452,     0,     0,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,     0,     0,   276,   453,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
      40,   288,     0,     0,     0,   236,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   289,   290,   291,     0,     0,
       0,     0,   292,   293,     0,     0,     0,     0,     0,   238,
       0,   294,     0,     0,    41,   454,   255,   745,   295,   296,
     266,   267,   268,   269,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,   237,   271,     0,     0,     0,   272,     0,
     273,     0,     0,   274,     0,     0,     0,   275,     0,     0,
       0,     0,   238,   239,   169,   170,   171,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,   188,   189,   190,     0,     0,   193,   194,
     195,   196,   240,     0,   276,     0,   241,     0,   417,     0,
       0,     0,     0,     0,     0,     0,   277,   278,     0,     0,
       0,     0,     0,   242,     0,     0,   239,     0,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   489,   288,     0,
       0,     0,     0,     0,     0,   240,     0,     0,     0,   241,
       0,     0,   289,   290,   291,     0,     0,     0,     0,   292,
     293,     0,   266,   267,   268,   269,   242,   490,   491,     0,
       0,    41,     0,   255,   270,   295,   296,   244,     0,     0,
     236,     0,     0,     0,     0,   237,   271,     0,     0,     0,
     272,     0,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,   238,     0,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   188,   189,   190,     0,     0,
     193,   194,   195,   196,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   278,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,     0,   236,     0,     0,     0,     0,   237,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   489,
     288,     0,     0,     0,     0,     0,   238,   240,     0,     0,
       0,   241,     0,     0,   289,   290,   291,   266,   267,   268,
     269,   292,   293,     0,     0,     0,     0,     0,   242,   270,
     500,     0,     0,    41,     0,   255,     0,   295,   296,   244,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
     239,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,   240,
       0,   276,     0,   241,     0,   420,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
     242,     0,   576,     0,     0,     0,     0,   435,     0,     0,
       0,   244,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,   266,   267,   268,   269,   292,   293,     0,     0,
       0,     0,     0,     0,   270,   294,     0,     0,    41,   577,
     255,     0,   295,   296,     0,     0,   271,     0,     0,     0,
     272,     0,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   169,   170,   171,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   188,   189,   190,     0,     0,
     193,   194,   195,   196,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,   237,     0,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,    40,
     288,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   289,   290,   291,   266,   267,   268,
     269,   292,   293,     0,     0,     0,     0,     0,   512,   270,
     294,   513,     0,    41,     0,   255,     0,   295,   296,     0,
       0,   271,     0,     0,     0,   272,     0,   273,     0,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
     239,   169,   170,   171,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     188,   189,   190,     0,     0,   193,   194,   195,   196,   240,
       0,   276,     0,   241,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   277,   278,     0,     0,     0,     0,     0,
     242,     0,   587,     0,     0,     0,     0,   435,     0,     0,
       0,   244,     0,     0,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,    40,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   291,     0,     0,     0,     0,   292,   293,     0,   266,
     267,   268,   269,   607,     0,   294,     0,     0,    41,     0,
     255,   270,   295,   296,   236,     0,     0,     0,     0,   237,
       0,     0,     0,   271,     0,     0,     0,   272,     0,   273,
       0,     0,   274,     0,     0,     0,   275,     0,   238,     0,
       0,     0,     0,   169,   170,   171,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   188,   189,   190,     0,     0,   193,   194,   195,
     196,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   278,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,   237,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,    40,   288,     0,     0,
       0,   240,     0,   238,     0,   241,     0,   424,     0,   608,
       0,   289,   290,   291,   266,   267,   268,   269,   292,   293,
       0,     0,   242,     0,     0,     0,   270,   294,     0,     0,
      41,     0,   255,   244,   295,   296,     0,     0,   271,     0,
       0,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,     0,     0,     0,     0,     0,   239,   169,   170,
     171,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   188,   189,   190,
       0,     0,   193,   194,   195,   196,   240,     0,   276,     0,
     241,     0,   611,     0,     0,     0,     0,     0,     0,     0,
     277,   278,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    40,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   290,   291,     0,
       0,     0,     0,   292,   293,     0,   266,   267,   268,   269,
       0,   335,   294,     0,     0,    41,     0,   255,   270,   295,
     296,   236,     0,     0,     0,     0,   237,     0,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,   238,     0,     0,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,   239,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,   240,     0,
       0,     0,   241,     0,   612,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,   533,   242,
       0,     0,     0,   270,   294,     0,     0,    41,     0,   255,
     244,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,   237,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,   294,
       0,     0,    41,     0,   255,     0,   295,   296,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   239,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,   240,     0,
     276,     0,   241,     0,   614,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     244,   237,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,   547,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,   585,     0,     0,   239,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,   240,     0,   276,     0,   241,     0,   615,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   244,   237,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,   294,
       0,     0,    41,     0,   255,     0,   295,   296,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   239,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,   240,     0,
     276,     0,   241,     0,   647,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     244,   237,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,   605,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,   239,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,   240,     0,   276,     0,   241,     0,   707,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   244,   237,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,   294,
     619,     0,    41,     0,   255,     0,   295,   296,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   239,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,   240,     0,
     276,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,   242,
       0,     0,   728,     0,     0,     0,   236,     0,     0,     0,
     244,   237,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,   627,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,   239,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,   240,     0,   276,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   242,     0,     0,   768,     0,     0,
       0,   681,     0,     0,     0,   244,     0,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,   294,
       0,     0,    41,     0,   255,     0,   295,   296,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   682,     0,     0,     0,
       0,     0,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,     0,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,   237,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,   266,   267,   268,   269,
     292,   293,     0,     0,     0,     0,     0,     0,   270,   294,
     713,     0,    41,     0,   255,     0,   295,   296,     0,     0,
     271,     0,     0,     0,   272,     0,   273,     0,     0,   274,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   239,
     169,   170,   171,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   188,
     189,   190,     0,     0,   193,   194,   195,   196,   240,     0,
     276,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   278,     0,     0,     0,     0,     0,   242,
       0,     0,   779,     0,     0,     0,   236,     0,     0,     0,
     244,   237,     0,     0,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,   288,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   266,   267,   268,   269,   292,   293,     0,     0,     0,
       0,     0,     0,   270,   294,   731,     0,    41,     0,   255,
       0,   295,   296,     0,     0,   271,     0,     0,     0,   272,
       0,   273,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,   239,   169,   170,   171,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   188,   189,   190,     0,     0,   193,
     194,   195,   196,   240,     0,   276,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   242,     0,   741,     0,     0,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,   279,
     280,   281,   282,   283,   284,   285,   286,   287,    40,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   289,   290,   291,     0,     0,     0,     0,
     292,   293,     0,   266,   267,   268,   269,   748,     0,   294,
       0,     0,    41,     0,   255,   270,   295,   296,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,   272,     0,   273,     0,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   169,   170,   171,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   188,   189,   190,     0,
       0,   193,   194,   195,   196,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
     278,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   279,   280,   281,   282,   283,   284,   285,   286,   287,
      40,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,   290,   291,     0,     0,
       0,     0,   292,   293,     0,   266,   267,   268,   269,   769,
       0,   294,     0,     0,    41,     0,   255,   270,   295,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   188,   189,
     190,     0,     0,   193,   194,   195,   196,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   279,   280,   281,   282,   283,   284,   285,
     286,   287,    40,   288,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,   290,   291,
     266,   267,   268,   269,   292,   293,     0,     0,     0,     0,
       0,     0,   270,   294,     0,     0,    41,     0,   255,     0,
     295,   296,     0,     0,   271,     0,     0,     0,   272,     0,
     273,     0,     0,   274,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,   169,   170,   171,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,   188,   189,   190,     0,     0,   193,   194,
     195,   196,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   279,   280,
     281,   282,   283,   284,   285,   286,   287,    40,   288,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   289,   290,   291,     0,     0,   167,     0,   292,
     293,     0,     0,     0,     0,     0,     0,     0,   294,     0,
       0,    41,   168,   255,     0,   295,   296,     0,     0,     0,
       0,     0,     0,     0,     0,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   367,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   384,     0,   166,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       0,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,    41,     0,   559,   560,   561,   562,   563,   564,
     565,   566,   385,   386,   387,   388,   389,   567,   568,   390,
     391,   392,   393,   569,   394,   395,   396,   383,   384,   397,
     570,   398,   399,   571,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
       0,     0,   573,   400,     0,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,     0,     0,     0,     0,     0,
       0,     0,     0,   411,   412,     0,     0,     0,    41,     0,
     604,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,   560,   561,
     562,   563,   564,   565,   566,   385,   386,   387,   388,   389,
     567,   568,   390,   391,   392,   393,   569,   394,   395,   396,
     383,   384,   397,   570,   398,   399,   571,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   573,   400,     0,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,     0,     0,
       0,     0,     0,     0,     0,     0,   411,   412,     0,     0,
       0,     0,     0,   669,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,   560,   561,   562,   563,   564,   565,   566,   385,   386,
     387,   388,   389,   567,   568,   390,   391,   392,   393,   569,
     394,   395,   396,   383,   384,   397,   570,   398,   399,   571,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,   400,
       0,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,     0,     0,     0,     0,     0,     0,     0,     0,   411,
     412,     0,     0,     0,     0,     0,   676,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,   560,   561,   562,   563,   564,   565,
     566,   385,   386,   387,   388,   389,   567,   568,   390,   391,
     392,   393,   569,   394,   395,   396,   383,   384,   397,   570,
     398,   399,   571,   572,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,   400,     0,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,     0,     0,     0,     0,     0,     0,
       0,     0,   411,   412,     0,     0,     0,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,   560,   561,   562,
     563,   564,   565,   566,   385,   386,   387,   388,   389,   567,
     568,   390,   391,   392,   393,   569,   394,   395,   396,   383,
     384,   397,   570,   398,   399,   571,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   400,     0,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,     0,     0,     0,
       0,     0,     0,     0,     0,   411,   412,     0,     0,     0,
       0,     0,   717,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   384,     0,     0,     0,     0,     0,   559,
     560,   561,   562,   563,   564,   565,   566,   385,   386,   387,
     388,   389,   567,   568,   390,   391,   392,   393,   569,   394,
     395,   396,     0,     0,   397,   570,   398,   399,   571,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   400,     0,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
       0,   383,   384,     0,     0,     0,     0,     0,   411,   412,
     385,   386,   387,   388,   389,   719,     0,   390,   391,   392,
     393,     0,   394,   395,   396,     0,     0,   397,     0,   398,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,     0,   383,   384,     0,     0,     0,     0,
       0,   411,   412,     0,     0,     0,     0,     0,   413,   385,
     386,   387,   388,   389,     0,     0,   390,   391,   392,   393,
       0,   394,   395,   396,     0,     0,   397,     0,   398,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   383,   384,     0,     0,     0,     0,     0,     0,
     411,   412,   385,   386,   387,   388,   389,   764,     0,   390,
     391,   392,   393,     0,   394,   395,   396,     0,     0,   397,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,     0,     0,     0,   383,   384,
       0,     0,     0,   411,   412,     0,     0,   509,     0,     0,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,     0,     0,   397,     0,   398,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,     0,   383,   384,     0,     0,     0,     0,
       0,   411,   412,     0,     0,   610,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,   393,     0,   394,   395,
     396,     0,     0,   397,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,     0,
     383,   384,     0,     0,     0,     0,     0,   411,   412,     0,
       0,   613,   385,   386,   387,   388,   389,     0,     0,   390,
     391,   392,   393,     0,   394,   395,   396,     0,     0,   397,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,     0,   383,   384,     0,     0,
       0,     0,     0,   411,   412,     0,     0,   687,   385,   386,
     387,   388,   389,     0,     0,   390,   391,   392,   393,     0,
     394,   395,   396,     0,     0,   397,     0,   398,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,     0,   383,   384,     0,     0,     0,     0,     0,   411,
     412,     0,     0,   751,   385,   386,   387,   388,   389,     0,
       0,   390,   391,   392,   393,     0,   394,   395,   396,     0,
       0,   397,     0,   398,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,     0,     0,     0,
       0,     0,     0,     0,     0,   411,   412,     0,     0,   762,
     385,   386,   387,   388,   389,     0,     0,   390,   391,   392,
     393,     0,   394,   395,   396,     0,     0,   397,     0,   398,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,     0,   167,     0,
       0,   400,     0,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   168,     0,     0,     0,     0,     0,     0,
       0,   411,   412,     0,     0,   775,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,     0,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   168,     0,     0,
     279,   280,   281,     0,     0,     0,     0,     0,     0,    40,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   146,   383,   384,    41,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   383,   384,
       0,    41,     0,   385,   386,   387,   388,   389,     0,     0,
     390,   391,   392,   393,     0,   394,   395,   396,     0,     0,
     397,     0,   398,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,     0,     0,   400,     0,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,     0,     0,     0,     0,
       0,     0,     0,     0,   411,   412,   432,     0,     0,     0,
     383,   384,     0,     0,     0,    41,   385,   386,   387,   388,
     389,     0,     0,   390,   391,   392,   393,     0,   394,   395,
     396,     0,     0,   397,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,     0,
       0,     0,     0,     0,     0,     0,     0,   411,   412,   645,
     559,   560,   561,   562,   563,   564,   565,   566,   385,   386,
     387,   388,   389,   567,   568,   390,   391,   392,   393,   569,
     394,   395,   396,  -193,     0,   397,   570,   398,   399,   571,
     572,   383,   384,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,   400,
       0,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,     0,     0,     0,     0,     0,     0,     0,     0,   411,
     412,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,   560,   561,   562,   563,   564,   565,   566,   385,
     386,   387,   388,   389,   567,   568,   390,   391,   392,   393,
     600,   394,   395,   396,     0,     0,   397,   570,   398,   399,
     571,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,     0,   601,
     400,     0,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,     0,     0,     0,   385,   386,   387,   388,   389,
     411,   412,   390,   391,   392,   393,     0,   394,   395,   396,
       0,     0,   397,     0,   398,   399,     0,     0,   503,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
     384,     0,     0,     0,     0,     0,   400,     0,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,     0,     0,
       0,   385,   386,   387,   388,   389,   411,   412,   390,   391,
     392,   393,     0,   394,   395,   396,     0,     0,   397,     0,
     398,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,   384,     0,     0,     0,
       0,     0,   400,   644,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,     0,     0,     0,   385,   386,   387,
     388,   389,   411,   412,   390,   391,   392,   393,     0,   394,
     395,   396,     0,     0,   397,     0,   398,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,   384,     0,     0,   705,     0,     0,   400,     0,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
       0,     0,     0,   385,   386,   387,   388,   389,   411,   412,
     390,   391,   392,   393,     0,   394,   395,   396,     0,     0,
     397,     0,   398,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,   384,     0,
       0,     0,     0,     0,   400,     0,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,     0,     0,     0,   385,
     386,   387,   388,   389,   411,   412,   390,   391,   392,   393,
       0,   394,   395,   396,     0,     0,   397,     0,   398,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,     0,     0,     0,   385,   386,   387,   388,   389,
     411,   412,   390,   391,   392,   393,     0,   394,   395,   396,
       0,     0,   397,     0,   398,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   384,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,   405,   406,   407,   408,   409,   410,     0,     0,
       0,   385,   386,   387,   388,   389,   411,   412,   390,   391,
     392,   393,     0,   394,   395,   396,     0,     0,   397,     0,
     398,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,   406,
     407,   408,   409,   410,   385,   386,   387,   388,   389,     0,
       0,   390,   411,   412,   393,     0,   394,   395,   396,     0,
       0,   397,     0,   398,   399,     0,     0,     0,     0,     0,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   128,   129,   130,
     131,     0,     0,     0,     0,   411,   412,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,     0,     0,   142,   143
};

static const yytype_int16 yycheck[] =
{
      12,   101,   251,   347,   368,   213,   148,   320,   281,   322,
       0,   324,   231,   232,   233,     5,   219,    32,   345,    18,
     435,   249,   414,    17,    13,    14,   353,    67,   512,   101,
      18,    45,   112,    18,    31,   108,    20,   120,    28,   130,
      30,   130,    32,   130,    34,   130,   108,    33,   107,    39,
     108,   110,   130,   130,   159,    45,    68,   115,   108,   130,
     144,    51,   167,   130,   137,   143,   160,   159,    52,   137,
     162,   165,   136,   136,   136,   137,   167,   139,   167,   137,
     167,   165,   167,   166,   139,   165,   164,   137,   480,   101,
     167,   164,   136,   164,   161,   107,    86,   165,   136,   158,
     159,   165,   165,    19,    20,   139,   162,   130,    10,   165,
     338,   166,   212,    99,   100,   130,   130,   444,   218,    21,
      22,   165,    13,    14,   162,   130,   453,   227,   228,   229,
     164,   231,   232,   233,   234,   124,   130,   136,   622,   128,
     212,   130,   131,   558,   130,   142,   218,   160,   136,   164,
     477,   136,   165,   136,   124,   227,   228,   229,   258,   231,
     232,   233,   159,   382,   148,   160,   130,   207,   268,   159,
     165,   136,   161,   170,   158,   165,   136,   160,   136,   279,
     280,   136,    56,    99,   100,   434,   258,    31,   136,    19,
      20,   107,    36,   109,   110,   111,   268,   162,   114,   166,
     212,   136,   162,   516,   162,   160,   218,   279,   280,   132,
     133,    55,   136,   130,   162,   227,   228,   229,   137,   231,
     232,   233,   234,   365,   616,   367,   429,   162,   136,   130,
      31,   504,   144,   124,   626,    36,   160,   128,   446,   130,
     131,   139,   158,   159,   344,   144,   258,   136,   130,   576,
     642,   164,   130,   165,    55,   619,   268,   161,   358,   359,
     587,   361,   362,   627,   108,   109,   165,   279,   280,    99,
     100,   130,   344,   166,   666,   168,   169,   107,   166,   109,
     110,   111,   136,   166,   114,   139,   358,   359,   142,   361,
     362,   510,   124,   137,   138,   132,   133,   134,   142,   130,
       4,   443,     6,   130,    19,    20,   130,   108,   109,   165,
     165,   167,   167,   130,   115,   159,   416,   147,   148,   149,
     165,   634,   167,   137,   139,   425,   170,   427,   158,   159,
     166,   165,   344,   167,    61,    33,   137,   138,    65,    33,
     161,   142,   130,   132,   416,    52,   358,   359,   166,   361,
     362,   143,   139,   425,    81,    82,    83,    84,   159,   139,
      19,    20,   165,   161,   165,   143,   574,   379,   143,   170,
     143,   143,   143,   143,   582,   143,   720,   130,    42,   130,
     166,   165,    97,    98,   130,   593,   101,   161,   143,   104,
     105,   106,   107,   159,   109,   110,   111,   161,   143,   114,
     161,   116,   117,   143,   416,   225,   143,   169,   162,   159,
     510,   142,   161,   425,   130,   427,   160,   517,   130,   130,
     130,   130,   242,   138,   144,   140,   141,   142,   143,   144,
     144,   144,   640,   148,   149,   112,   165,   124,   510,    35,
      99,   100,   165,   158,   159,   517,   695,   165,   107,    18,
     109,   110,   111,   136,   136,   114,   598,   277,   278,   165,
     165,   281,   670,    19,    20,   165,   167,   160,   130,   289,
     290,   291,   292,   293,   294,   132,   161,   130,   139,    12,
     124,    38,   166,   130,   130,   161,   145,   146,   147,   148,
     149,   160,   160,   160,   160,   144,   130,    31,   510,   158,
     159,   144,    36,    48,   162,   517,   160,   607,   160,   160,
     160,   167,   160,   721,   372,    51,    90,    54,   760,   222,
      52,    55,    68,    31,   303,   345,   677,   347,    36,     1,
     427,   206,   253,   353,   163,   607,   744,   357,   235,   747,
     360,   349,    64,    99,   100,   598,   366,    55,   368,   268,
     625,   107,   368,   755,   110,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,   385,   386,    -1,    -1,   389,
     390,   391,   392,    -1,   394,   109,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,    -1,   412,    -1,    31,   607,    -1,    -1,    -1,    36,
      -1,   109,   158,   159,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
     440,    -1,    -1,    -1,   444,   159,    -1,   447,   448,    -1,
     138,   165,   452,   453,   142,    -1,   170,    -1,   458,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   748,    -1,
      -1,   159,    -1,   161,    -1,    -1,    -1,   477,    -1,    -1,
      -1,   481,   170,    -1,    -1,    -1,    -1,    -1,    -1,   769,
      -1,   491,   109,    -1,    -1,    -1,   748,    -1,    -1,    -1,
     500,    19,    20,   503,   504,    -1,    -1,   507,    31,    -1,
      -1,    -1,    -1,    36,    -1,   707,    -1,   769,    -1,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   144,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,   738,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   555,   748,    -1,    -1,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,    19,    20,   576,   769,    -1,    97,
      98,    99,   100,    -1,    -1,    -1,   109,   587,    -1,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
     600,   601,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   611,   612,    -1,   614,   138,    -1,   617,    -1,   142,
      -1,    -1,    -1,   623,    -1,   625,    -1,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    -1,   159,    -1,   638,    -1,
     158,   159,   165,    -1,   644,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   681,   682,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   691,    -1,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
     710,    -1,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,
     720,   166,    -1,    -1,   724,    -1,    -1,    -1,    -1,   729,
      -1,    -1,    -1,   733,    -1,   735,   736,    -1,    -1,    -1,
      -1,   741,     1,    -1,    -1,    -1,     5,    -1,     7,     8,
       9,    -1,    11,    -1,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,   763,    -1,    24,    25,    26,    27,    -1,
      -1,   771,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    47,    -1,
      49,    50,    -1,    -1,    53,    54,    -1,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,   167,   168,
     169,     1,    -1,    -1,    -1,     5,    -1,     7,     8,     9,
      -1,    11,    -1,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    47,    -1,    49,
      50,    -1,    -1,    53,    54,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,    -1,    55,
      -1,   161,    -1,    -1,   164,   165,   166,   167,   168,   169,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    55,   109,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,   138,    -1,    87,    -1,   142,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,   142,
      -1,    -1,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
     153,    -1,    13,    14,    15,    16,   159,   160,   161,    -1,
      -1,   164,    -1,   166,    25,   168,   169,   170,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    55,   138,    -1,    -1,
      -1,   142,    -1,    -1,   145,   146,   147,    13,    14,    15,
      16,   152,   153,    -1,    -1,    -1,    -1,    -1,   159,    25,
     161,    -1,    -1,   164,    -1,   166,    -1,   168,   169,   170,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
     109,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,   138,
      -1,    87,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   170,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,    13,    14,    15,    16,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   161,    -1,    -1,   164,   165,
     166,    -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,    13,    14,    15,
      16,   152,   153,    -1,    -1,    -1,    -1,    -1,   159,    25,
     161,   162,    -1,   164,    -1,   166,    -1,   168,   169,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
     109,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    -1,    -1,    81,    82,    83,    84,   138,
      -1,    87,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   170,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,   153,    -1,    13,
      14,    15,    16,    17,    -1,   161,    -1,    -1,   164,    -1,
     166,    25,   168,   169,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    55,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
      -1,   138,    -1,    55,    -1,   142,    -1,   144,    -1,   143,
      -1,   145,   146,   147,    13,    14,    15,    16,   152,   153,
      -1,    -1,   159,    -1,    -1,    -1,    25,   161,    -1,    -1,
     164,    -1,   166,   170,   168,   169,    -1,    -1,    37,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,   109,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    82,    83,    84,   138,    -1,    87,    -1,
     142,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,   153,    -1,    13,    14,    15,    16,
      -1,   160,   161,    -1,    -1,   164,    -1,   166,    25,   168,
     169,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    55,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,   138,    -1,
      -1,    -1,   142,    -1,   144,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    20,   159,
      -1,    -1,    -1,    25,   161,    -1,    -1,   164,    -1,   166,
     170,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    25,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     170,    36,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   161,   162,    -1,   164,    -1,   166,
      -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,   109,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   138,    -1,    87,    -1,   142,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   170,    36,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    25,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     170,    36,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   161,   162,    -1,   164,    -1,   166,
      -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   109,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   138,    -1,    87,    -1,   142,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   170,    36,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    25,   161,
     162,    -1,   164,    -1,   166,    -1,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     170,    36,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   161,   162,    -1,   164,    -1,   166,
      -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   109,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   138,    -1,    87,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,    -1,    -1,
      -1,   113,    -1,    -1,    -1,   170,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    25,   161,
      -1,    -1,   164,    -1,   166,    -1,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,   126,
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
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    13,    14,    15,    16,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    25,   161,
     162,    -1,   164,    -1,   166,    -1,   168,   169,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,   109,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    82,    83,    84,   138,    -1,
      87,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     170,    36,    -1,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,    13,    14,    15,    16,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    25,   161,   162,    -1,   164,    -1,   166,
      -1,   168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,   109,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    -1,    -1,    81,
      82,    83,    84,   138,    -1,    87,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,   159,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,   153,    -1,    13,    14,    15,    16,    17,    -1,   161,
      -1,    -1,   164,    -1,   166,    25,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      13,    14,    15,    16,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   161,    -1,    -1,   164,    -1,   166,    -1,
     168,   169,    -1,    -1,    37,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,   145,   146,   147,    -1,    -1,    29,    -1,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,    44,   166,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,   164,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    19,    20,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,    -1,    -1,    -1,   164,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      19,    20,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    19,    20,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    19,    20,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    19,
      20,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    19,    20,    -1,    -1,    -1,    -1,    -1,   158,   159,
      97,    98,    99,   100,   101,   165,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   165,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    97,    98,    99,   100,   101,   165,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,   158,   159,    -1,    -1,   162,    -1,    -1,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    -1,   162,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      19,    20,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,
      -1,   162,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,   158,   159,    -1,    -1,   162,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,   158,
     159,    -1,    -1,   162,    97,    98,    99,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,   162,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    -1,   162,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
     121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,   161,    19,    20,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    19,    20,
      -1,   164,    -1,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      19,    20,    -1,    -1,    -1,   164,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     158,   159,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   158,   159,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   158,   159,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    -1,    -1,   135,    -1,    -1,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   158,   159,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,
      -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   158,   159,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     158,   159,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   158,   159,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,   148,   149,    97,    98,    99,   100,   101,    -1,
      -1,   104,   158,   159,   107,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     143,   144,   145,   146,   147,   148,   149,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    -1,   152,   153
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   172,     0,     5,    28,    30,    32,    34,    39,    45,
      51,    86,   159,   165,   173,   178,   179,   183,   202,   206,
     220,   252,   256,   257,   260,   267,   268,   130,   197,   198,
     124,   184,   185,   130,   180,   181,   130,   166,   130,    32,
     130,   164,   199,   200,   201,   211,     4,     6,    56,   251,
     130,   137,   136,   139,   136,    20,    52,   148,   158,   182,
     139,   166,   130,   254,   255,   164,   130,   161,   136,   160,
     130,   259,    33,    99,   100,   130,   205,   166,   166,    13,
      14,   124,   128,   130,   131,   161,   175,   197,   124,   185,
     130,   130,   130,    61,    65,    81,    82,    83,    84,   263,
     253,   137,   167,   254,   130,   198,   200,   139,   258,   166,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   116,   117,
     118,   119,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   152,   153,    33,    33,   161,   203,   250,   130,
     243,   244,   132,   174,    13,    14,   124,   128,   130,   175,
     195,   196,   182,   166,   130,   167,    23,    29,    44,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   211,
     262,   264,   265,   266,   162,   211,   238,   159,   162,   235,
     239,   240,   139,   204,   130,   167,   249,   261,   139,   165,
     132,   133,   136,   162,   253,   137,   165,   143,   143,   143,
     161,   143,   143,   143,   143,   143,    31,    36,    55,   109,
     138,   142,   159,   165,   170,   167,   235,   237,   198,    45,
     246,   261,   162,   165,   266,   166,   207,   136,   139,   142,
     248,   266,   167,   243,   195,   167,    13,    14,    15,    16,
      25,    37,    41,    43,    46,    50,    87,    99,   100,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   131,   145,
     146,   147,   152,   153,   161,   168,   169,   177,   207,   211,
     213,   222,   223,   226,   227,   228,   231,   232,   233,   234,
     262,   269,   272,   277,   279,   281,   266,   266,   266,   130,
     203,   266,   203,   266,   203,   266,   130,   241,   242,   266,
     244,    31,   142,   159,   170,   160,   234,    42,   236,   165,
     160,   246,   108,   137,   139,   245,   239,   209,   130,   266,
     108,   115,   137,   247,   266,   269,   161,   161,   143,   143,
     161,   143,   143,   234,   234,    11,   266,    11,   266,   234,
     274,   278,   176,   234,   234,   234,   234,   234,   234,   169,
     130,   161,   202,    19,    20,    97,    98,    99,   100,   101,
     104,   105,   106,   107,   109,   110,   111,   114,   116,   117,
     138,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   158,   159,   165,   161,   144,   165,   144,   162,   204,
     144,   204,   144,   204,   144,   139,   144,   165,   144,   165,
     142,   160,   160,   246,   266,   113,   210,   229,   234,     1,
       7,     8,     9,    11,    24,    26,    27,    36,    40,    47,
      49,    53,    54,    88,   165,   167,   186,   187,   190,   191,
     192,   193,   194,   208,   210,   212,   214,   215,   216,   217,
     218,   220,   221,   229,   230,   234,   165,   247,   210,   234,
     161,   130,   234,   266,   266,   234,   266,   266,   261,   130,
     160,   161,   234,   270,   271,   273,   274,   275,   276,   261,
     161,   271,   276,   120,   165,   167,   133,   134,   174,   162,
     143,   211,   159,   162,   224,   234,   203,    17,   130,   130,
     234,   234,   234,   234,   234,   234,   130,   234,   130,   234,
     234,   234,   234,    20,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   130,   234,   162,   224,   266,
     144,   144,   144,   266,   241,   245,   169,   227,   112,    89,
      90,    91,    92,    93,    94,    95,    96,   102,   103,   108,
     115,   118,   119,   137,   234,   261,   108,   165,   210,   234,
     207,   234,   234,   165,   124,    53,   234,   108,   210,   234,
      35,   165,   165,   234,   165,   165,   165,    18,   219,   165,
     108,   137,   210,   234,   165,   162,   224,    17,   143,   234,
     162,   144,   144,   162,   144,   144,    18,   245,   160,   162,
     136,   160,   165,   136,   160,   165,    18,   162,   167,   167,
     234,   274,   167,   234,   203,   266,   130,   270,   136,   162,
     204,   266,   161,   130,   139,   160,   162,   144,   234,   227,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   207,    18,   210,   234,   165,
      12,   207,   139,   124,   210,   234,   165,   166,   207,    38,
     249,   113,   113,   165,   162,   266,   130,   162,   234,   234,
     234,   161,   224,   234,   271,   130,   160,   270,   234,   160,
     275,   224,   271,   160,   160,   135,   204,   144,   160,   234,
     120,   207,   225,   162,   224,   234,   224,   165,   207,   165,
     209,    10,    21,    22,   188,   189,   234,   234,   162,   144,
     165,   162,   234,   165,   160,   245,   165,   167,   144,   211,
     162,   108,   234,   162,   165,   167,   207,   234,    17,   234,
     130,   162,   234,   160,   234,   234,   160,   211,   234,   207,
     207,   266,   162,   144,   165,   280,   280,   189,   162,    17,
     234,    48,   160,   167,   266,   162,   234,   160,   160,   162
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   173,   174,   174,   175,   176,   176,
     176,   177,   178,   179,   180,   180,   180,   181,   181,   182,
     182,   183,   184,   184,   185,   185,   186,   187,   187,   188,
     188,   189,   189,   189,   190,   190,   191,   192,   193,   194,
     195,   195,   195,   195,   195,   195,   196,   196,   197,   197,
     197,   197,   197,   197,   197,   197,   198,   198,   199,   199,
     200,   200,   201,   201,   202,   202,   203,   203,   203,   204,
     204,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   206,   207,
     207,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   209,
     209,   209,   210,   210,   211,   211,   211,   212,   213,   213,
     213,   213,   214,   215,   216,   216,   216,   216,   216,   217,
     217,   217,   217,   218,   219,   219,   220,   220,   221,   222,
     222,   222,   223,   223,   223,   223,   223,   223,   224,   224,
     225,   225,   225,   226,   226,   227,   227,   228,   228,   228,
     228,   228,   228,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   230,
     230,   231,   232,   232,   233,   233,   233,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   235,
     235,   236,   236,   237,   238,   238,   239,   239,   240,   240,
     241,   241,   242,   242,   243,   244,   244,   245,   245,   246,
     246,   246,   246,   246,   247,   247,   247,   248,   248,   249,
     249,   249,   249,   249,   250,   250,   251,   251,   252,   253,
     253,   253,   254,   255,   255,   256,   257,   257,   258,   258,
     259,   260,   261,   261,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   263,   263,
     263,   263,   263,   263,   264,   265,   265,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   267,   268,   269,
     269,   269,   269,   270,   270,   271,   271,   272,   272,   272,
     272,   272,   273,   273,   274,   275,   275,   276,   276,   277,
     277,   278,   278,   279,   280,   280,   281,   281
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
       3,     4,     3,     3,     1,     3,     4,     3,     4,     2,
       4,     4,     6,     7,     3,     5,     4,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       3,     0,     1,     3,     0,     3,     2,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     2,     0,     1,     5,     0,
       3,     5,     4,     1,     2,     4,     5,     7,     0,     2,
       2,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     1,     1,
       4,     3,     4,     2,     3,     2,     4,     3,     2,     2,
       3,     2,     2,     4,     6,     4,     1,     4,     5,     1,
       4,     5,     1,     4,     5,     4,     4,     1,     6,     1,
       1,     1,     1,     3,     5,     1,     3,     5,     4,     7,
       5,     7,     1,     3,     3,     1,     1,     1,     3,     5,
       5,     1,     3,     4,     0,     3,    10,    10
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
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3066 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 174: /* character_sequence  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3072 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 175: /* string_constant  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3078 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 176: /* string_builder_body  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3084 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 177: /* string_builder  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3090 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 180: /* require_module_name  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3096 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expression_label  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3102 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 187: /* expression_goto  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3108 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 189: /* expression_else  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3114 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 191: /* expression_if_then_else  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3120 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 192: /* expression_for_loop  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3126 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 193: /* expression_while_loop  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3132 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 194: /* expression_with  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3138 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 195: /* annotation_argument_value  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3144 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* annotation_argument_value_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3150 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* annotation_argument  */
#line 110 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3156 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 198: /* annotation_argument_list  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3162 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* annotation_declaration_name  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3168 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 200: /* annotation_declaration  */
#line 112 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3174 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* annotation_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3180 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* optional_annotation_list  */
#line 113 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3186 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* optional_function_argument_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3192 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* optional_function_type  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3198 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* function_name  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3204 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* expression_block  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3210 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* expression_any  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3216 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* expressions  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3222 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* expr_pipe  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3228 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* name_in_namespace  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3234 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* expression_delete  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3240 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* expr_new  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3246 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* expression_break  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3252 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* expression_continue  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3258 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* expression_return  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3264 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 217: /* expression_yield  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3270 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 218: /* expression_try_catch  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3276 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expression_let  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3282 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expr_cast  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3288 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_type_info  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3294 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* expr_list  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3300 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* block_or_simple_block  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3306 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expr_block  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3312 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expr_numeric_const  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3318 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expr_assign  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3324 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* expr_assign_pipe  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3330 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* expr_named_call  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3336 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 232: /* expr_method_call  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3342 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* func_addr_expr  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3348 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* expr  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3354 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 235: /* optional_field_annotation  */
#line 111 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3360 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 237: /* structure_variable_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3366 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 238: /* struct_variable_declaration_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3372 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 239: /* function_argument_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3378 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 240: /* function_argument_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3384 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* tuple_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3390 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 242: /* tuple_type_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3396 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 243: /* variant_type  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3402 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 244: /* variant_type_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3408 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 246: /* variable_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3414 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* let_variable_declaration  */
#line 106 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3420 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 250: /* global_variable_declaration_list  */
#line 107 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3426 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* enum_list  */
#line 115 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3432 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* optional_structure_parent  */
#line 104 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3438 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 261: /* variable_name_list  */
#line 105 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3444 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 264: /* structure_type_declaration  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3450 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* auto_type_declaration  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3456 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 266: /* type_declaration  */
#line 108 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3462 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 269: /* make_decl  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3468 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 270: /* make_struct_fields  */
#line 114 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3474 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 271: /* make_struct_dim  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3480 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 272: /* make_struct_decl  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3486 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 273: /* make_tuple  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3492 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 274: /* make_map_tuple  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3498 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 275: /* make_any_tuple  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3504 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 276: /* make_dim  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3510 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 277: /* make_dim_decl  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3516 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 278: /* make_table  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3522 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 279: /* make_table_decl  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3528 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 280: /* array_comprehension_where  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3534 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 281: /* array_comprehension  */
#line 109 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3540 "ds_parser.cpp" /* yacc.c:1254  */
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
        case 14:
#line 399 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3836 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 15:
#line 411 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3842 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 412 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3848 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 416 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 3854 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 420 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 3863 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 424 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_shared<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 3877 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 433 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 3887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 441 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 457 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3928 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 3936 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 485 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 491 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 3958 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 500 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 3984 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 28:
#line 521 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 544 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 545 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4021 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 558 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 561 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4037 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 36:
#line 567 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 37:
#line 573 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 38:
#line 576 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4061 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 582 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4067 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 583 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4073 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 587 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4079 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 588 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4085 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 589 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4096 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 598 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 599 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 603 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 612 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->iterators = *(yyvsp[-4].pNameList);
        delete (yyvsp[-4].pNameList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 624 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 634 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 643 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4162 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 644 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4174 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4180 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 647 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4186 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 648 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 652 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4202 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 657 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4212 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 665 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4218 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4224 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 667 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4230 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 668 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4236 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 669 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4242 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 670 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4248 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 671 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 672 "ds_parser.ypp" /* yacc.c:1660  */
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4262 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 678 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4280 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 691 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 707 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4304 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 708 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 712 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        delete (yyvsp[0].s);
    }
#line 4322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 719 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann->shared_from_this();
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 731 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4345 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 735 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 742 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 743 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 747 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4372 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4378 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4384 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4393 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4402 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 764 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s); 
	}
#line 4411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 768 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 769 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4423 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4429 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4441 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4447 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4459 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 778 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 779 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 780 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4495 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4507 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 784 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4513 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 785 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 786 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 787 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 788 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4537 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 789 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4543 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 790 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4549 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 791 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4555 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 792 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 793 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 794 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 795 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4579 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4585 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 797 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4591 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 4597 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 4603 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 800 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 4609 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 801 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 4615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 802 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 4621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 803 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 4627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 808 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 883 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4745 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4751 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 902 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4781 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 904 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4793 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4799 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 906 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4805 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4811 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 908 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4817 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4823 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 910 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 914 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(), yylloc.first_column,yylloc.first_line);
    }
#line 4838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 918 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 4849 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 924 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 4857 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 930 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4873 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 941 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 4881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 947 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 948 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4903 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 959 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 4909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 963 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 4925 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 4933 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 975 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 4942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 979 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4950 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 985 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 4956 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 989 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 4962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 993 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 4970 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 4978 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 4988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5006 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1015 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5014 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1018 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5024 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1023 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1026 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5042 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1034 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1040 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5056 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1041 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1045 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1046 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1050 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5110 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1084 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 5118 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1087 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5128 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-4])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1100 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1104 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5157 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1109 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5168 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1115 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-5])),*(yyvsp[-3].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)));
            delete (yyvsp[-3].s);
    }
#line 5177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1119 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-8])),*(yyvsp[-6].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-4].s));
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1124 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-10])),*(yyvsp[-8].s),TypeDeclPtr((yyvsp[-1].pTypeDecl)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
#line 5198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1133 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5214 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1142 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5220 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1143 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1150 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_shared<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1161 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false;   /* block */  }
#line 5251 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1162 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true;    /* lambda */ }
#line 5257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1166 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_shared<TypeDecl>(Type::autoinfer);
    }
#line 5267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1172 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5323 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1226 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5329 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5335 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1228 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5341 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1229 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5347 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1230 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5353 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1231 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5359 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1235 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5365 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1236 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5371 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1237 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5377 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1238 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5383 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1239 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1240 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5395 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5401 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1242 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5407 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1243 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5413 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1245 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1246 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1247 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1248 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5443 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1249 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5449 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5455 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1254 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5461 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1255 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5467 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1259 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5489 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5501 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1285 "ds_parser.ypp" /* yacc.c:1660  */
    { 
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
	}
#line 5510 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1289 "ds_parser.ypp" /* yacc.c:1660  */
    {
		auto expr = new ExprAddr(tokAt((yylsp[-4])),*(yyvsp[0].s)); 
		delete (yyvsp[0].s); 
        expr->funcType = make_shared<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5522 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1296 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1310 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1311 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1312 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1313 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1314 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1315 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1316 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1317 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1318 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1319 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5598 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1320 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5604 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1321 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5610 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1322 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5616 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1323 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5622 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1324 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5628 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1325 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1326 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5640 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1327 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1328 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5652 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1329 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5658 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1330 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5664 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1331 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1332 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1333 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1334 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1335 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5694 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1336 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5700 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1337 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5706 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1338 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5712 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1339 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5718 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1340 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5724 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1341 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5730 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1342 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5736 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1343 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5742 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1344 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5748 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1345 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5754 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1346 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5760 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1347 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5766 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1348 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 5775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1352 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 5784 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1356 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 5792 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1359 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 5800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1362 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1363 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5812 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1364 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5818 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1365 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5828 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 5838 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1375 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5844 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1376 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 5852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
#line 5860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1382 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5869 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1386 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 5878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1390 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 5887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1394 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1395 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1396 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1397 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5911 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1399 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5923 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1400 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5938 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1410 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1420 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1454 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 5995 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1455 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6001 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1459 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1460 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1464 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6023 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1472 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1475 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6040 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1482 "ds_parser.ypp" /* yacc.c:1660  */
    { 
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true; 
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6050 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1487 "ds_parser.ypp" /* yacc.c:1660  */
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl); 
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1495 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1496 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6072 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1500 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6080 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6091 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1512 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6097 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1513 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6103 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1517 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<string>();
        na->push_back(*(yyvsp[-2].s));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1526 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6120 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1527 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6126 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1532 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6132 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1533 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6138 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1537 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[0])),(yyvsp[0].pNameList),autoT,nullptr);
    }
#line 6148 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1542 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1545 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6165 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1549 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6176 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-2])),(yyvsp[-2].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1564 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 6193 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1565 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 6199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1566 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6205 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1570 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6211 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1571 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6225 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1578 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-5])),(yyvsp[-5].pNameList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1583 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6245 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1588 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-4])),(yyvsp[-4].pNameList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6258 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1596 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration(tokAt((yylsp[-3])),(yyvsp[-3].pNameList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6271 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1607 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1610 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6288 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1617 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1618 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6300 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1622 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6332 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1652 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1655 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1664 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6368 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1677 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1698 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1709 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1724 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 6421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1725 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 6427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1729 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1768 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1834 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1841 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 6559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1850 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 6565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1851 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 6571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1852 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1853 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1855 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 6595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1856 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 6601 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1857 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 6607 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1858 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 6613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1859 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1860 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1861 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6631 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1862 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 6637 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 6643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 6649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1865 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 6655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1866 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 6661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1867 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 6667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1868 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 6673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1869 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 6679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1870 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 6685 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1871 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 6691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1872 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 6697 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1873 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 6703 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 6709 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 6715 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 6721 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1880 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 6727 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 6733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1882 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 6739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1886 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 6752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1897 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6761 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1901 "ds_parser.ypp" /* yacc.c:1660  */
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 6773 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1911 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 6779 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1912 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6785 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 369:
#line 1913 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 6791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 370:
#line 1914 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 371:
#line 1928 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6821 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 1934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6830 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 1938 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6840 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 1943 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6850 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 1948 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 6860 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 1953 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->ref = false;
        (yyvsp[-3].pTypeDecl)->constant = false;
        (yyvsp[-3].pTypeDecl)->removeRef = true;
        (yyvsp[-3].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 6872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 1960 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6882 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 1965 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 6891 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 1969 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 6900 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 1973 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 6910 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 1978 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6920 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 1983 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_shared<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6932 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 1990 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6942 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 1995 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2001 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2006 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 6972 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2010 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 6982 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2015 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 6996 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2024 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7005 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2028 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7015 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2033 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7029 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2042 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7038 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2046 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7048 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2051 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-4]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
    }
#line 7062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2060 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7073 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2066 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7084 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2075 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_need_oxford_comma=false;
    }
#line 7092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2081 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto vtype = make_shared<TypeDecl>(Type::tVariant);
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
#line 7109 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2096 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7115 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2097 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7121 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2098 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7127 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2099 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7133 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2103 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2110 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_shared<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7156 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2119 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7166 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2124 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2131 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2136 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7196 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 409:
#line 2142 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7207 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 410:
#line 2148 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7219 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 411:
#line 2155 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7232 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 412:
#line 2166 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7240 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 413:
#line 2169 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7256 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 414:
#line 2183 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7267 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 415:
#line 2192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 416:
#line 2193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 417:
#line 2197 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 418:
#line 2202 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7298 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 419:
#line 2209 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 420:
#line 2214 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7321 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 421:
#line 2225 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7331 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 422:
#line 2230 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7340 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 423:
#line 2237 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_shared<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 424:
#line 2249 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 7360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 425:
#line 2250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7366 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 426:
#line 2254 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7385 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 427:
#line 2268 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7404 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 7408 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2284 "ds_parser.ypp" /* yacc.c:1903  */


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


