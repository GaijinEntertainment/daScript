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
#line 62 "ds_parser.ypp"

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

#line 107 "ds_parser.cpp"

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

#line 209 "ds_parser.cpp"

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
    ANDAND = 383,
    OROR = 384,
    XORXOR = 385,
    ANDANDEQU = 386,
    OROREQU = 387,
    XORXOREQU = 388,
    BRABRAB = 389,
    BRACBRB = 390,
    CBRCBRB = 391,
    INTEGER = 392,
    LONG_INTEGER = 393,
    UNSIGNED_INTEGER = 394,
    UNSIGNED_LONG_INTEGER = 395,
    FLOAT = 396,
    DOUBLE = 397,
    NAME = 398,
    BEGIN_STRING = 399,
    STRING_CHARACTER = 400,
    END_STRING = 401,
    BEGIN_STRING_EXPR = 402,
    END_STRING_EXPR = 403,
    END_OF_READ = 404,
    UNARY_MINUS = 405,
    UNARY_PLUS = 406,
    PRE_INC = 407,
    PRE_DEC = 408,
    POST_INC = 409,
    POST_DEC = 410,
    DEREF = 411
  };
#endif

/* Value type.  */
#if ! defined DAS_YYSTYPE && ! defined DAS_YYSTYPE_IS_DECLARED
union DAS_YYSTYPE
{
#line 94 "ds_parser.ypp"

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

#line 406 "ds_parser.cpp"

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
#define YYLAST   7699

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  184
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  528
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  940

#define YYUNDEFTOK  2
#define YYMAXUTOK   411

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
       2,     2,     2,   167,     2,   183,   178,   163,   156,     2,
     176,   177,   161,   160,   150,   159,   172,   162,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   153,   180,
     157,   151,   158,   152,   179,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   174,     2,   175,   155,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   181,   154,   182,   166,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   164,   165,   168,   169,   170,
     171,   173
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   441,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   457,   469,   470,   474,   478,
     483,   492,   500,   516,   521,   529,   529,   559,   581,   585,
     588,   594,   603,   624,   647,   648,   652,   656,   657,   661,
     664,   670,   676,   679,   685,   686,   690,   691,   692,   701,
     702,   706,   715,   731,   739,   749,   758,   759,   760,   761,
     762,   763,   767,   772,   780,   781,   782,   783,   784,   785,
     786,   787,   793,   806,   822,   823,   827,   834,   846,   850,
     857,   858,   862,   863,   864,   868,   873,   880,   884,   885,
     886,   887,   888,   889,   890,   891,   892,   893,   894,   895,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,   908,   909,   910,   911,   912,   913,   914,   915,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
     929,   950,   988,   995,   999,  1010,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1032,  1037,  1043,  1049,  1070,  1076,
    1077,  1088,  1092,  1098,  1101,  1104,  1108,  1114,  1118,  1122,
    1125,  1128,  1133,  1136,  1144,  1147,  1152,  1155,  1163,  1169,
    1170,  1174,  1208,  1208,  1208,  1211,  1211,  1211,  1216,  1216,
    1216,  1224,  1228,  1233,  1239,  1239,  1239,  1243,  1243,  1243,
    1248,  1248,  1248,  1257,  1260,  1266,  1267,  1274,  1285,  1286,
    1287,  1291,  1292,  1293,  1294,  1298,  1303,  1311,  1312,  1316,
    1321,  1385,  1386,  1387,  1388,  1389,  1390,  1394,  1395,  1396,
    1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,  1412,  1416,  1417,  1421,  1432,
    1437,  1447,  1451,  1451,  1451,  1458,  1458,  1458,  1472,  1476,
    1476,  1476,  1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,
    1522,  1523,  1524,  1525,  1526,  1527,  1531,  1535,  1538,  1541,
    1542,  1543,  1544,  1553,  1562,  1563,  1566,  1566,  1566,  1569,
    1573,  1577,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1597,
    1607,  1662,  1663,  1667,  1668,  1672,  1680,  1683,  1688,  1687,
    1732,  1790,  1795,  1803,  1804,  1808,  1811,  1820,  1821,  1825,
    1834,  1835,  1840,  1841,  1845,  1851,  1857,  1860,  1864,  1870,
    1879,  1880,  1881,  1885,  1886,  1890,  1893,  1898,  1903,  1911,
    1922,  1925,  1932,  1933,  1937,  1964,  1964,  1976,  1979,  1988,
    2001,  2013,  2014,  2018,  2022,  2033,  2048,  2049,  2053,  2090,
    2091,  2095,  2095,  2179,  2186,  2195,  2196,  2197,  2198,  2199,
    2200,  2201,  2202,  2203,  2204,  2205,  2206,  2207,  2208,  2209,
    2210,  2211,  2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,
    2223,  2224,  2225,  2226,  2227,  2228,  2232,  2243,  2247,  2257,
    2264,  2273,  2273,  2273,  2286,  2287,  2288,  2289,  2290,  2304,
    2310,  2314,  2318,  2323,  2328,  2333,  2338,  2342,  2346,  2351,
    2355,  2360,  2360,  2360,  2366,  2373,  2373,  2373,  2378,  2378,
    2378,  2384,  2384,  2384,  2389,  2393,  2393,  2393,  2398,  2398,
    2398,  2407,  2411,  2411,  2411,  2416,  2416,  2416,  2425,  2429,
    2429,  2429,  2434,  2434,  2434,  2443,  2443,  2443,  2449,  2449,
    2449,  2458,  2458,  2473,  2473,  2493,  2494,  2495,  2496,  2500,
    2507,  2514,  2520,  2529,  2534,  2541,  2542,  2546,  2552,  2559,
    2567,  2574,  2582,  2594,  2597,  2611,  2620,  2621,  2625,  2630,
    2637,  2642,  2653,  2658,  2665,  2677,  2678,  2682,  2700
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
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"",
  "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
      44,    61,    63,    58,   124,    94,    38,    60,    62,    45,
      43,    42,    47,    37,   405,   406,   126,    33,   407,   408,
     409,   410,    46,   411,    91,    93,    40,    41,    36,    64,
      59,   123,   125,    35
};
# endif

#define YYPACT_NINF -579

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-579)))

#define YYTABLE_NINF -483

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -579,    27,  -579,  -579,   -99,    37,   -78,   -69,  -131,  -579,
     -44,  -579,  -579,    13,  -579,  -579,  -579,  -579,  -579,   384,
    -579,   193,  -579,  -579,  -579,  -579,  -579,  -579,   117,  -579,
     108,   116,   139,  -579,  -579,    51,  -579,   -64,   174,  -579,
     182,   194,  -579,   203,   202,   181,  -579,   -16,  -579,  -579,
    -579,     3,   217,  -579,   180,    20,   -99,   233,    37,   232,
    -579,   237,   242,  -579,   263,  -579,   244,  -579,   -92,   216,
     221,  -579,   255,   -99,    13,  -579,  7532,   374,   380,  -579,
     241,   238,  -579,   269,  -579,  -579,   422,  -579,  -579,  -579,
    -579,  -579,   279,   197,  -579,  -579,  -579,  -579,   372,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,   245,   -72,  7448,
    -579,  -579,   284,   288,  -579,   -86,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,   -93,
     281,  -579,  -579,    -5,  -579,   251,   -67,   260,  -579,    94,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,     5,  -579,  -579,
      44,  -579,   289,   291,   296,   297,  -579,  -579,  -579,   276,
    -579,  -579,  -579,  -579,  -579,   301,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,   302,  -579,  -579,  -579,   303,
     304,  -579,  -579,  -579,  -579,   305,   306,  -579,  -579,  -579,
    -579,  -579,    -8,  -579,   265,   311,  -579,   285,  -579,   -99,
    -579,   -10,  -579,  -123,  7448,  -579,  1319,  -579,  -579,  -579,
    -579,  -579,   234,  -579,  -579,  -579,   197,  -579,   -66,  4696,
    -579,  -579,  -579,  -579,  -579,   323,  -579,   186,   200,   206,
    -579,  -579,  -579,  -579,  -579,  -579,   436,  -579,  -579,    -7,
    2548,  -579,  -579,   -63,  7448,   -62,   100,   330,  -579,   -68,
    -579,   336,  3457,  -579,  4696,  -579,  -579,   330,  -579,  -579,
    7365,   299,  1966,  -579,  -579,   238,  4696,   300,  4696,   320,
     321,   310,  -579,   307,   331,   343,  2684,   238,  -579,   335,
    2122,  4696,  4696,   215,  7199,  7282,  4696,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  4696,  4696,  4696,    -5,  4696,  4696,
    4696,  -579,   315,  -579,   459,  -579,  -579,   316,   317,  4696,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,   -84,   328,
    -579,   338,   339,  -579,  -579,  -579,   330,  -579,  -579,  -579,
     326,  -579,  -579,   -71,  -579,  -579,  -579,  -579,  -579,  6400,
     345,  -579,  -579,  -579,  -579,  -579,    11,   364,  7448,  -579,
     -46,  -579,  -579,  5372,  7448,  7448,  7448,  7448,   347,   284,
    7448,   241,  7448,   241,  7448,   241,  7531,   288,  -579,  -579,
    -579,   350,  -579,  -579,  6059,  -579,  -579,  3457,  -579,  -579,
     351,  -579,  -579,  -579,  7448,  -579,  2820,  -579,   400,     4,
     728,  -579,    -5,  2820,  -579,  -579,   409,  4784,   518,  6717,
    4696,   400,  -579,  -579,  4696,  -579,  -579,   379,   396,  6717,
    -579,  7448,  2820,  -579,  4882,   295,   295,   356,  -579,   330,
    1045,   330,  1658,  6498,  -579,   187,   259,   295,   295,   -47,
    -579,   295,   295,  5518,   -57,   355,  -579,  -579,   400,  -579,
    2258,  -579,  -579,  -579,  -579,   403,   215,  -579,    22,   395,
    4696,  4696,  4696,  4696,  4696,  4696,  4696,  4696,  4696,  4696,
    -579,  -579,  4696,  4696,  4696,  4696,  4696,  4696,   397,  2956,
    4696,   401,  4696,  4696,  4696,  4696,  4696,  4696,  4696,  4696,
    4696,  4696,  4696,  4696,  4696,  4696,  3092,  3228,  4696,  4696,
    4696,  4696,  4696,  4696,  4696,  4696,  4696,  4696,   402,  4696,
    3364,  -579,   532,   501,   366,  -579,   551,  -579,  -579,  -579,
    2820,  -579,    46,   563,   984,  1106,  -579,    80,  1258,   281,
    1597,   281,  1751,   281,   120,  -579,    91,  3457,   165,  -579,
    -579,  -579,  1908,  -579,  6294,  -579,  4696,  3518,  2412,  -579,
    4980,  4696,  4696,  -579,   238,  5594,  -579,  7448,  7448,  5673,
    7448,  -579,  -579,  2064,  -579,  5078,  -579,  -579,    12,   215,
      79,  1812,  3672,  6498,   398,   -11,   375,   399,  -579,  -579,
     146,    23,  3826,   -11,   236,  4696,  4696,   365,  -579,  4696,
     406,   408,  -579,   210,  -579,  -579,   308,   423,  -579,    65,
    6717,   -58,   241,  -579,   411,  -579,  -579,  6717,  6717,  6717,
    6717,  6717,  6717,  6717,  6717,   150,   150,   521,  6717,  6717,
     521,  7059,  7059,   393,  4696,  6717,     9,  -579,  6133,   102,
     102,  6717,   150,   150,  6717,  6717,  6936,  6790,  6863,  6717,
    6717,  6717,  4696,  6717,   427,  6571,  6961,   691,  7034,   521,
     521,    74,    74,     9,     9,     9,  -579,   570,  6207,  -579,
      66,    93,  -579,   330,  -579,   436,  -579,  2820,  -579,  5176,
    -579,  7448,  -579,  -579,  -579,   430,  -579,   417,  -579,   418,
    -579,   419,  7448,  -579,  7531,  -579,   288,   436,  4696,   -49,
    -579,    82,   421,   437,  5749,  -579,  -579,  -579,  2197,  2351,
    -579,  2487,   403,  -579,  4696,  -579,  4696,  4696,    -6,   439,
     423,   404,   412,  4696,   413,  4696,  4696,   423,   407,   415,
    6717,  -579,  -579,  6644,  -579,   441,  7448,   241,  1501,   238,
    -579,  -579,  4696,  -579,   112,   107,  4696,  -579,   448,   449,
     450,   451,  -579,   168,   281,  -579,  3980,   327,  -579,   327,
    -579,  4696,  -579,  -579,  -579,  -579,     3,  -579,  -579,  5274,
    -579,  -579,  2623,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  3457,  -579,  -579,  -579,  6717,   238,  -579,  -579,
     198,  -579,  -579,  -579,  -579,   424,   -39,  6717,  6717,   -11,
     426,   132,   398,   428,  -579,  6717,  -579,  -579,   -38,   -11,
     431,  -579,  -579,  -579,  2759,   281,  -579,  -579,   400,   420,
    6717,  -579,  -579,  -579,  -579,   -58,   434,   -94,  7448,  -579,
     156,  6717,  -579,     3,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  7448,  4134,   456,  4696,  4696,  4696,  4270,  4696,   435,
     438,  4696,  4696,  -579,  4696,   429,  -579,  -579,   444,   308,
    -579,  -579,  -579,  4424,  -579,  -579,  2895,  -579,   425,  -579,
    3031,   455,  5828,   458,   521,   521,   521,  -579,  5904,  5445,
     442,  -579,  6717,  6717,  5445,   446,    -5,  -579,  -579,  4696,
    6717,  -579,  -579,  -579,  -579,  -579,  4560,  -579,   535,   447,
    -579,   432,  -579,  -579,    -5,  6717,  -579,   452,  7448,   461,
    5983,  4696,   453,   457,  -579,  -579,  3167,  -579,  -579,  6717,
    -579,  -579,  -579,  7448,   454,  3303,  -579,  -579,   462,  -579
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,    80,     1,   179,     0,     0,     0,     0,     0,   180,
       0,   493,   491,     0,    14,     3,    10,     9,     8,     0,
       7,   372,     6,    11,     5,     4,    12,    13,    70,    72,
      27,    39,    36,    37,    29,    34,    28,     0,     0,    15,
       0,     0,    75,     0,   159,    76,    78,     0,    74,   390,
     389,     0,     0,   373,   375,     0,     0,     0,     0,     0,
      35,     0,     0,    32,     0,   377,     0,   381,     0,     0,
       0,   161,     0,     0,     0,    81,     0,     0,     0,    87,
      82,     0,   130,   386,   391,   370,     0,    68,    69,    66,
      67,    65,     0,     0,    64,    73,    40,    38,    34,    31,
      30,   420,   423,   421,   424,   422,   425,     0,     0,     0,
     383,   382,     0,     0,   160,     0,    79,    90,    91,    93,
      92,    94,    95,    96,    97,   122,   123,   120,   121,   113,
     124,   125,   114,   111,   112,   126,   127,   128,   129,   101,
     102,   103,    98,    99,   100,   116,   117,   115,   109,   110,
     105,   104,   106,   107,   108,    89,    88,   118,   119,   331,
      85,   154,   132,     0,   388,     0,     0,     0,    16,     0,
      60,    61,    58,    59,    57,    56,    62,     0,    33,   377,
       0,   384,     0,     0,     0,     0,   395,   415,   396,   427,
     397,   401,   402,   403,   404,   419,   408,   409,   410,   411,
     412,   413,   414,   416,   417,   464,   400,   407,   418,   471,
     478,   398,   405,   399,   406,     0,     0,   426,   434,   437,
     435,   436,     0,   429,     0,     0,   350,     0,    77,     0,
      83,     0,   343,     0,     0,   131,     0,   387,   336,   393,
     374,   371,   363,   376,    17,    18,     0,    71,     0,     0,
     378,   455,   458,   461,   451,     0,   431,   465,   472,   479,
     485,   488,   442,   447,   441,   454,     0,   450,   444,     0,
       0,   380,   446,     0,     0,     0,     0,     0,   341,   354,
      84,   331,    86,   156,     0,    49,    50,     0,   268,   269,
       0,     0,     0,   262,   167,     0,     0,     0,     0,     0,
       0,     0,   168,     0,     0,     0,     0,     0,   419,     0,
       0,     0,     0,     0,     0,     0,     0,   221,   223,   222,
     224,   225,   226,    19,     0,     0,     0,     0,     0,     0,
       0,   208,   209,   135,   133,   266,   265,     0,     0,     0,
     149,   144,   142,   141,   143,   219,   155,   136,   263,     0,
     324,     0,     0,   147,   148,   150,     0,   140,   323,   322,
      80,   327,   264,     0,   137,   326,   325,   304,   270,   227,
       0,   267,   495,   496,   497,   498,   331,     0,     0,   364,
       0,    63,   385,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,    82,     0,    82,     0,     0,   449,   443,
     445,     0,   448,   439,     0,   430,   494,   349,   492,   351,
       0,   342,   353,   352,     0,   355,     0,   344,     0,     0,
     163,   166,     0,     0,   169,   172,     0,   227,     0,   162,
       0,     0,   182,   185,     0,   153,   188,     0,     0,    43,
      53,     0,     0,   176,   227,   296,   297,   209,   158,     0,
     505,     0,     0,     0,   522,     0,     0,   274,   273,   309,
      25,   272,   271,     0,   210,     0,   151,   152,     0,   330,
       0,   139,   145,   146,   181,   217,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,   299,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,     0,
       0,   392,     0,   333,     0,   394,     0,   361,   362,   360,
       0,   379,     0,     0,     0,     0,   428,     0,     0,    85,
       0,    85,     0,    85,   159,   347,     0,   345,     0,   440,
     438,   332,   356,   359,   358,    54,     0,     0,     0,   173,
     227,     0,     0,   170,     0,     0,    55,     0,     0,     0,
       0,    41,    42,     0,   177,   227,   174,   210,     0,     0,
     159,     0,     0,   513,   503,   505,     0,   516,   517,   518,
       0,     0,     0,   505,     0,     0,     0,     0,    22,     0,
      20,     0,   300,   252,   251,   154,    46,     0,   305,     0,
     203,     0,    82,   157,     0,   319,   320,   237,   238,   240,
     239,   241,   231,   232,   233,   275,   276,   288,   242,   243,
     289,   286,   287,     0,     0,   229,   314,   303,     0,   328,
     329,   230,   277,   278,   244,   245,   293,   294,   295,   234,
     235,   236,     0,   228,     0,     0,   291,   292,   290,   284,
     285,   280,   279,   281,   282,   283,   258,     0,     0,   307,
       0,   333,   334,     0,   337,   360,   365,     0,   369,   227,
     456,     0,   462,   452,   432,     0,   466,     0,   473,     0,
     480,     0,     0,   486,     0,   489,     0,   352,     0,     0,
     164,     0,     0,     0,     0,   171,   178,   310,     0,     0,
     311,     0,   217,   175,     0,   506,     0,     0,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     515,   523,   524,     0,    23,    26,     0,    82,     0,     0,
      44,    45,     0,    51,     0,     0,     0,   306,     0,     0,
       0,     0,   215,     0,    85,   316,     0,   247,   302,   246,
     321,     0,   260,   301,   308,   338,     0,   335,   367,   227,
     368,   457,     0,   463,   453,   433,   467,   469,   474,   476,
     481,   483,   346,   487,   348,   490,   357,     0,   165,   194,
       0,   191,   183,   186,   189,     0,     0,   500,   499,   505,
       0,     0,   504,     0,   508,   514,   520,   519,     0,   505,
       0,   521,    21,    24,     0,    85,   134,    47,     0,     0,
     204,   213,   214,   212,   211,     0,     0,     0,     0,   249,
       0,   315,   261,     0,   340,   366,   459,   470,   477,   484,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,     0,     0,   511,   253,     0,    46,
     248,   216,   218,     0,   205,   220,     0,   250,     0,   460,
       0,     0,     0,     0,   184,   187,   190,   312,     0,   525,
       0,   509,   502,   501,   525,     0,     0,   256,    48,     0,
     206,   317,   339,   195,   197,   192,     0,   313,     0,     0,
     510,     0,   512,   254,     0,   207,   318,     0,     0,     0,
       0,     0,     0,     0,   257,   196,     0,   200,   193,   526,
     527,   528,   198,     0,     0,     0,   199,   201,     0,   202
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -579,  -579,  -579,   171,   575,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   536,  -579,  -579,   584,  -579,  -579,
    -579,  -224,  -579,  -579,  -579,  -579,  -579,  -579,   405,  -579,
     590,   -52,  -579,   573,  -579,   -40,  -381,  -538,  -579,  -579,
    -195,  -127,   -76,  -579,    35,  -284,   -13,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   651,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -514,  -579,  -579,  -182,  -579,   -65,  -296,  -579,  -235,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
     443,  -579,  -579,   278,   -26,  -579,  -579,  -579,   377,  -579,
     -48,  -579,  -271,   264,  -277,  -270,   124,  -579,   312,  -579,
    -579,  -579,  -579,   483,   595,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -157,  -106,  -579,  -579,  -579,   277,  -579,
    -579,  -579,   -87,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,   381,  -578,  -433,  -575,  -579,  -579,  -310,   -61,   218,
    -579,  -579,  -579,  -227,  -579
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,   169,   175,   456,   335,   745,   336,   611,
      16,    17,    35,    36,    63,    18,    32,    33,   337,   338,
     752,   753,   339,   340,   341,   342,   343,   344,   176,   177,
      29,    30,    45,    46,    47,    19,   160,   235,    80,    20,
      81,    82,   345,   346,   236,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   577,   854,   578,
     855,   580,   856,   359,   851,   917,   918,   934,   933,   938,
     619,   875,   360,   762,   763,   622,   361,   362,   426,   364,
     365,   366,   367,   746,   896,   747,   914,   368,   677,   842,
     620,   838,   916,   231,   683,   534,   376,   843,   232,   233,
     555,   556,   226,   227,   727,   278,   540,   380,   241,   166,
      54,    22,    86,   108,    67,    68,    23,    24,   164,    84,
      52,    25,   165,   279,   370,   107,   219,   220,   224,   221,
     389,   785,   557,   387,   784,   384,   781,   385,   879,   386,
     783,   390,   786,   391,   847,   392,   788,   393,   848,   394,
     790,   395,   849,   396,   793,   397,   795,    26,    41,    27,
      40,   371,   594,   595,   596,   372,   597,   598,   599,   600,
     373,   455,   374,   909,   375
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   363,   416,   218,   409,   162,   454,   411,   425,   242,
     549,   697,   551,   699,   553,   701,   680,   448,   -80,   603,
     731,   115,   222,   566,   262,   399,   443,     2,   738,   478,
     479,   724,   873,     3,    87,    88,   277,    76,   589,   755,
     624,   229,   736,   589,    28,   412,    42,   476,   263,   264,
      38,    66,   709,   711,   280,   758,     4,   281,     5,   469,
       6,    48,     7,   759,    56,    34,     8,   537,   501,   502,
      43,   180,    59,     9,    37,   538,   239,   180,   262,    10,
     405,   225,   377,   413,   230,   414,    44,   161,   415,    64,
     110,   228,   470,   760,   478,   479,   217,    11,   761,    39,
     613,   756,   263,   264,    60,   539,   265,    77,    78,   477,
     181,   756,   756,   490,   491,   240,   382,    65,    12,   406,
     408,   498,    43,   500,   501,   502,   503,   529,   218,   504,
     419,   797,   563,   239,    74,   682,   775,   754,    44,   569,
      43,   858,   864,   266,   267,   229,    79,   282,   268,   400,
     237,   269,   812,   810,   377,   246,    44,    89,   584,    75,
     265,    90,   377,    91,    92,   625,   270,   401,   218,   730,
     478,   479,   271,   377,    31,   272,   402,   276,   490,   491,
     623,   528,   247,   529,   218,    13,   498,   407,   500,   501,
     502,   503,   412,   531,   504,   249,    93,   266,   267,   242,
     726,    13,   268,   420,   690,   269,    72,    14,   218,   218,
     806,   170,   171,    61,   498,   756,   756,   501,   502,   428,
     270,   217,   818,    62,   250,   412,   837,   450,   452,   272,
     413,   440,   756,   726,   859,   525,   526,   527,   694,   244,
     245,   764,   757,   774,   865,   412,   528,    72,   529,   703,
      56,    53,   840,   861,   490,   491,   688,   729,    56,   798,
     695,   217,   498,   413,   500,   501,   502,   503,    55,    57,
     504,   704,   218,   702,   528,   410,   529,   217,   218,   218,
     218,   218,   829,   413,   218,   708,   218,   868,   218,    58,
     218,   536,   588,   725,   601,   809,   741,   542,   543,   544,
     545,   217,   217,   548,   819,   550,   756,   552,   218,   523,
     524,   525,   526,   527,   460,   478,   479,    66,   835,   749,
     475,   734,   528,   705,   529,    69,   735,   562,   101,    72,
     750,   751,   102,   877,   172,   218,   532,    70,   173,  -468,
     174,    92,   565,   836,  -468,   706,    71,   478,   479,   103,
     104,   105,   106,  -475,   583,   576,   852,    73,  -475,  -482,
      83,    85,  -468,  -255,  -482,   217,   825,   606,  -255,   607,
      96,   217,   217,   217,   217,    98,  -475,   217,   853,   217,
      99,   217,  -482,   217,   377,   100,  -255,   378,    49,    50,
     379,    51,   616,   331,   447,   109,   161,   112,   114,   490,
     491,   217,   113,   778,   168,   608,   609,   498,   157,   568,
     501,   502,   503,   777,   158,   504,   735,   159,   739,   161,
     478,   479,   163,   167,   168,    60,   179,   223,   217,   488,
     489,   225,   238,   492,   234,   409,   495,   496,   497,   498,
     243,   500,   501,   502,   503,   273,   251,   504,   252,   506,
     507,   614,   255,   253,   254,   510,   511,   512,   256,   257,
     258,   259,   260,   261,   274,   275,   388,   528,   398,   529,
     229,   218,   218,   239,   218,   422,   430,   432,   433,   517,
     437,   518,   519,   520,   521,   522,   434,   435,   436,   526,
     718,   719,   441,   721,   464,   465,   466,   467,   716,   528,
      13,   529,   488,   489,   490,   491,   492,   535,   471,   495,
     496,   497,   498,   363,   500,   501,   502,   503,   472,   473,
     504,   530,   506,   507,   546,   559,   561,   476,   510,   511,
     512,   574,   581,   582,   862,   587,   615,   621,   626,   681,
     643,   478,   479,   682,   647,   676,   684,   742,   729,   733,
     732,   244,   517,   744,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   217,   217,   754,   217,   765,   766,
     770,   772,   528,   405,   529,   787,   789,   791,   799,   813,
     800,   161,   811,   262,   921,   218,   823,   814,   816,   820,
     821,   831,   832,   833,   834,   262,   218,   870,   218,   883,
     857,   860,   897,   863,   782,   902,   866,   263,   264,   872,
     890,   895,   904,   891,   923,   792,   906,   910,   927,   263,
     264,   912,   922,   488,   489,   490,   491,   610,   930,   925,
      94,   936,   931,   498,   178,   500,   501,   502,   503,   939,
     218,   504,    97,   506,   507,   898,    95,   116,   878,   844,
     748,   381,    21,   871,   533,   776,   794,   805,   417,   824,
     687,   558,   248,   111,   537,   265,   547,   911,   474,     0,
     604,   421,   538,   827,   817,     0,     0,   265,   217,   369,
     523,   524,   525,   526,   527,     0,     0,     0,     0,   217,
       0,   217,   383,   528,     0,   529,     0,     0,     0,     0,
       0,     0,   685,   267,     0,     0,     0,   268,     0,     0,
     269,   478,   479,   404,   266,   267,     0,     0,     0,   268,
       0,   850,   269,     0,     0,   270,     0,   418,     0,     0,
       0,   686,   218,   217,   272,   427,     0,   270,     0,   429,
       0,   431,     0,   691,     0,   218,   272,     0,     0,   439,
       0,   876,   869,   444,   445,   446,     0,     0,     0,   453,
     262,   874,     0,     0,   880,     0,     0,   457,   458,   459,
       0,   461,   462,   463,     0,     0,     0,     0,     0,     0,
       0,     0,   468,     0,   263,   264,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,   491,   492,     0,     0,
     495,   496,   497,   498,     0,   500,   501,   502,   503,     0,
       0,   504,   218,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   217,     0,   218,     0,     0,
       0,   926,     0,     0,     0,     0,     0,     0,   217,     0,
       0,     0,   265,     0,     0,     0,   935,   520,   521,   522,
     523,   524,   525,   526,   527,     0,     0,     0,     0,   564,
       0,     0,     0,   528,     0,   529,   570,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,   579,     0,   266,
     267,     0,     0,   913,   268,   585,     0,   269,     0,     0,
       0,     0,     0,   593,     0,   593,     0,     0,     0,     0,
       0,   924,   270,     0,   567,   217,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,     0,   627,   628,   629,   630,   631,   632,   633,
     634,   635,   636,     0,     0,   637,   638,   639,   640,   641,
     642,     0,   645,   646,     0,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   663,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,     0,   678,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   714,     0,     0,   645,   663,   262,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   457,   463,     0,     0,     0,     0,
     263,   264,     0,     0,     0,   463,     0,     0,   740,   453,
       0,     0,   743,     0,     0,     0,     0,     0,     0,   288,
     289,   290,   291,     0,     0,     0,     0,     0,     0,     0,
       0,   293,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,     0,   297,     0,     0,   299,   767,     0,   300,
       0,     0,   301,     0,   589,     0,   304,     0,   265,     0,
       0,   263,   264,     0,     0,   769,   186,   187,   188,     0,
     190,   191,   192,   193,   194,   308,   196,   197,   198,   199,
     200,   201,   202,   203,   204,     0,   206,   207,   208,     0,
     779,   211,   212,   213,   214,   266,   267,   309,   262,     0,
     268,     0,   692,   269,     0,     0,     0,     0,     0,   311,
     312,   796,     0,     0,     0,     0,     0,     0,   270,   265,
       0,     0,   263,   264,     0,     0,     0,   272,     0,   807,
     808,     0,    43,     0,     0,     0,   815,     0,   593,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   590,   323,
       0,   369,     0,     0,     0,   828,   266,   267,     0,   830,
       0,   268,     0,     0,   591,   325,   326,     0,   327,     0,
       0,   328,   329,     0,   841,     0,     0,     0,     0,   270,
     265,   592,     0,   331,   332,     0,   161,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   266,   267,     0,
       0,     0,   268,     0,   693,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,     0,     0,     0,     0,     0,     0,     0,     0,   272,
     262,     0,     0,     0,     0,   882,     0,   884,   885,   886,
     888,   889,     0,     0,   892,   893,     0,   894,     0,     0,
       0,     0,     0,     0,   263,   264,   900,     0,     0,     0,
     283,     0,     0,     0,     0,     3,     0,   284,   285,   286,
       0,   287,     0,   288,   289,   290,   291,     0,     0,     0,
       0,     0,   915,     0,   292,   293,   294,   295,     0,   920,
       0,     0,     0,     0,     0,     0,   296,   297,     0,   298,
     299,     0,     0,   300,   929,     9,   301,   302,     0,   303,
     304,     0,   265,   305,   306,     0,     0,     0,     0,   307,
     186,   187,   188,     0,   190,   191,   192,   193,   194,   308,
     196,   197,   198,   199,   200,   201,   202,   203,   204,     0,
     206,   207,   208,     0,     0,   211,   212,   213,   214,   266,
     267,   309,   310,     0,   268,     0,   696,   269,     0,     0,
       0,     0,     0,   311,   312,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   313,     0,
       0,   272,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,   314,   315,   316,   317,   318,   319,   320,
     321,   322,    44,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,   331,   332,   333,
     161,   334,   283,     0,     0,     0,     0,     3,     0,   284,
     285,   286,     0,   287,     0,   288,   289,   290,   291,     0,
       0,     0,     0,     0,     0,     0,   292,   293,   294,   295,
       0,     0,     0,     0,     0,     0,     0,     0,   296,   297,
       0,   298,   299,     0,     0,   300,     0,     9,   301,   302,
       0,   303,   304,     0,     0,   305,   306,     0,     0,     0,
       0,   307,   186,   187,   188,     0,   190,   191,   192,   193,
     194,   308,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,   206,   207,   208,     0,     0,   211,   212,   213,
     214,     0,     0,   309,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,    43,   262,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    44,   323,     0,     0,     0,     0,
       0,     0,     0,   263,   264,     0,     0,     0,     0,     0,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,     0,   288,   289,   290,   291,     0,   330,     0,   331,
     332,   333,   161,   826,   293,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,   297,     0,     0,   299,
       0,     0,   300,     0,     0,   301,     0,     0,     0,   304,
       0,   265,     0,     0,   263,   264,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   698,   269,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   270,   265,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,   262,     0,    43,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   590,   323,     0,     0,     0,     0,   263,   264,   266,
     267,     0,     0,     0,   268,     0,     0,   591,   325,   326,
       0,   327,     0,     0,   328,   329,   288,   289,   290,   291,
       0,     0,   270,     0,   602,     0,   331,   332,   293,   161,
       0,   272,     0,     0,   399,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,     0,   265,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,   266,   267,   309,     0,     0,   268,     0,   700,
     269,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,    43,
     262,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    44,   323,     0,     0,     0,
       0,     0,     0,     0,   263,   264,     0,     0,   400,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     288,   289,   290,   291,     0,     0,   401,     0,   330,     0,
     331,   332,   293,   161,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,   297,     0,     0,   299,     0,     0,
     300,     0,     0,   301,     0,     0,     0,   304,     0,     0,
       0,   412,   265,     0,     0,     0,     0,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   308,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,     0,     0,   309,   707,
     267,     0,     0,     0,   268,     0,     0,   269,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   270,     0,     0,   313,     0,     0,     0,     0,
       0,   272,     0,    43,     0,     0,   262,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    44,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,   264,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,     0,     0,   288,   289,   290,   291,
       0,     0,   330,     0,   331,   332,   424,   161,   293,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,     0,     0,     0,     0,   265,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,     0,     0,   309,   266,   267,     0,     0,     0,
     268,     0,   722,   269,     0,     0,   311,   312,     0,   262,
       0,     0,     0,     0,     0,   442,     0,     0,   270,     0,
       0,   313,     0,     0,     0,     0,     0,   272,     0,    43,
       0,     0,     0,   263,   264,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    44,   323,     0,     0,     0,
       0,     0,   288,   289,   290,   291,     0,     0,     0,     0,
       0,   324,   325,   326,   293,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   297,     0,   330,   299,
     331,   332,   300,   161,     0,   301,     0,     0,     0,   304,
       0,   265,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   802,   269,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,   262,     0,    43,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    44,   323,     0,     0,     0,     0,   263,   264,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,   288,   289,   290,   291,
     712,     0,   617,     0,   330,   618,   331,   332,   293,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,     0,   265,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,   266,   267,   309,     0,     0,   268,     0,   803,
     269,     0,     0,     0,     0,     0,   311,   312,     0,   262,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,    43,
       0,     0,     0,   263,   264,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    44,   323,     0,     0,     0,
       0,     0,   288,   289,   290,   291,     0,     0,     0,   713,
       0,   324,   325,   326,   293,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   297,     0,   330,   299,
     331,   332,   300,   161,     0,   301,     0,     0,     0,   304,
       0,   265,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   804,   269,     0,     0,     0,
       0,     0,   311,   312,     0,   262,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,    43,     0,     0,     0,   263,
     264,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    44,   323,     0,     0,     0,     0,     0,   288,   289,
     290,   291,     0,     0,     0,     0,     0,   324,   325,   326,
     293,   327,     0,     0,   328,   329,     0,     0,     0,     0,
       0,     0,   297,   403,   330,   299,   331,   332,   300,   161,
       0,   301,     0,     0,     0,   304,     0,   265,   438,     0,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,   266,   267,   309,     0,     0,   268,
       0,   846,   269,     0,     0,     0,     0,     0,   311,   312,
       0,   262,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,    43,     0,     0,     0,   263,   264,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    44,   323,     0,
       0,     0,     0,     0,   288,   289,   290,   291,     0,     0,
       0,     0,     0,   324,   325,   326,   293,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   297,     0,
     330,   299,   331,   332,   300,   161,     0,   301,     0,     0,
       0,   304,     0,   265,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
     266,   267,   309,     0,     0,   268,     0,   867,   269,     0,
       0,     0,     0,     0,   311,   312,     0,   262,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   313,
       0,     0,   272,     0,     0,     0,     0,    43,     0,     0,
       0,   263,   264,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    44,   323,     0,     0,     0,     0,     0,
     288,   289,   290,   291,     0,     0,     0,     0,     0,   324,
     325,   326,   293,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   297,     0,   330,   299,   331,   332,
     300,   161,     0,   301,     0,     0,     0,   304,     0,   265,
       0,     0,     0,     0,     0,     0,     0,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   308,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,   266,   267,   309,     0,
       0,   268,     0,   901,   269,     0,     0,     0,     0,     0,
     311,   312,     0,   262,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,   644,     0,     0,   272,     0,
       0,     0,     0,    43,     0,     0,     0,   263,   264,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    44,
     323,     0,     0,     0,     0,     0,   288,   289,   290,   291,
       0,     0,     0,     0,     0,   324,   325,   326,   293,   327,
       0,     0,   328,   329,     0,     0,     0,     0,     0,     0,
     297,     0,   330,   299,   331,   332,   300,   161,     0,   301,
       0,     0,     0,   304,     0,   265,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,   266,   267,   309,     0,     0,   268,     0,   903,
     269,     0,     0,     0,     0,     0,   311,   312,     0,   262,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   662,     0,     0,   272,     0,     0,     0,     0,    43,
       0,     0,     0,   263,   264,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    44,   323,     0,     0,     0,
       0,     0,   288,   289,   290,   291,     0,     0,     0,   664,
       0,   324,   325,   326,   293,   327,     0,     0,   328,   329,
       0,     0,     0,     0,     0,     0,   297,     0,   330,   299,
     331,   332,   300,   161,     0,   301,     0,     0,     0,   304,
       0,   265,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,   932,   269,     0,     0,     0,
       0,     0,   311,   312,     0,   262,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,    43,     0,     0,     0,   263,
     264,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    44,   323,     0,     0,     0,     0,     0,   288,   289,
     290,   291,     0,     0,     0,     0,     0,   324,   325,   326,
     293,   327,     0,     0,   328,   329,     0,     0,     0,     0,
       0,     0,   297,     0,   330,   299,   331,   332,   300,   161,
       0,   301,     0,     0,     0,   304,     0,   265,     0,     0,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,   266,   267,   309,     0,     0,   268,
       0,   937,   269,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,     0,     0,   272,     0,     0,   262,
       0,    43,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    44,   323,     0,
       0,     0,     0,   263,   264,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   326,     0,   327,     0,     0,
     328,   329,   288,   289,   290,   291,     0,     0,     0,     0,
     330,   679,   331,   332,   293,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   297,     0,     0,   299,
       0,     0,   300,     0,     0,   301,     0,     0,     0,   304,
       0,   265,     0,     0,     0,     0,     0,     0,     0,   186,
     187,   188,     0,   190,   191,   192,   193,   194,   308,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   206,
     207,   208,     0,     0,   211,   212,   213,   214,   266,   267,
     309,     0,     0,   268,     0,     0,   269,     0,     0,     0,
       0,     0,   311,   312,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    44,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,   288,   289,   290,   291,
       0,     0,     0,     0,   330,   710,   331,   332,   293,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,     0,     0,   299,     0,     0,   300,     0,     0,   301,
       0,     0,     0,   304,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   187,   188,     0,   190,   191,   192,
     193,   194,   308,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   206,   207,   208,     0,     0,   211,   212,
     213,   214,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    44,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     288,   289,   290,   291,     0,     0,     0,     0,   330,   728,
     331,   332,   293,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   297,     0,     0,   299,     0,     0,
     300,     0,     0,   301,     0,     0,     0,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   308,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    44,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   288,   289,   290,   291,     0,     0,
       0,     0,   330,   737,   331,   332,   293,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,   299,     0,     0,   300,     0,     0,   301,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    44,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,   288,   289,
     290,   291,   881,     0,     0,     0,   330,   839,   331,   332,
     293,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,     0,   299,     0,     0,   300,     0,
       0,   301,     0,     0,     0,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    44,   323,     0,
       0,     0,     0,     0,   288,   289,   290,   291,     0,     0,
       0,     0,     0,   324,   325,   326,   293,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   297,     0,
     330,   299,   331,   332,   300,   161,     0,   301,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    44,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     325,   326,     0,   327,     0,     0,   328,   329,   288,   289,
     290,   291,     0,     0,     0,     0,   330,   887,   331,   332,
     293,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,     0,   299,     0,     0,   300,     0,
       0,   301,     0,     0,     0,   304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   187,   188,     0,   190,
     191,   192,   193,   194,   308,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   206,   207,   208,     0,     0,
     211,   212,   213,   214,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   312,
       0,     0,     0,     0,     0,     0,     0,   899,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,     0,     0,     0,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    44,   323,     0,
       0,     0,     0,     0,   288,   289,   290,   291,   919,     0,
       0,     0,     0,   324,   325,   326,   293,   327,     0,     0,
     328,   329,     0,     0,     0,     0,     0,     0,   297,     0,
     330,   299,   331,   332,   300,   161,     0,   301,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,   188,     0,   190,   191,   192,   193,   194,
     308,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   206,   207,   208,     0,     0,   211,   212,   213,   214,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    44,   323,     0,     0,     0,     0,     0,
     288,   289,   290,   291,     0,     0,     0,     0,     0,   324,
     325,   326,   293,   327,     0,     0,   328,   329,     0,     0,
       0,     0,     0,     0,   297,     0,   330,   299,   331,   332,
     300,   161,     0,   301,     0,     0,     0,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   187,   188,
       0,   190,   191,   192,   193,   194,   308,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   206,   207,   208,
       0,     0,   211,   212,   213,   214,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   312,     0,     0,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    44,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,     0,     0,     0,     0,     0,     0,
       0,     0,   330,     0,   331,   332,     0,   161,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   571,   500,   501,
     502,   503,   478,   479,   504,   505,   506,   507,   508,   509,
       0,     0,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   572,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,     0,
       0,     0,     0,     0,   573,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   571,   500,   501,   502,   503,
     478,   479,   504,   505,   506,   507,   508,   509,     0,     0,
     510,   511,   512,   513,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   572,   517,     0,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,     0,     0,     0,     0,
       0,     0,     0,     0,   528,     0,   529,     0,     0,     0,
       0,     0,   586,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   571,   500,   501,   502,   503,   478,   479,
     504,   505,   506,   507,   508,   509,     0,     0,   510,   511,
     512,   513,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   572,   517,     0,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,     0,     0,     0,     0,     0,
     715,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   571,   500,   501,   502,   503,   478,   479,   504,   505,
     506,   507,   508,   509,     0,     0,   510,   511,   512,   513,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   572,
     517,     0,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   529,     0,     0,     0,     0,     0,   723,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   571,
     500,   501,   502,   503,   478,   479,   504,   505,   506,   507,
     508,   509,     0,     0,   510,   511,   512,   513,   514,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   572,   517,     0,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,     0,     0,     0,     0,     0,   780,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   571,   500,   501,
     502,   503,   478,   479,   504,   505,   506,   507,   508,   509,
       0,     0,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   572,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,     0,
       0,     0,     0,     0,   845,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   478,   479,     0,     0,     0,
       0,     0,     0,     0,   488,   489,   490,   491,   492,     0,
       0,   495,   496,   497,   498,     0,   500,   501,   502,   503,
       0,     0,   504,     0,   506,   507,     0,     0,     0,     0,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   517,     0,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,     0,     0,   478,   479,
       0,     0,     0,     0,   528,     0,   529,   488,   489,   490,
     491,   492,   541,     0,   495,   496,   497,   498,     0,   500,
     501,   502,   503,     0,     0,   504,     0,   506,   507,     0,
       0,     0,     0,   510,   511,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   517,     0,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,     0,
       0,     0,     0,     0,   478,   479,     0,   528,     0,   529,
     488,   489,   490,   491,   492,   908,     0,   495,   496,   497,
     498,     0,   500,   501,   502,   503,     0,     0,   504,     0,
     506,   507,     0,     0,     0,     0,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   529,   478,   479,   612,   488,   489,   490,   491,
     492,     0,     0,   495,   496,   497,   498,     0,   500,   501,
     502,   503,     0,     0,   504,     0,   506,   507,     0,     0,
       0,     0,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   478,
     479,   717,     0,     0,     0,   488,   489,   490,   491,   492,
       0,     0,   495,   496,   497,   498,     0,   500,   501,   502,
     503,     0,     0,   504,     0,   506,   507,     0,     0,     0,
       0,   510,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   517,     0,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   478,   479,
     720,   488,   489,   490,   491,   492,     0,     0,   495,   496,
     497,   498,     0,   500,   501,   502,   503,     0,     0,   504,
       0,   506,   507,     0,     0,     0,     0,   510,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,     0,     0,     0,     0,     0,     0,     0,
       0,   528,     0,   529,   478,   479,   801,     0,     0,     0,
     488,   489,   490,   491,   492,     0,     0,   495,   496,   497,
     498,     0,   500,   501,   502,   503,     0,     0,   504,     0,
     506,   507,     0,     0,     0,     0,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   529,   478,   479,   905,   488,   489,   490,   491,
     492,     0,     0,   495,   496,   497,   498,     0,   500,   501,
     502,   503,     0,     0,   504,     0,   506,   507,     0,     0,
       0,     0,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,   478,
     479,   907,     0,     0,     0,   488,   489,   490,   491,   492,
       0,     0,   495,   496,   497,   498,     0,   500,   501,   502,
     503,     0,     0,   504,     0,   506,   507,     0,     0,     0,
       0,   510,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   517,     0,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,     0,     0,     0,
       0,     0,     0,   478,   479,   528,     0,   529,     0,     0,
     928,   488,   489,   490,   491,   492,     0,     0,   495,   496,
     497,   498,     0,   500,   501,   502,   503,     0,     0,   504,
       0,   506,   507,     0,     0,     0,     0,   510,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,     0,     0,     0,     0,   478,   479,     0,
       0,   528,     0,   529,   560,   488,   489,   490,   491,   492,
       0,     0,   495,   496,   497,   498,     0,   500,   501,   502,
     503,     0,     0,   504,     0,   506,   507,     0,     0,     0,
       0,   510,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   517,     0,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   768,   488,
     489,   490,   491,   492,   478,   479,   495,   496,   497,   498,
       0,   500,   501,   502,   503,     0,     0,   504,     0,   506,
     507,     0,     0,     0,     0,   510,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   529,   773,     0,     0,     0,     0,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   571,   500,   501,
     502,   503,  -227,     0,   504,   505,   506,   507,   508,   509,
     478,   479,   510,   511,   512,   513,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   572,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   478,   479,
     504,   505,   506,   507,   508,   509,     0,     0,   510,   511,
     512,   513,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,     0,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   478,   479,     0,     0,     0,     0,     0,     0,     0,
     488,   489,   490,   491,   492,     0,     0,   495,   496,   497,
     498,     0,   500,   501,   502,   503,     0,     0,   504,     0,
     506,   507,     0,     0,   605,     0,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,     0,     0,   478,   479,     0,     0,     0,     0,
     528,     0,   529,   488,   489,   490,   491,   492,     0,     0,
     495,   496,   497,   498,     0,   500,   501,   502,   503,     0,
       0,   504,     0,   506,   507,     0,     0,     0,     0,   510,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   517,   771,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,     0,     0,   478,   479,     0,
       0,     0,     0,   528,     0,   529,   488,   489,   490,   491,
     492,     0,     0,   495,   496,   497,   498,     0,   500,   501,
     502,   503,     0,     0,   504,     0,   506,   507,     0,     0,
       0,     0,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   822,     0,     0,     0,   517,     0,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,     0,     0,
     478,   479,     0,     0,     0,     0,   528,     0,   529,   488,
     489,   490,   491,   492,     0,     0,   495,   496,   497,   498,
       0,   500,   501,   502,   503,     0,     0,   504,     0,   506,
     507,     0,     0,     0,     0,   510,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,     0,     0,   478,   479,     0,     0,     0,     0,   528,
       0,   529,   488,   489,   490,   491,   492,     0,     0,   495,
     496,   497,   498,     0,   500,   501,   502,   503,     0,     0,
     504,     0,   506,   507,     0,     0,     0,     0,   510,     0,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,     0,     0,   478,   479,     0,     0,
       0,     0,   528,     0,   529,   488,   489,   490,   491,   492,
       0,     0,   495,   496,   497,   498,     0,   500,   501,   502,
     503,   478,   479,   504,     0,   506,   507,     0,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,     0,     0,     0,
       0,     0,     0,     0,     0,   528,     0,   529,   488,   489,
     490,   491,   492,     0,     0,   495,   496,   497,   498,     0,
     500,   501,   502,   503,   478,   479,   504,     0,   506,   507,
       0,     0,     0,   488,   489,   490,   491,   492,     0,     0,
     495,   496,   497,   498,     0,   500,   501,   502,   503,   478,
     479,   504,     0,   506,   507,     0,     0,     0,     0,     0,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,     0,     0,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,   526,   527,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,   529,   488,   489,   490,   491,
     492,     0,     0,   495,   496,   497,   498,     0,   500,   501,
     502,   503,     0,     0,   504,     0,   506,   507,     0,     0,
       0,   488,   489,   490,   491,   492,     0,     0,   495,     0,
       0,   498,     0,   500,   501,   502,   503,     0,     0,   504,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   521,   522,   523,   524,   525,   526,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   528,     0,   529,     0,
       0,   449,     0,     0,     0,     0,   521,   522,   523,   524,
     525,   526,   527,   182,     0,     0,     0,     0,     0,   183,
       0,   528,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,     0,     0,     0,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   182,     0,     0,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   185,    44,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   182,
       0,     0,     0,     0,     0,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,    44,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,     0,     0,     0,     0,     0,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,   184,     0,     0,     0,     0,     0,   314,
     315,   316,     0,     0,     0,     0,     0,   185,    44,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,    44,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,     0,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,    43,     0,
     139,   140,   141,   142,   143,   144,     0,     0,     0,     0,
       0,     0,     0,     0,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,     0,     0,   155,   156
};

static const yytype_int16 yycheck[] =
{
      13,   236,   279,   109,   275,    81,   316,   277,   292,   166,
     391,   549,   393,   551,   395,   553,   530,   313,     7,   452,
     595,    73,   109,    19,    32,    32,   310,     0,   603,    20,
      21,    19,   126,     6,    14,    15,    46,    34,    49,   617,
      18,   134,    19,    49,   143,   113,    33,   118,    56,    57,
     181,   143,   566,   567,   177,   113,    29,   180,    31,   143,
      33,    74,    35,   121,   150,   143,    39,   113,   115,   116,
     127,   143,    21,    46,   143,   121,   143,   143,    32,    52,
     143,   143,   150,   151,   177,   153,   143,   181,   156,   153,
     182,   177,   176,   151,    20,    21,   109,    70,   156,   143,
     157,   150,    56,    57,    53,   151,   114,   104,   105,   180,
     182,   150,   150,   104,   105,   182,   182,   181,    91,   182,
     182,   112,   127,   114,   115,   116,   117,   174,   234,   120,
     287,   180,   416,   143,   150,    42,    43,   143,   143,   423,
     127,   180,   180,   151,   152,   134,   143,   234,   156,   156,
     163,   159,   730,   728,   150,   150,   143,   137,   442,   175,
     114,   141,   150,   143,   144,   143,   174,   174,   274,   180,
      20,    21,   180,   150,   137,   183,   183,   229,   104,   105,
     476,   172,   177,   174,   290,   174,   112,   274,   114,   115,
     116,   117,   113,   182,   120,   151,   176,   151,   152,   356,
     121,   174,   156,   290,   158,   159,   127,   180,   314,   315,
     724,    14,    15,   162,   112,   150,   150,   115,   116,   295,
     174,   234,   736,   172,   180,   113,   764,   314,   315,   183,
     151,   307,   150,   121,   809,   161,   162,   163,   158,   145,
     146,   622,   177,   177,   819,   113,   172,   127,   174,   158,
     150,    58,   766,   121,   104,   105,   540,   150,   150,   177,
     180,   274,   112,   151,   114,   115,   116,   117,   151,   153,
     120,   180,   378,   153,   172,   175,   174,   290,   384,   385,
     386,   387,   175,   151,   390,   562,   392,   825,   394,   150,
     396,   378,   449,   589,   451,   728,   606,   384,   385,   386,
     387,   314,   315,   390,   737,   392,   150,   394,   414,   159,
     160,   161,   162,   163,   327,    20,    21,   143,   150,    11,
     360,   175,   172,   158,   174,   143,   180,   414,    65,   127,
      22,    23,    69,   177,   137,   441,   376,   143,   141,   153,
     143,   144,   418,   175,   158,   180,   143,    20,    21,    86,
      87,    88,    89,   153,   441,   431,   158,   176,   158,   153,
     143,   181,   176,   153,   158,   378,   747,   180,   158,   182,
     137,   384,   385,   386,   387,   143,   176,   390,   180,   392,
     143,   394,   176,   396,   150,   143,   176,   153,     4,     5,
     156,     7,   468,   178,   179,   151,   181,   181,   143,   104,
     105,   414,   181,   687,   145,   146,   147,   112,    34,   422,
     115,   116,   117,   683,    34,   120,   180,   176,   182,   181,
      20,    21,   153,     1,   145,    53,   181,   143,   441,   102,
     103,   143,   181,   106,   153,   706,   109,   110,   111,   112,
     180,   114,   115,   116,   117,   180,   157,   120,   157,   122,
     123,   464,   176,   157,   157,   128,   129,   130,   157,   157,
     157,   157,   157,   157,   153,   180,   143,   172,    32,   174,
     134,   577,   578,   143,   580,   176,   176,   157,   157,   152,
     137,   154,   155,   156,   157,   158,   176,   180,   157,   162,
     577,   578,   157,   580,   179,    36,   180,   180,   574,   172,
     174,   174,   102,   103,   104,   105,   106,   143,   180,   109,
     110,   111,   112,   748,   114,   115,   116,   117,   180,   180,
     120,   176,   122,   123,   177,   175,   175,   118,   128,   129,
     130,    13,   153,   137,   811,   179,   181,   134,   143,     7,
     143,    20,    21,    42,   143,   143,   180,   182,   150,   150,
     175,   145,   152,   145,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   577,   578,   143,   580,   157,   176,
     143,     1,   172,   143,   174,   158,   158,   158,   157,   175,
     143,   181,   143,    32,    49,   691,   145,   175,   175,   182,
     175,   143,   143,   143,   143,    32,   702,   177,   704,   143,
     176,   175,   158,   175,   691,   180,   175,    56,    57,   175,
     175,   182,   157,   175,   182,   702,   158,   175,   157,    56,
      57,   175,   175,   102,   103,   104,   105,   456,   175,   177,
      55,   177,   175,   112,    98,   114,   115,   116,   117,   177,
     746,   120,    58,   122,   123,   869,    56,    74,   843,   776,
     615,   246,     1,   835,   376,   681,   704,   722,   281,   746,
     536,   397,   179,    68,   113,   114,   389,   894,   356,    -1,
     452,   290,   121,   749,   735,    -1,    -1,   114,   691,   236,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,   702,
      -1,   704,   249,   172,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,    -1,    -1,    -1,   156,    -1,    -1,
     159,    20,    21,   270,   151,   152,    -1,    -1,    -1,   156,
      -1,   797,   159,    -1,    -1,   174,    -1,   284,    -1,    -1,
      -1,   180,   838,   746,   183,   292,    -1,   174,    -1,   296,
      -1,   298,    -1,   180,    -1,   851,   183,    -1,    -1,   306,
      -1,   838,   828,   310,   311,   312,    -1,    -1,    -1,   316,
      32,   837,    -1,    -1,   851,    -1,    -1,   324,   325,   326,
      -1,   328,   329,   330,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   339,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,   918,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   838,    -1,   933,    -1,    -1,
      -1,   918,    -1,    -1,    -1,    -1,    -1,    -1,   851,    -1,
      -1,    -1,   114,    -1,    -1,    -1,   933,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,   416,
      -1,    -1,    -1,   172,    -1,   174,   423,    -1,    -1,    -1,
      -1,    -1,    -1,   430,    -1,    -1,    -1,   434,    -1,   151,
     152,    -1,    -1,   896,   156,   442,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   450,    -1,   452,    -1,    -1,    -1,    -1,
      -1,   914,   174,    -1,   176,   918,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     933,    -1,    -1,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,    -1,    -1,   492,   493,   494,   495,   496,
     497,    -1,   499,   500,    -1,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,    -1,   529,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   540,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   568,    -1,    -1,   571,   572,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   591,   592,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,   602,    -1,    -1,   605,   606,
      -1,    -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,   644,    -1,    44,
      -1,    -1,    47,    -1,    49,    -1,    51,    -1,   114,    -1,
      -1,    56,    57,    -1,    -1,   662,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
     687,    86,    87,    88,    89,   151,   152,    92,    32,    -1,
     156,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   104,
     105,   708,    -1,    -1,    -1,    -1,    -1,    -1,   174,   114,
      -1,    -1,    56,    57,    -1,    -1,    -1,   183,    -1,   726,
     727,    -1,   127,    -1,    -1,    -1,   733,    -1,   735,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   748,    -1,    -1,    -1,   752,   151,   152,    -1,   756,
      -1,   156,    -1,    -1,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,    -1,   771,    -1,    -1,    -1,    -1,   174,
     114,   176,    -1,   178,   179,    -1,   181,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,
      -1,    -1,   156,    -1,   158,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      32,    -1,    -1,    -1,    -1,   852,    -1,   854,   855,   856,
     857,   858,    -1,    -1,   861,   862,    -1,   864,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,   873,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,   899,    -1,    25,    26,    27,    28,    -1,   906,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      41,    -1,    -1,    44,   921,    46,    47,    48,    -1,    50,
      51,    -1,   114,    54,    55,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    83,    -1,    -1,    86,    87,    88,    89,   151,
     152,    92,    93,    -1,   156,    -1,   158,   159,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,   183,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,    -1,   163,    -1,    -1,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,   178,   179,   180,
     181,   182,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    41,    -1,    -1,    44,    -1,    46,    47,    48,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    -1,    81,    82,    83,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    32,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,   176,    -1,   178,
     179,   180,   181,   182,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,   114,    -1,    -1,    56,    57,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   151,   152,
      92,    -1,    -1,   156,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    32,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    56,    57,   151,
     152,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,    14,    15,    16,    17,
      -1,    -1,   174,    -1,   176,    -1,   178,   179,    26,   181,
      -1,   183,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   151,   152,    92,    -1,    -1,   156,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,   127,
      32,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
      14,    15,    16,    17,    -1,    -1,   174,    -1,   176,    -1,
     178,   179,    26,   181,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,   151,
     152,    -1,    -1,    -1,   156,    -1,    -1,   159,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,   174,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   127,    -1,    -1,    32,    -1,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    -1,    -1,    -1,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,   176,    -1,   178,   179,   180,   181,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,   151,   152,    -1,    -1,    -1,
     156,    -1,   158,   159,    -1,    -1,   104,   105,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,   174,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,   183,    -1,   127,
      -1,    -1,    -1,    56,    57,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    26,   163,    -1,    -1,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   176,    41,
     178,   179,    44,   181,    -1,    47,    -1,    -1,    -1,    51,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   151,   152,
      92,    -1,    -1,   156,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    32,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,    14,    15,    16,    17,
      18,    -1,   174,    -1,   176,   177,   178,   179,    26,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   151,   152,    92,    -1,    -1,   156,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    56,    57,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,   157,
      -1,   159,   160,   161,    26,   163,    -1,    -1,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   176,    41,
     178,   179,    44,   181,    -1,    47,    -1,    -1,    -1,    51,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   151,   152,
      92,    -1,    -1,   156,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    56,
      57,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      26,   163,    -1,    -1,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    38,   175,   176,    41,   178,   179,    44,   181,
      -1,    47,    -1,    -1,    -1,    51,    -1,   114,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,   151,   152,    92,    -1,    -1,   156,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    56,    57,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    26,   163,    -1,    -1,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     176,    41,   178,   179,    44,   181,    -1,    47,    -1,    -1,
      -1,    51,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
     151,   152,    92,    -1,    -1,   156,    -1,   158,   159,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,   183,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    56,    57,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    26,   163,    -1,    -1,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   176,    41,   178,   179,
      44,   181,    -1,    47,    -1,    -1,    -1,    51,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,   151,   152,    92,    -1,
      -1,   156,    -1,   158,   159,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    32,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,   183,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    56,    57,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    26,   163,
      -1,    -1,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,   176,    41,   178,   179,    44,   181,    -1,    47,
      -1,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,   151,   152,    92,    -1,    -1,   156,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,   183,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    56,    57,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      -1,   159,   160,   161,    26,   163,    -1,    -1,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,   176,    41,
     178,   179,    44,   181,    -1,    47,    -1,    -1,    -1,    51,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   151,   152,
      92,    -1,    -1,   156,    -1,   158,   159,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    56,
      57,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      26,   163,    -1,    -1,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,   176,    41,   178,   179,    44,   181,
      -1,    47,    -1,    -1,    -1,    51,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,   151,   152,    92,    -1,    -1,   156,
      -1,   158,   159,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    32,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
     176,   177,   178,   179,    26,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    83,    -1,    -1,    86,    87,    88,    89,   151,   152,
      92,    -1,    -1,   156,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   176,   177,   178,   179,    26,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    81,    82,    83,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,   176,   177,
     178,   179,    26,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,   176,   177,   178,   179,    26,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   176,   177,   178,   179,
      26,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    26,   163,    -1,    -1,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     176,    41,   178,   179,    44,   181,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   176,   177,   178,   179,
      26,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,   159,   160,   161,    26,   163,    -1,    -1,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
     176,    41,   178,   179,    44,   181,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    26,   163,    -1,    -1,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,   176,    41,   178,   179,
      44,   181,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   178,   179,    -1,   181,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   172,    -1,   174,   102,   103,   104,
     105,   106,   180,    -1,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,
      -1,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,   172,    -1,   174,
     102,   103,   104,   105,   106,   180,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,    20,    21,   177,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    20,
      21,   177,    -1,    -1,    -1,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    20,    21,
     177,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    -1,    -1,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,   174,    20,    21,   177,    -1,    -1,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,    20,    21,   177,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    20,
      21,   177,    -1,    -1,    -1,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   172,    -1,   174,    -1,    -1,
     177,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    -1,    -1,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   172,    -1,   174,   175,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,   102,
     103,   104,   105,   106,    20,    21,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
     123,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,   174,   175,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
      20,    21,   128,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     172,    -1,   174,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   172,    -1,   174,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   172,    -1,   174,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
     123,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   172,
      -1,   174,   102,   103,   104,   105,   106,    -1,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   172,    -1,   174,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    20,    21,   120,    -1,   122,   123,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   102,   103,
     104,   105,   106,    -1,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    20,    21,   120,    -1,   122,   123,
      -1,    -1,    -1,   102,   103,   104,   105,   106,    -1,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    20,
      21,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,    -1,    -1,    -1,    -1,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,   102,   103,   104,   105,
     106,    -1,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,   143,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,   143,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    45,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    59,   143,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,   143,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   127,    -1,
     128,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   185,     0,     6,    29,    31,    33,    35,    39,    46,
      52,    70,    91,   174,   180,   186,   194,   195,   199,   219,
     223,   238,   295,   300,   301,   305,   341,   343,   143,   214,
     215,   137,   200,   201,   143,   196,   197,   143,   181,   143,
     344,   342,    33,   127,   143,   216,   217,   218,   230,     4,
       5,     7,   304,    58,   294,   151,   150,   153,   150,    21,
      53,   162,   172,   198,   153,   181,   143,   298,   299,   143,
     143,   143,   127,   176,   150,   175,    34,   104,   105,   143,
     222,   224,   225,   143,   303,   181,   296,    14,    15,   137,
     141,   143,   144,   176,   188,   214,   137,   201,   143,   143,
     143,    65,    69,    86,    87,    88,    89,   309,   297,   151,
     182,   298,   181,   181,   143,   215,   217,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   122,   123,   124,   125,   128,
     129,   130,   131,   132,   133,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   166,   167,    34,    34,   176,
     220,   181,   226,   153,   302,   306,   293,     1,   145,   187,
      14,    15,   137,   141,   143,   188,   212,   213,   198,   181,
     143,   182,    24,    30,    45,    59,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   230,   308,   310,
     311,   313,   316,   143,   312,   143,   286,   287,   177,   134,
     177,   277,   282,   283,   153,   221,   228,   230,   181,   143,
     182,   292,   307,   180,   145,   146,   150,   177,   297,   151,
     180,   157,   157,   157,   157,   176,   157,   157,   157,   157,
     157,   157,    32,    56,    57,   114,   151,   152,   156,   159,
     174,   180,   183,   180,   153,   180,   215,    46,   289,   307,
     177,   180,   316,     1,     8,     9,    10,    12,    14,    15,
      16,    17,    25,    26,    27,    28,    37,    38,    40,    41,
      44,    47,    48,    50,    51,    54,    55,    60,    70,    92,
      93,   104,   105,   119,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   144,   159,   160,   161,   163,   166,   167,
     176,   178,   179,   180,   182,   190,   192,   202,   203,   206,
     207,   208,   209,   210,   211,   226,   227,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   247,
     256,   260,   261,   262,   263,   264,   265,   266,   271,   274,
     308,   345,   349,   354,   356,   358,   280,   150,   153,   156,
     291,   212,   182,   274,   319,   321,   323,   317,   143,   314,
     325,   327,   329,   331,   333,   335,   337,   339,    32,    32,
     156,   174,   183,   175,   274,   143,   182,   316,   182,   286,
     175,   289,   113,   151,   153,   156,   288,   282,   274,   307,
     316,   345,   176,   113,   180,   229,   262,   274,   226,   274,
     176,   274,   157,   157,   176,   180,   157,   137,    54,   274,
     226,   157,   113,   229,   274,   274,   274,   179,   260,    12,
     316,    12,   316,   274,   351,   355,   189,   274,   274,   274,
     230,   274,   274,   274,   179,    36,   180,   180,   274,   143,
     176,   180,   180,   180,   292,   219,   118,   180,    20,    21,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   120,   121,   122,   123,   124,   125,
     128,   129,   130,   131,   132,   133,   151,   152,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   172,   174,
     176,   182,   219,   277,   279,   143,   316,   113,   121,   151,
     290,   180,   316,   316,   316,   316,   177,   312,   316,   220,
     316,   220,   316,   220,   143,   284,   285,   316,   287,   175,
     175,   175,   316,   229,   274,   226,    19,   176,   230,   229,
     274,   113,   151,   180,    13,   274,   226,   241,   243,   274,
     245,   153,   137,   316,   229,   274,   180,   179,   307,    49,
     143,   159,   176,   274,   346,   347,   348,   350,   351,   352,
     353,   307,   176,   347,   353,   126,   180,   182,   146,   147,
     187,   193,   177,   157,   230,   181,   226,   174,   177,   254,
     274,   134,   259,   260,    18,   143,   143,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   143,   119,   274,   274,   143,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   119,   274,    21,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   143,   272,   274,   177,
     254,     7,    42,   278,   180,   151,   180,   290,   229,   274,
     158,   180,   158,   158,   158,   180,   158,   221,   158,   221,
     158,   221,   153,   158,   180,   158,   180,   151,   288,   254,
     177,   254,    18,   157,   274,   180,   226,   177,   316,   316,
     177,   316,   158,   180,    19,   260,   121,   288,   177,   150,
     180,   348,   175,   150,   175,   180,    19,   177,   348,   182,
     274,   351,   182,   274,   145,   191,   267,   269,   228,    11,
      22,    23,   204,   205,   143,   346,   150,   177,   113,   121,
     151,   156,   257,   258,   220,   157,   176,   274,   175,   274,
     143,   153,     1,   175,   177,    43,   278,   289,   229,   274,
     180,   320,   316,   324,   318,   315,   326,   158,   330,   158,
     334,   158,   316,   338,   284,   340,   274,   180,   177,   157,
     143,   177,   158,   158,   158,   259,   254,   274,   274,   347,
     348,   143,   346,   175,   175,   274,   175,   352,   254,   347,
     182,   175,   148,   145,   316,   220,   182,   226,   274,   175,
     274,   143,   143,   143,   143,   150,   175,   221,   275,   177,
     254,   274,   273,   281,   225,   180,   158,   328,   332,   336,
     226,   248,   158,   180,   242,   244,   246,   176,   180,   348,
     175,   121,   288,   175,   180,   348,   175,   158,   221,   226,
     177,   257,   175,   126,   226,   255,   316,   177,   224,   322,
     316,    18,   274,   143,   274,   274,   274,   177,   274,   274,
     175,   175,   274,   274,   274,   182,   268,   158,   205,   113,
     274,   158,   180,   158,   157,   177,   158,   177,   180,   357,
     175,   357,   175,   230,   270,   274,   276,   249,   250,    18,
     274,    49,   175,   182,   230,   177,   316,   157,   177,   274,
     175,   175,   158,   252,   251,   316,   177,   158,   253,   177
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   184,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   186,   187,   187,   188,   189,
     189,   189,   190,   191,   191,   193,   192,   194,   195,   196,
     196,   196,   197,   197,   198,   198,   199,   200,   200,   201,
     201,   202,   203,   203,   204,   204,   205,   205,   205,   206,
     206,   207,   208,   209,   210,   211,   212,   212,   212,   212,
     212,   212,   213,   213,   214,   214,   214,   214,   214,   214,
     214,   214,   215,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   220,   220,   220,   221,   221,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     223,   224,   225,   226,   226,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   228,   228,   228,   229,   229,   230,
     230,   230,   231,   232,   232,   232,   232,   233,   234,   235,
     235,   235,   235,   235,   236,   236,   236,   236,   237,   238,
     238,   239,   241,   242,   240,   243,   244,   240,   245,   246,
     240,   247,   247,   247,   248,   249,   247,   250,   251,   247,
     252,   253,   247,   254,   254,   255,   255,   255,   256,   256,
     256,   257,   257,   257,   257,   258,   258,   259,   259,   260,
     260,   261,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   263,   263,   264,   265,
     265,   266,   267,   268,   266,   269,   270,   266,   271,   272,
     273,   271,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   275,   276,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   277,   277,   278,   278,   279,   280,   280,   281,   280,
     280,   282,   282,   283,   283,   284,   284,   285,   285,   286,
     287,   287,   288,   288,   289,   289,   289,   289,   289,   289,
     290,   290,   290,   291,   291,   292,   292,   292,   292,   292,
     293,   293,   294,   294,   295,   296,   295,   297,   297,   297,
     298,   299,   299,   300,   301,   301,   302,   302,   303,   304,
     304,   306,   305,   307,   307,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     309,   309,   309,   309,   309,   309,   310,   311,   311,   312,
     312,   314,   315,   313,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   317,   318,   316,   316,   319,   320,   316,   321,   322,
     316,   323,   324,   316,   316,   325,   326,   316,   327,   328,
     316,   316,   329,   330,   316,   331,   332,   316,   316,   333,
     334,   316,   335,   336,   316,   337,   338,   316,   339,   340,
     316,   342,   341,   344,   343,   345,   345,   345,   345,   346,
     346,   346,   346,   347,   347,   348,   348,   349,   349,   349,
     349,   349,   349,   350,   350,   351,   352,   352,   353,   353,
     354,   354,   355,   355,   356,   357,   357,   358,   358
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     2,     3,     7,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     2,     1,
       3,     2,     2,     2,     4,     5,     2,     1,     1,     2,
       3,     4,     2,     3,     3,     4,     2,     3,     4,     1,
       1,     2,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     5,     8,    10,     0,     0,    10,     0,     0,    13,
       0,     0,    15,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     6,     5,
       6,     3,     0,     0,     8,     0,     0,     9,     3,     0,
       0,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     4,     3,     1,     3,     4,     3,     4,     2,
       4,     4,     7,     8,     3,     5,     0,     0,     8,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     0,     4,     0,     1,     3,     0,     3,     0,     7,
       5,     2,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     4,     6,     5,     5,     4,
       0,     2,     0,     1,     5,     0,     5,     0,     3,     5,
       4,     1,     2,     4,     5,     7,     0,     2,     2,     1,
       1,     0,     7,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     0,     0,     6,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       8,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     0,     7,     0,     7,     1,     1,     1,     1,     3,
       3,     5,     5,     1,     3,     0,     2,     6,     5,     7,
       8,     6,     8,     1,     3,     3,     1,     1,     1,     3,
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
    case 143: /* "name"  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3436 "ds_parser.cpp"
        break;

    case 187: /* character_sequence  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3442 "ds_parser.cpp"
        break;

    case 188: /* string_constant  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3448 "ds_parser.cpp"
        break;

    case 189: /* string_builder_body  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3454 "ds_parser.cpp"
        break;

    case 190: /* string_builder  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3460 "ds_parser.cpp"
        break;

    case 192: /* expr_reader  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3466 "ds_parser.cpp"
        break;

    case 196: /* require_module_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3472 "ds_parser.cpp"
        break;

    case 202: /* expression_label  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3478 "ds_parser.cpp"
        break;

    case 203: /* expression_goto  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3484 "ds_parser.cpp"
        break;

    case 205: /* expression_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3490 "ds_parser.cpp"
        break;

    case 207: /* expression_if_then_else  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3496 "ds_parser.cpp"
        break;

    case 208: /* expression_for_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3502 "ds_parser.cpp"
        break;

    case 209: /* expression_unsafe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3508 "ds_parser.cpp"
        break;

    case 210: /* expression_while_loop  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3514 "ds_parser.cpp"
        break;

    case 211: /* expression_with  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3520 "ds_parser.cpp"
        break;

    case 212: /* annotation_argument_value  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3526 "ds_parser.cpp"
        break;

    case 213: /* annotation_argument_value_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3532 "ds_parser.cpp"
        break;

    case 214: /* annotation_argument  */
#line 130 "ds_parser.ypp"
      { delete ((*yyvaluep).aa); }
#line 3538 "ds_parser.cpp"
        break;

    case 215: /* annotation_argument_list  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3544 "ds_parser.cpp"
        break;

    case 216: /* annotation_declaration_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3550 "ds_parser.cpp"
        break;

    case 217: /* annotation_declaration  */
#line 132 "ds_parser.ypp"
      { delete ((*yyvaluep).fa); }
#line 3556 "ds_parser.cpp"
        break;

    case 218: /* annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3562 "ds_parser.cpp"
        break;

    case 219: /* optional_annotation_list  */
#line 133 "ds_parser.ypp"
      { delete ((*yyvaluep).faList); }
#line 3568 "ds_parser.cpp"
        break;

    case 220: /* optional_function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3574 "ds_parser.cpp"
        break;

    case 221: /* optional_function_type  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3580 "ds_parser.cpp"
        break;

    case 222: /* function_name  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3586 "ds_parser.cpp"
        break;

    case 226: /* expression_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3592 "ds_parser.cpp"
        break;

    case 227: /* expression_any  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3598 "ds_parser.cpp"
        break;

    case 228: /* expressions  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3604 "ds_parser.cpp"
        break;

    case 229: /* expr_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3610 "ds_parser.cpp"
        break;

    case 230: /* name_in_namespace  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3616 "ds_parser.cpp"
        break;

    case 231: /* expression_delete  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3622 "ds_parser.cpp"
        break;

    case 232: /* expr_new  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3628 "ds_parser.cpp"
        break;

    case 233: /* expression_break  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3634 "ds_parser.cpp"
        break;

    case 234: /* expression_continue  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3640 "ds_parser.cpp"
        break;

    case 235: /* expression_return  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3646 "ds_parser.cpp"
        break;

    case 236: /* expression_yield  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3652 "ds_parser.cpp"
        break;

    case 237: /* expression_try_catch  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3658 "ds_parser.cpp"
        break;

    case 239: /* expression_let  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3664 "ds_parser.cpp"
        break;

    case 240: /* expr_cast  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3670 "ds_parser.cpp"
        break;

    case 247: /* expr_type_info  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3676 "ds_parser.cpp"
        break;

    case 254: /* expr_list  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3682 "ds_parser.cpp"
        break;

    case 255: /* block_or_simple_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3688 "ds_parser.cpp"
        break;

    case 257: /* capture_entry  */
#line 136 "ds_parser.ypp"
      { delete ((*yyvaluep).pCapt); }
#line 3694 "ds_parser.cpp"
        break;

    case 258: /* capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3700 "ds_parser.cpp"
        break;

    case 259: /* optional_capture_list  */
#line 137 "ds_parser.ypp"
      { delete ((*yyvaluep).pCaptList); }
#line 3706 "ds_parser.cpp"
        break;

    case 260: /* expr_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3712 "ds_parser.cpp"
        break;

    case 261: /* expr_numeric_const  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3718 "ds_parser.cpp"
        break;

    case 262: /* expr_assign  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3724 "ds_parser.cpp"
        break;

    case 263: /* expr_assign_pipe  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3730 "ds_parser.cpp"
        break;

    case 264: /* expr_named_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3736 "ds_parser.cpp"
        break;

    case 265: /* expr_method_call  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3742 "ds_parser.cpp"
        break;

    case 266: /* func_addr_expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3748 "ds_parser.cpp"
        break;

    case 271: /* expr_field  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3754 "ds_parser.cpp"
        break;

    case 274: /* expr  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3760 "ds_parser.cpp"
        break;

    case 277: /* optional_field_annotation  */
#line 131 "ds_parser.ypp"
      { delete ((*yyvaluep).aaList); }
#line 3766 "ds_parser.cpp"
        break;

    case 279: /* structure_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3772 "ds_parser.cpp"
        break;

    case 280: /* struct_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3778 "ds_parser.cpp"
        break;

    case 282: /* function_argument_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3784 "ds_parser.cpp"
        break;

    case 283: /* function_argument_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3790 "ds_parser.cpp"
        break;

    case 284: /* tuple_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3796 "ds_parser.cpp"
        break;

    case 285: /* tuple_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3802 "ds_parser.cpp"
        break;

    case 286: /* variant_type  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3808 "ds_parser.cpp"
        break;

    case 287: /* variant_type_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3814 "ds_parser.cpp"
        break;

    case 289: /* variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3820 "ds_parser.cpp"
        break;

    case 292: /* let_variable_declaration  */
#line 126 "ds_parser.ypp"
      { delete ((*yyvaluep).pVarDecl); }
#line 3826 "ds_parser.cpp"
        break;

    case 293: /* global_variable_declaration_list  */
#line 127 "ds_parser.ypp"
      { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3832 "ds_parser.cpp"
        break;

    case 297: /* enum_list  */
#line 135 "ds_parser.ypp"
      { delete ((*yyvaluep).pEnum); }
#line 3838 "ds_parser.cpp"
        break;

    case 302: /* optional_structure_parent  */
#line 123 "ds_parser.ypp"
      { delete ((*yyvaluep).s); }
#line 3844 "ds_parser.cpp"
        break;

    case 307: /* variable_name_with_pos_list  */
#line 125 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameWithPosList); }
#line 3850 "ds_parser.cpp"
        break;

    case 310: /* structure_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3856 "ds_parser.cpp"
        break;

    case 311: /* auto_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3862 "ds_parser.cpp"
        break;

    case 312: /* bitfield_bits  */
#line 124 "ds_parser.ypp"
      { delete ((*yyvaluep).pNameList); }
#line 3868 "ds_parser.cpp"
        break;

    case 313: /* bitfield_type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3874 "ds_parser.cpp"
        break;

    case 316: /* type_declaration  */
#line 128 "ds_parser.ypp"
      { delete ((*yyvaluep).pTypeDecl); }
#line 3880 "ds_parser.cpp"
        break;

    case 345: /* make_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3886 "ds_parser.cpp"
        break;

    case 346: /* make_struct_fields  */
#line 134 "ds_parser.ypp"
      { delete ((*yyvaluep).pMakeStruct); }
#line 3892 "ds_parser.cpp"
        break;

    case 347: /* make_struct_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3898 "ds_parser.cpp"
        break;

    case 348: /* optional_block  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3904 "ds_parser.cpp"
        break;

    case 349: /* make_struct_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3910 "ds_parser.cpp"
        break;

    case 350: /* make_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3916 "ds_parser.cpp"
        break;

    case 351: /* make_map_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3922 "ds_parser.cpp"
        break;

    case 352: /* make_any_tuple  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3928 "ds_parser.cpp"
        break;

    case 353: /* make_dim  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3934 "ds_parser.cpp"
        break;

    case 354: /* make_dim_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3940 "ds_parser.cpp"
        break;

    case 355: /* make_table  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3946 "ds_parser.cpp"
        break;

    case 356: /* make_table_decl  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3952 "ds_parser.cpp"
        break;

    case 357: /* array_comprehension_where  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3958 "ds_parser.cpp"
        break;

    case 358: /* array_comprehension  */
#line 129 "ds_parser.ypp"
      { delete ((*yyvaluep).pExpression); }
#line 3964 "ds_parser.cpp"
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
#line 457 "ds_parser.ypp"
    {
        if ( g_Program->thisModule->name.empty() ) {
            g_Program->thisModule->name = *(yyvsp[0].s);
        } else if ( g_Program->thisModule->name != *(yyvsp[0].s) ){
            das_yyerror("this module already has a name, and its " + g_Program->thisModule->name,tokAt((yylsp[0])),
                CompilationError::module_already_has_a_name);
        }
        delete (yyvsp[0].s);
    }
#line 4264 "ds_parser.cpp"
    break;

  case 16:
#line 469 "ds_parser.ypp"
    { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4270 "ds_parser.cpp"
    break;

  case 17:
#line 470 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4276 "ds_parser.cpp"
    break;

  case 18:
#line 474 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[-1].s); }
#line 4282 "ds_parser.cpp"
    break;

  case 19:
#line 478 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4292 "ds_parser.cpp"
    break;

  case 20:
#line 483 "ds_parser.ypp"
    {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror("invalid escape sequence",tokAt((yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt((yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4306 "ds_parser.cpp"
    break;

  case 21:
#line 492 "ds_parser.ypp"
    {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4316 "ds_parser.cpp"
    break;

  case 22:
#line 500 "ds_parser.ypp"
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
#line 4334 "ds_parser.cpp"
    break;

  case 23:
#line 516 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4344 "ds_parser.cpp"
    break;

  case 24:
#line 521 "ds_parser.ypp"
    {
        if ( !g_ReaderMacro->accept(g_Program.get(), g_Program->thisModule.get(), g_ReaderExpr, (yyvsp[0].ch), tokAt((yylsp[0]))) ) {
            das_yyend_reader();
        }
    }
#line 4354 "ds_parser.cpp"
    break;

  case 25:
#line 529 "ds_parser.ypp"
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
#line 4381 "ds_parser.cpp"
    break;

  case 26:
#line 550 "ds_parser.ypp"
    {
        g_ReaderExpr->at = tokRangeAt((yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = g_ReaderExpr;
        delete (yyvsp[-2].s);
        g_ReaderMacro = nullptr;
        g_ReaderExpr = nullptr;
    }
#line 4393 "ds_parser.cpp"
    break;

  case 27:
#line 559 "ds_parser.ypp"
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
#line 4417 "ds_parser.cpp"
    break;

  case 29:
#line 585 "ds_parser.ypp"
    {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4425 "ds_parser.cpp"
    break;

  case 30:
#line 588 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4436 "ds_parser.cpp"
    break;

  case 31:
#line 594 "ds_parser.ypp"
    {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4447 "ds_parser.cpp"
    break;

  case 32:
#line 603 "ds_parser.ypp"
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
#line 4473 "ds_parser.cpp"
    break;

  case 33:
#line 624 "ds_parser.ypp"
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
#line 4498 "ds_parser.cpp"
    break;

  case 34:
#line 647 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4504 "ds_parser.cpp"
    break;

  case 35:
#line 648 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4510 "ds_parser.cpp"
    break;

  case 39:
#line 661 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4518 "ds_parser.cpp"
    break;

  case 40:
#line 664 "ds_parser.ypp"
    {
        g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4526 "ds_parser.cpp"
    break;

  case 41:
#line 670 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprLabel(tokAt((yylsp[-2])),(yyvsp[-1].i));
    }
#line 4534 "ds_parser.cpp"
    break;

  case 42:
#line 676 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-2])),(yyvsp[0].i));
    }
#line 4542 "ds_parser.cpp"
    break;

  case 43:
#line 679 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprGoto(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4550 "ds_parser.cpp"
    break;

  case 44:
#line 685 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4556 "ds_parser.cpp"
    break;

  case 45:
#line 686 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4562 "ds_parser.cpp"
    break;

  case 46:
#line 690 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 4568 "ds_parser.cpp"
    break;

  case 47:
#line 691 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4574 "ds_parser.cpp"
    break;

  case 48:
#line 692 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4585 "ds_parser.cpp"
    break;

  case 49:
#line 701 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 4591 "ds_parser.cpp"
    break;

  case 50:
#line 702 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 4597 "ds_parser.cpp"
    break;

  case 51:
#line 706 "ds_parser.ypp"
    {
        auto eite = new ExprIfThenElse(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4608 "ds_parser.cpp"
    break;

  case 52:
#line 715 "ds_parser.ypp"
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
#line 4626 "ds_parser.cpp"
    break;

  case 53:
#line 731 "ds_parser.ypp"
    {
        auto pUnsafe = new ExprUnsafe(tokAt((yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4636 "ds_parser.cpp"
    break;

  case 54:
#line 739 "ds_parser.ypp"
    {
        auto pWhile = new ExprWhile(tokAt((yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 4648 "ds_parser.cpp"
    break;

  case 55:
#line 749 "ds_parser.ypp"
    {
        auto pWith = new ExprWith(tokAt((yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 4659 "ds_parser.cpp"
    break;

  case 56:
#line 758 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4665 "ds_parser.cpp"
    break;

  case 57:
#line 759 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 4671 "ds_parser.cpp"
    break;

  case 58:
#line 760 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 4677 "ds_parser.cpp"
    break;

  case 59:
#line 761 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 4683 "ds_parser.cpp"
    break;

  case 60:
#line 762 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",true); }
#line 4689 "ds_parser.cpp"
    break;

  case 61:
#line 763 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument("",false); }
#line 4695 "ds_parser.cpp"
    break;

  case 62:
#line 767 "ds_parser.ypp"
    {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 4705 "ds_parser.cpp"
    break;

  case 63:
#line 772 "ds_parser.ypp"
    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 4715 "ds_parser.cpp"
    break;

  case 64:
#line 780 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4721 "ds_parser.cpp"
    break;

  case 65:
#line 781 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s)); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 4727 "ds_parser.cpp"
    break;

  case 66:
#line 782 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i)); delete (yyvsp[-2].s); }
#line 4733 "ds_parser.cpp"
    break;

  case 67:
#line 783 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd))); delete (yyvsp[-2].s); }
#line 4739 "ds_parser.cpp"
    break;

  case 68:
#line 784 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true); delete (yyvsp[-2].s); }
#line 4745 "ds_parser.cpp"
    break;

  case 69:
#line 785 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false); delete (yyvsp[-2].s); }
#line 4751 "ds_parser.cpp"
    break;

  case 70:
#line 786 "ds_parser.ypp"
    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true); delete (yyvsp[0].s); }
#line 4757 "ds_parser.cpp"
    break;

  case 71:
#line 787 "ds_parser.ypp"
    {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList)); delete (yyvsp[-4].s); }
    }
#line 4765 "ds_parser.cpp"
    break;

  case 72:
#line 793 "ds_parser.ypp"
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
#line 4783 "ds_parser.cpp"
    break;

  case 73:
#line 806 "ds_parser.ypp"
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
#line 4801 "ds_parser.cpp"
    break;

  case 74:
#line 822 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 4807 "ds_parser.cpp"
    break;

  case 75:
#line 823 "ds_parser.ypp"
    { (yyval.s) = new string("require"); }
#line 4813 "ds_parser.cpp"
    break;

  case 76:
#line 827 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[0].s),tokAt((yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 4825 "ds_parser.cpp"
    break;

  case 77:
#line 834 "ds_parser.ypp"
    {
        (yyval.fa) = new AnnotationDeclaration();
        if ( auto ann = findAnnotation(*(yyvsp[-3].s),tokAt((yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
#line 4839 "ds_parser.cpp"
    break;

  case 78:
#line 846 "ds_parser.ypp"
    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4848 "ds_parser.cpp"
    break;

  case 79:
#line 850 "ds_parser.ypp"
    {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 4857 "ds_parser.cpp"
    break;

  case 80:
#line 857 "ds_parser.ypp"
    { (yyval.faList) = nullptr; }
#line 4863 "ds_parser.cpp"
    break;

  case 81:
#line 858 "ds_parser.ypp"
    { (yyval.faList) = (yyvsp[-1].faList); }
#line 4869 "ds_parser.cpp"
    break;

  case 82:
#line 862 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4875 "ds_parser.cpp"
    break;

  case 83:
#line 863 "ds_parser.ypp"
    { (yyval.pVarDeclList) = nullptr; }
#line 4881 "ds_parser.cpp"
    break;

  case 84:
#line 864 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 4887 "ds_parser.cpp"
    break;

  case 85:
#line 868 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4897 "ds_parser.cpp"
    break;

  case 86:
#line 873 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 4906 "ds_parser.cpp"
    break;

  case 87:
#line 880 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 4915 "ds_parser.cpp"
    break;

  case 88:
#line 884 "ds_parser.ypp"
    { (yyval.s) = new string("!"); }
#line 4921 "ds_parser.cpp"
    break;

  case 89:
#line 885 "ds_parser.ypp"
    { (yyval.s) = new string("~"); }
#line 4927 "ds_parser.cpp"
    break;

  case 90:
#line 886 "ds_parser.ypp"
    { (yyval.s) = new string("+="); }
#line 4933 "ds_parser.cpp"
    break;

  case 91:
#line 887 "ds_parser.ypp"
    { (yyval.s) = new string("-="); }
#line 4939 "ds_parser.cpp"
    break;

  case 92:
#line 888 "ds_parser.ypp"
    { (yyval.s) = new string("*="); }
#line 4945 "ds_parser.cpp"
    break;

  case 93:
#line 889 "ds_parser.ypp"
    { (yyval.s) = new string("/="); }
#line 4951 "ds_parser.cpp"
    break;

  case 94:
#line 890 "ds_parser.ypp"
    { (yyval.s) = new string("%="); }
#line 4957 "ds_parser.cpp"
    break;

  case 95:
#line 891 "ds_parser.ypp"
    { (yyval.s) = new string("&="); }
#line 4963 "ds_parser.cpp"
    break;

  case 96:
#line 892 "ds_parser.ypp"
    { (yyval.s) = new string("|="); }
#line 4969 "ds_parser.cpp"
    break;

  case 97:
#line 893 "ds_parser.ypp"
    { (yyval.s) = new string("^="); }
#line 4975 "ds_parser.cpp"
    break;

  case 98:
#line 894 "ds_parser.ypp"
    { (yyval.s) = new string("&&="); }
#line 4981 "ds_parser.cpp"
    break;

  case 99:
#line 895 "ds_parser.ypp"
    { (yyval.s) = new string("||="); }
#line 4987 "ds_parser.cpp"
    break;

  case 100:
#line 896 "ds_parser.ypp"
    { (yyval.s) = new string("^^="); }
#line 4993 "ds_parser.cpp"
    break;

  case 101:
#line 897 "ds_parser.ypp"
    { (yyval.s) = new string("&&"); }
#line 4999 "ds_parser.cpp"
    break;

  case 102:
#line 898 "ds_parser.ypp"
    { (yyval.s) = new string("||"); }
#line 5005 "ds_parser.cpp"
    break;

  case 103:
#line 899 "ds_parser.ypp"
    { (yyval.s) = new string("^^"); }
#line 5011 "ds_parser.cpp"
    break;

  case 104:
#line 900 "ds_parser.ypp"
    { (yyval.s) = new string("+"); }
#line 5017 "ds_parser.cpp"
    break;

  case 105:
#line 901 "ds_parser.ypp"
    { (yyval.s) = new string("-"); }
#line 5023 "ds_parser.cpp"
    break;

  case 106:
#line 902 "ds_parser.ypp"
    { (yyval.s) = new string("*"); }
#line 5029 "ds_parser.cpp"
    break;

  case 107:
#line 903 "ds_parser.ypp"
    { (yyval.s) = new string("/"); }
#line 5035 "ds_parser.cpp"
    break;

  case 108:
#line 904 "ds_parser.ypp"
    { (yyval.s) = new string("%"); }
#line 5041 "ds_parser.cpp"
    break;

  case 109:
#line 905 "ds_parser.ypp"
    { (yyval.s) = new string("<"); }
#line 5047 "ds_parser.cpp"
    break;

  case 110:
#line 906 "ds_parser.ypp"
    { (yyval.s) = new string(">"); }
#line 5053 "ds_parser.cpp"
    break;

  case 111:
#line 907 "ds_parser.ypp"
    { (yyval.s) = new string("=="); }
#line 5059 "ds_parser.cpp"
    break;

  case 112:
#line 908 "ds_parser.ypp"
    { (yyval.s) = new string("!="); }
#line 5065 "ds_parser.cpp"
    break;

  case 113:
#line 909 "ds_parser.ypp"
    { (yyval.s) = new string("<="); }
#line 5071 "ds_parser.cpp"
    break;

  case 114:
#line 910 "ds_parser.ypp"
    { (yyval.s) = new string(">="); }
#line 5077 "ds_parser.cpp"
    break;

  case 115:
#line 911 "ds_parser.ypp"
    { (yyval.s) = new string("&"); }
#line 5083 "ds_parser.cpp"
    break;

  case 116:
#line 912 "ds_parser.ypp"
    { (yyval.s) = new string("|"); }
#line 5089 "ds_parser.cpp"
    break;

  case 117:
#line 913 "ds_parser.ypp"
    { (yyval.s) = new string("^"); }
#line 5095 "ds_parser.cpp"
    break;

  case 118:
#line 914 "ds_parser.ypp"
    { (yyval.s) = new string("++"); }
#line 5101 "ds_parser.cpp"
    break;

  case 119:
#line 915 "ds_parser.ypp"
    { (yyval.s) = new string("--"); }
#line 5107 "ds_parser.cpp"
    break;

  case 120:
#line 916 "ds_parser.ypp"
    { (yyval.s) = new string("+++"); }
#line 5113 "ds_parser.cpp"
    break;

  case 121:
#line 917 "ds_parser.ypp"
    { (yyval.s) = new string("---"); }
#line 5119 "ds_parser.cpp"
    break;

  case 122:
#line 918 "ds_parser.ypp"
    { (yyval.s) = new string("<<"); }
#line 5125 "ds_parser.cpp"
    break;

  case 123:
#line 919 "ds_parser.ypp"
    { (yyval.s) = new string(">>"); }
#line 5131 "ds_parser.cpp"
    break;

  case 124:
#line 920 "ds_parser.ypp"
    { (yyval.s) = new string("<<="); }
#line 5137 "ds_parser.cpp"
    break;

  case 125:
#line 921 "ds_parser.ypp"
    { (yyval.s) = new string(">>="); }
#line 5143 "ds_parser.cpp"
    break;

  case 126:
#line 922 "ds_parser.ypp"
    { (yyval.s) = new string("<<<"); }
#line 5149 "ds_parser.cpp"
    break;

  case 127:
#line 923 "ds_parser.ypp"
    { (yyval.s) = new string(">>>"); }
#line 5155 "ds_parser.cpp"
    break;

  case 128:
#line 924 "ds_parser.ypp"
    { (yyval.s) = new string("<<<="); }
#line 5161 "ds_parser.cpp"
    break;

  case 129:
#line 925 "ds_parser.ypp"
    { (yyval.s) = new string(">>>="); }
#line 5167 "ds_parser.cpp"
    break;

  case 130:
#line 929 "ds_parser.ypp"
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
#line 5190 "ds_parser.cpp"
    break;

  case 131:
#line 950 "ds_parser.ypp"
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
#line 5230 "ds_parser.cpp"
    break;

  case 132:
#line 988 "ds_parser.ypp"
    {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5239 "ds_parser.cpp"
    break;

  case 133:
#line 995 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-2]),(yylsp[0]));
    }
#line 5248 "ds_parser.cpp"
    break;

  case 134:
#line 999 "ds_parser.ypp"
    {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt((yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5261 "ds_parser.cpp"
    break;

  case 135:
#line 1010 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5267 "ds_parser.cpp"
    break;

  case 136:
#line 1011 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5273 "ds_parser.cpp"
    break;

  case 137:
#line 1012 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5279 "ds_parser.cpp"
    break;

  case 138:
#line 1013 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5285 "ds_parser.cpp"
    break;

  case 139:
#line 1014 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5291 "ds_parser.cpp"
    break;

  case 140:
#line 1015 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5297 "ds_parser.cpp"
    break;

  case 141:
#line 1016 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5303 "ds_parser.cpp"
    break;

  case 142:
#line 1017 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5309 "ds_parser.cpp"
    break;

  case 143:
#line 1018 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5315 "ds_parser.cpp"
    break;

  case 144:
#line 1019 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5321 "ds_parser.cpp"
    break;

  case 145:
#line 1020 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5327 "ds_parser.cpp"
    break;

  case 146:
#line 1021 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5333 "ds_parser.cpp"
    break;

  case 147:
#line 1022 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5339 "ds_parser.cpp"
    break;

  case 148:
#line 1023 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5345 "ds_parser.cpp"
    break;

  case 149:
#line 1024 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5351 "ds_parser.cpp"
    break;

  case 150:
#line 1025 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5357 "ds_parser.cpp"
    break;

  case 151:
#line 1026 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5363 "ds_parser.cpp"
    break;

  case 152:
#line 1027 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5369 "ds_parser.cpp"
    break;

  case 153:
#line 1028 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 5375 "ds_parser.cpp"
    break;

  case 154:
#line 1032 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5385 "ds_parser.cpp"
    break;

  case 155:
#line 1037 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5396 "ds_parser.cpp"
    break;

  case 156:
#line 1043 "ds_parser.ypp"
    {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5404 "ds_parser.cpp"
    break;

  case 157:
#line 1049 "ds_parser.ypp"
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
#line 5430 "ds_parser.cpp"
    break;

  case 158:
#line 1070 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5438 "ds_parser.cpp"
    break;

  case 159:
#line 1076 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 5444 "ds_parser.cpp"
    break;

  case 160:
#line 1077 "ds_parser.ypp"
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
#line 5460 "ds_parser.cpp"
    break;

  case 161:
#line 1088 "ds_parser.ypp"
    { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5466 "ds_parser.cpp"
    break;

  case 162:
#line 1092 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprDelete(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5474 "ds_parser.cpp"
    break;

  case 163:
#line 1098 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5482 "ds_parser.cpp"
    break;

  case 164:
#line 1101 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprNew(tokAt((yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5490 "ds_parser.cpp"
    break;

  case 165:
#line 1104 "ds_parser.ypp"
    {
        auto pNew = new ExprNew(tokAt((yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5499 "ds_parser.cpp"
    break;

  case 166:
#line 1108 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAscend(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5507 "ds_parser.cpp"
    break;

  case 167:
#line 1114 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprBreak(tokAt((yylsp[0]))); }
#line 5513 "ds_parser.cpp"
    break;

  case 168:
#line 1118 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprContinue(tokAt((yylsp[0]))); }
#line 5519 "ds_parser.cpp"
    break;

  case 169:
#line 1122 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),nullptr);
    }
#line 5527 "ds_parser.cpp"
    break;

  case 170:
#line 1125 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5535 "ds_parser.cpp"
    break;

  case 171:
#line 1128 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5545 "ds_parser.cpp"
    break;

  case 172:
#line 1133 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprReturn(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5553 "ds_parser.cpp"
    break;

  case 173:
#line 1136 "ds_parser.ypp"
    {
        auto pRet = new ExprReturn(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5563 "ds_parser.cpp"
    break;

  case 174:
#line 1144 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 5571 "ds_parser.cpp"
    break;

  case 175:
#line 1147 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5581 "ds_parser.cpp"
    break;

  case 176:
#line 1152 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprYield(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5589 "ds_parser.cpp"
    break;

  case 177:
#line 1155 "ds_parser.ypp"
    {
        auto pRet = new ExprYield(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 5599 "ds_parser.cpp"
    break;

  case 178:
#line 1163 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprTryCatch(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5607 "ds_parser.cpp"
    break;

  case 179:
#line 1169 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 5613 "ds_parser.cpp"
    break;

  case 180:
#line 1170 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 5619 "ds_parser.cpp"
    break;

  case 181:
#line 1174 "ds_parser.ypp"
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
#line 5655 "ds_parser.cpp"
    break;

  case 182:
#line 1208 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5661 "ds_parser.cpp"
    break;

  case 183:
#line 1208 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5667 "ds_parser.cpp"
    break;

  case 184:
#line 1208 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 5675 "ds_parser.cpp"
    break;

  case 185:
#line 1211 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5681 "ds_parser.cpp"
    break;

  case 186:
#line 1211 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5687 "ds_parser.cpp"
    break;

  case 187:
#line 1211 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 5697 "ds_parser.cpp"
    break;

  case 188:
#line 1216 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5703 "ds_parser.cpp"
    break;

  case 189:
#line 1216 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5709 "ds_parser.cpp"
    break;

  case 190:
#line 1216 "ds_parser.ypp"
    {
        auto pCast = new ExprCast(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 5719 "ds_parser.cpp"
    break;

  case 191:
#line 1224 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            delete (yyvsp[-2].s);
    }
#line 5728 "ds_parser.cpp"
    break;

  case 192:
#line 1228 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5738 "ds_parser.cpp"
    break;

  case 193:
#line 1233 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
#line 5749 "ds_parser.cpp"
    break;

  case 194:
#line 1239 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5755 "ds_parser.cpp"
    break;

  case 195:
#line 1239 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5761 "ds_parser.cpp"
    break;

  case 196:
#line 1239 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-9])),*(yyvsp[-7].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
            delete (yyvsp[-7].s);
    }
#line 5770 "ds_parser.cpp"
    break;

  case 197:
#line 1243 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5776 "ds_parser.cpp"
    break;

  case 198:
#line 1243 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5782 "ds_parser.cpp"
    break;

  case 199:
#line 1243 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-12])),*(yyvsp[-10].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-8].s));
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5792 "ds_parser.cpp"
    break;

  case 200:
#line 1248 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 5798 "ds_parser.cpp"
    break;

  case 201:
#line 1248 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 5804 "ds_parser.cpp"
    break;

  case 202:
#line 1248 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprTypeInfo(tokAt((yylsp[-14])),*(yyvsp[-12].s),TypeDeclPtr((yyvsp[-3].pTypeDecl)),*(yyvsp[-10].s),*(yyvsp[-8].s));
            delete (yyvsp[-12].s);
            delete (yyvsp[-10].s);
            delete (yyvsp[-8].s);
    }
#line 5815 "ds_parser.cpp"
    break;

  case 203:
#line 1257 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5823 "ds_parser.cpp"
    break;

  case 204:
#line 1260 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprSequence(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5831 "ds_parser.cpp"
    break;

  case 205:
#line 1266 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5837 "ds_parser.cpp"
    break;

  case 206:
#line 1267 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5849 "ds_parser.cpp"
    break;

  case 207:
#line 1274 "ds_parser.ypp"
    {
            auto retE = make_smart<ExprReturn>(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt((yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 5862 "ds_parser.cpp"
    break;

  case 208:
#line 1285 "ds_parser.ypp"
    { (yyval.i) = 0;   /* block */  }
#line 5868 "ds_parser.cpp"
    break;

  case 209:
#line 1286 "ds_parser.ypp"
    { (yyval.i) = 1;   /* lambda */ }
#line 5874 "ds_parser.cpp"
    break;

  case 210:
#line 1287 "ds_parser.ypp"
    { (yyval.i) = 2;   /* local function */ }
#line 5880 "ds_parser.cpp"
    break;

  case 211:
#line 1291 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 5886 "ds_parser.cpp"
    break;

  case 212:
#line 1292 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 5892 "ds_parser.cpp"
    break;

  case 213:
#line 1293 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 5898 "ds_parser.cpp"
    break;

  case 214:
#line 1294 "ds_parser.ypp"
    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 5904 "ds_parser.cpp"
    break;

  case 215:
#line 1298 "ds_parser.ypp"
    {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 5914 "ds_parser.cpp"
    break;

  case 216:
#line 1303 "ds_parser.ypp"
    {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 5924 "ds_parser.cpp"
    break;

  case 217:
#line 1311 "ds_parser.ypp"
    { (yyval.pCaptList) = nullptr; }
#line 5930 "ds_parser.cpp"
    break;

  case 218:
#line 1312 "ds_parser.ypp"
    { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 5936 "ds_parser.cpp"
    break;

  case 219:
#line 1316 "ds_parser.ypp"
    {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt((yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 5946 "ds_parser.cpp"
    break;

  case 220:
#line 1322 "ds_parser.ypp"
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
#line 6011 "ds_parser.cpp"
    break;

  case 221:
#line 1385 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt(tokAt((yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6017 "ds_parser.cpp"
    break;

  case 222:
#line 1386 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt(tokAt((yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6023 "ds_parser.cpp"
    break;

  case 223:
#line 1387 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstInt64(tokAt((yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6029 "ds_parser.cpp"
    break;

  case 224:
#line 1388 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstUInt64(tokAt((yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6035 "ds_parser.cpp"
    break;

  case 225:
#line 1389 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstFloat(tokAt((yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6041 "ds_parser.cpp"
    break;

  case 226:
#line 1390 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstDouble(tokAt((yylsp[0])),(double)(yyvsp[0].d)); }
#line 6047 "ds_parser.cpp"
    break;

  case 227:
#line 1394 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6053 "ds_parser.cpp"
    break;

  case 228:
#line 1395 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6059 "ds_parser.cpp"
    break;

  case 229:
#line 1396 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6065 "ds_parser.cpp"
    break;

  case 230:
#line 1397 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprClone(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6071 "ds_parser.cpp"
    break;

  case 231:
#line 1398 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6077 "ds_parser.cpp"
    break;

  case 232:
#line 1399 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6083 "ds_parser.cpp"
    break;

  case 233:
#line 1400 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6089 "ds_parser.cpp"
    break;

  case 234:
#line 1401 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6095 "ds_parser.cpp"
    break;

  case 235:
#line 1402 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6101 "ds_parser.cpp"
    break;

  case 236:
#line 1403 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6107 "ds_parser.cpp"
    break;

  case 237:
#line 1404 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6113 "ds_parser.cpp"
    break;

  case 238:
#line 1405 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6119 "ds_parser.cpp"
    break;

  case 239:
#line 1406 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6125 "ds_parser.cpp"
    break;

  case 240:
#line 1407 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6131 "ds_parser.cpp"
    break;

  case 241:
#line 1408 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6137 "ds_parser.cpp"
    break;

  case 242:
#line 1409 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6143 "ds_parser.cpp"
    break;

  case 243:
#line 1410 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6149 "ds_parser.cpp"
    break;

  case 244:
#line 1411 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6155 "ds_parser.cpp"
    break;

  case 245:
#line 1412 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6161 "ds_parser.cpp"
    break;

  case 246:
#line 1416 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprCopy(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6167 "ds_parser.cpp"
    break;

  case 247:
#line 1417 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprMove(tokAt((yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6173 "ds_parser.cpp"
    break;

  case 248:
#line 1421 "ds_parser.ypp"
    {
        auto nc = new ExprNamedCall(tokAt((yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6185 "ds_parser.cpp"
    break;

  case 249:
#line 1432 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6195 "ds_parser.cpp"
    break;

  case 250:
#line 1437 "ds_parser.ypp"
    {
        auto pInvoke = makeInvokeMethod(tokAt((yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6207 "ds_parser.cpp"
    break;

  case 251:
#line 1447 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAddr(tokAt((yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6216 "ds_parser.cpp"
    break;

  case 252:
#line 1451 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6222 "ds_parser.cpp"
    break;

  case 253:
#line 1451 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6228 "ds_parser.cpp"
    break;

  case 254:
#line 1451 "ds_parser.ypp"
    {
        auto expr = new ExprAddr(tokAt((yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6240 "ds_parser.cpp"
    break;

  case 255:
#line 1458 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6246 "ds_parser.cpp"
    break;

  case 256:
#line 1458 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6252 "ds_parser.cpp"
    break;

  case 257:
#line 1458 "ds_parser.ypp"
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
#line 6268 "ds_parser.cpp"
    break;

  case 258:
#line 1472 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6277 "ds_parser.cpp"
    break;

  case 259:
#line 1476 "ds_parser.ypp"
    { das_supress_errors=true; }
#line 6283 "ds_parser.cpp"
    break;

  case 260:
#line 1476 "ds_parser.ypp"
    { das_supress_errors=false; }
#line 6289 "ds_parser.cpp"
    break;

  case 261:
#line 1476 "ds_parser.ypp"
    {
        printf("here\n");
        (yyval.pExpression) = new ExprField(tokAt((yylsp[-3])), tokAt((yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6299 "ds_parser.cpp"
    break;

  case 262:
#line 1484 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstPtr(tokAt((yylsp[0])),nullptr); }
#line 6305 "ds_parser.cpp"
    break;

  case 263:
#line 1485 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprVar(tokAt((yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6311 "ds_parser.cpp"
    break;

  case 264:
#line 1486 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6317 "ds_parser.cpp"
    break;

  case 265:
#line 1487 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6323 "ds_parser.cpp"
    break;

  case 266:
#line 1488 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6329 "ds_parser.cpp"
    break;

  case 267:
#line 1489 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6335 "ds_parser.cpp"
    break;

  case 268:
#line 1490 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),true); }
#line 6341 "ds_parser.cpp"
    break;

  case 269:
#line 1491 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprConstBool(tokAt((yylsp[0])),false); }
#line 6347 "ds_parser.cpp"
    break;

  case 270:
#line 1492 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6353 "ds_parser.cpp"
    break;

  case 271:
#line 1493 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6359 "ds_parser.cpp"
    break;

  case 272:
#line 1494 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6365 "ds_parser.cpp"
    break;

  case 273:
#line 1495 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6371 "ds_parser.cpp"
    break;

  case 274:
#line 1496 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6377 "ds_parser.cpp"
    break;

  case 275:
#line 1497 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6383 "ds_parser.cpp"
    break;

  case 276:
#line 1498 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6389 "ds_parser.cpp"
    break;

  case 277:
#line 1499 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6395 "ds_parser.cpp"
    break;

  case 278:
#line 1500 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6401 "ds_parser.cpp"
    break;

  case 279:
#line 1501 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6407 "ds_parser.cpp"
    break;

  case 280:
#line 1502 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6413 "ds_parser.cpp"
    break;

  case 281:
#line 1503 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6419 "ds_parser.cpp"
    break;

  case 282:
#line 1504 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6425 "ds_parser.cpp"
    break;

  case 283:
#line 1505 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6431 "ds_parser.cpp"
    break;

  case 284:
#line 1506 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6437 "ds_parser.cpp"
    break;

  case 285:
#line 1507 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6443 "ds_parser.cpp"
    break;

  case 286:
#line 1508 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6449 "ds_parser.cpp"
    break;

  case 287:
#line 1509 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6455 "ds_parser.cpp"
    break;

  case 288:
#line 1510 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6461 "ds_parser.cpp"
    break;

  case 289:
#line 1511 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6467 "ds_parser.cpp"
    break;

  case 290:
#line 1512 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6473 "ds_parser.cpp"
    break;

  case 291:
#line 1513 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6479 "ds_parser.cpp"
    break;

  case 292:
#line 1514 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6485 "ds_parser.cpp"
    break;

  case 293:
#line 1515 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6491 "ds_parser.cpp"
    break;

  case 294:
#line 1516 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6497 "ds_parser.cpp"
    break;

  case 295:
#line 1517 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp2(tokAt((yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6503 "ds_parser.cpp"
    break;

  case 296:
#line 1518 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6509 "ds_parser.cpp"
    break;

  case 297:
#line 1519 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6515 "ds_parser.cpp"
    break;

  case 298:
#line 1520 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6521 "ds_parser.cpp"
    break;

  case 299:
#line 1521 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprOp1(tokAt((yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6527 "ds_parser.cpp"
    break;

  case 300:
#line 1522 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 6533 "ds_parser.cpp"
    break;

  case 301:
#line 1523 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6539 "ds_parser.cpp"
    break;

  case 302:
#line 1524 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeAt(tokAt((yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6545 "ds_parser.cpp"
    break;

  case 303:
#line 1525 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprSafeField(tokAt((yylsp[-1])), tokAt((yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6551 "ds_parser.cpp"
    break;

  case 304:
#line 1526 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6557 "ds_parser.cpp"
    break;

  case 305:
#line 1527 "ds_parser.ypp"
    {
            (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 6566 "ds_parser.cpp"
    break;

  case 306:
#line 1531 "ds_parser.ypp"
    {
            (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 6575 "ds_parser.cpp"
    break;

  case 307:
#line 1535 "ds_parser.ypp"
    {
        (yyval.pExpression) = g_Program->makeCall(tokAt((yylsp[-2])),das_to_string((yyvsp[-2].type)));
    }
#line 6583 "ds_parser.cpp"
    break;

  case 308:
#line 1538 "ds_parser.ypp"
    {
        (yyval.pExpression) = parseFunctionArguments(g_Program->makeCall(tokAt((yylsp[-3])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 6591 "ds_parser.cpp"
    break;

  case 309:
#line 1541 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6597 "ds_parser.cpp"
    break;

  case 310:
#line 1542 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprPtr2Ref(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6603 "ds_parser.cpp"
    break;

  case 311:
#line 1543 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprRef2Ptr(tokAt((yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 6609 "ds_parser.cpp"
    break;

  case 312:
#line 1544 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6623 "ds_parser.cpp"
    break;

  case 313:
#line 1553 "ds_parser.ypp"
    {
        auto gen = new ExprMakeGenerator(tokAt((yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 6637 "ds_parser.cpp"
    break;

  case 314:
#line 1562 "ds_parser.ypp"
    { (yyval.pExpression) = new ExprNullCoalescing(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6643 "ds_parser.cpp"
    break;

  case 315:
#line 1563 "ds_parser.ypp"
    {
            (yyval.pExpression) = new ExprOp3(tokAt((yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 6651 "ds_parser.cpp"
    break;

  case 316:
#line 1566 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 6657 "ds_parser.cpp"
    break;

  case 317:
#line 1566 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 6663 "ds_parser.cpp"
    break;

  case 318:
#line 1566 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIs(tokAt((yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6671 "ds_parser.cpp"
    break;

  case 319:
#line 1569 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprIsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6680 "ds_parser.cpp"
    break;

  case 320:
#line 1573 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6689 "ds_parser.cpp"
    break;

  case 321:
#line 1577 "ds_parser.ypp"
    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt((yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6698 "ds_parser.cpp"
    break;

  case 322:
#line 1581 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6704 "ds_parser.cpp"
    break;

  case 323:
#line 1582 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6710 "ds_parser.cpp"
    break;

  case 324:
#line 1583 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6716 "ds_parser.cpp"
    break;

  case 325:
#line 1584 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6722 "ds_parser.cpp"
    break;

  case 326:
#line 1585 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6728 "ds_parser.cpp"
    break;

  case 327:
#line 1586 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6734 "ds_parser.cpp"
    break;

  case 328:
#line 1587 "ds_parser.ypp"
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
#line 6749 "ds_parser.cpp"
    break;

  case 329:
#line 1597 "ds_parser.ypp"
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
#line 6764 "ds_parser.cpp"
    break;

  case 330:
#line 1607 "ds_parser.ypp"
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
#line 6821 "ds_parser.cpp"
    break;

  case 331:
#line 1662 "ds_parser.ypp"
    { (yyval.aaList) = nullptr; }
#line 6827 "ds_parser.cpp"
    break;

  case 332:
#line 1663 "ds_parser.ypp"
    { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 6833 "ds_parser.cpp"
    break;

  case 333:
#line 1667 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 6839 "ds_parser.cpp"
    break;

  case 334:
#line 1668 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 6845 "ds_parser.cpp"
    break;

  case 335:
#line 1672 "ds_parser.ypp"
    {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 6855 "ds_parser.cpp"
    break;

  case 336:
#line 1680 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 6863 "ds_parser.cpp"
    break;

  case 337:
#line 1683 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 6872 "ds_parser.cpp"
    break;

  case 338:
#line 1688 "ds_parser.ypp"
    { das_force_oxford_comma=true; }
#line 6878 "ds_parser.cpp"
    break;

  case 339:
#line 1688 "ds_parser.ypp"
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
#line 6927 "ds_parser.cpp"
    break;

  case 340:
#line 1732 "ds_parser.ypp"
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
#line 6987 "ds_parser.cpp"
    break;

  case 341:
#line 1790 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 6997 "ds_parser.cpp"
    break;

  case 342:
#line 1795 "ds_parser.ypp"
    {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7007 "ds_parser.cpp"
    break;

  case 343:
#line 1803 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7013 "ds_parser.cpp"
    break;

  case 344:
#line 1804 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7019 "ds_parser.cpp"
    break;

  case 345:
#line 1808 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7027 "ds_parser.cpp"
    break;

  case 346:
#line 1811 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7038 "ds_parser.cpp"
    break;

  case 347:
#line 1820 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7044 "ds_parser.cpp"
    break;

  case 348:
#line 1821 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7050 "ds_parser.cpp"
    break;

  case 349:
#line 1825 "ds_parser.ypp"
    {
        auto na = new vector<pair<string,LineInfo>>();
        na->push_back(make_pair(*(yyvsp[-2].s),tokAt((yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7061 "ds_parser.cpp"
    break;

  case 350:
#line 1834 "ds_parser.ypp"
    { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7067 "ds_parser.cpp"
    break;

  case 351:
#line 1835 "ds_parser.ypp"
    { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7073 "ds_parser.cpp"
    break;

  case 352:
#line 1840 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7079 "ds_parser.cpp"
    break;

  case 353:
#line 1841 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7085 "ds_parser.cpp"
    break;

  case 354:
#line 1845 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7096 "ds_parser.cpp"
    break;

  case 355:
#line 1851 "ds_parser.ypp"
    {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt((yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7107 "ds_parser.cpp"
    break;

  case 356:
#line 1857 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7115 "ds_parser.cpp"
    break;

  case 357:
#line 1860 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7124 "ds_parser.cpp"
    break;

  case 358:
#line 1864 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7135 "ds_parser.cpp"
    break;

  case 359:
#line 1870 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7146 "ds_parser.cpp"
    break;

  case 360:
#line 1879 "ds_parser.ypp"
    { (yyval.i) = CorM_COPY; }
#line 7152 "ds_parser.cpp"
    break;

  case 361:
#line 1880 "ds_parser.ypp"
    { (yyval.i) = CorM_MOVE; }
#line 7158 "ds_parser.cpp"
    break;

  case 362:
#line 1881 "ds_parser.ypp"
    { (yyval.i) = CorM_CLONE; }
#line 7164 "ds_parser.cpp"
    break;

  case 363:
#line 1885 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7170 "ds_parser.cpp"
    break;

  case 364:
#line 1886 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7176 "ds_parser.cpp"
    break;

  case 365:
#line 1890 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7184 "ds_parser.cpp"
    break;

  case 366:
#line 1893 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7194 "ds_parser.cpp"
    break;

  case 367:
#line 1898 "ds_parser.ypp"
    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7204 "ds_parser.cpp"
    break;

  case 368:
#line 1903 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7217 "ds_parser.cpp"
    break;

  case 369:
#line 1911 "ds_parser.ypp"
    {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt((yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7230 "ds_parser.cpp"
    break;

  case 370:
#line 1922 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7238 "ds_parser.cpp"
    break;

  case 371:
#line 1925 "ds_parser.ypp"
    {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7247 "ds_parser.cpp"
    break;

  case 372:
#line 1932 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7253 "ds_parser.cpp"
    break;

  case 373:
#line 1933 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7259 "ds_parser.cpp"
    break;

  case 374:
#line 1937 "ds_parser.ypp"
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
#line 7291 "ds_parser.cpp"
    break;

  case 375:
#line 1964 "ds_parser.ypp"
    {
            das_force_oxford_comma = true;
            das_supress_errors = true;
        }
#line 7300 "ds_parser.cpp"
    break;

  case 376:
#line 1967 "ds_parser.ypp"
    {
        das_yyerror("global variable declaration requires new line", tokAt((yylsp[-1])),
            CompilationError::syntax_error);
        das_supress_errors = false;
        yyerrok;
    }
#line 7311 "ds_parser.cpp"
    break;

  case 377:
#line 1976 "ds_parser.ypp"
    {
        (yyval.pEnum) = new Enumeration();
    }
#line 7319 "ds_parser.cpp"
    break;

  case 378:
#line 1979 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt((yylsp[-1]))) ) {
            das_yyerror("enumeration alread declared " + *(yyvsp[-1].s), tokAt((yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7333 "ds_parser.cpp"
    break;

  case 379:
#line 1988 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt((yylsp[-3]))) ) {
            das_yyerror("enumeration value alread declared " + *(yyvsp[-3].s), tokAt((yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7347 "ds_parser.cpp"
    break;

  case 380:
#line 2001 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-3].s),tokAt((yylsp[-3])));
        (yyvsp[-1].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !g_Program->addAlias(TypeDeclPtr((yyvsp[-1].pTypeDecl))) ) {
            das_yyerror("type alias is already defined "+*(yyvsp[-3].s),tokAt((yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 7361 "ds_parser.cpp"
    break;

  case 384:
#line 2022 "ds_parser.ypp"
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
#line 7377 "ds_parser.cpp"
    break;

  case 385:
#line 2033 "ds_parser.ypp"
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
#line 7394 "ds_parser.cpp"
    break;

  case 386:
#line 2048 "ds_parser.ypp"
    { (yyval.s) = nullptr; }
#line 7400 "ds_parser.cpp"
    break;

  case 387:
#line 2049 "ds_parser.ypp"
    { (yyval.s) = (yyvsp[0].s); }
#line 7406 "ds_parser.cpp"
    break;

  case 388:
#line 2053 "ds_parser.ypp"
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
#line 7445 "ds_parser.cpp"
    break;

  case 389:
#line 2090 "ds_parser.ypp"
    { (yyval.b) = true; }
#line 7451 "ds_parser.cpp"
    break;

  case 390:
#line 2091 "ds_parser.ypp"
    { (yyval.b) = false; }
#line 7457 "ds_parser.cpp"
    break;

  case 391:
#line 2095 "ds_parser.ypp"
    { if ( (yyvsp[0].pStructure) ) (yyvsp[0].pStructure)->isClass = (yyvsp[-1].b); }
#line 7463 "ds_parser.cpp"
    break;

  case 392:
#line 2095 "ds_parser.ypp"
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
#line 7549 "ds_parser.cpp"
    break;

  case 393:
#line 2179 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<pair<string,LineInfo>>();
        pSL->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7561 "ds_parser.cpp"
    break;

  case 394:
#line 2186 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(make_pair(*(yyvsp[0].s),tokAt((yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 7572 "ds_parser.cpp"
    break;

  case 395:
#line 2195 "ds_parser.ypp"
    { (yyval.type) = Type::tBool; }
#line 7578 "ds_parser.cpp"
    break;

  case 396:
#line 2196 "ds_parser.ypp"
    { (yyval.type) = Type::tString; }
#line 7584 "ds_parser.cpp"
    break;

  case 397:
#line 2197 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7590 "ds_parser.cpp"
    break;

  case 398:
#line 2198 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7596 "ds_parser.cpp"
    break;

  case 399:
#line 2199 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7602 "ds_parser.cpp"
    break;

  case 400:
#line 2200 "ds_parser.ypp"
    { (yyval.type) = Type::tInt64; }
#line 7608 "ds_parser.cpp"
    break;

  case 401:
#line 2201 "ds_parser.ypp"
    { (yyval.type) = Type::tInt2; }
#line 7614 "ds_parser.cpp"
    break;

  case 402:
#line 2202 "ds_parser.ypp"
    { (yyval.type) = Type::tInt3; }
#line 7620 "ds_parser.cpp"
    break;

  case 403:
#line 2203 "ds_parser.ypp"
    { (yyval.type) = Type::tInt4; }
#line 7626 "ds_parser.cpp"
    break;

  case 404:
#line 2204 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7632 "ds_parser.cpp"
    break;

  case 405:
#line 2205 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7638 "ds_parser.cpp"
    break;

  case 406:
#line 2206 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7644 "ds_parser.cpp"
    break;

  case 407:
#line 2207 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt64; }
#line 7650 "ds_parser.cpp"
    break;

  case 408:
#line 2208 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt2; }
#line 7656 "ds_parser.cpp"
    break;

  case 409:
#line 2209 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt3; }
#line 7662 "ds_parser.cpp"
    break;

  case 410:
#line 2210 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt4; }
#line 7668 "ds_parser.cpp"
    break;

  case 411:
#line 2211 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat; }
#line 7674 "ds_parser.cpp"
    break;

  case 412:
#line 2212 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat2; }
#line 7680 "ds_parser.cpp"
    break;

  case 413:
#line 2213 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat3; }
#line 7686 "ds_parser.cpp"
    break;

  case 414:
#line 2214 "ds_parser.ypp"
    { (yyval.type) = Type::tFloat4; }
#line 7692 "ds_parser.cpp"
    break;

  case 415:
#line 2215 "ds_parser.ypp"
    { (yyval.type) = Type::tVoid; }
#line 7698 "ds_parser.cpp"
    break;

  case 416:
#line 2216 "ds_parser.ypp"
    { (yyval.type) = Type::tRange; }
#line 7704 "ds_parser.cpp"
    break;

  case 417:
#line 2217 "ds_parser.ypp"
    { (yyval.type) = Type::tURange; }
#line 7710 "ds_parser.cpp"
    break;

  case 418:
#line 2218 "ds_parser.ypp"
    { (yyval.type) = Type::tDouble; }
#line 7716 "ds_parser.cpp"
    break;

  case 419:
#line 2219 "ds_parser.ypp"
    { (yyval.type) = Type::tBitfield; }
#line 7722 "ds_parser.cpp"
    break;

  case 420:
#line 2223 "ds_parser.ypp"
    { (yyval.type) = Type::tInt; }
#line 7728 "ds_parser.cpp"
    break;

  case 421:
#line 2224 "ds_parser.ypp"
    { (yyval.type) = Type::tInt8; }
#line 7734 "ds_parser.cpp"
    break;

  case 422:
#line 2225 "ds_parser.ypp"
    { (yyval.type) = Type::tInt16; }
#line 7740 "ds_parser.cpp"
    break;

  case 423:
#line 2226 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt; }
#line 7746 "ds_parser.cpp"
    break;

  case 424:
#line 2227 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt8; }
#line 7752 "ds_parser.cpp"
    break;

  case 425:
#line 2228 "ds_parser.ypp"
    { (yyval.type) = Type::tUInt16; }
#line 7758 "ds_parser.cpp"
    break;

  case 426:
#line 2232 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = g_Program->makeTypeDeclaration(tokAt((yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 7771 "ds_parser.cpp"
    break;

  case 427:
#line 2243 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 7780 "ds_parser.cpp"
    break;

  case 428:
#line 2247 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[-1].s),tokAt((yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 7792 "ds_parser.cpp"
    break;

  case 429:
#line 2257 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 7804 "ds_parser.cpp"
    break;

  case 430:
#line 2264 "ds_parser.ypp"
    {
        das_checkName(*(yyvsp[0].s),tokAt((yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 7815 "ds_parser.cpp"
    break;

  case 431:
#line 2273 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 7821 "ds_parser.cpp"
    break;

  case 432:
#line 2273 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 7827 "ds_parser.cpp"
    break;

  case 433:
#line 2273 "ds_parser.ypp"
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
#line 7842 "ds_parser.cpp"
    break;

  case 434:
#line 2286 "ds_parser.ypp"
    { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt((yylsp[0])); }
#line 7848 "ds_parser.cpp"
    break;

  case 435:
#line 2287 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7854 "ds_parser.cpp"
    break;

  case 436:
#line 2288 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7860 "ds_parser.cpp"
    break;

  case 437:
#line 2289 "ds_parser.ypp"
    { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 7866 "ds_parser.cpp"
    break;

  case 438:
#line 2290 "ds_parser.ypp"
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
#line 7885 "ds_parser.cpp"
    break;

  case 439:
#line 2304 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7896 "ds_parser.cpp"
    break;

  case 440:
#line 2310 "ds_parser.ypp"
    {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 7905 "ds_parser.cpp"
    break;

  case 441:
#line 2314 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7914 "ds_parser.cpp"
    break;

  case 442:
#line 2318 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7924 "ds_parser.cpp"
    break;

  case 443:
#line 2323 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7934 "ds_parser.cpp"
    break;

  case 444:
#line 2328 "ds_parser.ypp"
    {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 7944 "ds_parser.cpp"
    break;

  case 445:
#line 2333 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7954 "ds_parser.cpp"
    break;

  case 446:
#line 2338 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 7963 "ds_parser.cpp"
    break;

  case 447:
#line 2342 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 7972 "ds_parser.cpp"
    break;

  case 448:
#line 2346 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7982 "ds_parser.cpp"
    break;

  case 449:
#line 2351 "ds_parser.ypp"
    {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 7991 "ds_parser.cpp"
    break;

  case 450:
#line 2355 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8001 "ds_parser.cpp"
    break;

  case 451:
#line 2360 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8007 "ds_parser.cpp"
    break;

  case 452:
#line 2360 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8013 "ds_parser.cpp"
    break;

  case 453:
#line 2360 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8024 "ds_parser.cpp"
    break;

  case 454:
#line 2366 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt((yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8036 "ds_parser.cpp"
    break;

  case 455:
#line 2373 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8042 "ds_parser.cpp"
    break;

  case 456:
#line 2373 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8048 "ds_parser.cpp"
    break;

  case 457:
#line 2373 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8058 "ds_parser.cpp"
    break;

  case 458:
#line 2378 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8064 "ds_parser.cpp"
    break;

  case 459:
#line 2378 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8070 "ds_parser.cpp"
    break;

  case 460:
#line 2378 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8081 "ds_parser.cpp"
    break;

  case 461:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8087 "ds_parser.cpp"
    break;

  case 462:
#line 2384 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8093 "ds_parser.cpp"
    break;

  case 463:
#line 2384 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8103 "ds_parser.cpp"
    break;

  case 464:
#line 2389 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8112 "ds_parser.cpp"
    break;

  case 465:
#line 2393 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8118 "ds_parser.cpp"
    break;

  case 466:
#line 2393 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8124 "ds_parser.cpp"
    break;

  case 467:
#line 2393 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8134 "ds_parser.cpp"
    break;

  case 468:
#line 2398 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8140 "ds_parser.cpp"
    break;

  case 469:
#line 2398 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8146 "ds_parser.cpp"
    break;

  case 470:
#line 2398 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8160 "ds_parser.cpp"
    break;

  case 471:
#line 2407 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8169 "ds_parser.cpp"
    break;

  case 472:
#line 2411 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8175 "ds_parser.cpp"
    break;

  case 473:
#line 2411 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8181 "ds_parser.cpp"
    break;

  case 474:
#line 2411 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8191 "ds_parser.cpp"
    break;

  case 475:
#line 2416 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8197 "ds_parser.cpp"
    break;

  case 476:
#line 2416 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8203 "ds_parser.cpp"
    break;

  case 477:
#line 2416 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8217 "ds_parser.cpp"
    break;

  case 478:
#line 2425 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[0]));
    }
#line 8226 "ds_parser.cpp"
    break;

  case 479:
#line 2429 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8232 "ds_parser.cpp"
    break;

  case 480:
#line 2429 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8238 "ds_parser.cpp"
    break;

  case 481:
#line 2429 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8248 "ds_parser.cpp"
    break;

  case 482:
#line 2434 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8254 "ds_parser.cpp"
    break;

  case 483:
#line 2434 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8260 "ds_parser.cpp"
    break;

  case 484:
#line 2434 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8274 "ds_parser.cpp"
    break;

  case 485:
#line 2443 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8280 "ds_parser.cpp"
    break;

  case 486:
#line 2443 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8286 "ds_parser.cpp"
    break;

  case 487:
#line 2443 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8297 "ds_parser.cpp"
    break;

  case 488:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth ++; }
#line 8303 "ds_parser.cpp"
    break;

  case 489:
#line 2449 "ds_parser.ypp"
    { das_arrow_depth --; }
#line 8309 "ds_parser.cpp"
    break;

  case 490:
#line 2449 "ds_parser.ypp"
    {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt((yylsp[-5]));
        varDeclToTypeDecl((yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 8320 "ds_parser.cpp"
    break;

  case 491:
#line 2458 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8326 "ds_parser.cpp"
    break;

  case 492:
#line 2458 "ds_parser.ypp"
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
#line 8343 "ds_parser.cpp"
    break;

  case 493:
#line 2473 "ds_parser.ypp"
    { das_need_oxford_comma=false; }
#line 8349 "ds_parser.cpp"
    break;

  case 494:
#line 2473 "ds_parser.ypp"
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
#line 8370 "ds_parser.cpp"
    break;

  case 495:
#line 2493 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8376 "ds_parser.cpp"
    break;

  case 496:
#line 2494 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8382 "ds_parser.cpp"
    break;

  case 497:
#line 2495 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8388 "ds_parser.cpp"
    break;

  case 498:
#line 2496 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8394 "ds_parser.cpp"
    break;

  case 499:
#line 2500 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8406 "ds_parser.cpp"
    break;

  case 500:
#line 2507 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 8418 "ds_parser.cpp"
    break;

  case 501:
#line 2514 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8429 "ds_parser.cpp"
    break;

  case 502:
#line 2520 "ds_parser.ypp"
    {
        auto mfd = make_smart<MakeFieldDecl>(tokAt((yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 8440 "ds_parser.cpp"
    break;

  case 503:
#line 2529 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 8450 "ds_parser.cpp"
    break;

  case 504:
#line 2534 "ds_parser.ypp"
    {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8459 "ds_parser.cpp"
    break;

  case 505:
#line 2541 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8465 "ds_parser.cpp"
    break;

  case 506:
#line 2542 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8471 "ds_parser.cpp"
    break;

  case 507:
#line 2546 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8482 "ds_parser.cpp"
    break;

  case 508:
#line 2552 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 8494 "ds_parser.cpp"
    break;

  case 509:
#line 2559 "ds_parser.ypp"
    {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt((yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 8507 "ds_parser.cpp"
    break;

  case 510:
#line 2567 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 8519 "ds_parser.cpp"
    break;

  case 511:
#line 2574 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-5]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8532 "ds_parser.cpp"
    break;

  case 512:
#line 2582 "ds_parser.ypp"
    {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt((yylsp[-7]));
        auto tam = g_Program->makeCall(tokAt((yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 8546 "ds_parser.cpp"
    break;

  case 513:
#line 2594 "ds_parser.ypp"
    {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 8554 "ds_parser.cpp"
    break;

  case 514:
#line 2597 "ds_parser.ypp"
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
#line 8570 "ds_parser.cpp"
    break;

  case 515:
#line 2611 "ds_parser.ypp"
    {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt((yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 8581 "ds_parser.cpp"
    break;

  case 516:
#line 2620 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8587 "ds_parser.cpp"
    break;

  case 517:
#line 2621 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8593 "ds_parser.cpp"
    break;

  case 518:
#line 2625 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8603 "ds_parser.cpp"
    break;

  case 519:
#line 2630 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8612 "ds_parser.cpp"
    break;

  case 520:
#line 2637 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8622 "ds_parser.cpp"
    break;

  case 521:
#line 2642 "ds_parser.ypp"
    {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt((yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
       auto tam = g_Program->makeCall(tokAt((yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 8635 "ds_parser.cpp"
    break;

  case 522:
#line 2653 "ds_parser.ypp"
    {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 8645 "ds_parser.cpp"
    break;

  case 523:
#line 2658 "ds_parser.ypp"
    {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 8654 "ds_parser.cpp"
    break;

  case 524:
#line 2665 "ds_parser.ypp"
    {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt((yylsp[-3]));
        auto ttm = g_Program->makeCall(tokAt((yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 8668 "ds_parser.cpp"
    break;

  case 525:
#line 2677 "ds_parser.ypp"
    { (yyval.pExpression) = nullptr; }
#line 8674 "ds_parser.cpp"
    break;

  case 526:
#line 2678 "ds_parser.ypp"
    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 8680 "ds_parser.cpp"
    break;

  case 527:
#line 2682 "ds_parser.ypp"
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
#line 8703 "ds_parser.cpp"
    break;

  case 528:
#line 2700 "ds_parser.ypp"
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
#line 8726 "ds_parser.cpp"
    break;


#line 8730 "ds_parser.cpp"

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
#line 2720 "ds_parser.ypp"


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
