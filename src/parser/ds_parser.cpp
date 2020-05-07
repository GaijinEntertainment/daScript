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
#line 59 "ds_parser.ypp"

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
    void runFunctionAnnotations ( Function * func, AnnotationList * annL, const LineInfo & at );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 201 "ds_parser.cpp"

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
#line 86 "ds_parser.ypp"

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

#line 387 "ds_parser.cpp"

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
#define YYLAST   6908

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  162
/* YYNRULES -- Number of rules.  */
#define YYNRULES  488
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  872

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
       0,   406,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   422,   434,   435,   439,   443,
     448,   457,   465,   481,   503,   507,   510,   516,   525,   546,
     569,   570,   574,   578,   579,   583,   586,   592,   598,   601,
     607,   608,   612,   613,   614,   623,   624,   628,   637,   652,
     662,   671,   672,   673,   674,   675,   676,   680,   685,   693,
     694,   695,   696,   697,   698,   699,   700,   706,   719,   735,
     736,   740,   747,   759,   763,   770,   771,   775,   776,   777,
     781,   786,   793,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   836,
     856,   896,   899,   909,   910,   911,   912,   913,   914,   915,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
     926,   930,   935,   941,   947,   958,   964,   965,   976,   980,
     986,   989,   992,   996,  1002,  1006,  1010,  1013,  1016,  1021,
    1024,  1032,  1035,  1040,  1043,  1051,  1057,  1058,  1062,  1063,
    1067,  1101,  1101,  1101,  1104,  1104,  1104,  1109,  1109,  1109,
    1117,  1121,  1126,  1132,  1132,  1132,  1136,  1136,  1136,  1141,
    1141,  1141,  1150,  1153,  1159,  1160,  1167,  1178,  1179,  1183,
    1188,  1243,  1244,  1245,  1246,  1247,  1248,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1271,  1272,  1276,  1287,  1292,  1302,  1306,
    1306,  1306,  1313,  1313,  1313,  1327,  1328,  1329,  1330,  1331,
    1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1370,  1374,  1377,  1380,  1381,
    1382,  1383,  1388,  1393,  1394,  1397,  1397,  1397,  1400,  1404,
    1408,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1428,  1438,
    1493,  1494,  1498,  1499,  1503,  1511,  1514,  1518,  1559,  1564,
    1572,  1573,  1577,  1580,  1589,  1590,  1594,  1603,  1604,  1609,
    1610,  1614,  1620,  1626,  1629,  1633,  1639,  1648,  1649,  1650,
    1654,  1655,  1659,  1662,  1667,  1672,  1680,  1691,  1694,  1701,
    1702,  1706,  1733,  1733,  1745,  1748,  1757,  1770,  1782,  1783,
    1787,  1791,  1802,  1817,  1818,  1822,  1862,  1863,  1867,  1944,
    1951,  1960,  1961,  1962,  1963,  1964,  1965,  1966,  1967,  1968,
    1969,  1970,  1971,  1972,  1973,  1974,  1975,  1976,  1977,  1978,
    1979,  1980,  1981,  1982,  1983,  1984,  1988,  1989,  1990,  1991,
    1992,  1993,  1997,  2008,  2012,  2022,  2029,  2038,  2038,  2038,
    2051,  2052,  2053,  2054,  2055,  2069,  2075,  2079,  2084,  2089,
    2094,  2099,  2103,  2107,  2112,  2116,  2121,  2121,  2121,  2127,
    2134,  2134,  2134,  2139,  2139,  2139,  2145,  2145,  2145,  2150,
    2154,  2154,  2154,  2159,  2159,  2159,  2168,  2172,  2172,  2172,
    2177,  2177,  2177,  2186,  2190,  2190,  2190,  2195,  2195,  2195,
    2204,  2204,  2204,  2210,  2210,  2210,  2219,  2219,  2234,  2234,
    2254,  2255,  2256,  2257,  2261,  2268,  2277,  2282,  2289,  2294,
    2300,  2306,  2313,  2324,  2327,  2341,  2350,  2351,  2355,  2360,
    2367,  2372,  2383,  2388,  2395,  2407,  2408,  2412,  2429
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

#define YYPACT_NINF -523

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-523)))

#define YYTABLE_NINF -448

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -523,    14,  -523,  -523,   -90,   -49,   -69,   -34,   -55,  -523,
      64,  -523,  -523,   -10,  -523,  -523,  -523,  -523,  -523,   154,
    -523,    86,  -523,  -523,  -523,  -523,  -523,  -523,   -20,  -523,
      63,   114,    67,  -523,  -523,    34,  -523,   -50,    83,  -523,
     122,   160,  -523,   127,   184,   163,  -523,    62,  -523,  -523,
    -523,     6,   192,  -523,   177,     4,   -90,   211,   -49,   206,
    -523,   216,   232,  -523,   204,  -523,   227,  -523,   -96,   197,
     198,   237,  -523,   -90,   -10,  -523,  6751,   340,   347,  -523,
     221,  -523,   247,   219,  -523,   392,  -523,  -523,  -523,  -523,
    -523,   258,    16,  -523,  -523,  -523,  -523,   352,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,   234,   -92,  5796,  -523,
    -523,   278,   279,  -523,   -62,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,   -93,   271,   -78,  -523,  -523,   -88,   244,  -523,
     168,  -523,  -523,  -523,  -523,  -523,  -523,  -523,   -56,  -523,
    -523,   -53,  -523,   272,   274,   275,   276,  -523,  -523,  -523,
     260,  -523,  -523,  -523,  -523,  -523,   277,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,   280,  -523,  -523,  -523,
     287,   289,  -523,  -523,  -523,  -523,   293,   296,  -523,  -523,
    -523,  -523,  -523,    30,  -523,   281,   302,  -523,   282,  -523,
     -90,  -523,    21,  -523,   194,  5796,   283,  -523,    10,  -523,
    -523,  -523,   106,  -523,  -523,  -523,    16,  -523,   -82,  4223,
    -523,  -523,  -523,  -523,  -523,   313,  -523,   155,   171,   212,
    -523,  -523,  -523,  -523,  -523,   417,  -523,  -523,    36,  2225,
    -523,  -523,   -77,  5796,   -76,    80,   317,  -523,    78,  -523,
     332,  4220,  -523,  -523,  -523,   449,   418,   291,   326,  5796,
    -523,   -47,  -523,  -523,  -523,  -523,  5719,   297,  -523,   298,
     320,   325,   307,   328,  -523,   329,  4223,  4223,  4351,  4429,
    4223,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  4223,  4223,
    4223,  4223,  4223,  4223,  -523,   311,  -523,  -523,   -64,  -523,
    -523,  -523,   318,  -523,  -523,  -523,  -523,  -523,  5063,   315,
    -523,  -523,  -523,  -523,  -523,  5796,  5796,  5796,  5796,   316,
     278,  5796,   221,  5796,   221,  5796,   221,  5873,   279,  -523,
    -523,  -523,   319,  -523,  -523,  5867,  -523,  -523,  4220,  -523,
    -523,   321,  -523,  -523,  -523,  5796,  -523,  2354,  -523,   928,
     418,  -523,   317,  -523,  -523,   535,  -523,  -523,  -523,  2354,
     167,  -523,   351,  4223,  -523,  -523,  4223,  -523,  5796,   458,
     458,   317,  1249,   317,  1394,  6259,  -523,   -61,   174,   458,
     458,   -63,   458,   458,  5200,   -71,  -523,  1822,   221,    19,
     359,  4223,  4223,  -523,  -523,  4223,  4223,  4223,  4223,   360,
    4223,   362,  4223,  4223,  4223,  4223,  4223,  2492,  4223,  4223,
    4223,  4223,  4223,  4223,  4223,  4223,  4223,  4223,   363,  4223,
    -523,  2621,   463,   604,   657,   752,  -523,    43,  1538,   271,
    1772,   271,  2351,   271,    38,  -523,   103,  4220,   113,  -523,
    -523,  -523,   261,   195,  -523,   382,  6090,  -523,  4223,  -523,
    -523,   317,  1684,  -523,   283,  4223,  4223,  -523,   334,   372,
    2759,  1967,  -523,   471,   337,   338,  4223,  -523,  -523,  -523,
    -523,  -523,  -523,   344,   345,   346,  -523,  -523,  -523,   492,
    -523,   -91,  -523,  6194,     6,  -523,   417,  -523,  2354,  -523,
    4518,  2888,  2096,  5269,  5796,  5796,  5337,  5796,  2442,     5,
     -72,  1539,   357,  3026,  6259,   383,    40,   384,  -523,  -523,
      44,     7,  3164,   105,   110,  4223,  4223,   353,  -523,  4223,
     389,  -523,   214,  -523,   393,  -523,    55,  6462,   271,   381,
    -523,  -523,   233,   233,    13,    13,  6717,  6717,   364,   356,
    -523,  5944,    74,    74,   233,   233,   395,  6328,  6527,  6596,
    6661,    13,    13,   286,   286,   356,   356,   356,  -523,  6013,
    -523,    76,  -523,  5796,  -523,  -523,  -523,   399,  -523,   386,
    -523,   391,  -523,   414,  5796,  -523,  5873,  -523,   279,   417,
    4223,  -523,  -523,   195,  4223,  4223,  4223,  4223,  4223,  4223,
    4223,  4223,  4223,  4223,  4223,  4223,  4223,  4223,  4223,  4422,
       9,  2354,  -523,  -523,  4614,   528,  6462,  4422,  -523,   420,
     436,  6462,  2354,  -523,  4710,   394,  -523,  -523,  4422,  -523,
    -523,  -523,   526,   317,  -523,  3302,  3431,  -523,  -523,  4806,
    -523,  -523,    88,   422,   443,  5405,  -523,  2618,  2709,  -523,
    2885,   412,  4223,  4223,  -523,   393,   445,   415,   393,  4223,
     419,  4223,  4223,   393,   421,   424,  6462,  -523,  -523,  6393,
    5796,   221,    26,    93,  4223,  -523,   -83,  -523,  3560,  -523,
    -523,  4223,  -523,  -523,  -523,  2976,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  4220,  -523,  -523,  -523,  6462,
    -523,  6462,  6462,  6462,  6462,  6462,  6462,  6462,  6462,  6462,
    6462,  6462,  6462,  6462,  6462,  6462,  -523,  4223,  -523,  4902,
    -523,   283,  -523,  -523,  -523,  -523,  4998,  -523,  -523,   331,
    -523,  -523,  4223,  4223,  -523,  -523,  -523,   129,  -523,  -523,
    -523,  -523,  3698,   -81,  6462,   131,    26,  -523,   383,  6462,
    -523,  -523,   -67,   185,  -523,  -523,  -523,  3114,   271,   423,
    6462,  3836,  -523,  -523,  5796,  -523,   108,  6462,  -523,  -523,
    -523,  -523,   -65,  -523,  -523,  -523,  1101,   283,  -523,  -523,
    4223,  -523,   735,   735,  5796,  3965,   448,  4223,  4223,  4223,
    -523,  5473,  4223,   428,  4223,  4223,   429,  -523,   437,  -523,
    4223,  6462,  3252,  -523,  -523,   283,  -523,  -523,  4422,  3557,
     440,  5541,   450,    13,    13,    13,  -523,  5135,  -523,  6462,
    5135,  -523,   -78,  -523,  6462,  -523,  -523,   331,  -523,  -523,
    -523,  4094,   548,   434,   430,  -523,   -78,  -523,  -523,   435,
    5796,   461,  5609,  4223,   451,   452,  -523,  -523,  3648,  -523,
    -523,  6462,  -523,  -523,  -523,  5796,   446,  3786,  -523,  -523,
     454,  -523
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
     356,     0,     0,   340,   342,     0,     0,     0,     0,     0,
      31,     0,     0,    28,     0,   344,     0,   348,     0,     0,
       0,     0,   148,     0,     0,    76,     0,     0,     0,    82,
      77,   119,   353,     0,   337,     0,    63,    64,    61,    62,
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
       0,    78,     0,   310,     0,     0,     0,   354,   300,   359,
     341,   338,   330,   343,    17,    18,     0,    66,     0,     0,
     345,   420,   423,   426,   416,     0,   397,   430,   437,   444,
     450,   453,   407,   412,   419,     0,   415,   409,     0,     0,
     347,   411,     0,     0,     0,     0,     0,   308,   321,    79,
     300,    81,   141,   120,   358,     0,   302,     0,     0,     0,
     331,     0,    58,   352,   240,   241,     0,     0,   235,     0,
       0,     0,     0,     0,   385,     0,     0,     0,     0,     0,
       0,   201,   203,   202,   204,   205,   206,    19,     0,     0,
       0,     0,     0,     0,   197,   198,   238,   199,   236,   293,
     292,   291,    75,   296,   237,   295,   294,   273,     0,     0,
     239,   460,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,    77,     0,    77,     0,    77,     0,     0,   414,
     408,   410,     0,   413,   405,     0,   396,   459,   316,   457,
     318,     0,   309,   320,   319,     0,   322,     0,   311,     0,
     302,   303,     0,   306,   360,     0,   328,   329,   327,     0,
     150,   153,     0,     0,   171,   174,     0,   177,     0,   264,
     265,     0,     0,     0,     0,     0,   482,     0,     0,   245,
     244,   278,   243,   242,     0,     0,   299,     0,    77,     0,
       0,     0,     0,   266,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,   394,     0,     0,    80,
       0,    80,     0,    80,   146,   314,     0,   312,     0,   406,
     404,   301,   323,     0,   326,     0,   325,   143,     0,    45,
      46,     0,     0,   154,     0,     0,     0,   155,     0,     0,
       0,     0,   123,   121,     0,     0,     0,   136,   131,   129,
     130,   142,   124,     0,     0,     0,   134,   135,   137,   166,
     128,     0,   125,   207,     0,   304,   327,   332,     0,   336,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,     0,     0,   473,   466,     0,   476,   477,   478,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
      20,   268,   229,   228,     0,   274,     0,   192,    80,     0,
     288,   289,   246,   247,   259,   260,   257,   258,     0,   283,
     272,     0,   297,   298,   248,   249,     0,     0,   262,   263,
     261,   255,   256,   251,   250,   252,   253,   254,   271,     0,
     276,     0,   421,     0,   427,   417,   398,     0,   431,     0,
     438,     0,   445,     0,     0,   451,     0,   454,     0,   319,
       0,   198,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,   159,   207,     0,   149,     0,   140,     0,
       0,    39,     0,   163,   207,     0,   138,   139,     0,   127,
     132,   133,     0,     0,   126,     0,     0,   307,   334,   207,
     335,   151,     0,     0,     0,     0,   279,     0,     0,   280,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   475,   483,   484,     0,
       0,    77,     0,     0,     0,   275,     0,   285,     0,   270,
     290,     0,   269,   277,   422,     0,   428,   418,   399,   432,
     434,   439,   441,   446,   448,   313,   452,   315,   455,   324,
     144,   214,   215,   217,   216,   218,   211,   212,   213,   219,
     220,   209,   210,   221,   222,   208,    49,     0,   160,   207,
     157,     0,    50,    37,    38,   164,   207,   161,   141,    42,
     167,   170,     0,     0,   333,   152,   183,     0,   180,   172,
     175,   178,     0,     0,   464,     0,     0,   468,   467,   474,
     480,   479,     0,     0,   471,   481,    21,     0,    80,     0,
     193,     0,   194,   200,     0,   226,     0,   284,   424,   435,
     442,   449,     0,   158,   165,   162,     0,     0,    40,    41,
       0,    47,   224,   223,     0,     0,     0,     0,     0,     0,
     281,     0,     0,     0,     0,     0,     0,   230,     0,   225,
       0,   195,     0,   227,   425,     0,   122,    43,     0,     0,
       0,     0,     0,   173,   176,   179,   282,   485,   470,   465,
     485,   472,     0,   233,   196,   286,    48,    42,   184,   186,
     181,     0,     0,     0,     0,   231,     0,   287,    44,     0,
       0,     0,     0,     0,     0,     0,   234,   185,     0,   189,
     182,   486,   487,   488,   187,     0,     0,     0,   188,   190,
       0,   191
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -523,  -523,  -523,   217,   555,  -523,  -523,  -523,  -523,  -523,
    -523,   521,  -523,  -523,   561,  -523,  -523,  -523,  -214,  -523,
    -523,  -523,  -523,  -523,   388,  -523,   569,   -52,  -523,   552,
    -523,  -192,  -337,  -441,  -523,  -523,   125,  -173,  -523,  -108,
    -352,   -13,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
     630,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -428,  -523,  -523,  -434,
    -523,  -368,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
     120,  -523,  -523,   405,   264,  -523,  -523,   365,  -523,    42,
    -523,  -260,   292,  -265,  -218,   269,  -523,    -4,  -523,  -523,
    -523,  -523,   478,   581,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -151,  -106,  -523,  -523,  -523,   310,  -523,  -523,  -523,
     -73,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,
    -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,  -523,   367,
    -522,  -378,  -523,  -523,  -289,   -14,   257,  -523,  -523,  -523,
    -172,  -523
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   160,   166,   398,   316,    16,    17,    35,
      36,    63,    18,    32,    33,   484,   485,   790,   791,   486,
     487,   488,   489,   490,   167,   168,    29,    30,    45,    46,
      47,    19,   153,   226,    80,    20,    81,   317,   491,   369,
     492,   318,   493,   319,   494,   495,   496,   497,   498,   643,
     499,   500,   320,   514,   797,   515,   798,   517,   799,   321,
     794,   849,   850,   866,   865,   870,   546,   773,   322,   323,
     324,   465,   502,   325,   326,   327,   680,   832,   681,   846,
     547,   774,   847,   222,   372,   277,   228,   223,   224,   455,
     456,   217,   218,   663,   267,   379,   281,   231,   157,    54,
      22,    85,   107,    67,    68,    23,    24,   155,    83,    52,
      25,   268,   329,   106,   210,   211,   215,   212,   340,   698,
     457,   338,   697,   335,   694,   336,   814,   337,   696,   341,
     699,   342,   779,   343,   701,   344,   780,   345,   703,   346,
     781,   347,   706,   348,   708,    26,    41,    27,    40,   330,
     525,   526,   331,   527,   528,   529,   530,   332,   397,   333,
     843,   334
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   501,   209,   367,   360,   449,   232,   451,   589,   453,
     591,   396,   593,   581,     2,   464,   533,   -75,    86,    87,
       3,   114,   683,    42,   662,   603,   672,   509,   727,   602,
     161,   162,   220,   409,   410,   213,   275,   549,    66,   363,
      76,   771,   171,     4,    28,     5,   229,     6,   362,     7,
     421,   422,   171,   273,     8,    59,    43,   356,   216,   684,
       9,    48,   252,    43,   376,    34,    10,   266,   350,   364,
     406,   548,   377,   684,   221,   684,   542,   109,    56,    31,
     644,   172,    11,   652,   236,   230,   253,    60,   239,   272,
     802,   283,    44,    64,   378,   208,   357,   359,    71,    44,
      37,   439,   407,    12,   805,   219,   815,   686,    77,    78,
     536,   237,   537,   411,   412,   413,   414,    38,   240,   209,
     623,    55,    65,   419,    43,   420,   421,   422,   423,   633,
     408,   424,    88,   425,   426,   220,    89,   363,    90,    91,
      79,   227,   254,    53,   163,   278,   758,   278,   164,   278,
     165,    91,   271,   550,   505,   229,   648,   209,    49,    50,
      44,    51,   433,   434,   435,   436,   437,   364,   265,   710,
      92,   255,   256,   209,    13,   438,   257,   439,    13,   258,
     209,   594,   351,   274,   419,    14,    61,   421,   422,   363,
     358,   586,   209,   209,   259,   684,    62,   600,    39,   252,
     352,   260,    74,    56,   261,   667,   375,    58,    71,   670,
     353,   668,   208,   380,   587,   671,   684,    66,   278,   364,
      56,   365,   685,   253,   366,   392,   394,    75,   684,   209,
     209,   209,   209,   666,   753,   209,   438,   209,   439,   209,
     519,   209,   531,   693,   762,   361,   278,   677,   684,   279,
     208,   595,   280,   409,   410,   745,    69,    57,   769,   209,
     776,   597,   442,   443,   444,   445,   208,   100,   448,   728,
     450,   101,   452,   208,   596,   813,   668,   795,   674,   254,
     735,   671,   209,   675,   598,   208,   208,   755,   102,   103,
     104,   105,   462,   252,    70,   763,   803,    71,  -433,   782,
     796,   625,   668,  -433,   234,   235,   409,   410,   255,   256,
     159,   538,   539,   257,  -440,   518,   258,   253,    72,  -440,
     620,  -433,   208,   208,   208,   208,    82,   808,   208,    73,
     208,   259,   208,   511,   208,   413,   414,  -440,   360,    95,
      97,   261,   787,   419,   768,   420,   421,   422,   423,    84,
      98,   424,   208,   788,   789,  -447,   668,  -232,   806,   328,
    -447,   269,  -232,   314,   601,   270,    99,   272,   108,   111,
     112,   113,   363,   254,   150,   208,   409,   410,  -447,   355,
    -232,   151,   433,   434,   435,   436,   437,   152,   413,   414,
     154,   156,   543,   158,   159,   438,   419,   439,   420,   421,
     422,   423,   599,   256,   424,    60,   170,   257,   209,   209,
     258,   209,   214,   216,   225,   233,   389,   390,   501,   241,
     395,   242,   243,   244,   246,   259,   245,   247,   399,   400,
     401,   402,   403,   404,   248,   261,   249,   435,   436,   437,
     250,   657,   658,   251,   660,   263,   726,   339,   438,   349,
     439,   229,   262,   264,   732,   272,   370,   220,   413,   414,
     374,   371,   373,   382,   383,   739,   419,   384,   420,   421,
     422,   423,   385,   386,   424,   387,   388,   209,   409,   410,
     405,   441,    13,   446,   459,   512,   461,   466,   209,   503,
     209,   804,   232,   551,   558,   252,   560,   578,   603,   510,
     629,   208,   208,   513,   208,   628,   516,   635,   636,   637,
     695,   642,   524,   772,   524,   639,   640,   641,   438,   253,
     439,   705,   664,   666,   669,   234,   678,   682,   687,   690,
     688,   552,   553,   356,   700,   554,   555,   556,   557,   702,
     559,   731,   561,   562,   563,   564,   565,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   784,   579,
     413,   414,   704,   733,   734,   740,   738,   252,   419,   746,
     208,   421,   422,   423,   209,   254,   424,   747,   752,   756,
     757,   208,   822,   208,   760,   833,   764,   839,   619,   765,
     809,   253,   624,   828,   831,   626,   627,   853,   841,   854,
     631,   634,   857,   855,   255,   256,   638,   767,   859,   257,
      93,   582,   258,   868,   817,   540,   862,   863,   169,    96,
     438,   871,   439,   848,   282,    94,   115,   259,   649,   647,
     786,    21,   655,   276,   504,   368,   252,   261,   707,   741,
     458,   399,   836,   404,   508,   837,   376,   254,   238,   110,
     447,   534,   404,   381,   377,   676,   395,   761,   844,   679,
     253,     0,     0,     0,     0,     0,     0,   208,   209,     0,
       0,     0,     0,     0,     0,     0,   506,   256,     0,     0,
       0,   257,     0,     0,   258,     0,     0,     0,   209,   252,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   812,     0,     0,     0,     0,   507,     0,     0,   261,
       0,     0,     0,   253,     0,     0,   254,     0,     0,     0,
     709,   819,     0,     0,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   725,     0,
       0,   729,     0,     0,   209,   255,   256,     0,     0,     0,
     257,     0,   736,   258,     0,   409,   410,     0,     0,   209,
       0,   208,     0,     0,     0,   721,   725,     0,   259,   254,
       0,     0,     0,     0,     0,   583,     0,   858,   261,     0,
       0,   208,     0,   754,   252,     0,     0,     0,     0,   759,
       0,   524,   867,     0,     0,     0,     0,     0,   255,   256,
       0,     0,     0,   257,   770,   584,   258,     0,   253,     0,
       0,   777,     0,     0,     0,     0,     0,     0,     0,   845,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     0,   856,     0,   411,   412,   208,     0,   415,
       0,     0,   416,   417,   418,   419,     0,   420,   421,   422,
     423,     0,   208,   424,     0,   425,   426,     0,     0,     0,
       0,     0,   792,   793,   254,     0,     0,     0,     0,     0,
       0,     0,   801,     0,     0,     0,     0,   427,     0,   428,
     429,   430,   431,   432,     0,     0,     0,   436,   437,     0,
       0,   811,     0,   255,   256,     0,     0,   438,   257,   439,
     585,   258,     0,     0,     0,     0,   503,     0,     0,     0,
     818,     0,     0,     0,     0,   821,   259,   823,   824,   825,
       0,     0,   827,     0,   829,   830,   261,     0,     0,   467,
     834,     0,     0,     0,     3,     0,   468,   469,   470,     0,
     471,     0,   284,   285,   286,   287,     0,     0,     0,     0,
       0,     0,     0,   472,   288,   473,   474,     0,     0,     0,
       0,   852,     0,     0,     0,   475,   289,     0,     0,   476,
     290,     0,   291,   861,     9,   292,   477,     0,   478,   293,
       0,     0,   479,   480,     0,     0,     0,   177,   178,   179,
       0,   181,   182,   183,   184,   185,   294,   187,   188,   189,
     190,   191,   192,   193,   194,   195,     0,   197,   198,   199,
       0,     0,   202,   203,   204,   205,     0,     0,   295,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     296,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    43,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   309,   310,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,   314,   315,    44,   482,
     272,   483,   467,     0,     0,     0,     0,     3,     0,   468,
     469,   470,     0,   471,     0,   284,   285,   286,   287,     0,
       0,     0,     0,     0,     0,     0,   472,   288,   473,   474,
       0,     0,     0,     0,     0,     0,     0,     0,   475,   289,
       0,     0,   476,   290,     0,   291,     0,     9,   292,   477,
       0,   478,   293,     0,     0,   479,   480,     0,     0,     0,
     177,   178,   179,     0,   181,   182,   183,   184,   185,   294,
     187,   188,   189,   190,   191,   192,   193,   194,   195,     0,
     197,   198,   199,     0,     0,   202,   203,   204,   205,     0,
       0,   295,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    43,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,   309,   310,     0,     0,     0,     0,   311,   312,     0,
       0,     0,     0,   284,   285,   286,   287,   313,     0,   314,
     315,    44,   482,   272,   816,   288,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,     0,   289,     0,     0,
       0,   290,     0,   291,     0,     0,   292,     0,     0,     0,
     293,     0,     0,     0,     0,   253,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   297,     0,     0,     0,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   520,   307,     0,     0,     0,     0,     0,
     255,   256,     0,     0,     0,   257,     0,     0,   521,   309,
     310,     0,     0,     0,     0,   311,   312,     0,   284,   285,
     286,   287,     0,   259,   522,   523,     0,   314,   315,    44,
     288,   272,     0,   261,     0,     0,   252,     0,     0,     0,
       0,     0,   289,     0,     0,     0,   290,     0,   291,     0,
       0,   292,     0,     0,     0,   293,     0,     0,     0,     0,
     253,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   520,   307,
       0,     0,     0,     0,     0,   255,   256,     0,     0,     0,
     257,     0,     0,   521,   309,   310,     0,     0,     0,     0,
     311,   312,     0,   284,   285,   286,   287,     0,   259,     0,
     532,     0,   314,   315,    44,   288,   272,     0,   261,     0,
     252,   350,     0,     0,     0,     0,     0,   289,     0,     0,
       0,   290,     0,   291,     0,     0,   292,     0,     0,     0,
     293,     0,     0,     0,   253,     0,     0,     0,   177,   178,
     179,     0,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,   297,     0,     0,     0,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    43,   307,     0,     0,     0,     0,   255,
     256,     0,     0,     0,   257,   351,   588,   258,   308,   309,
     310,     0,     0,     0,     0,   311,   312,     0,   284,   285,
     286,   287,   259,   352,     0,   313,     0,   314,   315,    44,
     288,   272,   261,   353,     0,     0,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,   290,     0,   291,     0,
       0,   292,     0,     0,     0,   293,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   297,     0,     0,
       0,     0,     0,     0,     0,   621,     0,     0,     0,     0,
       0,   463,     0,     0,   252,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,    43,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,   308,   309,   310,   284,   285,   286,   287,
     311,   312,     0,     0,     0,     0,     0,     0,   288,     0,
     313,     0,   314,   315,    44,   622,   272,     0,     0,     0,
     289,     0,     0,     0,   290,     0,   291,     0,     0,   292,
       0,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   254,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,   255,   256,     0,     0,     0,   257,     0,
     590,   258,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,     0,     0,     0,     0,   311,   312,
       0,   284,   285,   286,   287,     0,   544,     0,   313,   545,
     314,   315,    44,   288,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,     0,     0,   290,
       0,   291,     0,     0,   292,     0,     0,     0,   293,     0,
       0,     0,     0,     0,     0,     0,   177,   178,   179,     0,
     181,   182,   183,   184,   185,   294,   187,   188,   189,   190,
     191,   192,   193,   194,   195,     0,   197,   198,   199,     0,
       0,   202,   203,   204,   205,     0,     0,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
     297,     0,     0,     0,     0,     0,     0,     0,   632,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    43,   307,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,   287,   653,     0,   308,   309,   310,     0,
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
     285,   286,   287,   654,     0,   308,   309,   310,     0,     0,
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
     354,   313,   289,   314,   315,    44,   290,   272,   291,     0,
       0,   292,     0,     0,     0,   293,     0,   253,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   297,     0,     0,
       0,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,   463,     0,     0,   252,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,    43,   307,
       0,     0,   255,   256,     0,     0,     0,   257,   253,   592,
     258,     0,     0,   308,   309,   310,   284,   285,   286,   287,
     311,   312,     0,   566,     0,   259,     0,     0,   288,     0,
     313,     0,   314,   315,    44,   261,   272,     0,     0,     0,
     289,     0,     0,     0,   290,     0,   291,     0,     0,   292,
       0,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   254,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,   255,   256,     0,     0,     0,   257,     0,
     661,   258,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,   287,     0,
       0,   308,   309,   310,     0,     0,     0,   288,   311,   312,
     252,     0,     0,     0,     0,     0,     0,     0,   313,   289,
     314,   315,    44,   290,   272,   291,     0,     0,   292,     0,
       0,     0,   293,     0,   253,     0,     0,     0,     0,     0,
     177,   178,   179,     0,   181,   182,   183,   184,   185,   294,
     187,   188,   189,   190,   191,   192,   193,   194,   195,     0,
     197,   198,   199,     0,     0,   202,   203,   204,   205,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   297,     0,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    43,   307,     0,     0,   255,
     256,     0,     0,     0,   257,   253,   749,   258,     0,     0,
     308,   309,   310,   284,   285,   286,   287,   311,   312,     0,
       0,     0,   259,     0,     0,   288,     0,   313,   580,   314,
     315,    44,   261,   272,     0,     0,     0,   289,     0,     0,
       0,   290,     0,   291,     0,     0,   292,     0,     0,     0,
     293,     0,     0,   630,     0,     0,     0,     0,   177,   178,
     179,   254,   181,   182,   183,   184,   185,   294,   187,   188,
     189,   190,   191,   192,   193,   194,   195,     0,   197,   198,
     199,     0,     0,   202,   203,   204,   205,     0,     0,   295,
     255,   256,     0,     0,     0,   257,     0,   750,   258,     0,
       0,   296,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    43,   307,     0,     0,     0,     0,     0,
       0,     0,   284,   285,   286,   287,     0,     0,   308,   309,
     310,     0,     0,     0,   288,   311,   312,   252,     0,     0,
       0,     0,     0,     0,     0,   313,   289,   314,   315,    44,
     290,   272,   291,     0,     0,   292,     0,     0,     0,   293,
       0,   253,     0,     0,     0,     0,     0,   177,   178,   179,
       0,   181,   182,   183,   184,   185,   294,   187,   188,   189,
     190,   191,   192,   193,   194,   195,     0,   197,   198,   199,
       0,     0,   202,   203,   204,   205,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     296,   297,     0,     0,     0,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    43,   307,     0,     0,   255,   256,     0,     0,
       0,   257,   253,   751,   258,     0,     0,   308,   309,   310,
     284,   285,   286,   287,   311,   312,     0,     0,     0,   259,
       0,     0,   288,     0,   313,   651,   314,   315,    44,   261,
     272,     0,     0,     0,   289,     0,     0,     0,   290,     0,
     291,     0,     0,   292,     0,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   254,   181,
     182,   183,   184,   185,   294,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   295,   255,   256,     0,
       0,     0,   257,     0,   778,   258,     0,     0,   296,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     261,   298,   299,   300,   301,   302,   303,   304,   305,   306,
      43,   307,     0,     0,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,   308,   309,   310,   284,   285,
     286,   287,   311,   312,     0,     0,     0,     0,     0,     0,
     288,     0,   313,   665,   314,   315,    44,     0,   272,     0,
       0,     0,   289,     0,     0,     0,   290,     0,   291,     0,
       0,   292,     0,     0,     0,   293,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   254,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,   255,   256,     0,     0,     0,
     257,     0,   807,   258,     0,     0,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   261,   298,
     299,   300,   301,   302,   303,   304,   305,   306,    43,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   253,     0,
       0,     0,     0,   308,   309,   310,   284,   285,   286,   287,
     311,   312,     0,     0,     0,     0,     0,     0,   288,     0,
     313,   673,   314,   315,    44,     0,   272,     0,     0,     0,
     289,     0,     0,     0,   290,     0,   291,     0,     0,   292,
       0,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   254,   181,   182,   183,   184,   185,
     294,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,   197,   198,   199,     0,     0,   202,   203,   204,   205,
       0,     0,   295,   255,   256,     0,     0,     0,   257,     0,
     835,   258,     0,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,   742,
       0,     0,     0,     0,     0,     0,   261,   298,   299,   300,
     301,   302,   303,   304,   305,   306,    43,   307,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,   287,     0,
       0,   308,   309,   310,     0,     0,     0,   288,   311,   312,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   289,
     314,   315,    44,   290,   272,   291,     0,     0,   292,     0,
       0,     0,   293,     0,     0,     0,     0,     0,     0,     0,
     177,   178,   179,     0,   181,   182,   183,   184,   185,   294,
     187,   188,   189,   190,   191,   192,   193,   194,   195,     0,
     197,   198,   199,     0,     0,   202,   203,   204,   205,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   743,     0,
       0,     0,     0,     0,     0,     0,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    43,   307,     0,     0,     0,
       0,     0,     0,     0,   284,   285,   286,   287,     0,     0,
     308,   309,   310,     0,     0,     0,   288,   311,   312,   252,
       0,     0,     0,     0,     0,     0,     0,   313,   289,   314,
     315,    44,   290,   272,   291,     0,     0,   292,     0,     0,
       0,   293,     0,   253,     0,     0,     0,     0,     0,   177,
     178,   179,     0,   181,   182,   183,   184,   185,   294,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,   197,
     198,   199,     0,     0,   202,   203,   204,   205,     0,     0,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   297,     0,     0,     0,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,     0,   298,   299,   300,   301,   302,
     303,   304,   305,   306,    43,   307,     0,     0,   255,   256,
       0,     0,     0,   257,   253,   838,   258,     0,     0,   308,
     309,   310,   284,   285,   286,   287,   311,   312,     0,     0,
       0,   259,     0,     0,   288,     0,   313,   775,   314,   315,
      44,   261,   272,     0,     0,     0,   289,     0,     0,     0,
     290,     0,   291,     0,     0,   292,     0,     0,     0,   293,
       0,     0,     0,     0,     0,     0,     0,   177,   178,   179,
     254,   181,   182,   183,   184,   185,   294,   187,   188,   189,
     190,   191,   192,   193,   194,   195,     0,   197,   198,   199,
       0,     0,   202,   203,   204,   205,     0,     0,   295,   255,
     256,     0,     0,     0,   257,     0,   864,   258,     0,     0,
     296,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   261,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    43,   307,     0,     0,     0,     0,     0,     0,
       0,     0,   253,     0,     0,     0,     0,   308,   309,   310,
     284,   285,   286,   287,   311,   312,     0,     0,     0,     0,
       0,     0,   288,     0,   313,   800,   314,   315,    44,     0,
     272,     0,     0,     0,   289,     0,     0,     0,   290,     0,
     291,     0,     0,   292,     0,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   254,   181,
     182,   183,   184,   185,   294,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     0,   197,   198,   199,     0,     0,
     202,   203,   204,   205,     0,     0,   295,   255,   256,     0,
       0,     0,   257,     0,   869,   258,     0,     0,   296,   297,
       0,     0,     0,     0,     0,     0,     0,   810,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,   298,   299,   300,   301,   302,   303,   304,   305,   306,
      43,   307,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,   287,   820,     0,   308,   309,   310,     0,     0,
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
     286,   287,   851,     0,   308,   309,   310,     0,     0,     0,
     288,   311,   312,     0,     0,     0,     0,     0,     0,     0,
       0,   313,   289,   314,   315,    44,   290,   272,   291,     0,
       0,   292,     0,     0,     0,   293,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,     0,   181,   182,   183,
     184,   185,   294,   187,   188,   189,   190,   191,   192,   193,
     194,   195,     0,   197,   198,   199,     0,     0,   202,   203,
     204,   205,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
     299,   300,   301,   302,   303,   304,   305,   306,    43,   307,
       0,     0,     0,     0,     0,     0,     0,   284,   285,   286,
     287,     0,     0,   308,   309,   310,     0,     0,     0,   288,
     311,   312,   252,     0,     0,     0,     0,     0,     0,     0,
     313,   289,   314,   315,    44,   290,   272,   291,     0,     0,
     292,     0,     0,     0,   293,     0,   253,     0,     0,     0,
       0,     0,   177,   178,   179,     0,   181,   182,   183,   184,
     185,   294,   187,   188,   189,   190,   191,   192,   193,   194,
     195,     0,   197,   198,   199,     0,     0,   202,   203,   204,
     205,     0,     0,   295,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,   297,     0,     0,     0,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    43,   307,     0,
       0,   255,   256,   391,     0,     0,   257,     0,     0,   258,
       0,     0,   308,   309,   310,   173,     0,     0,     0,   311,
     312,   174,     0,     0,   259,     0,     0,     0,     0,   313,
       0,   314,   315,    44,   261,   272,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   393,   409,   410,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,   174,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
       0,    44,   411,   412,   413,   414,   415,     0,     0,   416,
     417,   418,   419,     0,   420,   421,   422,   423,   409,   410,
     424,     0,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   427,     0,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,     0,     0,     0,     0,
       0,     0,     0,     0,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     604,   605,   606,   607,   608,   609,   610,   611,   411,   412,
     413,   414,   415,   612,   613,   416,   417,   418,   419,   614,
     420,   421,   422,   423,   409,   410,   424,   615,   425,   426,
     616,   617,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   618,
     427,     0,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,     0,     0,     0,     0,     0,     0,     0,     0,
     438,     0,   439,     0,     0,     0,     0,     0,     0,   650,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   604,   605,   606,   607,
     608,   609,   610,   611,   411,   412,   413,   414,   415,   612,
     613,   416,   417,   418,   419,   614,   420,   421,   422,   423,
     409,   410,   424,   615,   425,   426,   616,   617,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   618,   427,     0,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,     0,   439,     0,
       0,     0,     0,     0,     0,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   604,   605,   606,   607,   608,   609,   610,   611,
     411,   412,   413,   414,   415,   612,   613,   416,   417,   418,
     419,   614,   420,   421,   422,   423,   409,   410,   424,   615,
     425,   426,   616,   617,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   618,   427,     0,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,     0,     0,     0,     0,     0,     0,
       0,     0,   438,     0,   439,     0,     0,     0,     0,     0,
       0,   737,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   604,   605,
     606,   607,   608,   609,   610,   611,   411,   412,   413,   414,
     415,   612,   613,   416,   417,   418,   419,   614,   420,   421,
     422,   423,   409,   410,   424,   615,   425,   426,   616,   617,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   618,   427,     0,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,   438,     0,
     439,     0,     0,     0,     0,     0,     0,   744,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   604,   605,   606,   607,   608,   609,
     610,   611,   411,   412,   413,   414,   415,   612,   613,   416,
     417,   418,   419,   614,   420,   421,   422,   423,   409,   410,
     424,   615,   425,   426,   616,   617,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   618,   427,     0,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,     0,     0,     0,     0,
       0,     0,     0,     0,   438,     0,   439,     0,     0,     0,
       0,     0,     0,   783,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,   410,     0,     0,     0,     0,     0,
     604,   605,   606,   607,   608,   609,   610,   611,   411,   412,
     413,   414,   415,   612,   613,   416,   417,   418,   419,   614,
     420,   421,   422,   423,     0,     0,   424,   615,   425,   426,
     616,   617,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   618,
     427,     0,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,     0,     0,     0,   409,   410,     0,     0,     0,
     438,     0,   439,   411,   412,   413,   414,   415,     0,   785,
     416,   417,   418,   419,     0,   420,   421,   422,   423,     0,
       0,   424,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,     0,     0,     0,
     409,   410,     0,     0,     0,   438,     0,   439,     0,     0,
       0,     0,     0,     0,   440,   411,   412,   413,   414,   415,
       0,     0,   416,   417,   418,   419,     0,   420,   421,   422,
     423,     0,     0,   424,     0,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   409,
     410,     0,     0,     0,     0,     0,     0,   438,     0,   439,
     411,   412,   413,   414,   415,     0,   842,   416,   417,   418,
     419,     0,   420,   421,   422,   423,     0,     0,   424,     0,
     425,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,     0,     0,     0,   409,   410,     0,
       0,     0,   438,     0,   439,     0,     0,   541,     0,   411,
     412,   413,   414,   415,     0,     0,   416,   417,   418,   419,
       0,   420,   421,   422,   423,     0,     0,   424,     0,   425,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,     0,     0,   409,   410,     0,     0,     0,
       0,   438,     0,   439,     0,     0,   656,   411,   412,   413,
     414,   415,     0,     0,   416,   417,   418,   419,     0,   420,
     421,   422,   423,     0,     0,   424,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,     0,     0,   409,   410,     0,     0,     0,     0,   438,
       0,   439,     0,     0,   659,   411,   412,   413,   414,   415,
       0,     0,   416,   417,   418,   419,     0,   420,   421,   422,
     423,     0,     0,   424,     0,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,     0,
       0,   409,   410,     0,     0,     0,     0,   438,     0,   439,
       0,     0,   748,   411,   412,   413,   414,   415,     0,     0,
     416,   417,   418,   419,     0,   420,   421,   422,   423,     0,
       0,   424,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,     0,     0,   409,
     410,     0,     0,     0,     0,   438,     0,   439,     0,     0,
     826,   411,   412,   413,   414,   415,     0,     0,   416,   417,
     418,   419,     0,   420,   421,   422,   423,     0,     0,   424,
       0,   425,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,   438,     0,   439,     0,     0,   840,   411,
     412,   413,   414,   415,     0,     0,   416,   417,   418,   419,
       0,   420,   421,   422,   423,     0,     0,   424,     0,   425,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,   174,
       0,   427,     0,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,     0,   175,     0,     0,     0,     0,     0,
       0,   438,     0,   439,     0,     0,   860,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,     0,     0,     0,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,   298,   299,   300,     0,     0,     0,
       0,     0,     0,    43,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,   409,   410,    44,
       0,     0,     0,     0,     0,     0,     0,   173,     0,     0,
       0,     0,     0,   174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,     0,   409,   410,    44,   411,   412,   413,
     414,   415,     0,     0,   416,   417,   418,   419,     0,   420,
     421,   422,   423,     0,     0,   424,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,   427,
       0,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,   439,   460,   409,   410,     0,     0,     0,     0,     0,
       0,     0,     0,    44,   411,   412,   413,   414,   415,     0,
       0,   416,   417,   418,   419,     0,   420,   421,   422,   423,
       0,     0,   424,     0,   425,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,     0,   439,   689,
     409,   410,     0,   411,   412,   413,   414,   415,     0,     0,
     416,   417,   418,   419,     0,   420,   421,   422,   423,     0,
       0,   424,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,   438,     0,   439,   692,     0,
       0,     0,   604,   605,   606,   607,   608,   609,   610,   611,
     411,   412,   413,   414,   415,   612,   613,   416,   417,   418,
     419,   614,   420,   421,   422,   423,  -207,     0,   424,   615,
     425,   426,   616,   617,   409,   410,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   618,   427,     0,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,     0,     0,     0,     0,     0,     0,
       0,     0,   438,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   409,
     410,     0,     0,     0,     0,     0,   604,   605,   606,   607,
     608,   609,   610,   611,   411,   412,   413,   414,   415,   612,
     613,   416,   417,   418,   419,   645,   420,   421,   422,   423,
       0,     0,   424,   615,   425,   426,   616,   617,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   646,   427,     0,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   409,   410,
       0,     0,     0,     0,     0,     0,   438,     0,   439,   411,
     412,   413,   414,   415,     0,     0,   416,   417,   418,   419,
       0,   420,   421,   422,   423,     0,     0,   424,     0,   425,
     426,     0,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   409,   410,     0,     0,     0,     0,     0,
       0,   438,     0,   439,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   416,   417,   418,   419,     0,
     420,   421,   422,   423,     0,     0,   424,     0,   425,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,   691,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   409,   410,     0,     0,     0,     0,     0,     0,
     438,     0,   439,   411,   412,   413,   414,   415,     0,     0,
     416,   417,   418,   419,     0,   420,   421,   422,   423,     0,
       0,   424,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   766,     0,     0,   427,     0,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   409,   410,     0,
       0,     0,     0,     0,     0,   438,     0,   439,     0,     0,
       0,     0,   411,   412,   413,   414,   415,     0,     0,   416,
     417,   418,   419,     0,   420,   421,   422,   423,     0,     0,
     424,     0,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   409,   410,     0,     0,
       0,     0,     0,     0,   438,     0,   439,   411,   412,   413,
     414,   415,     0,     0,   416,   417,   418,   419,     0,   420,
     421,   422,   423,     0,     0,   424,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   409,   410,     0,     0,     0,     0,     0,     0,   438,
       0,   439,     0,     0,     0,     0,   411,   412,   413,   414,
     415,     0,     0,   416,   417,   418,   419,     0,   420,   421,
     422,   423,     0,     0,   424,     0,   425,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   409,   410,     0,
       0,     0,   430,   431,   432,   433,   434,   435,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,   438,     0,
     439,   411,   412,   413,   414,   415,     0,     0,   416,   417,
     418,   419,     0,   420,   421,   422,   423,     0,     0,   424,
       0,   425,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   432,
     433,   434,   435,   436,   437,     0,     0,   411,   412,   413,
     414,   415,     0,   438,   416,   439,     0,   419,     0,   420,
     421,   422,   423,     0,     0,   424,     0,   425,   426,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,     0,     0,   431,   432,   433,   434,   435,   436,
     437,   134,   135,   136,   137,     0,     0,     0,     0,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,     0,     0,   148,   149
};

static const yytype_int16 yycheck[] =
{
      13,   369,   108,   268,   264,   342,   157,   344,   449,   346,
     451,   300,   453,   441,     0,   367,   394,     7,    14,    15,
       6,    73,   544,    33,    19,   116,    19,   379,    19,   463,
      14,    15,   125,    20,    21,   108,   228,    18,   134,   111,
      34,   124,   134,    29,   134,    31,   134,    33,   266,    35,
     113,   114,   134,   226,    40,    21,   134,   134,   134,   140,
      46,    74,    32,   134,   111,   134,    52,    46,    32,   141,
     134,   408,   119,   140,   167,   140,   147,   173,   140,   128,
     171,   173,    68,   511,   140,   173,    56,    53,   141,   172,
     171,   173,   170,   143,   141,   108,   173,   173,   170,   170,
     134,   164,   166,    89,   171,   167,   171,   548,   102,   103,
     171,   167,   173,   100,   101,   102,   103,   172,   171,   225,
     472,   141,   172,   110,   134,   112,   113,   114,   115,   481,
     322,   118,   128,   120,   121,   125,   132,   111,   134,   135,
     134,   154,   112,    57,   128,   140,   668,   140,   132,   140,
     134,   135,   225,   134,   372,   134,   508,   263,     4,     5,
     170,     7,   149,   150,   151,   152,   153,   141,   220,   603,
     166,   141,   142,   279,   164,   162,   146,   164,   164,   149,
     286,   143,   146,   173,   110,   171,   152,   113,   114,   111,
     263,   148,   298,   299,   164,   140,   162,   462,   134,    32,
     164,   171,   140,   140,   174,   165,   279,   140,   170,   165,
     174,   171,   225,   286,   171,   171,   140,   134,   140,   141,
     140,   143,   167,    56,   146,   298,   299,   165,   140,   335,
     336,   337,   338,   140,   662,   341,   162,   343,   164,   345,
     391,   347,   393,   167,   672,   165,   140,   536,   140,   143,
     263,   148,   146,    20,    21,   167,   134,   143,   165,   365,
     688,   148,   335,   336,   337,   338,   279,    63,   341,   621,
     343,    67,   345,   286,   171,   167,   171,   148,   173,   112,
     632,   171,   388,   173,   171,   298,   299,   665,    84,    85,
      86,    87,   365,    32,   134,   673,   165,   170,   143,   727,
     171,   474,   171,   148,   136,   137,    20,    21,   141,   142,
     136,   137,   138,   146,   143,   388,   149,    56,   134,   148,
     471,   166,   335,   336,   337,   338,   134,   768,   341,   166,
     343,   164,   345,   166,   347,   102,   103,   166,   598,   128,
     134,   174,    11,   110,   681,   112,   113,   114,   115,   172,
     134,   118,   365,    22,    23,   143,   171,   143,   173,   239,
     148,   167,   148,   168,   169,   171,   134,   172,   141,   172,
     172,   134,   111,   112,    34,   388,    20,    21,   166,   259,
     166,    34,   149,   150,   151,   152,   153,   166,   102,   103,
     143,   172,   405,     1,   136,   162,   110,   164,   112,   113,
     114,   115,   141,   142,   118,    53,   172,   146,   514,   515,
     149,   517,   134,   134,   143,   171,   296,   297,   786,   147,
     300,   147,   147,   147,   147,   164,   166,   147,   308,   309,
     310,   311,   312,   313,   147,   174,   147,   151,   152,   153,
     147,   514,   515,   147,   517,   143,   619,   134,   162,    32,
     164,   134,   171,   171,   627,   172,     7,   125,   102,   103,
     134,    43,   171,   166,   166,   638,   110,   147,   112,   113,
     114,   115,   147,   166,   118,   147,   147,   583,    20,    21,
     169,   166,   164,   167,   165,   134,   165,   367,   594,   369,
     596,   756,   643,   134,   134,    32,   134,   134,   116,   379,
     128,   514,   515,   383,   517,   171,   386,    36,   171,   171,
     583,    19,   392,   686,   394,   171,   171,   171,   162,    56,
     164,   594,   165,   140,   140,   136,   173,   134,   147,   134,
     166,   411,   412,   134,   148,   415,   416,   417,   418,   148,
     420,    13,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   731,   439,
     102,   103,   148,   143,   128,    39,   172,    32,   110,   147,
     583,   113,   114,   115,   680,   112,   118,   134,   166,   134,
     165,   594,   134,   596,   165,   148,   165,   147,   468,   165,
     167,    56,   472,   165,   165,   475,   476,    49,   148,   165,
     480,   481,   167,   173,   141,   142,   486,   680,   147,   146,
      55,   148,   149,   167,   787,   398,   165,   165,    97,    58,
     162,   167,   164,   837,   236,    56,    74,   164,   508,   504,
     738,     1,   512,   228,   370,   270,    32,   174,   596,   643,
     348,   521,   815,   523,   375,   818,   111,   112,   170,    68,
     340,   394,   532,   286,   119,   535,   536,   671,   830,   539,
      56,    -1,    -1,    -1,    -1,    -1,    -1,   680,   774,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,   149,    -1,    -1,    -1,   794,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,   774,    -1,    -1,    -1,    -1,   171,    -1,    -1,   174,
      -1,    -1,    -1,    56,    -1,    -1,   112,    -1,    -1,    -1,
     600,   794,    -1,    -1,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   617,   618,    -1,
      -1,   621,    -1,    -1,   850,   141,   142,    -1,    -1,    -1,
     146,    -1,   632,   149,    -1,    20,    21,    -1,    -1,   865,
      -1,   774,    -1,    -1,    -1,   645,   646,    -1,   164,   112,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   850,   174,    -1,
      -1,   794,    -1,   663,    32,    -1,    -1,    -1,    -1,   669,
      -1,   671,   865,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,   146,   684,   148,   149,    -1,    56,    -1,
      -1,   691,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   832,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   846,    -1,   100,   101,   850,    -1,   104,
      -1,    -1,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,    -1,   865,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,   742,   743,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   752,    -1,    -1,    -1,    -1,   142,    -1,   144,
     145,   146,   147,   148,    -1,    -1,    -1,   152,   153,    -1,
      -1,   771,    -1,   141,   142,    -1,    -1,   162,   146,   164,
     148,   149,    -1,    -1,    -1,    -1,   786,    -1,    -1,    -1,
     790,    -1,    -1,    -1,    -1,   795,   164,   797,   798,   799,
      -1,    -1,   802,    -1,   804,   805,   174,    -1,    -1,     1,
     810,    -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,   841,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    44,   853,    46,    47,    48,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    -1,    -1,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
      -1,    -1,    -1,    -1,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,
     172,   173,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,    48,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    -1,    -1,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,    -1,    -1,    -1,    -1,   156,   157,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,   166,    -1,   168,
     169,   170,   171,   172,   173,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,   149,   150,
     151,    -1,    -1,    -1,    -1,   156,   157,    -1,    14,    15,
      16,    17,    -1,   164,   165,   166,    -1,   168,   169,   170,
      26,   172,    -1,   174,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,   149,   150,   151,    -1,    -1,    -1,    -1,
     156,   157,    -1,    14,    15,    16,    17,    -1,   164,    -1,
     166,    -1,   168,   169,   170,    26,   172,    -1,   174,    -1,
      32,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    56,    -1,    -1,    -1,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,   146,   146,   148,   149,   149,   150,
     151,    -1,    -1,    -1,    -1,   156,   157,    -1,    14,    15,
      16,    17,   164,   164,    -1,   166,    -1,   168,   169,   170,
      26,   172,   174,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    32,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,   149,   150,   151,    14,    15,    16,    17,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     166,    -1,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,   112,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,    -1,
     148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    -1,   156,   157,
      -1,    14,    15,    16,    17,    -1,   164,    -1,   166,   167,
     168,   169,   170,    26,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
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
      15,    16,    17,   147,    -1,   149,   150,   151,    -1,    -1,
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
     165,   166,    38,   168,   169,   170,    42,   172,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    32,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,   141,   142,    -1,    -1,    -1,   146,    56,   148,
     149,    -1,    -1,   149,   150,   151,    14,    15,    16,    17,
     156,   157,    -1,    21,    -1,   164,    -1,    -1,    26,    -1,
     166,    -1,   168,   169,   170,   174,   172,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,   112,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,    -1,
     148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,   157,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    38,
     168,   169,   170,    42,   172,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,   141,
     142,    -1,    -1,    -1,   146,    56,   148,   149,    -1,    -1,
     149,   150,   151,    14,    15,    16,    17,   156,   157,    -1,
      -1,    -1,   164,    -1,    -1,    26,    -1,   166,   167,   168,
     169,   170,   174,   172,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,
      61,   112,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,
     141,   142,    -1,    -1,    -1,   146,    -1,   148,   149,    -1,
      -1,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,   149,   150,
     151,    -1,    -1,    -1,    26,   156,   157,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    38,   168,   169,   170,
      42,   172,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,   141,   142,    -1,    -1,
      -1,   146,    56,   148,   149,    -1,    -1,   149,   150,   151,
      14,    15,    16,    17,   156,   157,    -1,    -1,    -1,   164,
      -1,    -1,    26,    -1,   166,   167,   168,   169,   170,   174,
     172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,   112,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,   141,   142,    -1,
      -1,    -1,   146,    -1,   148,   149,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     174,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,   149,   150,   151,    14,    15,
      16,    17,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   166,   167,   168,   169,   170,    -1,   172,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,   112,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,   141,   142,    -1,    -1,    -1,
     146,    -1,   148,   149,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   174,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,   149,   150,   151,    14,    15,    16,    17,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     166,   167,   168,   169,   170,    -1,   172,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,   112,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    90,   141,   142,    -1,    -1,    -1,   146,    -1,
     148,   149,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,
      -1,   149,   150,   151,    -1,    -1,    -1,    26,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    38,
     168,   169,   170,    42,   172,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
     149,   150,   151,    -1,    -1,    -1,    26,   156,   157,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    38,   168,
     169,   170,    42,   172,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    56,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,    -1,   141,   142,
      -1,    -1,    -1,   146,    56,   148,   149,    -1,    -1,   149,
     150,   151,    14,    15,    16,    17,   156,   157,    -1,    -1,
      -1,   164,    -1,    -1,    26,    -1,   166,   167,   168,   169,
     170,   174,   172,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
     112,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    90,   141,
     142,    -1,    -1,    -1,   146,    -1,   148,   149,    -1,    -1,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,   174,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,   149,   150,   151,
      14,    15,    16,    17,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   166,   167,   168,   169,   170,    -1,
     172,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    61,   112,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,   141,   142,    -1,
      -1,    -1,   146,    -1,   148,   149,    -1,    -1,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   125,   126,   127,   128,   129,   130,   131,   132,   133,
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
      16,    17,    18,    -1,   149,   150,   151,    -1,    -1,    -1,
      26,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    38,   168,   169,   170,    42,   172,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    -1,    -1,   149,   150,   151,    -1,    -1,    -1,    26,
     156,   157,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    38,   168,   169,   170,    42,   172,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,   141,   142,    12,    -1,    -1,   146,    -1,    -1,   149,
      -1,    -1,   149,   150,   151,    24,    -1,    -1,    -1,   156,
     157,    30,    -1,    -1,   164,    -1,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,   174,   172,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    12,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,   170,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    20,    21,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     162,    -1,   164,   100,   101,   102,   103,   104,    -1,   171,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   100,   101,   102,   103,   104,
      -1,    -1,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,
     100,   101,   102,   103,   104,    -1,   171,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   162,    -1,   164,    -1,    -1,   167,    -1,   100,
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
      -1,    20,    21,    -1,    -1,    -1,    -1,   162,    -1,   164,
      -1,    -1,   167,   100,   101,   102,   103,   104,    -1,    -1,
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
     149,   150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   164,    -1,    -1,   167,   100,
     101,   102,   103,   104,    -1,    -1,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,   167,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    20,    21,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    20,    21,   170,   100,   101,   102,
     103,   104,    -1,    -1,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,   164,   165,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,   100,   101,   102,   103,   104,    -1,
      -1,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,
      20,    21,    -1,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,   122,   123,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   100,
     101,   102,   103,   104,    -1,    -1,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,   104,    -1,    -1,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   100,   101,   102,   103,   104,    -1,    -1,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,   104,    -1,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   100,   101,   102,
     103,   104,    -1,    -1,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,   164,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
     104,    -1,    -1,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   100,   101,   102,   103,   104,    -1,    -1,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   100,   101,   102,
     103,   104,    -1,   162,   107,   164,    -1,   110,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    -1,    -1,    -1,   147,   148,   149,   150,   151,   152,
     153,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   162,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   156,   157
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
       5,     7,   284,    57,   274,   141,   140,   143,   140,    21,
      53,   152,   162,   186,   143,   172,   134,   278,   279,   134,
     134,   170,   134,   166,   140,   165,    34,   102,   103,   134,
     209,   211,   134,   283,   172,   276,    14,    15,   128,   132,
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
     125,   167,   258,   262,   263,   143,   208,   216,   261,   134,
     173,   272,   286,   171,   136,   137,   140,   167,   277,   141,
     171,   147,   147,   147,   147,   166,   147,   147,   147,   147,
     147,   147,    32,    56,   112,   141,   142,   146,   149,   164,
     171,   174,   171,   143,   171,   202,    46,   269,   286,   167,
     171,   295,   172,   212,   173,   206,   258,   260,   140,   143,
     146,   271,   199,   173,    14,    15,    16,    17,    26,    38,
      42,    44,    47,    51,    68,    90,   102,   103,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   135,   149,   150,
     151,   156,   157,   166,   168,   169,   181,   212,   216,   218,
     227,   234,   243,   244,   245,   248,   249,   250,   255,   287,
     324,   327,   332,   334,   336,   298,   300,   302,   296,   134,
     293,   304,   306,   308,   310,   312,   314,   316,   318,    32,
      32,   146,   164,   174,   165,   255,   134,   173,   295,   173,
     266,   165,   269,   111,   141,   143,   146,   268,   262,   214,
       7,    43,   259,   171,   134,   295,   111,   119,   141,   270,
     295,   324,   166,   166,   147,   147,   166,   147,   147,   255,
     255,    12,   295,    12,   295,   255,   329,   333,   180,   255,
     255,   255,   255,   255,   255,   169,   134,   166,   206,    20,
      21,   100,   101,   102,   103,   104,   107,   108,   109,   110,
     112,   113,   114,   115,   118,   120,   121,   142,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   162,   164,
     171,   166,   295,   295,   295,   295,   167,   291,   295,   207,
     295,   207,   295,   207,   134,   264,   265,   295,   267,   165,
     165,   165,   295,   117,   215,   246,   255,     1,     8,     9,
      10,    12,    25,    27,    28,    37,    41,    48,    50,    54,
      55,    91,   171,   173,   190,   191,   194,   195,   196,   197,
     198,   213,   215,   217,   219,   220,   221,   222,   223,   225,
     226,   246,   247,   255,   259,   269,   141,   171,   270,   215,
     255,   166,   134,   255,   228,   230,   255,   232,   295,   286,
     134,   149,   165,   166,   255,   325,   326,   328,   329,   330,
     331,   286,   166,   326,   331,   124,   171,   173,   137,   138,
     178,   167,   147,   216,   164,   167,   241,   255,   207,    18,
     134,   134,   255,   255,   255,   255,   255,   255,   134,   255,
     134,   255,   255,   255,   255,   255,    21,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   134,   255,
     167,   241,   148,   171,   148,   148,   148,   171,   148,   208,
     148,   208,   148,   208,   143,   148,   171,   148,   171,   141,
     268,   169,   244,   116,    92,    93,    94,    95,    96,    97,
      98,    99,   105,   106,   111,   119,   122,   123,   141,   255,
     286,   111,   171,   215,   255,   212,   255,   255,   171,   128,
      54,   255,   111,   215,   255,    36,   171,   171,   255,   171,
     171,   171,    19,   224,   171,   111,   141,   211,   215,   255,
     171,   167,   241,    18,   147,   255,   167,   295,   295,   167,
     295,   148,    19,   268,   165,   167,   140,   165,   171,   140,
     165,   171,    19,   167,   173,   173,   255,   329,   173,   255,
     251,   253,   134,   325,   140,   167,   208,   147,   166,   165,
     134,   143,   165,   167,   299,   295,   303,   297,   294,   305,
     148,   309,   148,   313,   148,   295,   317,   264,   319,   255,
     244,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   212,    19,   215,   255,
     171,    13,   212,   143,   128,   215,   255,   171,   172,   212,
      39,   272,   117,   117,   171,   167,   147,   134,   167,   148,
     148,   148,   166,   241,   255,   326,   134,   165,   325,   255,
     165,   330,   241,   326,   165,   165,   139,   295,   207,   165,
     255,   124,   212,   242,   256,   167,   241,   255,   148,   307,
     311,   315,   241,   171,   212,   171,   214,    11,    22,    23,
     192,   193,   255,   255,   235,   148,   171,   229,   231,   233,
     167,   255,   171,   165,   268,   171,   173,   148,   208,   167,
     111,   255,   295,   167,   301,   171,   173,   212,   255,   295,
      18,   255,   134,   255,   255,   255,   167,   255,   165,   255,
     255,   165,   252,   148,   255,   148,   212,   212,   148,   147,
     167,   148,   171,   335,   335,   216,   254,   257,   193,   236,
     237,    18,   255,    49,   165,   173,   216,   167,   295,   147,
     167,   255,   165,   165,   148,   239,   238,   295,   167,   148,
     240,   167
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       4,     3,     7,     1,     1,     1,     2,     2,     1,     1,
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
       0,     4,     0,     1,     3,     0,     3,     5,     2,     3,
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
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3219 "ds_parser.cpp"
        break;

    case 178: /* character_sequence  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3225 "ds_parser.cpp"
        break;

    case 179: /* string_constant  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3231 "ds_parser.cpp"
        break;

    case 180: /* string_builder_body  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3237 "ds_parser.cpp"
        break;

    case 181: /* string_builder  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3243 "ds_parser.cpp"
        break;

    case 184: /* require_module_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3249 "ds_parser.cpp"
        break;

    case 190: /* expression_label  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3255 "ds_parser.cpp"
        break;

    case 191: /* expression_goto  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3261 "ds_parser.cpp"
        break;

    case 193: /* expression_else  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3267 "ds_parser.cpp"
        break;

    case 195: /* expression_if_then_else  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3273 "ds_parser.cpp"
        break;

    case 196: /* expression_for_loop  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3279 "ds_parser.cpp"
        break;

    case 197: /* expression_while_loop  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3285 "ds_parser.cpp"
        break;

    case 198: /* expression_with  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3291 "ds_parser.cpp"
        break;

    case 199: /* annotation_argument_value  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3297 "ds_parser.cpp"
        break;

    case 200: /* annotation_argument_value_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3303 "ds_parser.cpp"
        break;

    case 201: /* annotation_argument  */
#line 120 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3309 "ds_parser.cpp"
        break;

    case 202: /* annotation_argument_list  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3315 "ds_parser.cpp"
        break;

    case 203: /* annotation_declaration_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3321 "ds_parser.cpp"
        break;

    case 204: /* annotation_declaration  */
#line 122 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3327 "ds_parser.cpp"
        break;

    case 205: /* annotation_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3333 "ds_parser.cpp"
        break;

    case 206: /* optional_annotation_list  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3339 "ds_parser.cpp"
        break;

    case 207: /* optional_function_argument_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3345 "ds_parser.cpp"
        break;

    case 208: /* optional_function_type  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3351 "ds_parser.cpp"
        break;

    case 209: /* function_name  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3357 "ds_parser.cpp"
        break;

    case 212: /* expression_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3363 "ds_parser.cpp"
        break;

    case 213: /* expression_any  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3369 "ds_parser.cpp"
        break;

    case 214: /* expressions  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3375 "ds_parser.cpp"
        break;

    case 215: /* expr_pipe  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3381 "ds_parser.cpp"
        break;

    case 216: /* name_in_namespace  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3387 "ds_parser.cpp"
        break;

    case 217: /* expression_delete  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3393 "ds_parser.cpp"
        break;

    case 218: /* expr_new  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3399 "ds_parser.cpp"
        break;

    case 219: /* expression_break  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3405 "ds_parser.cpp"
        break;

    case 220: /* expression_continue  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3411 "ds_parser.cpp"
        break;

    case 221: /* expression_return  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3417 "ds_parser.cpp"
        break;

    case 222: /* expression_yield  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3423 "ds_parser.cpp"
        break;

    case 223: /* expression_try_catch  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3429 "ds_parser.cpp"
        break;

    case 226: /* expression_let  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3435 "ds_parser.cpp"
        break;

    case 227: /* expr_cast  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3441 "ds_parser.cpp"
        break;

    case 234: /* expr_type_info  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3447 "ds_parser.cpp"
        break;

    case 241: /* expr_list  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3453 "ds_parser.cpp"
        break;

    case 242: /* block_or_simple_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3459 "ds_parser.cpp"
        break;

    case 244: /* expr_block  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3465 "ds_parser.cpp"
        break;

    case 245: /* expr_numeric_const  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3471 "ds_parser.cpp"
        break;

    case 246: /* expr_assign  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3477 "ds_parser.cpp"
        break;

    case 247: /* expr_assign_pipe  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3483 "ds_parser.cpp"
        break;

    case 248: /* expr_named_call  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3489 "ds_parser.cpp"
        break;

    case 249: /* expr_method_call  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3495 "ds_parser.cpp"
        break;

    case 250: /* func_addr_expr  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3501 "ds_parser.cpp"
        break;

    case 255: /* expr  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3507 "ds_parser.cpp"
        break;

    case 258: /* optional_field_annotation  */
#line 121 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3513 "ds_parser.cpp"
        break;

    case 260: /* structure_variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3519 "ds_parser.cpp"
        break;

    case 261: /* struct_variable_declaration_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3525 "ds_parser.cpp"
        break;

    case 262: /* function_argument_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3531 "ds_parser.cpp"
        break;

    case 263: /* function_argument_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3537 "ds_parser.cpp"
        break;

    case 264: /* tuple_type  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3543 "ds_parser.cpp"
        break;

    case 265: /* tuple_type_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3549 "ds_parser.cpp"
        break;

    case 266: /* variant_type  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3555 "ds_parser.cpp"
        break;

    case 267: /* variant_type_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3561 "ds_parser.cpp"
        break;

    case 269: /* variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3567 "ds_parser.cpp"
        break;

    case 272: /* let_variable_declaration  */
#line 116 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3573 "ds_parser.cpp"
        break;

    case 273: /* global_variable_declaration_list  */
#line 117 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3579 "ds_parser.cpp"
        break;

    case 277: /* enum_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3585 "ds_parser.cpp"
        break;

    case 282: /* optional_structure_parent  */
#line 113 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3591 "ds_parser.cpp"
        break;

    case 286: /* variable_name_with_pos_list  */
#line 115 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3597 "ds_parser.cpp"
        break;

    case 289: /* structure_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3603 "ds_parser.cpp"
        break;

    case 290: /* auto_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3609 "ds_parser.cpp"
        break;

    case 291: /* bitfield_bits  */
#line 114 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3615 "ds_parser.cpp"
        break;

    case 292: /* bitfield_type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3621 "ds_parser.cpp"
        break;

    case 295: /* type_declaration  */
#line 118 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3627 "ds_parser.cpp"
        break;

    case 324: /* make_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3633 "ds_parser.cpp"
        break;

    case 325: /* make_struct_fields  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3639 "ds_parser.cpp"
        break;

    case 326: /* make_struct_dim  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3645 "ds_parser.cpp"
        break;

    case 327: /* make_struct_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3651 "ds_parser.cpp"
        break;

    case 328: /* make_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3657 "ds_parser.cpp"
        break;

    case 329: /* make_map_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3663 "ds_parser.cpp"
        break;

    case 330: /* make_any_tuple  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3669 "ds_parser.cpp"
        break;

    case 331: /* make_dim  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3675 "ds_parser.cpp"
        break;

    case 332: /* make_dim_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3681 "ds_parser.cpp"
        break;

    case 333: /* make_table  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3687 "ds_parser.cpp"
        break;

    case 334: /* make_table_decl  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3693 "ds_parser.cpp"
        break;

    case 335: /* array_comprehension_where  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3699 "ds_parser.cpp"
        break;

    case 336: /* array_comprehension  */
#line 119 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3705 "ds_parser.cpp"
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
#line 422 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4005 "ds_parser.cpp"
    break;

  case 16:
#line 434 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4011 "ds_parser.cpp"
    break;

  case 17:
#line 435 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4017 "ds_parser.cpp"
    break;

  case 18:
#line 439 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4023 "ds_parser.cpp"
    break;

  case 19:
#line 443 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4033 "ds_parser.cpp"
    break;

  case 20:
#line 448 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4047 "ds_parser.cpp"
    break;

  case 21:
#line 457 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4057 "ds_parser.cpp"
    break;

  case 22:
#line 465 "ds_parser.ypp"
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
#line 4075 "ds_parser.cpp"
    break;

  case 23:
#line 481 "ds_parser.ypp"
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
#line 4099 "ds_parser.cpp"
    break;

  case 25:
#line 507 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4107 "ds_parser.cpp"
    break;

  case 26:
#line 510 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4118 "ds_parser.cpp"
    break;

  case 27:
#line 516 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4129 "ds_parser.cpp"
    break;

  case 28:
#line 525 "ds_parser.ypp"
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
#line 4155 "ds_parser.cpp"
    break;

  case 29:
#line 546 "ds_parser.ypp"
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
#line 4180 "ds_parser.cpp"
    break;

  case 30:
#line 569 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4186 "ds_parser.cpp"
    break;

  case 31:
#line 570 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4192 "ds_parser.cpp"
    break;

  case 35:
#line 583 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4200 "ds_parser.cpp"
    break;

  case 36:
#line 586 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4208 "ds_parser.cpp"
    break;

  case 37:
#line 592 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4216 "ds_parser.cpp"
    break;

  case 38:
#line 598 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4224 "ds_parser.cpp"
    break;

  case 39:
#line 601 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4232 "ds_parser.cpp"
    break;

  case 40:
#line 607 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4238 "ds_parser.cpp"
    break;

  case 41:
#line 608 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4244 "ds_parser.cpp"
    break;

  case 42:
#line 612 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4250 "ds_parser.cpp"
    break;

  case 43:
#line 613 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4256 "ds_parser.cpp"
    break;

  case 44:
#line 614 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4267 "ds_parser.cpp"
    break;

  case 45:
#line 623 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4273 "ds_parser.cpp"
    break;

  case 46:
#line 624 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4279 "ds_parser.cpp"
    break;

  case 47:
#line 628 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4290 "ds_parser.cpp"
    break;

  case 48:
#line 637 "ds_parser.ypp"
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
#line 4307 "ds_parser.cpp"
    break;

  case 49:
#line 652 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4319 "ds_parser.cpp"
    break;

  case 50:
#line 662 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4330 "ds_parser.cpp"
    break;

  case 51:
#line 671 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4336 "ds_parser.cpp"
    break;

  case 52:
#line 672 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4342 "ds_parser.cpp"
    break;

  case 53:
#line 673 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4348 "ds_parser.cpp"
    break;

  case 54:
#line 674 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4354 "ds_parser.cpp"
    break;

  case 55:
#line 675 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4360 "ds_parser.cpp"
    break;

  case 56:
#line 676 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4366 "ds_parser.cpp"
    break;

  case 57:
#line 680 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
	}
#line 4376 "ds_parser.cpp"
    break;

  case 58:
#line 685 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4386 "ds_parser.cpp"
    break;

  case 59:
#line 693 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4392 "ds_parser.cpp"
    break;

  case 60:
#line 694 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4398 "ds_parser.cpp"
    break;

  case 61:
#line 695 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4404 "ds_parser.cpp"
    break;

  case 62:
#line 696 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4410 "ds_parser.cpp"
    break;

  case 63:
#line 697 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4416 "ds_parser.cpp"
    break;

  case 64:
#line 698 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4422 "ds_parser.cpp"
    break;

  case 65:
#line 699 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4428 "ds_parser.cpp"
    break;

  case 66:
#line 700 "ds_parser.ypp"
    {
		{ (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
	}
#line 4436 "ds_parser.cpp"
    break;

  case 67:
#line 706 "ds_parser.ypp"
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
#line 4454 "ds_parser.cpp"
    break;

  case 68:
#line 719 "ds_parser.ypp"
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
#line 4472 "ds_parser.cpp"
    break;

  case 69:
#line 735 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4478 "ds_parser.cpp"
    break;

  case 70:
#line 736 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4484 "ds_parser.cpp"
    break;

  case 71:
#line 740 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4496 "ds_parser.cpp"
    break;

  case 72:
#line 747 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4510 "ds_parser.cpp"
    break;

  case 73:
#line 759 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4519 "ds_parser.cpp"
    break;

  case 74:
#line 763 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4528 "ds_parser.cpp"
    break;

  case 75:
#line 770 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4534 "ds_parser.cpp"
    break;

  case 76:
#line 771 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4540 "ds_parser.cpp"
    break;

  case 77:
#line 775 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4546 "ds_parser.cpp"
    break;

  case 78:
#line 776 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4552 "ds_parser.cpp"
    break;

  case 79:
#line 777 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4558 "ds_parser.cpp"
    break;

  case 80:
#line 781 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4568 "ds_parser.cpp"
    break;

  case 81:
#line 786 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4577 "ds_parser.cpp"
    break;

  case 82:
#line 793 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
		(yyval.s) = (yyvsp[0].s);
	}
#line 4586 "ds_parser.cpp"
    break;

  case 83:
#line 797 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4592 "ds_parser.cpp"
    break;

  case 84:
#line 798 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4598 "ds_parser.cpp"
    break;

  case 85:
#line 799 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4604 "ds_parser.cpp"
    break;

  case 86:
#line 800 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4610 "ds_parser.cpp"
    break;

  case 87:
#line 801 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4616 "ds_parser.cpp"
    break;

  case 88:
#line 802 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4622 "ds_parser.cpp"
    break;

  case 89:
#line 803 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4628 "ds_parser.cpp"
    break;

  case 90:
#line 804 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4634 "ds_parser.cpp"
    break;

  case 91:
#line 805 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4640 "ds_parser.cpp"
    break;

  case 92:
#line 806 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4646 "ds_parser.cpp"
    break;

  case 93:
#line 807 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 4652 "ds_parser.cpp"
    break;

  case 94:
#line 808 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 4658 "ds_parser.cpp"
    break;

  case 95:
#line 809 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 4664 "ds_parser.cpp"
    break;

  case 96:
#line 810 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 4670 "ds_parser.cpp"
    break;

  case 97:
#line 811 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 4676 "ds_parser.cpp"
    break;

  case 98:
#line 812 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 4682 "ds_parser.cpp"
    break;

  case 99:
#line 813 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 4688 "ds_parser.cpp"
    break;

  case 100:
#line 814 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 4694 "ds_parser.cpp"
    break;

  case 101:
#line 815 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 4700 "ds_parser.cpp"
    break;

  case 102:
#line 816 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 4706 "ds_parser.cpp"
    break;

  case 103:
#line 817 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 4712 "ds_parser.cpp"
    break;

  case 104:
#line 818 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 4718 "ds_parser.cpp"
    break;

  case 105:
#line 819 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 4724 "ds_parser.cpp"
    break;

  case 106:
#line 820 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 4730 "ds_parser.cpp"
    break;

  case 107:
#line 821 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 4736 "ds_parser.cpp"
    break;

  case 108:
#line 822 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 4742 "ds_parser.cpp"
    break;

  case 109:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 4748 "ds_parser.cpp"
    break;

  case 110:
#line 824 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 4754 "ds_parser.cpp"
    break;

  case 111:
#line 825 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 4760 "ds_parser.cpp"
    break;

  case 112:
#line 826 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 4766 "ds_parser.cpp"
    break;

  case 113:
#line 827 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 4772 "ds_parser.cpp"
    break;

  case 114:
#line 828 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 4778 "ds_parser.cpp"
    break;

  case 115:
#line 829 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 4784 "ds_parser.cpp"
    break;

  case 116:
#line 830 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 4790 "ds_parser.cpp"
    break;

  case 117:
#line 831 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 4796 "ds_parser.cpp"
    break;

  case 118:
#line 832 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 4802 "ds_parser.cpp"
    break;

  case 119:
#line 836 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-1]),(yylsp[0]));
        runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt((yylsp[-2])));
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
#line 4824 "ds_parser.cpp"
    break;

  case 120:
#line 857 "ds_parser.ypp"
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-3]));
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
#line 4865 "ds_parser.cpp"
    break;

  case 121:
#line 896 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4873 "ds_parser.cpp"
    break;

  case 122:
#line 899 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        delete (yyvsp[-1].pExpression);
    }
#line 4885 "ds_parser.cpp"
    break;

  case 123:
#line 909 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4891 "ds_parser.cpp"
    break;

  case 124:
#line 910 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4897 "ds_parser.cpp"
    break;

  case 125:
#line 911 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4903 "ds_parser.cpp"
    break;

  case 126:
#line 912 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4909 "ds_parser.cpp"
    break;

  case 127:
#line 913 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4915 "ds_parser.cpp"
    break;

  case 128:
#line 914 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4921 "ds_parser.cpp"
    break;

  case 129:
#line 915 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4927 "ds_parser.cpp"
    break;

  case 130:
#line 916 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4933 "ds_parser.cpp"
    break;

  case 131:
#line 917 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4939 "ds_parser.cpp"
    break;

  case 132:
#line 918 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4945 "ds_parser.cpp"
    break;

  case 133:
#line 919 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4951 "ds_parser.cpp"
    break;

  case 134:
#line 920 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4957 "ds_parser.cpp"
    break;

  case 135:
#line 921 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4963 "ds_parser.cpp"
    break;

  case 136:
#line 922 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4969 "ds_parser.cpp"
    break;

  case 137:
#line 923 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4975 "ds_parser.cpp"
    break;

  case 138:
#line 924 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4981 "ds_parser.cpp"
    break;

  case 139:
#line 925 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 4987 "ds_parser.cpp"
    break;

  case 140:
#line 926 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4993 "ds_parser.cpp"
    break;

  case 141:
#line 930 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
		(yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5003 "ds_parser.cpp"
    break;

  case 142:
#line 935 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5014 "ds_parser.cpp"
    break;

  case 143:
#line 941 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5022 "ds_parser.cpp"
    break;

  case 144:
#line 947 "ds_parser.ypp"
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
#line 5038 "ds_parser.cpp"
    break;

  case 145:
#line 958 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5046 "ds_parser.cpp"
    break;

  case 146:
#line 964 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5052 "ds_parser.cpp"
    break;

  case 147:
#line 965 "ds_parser.ypp"
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
#line 5068 "ds_parser.cpp"
    break;

  case 148:
#line 976 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5074 "ds_parser.cpp"
    break;

  case 149:
#line 980 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5082 "ds_parser.cpp"
    break;

  case 150:
#line 986 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5090 "ds_parser.cpp"
    break;

  case 151:
#line 989 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5098 "ds_parser.cpp"
    break;

  case 152:
#line 992 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5107 "ds_parser.cpp"
    break;

  case 153:
#line 996 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5115 "ds_parser.cpp"
    break;

  case 154:
#line 1002 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5121 "ds_parser.cpp"
    break;

  case 155:
#line 1006 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5127 "ds_parser.cpp"
    break;

  case 156:
#line 1010 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5135 "ds_parser.cpp"
    break;

  case 157:
#line 1013 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5143 "ds_parser.cpp"
    break;

  case 158:
#line 1016 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5153 "ds_parser.cpp"
    break;

  case 159:
#line 1021 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5161 "ds_parser.cpp"
    break;

  case 160:
#line 1024 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5171 "ds_parser.cpp"
    break;

  case 161:
#line 1032 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5179 "ds_parser.cpp"
    break;

  case 162:
#line 1035 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5189 "ds_parser.cpp"
    break;

  case 163:
#line 1040 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5197 "ds_parser.cpp"
    break;

  case 164:
#line 1043 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5207 "ds_parser.cpp"
    break;

  case 165:
#line 1051 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5215 "ds_parser.cpp"
    break;

  case 166:
#line 1057 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5221 "ds_parser.cpp"
    break;

  case 167:
#line 1058 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5227 "ds_parser.cpp"
    break;

  case 168:
#line 1062 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5233 "ds_parser.cpp"
    break;

  case 169:
#line 1063 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5239 "ds_parser.cpp"
    break;

  case 170:
#line 1067 "ds_parser.ypp"
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
#line 5275 "ds_parser.cpp"
    break;

  case 171:
#line 1101 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5281 "ds_parser.cpp"
    break;

  case 172:
#line 1101 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5287 "ds_parser.cpp"
    break;

  case 173:
#line 1101 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5295 "ds_parser.cpp"
    break;

  case 174:
#line 1104 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5301 "ds_parser.cpp"
    break;

  case 175:
#line 1104 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5307 "ds_parser.cpp"
    break;

  case 176:
#line 1104 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5317 "ds_parser.cpp"
    break;

  case 177:
#line 1109 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5323 "ds_parser.cpp"
    break;

  case 178:
#line 1109 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5329 "ds_parser.cpp"
    break;

  case 179:
#line 1109 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5339 "ds_parser.cpp"
    break;

  case 180:
#line 1117 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5348 "ds_parser.cpp"
    break;

  case 181:
#line 1121 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5358 "ds_parser.cpp"
    break;

  case 182:
#line 1126 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5369 "ds_parser.cpp"
    break;

  case 183:
#line 1132 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5375 "ds_parser.cpp"
    break;

  case 184:
#line 1132 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5381 "ds_parser.cpp"
    break;

  case 185:
#line 1132 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5390 "ds_parser.cpp"
    break;

  case 186:
#line 1136 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5396 "ds_parser.cpp"
    break;

  case 187:
#line 1136 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5402 "ds_parser.cpp"
    break;

  case 188:
#line 1136 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5412 "ds_parser.cpp"
    break;

  case 189:
#line 1141 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5418 "ds_parser.cpp"
    break;

  case 190:
#line 1141 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5424 "ds_parser.cpp"
    break;

  case 191:
#line 1141 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5435 "ds_parser.cpp"
    break;

  case 192:
#line 1150 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5443 "ds_parser.cpp"
    break;

  case 193:
#line 1153 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5451 "ds_parser.cpp"
    break;

  case 194:
#line 1159 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5457 "ds_parser.cpp"
    break;

  case 195:
#line 1160 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5469 "ds_parser.cpp"
    break;

  case 196:
#line 1167 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
			retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5482 "ds_parser.cpp"
    break;

  case 197:
#line 1178 "ds_parser.ypp"
    { (yyval.b) = false;   /* block */  }
#line 5488 "ds_parser.cpp"
    break;

  case 198:
#line 1179 "ds_parser.ypp"
    { (yyval.b) = true;    /* lambda */ }
#line 5494 "ds_parser.cpp"
    break;

  case 199:
#line 1183 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5504 "ds_parser.cpp"
    break;

  case 200:
#line 1189 "ds_parser.ypp"
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
#line 5560 "ds_parser.cpp"
    break;

  case 201:
#line 1243 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5566 "ds_parser.cpp"
    break;

  case 202:
#line 1244 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5572 "ds_parser.cpp"
    break;

  case 203:
#line 1245 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5578 "ds_parser.cpp"
    break;

  case 204:
#line 1246 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5584 "ds_parser.cpp"
    break;

  case 205:
#line 1247 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5590 "ds_parser.cpp"
    break;

  case 206:
#line 1248 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5596 "ds_parser.cpp"
    break;

  case 207:
#line 1252 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5602 "ds_parser.cpp"
    break;

  case 208:
#line 1253 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5608 "ds_parser.cpp"
    break;

  case 209:
#line 1254 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5614 "ds_parser.cpp"
    break;

  case 210:
#line 1255 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5620 "ds_parser.cpp"
    break;

  case 211:
#line 1256 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5626 "ds_parser.cpp"
    break;

  case 212:
#line 1257 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5632 "ds_parser.cpp"
    break;

  case 213:
#line 1258 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5638 "ds_parser.cpp"
    break;

  case 214:
#line 1259 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5644 "ds_parser.cpp"
    break;

  case 215:
#line 1260 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5650 "ds_parser.cpp"
    break;

  case 216:
#line 1261 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5656 "ds_parser.cpp"
    break;

  case 217:
#line 1262 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5662 "ds_parser.cpp"
    break;

  case 218:
#line 1263 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5668 "ds_parser.cpp"
    break;

  case 219:
#line 1264 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5674 "ds_parser.cpp"
    break;

  case 220:
#line 1265 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5680 "ds_parser.cpp"
    break;

  case 221:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5686 "ds_parser.cpp"
    break;

  case 222:
#line 1267 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5692 "ds_parser.cpp"
    break;

  case 223:
#line 1271 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5698 "ds_parser.cpp"
    break;

  case 224:
#line 1272 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5704 "ds_parser.cpp"
    break;

  case 225:
#line 1276 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 5716 "ds_parser.cpp"
    break;

  case 226:
#line 1287 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5726 "ds_parser.cpp"
    break;

  case 227:
#line 1292 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 5738 "ds_parser.cpp"
    break;

  case 228:
#line 1302 "ds_parser.ypp"
    {
		(yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
	}
#line 5747 "ds_parser.cpp"
    break;

  case 229:
#line 1306 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5753 "ds_parser.cpp"
    break;

  case 230:
#line 1306 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5759 "ds_parser.cpp"
    break;

  case 231:
#line 1306 "ds_parser.ypp"
    {
		auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
		delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
		(yyval.pExpression) = expr;
    }
#line 5771 "ds_parser.cpp"
    break;

  case 232:
#line 1313 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5777 "ds_parser.cpp"
    break;

  case 233:
#line 1313 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5783 "ds_parser.cpp"
    break;

  case 234:
#line 1313 "ds_parser.ypp"
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
#line 5799 "ds_parser.cpp"
    break;

  case 235:
#line 1327 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 5805 "ds_parser.cpp"
    break;

  case 236:
#line 1328 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5811 "ds_parser.cpp"
    break;

  case 237:
#line 1329 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5817 "ds_parser.cpp"
    break;

  case 238:
#line 1330 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5823 "ds_parser.cpp"
    break;

  case 239:
#line 1331 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5829 "ds_parser.cpp"
    break;

  case 240:
#line 1332 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 5835 "ds_parser.cpp"
    break;

  case 241:
#line 1333 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 5841 "ds_parser.cpp"
    break;

  case 242:
#line 1334 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5847 "ds_parser.cpp"
    break;

  case 243:
#line 1335 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5853 "ds_parser.cpp"
    break;

  case 244:
#line 1336 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5859 "ds_parser.cpp"
    break;

  case 245:
#line 1337 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 5865 "ds_parser.cpp"
    break;

  case 246:
#line 1338 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5871 "ds_parser.cpp"
    break;

  case 247:
#line 1339 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5877 "ds_parser.cpp"
    break;

  case 248:
#line 1340 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5883 "ds_parser.cpp"
    break;

  case 249:
#line 1341 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5889 "ds_parser.cpp"
    break;

  case 250:
#line 1342 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5895 "ds_parser.cpp"
    break;

  case 251:
#line 1343 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5901 "ds_parser.cpp"
    break;

  case 252:
#line 1344 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5907 "ds_parser.cpp"
    break;

  case 253:
#line 1345 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5913 "ds_parser.cpp"
    break;

  case 254:
#line 1346 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5919 "ds_parser.cpp"
    break;

  case 255:
#line 1347 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5925 "ds_parser.cpp"
    break;

  case 256:
#line 1348 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5931 "ds_parser.cpp"
    break;

  case 257:
#line 1349 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5937 "ds_parser.cpp"
    break;

  case 258:
#line 1350 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5943 "ds_parser.cpp"
    break;

  case 259:
#line 1351 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5949 "ds_parser.cpp"
    break;

  case 260:
#line 1352 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5955 "ds_parser.cpp"
    break;

  case 261:
#line 1353 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5961 "ds_parser.cpp"
    break;

  case 262:
#line 1354 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5967 "ds_parser.cpp"
    break;

  case 263:
#line 1355 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5973 "ds_parser.cpp"
    break;

  case 264:
#line 1356 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5979 "ds_parser.cpp"
    break;

  case 265:
#line 1357 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 5985 "ds_parser.cpp"
    break;

  case 266:
#line 1358 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5991 "ds_parser.cpp"
    break;

  case 267:
#line 1359 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 5997 "ds_parser.cpp"
    break;

  case 268:
#line 1360 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6003 "ds_parser.cpp"
    break;

  case 269:
#line 1361 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6009 "ds_parser.cpp"
    break;

  case 270:
#line 1362 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6015 "ds_parser.cpp"
    break;

  case 271:
#line 1363 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6021 "ds_parser.cpp"
    break;

  case 272:
#line 1364 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6027 "ds_parser.cpp"
    break;

  case 273:
#line 1365 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6033 "ds_parser.cpp"
    break;

  case 274:
#line 1366 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6042 "ds_parser.cpp"
    break;

  case 275:
#line 1370 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6051 "ds_parser.cpp"
    break;

  case 276:
#line 1374 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6059 "ds_parser.cpp"
    break;

  case 277:
#line 1377 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6067 "ds_parser.cpp"
    break;

  case 278:
#line 1380 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6073 "ds_parser.cpp"
    break;

  case 279:
#line 1381 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6079 "ds_parser.cpp"
    break;

  case 280:
#line 1382 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6085 "ds_parser.cpp"
    break;

  case 281:
#line 1383 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-5])));
        gen->iterType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6095 "ds_parser.cpp"
    break;

  case 282:
#line 1388 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pExpression) = gen;
    }
#line 6105 "ds_parser.cpp"
    break;

  case 283:
#line 1393 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6111 "ds_parser.cpp"
    break;

  case 284:
#line 1394 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6119 "ds_parser.cpp"
    break;

  case 285:
#line 1397 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6125 "ds_parser.cpp"
    break;

  case 286:
#line 1397 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6131 "ds_parser.cpp"
    break;

  case 287:
#line 1397 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6139 "ds_parser.cpp"
    break;

  case 288:
#line 1400 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6148 "ds_parser.cpp"
    break;

  case 289:
#line 1404 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
		delete (yyvsp[0].s);
    }
#line 6157 "ds_parser.cpp"
    break;

  case 290:
#line 1408 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6166 "ds_parser.cpp"
    break;

  case 291:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6172 "ds_parser.cpp"
    break;

  case 292:
#line 1413 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6178 "ds_parser.cpp"
    break;

  case 293:
#line 1414 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6184 "ds_parser.cpp"
    break;

  case 294:
#line 1415 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6190 "ds_parser.cpp"
    break;

  case 295:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6196 "ds_parser.cpp"
    break;

  case 296:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6202 "ds_parser.cpp"
    break;

  case 297:
#line 1418 "ds_parser.ypp"
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
#line 6217 "ds_parser.cpp"
    break;

  case 298:
#line 1428 "ds_parser.ypp"
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
#line 6232 "ds_parser.cpp"
    break;

  case 299:
#line 1438 "ds_parser.ypp"
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
#line 6289 "ds_parser.cpp"
    break;

  case 300:
#line 1493 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6295 "ds_parser.cpp"
    break;

  case 301:
#line 1494 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6301 "ds_parser.cpp"
    break;

  case 302:
#line 1498 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6307 "ds_parser.cpp"
    break;

  case 303:
#line 1499 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6313 "ds_parser.cpp"
    break;

  case 304:
#line 1503 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6323 "ds_parser.cpp"
    break;

  case 305:
#line 1511 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6331 "ds_parser.cpp"
    break;

  case 306:
#line 1514 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6340 "ds_parser.cpp"
    break;

  case 307:
#line 1518 "ds_parser.ypp"
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-2]),(yylsp[0]));
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
                (yyvsp[-4].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure);
            } else {
                if ( (yyvsp[-1].b) ) {
                    das_yyerror("can't override constructor " + (yyvsp[0].pFuncDecl)->getMangledName(),
                        (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                modifyToConstructor((yyvsp[0].pFuncDecl), g_thisStructure);
            }
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt((yylsp[-3])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 6383 "ds_parser.cpp"
    break;

  case 308:
#line 1559 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
		}
#line 6393 "ds_parser.cpp"
    break;

  case 309:
#line 1564 "ds_parser.ypp"
    {
			(yyval.pVarDecl) = (yyvsp[0].pVarDecl);
			(yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
			(yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
		}
#line 6403 "ds_parser.cpp"
    break;

  case 310:
#line 1572 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6409 "ds_parser.cpp"
    break;

  case 311:
#line 1573 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6415 "ds_parser.cpp"
    break;

  case 312:
#line 1577 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6423 "ds_parser.cpp"
    break;

  case 313:
#line 1580 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6434 "ds_parser.cpp"
    break;

  case 314:
#line 1589 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6440 "ds_parser.cpp"
    break;

  case 315:
#line 1590 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6446 "ds_parser.cpp"
    break;

  case 316:
#line 1594 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6457 "ds_parser.cpp"
    break;

  case 317:
#line 1603 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6463 "ds_parser.cpp"
    break;

  case 318:
#line 1604 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6469 "ds_parser.cpp"
    break;

  case 319:
#line 1609 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6475 "ds_parser.cpp"
    break;

  case 320:
#line 1610 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6481 "ds_parser.cpp"
    break;

  case 321:
#line 1614 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6492 "ds_parser.cpp"
    break;

  case 322:
#line 1620 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6503 "ds_parser.cpp"
    break;

  case 323:
#line 1626 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6511 "ds_parser.cpp"
    break;

  case 324:
#line 1629 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6520 "ds_parser.cpp"
    break;

  case 325:
#line 1633 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6531 "ds_parser.cpp"
    break;

  case 326:
#line 1639 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6542 "ds_parser.cpp"
    break;

  case 327:
#line 1648 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 6548 "ds_parser.cpp"
    break;

  case 328:
#line 1649 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 6554 "ds_parser.cpp"
    break;

  case 329:
#line 1650 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 6560 "ds_parser.cpp"
    break;

  case 330:
#line 1654 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6566 "ds_parser.cpp"
    break;

  case 331:
#line 1655 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6572 "ds_parser.cpp"
    break;

  case 332:
#line 1659 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 6580 "ds_parser.cpp"
    break;

  case 333:
#line 1662 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6590 "ds_parser.cpp"
    break;

  case 334:
#line 1667 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6600 "ds_parser.cpp"
    break;

  case 335:
#line 1672 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 6613 "ds_parser.cpp"
    break;

  case 336:
#line 1680 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 6626 "ds_parser.cpp"
    break;

  case 337:
#line 1691 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6634 "ds_parser.cpp"
    break;

  case 338:
#line 1694 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 6643 "ds_parser.cpp"
    break;

  case 339:
#line 1701 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6649 "ds_parser.cpp"
    break;

  case 340:
#line 1702 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6655 "ds_parser.cpp"
    break;

  case 341:
#line 1706 "ds_parser.ypp"
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
#line 6687 "ds_parser.cpp"
    break;

  case 342:
#line 1733 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 6696 "ds_parser.cpp"
    break;

  case 343:
#line 1736 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 6707 "ds_parser.cpp"
    break;

  case 344:
#line 1745 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 6715 "ds_parser.cpp"
    break;

  case 345:
#line 1748 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s)) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 6729 "ds_parser.cpp"
    break;

  case 346:
#line 1757 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 6743 "ds_parser.cpp"
    break;

  case 347:
#line 1770 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 6757 "ds_parser.cpp"
    break;

  case 351:
#line 1791 "ds_parser.ypp"
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
#line 6773 "ds_parser.cpp"
    break;

  case 352:
#line 1802 "ds_parser.ypp"
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
#line 6790 "ds_parser.cpp"
    break;

  case 353:
#line 1817 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 6796 "ds_parser.cpp"
    break;

  case 354:
#line 1818 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 6802 "ds_parser.cpp"
    break;

  case 355:
#line 1822 "ds_parser.ypp"
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
#line 6844 "ds_parser.cpp"
    break;

  case 356:
#line 1862 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6850 "ds_parser.cpp"
    break;

  case 357:
#line 1863 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6856 "ds_parser.cpp"
    break;

  case 358:
#line 1867 "ds_parser.ypp"
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
#line 6935 "ds_parser.cpp"
    break;

  case 359:
#line 1944 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 6947 "ds_parser.cpp"
    break;

  case 360:
#line 1951 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 6958 "ds_parser.cpp"
    break;

  case 361:
#line 1960 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 6964 "ds_parser.cpp"
    break;

  case 362:
#line 1961 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 6970 "ds_parser.cpp"
    break;

  case 363:
#line 1962 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 6976 "ds_parser.cpp"
    break;

  case 364:
#line 1963 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 6982 "ds_parser.cpp"
    break;

  case 365:
#line 1964 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 6988 "ds_parser.cpp"
    break;

  case 366:
#line 1965 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 6994 "ds_parser.cpp"
    break;

  case 367:
#line 1966 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7000 "ds_parser.cpp"
    break;

  case 368:
#line 1967 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7006 "ds_parser.cpp"
    break;

  case 369:
#line 1968 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7012 "ds_parser.cpp"
    break;

  case 370:
#line 1969 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7018 "ds_parser.cpp"
    break;

  case 371:
#line 1970 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7024 "ds_parser.cpp"
    break;

  case 372:
#line 1971 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7030 "ds_parser.cpp"
    break;

  case 373:
#line 1972 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7036 "ds_parser.cpp"
    break;

  case 374:
#line 1973 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7042 "ds_parser.cpp"
    break;

  case 375:
#line 1974 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7048 "ds_parser.cpp"
    break;

  case 376:
#line 1975 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7054 "ds_parser.cpp"
    break;

  case 377:
#line 1976 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7060 "ds_parser.cpp"
    break;

  case 378:
#line 1977 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7066 "ds_parser.cpp"
    break;

  case 379:
#line 1978 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7072 "ds_parser.cpp"
    break;

  case 380:
#line 1979 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7078 "ds_parser.cpp"
    break;

  case 381:
#line 1980 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7084 "ds_parser.cpp"
    break;

  case 382:
#line 1981 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7090 "ds_parser.cpp"
    break;

  case 383:
#line 1982 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7096 "ds_parser.cpp"
    break;

  case 384:
#line 1983 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7102 "ds_parser.cpp"
    break;

  case 385:
#line 1984 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7108 "ds_parser.cpp"
    break;

  case 386:
#line 1988 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7114 "ds_parser.cpp"
    break;

  case 387:
#line 1989 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7120 "ds_parser.cpp"
    break;

  case 388:
#line 1990 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7126 "ds_parser.cpp"
    break;

  case 389:
#line 1991 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7132 "ds_parser.cpp"
    break;

  case 390:
#line 1992 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7138 "ds_parser.cpp"
    break;

  case 391:
#line 1993 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7144 "ds_parser.cpp"
    break;

  case 392:
#line 1997 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7157 "ds_parser.cpp"
    break;

  case 393:
#line 2008 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7166 "ds_parser.cpp"
    break;

  case 394:
#line 2012 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7178 "ds_parser.cpp"
    break;

  case 395:
#line 2022 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7190 "ds_parser.cpp"
    break;

  case 396:
#line 2029 "ds_parser.ypp"
    {
		das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7201 "ds_parser.cpp"
    break;

  case 397:
#line 2038 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7207 "ds_parser.cpp"
    break;

  case 398:
#line 2038 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7213 "ds_parser.cpp"
    break;

  case 399:
#line 2038 "ds_parser.ypp"
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
#line 7228 "ds_parser.cpp"
    break;

  case 400:
#line 2051 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7234 "ds_parser.cpp"
    break;

  case 401:
#line 2052 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7240 "ds_parser.cpp"
    break;

  case 402:
#line 2053 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7246 "ds_parser.cpp"
    break;

  case 403:
#line 2054 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7252 "ds_parser.cpp"
    break;

  case 404:
#line 2055 "ds_parser.ypp"
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
#line 7271 "ds_parser.cpp"
    break;

  case 405:
#line 2069 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7282 "ds_parser.cpp"
    break;

  case 406:
#line 2075 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7291 "ds_parser.cpp"
    break;

  case 407:
#line 2079 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7301 "ds_parser.cpp"
    break;

  case 408:
#line 2084 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7311 "ds_parser.cpp"
    break;

  case 409:
#line 2089 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7321 "ds_parser.cpp"
    break;

  case 410:
#line 2094 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7331 "ds_parser.cpp"
    break;

  case 411:
#line 2099 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7340 "ds_parser.cpp"
    break;

  case 412:
#line 2103 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7349 "ds_parser.cpp"
    break;

  case 413:
#line 2107 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7359 "ds_parser.cpp"
    break;

  case 414:
#line 2112 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7368 "ds_parser.cpp"
    break;

  case 415:
#line 2116 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7378 "ds_parser.cpp"
    break;

  case 416:
#line 2121 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7384 "ds_parser.cpp"
    break;

  case 417:
#line 2121 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7390 "ds_parser.cpp"
    break;

  case 418:
#line 2121 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7401 "ds_parser.cpp"
    break;

  case 419:
#line 2127 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
		(yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
		(yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7413 "ds_parser.cpp"
    break;

  case 420:
#line 2134 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7419 "ds_parser.cpp"
    break;

  case 421:
#line 2134 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7425 "ds_parser.cpp"
    break;

  case 422:
#line 2134 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7435 "ds_parser.cpp"
    break;

  case 423:
#line 2139 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7441 "ds_parser.cpp"
    break;

  case 424:
#line 2139 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7447 "ds_parser.cpp"
    break;

  case 425:
#line 2139 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7458 "ds_parser.cpp"
    break;

  case 426:
#line 2145 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7464 "ds_parser.cpp"
    break;

  case 427:
#line 2145 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7470 "ds_parser.cpp"
    break;

  case 428:
#line 2145 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7480 "ds_parser.cpp"
    break;

  case 429:
#line 2150 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7489 "ds_parser.cpp"
    break;

  case 430:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7495 "ds_parser.cpp"
    break;

  case 431:
#line 2154 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7501 "ds_parser.cpp"
    break;

  case 432:
#line 2154 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7511 "ds_parser.cpp"
    break;

  case 433:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7517 "ds_parser.cpp"
    break;

  case 434:
#line 2159 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7523 "ds_parser.cpp"
    break;

  case 435:
#line 2159 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7537 "ds_parser.cpp"
    break;

  case 436:
#line 2168 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7546 "ds_parser.cpp"
    break;

  case 437:
#line 2172 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7552 "ds_parser.cpp"
    break;

  case 438:
#line 2172 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7558 "ds_parser.cpp"
    break;

  case 439:
#line 2172 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7568 "ds_parser.cpp"
    break;

  case 440:
#line 2177 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7574 "ds_parser.cpp"
    break;

  case 441:
#line 2177 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7580 "ds_parser.cpp"
    break;

  case 442:
#line 2177 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7594 "ds_parser.cpp"
    break;

  case 443:
#line 2186 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7603 "ds_parser.cpp"
    break;

  case 444:
#line 2190 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7609 "ds_parser.cpp"
    break;

  case 445:
#line 2190 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7615 "ds_parser.cpp"
    break;

  case 446:
#line 2190 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7625 "ds_parser.cpp"
    break;

  case 447:
#line 2195 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7631 "ds_parser.cpp"
    break;

  case 448:
#line 2195 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7637 "ds_parser.cpp"
    break;

  case 449:
#line 2195 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7651 "ds_parser.cpp"
    break;

  case 450:
#line 2204 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7657 "ds_parser.cpp"
    break;

  case 451:
#line 2204 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7663 "ds_parser.cpp"
    break;

  case 452:
#line 2204 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7674 "ds_parser.cpp"
    break;

  case 453:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7680 "ds_parser.cpp"
    break;

  case 454:
#line 2210 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7686 "ds_parser.cpp"
    break;

  case 455:
#line 2210 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 7697 "ds_parser.cpp"
    break;

  case 456:
#line 2219 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7703 "ds_parser.cpp"
    break;

  case 457:
#line 2219 "ds_parser.ypp"
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
#line 7720 "ds_parser.cpp"
    break;

  case 458:
#line 2234 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 7726 "ds_parser.cpp"
    break;

  case 459:
#line 2234 "ds_parser.ypp"
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
#line 7747 "ds_parser.cpp"
    break;

  case 460:
#line 2254 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7753 "ds_parser.cpp"
    break;

  case 461:
#line 2255 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7759 "ds_parser.cpp"
    break;

  case 462:
#line 2256 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7765 "ds_parser.cpp"
    break;

  case 463:
#line 2257 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7771 "ds_parser.cpp"
    break;

  case 464:
#line 2261 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 7783 "ds_parser.cpp"
    break;

  case 465:
#line 2268 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b));
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 7794 "ds_parser.cpp"
    break;

  case 466:
#line 2277 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 7804 "ds_parser.cpp"
    break;

  case 467:
#line 2282 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7813 "ds_parser.cpp"
    break;

  case 468:
#line 2289 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7823 "ds_parser.cpp"
    break;

  case 469:
#line 2294 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStructureOrDefaultValue();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->at = tokAt((yylsp[-3]));
        (yyval.pExpression) = msd;
    }
#line 7834 "ds_parser.cpp"
    break;

  case 470:
#line 2300 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7845 "ds_parser.cpp"
    break;

  case 471:
#line 2306 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7857 "ds_parser.cpp"
    break;

  case 472:
#line 2313 "ds_parser.ypp"
    {
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStructureOrDefaultValue *)(yyvsp[-2].pExpression))->useInitializer = true;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-6]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 7870 "ds_parser.cpp"
    break;

  case 473:
#line 2324 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 7878 "ds_parser.cpp"
    break;

  case 474:
#line 2327 "ds_parser.ypp"
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
#line 7894 "ds_parser.cpp"
    break;

  case 475:
#line 2341 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 7905 "ds_parser.cpp"
    break;

  case 476:
#line 2350 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7911 "ds_parser.cpp"
    break;

  case 477:
#line 2351 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7917 "ds_parser.cpp"
    break;

  case 478:
#line 2355 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7927 "ds_parser.cpp"
    break;

  case 479:
#line 2360 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7936 "ds_parser.cpp"
    break;

  case 480:
#line 2367 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7946 "ds_parser.cpp"
    break;

  case 481:
#line 2372 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 7959 "ds_parser.cpp"
    break;

  case 482:
#line 2383 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 7969 "ds_parser.cpp"
    break;

  case 483:
#line 2388 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 7978 "ds_parser.cpp"
    break;

  case 484:
#line 2395 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 7992 "ds_parser.cpp"
    break;

  case 485:
#line 2407 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 7998 "ds_parser.cpp"
    break;

  case 486:
#line 2408 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8004 "ds_parser.cpp"
    break;

  case 487:
#line 2412 "ds_parser.ypp"
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
#line 8026 "ds_parser.cpp"
    break;

  case 488:
#line 2429 "ds_parser.ypp"
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
#line 8048 "ds_parser.cpp"
    break;


#line 8052 "ds_parser.cpp"

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
#line 2448 "ds_parser.ypp"


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

void runFunctionAnnotations ( Function * func, AnnotationList * annL, const LineInfo & at ) {
    if ( annL ) {
        for ( auto pA : *annL ) {
            if ( pA->annotation ) {
                if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                    auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                    string err;
                    if ( !ann->apply(func, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                        das_yyerror("can't apply annotation\n" + err,
                            at, CompilationError::invalid_annotation);
                    }
                } else {
                    das_yyerror("functions are only allowed function annotations",
                        at, CompilationError::invalid_annotation);
                }
            }
        }
        swap ( func->annotations, *annL );
        delete annL;
    }
}
