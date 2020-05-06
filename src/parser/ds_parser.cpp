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
#line 58 "ds_parser.ypp"

	#include "daScript/misc/platform.h"
	#include "daScript/simulate/debug_info.h"
	#include "daScript/ast/compilation_errors.h"

	#ifdef _MSC_VER
	#pragma warning(disable:4262)
	#pragma warning(disable:4127)
	#pragma warning(disable:4702)
	#endif

    using namespace das;

    void das_yyerror (const string & error);
    void das_yyfatalerror (const string & error, das::CompilationError cerr = das::CompilationError::syntax_error);
    void das_yyerror (const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified);
	void das_checkName(const string & name, const LineInfo &at);
    int yylex();
    void yybegin(const char * str);

#line 102 "ds_parser.cpp"

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
        VariableDeclaration ( vector<pair<string,LineInfo>> * n, TypeDecl * t, Expression * i )
            : pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () {
            if ( pNameList ) delete pNameList;
            delete pTypeDecl;
            if ( pInit ) delete pInit;
            if ( annotation ) delete annotation;
        }
        vector<pair<string,LineInfo>>   *pNameList;
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
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 200 "ds_parser.cpp"

/* Token type.  */
#ifndef DAS_YYTOKENTYPE
# define DAS_YYTOKENTYPE
  enum das_yytokentype
  {
    LEXER_ERROR = 258,
    DAS_STRUCT = 259,
    DAS_CLASS = 260,
    DAS_LET = 261,
    DAS_DEF = 262,
    DAS_WHILE = 263,
    DAS_IF = 264,
    DAS_STATIC_IF = 265,
    DAS_ELSE = 266,
    DAS_FOR = 267,
    DAS_CATCH = 268,
    DAS_TRUE = 269,
    DAS_FALSE = 270,
    DAS_NEWT = 271,
    DAS_TYPEINFO = 272,
    DAS_TYPE = 273,
    DAS_IN = 274,
    DAS_IS = 275,
    DAS_AS = 276,
    DAS_ELIF = 277,
    DAS_STATIC_ELIF = 278,
    DAS_ARRAY = 279,
    DAS_RETURN = 280,
    DAS_NULL = 281,
    DAS_BREAK = 282,
    DAS_TRY = 283,
    DAS_OPTIONS = 284,
    DAS_TABLE = 285,
    DAS_EXPECT = 286,
    DAS_CONST = 287,
    DAS_REQUIRE = 288,
    DAS_OPERATOR = 289,
    DAS_ENUM = 290,
    DAS_FINALLY = 291,
    DAS_DELETE = 292,
    DAS_DEREF = 293,
    DAS_SCOPE = 294,
    DAS_TYPEDEF = 295,
    DAS_WITH = 296,
    DAS_CAST = 297,
    DAS_OVERRIDE = 298,
    DAS_UPCAST = 299,
    DAS_ITERATOR = 300,
    DAS_VAR = 301,
    DAS_ADDR = 302,
    DAS_CONTINUE = 303,
    DAS_WHERE = 304,
    DAS_PASS = 305,
    DAS_REINTERPRET = 306,
    DAS_MODULE = 307,
    DAS_PUBLIC = 308,
    DAS_LABEL = 309,
    DAS_GOTO = 310,
    DAS_IMPLICIT = 311,
    DAS_SHARED = 312,
    DAS_SMART_PTR = 313,
    DAS_TBOOL = 314,
    DAS_TVOID = 315,
    DAS_TSTRING = 316,
    DAS_TAUTO = 317,
    DAS_TINT = 318,
    DAS_TINT2 = 319,
    DAS_TINT3 = 320,
    DAS_TINT4 = 321,
    DAS_TUINT = 322,
    DAS_TBITFIELD = 323,
    DAS_TUINT2 = 324,
    DAS_TUINT3 = 325,
    DAS_TUINT4 = 326,
    DAS_TFLOAT = 327,
    DAS_TFLOAT2 = 328,
    DAS_TFLOAT3 = 329,
    DAS_TFLOAT4 = 330,
    DAS_TRANGE = 331,
    DAS_TURANGE = 332,
    DAS_TBLOCK = 333,
    DAS_TINT64 = 334,
    DAS_TUINT64 = 335,
    DAS_TDOUBLE = 336,
    DAS_TFUNCTION = 337,
    DAS_TLAMBDA = 338,
    DAS_TINT8 = 339,
    DAS_TUINT8 = 340,
    DAS_TINT16 = 341,
    DAS_TUINT16 = 342,
    DAS_TTUPLE = 343,
    DAS_TVARIANT = 344,
    DAS_GENERATOR = 345,
    DAS_YIELD = 346,
    ADDEQU = 347,
    SUBEQU = 348,
    DIVEQU = 349,
    MULEQU = 350,
    MODEQU = 351,
    ANDEQU = 352,
    OREQU = 353,
    XOREQU = 354,
    SHL = 355,
    SHR = 356,
    ADDADD = 357,
    SUBSUB = 358,
    LEEQU = 359,
    SHLEQU = 360,
    SHREQU = 361,
    GREQU = 362,
    EQUEQU = 363,
    NOTEQU = 364,
    RARROW = 365,
    LARROW = 366,
    QQ = 367,
    QDOT = 368,
    QBRA = 369,
    LPIPE = 370,
    LBPIPE = 371,
    LAPIPE = 372,
    RPIPE = 373,
    CLONEEQU = 374,
    ROTL = 375,
    ROTR = 376,
    ROTLEQU = 377,
    ROTREQU = 378,
    MAPTO = 379,
    BRABRAB = 380,
    BRACBRB = 381,
    CBRCBRB = 382,
    INTEGER = 383,
    LONG_INTEGER = 384,
    UNSIGNED_INTEGER = 385,
    UNSIGNED_LONG_INTEGER = 386,
    FLOAT = 387,
    DOUBLE = 388,
    NAME = 389,
    BEGIN_STRING = 390,
    STRING_CHARACTER = 391,
    END_STRING = 392,
    BEGIN_STRING_EXPR = 393,
    END_STRING_EXPR = 394,
    UNARY_MINUS = 395,
    UNARY_PLUS = 396,
    PRE_INC = 397,
    PRE_DEC = 398,
    POST_INC = 399,
    POST_DEC = 400,
    DEREF = 401,
    COLCOL = 402
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 85 "ds_parser.ypp"

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
    vector<pair<string,LineInfo>> * pNameWithPosList;
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
    Function *                      pFuncDecl;

#line 386 "ds_parser.cpp"

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
#define YYLAST   6813

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  162
/* YYNRULES -- Number of rules.  */
#define YYNRULES  488
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  868

#define YYUNDEFTOK  2
#define YYMAXUTOK   402

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
       2,     2,     2,   157,     2,   174,   168,   153,   146,     2,
     166,   167,   151,   150,   140,   149,   162,   152,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   143,   171,
     147,   141,   148,   142,   169,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   164,     2,   165,   145,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   172,   144,   173,   156,     2,     2,     2,
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
     135,   136,   137,   138,   139,   154,   155,   158,   159,   160,
     161,   163,   170
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   405,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   421,   433,   434,   438,   442,
     447,   456,   464,   480,   502,   506,   509,   515,   524,   545,
     568,   569,   573,   577,   578,   582,   585,   591,   597,   600,
     606,   607,   611,   612,   613,   622,   623,   627,   636,   651,
     661,   670,   671,   672,   673,   674,   675,   679,   684,   692,
     693,   694,   695,   696,   697,   698,   699,   705,   718,   734,
     735,   739,   746,   758,   762,   769,   770,   774,   775,   776,
     780,   785,   792,   796,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   830,   831,   835,
     873,   914,   917,   927,   928,   929,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   948,   953,   959,   965,   976,   982,   983,   994,   998,
    1004,  1007,  1010,  1014,  1020,  1024,  1028,  1031,  1034,  1039,
    1042,  1050,  1053,  1058,  1061,  1069,  1075,  1076,  1080,  1081,
    1085,  1119,  1119,  1119,  1122,  1122,  1122,  1127,  1127,  1127,
    1135,  1139,  1144,  1150,  1150,  1150,  1154,  1154,  1154,  1159,
    1159,  1159,  1168,  1171,  1177,  1178,  1185,  1196,  1197,  1201,
    1206,  1261,  1262,  1263,  1264,  1265,  1266,  1270,  1271,  1272,
    1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1289,  1290,  1294,  1305,  1310,  1320,  1324,
    1324,  1324,  1331,  1331,  1331,  1345,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,
    1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,
    1370,  1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,  1382,  1383,  1384,  1388,  1392,  1395,  1398,  1399,
    1400,  1401,  1406,  1411,  1412,  1415,  1415,  1415,  1418,  1422,
    1426,  1430,  1431,  1432,  1433,  1434,  1435,  1436,  1446,  1456,
    1511,  1512,  1516,  1517,  1521,  1529,  1532,  1536,  1575,  1580,
    1588,  1589,  1593,  1596,  1605,  1606,  1610,  1619,  1620,  1625,
    1626,  1630,  1636,  1642,  1645,  1649,  1655,  1664,  1665,  1666,
    1670,  1671,  1675,  1678,  1683,  1688,  1696,  1707,  1710,  1717,
    1718,  1722,  1749,  1749,  1761,  1764,  1773,  1786,  1798,  1799,
    1803,  1807,  1818,  1833,  1834,  1838,  1878,  1879,  1883,  1960,
    1967,  1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,
    1985,  1986,  1987,  1988,  1989,  1990,  1991,  1992,  1993,  1994,
    1995,  1996,  1997,  1998,  1999,  2000,  2004,  2005,  2006,  2007,
    2008,  2009,  2013,  2024,  2028,  2038,  2045,  2054,  2054,  2054,
    2067,  2068,  2069,  2070,  2071,  2085,  2091,  2095,  2100,  2105,
    2110,  2115,  2119,  2123,  2128,  2132,  2137,  2137,  2137,  2143,
    2150,  2150,  2150,  2155,  2155,  2155,  2161,  2161,  2161,  2166,
    2170,  2170,  2170,  2175,  2175,  2175,  2184,  2188,  2188,  2188,
    2193,  2193,  2193,  2202,  2206,  2206,  2206,  2211,  2211,  2211,
    2220,  2220,  2220,  2226,  2226,  2226,  2235,  2235,  2250,  2250,
    2270,  2271,  2272,  2273,  2277,  2284,  2293,  2298,  2305,  2310,
    2316,  2322,  2329,  2340,  2343,  2357,  2366,  2367,  2371,  2376,
    2383,  2388,  2399,  2404,  2411,  2423,  2424,  2428,  2445
};
#endif

#if DAS_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"lexer error\"", "\"struct\"",
  "\"class\"", "\"let\"", "\"def\"", "\"while\"", "\"if\"",
  "\"static_if\"", "\"else\"", "\"for\"", "\"recover\"", "\"true\"",
  "\"false\"", "\"new\"", "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"",
  "\"as\"", "\"elif\"", "\"static_elif\"", "\"array\"", "\"return\"",
  "\"null\"", "\"break\"", "\"try\"", "\"options\"", "\"table\"",
  "\"expect\"", "\"const\"", "\"require\"", "\"operator\"", "\"enum\"",
  "\"finally\"", "\"delete\"", "\"deref\"", "\"scope\"", "\"typedef\"",
  "\"with\"", "\"cast\"", "\"override\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"shared\"", "\"smart_ptr\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
  "function_name", "global_function_declaration", "function_declaration",
  "expression_block", "expression_any", "expressions", "expr_pipe",
  "name_in_namespace", "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "let_scope", "kwd_let", "expression_let",
  "expr_cast", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "expr_type_info",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "expr_list",
  "block_or_simple_block", "block_or_lambda", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@13", "$@14",
  "$@15", "$@16", "expr", "$@17", "$@18", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@19", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "structure_declaration", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@20", "$@21", "type_declaration", "$@22",
  "$@23", "$@24", "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31",
  "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40",
  "$@41", "$@42", "$@43", "$@44", "$@45", "variant_alias_declaration",
  "$@46", "bitfield_alias_declaration", "$@47", "make_decl",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
      44,    61,    63,    58,   124,    94,    38,    60,    62,    45,
      43,    42,    47,    37,   395,   396,   126,    33,   397,   398,
     399,   400,    46,   401,    91,    93,    40,    41,    36,    64,
     402,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -516

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-516)))

#define YYTABLE_NINF -448

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -516,    17,  -516,  -516,   -78,   -67,   -69,   -59,   -92,  -516,
     -35,  -516,  -516,   -11,  -516,  -516,  -516,  -516,  -516,   392,
    -516,    78,  -516,  -516,  -516,  -516,  -516,  -516,    28,  -516,
      38,    52,    64,  -516,  -516,    15,  -516,    48,    80,  -516,
      90,   133,  -516,   114,   153,   125,  -516,   -86,  -516,  -516,
    -516,     0,  -516,   161,  -516,    37,    16,   -78,   117,   -67,
     168,  -516,   186,   202,  -516,   270,  -516,   224,  -516,   -90,
     196,   203,   230,  -516,   -78,   -11,  -516,  6656,   361,   366,
    -516,   241,   267,   237,  -516,   418,  -516,  -516,  -516,  -516,
    -516,   284,    73,  -516,  -516,  -516,  -516,   370,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,   252,   -87,  5800,  -516,
    -516,   291,   292,  -516,   -22,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,   -94,   285,   -70,  -516,  -516,   -62,   256,  -516,
     140,  -516,  -516,  -516,  -516,  -516,  -516,  -516,    43,  -516,
    -516,   -40,  -516,   282,   283,   287,   288,  -516,  -516,  -516,
     274,  -516,  -516,  -516,  -516,  -516,   289,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,   290,  -516,  -516,  -516,
     295,   296,  -516,  -516,  -516,  -516,   297,   298,  -516,  -516,
    -516,  -516,  -516,    -3,  -516,   261,   303,  -516,   276,  -516,
     -78,  -516,    -6,  -516,   -31,  5800,   277,  -516,   -19,  -516,
    -516,  -516,   220,  -516,  -516,  -516,    73,  -516,   -60,  4534,
    -516,  -516,  -516,  -516,  -516,   314,  -516,   113,   173,   192,
    -516,  -516,  -516,  -516,  -516,   419,  -516,  -516,    -4,  2536,
    -516,  -516,   -57,  5800,   -56,    79,   316,  -516,   111,  -516,
     293,  6011,  -516,  -516,  -516,  -516,    11,   281,   321,  5800,
    -516,    92,  -516,  -516,  -516,  -516,  5723,   294,  -516,   301,
     309,   311,   302,   315,  -516,   323,  4534,  4534,   465,   917,
    4534,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  4534,  4534,
    4534,  4534,  4534,  4534,  -516,   304,  -516,  -516,   -85,  -516,
    -516,  -516,   307,  -516,  -516,  -516,  -516,  -516,  5067,   306,
    -516,  -516,  -516,  -516,  -516,  5800,  5800,  5800,  5800,   308,
     291,  5800,   241,  5800,   241,  5800,   241,  5877,   292,  -516,
    -516,  -516,   313,  -516,  -516,  5871,  -516,  -516,  6011,  -516,
    -516,  -516,  -516,  -516,  -516,  5800,  -516,  2665,  -516,  1239,
    -516,   316,  -516,  -516,   587,  -516,  -516,  -516,  2665,   637,
    -516,   342,  4534,  -516,  -516,  4534,  -516,  5800,    84,    84,
     316,  1560,   316,  1705,  6263,  -516,    50,   266,    84,    84,
     -54,    84,    84,  5204,   -50,  -516,  2133,   241,     3,   345,
    4534,  4534,  -516,  -516,  4534,  4534,  4534,  4534,   346,  4534,
     349,  4534,  4534,  4534,  4534,  4534,  2803,  4534,  4534,  4534,
    4534,  4534,  4534,  4534,  4534,  4534,  4534,   352,  4534,  -516,
    2932,  1849,  2083,  2662,  2753,  -516,   -82,  2929,   285,  3020,
     285,  3196,   285,   -88,  -516,   -15,  6011,   148,  -516,  -516,
    3868,   106,  -516,   374,  6094,  -516,  4534,  -516,  -516,   316,
    1995,  -516,   277,  4534,  4534,  -516,   320,   364,  3070,  2278,
    -516,   457,   325,   326,  4534,  -516,  -516,  -516,  -516,  -516,
    -516,   329,   331,   332,  -516,  -516,  -516,   475,  -516,   -77,
    -516,  6198,  -516,   419,  -516,  2665,  -516,  1045,  3199,  2407,
    5273,  5800,  5800,  5341,  5800,  3287,    26,   -74,  1850,   340,
    3337,  6263,   368,   215,   369,  -516,  -516,   223,    32,  3475,
      67,   158,  4534,  4534,   333,  -516,  4534,   375,  -516,   219,
    -516,   378,  -516,    49,  6466,   285,   367,  -516,  -516,   563,
     563,  6531,  6531,   461,   461,   350,   351,  -516,  5948,   -72,
     -72,   563,   563,   383,  6332,   197,   774,   538,  6531,  6531,
      12,    12,   351,   351,   351,  -516,  6017,  -516,    60,  -516,
    5800,  -516,  -516,  -516,   385,  -516,   372,  -516,   373,  -516,
     407,  5800,  -516,  5877,  -516,   292,   419,  4534,  -516,  -516,
     106,  4534,  4534,  4534,  4534,  4534,  4534,  4534,  4534,  4534,
    4534,  4534,  4534,  4534,  4534,  4534,   910,    39,  2665,  -516,
    -516,  4618,   509,  6466,   910,  -516,   413,   429,  6466,  2665,
    -516,  4714,   394,  -516,  -516,   910,  -516,  -516,  -516,   530,
     316,  -516,  3613,  3742,  -516,  4810,  -516,  -516,    75,   425,
     443,  5409,  -516,  3425,  3563,  -516,  3870,   420,  4534,  4534,
    -516,   378,   451,   422,   378,  4534,   424,  4534,  4534,   378,
     427,   428,  6466,  -516,  -516,  6397,  5800,   241,    44,   118,
    4534,  -516,   -81,  -516,  3871,  -516,  -516,  4534,  -516,  -516,
    -516,  3959,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  6011,  -516,  -516,  -516,  6466,  -516,  6466,  6466,  6466,
    6466,  6466,  6466,  6466,  6466,  6466,  6466,  6466,  6466,  6466,
    6466,  6466,  -516,  4534,  -516,  4906,  -516,   277,  -516,  -516,
    -516,  -516,  5002,  -516,  -516,   271,  -516,  -516,  4534,  4534,
    -516,  -516,  -516,   155,  -516,  -516,  -516,  -516,  4009,   -64,
    6466,   227,    44,  -516,   368,  6466,  -516,  -516,   -42,   180,
    -516,  -516,  -516,  4097,   285,   423,  6466,  4147,  -516,  -516,
    5800,  -516,    88,  6466,  -516,  -516,  -516,  -516,    13,  -516,
    -516,  -516,  1412,   277,  -516,  -516,  4534,  -516,   269,   269,
    5800,  4276,   460,  4534,  4534,  4534,  -516,  5477,  4534,   430,
    4534,  4534,   431,  -516,   449,  -516,  4534,  6466,  4531,  -516,
    -516,   277,  -516,  -516,   910,  6526,   453,  5545,   450,  6531,
    6531,  6531,  -516,  5139,  -516,  6466,  5139,  -516,   -70,  -516,
    6466,  -516,  -516,   271,  -516,  -516,  -516,  4405,   552,   437,
     432,  -516,   -70,  -516,  -516,   439,  5800,   468,  5613,  4534,
     442,   452,  -516,  -516,  6564,  -516,  -516,  6466,  -516,  -516,
    -516,  5800,   454,  6573,  -516,  -516,   455,  -516
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    75,     1,   168,     0,     0,     0,     0,     0,   169,
       0,   458,   456,     0,    14,     3,    10,     9,     8,     0,
       7,   339,     6,    11,     5,     4,    12,    13,    65,    67,
      23,    35,    32,    33,    25,    30,    24,     0,     0,    15,
       0,     0,    70,   146,     0,    71,    73,     0,    69,   357,
     356,     0,   119,     0,   340,   342,     0,     0,     0,     0,
       0,    31,     0,     0,    28,     0,   344,     0,   348,     0,
       0,     0,     0,   148,     0,     0,    76,     0,     0,     0,
      82,    77,   353,     0,   337,     0,    63,    64,    61,    62,
      60,     0,     0,    59,    68,    36,    34,    30,    27,    26,
     386,   389,   387,   390,   388,   391,     0,     0,     0,   350,
     349,     0,     0,   147,     0,    74,    85,    86,    88,    87,
      89,    90,    91,    92,   111,   112,   109,   110,   102,   113,
     114,   103,   100,   101,   115,   116,   117,   118,   105,   106,
     104,    98,    99,    94,    93,    95,    96,    97,    84,    83,
     107,   108,   300,    80,     0,   355,   305,     0,     0,    16,
       0,    55,    56,    53,    54,    52,    51,    57,     0,    29,
     344,     0,   351,     0,     0,     0,     0,   361,   381,   362,
     393,   363,   367,   368,   369,   370,   385,   374,   375,   376,
     377,   378,   379,   380,   382,   383,   429,   366,   373,   384,
     436,   443,   364,   371,   365,   372,     0,     0,   392,   400,
     403,   401,   402,     0,   395,     0,     0,   317,     0,    72,
       0,    78,     0,   310,     0,     0,     0,   354,   302,   359,
     341,   338,   330,   343,    17,    18,     0,    66,     0,     0,
     345,   420,   423,   426,   416,     0,   397,   430,   437,   444,
     450,   453,   407,   412,   419,     0,   415,   409,     0,     0,
     347,   411,     0,     0,     0,     0,     0,   308,   321,    79,
     300,    81,   141,   120,   303,   358,   300,     0,     0,     0,
     331,     0,    58,   352,   240,   241,     0,     0,   235,     0,
       0,     0,     0,     0,   385,     0,     0,     0,     0,     0,
       0,   201,   203,   202,   204,   205,   206,    19,     0,     0,
       0,     0,     0,     0,   197,   198,   238,   199,   236,   293,
     292,   291,    75,   296,   237,   295,   294,   273,     0,     0,
     239,   460,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,    77,     0,    77,     0,    77,     0,     0,   414,
     408,   410,     0,   413,   405,     0,   396,   459,   316,   457,
     318,   301,   309,   320,   319,     0,   322,     0,   311,     0,
     307,     0,   306,   360,     0,   328,   329,   327,     0,   150,
     153,     0,     0,   171,   174,     0,   177,     0,   264,   265,
       0,     0,     0,     0,     0,   482,     0,     0,   245,   244,
     278,   243,   242,     0,     0,   299,     0,    77,     0,     0,
       0,     0,   266,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,   394,     0,     0,    80,     0,
      80,     0,    80,   146,   314,     0,   312,     0,   406,   404,
     323,     0,   326,     0,   325,   143,     0,    45,    46,     0,
       0,   154,     0,     0,     0,   155,     0,     0,     0,     0,
     123,   121,     0,     0,     0,   136,   131,   129,   130,   142,
     124,     0,     0,     0,   134,   135,   137,   166,   128,     0,
     125,   207,   304,   327,   332,     0,   336,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,     0,     0,
       0,   473,   466,     0,   476,   477,   478,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,    20,   268,   229,
     228,     0,   274,     0,   192,    80,     0,   288,   289,   246,
     247,   259,   260,   257,   258,     0,   283,   272,     0,   297,
     298,   248,   249,     0,     0,   262,   263,   261,   255,   256,
     251,   250,   252,   253,   254,   271,     0,   276,     0,   421,
       0,   427,   417,   398,     0,   431,     0,   438,     0,   445,
       0,     0,   451,     0,   454,     0,   319,     0,   198,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
     159,   207,     0,   149,     0,   140,     0,     0,    39,     0,
     163,   207,     0,   138,   139,     0,   127,   132,   133,     0,
       0,   126,     0,     0,   334,   207,   335,   151,     0,     0,
       0,     0,   279,     0,     0,   280,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   475,   483,   484,     0,     0,    77,     0,     0,
       0,   275,     0,   285,     0,   270,   290,     0,   269,   277,
     422,     0,   428,   418,   399,   432,   434,   439,   441,   446,
     448,   313,   452,   315,   455,   324,   144,   214,   215,   217,
     216,   218,   211,   212,   213,   219,   220,   209,   210,   221,
     222,   208,    49,     0,   160,   207,   157,     0,    50,    37,
      38,   164,   207,   161,   141,    42,   167,   170,     0,     0,
     333,   152,   183,     0,   180,   172,   175,   178,     0,     0,
     464,     0,     0,   468,   467,   474,   480,   479,     0,     0,
     471,   481,    21,     0,    80,     0,   193,     0,   194,   200,
       0,   226,     0,   284,   424,   435,   442,   449,     0,   158,
     165,   162,     0,     0,    40,    41,     0,    47,   224,   223,
       0,     0,     0,     0,     0,     0,   281,     0,     0,     0,
       0,     0,     0,   230,     0,   225,     0,   195,     0,   227,
     425,     0,   122,    43,     0,     0,     0,     0,     0,   173,
     176,   179,   282,   485,   470,   465,   485,   472,     0,   233,
     196,   286,    48,    42,   184,   186,   181,     0,     0,     0,
       0,   231,     0,   287,    44,     0,     0,     0,     0,     0,
       0,     0,   234,   185,     0,   189,   182,   486,   487,   488,
     187,     0,     0,     0,   188,   190,     0,   191
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -516,  -516,  -516,   207,   560,  -516,  -516,  -516,  -516,  -516,
    -516,   521,  -516,  -516,   561,  -516,  -516,  -516,  -209,  -516,
    -516,  -516,  -516,  -516,   390,  -516,   570,   -58,  -516,   553,
    -516,   310,  -336,  -437,  -516,  -516,   353,  -223,  -516,  -104,
    -358,   -13,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
     630,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -415,  -516,  -516,  -442,
    -516,  -364,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
     435,  -516,  -516,   357,  -516,  -516,  -516,   379,  -516,    41,
    -516,  -257,   312,  -264,  -239,   262,  -516,     4,  -516,  -516,
    -516,  -516,   467,   585,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -156,  -106,  -516,  -516,  -516,   317,  -516,  -516,  -516,
     -73,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,
    -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,  -516,   376,
    -515,  -381,  -516,  -516,  -286,   -12,   268,  -516,  -516,  -516,
    -159,  -516
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   160,   166,   397,   316,    16,    17,    35,
      36,    64,    18,    32,    33,   482,   483,   786,   787,   484,
     485,   486,   487,   488,   167,   168,    29,    30,    45,    46,
      47,    19,   153,   226,    81,    20,    52,   317,   489,   369,
     490,   318,   491,   319,   492,   493,   494,   495,   496,   640,
     497,   498,   320,   511,   793,   512,   794,   514,   795,   321,
     790,   845,   846,   862,   861,   866,   543,   769,   322,   323,
     324,   463,   500,   325,   326,   327,   676,   828,   677,   842,
     544,   770,   843,   222,   276,   277,   228,   223,   224,   454,
     455,   217,   218,   659,   267,   378,   281,   231,   157,    55,
      22,    85,   107,    68,    69,    23,    24,   155,    83,    53,
      25,   268,   329,   106,   210,   211,   215,   212,   340,   694,
     456,   338,   693,   335,   690,   336,   810,   337,   692,   341,
     695,   342,   775,   343,   697,   344,   776,   345,   699,   346,
     777,   347,   702,   348,   704,    26,    41,    27,    40,   330,
     522,   523,   331,   524,   525,   526,   527,   332,   396,   333,
     839,   334
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   232,   209,   273,   367,   499,   448,   360,   450,   462,
     452,   586,   530,   588,   395,   590,   114,     2,    51,   599,
     506,   546,    42,     3,   274,   578,   679,   362,   350,   252,
      86,    87,   408,   409,    77,   213,    60,   363,   418,   600,
     266,   420,   421,   767,    67,   658,     4,   171,     5,   405,
       6,   668,     7,   253,    75,   591,    28,     8,   723,   420,
     421,    31,    48,     9,    43,    34,   583,   364,    61,    10,
     220,   545,   229,   221,   171,    37,   680,   356,   216,    76,
      38,   406,    72,   109,    43,    11,   172,   161,   162,   584,
     437,   272,   438,   648,   641,   208,    72,   539,   680,    39,
      44,   239,    78,    79,   408,   409,    12,   798,   682,   254,
     438,   230,   620,   283,   412,   413,   357,   359,    57,   209,
      44,   630,   418,    43,   419,   420,   421,   422,   229,   801,
     423,   240,   502,   592,    80,    54,   269,   547,   255,   256,
     270,   227,   351,   257,    88,   219,   258,   644,    89,   754,
      90,    91,   271,   680,   275,   363,   593,   209,   706,    44,
     352,   259,   265,   434,   435,   436,   278,    62,   260,    56,
     353,   261,   278,   209,   437,   220,   438,    63,    57,   278,
     209,    13,    92,   236,   811,   364,   412,   413,    14,   680,
     358,    65,   209,   209,   418,    58,   597,   420,   421,   422,
     680,   163,   423,   375,    59,   164,   374,   165,    91,    84,
     237,   376,   208,   379,    67,   680,   681,   408,   409,    57,
      66,   533,   363,   534,    70,   391,   393,   689,   680,   209,
     209,   209,   209,   377,   516,   209,   528,   209,   664,   209,
     670,   209,   741,   749,   361,    95,   437,   673,   438,   622,
     208,   278,   364,   758,   365,   809,  -433,   366,   662,   209,
     724,  -433,   441,   442,   443,   444,   208,    71,   447,   772,
     449,   731,   451,   208,   314,   598,   234,   235,   272,  -433,
     751,   209,   783,   765,    72,   208,   208,    73,   759,   408,
     409,    74,   460,   784,   785,    82,   594,   410,   411,   412,
     413,   414,    97,   791,   415,   416,   417,   418,   778,   419,
     420,   421,   422,   617,   515,   423,  -440,   424,   425,   595,
      98,  -440,   208,   208,   208,   208,   792,   804,   208,   667,
     208,   671,   208,   100,   208,  -447,    99,   101,   360,  -440,
    -447,   764,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   664,   208,   802,   102,   103,   104,   105,  -447,   437,
     278,   438,  -232,   279,   113,   108,   280,  -232,   111,   410,
     411,   408,   409,   414,   208,   112,   415,   416,   417,   418,
     663,   419,   420,   421,   422,  -232,   664,   423,   666,   424,
     425,   540,   799,   722,   667,   150,    49,    50,   664,    51,
     151,   728,   159,   535,   536,   209,   209,   152,   209,   156,
     154,   426,   735,   427,   428,   429,   430,   431,   499,   158,
     159,   435,   436,    61,   170,   214,   216,   233,   225,   241,
     242,   437,   262,   438,   243,   244,   246,   247,   653,   654,
     245,   656,   248,   249,   250,   251,   263,   264,   339,   272,
     229,   349,   372,   412,   413,   373,   383,   220,   384,   768,
     381,   418,   386,   419,   420,   421,   422,   382,   385,   423,
     387,    13,   440,   404,   209,   445,   509,   390,   458,   548,
     555,   408,   409,   557,   232,   209,   575,   209,   800,   173,
     600,   625,   626,   632,   639,   174,   633,   634,   208,   208,
     636,   208,   637,   638,   780,   660,   674,   691,   662,   665,
     175,   234,   678,   437,   683,   438,   684,   686,   701,   356,
     696,   698,   727,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   700,   729,   730,   408,   409,
     813,   410,   411,   412,   413,   414,   734,   208,   415,   736,
     209,   418,   742,   419,   420,   421,   422,   743,   208,   423,
     208,   424,   425,   408,   409,   752,   748,   753,   832,   756,
     805,   833,   760,   761,   818,   824,   827,   829,   837,    43,
     835,   849,   850,   763,   537,   851,   853,   858,   430,   431,
     432,   433,   434,   435,   436,   855,    93,   859,   169,   252,
      96,   864,   867,   437,   844,   438,   282,    94,   115,   370,
     782,    21,   407,   371,   703,    44,   505,   238,   410,   411,
     412,   413,   414,   253,   737,   415,   416,   417,   418,   368,
     419,   420,   421,   422,   110,   757,   423,   446,   424,   425,
     457,   531,   380,   208,   209,   412,   413,   840,     0,   252,
       0,     0,     0,   418,   328,   419,   420,   421,   422,     0,
       0,   423,     0,     0,   209,   430,   431,   432,   433,   434,
     435,   436,     0,   253,   355,     0,     0,   808,   375,   254,
     437,     0,   438,     0,     0,     0,   376,     0,     0,     0,
       0,     0,   432,   433,   434,   435,   436,   815,     0,     0,
       0,     0,     0,     0,     0,   437,     0,   438,   503,   256,
       0,   388,   389,   257,     0,   394,   258,     0,     0,     0,
     209,     0,     0,   398,   399,   400,   401,   402,   403,   254,
       0,   259,     0,     0,     0,   209,     0,   208,   504,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   854,     0,     0,     0,   208,   255,   256,
       0,     0,     0,   257,     0,     0,   258,     0,   863,     0,
       0,     0,     0,     0,   408,   409,     0,     0,     0,     0,
       0,   259,   464,   508,   501,     0,     0,     0,     0,     0,
       0,   261,     0,   507,     0,   841,     0,   510,     0,     0,
     513,     0,     0,     0,     0,     0,   521,     0,   521,   852,
       0,     0,     0,   208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   549,   550,     0,   208,   551,
     552,   553,   554,     0,   556,     0,   558,   559,   560,   561,
     562,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,     0,   576,   410,   411,   412,   413,   414,     0,
       0,   415,   416,   417,   418,     0,   419,   420,   421,   422,
       0,     0,   423,     0,   424,   425,     0,     0,     0,     0,
       0,   616,     0,     0,     0,   621,     0,     0,   623,   624,
       0,     0,     0,   628,   631,     0,     0,     0,     0,   635,
     429,   430,   431,   432,   433,   434,   435,   436,     0,   392,
     408,   409,     0,     0,     0,     0,   437,     0,   438,     0,
     645,   173,     0,     0,   651,     0,     0,   174,     0,     0,
       0,     0,     0,   398,     0,   403,     0,     0,     0,     0,
       0,     0,   175,     0,   403,     0,     0,   672,   394,     0,
       0,   675,     0,     0,     0,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,     0,     0,
     410,   411,   412,   413,   414,     0,     0,   415,   416,   417,
     418,     0,   419,   420,   421,   422,     0,     0,   423,     0,
     424,   425,   705,     0,     0,     0,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,    43,   426,   725,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   732,   408,   409,     0,     0,     0,
       0,     0,   437,     0,   438,     0,     0,   717,   721,     0,
       0,     0,   272,     0,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,     0,
     755,     0,   521,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   766,     0,     0,     0,     0,
       0,     0,   773,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   601,   602,   603,
     604,   605,   606,   607,   608,   410,   411,   412,   413,   414,
     609,   610,   415,   416,   417,   418,   611,   419,   420,   421,
     422,     0,     0,   423,   612,   424,   425,   613,   614,     0,
       0,     0,     0,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,   797,     0,     0,   615,   426,     0,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,     0,
       0,     0,   807,     0,     0,     0,     0,   437,     0,   438,
       0,     0,     0,     0,     0,     0,   646,   501,     0,     0,
       0,   814,     0,     0,     0,     0,   817,     0,   819,   820,
     821,     0,     0,   823,     0,   825,   826,     0,     0,     0,
     465,   830,     0,     0,     0,     3,     0,   466,   467,   468,
       0,   469,     0,   284,   285,   286,   287,     0,     0,     0,
       0,     0,     0,     0,   470,   288,   471,   472,     0,     0,
       0,     0,   848,     0,     0,     0,   473,   289,     0,     0,
     474,   290,     0,   291,   857,     9,   292,   475,     0,   476,
     293,     0,     0,   477,   478,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,     0,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    43,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   309,
     310,     0,     0,     0,     0,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,   314,   315,    44,
     480,   272,   481,   465,     0,     0,     0,     0,     3,     0,
     466,   467,   468,     0,   469,     0,   284,   285,   286,   287,
       0,     0,     0,     0,     0,     0,     0,   470,   288,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     289,     0,     0,   474,   290,     0,   291,     0,     9,   292,
     475,     0,   476,   293,     0,     0,   477,   478,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
       0,     0,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,   284,   285,   286,   287,   313,     0,
     314,   315,    44,   480,   272,   812,   288,     0,     0,     0,
       0,     0,   252,     0,     0,     0,     0,     0,   289,     0,
       0,     0,   290,     0,   291,     0,     0,   292,     0,     0,
       0,   293,     0,     0,     0,     0,   253,     0,     0,   177,
     178,   179,     0,   181,   182,   183,   184,   185,   294,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   297,     0,     0,     0,     0,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   517,   307,     0,     0,     0,     0,
       0,   255,   256,     0,     0,     0,   257,     0,     0,   518,
     309,   310,     0,     0,     0,     0,   311,   312,     0,   284,
     285,   286,   287,     0,   259,   519,   520,     0,   314,   315,
      44,   288,   272,     0,   261,     0,     0,   252,     0,     0,
       0,     0,     0,   289,     0,     0,     0,   290,     0,   291,
       0,     0,   292,     0,     0,     0,   293,     0,     0,     0,
       0,   253,     0,     0,   177,   178,   179,     0,   181,   182,
     183,   184,   185,   294,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   517,
     307,     0,     0,     0,     0,     0,   255,   256,     0,     0,
       0,   257,     0,     0,   518,   309,   310,     0,     0,     0,
       0,   311,   312,     0,   284,   285,   286,   287,     0,   259,
       0,   529,     0,   314,   315,    44,   288,   272,     0,   261,
       0,   252,   350,     0,     0,     0,     0,     0,   289,     0,
       0,     0,   290,     0,   291,     0,     0,   292,     0,     0,
       0,   293,     0,     0,     0,   253,     0,     0,     0,   177,
     178,   179,     0,   181,   182,   183,   184,   185,   294,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   297,     0,     0,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,    43,   307,     0,     0,     0,     0,
     255,   256,     0,     0,     0,   257,   351,   579,   258,   308,
     309,   310,     0,     0,     0,     0,   311,   312,     0,   284,
     285,   286,   287,   259,   352,     0,   313,     0,   314,   315,
      44,   288,   272,   261,   353,     0,     0,     0,     0,     0,
       0,     0,     0,   289,     0,     0,     0,   290,     0,   291,
       0,     0,   292,     0,     0,     0,   293,     0,     0,     0,
       0,     0,     0,     0,   177,   178,   179,     0,   181,   182,
     183,   184,   185,   294,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,   297,     0,
       0,     0,     0,     0,     0,     0,   618,     0,     0,     0,
       0,     0,   461,     0,     0,   252,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    43,
     307,     0,     0,     0,     0,     0,     0,     0,     0,   253,
       0,     0,     0,     0,   308,   309,   310,   284,   285,   286,
     287,   311,   312,     0,     0,     0,     0,     0,     0,   288,
       0,   313,     0,   314,   315,    44,   619,   272,     0,     0,
       0,   289,     0,     0,     0,   290,     0,   291,     0,     0,
     292,     0,     0,     0,   293,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   179,   254,   181,   182,   183,   184,
     185,   294,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   295,   255,   256,     0,     0,     0,   257,
       0,     0,   258,     0,     0,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,   580,     0,     0,   261,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    43,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,   309,   310,     0,     0,     0,     0,   311,
     312,     0,   284,   285,   286,   287,     0,   541,     0,   313,
     542,   314,   315,    44,   288,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,     0,     0,     0,
     290,     0,   291,     0,     0,   292,     0,     0,     0,   293,
       0,     0,     0,     0,     0,     0,     0,   177,   178,   179,
       0,   181,   182,   183,   184,   185,   294,   187,   188,   189,
     190,   191,   192,   193,   194,   195,     0,   197,   198,   199,
       0,     0,   202,   203,   204,   205,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     296,   297,     0,     0,     0,     0,     0,     0,     0,   629,
       0,     0,     0,     0,     0,   461,     0,     0,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    43,   307,     0,     0,     0,     0,     0,     0,
       0,   284,   285,   286,   287,   649,     0,   308,   309,   310,
       0,     0,     0,   288,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,   313,   289,   314,   315,    44,   290,
     272,   291,     0,     0,   292,     0,     0,     0,   293,     0,
       0,     0,     0,     0,     0,     0,   177,   178,   179,     0,
     181,   182,   183,   184,   185,   294,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    43,   307,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,   287,   650,     0,   308,   309,   310,     0,
       0,     0,   288,   311,   312,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   289,   314,   315,    44,   290,   272,
     291,     0,     0,   292,     0,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,     0,   181,
     182,   183,   184,   185,   294,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
      43,   307,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,   287,     0,     0,   308,   309,   310,     0,     0,
       0,   288,   311,   312,   252,     0,     0,     0,     0,     0,
       0,   354,   313,   289,   314,   315,    44,   290,   272,   291,
       0,     0,   292,     0,     0,     0,   293,     0,   253,     0,
       0,     0,     0,     0,   177,   178,   179,     0,   181,   182,
     183,   184,   185,   294,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,   297,     0,
       0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,   461,     0,     0,   252,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    43,
     307,     0,     0,   255,   256,     0,     0,     0,   257,   253,
     581,   258,     0,     0,   308,   309,   310,   284,   285,   286,
     287,   311,   312,     0,   563,     0,   259,     0,     0,   288,
       0,   313,     0,   314,   315,    44,   261,   272,     0,     0,
       0,   289,     0,     0,     0,   290,     0,   291,     0,     0,
     292,     0,     0,     0,   293,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   179,   254,   181,   182,   183,   184,
     185,   294,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   295,   255,   256,     0,     0,     0,   257,
       0,   582,   258,     0,     0,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    43,   307,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,   287,
       0,     0,   308,   309,   310,     0,     0,     0,   288,   311,
     312,   252,     0,     0,     0,     0,     0,     0,     0,   313,
     289,   314,   315,    44,   290,   272,   291,     0,     0,   292,
       0,     0,     0,   293,     0,   253,     0,     0,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,   297,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
     255,   256,     0,     0,     0,   257,   253,   585,   258,     0,
       0,   308,   309,   310,   284,   285,   286,   287,   311,   312,
       0,     0,     0,   259,     0,     0,   288,     0,   313,   577,
     314,   315,    44,   261,   272,     0,     0,     0,   289,     0,
       0,     0,   290,     0,   291,     0,     0,   292,     0,     0,
       0,   293,     0,     0,   627,     0,     0,     0,     0,   177,
     178,   179,   254,   181,   182,   183,   184,   185,   294,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     295,   255,   256,     0,     0,     0,   257,     0,   587,   258,
       0,     0,   296,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   261,   298,   299,   300,   301,   302,
     303,   304,   305,   306,    43,   307,     0,     0,     0,     0,
       0,     0,     0,   284,   285,   286,   287,     0,     0,   308,
     309,   310,     0,     0,     0,   288,   311,   312,   252,     0,
       0,     0,     0,     0,     0,     0,   313,   289,   314,   315,
      44,   290,   272,   291,     0,     0,   292,     0,     0,     0,
     293,     0,   253,     0,     0,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   297,     0,     0,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    43,   307,     0,     0,   255,   256,     0,
       0,     0,   257,   253,   589,   258,     0,     0,   308,   309,
     310,   284,   285,   286,   287,   311,   312,     0,     0,     0,
     259,     0,     0,   288,     0,   313,   647,   314,   315,    44,
     261,   272,     0,     0,     0,   289,     0,     0,     0,   290,
       0,   291,     0,     0,   292,     0,     0,     0,   293,     0,
       0,     0,     0,     0,     0,     0,   177,   178,   179,   254,
     181,   182,   183,   184,   185,   294,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   295,   255,   256,
       0,     0,     0,   257,     0,   657,   258,     0,     0,   296,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   261,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    43,   307,     0,     0,     0,     0,     0,     0,     0,
       0,   253,     0,     0,     0,     0,   308,   309,   310,   284,
     285,   286,   287,   311,   312,     0,     0,     0,     0,     0,
       0,   288,     0,   313,   661,   314,   315,    44,     0,   272,
       0,     0,     0,   289,     0,     0,     0,   290,     0,   291,
       0,     0,   292,     0,     0,     0,   293,     0,     0,     0,
       0,     0,     0,     0,   177,   178,   179,   254,   181,   182,
     183,   184,   185,   294,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   295,   255,   256,     0,     0,
       0,   257,     0,   745,   258,     0,     0,   296,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   261,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    43,
     307,     0,     0,     0,     0,     0,     0,     0,     0,   253,
       0,     0,     0,     0,   308,   309,   310,   284,   285,   286,
     287,   311,   312,     0,     0,     0,     0,     0,     0,   288,
       0,   313,   669,   314,   315,    44,     0,   272,     0,     0,
       0,   289,     0,     0,     0,   290,     0,   291,     0,     0,
     292,     0,     0,     0,   293,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   179,   254,   181,   182,   183,   184,
     185,   294,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   295,   255,   256,     0,     0,     0,   257,
       0,   746,   258,     0,     0,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
     738,     0,     0,     0,     0,     0,     0,   261,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    43,   307,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,   287,
       0,     0,   308,   309,   310,     0,     0,     0,   288,   311,
     312,     0,     0,     0,     0,     0,     0,     0,     0,   313,
     289,   314,   315,    44,   290,   272,   291,     0,     0,   292,
       0,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,     0,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   739,
       0,     0,     0,     0,     0,     0,     0,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,   287,     0,
       0,   308,   309,   310,     0,     0,     0,   288,   311,   312,
     252,     0,   252,     0,     0,     0,     0,     0,   313,   289,
     314,   315,    44,   290,   272,   291,     0,     0,   292,     0,
       0,     0,   293,     0,   253,     0,   253,     0,     0,     0,
     177,   178,   179,     0,   181,   182,   183,   184,   185,   294,
     187,   188,   189,   190,   191,   192,   193,   194,   195,     0,
     197,   198,   199,     0,     0,   202,   203,   204,   205,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   297,     0,     0,     0,     0,   363,
     254,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    43,   307,     0,     0,   596,
     256,   255,   256,     0,   257,   253,   257,   258,   747,   258,
     308,   309,   310,   284,   285,   286,   287,   311,   312,     0,
       0,     0,   259,     0,   259,   288,     0,   313,   771,   314,
     315,    44,   261,   272,   261,     0,     0,   289,     0,     0,
       0,   290,     0,   291,     0,     0,   292,     0,     0,     0,
     293,     0,     0,     0,     0,     0,     0,     0,   177,   178,
     179,   254,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
     255,   256,     0,     0,     0,   257,     0,   774,   258,     0,
       0,   296,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   261,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    43,   307,     0,     0,     0,     0,     0,
       0,     0,     0,   253,     0,     0,     0,     0,   308,   309,
     310,   284,   285,   286,   287,   311,   312,     0,     0,     0,
       0,     0,     0,   288,     0,   313,   796,   314,   315,    44,
       0,   272,     0,     0,     0,   289,     0,     0,     0,   290,
       0,   291,     0,     0,   292,     0,     0,     0,   293,     0,
       0,     0,     0,     0,     0,     0,   177,   178,   179,   254,
     181,   182,   183,   184,   185,   294,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   295,   255,   256,
       0,     0,     0,   257,     0,   803,   258,     0,     0,   296,
     297,     0,     0,     0,     0,     0,     0,     0,   806,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    43,   307,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,   287,   816,     0,   308,   309,   310,     0,
       0,     0,   288,   311,   312,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   289,   314,   315,    44,   290,   272,
     291,     0,     0,   292,     0,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,     0,   181,
     182,   183,   184,   185,   294,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   296,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   299,   300,   301,   302,   303,   304,   305,   306,
      43,   307,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,   287,   847,     0,   308,   309,   310,     0,     0,
       0,   288,   311,   312,     0,     0,     0,     0,     0,     0,
       0,     0,   313,   289,   314,   315,    44,   290,   272,   291,
       0,     0,   292,     0,     0,     0,   293,     0,     0,     0,
       0,     0,     0,     0,   177,   178,   179,     0,   181,   182,
     183,   184,   185,   294,   187,   188,   189,   190,   191,   192,
     193,   194,   195,     0,   197,   198,   199,     0,     0,   202,
     203,   204,   205,     0,     0,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    43,
     307,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,   287,     0,     0,   308,   309,   310,     0,     0,     0,
     288,   311,   312,   252,     0,     0,     0,     0,     0,     0,
       0,   313,   289,   314,   315,    44,   290,   272,   291,     0,
       0,   292,     0,     0,     0,   293,     0,   253,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   297,   408,   409,
       0,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,    43,   307,
       0,     0,   255,   256,     0,     0,     0,   257,     0,   831,
     258,     0,     0,   308,   309,   310,     0,     0,     0,     0,
     311,   312,     0,     0,     0,   259,     0,     0,     0,     0,
     313,     0,   314,   315,    44,   261,   272,     0,     0,     0,
     601,   602,   603,   604,   605,   606,   607,   608,   410,   411,
     412,   413,   414,   609,   610,   415,   416,   417,   418,   611,
     419,   420,   421,   422,   408,   409,   423,   612,   424,   425,
     613,   614,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   615,
     426,     0,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,   438,     0,     0,     0,     0,     0,     0,   726,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   601,   602,   603,   604,
     605,   606,   607,   608,   410,   411,   412,   413,   414,   609,
     610,   415,   416,   417,   418,   611,   419,   420,   421,   422,
     408,   409,   423,   612,   424,   425,   613,   614,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   615,   426,     0,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,   438,     0,
       0,     0,     0,     0,     0,   733,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   601,   602,   603,   604,   605,   606,   607,   608,
     410,   411,   412,   413,   414,   609,   610,   415,   416,   417,
     418,   611,   419,   420,   421,   422,   408,   409,   423,   612,
     424,   425,   613,   614,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   615,   426,     0,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   438,     0,     0,     0,     0,     0,
       0,   740,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   601,   602,
     603,   604,   605,   606,   607,   608,   410,   411,   412,   413,
     414,   609,   610,   415,   416,   417,   418,   611,   419,   420,
     421,   422,   408,   409,   423,   612,   424,   425,   613,   614,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   615,   426,     0,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
     438,     0,     0,     0,     0,     0,     0,   779,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   408,   409,     0,
       0,     0,     0,     0,   601,   602,   603,   604,   605,   606,
     607,   608,   410,   411,   412,   413,   414,   609,   610,   415,
     416,   417,   418,   611,   419,   420,   421,   422,     0,     0,
     423,   612,   424,   425,   613,   614,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   615,   426,     0,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,     0,     0,     0,   408,
     409,     0,     0,     0,   437,     0,   438,   410,   411,   412,
     413,   414,     0,   781,   415,   416,   417,   418,     0,   419,
     420,   421,   422,     0,     0,   423,     0,   424,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,     0,     0,     0,   408,   409,     0,     0,     0,   437,
       0,   438,     0,     0,     0,     0,     0,     0,   439,   410,
     411,   412,   413,   414,     0,     0,   415,   416,   417,   418,
       0,   419,   420,   421,   422,     0,     0,   423,     0,   424,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   408,   409,     0,     0,     0,     0,     0,
       0,   437,     0,   438,   410,   411,   412,   413,   414,     0,
     838,   415,   416,   417,   418,     0,   419,   420,   421,   422,
       0,     0,   423,     0,   424,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,     0,     0,
       0,   408,   409,     0,     0,     0,   437,     0,   438,     0,
       0,   538,     0,   410,   411,   412,   413,   414,     0,     0,
     415,   416,   417,   418,     0,   419,   420,   421,   422,     0,
       0,   423,     0,   424,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,     0,     0,   408,
     409,     0,     0,     0,     0,   437,     0,   438,     0,     0,
     652,   410,   411,   412,   413,   414,     0,     0,   415,   416,
     417,   418,     0,   419,   420,   421,   422,     0,     0,   423,
       0,   424,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,     0,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,     0,     0,   408,   409,     0,
       0,     0,     0,   437,     0,   438,     0,     0,   655,   410,
     411,   412,   413,   414,     0,     0,   415,   416,   417,   418,
       0,   419,   420,   421,   422,     0,     0,   423,     0,   424,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,     0,     0,   408,   409,     0,     0,     0,
       0,   437,     0,   438,     0,     0,   744,   410,   411,   412,
     413,   414,     0,     0,   415,   416,   417,   418,     0,   419,
     420,   421,   422,     0,     0,   423,     0,   424,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,     0,     0,   408,   409,     0,     0,     0,     0,   437,
       0,   438,     0,     0,   822,   410,   411,   412,   413,   414,
       0,     0,   415,   416,   417,   418,     0,   419,   420,   421,
     422,     0,     0,   423,     0,   424,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,     0,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,     0,     0,     0,   437,     0,   438,
       0,     0,   836,   410,   411,   412,   413,   414,     0,     0,
     415,   416,   417,   418,     0,   419,   420,   421,   422,     0,
       0,   423,     0,   424,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,     0,     0,
       0,     0,     0,   174,     0,   426,     0,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,     0,   175,     0,
       0,     0,     0,     0,     0,   437,     0,   438,     0,     0,
     856,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,     0,     0,     0,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,   298,   299,
     300,     0,     0,     0,     0,     0,     0,    43,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
       0,   408,   409,    44,     0,     0,     0,     0,     0,     0,
       0,   173,     0,     0,     0,     0,     0,   174,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,   408,   409,
      44,   410,   411,   412,   413,   414,     0,     0,   415,   416,
     417,   418,     0,   419,   420,   421,   422,     0,     0,   423,
       0,   424,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   453,     0,   426,     0,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,   438,   459,   408,   409,     0,
       0,     0,     0,   252,     0,     0,     0,    44,   410,   411,
     412,   413,   414,     0,     0,   415,   416,   417,   418,     0,
     419,   420,   421,   422,     0,     0,   423,   253,   424,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,     0,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,   438,   685,   408,   409,     0,   410,   411,   412,
     413,   414,     0,   254,   415,   416,   417,   418,     0,   419,
     420,   421,   422,     0,     0,   423,     0,   424,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,   256,     0,     0,     0,   257,     0,   426,
     258,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,     0,     0,     0,     0,   259,     0,     0,     0,   437,
       0,   438,   688,     0,     0,   261,   601,   602,   603,   604,
     605,   606,   607,   608,   410,   411,   412,   413,   414,   609,
     610,   415,   416,   417,   418,   611,   419,   420,   421,   422,
    -207,     0,   423,   612,   424,   425,   613,   614,   408,   409,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   615,   426,     0,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   409,     0,     0,     0,     0,     0,
     601,   602,   603,   604,   605,   606,   607,   608,   410,   411,
     412,   413,   414,   609,   610,   415,   416,   417,   418,   642,
     419,   420,   421,   422,     0,     0,   423,   612,   424,   425,
     613,   614,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   643,
     426,     0,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   408,   409,     0,     0,     0,     0,     0,     0,
     437,     0,   438,   410,   411,   412,   413,   414,     0,     0,
     415,   416,   417,   418,     0,   419,   420,   421,   422,     0,
       0,   423,     0,   424,   425,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   408,   409,     0,
       0,     0,     0,     0,     0,   437,     0,   438,     0,     0,
       0,     0,   410,   411,   412,   413,   414,     0,     0,   415,
     416,   417,   418,     0,   419,   420,   421,   422,     0,     0,
     423,     0,   424,   425,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   426,   687,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   408,   409,     0,     0,
       0,     0,     0,     0,   437,     0,   438,   410,   411,   412,
     413,   414,     0,     0,   415,   416,   417,   418,     0,   419,
     420,   421,   422,     0,     0,   423,     0,   424,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   762,     0,     0,   426,
       0,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   408,   409,     0,     0,     0,     0,     0,   252,   437,
       0,   438,     0,     0,     0,     0,   410,   411,   412,   413,
     414,     0,     0,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   253,     0,   423,     0,   424,   425,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,   426,     0,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     253,     0,     0,     0,     0,     0,     0,     0,   437,   253,
     438,   410,   411,   412,   413,     0,     0,     0,   254,     0,
       0,   418,     0,   419,   420,   421,   422,     0,     0,   423,
       0,   424,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   255,   256,     0,
       0,     0,   257,     0,   834,   258,   254,     0,     0,     0,
     432,   433,   434,   435,   436,   254,     0,     0,     0,     0,
     259,     0,     0,   437,     0,   438,     0,     0,     0,     0,
     261,     0,     0,     0,     0,   255,   256,     0,     0,     0,
     257,     0,   860,   258,   255,   256,     0,     0,     0,   257,
       0,   865,   258,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   261,     0,
       0,     0,     0,     0,     0,     0,     0,   261,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
       0,     0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   157,   108,   226,   268,   369,   342,   264,   344,   367,
     346,   448,   393,   450,   300,   452,    74,     0,     7,   461,
     378,    18,    33,     6,    43,   440,   541,   266,    32,    32,
      14,    15,    20,    21,    34,   108,    21,   111,   110,   116,
      46,   113,   114,   124,   134,    19,    29,   134,    31,   134,
      33,    19,    35,    56,   140,   143,   134,    40,    19,   113,
     114,   128,    75,    46,   134,   134,   148,   141,    53,    52,
     164,   407,   134,   167,   134,   134,   140,   134,   134,   165,
     172,   166,   170,   173,   134,    68,   173,    14,    15,   171,
     162,   172,   164,   508,   171,   108,   170,   147,   140,   134,
     170,   141,   102,   103,    20,    21,    89,   171,   545,   112,
     164,   173,   470,   173,   102,   103,   173,   173,   140,   225,
     170,   479,   110,   134,   112,   113,   114,   115,   134,   171,
     118,   171,   371,   148,   134,    57,   167,   134,   141,   142,
     171,   154,   146,   146,   128,   167,   149,   505,   132,   664,
     134,   135,   225,   140,   173,   111,   171,   263,   600,   170,
     164,   164,   220,   151,   152,   153,   140,   152,   171,   141,
     174,   174,   140,   279,   162,   164,   164,   162,   140,   140,
     286,   164,   166,   140,   171,   141,   102,   103,   171,   140,
     263,   143,   298,   299,   110,   143,   460,   113,   114,   115,
     140,   128,   118,   111,   140,   132,   279,   134,   135,   172,
     167,   119,   225,   286,   134,   140,   167,    20,    21,   140,
     172,   171,   111,   173,   134,   298,   299,   167,   140,   335,
     336,   337,   338,   141,   390,   341,   392,   343,   171,   345,
     173,   347,   167,   658,   165,   128,   162,   533,   164,   472,
     263,   140,   141,   668,   143,   167,   143,   146,   140,   365,
     618,   148,   335,   336,   337,   338,   279,   134,   341,   684,
     343,   629,   345,   286,   168,   169,   136,   137,   172,   166,
     661,   387,    11,   165,   170,   298,   299,   134,   669,    20,
      21,   166,   365,    22,    23,   134,   148,   100,   101,   102,
     103,   104,   134,   148,   107,   108,   109,   110,   723,   112,
     113,   114,   115,   469,   387,   118,   143,   120,   121,   171,
     134,   148,   335,   336,   337,   338,   171,   764,   341,   171,
     343,   173,   345,    63,   347,   143,   134,    67,   595,   166,
     148,   677,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   171,   365,   173,    84,    85,    86,    87,   166,   162,
     140,   164,   143,   143,   134,   141,   146,   148,   172,   100,
     101,    20,    21,   104,   387,   172,   107,   108,   109,   110,
     165,   112,   113,   114,   115,   166,   171,   118,   165,   120,
     121,   404,   165,   616,   171,    34,     4,     5,   171,     7,
      34,   624,   136,   137,   138,   511,   512,   166,   514,   172,
     143,   142,   635,   144,   145,   146,   147,   148,   782,     1,
     136,   152,   153,    53,   172,   134,   134,   171,   143,   147,
     147,   162,   171,   164,   147,   147,   147,   147,   511,   512,
     166,   514,   147,   147,   147,   147,   143,   171,   134,   172,
     134,    32,   171,   102,   103,   134,   147,   164,   147,   682,
     166,   110,   147,   112,   113,   114,   115,   166,   166,   118,
     147,   164,   166,   169,   580,   167,   134,    12,   165,   134,
     134,    20,    21,   134,   640,   591,   134,   593,   752,    24,
     116,   171,   128,    36,    19,    30,   171,   171,   511,   512,
     171,   514,   171,   171,   727,   165,   173,   580,   140,   140,
      45,   136,   134,   162,   147,   164,   166,   134,   591,   134,
     148,   148,    13,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,   148,   143,   128,    20,    21,
     783,   100,   101,   102,   103,   104,   172,   580,   107,    39,
     676,   110,   147,   112,   113,   114,   115,   134,   591,   118,
     593,   120,   121,    20,    21,   134,   166,   165,   811,   165,
     167,   814,   165,   165,   134,   165,   165,   148,   148,   134,
     147,    49,   165,   676,   397,   173,   167,   165,   147,   148,
     149,   150,   151,   152,   153,   147,    56,   165,    97,    32,
      59,   167,   167,   162,   833,   164,   236,    57,    75,   276,
     734,     1,   322,   276,   593,   170,   374,   170,   100,   101,
     102,   103,   104,    56,   640,   107,   108,   109,   110,   270,
     112,   113,   114,   115,    69,   667,   118,   340,   120,   121,
     348,   393,   286,   676,   770,   102,   103,   826,    -1,    32,
      -1,    -1,    -1,   110,   239,   112,   113,   114,   115,    -1,
      -1,   118,    -1,    -1,   790,   147,   148,   149,   150,   151,
     152,   153,    -1,    56,   259,    -1,    -1,   770,   111,   112,
     162,    -1,   164,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,   790,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   141,   142,
      -1,   296,   297,   146,    -1,   300,   149,    -1,    -1,    -1,
     846,    -1,    -1,   308,   309,   310,   311,   312,   313,   112,
      -1,   164,    -1,    -1,    -1,   861,    -1,   770,   171,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   846,    -1,    -1,    -1,   790,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,   149,    -1,   861,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,   164,   367,   166,   369,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   378,    -1,   828,    -1,   382,    -1,    -1,
     385,    -1,    -1,    -1,    -1,    -1,   391,    -1,   393,   842,
      -1,    -1,    -1,   846,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   410,   411,    -1,   861,   414,
     415,   416,   417,    -1,   419,    -1,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,    -1,   438,   100,   101,   102,   103,   104,    -1,
      -1,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,   466,    -1,    -1,    -1,   470,    -1,    -1,   473,   474,
      -1,    -1,    -1,   478,   479,    -1,    -1,    -1,    -1,   484,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    12,
      20,    21,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,
     505,    24,    -1,    -1,   509,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,   518,    -1,   520,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,   529,    -1,    -1,   532,   533,    -1,
      -1,   536,    -1,    -1,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
     100,   101,   102,   103,   104,    -1,    -1,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,   121,   597,    -1,    -1,    -1,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   134,   142,   618,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   629,    20,    21,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,    -1,    -1,   642,   643,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   659,    -1,    -1,    -1,    -1,    -1,
     665,    -1,   667,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   680,    -1,    -1,    -1,    -1,
      -1,    -1,   687,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,   119,   120,   121,   122,   123,    -1,
      -1,    -1,    -1,   738,   739,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   748,    -1,    -1,   141,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,    -1,   767,    -1,    -1,    -1,    -1,   162,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,   171,   782,    -1,    -1,
      -1,   786,    -1,    -1,    -1,    -1,   791,    -1,   793,   794,
     795,    -1,    -1,   798,    -1,   800,   801,    -1,    -1,    -1,
       1,   806,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,   837,    -1,    -1,    -1,    37,    38,    -1,    -1,
      41,    42,    -1,    44,   849,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,    -1,    -1,    -1,    -1,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,
     171,   172,   173,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,
      48,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    -1,   156,   157,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,   166,    -1,
     168,   169,   170,   171,   172,   173,    26,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,   149,
     150,   151,    -1,    -1,    -1,    -1,   156,   157,    -1,    14,
      15,    16,    17,    -1,   164,   165,   166,    -1,   168,   169,
     170,    26,   172,    -1,   174,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,   149,   150,   151,    -1,    -1,    -1,
      -1,   156,   157,    -1,    14,    15,    16,    17,    -1,   164,
      -1,   166,    -1,   168,   169,   170,    26,   172,    -1,   174,
      -1,    32,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    56,    -1,    -1,    -1,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,   146,   146,   148,   149,   149,
     150,   151,    -1,    -1,    -1,    -1,   156,   157,    -1,    14,
      15,    16,    17,   164,   164,    -1,   166,    -1,   168,   169,
     170,    26,   172,   174,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,   149,   150,   151,    14,    15,    16,
      17,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   166,    -1,   168,   169,   170,   171,   172,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,   112,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,   174,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,    -1,    -1,    -1,    -1,   156,
     157,    -1,    14,    15,    16,    17,    -1,   164,    -1,   166,
     167,   168,   169,   170,    26,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   149,   150,   151,
      -1,    -1,    -1,    26,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    38,   168,   169,   170,    42,
     172,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,   147,    -1,   149,   150,   151,    -1,
      -1,    -1,    26,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    38,   168,   169,   170,    42,   172,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,   149,   150,   151,    -1,    -1,
      -1,    26,   156,   157,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   165,   166,    38,   168,   169,   170,    42,   172,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,    56,
     148,   149,    -1,    -1,   149,   150,   151,    14,    15,    16,
      17,   156,   157,    -1,    21,    -1,   164,    -1,    -1,    26,
      -1,   166,    -1,   168,   169,   170,   174,   172,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,   112,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,
     157,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      38,   168,   169,   170,    42,   172,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
     141,   142,    -1,    -1,    -1,   146,    56,   148,   149,    -1,
      -1,   149,   150,   151,    14,    15,    16,    17,   156,   157,
      -1,    -1,    -1,   164,    -1,    -1,    26,    -1,   166,   167,
     168,   169,   170,   174,   172,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,
      60,    61,   112,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,   141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,   149,
     150,   151,    -1,    -1,    -1,    26,   156,   157,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    38,   168,   169,
     170,    42,   172,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    56,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,   141,   142,    -1,
      -1,    -1,   146,    56,   148,   149,    -1,    -1,   149,   150,
     151,    14,    15,    16,    17,   156,   157,    -1,    -1,    -1,
     164,    -1,    -1,    26,    -1,   166,   167,   168,   169,   170,
     174,   172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,   112,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,   141,   142,
      -1,    -1,    -1,   146,    -1,   148,   149,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   174,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,   149,   150,   151,    14,
      15,    16,    17,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   166,   167,   168,   169,   170,    -1,   172,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,   112,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,   141,   142,    -1,    -1,
      -1,   146,    -1,   148,   149,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   174,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,   149,   150,   151,    14,    15,    16,
      17,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   166,   167,   168,   169,   170,    -1,   172,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,   112,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,   174,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      38,   168,   169,   170,    42,   172,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,   157,
      32,    -1,    32,    -1,    -1,    -1,    -1,    -1,   166,    38,
     168,   169,   170,    42,   172,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    56,    -1,    56,    -1,    -1,    -1,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,   111,
     112,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,   141,
     142,   141,   142,    -1,   146,    56,   146,   149,   148,   149,
     149,   150,   151,    14,    15,    16,    17,   156,   157,    -1,
      -1,    -1,   164,    -1,   164,    26,    -1,   166,   167,   168,
     169,   170,   174,   172,   174,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,   112,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
     141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,   174,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,   149,   150,
     151,    14,    15,    16,    17,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   166,   167,   168,   169,   170,
      -1,   172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,   112,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,   141,   142,
      -1,    -1,    -1,   146,    -1,   148,   149,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   149,   150,   151,    -1,
      -1,    -1,    26,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    38,   168,   169,   170,    42,   172,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   149,   150,   151,    -1,    -1,
      -1,    26,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    38,   168,   169,   170,    42,   172,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,   149,   150,   151,    -1,    -1,    -1,
      26,   156,   157,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    38,   168,   169,   170,    42,   172,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    20,    21,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,   141,   142,    -1,    -1,    -1,   146,    -1,   148,
     149,    -1,    -1,   149,   150,   151,    -1,    -1,    -1,    -1,
     156,   157,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
     166,    -1,   168,   169,   170,   174,   172,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    20,    21,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      20,    21,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    20,    21,   118,   119,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   162,    -1,   164,   100,   101,   102,
     103,   104,    -1,   171,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   162,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,   100,
     101,   102,   103,   104,    -1,    -1,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,   100,   101,   102,   103,   104,    -1,
     171,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   162,    -1,   164,    -1,
      -1,   167,    -1,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
     167,   100,   101,   102,   103,   104,    -1,    -1,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   162,    -1,   164,    -1,    -1,   167,   100,
     101,   102,   103,   104,    -1,    -1,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,   167,   100,   101,   102,
     103,   104,    -1,    -1,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   162,
      -1,   164,    -1,    -1,   167,   100,   101,   102,   103,   104,
      -1,    -1,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,
      -1,    -1,   167,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
     167,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,   134,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    20,    21,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    20,    21,
     170,   100,   101,   102,   103,   104,    -1,    -1,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,    -1,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   164,   165,    20,    21,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,   170,   100,   101,
     102,   103,   104,    -1,    -1,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,    -1,   118,    56,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    20,    21,    -1,   100,   101,   102,
     103,   104,    -1,   112,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,   146,    -1,   142,
     149,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,   162,
      -1,   164,   165,    -1,    -1,   174,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,   122,   123,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   100,   101,   102,
     103,   104,    -1,    -1,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,    -1,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    20,    21,    -1,    -1,    -1,    -1,    -1,    32,   162,
      -1,   164,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
     104,    -1,    -1,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,    56,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,   142,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    56,
     164,   100,   101,   102,   103,    -1,    -1,    -1,   112,    -1,
      -1,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,   146,    -1,   148,   149,   112,    -1,    -1,    -1,
     149,   150,   151,   152,   153,   112,    -1,    -1,    -1,    -1,
     164,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
     146,    -1,   148,   149,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,   121,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,   156,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   176,     0,     6,    29,    31,    33,    35,    40,    46,
      52,    68,    89,   164,   171,   177,   182,   183,   187,   206,
     210,   225,   275,   280,   281,   285,   320,   322,   134,   201,
     202,   128,   188,   189,   134,   184,   185,   134,   172,   134,
     323,   321,    33,   134,   170,   203,   204,   205,   216,     4,
       5,     7,   211,   284,    57,   274,   141,   140,   143,   140,
      21,    53,   152,   162,   186,   143,   172,   134,   278,   279,
     134,   134,   170,   134,   166,   140,   165,    34,   102,   103,
     134,   209,   134,   283,   172,   276,    14,    15,   128,   132,
     134,   135,   166,   179,   201,   128,   189,   134,   134,   134,
      63,    67,    84,    85,    86,    87,   288,   277,   141,   173,
     278,   172,   172,   134,   202,   204,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   120,   121,   122,   123,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   156,   157,
      34,    34,   166,   207,   143,   282,   172,   273,     1,   136,
     178,    14,    15,   128,   132,   134,   179,   199,   200,   186,
     172,   134,   173,    24,    30,    45,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   216,   287,
     289,   290,   292,   295,   134,   291,   134,   266,   267,   167,
     164,   167,   258,   262,   263,   143,   208,   216,   261,   134,
     173,   272,   286,   171,   136,   137,   140,   167,   277,   141,
     171,   147,   147,   147,   147,   166,   147,   147,   147,   147,
     147,   147,    32,    56,   112,   141,   142,   146,   149,   164,
     171,   174,   171,   143,   171,   202,    46,   269,   286,   167,
     171,   295,   172,   212,    43,   173,   259,   260,   140,   143,
     146,   271,   199,   173,    14,    15,    16,    17,    26,    38,
      42,    44,    47,    51,    68,    90,   102,   103,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   135,   149,   150,
     151,   156,   157,   166,   168,   169,   181,   212,   216,   218,
     227,   234,   243,   244,   245,   248,   249,   250,   255,   287,
     324,   327,   332,   334,   336,   298,   300,   302,   296,   134,
     293,   304,   306,   308,   310,   312,   314,   316,   318,    32,
      32,   146,   164,   174,   165,   255,   134,   173,   295,   173,
     266,   165,   269,   111,   141,   143,   146,   268,   262,   214,
     211,   258,   171,   134,   295,   111,   119,   141,   270,   295,
     324,   166,   166,   147,   147,   166,   147,   147,   255,   255,
      12,   295,    12,   295,   255,   329,   333,   180,   255,   255,
     255,   255,   255,   255,   169,   134,   166,   206,    20,    21,
     100,   101,   102,   103,   104,   107,   108,   109,   110,   112,
     113,   114,   115,   118,   120,   121,   142,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   162,   164,   171,
     166,   295,   295,   295,   295,   167,   291,   295,   207,   295,
     207,   295,   207,   134,   264,   265,   295,   267,   165,   165,
     295,   117,   215,   246,   255,     1,     8,     9,    10,    12,
      25,    27,    28,    37,    41,    48,    50,    54,    55,    91,
     171,   173,   190,   191,   194,   195,   196,   197,   198,   213,
     215,   217,   219,   220,   221,   222,   223,   225,   226,   246,
     247,   255,   269,   141,   171,   270,   215,   255,   166,   134,
     255,   228,   230,   255,   232,   295,   286,   134,   149,   165,
     166,   255,   325,   326,   328,   329,   330,   331,   286,   166,
     326,   331,   124,   171,   173,   137,   138,   178,   167,   147,
     216,   164,   167,   241,   255,   207,    18,   134,   134,   255,
     255,   255,   255,   255,   255,   134,   255,   134,   255,   255,
     255,   255,   255,    21,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   134,   255,   167,   241,   148,
     171,   148,   148,   148,   171,   148,   208,   148,   208,   148,
     208,   143,   148,   171,   148,   171,   141,   268,   169,   244,
     116,    92,    93,    94,    95,    96,    97,    98,    99,   105,
     106,   111,   119,   122,   123,   141,   255,   286,   111,   171,
     215,   255,   212,   255,   255,   171,   128,    54,   255,   111,
     215,   255,    36,   171,   171,   255,   171,   171,   171,    19,
     224,   171,   111,   141,   215,   255,   171,   167,   241,    18,
     147,   255,   167,   295,   295,   167,   295,   148,    19,   268,
     165,   167,   140,   165,   171,   140,   165,   171,    19,   167,
     173,   173,   255,   329,   173,   255,   251,   253,   134,   325,
     140,   167,   208,   147,   166,   165,   134,   143,   165,   167,
     299,   295,   303,   297,   294,   305,   148,   309,   148,   313,
     148,   295,   317,   264,   319,   255,   244,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   212,    19,   215,   255,   171,    13,   212,   143,
     128,   215,   255,   171,   172,   212,    39,   272,   117,   117,
     171,   167,   147,   134,   167,   148,   148,   148,   166,   241,
     255,   326,   134,   165,   325,   255,   165,   330,   241,   326,
     165,   165,   139,   295,   207,   165,   255,   124,   212,   242,
     256,   167,   241,   255,   148,   307,   311,   315,   241,   171,
     212,   171,   214,    11,    22,    23,   192,   193,   255,   255,
     235,   148,   171,   229,   231,   233,   167,   255,   171,   165,
     268,   171,   173,   148,   208,   167,   111,   255,   295,   167,
     301,   171,   173,   212,   255,   295,    18,   255,   134,   255,
     255,   255,   167,   255,   165,   255,   255,   165,   252,   148,
     255,   148,   212,   212,   148,   147,   167,   148,   171,   335,
     335,   216,   254,   257,   193,   236,   237,    18,   255,    49,
     165,   173,   216,   167,   295,   147,   167,   255,   165,   165,
     148,   239,   238,   295,   167,   148,   240,   167
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   175,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   177,   178,   178,   179,   180,
     180,   180,   181,   182,   183,   184,   184,   184,   185,   185,
     186,   186,   187,   188,   188,   189,   189,   190,   191,   191,
     192,   192,   193,   193,   193,   194,   194,   195,   196,   197,
     198,   199,   199,   199,   199,   199,   199,   200,   200,   201,
     201,   201,   201,   201,   201,   201,   201,   202,   202,   203,
     203,   204,   204,   205,   205,   206,   206,   207,   207,   207,
     208,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   210,
     211,   212,   212,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   215,   215,   216,   216,   216,   217,
     218,   218,   218,   218,   219,   220,   221,   221,   221,   221,
     221,   222,   222,   222,   222,   223,   224,   224,   225,   225,
     226,   228,   229,   227,   230,   231,   227,   232,   233,   227,
     234,   234,   234,   235,   236,   234,   237,   238,   234,   239,
     240,   234,   241,   241,   242,   242,   242,   243,   243,   244,
     244,   245,   245,   245,   245,   245,   245,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   247,   247,   248,   249,   249,   250,   251,
     252,   250,   253,   254,   250,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   256,   257,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     258,   258,   259,   259,   260,   261,   261,   261,   262,   262,
     263,   263,   264,   264,   265,   265,   266,   267,   267,   268,
     268,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     271,   271,   272,   272,   272,   272,   272,   273,   273,   274,
     274,   275,   276,   275,   277,   277,   277,   278,   279,   279,
     280,   281,   281,   282,   282,   283,   284,   284,   285,   286,
     286,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   288,   288,   288,   288,
     288,   288,   289,   290,   290,   291,   291,   293,   294,   292,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   297,   295,   295,
     298,   299,   295,   300,   301,   295,   302,   303,   295,   295,
     304,   305,   295,   306,   307,   295,   295,   308,   309,   295,
     310,   311,   295,   295,   312,   313,   295,   314,   315,   295,
     316,   317,   295,   318,   319,   295,   321,   320,   323,   322,
     324,   324,   324,   324,   325,   325,   326,   326,   327,   327,
     327,   327,   327,   328,   328,   329,   330,   330,   331,   331,
     332,   332,   333,   333,   334,   335,   335,   336,   336
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     0,
       2,     4,     3,     2,     2,     1,     3,     3,     2,     4,
       0,     1,     2,     1,     3,     1,     3,     3,     3,     2,
       1,     1,     0,     2,     4,     1,     1,     4,     6,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     4,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       5,     3,     7,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     2,     2,
       2,     0,     2,     2,     3,     2,     1,     3,     2,     2,
       2,     4,     5,     2,     1,     1,     2,     3,     4,     2,
       3,     3,     4,     2,     3,     4,     0,     2,     1,     1,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       5,     8,    10,     0,     0,    10,     0,     0,    13,     0,
       0,    15,     1,     3,     1,     2,     3,     1,     1,     1,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     6,     5,     6,     3,     0,
       0,     8,     0,     0,     9,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       4,     3,     3,     1,     3,     4,     3,     4,     2,     4,
       4,     6,     7,     3,     5,     0,     0,     8,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       0,     3,     0,     1,     3,     0,     3,     3,     2,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     4,     6,     5,     5,     4,     0,     2,     0,
       1,     5,     0,     5,     0,     3,     5,     4,     1,     2,
       4,     5,     7,     0,     2,     2,     1,     1,     6,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     0,     0,     6,
       1,     1,     1,     1,     4,     3,     4,     2,     3,     2,
       3,     2,     2,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     8,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     0,     7,     0,     7,
       1,     1,     1,     1,     3,     5,     1,     3,     5,     4,
       7,     5,     7,     1,     3,     3,     1,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10,    10
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
    case 134: /* "name"  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3197 "ds_parser.cpp"
        break;

    case 178: /* character_sequence  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3203 "ds_parser.cpp"
        break;

    case 179: /* string_constant  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3209 "ds_parser.cpp"
        break;

    case 180: /* string_builder_body  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3215 "ds_parser.cpp"
        break;

    case 181: /* string_builder  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3221 "ds_parser.cpp"
        break;

    case 184: /* require_module_name  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3227 "ds_parser.cpp"
        break;

    case 190: /* expression_label  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3233 "ds_parser.cpp"
        break;

    case 191: /* expression_goto  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3239 "ds_parser.cpp"
        break;

    case 193: /* expression_else  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3245 "ds_parser.cpp"
        break;

    case 195: /* expression_if_then_else  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3251 "ds_parser.cpp"
        break;

    case 196: /* expression_for_loop  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3257 "ds_parser.cpp"
        break;

    case 197: /* expression_while_loop  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3263 "ds_parser.cpp"
        break;

    case 198: /* expression_with  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3269 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3275 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_value_list  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3281 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3287 "ds_parser.cpp"
        break;

    case 202: /* annotation_argument_list  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3293 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration_name  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3299 "ds_parser.cpp"
        break;

    case 204: /* annotation_declaration  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3305 "ds_parser.cpp"
        break;

    case 205: /* annotation_list  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3311 "ds_parser.cpp"
        break;

    case 206: /* optional_annotation_list  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3317 "ds_parser.cpp"
        break;

    case 207: /* optional_function_argument_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3323 "ds_parser.cpp"
        break;

    case 208: /* optional_function_type  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3329 "ds_parser.cpp"
        break;

    case 209: /* function_name  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3335 "ds_parser.cpp"
        break;

    case 212: /* expression_block  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3341 "ds_parser.cpp"
        break;

    case 213: /* expression_any  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3347 "ds_parser.cpp"
        break;

    case 214: /* expressions  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3353 "ds_parser.cpp"
        break;

    case 215: /* expr_pipe  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3359 "ds_parser.cpp"
        break;

    case 216: /* name_in_namespace  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3365 "ds_parser.cpp"
        break;

    case 217: /* expression_delete  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3371 "ds_parser.cpp"
        break;

    case 218: /* expr_new  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3377 "ds_parser.cpp"
        break;

    case 219: /* expression_break  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3383 "ds_parser.cpp"
        break;

    case 220: /* expression_continue  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3389 "ds_parser.cpp"
        break;

    case 221: /* expression_return  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3395 "ds_parser.cpp"
        break;

    case 222: /* expression_yield  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3401 "ds_parser.cpp"
        break;

    case 223: /* expression_try_catch  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3407 "ds_parser.cpp"
        break;

    case 226: /* expression_let  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3413 "ds_parser.cpp"
        break;

    case 227: /* expr_cast  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3419 "ds_parser.cpp"
        break;

    case 234: /* expr_type_info  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3425 "ds_parser.cpp"
        break;

    case 241: /* expr_list  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3431 "ds_parser.cpp"
        break;

    case 242: /* block_or_simple_block  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3437 "ds_parser.cpp"
        break;

    case 244: /* expr_block  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3443 "ds_parser.cpp"
        break;

    case 245: /* expr_numeric_const  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3449 "ds_parser.cpp"
        break;

    case 246: /* expr_assign  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3455 "ds_parser.cpp"
        break;

    case 247: /* expr_assign_pipe  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3461 "ds_parser.cpp"
        break;

    case 248: /* expr_named_call  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3467 "ds_parser.cpp"
        break;

    case 249: /* expr_method_call  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3473 "ds_parser.cpp"
        break;

    case 250: /* func_addr_expr  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3479 "ds_parser.cpp"
        break;

    case 255: /* expr  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3485 "ds_parser.cpp"
        break;

    case 258: /* optional_field_annotation  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3491 "ds_parser.cpp"
        break;

    case 260: /* structure_variable_declaration  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3497 "ds_parser.cpp"
        break;

    case 261: /* struct_variable_declaration_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3503 "ds_parser.cpp"
        break;

    case 262: /* function_argument_declaration  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3509 "ds_parser.cpp"
        break;

    case 263: /* function_argument_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3515 "ds_parser.cpp"
        break;

    case 264: /* tuple_type  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3521 "ds_parser.cpp"
        break;

    case 265: /* tuple_type_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3527 "ds_parser.cpp"
        break;

    case 266: /* variant_type  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3533 "ds_parser.cpp"
        break;

    case 267: /* variant_type_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3539 "ds_parser.cpp"
        break;

    case 269: /* variable_declaration  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3545 "ds_parser.cpp"
        break;

    case 272: /* let_variable_declaration  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3551 "ds_parser.cpp"
        break;

    case 273: /* global_variable_declaration_list  */
#line 116 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3557 "ds_parser.cpp"
        break;

    case 277: /* enum_list  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3563 "ds_parser.cpp"
        break;

    case 282: /* optional_structure_parent  */
#line 112 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3569 "ds_parser.cpp"
        break;

    case 286: /* variable_name_with_pos_list  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3575 "ds_parser.cpp"
        break;

    case 289: /* structure_type_declaration  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3581 "ds_parser.cpp"
        break;

    case 290: /* auto_type_declaration  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3587 "ds_parser.cpp"
        break;

    case 291: /* bitfield_bits  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3593 "ds_parser.cpp"
        break;

    case 292: /* bitfield_type_declaration  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3599 "ds_parser.cpp"
        break;

    case 295: /* type_declaration  */
#line 117 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3605 "ds_parser.cpp"
        break;

    case 324: /* make_decl  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3611 "ds_parser.cpp"
        break;

    case 325: /* make_struct_fields  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3617 "ds_parser.cpp"
        break;

    case 326: /* make_struct_dim  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3623 "ds_parser.cpp"
        break;

    case 327: /* make_struct_decl  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3629 "ds_parser.cpp"
        break;

    case 328: /* make_tuple  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3635 "ds_parser.cpp"
        break;

    case 329: /* make_map_tuple  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3641 "ds_parser.cpp"
        break;

    case 330: /* make_any_tuple  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3647 "ds_parser.cpp"
        break;

    case 331: /* make_dim  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3653 "ds_parser.cpp"
        break;

    case 332: /* make_dim_decl  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3659 "ds_parser.cpp"
        break;

    case 333: /* make_table  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3665 "ds_parser.cpp"
        break;

    case 334: /* make_table_decl  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3671 "ds_parser.cpp"
        break;

    case 335: /* array_comprehension_where  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3677 "ds_parser.cpp"
        break;

    case 336: /* array_comprehension  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3683 "ds_parser.cpp"
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
  case 15:
#line 421 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 3983 "ds_parser.cpp"
    break;

  case 16:
#line 433 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 3989 "ds_parser.cpp"
    break;

  case 17:
#line 434 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 3995 "ds_parser.cpp"
    break;

  case 18:
#line 438 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4001 "ds_parser.cpp"
    break;

  case 19:
#line 442 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4011 "ds_parser.cpp"
    break;

  case 20:
#line 447 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4025 "ds_parser.cpp"
    break;

  case 21:
#line 456 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4035 "ds_parser.cpp"
    break;

  case 22:
#line 464 "ds_parser.ypp"
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
#line 4053 "ds_parser.cpp"
    break;

  case 23:
#line 480 "ds_parser.ypp"
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
#line 4077 "ds_parser.cpp"
    break;

  case 25:
#line 506 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4085 "ds_parser.cpp"
    break;

  case 26:
#line 509 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4096 "ds_parser.cpp"
    break;

  case 27:
#line 515 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4107 "ds_parser.cpp"
    break;

  case 28:
#line 524 "ds_parser.ypp"
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
#line 4133 "ds_parser.cpp"
    break;

  case 29:
#line 545 "ds_parser.ypp"
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
#line 4158 "ds_parser.cpp"
    break;

  case 30:
#line 568 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4164 "ds_parser.cpp"
    break;

  case 31:
#line 569 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4170 "ds_parser.cpp"
    break;

  case 35:
#line 582 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4178 "ds_parser.cpp"
    break;

  case 36:
#line 585 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4186 "ds_parser.cpp"
    break;

  case 37:
#line 591 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4194 "ds_parser.cpp"
    break;

  case 38:
#line 597 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4202 "ds_parser.cpp"
    break;

  case 39:
#line 600 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4210 "ds_parser.cpp"
    break;

  case 40:
#line 606 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4216 "ds_parser.cpp"
    break;

  case 41:
#line 607 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4222 "ds_parser.cpp"
    break;

  case 42:
#line 611 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4228 "ds_parser.cpp"
    break;

  case 43:
#line 612 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4234 "ds_parser.cpp"
    break;

  case 44:
#line 613 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4245 "ds_parser.cpp"
    break;

  case 45:
#line 622 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4251 "ds_parser.cpp"
    break;

  case 46:
#line 623 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4257 "ds_parser.cpp"
    break;

  case 47:
#line 627 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4268 "ds_parser.cpp"
    break;

  case 48:
#line 636 "ds_parser.ypp"
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        for ( const auto & np : *(yyvsp[-4].pNameWithPosList) ) {
            pFor->iterators.push_back(np.first);
            pFor->iteratorsAt.push_back(np.second);
        }
        delete (yyvsp[-4].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4285 "ds_parser.cpp"
    break;

  case 49:
#line 651 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4297 "ds_parser.cpp"
    break;

  case 50:
#line 661 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4308 "ds_parser.cpp"
    break;

  case 51:
#line 670 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4314 "ds_parser.cpp"
    break;

  case 52:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4320 "ds_parser.cpp"
    break;

  case 53:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4326 "ds_parser.cpp"
    break;

  case 54:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4332 "ds_parser.cpp"
    break;

  case 55:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4338 "ds_parser.cpp"
    break;

  case 56:
#line 675 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4344 "ds_parser.cpp"
    break;

  case 57:
#line 679 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4354 "ds_parser.cpp"
    break;

  case 58:
#line 684 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4364 "ds_parser.cpp"
    break;

  case 59:
#line 692 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4370 "ds_parser.cpp"
    break;

  case 60:
#line 693 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4376 "ds_parser.cpp"
    break;

  case 61:
#line 694 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4382 "ds_parser.cpp"
    break;

  case 62:
#line 695 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4388 "ds_parser.cpp"
    break;

  case 63:
#line 696 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4394 "ds_parser.cpp"
    break;

  case 64:
#line 697 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4400 "ds_parser.cpp"
    break;

  case 65:
#line 698 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4406 "ds_parser.cpp"
    break;

  case 66:
#line 699 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4414 "ds_parser.cpp"
    break;

  case 67:
#line 705 "ds_parser.ypp"
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
#line 4432 "ds_parser.cpp"
    break;

  case 68:
#line 718 "ds_parser.ypp"
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
#line 4450 "ds_parser.cpp"
    break;

  case 69:
#line 734 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4456 "ds_parser.cpp"
    break;

  case 70:
#line 735 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4462 "ds_parser.cpp"
    break;

  case 71:
#line 739 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4474 "ds_parser.cpp"
    break;

  case 72:
#line 746 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4488 "ds_parser.cpp"
    break;

  case 73:
#line 758 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4497 "ds_parser.cpp"
    break;

  case 74:
#line 762 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4506 "ds_parser.cpp"
    break;

  case 75:
#line 769 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4512 "ds_parser.cpp"
    break;

  case 76:
#line 770 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4518 "ds_parser.cpp"
    break;

  case 77:
#line 774 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4524 "ds_parser.cpp"
    break;

  case 78:
#line 775 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4530 "ds_parser.cpp"
    break;

  case 79:
#line 776 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4536 "ds_parser.cpp"
    break;

  case 80:
#line 780 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4546 "ds_parser.cpp"
    break;

  case 81:
#line 785 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4555 "ds_parser.cpp"
    break;

  case 82:
#line 792 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4564 "ds_parser.cpp"
    break;

  case 83:
#line 796 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4570 "ds_parser.cpp"
    break;

  case 84:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4576 "ds_parser.cpp"
    break;

  case 85:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4582 "ds_parser.cpp"
    break;

  case 86:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4588 "ds_parser.cpp"
    break;

  case 87:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4594 "ds_parser.cpp"
    break;

  case 88:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4600 "ds_parser.cpp"
    break;

  case 89:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4606 "ds_parser.cpp"
    break;

  case 90:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4612 "ds_parser.cpp"
    break;

  case 91:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4618 "ds_parser.cpp"
    break;

  case 92:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4624 "ds_parser.cpp"
    break;

  case 93:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4630 "ds_parser.cpp"
    break;

  case 94:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4636 "ds_parser.cpp"
    break;

  case 95:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4642 "ds_parser.cpp"
    break;

  case 96:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4648 "ds_parser.cpp"
    break;

  case 97:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4654 "ds_parser.cpp"
    break;

  case 98:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4660 "ds_parser.cpp"
    break;

  case 99:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4666 "ds_parser.cpp"
    break;

  case 100:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4672 "ds_parser.cpp"
    break;

  case 101:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4678 "ds_parser.cpp"
    break;

  case 102:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4684 "ds_parser.cpp"
    break;

  case 103:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4690 "ds_parser.cpp"
    break;

  case 104:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4696 "ds_parser.cpp"
    break;

  case 105:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4702 "ds_parser.cpp"
    break;

  case 106:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4708 "ds_parser.cpp"
    break;

  case 107:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4714 "ds_parser.cpp"
    break;

  case 108:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4720 "ds_parser.cpp"
    break;

  case 109:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4726 "ds_parser.cpp"
    break;

  case 110:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4732 "ds_parser.cpp"
    break;

  case 111:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4738 "ds_parser.cpp"
    break;

  case 112:
#line 825 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4744 "ds_parser.cpp"
    break;

  case 113:
#line 826 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4750 "ds_parser.cpp"
    break;

  case 114:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4756 "ds_parser.cpp"
    break;

  case 115:
#line 828 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4762 "ds_parser.cpp"
    break;

  case 116:
#line 829 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4768 "ds_parser.cpp"
    break;

  case 117:
#line 830 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4774 "ds_parser.cpp"
    break;

  case 118:
#line 831 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4780 "ds_parser.cpp"
    break;

  case 119:
#line 835 "ds_parser.ypp"
    {
        DAS_ASSERT((yyvsp[0].pFuncDecl) && "function declaration must return function");
        if ( (yyvsp[-1].faList) ) {
            for ( auto pA : *(yyvsp[-1].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply((yyvsp[0].pFuncDecl), *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't apply annotation\n" + err,
                                tokAt((yylsp[-1])), CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("functions are only allowed function annotations",
                            tokAt((yylsp[-1])), CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( (yyvsp[0].pFuncDecl)->annotations, *(yyvsp[-1].faList) );
            delete (yyvsp[-1].faList);
        }
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            if ( !g_Program->addGeneric((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("generic function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        } else {
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
    }
#line 4820 "ds_parser.cpp"
    break;

  case 120:
#line 874 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-3]));
        pFunction->atDecl = tokRangeAt((yylsp[-4]), (yylsp[0]));
        pFunction->name = *(yyvsp[-3].s);
        pFunction->body = ExpressionPtr((yyvsp[0].pExpression));
        pFunction->result = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name_at.first) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.first;
                            pVar->at = name_at.second;
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
                            das_yyerror("function argument is already declared " + name_at.first,name_at.second,
                                CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        delete (yyvsp[-3].s);
        (yyval.pFuncDecl) = pFunction.orphan();
    }
#line 4862 "ds_parser.cpp"
    break;

  case 121:
#line 914 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4870 "ds_parser.cpp"
    break;

  case 122:
#line 917 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4882 "ds_parser.cpp"
    break;

  case 123:
#line 927 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4888 "ds_parser.cpp"
    break;

  case 124:
#line 928 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4894 "ds_parser.cpp"
    break;

  case 125:
#line 929 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4900 "ds_parser.cpp"
    break;

  case 126:
#line 930 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4906 "ds_parser.cpp"
    break;

  case 127:
#line 931 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4912 "ds_parser.cpp"
    break;

  case 128:
#line 932 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4918 "ds_parser.cpp"
    break;

  case 129:
#line 933 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4924 "ds_parser.cpp"
    break;

  case 130:
#line 934 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4930 "ds_parser.cpp"
    break;

  case 131:
#line 935 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4936 "ds_parser.cpp"
    break;

  case 132:
#line 936 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4942 "ds_parser.cpp"
    break;

  case 133:
#line 937 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4948 "ds_parser.cpp"
    break;

  case 134:
#line 938 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4954 "ds_parser.cpp"
    break;

  case 135:
#line 939 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4960 "ds_parser.cpp"
    break;

  case 136:
#line 940 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4966 "ds_parser.cpp"
    break;

  case 137:
#line 941 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4972 "ds_parser.cpp"
    break;

  case 138:
#line 942 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4978 "ds_parser.cpp"
    break;

  case 139:
#line 943 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4984 "ds_parser.cpp"
    break;

  case 140:
#line 944 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4990 "ds_parser.cpp"
    break;

  case 141:
#line 948 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5000 "ds_parser.cpp"
    break;

  case 142:
#line 953 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5011 "ds_parser.cpp"
    break;

  case 143:
#line 959 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5019 "ds_parser.cpp"
    break;

  case 144:
#line 965 "ds_parser.ypp"
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
#line 5035 "ds_parser.cpp"
    break;

  case 145:
#line 976 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5043 "ds_parser.cpp"
    break;

  case 146:
#line 982 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5049 "ds_parser.cpp"
    break;

  case 147:
#line 983 "ds_parser.ypp"
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
#line 5065 "ds_parser.cpp"
    break;

  case 148:
#line 994 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5071 "ds_parser.cpp"
    break;

  case 149:
#line 998 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5079 "ds_parser.cpp"
    break;

  case 150:
#line 1004 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5087 "ds_parser.cpp"
    break;

  case 151:
#line 1007 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5095 "ds_parser.cpp"
    break;

  case 152:
#line 1010 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5104 "ds_parser.cpp"
    break;

  case 153:
#line 1014 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5112 "ds_parser.cpp"
    break;

  case 154:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5118 "ds_parser.cpp"
    break;

  case 155:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5124 "ds_parser.cpp"
    break;

  case 156:
#line 1028 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5132 "ds_parser.cpp"
    break;

  case 157:
#line 1031 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5140 "ds_parser.cpp"
    break;

  case 158:
#line 1034 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5150 "ds_parser.cpp"
    break;

  case 159:
#line 1039 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5158 "ds_parser.cpp"
    break;

  case 160:
#line 1042 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5168 "ds_parser.cpp"
    break;

  case 161:
#line 1050 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5176 "ds_parser.cpp"
    break;

  case 162:
#line 1053 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5186 "ds_parser.cpp"
    break;

  case 163:
#line 1058 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5194 "ds_parser.cpp"
    break;

  case 164:
#line 1061 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5204 "ds_parser.cpp"
    break;

  case 165:
#line 1069 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5212 "ds_parser.cpp"
    break;

  case 166:
#line 1075 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5218 "ds_parser.cpp"
    break;

  case 167:
#line 1076 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5224 "ds_parser.cpp"
    break;

  case 168:
#line 1080 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5230 "ds_parser.cpp"
    break;

  case 169:
#line 1081 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5236 "ds_parser.cpp"
    break;

  case 170:
#line 1085 "ds_parser.ypp"
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-2]));
        pLet->inScope = (yyvsp[-1].b);
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name_at : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name_at.first) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.first;
                    pVar->at = name_at.second;
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
                    das_yyerror("local variable is already declared " + name_at.first,name_at.second,
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 5272 "ds_parser.cpp"
    break;

  case 171:
#line 1119 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5278 "ds_parser.cpp"
    break;

  case 172:
#line 1119 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5284 "ds_parser.cpp"
    break;

  case 173:
#line 1119 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5292 "ds_parser.cpp"
    break;

  case 174:
#line 1122 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5298 "ds_parser.cpp"
    break;

  case 175:
#line 1122 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5304 "ds_parser.cpp"
    break;

  case 176:
#line 1122 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5314 "ds_parser.cpp"
    break;

  case 177:
#line 1127 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5320 "ds_parser.cpp"
    break;

  case 178:
#line 1127 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5326 "ds_parser.cpp"
    break;

  case 179:
#line 1127 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5336 "ds_parser.cpp"
    break;

  case 180:
#line 1135 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5345 "ds_parser.cpp"
    break;

  case 181:
#line 1139 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5355 "ds_parser.cpp"
    break;

  case 182:
#line 1144 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5366 "ds_parser.cpp"
    break;

  case 183:
#line 1150 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5372 "ds_parser.cpp"
    break;

  case 184:
#line 1150 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5378 "ds_parser.cpp"
    break;

  case 185:
#line 1150 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5387 "ds_parser.cpp"
    break;

  case 186:
#line 1154 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5393 "ds_parser.cpp"
    break;

  case 187:
#line 1154 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5399 "ds_parser.cpp"
    break;

  case 188:
#line 1154 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5409 "ds_parser.cpp"
    break;

  case 189:
#line 1159 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5415 "ds_parser.cpp"
    break;

  case 190:
#line 1159 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5421 "ds_parser.cpp"
    break;

  case 191:
#line 1159 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5432 "ds_parser.cpp"
    break;

  case 192:
#line 1168 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5440 "ds_parser.cpp"
    break;

  case 193:
#line 1171 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5448 "ds_parser.cpp"
    break;

  case 194:
#line 1177 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5454 "ds_parser.cpp"
    break;

  case 195:
#line 1178 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5466 "ds_parser.cpp"
    break;

  case 196:
#line 1185 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5479 "ds_parser.cpp"
    break;

  case 197:
#line 1196 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5485 "ds_parser.cpp"
    break;

  case 198:
#line 1197 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5491 "ds_parser.cpp"
    break;

  case 199:
#line 1201 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5501 "ds_parser.cpp"
    break;

  case 200:
#line 1207 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-4].b));
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name_at.first) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.first;
                            pVar->at = name_at.second;
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
                            das_yyerror("block argument is already declared " + name_at.first,
                                name_at.second,CompilationError::argument_already_declared);
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
#line 5557 "ds_parser.cpp"
    break;

  case 201:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5563 "ds_parser.cpp"
    break;

  case 202:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5569 "ds_parser.cpp"
    break;

  case 203:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5575 "ds_parser.cpp"
    break;

  case 204:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5581 "ds_parser.cpp"
    break;

  case 205:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5587 "ds_parser.cpp"
    break;

  case 206:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5593 "ds_parser.cpp"
    break;

  case 207:
#line 1270 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5599 "ds_parser.cpp"
    break;

  case 208:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5605 "ds_parser.cpp"
    break;

  case 209:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5611 "ds_parser.cpp"
    break;

  case 210:
#line 1273 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5617 "ds_parser.cpp"
    break;

  case 211:
#line 1274 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5623 "ds_parser.cpp"
    break;

  case 212:
#line 1275 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5629 "ds_parser.cpp"
    break;

  case 213:
#line 1276 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5635 "ds_parser.cpp"
    break;

  case 214:
#line 1277 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5641 "ds_parser.cpp"
    break;

  case 215:
#line 1278 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5647 "ds_parser.cpp"
    break;

  case 216:
#line 1279 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5653 "ds_parser.cpp"
    break;

  case 217:
#line 1280 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5659 "ds_parser.cpp"
    break;

  case 218:
#line 1281 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5665 "ds_parser.cpp"
    break;

  case 219:
#line 1282 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5671 "ds_parser.cpp"
    break;

  case 220:
#line 1283 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5677 "ds_parser.cpp"
    break;

  case 221:
#line 1284 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5683 "ds_parser.cpp"
    break;

  case 222:
#line 1285 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5689 "ds_parser.cpp"
    break;

  case 223:
#line 1289 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5695 "ds_parser.cpp"
    break;

  case 224:
#line 1290 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5701 "ds_parser.cpp"
    break;

  case 225:
#line 1294 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5713 "ds_parser.cpp"
    break;

  case 226:
#line 1305 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5723 "ds_parser.cpp"
    break;

  case 227:
#line 1310 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5735 "ds_parser.cpp"
    break;

  case 228:
#line 1320 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5744 "ds_parser.cpp"
    break;

  case 229:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5750 "ds_parser.cpp"
    break;

  case 230:
#line 1324 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5756 "ds_parser.cpp"
    break;

  case 231:
#line 1324 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5768 "ds_parser.cpp"
    break;

  case 232:
#line 1331 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5774 "ds_parser.cpp"
    break;

  case 233:
#line 1331 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5780 "ds_parser.cpp"
    break;

  case 234:
#line 1331 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-7])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
		(yyval.pExpression) = expr;
    }
#line 5796 "ds_parser.cpp"
    break;

  case 235:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5802 "ds_parser.cpp"
    break;

  case 236:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5808 "ds_parser.cpp"
    break;

  case 237:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5814 "ds_parser.cpp"
    break;

  case 238:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5820 "ds_parser.cpp"
    break;

  case 239:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5826 "ds_parser.cpp"
    break;

  case 240:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5832 "ds_parser.cpp"
    break;

  case 241:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5838 "ds_parser.cpp"
    break;

  case 242:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5844 "ds_parser.cpp"
    break;

  case 243:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5850 "ds_parser.cpp"
    break;

  case 244:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5856 "ds_parser.cpp"
    break;

  case 245:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5862 "ds_parser.cpp"
    break;

  case 246:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5868 "ds_parser.cpp"
    break;

  case 247:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5874 "ds_parser.cpp"
    break;

  case 248:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5880 "ds_parser.cpp"
    break;

  case 249:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5886 "ds_parser.cpp"
    break;

  case 250:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5892 "ds_parser.cpp"
    break;

  case 251:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5898 "ds_parser.cpp"
    break;

  case 252:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5904 "ds_parser.cpp"
    break;

  case 253:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5910 "ds_parser.cpp"
    break;

  case 254:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5916 "ds_parser.cpp"
    break;

  case 255:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5922 "ds_parser.cpp"
    break;

  case 256:
#line 1366 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5928 "ds_parser.cpp"
    break;

  case 257:
#line 1367 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5934 "ds_parser.cpp"
    break;

  case 258:
#line 1368 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5940 "ds_parser.cpp"
    break;

  case 259:
#line 1369 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5946 "ds_parser.cpp"
    break;

  case 260:
#line 1370 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5952 "ds_parser.cpp"
    break;

  case 261:
#line 1371 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5958 "ds_parser.cpp"
    break;

  case 262:
#line 1372 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5964 "ds_parser.cpp"
    break;

  case 263:
#line 1373 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5970 "ds_parser.cpp"
    break;

  case 264:
#line 1374 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5976 "ds_parser.cpp"
    break;

  case 265:
#line 1375 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5982 "ds_parser.cpp"
    break;

  case 266:
#line 1376 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5988 "ds_parser.cpp"
    break;

  case 267:
#line 1377 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5994 "ds_parser.cpp"
    break;

  case 268:
#line 1378 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6000 "ds_parser.cpp"
    break;

  case 269:
#line 1379 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6006 "ds_parser.cpp"
    break;

  case 270:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6012 "ds_parser.cpp"
    break;

  case 271:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6018 "ds_parser.cpp"
    break;

  case 272:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6024 "ds_parser.cpp"
    break;

  case 273:
#line 1383 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6030 "ds_parser.cpp"
    break;

  case 274:
#line 1384 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6039 "ds_parser.cpp"
    break;

  case 275:
#line 1388 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6048 "ds_parser.cpp"
    break;

  case 276:
#line 1392 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6056 "ds_parser.cpp"
    break;

  case 277:
#line 1395 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6064 "ds_parser.cpp"
    break;

  case 278:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6070 "ds_parser.cpp"
    break;

  case 279:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6076 "ds_parser.cpp"
    break;

  case 280:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6082 "ds_parser.cpp"
    break;

  case 281:
#line 1401 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6092 "ds_parser.cpp"
    break;

  case 282:
#line 1406 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6102 "ds_parser.cpp"
    break;

  case 283:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6108 "ds_parser.cpp"
    break;

  case 284:
#line 1412 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6116 "ds_parser.cpp"
    break;

  case 285:
#line 1415 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6122 "ds_parser.cpp"
    break;

  case 286:
#line 1415 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6128 "ds_parser.cpp"
    break;

  case 287:
#line 1415 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6136 "ds_parser.cpp"
    break;

  case 288:
#line 1418 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6145 "ds_parser.cpp"
    break;

  case 289:
#line 1422 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6154 "ds_parser.cpp"
    break;

  case 290:
#line 1426 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6163 "ds_parser.cpp"
    break;

  case 291:
#line 1430 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6169 "ds_parser.cpp"
    break;

  case 292:
#line 1431 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6175 "ds_parser.cpp"
    break;

  case 293:
#line 1432 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6181 "ds_parser.cpp"
    break;

  case 294:
#line 1433 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6187 "ds_parser.cpp"
    break;

  case 295:
#line 1434 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6193 "ds_parser.cpp"
    break;

  case 296:
#line 1435 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6199 "ds_parser.cpp"
    break;

  case 297:
#line 1436 "ds_parser.ypp"
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
#line 6214 "ds_parser.cpp"
    break;

  case 298:
#line 1446 "ds_parser.ypp"
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
#line 6229 "ds_parser.cpp"
    break;

  case 299:
#line 1456 "ds_parser.ypp"
    {
        Enumeration * pEnum = nullptr;
		Expression * resConst = nullptr;
        auto enums = g_Program->findEnum(*(yyvsp[-1].s));
        auto aliases = g_Program->findAlias(*(yyvsp[-1].s));
        if ( enums.size()+aliases.size() > 1 ) {
            string candidates;
            if ( enums.size() ) candidates += g_Program->describeCandidates(enums);
            if ( aliases.size() ) candidates += g_Program->describeCandidates(aliases);
            das_yyerror("too many options for " + *(yyvsp[-1].s) + "\n" + candidates, tokAt((yylsp[-1])),
                CompilationError::type_not_found);
        } else if ( enums.size()==0 && aliases.size()==0 ) {
            das_yyerror("enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::type_not_found);
        } else if ( enums.size()==1 ) {
            pEnum = enums.back().get();
        } else if ( aliases.size()==1 ) {
            auto alias = aliases.back();
            if ( alias->isEnum() ) {
                pEnum = alias->enumType;
            } else if ( alias->isBitfield() ) {
                int bit = alias->findArgumentIndex(*(yyvsp[0].s));
                if ( bit!=-1 ) {
                    auto td = make_smart<TypeDecl>(*alias);
                    td->ref = false;
                    auto bitConst = new ExprConstBitfield(tokAt((yylsp[0])), 1u << bit);
                    bitConst->bitfieldType = make_smart<TypeDecl>(*alias);
                    resConst = bitConst;
                } else {
                    das_yyerror("enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                        CompilationError::bitfield_not_found);
                }
            }
        }
        if ( pEnum ) {
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
				auto td = make_smart<TypeDecl>(pEnum);
                resConst = new ExprConstEnumeration(tokAt((yylsp[0])), *(yyvsp[0].s), td);
            } else {
                das_yyerror("enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt((yylsp[0])),
                    CompilationError::enumeration_not_found);
            }
        }
		if ( resConst ) {
			(yyval.pExpression) = resConst;
		} else {
			(yyval.pExpression) = new ExprConstInt(0);	// dummy
		}
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 6286 "ds_parser.cpp"
    break;

  case 300:
#line 1511 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6292 "ds_parser.cpp"
    break;

  case 301:
#line 1512 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-1].aaList); }
#line 6298 "ds_parser.cpp"
    break;

  case 302:
#line 1516 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6304 "ds_parser.cpp"
    break;

  case 303:
#line 1517 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6310 "ds_parser.cpp"
    break;

  case 304:
#line 1521 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6320 "ds_parser.cpp"
    break;

  case 305:
#line 1529 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6328 "ds_parser.cpp"
    break;

  case 306:
#line 1532 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6337 "ds_parser.cpp"
    break;

  case 307:
#line 1536 "ds_parser.ypp"
    {
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[0].pFuncDecl)->getMangledName(),
                (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            if ( (yyvsp[0].pFuncDecl)->name != g_thisStructure->name ) {
                auto varName = (yyvsp[0].pFuncDecl)->name;
                (yyvsp[0].pFuncDecl)->name = g_thisStructure->name + "`" + (yyvsp[0].pFuncDecl)->name;
                auto vars = new vector<pair<string,LineInfo>>();
                vars->emplace_back(make_pair(varName,(yyvsp[0].pFuncDecl)->at));
                Expression * finit = new ExprAddr((yyvsp[0].pFuncDecl)->at, "_::" + (yyvsp[0].pFuncDecl)->name);
                if ( (yyvsp[-1].b) ) {
                    finit = new ExprCast((yyvsp[0].pFuncDecl)->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = (yyvsp[-1].b);
                (yyvsp[-2].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure);
            } else {
                if ( (yyvsp[-1].b) ) {
                    das_yyerror("can't override constructor " + (yyvsp[0].pFuncDecl)->getMangledName(),
                        (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                modifyToConstructor((yyvsp[0].pFuncDecl), g_thisStructure);
            }
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
        }
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
    }
#line 6378 "ds_parser.cpp"
    break;

  case 308:
#line 1575 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6388 "ds_parser.cpp"
    break;

  case 309:
#line 1580 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6398 "ds_parser.cpp"
    break;

  case 310:
#line 1588 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6404 "ds_parser.cpp"
    break;

  case 311:
#line 1589 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6410 "ds_parser.cpp"
    break;

  case 312:
#line 1593 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6418 "ds_parser.cpp"
    break;

  case 313:
#line 1596 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6429 "ds_parser.cpp"
    break;

  case 314:
#line 1605 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6435 "ds_parser.cpp"
    break;

  case 315:
#line 1606 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6441 "ds_parser.cpp"
    break;

  case 316:
#line 1610 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6452 "ds_parser.cpp"
    break;

  case 317:
#line 1619 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6458 "ds_parser.cpp"
    break;

  case 318:
#line 1620 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6464 "ds_parser.cpp"
    break;

  case 319:
#line 1625 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6470 "ds_parser.cpp"
    break;

  case 320:
#line 1626 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6476 "ds_parser.cpp"
    break;

  case 321:
#line 1630 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6487 "ds_parser.cpp"
    break;

  case 322:
#line 1636 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6498 "ds_parser.cpp"
    break;

  case 323:
#line 1642 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6506 "ds_parser.cpp"
    break;

  case 324:
#line 1645 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6515 "ds_parser.cpp"
    break;

  case 325:
#line 1649 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6526 "ds_parser.cpp"
    break;

  case 326:
#line 1655 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6537 "ds_parser.cpp"
    break;

  case 327:
#line 1664 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6543 "ds_parser.cpp"
    break;

  case 328:
#line 1665 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6549 "ds_parser.cpp"
    break;

  case 329:
#line 1666 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6555 "ds_parser.cpp"
    break;

  case 330:
#line 1670 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6561 "ds_parser.cpp"
    break;

  case 331:
#line 1671 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6567 "ds_parser.cpp"
    break;

  case 332:
#line 1675 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6575 "ds_parser.cpp"
    break;

  case 333:
#line 1678 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6585 "ds_parser.cpp"
    break;

  case 334:
#line 1683 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6595 "ds_parser.cpp"
    break;

  case 335:
#line 1688 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6608 "ds_parser.cpp"
    break;

  case 336:
#line 1696 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6621 "ds_parser.cpp"
    break;

  case 337:
#line 1707 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6629 "ds_parser.cpp"
    break;

  case 338:
#line 1710 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6638 "ds_parser.cpp"
    break;

  case 339:
#line 1717 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6644 "ds_parser.cpp"
    break;

  case 340:
#line 1718 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6650 "ds_parser.cpp"
    break;

  case 341:
#line 1722 "ds_parser.ypp"
    {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.first;
                    pVar->at = name_at.second;
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
                        das_yyerror("global variable is already declared " + name_at.first,name_at.second,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 6682 "ds_parser.cpp"
    break;

  case 342:
#line 1749 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6691 "ds_parser.cpp"
    break;

  case 343:
#line 1752 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6702 "ds_parser.cpp"
    break;

  case 344:
#line 1761 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6710 "ds_parser.cpp"
    break;

  case 345:
#line 1764 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6724 "ds_parser.cpp"
    break;

  case 346:
#line 1773 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6738 "ds_parser.cpp"
    break;

  case 347:
#line 1786 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6752 "ds_parser.cpp"
    break;

  case 351:
#line 1807 "ds_parser.ypp"
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
#line 6768 "ds_parser.cpp"
    break;

  case 352:
#line 1818 "ds_parser.ypp"
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
#line 6785 "ds_parser.cpp"
    break;

  case 353:
#line 1833 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6791 "ds_parser.cpp"
    break;

  case 354:
#line 1834 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6797 "ds_parser.cpp"
    break;

  case 355:
#line 1838 "ds_parser.ypp"
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
                g_thisStructure = pStruct.get();
            }
        } else {
            (yyval.pStructure) = nullptr;
        }
        delete (yyvsp[-1].s);
    }
#line 6839 "ds_parser.cpp"
    break;

  case 356:
#line 1878 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6845 "ds_parser.cpp"
    break;

  case 357:
#line 1879 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6851 "ds_parser.cpp"
    break;

  case 358:
#line 1883 "ds_parser.ypp"
    {
        if ( (yyvsp[-3].pStructure) ) {
            auto pStruct = (yyvsp[-3].pStructure);
            pStruct->at = tokAt((yylsp[-3]));
            pStruct->isClass = (yyvsp[-4].b);
            if ( pStruct->parent && pStruct->parent->isClass != pStruct->isClass ) {
                if ( pStruct->isClass ) {
                    das_yyerror("class can only derive from class", pStruct->at,
                        CompilationError::invalid_override);
                } else {
                    das_yyerror("struct can only derive from struct", pStruct->at,
                        CompilationError::invalid_override);
                }
            }
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name_at.first);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror("structure field is not overriding anything, "+name_at.first,name_at.second,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name_at.first, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, name_at.second);
                        }
                    } else {
                        if ( pDecl->override ) {
                                auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                                oldFd->init = init;
                                oldFd->parentType = oldFd->type->isAuto();
                        } else {
                            das_yyerror("structure field is already declared "+name_at.first
                                +", use override to replace initial value instead",name_at.second,
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
        g_thisStructure = nullptr;
    }
#line 6930 "ds_parser.cpp"
    break;

  case 359:
#line 1960 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6942 "ds_parser.cpp"
    break;

  case 360:
#line 1967 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 6953 "ds_parser.cpp"
    break;

  case 361:
#line 1976 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6959 "ds_parser.cpp"
    break;

  case 362:
#line 1977 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6965 "ds_parser.cpp"
    break;

  case 363:
#line 1978 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6971 "ds_parser.cpp"
    break;

  case 364:
#line 1979 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6977 "ds_parser.cpp"
    break;

  case 365:
#line 1980 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6983 "ds_parser.cpp"
    break;

  case 366:
#line 1981 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6989 "ds_parser.cpp"
    break;

  case 367:
#line 1982 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 6995 "ds_parser.cpp"
    break;

  case 368:
#line 1983 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7001 "ds_parser.cpp"
    break;

  case 369:
#line 1984 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7007 "ds_parser.cpp"
    break;

  case 370:
#line 1985 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7013 "ds_parser.cpp"
    break;

  case 371:
#line 1986 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7019 "ds_parser.cpp"
    break;

  case 372:
#line 1987 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7025 "ds_parser.cpp"
    break;

  case 373:
#line 1988 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7031 "ds_parser.cpp"
    break;

  case 374:
#line 1989 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7037 "ds_parser.cpp"
    break;

  case 375:
#line 1990 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7043 "ds_parser.cpp"
    break;

  case 376:
#line 1991 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7049 "ds_parser.cpp"
    break;

  case 377:
#line 1992 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7055 "ds_parser.cpp"
    break;

  case 378:
#line 1993 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7061 "ds_parser.cpp"
    break;

  case 379:
#line 1994 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7067 "ds_parser.cpp"
    break;

  case 380:
#line 1995 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7073 "ds_parser.cpp"
    break;

  case 381:
#line 1996 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7079 "ds_parser.cpp"
    break;

  case 382:
#line 1997 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7085 "ds_parser.cpp"
    break;

  case 383:
#line 1998 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7091 "ds_parser.cpp"
    break;

  case 384:
#line 1999 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7097 "ds_parser.cpp"
    break;

  case 385:
#line 2000 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7103 "ds_parser.cpp"
    break;

  case 386:
#line 2004 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7109 "ds_parser.cpp"
    break;

  case 387:
#line 2005 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7115 "ds_parser.cpp"
    break;

  case 388:
#line 2006 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7121 "ds_parser.cpp"
    break;

  case 389:
#line 2007 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7127 "ds_parser.cpp"
    break;

  case 390:
#line 2008 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7133 "ds_parser.cpp"
    break;

  case 391:
#line 2009 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7139 "ds_parser.cpp"
    break;

  case 392:
#line 2013 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7152 "ds_parser.cpp"
    break;

  case 393:
#line 2024 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7161 "ds_parser.cpp"
    break;

  case 394:
#line 2028 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7173 "ds_parser.cpp"
    break;

  case 395:
#line 2038 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7185 "ds_parser.cpp"
    break;

  case 396:
#line 2045 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7196 "ds_parser.cpp"
    break;

  case 397:
#line 2054 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7202 "ds_parser.cpp"
    break;

  case 398:
#line 2054 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7208 "ds_parser.cpp"
    break;

  case 399:
#line 2054 "ds_parser.ypp"
    {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror("only 32 different bits allowed",tokAt((yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
#line 7223 "ds_parser.cpp"
    break;

  case 400:
#line 2067 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7229 "ds_parser.cpp"
    break;

  case 401:
#line 2068 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7235 "ds_parser.cpp"
    break;

  case 402:
#line 2069 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7241 "ds_parser.cpp"
    break;

  case 403:
#line 2070 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7247 "ds_parser.cpp"
    break;

  case 404:
#line 2071 "ds_parser.ypp"
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
#line 7266 "ds_parser.cpp"
    break;

  case 405:
#line 2085 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7277 "ds_parser.cpp"
    break;

  case 406:
#line 2091 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7286 "ds_parser.cpp"
    break;

  case 407:
#line 2095 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7296 "ds_parser.cpp"
    break;

  case 408:
#line 2100 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7306 "ds_parser.cpp"
    break;

  case 409:
#line 2105 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7316 "ds_parser.cpp"
    break;

  case 410:
#line 2110 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7326 "ds_parser.cpp"
    break;

  case 411:
#line 2115 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7335 "ds_parser.cpp"
    break;

  case 412:
#line 2119 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7344 "ds_parser.cpp"
    break;

  case 413:
#line 2123 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7354 "ds_parser.cpp"
    break;

  case 414:
#line 2128 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7363 "ds_parser.cpp"
    break;

  case 415:
#line 2132 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7373 "ds_parser.cpp"
    break;

  case 416:
#line 2137 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7379 "ds_parser.cpp"
    break;

  case 417:
#line 2137 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7385 "ds_parser.cpp"
    break;

  case 418:
#line 2137 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7396 "ds_parser.cpp"
    break;

  case 419:
#line 2143 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7408 "ds_parser.cpp"
    break;

  case 420:
#line 2150 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7414 "ds_parser.cpp"
    break;

  case 421:
#line 2150 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7420 "ds_parser.cpp"
    break;

  case 422:
#line 2150 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7430 "ds_parser.cpp"
    break;

  case 423:
#line 2155 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7436 "ds_parser.cpp"
    break;

  case 424:
#line 2155 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7442 "ds_parser.cpp"
    break;

  case 425:
#line 2155 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7453 "ds_parser.cpp"
    break;

  case 426:
#line 2161 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7459 "ds_parser.cpp"
    break;

  case 427:
#line 2161 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7465 "ds_parser.cpp"
    break;

  case 428:
#line 2161 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7475 "ds_parser.cpp"
    break;

  case 429:
#line 2166 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7484 "ds_parser.cpp"
    break;

  case 430:
#line 2170 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7490 "ds_parser.cpp"
    break;

  case 431:
#line 2170 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7496 "ds_parser.cpp"
    break;

  case 432:
#line 2170 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7506 "ds_parser.cpp"
    break;

  case 433:
#line 2175 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7512 "ds_parser.cpp"
    break;

  case 434:
#line 2175 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7518 "ds_parser.cpp"
    break;

  case 435:
#line 2175 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7532 "ds_parser.cpp"
    break;

  case 436:
#line 2184 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7541 "ds_parser.cpp"
    break;

  case 437:
#line 2188 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7547 "ds_parser.cpp"
    break;

  case 438:
#line 2188 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7553 "ds_parser.cpp"
    break;

  case 439:
#line 2188 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7563 "ds_parser.cpp"
    break;

  case 440:
#line 2193 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7569 "ds_parser.cpp"
    break;

  case 441:
#line 2193 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7575 "ds_parser.cpp"
    break;

  case 442:
#line 2193 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7589 "ds_parser.cpp"
    break;

  case 443:
#line 2202 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7598 "ds_parser.cpp"
    break;

  case 444:
#line 2206 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7604 "ds_parser.cpp"
    break;

  case 445:
#line 2206 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7610 "ds_parser.cpp"
    break;

  case 446:
#line 2206 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7620 "ds_parser.cpp"
    break;

  case 447:
#line 2211 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7626 "ds_parser.cpp"
    break;

  case 448:
#line 2211 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7632 "ds_parser.cpp"
    break;

  case 449:
#line 2211 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7646 "ds_parser.cpp"
    break;

  case 450:
#line 2220 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7652 "ds_parser.cpp"
    break;

  case 451:
#line 2220 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7658 "ds_parser.cpp"
    break;

  case 452:
#line 2220 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7669 "ds_parser.cpp"
    break;

  case 453:
#line 2226 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7675 "ds_parser.cpp"
    break;

  case 454:
#line 2226 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7681 "ds_parser.cpp"
    break;

  case 455:
#line 2226 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7692 "ds_parser.cpp"
    break;

  case 456:
#line 2235 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7698 "ds_parser.cpp"
    break;

  case 457:
#line 2235 "ds_parser.ypp"
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
#line 7715 "ds_parser.cpp"
    break;

  case 458:
#line 2250 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7721 "ds_parser.cpp"
    break;

  case 459:
#line 2250 "ds_parser.ypp"
    {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt((yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror("only 32 different bits allowed",tokAt((yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !g_Program->addAlias(btype) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-4].s),tokAt((yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
#line 7742 "ds_parser.cpp"
    break;

  case 460:
#line 2270 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7748 "ds_parser.cpp"
    break;

  case 461:
#line 2271 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7754 "ds_parser.cpp"
    break;

  case 462:
#line 2272 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7760 "ds_parser.cpp"
    break;

  case 463:
#line 2273 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7766 "ds_parser.cpp"
    break;

  case 464:
#line 2277 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7778 "ds_parser.cpp"
    break;

  case 465:
#line 2284 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7789 "ds_parser.cpp"
    break;

  case 466:
#line 2293 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7799 "ds_parser.cpp"
    break;

  case 467:
#line 2298 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7808 "ds_parser.cpp"
    break;

  case 468:
#line 2305 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7818 "ds_parser.cpp"
    break;

  case 469:
#line 2310 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7829 "ds_parser.cpp"
    break;

  case 470:
#line 2316 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7840 "ds_parser.cpp"
    break;

  case 471:
#line 2322 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7852 "ds_parser.cpp"
    break;

  case 472:
#line 2329 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7865 "ds_parser.cpp"
    break;

  case 473:
#line 2340 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7873 "ds_parser.cpp"
    break;

  case 474:
#line 2343 "ds_parser.ypp"
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
#line 7889 "ds_parser.cpp"
    break;

  case 475:
#line 2357 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7900 "ds_parser.cpp"
    break;

  case 476:
#line 2366 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7906 "ds_parser.cpp"
    break;

  case 477:
#line 2367 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7912 "ds_parser.cpp"
    break;

  case 478:
#line 2371 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7922 "ds_parser.cpp"
    break;

  case 479:
#line 2376 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7931 "ds_parser.cpp"
    break;

  case 480:
#line 2383 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7941 "ds_parser.cpp"
    break;

  case 481:
#line 2388 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7954 "ds_parser.cpp"
    break;

  case 482:
#line 2399 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7964 "ds_parser.cpp"
    break;

  case 483:
#line 2404 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7973 "ds_parser.cpp"
    break;

  case 484:
#line 2411 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7987 "ds_parser.cpp"
    break;

  case 485:
#line 2423 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7993 "ds_parser.cpp"
    break;

  case 486:
#line 2424 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7999 "ds_parser.cpp"
    break;

  case 487:
#line 2428 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.first);
            pFor->iteratorsAt.push_back(np.second);
        }
        delete (yyvsp[-7].pNameWithPosList);
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
#line 8021 "ds_parser.cpp"
    break;

  case 488:
#line 2445 "ds_parser.ypp"
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.first);
            pFor->iteratorsAt.push_back(np.second);
        }
        delete (yyvsp[-7].pNameWithPosList);
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
#line 8043 "ds_parser.cpp"
    break;


#line 8047 "ds_parser.cpp"

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
#line 2464 "ds_parser.ypp"


void das_checkName(const string & name, const LineInfo &at) {
	if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
		g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
	}
}

void das_yyerror(const string & error, const LineInfo & at, CompilationError cerr) {
    g_Program->error(error,"","",at,cerr);
}

void das_yyfatalerror(const string & error, CompilationError cerr) {
    g_Program->error(error,"","",LineInfo(g_FileAccessStack.back(),
        yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line),cerr);
}

void das_yyerror(const string & error) {
    if ( !das_supress_errors ) {
        g_Program->error(error,"","",LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line),
                CompilationError::syntax_error);
    }
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
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie ) {
    return LineInfo(g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
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
                        (*pDecl->pNameList)[ai].second,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
                if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].first.empty() ) {
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
                        pType->argNames.push_back(name.first);
                    }
                } else {
                    pType->argNames.push_back(string());
                }
            }
        }
    }
}


