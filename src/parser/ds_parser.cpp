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
#line 62 "ds_parser.ypp" /* yacc.c:338  */

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

    void das_yybegin_reader ( void );
    void das_yyend_reader ( void );

    __forceinline string inThisModule ( const string & name ) { return "_::" + name; }

#line 106 "ds_parser.cpp" /* yacc.c:338  */
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
        extern ProgramPtr               g_Program;
        extern FileAccessPtr            g_Access;
        extern vector<FileInfo *>       g_FileAccessStack;
        extern das_map<string,string>   das_module_alias;

        extern ReaderMacro *            g_ReaderMacro;
        extern ExprReader *             g_ReaderExpr;
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
    void varDeclToTypeDecl ( TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = true );
    void runFunctionAnnotations ( Function * func, AnnotationList * annL, const LineInfo & at );

    LineInfo tokAt ( const struct YYLTYPE & li );
    LineInfo tokRangeAt ( const struct YYLTYPE & li, const struct YYLTYPE & lie );

    Annotation * findAnnotation ( const string & name, const LineInfo & at );

    extern bool das_need_oxford_comma;
    extern bool das_force_oxford_comma;
    extern bool das_supress_errors;
    extern int das_arrow_depth;

    extern Structure * g_thisStructure;

#line 207 "ds_parser.cpp" /* yacc.c:353  */

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
    DAS_TYPEDEF = 294,
    DAS_WITH = 295,
    DAS_CAST = 296,
    DAS_OVERRIDE = 297,
    DAS_ABSTRACT = 298,
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
    DAS_EXPLICIT = 312,
    DAS_SHARED = 313,
    DAS_SMART_PTR = 314,
    DAS_UNSAFE = 315,
    DAS_TBOOL = 316,
    DAS_TVOID = 317,
    DAS_TSTRING = 318,
    DAS_TAUTO = 319,
    DAS_TINT = 320,
    DAS_TINT2 = 321,
    DAS_TINT3 = 322,
    DAS_TINT4 = 323,
    DAS_TUINT = 324,
    DAS_TBITFIELD = 325,
    DAS_TUINT2 = 326,
    DAS_TUINT3 = 327,
    DAS_TUINT4 = 328,
    DAS_TFLOAT = 329,
    DAS_TFLOAT2 = 330,
    DAS_TFLOAT3 = 331,
    DAS_TFLOAT4 = 332,
    DAS_TRANGE = 333,
    DAS_TURANGE = 334,
    DAS_TBLOCK = 335,
    DAS_TINT64 = 336,
    DAS_TUINT64 = 337,
    DAS_TDOUBLE = 338,
    DAS_TFUNCTION = 339,
    DAS_TLAMBDA = 340,
    DAS_TINT8 = 341,
    DAS_TUINT8 = 342,
    DAS_TINT16 = 343,
    DAS_TUINT16 = 344,
    DAS_TTUPLE = 345,
    DAS_TVARIANT = 346,
    DAS_GENERATOR = 347,
    DAS_YIELD = 348,
    ADDEQU = 349,
    SUBEQU = 350,
    DIVEQU = 351,
    MULEQU = 352,
    MODEQU = 353,
    ANDEQU = 354,
    OREQU = 355,
    XOREQU = 356,
    SHL = 357,
    SHR = 358,
    ADDADD = 359,
    SUBSUB = 360,
    LEEQU = 361,
    SHLEQU = 362,
    SHREQU = 363,
    GREQU = 364,
    EQUEQU = 365,
    NOTEQU = 366,
    RARROW = 367,
    LARROW = 368,
    QQ = 369,
    QDOT = 370,
    QBRA = 371,
    LPIPE = 372,
    LBPIPE = 373,
    LAPIPE = 374,
    RPIPE = 375,
    CLONEEQU = 376,
    ROTL = 377,
    ROTR = 378,
    ROTLEQU = 379,
    ROTREQU = 380,
    MAPTO = 381,
    COLCOL = 382,
    BRABRAB = 383,
    BRACBRB = 384,
    CBRCBRB = 385,
    INTEGER = 386,
    LONG_INTEGER = 387,
    UNSIGNED_INTEGER = 388,
    UNSIGNED_LONG_INTEGER = 389,
    FLOAT = 390,
    DOUBLE = 391,
    NAME = 392,
    BEGIN_STRING = 393,
    STRING_CHARACTER = 394,
    END_STRING = 395,
    BEGIN_STRING_EXPR = 396,
    END_STRING_EXPR = 397,
    END_OF_READ = 398,
    UNARY_MINUS = 399,
    UNARY_PLUS = 400,
    PRE_INC = 401,
    PRE_DEC = 402,
    POST_INC = 403,
    POST_DEC = 404,
    DEREF = 405
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED

union DAS_YYSTYPE
{
#line 94 "ds_parser.ypp" /* yacc.c:353  */

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
    CaptureEntry *                  pCapt;
    vector<CaptureEntry> *          pCaptList;

#line 399 "ds_parser.cpp" /* yacc.c:353  */
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
#define YYLAST   7322

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  516
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  922

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   405

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   161,     2,   177,   172,   157,   150,     2,
     170,   171,   155,   154,   144,   153,   166,   156,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   147,   174,
     151,   145,   152,   146,   173,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   168,     2,   169,   149,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   175,   148,   176,   160,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   158,
     159,   162,   163,   164,   165,   167
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   430,   430,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   447,   459,   460,   464,   468,
     473,   482,   490,   506,   511,   519,   519,   549,   571,   575,
     578,   584,   593,   614,   637,   638,   642,   646,   647,   651,
     654,   660,   666,   669,   675,   676,   680,   681,   682,   691,
     692,   696,   705,   721,   729,   739,   748,   749,   750,   751,
     752,   753,   757,   762,   770,   771,   772,   773,   774,   775,
     776,   777,   783,   796,   812,   813,   817,   824,   836,   840,
     847,   848,   852,   853,   854,   858,   863,   870,   874,   875,
     876,   877,   878,   879,   880,   881,   882,   883,   884,   885,
     886,   887,   888,   889,   890,   891,   892,   893,   894,   895,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,   908,   909,   913,   934,   972,   979,   983,   994,
     995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,
    1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1016,  1021,
    1027,  1033,  1054,  1060,  1061,  1072,  1076,  1082,  1085,  1088,
    1092,  1098,  1102,  1106,  1109,  1112,  1117,  1120,  1128,  1131,
    1136,  1139,  1147,  1153,  1154,  1158,  1192,  1192,  1192,  1195,
    1195,  1195,  1200,  1200,  1200,  1208,  1212,  1217,  1223,  1223,
    1223,  1227,  1227,  1227,  1232,  1232,  1232,  1241,  1244,  1250,
    1251,  1258,  1269,  1270,  1271,  1275,  1276,  1277,  1278,  1282,
    1287,  1295,  1296,  1300,  1305,  1369,  1370,  1371,  1372,  1373,
    1374,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,  1386,
    1387,  1388,  1389,  1390,  1391,  1392,  1393,  1397,  1398,  1402,
    1413,  1418,  1428,  1432,  1432,  1432,  1439,  1439,  1439,  1453,
    1457,  1457,  1457,  1465,  1466,  1467,  1468,  1469,  1470,  1471,
    1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,
    1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,
    1502,  1503,  1504,  1505,  1509,  1513,  1516,  1519,  1520,  1521,
    1522,  1531,  1540,  1541,  1544,  1544,  1544,  1547,  1551,  1555,
    1559,  1560,  1561,  1562,  1563,  1564,  1565,  1575,  1585,  1640,
    1641,  1645,  1646,  1650,  1658,  1661,  1666,  1665,  1710,  1768,
    1773,  1781,  1782,  1786,  1789,  1798,  1799,  1803,  1812,  1813,
    1818,  1819,  1823,  1829,  1835,  1838,  1842,  1848,  1857,  1858,
    1859,  1863,  1864,  1868,  1871,  1876,  1881,  1889,  1900,  1903,
    1910,  1911,  1915,  1942,  1942,  1954,  1957,  1966,  1979,  1991,
    1992,  1996,  2000,  2011,  2026,  2027,  2031,  2068,  2069,  2073,
    2073,  2157,  2164,  2173,  2174,  2175,  2176,  2177,  2178,  2179,
    2180,  2181,  2182,  2183,  2184,  2185,  2186,  2187,  2188,  2189,
    2190,  2191,  2192,  2193,  2194,  2195,  2196,  2197,  2201,  2202,
    2203,  2204,  2205,  2206,  2210,  2221,  2225,  2235,  2242,  2251,
    2251,  2251,  2264,  2265,  2266,  2267,  2268,  2282,  2288,  2292,
    2296,  2301,  2306,  2311,  2316,  2320,  2324,  2329,  2333,  2338,
    2338,  2338,  2344,  2351,  2351,  2351,  2356,  2356,  2356,  2362,
    2362,  2362,  2367,  2371,  2371,  2371,  2376,  2376,  2376,  2385,
    2389,  2389,  2389,  2394,  2394,  2394,  2403,  2407,  2407,  2407,
    2412,  2412,  2412,  2421,  2421,  2421,  2427,  2427,  2427,  2436,
    2436,  2451,  2451,  2471,  2472,  2473,  2474,  2478,  2485,  2492,
    2498,  2507,  2512,  2519,  2520,  2524,  2530,  2537,  2545,  2552,
    2560,  2572,  2575,  2589,  2598,  2599,  2603,  2608,  2615,  2620,
    2631,  2636,  2643,  2655,  2656,  2660,  2678
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
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"with\"",
  "\"cast\"", "\"override\"", "\"abstract\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"explicit\"", "\"shared\"", "\"smart_ptr\"",
  "\"unsafe\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"",
  "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"+=\"",
  "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"",
  "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"",
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"", "','", "'='", "'?'",
  "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'",
  "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "'.'", "DEREF", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "module_declaration", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@1", "options_declaration", "require_declaration",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_if_then_else", "expression_for_loop",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "function_declaration_header", "function_declaration",
  "expression_block", "expression_any", "expressions", "expr_pipe",
  "name_in_namespace", "expression_delete", "expr_new", "expression_break",
  "expression_continue", "expression_return", "expression_yield",
  "expression_try_catch", "kwd_let", "expression_let", "expr_cast", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "expr_type_info", "$@8", "$@9",
  "$@10", "$@11", "$@12", "$@13", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_expr", "$@14", "$@15", "$@16", "$@17", "expr_field", "$@18",
  "$@19", "expr", "$@20", "$@21", "optional_field_annotation",
  "optional_override", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@22",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list",
  "optional_shared", "global_let", "$@23", "enum_list", "single_alias",
  "alias_list", "alias_declaration", "enum_declaration",
  "optional_structure_parent", "structure_name", "class_or_struct",
  "structure_declaration", "$@24", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@25", "$@26", "type_declaration", "$@27",
  "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36",
  "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43", "$@44", "$@45",
  "$@46", "$@47", "$@48", "$@49", "$@50", "variant_alias_declaration",
  "$@51", "bitfield_alias_declaration", "$@52", "make_decl",
  "make_struct_fields", "make_struct_dim", "optional_block",
  "make_struct_decl", "make_tuple", "make_map_tuple", "make_any_tuple",
  "make_dim", "make_dim_decl", "make_table", "make_table_decl",
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
     395,   396,   397,   398,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   399,   400,
     126,    33,   401,   402,   403,   404,    46,   405,    91,    93,
      40,    41,    36,    64,    59,   123,   125,    35
};
# endif

#define YYPACT_NINF -586

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-586)))

#define YYTABLE_NINF -471

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -586,    25,  -586,  -586,   -58,   -22,   -11,    -5,   -42,  -586,
       1,  -586,  -586,    -6,  -586,  -586,  -586,  -586,  -586,   338,
    -586,   114,  -586,  -586,  -586,  -586,  -586,  -586,    45,  -586,
      67,    92,    70,  -586,  -586,     2,  -586,   -27,   105,  -586,
     119,   126,  -586,   175,    97,   147,  -586,    50,  -586,  -586,
    -586,     6,   186,  -586,   149,    15,   -58,   215,   -22,   218,
    -586,   220,   221,  -586,   251,  -586,   216,  -586,   -94,   185,
     187,  -586,   226,   -58,    -6,  -586,  7161,   330,   335,  -586,
     200,   199,  -586,   225,  -586,  -586,   378,  -586,  -586,  -586,
    -586,  -586,   241,    32,  -586,  -586,  -586,  -586,   328,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,   207,   -93,  7086,
    -586,  -586,   246,   247,  -586,   -20,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,   -71,   238,  -586,  -586,   -47,  -586,   212,
     -78,   214,  -586,   -53,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,   -10,  -586,  -586,   -77,  -586,   239,   240,   242,   243,
    -586,  -586,  -586,   222,  -586,  -586,  -586,  -586,  -586,   245,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,   248,
    -586,  -586,  -586,   249,   253,  -586,  -586,  -586,  -586,   254,
     255,  -586,  -586,  -586,  -586,  -586,   416,  -586,   223,   260,
    -586,   224,  -586,   -58,  -586,   -18,  -586,    27,  7086,  -586,
    1289,  -586,  -586,  -586,  -586,  -586,   197,  -586,  -586,  -586,
      32,  -586,   -74,  4676,  -586,  -586,  -586,  -586,  -586,   265,
    -586,   100,   117,   136,  -586,  -586,  -586,  -586,  -586,  -586,
     379,  -586,  -586,     5,  2618,  -586,  -586,   -72,  7086,   -70,
      51,   273,  -586,   -32,  -586,   284,  6735,  -586,  4676,  -586,
    -586,   273,  -586,  -586,  7009,   250,  2060,  -586,  -586,   199,
    4676,   257,  4676,   262,   264,   263,  -586,   258,   270,   293,
    2748,   199,  -586,   274,  2210,  4676,  4676,   178,  6852,  6932,
    4676,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  4676,  4676,
    4676,   -47,  4676,  4676,  4676,  -586,   267,  -586,   399,  -586,
    -586,   268,   269,  4676,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,   -63,   272,  -586,   275,   276,  -586,  -586,  -586,
     273,  -586,  -586,  -586,   279,  -586,  -586,   -85,  -586,  -586,
    -586,  -586,  -586,  5936,   271,  -586,  -586,  -586,  -586,  -586,
       7,   307,  7086,  -586,    26,  -586,  -586,   496,  7086,  7086,
    7086,  7086,   280,   246,  7086,   200,  7086,   200,  7086,   200,
    7163,   247,  -586,  -586,  -586,   283,  -586,  -586,  5612,  -586,
    -586,  6735,  -586,  -586,   285,  -586,  -586,  -586,  7086,  -586,
    2878,  -586,   388,    -2,  1911,  -586,   -47,  2878,  -586,  -586,
     337,   935,   440,  6212,  4676,   388,  -586,  -586,  4676,  -586,
    -586,   309,   326,  6212,  -586,  7086,  2878,  -586,  1092,   210,
     210,   288,  -586,   273,  1616,   273,  1764,  6003,  -586,    77,
     192,   210,   210,   -65,  -586,   210,   210,   681,   107,   289,
    -586,  -586,   388,  -586,  2340,  -586,  -586,  -586,  -586,   346,
     178,  -586,     3,   321,  4676,  4676,  4676,  4676,  4676,  4676,
    4676,  4676,  4676,  4676,  -586,  -586,  4676,  4676,  4676,  4676,
    4676,  4676,   329,  3008,  4676,   339,  4676,  4676,  4676,  4676,
    4676,  4676,  4676,  4676,  3138,  3268,  4676,  4676,  4676,  4676,
    4676,  4676,  4676,  4676,  4676,  4676,   340,  4676,  3398,  -586,
     456,   423,   301,  -586,  1111,  -586,  -586,  -586,  2878,  -586,
    2433,  3491,  3639,  3787,  -586,   -86,  3935,   238,  4083,   238,
    4673,   238,   162,  -586,    93,  6735,   102,  -586,  -586,  -586,
     382,  -586,  5830,  -586,  4676,  3546,  2488,  -586,  4758,  4676,
    4676,  -586,   199,  5192,  -586,  7086,  7086,  5262,  7086,  -586,
    -586,  6503,  -586,  4856,  -586,  -586,    13,   178,   -51,  1912,
    3694,  6003,   341,   -14,   315,   343,  -586,  -586,   -46,    20,
    3842,   -14,   180,  4676,  4676,   312,  -586,  4676,   344,   350,
    -586,   158,  -586,  -586,   296,   369,  -586,    35,  6212,     4,
     200,  -586,   356,  -586,  -586,  6212,  6212,  6212,  6212,  6212,
    6212,  6212,  6212,   962,   962,   314,  6212,  6212,   314,   848,
     848,   342,  4676,  6212,   145,  -586,  5680,   -67,   -67,  6212,
     962,   962,  6212,  6212,  4676,  6212,   372,  6074,  6279,  6350,
    6417,   314,   314,    72,    72,   145,   145,   145,  -586,   512,
    5750,  -586,    36,    30,  -586,   273,  -586,   379,  -586,  2878,
    -586,  4954,  -586,  7086,  -586,  -586,  -586,   377,  -586,   363,
    -586,   366,  -586,   367,  7086,  -586,  7163,  -586,   247,   379,
    4676,   -69,  -586,    47,   370,   383,  5332,  -586,  -586,  -586,
    6522,  6550,  -586,  6564,   346,  -586,  4676,  -586,  4676,  4676,
      -8,   386,   369,   355,   357,  4676,   360,  4676,  4676,   369,
     349,   362,  6212,  -586,  -586,  6141,  -586,   394,  7086,   200,
    1465,   199,  -586,  -586,  4676,  -586,    87,    66,  4676,  -586,
     409,   410,   411,   412,  -586,    68,   238,  -586,  3990,  6478,
    -586,  6478,  -586,  4676,  -586,  -586,  -586,  -586,     6,  -586,
    -586,  5052,  -586,  -586,  6583,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  6735,  -586,  -586,  -586,  6212,   199,
    -586,  -586,   116,  -586,  -586,  -586,  -586,   381,   -66,  6212,
    6212,   -14,   389,    96,   341,   391,  -586,  6212,  -586,  -586,
     -60,   -14,   395,  -586,  -586,  -586,  6609,   238,  -586,  -586,
     388,   397,  6212,  -586,  -586,  -586,  -586,     4,   396,   -84,
    7086,  -586,    52,  6212,  -586,     6,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  7086,  4138,   420,  4676,  4676,  4676,  4268,
    4676,   401,   402,  4676,  4676,  -586,  4676,   398,  -586,  -586,
     421,   296,  -586,  -586,  -586,  4416,  -586,  -586,  6620,  -586,
     403,  -586,  6630,   424,  5402,   427,   314,   314,   314,  -586,
    5472,  5119,   407,  -586,  6212,  6212,  5119,   413,   -47,  -586,
    -586,  4676,  6212,  -586,  -586,  -586,  -586,  -586,  4546,  -586,
     523,   414,  -586,   405,  -586,  -586,   -47,  6212,  -586,   417,
    7086,   434,  5542,  4676,   418,   422,  -586,  -586,  6649,  -586,
    -586,  6212,  -586,  -586,  -586,  7086,   425,  6689,  -586,  -586,
     432,  -586
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   173,     0,     0,     0,     0,     0,   174,
       0,   481,   479,     0,    14,     3,    10,     9,     8,     0,
       7,   360,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,     0,   153,    76,    78,     0,    74,   378,
     377,     0,     0,   361,   363,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   365,     0,   369,     0,     0,
       0,   155,     0,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   124,   374,   379,   358,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   408,   411,   409,   412,   410,   413,     0,     0,     0,
     371,   370,     0,     0,   154,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   116,   117,   114,   115,   107,
     118,   119,   108,   105,   106,   120,   121,   122,   123,   110,
     111,   109,   103,   104,    99,    98,   100,   101,   102,    89,
      88,   112,   113,   319,    85,   148,   126,     0,   376,     0,
       0,     0,    16,     0,    60,    61,    58,    59,    57,    56,
      62,     0,    33,   365,     0,   372,     0,     0,     0,     0,
     383,   403,   384,   415,   385,   389,   390,   391,   392,   407,
     396,   397,   398,   399,   400,   401,   402,   404,   405,   452,
     388,   395,   406,   459,   466,   386,   393,   387,   394,     0,
       0,   414,   422,   425,   423,   424,     0,   417,     0,     0,
     338,     0,    77,     0,    83,     0,   331,     0,     0,   125,
       0,   375,   324,   381,   362,   359,   351,   364,    17,    18,
       0,    71,     0,     0,   366,   443,   446,   449,   439,     0,
     419,   453,   460,   467,   473,   476,   430,   435,   429,   442,
       0,   438,   432,     0,     0,   368,   434,     0,     0,     0,
       0,     0,   329,   342,    84,   319,    86,   150,     0,    49,
      50,     0,   259,   260,     0,     0,     0,   253,   161,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,   215,   217,   216,   218,   219,   220,    19,     0,     0,
       0,     0,     0,     0,     0,   202,   203,   129,   127,   257,
     256,     0,     0,     0,   143,   138,   136,   135,   137,   213,
     149,   130,   254,     0,   312,     0,     0,   141,   142,   144,
       0,   134,   311,   310,    80,   315,   255,     0,   131,   314,
     313,   292,   261,   221,     0,   258,   483,   484,   485,   486,
     319,     0,     0,   352,     0,    63,   373,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,    82,     0,    82,
       0,     0,   437,   431,   433,     0,   436,   427,     0,   418,
     482,   337,   480,   339,     0,   330,   341,   340,     0,   343,
       0,   332,     0,     0,   157,   160,     0,     0,   163,   166,
       0,   221,     0,   156,     0,     0,   176,   179,     0,   147,
     182,     0,     0,    43,    53,     0,     0,   170,   221,   284,
     285,   203,   152,     0,   493,     0,     0,     0,   510,     0,
       0,   265,   264,   297,    25,   263,   262,     0,   204,     0,
     145,   146,     0,   318,     0,   133,   139,   140,   175,   211,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,     0,     0,   380,
       0,   321,     0,   382,     0,   349,   350,   348,     0,   367,
       0,     0,     0,     0,   416,     0,     0,    85,     0,    85,
       0,    85,   153,   335,     0,   333,     0,   428,   426,   320,
     344,   347,   346,    54,     0,     0,     0,   167,   221,     0,
       0,   164,     0,     0,    55,     0,     0,     0,     0,    41,
      42,     0,   171,   221,   168,   204,     0,     0,   153,     0,
       0,   501,   491,   493,     0,   504,   505,   506,     0,     0,
       0,   493,     0,     0,     0,     0,    22,     0,    20,     0,
     288,   243,   242,   148,    46,     0,   293,     0,   197,     0,
      82,   151,     0,   307,   308,   228,   229,   231,   230,   232,
     225,   226,   227,   266,   267,   279,   233,   234,   280,   277,
     278,     0,     0,   223,   302,   291,     0,   316,   317,   224,
     268,   269,   235,   236,     0,   222,     0,     0,   282,   283,
     281,   275,   276,   271,   270,   272,   273,   274,   249,     0,
       0,   295,     0,   321,   322,     0,   325,   348,   353,     0,
     357,   221,   444,     0,   450,   440,   420,     0,   454,     0,
     461,     0,   468,     0,     0,   474,     0,   477,     0,   340,
       0,     0,   158,     0,     0,     0,     0,   165,   172,   298,
       0,     0,   299,     0,   211,   169,     0,   494,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   511,   512,     0,    23,    26,     0,    82,
       0,     0,    44,    45,     0,    51,     0,     0,     0,   294,
       0,     0,     0,     0,   209,     0,    85,   304,     0,   238,
     290,   237,   309,     0,   251,   289,   296,   326,     0,   323,
     355,   221,   356,   445,     0,   451,   441,   421,   455,   457,
     462,   464,   469,   471,   334,   475,   336,   478,   345,     0,
     159,   188,     0,   185,   177,   180,   183,     0,     0,   488,
     487,   493,     0,     0,   492,     0,   496,   502,   508,   507,
       0,   493,     0,   509,    21,    24,     0,    85,   128,    47,
       0,     0,   198,   207,   208,   206,   205,     0,     0,     0,
       0,   240,     0,   303,   252,     0,   328,   354,   447,   458,
     465,   472,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,   499,   244,
       0,    46,   239,   210,   212,     0,   199,   214,     0,   241,
       0,   448,     0,     0,     0,     0,   178,   181,   184,   300,
       0,   513,     0,   497,   490,   489,   513,     0,     0,   247,
      48,     0,   200,   305,   327,   189,   191,   186,     0,   301,
       0,     0,   498,     0,   500,   245,     0,   201,   306,     0,
       0,     0,     0,     0,     0,     0,   248,   190,     0,   194,
     187,   514,   515,   516,   192,     0,     0,     0,   193,   195,
       0,   196
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -586,  -586,  -586,   139,   537,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,   497,  -586,  -586,   536,  -586,  -586,
    -586,  -247,  -586,  -586,  -586,  -586,  -586,  -586,   374,  -586,
     553,   -64,  -586,   541,  -586,  -285,  -377,  -503,  -586,  -586,
    -208,  -138,   -76,  -586,    18,  -280,   -13,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,   622,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -502,  -586,  -586,  -193,  -586,   -79,  -296,  -586,  -229,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
     431,  -586,  -586,   256,   -36,  -586,  -586,  -586,   353,  -586,
     -57,  -586,  -265,   244,  -271,  -264,   106,  -586,   281,  -586,
    -586,  -586,  -586,   459,   565,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -145,  -106,  -586,  -586,  -586,   282,  -586,
    -586,  -586,   -87,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,
    -586,   352,  -585,  -433,  -565,  -586,  -586,  -291,   -83,   191,
    -586,  -586,  -586,  -238,  -586
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   163,   169,   450,   329,   727,   330,   599,
      16,    17,    35,    36,    63,    18,    32,    33,   331,   332,
     734,   735,   333,   334,   335,   336,   337,   338,   170,   171,
      29,    30,    45,    46,    47,    19,   154,   229,    80,    20,
      81,    82,   339,   340,   230,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   565,   836,   566,
     837,   568,   838,   353,   833,   899,   900,   916,   915,   920,
     607,   857,   354,   744,   745,   610,   355,   356,   420,   358,
     359,   360,   361,   728,   878,   729,   896,   362,   659,   824,
     608,   820,   898,   225,   665,   522,   370,   825,   226,   227,
     543,   544,   220,   221,   709,   272,   528,   374,   235,   160,
      54,    22,    86,   108,    67,    68,    23,    24,   158,    84,
      52,    25,   159,   273,   364,   107,   213,   214,   218,   215,
     383,   767,   545,   381,   766,   378,   763,   379,   861,   380,
     765,   384,   768,   385,   829,   386,   770,   387,   830,   388,
     772,   389,   831,   390,   775,   391,   777,    26,    41,    27,
      40,   365,   582,   583,   584,   366,   585,   586,   587,   588,
     367,   449,   368,   891,   369
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   357,   410,   212,   403,   156,   419,   405,   537,   115,
     539,   442,   541,   591,   -80,   236,   662,   554,   713,   448,
     737,   612,   216,    59,   437,     2,   720,    42,   271,    87,
      88,     3,   706,   470,   679,   577,   681,   393,   683,   718,
      76,   577,   855,    66,   174,   492,   164,   165,   495,   496,
     495,   496,   691,   693,     4,    60,     5,   223,     6,   233,
       7,    48,   406,   174,     8,   399,   676,   219,   243,   469,
     708,     9,   664,   757,   463,   738,    72,    10,   738,    28,
      43,   406,   110,   175,   738,   520,   238,   239,   677,   471,
      44,   155,   472,   473,   407,    11,   211,   244,   234,   516,
     224,   517,   376,   517,   400,   779,   402,   464,   840,    31,
      77,    78,   371,   407,   846,   408,    12,   740,   409,   233,
      64,    43,   212,   716,    56,   741,    34,   794,   717,   736,
     551,    44,    37,    38,   240,   223,   413,   557,    39,   525,
     613,   276,   371,    79,   231,   792,    89,   526,    65,   742,
      90,   222,    91,    92,   743,   394,   572,   371,    61,   270,
     712,   241,   212,   166,   371,   472,   473,   167,    62,   168,
      92,   527,    53,   395,   611,    13,   484,   485,   212,   738,
     738,   401,   396,   519,   492,    93,   494,   495,   496,   497,
      55,   738,   498,    13,    74,    56,   738,   414,   274,    14,
     406,   275,   212,   212,   788,   236,   739,   756,   708,   406,
     711,    56,   817,   422,    58,   211,   800,   843,   780,    75,
     404,   444,   446,   859,    72,   434,   841,   513,   514,   515,
     472,   473,   407,   746,    43,   811,   847,   818,   516,    57,
     517,   407,    66,   819,    44,   685,   822,  -456,   670,   484,
     485,   594,  -456,   595,   687,   211,    69,   492,   601,   494,
     495,   496,   497,    70,  -463,   498,   212,   686,   834,  -463,
    -456,   211,   212,   212,   212,   212,   688,   791,   212,   690,
     212,   707,   212,  -470,   212,   524,   801,  -463,  -470,    72,
     835,   530,   531,   532,   533,   211,   211,   536,   576,   538,
     589,   540,   212,   723,   850,  -246,  -470,   731,   454,   684,
    -246,   516,    71,   517,   484,   485,   101,    73,   732,   733,
     102,   550,   492,    83,    85,   495,   496,   497,  -246,   212,
     498,   162,   596,   597,   472,   473,   553,   103,   104,   105,
     106,   371,    49,    50,   372,    51,    96,   373,   571,   564,
     325,   441,   807,   155,   717,    98,   721,    99,   100,   211,
     112,   109,   113,   114,   151,   211,   211,   211,   211,   152,
     153,   211,   157,   211,   155,   211,   516,   211,   517,   161,
     162,    60,   173,   217,   219,   228,   604,   232,   237,   760,
     245,   246,   249,   247,   248,   211,   250,   267,   269,   251,
     252,   759,   382,   556,   253,   254,   255,   268,   472,   473,
     233,   392,   223,   426,   256,   427,   482,   483,   484,   485,
     416,   430,   211,   403,   431,   435,   492,   424,   494,   495,
     496,   497,   429,   428,   498,   459,   500,   501,   257,   258,
     458,   518,   460,   461,   523,   602,   465,    13,   256,   466,
     467,   534,   547,   562,   549,   470,   569,   570,   614,   212,
     212,   575,   212,   663,   603,   664,   631,   511,   512,   513,
     514,   515,   257,   258,   609,   666,   635,   658,   700,   701,
     516,   703,   517,   238,   714,   711,   698,   715,   724,   726,
     482,   483,   484,   485,   486,   406,   259,   489,   490,   491,
     492,   357,   494,   495,   496,   497,   736,   747,   498,   752,
     500,   501,   748,   754,   399,   769,   472,   473,   771,   773,
     782,   781,   844,   793,   795,   802,   796,   689,   261,   798,
     259,   803,   262,   805,   505,   263,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   813,   814,   815,   816,
     264,   839,   211,   211,   516,   211,   517,   865,   842,   266,
     845,   260,   261,   155,   848,   854,   262,   212,   852,   263,
     872,   873,   903,   879,   877,   886,   892,   884,   212,   888,
     212,   905,   894,   904,   264,   909,   764,   912,   907,   598,
     265,   913,    94,   266,    97,   172,   918,   774,   482,   483,
     484,   485,   486,   921,   880,   489,   490,   491,   492,    95,
     494,   495,   496,   497,   375,   116,   498,   860,   500,   501,
     826,   730,   212,    21,   853,   787,   521,   758,   411,   776,
     669,   468,   242,   111,   799,   546,   415,   592,   893,     0,
       0,   806,   505,     0,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,     0,   809,     0,     0,     0,     0,
     211,   363,   516,     0,   517,   535,     0,     0,     0,     0,
     529,   211,     0,   211,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,   472,   473,   832,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,   212,   211,     0,   421,     0,     0,
       0,   423,     0,   425,     0,     0,     0,   212,     0,     0,
       0,   433,     0,   858,   851,   438,   439,   440,     0,     0,
       0,   447,     0,   856,     0,     0,   862,     0,     0,   451,
     452,   453,     0,   455,   456,   457,     0,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   486,     0,     0,
     489,   490,   491,   492,   212,   494,   495,   496,   497,     0,
       0,   498,     0,   500,   501,     0,     0,   211,     0,   212,
       0,     0,     0,   908,     0,     0,     0,     0,     0,     0,
     211,     0,     0,     0,     0,     0,     0,   505,   917,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,     0,
       0,   552,     0,     0,     0,     0,     0,   516,   558,   517,
       0,     0,   600,     0,     0,   563,     0,     0,     0,   567,
       0,     0,     0,     0,     0,   895,     0,   573,   472,   473,
       0,     0,     0,     0,     0,   581,     0,   581,     0,     0,
       0,     0,     0,   906,     0,     0,     0,   211,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   211,     0,     0,   615,   616,   617,   618,   619,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,   629,   630,     0,   633,   634,     0,   636,   637,   638,
     639,   640,   641,   642,   643,   645,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,   660,     0,
     482,   483,   484,   485,   486,   472,   473,   489,     0,   671,
     492,     0,   494,   495,   496,   497,     0,     0,   498,     0,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   472,   473,     0,     0,     0,   696,     0,     0,
     633,   645,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,   514,   515,     0,     0,     0,     0,
     451,   457,     0,     0,   516,     0,   517,     0,     0,     0,
       0,   457,     0,     0,   722,   447,     0,     0,   725,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   559,   494,
     495,   496,   497,     0,     0,   498,   499,   500,   501,   502,
     503,     0,     0,   749,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,   492,   751,   494,   495,   496,   497,
     560,   505,   498,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,     0,     0,     0,     0,     0,     0,     0,
     761,   516,     0,   517,     0,     0,     0,     0,     0,   561,
       0,     0,   472,   473,     0,   511,   512,   513,   514,   515,
       0,   778,     0,     0,     0,     0,     0,     0,   516,     0,
     517,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,   256,     0,     0,   797,     0,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,   810,     0,   257,   258,   812,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   823,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   559,   494,   495,   496,   497,
       0,     0,   498,   499,   500,   501,   502,   503,     0,     0,
       0,     0,     0,     0,   525,   259,     0,     0,     0,     0,
       0,     0,   526,     0,     0,     0,     0,   560,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,     0,     0,     0,     0,   667,   261,   516,     0,
     517,   262,     0,     0,   263,   864,   574,   866,   867,   868,
     870,   871,     0,     0,   874,   875,     0,   876,     0,   264,
       0,     0,     0,     0,     0,   668,   882,     0,   266,     0,
     277,     0,     0,     0,     0,     3,     0,   278,   279,   280,
       0,   281,     0,   282,   283,   284,   285,     0,     0,     0,
       0,     0,   897,     0,   286,   287,   288,   289,     0,   902,
       0,     0,     0,     0,     0,     0,   290,   291,     0,   292,
     293,     0,     0,   294,   911,     9,   295,   296,     0,   297,
     298,     0,     0,   299,   300,     0,     0,     0,     0,   301,
     180,   181,   182,     0,   184,   185,   186,   187,   188,   302,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     200,   201,   202,     0,     0,   205,   206,   207,   208,     0,
       0,   303,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,    43,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    44,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   318,   319,   320,     0,   321,     0,     0,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,   325,   326,   327,   155,   328,   277,     0,     0,     0,
       0,     3,     0,   278,   279,   280,     0,   281,     0,   282,
     283,   284,   285,     0,     0,     0,     0,     0,     0,     0,
     286,   287,   288,   289,     0,     0,     0,     0,     0,     0,
       0,     0,   290,   291,     0,   292,   293,     0,     0,   294,
       0,     9,   295,   296,     0,   297,   298,     0,     0,   299,
     300,     0,     0,     0,     0,   301,   180,   181,   182,     0,
     184,   185,   186,   187,   188,   302,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,   200,   201,   202,     0,
       0,   205,   206,   207,   208,     0,     0,   303,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,    43,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    44,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,   319,
     320,     0,   321,     0,     0,   322,   323,     0,     0,     0,
     282,   283,   284,   285,     0,   324,     0,   325,   326,   327,
     155,   808,   287,     0,     0,     0,     0,     0,   256,     0,
       0,     0,     0,     0,   291,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,   577,     0,   298,     0,     0,
       0,     0,   257,   258,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   578,   317,     0,     0,     0,     0,     0,
       0,   260,   261,     0,     0,     0,   262,     0,     0,   579,
     319,   320,     0,   321,     0,     0,   322,   323,   282,   283,
     284,   285,     0,     0,   264,     0,   580,     0,   325,   326,
     287,   155,     0,   266,     0,     0,   256,     0,     0,     0,
       0,     0,   291,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
     257,   258,     0,     0,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,     0,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   578,   317,     0,     0,     0,     0,     0,     0,   260,
     261,     0,     0,     0,   262,     0,     0,   579,   319,   320,
       0,   321,     0,     0,   322,   323,   282,   283,   284,   285,
       0,     0,   264,     0,   590,     0,   325,   326,   287,   155,
       0,   266,     0,   256,   393,     0,     0,     0,     0,     0,
     291,     0,     0,   293,     0,     0,   294,     0,     0,   295,
       0,     0,     0,   298,     0,     0,     0,   257,   258,     0,
       0,     0,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,     0,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,     0,     0,     0,     0,   260,   261,     0,     0,
       0,   262,   394,     0,   263,   318,   319,   320,     0,   321,
       0,     0,   322,   323,   282,   283,   284,   285,     0,   264,
     395,   555,   324,     0,   325,   326,   287,   155,   266,   396,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,   293,     0,     0,   294,     0,     0,   295,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   417,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,   319,   320,     0,   321,     0,     0,
     322,   323,     0,     0,   282,   283,   284,   285,     0,     0,
     324,     0,   325,   326,   418,   155,   287,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,   293,     0,     0,   294,     0,     0,   295,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
       0,     0,     0,     0,   282,   283,   284,   285,     0,     0,
       0,     0,     0,   318,   319,   320,   287,   321,     0,     0,
     322,   323,     0,     0,     0,     0,     0,     0,   291,     0,
     324,   293,   325,   326,   294,   155,     0,   295,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,     0,     0,   318,   319,   320,     0,   321,     0,     0,
     322,   323,   282,   283,   284,   285,   694,     0,   605,     0,
     324,   606,   325,   326,   287,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   291,     0,     0,   293,
       0,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   260,   261,
     303,     0,     0,   262,     0,   672,   263,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,   695,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,   397,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,   432,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   644,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,   646,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   258,     0,
       0,   318,   319,   320,     0,   321,     0,     0,   322,   323,
     282,   283,   284,   285,     0,     0,     0,     0,   324,   661,
     325,   326,   287,   155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,   259,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,   260,   261,   303,     0,
       0,   262,     0,     0,   263,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,   673,     0,     0,   266,     0,
       0,   256,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   257,   258,     0,     0,   318,
     319,   320,     0,   321,     0,     0,   322,   323,   282,   283,
     284,   285,     0,     0,     0,     0,   324,   692,   325,   326,
     287,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   291,     0,     0,   293,     0,     0,   294,     0,
       0,   295,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   180,   181,   182,     0,   184,
     185,   186,   187,   188,   302,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   200,   201,   202,     0,     0,
     205,   206,   207,   208,   260,   261,   303,     0,     0,   262,
       0,   674,   263,     0,     0,     0,     0,     0,   305,   306,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,   256,
       0,    43,   308,   309,   310,   311,   312,   313,   314,   315,
     316,    44,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,   258,     0,     0,   318,   319,   320,
       0,   321,     0,     0,   322,   323,   282,   283,   284,   285,
       0,     0,     0,     0,   324,   710,   325,   326,   287,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     291,     0,     0,   293,     0,     0,   294,     0,     0,   295,
       0,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   180,   181,   182,     0,   184,   185,   186,
     187,   188,   302,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,   200,   201,   202,     0,     0,   205,   206,
     207,   208,   260,   261,   303,     0,     0,   262,     0,   675,
     263,     0,     0,     0,     0,     0,   305,   306,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,   266,     0,     0,   256,     0,    43,
     308,   309,   310,   311,   312,   313,   314,   315,   316,    44,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   258,     0,     0,   318,   319,   320,     0,   321,
       0,     0,   322,   323,   282,   283,   284,   285,     0,     0,
       0,     0,   324,   719,   325,   326,   287,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,   293,     0,     0,   294,     0,     0,   295,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,   259,
       0,   180,   181,   182,     0,   184,   185,   186,   187,   188,
     302,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,   200,   201,   202,     0,     0,   205,   206,   207,   208,
     260,   261,   303,     0,     0,   262,     0,   678,   263,     0,
       0,     0,     0,     0,   305,   306,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,     0,   256,     0,    43,   308,   309,
     310,   311,   312,   313,   314,   315,   316,    44,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,     0,     0,   318,   319,   320,     0,   321,     0,     0,
     322,   323,   282,   283,   284,   285,   863,     0,     0,     0,
     324,   821,   325,   326,   287,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   291,     0,     0,   293,
       0,     0,   294,     0,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,   259,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,   260,   261,
     303,     0,     0,   262,     0,   680,   263,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,   282,   283,   284,   285,     0,     0,     0,     0,
       0,   318,   319,   320,   287,   321,     0,     0,   322,   323,
       0,     0,     0,     0,     0,     0,   291,     0,   324,   293,
     325,   326,   294,   155,     0,   295,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,     0,   184,   185,   186,   187,   188,   302,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,   200,
     201,   202,     0,     0,   205,   206,   207,   208,     0,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   308,   309,   310,   311,
     312,   313,   314,   315,   316,    44,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   318,   319,   320,     0,   321,     0,     0,   322,   323,
     282,   283,   284,   285,     0,     0,     0,     0,   324,   869,
     325,   326,   287,   155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,   293,     0,     0,
     294,     0,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,   881,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,     0,     0,
     282,   283,   284,   285,   901,     0,     0,     0,     0,   318,
     319,   320,   287,   321,     0,     0,   322,   323,     0,     0,
       0,     0,     0,     0,   291,     0,   324,   293,   325,   326,
     294,   155,     0,   295,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,     0,     0,
     282,   283,   284,   285,     0,     0,     0,     0,     0,   318,
     319,   320,   287,   321,     0,   256,   322,   323,     0,     0,
       0,     0,     0,     0,   291,     0,   324,   293,   325,   326,
     294,   155,     0,   295,     0,     0,     0,   298,     0,   257,
     258,     0,     0,     0,     0,     0,     0,   180,   181,   182,
       0,   184,   185,   186,   187,   188,   302,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,   200,   201,   202,
       0,     0,   205,   206,   207,   208,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,   473,
     305,   306,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,   308,   309,   310,   311,   312,   313,
     314,   315,   316,    44,   317,     0,     0,     0,   260,   261,
       0,     0,     0,   262,     0,   682,   263,     0,     0,   318,
     319,   320,     0,   321,     0,     0,   322,   323,     0,     0,
       0,   264,     0,     0,     0,     0,   324,     0,   325,   326,
     266,   155,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   559,   494,   495,   496,   497,   472,   473,   498,   499,
     500,   501,   502,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   560,   505,     0,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,   517,     0,     0,     0,
       0,     0,   697,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   559,
     494,   495,   496,   497,   472,   473,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   560,   505,     0,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,   516,     0,   517,     0,     0,     0,     0,     0,
     705,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   559,   494,   495,
     496,   497,   472,   473,   498,   499,   500,   501,   502,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   560,
     505,     0,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
     516,     0,   517,     0,     0,     0,     0,     0,   762,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   472,
     473,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   559,   494,   495,   496,   497,
       0,     0,   498,   499,   500,   501,   502,   503,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   560,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,   482,   483,   484,   485,   486,   827,     0,   489,   490,
     491,   492,     0,   494,   495,   496,   497,     0,     0,   498,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,     0,     0,     0,
       0,     0,   472,   473,     0,   516,     0,   517,     0,     0,
       0,     0,     0,   890,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   699,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   702,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   783,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   887,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
       0,     0,   472,   473,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   889,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     472,   473,     0,     0,     0,     0,     0,     0,   516,     0,
     517,     0,     0,   910,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     472,   473,     0,     0,     0,     0,     0,     0,   516,     0,
     517,   548,   482,   483,   484,   485,   486,     0,     0,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,     0,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,   517,   750,
     472,   473,   482,   483,   484,   485,   486,     0,     0,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   505,     0,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,   517,   755,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   559,   494,   495,   496,   497,  -221,     0,
     498,   499,   500,   501,   502,   503,   472,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   560,   505,     0,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,   517,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,   473,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,     0,     0,   498,   499,   500,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   472,   473,     0,     0,     0,     0,
       0,     0,   516,     0,   517,   482,   483,   484,   485,   486,
       0,     0,   489,   490,   491,   492,     0,   494,   495,   496,
     497,     0,     0,   498,     0,   500,   501,     0,     0,   593,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   472,   473,     0,     0,     0,     0,     0,     0,   516,
       0,   517,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,     0,   489,   490,   491,   492,     0,   494,   495,
     496,   497,     0,     0,   498,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     505,   753,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   472,   473,     0,     0,     0,     0,     0,     0,
     516,     0,   517,   482,   483,   484,   485,   486,     0,     0,
     489,   490,   491,   492,     0,   494,   495,   496,   497,     0,
       0,   498,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   804,     0,     0,     0,   505,     0,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   472,
     473,     0,     0,     0,     0,     0,     0,   516,     0,   517,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
       0,   489,   490,   491,   492,     0,   494,   495,   496,   497,
       0,     0,   498,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   505,     0,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     472,   473,     0,     0,     0,     0,     0,     0,   516,     0,
     517,   482,   483,   484,   485,   486,     0,     0,   489,   490,
     491,   492,     0,   494,   495,   496,   497,     0,     0,   498,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   472,   473,     0,
       0,     0,     0,     0,     0,   516,     0,   517,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,     0,   489,
     490,   491,   492,     0,   494,   495,   496,   497,     0,     0,
     498,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,   473,
     508,   509,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,   516,     0,   517,   482,
     483,   484,   485,   486,     0,     0,   489,   490,   491,   492,
       0,   494,   495,   496,   497,   256,     0,   498,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,     0,     0,     0,   257,
     258,     0,     0,     0,     0,     0,     0,     0,   509,   510,
     511,   512,   513,   514,   515,     0,     0,     0,   257,   258,
     482,   483,   256,   516,   486,   517,     0,   489,   490,   491,
     492,     0,   494,   495,   496,   497,   256,     0,   498,     0,
     500,   501,     0,     0,     0,     0,   257,   258,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   259,     0,     0,
     257,   258,     0,     0,   505,     0,   506,   507,   508,   509,
     510,     0,     0,     0,   514,     0,   259,     0,     0,   257,
     258,   256,     0,     0,   516,     0,   517,     0,   260,   261,
       0,     0,   256,   262,     0,   704,   263,     0,     0,     0,
       0,     0,   256,     0,   259,   257,   258,   260,   261,     0,
       0,   264,   262,     0,   784,   263,   257,   258,   259,     0,
     266,   256,     0,     0,     0,     0,   257,   258,     0,     0,
     264,     0,     0,     0,     0,   260,   261,   259,     0,   266,
     262,     0,   785,   263,     0,   257,   258,     0,     0,   260,
     261,     0,     0,     0,   262,     0,   786,   263,   264,     0,
       0,   256,     0,   259,     0,     0,     0,   266,   260,   261,
       0,     0,   264,   262,   259,   828,   263,     0,     0,     0,
       0,   266,     0,     0,   259,   257,   258,     0,     0,     0,
       0,   264,     0,     0,   260,   261,     0,     0,     0,   262,
     266,   849,   263,   259,     0,   260,   261,   256,     0,     0,
     262,     0,   883,   263,     0,   260,   261,   264,     0,     0,
     262,     0,   885,   263,     0,     0,   266,     0,   264,     0,
       0,   257,   258,     0,   260,   261,     0,   266,   264,   262,
       0,   914,   263,   259,     0,     0,     0,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,     0,     0,   260,   261,     0,     0,     0,   262,
       0,   919,   263,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,   443,     0,   266,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
     260,   261,   177,     0,     0,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,   179,   266,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,   179,     0,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,   179,    44,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,     0,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,    43,   308,   309,   310,
       0,     0,     0,     0,     0,   179,    44,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,   179,    44,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   136,   137,   138,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
       0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      13,   230,   273,   109,   269,    81,   286,   271,   385,    73,
     387,   307,   389,   446,     7,   160,   518,    19,   583,   310,
     605,    18,   109,    21,   304,     0,   591,    33,    46,    14,
      15,     6,    19,   118,   537,    49,   539,    32,   541,    19,
      34,    49,   126,   137,   137,   112,    14,    15,   115,   116,
     115,   116,   554,   555,    29,    53,    31,   128,    33,   137,
      35,    74,   113,   137,    39,   137,   152,   137,   145,   354,
     121,    46,    42,    43,   137,   144,   127,    52,   144,   137,
     127,   113,   176,   176,   144,   370,   139,   140,   174,   174,
     137,   175,    20,    21,   145,    70,   109,   174,   176,   166,
     171,   168,   176,   168,   176,   174,   176,   170,   174,   131,
     104,   105,   144,   145,   174,   147,    91,   113,   150,   137,
     147,   127,   228,   169,   144,   121,   137,   712,   174,   137,
     410,   137,   137,   175,   144,   128,   281,   417,   137,   113,
     137,   228,   144,   137,   157,   710,   131,   121,   175,   145,
     135,   171,   137,   138,   150,   150,   436,   144,   156,   223,
     174,   171,   268,   131,   144,    20,    21,   135,   166,   137,
     138,   145,    58,   168,   470,   168,   104,   105,   284,   144,
     144,   268,   177,   176,   112,   170,   114,   115,   116,   117,
     145,   144,   120,   168,   144,   144,   144,   284,   171,   174,
     113,   174,   308,   309,   706,   350,   171,   171,   121,   113,
     144,   144,   144,   289,   144,   228,   718,   121,   171,   169,
     169,   308,   309,   171,   127,   301,   791,   155,   156,   157,
      20,    21,   145,   610,   127,   169,   801,   169,   166,   147,
     168,   145,   137,   746,   137,   152,   748,   147,   528,   104,
     105,   174,   152,   176,   152,   268,   137,   112,   151,   114,
     115,   116,   117,   137,   147,   120,   372,   174,   152,   152,
     170,   284,   378,   379,   380,   381,   174,   710,   384,   550,
     386,   577,   388,   147,   390,   372,   719,   170,   152,   127,
     174,   378,   379,   380,   381,   308,   309,   384,   443,   386,
     445,   388,   408,   594,   807,   147,   170,    11,   321,   147,
     152,   166,   137,   168,   104,   105,    65,   170,    22,    23,
      69,   408,   112,   137,   175,   115,   116,   117,   170,   435,
     120,   139,   140,   141,    20,    21,   412,    86,    87,    88,
      89,   144,     4,     5,   147,     7,   131,   150,   435,   425,
     172,   173,   729,   175,   174,   137,   176,   137,   137,   372,
     175,   145,   175,   137,    34,   378,   379,   380,   381,    34,
     170,   384,   147,   386,   175,   388,   166,   390,   168,     1,
     139,    53,   175,   137,   137,   147,   462,   175,   174,   669,
     151,   151,   170,   151,   151,   408,   151,   174,   174,   151,
     151,   665,   137,   416,   151,   151,   151,   147,    20,    21,
     137,    32,   128,   151,    32,   151,   102,   103,   104,   105,
     170,   151,   435,   688,   131,   151,   112,   170,   114,   115,
     116,   117,   174,   170,   120,    36,   122,   123,    56,    57,
     173,   170,   174,   174,   137,   458,   174,   168,    32,   174,
     174,   171,   169,    13,   169,   118,   147,   131,   137,   565,
     566,   173,   568,     7,   175,    42,   137,   153,   154,   155,
     156,   157,    56,    57,   128,   174,   137,   137,   565,   566,
     166,   568,   168,   139,   169,   144,   562,   144,   176,   139,
     102,   103,   104,   105,   106,   113,   114,   109,   110,   111,
     112,   730,   114,   115,   116,   117,   137,   151,   120,   137,
     122,   123,   170,     1,   137,   152,    20,    21,   152,   152,
     137,   151,   793,   137,   169,   176,   169,   145,   146,   169,
     114,   169,   150,   139,   146,   153,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   137,   137,   137,   137,
     168,   170,   565,   566,   166,   568,   168,   137,   169,   177,
     169,   145,   146,   175,   169,   169,   150,   673,   171,   153,
     169,   169,    49,   152,   176,   151,   169,   174,   684,   152,
     686,   176,   169,   169,   168,   151,   673,   169,   171,   450,
     174,   169,    55,   177,    58,    98,   171,   684,   102,   103,
     104,   105,   106,   171,   851,   109,   110,   111,   112,    56,
     114,   115,   116,   117,   240,    74,   120,   825,   122,   123,
     758,   603,   728,     1,   817,   704,   370,   663,   275,   686,
     524,   350,   173,    68,   717,   391,   284,   446,   876,    -1,
      -1,   728,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,   731,    -1,    -1,    -1,    -1,
     673,   230,   166,    -1,   168,   383,    -1,    -1,    -1,    -1,
     174,   684,    -1,   686,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   264,    -1,    -1,    -1,    -1,
      -1,    20,    21,   779,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,   820,   728,    -1,   286,    -1,    -1,
      -1,   290,    -1,   292,    -1,    -1,    -1,   833,    -1,    -1,
      -1,   300,    -1,   820,   810,   304,   305,   306,    -1,    -1,
      -1,   310,    -1,   819,    -1,    -1,   833,    -1,    -1,   318,
     319,   320,    -1,   322,   323,   324,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   333,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,   900,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,   820,    -1,   915,
      -1,    -1,    -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,
     833,    -1,    -1,    -1,    -1,    -1,    -1,   146,   915,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   410,    -1,    -1,    -1,    -1,    -1,   166,   417,   168,
      -1,    -1,   171,    -1,    -1,   424,    -1,    -1,    -1,   428,
      -1,    -1,    -1,    -1,    -1,   878,    -1,   436,    20,    21,
      -1,    -1,    -1,    -1,    -1,   444,    -1,   446,    -1,    -1,
      -1,    -1,    -1,   896,    -1,    -1,    -1,   900,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   915,    -1,    -1,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,   486,   487,   488,
     489,   490,   491,    -1,   493,   494,    -1,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,    -1,   517,    -1,
     102,   103,   104,   105,   106,    20,    21,   109,    -1,   528,
     112,    -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,   556,    -1,    -1,
     559,   560,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
     579,   580,    -1,    -1,   166,    -1,   168,    -1,    -1,    -1,
      -1,   590,    -1,    -1,   593,   594,    -1,    -1,   597,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,   124,
     125,    -1,    -1,   632,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   644,   114,   115,   116,   117,
     145,   146,   120,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     669,   166,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    20,    21,    -1,   153,   154,   155,   156,   157,
      -1,   690,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   708,
     709,    -1,    -1,    32,    -1,    -1,   715,    -1,   717,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   730,    -1,    -1,    -1,   734,    -1,    56,    57,   738,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   753,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   166,    -1,
     168,   150,    -1,    -1,   153,   834,   174,   836,   837,   838,
     839,   840,    -1,    -1,   843,   844,    -1,   846,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   174,   855,    -1,   177,    -1,
       1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,   881,    -1,    25,    26,    27,    28,    -1,   888,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    -1,    -1,    44,   903,    46,    47,    48,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   154,   155,    -1,   157,    -1,    -1,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    41,    -1,    -1,    44,
      -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    92,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,
     155,    -1,   157,    -1,    -1,   160,   161,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,   170,    -1,   172,   173,   174,
     175,   176,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    49,    -1,    51,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,    14,    15,
      16,    17,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
      26,   175,    -1,   177,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,
      -1,   157,    -1,    -1,   160,   161,    14,    15,    16,    17,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,    26,   175,
      -1,   177,    -1,    32,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,    -1,
      -1,   150,   150,    -1,   153,   153,   154,   155,    -1,   157,
      -1,    -1,   160,   161,    14,    15,    16,    17,    -1,   168,
     168,   170,   170,    -1,   172,   173,    26,   175,   177,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
     170,    -1,   172,   173,   174,   175,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,   153,   154,   155,    26,   157,    -1,    -1,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     170,    41,   172,   173,    44,   175,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,    14,    15,    16,    17,    18,    -1,   168,    -1,
     170,   171,   172,   173,    26,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   145,   146,
      92,    -1,    -1,   150,    -1,   152,   153,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,   151,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,   169,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,   153,   154,   155,    -1,   157,    -1,    -1,   160,   161,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,   170,   171,
     172,   173,    26,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,   145,   146,    92,    -1,
      -1,   150,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,   177,    -1,
      -1,    32,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   170,   171,   172,   173,
      26,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,   145,   146,    92,    -1,    -1,   150,
      -1,   152,   153,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    32,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,   153,   154,   155,
      -1,   157,    -1,    -1,   160,   161,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   170,   171,   172,   173,    26,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   145,   146,    92,    -1,    -1,   150,    -1,   152,
     153,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    32,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    -1,    -1,   153,   154,   155,    -1,   157,
      -1,    -1,   160,   161,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,   170,   171,   172,   173,    26,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     145,   146,    92,    -1,    -1,   150,    -1,   152,   153,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    32,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     170,   171,   172,   173,    26,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   145,   146,
      92,    -1,    -1,   150,    -1,   152,   153,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    26,   157,    -1,    -1,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   170,    41,
     172,   173,    44,   175,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   154,   155,    -1,   157,    -1,    -1,   160,   161,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,   170,   171,
     172,   173,    26,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   153,
     154,   155,    26,   157,    -1,    -1,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   170,    41,   172,   173,
      44,   175,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,   153,
     154,   155,    26,   157,    -1,    32,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   170,    41,   172,   173,
      44,   175,    -1,    47,    -1,    -1,    -1,    51,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     104,   105,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,    -1,   145,   146,
      -1,    -1,    -1,   150,    -1,   152,   153,    -1,    -1,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,
     177,   175,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,   102,   103,   104,   105,   106,   174,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   166,    -1,   168,    -1,    -1,
      -1,    -1,    -1,   174,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,    -1,    -1,   171,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,   169,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,
      20,    21,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,    -1,    -1,
      -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    32,    -1,   120,    -1,   122,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    56,    57,
     102,   103,    32,   166,   106,   168,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    32,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   114,    -1,    -1,
      56,    57,    -1,    -1,   146,    -1,   148,   149,   150,   151,
     152,    -1,    -1,    -1,   156,    -1,   114,    -1,    -1,    56,
      57,    32,    -1,    -1,   166,    -1,   168,    -1,   145,   146,
      -1,    -1,    32,   150,    -1,   152,   153,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   114,    56,    57,   145,   146,    -1,
      -1,   168,   150,    -1,   152,   153,    56,    57,   114,    -1,
     177,    32,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
     168,    -1,    -1,    -1,    -1,   145,   146,   114,    -1,   177,
     150,    -1,   152,   153,    -1,    56,    57,    -1,    -1,   145,
     146,    -1,    -1,    -1,   150,    -1,   152,   153,   168,    -1,
      -1,    32,    -1,   114,    -1,    -1,    -1,   177,   145,   146,
      -1,    -1,   168,   150,   114,   152,   153,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   114,    56,    57,    -1,    -1,    -1,
      -1,   168,    -1,    -1,   145,   146,    -1,    -1,    -1,   150,
     177,   152,   153,   114,    -1,   145,   146,    32,    -1,    -1,
     150,    -1,   152,   153,    -1,   145,   146,   168,    -1,    -1,
     150,    -1,   152,   153,    -1,    -1,   177,    -1,   168,    -1,
      -1,    56,    57,    -1,   145,   146,    -1,   177,   168,   150,
      -1,   152,   153,   114,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,    -1,    -1,    -1,   150,
      -1,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
     145,   146,    30,    -1,    -1,   150,    -1,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,   177,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,   137,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    59,   137,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,   137,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   179,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    70,    91,   168,   174,   180,   188,   189,   193,   213,
     217,   232,   289,   294,   295,   299,   335,   337,   137,   208,
     209,   131,   194,   195,   137,   190,   191,   137,   175,   137,
     338,   336,    33,   127,   137,   210,   211,   212,   224,     4,
       5,     7,   298,    58,   288,   145,   144,   147,   144,    21,
      53,   156,   166,   192,   147,   175,   137,   292,   293,   137,
     137,   137,   127,   170,   144,   169,    34,   104,   105,   137,
     216,   218,   219,   137,   297,   175,   290,    14,    15,   131,
     135,   137,   138,   170,   182,   208,   131,   195,   137,   137,
     137,    65,    69,    86,    87,    88,    89,   303,   291,   145,
     176,   292,   175,   175,   137,   209,   211,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   122,   123,   124,   125,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   160,
     161,    34,    34,   170,   214,   175,   220,   147,   296,   300,
     287,     1,   139,   181,    14,    15,   131,   135,   137,   182,
     206,   207,   192,   175,   137,   176,    24,    30,    45,    59,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   224,   302,   304,   305,   307,   310,   137,   306,   137,
     280,   281,   171,   128,   171,   271,   276,   277,   147,   215,
     222,   224,   175,   137,   176,   286,   301,   174,   139,   140,
     144,   171,   291,   145,   174,   151,   151,   151,   151,   170,
     151,   151,   151,   151,   151,   151,    32,    56,    57,   114,
     145,   146,   150,   153,   168,   174,   177,   174,   147,   174,
     209,    46,   283,   301,   171,   174,   310,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    25,    26,    27,    28,
      37,    38,    40,    41,    44,    47,    48,    50,    51,    54,
      55,    60,    70,    92,    93,   104,   105,   119,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   138,   153,   154,
     155,   157,   160,   161,   170,   172,   173,   174,   176,   184,
     186,   196,   197,   200,   201,   202,   203,   204,   205,   220,
     221,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   241,   250,   254,   255,   256,   257,   258,
     259,   260,   265,   268,   302,   339,   343,   348,   350,   352,
     274,   144,   147,   150,   285,   206,   176,   268,   313,   315,
     317,   311,   137,   308,   319,   321,   323,   325,   327,   329,
     331,   333,    32,    32,   150,   168,   177,   169,   268,   137,
     176,   310,   176,   280,   169,   283,   113,   145,   147,   150,
     282,   276,   268,   301,   310,   339,   170,   113,   174,   223,
     256,   268,   220,   268,   170,   268,   151,   151,   170,   174,
     151,   131,    54,   268,   220,   151,   113,   223,   268,   268,
     268,   173,   254,    12,   310,    12,   310,   268,   345,   349,
     183,   268,   268,   268,   224,   268,   268,   268,   173,    36,
     174,   174,   268,   137,   170,   174,   174,   174,   286,   213,
     118,   174,    20,    21,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   120,   121,
     122,   123,   124,   125,   145,   146,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   166,   168,   170,   176,
     213,   271,   273,   137,   310,   113,   121,   145,   284,   174,
     310,   310,   310,   310,   171,   306,   310,   214,   310,   214,
     310,   214,   137,   278,   279,   310,   281,   169,   169,   169,
     310,   223,   268,   220,    19,   170,   224,   223,   268,   113,
     145,   174,    13,   268,   220,   235,   237,   268,   239,   147,
     131,   310,   223,   268,   174,   173,   301,    49,   137,   153,
     170,   268,   340,   341,   342,   344,   345,   346,   347,   301,
     170,   341,   347,   126,   174,   176,   140,   141,   181,   187,
     171,   151,   224,   175,   220,   168,   171,   248,   268,   128,
     253,   254,    18,   137,   137,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   137,   119,   268,   268,   137,   268,   268,   268,   268,
     268,   268,   268,   268,   119,   268,    21,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   137,   266,
     268,   171,   248,     7,    42,   272,   174,   145,   174,   284,
     223,   268,   152,   174,   152,   152,   152,   174,   152,   215,
     152,   215,   152,   215,   147,   152,   174,   152,   174,   145,
     282,   248,   171,   248,    18,   151,   268,   174,   220,   171,
     310,   310,   171,   310,   152,   174,    19,   254,   121,   282,
     171,   144,   174,   342,   169,   144,   169,   174,    19,   171,
     342,   176,   268,   345,   176,   268,   139,   185,   261,   263,
     222,    11,    22,    23,   198,   199,   137,   340,   144,   171,
     113,   121,   145,   150,   251,   252,   214,   151,   170,   268,
     169,   268,   137,   147,     1,   169,   171,    43,   272,   283,
     223,   268,   174,   314,   310,   318,   312,   309,   320,   152,
     324,   152,   328,   152,   310,   332,   278,   334,   268,   174,
     171,   151,   137,   171,   152,   152,   152,   253,   248,   268,
     268,   341,   342,   137,   340,   169,   169,   268,   169,   346,
     248,   341,   176,   169,   142,   139,   310,   214,   176,   220,
     268,   169,   268,   137,   137,   137,   137,   144,   169,   215,
     269,   171,   248,   268,   267,   275,   219,   174,   152,   322,
     326,   330,   220,   242,   152,   174,   236,   238,   240,   170,
     174,   342,   169,   121,   282,   169,   174,   342,   169,   152,
     215,   220,   171,   251,   169,   126,   220,   249,   310,   171,
     218,   316,   310,    18,   268,   137,   268,   268,   268,   171,
     268,   268,   169,   169,   268,   268,   268,   176,   262,   152,
     199,   113,   268,   152,   174,   152,   151,   171,   152,   171,
     174,   351,   169,   351,   169,   224,   264,   268,   270,   243,
     244,    18,   268,    49,   169,   176,   224,   171,   310,   151,
     171,   268,   169,   169,   152,   246,   245,   310,   171,   152,
     247,   171
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   178,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   180,   181,   181,   182,   183,
     183,   183,   184,   185,   185,   187,   186,   188,   189,   190,
     190,   190,   191,   191,   192,   192,   193,   194,   194,   195,
     195,   196,   197,   197,   198,   198,   199,   199,   199,   200,
     200,   201,   202,   203,   204,   205,   206,   206,   206,   206,
     206,   206,   207,   207,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   213,   214,   214,   214,   215,   215,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   217,   218,   219,   220,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     222,   223,   223,   224,   224,   224,   225,   226,   226,   226,
     226,   227,   228,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   231,   232,   232,   233,   235,   236,   234,   237,
     238,   234,   239,   240,   234,   241,   241,   241,   242,   243,
     241,   244,   245,   241,   246,   247,   241,   248,   248,   249,
     249,   249,   250,   250,   250,   251,   251,   251,   251,   252,
     252,   253,   253,   254,   254,   255,   255,   255,   255,   255,
     255,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   257,   257,   258,
     259,   259,   260,   261,   262,   260,   263,   264,   260,   265,
     266,   267,   265,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   269,   270,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   271,
     271,   272,   272,   273,   274,   274,   275,   274,   274,   276,
     276,   277,   277,   278,   278,   279,   279,   280,   281,   281,
     282,   282,   283,   283,   283,   283,   283,   283,   284,   284,
     284,   285,   285,   286,   286,   286,   286,   286,   287,   287,
     288,   288,   289,   290,   289,   291,   291,   291,   292,   293,
     293,   294,   295,   295,   296,   296,   297,   298,   298,   300,
     299,   301,   301,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   303,   303,
     303,   303,   303,   303,   304,   305,   305,   306,   306,   308,
     309,   307,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     312,   310,   310,   313,   314,   310,   315,   316,   310,   317,
     318,   310,   310,   319,   320,   310,   321,   322,   310,   310,
     323,   324,   310,   325,   326,   310,   310,   327,   328,   310,
     329,   330,   310,   331,   332,   310,   333,   334,   310,   336,
     335,   338,   337,   339,   339,   339,   339,   340,   340,   340,
     340,   341,   341,   342,   342,   343,   343,   343,   343,   343,
     343,   344,   344,   345,   346,   346,   347,   347,   348,   348,
     349,   349,   350,   351,   351,   352,   352
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     4,     6,     2,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     1,     1,     1,     4,     1,     3,
       0,     3,     0,     2,     3,     0,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     3,     7,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     2,     1,     3,     2,     2,     2,     4,     5,
       2,     1,     1,     2,     3,     4,     2,     3,     3,     4,
       2,     3,     4,     1,     1,     2,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     5,     8,    10,     0,     0,
      10,     0,     0,    13,     0,     0,    15,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     0,     4,     1,     6,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     6,
       5,     6,     3,     0,     0,     8,     0,     0,     9,     3,
       0,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       4,     3,     1,     3,     4,     3,     4,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       4,     0,     1,     3,     0,     3,     0,     7,     5,     2,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     4,     6,     5,     5,     4,     0,     2,
       0,     1,     5,     0,     5,     0,     3,     5,     4,     1,
       2,     4,     5,     7,     0,     2,     2,     1,     1,     0,
       7,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     0,
       0,     6,     1,     1,     1,     1,     4,     3,     4,     2,
       2,     3,     2,     3,     2,     2,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     8,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     0,
       7,     0,     7,     1,     1,     1,     1,     3,     3,     5,
       5,     1,     3,     0,     2,     6,     5,     7,     8,     6,
       8,     1,     3,     3,     1,     1,     1,     3,     5,     5,
       1,     3,     4,     0,     3,    10,    10
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
          case 137: /* "name"  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3340 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 181: /* character_sequence  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3346 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 182: /* string_constant  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3352 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 183: /* string_builder_body  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3358 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 184: /* string_builder  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3364 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 186: /* expr_reader  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3370 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 190: /* require_module_name  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3376 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 196: /* expression_label  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3382 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 197: /* expression_goto  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3388 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 199: /* expression_else  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3394 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 201: /* expression_if_then_else  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3400 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 202: /* expression_for_loop  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3406 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 203: /* expression_unsafe  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3412 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 204: /* expression_while_loop  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3418 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 205: /* expression_with  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3424 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 206: /* annotation_argument_value  */
#line 130 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3430 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 207: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3436 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 208: /* annotation_argument  */
#line 130 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aa); }
#line 3442 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 209: /* annotation_argument_list  */
#line 131 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3448 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 210: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3454 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 211: /* annotation_declaration  */
#line 132 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).fa); }
#line 3460 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 212: /* annotation_list  */
#line 133 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3466 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 213: /* optional_annotation_list  */
#line 133 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).faList); }
#line 3472 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 214: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3478 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 215: /* optional_function_type  */
#line 128 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3484 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 216: /* function_name  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3490 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 220: /* expression_block  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3496 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 221: /* expression_any  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3502 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 222: /* expressions  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3508 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 223: /* expr_pipe  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3514 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 224: /* name_in_namespace  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3520 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 225: /* expression_delete  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3526 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 226: /* expr_new  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3532 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 227: /* expression_break  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3538 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 228: /* expression_continue  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3544 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 229: /* expression_return  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3550 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 230: /* expression_yield  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3556 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 231: /* expression_try_catch  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3562 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 233: /* expression_let  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3568 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 234: /* expr_cast  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3574 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 241: /* expr_type_info  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3580 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 248: /* expr_list  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3586 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 249: /* block_or_simple_block  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3592 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 251: /* capture_entry  */
#line 136 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pCapt); }
#line 3598 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 252: /* capture_list  */
#line 137 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pCaptList); }
#line 3604 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 253: /* optional_capture_list  */
#line 137 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pCaptList); }
#line 3610 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 254: /* expr_block  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3616 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 255: /* expr_numeric_const  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3622 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 256: /* expr_assign  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3628 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 257: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3634 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 258: /* expr_named_call  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3640 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 259: /* expr_method_call  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 260: /* func_addr_expr  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3652 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 265: /* expr_field  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 268: /* expr  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 271: /* optional_field_annotation  */
#line 131 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).aaList); }
#line 3670 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 273: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3676 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 274: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3682 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 276: /* function_argument_declaration  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3688 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 277: /* function_argument_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3694 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 278: /* tuple_type  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3700 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 279: /* tuple_type_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3706 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 280: /* variant_type  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3712 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 281: /* variant_type_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3718 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 283: /* variable_declaration  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3724 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 286: /* let_variable_declaration  */
#line 126 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pVarDecl); }
#line 3730 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 287: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp" /* yacc.c:1254  */
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3736 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 291: /* enum_list  */
#line 135 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pEnum); }
#line 3742 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 296: /* optional_structure_parent  */
#line 123 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).s); }
#line 3748 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 301: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3754 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 304: /* structure_type_declaration  */
#line 128 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3760 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 305: /* auto_type_declaration  */
#line 128 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3766 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 306: /* bitfield_bits  */
#line 124 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pNameList); }
#line 3772 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 307: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3778 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 310: /* type_declaration  */
#line 128 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pTypeDecl); }
#line 3784 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 339: /* make_decl  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3790 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 340: /* make_struct_fields  */
#line 134 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pMakeStruct); }
#line 3796 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 341: /* make_struct_dim  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3802 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 342: /* optional_block  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3808 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 343: /* make_struct_decl  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3814 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 344: /* make_tuple  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3820 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 345: /* make_map_tuple  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3826 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 346: /* make_any_tuple  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3832 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 347: /* make_dim  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3838 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 348: /* make_dim_decl  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3844 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 349: /* make_table  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3850 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 350: /* make_table_decl  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3856 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 351: /* array_comprehension_where  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3862 "ds_parser.cpp" /* yacc.c:1254  */
        break;

    case 352: /* array_comprehension  */
#line 129 "ds_parser.ypp" /* yacc.c:1254  */
      { delete ((*yyvaluep).pExpression); }
#line 3868 "ds_parser.cpp" /* yacc.c:1254  */
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
        case 15:
#line 447 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4164 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 16:
#line 459 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4170 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 17:
#line 460 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4176 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 18:
#line 464 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 4182 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 19:
#line 468 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4192 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 20:
#line 473 "ds_parser.ypp" /* yacc.c:1660  */
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4206 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 21:
#line 482 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 22:
#line 490 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 23:
#line 506 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4244 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 24:
#line 511 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4254 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 25:
#line 519 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto macros = g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das_yyerror("reader macro " + *(yyvsp[0].s) + " not found",tokAt((yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror("too many options for macro " + *(yyvsp[0].s) +  "\n" + options, tokAt((yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das_yyerror("expecting ~ after the reader macro", tokAt((yylsp[0])),
                CompilationError::syntax_error);
        } else {
            g_ReaderMacro = macros.back().get();
            g_ReaderExpr = new ExprReader(tokAt((yylsp[-1])),g_ReaderMacro);
            yyclearin ;
            das_yybegin_reader();
        }
    }
#line 4281 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 26:
#line 540 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4293 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 27:
#line 549 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4317 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 29:
#line 575 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4325 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 30:
#line 578 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4336 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 31:
#line 584 "ds_parser.ypp" /* yacc.c:1660  */
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4347 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 32:
#line 593 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4373 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 33:
#line 614 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 34:
#line 637 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4404 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 35:
#line 638 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4410 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 39:
#line 651 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 40:
#line 654 "ds_parser.ypp" /* yacc.c:1660  */
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4426 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 41:
#line 660 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4434 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 42:
#line 666 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 43:
#line 669 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 44:
#line 675 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4456 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 45:
#line 676 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4462 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 46:
#line 680 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 4468 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 47:
#line 681 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4474 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 48:
#line 682 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 49:
#line 691 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 4491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 50:
#line 692 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 4497 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 51:
#line 696 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 52:
#line 705 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = new ExprFor(tokAt((yylsp[-5])));
        pFor->visibility = tokRangeAt((yylsp[-5]),(yylsp[0]));
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
#line 4526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 53:
#line 721 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4536 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 54:
#line 729 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4548 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 55:
#line 739 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4559 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 56:
#line 748 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4565 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 57:
#line 749 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4571 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 58:
#line 750 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 59:
#line 751 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 60:
#line 752 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4589 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 61:
#line 753 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4595 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 62:
#line 757 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4605 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 63:
#line 762 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4615 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 64:
#line 770 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4621 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 65:
#line 771 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4627 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 66:
#line 772 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4633 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 67:
#line 773 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4639 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 68:
#line 774 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4645 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 69:
#line 775 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4651 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 70:
#line 776 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4657 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 71:
#line 777 "ds_parser.ypp" /* yacc.c:1660  */
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4665 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 72:
#line 783 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 73:
#line 796 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 4701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 74:
#line 812 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4707 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 75:
#line 813 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("require"); }
#line 4713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 76:
#line 817 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4725 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 77:
#line 824 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4739 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 78:
#line 836 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4748 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 79:
#line 840 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4757 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 80:
#line 847 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = nullptr; }
#line 4763 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 81:
#line 848 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4769 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 82:
#line 852 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4775 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 83:
#line 853 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = nullptr; }
#line 4781 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 84:
#line 854 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4787 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 85:
#line 858 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4797 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 86:
#line 863 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4806 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 87:
#line 870 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4815 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 88:
#line 874 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!"); }
#line 4821 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 89:
#line 875 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("~"); }
#line 4827 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 90:
#line 876 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+="); }
#line 4833 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 91:
#line 877 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-="); }
#line 4839 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 92:
#line 878 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*="); }
#line 4845 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 93:
#line 879 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/="); }
#line 4851 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 94:
#line 880 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%="); }
#line 4857 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 95:
#line 881 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&="); }
#line 4863 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 96:
#line 882 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|="); }
#line 4869 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 97:
#line 883 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^="); }
#line 4875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 98:
#line 884 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+"); }
#line 4881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 99:
#line 885 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("-"); }
#line 4887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 100:
#line 886 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("*"); }
#line 4893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 101:
#line 887 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("/"); }
#line 4899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 102:
#line 888 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("%"); }
#line 4905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 103:
#line 889 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<"); }
#line 4911 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 104:
#line 890 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">"); }
#line 4917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 105:
#line 891 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("=="); }
#line 4923 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 106:
#line 892 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("!="); }
#line 4929 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 107:
#line 893 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<="); }
#line 4935 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 108:
#line 894 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">="); }
#line 4941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 109:
#line 895 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("&"); }
#line 4947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 110:
#line 896 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("|"); }
#line 4953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 111:
#line 897 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("^"); }
#line 4959 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 112:
#line 898 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("++"); }
#line 4965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 113:
#line 899 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("--"); }
#line 4971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 114:
#line 900 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("+++"); }
#line 4977 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 115:
#line 901 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("---"); }
#line 4983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 116:
#line 902 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<"); }
#line 4989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 117:
#line 903 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>"); }
#line 4995 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 118:
#line 904 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<="); }
#line 5001 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 119:
#line 905 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>="); }
#line 5007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 120:
#line 906 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<"); }
#line 5013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 121:
#line 907 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>"); }
#line 5019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 122:
#line 908 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string("<<<="); }
#line 5025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 123:
#line 909 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = new string(">>>="); }
#line 5031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 124:
#line 913 "ds_parser.ypp" /* yacc.c:1660  */
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
        (yyvsp[0].pFuncDecl)->delRef();
    }
#line 5054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 125:
#line 934 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt((yylsp[-2]));
        pFunction->name = *(yyvsp[-2].s);
        pFunction->result = TypeDeclPtr((yyvsp[0].pTypeDecl));
        if ( (yyvsp[-1].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
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
            deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        }
        delete (yyvsp[-2].s);
        (yyval.pFuncDecl) = pFunction.orphan();
    }
#line 5094 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 126:
#line 972 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5103 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 127:
#line 979 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5112 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 128:
#line 983 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 129:
#line 994 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 5131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 130:
#line 995 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 131:
#line 996 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5143 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 132:
#line 997 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5149 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 133:
#line 998 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5155 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 134:
#line 999 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5161 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 135:
#line 1000 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5167 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 136:
#line 1001 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5173 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 137:
#line 1002 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5179 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 138:
#line 1003 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5185 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 139:
#line 1004 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5191 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 140:
#line 1005 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5197 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 141:
#line 1006 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5203 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 142:
#line 1007 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5209 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 143:
#line 1008 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5215 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 144:
#line 1009 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5221 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 145:
#line 1010 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5227 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 146:
#line 1011 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5233 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 147:
#line 1012 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 5239 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 148:
#line 1016 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5249 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 149:
#line 1021 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5260 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 150:
#line 1027 "ds_parser.ypp" /* yacc.c:1660  */
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 151:
#line 1033 "ds_parser.ypp" /* yacc.c:1660  */
    {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        } else if ( pipeCall->rtti_isMakeStruct() ) {
            auto pMS = (ExprMakeStruct *) pipeCall;
            if ( pMS->block ) {
                das_yyerror("can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt((yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
        } else {
            das_yyerror("can only pipe into function call or [[ make structure ]]",
                tokAt((yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
        }
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 5294 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 152:
#line 1054 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5302 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 153:
#line 1060 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5308 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 154:
#line 1061 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 5324 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 155:
#line 1072 "ds_parser.ypp" /* yacc.c:1660  */
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5330 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 156:
#line 1076 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5338 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 157:
#line 1082 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5346 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 158:
#line 1085 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5354 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 159:
#line 1088 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5363 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 160:
#line 1092 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5371 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 161:
#line 1098 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5377 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 162:
#line 1102 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5383 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 163:
#line 1106 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5391 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 164:
#line 1109 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5399 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 165:
#line 1112 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 166:
#line 1117 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5417 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 167:
#line 1120 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5427 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 168:
#line 1128 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5435 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 169:
#line 1131 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5445 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 170:
#line 1136 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5453 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 171:
#line 1139 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 172:
#line 1147 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 173:
#line 1153 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 5477 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 174:
#line 1154 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 5483 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 175:
#line 1158 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pLet = new ExprLet();
        pLet->at = tokAt((yylsp[-1]));
        pLet->atInit = tokAt((yylsp[0]));
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
                    if ( (yyvsp[-1].b) ) {
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
#line 5519 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 176:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5525 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 177:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 178:
#line 1192 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5539 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 179:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5545 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 180:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5551 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 181:
#line 1195 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5561 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 182:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5567 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 183:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5573 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 184:
#line 1200 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5583 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 185:
#line 1208 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 186:
#line 1212 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5602 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 187:
#line 1217 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5613 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 188:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5619 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 189:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5625 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 190:
#line 1223 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5634 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 191:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5640 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 192:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5646 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 193:
#line 1227 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5656 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 194:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 5662 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 195:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 5668 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 196:
#line 1232 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5679 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 197:
#line 1241 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5687 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 198:
#line 1244 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5695 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 199:
#line 1250 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5701 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 200:
#line 1251 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 201:
#line 1258 "ds_parser.ypp" /* yacc.c:1660  */
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5726 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 202:
#line 1269 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = 0;   /* block */  }
#line 5732 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 203:
#line 1270 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = 1;   /* lambda */ }
#line 5738 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 204:
#line 1271 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = 2;   /* local function */ }
#line 5744 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 205:
#line 1275 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5750 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 206:
#line 1276 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5756 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 207:
#line 1277 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5762 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 208:
#line 1278 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5768 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 209:
#line 1282 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5778 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 210:
#line 1287 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5788 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 211:
#line 1295 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCaptList) = nullptr; }
#line 5794 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 212:
#line 1296 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 213:
#line 1300 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 214:
#line 1306 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkb = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-5].i)==1, (yyvsp[-5].i)==2);
        (yyval.pExpression) = mkb;
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
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( mkb->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
            if ( (yyvsp[-5].i) != 1 ) {   // if its not lambda, can't capture
                das_yyerror("can only have capture section for lambda",
                    mkb->at,CompilationError::invalid_capture);
            }
        }
        if ( (yyvsp[-4].faList) ) {
            for ( auto pA : *(yyvsp[-4].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror("can't apply annotation\n" + err, tokAt((yylsp[-4])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror("blocks are only allowed function annotations", tokAt((yylsp[-4])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-4].faList) );
            delete (yyvsp[-4].faList);
        }
    }
#line 5875 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 215:
#line 1369 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 5881 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 216:
#line 1370 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 5887 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 217:
#line 1371 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 5893 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 218:
#line 1372 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 5899 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 219:
#line 1373 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 5905 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 220:
#line 1374 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 5911 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 221:
#line 1378 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5917 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 222:
#line 1379 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 5923 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 223:
#line 1380 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5929 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 224:
#line 1381 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5935 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 225:
#line 1382 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5941 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 226:
#line 1383 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5947 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 227:
#line 1384 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5953 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 228:
#line 1385 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5959 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 229:
#line 1386 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5965 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 230:
#line 1387 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5971 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 231:
#line 1388 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5977 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 232:
#line 1389 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5983 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 233:
#line 1390 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5989 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 234:
#line 1391 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 5995 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 235:
#line 1392 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6001 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 236:
#line 1393 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6007 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 237:
#line 1397 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6013 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 238:
#line 1398 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 239:
#line 1402 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 240:
#line 1413 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6041 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 241:
#line 1418 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6053 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 242:
#line 1428 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6062 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 243:
#line 1432 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 6068 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 244:
#line 1432 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 6074 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 245:
#line 1432 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6086 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 246:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 6092 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 247:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 6098 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 248:
#line 1439 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 249:
#line 1453 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6123 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 250:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    { das_supress_errors=true; }
#line 6129 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 251:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    { das_supress_errors=false; }
#line 6135 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 252:
#line 1457 "ds_parser.ypp" /* yacc.c:1660  */
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6145 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 253:
#line 1465 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6151 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 254:
#line 1466 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6157 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 255:
#line 1467 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6163 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 256:
#line 1468 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6169 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 257:
#line 1469 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 258:
#line 1470 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6181 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 259:
#line 1471 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6187 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 260:
#line 1472 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6193 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 261:
#line 1473 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6199 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 262:
#line 1474 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6205 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 263:
#line 1475 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6211 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 264:
#line 1476 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6217 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 265:
#line 1477 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6223 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 266:
#line 1478 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6229 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 267:
#line 1479 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6235 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 268:
#line 1480 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6241 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 269:
#line 1481 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6247 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 270:
#line 1482 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6253 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 271:
#line 1483 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6259 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 272:
#line 1484 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6265 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 273:
#line 1485 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6271 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 274:
#line 1486 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6277 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 275:
#line 1487 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6283 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 276:
#line 1488 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6289 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 277:
#line 1489 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6295 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 278:
#line 1490 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6301 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 279:
#line 1491 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6307 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 280:
#line 1492 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6313 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 281:
#line 1493 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6319 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 282:
#line 1494 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6325 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 283:
#line 1495 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6331 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 284:
#line 1496 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6337 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 285:
#line 1497 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6343 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 286:
#line 1498 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6349 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 287:
#line 1499 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6355 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 288:
#line 1500 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6361 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 289:
#line 1501 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6367 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 290:
#line 1502 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6373 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 291:
#line 1503 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6379 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 292:
#line 1504 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6385 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 293:
#line 1505 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6394 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 294:
#line 1509 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6403 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 295:
#line 1513 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6411 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 296:
#line 1516 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6419 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 297:
#line 1519 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6425 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 298:
#line 1520 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 299:
#line 1521 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6437 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 300:
#line 1522 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6451 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 301:
#line 1531 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6465 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 302:
#line 1540 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6471 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 303:
#line 1541 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6479 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 304:
#line 1544 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 6485 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 305:
#line 1544 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 6491 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 306:
#line 1544 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6499 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 307:
#line 1547 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 308:
#line 1551 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6517 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 309:
#line 1555 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 310:
#line 1559 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 311:
#line 1560 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 312:
#line 1561 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 313:
#line 1562 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 314:
#line 1563 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 315:
#line 1564 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 316:
#line 1565 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6577 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 317:
#line 1575 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 6592 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 318:
#line 1585 "ds_parser.ypp" /* yacc.c:1660  */
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
            (yyval.pExpression) = new ExprConstInt(0);   // dummy
        }
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 6649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 319:
#line 1640 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = nullptr; }
#line 6655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 320:
#line 1641 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6661 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 321:
#line 1645 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6667 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 322:
#line 1646 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6673 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 323:
#line 1650 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6683 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 324:
#line 1658 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6691 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 325:
#line 1661 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6700 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 326:
#line 1666 "ds_parser.ypp" /* yacc.c:1660  */
    { das_force_oxford_comma=true; }
#line 6706 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 327:
#line 1666 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( !g_thisStructure->isClass ) {
            das_yyerror("structure can't have member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->name==g_thisStructure->name || (yyvsp[-1].pFuncDecl)->name=="finalize" ) {
            das_yyerror("initializers and finalizers can't be abstract " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-5].faList)!=nullptr ) {
            das_yyerror("abstract functions can't have annotations " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
            delete (yyvsp[-5].faList);
        } else if ( (yyvsp[-1].pFuncDecl)->result->baseType==Type::autoinfer ) {
            das_yyerror("abstract functions must specify return type explicitly " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            auto varName = (yyvsp[-1].pFuncDecl)->name;
            (yyvsp[-1].pFuncDecl)->name = g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
            auto vars = new vector<pair<string,LineInfo>>();
            vars->emplace_back(make_pair(varName,(yyvsp[-1].pFuncDecl)->at));
            TypeDecl * funcType = new TypeDecl(Type::tFunction);
            funcType->at = (yyvsp[-1].pFuncDecl)->at;
            swap ( funcType->firstType, (yyvsp[-1].pFuncDecl)->result );
            funcType->argTypes.reserve ( (yyvsp[-1].pFuncDecl)->arguments.size() );
            if ( g_thisStructure->isClass ) {
                funcType->argTypes.push_back(make_smart<TypeDecl>(g_thisStructure));
                funcType->argNames.push_back("self");
            }
            for ( auto & arg : (yyvsp[-1].pFuncDecl)->arguments ) {
                funcType->argTypes.push_back(arg->type);
                funcType->argNames.push_back(arg->name);
            }
            VariableDeclaration * decl = new VariableDeclaration(
                vars,
                funcType,
                nullptr
            );
            (yyvsp[-6].pVarDeclList)->push_back(decl);
        }
        (yyvsp[-1].pFuncDecl)->delRef();
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
    }
#line 6755 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 328:
#line 1710 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt((yylsp[-2]),(yylsp[0]));
        if ( !g_thisStructure ) {
            das_yyerror("internal error. member function outside of class.",
                (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( !g_thisStructure->isClass ) {
            das_yyerror("structure can't have member function",
                (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            das_yyerror("generic function can't be a member of a class " + (yyvsp[0].pFuncDecl)->getMangledName(),
                (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            if ( (yyvsp[0].pFuncDecl)->name != g_thisStructure->name && (yyvsp[0].pFuncDecl)->name != "finalize") {
                auto varName = (yyvsp[0].pFuncDecl)->name;
                (yyvsp[0].pFuncDecl)->name = g_thisStructure->name + "`" + (yyvsp[0].pFuncDecl)->name;
                auto vars = new vector<pair<string,LineInfo>>();
                vars->emplace_back(make_pair(varName,(yyvsp[0].pFuncDecl)->at));
                Expression * finit = new ExprAddr((yyvsp[0].pFuncDecl)->at, inThisModule((yyvsp[0].pFuncDecl)->name));
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
                modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, false);
            } else {
                if ( (yyvsp[-1].b) ) {
                    das_yyerror("can't override constructor " + (yyvsp[0].pFuncDecl)->getMangledName(),
                        (yyvsp[0].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[0].pFuncDecl)->name!="finalize" ) {
                    auto ctr = makeClassConstructor(g_thisStructure, (yyvsp[0].pFuncDecl));
                    if ( !g_Program->addFunction(ctr) ) {
                        das_yyerror("intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    (yyvsp[0].pFuncDecl)->name = g_thisStructure->name + "`" + g_thisStructure->name;
                    modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, false);
                } else {
                    modifyToClassMember((yyvsp[0].pFuncDecl), g_thisStructure, true);
                }
            }
            runFunctionAnnotations((yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt((yylsp[-3])));
            if ( !g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror("function is already defined " + (yyvsp[0].pFuncDecl)->getMangledName(),
                    (yyvsp[0].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[0].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 6815 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 329:
#line 1768 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6825 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 330:
#line 1773 "ds_parser.ypp" /* yacc.c:1660  */
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 6835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 331:
#line 1781 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 332:
#line 1782 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6847 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 333:
#line 1786 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6855 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 334:
#line 1789 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6866 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 335:
#line 1798 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6872 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 336:
#line 1799 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6878 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 337:
#line 1803 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 6889 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 338:
#line 1812 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6895 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 339:
#line 1813 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 6901 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 340:
#line 1818 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6907 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 341:
#line 1819 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 6913 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 342:
#line 1823 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 6924 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 343:
#line 1829 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 6935 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 344:
#line 1835 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 6943 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 345:
#line 1838 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 346:
#line 1842 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6963 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 347:
#line 1848 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 6974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 348:
#line 1857 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_COPY; }
#line 6980 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 349:
#line 1858 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_MOVE; }
#line 6986 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 350:
#line 1859 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.i) = CorM_CLONE; }
#line 6992 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 351:
#line 1863 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 6998 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 352:
#line 1864 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 7004 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 353:
#line 1868 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7012 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 354:
#line 1871 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7022 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 355:
#line 1876 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7032 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 356:
#line 1881 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 357:
#line 1889 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7058 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 358:
#line 1900 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 359:
#line 1903 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7075 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 360:
#line 1910 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 7081 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 361:
#line 1911 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 7087 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 362:
#line 1915 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7119 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 363:
#line 1942 "ds_parser.ypp" /* yacc.c:1660  */
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7128 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 364:
#line 1945 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7139 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 365:
#line 1954 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7147 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 366:
#line 1957 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7161 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 367:
#line 1966 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7175 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 368:
#line 1979 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7189 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 372:
#line 2000 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-3]));
        pEnum->name = *(yyvsp[-3].s);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7205 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 373:
#line 2011 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-5].s),tokAt((yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt((yylsp[-5]));
        pEnum->name = *(yyvsp[-5].s);
        pEnum->baseType = (yyvsp[-3].type);
        if ( !g_Program->addEnumeration(pEnum) ) {
            das_yyerror("enumeration is already defined "+*(yyvsp[-5].s),tokAt((yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-5].s);
    }
#line 7222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 374:
#line 2026 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = nullptr; }
#line 7228 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 375:
#line 2027 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.s) = (yyvsp[0].s); }
#line 7234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 376:
#line 2031 "ds_parser.ypp" /* yacc.c:1660  */
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
                pStruct->genCtor = false;
            } else if ( structs.size()==0 ) {
                das_yyerror("parent structure not found "+*(yyvsp[0].s),tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = g_Program->describeCandidates(structs);
                das_yyerror("too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt((yylsp[0])),
                    CompilationError::structure_not_found);
            }
            delete (yyvsp[0].s);
        }
        if ( !pStruct ) {
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
        }
        if ( !g_Program->addStructure(pStruct) ) {
            das_yyerror("structure is already defined "+*(yyvsp[-1].s),tokAt((yylsp[-1])),
                CompilationError::structure_already_declared);
            (yyval.pStructure) = nullptr;
        } else {
            (yyval.pStructure) = pStruct.get();
            g_thisStructure = pStruct.get();
        }
        delete (yyvsp[-1].s);
    }
#line 7273 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 377:
#line 2068 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = true; }
#line 7279 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 378:
#line 2069 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.b) = false; }
#line 7285 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 379:
#line 2073 "ds_parser.ypp" /* yacc.c:1660  */
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7291 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 380:
#line 2073 "ds_parser.ypp" /* yacc.c:1660  */
    {
        if ( (yyvsp[-4].pStructure) ) {
            auto pStruct = (yyvsp[-4].pStructure);
            pStruct->at = tokAt((yylsp[-4]));
            if ( pStruct->parent && pStruct->parent->isClass != pStruct->isClass ) {
                if ( pStruct->isClass ) {
                    das_yyerror("class can only derive from class", pStruct->at,
                        CompilationError::invalid_override);
                } else {
                    das_yyerror("struct can only derive from struct", pStruct->at,
                        CompilationError::invalid_override);
                }
            }
            if ( pStruct->isClass ) {
                makeClassRtti(pStruct);
                auto virtfin = makeClassFinalize(pStruct);
                if ( !g_Program->addFunction(virtfin) ) {
                    das_yyerror("built-in finalizer is already defined " + virtfin->getMangledName(),
                        virtfin->at, CompilationError::function_already_declared);
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
            if ( (yyvsp[-6].faList) ) {
                for ( auto pA : *(yyvsp[-6].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror("can't 'touch' with structure annotation\n" + err,
                                    tokAt((yylsp[-5])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-6].faList)->size()!=1 ) {
                                das_yyerror("structures are only allowed one structure type annotation", tokAt((yylsp[-5])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror("handled structure is already defined "+pStruct->name,tokAt((yylsp[-5])),
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
                swap ( pStruct->annotations, *(yyvsp[-6].faList) );
                delete (yyvsp[-6].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        g_thisStructure = nullptr;
    }
#line 7377 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 381:
#line 2157 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7389 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 382:
#line 2164 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7400 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 383:
#line 2173 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBool; }
#line 7406 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 384:
#line 2174 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tString; }
#line 7412 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 385:
#line 2175 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 7418 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 386:
#line 2176 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 7424 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 387:
#line 2177 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 7430 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 388:
#line 2178 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt64; }
#line 7436 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 389:
#line 2179 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt2; }
#line 7442 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 390:
#line 2180 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt3; }
#line 7448 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 391:
#line 2181 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt4; }
#line 7454 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 392:
#line 2182 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 7460 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 393:
#line 2183 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 7466 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 394:
#line 2184 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 7472 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 395:
#line 2185 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt64; }
#line 7478 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 396:
#line 2186 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt2; }
#line 7484 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 397:
#line 2187 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt3; }
#line 7490 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 398:
#line 2188 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt4; }
#line 7496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 399:
#line 2189 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat; }
#line 7502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 400:
#line 2190 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat2; }
#line 7508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 401:
#line 2191 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat3; }
#line 7514 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 402:
#line 2192 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tFloat4; }
#line 7520 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 403:
#line 2193 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tVoid; }
#line 7526 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 404:
#line 2194 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tRange; }
#line 7532 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 405:
#line 2195 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tURange; }
#line 7538 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 406:
#line 2196 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tDouble; }
#line 7544 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 407:
#line 2197 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tBitfield; }
#line 7550 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 408:
#line 2201 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt; }
#line 7556 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 409:
#line 2202 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt8; }
#line 7562 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 410:
#line 2203 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tInt16; }
#line 7568 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 411:
#line 2204 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt; }
#line 7574 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 412:
#line 2205 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt8; }
#line 7580 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 413:
#line 2206 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.type) = Type::tUInt16; }
#line 7586 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 414:
#line 2210 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7599 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 415:
#line 2221 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7608 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 416:
#line 2225 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7620 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 417:
#line 2235 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7632 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 418:
#line 2242 "ds_parser.ypp" /* yacc.c:1660  */
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7643 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 419:
#line 2251 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7649 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 420:
#line 2251 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7655 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 421:
#line 2251 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7670 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 422:
#line 2264 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7676 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 423:
#line 2265 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7682 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 424:
#line 2266 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7688 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 425:
#line 2267 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7694 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 426:
#line 2268 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 7713 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 427:
#line 2282 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7724 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 428:
#line 2288 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7733 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 429:
#line 2292 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7742 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 430:
#line 2296 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7752 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 431:
#line 2301 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7762 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 432:
#line 2306 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7772 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 433:
#line 2311 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7782 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 434:
#line 2316 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7791 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 435:
#line 2320 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7800 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 436:
#line 2324 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7810 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 437:
#line 2329 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7819 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 438:
#line 2333 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7829 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 439:
#line 2338 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7835 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 440:
#line 2338 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7841 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 441:
#line 2338 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7852 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 442:
#line 2344 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 7864 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 443:
#line 2351 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7870 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 444:
#line 2351 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7876 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 445:
#line 2351 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7886 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 446:
#line 2356 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7892 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 447:
#line 2356 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7898 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 448:
#line 2356 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7909 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 449:
#line 2362 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7915 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 450:
#line 2362 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7921 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 451:
#line 2362 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7931 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 452:
#line 2367 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7940 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 453:
#line 2371 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7946 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 454:
#line 2371 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7952 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 455:
#line 2371 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 7962 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 456:
#line 2376 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 7968 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 457:
#line 2376 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 7974 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 458:
#line 2376 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 7988 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 459:
#line 2385 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7997 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 460:
#line 2389 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8003 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 461:
#line 2389 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8009 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 462:
#line 2389 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8019 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 463:
#line 2394 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8025 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 464:
#line 2394 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8031 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 465:
#line 2394 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8045 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 466:
#line 2403 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8054 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 467:
#line 2407 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8060 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 468:
#line 2407 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8066 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 469:
#line 2407 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8076 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 470:
#line 2412 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8082 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 471:
#line 2412 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8088 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 472:
#line 2412 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8102 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 473:
#line 2421 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8108 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 474:
#line 2421 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8114 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 475:
#line 2421 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8125 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 476:
#line 2427 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth ++; }
#line 8131 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 477:
#line 2427 "ds_parser.ypp" /* yacc.c:1660  */
    { das_arrow_depth --; }
#line 8137 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 478:
#line 2427 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8148 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 479:
#line 2436 "ds_parser.ypp" /* yacc.c:1660  */
    { das_need_oxford_comma=false; }
#line 8154 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 480:
#line 2436 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 8171 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 481:
#line 2451 "ds_parser.ypp" /* yacc.c:1660  */
    { das_need_oxford_comma=false; }
#line 8177 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 482:
#line 2451 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 8198 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 483:
#line 2471 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8204 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 484:
#line 2472 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8210 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 485:
#line 2473 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8216 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 486:
#line 2474 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8222 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 487:
#line 2478 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8234 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 488:
#line 2485 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8246 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 489:
#line 2492 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8257 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 490:
#line 2498 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8268 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 491:
#line 2507 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8278 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 492:
#line 2512 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8287 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 493:
#line 2519 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 8293 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 494:
#line 2520 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8299 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 495:
#line 2524 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8310 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 496:
#line 2530 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8322 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 497:
#line 2537 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8335 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 498:
#line 2545 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8347 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 499:
#line 2552 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8360 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 500:
#line 2560 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8374 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 501:
#line 2572 "ds_parser.ypp" /* yacc.c:1660  */
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8382 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 502:
#line 2575 "ds_parser.ypp" /* yacc.c:1660  */
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
#line 8398 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 503:
#line 2589 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8409 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 504:
#line 2598 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8415 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 505:
#line 2599 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8421 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 506:
#line 2603 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8431 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 507:
#line 2608 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8440 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 508:
#line 2615 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8450 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 509:
#line 2620 "ds_parser.ypp" /* yacc.c:1660  */
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8463 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 510:
#line 2631 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8473 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 511:
#line 2636 "ds_parser.ypp" /* yacc.c:1660  */
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8482 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 512:
#line 2643 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8496 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 513:
#line 2655 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = nullptr; }
#line 8502 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 514:
#line 2656 "ds_parser.ypp" /* yacc.c:1660  */
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8508 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 515:
#line 2660 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-8]),(yylsp[0]));
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
#line 8531 "ds_parser.cpp" /* yacc.c:1660  */
    break;

  case 516:
#line 2678 "ds_parser.ypp" /* yacc.c:1660  */
    {
        auto pFor = make_smart<ExprFor>(tokAt((yylsp[-8])));
        pFor->visibility = tokRangeAt((yylsp[-8]),(yylsp[0]));
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
#line 8554 "ds_parser.cpp" /* yacc.c:1660  */
    break;


#line 8558 "ds_parser.cpp" /* yacc.c:1660  */
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
#line 2698 "ds_parser.ypp" /* yacc.c:1903  */


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
